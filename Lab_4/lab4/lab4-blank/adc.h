// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: adc.h
// Description: ADC module header. Reads the potentiometer on A0.
//----------------------------------------------------------------------//

#ifndef LAB4_ADC_H
#define LAB4_ADC_H

#include <avr/io.h>
#include <stdint.h>

void initADC(void);

// Returns the 10-bit result formed from ADCL and ADCH (0 to 1023).
uint16_t readADC(void);

#endif /* LAB4_ADC_H */
