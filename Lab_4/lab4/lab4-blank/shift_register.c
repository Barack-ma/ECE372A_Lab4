// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: shift_register.c
// Description: Bit-banged serial load of the SN74HC595.
//
// Extra credit wiring (Arduino Mega 2560). PORTC is not used.
//   PA2 / D24 -> 74HC595 pin 14 (SER)
//   PA3 / D25 -> 74HC595 pin 11 (SRCLK)
//   PA4 / D26 -> 74HC595 pin 12 (RCLK)
//   74HC595 pin 13 (OE)    -> GND   (outputs enabled)
//   74HC595 pin 10 (SRCLR) -> 5 V   (do not clear)
//   74HC595 pin 16 -> 5 V, pin 8 -> GND
//
// Segment outputs, each through a 1 k resistor, to the 5611AS:
//   Q0 pin 15 -> pin 7  (A)
//   Q1 pin 1  -> pin 6  (B)
//   Q2 pin 2  -> pin 4  (C)
//   Q3 pin 3  -> pin 2  (D)
//   Q4 pin 4  -> pin 1  (E)
//   Q5 pin 5  -> pin 9  (F)
//   Q6 pin 6  -> pin 10 (G)
//   Q7 pin 7  is unused (decimal point, display pin 5)
//   5611AS pins 3 and 8 (common cathode) -> GND
//----------------------------------------------------------------------//

#include "shift_register.h"

#define SER_PIN   PA2
#define SRCLK_PIN PA3
#define RCLK_PIN  PA4

void initShiftRegister(void) {
  DDRA |= (1 << SER_PIN) | (1 << SRCLK_PIN) | (1 << RCLK_PIN);
  // Idle the serial lines low so the first clock has a known edge.
  PORTA &= ~((1 << SER_PIN) | (1 << SRCLK_PIN) | (1 << RCLK_PIN));
}

/*
 * The 595 samples SER on the rising edge of SRCLK and shifts toward Q7,
 * so the last bit clocked in lands on Q0. Clock bit 7 first.
 * A rising edge on RCLK then copies the shift register to the outputs.
 */
void shiftRegisterWrite(uint8_t data) {
  uint8_t bit;

  for (bit = 0; bit < 8; bit++) {
    if (data & 0x80) {
      PORTA |= (1 << SER_PIN);
    } else {
      PORTA &= ~(1 << SER_PIN);
    }

    PORTA |= (1 << SRCLK_PIN);
    PORTA &= ~(1 << SRCLK_PIN);
    data <<= 1;
  }

  PORTA |= (1 << RCLK_PIN);
  PORTA &= ~(1 << RCLK_PIN);
}
