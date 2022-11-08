#include "lcd16x2.h"
  
static void LCD_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef        GPIO_LCD_InitStruction;
    
    /*Configure GPIO pin Output Level */
    GPIO_WriteBit(GPIOA, LCD_D4|LCD_D5|LCD_D5|LCD_D7, (BitAction)0); 
    GPIO_WriteBit(GPIOC, LCD_RS|LCD_RW|LCD_EN, (BitAction)0); 
    
    GPIO_LCD_InitStruction.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_LCD_InitStruction.GPIO_Pin     = LCD_D4|LCD_D5|LCD_D6|LCD_D7;      
    GPIO_LCD_InitStruction.GPIO_Speed   = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_LCD_InitStruction);
 
 
    GPIO_LCD_InitStruction.GPIO_Pin =  LCD_EN|LCD_RS|LCD_RW;
    GPIO_Init(GPIOC, &GPIO_LCD_InitStruction);
}
static void LCD_Enable(void)
{
    GPIO_SetBits(GPIOC, LCD_EN);
    Delay_Ms(1);
    GPIO_ResetBits(GPIOC, LCD_EN);
    Delay_Ms(1);
}
 
static void LCD_Send4Bit(uint8_t Data)
{
  GPIO_WriteBit(GPIOA, LCD_D4, (BitAction)(Data & 1));
  GPIO_WriteBit(GPIOA, LCD_D5, (BitAction)((Data>>1)&1));
  GPIO_WriteBit(GPIOA, LCD_D6, (BitAction)((Data>>2)&1));
  GPIO_WriteBit(GPIOA, LCD_D7, (BitAction)((Data>>3)&1));
}
 
static void LCD_SendCommand(uint8_t command)
{
  LCD_Send4Bit(command >> 4);
  LCD_Enable();
  LCD_Send4Bit(command);
  LCD_Enable();
}

static void LCD_PutChar(uint8_t data)
{
  GPIO_SetBits(GPIOC, LCD_RS);
  LCD_SendCommand(data);
  GPIO_ResetBits(GPIOC, LCD_RS);
}

void LCD_Clear()
{
	LCD_SendCommand(0x01);
	Delay_Ms(1);
}
 
void LCD_Init()
{
  LCD_Config();
    
  LCD_Send4Bit(0x00);
  GPIO_WriteBit(GPIOC, LCD_RS, (BitAction)0);
  LCD_Send4Bit(0x03);
  LCD_Enable();
  LCD_Enable();
  LCD_Enable();
  LCD_Send4Bit(0x02);
  LCD_Enable();
  LCD_SendCommand(0x28); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
  LCD_SendCommand(0x0C); // cho phep hien thi man hinh
  LCD_SendCommand(0x06); // tang ID, khong dich khung hinh
  LCD_SendCommand(0x01); // xoa toan bo khung hinh
}
 
void LCD_SetRowCol(uint8_t row, uint8_t col)
{
  uint8_t address;
  if(row == 0)address=(0x80 + col);
  else if(row == 1) address=(0xc0 + col);
  LCD_SendCommand(address);
}
 
void LCD_PutStr(char *s)
{
  while (*s)
  {
    LCD_PutChar(*s);
    s++;
  }
}

