#include "stm32f4xx.h"                  // Device header

int main ()
{
	GPIO_InitTypeDef		GPIO_InitStructure;

	/* RCC Configurations */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* TIM3 Configurations */
	TIM_DeInit(TIM3);
	TIM_CounterModeConfig(TIM3, TIM_CounterMode_Up);
	TIM_PrescalerConfig(TIM3, 41999, TIM_PSCReloadMode_Update);
	TIM_SetAutoreload(TIM3, 1999);
	
	/* GPIOD Configurations for LEDs */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	
	TIM_Cmd(TIM3, ENABLE);
	
while (1)
{	
	if(TIM_GetCounter(TIM3)>0 && TIM_GetCounter(TIM3)<500)
{		
			GPIO_SetBits(GPIOD, GPIO_Pin_12);
			GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			GPIO_ResetBits(GPIOD, GPIO_Pin_15);
}	
	
	else if(TIM_GetCounter(TIM3)>=500 && TIM_GetCounter(TIM3)<1000)
{
			GPIO_SetBits(GPIOD, GPIO_Pin_13);
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			GPIO_ResetBits(GPIOD, GPIO_Pin_15);
}
	else if(TIM_GetCounter(TIM3)>=1000 && TIM_GetCounter(TIM3)<1500)
{
			GPIO_SetBits(GPIOD, GPIO_Pin_14);
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			GPIO_ResetBits(GPIOD, GPIO_Pin_15);
}
	else if(TIM_GetCounter(TIM3)>=1500 && TIM_GetCounter(TIM3)<2000)
{
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
}
}
}
