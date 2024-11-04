/*
Функционал
*/

#include <avr/wdt.h>
#define Freq_CPU 	 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>


#define BAUD 9600
#define MYUBRR Freq_CPU/16/BAUD-1
//Режимы опроса строк кнопок, какая строка в данный момент опрашивается
#define		B1		1
#define		B2		2
// Режимы кнопок, какая кнопка в данный момент нажата------------------
#define		None	1
#define		Plus	2
#define		Minus	3
#define		Enter	4
#define		Exit	5
// ============== Режимы работы ======================================
#define		Turning_1	1	
#define		Start		2
#define		LED_Setup	3
//===================	EEPROM	=================================================
uint16_t	ADR_EEPROM_Green_color		= 0;
uint16_t	ADR_EEPROM_Red_color		= 1;
uint16_t	ADR_EEPROM_Blue_color		= 2;
//========================	BMP280	==========================================
/*
int32_t temperature;
int32_t pressure;
int32_t altitude;
uint8_t error_code=0;

int32_t var1, var2;

*/
//====================================================================

//#define		DATA_RESET	PORTA &= (~((1<<DDA6)|(1<<DDA5)|(1<<DDA4))) // Сброс данных в ноль
//#define		DATA_RESET	PORTA &= (PINA|0b00001111) // Сброс данных в ноль

#define		L1_SET		PORTB |=(1<<DDB0)		// Установка бита 
#define		L1_RESET	PORTB &= (~(1<<DDB0))	//Сброс бита
#define		L2_SET		PORTB |=(1<<DDB1)	
#define		L2_RESET	PORTB &= (~(1<<DDB1))
#define		L3_SET		PORTD |=(1<<DDD4)
#define		L3_RESET	PORTD &= (~(1<<DDD4))
#define		L4_SET		PORTA |=(1<<DDA4)
#define		L4_RESET	PORTA &= (~(1<<DDA4))
#define		L5_SET		PORTA |=(1<<DDA5)
#define		L5_RESET	PORTA &= (~(1<<DDA5))
#define		L6_SET		PORTA |=(1<<DDA6) 
#define		L6_RESET	PORTA &= (~(1<<DDA6))
#define		B1_SET		PORTC |=(1<<DDC6)
#define		B1_RESET	PORTC &= (~(1<<DDC6))
#define		B2_SET		PORTC |=(1<<DDC7)
#define		B2_RESET	PORTC &= (~(1<<DDC7))
#define		Minus_SET	PORTB |=(1<<DDB5)
#define		Minus_RESET	PORTB &= (~(1<<DDB5))
#define		Plus_SET	PORTB |=(1<<DDB6)
#define		Plus_RESET	PORTB &= (~(1<<DDB6))
#define		Plus_minus_SET		PORTB |=((1<<DDB6)|(1<<DDB5))
#define		Plus_minus_RESET	PORTB &=(~((1<<DDB6)|(1<<DDB5)))

//===============	Переменные для LED	==========================
#define		Green_colour	21
#define		Red_color		22
#define		Blue_color		23
char		Regime_LED	= Red_color;
uint8_t	Color_Red	= 0;
uint8_t	Color_Green	= 0;
uint8_t	Color_Blue	= 0;
//=====	RGB
uint8_t js = 1;
uint8_t ks = 1;
char Flag_RGB = 0;

//=================
#define		Set_hours		13
#define		Set_minutes		14
#define		Set_seconds		15
#define		Tun_hours		16
#define		Tun_minutes		17
#define		Tun_seconds		18
//===========	переменные для бегущей линии	===============
char		Pixel_0		=	0;			
char		Pixel_1		=	0;
char		Pixel_2		=	0;
char		Pixel_3		=	0;
char		Pixel_4		=	0;
char		Pixel_5		=	0;
char		Pixel_6		=	0;
char		Pixel_7		=	0;
char		Pixel_8		=	0;
char		Pixel_9		=	0;
char		Pixel_10	=	0;
char		Pixel_11	=	0;

#define		Napruga	ADC/4.73

#define		FREQ	30000	//Частота преобразователя DC-DC 
#define		DUTY	0.2		// 20% наполнение преобразован
#define		Volt	170	//выходное напряжение Вольт
char			Line	=	1;
uint8_t			Count_Reset_Ext_interrupt	= 0;
uint16_t		Count_return_setup	= 0;
uint8_t			Count_blink			= 0;
char			Flag_PM				= 0 ;

 //===============================================================
 uint16_t TCNT1_1 = 0;
 uint16_t TCNT1_2 = 0;

