#include <stdbool.h>
#include <stdint.h>
#include "io.h"
#include "radio.h"


radio_protocol_t currentProtocol = '0';

radio_protocol_t radio_getProtocol(void) {
    return currentProtocol;
}

void radio_setProtocol(radio_protocol_t protocol) {
    io_clock_off();

    currentProtocol = protocol;
    switch (protocol) {
        case PROTOCOL_OFF:   radio_PostPostScale = 0; break;
        case PROTOCOL_WWVB:  radio_PostPostScale = io_clock_setup60khz(); io_clock_on(); break;
        case PROTOCOL_DCF77: radio_PostPostScale = io_clock_setup77khz(); io_clock_on(); break;
        case PROTOCOL_MSF:   radio_PostPostScale = io_clock_setup40khz(); io_clock_on(); break;
        case PROTOCOL_JJY40: radio_PostPostScale = io_clock_setup40khz(); io_clock_on(); break;
        case PROTOCOL_JJY60: radio_PostPostScale = io_clock_setup60khz(); io_clock_on(); break;
    }
}


void radio_setTime(uint8_t second, uint8_t tenths) {
    uint8_t currBuffer = radio_BufferIndex;
    uint8_t nextBuffer = (currBuffer + 1) & 0x01;
    if (radio_Buffer[nextBuffer][0] != 0xFF) {  // switch only if buffer is present
        radio_BufferIndex = nextBuffer;         // do the switch
        radio_Buffer[currBuffer][0] = 0xFF;     // disable the current buffer
    }

    radio_CurrentTenth = tenths;
    radio_CurrentSecond = second;
}


bool radio_setBuffer(const uint8_t* source, const uint8_t count, uint8_t* outUsedBuffer) {
    uint8_t currBuffer = radio_BufferIndex;
    uint8_t nextBuffer = (currBuffer + 1) & 0x01;
    *outUsedBuffer = nextBuffer;

    radio_Buffer[nextBuffer][0] = 0;  // TODO; just for test

    if (count <= 59) { radio_Buffer[nextBuffer][59] = 0xFF; }
    if (count <= 60) { radio_Buffer[nextBuffer][60] = 0xFF; }

    return true;
}


bool radio_beat(void) {
    if (radio_CurrentTenth < 9) {
        radio_CurrentTenth += 1;
    } else {
        radio_CurrentTenth = 0;
        if (radio_CurrentSecond != 0xFF) {
            radio_CurrentSecond += 1;

            uint8_t currBuffer = radio_BufferIndex;
            uint8_t lastSecond = radio_Buffer[currBuffer][60] == 0xFF ? 59 : 60;
            if (radio_CurrentSecond > lastSecond) {
                radio_setTime(0, 0);  // switch to next minute
            }
        }
    }

    return (radio_CurrentTenth == 0);  // starting a new second
}

void radio_output(void) {
}
