#include "stdint.h"

typedef enum {false=0, true=1} bool;

struct sht{
	float temp;
	float humid;
};

struct adxl{
	float x_deg;
	float y_deg;
	float z_deg;
};

#define ERROR false
#define pi 3.14

/* STM32F4 Pin Defines */
#define I2C_SDA GPIO_Pin_11
#define I2C_SCL GPIO_Pin_10
#define CCS_WAKE_PIN GPIO_Pin_9

/* Sensor Register Defines
*******************************/

/* ADXL345 Accelerometer */
#define ADXL_ADDR 0x53<<1
#define ADXL_DUMMY_REG 0x00
#define ADXL_DATA_FORMAT 0x31
#define ADXL_POWER_CTL 0x2D
#define ADXL_DATA_REG 0x32
#define ADXL_BYTES_TO_READ 6

/* MPL115A2 Air Pressure */
#define MPL_ADDR 0x60<<1
#define MPL_DATA_REG 0x00
#define MPL_COEF_REG 0x04
#define MPL_CONVERT_REG 0x12
#define MPL_BYTES_TO_READ_COEF 8
#define MPL_BYTES_TO_READ_DATA 4

/* SHT20 Temperature & Humidity Sensor */
#define SHT_ADDR 0x40<<1
#define SHT_TRIG_TEMP_HOLD_CMD 0xE3
#define SHT_TRIG_HUMID_HOLD_CMD 0xE5
#define SHT_TRIG_TEMP_NOHOLD_CMD 0xF3
#define SHT_TRIG_HUMID_NOHOLD_CMD 0xF5
#define SHT_WRITE_USR_REG_CMD 0xE6
#define SHT_READ_USR_REG_CMD 0xE7
#define SHT_RESET_CMD 0xFE
#define SHT_BYTES_TO_READ 2
#define SHT_TEMP_DELAYTIME 85
#define SHT_HUMID_DELAYTIME 29

/* CCS811 CO2&TVOC Sensor */
#define CCS_ADDR 0x5A<<1
#define CCS_STATUS_REG 0x00
#define CCS_MEAS_REG 0X01
#define CCS_ALG_RESULT 0x02
#define CCS_DATA_REG_RAW 0x03
#define CCS_DATA_REG_ENV 0x05
#define CCS_NTC_REG 0x06
#define CCS_THRS_REG 0x10
#define CCS_BASELINE 0x11
#define CCS_HW_ID 0x20
#define CCS_ERR_ID 0xE0
#define CCS_APP_START 0xF4
#define CCS_SW_RESET 0xFF

#define CCS_BYTES_TO_READ 8
#define CCS_DEVICE_ID 0x81

/* Nova PM SDS011 Dust Sensor */
#define SDS_BYTES_TO_READ 11
#define SDS_BYTES_TO_SEND 19
#define SDS_WORKTIME_MS 3000
#define SDS_SLEEPTIME_MS 3000

uint8_t SDS011_CMD_MODE[19] = {
    0xAA, // head
    0xB4, // command id
    0x02, // data byte 1
    0x01, // data byte 2, to be changed (default: set mode)
    0x00, // data byte 3, to be changed (default: active mode)
    0x00, // data byte 4
    0x00, // data byte 5
    0x00, // data byte 6
    0x00, // data byte 7
    0x00, // data byte 8
    0x00, // data byte 9
    0x00, // data byte 10
    0x00, // data byte 11
    0x00, // data byte 12
    0x00, // data byte 13
    0xFF, // data byte 14 (device id byte 1)
    0xFF, // data byte 15 (device id byte 2)
    0x01, // checksum (change if data bytes change)
    0xAB  // tail
};

// Query data command
uint8_t SDS011_CMD_QUERY[19] = {
    0xAA, // head
    0xB4, // command id
    0x04, // data byte 1
    0x00, // data byte 2
    0x00, // data byte 3
    0x00, // data byte 4
    0x00, // data byte 5
    0x00, // data byte 6
    0x00, // data byte 7
    0x00, // data byte 8
    0x00, // data byte 9
    0x00, // data byte 10
    0x00, // data byte 11
    0x00, // data byte 12
    0x00, // data byte 13
    0xFF, // data byte 14 (device id byte 1)
    0xFF, // data byte 15 (device id byte 2)
    0x02, // checksum (change if data bytes change)
    0xAB  // tail
};

// Set Device ID
uint8_t SDS011_CMD_DEVID[19] = {
    0xAA, // head
    0xB4, // command id
    0x05, // data byte 1
    0x00, // data byte 2
    0x00, // data byte 3
    0x00, // data byte 4
    0x00, // data byte 5
    0x00, // data byte 6
    0x00, // data byte 7
    0x00, // data byte 8
    0x00, // data byte 9
    0x00, // data byte 10
    0x00, // data byte 11
    0x34, // data byte 12 (new device id byte 1)
    0x12, // data byte 13 (new device id byte 2)
    0xFF, // data byte 14 (device id byte 1)
    0xFF, // data byte 15 (device id byte 2)
    0x49, // checksum (change if data bytes change)
    0xAB  // tail
};

// Set sleep and work
uint8_t SDS011_CMD_SLEEP[19] = {
    0xAA, // head
    0xB4, // command id
    0x06, // data byte 1
    0x01, // data byte 2, to be changed (default: set mode)
    0x00, // data byte 3, to be changed (default: sleep)
    0x00, // data byte 4
    0x00, // data byte 5
    0x00, // data byte 6
    0x00, // data byte 7
    0x00, // data byte 8
    0x00, // data byte 9
    0x00, // data byte 10
    0x00, // data byte 11
    0x00, // data byte 12
    0x00, // data byte 13
    0xFF, // data byte 14 (device id byte 1)
    0xFF, // data byte 15 (device id byte 2)
    0x05, // checksum (change if data bytes change)
    0xAB  // tail
};

// Set working period
uint8_t SDS011_CMD_PERIOD[19] = {
    0xAA, // head
    0xB4, // command id
    0x08, // data byte 1
    0x01, // data byte 2, to be changed (default: set mode)
    0x00, // data byte 3, to be changed (default: continous)
    0x00, // data byte 4
    0x00, // data byte 5
    0x00, // data byte 6
    0x00, // data byte 7
    0x00, // data byte 8
    0x00, // data byte 9
    0x00, // data byte 10
    0x00, // data byte 11
    0x00, // data byte 12
    0x00, // data byte 13
    0xFF, // data byte 14 (device id byte 1)
    0xFF, // data byte 15 (device id byte 2)
    0x07, // checksum (change if data bytes change)
    0xAB  // tail
};

// Check firmware version
uint8_t SDS011_CMD_FWVER[19] = {
    0xAA, // head
    0xB4, // command id
    0x07, // data byte 1
    0x00, // data byte 2
    0x00, // data byte 3
    0x00, // data byte 4
    0x00, // data byte 5
    0x00, // data byte 6
    0x00, // data byte 7
    0x00, // data byte 8
    0x00, // data byte 9
    0x00, // data byte 10
    0x00, // data byte 11
    0x00, // data byte 12
    0x00, // data byte 13
    0xFF, // data byte 14 (device id byte 1)
    0xFF, // data byte 15 (device id byte 2)
    0x05, // checksum (change if data bytes change)
    0xAB  // tail
};
