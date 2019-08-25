#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

uint16_t adc_data;
char data_array[50];

void inits(void)
{
	ADC_InitTypeDef         adcX;
	GPIO_InitTypeDef	gpioX;
	USART_InitTypeDef	usartX;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 TX Configurations */
	/* USB-TTL Module's Rx -> PA2 */
	gpioX.GPIO_Mode  = GPIO_Mode_AF;
	gpioX.GPIO_Pin   = GPIO_Pin_2;
	gpioX.GPIO_Speed = GPIO_Speed_50MHz;
	gpioX.GPIO_OType = GPIO_OType_PP;
	gpioX.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpioX);

	usartX.USART_BaudRate            = 19200;
	usartX.USART_Mode                = USART_Mode_Tx;
	usartX.USART_Parity              = USART_Parity_No;
	usartX.USART_WordLength          = USART_WordLength_8b;
	usartX.USART_StopBits            = USART_StopBits_1;
	usartX.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &usartX);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

	/* ADC1-Ch0 Configurations */
	/* Potentiometer's  Data Pin -> PA0 */
	gpioX.GPIO_Mode  = GPIO_Mode_AN;
	gpioX.GPIO_Pin   = GPIO_Pin_0;
	gpioX.GPIO_Speed = GPIO_Speed_100MHz;
	gpioX.GPIO_OType = GPIO_OType_PP;
	gpioX.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioX);

	adcX.ADC_DataAlign          = ADC_DataAlign_Right;
	adcX.ADC_ContinuousConvMode = ENABLE;
	adcX.ADC_NbrOfConversion    = 1;
	adcX.ADC_Resolution         = ADC_Resolution_8b;
	ADC_Init(ADC1, &adcX);

	ADC_Cmd(ADC1, ENABLE);
	USART_Cmd(USART2, ENABLE);
}

uint8_t read_adc(void)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

void send_data(USART_TypeDef* USARTx, volatile char *data)
{
	while(*data)
	{
	    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
	    USART_SendData(USARTx, *data);
	    *data++;
	}
}

int main()
{
	inits();

	while(1)
	{
	    adc_data = read_adc();
	    sprintf(data_array, "%d\n", adc_data);
	    send_data(USART2, data_array);
	}
}
