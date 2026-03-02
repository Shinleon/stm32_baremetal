/* The goal of startup.c is to
 * 1) initialize main stack pointer
 * 2) initialize interrupt vector table 
 * 3) copy `.data` from flash to SRAM
 * 4) zero-fill `.bss` section
 *    `.bss` is "Block Started by Symbol"
 */
#include <stdint.h>

#define SRAM_START (0x20000000U)
#define SRAM_SIZE  (96U * 1024U)
#define SRAM_SEND  (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)
#define ISR_VECTOR_SIZE_WORDS (98)

void reset_handler(void);
void default_handler(void);

// Cortex-M system exceptions
void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handle(void) __attribute__((weak, alias("default_handler")));
void memMngr_fault_handle(void) __attribute__((weak, alias("default_handler")));
void bus_fault_handle(void) __attribute__((weak, alias("default_handler")));
void usage_fault_handle(void) __attribute__((weak, alias("default_handler")));
void svcall_handle(void) __attribute__((weak, alias("default_handler")));
void debug_monitor_handle(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));

void wwdg_handler(void) __attribute__((weak, alias("default_handler")));
void pvd_handler(void) __attribute__((weak, alias("default_handler")));
void exti19_tamp_stamp_handler(void) __attribute__((weak, alias("default_handler")));
void exti20_rtc_wkup_handler(void) __attribute__((weak, alias("default_handler")));
void flash_handler(void) __attribute__((weak, alias("default_handler")));
void rcc_handler(void) __attribute__((weak, alias("default_handler")));
void exti0_handler(void) __attribute__((weak, alias("default_handler")));
void exti1_handler(void) __attribute__((weak, alias("default_handler")));
void exti2_handler(void) __attribute__((weak, alias("default_handler")));
void exti3_handler(void) __attribute__((weak, alias("default_handler")));
void exti4_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_ch1_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_ch2_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_ch3_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_ch4_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_ch5_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_ch6_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_ch7_handler(void) __attribute__((weak, alias("default_handler")));
void adc_handler(void) __attribute__((weak, alias("default_handler")));
void can1_TX_handler(void) __attribute__((weak, alias("default_handler")));
void can1_RX0_handler(void) __attribute__((weak, alias("default_handler")));
void can1_RX1_handler(void) __attribute__((weak, alias("default_handler")));
void can1_SCE_handler(void) __attribute__((weak, alias("default_handler")));
void exti9_5_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_brk_tim15_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_up_tim16_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_trg_com_tim17_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_cc_handler(void) __attribute__((weak, alias("default_handler")));
void tim2_handler(void) __attribute__((weak, alias("default_handler")));
void tim3_handler(void) __attribute__((weak, alias("default_handler")));
void tim4_handler(void) __attribute__((weak, alias("default_handler")));
void i2c1_ev_handler(void) __attribute__((weak, alias("default_handler")));
void i2c1_er_handler(void) __attribute__((weak, alias("default_handler")));
void i2c2_ev_handler(void) __attribute__((weak, alias("default_handler")));
void i2c2_er_handler(void) __attribute__((weak, alias("default_handler")));
void spi1_handler(void) __attribute__((weak, alias("default_handler")));
void spi2_handler(void) __attribute__((weak, alias("default_handler")));
void usart1_handler(void) __attribute__((weak, alias("default_handler")));
void usart2_handler(void) __attribute__((weak, alias("default_handler")));
void exti15_10_handler(void) __attribute__((weak, alias("default_handler")));
void exti18_rtc_alarm_handler(void) __attribute__((weak, alias("default_handler")));
void dfsdm1_flt3_handler(void)  __attribute__((weak, alias("default_handler")));
void tim8_brk_handler(void)  __attribute__((weak, alias("default_handler")));
void tim8_up_handler(void) __attribute__((weak, alias("default_handler")));
void tim8_trg_com_handler(void) __attribute__((weak, alias("default_handler")));
void tim8_cc_handler(void) __attribute__((weak, alias("default_handler"))); // tim 8 capture/compare
void adc3_handler(void) __attribute__((weak, alias("default_handler")));
void fmc_handler(void) __attribute__((weak, alias("default_handler")));
void sdmmc1_handler(void) __attribute__((weak, alias("default_handler")));
void tim5_handler(void) __attribute__((weak, alias("default_handler")));
void spi3_handler(void) __attribute__((weak, alias("default_handler")));
void uart4_handler(void) __attribute__((weak, alias("default_handler")));
void uart5_handler(void) __attribute__((weak, alias("default_handler")));
void tim6_dacunder_handler(void) __attribute__((weak, alias("default_handler")));
void tim7_handler(void) __attribute__((weak, alias("default_handler")));
void dma2_ch1_handler(void) __attribute__((weak, alias("default_handler")));
void dma2_ch2_handler(void) __attribute__((weak, alias("default_handler")));
void dma2_ch3_handler(void) __attribute__((weak, alias("default_handler")));
void dma2_ch4_handler(void) __attribute__((weak, alias("default_handler")));
void dma2_ch5_handler(void) __attribute__((weak, alias("default_handler")));
void dfsdm1_flt0_handler(void) __attribute__((weak, alias("default_handler")));
void dfsdm1_flt1_handler(void) __attribute__((weak, alias("default_handler")));
void dfsdm1_flt2_handler(void) __attribute__((weak, alias("default_handler")));
void comp_handler(void) __attribute__((weak, alias("default_handler")));
void lptim1_handler(void) __attribute__((weak, alias("default_handler")));
void lptim2_handler(void) __attribute__((weak, alias("default_handler")));
void otg_fs_handler(void) __attribute__((weak, alias("default_handler"))); // L4x5/L4x6 only
void dma2_ch6_handler(void) __attribute__((weak, alias("default_handler")));
void dma2_ch7_handler(void) __attribute__((weak, alias("default_handler")));
void lpuart1_handler(void) __attribute__((weak, alias("default_handler")));
void quadspi_handler(void) __attribute__((weak, alias("default_handler")));
void i2c3_ev_handler(void) __attribute__((weak, alias("default_handler")));
void i2c3_er_handler(void) __attribute__((weak, alias("default_handler")));
void sai1_handler(void) __attribute__((weak, alias("default_handler")));
void sai2_handler(void) __attribute__((weak, alias("default_handler")));
void swpmi1_handler(void) __attribute__((weak, alias("default_handler")));
void tsc_handler(void) __attribute__((weak, alias("default_handler")));
void lcd_handler(void) __attribute__((weak, alias("default_handler"))); // L4x6 only
void aes_handler(void) __attribute__((weak, alias("default_handler"))); //L486/L4A6 only
void rng_and_hash_handler(void) __attribute__((weak, alias("default_handler"))); // L4Ax only
void fpu_handler(void) __attribute__((weak, alias("default_handler"))); 
void hash_and_crs_handler(void) __attribute__((weak, alias("default_handler"))); // L4Ax only

