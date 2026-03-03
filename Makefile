
cc=arm-none-eabi-gcc

all: startup.c linker_script.ld main.c
	$(cc) main.c startup.c -T linker_script.ld -o blink.elf -mcpu=cortex-m4 -mthumb -nostdlib
