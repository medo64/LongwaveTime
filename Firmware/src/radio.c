#include <stdbool.h>
#include <stdint.h>
#include "io.h"
#include "radio.h"


radio_protocol currentProtocol = '0';

radio_protocol radio_getProtocol() {
    return currentProtocol;
}

void radio_setProtocol(radio_protocol protocol) {
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
    // TODO; switch buffer if possible

    radio_CurrentTenth = tenths;
    radio_CurrentSecond = second;
}


bool radio_beat() {
    if (radio_CurrentTenth < 9) {
        radio_CurrentTenth += 1;
    } else {
        radio_CurrentTenth = 0;
        if (radio_CurrentSecond != 0xFF) {
            radio_CurrentSecond += 1;

            uint8_t lastSecond = radio_Buffer[radio_BufferIndex][60] == 0xFF ? 59 : 60;
            if (radio_CurrentSecond > lastSecond) { radio_CurrentSecond = 0; }  // TODO check against buffer length
        }
    }

    return (radio_CurrentTenth == 0);  // starting a new second
}

void radio_output() {
}

void radio_nextMinute() {
}
