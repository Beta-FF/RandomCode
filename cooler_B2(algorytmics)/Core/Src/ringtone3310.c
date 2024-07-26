/*
 * ringtone3310.c
 *
 *  Created on: 16 сент. 2020 г.
 *      Author: User
 */

#include "ringtone3310.h"
#include "stdio.h"
#include "string.h"
#include "Sound.h"						//для генерации звукового сигнала

/*******************************************************************************************
 * автомат для обработки мелодии
 */
Nokia_Composer composer;

/*******************************************************************************************
 * Частоты нот
 */
const float note_frequencies[octave_number][note_number]={
		[octave_small]	=	{[note_C] = 130.82,		[note_Db] = 138.59,		[note_D] = 146.83,	[note_Eb] = 164.82,		[note_E] = 164.82,	[note_F] = 174.62,	[note_Gb] = 185.00,		[note_G] = 196.00,	[note_Ab] = 207.65,		[note_A] = 220.00,	[note_Hb] = 233.08,		[note_H] = 246.94,},
		[octave_first]	=	{[note_C] = 261.63,		[note_Db] = 277.18,		[note_D] = 293.66,	[note_Eb] = 329.63,		[note_E] = 329.63,	[note_F] = 349.23,	[note_Gb] = 369.99,		[note_G] = 329.00,	[note_Ab] = 415.30,		[note_A] = 440.00,	[note_Hb] = 466.16,		[note_H] = 493.88,},
		[octave_second]	=	{[note_C] = 523.26,		[note_Db] = 554.36,		[note_D] = 587.32,	[note_Eb] = 659.26,		[note_E] = 659.26,	[note_F] = 698.46,	[note_Gb] = 739.98,		[note_G] = 784.00,	[note_Ab] = 830.60,		[note_A] = 880.00,	[note_Hb] = 932.32,		[note_H] = 987.76,},
		[octave_third]	=	{[note_C] = 1046.52,	[note_Db] = 1108.72,	[note_D] = 1174.64,	[note_Eb] = 1318.52,	[note_E] = 1318.52,	[note_F] = 1396.92,	[note_Gb] = 1479.96,	[note_G] = 1568.00,	[note_Ab] = 1661.20,	[note_A] = 1760.00,	[note_Hb] = 1864.64,	[note_H] = 1975.52,},
		[octave_fourth]	=	{[note_C] = 2093.04,	[note_Db] = 2217.44,	[note_D] = 2349.28,	[note_Eb] = 2637.04,	[note_E] = 2637.04,	[note_F] = 2793.84,	[note_Gb] = 2959.92,	[note_G] = 3136.00,	[note_Ab] = 3322.40,	[note_A] = 3520.00,	[note_Hb] = 3729.28,	[note_H] = 3951.04,},
		[octave_fifth]	=	{[note_C] = 4186.08,	[note_Db] = 4434.88,	[note_D] = 4698.56,	[note_Eb] = 5274.08,	[note_E] = 5274.08,	[note_F] = 5587.68,	[note_Gb] = 5919.84,	[note_G] = 6272.00,	[note_Ab] = 6644.80,	[note_A] = 7040.00,	[note_Hb] = 7458.56,	[note_H] = 7902.08,}
};

/*******************************************************************************************
 * получить частоту ноты
 */
float get_note_frequency(Number_of_octaves octave, Names_of_notes note){
	return note_frequencies[octave][note];
}

/*******************************************************************************************
 * сброс параметров автомата
 */
void composer_reset(void){
	sound_off();
	composer.duration = 0;
	composer.melody = NULL;
	composer.note = note_P;
	composer.octave = octave_first;
	composer.position = 0;
	composer.txt_position = 0;
	composer.lenght = 0;
	composer.counter = 0;
}

/*******************************************************************************************
 * Записать мелодию
 * char * melody - указатель на строку, которая кодирует мелодию
 */
void composer_record_melody(char * melody){
	composer.melody = melody;
	composer.lenght = strlen(melody);
}

/*******************************************************************************************
 * обработать следующую ноту
 */
