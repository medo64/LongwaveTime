#pragma once
#include <xc.h>
#include "Microchip/usb_device.h"


void io_init(void);
void io_splash(void);

#define io_led_activity_off()     LATC2 = 1
#define io_led_activity_on()      LATC2 = 0
#define io_led_activity_toggle()  LATC2 = !LATC2

void io_led_activity_tick(void);
void io_led_activity_blink(void);

#define io_led_pps_off()          LATA5 = 1
#define io_led_pps_on()           LATA5 = 0
#define io_led_pps_toggle()       LATA5 = !LATA5

#define io_tick_toggle()          LATC5 = !LATC5
#define io_tick_off()             LATC5 = 0

uint16_t io_clock_setup40khz(void);
uint16_t io_clock_setup60khz(void);
uint16_t io_clock_setup77khz(void);
void io_clock_on(void);
void io_clock_off(void);
#define io_clock_enable()         TRISC3 = 0
#define io_clock_disable()        TRISC3 = 1
#define io_clock_isEnabled()      TRISC3 == 0

#define io_attenuate_enable()     TRISC4 = 0
#define io_attenuate_disable()    TRISC4 = 1
#define io_attenuate_isEnabled()  TRISC4 == 0
