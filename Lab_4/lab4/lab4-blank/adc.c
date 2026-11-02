// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: adc.c
// Description: ADC module. Samples the potentiometer wiper on A0 (PF0 / ADC0).
//----------------------------------------------------------------------//

#include "adc.h"

/*
 * A0 is ADC channel 0. AVCC is the reference, so 0 V reads as 0 and
 * 5 V reads as 1023. The prescaler divides 16 MHz by 128 to put the
 * ADC clock at 125 kHz, inside the 50-200 kHz range the converter wants.
 */
void initADC(void) {
  DIDR0 |= (1 << ADC0D);                 // turn off the digital buffer on A0
  ADMUX = (1 << REFS0);                  // AVCC reference, right-adjusted, ADC0
  ADCSRB = 0;                            // MUX5 = 0, free-running off
  ADCSRA = (1 << ADEN)
         | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // Throw away the first conversion. It is less accurate after enabling.
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC)) {
  }
}

/*
 * ADCL must be read before ADCH. Reading ADCL locks the result until
 * ADCH is read, so the two bytes stay from the same conversion.
 */
uint16_t readADC(void) {
  uint8_t low;
  uint8_t high;

  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC)) {
  }

  low = ADCL;
  high = ADCH;
  return ((uint16_t)high << 8) | low;
}
