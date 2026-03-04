#include <stdint.h>

#define PERIPHERAL_BASE (0x40000000UL)

// AHB1
#define AHB1_OFFSET (0x00020000UL)
#define AHB1_BASE (PERIPHERAL_BASE + AHB1_OFFSET)

// AHB2 
#define AHB2_OFFSET (0x08000000UL)
#define AHB2_BASE (PERIPHERAL_BASE + AHB2_OFFSET)

// GPIO, located in AHB2
#define GPIOA_OFFSET (0x0U)
#define GPIOA_BASE (AHB2_BASE + GPIOA_OFFSET)

// RCC, located in AHB1 
#define RCC_OFFSET (0x1000UL)
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)

// RCC periph clock enable register, for GPIO A (0: disabled, 1:enabled)
#define RCC_AHB2ENR_OFFSET (0x4CU)
#define RCC_AHB2ENR ((volatile uint32_t*) (RCC_BASE + RCC_AHB2ENR_OFFSET))
#define RCC_AHB2ENR_GPIOAEN (0U)

// GPIO port mode registers
#define GPIO_MODER_OFFSET (0x0U)
#define GPIOA_MODER ((volatile uint32_t*) (GPIOA_BASE + GPIO_MODER_OFFSET))
#define GPIO_MODER_MODER5 (10U)

// GPIO output data register (read/write to pin)
#define GPIO_ODR_OFFSET (0x14U)
#define GPIOA_ODR ((volatile uint32_t*) (GPIOA_BASE + GPIO_ODR_OFFSET))

#define LED_PIN (5)

void main(void)
{
  *RCC_AHB2ENR |= (1 << RCC_AHB2ENR_GPIOAEN);

  volatile uint32_t dummy;
  dummy = *RCC_AHB2ENR;
  dummy = *RCC_AHB2ENR;
  *GPIOA_MODER |= (1 << GPIO_MODER_MODER5);
  *GPIOA_MODER &= ~(1 << (GPIO_MODER_MODER5 + 1));

  while(1) {
    *GPIOA_ODR ^= (1 << LED_PIN);
    for(uint32_t i = 0; i < 1000000; i++);
  }

}
