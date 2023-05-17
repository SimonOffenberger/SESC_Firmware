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
#include "uart_debug.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart1;
static char UART_TX_Buffer[UART_TX_BUFFER_SIZE]={0};
static char UART_TX_Buffer_DMA_only[UART_TX_BUFFER_SIZE]={0};
static uint16_t tx_buffer_end_pos=0;

void reverse(uint8_t *s);
uint16_t ui32toa(uint32_t n, uint8_t *s);

void UART_transmit_string(char payload[]){
	for(uint16_t size=0;payload[size]!='\0';size++){

		if(tx_buffer_end_pos >= UART_TX_BUFFER_SIZE)break;

		UART_TX_Buffer[tx_buffer_end_pos]=payload[size];
		tx_buffer_end_pos++;
	}
	UART_TX_Buffer[tx_buffer_end_pos] = '\0';
}

void UART_writeLine(char payload[]){

	for(uint16_t size=0;payload[size]!='\0';size++){

		if(tx_buffer_end_pos >= UART_TX_BUFFER_SIZE)break;

		UART_TX_Buffer[tx_buffer_end_pos]=payload[size];
		tx_buffer_end_pos++;
	}
	
	if((tx_buffer_end_pos) < UART_TX_BUFFER_SIZE-2){
		UART_TX_Buffer[tx_buffer_end_pos++]='\r';
		UART_TX_Buffer[tx_buffer_end_pos++]='\n';
	}

	UART_TX_Buffer[tx_buffer_end_pos] = '\0';	 // place zero terminator at the end of the string
	//HAL_UART_Transmit_DMA(&huart1,(uint8_t*)UART_TX_Buffer,size);
}

void UART_transmit_ui32(uint32_t num){

	if((tx_buffer_end_pos) < UART_TX_BUFFER_SIZE-10){
		tx_buffer_end_pos+= ui32toa(num, &UART_TX_Buffer[tx_buffer_end_pos]);
	}

	UART_TX_Buffer[tx_buffer_end_pos] = '\0';	 // place zero terminator at the end of the string
}

void UART_transmit_crlf(){

	if((tx_buffer_end_pos) < UART_TX_BUFFER_SIZE-2){
		UART_TX_Buffer[tx_buffer_end_pos++]='\r';
		UART_TX_Buffer[tx_buffer_end_pos++]='\n';
	}
	UART_TX_Buffer[tx_buffer_end_pos] = '\0';	 // place zero terminator at the end of the string
}

void UART_transmit_buffer(){

	for(uint32_t size=0;UART_TX_Buffer[size]!='\0';size++){
		UART_TX_Buffer_DMA_only[size] = UART_TX_Buffer[size];
	}
	HAL_UART_Transmit_DMA(&huart1,(uint8_t*)UART_TX_Buffer_DMA_only,tx_buffer_end_pos);
	tx_buffer_end_pos=0;
}

// UART1 reverse
void reverse(uint8_t *s){
	uint32_t c, i, j;

	for (i = 0, j = strlen((char*)s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

// UART1 ui32toa
uint16_t ui32toa(uint32_t n, uint8_t *s){
	uint16_t i = 0;

	do {       																// generate digits in reverse order
		s[i++] = n % 10 + '0';   								// get next digit
	} while ((n /= 10) > 0);     							// delete it
	s[i] = '\0';
	reverse(s);
	return i;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
