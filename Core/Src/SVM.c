/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "SVM.h"
#include "tim.h"
#include "adc.h"
#include "global_var.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>




static uint16_t CCR_U=0;
static uint16_t CCR_V=0;
static uint16_t CCR_W=0;
static uint16_t CCR_CURR_AV1=0;
static uint16_t CCR_CURR_AV2=0;
static uint16_t current_sector=0;



const uint16_t SPWM[180] = {
	
0		,
30      ,
61      ,
91      ,
122     ,
152     ,
183     ,
213     ,
243     ,
273     ,
304     ,
334     ,
364     ,
394     ,
424     ,
454     ,
483     ,
513     ,
542     ,
572     ,
601     ,
630     ,
659     ,
688     ,
717     ,
745     ,
774     ,
802     ,
830     ,
858     ,
886     ,
894     ,
902     ,
909     ,
917     ,
924     ,
931     ,
938     ,
944     ,
951     ,
957     ,
963     ,
968     ,
974     ,
979     ,
983     ,
988     ,
992     ,
997     ,
1001    ,
1004    ,
1007    ,
1010    ,
1013    ,
1015    ,
1018    ,
1020    ,
1021    ,
1022    ,
1023    ,
1024    ,
1024    ,
1024    ,
1024    ,
1024    ,
1022    ,
1021    ,
1020    ,
1018    ,
1015    ,
1013    ,
1010    ,
1007    ,
1003    ,
999     ,
995     ,
990     ,
985     ,
980     ,
974     ,
968     ,
961     ,
954     ,
948     ,
940     ,
932     ,
924     ,
915     ,
906     ,
897     ,
887     ,
897     ,
906     ,
915     ,
924     ,
932     ,
940     ,
948     ,
954     ,
961     ,
968     ,
974     ,
980     ,
985     ,
990     ,
995     ,
999     ,
1003    ,
1007    ,
1010    ,
1013    ,
1015    ,
1018    ,
1020    ,
1021    ,
1022    ,
1024    ,
1024    ,
1024    ,
1024    ,
1024    ,
1023    ,
1022    ,
1021    ,
1020    ,
1018    ,
1015    ,
1013    ,
1010    ,
1007    ,
1004    ,
1001    ,
997     ,
992     ,
988     ,
983     ,
979     ,
974     ,
968     ,
963     ,
957     ,
951     ,
944     ,
938     ,
931     ,
924     ,
917     ,
909     ,
902     ,
894     ,
886     ,
858     ,
830     ,
802     ,
774     ,
745     ,
717     ,
688     ,
659     ,
630     ,
601     ,
572     ,
542     ,
513     ,
483     ,
454     ,
424     ,
394     ,
364     ,
334     ,
304     ,
273     ,
243     ,
213     ,
183     ,
152     ,
122     ,
91      ,
61      ,
30

};


void initSVM(uint16_t switching_frequency){
	
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn,0,0);
	__HAL_TIM_ENABLE_IT(&htim1,TIM_IT_UPDATE);

	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1); // W
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2); // V
	HAL_TIM_PWM_Start_IT(&htim1,TIM_CHANNEL_3); // U
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_5);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_6);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);


	PWM_TIM->BDTR&= ~(255);
	PWM_TIM->BDTR|= 128;
	PWM_TIM->ARR=PWM_TIM_ARR;

	__enable_irq();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
	PWM_U=CCR_U;
	PWM_V=CCR_V;
	PWM_W=CCR_W;
	
	Motor.MotorCurrents.PhaseCurrents[0] = (((Motor.MotorCurrents.ADC_VAL_PhaseCurrents[0]-1970)*110000)>>12);
	Motor.MotorCurrents.PhaseCurrents[1] = (((Motor.MotorCurrents.ADC_VAL_PhaseCurrents[1]-1970)*110000)>>12);
	Motor.MotorCurrents.PhaseCurrents[2] = (((Motor.MotorCurrents.ADC_VAL_PhaseCurrents[2]-1970)*110000)>>12);

	transform_to_Clark_domain(Motor.MotorCurrents.PhaseCurrents, Motor.MotorCurrents.AlphaBetaCurrents);
	transform_to_Park_domain(Motor.MotorCurrents.AlphaBetaCurrents, Motor.MotorCurrents.DQCurrenst, Motor.rotorphase);
}


