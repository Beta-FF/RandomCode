/*
 * Sound.c
 *
 *  Created on: 15 сент. 2020 г.
 *      Author: User
 */

#include "Sound.h"

extern TIM_HandleTypeDef htim1;


/*******************************************************************************************
 * Выключить звук
 */
void sound_off(void){
	HAL_TIM_OC_Stop(&htim1, TIM_CHANNEL_1);
}

/*******************************************************************************************
 * Включить звук
 */
void sound_on(float frequency){
	if(frequency !=0){
		TIM1->ARR = (1.0 / (double)frequency) * 1000000;
		HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1);
	}
	else{
		HAL_TIM_OC_Stop(&htim1, TIM_CHANNEL_1);
	}
}
