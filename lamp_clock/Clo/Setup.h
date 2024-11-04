/*
������������ <<���������>>

	*/	



uint8_t		BCD_data		=	0;
uint8_t		Low				=	0;
uint8_t		High			=	0;
uint8_t		Tmp_Hours		=	0;
uint8_t		Tmp_Minutes		=	0;


void Decoder_DEC (uint8_t datas)	
{
	High	=	(datas%100)/10;
	Low		=	(datas%10)/1;
}

void Decoder_in_BCD (void)
{
	BCD_data = High<<4;
	BCD_data = (BCD_data+Low);
}	

void Correct_hours (void)
{
	if (Flag_key_Plus) 
		{	
			if (Tmp_Hours == 23) {Tmp_Hours = 0;}
					else {Tmp_Hours ++;}
					Flag_key_Plus = 0;
					Decoder_DEC (Tmp_Hours);
					number_5 = Low;
					number_6 = High;
		}
	if (Flag_key_Minus)
		{	
			if (Tmp_Hours == 0) {Tmp_Hours = 23;}
					else {Tmp_Hours --;}
					Flag_key_Minus = 0;
					Decoder_DEC (Tmp_Hours);
					number_5 = Low;
					number_6 = High;
		}		
	
}
void Correct_minutes (void)
{
	if (Flag_key_Plus)
		{
			if (Tmp_Minutes == 59) {Tmp_Minutes = 0;}
			else {Tmp_Minutes ++;}
			Flag_key_Plus = 0;
			Decoder_DEC (Tmp_Minutes);
			number_3 = Low;
			number_4 = High;
			
		}
	if (Flag_key_Minus)
		{
			if (Tmp_Minutes == 0) {Tmp_Minutes = 59;}
			else {Tmp_Minutes --;}
			Flag_key_Minus = 0;
			Decoder_DEC (Tmp_Minutes);
			number_3 = Low;
			number_4 = High;
			
		}
	
}

void Set_hr (void)	//	����� ������ ��� ��������� ������ <<����>>	
{	
	if (Flag_key_Plus) { Regime_setup = Set_minutes; Flag_key_Plus = 0; str_uart_send("Select Minutes\r\n"); }			// ������������ �� ������
		if (Flag_key_Minus)	{ Regime_setup = Set_seconds; Flag_key_Minus = 0; str_uart_send("Select Seconds\r\n"); }		// ������������ �� �������
			if (Flag_key_Enter)	{ 
					tim_blink_enable = 1; 
					Regime_setup = Tun_hours; 
					str_uart_send("Tuning Hours\r\n");
					Flag_key_Enter = 0;
					Tmp_Hours = ((number_6 * 10)+ number_5);
					_delay_ms(300); }	// ���� � ��������� �����
				if (Flag_key_Exit) { Regime = Start; Flag_key_Exit = 0;	}			// ������� � ����� <<Start>>
}

void Set_min (void)	//	����� ������ ��� ��������� ������ <<������>>	
{	
	if (Flag_key_Plus) { Regime_setup = Set_seconds; Flag_key_Plus = 0; str_uart_send("Select Seconds\r\n"); }			// ������������ �� �������
		if (Flag_key_Minus) { Regime_setup = Set_hours; Flag_key_Minus = 0; str_uart_send("Select Hours\r\n"); }		// ������������ �� ����
			if (Flag_key_Enter) { 
					tim_blink_enable = 1; 
					Regime_setup = Tun_minutes;
					str_uart_send("Tuning Minutes\r\n"); 
					Flag_key_Enter = 0;
					Tmp_Minutes = ((number_4*10)+number_3);
					_delay_ms(300); } // ���� � ��������� �����
				if (Flag_key_Exit) { Regime = Start; Flag_key_Exit = 0; tim_return_enable = 0; }			// ������� � ����� <<Start>>
}

void Set_sec (void) //	����� ������ ��� ��������� ������ <<�������>>	
{
	if (Flag_key_Plus) { Regime_setup = Set_hours; Flag_key_Plus = 0; str_uart_send("Select Hours\r\n"); }				// ������������ �� ����
		if (Flag_key_Minus) { Regime_setup = Set_minutes; Flag_key_Minus = 0; str_uart_send("Select Minutes\r\n"); }		// ������������ �� ������
			if (Flag_key_Enter) { Flag_key_Enter = 0; }			 // ����� ������
				if (Flag_key_Exit) { Regime = Start; Flag_key_Exit = 0; tim_return_enable = 0; }			// ������� � ����� <<Start>>
}

void Tun_hr (void)
{	//	=====================	���������� �������� ���������	================================================
	if (Count_blink > 20) { Enable_Seg_Hours = 0; Count_blink = 0; } //	�������� ������� (56 ��� �������� �������)
		else { Enable_Seg_Hours = 1; }
		// ====================	�����, ������� � ����� ����� ��������� <<Set>> 	===================================		
		if (Flag_key_Exit) { Regime_setup = Set_hours; tim_blink_enable = 0; Enable_Seg_Hours = 1; Flag_key_Exit = 0; _delay_ms(300); }	
			// ================	������ �������� � ���������� DS1307	===================================
			if (Flag_key_Enter) { Decoder_in_BCD (); Set_Hours_DS1307(BCD_data); Flag_key_Enter = 0; }
Correct_hours (); //	������������ ����������� ��������� �������
}

void Tun_min (void)
{	//	=====================	���������� �������� ���������	================================================
	if (Count_blink > 20) { Enable_Seg_Minutes = 0; Count_blink = 0; } //	�������� ������� (56 ��� �������� �������)
	else { Enable_Seg_Minutes = 1; }
	// ====================	�����, ������� � ����� ����� ��������� <<Set>> 	===================================
	if (Flag_key_Exit) { Regime_setup = Set_minutes; tim_blink_enable = 0; Enable_Seg_Minutes = 1; Flag_key_Exit = 0; _delay_ms(300); }
	// ================	������ �������� � ���������� DS1307	===================================
		if (Flag_key_Enter) { Decoder_in_BCD (); Set_Minutes_DS1307(BCD_data); Flag_key_Enter = 0; }
		Correct_minutes (); //	������������ ����������� ��������� �������
}

void Tun_sec (void)
{
	
}

void Setup (void)
{	
	_delay_ms(100);
	
		Decoder_Button_Setup ();

			switch (Regime_setup)
			{	
				case Set_hours		:	Set_hr (); break;
				case Set_minutes	:	Set_min (); break;
				case Set_seconds	:	Set_sec (); break;
				case Tun_hours		:	tim_blink_enable = 1; Tun_hr (); break;
				case Tun_minutes	:	tim_blink_enable = 1; Tun_min (); break;
				case Tun_seconds	:	tim_blink_enable = 1; Tun_sec (); break;
			
			}
	if (Count_return_setup > 6720) { tim_return_enable = 0; Regime = Start;} //	����� �� ������ �������� ����� 2 ������ 

}