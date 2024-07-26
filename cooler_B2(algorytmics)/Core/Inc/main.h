/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define BTN3_Pin GPIO_PIN_0
#define BTN3_GPIO_Port GPIOC
#define BTN2_Pin GPIO_PIN_1
#define BTN2_GPIO_Port GPIOC
#define BTN4_Pin GPIO_PIN_3
#define BTN4_GPIO_Port GPIOC
#define VADJ1_Pin GPIO_PIN_0
#define VADJ1_GPIO_Port GPIOA
#define VADJ2_Pin GPIO_PIN_1
#define VADJ2_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define TEMP1_Pin GPIO_PIN_4
#define TEMP1_GPIO_Port GPIOA
#define GREEN2_Pin GPIO_PIN_5
#define GREEN2_GPIO_Port GPIOA
#define GREEN1_Pin GPIO_PIN_6
#define GREEN1_GPIO_Port GPIOA
#define YELL2_Pin GPIO_PIN_7
#define YELL2_GPIO_Port GPIOA
#define IC3_EN_Pin GPIO_PIN_4
#define IC3_EN_GPIO_Port GPIOC
#define IC1_BIT1_Pin GPIO_PIN_5
#define IC1_BIT1_GPIO_Port GPIOC
#define TEMP2_Pin GPIO_PIN_0
#define TEMP2_GPIO_Port GPIOB
#define IC3_BIT2_Pin GPIO_PIN_1
#define IC3_BIT2_GPIO_Port GPIOB
#define IC3_BIT1_Pin GPIO_PIN_2
#define IC3_BIT1_GPIO_Port GPIOB
#define BTN1_Pin GPIO_PIN_10
#define BTN1_GPIO_Port GPIOB
#define IC3_BIT0_Pin GPIO_PIN_12
#define IC3_BIT0_GPIO_Port GPIOB
#define UPDATE_Pin GPIO_PIN_14
#define UPDATE_GPIO_Port GPIOB
#define IC1_BIT0_Pin GPIO_PIN_6
#define IC1_BIT0_GPIO_Port GPIOC
#define RED2_Pin GPIO_PIN_7
#define RED2_GPIO_Port GPIOC
#define RTC_RST_Pin GPIO_PIN_8
#define RTC_RST_GPIO_Port GPIOC
#define SQW_Pin GPIO_PIN_9
#define SQW_GPIO_Port GPIOC
#define SPEAKER_Pin GPIO_PIN_8
#define SPEAKER_GPIO_Port GPIOA
#define RED1_Pin GPIO_PIN_9
#define RED1_GPIO_Port GPIOA
#define LINE1_Pin GPIO_PIN_10
#define LINE1_GPIO_Port GPIOA
#define IC1_EN_Pin GPIO_PIN_11
#define IC1_EN_GPIO_Port GPIOA
#define IC1_BIT2_Pin GPIO_PIN_12
#define IC1_BIT2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LINE2_Pin GPIO_PIN_3
#define LINE2_GPIO_Port GPIOB
#define LINE4_Pin GPIO_PIN_4
#define LINE4_GPIO_Port GPIOB
#define LINE3_Pin GPIO_PIN_5
#define LINE3_GPIO_Port GPIOB
#define YELL1_Pin GPIO_PIN_6
#define YELL1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
