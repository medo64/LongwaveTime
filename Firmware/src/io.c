#include <xc.h>
#include <stdint.h>
#include "io.h"
#include "system.h"

void io_init() {
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

void io_splash() {
    io_led_activity_on(); wait_short();
    io_led_pps_on(); io_led_activity_off(); wait_short();
    io_led_activity_on(); io_led_pps_off(); wait_short();
    io_led_pps_on(); io_led_activity_off(); wait_short();
    io_led_pps_off(); io_led_activity_on();
}


#define LED_ACTIVITY_TIMEOUT       2000
#define LED_ACTIVITY_TIMEOUT_NONE  65535
uint16_t LedAcivityTimeout = LED_ACTIVITY_TIMEOUT_NONE;

void io_led_activity_tick() {
    if (LedAcivityTimeout != LED_ACTIVITY_TIMEOUT_NONE) {
        if (LedAcivityTimeout == 0) {
            io_led_activity_on();
            LedAcivityTimeout = LED_ACTIVITY_TIMEOUT_NONE;
        } else {
            LedAcivityTimeout--;
        }
    }
}

void io_led_activity_blink() {
    io_led_activity_off();
    LedAcivityTimeout = LED_ACTIVITY_TIMEOUT;
}


void io_clock_setup60khz() {
    PWM2CONbits.PWM2OE = 0; TRISCbits.TRISC3 = 1;      // 1: Disable the PWM0 pin output driver
    PWM2CON = 0;                                       // 2: Clear the PWMxCON register
    PR2 = 0b00110001;                                  // 3: Load the PR2 register with the PWM period value
    PWM2DCH = 0; PWM2DCL = 0;                          // 4: Clear the PWMxDCH register and bits <7:6> of the PWMxDCL register
    T2CON = 0b01011101;                                // 5: Configure and start Timer2 (1:12 postscaler)
    PWM2CONbits.PWM2EN = 1; while (!PIR1bits.TMR2IF);  // 6: Enable PWM output pin and wait until Timer2 overflows; TMR2IF bit of the PIR1 register is set
    PWM2CONbits.PWM2OE = 1; //(we'll clear tris later) // 7: Enable the PWMx pin output driver(s) by clearing the associated TRIS bit(s) and setting the PWMxOE bit of the PWMxCON register
    PWM2DCH = 0x19;
}

void io_clock_setup75khz() {
    PWM2CONbits.PWM2OE = 0; TRISCbits.TRISC3 = 1;      // 1: Disable the PWM0 pin output driver
    PWM2CON = 0;                                       // 2: Clear the PWMxCON register
    PR2 = 0b00100111;                                  // 3: Load the PR2 register with the PWM period value
    PWM2DCH = 0; PWM2DCL = 0;                          // 4: Clear the PWMxDCH register and bits <7:6> of the PWMxDCL register
    T2CON = 0b01110101;                                // 5: Configure and start Timer2 (1:15 postscaler)
    PWM2CONbits.PWM2EN = 1; while (!PIR1bits.TMR2IF);  // 6: Enable PWM output pin and wait until Timer2 overflows; TMR2IF bit of the PIR1 register is set
    PWM2CONbits.PWM2OE = 1; //(we'll clear tris later) // 7: Enable the PWMx pin output driver(s) by clearing the associated TRIS bit(s) and setting the PWMxOE bit of the PWMxCON register
    PWM2DCH = 0x14;
}

void io_clock_on() {
    PIR1bits.TMR2IF = 0;   // reset interrupt flag
    PIE1bits.TMR2IE = 1;   // enable interrupt
    INTCONbits.PEIE = 1;   // enable peripheral interrupts
    INTCONbits.GIE = 1;    // enable global interrupts
    TRISCbits.TRISC3 = 0;  // enable output
}

void io_clock_off() {
    PIE1bits.TMR2IE = 0;  // disable interrupt
}
