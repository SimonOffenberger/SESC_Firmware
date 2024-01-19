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
#include "phaseEstimater.h"
#include "SVM.h"
#include "encoder.h"
#include "main.h"
#include "tim.h"
#include "global_var.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "uart_debug.h"
 
static uint16_t phase=0;

//#define SectorA 4 0�   offset
//#define SectorB 6 60�  offset
//#define SectorC 2 120� offset
//#define SectorD 3 180� offset
//#define SectorE 1 240� offset
//#define SectorF 5 300� offset
//																		//E   C   D   A F   B   X   X
//uint16_t HallSecotor_phase_offset[8]={255,240,120,180,0,300,60,255};

																				//E   C   D   A F   B   X   X
//uint16_t HallSector_phase_offset[8]={255,350,230,275,93,35,156,255};
uint16_t  HallSector_Averages[8]={1};
uint16_t HallSector_phase_offset[8]={255};
uint16_t HallSector_phase_span[8]={0};

void init_phase_estimater(){
	__HAL_TIM_DISABLE(&SectorTIM);
	SectorTIM.Instance->ARR=65535;
	SectorTIM.Instance->PSC=6;
	__HAL_TIM_ENABLE(&SectorTIM);
}

uint16_t time_between_sectors=0;

// make Pin State Global to see them on STM Studio
uint8_t HALL1 = 0;
uint8_t HALL2 = 0;
uint8_t HALL3 = 0;

uint16_t initialize_Hallsensor_phase_estimator(uint16_t setpoint){
	uint16_t init_done=0;
	uint8_t HallSector=0;
	uint16_t in_toleranz_count=0;

	static uint16_t voltage_phase=0;

	static uint8_t init=0;

	static uint8_t lock_HALL1=0;
	static uint8_t lock_HALL2=0;
	static uint8_t lock_HALL3=0;

	static uint16_t phase_old=0;

	static uint8_t prev_Sector=0;

	if(!init){
		// configure Sector Tim to Output a 10Hz Sinewave;
		SectorTIM.Instance-> ARR= 360;
		SectorTIM.Instance-> PSC = 30000;
		init=1;
	}

	voltage_phase = SectorTIM.Instance->CNT;
	Motor.rotorphase=voltage_phase;
	HALL1 = (HALL1<<1)|HAL_GPIO_ReadPin(HALL1_GPIO_Port,HALL1_Pin);
	HALL2 = (HALL2<<1)|HAL_GPIO_ReadPin(HALL2_GPIO_Port,HALL2_Pin);
	HALL3 = (HALL3<<1)|HAL_GPIO_ReadPin(HALL3_GPIO_Port,HALL3_Pin);

	uint8_t HALL1_rising_edge =		  (HALL1&0x0F)==3;
	uint8_t HALL1_falling_edge=	    (HALL1&0x0F)==14;
	uint8_t HALL2_rising_edge =		  (HALL2&0x0F)==3;
	uint8_t HALL2_falling_edge=	    (HALL2&0x0F)==14;
	uint8_t HALL3_rising_edge =		  (HALL3&0x0F)==3;
	uint8_t HALL3_falling_edge=	    (HALL3&0x0F)==14;

	uint8_t valid_signals=0;

	//Bei einer Flanke wenn diese nicht gerade gelockt ist
	if((HALL1_falling_edge||HALL1_rising_edge)&&!lock_HALL1){
		lock_HALL1=1;
		lock_HALL2=0;
		lock_HALL3=0;
		valid_signals = (HALL1_rising_edge<<7) || (HALL1_falling_edge<<6) || (HALL2_rising_edge<<5) || (HALL2_falling_edge<<4) || (HALL3_rising_edge<<3) || (HALL3_falling_edge<<2);
	}
	//Bei einer Flanke wenn diese nicht gerade gelockt ist
	if((HALL2_falling_edge||HALL2_rising_edge)&&!lock_HALL2){
		lock_HALL1=0;
		lock_HALL2=1;
		lock_HALL3=0;
		valid_signals = (HALL1_rising_edge<<7) || (HALL1_falling_edge<<6) || (HALL2_rising_edge<<5) || (HALL2_falling_edge<<4) || (HALL3_rising_edge<<3) || (HALL3_falling_edge<<2);
	}
	//Bei einer Flanke wenn diese nicht gerade gelockt ist
	if((HALL3_falling_edge||HALL3_rising_edge)&&!lock_HALL3){
		lock_HALL1=0;
		lock_HALL2=0;
		lock_HALL3=1;
		valid_signals = (HALL1_rising_edge<<7) || (HALL1_falling_edge<<6) || (HALL2_rising_edge<<5) || (HALL2_falling_edge<<4) || (HALL3_rising_edge<<3) || (HALL3_falling_edge<<2);
	}


	if(valid_signals){
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);

		if(phase_old < voltage_phase){
			time_between_sectors = voltage_phase - phase_old;
		}
		else{
			time_between_sectors =360 - (phase_old - voltage_phase);
		}
		phase_old = voltage_phase;
		// Get the current Hallsektor 
		HallSector = (HALL3<<2)|(HALL2<<1)|(HALL1);

		// Check if the sector is actually possible, the sectors should be spaced apart by 60�
		if(time_between_sectors>45 && time_between_sectors <75){

			HallSector_phase_offset[HallSector] = (HallSector_Averages[HallSector] * voltage_phase) /(HallSector_Averages[HallSector] +1);
			HallSector_Averages[HallSector] ++;

			if( HallSector_phase_offset[HallSector]>HallSector_phase_offset[prev_Sector]){
				// calculate the phase span of the previous Sektor
				HallSector_phase_span[prev_Sector] = HallSector_phase_offset[HallSector]-HallSector_phase_offset[prev_Sector];
			}
			else {
				HallSector_phase_span[prev_Sector] =(360 + HallSector_phase_offset[HallSector]) - HallSector_phase_offset[prev_Sector];
			}

		}
		prev_Sector = HallSector;
	}

	for(int i=0; i<8;i++){
		if(HallSector_Averages[i] >= 256) {
			in_toleranz_count++;
		}
	}

	if(in_toleranz_count==6){
		init_done=1;
		for(int i=0;i<8;i++){
			if(HallSector_Averages[i]<256){
				HallSector_phase_offset[i]=255;	// Für ungültige Sektoren wird die Zahl 255 verwendet
			}
			else{
				UART_transmit_string("Hallsektor: ");
				UART_transmit_ui32(i);
				UART_transmit_string(" Offset: ");
				UART_transmit_ui32(HallSector_phase_offset[i]);
				UART_transmit_string(" Phasespan: ");
				UART_transmit_ui32(HallSector_phase_span[i]);
				UART_transmit_crlf();
			}
		}
	}

	setVoltageVector(voltage_phase, 0, setpoint);

	return init_done;
}


