#include "stm32f4xx.h"                  // Device header
#include "user_defines.h"
#include "delay.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_rcc.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_exti.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

/***************************************************************/
/********************  Function Prototypes  ********************/
/***************************************************************/

void onboard_leds_conf();
void uart_conf(USART_TypeDef* USARTx, uint32_t baudrate);
void adc_conf();
void i2c_conf(uint32_t i2c_bus_speed);
void TM_I2C_ReadMulti_SHT(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count, uint8_t delay);
float pressure();
struct adxl accelerometer();
float sound();
uint16_t co2_density();
float dust();
struct sht temp_humid();
void emergency_states_gpio_and_irq_conf();

/***************************************************************/
/***************************  Tasks  ***************************/
/***************************************************************/

void vTaskPressure(void *pvParameters)
{
	float pres;
	volatile uint32_t delay_time;

	for(;;)
	{
		pres = pressure();
		for(delay_time = 0; delay_time < 1000000; delay_time++);
	}
}

void vTaskAccel(void *pvParameters)
{
	struct adxl accel;
	volatile uint32_t delay_time;

	for(;;)
	{
		accel = accelerometer();
		for(delay_time = 0; delay_time < 1000000; delay_time++);
	}
}

void vTaskSound(void *pvParameters)
{
	float sound_db;
	volatile uint32_t delay_time;

	for(;;)
	{
		sound_db = sound();
		for(delay_time = 0; delay_time < 1000000; delay_time++);
	}
}

void vTaskCO2(void *pvParameters)
{
	uint16_t co2;
	volatile uint32_t delay_time;

	for(;;)
	{
		co2 = co2_density();
		for(delay_time = 0; delay_time < 1000000; delay_time++);
	}
}

void vTaskDust(void *pvParameters)
{
	float pm_25;
	volatile uint32_t delay_time;

	for(;;)
	{
		pm_25 = dust();
		for(delay_time = 0; delay_time < 1000000; delay_time++);
	}
}

void vTaskTempHumid(void *pvParameters)
{
	struct sht temp_humi;
	volatile uint32_t delay_time;

	for(;;)
	{
		temp_humi = temp_humid();
		for(delay_time = 0; delay_time < 1000000; delay_time++);
	}
}

/***************************************************************/
/*************************  Functions  *************************/
/***************************************************************/

void onboard_leds_conf()
{
	TM_GPIO_Init(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
}

void uart_conf(USART_TypeDef* USARTx, uint32_t baudrate)
{
	USART_DeInit(USARTx);
	TM_USART_Init(USARTx, TM_USART_PinsPack_1, baudrate);
}

void adc_conf()
{
	ADC_DeInit();
	TM_ADC_Init(ADC1, ADC_Channel_10);
}

void i2c_conf(uint32_t i2c_bus_speed)
{
	I2C_DeInit(I2C2);
	TM_GPIO_InitAlternate(GPIOB, I2C_SCL, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_I2C2);
	TM_GPIO_InitAlternate(GPIOB, I2C_SDA, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_I2C2);

	TM_I2C_Init(I2C2, TM_I2C_PinsPack_1, i2c_bus_speed);
}

void emergency_states_gpio_and_irq_conf()
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	TM_GPIO_Init(GPIOA, GPIO_Pin_5, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_High);		// Emergency Button
	TM_GPIO_Init(GPIOA, GPIO_Pin_6, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_High);		// Water Level Sensor
	TM_GPIO_Init(GPIOA, GPIO_Pin_7, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_High);		// Cabin Sensor

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);


	EXTI_InitStruct.EXTI_Line = EXTI_Line5;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line7;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);


	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void TM_I2C_ReadMulti_SHT(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count, uint8_t delay)
{
	TM_I2C_Start(I2Cx, address, 0, 1);
	TM_I2C_WriteData(I2Cx, reg);
	TM_I2C_Start(I2Cx, address, 1, 1);

	delay_ms(delay);

	while(count--)
	{
		if(!count)
		{
			*data++ = TM_I2C_ReadNack(I2Cx);
		}
		else
			*data++ = TM_I2C_ReadAck(I2Cx);
	}
}

