#include "inits.h"
#include "stm32f4xx_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f4xx_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void adc_inits(void)
{
	ADC_InitTypeDef adcX;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_DeInit();
	adcX.ADC_DataAlign = ADC_DataAlign_Right;
	adcX.ADC_NbrOfConversion = 1;
	adcX.ADC_Resolution = ADC_Resolution_12b;
	ADC_Init(ADC1, &adcX);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);

	ADC_Cmd(ADC1, ENABLE);

	ADC_TempSensorVrefintCmd(ENABLE);
}

uint16_t read_adc(void)
{
	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);

	return ADC_GetConversionValue(ADC1);
}
