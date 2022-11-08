#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"

#define LED7SEG_A   GPIO_Pin_0
#define LED7SEG_B   GPIO_Pin_1
#define LED7SEG_C   GPIO_Pin_2
#define LED7SEG_D   GPIO_Pin_3
#define LED7SEG_E   GPIO_Pin_4
#define LED7SEG_F   GPIO_Pin_5
#define LED7SEG_G   GPIO_Pin_6
#define LED7SEG_DP  GPIO_Pin_7

#define LED1        GPIO_Pin_5
#define LED2        GPIO_Pin_2
#define LED3        GPIO_Pin_3
#define LED4        GPIO_Pin_4

uint8_t LED7SEG[10]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
volatile uint8_t stateScanLed = 0;
volatile uint16_t number7Seg;

void GPIO_Config(void);
void Clock_Config(void);
void Led7Seg_Display(uint8_t Data7seg);
void Led7Seg_Number(uint16_t num);
void TIM3_configuration(void);
void NVIC_configuration(void);

int main(void)
{
	DelayInit();
	Clock_Config(); // configuraion clock
	SystemCoreClockUpdate(); // update SystemCoreClock varibale
	GPIO_Config();
	NVIC_configuration();
	TIM3_configuration();
	uint16_t num = 1234;

	while(1)
	{
			Led7Seg_Number(num);
			num += 1;
			TIM2_Ms(100);
	}
}


void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*enble clock for GPIOC*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	
	/*Configuration GPIO pin*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = LED7SEG_A|LED7SEG_B|LED7SEG_C|LED7SEG_D|LED7SEG_E|LED7SEG_F|LED7SEG_G|LED7SEG_A|LED7SEG_DP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = LED1 | LED2 | LED3 | LED4;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

void Clock_Config(void)
{
	/* RCC system reset */
	RCC_DeInit();
	/* HCLK = SYSCLK */
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	/* PCLK2 = HCLK */
	RCC_PCLK2Config(RCC_HCLK_Div2);
	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config(RCC_HCLK_Div2);
	/*enable HSI source clock*/
	RCC_HSICmd(ENABLE); 
	/* Wait till PLL is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}
	/* Select PLL as system clock source */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	/* Wait till PLL is used as system clock source */
	while(RCC_GetSYSCLKSource() != 0x00) {}    
}

void TIM3_configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	
	/*
			Luu y: voi phien ban proteus 8.6, chung ta phai enable TIM1 thi moi co the dung TIMx.
			=> bug cua phan men proteus.
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_Cmd(TIM1, ENABLE);
	
	/*configuration TIM3*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_InitStructure.TIM_Period = 79;
	TIM_InitStructure.TIM_Prescaler = 49;    
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_RepetitionCounter = 0x00;    
	TIM_TimeBaseInit(TIM3, &TIM_InitStructure);

	TIM_TimeBaseInit(TIM3, &TIM_InitStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //enable update interrup
	TIM_Cmd(TIM3, ENABLE);

}
 
void NVIC_configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Led7Seg_Number(uint16_t num)
{
	number7Seg = num;
}
 
void Led7Seg_Display(uint8_t Data7seg)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_0 << i, (BitAction)(Data7seg & (0x01 << i)));		
	}
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
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(1));
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(0));
			GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)(0));
			GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(0));
			Led7Seg_Display(LED7SEG[Digit4]);
			//Display7Seg(0xff);
		break;
		case 1:
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1));
			GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)(0));
			GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(0));
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(0));
			Led7Seg_Display(LED7SEG[Digit3]);	 
		break;
		case 2:
			GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)(1));
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(0));
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(0));
			GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(0));
			Led7Seg_Display(LED7SEG[Digit2]);
		break;
		case 3:
			GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(1));
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(0));
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(0));
			GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)(0));
		  Led7Seg_Display(LED7SEG[Digit1]);
	  break;
	}
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  // clear update interrupt flag
}

