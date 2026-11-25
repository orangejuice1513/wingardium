/**
* @file  : read_joystick_data.c
* @author: julia jiang
* @brief : gets raw x and y values from the joystick through ADC pins and saves it to the raw data struct @ 60Hz for now
* using a 12 bit ADC rn for funsies 
* 
* params up for change: frequency of data saving, ADC bit resolution  
* f_ADC = 80 MHz pg 214 
* f_CPU = 4MHz default pg 39 
*/

#include <stdio.h>
#include "stm32l4xx_hal.h"

#include "data_struct.h"

// nucleo pins A0 = PA3, A1 = PC0 (both in ADC1)
GPIO_InitTypeDef GPIOA_InitStruct = {0};
GPIO_InitTypeDef GPIOC_InitStruct = {0};

// for the adc
ADC_HandleTypeDef hadc1; 
ADC_ChannelConfTypeDef sConfig = {0};


// initialize clock for GPIOA, GPIOC, and ADC bus  
void init_clock(){
    // enable GPIO port clocks 
    __HAL_RCC_GPIOA_CLK_ENABLE(); // for PA3
    __HAL_RCC_GPIOC_CLK_ENABLE(); // for PC0

    // enable ADC peripheral clock 
    __HAL_RCC_ADC1_CLK_ENABLE();
}

// initialize GPIO pins to be analog and no pulldown 
void init_pins(){
    // initialize PA3
    GPIOA_InitStruct.Pin = GPIO_PIN_3;
    GPIOA_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIOA_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIOA_InitStruct);

    // initialize PC0
    GPIOC_InitStruct.Pin = GPIO_PIN_0
    GPIOC_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIOC_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIOC_InitStruct);
}

// configure the actual adc peripheral 
void init_adc(){
    hadc1.Instance = ADC1;

    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1; // give the ADC the 4 MHz 
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT; // Standard right-aligned integer
    hadc1.Init.ScanConvMode          = ENABLE; // reading 2 pins 
    hadc1.Init.ContinuousConvMode    = DISABLE; // DISABLE = 1 reading per trigger; ENABLE = loop forever
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START; 
    hadc1.Init.NbrOfConversion       = 2; // 2 pins 
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV; //

    HAL_ADC_Init(&hadc1); // applies the settings
}

void init_adc_channels(){
    // shared by both channels 
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED; // measure relative to gnd 
    sConfig.OffsetNumber = ADC_OFFSET_NONE; // no offset from the adc read 
    sConfig.Offset       = 0;

    // PA3 (ADC_CHANNEL_8) pg 84 
    sConfig.Channel = ADC_CHANNEL_5;
    sConfig.Rank    = ADC_REGULAR_RANK_1;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    // PC0 (ADC_CHANNEL_1) pg 81 
    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

void init_adc_everything(){
    init_clock();
    init_pins();
    init_adc();
    init_adc_channels();
}

void save_adc_data(){
    HAL_ADC_Start(&hadc1);
    
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK){ // wait for adc to be done measuring
        data_struct.raw_data.xin = HAL_ADC_GetValue(&hadc1); // grab the value 
    }
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        data_struct.raw_data.yin = HAL_ADC_GetValue(&hadc1); 
    }
    
    HAL_ADC_Stop(&hadc1); 
}