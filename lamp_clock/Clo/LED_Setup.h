/*
Режим настройки светодиодов
*/

#include "eeprom_san.h"
#include "light_ws2812.h"

#define MAXPIX 6
#define COLORLENGTH MAXPIX/2
#define FADE 256/COLORLENGTH
/*
#define		Green_colour	21
#define		Red_color		22
#define		Blue_color		23
char		Regime_LED	= Red_color;*/

struct cRGB colors[16];
struct cRGB led[MAXPIX];

/*
uint8_t	Color_Red	= 0;
uint8_t	Color_Green	= 0;
uint8_t	Color_Blue	= 0;*/
uint8_t i;

	
void Decoder_Color_Green_in_BCD (uint8_t w)
{
	number_4 = (w%100)/10;
	number_3 = (w%10)/1;
}

void Decoder_Color_Red_in_BCD (uint8_t w)
{
	number_6 = (w%100)/10;
	number_5 = (w%10)/1;
}
void Decoder_Color_Blue_in_BCD (uint8_t w)
{
	number_2 = (w%100)/10;
	number_1 = (w%10)/1;
}
		
void LED_init (void)
{	
	/*EEPROM_read (ADR_EEPROM_Green_color);
	Color_Green = EEDR;
	EEPROM_read (ADR_EEPROM_Red_color);
	Color_Red = EEDR;
	EEPROM_read (ADR_EEPROM_Blue_color);
	Color_Blue = EEDR;*/
	
	Color_Green = 16;
	
	Color_Red = 0;
	
	Color_Blue = 16;
	for(i=MAXPIX; i>0; i--)
	{
		led[i-1].r=Color_Red;led[i-1].g=Color_Green;led[i-1].b=Color_Blue;
	}
	ws2812_sendarray((uint8_t *)led,MAXPIX*3);
}

void RGB_Write (void)
{
	for(i=MAXPIX; i>0; i--)
	{
		led[i-1].r=Color_Red;led[i-1].g=Color_Green;led[i-1].b=Color_Blue;
	}
	ws2812_sendarray((uint8_t *)led,MAXPIX*3);
}
void Set_Green (void)
{
	Decoder_Color_Green_in_BCD (Color_Green);
	if (Flag_key_Plus) { Color_Green = Color_Green+8; Flag_key_Plus = 0; }
	if (Flag_key_Minus) { Color_Green = Color_Green-8; Flag_key_Minus = 0; }
	if (Flag_key_Enter)	{ Regime_LED = Red_color; Flag_key_Enter = 0; 
						str_uart_send("SET - Red_color\r\n");}
}

void Set_Red (void)
{
	Decoder_Color_Red_in_BCD (Color_Red);
	if (Flag_key_Plus) { Color_Red = Color_Red+8; Flag_key_Plus = 0; }
	if (Flag_key_Minus) { Color_Red = Color_Red-8; Flag_key_Minus = 0; }
	if (Flag_key_Enter)	{Regime_LED = Blue_color; Flag_key_Enter = 0; 
						str_uart_send("SET - Blue_color\r\n");}	
}

void Set_Blue (void)
{	
	Decoder_Color_Blue_in_BCD (Color_Blue);
	if (Flag_key_Plus) { Color_Blue = Color_Blue+8; Flag_key_Plus = 0; }
	if (Flag_key_Minus) { Color_Blue = Color_Blue-8; Flag_key_Minus = 0; }
	if (Flag_key_Enter)	{Regime_LED = Green_colour; Flag_key_Enter = 0; 
						str_uart_send("SET - Green_colour\r\n");}	
}

void LED_Stp (void)
{
	
	//=====================================================================================
	_delay_ms(100);
	for(i=MAXPIX; i>0; i--)
	{
		led[i-1].r=Color_Red;led[i-1].g=Color_Green;led[i-1].b=Color_Blue;
	}
	ws2812_sendarray((uint8_t *)led,MAXPIX*3);
	//=====================================================================
	Decoder_Button_Setup ();
	
	
	switch (Regime_LED)
	{
		case Green_colour	:	Set_Green ();	break;
		case Red_color		:	Set_Red ();		break;
		case Blue_color		:	Set_Blue ();	break;
		
	}
	if (Flag_key_Exit)
	{
		Flag_key_Exit = 0; tim_return_enable = 0;
		str_uart_send("Exit from LED_Setup\r\n");
		str_uart_send("Regime - Work\r\n");
		Regime = Start;
		EEPROM_Write (ADR_EEPROM_Green_color, Color_Green);
		EEPROM_Write (ADR_EEPROM_Red_color, Color_Red);
		EEPROM_Write (ADR_EEPROM_Blue_color, Color_Blue);
	}
	if (Count_return_setup > 6700) {tim_return_enable = 0; Regime = Start; str_uart_send("Exit to LED_Setup\r\n");} // Передаем строку по USART} //	выход из режима настроек через 2 минуты */

}
	//=====================================================================
	
	void raynbow_ini (void)
	{
	

	DDRB|=_BV(ws2812_pin);
	
	static uint8_t i;
	for(i=MAXPIX; i>0; i--)
	{
		led[i-1].r=0;led[i-1].g=0;led[i-1].b=0;
	}
	
	//Rainbowcolors
	colors[0].r=150; colors[0].g=150; colors[0].b=150;
	colors[1].r=255; colors[1].g=000; colors[1].b=000;//red
	colors[2].r=255; colors[2].g=100; colors[2].b=000;//orange
	colors[3].r=100; colors[3].g=255; colors[3].b=000;//yellow
	colors[4].r=000; colors[4].g=255; colors[4].b=000;//green
	colors[5].r=000; colors[5].g=100; colors[5].b=255;//light blue (türkis)
	colors[6].r=000; colors[6].g=000; colors[6].b=255;//blue
	colors[7].r=100; colors[7].g=000; colors[7].b=255;//violet	
	}
	void maini(void)
	{
		 
		
		
		//while(1)
		//{
			//shift all vallues by one led
			static uint8_t ui=0;
			for(ui=MAXPIX; ui>1; ui--)
			led[ui-1]=led[ui-2];
			//change colour when colourlength is reached
			if(ks>COLORLENGTH)
			{
				js++;
				if(js>7)
				{
					js=0;
				}

				ks=0;
			}
			ks++;
			//loop colouers
			
			//fade red
			if(led[0].r<(colors[js].r-FADE))
			led[0].r+=FADE;
			
			if(led[0].r>(colors[js].r+FADE))
			led[0].r-=FADE;

			if(led[0].g<(colors[js].g-FADE))
			led[0].g+=FADE;
			
			if(led[0].g>(colors[js].g+FADE))
			led[0].g-=FADE;

			if(led[0].b<(colors[js].b-FADE))
			led[0].b+=FADE;
			
			if(led[0].b>(colors[js].b+FADE))
			led[0].b-=FADE;

			//_delay_ms(10);
			ws2812_sendarray((uint8_t *)led,MAXPIX*3);
	//	}
	}
	
	
	
	
	
	
	
	
	



// =====================================================================================

 




