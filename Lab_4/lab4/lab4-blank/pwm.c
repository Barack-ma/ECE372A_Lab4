// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: pwm.c
// Description: Timer 3 PWM and L293D direction control for one DC motor.
//
// Wiring (Arduino Mega 2560):
//   PE3 / OC3A / D5  -> L293D pin 1  (1,2EN)   speed PWM, scope this pin
//   PA0 / D22        -> L293D pin 2  (1A)
//   PA1 / D23        -> L293D pin 7  (2A)
//   L293D pin 3 (1Y) and pin 6 (2Y) -> motor
//   L293D pin 8 (VCC2) motor supply, pin 16 (VCC1) 5 V
//   L293D pins 4, 5, 12, 13 -> GND, and GND shared with the Mega
//   L293D pin 9 (3,4EN) -> GND so the unused half stays off
//
// Clockwise is 1A high and 2A low. If the shaft spins the other way,
// swap the two motor wires.
//----------------------------------------------------------------------//

#include "pwm.h"

// Fast PWM, mode 14, TOP = ICR3. Non-inverting output on OC3A.
// f_PWM = 16 MHz / (8 * 1024) = 1.953 kHz.
#define PWM_TOP 1023

static void driveEnableLow(void) {
  TCCR3A &= ~(1 << COM3A1);   // disconnect OC3A from the pin
  PORTE &= ~(1 << PE3);       // hold the L293D enable low
  OCR3A = 0;
}

static void setClockwise(void) {
  PORTA |= (1 << PA0);
  PORTA &= ~(1 << PA1);
}

static void setCounterClockwise(void) {
  PORTA &= ~(1 << PA0);
  PORTA |= (1 << PA1);
}

static void coastMotor(void) {
  PORTA &= ~((1 << PA0) | (1 << PA1));
}

void initPWM(void) {
  DDRA |= (1 << DDA0) | (1 << DDA1);     // direction pins
  DDRE |= (1 << DDE3);                   // OC3A / enable

  ICR3 = PWM_TOP;
  OCR3A = 0;

  // WGM3[3:0] = 1110 (mode 14). Clock prescaler = 8. OC3A starts disconnected.
  TCCR3A = (1 << WGM31);
  TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31);

  coastMotor();
  driveEnableLow();
}

/*
 * Potentiometer map, with 2.5 V at ADC count 512:
 *   0 V  .. 2.5 V   clockwise,        duty falls linearly to 0
 *   2.5 V           stopped
 *   2.5 V .. 5 V    counterclockwise, duty rises linearly to max
 *
 * OCR3A spans 0..1023, the same width as the ADC, so full scale on
 * either side of center is essentially 100% duty.
 */
void changeDutyCycle(uint16_t adcValue) {
  uint16_t duty;

  if (adcValue > 1023) {
    adcValue = 1023;
  }

  if (adcValue < 512) {
    duty = (uint16_t)(((uint32_t)(512 - adcValue) * PWM_TOP) / 512);
    setClockwise();
  } else if (adcValue > 512) {
    duty = (uint16_t)(((uint32_t)(adcValue - 512) * PWM_TOP) / 511);
    setCounterClockwise();
  } else {
    duty = 0;
    coastMotor();
  }

  // A few counts around 2.5 V still mean "stopped". Coasting here keeps
  // ADC noise from reversing the motor when the pot is centered.
  if (duty < 16) {
    duty = 0;
    coastMotor();
  }

  if (duty == 0) {
    driveEnableLow();
  } else {
    TCCR3A |= (1 << COM3A1);             // non-inverting PWM on OC3A
    OCR3A = duty;
  }
}

void turnMotorOff(void) {
  coastMotor();
  driveEnableLow();
}
