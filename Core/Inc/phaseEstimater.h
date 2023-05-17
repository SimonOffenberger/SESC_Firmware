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
#ifndef phaseEsitmater_H
#define phaseEsitmater_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "global_var.h"
#define SectorTIM htim6

#define SectorA 4
#define SectorB 6
#define SectorC 2
#define SectorD 3
#define SectorE 1
#define SectorF 5

void estimate_rotor_phase(motor_TypeDef* motor);
void init_phase_estimater();
uint16_t initialize_Hallsensor_phase_estimator(uint16_t setpoint);

#ifdef __cplusplus
}
#endif

#endif /* phaseEsitmater_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
