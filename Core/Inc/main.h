/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f3xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Sense_W_Pin GPIO_PIN_0
#define Sense_W_GPIO_Port GPIOA
#define MCurrent_W_Pin GPIO_PIN_1
#define MCurrent_W_GPIO_Port GPIOA
#define Sense_V_Pin GPIO_PIN_2
#define Sense_V_GPIO_Port GPIOA
#define Sense_Temp_Top_Pin GPIO_PIN_3
#define Sense_Temp_Top_GPIO_Port GPIOA
#define MCurrent_V_Pin GPIO_PIN_6
#define MCurrent_V_GPIO_Port GPIOA
#define Sense_U_Pin GPIO_PIN_7
#define Sense_U_GPIO_Port GPIOA
#define MCurrent_U_Pin GPIO_PIN_0
#define MCurrent_U_GPIO_Port GPIOB
#define AN_IN_Pin GPIO_PIN_1
#define AN_IN_GPIO_Port GPIOB
#define HALL1_Pin GPIO_PIN_2
#define HALL1_GPIO_Port GPIOB
#define HALL2_Pin GPIO_PIN_10
#define HALL2_GPIO_Port GPIOB
#define HALL3_Pin GPIO_PIN_11
#define HALL3_GPIO_Port GPIOB
#define UHI_Pin GPIO_PIN_13
#define UHI_GPIO_Port GPIOB
#define VHI_Pin GPIO_PIN_14
#define VHI_GPIO_Port GPIOB
#define WHI_Pin GPIO_PIN_15
#define WHI_GPIO_Port GPIOB
#define ULI_Pin GPIO_PIN_8
#define ULI_GPIO_Port GPIOA
#define VLI_Pin GPIO_PIN_9
#define VLI_GPIO_Port GPIOA
#define WLI_Pin GPIO_PIN_10
#define WLI_GPIO_Port GPIOA
#define EncoderA_Pin GPIO_PIN_3
#define EncoderA_GPIO_Port GPIOB
#define EncoderB_Pin GPIO_PIN_4
#define EncoderB_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define Sector_TIM TIM6

#define false 0
#define true 1
#define ui8 unsigned char
#define bool _Bool
#define string char*
#define ui32 unsigned int
#define ui16 uint16_t
#define state_is(x)        ((ui32)state   == (ui32)(x))
#define event_occured(x)   ((ui32)event   == (ui32)(x))

#define kHz * 1000
#define MHz * 1000000
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