float pressure()
{
	uint8_t coef_reg[MPL_BYTES_TO_READ_COEF];
	uint8_t data_reg[MPL_BYTES_TO_READ_DATA];
	float A0, B1, B2, C12;   // Coefficents
	float pres, t_adc;

	i2c_conf(TM_I2C_CLOCK_STANDARD);

	TM_I2C_ReadMulti(I2C2, MPL_ADDR, MPL_COEF_REG, coef_reg, MPL_BYTES_TO_READ_COEF);

	A0 = (coef_reg[0] * 256 + coef_reg[1]) / 8.0;
	B1 = (coef_reg[2] * 256 + coef_reg[3]);
	if(B1 > 32767)
		B1 -= 65536;
	B1 = B1 / 8192.0;
	B2 = (coef_reg[4] * 256 + coef_reg[5]);
	if(B2 > 32767)
		B2 -= 65535;
	B2 = B2 / 16384.0;
	C12 = ((coef_reg[6] * 256 + coef_reg[7]) / 4) / 4194304.0;

	TM_I2C_Write(I2C2, MPL_ADDR, MPL_CONVERT_REG, 0x00);

	delay_ms(500);

	TM_I2C_ReadMulti(I2C2, MPL_ADDR, MPL_DATA_REG, data_reg, MPL_BYTES_TO_READ_DATA);

	pres = ((data_reg[0] * 256) + (data_reg[1] & 0xC0)) / 64;
	t_adc = ((data_reg[2] * 256) + (data_reg[3] & 0xC0)) / 64;

	pres = A0 + (B1 + C12 * t_adc) * pres + B2 * t_adc;

	pres = ((65.0 / 1023.0) * pres + 50) * 10;

	return pres;
}

struct adxl accelerometer()
{
	uint8_t data_reg[ADXL_BYTES_TO_READ];
	uint8_t dummy_reg = TM_I2C_Read(I2C2, ADXL_ADDR, ADXL_DUMMY_REG);
	float x, y, z;
	struct adxl degrees;

	i2c_conf(TM_I2C_CLOCK_STANDARD);

	while(dummy_reg != 0xE5)
	{
		i2c_conf(TM_I2C_CLOCK_STANDARD);
		dummy_reg = TM_I2C_Read(I2C2, ADXL_ADDR, ADXL_DUMMY_REG);
	}

	TM_I2C_Write(I2C2, ADXL_ADDR, ADXL_DATA_FORMAT, 0x04);
	TM_I2C_Write(I2C2, ADXL_ADDR, ADXL_POWER_CTL, 0x08);

	TM_I2C_ReadMulti(I2C2, ADXL_ADDR, ADXL_DATA_REG, data_reg, ADXL_BYTES_TO_READ);

	x = ((data_reg[1]<<8 | data_reg[0]) - 32768) * 0.061;
	y = ((data_reg[3]<<8 | data_reg[2]) - 32768) * 0.061;
	z = ((data_reg[5]<<8 | data_reg[4]) - 32768) * 0.061;

	degrees.x_deg = atan(y/z) / pi * 180;
	degrees.y_deg = atan(x/z) / pi * 180;
	degrees.z_deg = atan(y/x) / pi * 180;

	return degrees;
}

float sound()
{
	uint16_t sound_adc;
	uint16_t max_amplitude = 0, min_amplitude = 4095;
	uint8_t counter = 0;
	float sound_db;

	while(counter < 100)
	{
		sound_adc = TM_ADC_Read(ADC1, ADC_Channel_10);

		if(sound_adc > max_amplitude)
			max_amplitude = sound_adc;
		else if(sound_adc < min_amplitude)
			min_amplitude = sound_adc;

		counter++;
	}
	counter = 0;

	sound_db = 20 * log10(6.25 * (max_amplitude-min_amplitude));

	return sound_db;
}

uint16_t co2_density()
{
	uint8_t data_reg[8];
	uint8_t dummy_reg;
	uint8_t status_reg;
	uint8_t reset_key[]={0x11, 0xE5, 0x72, 0x8A};
	bool ccs_sleep = true;
	uint16_t eco2;

	i2c_conf(TM_I2C_CLOCK_STANDARD/10);

//	TM_I2C_WriteMulti(I2C2, CCS_ADDR, CCS_SW_RESET, reset_key, 4);

	dummy_reg = TM_I2C_Read(I2C2, CCS_ADDR, CCS_HW_ID);
	if(dummy_reg != CCS_DEVICE_ID)
	{
		i2c_conf(TM_I2C_CLOCK_STANDARD/10);
		return ERROR;
	}

	status_reg = TM_I2C_Read(I2C2, CCS_ADDR, CCS_STATUS_REG);
	if(!(status_reg & 0x10))
		return ERROR;

	uint8_t err_code = TM_I2C_Read(I2C2, CCS_ADDR, CCS_ERR_ID);

	TM_I2C_WriteNoRegister(I2C2, CCS_ADDR, CCS_APP_START);
	TM_I2C_Write(I2C2, CCS_ADDR, CCS_MEAS_REG, 0x40);

	delay_ms(1000);

	while(!(status_reg = TM_I2C_Read(I2C2, CCS_ADDR, CCS_STATUS_REG) & 0x80));

	status_reg = TM_I2C_Read(I2C2, CCS_ADDR, CCS_STATUS_REG);

	if(!(status_reg & 0x80))
		return ERROR;

	delay_ms(1000);
	TM_I2C_ReadMulti(I2C2, CCS_ADDR, CCS_ALG_RESULT, data_reg, 4);
	eco2 = (data_reg[0] << 8) + data_reg[1];

	return eco2;

//	TM_I2C_WriteMulti(I2C2, CCS_ADDR, CCS_SW_RESET, reset_key, 4);
}

