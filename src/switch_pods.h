#ifndef SWITCH_PODS_H
#define SWITCH_PODS_H

// PIC18F65K22 Configuration Bit Settings

#include <xc.h>

// CONFIG1L
#pragma config RETEN = ON       // VREG Sleep Enable bit (Enabled)
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG    // SOSC Power Selection and mode Configuration bits (Digital (SCLKI) mode)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled) <- disabled temporarly

// CONFIG1H
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 0         // Brown-out Reset Voltage bits (3.0V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

// CONFIG3L
#pragma config RTCOSC = SOSCREF // RTCC Clock Select (RTCC uses SOSC)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 Mux (RC1)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RG5 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-01FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 02000-03FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 04000-05FFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 06000-07FFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-017FF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 01800-03FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 04000-05FFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 06000-07FFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBRT0 = OFF      // Table Read Protect 00800-017FF (Disabled)
#pragma config EBRT1 = OFF      // Table Read Protect 01800-03FFF (Disabled)
#pragma config EBRT2 = OFF      // Table Read Protect 04000-05FFF (Disabled)
#pragma config EBRT3 = OFF      // Table Read Protect 06000-07FFF (Disabled)

// CONFIG7H
#pragma config EBRTB = OFF      // Table Read Protect Boot (Disabled)

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
