#include "delay.h"

void DelayInit(void)
{
	/* su dung timer nao do de tao delay */
	TIM_TimeBaseInitTypeDef timInit;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timInit.TIM_CounterMode = TIM_CounterMode_Up;
	timInit.TIM_Period = 0xffff;
	timInit.TIM_Prescaler = 8 - 1;
	
	TIM_TimeBaseInit(TIM2, &timInit);
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_Ms(uint32_t u32DelayMs)
{
	while(u32DelayMs)
	{
		Delay_1Ms();
		u32DelayMs--;
	}
}

void Delay_1Ms(void)
{
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < 1000);
}
