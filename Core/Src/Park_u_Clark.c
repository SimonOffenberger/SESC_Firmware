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
#include "Park_u_Clark.h"
#include "global_var.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"
 

const SIN_TABLE[91]={
		0		 ,
		18       ,
		36       ,
		54       ,
		71       ,
		89       ,
		107      ,
		125      ,
		143      ,
		160      ,
		178      ,
		195      ,
		213      ,
		230      ,
		248      ,
		265      ,
		282      ,
		299      ,
		316      ,
		333      ,
		350      ,
		367      ,
		384      ,
		400      ,
		416      ,
		433      ,
		449      ,
		465      ,
		481      ,
		496      ,
		512      ,
		527      ,
		543      ,
		558      ,
		573      ,
		587      ,
		602      ,
		616      ,
		630      ,
		644      ,
		658      ,
		672      ,
		685      ,
		698      ,
		711      ,
		724      ,
		737      ,
		749      ,
		761      ,
		773      ,
		784      ,
		796      ,
		807      ,
		818      ,
		828      ,
		839      ,
		849      ,
		859      ,
		868      ,
		878      ,
		887      ,
		896      ,
		904      ,
		912      ,
		920      ,
		928      ,
		935      ,
		943      ,
		949      ,
		956      ,
		962      ,
		968      ,
		974      ,
		979      ,
		984      ,
		989      ,
		994      ,
		998      ,
		1002     ,
		1005     ,
		1008     ,
		1011     ,
		1014     ,
		1016     ,
		1018     ,
		1020     ,
		1022     ,
		1023     ,
		1023     ,
		1024     ,
		1024
};


int16_t sin_from_table(uint16_t phase){

	if(phase>=360) phase = phase -360;

	if(phase>=0 && phase <90){
		return SIN_TABLE[phase];
	}
	else if(phase>=90 && phase<180){
		return SIN_TABLE[90-(phase-90)];
	}
	else if(phase>=180 && phase <270){
		return -SIN_TABLE[phase-180];
	}
	else if(phase>=270 && phase<360){
		return -SIN_TABLE[90-((phase-180)-90)];
	}
	else {
		return 0;
	}

}

int16_t cos_from_table(uint16_t phase){

	phase = phase+90;

	if(phase>=360) phase = phase -360;

	if(phase>=0 && phase <90){
		return SIN_TABLE[phase];
	}
	else if(phase>=90 && phase<180){
		return SIN_TABLE[90-(phase-90)];
	}
	else if(phase>=180 && phase <270){
		return -SIN_TABLE[phase-180];
	}
	else if(phase>=270 && phase<360){
		return -SIN_TABLE[90-((phase-180)-90)];
	}
	else {
		return 0;
	}


}


void transform_to_Clark_domain(int * PhaseCurrents, int * AlphaBetaCurrents){
		// iapha = Ia
		// ibeta = (Ib - Ic)/wurzel(3)

	//PhaseCurrents[0]= sin_from_table(Motor.rotorphase);
	//PhaseCurrents[1]= sin_from_table(Motor.rotorphase+240);
	//PhaseCurrents[2]= sin_from_table(Motor.rotorphase+120);
	
	AlphaBetaCurrents[0] = PhaseCurrents[0];
	AlphaBetaCurrents[1] = (((int64_t)(PhaseCurrents[1] - PhaseCurrents[2])<<10)/1773);


}

void transform_to_Park_domain(int * AlphaBetaCurrents,int * DQCurrents, uint32_t phase){
//		// iq = -sin(phase)*ialpha + cos(phase)*ibeta
//		// id =  cos(phase)*ialpha + sin(phase)*ibeta



	DQCurrents[0] = (-(sin_from_table(phase)*AlphaBetaCurrents[0]) + (cos_from_table(phase)*AlphaBetaCurrents[1]))>>10;
	DQCurrents[1] = ( (cos_from_table(phase)*AlphaBetaCurrents[0]) + (sin_from_table(phase)*AlphaBetaCurrents[1]))>>10;



}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
