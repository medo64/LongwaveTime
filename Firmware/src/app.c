#include <stdint.h>
#include "Microchip/usb.h"
#include "Microchip/usb_device.h"
#include "Microchip/usb_device_cdc.h"
#include "app.h"
#include "io.h"
#include "radio.h"
#include "settings.h"
#include "system.h"
#include "usb.h"


const uint8_t APP_VERSION[4] = { '2', '1', '1', '2' };

bool processInput(const uint8_t* dataIn, const uint8_t count);


void main(void) {
    init();
    io_init();
    io_splash();
    
    settings_init();

    USBDeviceInit();
    USBDeviceAttach();
    
    io_led_activity_on();

    while(true) {
        io_led_activity_tick();

        USBDeviceTasks();

        if (USBGetDeviceState() < CONFIGURED_STATE) { continue; }
        if (USBIsDeviceSuspended()) { continue; }

        CDCTxService();

        // USB receive
        uint8_t readCount = getsUSBUSART(usb_ReadBuffer, USB_READ_BUFFER_MAX);  // until the buffer is free
        if (readCount > 0) {
            io_led_activity_blink();
            for (uint8_t i = 0; i < readCount; i++) {  // copy to buffer
                uint8_t value = usb_ReadBuffer[i];
                if (usb_InputBufferCorrupted && ((value == 0x0A) || (value == 0x0D))) {
                    usb_InputBufferCount = 0;  // clear the whole buffer
                    usb_InputBufferCorrupted = false;
                } else if (usb_InputBufferCount < USB_INPUT_BUFFER_MAX) {
                    usb_InputBuffer[usb_InputBufferCount] = value;
                    usb_InputBufferCount++;
                } else {
                    usb_InputBufferCorrupted = true;  // no more buffer; darn it
                }
            }
        }

        // Process
        if (usb_InputBufferCount > 0) {
            uint8_t offset = 0;
            for (uint8_t i = 0; i < usb_InputBufferCount; i++) {  // find EOLs
                uint8_t eolChar = usb_InputBuffer[i];  // this'll be EOL eventually

                if ((eolChar == 0x0A) || (eolChar == 0x0D)) {  // start line processing on either CR or LF
                    uint8_t len = i - offset;
                    if (len > 0) {
                        usb_outputBufferAppend(usb_InputBuffer[offset]);  // response always starts with the same letter
                        if (!processInput(&usb_InputBuffer[offset], len)) {  // if there's any error, return exclamation point and bell
                            usb_outputBufferAppend('!');
                            usb_outputBufferAppend(0x07);
                        }
                        usb_outputBufferAppend(0x0D);
                        usb_outputBufferAppend(0x0A);
                    }

                    offset = i + 1;  // set the next start
                }
            }

            if (offset > 0) {  // move unused portion of buffer to the start
                usb_InputBufferCount -= offset;
                usb_bufferCopy(&usb_InputBuffer[0], &usb_InputBuffer[offset], usb_InputBufferCount);
            }
        }

        // USB send
        if ((usb_OutputBufferCount > 0) && USBUSARTIsTxTrfReady()) {  // send output if TX is ready
            io_led_activity_blink();
            uint8_t writeCount = 0;
            for (uint8_t i = 0; i < USB_WRITE_BUFFER_MAX; i++) {  // copy to output buffer
                if (i < usb_OutputBufferCount) {
                    usb_WriteBuffer[i] = usb_OutputBuffer[i];
                    writeCount++;
                } else {
                    break;
                }
            }
            if (writeCount > 0) {
                putUSBUSART(&usb_WriteBuffer[0], writeCount);  // send data
                usb_OutputBufferCount -= writeCount;  // reduce buffer for the length used
                usb_bufferCopy(&usb_OutputBuffer[0], &usb_OutputBuffer[writeCount], usb_OutputBufferCount);  // move buffer content to beginning
            }
        }
    }
}


uint16_t timerCounter100ms = 0;

void __interrupt() isr(void) {
    if (PIR1bits.TMR2IF) {  // ticks every 0.2 ms - i.e. 500 needed for 100 ms
        PIR1bits.TMR2IF = 0;  // reset flag
        if (radio_PostPostScale == 0) {  // if post-post-scaler is 0, no protocol is defined
            io_tick_off();
            io_led_pps_off();
            return;
        }

        if (timerCounter100ms == 0) {  // 100 ms
            io_tick_toggle();  // toggle tick
            if (radio_beat()) {  // do PPS
                if (TestMode) { io_led_pps_off(); } else { io_led_pps_on(); }
                if (TestModeOutput) { usb_outputBufferAppend(' '); }
            } else {
                if (TestMode) { io_led_pps_on(); } else { io_led_pps_off(); }
            }

            // set output
            if (!TestMode || TestModeOutput) {
                radio_output(radio_CurrentSecond, radio_CurrentTenth);
            }
        }
        
        // increase 100ms counter
        timerCounter100ms += 1;
        if (timerCounter100ms >= radio_PostPostScale) { timerCounter100ms = 0; }
    }
}


