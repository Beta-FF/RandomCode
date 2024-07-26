/*
 * keyboard.h
 *
 *  Created on: Sep 25, 2020
 *      Author: User
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

//интервал срабатывания кнопки
#define BUTTON_INTERVAL	5

/**********************************************************************************************
 * коды клавиатуры
 */
typedef enum {
	button_S1 = 0,
	button_S2,
	button_S3,
	button_S4,
	button_S5,
	button_S6,
	button_S7,
	button_S8,
	button_S9,
	button_S10,
	button_S11,
	button_S12,
	button_S13,
	button_S14,
	button_S15,
	button_S16,

	button_number,

	button_min = button_S1,
	button_max = button_S16
}Button_Name;

/**********************************************************************************************
 * параметры кнопки
 */
typedef struct {
	//линия сканирования, на которой висит кнопка
	uint8_t scan_line;

	//пин микроконтроллера, по которому опрашивается кнопка
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;

	//код кнопки
	char code;

	//счетчик для защиты от дребезга
	uint16_t counter;

	//граница дребезга контактов
	uint16_t protection;
} Keyboard_config;

/**********************************************************************************************
 * установка линии сканирования клавиатуры
 */
void button_set_decoder(uint8_t num);

/**********************************************************************************************
 * опрос клавиатуры
 */
char button_get_key(void);

/**********************************************************************************************
 * бработка клавиатуры
 */
void button_polling(void);

/**********************************************************************************************
 * установить новый код кнопки
 */
void button_set_new_key_code(Button_Name name, uint8_t code);

#endif /* INC_KEYBOARD_H_ */
