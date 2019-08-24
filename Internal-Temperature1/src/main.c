#include "stm32f4xx.h"                  // Device header
#include "inits.h"

float Temperature;
uint16_t okunan_adc;

int main()
{
	adc_inits();
	
while(1)
{
	read_adc();
	
	okunan_adc = ADC1->DR;

  	Temperature = okunan_adc * 3300;
  	Temperature /= 4096;
  	Temperature -= 760;
 	Temperature /= 2.5; 
  	Temperature += 25; 
		
}	
}
