#include <stdint.h>

#define SRAM_START (0x20000000U)
#define SRAM_SIZE  (96U *1024U)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)
#define ISR_VECTOR_SIZE_WORDS (98)

void reset_handler(void);
void default_handler(void);

void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handle(void) __attribute__((weak, alias("default_handler")));
void memMngr_fault_handle(void) __attribute__((weak, alias("default_handler")));
void bus_fault_handle(void) __attribute__((weak, alias("default_handler")));
void usage_fault_handle(void) __attribute__((weak, alias("default_handler")));
void svcall_handle(void) __attribute__((weak, alias("default_handler")));
void debug_monitor_handle(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));

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
}
