#include "stm32f4xx.h"                  // Device header
#include "inits.h"

float Temperature;
uint16_t adc_value;

int main()
{
	adc_inits();

	while(1)
	{
		read_adc();

		adc_value = ADC1->DR;

  		Temperature = adc_value * 3300;
	  	Temperature /= 4096;
  		Temperature -= 760;
 		Temperature /= 2.5;
  		Temperature += 25;
	}
}
