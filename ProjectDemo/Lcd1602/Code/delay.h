/**
  ******************************************************************************
  * @file    delay.h
  * @author  Chu Xuan Quang
  * @date    07-11-2022  
	******************************************************************************
*/
#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"
void Delay_Init(void);
void Delay_Ms(uint32_t u32DelayMs);
#endif

