#include "switch_pods.h"

char checkButton(int row);
void setOutput(int row, int value);

int main(void)
{
	//char buttonState = 1;
	//char ledState = 0;
	//char state = 0;
	
	volatile int buttonMask[NUM_IO] = {BTN1,BTN2,BTN3,BTN4,BTN5,BTN6,BTN7,BTN8,BTN9};
	volatile int outputMask[NUM_IO] = {R1,R2,R3,R4,R5,R6,R7,R8,R9};
	volatile int buttonDown[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile int buttonEventDown[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile int buttonEventUp[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile int buttonLatch[NUM_IO] = {1,1,1,1,1,1,1,1,1};
	volatile int buttonState[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile int i;
	volatile int programState = 0;	// 0: normal, 1: programming mode
	//volatile int row = 0;
	//volatile int prevRow = 9;
	volatile int programCounter = 0;
	

	// Setup the I/O Ports
	DDRC = 0xBF;	// 1011 1111 0:input, 1:output
	DDRA = 0x03;	// 0000 0011 0:input, 1:output
	DDRD = 0x00;	// 0000 0000 0:input, 1:output
	PORTD = 0xFF;	// Setup pull-ups on port D
	PORTA |= 0x04;	// Setup pull-up on PA2

	// Read the button latch configuration from EEPROM
	for(i=0;i<NUM_IO;i++)
	{
		uint8_t address = (uint8_t) i;
		buttonLatch[i] = (int)( 0x00FF & eeprom_read_byte( address ) );
	}

	for(;;)
	{
		_delay_ms(20);

		// Check if trying to enter programming mode
		if(buttonDown[0] && buttonDown[8])
		{
			if(programCounter > 100)
			{
				// Change operating mode
				if(programState == 0)
				{
					programState = 1;
				}
				else
				{
					programState = 0;
				}

				// Flash all LEDs to show change of state
				for(i=0;i<NUM_IO;i++)
				{
					setOutput(outputMask[i], 1);
				}

				_delay_ms(1000);

				for(i=0;i<NUM_IO;i++)
				{
					setOutput(outputMask[i], 0);
				}

				_delay_ms(1000);

				buttonDown[0] = 0;
				buttonDown[8] = 0;

				// Wait until the buttons have been released
				while(checkButton(buttonMask[0]) || checkButton(buttonMask[8]));

				// Save the latch configuration in EEPROM if exiting programming mode
				if(programState == 0)
				{
					for(i=0;i<NUM_IO;i++)
					{
						uint8_t address = (uint8_t) i;
						uint8_t data = (uint8_t) buttonLatch[i];
						eeprom_write_byte( address, data );
					}
				}				

				// Reset all buttons
				for(i=0;i<NUM_IO;i++)
				{
					buttonEventDown[i] = 0;
					buttonEventUp[i] = 0;
					buttonState[i] = 0;
					buttonDown[i] = 0;

					//if(buttonLatch[i] == 0)
					//{
					//	buttonState[i] = 0;
					//}
				}
			}
			else
			{
				programCounter++;
			}
		}
		else
		{
			programCounter = 0;
		}
		
		// Check the button inputs
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

			//setOutput(outputMask[i], buttonDown[i]);
			if(programState == 0)
			{
				if(buttonLatch[i] == 1)
				{
					//buttonEventUp[i] = 0;

					if(buttonEventUp[i] == 1)
					{
						if(buttonState[i] == 1)
						{
							buttonState[i] = 0;
						}
						else
						{
							buttonState[i] = 1;
						}

						buttonEventUp[i] = 0;
					}
					
				}
				else
				{
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

				setOutput(outputMask[i], buttonState[i]);
			}
			else
			{
				// Programming mode
				if(buttonEventUp[i] == 1)
				{
					if(buttonLatch[i] == 0)
					{
						buttonLatch[i] = 1;
					}
					else
					{
						buttonLatch[i] = 0;
					}

					buttonEventUp[i] = 0;
				}

				setOutput(outputMask[i], buttonLatch[i]);
			}
			//setOutput(outputMask[i], 1);
		}
	}
}

char checkButton(int row)
{
	volatile int port;
	volatile unsigned char mask;
	volatile int state;

	port = (row >> 8);
	mask = (char)(row & 0x00FF);

	if(port == 0)
	{
		if( !(PIND & mask) ) 	state = 1;
		else					state = 0;
	}
	else
	{
		if( !(PINA & mask) ) 	state = 1;
		else					state = 0;
	}

	return state;
}

void setOutput(int row, int value)
{
	volatile int port;
	volatile unsigned char mask;
	//volatile unsigned char invMask;

	port = (row >> 8);
	mask = (unsigned char)(row & 0x00FF);
	//invMask = !mask;

	if(port == 0)
	{
		if(value == 1)
		{
			PORTC |= mask;
		}
		else
		{
			PORTC &= (~mask);
		}
	}
	else
	{
		if(value == 1)
		{
			PORTA |= mask;
		}
		else
		{
			PORTA &= (~mask);
		}
	}
}
