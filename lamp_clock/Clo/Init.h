/*
Подпрограмма <<Инициализация>> всего


*/

void W_DOG(void)	// Запуск сторожевого таймера
{
	WDTCR |= ((1<<WDP2)|(1<<WDP1)|(1<<WDP0));
	WDTCR |= ((1<<WDTOE)|(1<<WDE));
}

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void adc_init(void) // Настройка АЦП
{
	ADMUX |= ((1<<REFS0)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)|(1<<REFS1));// input adc7, reference voltage is VCC
	ADCSRA |= ((1<<ADEN)|(1<<ADIE)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));//|(1<<ADPS0)); //division 128
}

void pin_init(void) // Настройка портов ввода/вывода
{
	// init Port A
	DDRA  |= 	((1<<DDA6)|(1<<DDA5)|(1<<DDA4)|(1<<DDA3)|(1<<DDA2)|(1<<DDA1)|(1<<DDA0));	// Port A настройка на выход кроме PD7
	PORTA	&= ~((1<<DDA6)|(1<<DDA5)|(1<<DDA4)|(1<<DDA3)|(1<<DDA2)|(1<<DDA1)|(1<<DDA0));
	
	// init Port B
	DDRB	|= 	((1<<DDB7)|(1<<DDB6)|(1<<DDB5)|(1<<DDB4)|(1<<DDB3)|(1<<DDB1)|(1<<DDB0)); // настройка на выход
	PORTB	&= ~((1<<DDB7)|(1<<DDB6)|(1<<DDB5)|(1<<DDB4)|(1<<DDB3)|(1<<DDB1)|(1<<DDB0));
	
	PORTB |= (1<<DDB2); // настройка на вход
	// init Port C
	DDRC	|= 	((1<<DDC7)|(1<<DDC6)); // настройка на выход
	PORTC	&= ~((1<<DDC7)|(1<<DDC6));
	
	// init Port D
	DDRD	|= 	((1<<DDD5)|(1<<DDD4)|(1<<DDD1)); // настройка на выход
	PORTD &= ~((1<<DDD5)|(1<<DDD4)|(1<<DDD1));
	
	PORTD |= ((1<<DDD3)|(1<<DDD2)); // настройка на вход
}

void Button_init(void)// Установка 1 на выводах B1 и B2
{
	B1_SET;
	B2_SET;
}

void timer0_init(void)
{
	TCCR0 	|= (1<<CS02)|(1<<WGM01);	//|(1<<CS00)); 	//Предделитель на 256, сброс при совпадении
	TIMSK 	|= (1<<OCIE0);	// разрешение прерывания по событию «совпадение»
	OCR0	= 192;			// при 192 300 герц
}

void timer1_init(void)
{
	TCCR1A 	|= (1<<COM1A1)|(1<<WGM11);
	TCCR1B 	|= (1<<WGM13) |(1<<WGM12)|(1<<CS10); // Без предделителя
	TCNT1 	= 0;
	ICR1 	= FREQPWM;
	OCR1A 	= 0;
}

void timer2_init (void)
{
	TCCR2	|= ((1<<CS22)|(1<<CS21)|(1<<CS20)); 		//Предделитель на 1024,  56 герц при переполнении
	TIMSK 	|= (1<<TOIE2);			// разрешение прерывания по событию «переполнение» таймера/счетчика T2

}

void Interrupt_init (void) // Настройка внешних прерываний
{
	GICR	|= 	((1<<INT2)|(1<<INT1)|(1<<INT0));	//	Разрешение прерываний*/
}

