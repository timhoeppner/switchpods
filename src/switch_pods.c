#include "switch_pods.h"

char checkButton(int row);
void setOutputs(int alwayson, int leftled, int rightled, int relay, int value);
void setupIO(int row, int direction);

int main(void)
{
	//char buttonState = 1;
	//char ledState = 0;
	//char state = 0;

	volatile int buttonMask[NUM_IO] = {BTN1,BTN2,BTN3,BTN4,BTN5,BTN6,BTN7,BTN8,BTN9};
	volatile int leftLEDMask[NUM_IO] = {L1,L2,L3,L4,L5,L6,L7,L8,L9};
	volatile int rightLEDMask[NUM_IO] = {R1,R2,R3,R4,R5,R6,R7,R8,R9};
	volatile int relayMask[NUM_IO] = {OUT1,OUT2,OUT3,OUT4,OUT5,OUT6,OUT7,OUT8,OUT9};
	volatile int buttonDown[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile int buttonEventDown[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile int buttonEventUp[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile int buttonLatch[NUM_IO] = {1,1,1,1,1,1,1,1,1};
	volatile int buttonState[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile int i;
	volatile int programState = 0;	// 0: normal, 1: programming mode
	//volatile int row = 0;
	//volatile int prevRow = 9;
	//volatile int programCounter = 0;


	// Setup the I/O Ports
	/*DDRC = 0xBF;	// 1011 1111 0:input, 1:output
	DDRA = 0x03;	// 0000 0011 0:input, 1:output
	DDRD = 0x00;	// 0000 0000 0:input, 1:output
	PORTD = 0xFF;	// Setup pull-ups on port D
	PORTA |= 0x04;	// Setup pull-up on PA2*/

	setupIO(PROGRAM_MODE, 1);
	setupIO(ALWAYS_ON, 1);
	setupIO(ANA_BRIGHTNESS, 0);

	for(i=0;i<NUM_IO;i++)
	{
		setupIO(buttonMask[i], 1);
		setupIO(leftLEDMask[i], 0);
		setupIO(rightLEDMask[i], 0);
		setupIO(relayMask[i], 0);

		//uint8_t address = (uint8_t) i;
		//buttonLatch[i] = (int)( 0x00FF & eeprom_read_byte( address ) );
	}

	for(;;)
	{
		_delay_ms(20);

        // Recalculate button events
        for(i=0;i<NUM_IO;i++)
		{
			if(checkButton(buttonMask[i]) == 1)
			{
				if(!buttonDown[i])
				{
					buttonEventDown[i] = 1;
				}
				buttonDown[i] = 1;
			}
			else
			{
				if(buttonDown[i] == 1)
				{
					buttonEventUp[i] = 1;
				}
				buttonDown[i] = 0;
			}
		}

		if( checkButton(PROGRAM_MODE) == 1 )
		{
		    // Program mode stuff ...
		    if( programState == 0 )
		    {
		        // Switching from normal mode ...
		        _delay_ms(500);

		        // Set outputs to the button latch settings
		        for(i=0;i<NUM_IO;i++)
		        {
		            setOutputs( checkButton(ALWAYS_ON), leftLEDMask[i], rightLEDMask[i], NULL_MASK, buttonLatch[i] );
		            buttonEventUp[i] = 0;   // Clear button events
		        }

		        programState = 1;
		    }

            // Any buttons pushed? if they are then toggle the latching option
            for(i=0;i<NUM_IO;i++)
            {
                if( buttonEventUp[i] == 1 )
                {
                    if( buttonLatch[i] == 0 )
                    {
                        buttonLatch[i] = 1;
                    }
                    else
                    {
                        buttonLatch[i] = 0;
                    }

                    buttonEventUp[i] = 0;
                }

                setOutputs( checkButton(ALWAYS_ON), leftLEDMask[i], rightLEDMask[i], NULL_MASK, buttonLatch[i] );
            }
		}
		else
		{
		    // Normal operation stuff ...
		    if( programState == 1 )
		    {
		        // Switching from program mode ...
		        _delay_ms(500);

		        // Set outputs to the button latch settings
		        for(i=0;i<NUM_IO;i++)
		        {
		            // SAVE BUTTON LATCH STATE IN EEPROM!!!!!!!!

		            setOutputs( checkButton(ALWAYS_ON), leftLEDMask[i], rightLEDMask[i], relayMask[i], buttonState[i] );
		            buttonEventDown[i]  = 0;   // Clear button down events
		            buttonEventUp[i]    = 0;   // Clear button up events
		        }

		        programState = 0;
		    }

		    // Any buttons pushed?
            for(i=0;i<NUM_IO;i++)
            {
                if( buttonLatch[i] == 1 )
                {
                    // This row uses latching buttons
                    if( buttonEventUp[i] == 1 )
                    {
                        if( buttonState[i] == 0 )
                        {
                            buttonState[i] = 1;
                        }
                        else
                        {
                            buttonState[i] = 0;
                        }

                        buttonEventUp[i] = 0;
                    }
                }
                else
                {
                    // This row uses momentary buttons
                    if(buttonEventDown[i] == 1)
					{
						buttonState[i] = 1;
						buttonEventDown[i] = 0;
					}

					if(buttonEventUp[i] == 1)
					{
						buttonState[i] = 0;
						buttonEventUp[i] = 0;
					}
                }

                setOutputs( checkButton(ALWAYS_ON), leftLEDMask[i], rightLEDMask[i], relayMask[i], buttonState[i] );
            }
		}

	}
}

char checkButton(int row)
{
	volatile int port;
	volatile unsigned char mask;
	volatile int state = 0;

	port = (row >> 8);
	mask = (char)(row & 0x00FF);

	switch( port )
	{
    case 0:
        state = (LATA & mask) ? 0 : 1;
        break;
    case 1:
        state = (LATB & mask) ? 0 : 1;
        break;
    case 2:
        state = (LATC & mask) ? 0 : 1;
        break;
    case 3:
        state = (LATD & mask) ? 0 : 1;
        break;
    case 4:
        state = (LATE & mask) ? 0 : 1;
        break;
    case 5:
        state = (LATF & mask) ? 0 : 1;
        break;
    case 6:
        state = (LATG & mask) ? 0 : 1;
        break;
    default:
        // ...
	}

	return state;
}

void setOutput(int row, int value)
{
    volatile int port;
	volatile unsigned char mask;

	port = (row >> 8);
	mask = (char)(row & 0x00FF);

	switch( port )
	{
    case 0:
        if(value == 1) { PORTA |= mask; } else { PORTA &= (~mask); }
        break;
    case 1:
        if(value == 1) { PORTB |= mask; } else { PORTB &= (~mask); }
        break;
    case 2:
        if(value == 1) { PORTC |= mask; } else { PORTC &= (~mask); }
        break;
    case 3:
        if(value == 1) { PORTD |= mask; } else { PORTD &= (~mask); }
        break;
    case 4:
        if(value == 1) { PORTE |= mask; } else { PORTE &= (~mask); }
        break;
    case 5:
        if(value == 1) { PORTF |= mask; } else { PORTF &= (~mask); }
        break;
    case 6:
        if(value == 1) { PORTG |= mask; } else { PORTG &= (~mask); }
        break;
    default:
        // ...
	}
}

void setOutputs(int alwayson, int leftled, int rightled, int relay, int value)
{
	if( alwayson == 0 )
	{
	    setOutput(leftled, 1);
	    setOutput(rightled, value);
	}
	else
	{
	    setOutput(leftled, value);
	    setOutput(rightled, 1);
	}

	setOutput(relay, value);
}
