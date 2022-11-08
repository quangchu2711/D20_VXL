#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "lcd16x2.h"
#include "delay.h"

void LCD_DisplayNumber(uint8_t row, uint8_t col, uint16_t value);

int main(void)
{
		SystemCoreClockUpdate(); 
    Delay_Init();
    LCD_Init();
		uint16_t number = 0;
    while(1)
		{
			number += 1;
			Delay_Ms(10);
			LCD_DisplayNumber(0, 0, number);		
    }       
 
}

void LCD_DisplayNumber(uint8_t row, uint8_t col, uint16_t value)
{
	char str[6];
	str[5] = '\0';
	str[0] = (char)((value/10000) + '0');
	str[1] = (char)((value/1000)%10 + '0');
	str[2] = (char)((value/100)%10 + '0');
	str[3] = (char)((value/10)%10 + '0');	
	str[4] = (char)((value%10) + '0');	
	LCD_SetRowCol(row, col);
	LCD_PutStr(str);	
}