bool processInput(const uint8_t* dataIn, const uint8_t count) {
    if (count == 0) {  // if line is empty just return empty
        return true;
    }

    uint8_t* data = (uint8_t*)dataIn;

    switch (data[0]) {
        case 'P': {  // Protocol
            if (count > 2) { return false; }
            if (count == 2) {  // set protocol
                switch (data[1]) {
                    case '0': radio_setProtocol(PROTOCOL_OFF);   break;
                    case 'W': radio_setProtocol(PROTOCOL_WWVB);  break;
                    case 'D': radio_setProtocol(PROTOCOL_DCF77); break;
                    case 'M': radio_setProtocol(PROTOCOL_MSF);   break;
                    case '4': radio_setProtocol(PROTOCOL_JJY40); break;
                    case '6': radio_setProtocol(PROTOCOL_JJY60); break;
                    default: return false;
                }
            }
            usb_outputBufferAppend(radio_getProtocol());
            return true;
        }

        case 'R': {  // Raw data
            if ((count != 1) && (count != 60) && (count != 61) && (count != 62)) { return false; }
            uint8_t currBuffer = radio_BufferIndex;
            uint8_t nextBuffer = (currBuffer + 1) & 0x01;
            if (count > 1) {  // set buffer
                if (!radio_setBuffer(nextBuffer, &data[1], count - 1)) { return false; }
                if (currBuffer == 0xFF) {  // if buffer is invalid, apply immediately
                    radio_BufferIndex = nextBuffer;
                    currBuffer = nextBuffer;
                    nextBuffer = (currBuffer + 1) & 0x01;
                }
            }
            bool isCurrBufferOk = (currBuffer != 0xFF) && (radio_Buffer[currBuffer][0] != 0xFF);
            bool isNextBufferOk = (radio_Buffer[nextBuffer][0] != 0xFF);
            if (isCurrBufferOk) {
                switch (currBuffer) {
                    case 0: usb_outputBufferAppend('X'); break;
                    case 1: usb_outputBufferAppend('Y'); break;
                    default: usb_outputBufferAppend('='); break;  // should never actually happen
                }
            } else {
                usb_outputBufferAppend('~');
            }
            if (isNextBufferOk) {
                switch (nextBuffer) {
                    case 0: usb_outputBufferAppend('X'); break;
                    case 1: usb_outputBufferAppend('Y'); break;
                    default: usb_outputBufferAppend('='); break;  // should never actually happen
                }
            } else {
                usb_outputBufferAppend('~');
            }
            return true;
        }

        case 'S': {
            if ((count != 1) && (count != 3) && (count != 4)) { return false; }
            if (count >= 3) {
                if ((data[1] < '0') || (data[1] > '9')) { return false; }
                if ((data[2] < '0') || (data[2] > '9')) { return false; }
                if ((count == 4) && ((data[3] < '0') || (data[3] > '9'))) { return false; }
                uint8_t parsedSecond = (data[1] - 0x30) * 10 + (data[2] - 0x30);
                if (parsedSecond > 60) { return false; }
                uint8_t parsedTenth = (count == 4) ? (data[3] - 0x30) : 0;
                radio_setTime(parsedSecond, parsedTenth);
            }
            if (radio_CurrentSecond == 0xFF) {  // just check second for invalid data
                usb_outputBufferAppend('-');
                usb_outputBufferAppend('-');
                usb_outputBufferAppend('-');
            } else {
                usb_outputBufferAppend(0x30 + radio_CurrentSecond / 10);
                usb_outputBufferAppend(0x30 + radio_CurrentSecond % 10);
                usb_outputBufferAppend(0x30 + radio_CurrentTenth);
            }
            return true;
        }

        case 'T': {  // Test - T++ to enter
            if (!TestMode) {
                if ((count == 3) && (data[1] == '*') && (data[2] == '*')) {
                    TestMode = true;
                    return true;
                }
                return false;
            }

            if (count > 2) { return false; }
            if (count == 2) {
                switch (data[1]) {
                    case '0': radio_setProtocol(PROTOCOL_OFF);   break;
                    case 'W': radio_setProtocol(PROTOCOL_WWVB);  break;
                    case 'D': radio_setProtocol(PROTOCOL_DCF77); break;
                    case 'M': radio_setProtocol(PROTOCOL_MSF);   break;
                    case '4': radio_setProtocol(PROTOCOL_JJY40); break;
                    case '6': radio_setProtocol(PROTOCOL_JJY60); break;

                    case 'C': io_clock_enable();  break;
                    case 'c': io_clock_disable(); break;

                    case 'A': io_attenuate_enable();  break;
                    case 'a': io_attenuate_disable(); break;

                    case 'O': TestModeOutput = true;  break;

                    case '-': reset(); break;

                    default: return false;
                }
            }
            usb_outputBufferAppend(radio_getProtocol());
            usb_outputBufferAppend(io_clock_isEnabled() ? 'C' : 'c' );
            usb_outputBufferAppend(io_attenuate_isEnabled() ? 'A' : 'a' );
            return true;
        }

        case 'V': {  // Version
            if (count != 1) { return false; }
            for (uint8_t i = 0; i < sizeof(APP_VERSION); i++) {
                usb_outputBufferAppend(APP_VERSION[i]);
            }
            return true;
        }

        default: return false;
    }
}
