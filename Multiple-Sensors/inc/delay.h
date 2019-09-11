#include "stm32f4xx.h"                  // Device header

uint32_t ticks = 0;

void delay_ms(uint32_t ms)
{
	SysTick_Config(SystemCoreClock/1000);
	ticks = ms;
	while(ticks);
}

void delay_us(uint32_t us)
{
	SysTick_Config(SystemCoreClock/1000000);
	ticks = us;
	while(ticks);
}
