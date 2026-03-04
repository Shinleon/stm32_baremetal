#include <stdint.h>
#include "stm32l4xx.h"

#define LED_PIN (5)

void main(void)
{
  RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOAEN_Pos);

  GPIOA->MODER |= (1 << GPIO_MODER_MODE5_Pos);

  while(1) {
    for(uint32_t i = 0; i < 1000000; i++);
    GPIOA->ODR |= (1 << LED_PIN);
  }

}
