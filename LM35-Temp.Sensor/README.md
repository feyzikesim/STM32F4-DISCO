# STM32F4-LM35 Temperature Sensor with ADC

# Description

This project basically is an ADC example. LM35 is an analog temperature sensor. Actually there is no difference between reading an analog value of a potentiometer. As described in the datasheet, 

        T(°C) = VOUT / (10 mv/°C)

        VOUT = ADC_Value * 5 /4096

# Connections

LM35's VOUT -> PA0

# Needs

Discovery Board

LM35