//===============================================================

char			Flag_Read_I2S		= 0;
char			Flag_Sec_DS1307		= 0;
char			Flag_Sec			= 0;
char			Flag_lamp			= 0;
char			Flag_key_Minus		= 0;
char			Flag_key_Plus		= 0;
char			Flag_key_Enter		= 0;
char			Flag_key_Exit		= 0;
char			Enable_Seg_Hours	= 1;
char			Enable_Seg_Minutes	= 1;
char			Enable_Seg_Seconds	= 1;
char			String_button		= B1;
char			Button				= None;
char			Key_enable			= 1;
char			tim_enable			= 0;
char			tim_blink_enable	= 0;
char			tim_return_enable	= 0;
char			PIND_read			= 0;
char			Regime				= Start;
char			Regime_setup		= Set_hours;
uint8_t			number_1			= 0;
uint8_t			number_2			= 0;
uint8_t			number_3			= 0;
uint8_t			number_4			= 0;
uint8_t			number_5			= 0;
uint8_t			number_6			= 0;
uint8_t			DATA	=	0;
unsigned int	PWM_OUT	=	0;
unsigned int	FREQPWM	=	(Freq_CPU/FREQ);
unsigned int 	PWM_LIM		=	(Freq_CPU / FREQ*DUTY);
unsigned int 	VOLT_IN		=	Volt*4.73;	// 946 2.36В при 200В выходных

void USART_Transmit(unsigned char data ) // Передача по UART байта
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

void TX_printf (uint16_t a)	// Передача по UART 16 разрядного значения в кодировке ASCII
{	
		// value by the numbers
	char first_number	= 0;
	char second_number	= 0;
	char third_number	= 0;
	char fourth_number	= 0;

	// bit calculation
	first_number	= (a%10000)/1000;
	second_number	= (a%1000)/100;
	third_number	= (a%100)/10;
	fourth_number	= (a%10)/1;
		
	for ( char i=4;i;i--)// UART write cycle
	{
		switch (i)
		{
			case 4 :	{USART_Transmit(first_number+48);	}	break;
			case 3 :	{USART_Transmit(second_number+48);	}	break;
			case 2 :	{USART_Transmit(third_number+48);	}	break;
			case 1 :	{USART_Transmit(fourth_number+48);	}	break;
		}	
	}
	USART_Transmit (13);
}

void Decoder_Button_Setup (void) // Определение нажатой кнопки
{
	if (Button != None)
	{	Key_enable = 0;		//	Выключение скана кнопок
		_delay_ms(200);		//	Антидребезг
		switch (Button)
		{	//============== Установка флагов при нажатии кнопок	==========================
			case Minus :	{	Flag_key_Minus = 1;	}	break;
			case Plus :		{	Flag_key_Plus = 1;	}	break;
			case Exit :		{	Flag_key_Exit = 1;	}	break;
			case Enter :	{	Flag_key_Enter = 1;	}	break;
		}
		
	}
	Button = None;
	Key_enable = 1;						//	Включение скана кнопок
	GICR	|= 	((1<<INT1)|(1<<INT0));	//	Разрешение прерываний
}

void str_uart_send(char *string) // Функция передачи строки по USART
{
	while(*string != '\0')
	{
		USART_Transmit(*string);
		string++;
	}
}

void Check_regime (void)
{
		if (Regime == Turning_1 && Regime_setup == Set_seconds)  {Flag_PM = 1;}
		else	if (Regime == LED_Setup && Regime_LED == Blue_color) {Flag_PM = 1;}
		else  	{Flag_PM = 0;}
}