// setpoint 0..100% => 0..256

void setVoltageVector(int phase,int phaseshift, uint16_t setpoint){
	phase=phase+phaseshift;
	
	uint16_t active_vectors_time= PWM_TIM_ARR*setpoint>>8;
	uint16_t inactive_vector_time= PWM_TIM_ARR-active_vectors_time;
	
	uint16_t active_vector1_time=0;
	uint16_t active_vector2_time=0;
	
	uint16_t CCR_AV1=0;
	uint16_t CCR_AV2=0;
	uint16_t CCR_IAV=0;
	
	if(phase>=360) phase=phase-360;
	if(phase<0) 	phase=phase+360;
	
	current_sector=(uint16_t)phase/60;
	
//	switch(current_sector){
//		case(0):										// phase 0-59�
//			CCR_AV1= inactive_vector_time>>1;									// start of active vector 1
//			active_vector2_time= ((active_vectors_time * phase)/60);
//			active_vector1_time= active_vectors_time-active_vector2_time;
//			CCR_CURR_AV1=	CCR_AV1+(active_vector1_time>>1);		// middle of active vector 1 to sample current
//			CCR_AV2= active_vector1_time + CCR_AV1;							// start of active vector 2
//			CCR_CURR_AV2 =CCR_AV2 + (active_vector2_time>>1);	// middle of active vector 2 to sample current
//			CCR_IAV = active_vector2_time+ CCR_AV2;
//			
//			CCR_U = CCR_AV1;
//			CCR_V = CCR_AV2;
//			CCR_W = CCR_IAV;
//			break;
//		case(1):										// phase 60-119�
//			CCR_AV1= inactive_vector_time>>1;									// start of active vector 1
//			active_vector2_time= ((active_vectors_time * (phase-60))/60);
//			active_vector1_time= active_vectors_time-active_vector2_time;
//			CCR_CURR_AV1=	CCR_AV1+(active_vector2_time>>1);		// middle of active vector 1 to sample current
//			CCR_AV2= active_vector2_time + CCR_AV1;							// start of active vector 2
//			CCR_CURR_AV2 =CCR_AV2 + (active_vector1_time>>1);	// middle of active vector 2 to sample current
//			CCR_IAV = active_vector1_time+ CCR_AV2;
//			
//			CCR_U = CCR_AV2;
//			CCR_V = CCR_AV1;
//			CCR_W = CCR_IAV;
//			break;		
//		case(2):										// phase 120-179�
//			CCR_AV1= inactive_vector_time>>1;									// start of active vector 1
//			active_vector2_time= ((active_vectors_time * (phase-120))/60);
//			active_vector1_time= active_vectors_time-active_vector2_time;
//			CCR_CURR_AV1=	CCR_AV1+(active_vector1_time>>1);		// middle of active vector 1 to sample current
//			CCR_AV2= active_vector1_time + CCR_AV1;							// start of active vector 2
//			CCR_CURR_AV2 =CCR_AV2 + (active_vector2_time>>1);	// middle of active vector 2 to sample current
//			CCR_IAV = active_vector2_time+ CCR_AV2;
//			
//			CCR_U = CCR_IAV;
//			CCR_V = CCR_AV1;
//			CCR_W = CCR_AV2;
//			break;
//		case(3):										// phase 180-239�
//			CCR_AV1= inactive_vector_time>>1;									// start of active vector 1
//			active_vector2_time= ((active_vectors_time * (phase-180))/60);
//			active_vector1_time= active_vectors_time-active_vector2_time;
//			CCR_CURR_AV1=	CCR_AV1+(active_vector2_time>>1);		// middle of active vector 1 to sample current
//			CCR_AV2= active_vector2_time + CCR_AV1;							// start of active vector 2
//			CCR_CURR_AV2 =CCR_AV2 + (active_vector1_time>>1);	// middle of active vector 2 to sample current
//			CCR_IAV = active_vector1_time+ CCR_AV2;
//			
//			CCR_U = CCR_IAV;
//			CCR_V = CCR_AV2;
//			CCR_W = CCR_AV1;
//			break;
//		case(4):										// phase 240-299�
//			CCR_AV1= inactive_vector_time>>1;									// start of active vector 1
//			active_vector2_time= ((active_vectors_time * (phase-240))/60);
//			active_vector1_time= active_vectors_time-active_vector2_time;
//			CCR_CURR_AV1=	CCR_AV1+(active_vector1_time>>1);		// middle of active vector 1 to sample current
//			CCR_AV2= active_vector1_time + CCR_AV1;							// start of active vector 2
//			CCR_CURR_AV2 =CCR_AV2 + (active_vector2_time>>1);	// middle of active vector 2 to sample current
//			CCR_IAV = active_vector2_time+ CCR_AV2;
//			
//			CCR_U = CCR_AV2;
//			CCR_V = CCR_IAV;
//			CCR_W = CCR_AV1;
//			break;
//		case(5):										// phase 300-359�
//			CCR_AV1= inactive_vector_time>>1;									// start of active vector 1
//			active_vector2_time= ((active_vectors_time * (phase-300))/60);
//			active_vector1_time= active_vectors_time-active_vector2_time;
//			CCR_CURR_AV1=	CCR_AV1+(active_vector2_time>>1);		// middle of active vector 1 to sample current
//			CCR_AV2= active_vector2_time + CCR_AV1;							// start of active vector 2
//			CCR_CURR_AV2 =CCR_AV2 + (active_vector1_time>>1);	// middle of active vector 2 to sample current
//			CCR_IAV = active_vector1_time+ CCR_AV2;
//			
//			CCR_U = CCR_AV1;
//			CCR_V = CCR_IAV;
//			CCR_W = CCR_AV2;
//			break;
//	}
	
	if(phase<180)									CCR_U=PWM_TIM_ARR_half+((uint32_t)(SPWM[phase]*setpoint*PWM_TIM_ARR_half)>>18);
	else 											CCR_U=PWM_TIM_ARR_half-((uint32_t)(SPWM[phase-180]*setpoint*PWM_TIM_ARR_half)>>18);
	
	if(phase<120)									CCR_V=PWM_TIM_ARR_half-((uint32_t)(SPWM[phase+60]*setpoint*PWM_TIM_ARR_half)>>18);
	else if((phase<300)) 							CCR_V=PWM_TIM_ARR_half+((uint32_t)(SPWM[phase-120]*setpoint*PWM_TIM_ARR_half)>>18);
	else											CCR_V=PWM_TIM_ARR_half-((uint32_t)(SPWM[phase-300]*setpoint*PWM_TIM_ARR_half)>>18);
																												 
	if(phase<60)									CCR_W=PWM_TIM_ARR_half+((uint32_t)(SPWM[phase+120]*setpoint*PWM_TIM_ARR_half)>>18);
	else if((phase<240)) 							CCR_W=PWM_TIM_ARR_half-((uint32_t)(SPWM[phase-60]*setpoint*PWM_TIM_ARR_half)>>18);
	else											CCR_W=PWM_TIM_ARR_half+((uint32_t)(SPWM[phase-240]*setpoint*PWM_TIM_ARR_half)>>18);
	
	
	
	
	// calculte the CCR5 und CCR6 register to determine when the current is sampled
	// these CCR registers must be set in die middel of the active vectors
	
//	if(phase<)
	
	
								
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