float dust()
{
	char received_data[SDS_BYTES_TO_READ];
	uint16_t check_sum;
	float pm_25;

	TM_USART_Send(UART4, SDS011_CMD_MODE, SDS_BYTES_TO_SEND);
	delay_ms(SDS_WORKTIME_MS);

	TM_USART_Send(UART4, SDS011_CMD_QUERY, SDS_BYTES_TO_SEND);
	delay_ms(100);

	TM_USART_Gets(UART4, received_data, SDS_BYTES_TO_READ);

	check_sum = (received_data[2]+received_data[3]+received_data[4]+received_data[5]+received_data[6]+received_data[7]) % 256;

	if(check_sum != received_data[8])
	{
		TM_USART_ClearBuffer(UART4);

		for(int i = 0; i <= SDS_BYTES_TO_READ; i++)
			received_data[i] = 0;

		return ERROR;
	}
	pm_25 = ((received_data[3] << 8) | received_data[2]) / 10.0;
	delay_ms(100);

	TM_USART_Send(UART4, SDS011_CMD_SLEEP, SDS_BYTES_TO_SEND);
	delay_ms(SDS_SLEEPTIME_MS);

	return pm_25;
}

struct sht temp_humid()
{
	uint8_t temp_raw[SHT_BYTES_TO_READ], humid_raw[SHT_BYTES_TO_READ];
	struct sht temp_humi;

	i2c_conf(TM_I2C_CLOCK_STANDARD);

//	TM_I2C_WriteNoRegister(I2C2, SHT_ADDR, SHT_TRIG_HUMID_HOLD_CMD);  // ??
	TM_I2C_ReadMulti_SHT(I2C2, SHT_ADDR, SHT_TRIG_HUMID_HOLD_CMD, humid_raw, SHT_BYTES_TO_READ, SHT_HUMID_DELAYTIME);

	TM_I2C_ReadMulti_SHT(I2C2, SHT_ADDR, SHT_TRIG_TEMP_HOLD_CMD, temp_raw, SHT_BYTES_TO_READ, SHT_TEMP_DELAYTIME);

	temp_humi.temp = (((temp_raw[0] << 8) | temp_raw[1]) / 65536.0 * 175.72) - 46.85;
	temp_humi.humid = (((humid_raw[0] << 8) | humid_raw[1]) / 65536.0 * 125) - 6;

//	if(temp_humid.temp < 0 || temp_humid.humid < 0)
//		return temp_humid;

	return temp_humi;
}

int main()
{
	SystemInit();
	onboard_leds_conf();
	adc_conf();
	uart_conf(UART4, 9600);
	emergency_states_gpio_and_irq_conf();

	while(1)
	{
		accelerometer();
		pressure();
		co2_density();
		sound();
		temp_humid();
		dust();
	}
}

void EXTI9_5_IRQHandler()
{
	SysTick_Config(SystemCoreClock/1000);
	uint32_t current_ticks;

	if(EXTI_GetITStatus(EXTI_Line5))
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
		{
			current_ticks = SysTick->VAL;

			while((SysTick->VAL - current_ticks) <= 500);

			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
				TM_GPIO_TogglePinValue(GPIOD, GPIO_Pin_12);
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	else if(EXTI_GetITStatus(EXTI_Line6))
	{
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
//		{
//			current_ticks = SysTick->VAL;
//
//			while((SysTick->VAL - current_ticks) <= 300);
//
//			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
				TM_GPIO_TogglePinValue(GPIOD, GPIO_Pin_13);
//		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	else if(EXTI_GetITStatus(EXTI_Line7))
	{
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
//		{
//			current_ticks = SysTick->VAL;
//
//			while((SysTick->VAL - current_ticks) <= 300);
//
//			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
				TM_GPIO_TogglePinValue(GPIOD, GPIO_Pin_14);
//		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}
