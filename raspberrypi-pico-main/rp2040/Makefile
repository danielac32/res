
	

CROSS_COMPILE = arm-none-eabi-
CC              = $(CROSS_COMPILE)gcc
CFLAGS += -mthumb  -Werror -Os -mcpu=cortex-m0 -nostartfiles -ffreestanding
OBJS += start.s
OBJS += boot2.s
#OBJS += boot.o
OBJS += gpio.o
#OBJS += nvic.o
OBJS += init.o
OBJS += resets.o
OBJS += uart.o
OBJS += xosc.o

ELF = kern.elf
BIN = kern.bin

other:	$(OBJS)
	clear
	clear
	clear
	$(CC) $(CFLAGS) $(OBJS) -T memory.ld -o $(ELF)
	#arm-none-eabi-gcc -O0 -nostartfiles exception.c init.c main.c serial.c -o kern.elf linker_script.ld -mthumb -mcpu=cortex-m0
	arm-none-eabi-objcopy -O binary  $(ELF) kern.bin
	arm-none-eabi-objdump -d $(ELF)
	gcc -O2 makeuf2.c -o makeuf2
	./makeuf2 kern.bin main.uf2



clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.uf2
	rm -f makeuf2