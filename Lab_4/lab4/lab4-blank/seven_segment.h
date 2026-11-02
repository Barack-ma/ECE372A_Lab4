// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: seven_segment.h
// Description: Seven-segment display module header.
//              Common-cathode 5611AS, driven through the 74HC595.
//----------------------------------------------------------------------//

#ifndef LAB4_SEVEN_SEGMENT_H
#define LAB4_SEVEN_SEGMENT_H

#include <avr/io.h>
#include <stdint.h>

void initSevenSegment(void);
void displayDigit(uint8_t digit);
void clearSevenSegment(void);

#endif /* LAB4_SEVEN_SEGMENT_H */
