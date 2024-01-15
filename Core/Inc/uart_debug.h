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
#ifndef uart_debug_H
#define uart_debug_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define UART_TX_BUFFER_SIZE 256

void UART_transmit_string(char payload[]);
void UART_writeLine(char payload[]);
void UART_transmit_ui32(uint32_t num);
void UART_transmit_crlf(void);
void UART_transmit_buffer(void);
uint16_t ui32toa(uint32_t n, uint8_t *s);
#ifdef __cplusplus
}
#endif

#endif /* uart_debug_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
