/**
  ******************************************************************************
  * @file    led7Seg.h
  * @author  Chu Xuan Quang
  * @date    12-11-2021
  * @brief   led7Seg program body
  ******************************************************************************
  * @attention
  ******************************************************************************
*/
#ifndef LED7SEG_H_
#define LED7SEG_H_

#include "stm32f10x.h"

void Led7Seg_Display(uint8_t Data7seg);
void Led7Seg_Enable(void);
void Led7Seg_Config(void);
void Led7Seg_Number(uint16_t num);

#endif
