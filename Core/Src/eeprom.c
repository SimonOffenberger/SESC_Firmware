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
#include "eeprom.h"
#include "i2c.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


eeprom_typedef EEPROM = {0};

uint8_t I2C_write_dma(eeprom_typedef * eeprom_Handle,uint8_t* data,uint16_t datalength){
	uint8_t state=0;

	for(int i=0; i<datalength; i++){
		eeprom_Handle->Writebuffer.write_buffer[i] = data[i];
	}

	HAL_DMA_Start(eeprom_Handle->i2c_handle->hdmatx, eeprom_Handle->Writebuffer.write_buffer ,eeprom_Handle->i2c_handle->Instance->TXDR, datalength);

	return state;
}

uint8_t initialize_eeprom(eeprom_typedef* eeprom_Handle){
	uint8_t dummy_data=0;
	HAL_StatusTypeDef I2C_Status=0;

	I2C_write_dma(eeprom_Handle, &dummy_data, 10);

	//I2C_Status = HAL_I2C_Mem_Read_DMA(eeprom_Handle->i2c_handle,EEPROM_DEV_ADR  /*eeprom_Handle->eeprom_adr*/, 0, 1, &dummy_data, 1);

	return I2C_Status;
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
