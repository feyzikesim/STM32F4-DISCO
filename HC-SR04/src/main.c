#include "stm32f4xx.h"                  // Device header

/* Trigger Pin defines */
#define TRIG_PORT   GPIOC
#define TRIG_PIN    GPIO_Pin_3
#define TRIG_RCC    RCC_AHB1Periph_GPIOC

/* Echo Pin defines */
#define ECHO_PORT   GPIOA
#define ECHO_PIN    GPIO_Pin_1
#define ECHO_RCC    RCC_AHB1Periph_GPIOA

/* LED defines */
#define LED_PORT    GPIOD
#define LED_1       GPIO_Pin_12
#define LED_2       GPIO_Pin_13
#define LED_3       GPIO_Pin_14
#define LED_4       GPIO_Pin_15
#define LED_RCC     RCC_AHB1Periph_GPIOD

/* Functions that used */
void delay_us(uint32_t us);
float distance_read(void);

uint32_t i;
float distance_cm;

int main()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable Clock */
	RCC_AHB1PeriphClockCmd(TRIG_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(ECHO_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);

	/* Common Settings */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

	/* Trigger Pin Settings */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin   = TRIG_PIN;
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);

	/* Echo Pin Settings */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin   = ECHO_PIN;
	GPIO_Init(ECHO_PORT, &GPIO_InitStructure);

	/* LED Settings */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin   = LED_1 | LED_2 | LED_3 | LED_4;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	/* Trigger Pin Low */
	GPIO_ResetBits(TRIG_PORT, TRIG_PIN);

	SysTick_Config(SystemCoreClock/1000000);

	while(1)
	{
		distance_cm = distance_read();

		if(distance_cm < 50)
		{
			GPIO_SetBits(LED_PORT, LED_1);
			GPIO_ResetBits(LED_PORT, LED_2 | LED_3 | LED_4);
		}
		else if(distance_cm < 100)
		{
			GPIO_SetBits(LED_PORT, LED_2);
			GPIO_ResetBits(LED_PORT, LED_1 | LED_3 | LED_4);
		}
		else if(distance_cm < 150)
		{
			GPIO_SetBits(LED_PORT, LED_3);
			GPIO_ResetBits(LED_PORT, LED_1 | LED_2 | LED_4);
		}
		else if(distance_cm < 200)
		{
			GPIO_SetBits(LED_PORT, LED_4);
			GPIO_ResetBits(LED_PORT, LED_1 | LED_2 | LED_3);
		}
		else
			GPIO_ResetBits(LED_PORT, LED_1 | LED_2 | LED_3 | LED_4);
	}
}

void delay_us(uint32_t us)
{
	i = us;
	while(i);
}

float distance_read(void)
{
	float distance;
        uint16_t ttl_high_us;

	/* TRIG_PIN = 1 for 10 us */
	GPIO_SetBits(TRIG_PORT, TRIG_PIN);
	delay_us(10);
	GPIO_ResetBits(TRIG_PORT, TRIG_PIN);

	/* Wait until ECHO_PIN = 1 */
	while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) != 1);

	/* Assign starting value to SysTick */
	SysTick->VAL = SysTick->LOAD;

	while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == 1)
        {
		if(SysTick->VAL == 0)					/* While the ECHO_PIN = 1 */
			ttl_high_us++;		      		        /* j increases for every 1 us */
		else
			ttl_high_us=ttl_high_us;
	}

	distance = ttl_high_us / 58;
	ttl_high_us = 0;
	delay_us(10000);					/* Wait 10ms for sensor */

	return distance;
}

void SysTick_Handler(void)
{
	if(i)
		i--;
}
