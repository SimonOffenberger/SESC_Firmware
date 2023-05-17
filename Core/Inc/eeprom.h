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
#ifndef EEPROM_H
#define EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef struct{
	uint8_t read_pointer;
	uint8_t current_pointer;
	uint8_t read_buffer[25];
}Read_buffer_typedef;

typedef struct{
	uint8_t write_pointer;
	uint8_t current_pointer;
	uint8_t write_buffer[25];
}Write_buffer_typedef;

typedef struct{
	I2C_HandleTypeDef* i2c_handle;
	uint8_t eeprom_adr;
	Write_buffer_typedef Writebuffer;
	Read_buffer_typedef Readbuffer;
}eeprom_typedef;



#define EEPROM_DEV_ADR 0x50


uint8_t I2C_write_dma(eeprom_typedef * eeprom_Handle,uint8_t* data,uint16_t datalength);

#ifdef __cplusplus
}
#endif

#endif /* uart_debug_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
