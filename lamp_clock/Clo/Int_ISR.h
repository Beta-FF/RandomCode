/*
���������� ����������, ��� ������ �����/�����, ��� ���� ���������
INT0				������� ���������� 0						1 $0002
INT1				������� ���������� 1						2 $0004
TIMER2				COMP ���������� �������/�������� �2			3 $0006
TIMER2				OVF ������������ �������/�������� �2		4 $0008
TIMER1				CAPT ������ �������/�������� �1				5 $000A
TIMER1				COMPA ���������� ��� �������/�������� �1	6 $000C
TIMER1				COMPB ���������� �» �������/�������� �1	7 $000E
TIMER1				OVF ������������ �������/�������� �1		8 $0010
TIMER0				OVF ������������ �������/�������� �0		9 $0012
SPI, STC			�������� �� SPI ���������					10 $0014
USART, RXC USART,	����� ��������								11 $0016
USART, UDRE			������� ������ USART ����					12 $0018
USART, TXC USART,	�������� ���������							13 $001A
ADC					�������������� ��� ���������				14 $001C
EE_RDY EEPROM,		������										15 $001E
ANA_COMP			���������� ����������						16 $0020
TWI					���������� �� ������ TWI					17 $0022
INT2				������� ���������� 2						18 $0024
TIMER0 COMP			���������� �������/�������� �0				19 $0026
SPM_RDY				���������� SPM								20 $0028
*/

ISR(ADC_vect)
{
	wdt_reset ();
	//=====================	���	8862 �� ===========================================
		static uint16_t  RR;		
			RR = (ADC);							// ��������� �������� �� ���
				if (RR>VOLT_IN) {OCR1A = 0;}	// ���� ���������� ���� ��������������, �� ��������� ���
					else {OCR1A = PWM_LIM;}		// �����, �������� ���
	//=====================	���������	===================================
		static uint8_t Freq_swich_ind;		// ������� ������������ ���� ~300 ��, ~50 �� �� ����� (��� Freq_swich_ind = 30)
			Freq_swich_ind ++;					
				if (Freq_swich_ind >= 30) {Freq_swich_ind = 0; Ind_Lamp ();}  // 30 ��� 14 ��� ������������ �� ��������� �����
	//=====================	����� ������	===================================			
		static uint8_t Freq_swich_key;		// ������� ������������ ������ ~50 �� (��� Freq_swich_key = 177)	
			Freq_swich_key ++;
				if (Freq_swich_key >= 177) {Freq_swich_key = 0; Swich_key ();}	// 177 ��� 14 ���	
	//=====================	������ �����	==========================================
		static	uint16_t Freq_swich_run;		// ������� ������������ ��������
			Freq_swich_run ++;
				if ( Flag_Sec/*Freq_swich_run >= 8862*/) {/*Freq_swich_run = 0;*/Flag_Sec=0; swich_pxl ();}//403 ��� 14 ���
	//===================== RGB		=================================
		static uint16_t Freq_RGB;
			Freq_RGB++;
				if (Freq_RGB>=4444) {Flag_RGB =1;}
				
}


ISR	(INT0_vect)				// ���������� ���������� ��� ������� ������
{	
	GICR &= ~((1<<INT1)|(1<<INT0));						//	������ ���������� �� ������
		Count_return_setup = 0;							// ���������� ������� ��������
			if (String_button == B2) { Button = Enter;}	// ����� ENTER
				else{Button = Plus;}					// ����� ����
}

ISR (INT1_vect)				// ���������� ���������� ��� ������� ������
{
	GICR &= ~((1<<INT1)|(1<<INT0));					//	������ ���������� �� ������
		Count_return_setup = 0;							//	���������� ������� ��������
			if (String_button == B1) {Button = Minus;}	// ����� �����
				else {Button = Exit;}					// ����� EXIT
}

ISR (INT2_vect)
{
	GICR	&= 	~((1<<INT2));	//	������ ���������� INT2
	Flag_Sec_DS1307 = 1;
	
	//Flag_Sek_DS1307_1 = 1;
	//=====================	 ������� - ������� ������������ ���������� � �������	============
	//	static uint16_t FQ;
	//	FQ++;
	//	if (Flag_Sek_DS1307_1) {tmp = FQ; FQ =0; Flag_Sek_DS1307_1 = 0;}
}