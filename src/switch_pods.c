//#include <htc.h>           /* Global Header File */
#include <stdint.h>        /* For uint8_t definition */
#include <plib/EEP.h>
#include <plib/timers.h>
#include <plib/adc.h>

#include "switch_pods.h"

char checkButton(uint16_t row);
void setOutput(uint16_t row, uint16_t value);
void setOutputs(uint16_t alwayson, uint16_t ledIndex, uint16_t relay, uint16_t value);
void setupIO(uint16_t row, uint16_t direction);
void _delay_ms(uint16_t ms);

volatile uint16_t buttonMask[NUM_IO] = {BTN1,BTN2,BTN3,BTN4,BTN5,BTN6,BTN7,BTN8,BTN9};
volatile uint16_t leftLEDMask[NUM_IO] = {L1,L2,L3,L4,L5,L6,L7,L8,L9};
volatile uint16_t rightLEDMask[NUM_IO] = {R1,R2,R3,R4,R5,R6,R7,R8,R9};
volatile uint16_t relayMask[NUM_IO] = {OUT1,OUT2,OUT3,OUT4,OUT5,OUT6,OUT7,OUT8,OUT9};
volatile uint16_t outputFlag[NUM_IO] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile uint16_t outputAlwaysOnFlag = 0;
volatile uint16_t outputHighLength = OUTPUT_PWM_MIN_HIGH;
volatile uint16_t outputPWMState = 0;

void interrupt pwm_int(void)
{
    uint16_t i;

    if( TMR0IE && TMR0IF )
    {
        TMR0IF = 0;
        if(outputPWMState == 1)
        {
            // Set next interrupt for PWM low time
            WriteTimer0(0xffff - outputHighLength);
        }
        else
        {
            // Set next interrupt for PWM high time
            WriteTimer0(0xffff - OUTPUT_PWM_PERIOD + outputHighLength);
        }

        // Turn the LEDs on or off depending on which PWM state we are in
        for(i=0;i<NUM_IO;i++)
        {
            if( outputAlwaysOnFlag == 0 )
            {
                setOutput(leftLEDMask[i], outputPWMState);
                setOutput(rightLEDMask[i], outputFlag[i] & outputPWMState);
            }
            else
            {
                setOutput(leftLEDMask[i], outputFlag[i] & outputPWMState);
                setOutput(rightLEDMask[i], outputPWMState);
            }
        }

        outputPWMState = !outputPWMState;

        return;
    }
}

