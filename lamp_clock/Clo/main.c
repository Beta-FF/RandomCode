/*
 * Clo.c
 *
 * Created: 13.04.2017 22:11:27
 * Author : Неизвестных Александр Сергеевич
 */ 
#define F_CPU 	14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "TWI.h"
#include "DS1307.h"
#include "Func.h"
#include "LED_Setup.h"
#include "Work.h"
#include "Setup.h"
#include "Init.h"
#include "Int_ISR.h"
#include "Ringtone.h"
//#include <stdio.h>



int main(void)
{	
	
	
	
	pin_init();
	I2C_Init();
	DS1307_init();
	timer1_init();
	//timer0_init();
	//timer2_init();
	adc_init();
	USART_Init(MYUBRR);
	Button_init();
	Interrupt_init();
	W_DOG();
	LED_init ();
	sei(); // global on interrupt
	
	Demo ();
	//raynbow_ini();
	//=====================================================
Flag_lamp = 1;

	//==================================================================	
    while (1) 
    {	//play();
	
			wdt_reset ();
			
				switch (Regime)
				{
					case Start		:	work ();	break;
					case Turning_1	:	Setup ();	break;
					case LED_Setup	:	LED_Stp();	break;
				}	
			Check_regime ();
						
    }
}



