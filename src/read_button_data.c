// read data from the button 
// in initialization, default is false 
// store the button 

#include <stdio.h>
#include "stm32l4xx_hal.h"

#include "data.h"

GPIO_InitTypeDef GPIO_InitStruct = {0};

// configures GPIO pin as pullup and input 
static void MX_GPIO_Init(void)
{
  // enable the GPIOB peripheral clock 
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // configure PB1 as falling edge interrupt w pullup
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; 
  GPIO_InitStruct.Pull = GPIO_PULLUP;        
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn); // enable EXTI0 in NVIC 
}


void callback(void){
    HAL_GPIO_EXTI_Callback();
}

// connect the GPIO pin to EXTI
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1); // clear hardware interrupt flag 
}