uint16_t main(void)
{
	//char buttonState = 1;
	//char ledState = 0;
	//char state = 0;

	volatile uint16_t buttonDown[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile uint16_t buttonEventDown[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile uint16_t buttonEventUp[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile uint16_t buttonLatch[NUM_IO] = {1,1,1,1,1,1,1,1,1};
	volatile uint16_t buttonState[NUM_IO] = {0,0,0,0,0,0,0,0,0};
	volatile uint16_t i;
	volatile uint16_t programState = 0;	// 0: normal, 1: programming mode
	//volatile uint16_t row = 0;
	//volatile uint16_t prevRow = 9;
	//volatile uint16_t programCounter = 0;

        volatile uint16_t adcResult = 0;


	// Setup the I/O Ports
	/*DDRC = 0xBF;	// 1011 1111 0:input, 1:output
	DDRA = 0x03;	// 0000 0011 0:input, 1:output
	DDRD = 0x00;	// 0000 0000 0:input, 1:output
	PORTD = 0xFF;	// Setup pull-ups on port D
	PORTA |= 0x04;	// Setup pull-up on PA2*/

        // Setup the PWM timer
        WriteTimer0(0xffff - 0x10); // This should trigger the interrupt almost immediately
        OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_2);

        ANCON0 = 1; // Channel AN0 is analog input
        ANCON1 = 0;
        ANCON2 = 0;

        // Setup ADC
        ADCON0 = 0; // Channel AN0, int off, no conversion started
        ADCON1 = (0x2 << 4) | (0x0 << 3) | (0x0);   // Internal 2.048 reference, AVss VREF-, Negative channel AVss
        ADCON2 = (0x1 << 7) | (0x7 << 3) | (0x0);   // Right justified, 20 Tad acquisition time (max), Fosc / 2

        // Turn on the ADC
        ADCON0 = 1;

        // Start our first conversion
        ConvertADC();

	setupIO(PROGRAM_MODE, 1);
	setupIO(ALWAYS_ON, 1);
	setupIO(ANA_BRIGHTNESS, 1);

	for(i=0;i<NUM_IO;i++)
	{
		setupIO(buttonMask[i], 1);
		setupIO(leftLEDMask[i], 0);
		setupIO(rightLEDMask[i], 0);
		setupIO(relayMask[i], 0);

		//uint8_t address = (uint8_t) i;
		buttonLatch[i] = (uint16_t)( 0x00FF & Read_b_eep( i ) );
	}

        /*for(;;)
        {
            _delay_ms(1);
            setOutputs( !checkButton(ALWAYS_ON), leftLEDMask[2], rightLEDMask[2], NULL_MASK, 0 );

            _delay_ms(1);
            setOutputs( !checkButton(ALWAYS_ON), leftLEDMask[2], rightLEDMask[2], NULL_MASK, 1 );

            _delay_ms(10);
            setOutputs( !checkButton(ALWAYS_ON), leftLEDMask[2], rightLEDMask[2], NULL_MASK, 0 );

            _delay_ms(10);
            setOutputs( !checkButton(ALWAYS_ON), leftLEDMask[2], rightLEDMask[2], NULL_MASK, 1 );

            _delay_ms(100);
        }*/

        // Enable interrupts
        ei();

	for(;;)
	{
		_delay_ms(20);

                // Check ADC status
                if(BusyADC() == 0)
                {
                    // Conversion ready
                    adcResult = (uint16_t)(ReadADC() & 0x0fff);
                    outputHighLength = OUTPUT_PWM_MIN_HIGH + adcResult;
                    if(outputHighLength >= OUTPUT_PWM_PERIOD) outputHighLength = OUTPUT_PWM_PERIOD - 1 ;
                    ConvertADC();
                }

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
		            setOutputs( !checkButton(ALWAYS_ON), i, NULL_MASK, buttonLatch[i] );
		            buttonEventUp[i] = 0;   // Clear button events
                            buttonEventDown[i] = 0;
		        }

		        programState = 1;
		    }

            // Any buttons pushed? if they are then toggle the latching option
            for(i=0;i<NUM_IO;i++)
            {
                if( buttonEventDown[i] == 1 )
                {
                    if( buttonLatch[i] == 0 )
                    {
                        buttonLatch[i] = 1;
                    }
                    else
                    {
                        buttonLatch[i] = 0;
                    }

                    buttonEventDown[i] = 0;
                }

                setOutputs( !checkButton(ALWAYS_ON), i, NULL_MASK, buttonLatch[i] );
            }
		}
		else
		{
		    // Normal operation stuff ...
		    if( programState == 1 )
		    {
		        // Switching from program mode ...
		        _delay_ms(2000);

		        // Set outputs to the button latch settings
		        for(i=0;i<NUM_IO;i++)
		        {
		            Write_b_eep(i, buttonLatch[i]);
                            Busy_eep ();

		            setOutputs( !checkButton(ALWAYS_ON), i, relayMask[i], buttonState[i] );
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
                    if( buttonEventDown[i] == 1 )
                    {
                        if( buttonState[i] == 0 )
                        {
                            buttonState[i] = 1;
                        }
                        else
                        {
                            buttonState[i] = 0;
                        }

                        buttonEventDown[i] = 0;
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

                setOutputs( !checkButton(ALWAYS_ON), i, relayMask[i], buttonState[i] );
            }
		}

	}
}

void _delay_ms(uint16_t ms)
{
    for(int i=0; i<ms;i++)
    {
        for(int j=0; j<145; j++) ;
    }
}

void setupIO(uint16_t row, uint16_t direction)
{
	volatile uint16_t port;
	volatile unsigned char mask;
	volatile uint16_t state = 0;

	port = (row >> 8);
	mask = (char)(row & 0x00FF);

	switch( port )
	{
    case 0:
        if(direction == 1) { TRISA |= mask; } else { TRISA &= (~mask); }
        break;
    case 1:
        if(direction == 1) { TRISB |= mask; } else { TRISB &= (~mask); }
        break;
    case 2:
        if(direction == 1) { TRISC |= mask; } else { TRISC &= (~mask); }
        break;
    case 3:
        if(direction == 1) { TRISD |= mask; } else { TRISD &= (~mask); }
        break;
    case 4:
        if(direction == 1) { TRISE |= mask; } else { TRISE &= (~mask); }
        break;
    case 5:
        if(direction == 1) { TRISF |= mask; } else { TRISF &= (~mask); }
        break;
    case 6:
        if(direction == 1) { TRISG |= mask; } else { TRISG &= (~mask); }
        break;
    default:
        // ...
        break;
	}
}

char checkButton(uint16_t row)
{
	volatile uint16_t port;
	volatile unsigned char mask;
	volatile uint16_t state = 0;

	port = (row >> 8);
	mask = (char)(row & 0x00FF);

	switch( port )
	{
    case 0:
        state = (PORTA & mask) ? 0 : 1;
        break;
    case 1:
        state = (PORTB & mask) ? 0 : 1;
        break;
    case 2:
        state = (PORTC & mask) ? 0 : 1;
        break;
    case 3:
        state = (PORTD & mask) ? 0 : 1;
        break;
    case 4:
        state = (PORTE & mask) ? 0 : 1;
        break;
    case 5:
        state = (PORTF & mask) ? 0 : 1;
        break;
    case 6:
        state = (PORTG & mask) ? 0 : 1;
        break;
    default:
        // ...
        break;
	}

	return state;
}

void setOutput(uint16_t row, uint16_t value)
{
    volatile uint16_t port;
	volatile unsigned char mask;

	port = (row >> 8);
	mask = (char)(row & 0x00FF);

	switch( port )
	{
    case 0:
        if(value == 1) { LATA |= mask; } else { LATA &= (~mask); }
        break;
    case 1:
        if(value == 1) { LATB |= mask; } else { LATB &= (~mask); }
        break;
    case 2:
        if(value == 1) { LATC |= mask; } else { LATC &= (~mask); }
        break;
    case 3:
        if(value == 1) { LATD |= mask; } else { LATD &= (~mask); }
        break;
    case 4:
        if(value == 1) { LATE |= mask; } else { LATE &= (~mask); }
        break;
    case 5:
        if(value == 1) { LATF |= mask; } else { LATF &= (~mask); }
        break;
    case 6:
        if(value == 1) { LATG |= mask; } else { LATG &= (~mask); }
        break;
    default:
        // ...
        break;
	}
}

void setOutputs(uint16_t alwayson, uint16_t ledIndex, uint16_t relay, uint16_t value)
{
    outputAlwaysOnFlag = alwayson;

    // This flag allows the LEDs to be PWM'ed
    outputFlag[ledIndex] = value;

    /*if( alwayson == 0 )
    {
        setOutput(leftled, 1);
        setOutput(rightled, value);
    }
    else
    {
        setOutput(leftled, value);
        setOutput(rightled, 1);
    }*/


    // Relay output gets set on demand because it's not PWM'ed
    setOutput(relay, value);
}