void Ind_Lamp (void)	// индикация ламп
{	
	static uint8_t lamp = 6;
	switch (lamp)
	{	//================	Сукунды	===============================
		case 6 :	{L6_RESET;
			if (Regime == Turning_1 && Regime_setup == Set_seconds)  {Plus_SET;}
			else	if (Regime == LED_Setup && Regime_LED == Blue_color) {Plus_minus_SET;}
			else  	{Plus_minus_RESET;}
				//========	Бегущая линия начало	================
				if (Regime == Start && Pixel_0 == 1)  {Plus_SET;}
				if (Regime == Start && Pixel_1 == 1)  {Minus_SET;}	
				//========	Бегущая линия конец	================
			PORTA = number_1;
			L1_SET;
			lamp = 5;
		}	break;
		case 5 :	{L1_RESET;
			if (Regime == Turning_1 && Regime_setup == Set_seconds)  {Minus_SET; Plus_RESET;}
			else	if (Regime == LED_Setup && Regime_LED == Blue_color) {Plus_minus_SET;}
			else  	{Plus_minus_RESET;}
				//========	Бегущая линия начало	================
				if (Regime == Start && Pixel_2 == 1)  {Plus_SET;}
				if (Regime == Start && Pixel_3 == 1)  {Minus_SET;}
				//========	Бегущая линия конец	================
			PORTA = number_2;
			L2_SET;
			lamp = 4;
		}	break;
		//================	Минуты	===============================
		case 4 :	{L2_RESET;
			if (Regime == Turning_1 && Regime_setup == Set_minutes)  {Plus_SET;}
			else	if (Regime == LED_Setup && Regime_LED == Green_colour) {Plus_minus_SET;}
			else  	{Plus_minus_RESET;}
				//========	Бегущая линия начало	================
				if (Regime == Start && Pixel_4 == 1)  {Plus_SET;}
				if (Regime == Start && Pixel_5 == 1)  {Minus_SET;}
				//========	Бегущая линия конец	================
			if (Enable_Seg_Minutes) {PORTA = number_3; L3_SET;}
			lamp = 3;
		}	break;
		case 3 :	{L3_RESET;
			if (Regime == Turning_1 && Regime_setup == Set_minutes)  {Minus_SET; Plus_RESET;}
			else	if (Regime == LED_Setup && Regime_LED == Green_colour) {Plus_minus_SET;}
			else  	{Plus_minus_RESET;}
				//========	Бегущая линия начало	================
				if (Regime == Start && Pixel_6 == 1)  {Plus_SET;}
				if (Regime == Start && Pixel_7 == 1)  {Minus_SET;}
				//========	Бегущая линия конец	================
			if (Enable_Seg_Minutes) {PORTA = number_4; L4_SET;}
			lamp = 2;
		}	break;
		//================	Часы	===============================
		case 2 :	{L4_RESET;
			if (Regime == Turning_1 && Regime_setup == Set_hours)  {Plus_SET;}
			else	if (Regime == LED_Setup && Regime_LED == Red_color) {Plus_minus_SET;}
			else  	{Plus_minus_RESET;}
				//========	Бегущая линия начало	================
				if (Regime == Start && Pixel_8 == 1)  {Plus_SET;}
				if (Regime == Start && Pixel_9 == 1)  {Minus_SET;}
				//========	Бегущая линия конец	================
			if (Enable_Seg_Hours) {PORTA = number_5; L5_SET;}
			lamp = 1;
		}	break;
		case 1 :	{L5_RESET;
			if (Regime == Turning_1 && Regime_setup == Set_hours)  {Minus_SET; Plus_RESET;}
			else	if (Regime == LED_Setup && Regime_LED == Red_color) {Plus_minus_SET;}
			else  	{Plus_minus_RESET;}
				//========	Бегущая линия начало	================
				if (Regime == Start && Pixel_10 == 1)  {Plus_SET;}
				if (Regime == Start && Pixel_11 == 1)  {Minus_SET;}
				//========	Бегущая линия конец	================
			if (Enable_Seg_Hours) {PORTA = number_6; L6_SET;}
			lamp = 6;
		}	break;
	}
}

void Swich_key (void)	// Опрос кнопок 56 Гц
{	
	if (Key_enable)
	{	switch (Line)
		{
			case 1 :	{	B2_SET;		//	Выключение линии с кнопками Enter и Exit
				String_button = B1;		//	Установка флага режима В1
				B1_RESET;				//	Включение линии с кнопками плюс (+) и минус (-)
				Line = 2;
			}	break;
			
			case 2 :	{	B1_SET;				//	Выключение линии с кнопками плюс (+) и минус (-)
				String_button = B2;		//	Установка флага режима В2
				B2_RESET;				//	Включение линии с кнопками Enter и Exit
				Line = 1;
			}	break;
		}
	}
	//==================	Таймер для кнопок	=====================================================
	if (tim_enable) {Count_Reset_Ext_interrupt ++;}	// +1 при кахдом переполнении таймера
	else {Count_Reset_Ext_interrupt = 0;}
	//==================	Таймер для возврата из настроек	==========================================
	if (tim_return_enable) {Count_return_setup ++;}	// +1 при кахдом переполнении таймера
	else {Count_return_setup = 0;}
	//==================	Таймер для <<Blink>>	==========================================
	if (tim_blink_enable) {Count_blink ++;}
	else {Count_blink = 0;}
}

