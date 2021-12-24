#pragma once
#include <stdbool.h>
#include "Microchip/usb_device.h"


typedef enum RADIO_SIGNAL {
    SIGNAL_OFF  = 0b00,
    SIGNAL_LOW  = 0b10,
    SIGNAL_HIGH = 0b11,
} radio_signal;

typedef enum RADIO_PROTOCOL {
    PROTOCOL_OFF   = '0',
    PROTOCOL_WWVB  = 'W',
    PROTOCOL_DCF77 = 'D',
    PROTOCOL_MSF   = 'M',
    PROTOCOL_JJY40 = '4',
    PROTOCOL_JJY60 = '6',
} radio_protocol;


radio_protocol radio_getProtocol();               // get current protocol
void radio_setProtocol(radio_protocol protocol);  // set protocol

uint16_t radio_PostPostScale = 0;                 // scaling value to get 100 ms

bool radio_beat();                                // call every 100 ms; returns true once per seconds
void radio_output();                              // call to set radio for current beat interval
void radio_nextMinute();                          // switch to the next minute (double-buffer))

//RADIO_SIGNAL radio_getSignal();
