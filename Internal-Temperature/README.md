# STM32F4-Internal Temperature Sensor

# Description

This project was made for finding out the temperature from the internal temperature sensor. As described in the reference manual,

    T(°C) = ((Vsense - V25) / Avg_Slope) + 25

    V25 = 0.76 V and Avg_Slope = 2.5 mV/°C (from the datasheet)

    Vsense = ADC_Value * 3.3 / 4096

# Connections

No further connections needed.

# Needs

Discovery Board
