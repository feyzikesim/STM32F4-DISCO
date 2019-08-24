#include "stm32f4xx.h"                  // Device header

void read_adc(void)
{
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_GetConversionValue(ADC1);
}

void adc_init(void)
{
	ADC_InitTypeDef	adcX;
	GPIO_InitTypeDef gpioX;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	gpioX.GPIO_OType = GPIO_OType_PP;
	gpioX.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioX.GPIO_Speed = GPIO_Speed_100MHz;
	gpioX.GPIO_Mode	= GPIO_Mode_AN;
	gpioX.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &gpioX);
	
	adcX.ADC_ContinuousConvMode = ENABLE;
	adcX.ADC_DataAlign = ADC_DataAlign_Right;
	adcX.ADC_NbrOfConversion = 1;
	adcX.ADC_Resolution = ADC_Resolution_12b;
	ADC_Init(ADC1, &adcX);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);
	
	ADC_Cmd(ADC1, ENABLE);
}
int main()
{
	uint16_t Temperature;
	
	adc_init();
	
while(1)
{
	read_adc();
	
	Temperature = ADC1->DR;
	Temperature *= 3300;
	Temperature /= 4096;
	Temperature /= 10;
}
}
