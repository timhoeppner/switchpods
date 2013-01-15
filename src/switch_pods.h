#ifndef SWITCH_PODS_H
#define SWITCH_PODS_H

#define F_CPU 8000000UL  // 8 MHz

// AVR Libraries
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

// Application specific includes
//#include "buttons.h"

// Misc Constants
#define NUM_IO			9
#define NUM_IO_CHECKED	2

// Button Inputs (HighByte, 0:PORTD 1:PORTA, LowByte, Input Mask)
#define BTN1	0x0010
#define BTN2	0x0008
#define BTN3	0x0004
#define BTN4	0x0002
#define BTN5	0x0001
#define BTN6	0x0080
#define BTN7	0x0040
#define BTN8	0x0020
#define BTN9	0x0104

// LED Outputs (HighByte, 0:PORTC 1:PORTA, LowByte, Input Mask)
#define R1	0x0020
#define R2	0x0010
#define R3	0x0008
#define R4	0x0004
#define R5	0x0002
#define R6	0x0001
#define R7	0x0102
#define R8	0x0080
#define R9	0x0101


#endif