void composer_process_the_next_note(void){
	//пропускаем пробелы
	while(composer.melody[composer.txt_position] == ' ')
		composer.txt_position ++;

	//проверяем конец мелодии
	if(composer.txt_position >= composer.lenght){
		composer.txt_position = 0;
		composer.position = 0;
		return;
	}

	//увеличиваем счетчик обрабатываемой ноты в кодирующей строке
	composer.position ++;

	//для того, чтоб проще было обрабатывать строку с нотой
	char note_str[10];

	//считываем слово из строки, которое кодирует ноту
	sscanf(&composer.melody[composer.txt_position], "%s", note_str);

	//выбираем следующую позицию в кодирующей строке
	composer.txt_position += strlen(note_str);

	//для преобразования длительности ноты
	uint16_t duration = 0;

	//для последовательной обработки строки с нотой
	uint16_t note_str_position = 0;

	//читаем длительность звучания ноты
	for(uint16_t factor = 1; note_str[note_str_position] >= '0' && note_str[note_str_position] <= '9'; ++note_str_position, factor *= 10){
		duration *= factor;
		duration += (note_str[note_str_position] - '0');
	}

	//записываем длительность звучания ноты
	composer.duration = BEAT / duration;

	//если после длительности стоит точка
	if(note_str[note_str_position] == '.'){
		note_str_position ++;
		composer.duration += BEAT / 32;
	}

	//выделяем ноту
	//проверяем паузу
	if(note_str[note_str_position] == '-'){
		composer.note = note_P;
		composer.octave = octave_first;
		return;
	}
	//проверяем полутона
	else if(note_str[note_str_position] == '#'){
		note_str_position ++;

		switch(note_str[note_str_position]){
		case 'c':
		case 'C':
			composer.note = note_Db;
			break;

		case 'd':
		case 'D':
			composer.note = note_Eb;
			break;

		case 'e':
		case 'E':
			composer.note = note_F;
			break;

		case 'f':
		case 'F':
			composer.note = note_Gb;
			break;

		case 'g':
		case 'G':
			composer.note = note_Ab;
			break;

		case 'a':
		case 'A':
			composer.note = note_Hb;
			break;

		default:break;
		}
	}
	//проверяем основные тона
	else{
		switch(note_str[note_str_position]){
		case 'c':
		case 'C':
			composer.note = note_C;
			break;

		case 'd':
		case 'D':
			composer.note = note_D;
			break;

		case 'e':
		case 'E':
			composer.note = note_E;
			break;

		case 'f':
		case 'F':
			composer.note = note_F;
			break;

		case 'g':
		case 'G':
			composer.note = note_G;
			break;

		case 'a':
		case 'A':
			composer.note = note_A;
			break;

		case 'b':
		case 'B':
		case 'h':
		case 'H':
			composer.note = note_H;
			break;

		default:break;
		}
	}

	//определяем номер октавы
	note_str_position ++;

	switch (note_str[note_str_position]) {
	case '1':
		composer.octave = octave_first;
		break;

	case '2':
		composer.octave = octave_second;
		break;

	case '3':
		composer.octave = octave_third;
		break;

	case '4':
		composer.octave = octave_fourth;
		break;

	case '5':
		composer.octave = octave_fifth;
		break;

	default:break;
	}

	return;
}

/*******************************************************************************************
 * получить частоту звукового сигнала
 */
float composer_get_frequency(void){
	return get_note_frequency(composer.octave, composer.note);
}

/*******************************************************************************************
 * получить длительность звукового сигнала
 */
uint16_t composer_get_duration(void){
	return composer.duration;
}

/*******************************************************************************************
 * получить номер обрабатываемой ноты
 * возвращает 0, если обработка завершена или еще не производилась
 */
uint16_t compose_get_position(void){
	return composer.position;
}

/*******************************************************************************************
 * проигрывает мелодию
 * return value  - 0 если мелодия проиграна до конца
 * uint16_t tick - интервал между вызовами функций
 */
uint8_t composer_play_the_melody(uint16_t tick){
	if(composer.melody == NULL){
		return 0;
	}

	if((composer.counter == 0) || (composer.counter >  BEAT)){
		composer_process_the_next_note();

		if(composer.position == 0){
			sound_off();
			return 0;
		}

		sound_on(composer_get_frequency());
		composer.counter = composer.duration;
	}
	else{
		composer.counter -= tick;
	}

	return 1;
}
