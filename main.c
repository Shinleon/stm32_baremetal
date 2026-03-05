#include <stdint.h>
#include "stm32l4xx.h"

// User manual, 7.6 LED is on PA5 (pin21)
// User mnaul, 7.7 Button on PC13 (pin2); button is active low
#define LED_PIN (5)
#define USR_BUTTON (13)

enum GPIO_MODE {
  GPIO_INPUT = 0b00,
  GPIO_OUTPUT = 0b01
};

enum GPIO_PUPDR {
  GPIO_NOPUPD = 0b00,
  GPIO_PU = 0b01,
  GPIO_PD = 0b10
};

enum GPIO_OUTTYPE {
  GPIO_PUSHPULL = 0b0,
  GPIO_DRAIN = 0b1
};

void main(void)
{
  RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOAEN_Pos);
  RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOCEN_Pos);

  GPIOA->MODER = ((GPIO_OUTPUT << GPIO_MODER_MODE5_Pos) | ~(GPIO_MODER_MODE5_Msk)) & GPIOA->MODER;
  GPIOA->OTYPER = ((GPIO_DRAIN << GPIO_OTYPER_OT5_Pos) | ~(GPIO_OTYPER_OT5_Msk)) & GPIOA->OTYPER;
  GPIOA->PUPDR = ((GPIO_PD << GPIO_PUPDR_PUPD5_Pos) | ~(GPIO_PUPDR_PUPD5_Msk)) & GPIOA->PUPDR;
  
  GPIOC->MODER = ((GPIO_INPUT << GPIO_MODER_MODE13_Pos) | ~(GPIO_MODER_MODE13_Msk)) & GPIOC->MODER;
  GPIOC->PUPDR = ((GPIO_PU << GPIO_PUPDR_PUPD13_Pos) | ~(GPIO_PUPDR_PUPD13_Msk)) & GPIOC->PUPDR;
  
  while(1) {
    if(GPIOC->IDR & GPIO_IDR_ID13_Msk) {
      GPIOA->BSRR = GPIO_BSRR_BR5;
    } else {
      GPIOA->BSRR = GPIO_BSRR_BS5;
    }
  }

}


