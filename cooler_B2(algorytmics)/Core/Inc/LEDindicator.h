/*
 * LEDindicator.h
 *
 *  Created on: Sep 24, 2020
 *      Author: User
 */

#ifndef INC_LEDINDICATOR_H_
#define INC_LEDINDICATOR_H_

#include "main.h"

//количество индикаторов на дисплее
#define MAX_INDICATOR 4
#define MAX_SEGM_CODE 16

/******************************************************************************************************
 * обработка динамической индикации
 */
void indicator_dinamic(uint8_t* data, uint8_t max_indicator);

/******************************************************************************************************
 * установиь линию сканирования дисплея на выводах дешифратора
 */
void indicator_set_decoder(uint8_t num);

/******************************************************************************************************
 * выключить дешифратор
 */
void indicator_decoder_disable(void);

/******************************************************************************************************
 * заполнить шину данных индикатора
 */
void indicator_set_data_bus(uint8_t data);

/******************************************************************************************************
 * настроить яркость
 */
void indicator_set_brightnes(uint8_t brightnes);

/******************************************************************************************************
 * преобразовать число в семисегментный код
 */
uint8_t indicator_get_segm_code(uint8_t data, uint8_t pixel);

#endif /* INC_LEDINDICATOR_H_ */
