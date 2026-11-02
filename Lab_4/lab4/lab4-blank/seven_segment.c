// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: seven_segment.c
// Description: Digit patterns for a common-cathode 5611AS.
//              A 1 in the pattern lights that segment. Bit 0 is segment A
//              and bit 6 is segment G. The display is loaded by serially
//              shifting the pattern into the 74HC595 (extra credit).
//----------------------------------------------------------------------//

#include "seven_segment.h"
#include "shift_register.h"

// Segments: bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G.
static const uint8_t digitPatterns[10] = {
  0x3F, // 0: ABCDEF
  0x06, // 1: BC
  0x5B, // 2: ABDEG
  0x4F, // 3: ABCDG
  0x66, // 4: BCFG
  0x6D, // 5: ACDFG
  0x7D, // 6: ACDEFG
  0x07, // 7: ABC
  0x7F, // 8: ABCDEFG
  0x6F  // 9: ABCDFG
};

void initSevenSegment(void) {
  initShiftRegister();
  clearSevenSegment();
}

void displayDigit(uint8_t digit) {
  if (digit > 9) {
    clearSevenSegment();
    return;
  }
  shiftRegisterWrite(digitPatterns[digit]);
}

void clearSevenSegment(void) {
  shiftRegisterWrite(0x00);
}
