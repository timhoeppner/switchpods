#ifndef SWITCH_PODS_H
#define SWITCH_PODS_H

#define F_CPU 8000000UL  // 8 MHz, this might be wrong

// AVR Libraries
//#include <avr/io.h>
//#include <avr/eeprom.h>
//#include <util/delay.h>

// Application specific includes
//#include "buttons.h"

// Misc Constants
#define NUM_IO			9
//#define NUM_IO_CHECKED	2   // ????
#define NULL_MASK   0xffff

// Button Inputs
//   High Byte:
//      0: PORTA
//      1: PORTB
//      2: PORTC
//      3: PORTD
//      4: PORTE
//      5: PORTF
//      6: PORTG
//   Low Byte: pin mask
#define BTN1	0x0304
#define BTN2	0x0302
#define BTN3	0x0301
#define BTN4	0x0480
#define BTN5	0x0440
#define BTN6	0x0420
#define BTN7	0x0410
#define BTN8	0x0408
#define BTN9	0x0404

// LED Outputs - Left side
#define L1	0x0280
#define L2	0x0240
#define L3	0x0201
#define L4	0x0202
#define L5	0x0010
#define L6	0x0020
#define L7	0x0002
#define L8	0x0004
#define L9	0x0008

// LED Outputs - Right side
#define R1	0x0402
#define R2	0x0401
#define R3	0x0601
#define R4	0x0602
#define R5	0x0604
#define R6	0x0608
#define R7	0x0610
#define R8	0x0580
#define R9	0x0540

// Program mode, 0: ... 1: ....
#define PROGRAM_MODE    0x0510

// Always on, 0: left, 1: right ?? might have this backward, check silkscreen
#define ALWAYS_ON       0x0520

// Analog brightness pin
#define ANA_BRIGHTNESS  0x0001

// Relay outputs
#define OUT1    0x0308
#define OUT2    0x0310
#define OUT3    0x0320
#define OUT4    0x0340
#define OUT5    0x0380
#define OUT6    0x0101
#define OUT7    0x0102
#define OUT8    0x0104
#define OUT9    0x0108

#endif
