#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

GPIO_InitTypeDef gpioX;
USART_InitTypeDef usartX;
SPI_InitTypeDef spiX;

char data_array[50];
int x, y;

void usart_inits(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	gpioX.GPIO_Mode = GPIO_Mode_AF;
	gpioX.GPIO_Pin = GPIO_Pin_2;
	gpioX.GPIO_OType = GPIO_OType_PP;
	gpioX.GPIO_Speed = GPIO_Speed_100MHz;
	gpioX.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpioX);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

	usartX.USART_BaudRate = 9600;
	usartX.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartX.USART_Mode = USART_Mode_Tx;
	usartX.USART_Parity = USART_Parity_No;
	usartX.USART_StopBits = USART_StopBits_1;
	usartX.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &usartX);

	USART_Cmd(USART2, ENABLE);
}

void usart_send(USART_TypeDef* USARTx, volatile char *data)
{
	while(*data)
	{
		while(!(USARTx->SR & 0x00000040));
		USART_SendData(USARTx, *data);
		*data++;
	}
}

void spi_inits(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	gpioX.GPIO_Mode = GPIO_Mode_OUT;
	gpioX.GPIO_OType = GPIO_OType_PP;
	gpioX.GPIO_Pin = GPIO_Pin_3;
	gpioX.GPIO_Speed = GPIO_Speed_100MHz;
	gpioX.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &gpioX);

	gpioX.GPIO_Mode = GPIO_Mode_AF;
	gpioX.GPIO_OType = GPIO_OType_PP;
	gpioX.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpioX.GPIO_Speed = GPIO_Speed_100MHz;
	gpioX.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioX);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	spiX.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	spiX.SPI_DataSize = SPI_DataSize_8b;
	spiX.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spiX.SPI_Mode = SPI_Mode_Master;
	spiX.SPI_FirstBit = SPI_FirstBit_MSB;
	spiX.SPI_CPHA = SPI_CPHA_2Edge;
	spiX.SPI_CPOL = SPI_CPOL_High;
	spiX.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &spiX);

	GPIO_SetBits(GPIOE, GPIO_Pin_3);

	SPI_Cmd(SPI1, ENABLE);
}

uint16_t spi_read(uint8_t address)
{
	address = address | 0x80;

	GPIO_ResetBits(GPIOE, GPIO_Pin_3);

	while(!(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE)));
	SPI_I2S_SendData(SPI1, address);
	while(!(SPI1->SR & 0x0002));
	SPI_I2S_ReceiveData(SPI1);
	while((SPI1->SR & 0x0001));

	GPIO_SetBits(GPIOE, GPIO_Pin_3);

	return SPI_I2S_ReceiveData(SPI1);
}

int main()
{
	usart_inits();
	spi_inits();

	while(1)
	{
		x = spi_read(0x29);
		y = spi_read(0x2B);
		sprintf(data_array, "x=%d , y=%d\n", x, y);
		usart_send(USART2, data_array);
	}
}
