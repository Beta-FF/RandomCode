/*
Алгоритм при котором происходит отображение времени, ожидается нажатие кнопок



*/




 uint8_t BCD_H, BCD_L, SA;
void Decoder_from_BCD (uint8_t SA)
{
	//static uint8_t SA;
	BCD_H = SA>>4;
	BCD_L = (SA &= 0x0F);
}


void Decoder_Button_Work (void)
{
	if (Button != None)
	{	Key_enable = 0;		//	Выключение скана кнопок
		_delay_ms(100);
		switch (Button)
		{
			case Minus :	{			}	break;
			//====================================================
			case Plus :		{
					B1_RESET;				// включение линии для опроса кнопки (Plus)
					_delay_ms(100);				// Антидребезг
					PIND_read = ~(PIND | 0b11111011);	// считываем бит PD2, через маску
					if (PIND_read)
					{		tim_enable = 1;					// Запуск таймера отсчета нажатия кнопки
						PIND_read = ~(PIND | 0b11111011);	// считываем бит PD2, через маску
						while (PIND_read)
						{
							if (Count_Reset_Ext_interrupt > 128)
							{	
								Regime = LED_Setup;
								tim_return_enable = 1;
								str_uart_send("Enter to LED_Setup\r\n"); // Передаем строку по USART
								_delay_ms(1000);
								str_uart_send("SET - Red_color\r\n"); // Передаем строку по USART
								PIND_read = 0;
								Decoder_Color_Red_in_BCD (Color_Red);
								Decoder_Color_Green_in_BCD (Color_Green);
								Decoder_Color_Blue_in_BCD (Color_Blue);
							}
							else
							{
								PIND_read = ~(PIND | 0b11111011);	// считываем бит PD2, через маску
							}
						}
					}
					
			}	break;
			//====================================================
			case Exit :		{
			}	break;
			//====================================================
			case Enter :
			{ 
					B2_RESET;				// включение линии для опроса кнопки (Enter)
					_delay_ms(100);				// Антидребезг
					PIND_read = ~(PIND | 0b11111011);	// считываем бит PD2, через маску
					if (PIND_read)
					{		tim_enable = 1;					// Запуск таймера отсчета нажатия кнопки
						PIND_read = ~(PIND | 0b11111011);	// считываем бит PD2, через маску
						while (PIND_read)
						{							
							if (Count_Reset_Ext_interrupt > 128)
							{					
								Regime = Turning_1;
								tim_return_enable = 1;
								str_uart_send("Enter to Setup\r\n"); // Передаем строку по USART
								_delay_ms(1000);
								PIND_read = 0;
								str_uart_send("Select Hours\r\n");
							}
							else
							{
								PIND_read = ~(PIND | 0b11111011);	// считываем бит PD2, через маску
							}
						}
					}
			}
		}
		B1_SET;
		B2_SET;
		_delay_ms(100);
		tim_enable = 0;
		Button = None;
		Key_enable = 1;						//	Включение скана кнопок
		GICR	|= 	((1<<INT1)|(1<<INT0));	//	Разрешение прерываний
	}
}

void work (void)
{	
	//=================		Чтение из текущего времени из RTC DS1307 	=============================================
	
	if ((Flag_Sec_DS1307))		/* Если есть сигнал на выходе SQWE тогда считываем текущее время из DS1307 */
	{
		static uint8_t hours, minutes, seconds;
			DS1307_GetTime(&hours,&minutes,&seconds);
				Decoder_from_BCD(seconds);
				number_1 = BCD_L;
				number_2 = BCD_H;
					Decoder_from_BCD(minutes);
					number_3 = BCD_L;
					number_4 = BCD_H;
						Decoder_from_BCD(hours);
						number_5 = BCD_L;
						number_6 = BCD_H;
							Flag_Sec_DS1307 = 0; // Сброс флага сигнала SQWE
							GICR	|= 	(1<<INT2);	//	Разрешение прерываний INT2
	//TX_printf(ADC/4.73);
	Flag_Sec = 1;
	//TX_printf(tmp);
	}
Decoder_Button_Work();

}



