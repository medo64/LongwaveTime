#pragma once
#include <stdbool.h>

enum RADIO_SIGNAL { HIGH, LOW, OFF };


bool radio_beat();        // call every 100 ms; returns true once per seconds
void radio_output();      // call to set radio for current beat interval
void radio_nextMinute();  // switch to the next minute (double-buffer))

//RADIO_SIGNAL radio_getSignal();
