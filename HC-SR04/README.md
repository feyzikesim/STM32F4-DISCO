# STM32F4-HC-SR04 Ultrasonic Sensor

# Description

This project was made for finding out the distance of an object to HC-SR04. HC-SR04 is a cheap and popular ultrasonic ranging sensor. As described in the datasheet, we need to apply TTL-High signal to sensor's TRIG pin at least for 10 μs. After that sensor responses with 8-sonic bursts from ECHO pin. Later, ECHO pin remains high depending on the distance. Distance formula is,

    Distance(cm) = μs / 58,    μs->ECHO Pin High Time

# Connections

HC-SR04 ECHO -> PA1

HC-SR04 TRIG -> PC3

# Needs

Discovery Board

HC-SR04