// default 16 Cortex(R)-M4 interrupt channels come first, then, the 82 chip specific
//  fault interrupts follow: reference RM0351, section 13

uint32_t isr_vector[ISR_VECTOR_SIZE_WORDS] __attribute((section(".isr_vector"))) = {
  STACK_POINTER_INIT_ADDRESS,
  // Cortex-M system exceptions
  (uint32_t)&reset_handler,
  (uint32_t)&nmi_handler,
  (uint32_t)&hard_fault_handler,
  (uint32_t)&memMngr_fault_handler,
  (uint32_t)&bus_fault_handler,
  (uint32_t)&usage_fault_handler,
  0,
  0,
  0,
  0,
  (uint32_t)&svcall_handler,
  (uint32_t)&debug_monitor_handler,
  0,
  (uint32_t)&pendsv_handler,
  (uint32_t)&systick_handler,

  // STM32L47x interrupt handlers below VV
  /* watchdog */
  (uint32_t)&wwdg_handler(void),
  (uint32_t)&pvd_handler(void),
  (uint32_t)&exti19_tamp_stamp_handler(void),
  (uint32_t)&exti20_rtc_wkup_handler(void),
  (uint32_t)&flash_handler(void),
  (uint32_t)&rcc_handler(void),
  (uint32_t)&exti0_handler(void),
  (uint32_t)&exti1_handler(void),
  (uint32_t)&exti2_handler(void),
  (uint32_t)&exti3_handler(void),
  (uint32_t)&exti4_handler(void),
  (uint32_t)&dma1_ch1_handler(void),
  (uint32_t)&dma1_ch2_handler(void),
  (uint32_t)&dma1_ch3_handler(void),
  (uint32_t)&dma1_ch4_handler(void),
  (uint32_t)&dma1_ch5_handler(void),
  (uint32_t)&dma1_ch6_handler(void),
  (uint32_t)&dma1_ch7_handler(void),
  (uint32_t)&adc_handler(void),
  (uint32_t)&can1_TX_handler(void),
  (uint32_t)&can1_RX0_handler(void),
  (uint32_t)&can1_RX1_handler(void),
  (uint32_t)&can1_SCE_handler(void),
  (uint32_t)&exti9_5_handler(void),
  (uint32_t)&tim1_brk_tim15_handler(void),
  (uint32_t)&tim1_up_tim16_handler(void),
  (uint32_t)&tim1_trg_com_tim17_handler(void),
  (uint32_t)&tim1_cc_handler(void),
  (uint32_t)&tim2_handler(void),
  (uint32_t)&tim3_handler(void),
  (uint32_t)&tim4_handler(void),
  (uint32_t)&i2c1_ev_handler(void),
  (uint32_t)&i2c1_er_handler(void),
  (uint32_t)&i2c2_ev_handler(void),
  (uint32_t)&i2c2_er_handler(void),
  (uint32_t)&spi1_handler(void),
  (uint32_t)&spi2_handler(void),
  (uint32_t)&usart1_handler(void),
  (uint32_t)&usart2_handler(void),
  (uint32_t)&exti15_10_handler(void),
  (uint32_t)&exti18_rtc_alarm_handler(void),
  (uint32_t)&dfsdm1_flt3_handler(void),
  (uint32_t)&tim8_brk_handler(void),
  (uint32_t)&tim8_up_handler(void),
  (uint32_t)&tim8_trg_com_handler(void),
  (uint32_t)&tim8_cc_handler(void) ,// tim 8 capture/compare
  (uint32_t)&adc3_handler(void),
  (uint32_t)&fmc_handler(void),
  (uint32_t)&sdmmc1_handler(void),
  (uint32_t)&tim5_handler(void),
  (uint32_t)&spi3_handler(void),
  (uint32_t)&uart4_handler(void),
  (uint32_t)&uart5_handler(void),
  (uint32_t)&tim6_dacunder_handler(void),
  (uint32_t)&tim7_handler(void),
  (uint32_t)&dma2_ch1_handler(void),
  (uint32_t)&dma2_ch2_handler(void),
  (uint32_t)&dma2_ch3_handler(void),
  (uint32_t)&dma2_ch4_handler(void),
  (uint32_t)&dma2_ch5_handler(void),
  (uint32_t)&dfsdm1_flt0_handler(void),
  (uint32_t)&dfsdm1_flt1_handler(void),
  (uint32_t)&dfsdm1_flt2_handler(void),
  (uint32_t)&comp_handler(void),
  (uint32_t)&lptim1_handler(void),
  (uint32_t)&lptim2_handler(void),
  (uint32_t)&otg_fs_handler(void), // L4x5/L4x6 only
  (uint32_t)&dma2_ch6_handler(void),
  (uint32_t)&dma2_ch7_handler(void),
  (uint32_t)&lpuart1_handler(void),
  (uint32_t)&quadspi_handler(void),
  (uint32_t)&i2c3_ev_handler(void),
  (uint32_t)&i2c3_er_handler(void),
  (uint32_t)&sai1_handler(void),
  (uint32_t)&sai2_handler(void),
  (uint32_t)&swpmi1_handler(void),
  (uint32_t)&tsc_handler(void),
  (uint32_t)&lcd_handler(void), // L4x6 only
  (uint32_t)&aes_handler(void), //L486/L4A6 only
  (uint32_t)&rng_and_hash_handler(void), // L4Ax only
  (uint32_t)&fpu_handler(void),
  (uint32_t)&hash_and_crs_handler(void) // L4Ax only
};

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _sidata;
void main(void);

void reset_handler(void)
{
  // Copy .data from FLASH to SRAM
  uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
  uint8_t *flash_data = (uint8_t*) &_sidata; // Data load address (in flash)
  uint8_t *sram_data = (uint8_t*) &_sdata; // Data virtual address (in sram)
  
  for (uint32_t i = 0; i < data_size; i++)
  {
    sram_data[i] = flash_data[i];
  }

  // Zero-fill .bss section in SRAM
  uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
  uint8_t *bss = (uint8_t*) &_sbss;

  for (uint32_t i = 0; i < bss_size; i++)
  {
    bss[i] = 0;
  }
  
  main();
}

void default_handler(void)
{
  while(1);
}
