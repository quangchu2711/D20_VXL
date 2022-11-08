#include "led7Seg.h"

volatile uint8_t stateScanLed = 0;
volatile uint16_t number7Seg;

uint8_t LED7SEG[10]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void Led7Seg_Display(uint8_t Data7seg)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0 << i, (BitAction)(Data7seg & (0x01 << i)));		
	}
}

void Led7Seg_Enable(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  

  // Time base configuration
  TIM_TimeBaseInitTypeDef  tim3_init_struct;
  tim3_init_struct.TIM_Prescaler = 71;  
  tim3_init_struct.TIM_Period = 99;
  tim3_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &tim3_init_struct);

  TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);   
  TIM_Cmd(TIM3, ENABLE);                      

  // TIM3_IRQn configuration
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Led7Seg_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
	/*Configuration GPIO pin*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /*enble clock for GPIOC*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/*Configuration GPIO pin*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Led7Seg_Number(uint16_t num)
{
	number7Seg = num;
}

void TIM3_IRQHandler(void)
{
	uint8_t Digit1 = number7Seg % 10;
	uint8_t Digit2 = (number7Seg / 10) % 10;
	uint8_t Digit3 = (number7Seg / 100) % 10;
	uint8_t Digit4 = number7Seg / 1000;
	
	stateScanLed += 1;
	if (stateScanLed > 3) stateScanLed = 0;
	switch (stateScanLed)
	{
		case 0:
			GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(0));
			Led7Seg_Display(~LED7SEG[Digit4]);
			//Display7Seg(0xff);
		break;
		case 1:
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(1));
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(1));
			Led7Seg_Display(LED7SEG[Digit3]& 0x7f);	//Note: show dot 
		break;
		case 2:
			GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction)(1));
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(1));
			Led7Seg_Display(LED7SEG[Digit2]);
		break;
		case 3:
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction)(1));
			GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(0));
			GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(1));
		  Led7Seg_Display(LED7SEG[Digit1]);
	  break;
	}
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  // clear update interrupt flag
}
