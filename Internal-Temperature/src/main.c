#include "stm32f4xx.h"                  // Device header
#include "inits.h"

double Temperature;
uint16_t adc_value;

int main()
{
	double Vsense;
	double V25 = 0.76, Avg_Slope = 0.0025;

	adc_inits();

	while(1)
	{
		adc_value = read_adc();

  		Vsense = adc_value * 3.3 / 4096;

	  	Temperature = ((Vsense - V25) / Avg_Slope) + 25;
	}
}
