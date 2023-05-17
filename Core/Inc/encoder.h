/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef encoder_H
#define encoder_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

int readEncoder(GPIO_TypeDef* PORT_A, uint16_t PIN_A , GPIO_TypeDef* PORT_B, uint16_t PIN_B,int upperboundary, int lowerboundary);

#ifdef __cplusplus
}
#endif

#endif /* encoder_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
