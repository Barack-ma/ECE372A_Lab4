// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: shift_register.h
// Description: Shift-register module for the extra-credit display interface.
//              Replaces a direct PORTC drive of the seven-segment display.
//----------------------------------------------------------------------//

#ifndef LAB4_SHIFT_REGISTER_H
#define LAB4_SHIFT_REGISTER_H

#include <avr/io.h>
#include <stdint.h>

void initShiftRegister(void);

// Serially shifts one byte into the 74HC595 and latches it to the outputs.
// Bit 0 ends up on Q0. A 1 turns that segment on (common cathode).
void shiftRegisterWrite(uint8_t data);

#endif /* LAB4_SHIFT_REGISTER_H */
