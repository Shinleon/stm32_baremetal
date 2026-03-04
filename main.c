#include <stdint.h>
#include "stm32l4xx.h"

#define LED_PIN (5)

void main(void)
{
  RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOAEN_Pos);

  GPIOA->MODER = ((0b01 << GPIO_MODER_MODE5_Pos) | ~(GPIO_MODER_MODE5_Msk)) & GPIOA->MODER;
  
  while(1) {
    GPIOA->ODR ^= (1 << LED_PIN);
    for(uint32_t i = 0; i < 100000; i++);
  }

}


