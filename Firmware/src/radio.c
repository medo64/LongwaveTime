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


uint8_t minuteDescriptionIndex = 0;  // which buffer to use
uint8_t minuteDescription[2][61] = {  // describes each minute in 4 bits per second: 0b0000: 0; 0b0001: 1; 0b0010: 2; 0b0011: 3, 0b0100: M,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 00-09
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 10-19
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 20-29
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 30-39
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 40-49
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 50-59
    0xFF,                          // 60
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 00-09
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 10-19
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 20-29
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 30-39
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 40-49
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 50-59
    0xFF,                          // 60
};

uint8_t fragmentMinute = 0;  // where are we in the current minute (0-59)
uint8_t fragmentSecond = 0;  // where are we in the current second (0-9)

bool radio_beat() {
    bool isSecondStart = (fragmentSecond == 0);
    if (fragmentSecond >= 9) { fragmentSecond = 0; } else { fragmentSecond += 1; }

    if (isSecondStart) {
        //TODO prep for next second
    }

    return isSecondStart;
}

void radio_output() {
}

void radio_nextMinute() {
}
