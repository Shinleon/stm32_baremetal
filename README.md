# stm32_baremetal
Bare metal practice on STM nucleo   
Based on [this example](https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities/)  
Compiling using [`gcc-arm-none-eabi`](https://learn.arm.com/install-guides/gcc/arm-gnu/)  
Loading on board [using `openOCD`](https://github.com/openocd-org/openocd)   

## Hardware used  
Board: STM Nucleo-L476RG  
Microcontroller: STM32L476RGTx

## Project Outline  
### Memory outline
If we look in section *2.4 Embedded SRAM* of the Reference Manual for the `STM32L47xxx`,  we see that the STM32L47x has 96 Kbytes SRAM1 and 32 Kbytes SRAM2; there is also 1 MB Flash memory, shown in *2.1 System Architecture* **Figure 1**. Looking at **Figure 3** in section *2.2.2 Memory map and register boundary addresses*, we can see that Flash memory is in the range [`0x0800_0000`, `0x0810_0000`), and SRAM1 is in the range of [`0x2000_0000`, `0x2001_8000`).  

### c file + linker script => .elf file  