void swich_pxl (void)	// Обработка бегущей линии
{
	static	uint8_t Pixel;
		//if (Flag_Sec) {Pixel = 0; Flag_Sec = 0;} // Синхронизация бегуще линии к изменению секунд	
			if (Regime == Start)				// Если режим работа, то включаем переключения
		{
			switch (Pixel)
			{	//============ <<<<<<< =============================
				case 0 :	{Pixel_0 = 1; Pixel_1 = 0; Pixel ++;}	break;
				case 1 :	{Pixel_1 = 1; Pixel_0 = 0; Pixel ++;}	break;
				case 2 :	{Pixel_2 = 1; Pixel_1 = 0; Pixel ++;}	break;
				case 3 :	{Pixel_3 = 1; Pixel_2 = 0; Pixel ++;}	break;
				case 4 :	{Pixel_4 = 1; Pixel_3 = 0; Pixel ++;}	break;
				case 5 :	{Pixel_5 = 1; Pixel_4 = 0; Pixel ++;}	break;
				case 6 :	{Pixel_6 = 1; Pixel_5 = 0; Pixel ++;}	break;
				case 7 :	{Pixel_7 = 1; Pixel_6 = 0; Pixel ++;}	break;	
				case 8 :	{Pixel_8 = 1; Pixel_7 = 0; Pixel ++;}	break;
				case 9 :	{Pixel_9 = 1; Pixel_8 = 0; Pixel ++;}	break;
				case 10 :	{Pixel_10 = 1; Pixel_9 = 0; Pixel ++;}	break;
				case 11 :	{Pixel_11 = 1; Pixel_10 = 0; Pixel ++;}	break;	
				//========	>>>>>>>>>>>>>>	==========================
				case 12 :	{Pixel_10 = 1; Pixel_11 = 0; Pixel ++;}	break;
				case 13 :	{Pixel_9 = 1; Pixel_10 = 0; Pixel ++;}	break;
				case 14 :	{Pixel_8 = 1; Pixel_9 = 0; Pixel ++;}	break;
				case 15 :	{Pixel_7 = 1; Pixel_8 = 0; Pixel ++;}	break;
				case 16 :	{Pixel_6 = 1; Pixel_7 = 0; Pixel ++;}	break;
				case 17 :	{Pixel_5 = 1; Pixel_6 = 0; Pixel ++;}	break;
				case 18 :	{Pixel_4 = 1; Pixel_5 = 0; Pixel ++;}	break;
				case 19 :	{Pixel_3 = 1; Pixel_4 = 0; Pixel ++;}	break;
				case 20 :	{Pixel_2 = 1; Pixel_3 = 0; Pixel ++;}	break;
				case 21 :	{Pixel_1 = 1; Pixel_2 = 0; Pixel = 0;}	break;																
			}
		}
}

void Demo ()
{	_delay_ms(100);
	str_uart_send("pin_init:	Complite\r\n"); // Передаем строку по USART
	_delay_ms(100);
	str_uart_send("I2C_Init:	Complite\r\n"); // Передаем строку по USART
	_delay_ms(100);
	str_uart_send("DS1307_init:	Complite\r\n"); // Передаем строку по USART
	_delay_ms(100);
	str_uart_send("timer1_init:	Complite\r\n"); // Передаем строку по USART
	_delay_ms(100);
	str_uart_send("adc_init:	Complite\r\n"); // Передаем строку по USART
	_delay_ms(100);
	str_uart_send("USART_Init:	Complite\r\n"); // Передаем строку по USART
	
	str_uart_send("Button_init:	Complite\r\n"); // Передаем строку по USART
	_delay_ms(100);
	str_uart_send("Interrupt_init:	Complite\r\n"); // Передаем строку по USART
	_delay_ms(100);
	str_uart_send("W_DOG_init:	Complite\r\n"); // Передаем строку по USART
	_delay_ms(100);
	str_uart_send("LED_init:	Complite\r\n"); // Передаем строку по USART
}