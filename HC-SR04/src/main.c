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
void Delay_us(uint32_t us);
float distance_read(void);

uint32_t i, j;
float Distance_cm, dis;

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
		Distance_cm = distance_read();	
		
		if(Distance_cm < 50)
		{
		GPIO_SetBits(LED_PORT, LED_1);
		GPIO_ResetBits(LED_PORT, LED_2);
		GPIO_ResetBits(LED_PORT, LED_3);
		GPIO_ResetBits(LED_PORT, LED_4);
		}
		else if(Distance_cm < 100)
		{
		GPIO_SetBits(LED_PORT, LED_2);
		GPIO_ResetBits(LED_PORT, LED_1);
		GPIO_ResetBits(LED_PORT, LED_3);
		GPIO_ResetBits(LED_PORT, LED_4);
		}
		else if(Distance_cm < 150)
		{	
		GPIO_SetBits(LED_PORT, LED_3);
		GPIO_ResetBits(LED_PORT, LED_1);
		GPIO_ResetBits(LED_PORT, LED_2);
		GPIO_ResetBits(LED_PORT, LED_4);
		}
		else if(Distance_cm < 200)
		{	
		GPIO_SetBits(LED_PORT, LED_4);
		GPIO_ResetBits(LED_PORT, LED_1);
		GPIO_ResetBits(LED_PORT, LED_2);
		GPIO_ResetBits(LED_PORT, LED_3);
		}
		else
		GPIO_ResetBits(LED_PORT, LED_1 | LED_2 | LED_3 | LED_4);
				
	}
}

void Delay_us(uint32_t us)
{
  i = us;
  while(i != 0);
}

float distance_read(void)
{
	/* TRIG_PIN = 1 for 10 us */
	GPIO_SetBits(TRIG_PORT, TRIG_PIN);
	Delay_us(10);
	GPIO_ResetBits(TRIG_PORT, TRIG_PIN);
	
	/* Wait until ECHO_PIN = 1 */
	while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) != 1);
	
	/* Assign starting value to SysTick */
	SysTick->VAL = SysTick->LOAD;
	
	while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == 1)		
        {																													
		if(SysTick->VAL == 0)																		
		{						/* While the ECHO_PIN = 1 */
		j++;			      		        /* j increases for every 1 us */
		}
		else
		j=j;
	}
	
	dis = j / 58.2;
  
	j = 0;
	
	Delay_us(10000);					/* Wait 10ms for sensor */
	
	return dis;
}

void SysTick_Handler(void)
{
  if(i != 0)
  {
  i--;
  }
}
