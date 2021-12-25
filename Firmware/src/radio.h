#pragma once
#include <stdbool.h>
#include "Microchip/usb_device.h"


typedef enum RADIO_SIGNAL {
    SIGNAL_OFF  = 0b00,
    SIGNAL_NA   = 0b01,
    SIGNAL_LOW  = 0b10,
    SIGNAL_HIGH = 0b11,
} radio_signal_t;

typedef enum RADIO_PROTOCOL {
    PROTOCOL_OFF   = '0',
    PROTOCOL_WWVB  = 'W',
    PROTOCOL_DCF77 = 'D',
    PROTOCOL_MSF   = 'M',
    PROTOCOL_JJY40 = '4',
    PROTOCOL_JJY60 = '6',
} radio_protocol_t;


radio_protocol_t radio_getProtocol(void);           // get current protocol
void radio_setProtocol(radio_protocol_t protocol);  // set protocol

uint8_t radio_CurrentSecond = 0xFF;                  // keeping time in tens, ones, and tenths
uint8_t radio_CurrentTenth = 0xFF;
void radio_setTime(uint8_t second, uint8_t tenths);  // sets time and switches buffer if ready

uint8_t radio_BufferIndex = 1;     // buffer currently in use; start with 1 so that the next one is 0
uint8_t radio_Buffer[2][61] = {    // describes each minute in 4 bits per second: 0b0000: 0; 0b0001: 1; 0b0010: 2; 0b0011: 3, 0b0100: M,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 00-04
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 05-09
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 10-14
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 15-19
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 20-24
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 25-29
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 30-34
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 35-39
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 40-44
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 45-49
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 50-54
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 55-59
    0xFF,                          // 60
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 00-04
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 05-09
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 10-14
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 15-19
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 20-24
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 25-29
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 30-34
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 35-39
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 40-44
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 45-49
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 50-54
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 55-59
    0xFF,                          // 60
};
bool radio_setBuffer(const uint8_t* source, const uint8_t count, uint8_t* outUsedBuffer);  // return which buffer was set


uint16_t radio_PostPostScale = 0;                 // scaling value to get 100 ms
bool radio_beat(void);                            // call every 100 ms; returns true once per seconds


bool radio_output(uint8_t second, uint8_t tenth);  // call to set radio for current beat interval
