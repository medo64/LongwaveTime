#include <stdbool.h>
#include <stdint.h>
#include "io.h"
#include "radio.h"


const radio_signal_t RADIO_SIGNAL_DEFINITION_WWVB[5][10] = {  // https://en.wikipedia.org/wiki/WWVB
    SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
    SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_HIGH, SIGNAL_HIGH,
};

const radio_signal_t RADIO_SIGNAL_DEFINITION_DCF77[5][10] = {  // https://en.wikipedia.org/wiki/DCF77
    SIGNAL_LOW,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
    SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
};

const radio_signal_t RADIO_SIGNAL_DEFINITION_MSF[5][10] = {  // https://en.wikipedia.org/wiki/Time_from_NPL_(MSF)
    SIGNAL_OFF,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
    SIGNAL_OFF,  SIGNAL_HIGH, SIGNAL_OFF,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
    SIGNAL_OFF,  SIGNAL_OFF,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
    SIGNAL_OFF,  SIGNAL_OFF,  SIGNAL_OFF,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
    SIGNAL_OFF,  SIGNAL_OFF,  SIGNAL_OFF,  SIGNAL_OFF,  SIGNAL_OFF,  SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH,
};

const radio_signal_t RADIO_SIGNAL_DEFINITION_JJY[5][10] = {  // https://en.wikipedia.org/wiki/JJY
    SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_LOW,  SIGNAL_LOW,
    SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_HIGH, SIGNAL_HIGH, SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,  SIGNAL_LOW,
};

radio_signal_t radio_SignalDefinition[5][10] = {
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
    SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,   SIGNAL_NA,
};

radio_protocol_t currentProtocol = '0';

radio_protocol_t radio_getProtocol(void) {
    return currentProtocol;
}

void radio_setProtocol(radio_protocol_t protocol) {
    io_clock_off();

    currentProtocol = protocol;
    switch (protocol) {
        case PROTOCOL_OFF:
            for (unsigned i = 0; i < 5; i++) {
                for (unsigned j = 0; j < 10; j++) {
                    radio_SignalDefinition[i][j] = SIGNAL_NA;
                }
            }
            radio_PostPostScale = 0;
            break;

        case PROTOCOL_WWVB:
            for (unsigned i = 0; i < 5; i++) {
                for (unsigned j = 0; j < 10; j++) {
                    radio_SignalDefinition[i][j] = RADIO_SIGNAL_DEFINITION_WWVB[i][j];
                }
            }
            radio_PostPostScale = io_clock_setup60khz();
            io_clock_on();
            break;

        case PROTOCOL_DCF77:
            for (unsigned i = 0; i < 5; i++) {
                for (unsigned j = 0; j < 10; j++) {
                    radio_SignalDefinition[i][j] = RADIO_SIGNAL_DEFINITION_DCF77[i][j];
                }
            }
            radio_PostPostScale = io_clock_setup77khz();
            io_clock_on();
            break;

        case PROTOCOL_MSF:
            for (unsigned i = 0; i < 5; i++) {
                for (unsigned j = 0; j < 10; j++) {
                    radio_SignalDefinition[i][j] = RADIO_SIGNAL_DEFINITION_MSF[i][j];
                }
            }
            radio_PostPostScale = io_clock_setup40khz();
            io_clock_on();
            break;

        case PROTOCOL_JJY40:
            for (unsigned i = 0; i < 5; i++) {
                for (unsigned j = 0; j < 10; j++) {
                    radio_SignalDefinition[i][j] = RADIO_SIGNAL_DEFINITION_JJY[i][j];
                }
            }
            radio_PostPostScale = io_clock_setup40khz();
            io_clock_on();
            break;

        case PROTOCOL_JJY60:
            for (unsigned i = 0; i < 5; i++) {
                for (unsigned j = 0; j < 10; j++) {
                    radio_SignalDefinition[i][j] = RADIO_SIGNAL_DEFINITION_JJY[i][j];
                }
            }
            radio_PostPostScale = io_clock_setup60khz();
            io_clock_on();
            break;
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


bool radio_setBuffer(const uint8_t index, const uint8_t* source, const uint8_t count) {
    bool isOk = (count >= 59);
    for (unsigned i = 0; i < count; i++) {
        switch (*source) {
            case 0x30: radio_Buffer[index][i] = 0; break;
            case 0x31: radio_Buffer[index][i] = 1; break;
            case 0x32: radio_Buffer[index][i] = 2; break;
            case 0x33: radio_Buffer[index][i] = 3; break;
            case 'M':  radio_Buffer[index][i] = 4; break;
            default:
                radio_Buffer[index][i] = 0xFF;
                isOk = false;
                break;
        }
        source++;
    }

    if (count <= 60) { radio_Buffer[index][59] = 0xFF; }
    if (count <= 61) { radio_Buffer[index][60] = 0xFF; }

    return isOk;
}


bool radio_beat(void) {
    if (radio_CurrentTenth < 9) {
        radio_CurrentTenth += 1;
    } else {
        radio_CurrentTenth = 0;
        if (radio_CurrentSecond != 0xFF) {
            radio_CurrentSecond += 1;

            uint8_t currBuffer = radio_BufferIndex;
            uint8_t lastSecond = (radio_Buffer[currBuffer][60] == 0xFF) ? 59 : 60;
            if (radio_CurrentSecond > lastSecond) {
                radio_setTime(0, 0);  // switch to next minute
            }
        }
    }

    return (radio_CurrentTenth == 0);  // starting a new second
}


bool radio_output(uint8_t second, uint8_t tenth) {
    uint8_t currBuffer = radio_BufferIndex;
    if ((currBuffer == 0xFF) || (radio_Buffer[currBuffer][0] != 0xFF)) { 
        io_clock_disable();
        io_attenuate_disable();
        return false; 
    }

    uint8_t definitionIndex = radio_Buffer[currBuffer][second];
    radio_signal_t signal = radio_SignalDefinition[definitionIndex][tenth];
    switch (signal) {
        case SIGNAL_LOW:
            io_clock_enable();
            io_attenuate_enable();
            return true;
        case SIGNAL_HIGH:
            io_clock_enable();
            io_attenuate_disable();
            return true;
        default:
            io_clock_disable();
            io_attenuate_disable();
            return (signal == SIGNAL_OFF);  // false otherwise
    }
}
