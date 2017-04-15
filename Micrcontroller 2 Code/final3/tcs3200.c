/*******************************************************************************
TCS3200 Colour Sensor Library for AVR

Default Connection
******************

      | ATmega32
S2  -> PA0
S3  -> PA1
OUT -> PA2
LED -> PA3


NOTICE
--------
NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN
COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.


WRITTEN BY:
AVINASH GUPTA
gmail@avinashgupta.com (Yes ! Its correct !)
*******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "tcs3200.h"

void InitTCS3200()
{
	//Initialize S2 and S3 line as OUTPUT
	TCS_S2_DDR|=(1<<TCS_S2_POS);
	TCS_S3_DDR|=(1<<TCS_S3_POS);
	
	//LED Port
	TCS_LED_DDR|=(1<<TCS_LED_POS);	
}


void TCSSelectRed()
{
	TCSS2Low();
	TCSS3Low();
}

void TCSSelectGreen()
{
	TCSS2High();
	TCSS3High();
}

void TCSSelectBlue()
{
	TCSS2Low();
	TCSS3High();
}

void TCSSelectClear()
{
	TCSS2High();
	TCSS3Low();
}

uint32_t TCSMeasure()
{
	//If the function is entered when the level on OUT line was low
	//Then wait for it to become high.
	if(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)))
	{
		while(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)));	//Wait for rising edge	
	}
	
	
	while(TCS_OUT_PORT & (1<<TCS_OUT_POS));	//Wait for falling edge
	
	TCNT1=0x0000;//Reset Counter
	
	TCCR1B=(1<<CS10);	//Prescaller = F_CPU/1 (Start Counting)
	
	while(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)));	//Wait for rising edge
	
	//Stop Timer
	TCCR1B=0x00;
	
	return ((float)8000000UL/TCNT1);
	
}