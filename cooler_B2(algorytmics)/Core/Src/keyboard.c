/*
 * keyboard.c
 *
 *  Created on: Sep 25, 2020
 *      Author: User
 */

#include "main.h"
#include "keyboard.h"

/**********************************************************************************************
 * параметры кнопки
 */
Keyboard_config keyboard_konfig[button_number] = {
[button_S1]		= {	.protection = BUTTON_INTERVAL,	.scan_line = 0,		.GPIOx = LINE1_GPIO_Port,	.GPIO_Pin = LINE1_Pin,	.code = 0,	},
[button_S2] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 0,		.GPIOx = LINE2_GPIO_Port,	.GPIO_Pin = LINE2_Pin,	.code = 1,	},
[button_S3] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 0,		.GPIOx = LINE3_GPIO_Port,	.GPIO_Pin = LINE3_Pin,	.code = 2,	},
[button_S13] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 0,		.GPIOx = LINE4_GPIO_Port,	.GPIO_Pin = LINE4_Pin,	.code = 3,	},
[button_S4] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 1,		.GPIOx = LINE1_GPIO_Port,	.GPIO_Pin = LINE1_Pin,	.code = 4,	},
[button_S5] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 1,		.GPIOx = LINE2_GPIO_Port,	.GPIO_Pin = LINE2_Pin,	.code = 5,	},
[button_S6] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 1,		.GPIOx = LINE3_GPIO_Port,	.GPIO_Pin = LINE3_Pin,	.code = 6,	},
[button_S14] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 1,		.GPIOx = LINE4_GPIO_Port,	.GPIO_Pin = LINE4_Pin,	.code = 7,	},
[button_S7] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 2,		.GPIOx = LINE1_GPIO_Port,	.GPIO_Pin = LINE1_Pin,	.code = 8,	},
[button_S8] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 2,		.GPIOx = LINE2_GPIO_Port,	.GPIO_Pin = LINE2_Pin,	.code = 9,	},
[button_S9] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 2,		.GPIOx = LINE3_GPIO_Port,	.GPIO_Pin = LINE3_Pin,	.code = 10,	},
[button_S15] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 2,		.GPIOx = LINE4_GPIO_Port,	.GPIO_Pin = LINE4_Pin,	.code = 11,	},

[button_S10] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 3,		.GPIOx = BTN1_GPIO_Port,	.GPIO_Pin = BTN1_Pin,	.code = 12,	},
[button_S11] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 3,		.GPIOx = BTN2_GPIO_Port,	.GPIO_Pin = BTN2_Pin,	.code = 13,	},
[button_S12] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 3,		.GPIOx = BTN3_GPIO_Port,	.GPIO_Pin = BTN3_Pin,	.code = 14,	},
[button_S16] 	= {	.protection = BUTTON_INTERVAL,	.scan_line = 3,		.GPIOx = BTN4_GPIO_Port,	.GPIO_Pin = BTN4_Pin,	.code = 15,	},
} ;

/**********************************************************************************************
 * для промежуточного хранения номера нажатой кнопки
 */
char button_temp_key = -1;

/**********************************************************************************************
 * установка линии сканирования клавиатуры
 */
void button_set_decoder(uint8_t num){
	if(num < 3){
		HAL_GPIO_WritePin(IC3_BIT0_GPIO_Port, IC3_BIT0_Pin, num & (1<<0));
		HAL_GPIO_WritePin(IC3_BIT1_GPIO_Port, IC3_BIT1_Pin, num & (1<<1));
		HAL_GPIO_WritePin(IC3_BIT2_GPIO_Port, IC3_BIT2_Pin, num & (1<<2));
		HAL_GPIO_WritePin(IC3_EN_GPIO_Port, IC3_EN_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(IC3_EN_GPIO_Port, IC3_EN_Pin, GPIO_PIN_RESET);
	}
}

/**********************************************************************************************
 * обработка клавиатуры
 */
void button_polling(void){
	//сканирование матричной клавиатуры
	for(Button_Name num = button_min; num <= button_max; ++num){
		//устанавливаем линию сканирования
		button_set_decoder(keyboard_konfig[num].scan_line);

		//опрашиваем порт, на котором висит кнопка
		//если кнопка нажата, то
		if(HAL_GPIO_ReadPin(keyboard_konfig[num].GPIOx, keyboard_konfig[num].GPIO_Pin) == GPIO_PIN_RESET){
			//считаем, сколько тактов нажата кнопка
			++keyboard_konfig[num].counter;
			//ограничиваем счетчик от переполнения, если кнопка очень долго нажата
			if(keyboard_konfig[num].counter > keyboard_konfig[num].protection)
				keyboard_konfig[num].counter = keyboard_konfig[num].protection;
		}
		//иначе, если кнопку отпустили
		else{
			//проверяем сколько тактов кнопка была нажата
			//если время нажатия соответствует заданному количеству тактов, то
			if(keyboard_konfig[num].counter >= keyboard_konfig[num].protection){
				//очищаем счетчик длительности нажатия кнопки
				keyboard_konfig[num].counter = 0;
				//и возвращаем код кнопки
				button_temp_key = keyboard_konfig[num].code;
				return;
			}
		}
	}

	//если ни чего не нажималось
	//button_temp_key = -1;
	return;
}

/**********************************************************************************************
 * опрос клавиатуры
 */
char button_get_key(void){
	char temp = button_temp_key;
	button_temp_key = -1;
	return temp;
}

/**********************************************************************************************
 * установить новый код кнопки
 */
void button_set_new_key_code(Button_Name name, uint8_t code){
	keyboard_konfig[name].code = code;
}

