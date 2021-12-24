#pragma once
#include <xc.h>

void io_init();
void io_splash();

#define io_led_activity_off()     LATC2 = 1
#define io_led_activity_on()      LATC2 = 0
#define io_led_activity_toggle()  LATC2 = !LATC2

#define io_led_pps_off()          LATA5 = 1
#define io_led_pps_on()           LATA5 = 0
#define io_led_pps_toggle()       LATA5 = !LATA5

#define io_tick_toggle()          LATC5 = !LATC5

void io_clock_setup60khz();
void io_clock_setup75khz();
void io_clock_on();
void io_clock_off();

#define io_attenuate_on()         TRISC4 = 0
#define io_attenuate_off()        TRISC4 = 1
