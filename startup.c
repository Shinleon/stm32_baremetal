/* The goal of startup.c is to
 * 1) initialize main stack pointer
 * 2) initialize interrupt vector table 
 * 3) copy `.data` from flash to SRAM
 * 4) zero-fill `.bss` section
 *    `.bss` is "Block Started by Symbol"
 */

#define SRAM_START (0x20000000U)
#define SRAM_SIZE  (96U * 1024U)
#define SRAM_SEND  (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)


