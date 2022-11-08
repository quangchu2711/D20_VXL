/**
  ******************************************************************************
  * @file    lcd16x2.h
  * @author  Chu Xuan Quang
  * @date    07-11-2022  
	******************************************************************************
*/
#ifndef __LCD_H
#define __LCD_H
#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define LCD_RS      GPIO_Pin_13
#define LCD_RW      GPIO_Pin_14
#define LCD_EN      GPIO_Pin_15
#define LCD_D4      GPIO_Pin_8
#define LCD_D5      GPIO_Pin_9
#define LCD_D6      GPIO_Pin_10
#define LCD_D7      GPIO_Pin_11

void LCD_Clear(void);
void LCD_Init(void);
void LCD_SetRowCol(uint8_t row, uint8_t col);
void LCD_PutChar(uint8_t Data);
void LCD_PutStr(char *s);
#endif 
