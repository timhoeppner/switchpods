// eep
/*********************************************************************
* NOTES:
* Code uses the Peripheral library support available with MCC18 Compiler
* Code Tested on:
* PicDem2+ demo board with PIC18F4685 controller
**********************************************************************/
#define USE_OR_MASKS
#include <p18cxxx.h>
#include "EEP.h"
unsigned char
EEPWrite[15] = "MICROCHIP_TECH", EEPRead[15],Error=0 ;
void main(void)
{
unsigned char q=0;
unsigned int address;
address = 0x0200;
/* Write single byte to Internal EEP*/
for(q=0;q<16;q++)
{
    Write_b_eep (address, EEPWrite[q]);
    // write into to EEPROM
    address++;
    //increment the address of EEPROM to next location
    /* Checks & waits the status of ER bit in EECON1 register */
    Busy_eep ();
}
8
address = 0x0200;
// initialize the starting address
Error = 0;
//clear the error flag
/* Read single byte from Internal EEP*/
for(q=0;q<16;q++)
{
EEPRead[q] = Read_b_eep (address++);
//read the EEPROM data written previously from corresponding address
if ( EEPRead[q] != EEPWrite[q] )
that was written
{
Error=1;
occur, then flag the error status
while(1);
}
}
while(1);
I2C Functions
//check if the data read abck is same as
//if the data read/ write match does not
//error occured
//End of program
}
