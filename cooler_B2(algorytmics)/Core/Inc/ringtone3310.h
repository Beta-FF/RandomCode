/*
 * ringtone3310.h
 *
 *  Created on: 16 сент. 2020 г.
 *      Author: User
 */

#ifndef INC_RINGTONE3310_H_
#define INC_RINGTONE3310_H_

#include "main.h"

/*******************************************************************************************
 * длительность воспроизведения целой ноты
 */
#define BEAT 1600

/*******************************************************************************************
 * Название октав
 */
typedef enum{
	octave_small=0,
	octave_first,
	octave_second,
	octave_third,
	octave_fourth,
	octave_fifth,
	octave_number
} Number_of_octaves;

/*******************************************************************************************
 * Названия нот
 */
typedef enum{
	note_P = 0,
	note_C,
	note_Db,
	note_D,
	note_Eb,
	note_E,
	note_F,
	note_Gb,
	note_G,
	note_Ab,
	note_A,
	note_Hb,
	note_H,
	note_number
} Names_of_notes;

/*******************************************************************************************
 * автомат для обработки мелодии
 */
typedef struct {
	//номер октавы по текущей позиции
	Number_of_octaves	octave;

	//нота в текущей позиции
	Names_of_notes		note;

	//интервал звучания ноты
	uint16_t			duration;

	//позиция ноты в текстовой строке
	uint16_t			position;

	//номер обрабатываемого символа в строке с закодированной мелодией
	uint16_t			txt_position;

	//длина строки, которая кодирует мелодию
	uint16_t			lenght;

	//обратный счетчик для измерения интервалов
	uint16_t			counter;

	//строка с закодированной мелодией
	char 				*melody;
} Nokia_Composer;

/*******************************************************************************************
 * сброс параметров автомата
 */
void composer_reset(void);

/*******************************************************************************************
 * Записать мелодию
 * char * melody - указатель на строку, которая кодирует мелодию
 */
void composer_record_melody(char * melody);

/*******************************************************************************************
 * обработать следующую ноту
 */
void composer_process_the_next_note(void);

/*******************************************************************************************
 * проигрывает мелодию
 * return value  - 0 если мелодия проиграна до конца
 * uint16_t tick - интервал между вызовами функций
 */
uint8_t composer_play_the_melody(uint16_t tick);

/*******************************************************************************************
 * получить частоту звукового сигнала
 */
float composer_get_frequency(void);

/*******************************************************************************************
 * получить длительность звукового сигнала
 */
uint16_t composer_get_duration(void);

/*******************************************************************************************
 * получить номер обрабатываемой ноты
 * возвращает 0, если обработка завершена или еще не производилась
 */
uint16_t compose_get_position(void);

/*******************************************************************************************
 * получить частоту ноты
 */
float get_note_frequency(Number_of_octaves octave, Names_of_notes note);

#endif /* INC_RINGTONE3310_H_ */