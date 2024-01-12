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
#include "encoder.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int readEncoder(GPIO_TypeDef* PORT_A, uint16_t PIN_A , GPIO_TypeDef* PORT_B, uint16_t PIN_B,int upperboundary, int lowerboundary){
	static ui8 ENCODERA =0;
	static ui8 ENCODERB =0;
	static int encoder_val=0;
	
	ENCODERA=(ENCODERA<<1)|HAL_GPIO_ReadPin(PORT_A,PIN_A);
	ENCODERB=(ENCODERB<<1)|HAL_GPIO_ReadPin(PORT_B,PIN_B);
	
	bool ENCODERA_rising_edge =		(ENCODERA&0x0F)==3;
	bool ENCODERA_falling_edge=	  (ENCODERA&0x0F)==14;
	bool ENCODERB_rising_edge =		(ENCODERB&0x0F)==3;
	bool ENCODERB_falling_edge=	  (ENCODERB&0x0F)==14;
	
	static string event;
	static string state;
	static bool iniOK=false;
	
	if(!iniOK)																					{event="power on";											  state="idle";}
	else if (state_is("idle")&&ENCODERA_rising_edge)											{event="ENCODERA rising";											state="wait for ENCODERB";}
	else if (state_is("idle")&&ENCODERB_falling_edge)											{event="ENCODERB falling";											state="wait for ENCODERA";}
	else if (state_is("wait for ENCODERB")&&ENCODERB_rising_edge)								{event="count up";											  state="idle";}
	else if (state_is("wait for ENCODERA")&&ENCODERA_falling_edge)								{event="count down";											state="idle";}
	else 																						{event="none";}
	
	if(event_occured("count up")) 	encoder_val++;
	if(event_occured("count down")) encoder_val--;
	
	if(encoder_val > upperboundary) encoder_val=upperboundary;
	if(encoder_val < lowerboundary)	encoder_val=lowerboundary;

	
	iniOK=true;
	
	return encoder_val;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
