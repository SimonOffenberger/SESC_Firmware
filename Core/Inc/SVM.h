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
#ifndef SVM_H
#define SVM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define F_TIM (144 MHz)

#define PWM_Frequency (50 kHz)

#define PWM_TIM_ARR (uint16_t)(((uint32_t)(F_TIM/PWM_Frequency)/2)) 
#define PWM_TIM_ARR_half  (PWM_TIM_ARR/2)
	
#define PWM_TIM TIM1
#define CUR_AV1 PWM_TIM->CCR5
#define CUR_AV2 PWM_TIM->CCR6
#define PWM_U PWM_TIM->CCR3
#define PWM_V PWM_TIM->CCR2
#define PWM_W PWM_TIM->CCR1

#define set_U0()  	PWM_U=DUTY; PWM_V=0; 		PWM_W=0;
#define set_U60() 	PWM_U=DUTY; PWM_V=DUTY; PWM_W=0;
#define set_U120()  PWM_U=0; 		PWM_V=DUTY; PWM_W=0;
#define set_U180() 	PWM_U=0; 		PWM_V=DUTY; PWM_W=DUTY;
#define set_U240()  PWM_U=0; 		PWM_V=0; 		PWM_W=DUTY;
#define set_U300() 	PWM_U=DUTY; PWM_V=0; 		PWM_W=DUTY;

#define set3PhasePWM(U,V,W) PWM_U=U; PWM_V=V; 		PWM_W=W;

void setVoltageVector(int phase,int phaseshift, uint16_t setpoint);
void initSVM(uint16_t switching_frequency);

#ifdef __cplusplus
}
#endif

#endif /* uart_debug_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
