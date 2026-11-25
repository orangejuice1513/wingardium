/**
* @file  : read_imu_data.c
* @author: julia jiang
* @brief : gets raw accel and gyro values from imu thru i2c and saves it to the raw data struct @ 60Hz for now 
* 
* params up for change: frequency of data saving
* f_ADC = 80 MHz pg 214 
* f_CPU = 4MHz default pg 39 
*/

#include <stdint.h>
#include "stm32l4xx_hal.h"

// device addresses 
#define IMU_ADDR         0b11010101 // pg 18 of imu datasheet. last bit is 1 for read
#define WHO_AM_I         0x0F // pg 43 of imu datasheet 

// configuration structures 
I2C_HandleTypeDef hi2c1
GPIO_InitTypeDef GPIO_InitStruct = {0};

// SDA = PF0
// SCL = PF1

// initialize gpio f clock (i2c pins) and i2c clock 
init_clock(){
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_I2C2_CLK_ENABLE();
}

// initialize the scl and sda pins 
init_pins(){
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;      // PF1 = SCL, PF0 = SDA
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; // open drain cause i2c
    GPIO_InitStruct.Pull = GPIO_PULLUP; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2; // Connects PF0 and PF1 to I2C2 inside the chip
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

// initialize the i2c peripheral 
init_i2c(){
    hi2c1.Instance = I2C2;
    hi2c.Init.Timing = 0x00000E14; // Example: 100 kHz Standard Mode at 4 MHz clock
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    HAL_I2C_Init(&hi2c1);
}

init_everything_imu(){
    init_clock();
    init_pins();
    init_i2c();
}

read_imu_data(){
    HAL_I2C_Mem_Read(&hi2c1, (0x68 << 1), 0x00, I2C_MEMADD_SIZE_8BIT, sensor_data, 2, 100);
}