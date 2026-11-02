// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: main.c
// Description: Potentiometer sets the direction and speed of one DC motor.
//              Pressing the switch on PD0 turns the motor off and counts
//              the seven-segment display from 9 down to 0. The display
//              then turns off and the motor runs again. INT0 is disabled
//              for the whole countdown.
//
//              Extra credit: the 5611AS is driven by a 74HC595, not PORTC.
//              See shift_register.c for the display wiring and pwm.c for
//              the L293D wiring. Potentiometer wiper is A0; the other two
//              pot pins are 5 V and GND. Switch is D21 (PD0) to GND.
//----------------------------------------------------------------------//

#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "switch.h"
#include "seven_segment.h"

#define DEBOUNCE_MS 50

/*
 * WAIT_PRESS        motor follows the potentiometer
 * DEBOUNCE_PRESS    Timer 0 debounce of the press
 * COUNTDOWN         motor off, display counts 9..0, INT0 disabled
 * WAIT_RELEASE      motor running again, waiting for the button to come up
 * DEBOUNCE_RELEASE  Timer 0 debounce of the release
 */
typedef enum {
  WAIT_PRESS,
  DEBOUNCE_PRESS,
  COUNTDOWN,
  WAIT_RELEASE,
  DEBOUNCE_RELEASE
} state_t;

volatile state_t state = WAIT_PRESS;

int main(void) {
  uint8_t countdownDigit = 9;

  initTimer0();
  initTimer1();
  initADC();
  initPWM();
  initSevenSegment();
  initSwitchPD0();
  sei();

  while (1) {
    switch (state) {
      case WAIT_PRESS:
        changeDutyCycle(readADC());
        break;

      case DEBOUNCE_PRESS:
        delayMs(DEBOUNCE_MS);
        if (isSwitchPressed()) {
          turnMotorOff();
          disableSwitchInterrupt();
          countdownDigit = 9;
          displayDigit(countdownDigit);
          startTimer1();
          state = COUNTDOWN;
        } else {
          state = WAIT_PRESS;
        }
        break;

      case COUNTDOWN:
        // Ten 1-second ticks: 9 is shown immediately, then 8..0,
        // and the tenth tick blanks the display and restarts the motor.
        if (secondElapsed()) {
          if (countdownDigit == 0) {
            stopTimer1();
            clearSevenSegment();
            if (isSwitchPressed()) {
              state = WAIT_RELEASE;
            } else {
              state = WAIT_PRESS;
            }
            enableSwitchInterrupt();
          } else {
            countdownDigit--;
            displayDigit(countdownDigit);
          }
        }
        break;

      case WAIT_RELEASE:
        changeDutyCycle(readADC());
        break;

      case DEBOUNCE_RELEASE:
        delayMs(DEBOUNCE_MS);
        state = WAIT_PRESS;
        break;

      default:
        state = WAIT_PRESS;
        break;
    }
  }

  return 0;
}

// Any edge on INT0. Ignored while the countdown has disabled the interrupt.
ISR(INT0_vect) {
  if (state == WAIT_PRESS) {
    state = DEBOUNCE_PRESS;
  } else if (state == WAIT_RELEASE) {
    state = DEBOUNCE_RELEASE;
  }
}
