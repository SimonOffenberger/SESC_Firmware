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
#ifndef global_var_H
#define global_var_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


typedef struct{
	int DQCurrenst[2];
	int AlphaBetaCurrents[2];
	int32_t PhaseCurrents[3];
	uint16_t ADC_VAL_PhaseCurrents[3];
}currents_typedef;	
	
typedef struct{
	uint16_t rotorphase;
	uint16_t corr_rotor_phase;
	currents_typedef MotorCurrents;
}motor_TypeDef;	
	

#define U 0
#define V 1
#define W 2

#define ALPHA 0
#define BETA 0

#define D 0
#define Q 1

#define Index_LEG_U_AV1 0
#define Index_LEG_V_AV1 1
#define Index_LEG_W_AV1 2
#define Index_LEG_U_AV2 3
#define Index_LEG_V_AV2 4
#define Index_LEG_W_AV2 5

extern motor_TypeDef Motor;

#ifdef __cplusplus
}
#endif

#endif /* global_var_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
