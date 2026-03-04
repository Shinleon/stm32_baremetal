
CC=arm-none-eabi-gcc

CFLAGS=-mcpu=cortex-m4 -mthumb -nostdlib
CPPFLAGS=-DSTM32L476xx \
			-Ivendor/CMSIS/CMSIS/Core/Include \
			-Ivendor/STM32L4/Include

LINKER_FILE=linker_script.ld
LDFLAGS=-T $(LINKER_FILE)
VENDOR_SRC=vendor/STM32L4/Source/Templates/system_stm32l4xx.c

EXECUTABLE=blink.elf

PROGRAMMER=openocd
PROGRAMMER_FLAGS=-f ./stlink-v2-1.cfg -f ./stm32l4x.cfg

.PHONY: all flash clean
	
all: $(EXECUTABLE)

$(EXECUTABLE): main.o startup.o system_stm32l4xx.o 
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $(EXECUTABLE)

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c

startup.o: startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c

system_stm32l4xx.o: vendor/STM32L4/Source/Templates/system_stm32l4xx.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c

flash: blink.elf
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program blink.elf verify reset exit"

clean: 
	rm -rf $(EXECUTABLE) *.o
