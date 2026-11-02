// Author: Barack A. Marwanga
// Net ID: 23862638
// Date: 10/30/2026
// Assignment: Lab 4
// File: pwm.h
// Description: PWM and motor-control module header.
//----------------------------------------------------------------------//

#ifndef LAB4_PWM_H
#define LAB4_PWM_H

#include <avr/io.h>
#include <stdint.h>

void initPWM(void);

// adcValue is the 10-bit combination of ADCH and ADCL.
// Updates OCR3A and the L293D direction inputs from that reading.
void changeDutyCycle(uint16_t adcValue);

// Forces the L293D enable low and coasts the motor.
void turnMotorOff(void);

#endif /* LAB4_PWM_H */
