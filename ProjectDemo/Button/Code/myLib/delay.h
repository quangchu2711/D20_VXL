
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

void DelayInit(void);
void Delay_1Ms(void);
void TIM2_Ms(uint32_t u32DelayMs);

#ifdef __cplusplus
}
#endif

#endif

