#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define BUTTON_PORT 		GPIOB
#define BUTTON_PIN  		GPIO_Pin_0
#define BUTTON_CLOCK		RCC_APB2Periph_GPIOB
#define LED_CLOCK		    RCC_APB2Periph_GPIOA
#define LED_PORT				GPIOA
#define LED_PIN    		  GPIO_Pin_13

void Clock_Config(void);
void Button_Config(void);
void Led_Config(void);

uint8_t g_LedStatus;

int main(void)
{
	Clock_Config(); // configuraion clock
	SystemCoreClockUpdate(); // update SystemCoreClock varibale
	Led_Config();
	Button_Config();
	while(1)
	{
		if(GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN) == 0)
		{
				while(GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN) == 0);
				g_LedStatus = !g_LedStatus;
		}
		GPIO_WriteBit(LED_PORT, LED_PIN, (BitAction)(g_LedStatus));
	}
}

void Button_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BUTTON_CLOCK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin  = BUTTON_PIN;
    GPIO_Init(BUTTON_PORT,&GPIO_InitStructure);
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

void Led_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED_CLOCK, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = LED_PIN;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);  
}
