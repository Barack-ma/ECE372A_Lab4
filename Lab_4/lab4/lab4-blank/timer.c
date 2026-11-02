// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: timer.c
// Description: Timer 0 produces a 1 ms tick for switch debounce.
//              Timer 1 counts out the ten 1-second steps of the display.
//----------------------------------------------------------------------//

#include "timer.h"
#include <avr/interrupt.h>

static volatile bool secondFlag = false;

/*
 * Timer 0, CTC mode. Prescaler 64 and OCR0A = 249:
 *   16 MHz / 64 / 250 = 1 kHz, so each compare match is 1 ms.
 * The clock is left off here and is started inside delayMs.
 */
void initTimer0(void) {
  TCCR0A = (1 << WGM01);
  TCCR0B = 0;
  OCR0A = 249;
}

void delayMs(unsigned int delay) {
  unsigned int i;

  for (i = 0; i < delay; i++) {
    TCNT0 = 0;
    TCCR0B |= (1 << CS01) | (1 << CS00);          // prescaler 64
    while (!(TIFR0 & (1 << OCF0A))) {
    }
    TIFR0 |= (1 << OCF0A);
    TCCR0B &= ~((1 << CS01) | (1 << CS00));
  }
}

/*
 * Timer 1, CTC mode. Prescaler 1024 and OCR1A = 15624:
 *   16 MHz / 1024 = 15625 Hz, and counting 0..15624 is exactly 1 second.
 * The interrupt is armed here. startTimer1() turns the clock on.
 */
void initTimer1(void) {
  TCCR1A = 0;
  TCCR1B = (1 << WGM12);
  OCR1A = 15624;
  TIMSK1 = (1 << OCIE1A);
  TIFR1 |= (1 << OCF1A);
  secondFlag = false;
}

void startTimer1(void) {
  secondFlag = false;
  TCNT1 = 0;
  TIFR1 |= (1 << OCF1A);
  TCCR1B |= (1 << CS12) | (1 << CS10);            // prescaler 1024
}

void stopTimer1(void) {
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
  secondFlag = false;
}

bool secondElapsed(void) {
  if (secondFlag) {
    secondFlag = false;
    return true;
  }
  return false;
}

ISR(TIMER1_COMPA_vect) {
  secondFlag = true;
}
