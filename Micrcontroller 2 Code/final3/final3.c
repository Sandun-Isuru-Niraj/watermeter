/*
 * final.c
 *
 Flow Sensor ===> Yellow -> PD2
 
 
 
 
 
 color Sensor ===>	S2 -> PA0
					S3 -> PA1
					OUT-> PA2
					LED-> PA3
					S1,S2 -> VCC
 
 pH sensor ===>	P0 -> PA4
				D0 -> PA5
			
			
			
 * Created: 02/03/2017 11:17:08
 *  Author: Sandun Isuru Niraj
 */ 



#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include "adc.h"
#include "incousart.h"
#include "tcs3200.h"

int pHSensor();
int ColSensor();

uint32_t MeasureR();
uint32_t MeasureG();
uint32_t MeasureB();

int main(void)
{
	
	char pH1[5];
	char Col[5];
	usart_init();
	InitADC();
	InitTCS3200();

	
    while(1)
    {
        int pH2 = pHSensor();
		int Col2 = ColSensor();
		
		if(pH2>-2 || pH2<-12){
			PORTC = 0b00000001;
		}else{
			PORTC = 0b00000000;
		}			
		
		if(Col2<16){
			PORTB = 0b00000001;
		}else{
			PORTB = 0b00000000;
		}
        
		itoa(Col2,Col,10);
       
		usart_string_transmit(Col);
		usart_string_transmit("\n");
        _delay_ms(1000);
		
		
		
		
		
    }
}

int pHSensor(){
	
	int ph_ref;
	int ph_rel;
	float D0;
	float P0;
	uint16_t pH;
		DDRC = 0xff;
	
	ph_ref=ReadADC(4); //reading from D0
	ph_rel=ReadADC(5); //reading from P0
	D0 = 2.0 * (ph_ref * 5.0 / 1024);
	P0 = -1*((ph_rel*5/1024) - D0)/4.8262; 
	
	pH = ((-5038.8 * P0 / (273.15 + 37.0)) + 7.0);
		
		
		
	/*	char pH3[5];
		itoa(pH,pH3,10);
	 usart_string_transmit(pH3);
	 usart_string_transmit("\n");*/

	return pH;
}

int ColSensor(){
	
	uint8_t x=0;
	int8_t vx=1;
	DDRB = 0xff;
	uint32_t d;
	d=8000;
	
	uint32_t r,g,b;
	uint32_t _r,_g,_b;
	if(d==8000)
	{


		
		TCSLEDOn();

		r=MeasureR();
		g=MeasureG();
		b=MeasureB();

		TCSLEDOn();

		uint32_t smallest;

		if(r<b)
		{
			if(r<g)
			smallest=r;
			else
			smallest=g;
		}
		else
		{
			if(b<g)
			smallest=b;
			else
			smallest=g;
		}

		smallest=smallest/10;

		_r=r/smallest;
		_g=g/smallest;
		_b=b/smallest;
		
	}

	

	x+=vx;
	if(x==15 || x==0)
	vx=vx*-1;


	_delay_ms(50);
	
	
	
	
	
	return _b;
}

uint32_t MeasureR()
{
	TCSSelectRed();
	uint32_t r;

	_delay_ms(10);
	r=TCSMeasure();

	_delay_ms(10);
	r+=TCSMeasure();

	_delay_ms(10);
	r+=TCSMeasure();

	return r/3.3;

}

uint32_t MeasureG()
{
	TCSSelectGreen();
	uint32_t r;

	_delay_ms(10);
	r=TCSMeasure();

	_delay_ms(10);
	r+=TCSMeasure();

	_delay_ms(10);
	r+=TCSMeasure();

	return r/3;

}

uint32_t MeasureB()
{
	TCSSelectBlue();
	uint32_t r;

	_delay_ms(10);
	r=TCSMeasure();

	_delay_ms(10);
	r+=TCSMeasure();

	_delay_ms(10);
	r+=TCSMeasure();

	return r/4.2;

}