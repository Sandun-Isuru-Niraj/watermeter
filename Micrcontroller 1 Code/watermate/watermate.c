/*
 * watermate.c
 *
 * Created: 01/03/2017 18:24:32
 *  Author: Sandun Isuru Niraj
 */ 
#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "incousart.h"

uint16_t count=0;    //Main revolution counter

//uint16_t rpm=0;   //Revolution per minute

uint16_t rps=0;   //Revolution per second

int main(void)
{

	usart_init();
	char rps1[10];
	//Init INT0
	MCUCR|=(1<<ISC01);   //Falling edge on INT0 triggers interrupt.

	GICR|=(1<<INT0);  //Enable INT0 interrupt
	
	//Timer1 is used as 1 sec time base
	//Timer Clock = 1/1024 of sys clock
	//Mode = CTC (Clear Timer On Compare)
	TCCR1B|=((1<<WGM12)|(1<<CS12)|(1<<CS10));

	//Compare value=976

	OCR1A=976;

	TIMSK|=(1<<OCIE1A);  //Output compare 1A interrupt enable

	//Enable interrupts globaly
	sei();
	
		DDRC &= ~(1<<PC0);
		DDRB &= ~(1<<PB0);
		int pHsen=0;
		int Colsen =0;
		char cpHsen[2];
		char cColsen[2]; 
    while(1)
    {
			if(PINB & (1<<PB0) == 1){
			pHsen = 1;
		}else{
			pHsen = 0;
		}
		
		if(PINC & (1<<PC0) == 1){
			Colsen = 1;
		}else{
			Colsen = 0;
		}
		
		itoa(pHsen,cpHsen,10);
		itoa(Colsen,cColsen,10);
		
		
	/*	usart_string_transmit(cpHsen);
		usart_string_transmit("\n");
		usart_string_transmit(cColsen);
		usart_string_transmit("\n");
*/
		
		//itoa(rpm, rpm1,10);
		itoa(rps, rps1,10);
		//strcat(rps1,cpHsen);
		//strcat(rps1,cColsen);
		usart_string_transmit(rps1);
		usart_string_transmit("A");
		usart_string_transmit(cpHsen);
		usart_string_transmit("A");
		usart_string_transmit(cColsen);
		usart_string_transmit("\n");
		_delay_ms(1000);
		
       

    }
}

ISR(INT0_vect)
{
	//CPU Jumps here automatically when INT0 pin detect a falling edge
	count++;
}

ISR(TIMER1_COMPA_vect)
{
	//CPU Jumps here every 1 sec exactly!
	rps=count;
	//rpm=rps*60/7;
	count=0;
}