static uint8_t block_commutation_active=1;
static uint16_t Prev_Sector_times[8]={0};

void estimate_rotor_phase(motor_TypeDef* motor){
	static uint8_t init=0;

	if(!init){
		//init_phase_estimater();
		MX_TIM6_Init();
		init=1;
	}


	static uint8_t HallSector=0;
	uint16_t newSectorTime=0;
	static uint16_t SectorTime=0;
	static uint16_t PreviousHallSector=0;
	uint16_t CurrentSectorTime=SectorTIM.Instance->CNT;
	static prev_phase=0;

	
	static uint8_t HALL1 = 0;
	static uint8_t HALL2 = 0;
	static uint8_t HALL3 = 0;
	
/*	static uint8_t HALL1_pinstate = 0;
	static uint8_t HALL2_pinstate = 0;
	static uint8_t HALL3_pinstate = 0;*/
												 
 HALL1 = (HALL1<<1)|HAL_GPIO_ReadPin(HALL1_GPIO_Port,HALL1_Pin);
 HALL2 = (HALL2<<1)|HAL_GPIO_ReadPin(HALL2_GPIO_Port,HALL2_Pin);
 HALL3 = (HALL3<<1)|HAL_GPIO_ReadPin(HALL3_GPIO_Port,HALL3_Pin);

	uint8_t HALL1_rising_edge =		  (HALL1&0x0F)==3;
	uint8_t HALL1_falling_edge=	    (HALL1&0x0F)==14;
	uint8_t HALL2_rising_edge =		  (HALL2&0x0F)==3;
	uint8_t HALL2_falling_edge=	    (HALL2&0x0F)==14;
	uint8_t HALL3_rising_edge =		  (HALL3&0x0F)==3;
	uint8_t HALL3_falling_edge=	    (HALL3&0x0F)==14;
	
	uint8_t valid_signals = 0;// (HALL1_rising_edge<<7) || (HALL1_falling_edge<<6) || (HALL2_rising_edge<<5) || (HALL2_falling_edge<<4) || (HALL3_rising_edge<<3) || (HALL3_falling_edge<<2);

	static uint8_t lock_HALL1=0;
	static uint8_t lock_HALL2=0;
	static uint8_t lock_HALL3=0;

	//Bei einer Flanke wenn diese nicht gerade gelockt ist
	if((HALL1_falling_edge||HALL1_rising_edge)&&!lock_HALL1){
		lock_HALL1=1;
		lock_HALL2=0;
		lock_HALL3=0;
		valid_signals = (HALL1_rising_edge<<7) || (HALL1_falling_edge<<6) || (HALL2_rising_edge<<5) || (HALL2_falling_edge<<4) || (HALL3_rising_edge<<3) || (HALL3_falling_edge<<2);
	}
	//Bei einer Flanke wenn diese nicht gerade gelockt ist
	if((HALL2_falling_edge||HALL2_rising_edge)&&!lock_HALL2){
		lock_HALL1=0;
		lock_HALL2=1;
		lock_HALL3=0;
		valid_signals = (HALL1_rising_edge<<7) || (HALL1_falling_edge<<6) || (HALL2_rising_edge<<5) || (HALL2_falling_edge<<4) || (HALL3_rising_edge<<3) || (HALL3_falling_edge<<2);
	}
	//Bei einer Flanke wenn diese nicht gerade gelockt ist
	if((HALL3_falling_edge||HALL3_rising_edge)&&!lock_HALL3){
		lock_HALL1=0;
		lock_HALL2=0;
		lock_HALL3=1;
		valid_signals = (HALL1_rising_edge<<7) || (HALL1_falling_edge<<6) || (HALL2_rising_edge<<5) || (HALL2_falling_edge<<4) || (HALL3_rising_edge<<3) || (HALL3_falling_edge<<2);
	}

	
	if(valid_signals){
		HallSector = (HAL_GPIO_ReadPin(HALL3_GPIO_Port,HALL3_Pin)<<2)|(HAL_GPIO_ReadPin(HALL2_GPIO_Port,HALL2_Pin)<<1)|(HAL_GPIO_ReadPin(HALL1_GPIO_Port,HALL1_Pin));
	}
	
	
	uint16_t offset= HallSector_phase_offset[HallSector];
	
	if(offset!=255){																				// valid Sector
		if(HallSector!=PreviousHallSector){										// Sector changed
			if(__HAL_TIM_GET_FLAG(&SectorTIM,TIM_FLAG_UPDATE)){	// Timer overflowed
				__HAL_TIM_CLEAR_FLAG(&SectorTIM,TIM_FLAG_UPDATE);
				block_commutation_active=1;
			}
			else{
				// inhibit false sectorchanges due to artefacts in the Hall signal.
				
				// if the sector changes to quickly (artefacts) then don`t reset the hall sector time;
				if(SectorTime >= (Prev_Sector_times[PreviousHallSector]>>1)){
				SectorTime = SectorTIM.Instance->CNT;							  // Secure the Sector Time
				Prev_Sector_times[PreviousHallSector] = SectorTime;				// The Measured Sector Time is form the Previous Sector to this one;
				block_commutation_active=0;
				}
			}
			SectorTIM.Instance->CNT=0;													// reset counter
			//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		}
		
		
		if(block_commutation_active){
			motor->rotorphase = offset;
		}
		else{
			motor->rotorphase= offset + (CurrentSectorTime*HallSector_phase_span[HallSector])/Prev_Sector_times[HallSector];
			if(motor->rotorphase > (offset+HallSector_phase_span[HallSector])) motor->rotorphase= offset+HallSector_phase_span[HallSector];
		}
		PreviousHallSector=HallSector;
	}
	else{
		offset = HallSector_phase_offset[PreviousHallSector];
		if(block_commutation_active){
			motor->rotorphase = offset;
		}
		else{
			motor->rotorphase= offset + (CurrentSectorTime*HallSector_phase_span[HallSector])/Prev_Sector_times[HallSector];
			if(motor->rotorphase > (offset+HallSector_phase_span[HallSector])) motor->rotorphase= offset+HallSector_phase_span[HallSector];
		}
	}
	
	// Correct for glitches in the rotorphase

	int16_t rate_of_change = 0;

	// When the rotorphase passes 360° correct for it
	if(motor->rotorphase-prev_phase<300){
		rate_of_change = (motor->rotorphase+360)-prev_phase;
	}
	else if(motor->rotorphase - prev_phase >300){
		rate_of_change = (motor->rotorphase-360)-prev_phase;
	}
	else{
		rate_of_change = motor->rotorphase-prev_phase;
	}

	if(rate_of_change>5) motor->corr_rotor_phase = prev_phase + 5;
	else{ motor->corr_rotor_phase=motor->rotorphase;}

	prev_phase = motor->rotorphase;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
