# STM32F4-LED Toggle with Timer                                                                                         


# Description

This project is a simple timer example. LEDs on the board sequentially lit for 250 ms. Here is the equation,

    TIM3 Clock Freq = 84 MHz (from the datasheet)

    Prescaler Value = 41999

    Timer freq = (TIM3 Clock Freq) / (Prescaler + 1) = 2 kHz, so 2000 count = 1 sec

    Auto Reload value is 0 to 1999 -> 2000, so 500 count = 250 ms

# Connections

No further connections needed.

# Needs

Discovery Board
