/*
 * LEDindicator.c
 *
 *  Created on: Sep 24, 2020
 *      Author: User
 */

#include "LEDindicator.h"

extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim10;

/******************************************************************************************************
 * обработка динамической индикации
 */
void indicator_dinamic(uint8_t* data, uint8_t max_indicator){
	static uint8_t number = 0;

	//выводим код одного индикатора по очереди
	indicator_decoder_disable();
	indicator_set_data_bus(data[number]);
	indicator_set_decoder(number);

	//выбираем номер следубющего индикатора
	if(++number >= max_indicator)
		number = 0;
}

/******************************************************************************************************
 * установиь линию сканирования дисплея на выводах дешифратора
 */
void indicator_set_decoder(uint8_t num){
	HAL_GPIO_WritePin(IC1_EN_GPIO_Port, IC1_EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IC1_BIT0_GPIO_Port, IC1_BIT0_Pin, num & (1<<0));
	HAL_GPIO_WritePin(IC1_BIT1_GPIO_Port, IC1_BIT1_Pin, num & (1<<1));
	HAL_GPIO_WritePin(IC1_BIT2_GPIO_Port, IC1_BIT2_Pin, num & (1<<2));
	HAL_GPIO_WritePin(IC1_EN_GPIO_Port, IC1_EN_Pin, GPIO_PIN_SET);
}

/******************************************************************************************************
 * выключить дешифратор
 */
void indicator_decoder_disable(void){
	HAL_GPIO_WritePin(IC1_EN_GPIO_Port, IC1_EN_Pin, GPIO_PIN_RESET);
}

/******************************************************************************************************
 * заполнить шину данных индикатора
 */
void indicator_set_data_bus(uint8_t data){
	HAL_GPIO_WritePin(UPDATE_GPIO_Port, UPDATE_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &data, 1, 1);
	HAL_GPIO_WritePin(UPDATE_GPIO_Port, UPDATE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(UPDATE_GPIO_Port, UPDATE_Pin, GPIO_PIN_RESET);
}

/******************************************************************************************************
 * настроить яркость
 */
void indicator_set_brightnes(uint8_t brightnes){
	//кастыль нужен, т.к. управление яркостью происходит из двух разных прерываний, если они произойдут в одном такте, то индикатор не выключится
	if(brightnes == 0) brightnes = 1;
	__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, brightnes);
}

/******************************************************************************************************
 * преобразовать число в семисегментный код
 */
/*const uint8_t segm_code[MAX_SEGM_CODE]={
		0x81, 0xF3, 0x49, 0x61, 0x33, 0x25, 0x05, 0xF1, 0x01, 0x21,
		0x11, 0x07, 0x8D, 0x43, 0x0D, 0x1D
};*/

const uint8_t segm_code[MAX_SEGM_CODE]={
		0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
		0x11, 0x07, 0x8D, 0x43, 0x0D, 0x1D
};


uint8_t indicator_get_segm_code(uint8_t data, uint8_t pixel){
	if(pixel)
		return segm_code[data] & ~(1<<0);
	else
		return segm_code[data];
}
