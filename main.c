#include <stdint.h>
#include "stm32l4xx.h"

#define AF7 (0b0111)

#define SYS_FREQ (16000000)
#define APB1_CLK (SYS_FREQ)

#define UART_BAUDRATE (115200)
// User manual, 7.6 LED is on PA5 (pin21)
// User manual, 7.7 Button on PC13 (pin2); button is active low

enum GPIO_MODE {
  GPIO_INPUT = 0b00,
  GPIO_OUTPUT = 0b01,
  GPIO_ALTERNATE = 0b10
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

static uint16_t compute_uart_div(uint32_t periphClk, uint32_t baudrate_goal) {
  return ((periphClk +baudrate_goal/2U)/baudrate_goal);
}

static void uart_set_baudrate(USART_TypeDef* usartx, uint32_t periphclk, uint32_t baudrate) {
  usartx->BRR = compute_uart_div(periphclk, baudrate);
}

void uart2_write(uint8_t ch) {
  // check if empty transmit register;
  //  and then write
  while(!(USART2->ISR & USART_ISR_TXE)) {
  }
  USART2->TDR = (uint8_t) (ch & 0xFF);
}

void main(void)
{
  // enable clock for GPIO A, GPIO C
  RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOAEN_Pos);
  RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOCEN_Pos);

  // Set up the LED 
  GPIOA->MODER = ((GPIO_OUTPUT << GPIO_MODER_MODE5_Pos) | ~(GPIO_MODER_MODE5_Msk)) & GPIOA->MODER;
  GPIOA->OTYPER = ((GPIO_DRAIN << GPIO_OTYPER_OT5_Pos) | ~(GPIO_OTYPER_OT5_Msk)) & GPIOA->OTYPER;
  GPIOA->PUPDR = ((GPIO_PD << GPIO_PUPDR_PUPD5_Pos) | ~(GPIO_PUPDR_PUPD5_Msk)) & GPIOA->PUPDR;
 
  // Set up the button 
  GPIOC->MODER = ((GPIO_INPUT << GPIO_MODER_MODE13_Pos) | ~(GPIO_MODER_MODE13_Msk)) & GPIOC->MODER;
  GPIOC->PUPDR = ((GPIO_PU << GPIO_PUPDR_PUPD13_Pos) | ~(GPIO_PUPDR_PUPD13_Msk)) & GPIOC->PUPDR;
 
  /* Set up USART2: Ref Table 17 in datasheet, Port D, set them all to alternate function mode
   * pin 3: CTS
   * pin 4: RTS_DE
   * pin 5: TX
   * pin 6: RX
   * pin 8: CK (clock)
   */
  GPIOD->MODER = ((GPIO_ALTERNATE << GPIO_MODER_MODE5_Pos) | ~(GPIO_MODER_MODE2_Msk)) & GPIOD->MODER;

  //  Define which alternate function mode AF7 (0b0111);
  GPIOD->AFR[0] = ((AF7 << GPIO_AFRL_AFSEL5_Pos) | ~(GPIO_AFRL_AFSEL5_Msk)) & GPIOD->AFR[0];
 
  // set the baudrate of the USART connection
  uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

  // Enable transmission for USART2 (TE = 1)
  USART2->CR1 |= (0x1UL << USART_CR1_TE_Pos); 

  // Enable clock for USART2 (MUST BE DONE LAST, AFTER CONFIGURING USART) (UE = 1)
  RCC->APB1ENR1 |= (1 << RCC_APB1ENR1_USART2EN_Pos);

  while(1) {
    if(GPIOC->IDR & GPIO_IDR_ID13_Msk) {
      GPIOA->BSRR = GPIO_BSRR_BR5;
    } else {
      GPIOA->BSRR = GPIO_BSRR_BS5;
    }
    uart2_write('y');
  }

}


