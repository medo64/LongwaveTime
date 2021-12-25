#include <xc.h>
#include <stdint.h>
#include "io.h"
#include "system.h"


void io_init(void) {
    // Activity
    LATC2 = 1;   // default to off (inverse)
    TRISC2 = 0;  // enable output
    
    // PPS
    LATA5 = 1;   // default to off (inverse)
    TRISA5 = 0;  // enable output
    
    // Tick
    LATC5 = 0;   // default to off
    TRISC5 = 0;  // enable output

    // Clock
    LATC3 = 0;   // default to off
    TRISC3 = 1;  // keep tri-state (sorted in io_clock_setup)

    // Attenuation
    LATC4 = 0;   // it'll be always off
    TRISC4 = 1;  // keep tri-state
}

void io_splash(void) {
    io_led_activity_on(); wait_short();
    io_led_pps_on(); io_led_activity_off(); wait_short();
    io_led_activity_on(); io_led_pps_off(); wait_short();
    io_led_pps_on(); io_led_activity_off(); wait_short();
    io_led_pps_off(); io_led_activity_on();
}


#define LED_ACTIVITY_TIMEOUT       2000
#define LED_ACTIVITY_TIMEOUT_NONE  65535
uint16_t LedAcivityTimeout = LED_ACTIVITY_TIMEOUT_NONE;

void io_led_activity_tick(void) {
    if (LedAcivityTimeout != LED_ACTIVITY_TIMEOUT_NONE) {
        if (LedAcivityTimeout == 0) {
            io_led_activity_on();
            LedAcivityTimeout = LED_ACTIVITY_TIMEOUT_NONE;
        } else {
            LedAcivityTimeout--;
        }
    }
}

void io_led_activity_blink(void) {
    io_led_activity_off();
    LedAcivityTimeout = LED_ACTIVITY_TIMEOUT;
}


uint16_t io_clock_setup40khz(void) {
    PWM2CONbits.PWM2OE = 0;               // 1: Disable the PWM0 pin output driver
    PWM2CON = 0;                          // 2: Clear the PWMxCON register
    PR2 = 0b01001010;                     // 3: Load the PR2 register with the PWM period value
    PWM2DCH = 0; PWM2DCL = 0;             // 4: Clear the PWMxDCH register and bits <7:6> of the PWMxDCL register
    T2CON = 0b01111101;                   // 5: Configure and start Timer2 (1:4 prescaler, 1:16 postscaler)
    PWM2CONbits.PWM2EN = 1;               // 6: Enable PWM output pin
    PWM2CONbits.PWM2OE = 1;               // 7: Enable the PWMx pin output driver
    PWM2DCH = 37; PWM2DCL = 0b10000000;   // 4∗(PR2+1)∗DutyCycleRatio - 150
    return 250;                           // 100/((1/40000)*16*1000)
}

uint16_t io_clock_setup60khz(void) {
    PWM2CONbits.PWM2OE = 0;               // 1: Disable the PWM0 pin output driver
    PWM2CON = 0;                          // 2: Clear the PWMxCON register
    PR2 = 0b11000111;                     // 3: Load the PR2 register with the PWM period value
    PWM2DCH = 0; PWM2DCL = 0;             // 4: Clear the PWMxDCH register and bits <7:6> of the PWMxDCL register
    T2CON = 0b01111100;                   // 5: Configure and start Timer2 (1:1 prescaler, 1:16 postscaler)
    PWM2CONbits.PWM2EN = 1;               // 6: Enable PWM output pin
    PWM2CONbits.PWM2OE = 1;               // 7: Enable the PWMx pin output driver
    PWM2DCH = 100; PWM2DCL = 0b00000000;  // 4∗(PR2+1)∗DutyCycleRatio - 400
    return 375;                           // 100/((1/60000)*16*1000)
}

uint16_t io_clock_setup77khz(void) {
    PWM2CONbits.PWM2OE = 0;               // 1: Disable the PWM0 pin output driver
    PWM2CON = 0;                          // 2: Clear the PWMxCON register
    PR2 = 0b10011010;                     // 3: Load the PR2 register with the PWM period value
    PWM2DCH = 0; PWM2DCL = 0;             // 4: Clear the PWMxDCH register and bits <7:6> of the PWMxDCL register
    T2CON = 0b01101100;                   // 5: Configure and start Timer2 (1:1 prescaler, 1:14 postscaler)
    PWM2CONbits.PWM2EN = 1;               // 6: Enable PWM output pin
    PWM2CONbits.PWM2OE = 1;               // 7: Enable the PWMx pin output driver
    PWM2DCH = 77; PWM2DCL = 0b10000000;   // 4∗(PR2+1)∗DutyCycleRatio - 310
    return 553;                           // 100/((1/77419.35)*14*1000)
}

void io_clock_on(void) {
    INTCONbits.PEIE = 1;   // enable peripheral interrupts
    INTCONbits.GIE = 1;    // enable global interrupts
    PIR1bits.TMR2IF = 0;   // reset interrupt flag
    PIE1bits.TMR2IE = 1;   // enable interrupt
    TRISCbits.TRISC3 = 0;  // enable output
}

void io_clock_off(void) {
    TRISCbits.TRISC3 = 1;  // disable output
}
