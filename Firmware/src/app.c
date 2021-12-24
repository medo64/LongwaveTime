#include <stdint.h>
#include "Microchip/usb.h"
#include "Microchip/usb_device.h"
#include "Microchip/usb_device_cdc.h"
#include "io.h"
#include "radio.h"
#include "settings.h"
#include "system.h"
#include "usb.h"


void main(void) {
    init();
    io_init();
    io_splash();
    
    settings_init();

    USBDeviceInit();
    USBDeviceAttach();
    
    io_led_activity_on();

    io_clock_setup60khz(); io_clock_on();  //TEST

    while(true) {
        io_led_activity_tick();

        USBDeviceTasks();

        if (USBGetDeviceState() < CONFIGURED_STATE) { continue; }
        if (USBIsDeviceSuspended()) { continue; }

        CDCTxService();

        // USB receive
        uint8_t readCount = getsUSBUSART(UsbReadBuffer, USB_READ_BUFFER_MAX);  // until the buffer is free
        if (readCount > 0) {
            io_led_activity_blink();
            for (uint8_t i = 0; i < readCount; i++) {  // copy to buffer
                uint8_t value = UsbReadBuffer[i];
                if (UsbInputBufferCorrupted && ((value == 0x0A) || (value == 0x0D))) {
                    UsbInputBufferCount = 0;  // clear the whole buffer
                    UsbInputBufferCorrupted = false;
                } else if (UsbInputBufferCount < USB_INPUT_BUFFER_MAX) {
                    UsbInputBuffer[UsbInputBufferCount] = value;
                    UsbInputBufferCount++;
                } else {
                    UsbInputBufferCorrupted = true;  // no more buffer; darn it
                }
            }
        }

        // TODO: Process; just echo for now
        if (UsbInputBufferCount > 0) {
            for (uint8_t i = 0; i < UsbInputBufferCount; i++) {  // find EOLs
                UsbOutputBufferAppend(UsbInputBuffer[i]);
            }
            UsbInputBufferCount = 0;
        }

        // USB send
        if ((UsbOutputBufferCount > 0) && USBUSARTIsTxTrfReady()) {  // send output if TX is ready
            io_led_activity_blink();
            uint8_t writeCount = 0;
            for (uint8_t i = 0; i < USB_WRITE_BUFFER_MAX; i++) {  // copy to output buffer
                if (i < UsbOutputBufferCount) {
                    UsbWriteBuffer[i] = UsbOutputBuffer[i];
                    writeCount++;
                } else {
                    break;
                }
            }
            if (writeCount > 0) {
                putUSBUSART(&UsbWriteBuffer[0], writeCount);  // send data
                UsbOutputBufferCount -= writeCount;  // reduce buffer for the length used
                usbBufferCopy(&UsbOutputBuffer[0], &UsbOutputBuffer[writeCount], UsbOutputBufferCount);  // move buffer content to beginning
            }
        }
    }
}


uint16_t timerCounter100ms = 0;

void __interrupt() isr(void) {
    if (PIR1bits.TMR2IF) {  // ticks every 0.2 ms - i.e. 500 needed for 100 ms
        PIR1bits.TMR2IF = 0;  // reset flag

        if (timerCounter100ms == 0) {  // 100 ms
            io_tick_toggle();  // toggle tick
            if (radio_beat()) { io_led_pps_on(); } else { io_led_pps_off(); }  // do PPS and 
            radio_output();
        }
        
        // increase 100ms counter
        if (timerCounter100ms >= 499) { timerCounter100ms = 0; } else { timerCounter100ms += 1; }
    }
}
