//========================
#define F4 349
#define A4S 466
#define F5 698
#define C5 523
#define D5 587
#define D5S 622
#define A5S 932
#define LOOPS_PAUSE 1000 //between loops, ms
#define NOTES_PAUSE 1 //between notes, ms
#define SEQU_SIZE 19 //notes quantity
#define TEMPO 208 //quarter in minute
#define WHOLE_NOTE_DUR 240000/TEMPO //ms
uint16_t notes[]={F4, F4, F4, A4S, F5, D5S, D5, C5, A5S, F5,
D5S, D5, C5, A5S, F5, D5S, D5, D5S, C5 };
//whole note = 255
uint16_t beats[]={21, 21, 21, 128, 128, 21, 21, 21, 128, 64,
21, 21, 21, 128, 64, 21, 21, 21, 128};
uint16_t note_duration[SEQU_SIZE];
uint32_t signal_period[SEQU_SIZE];
uint32_t elapsed_time;
uint8_t i;

void VarDelay_us(uint32_t takt) {
	while(takt--) {
		_delay_us(1);
	}
}

int play(void) {
		//converting notes to signal period, us
	for (i = 0; i < SEQU_SIZE; i++) {
		signal_period[i] = 1000000 / notes[i];
	}
	//converting beats to notes duration, ms
	for (i = 0; i < SEQU_SIZE; i++) {
		note_duration[i] = (WHOLE_NOTE_DUR*beats[i])/255;
	}
	while(1) {
		for (i = 0; i < SEQU_SIZE; i++) {
			elapsed_time = 0;
			while (elapsed_time < 1000*((uint32_t)note_duration[i])) {
				PORTB |= 1<<7;
				VarDelay_us(signal_period[i]/2);
				PORTB &= ~(1<<7);
				VarDelay_us(signal_period[i]/2);
				elapsed_time += signal_period[i];
			}
			_delay_ms(NOTES_PAUSE);
		}
		_delay_ms(LOOPS_PAUSE);
	}
}