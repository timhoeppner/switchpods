/********************************************************************
* NOTES:
* Code uses the Peripheral library support available with MCC18 Compiler
* Code Tested on:
* PicDem2+ demo board with PIC18F4685 controller
**********************************************************************/
#define USE_OR_MASKS
#include <p18cxxx.h>
#include "timers.h"
#include "LCD.h"
//**** Prototypes ****
void User_Timer(void);
//*** Global variables ****
unsigned int msec, sec, min, hr,MSD, MdD, LSD;
char day=04,month=04,year=09;
void main(void)
{
unsigned char config1=0x00;
unsigned char config2=0x00;
unsigned int timer_value=0x00;
//----Configure Timers----
timer_value = 0x00;
WriteTimer1(timer_value);
//clear timer if previously contains any value
config1 = T1_8BIT_RW | T1_SOURCE_EXT | T1_PS_1_8
| T1_OSC1EN_ON | T1_SYNC_EXT_ON | TIMER_INT_ON;
OpenTimer1(config1);
//API configures the tmer1 as per user defined
parameters
while(1)
{
DisplayTime(hr,min,sec,msec);
DisplayDate(day,month,year);
while(!PIR1bits.TMR1IF);
second completion
User_Timer();
}
//Display the Time on LCD
//Displays the Data on LCD
//wait for timer interruption after one milli
//Update Timer count
//**** Close Timer ****
CloseTimer1();
}
void User_Timer(void)
{
msec++;
if(msec>=100)
{
sec++;
msec=0;
if(sec>=60)
{
min++;
sec=0;
if(min>=60)
{
hr++;
min=0;
if(hr>=24)
{
hr=0;
}
}
}
}
}

