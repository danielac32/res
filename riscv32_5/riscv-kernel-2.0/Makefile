 kern: 
	clear
	clear
	riscv32-unknown-elf-gcc -ffreestanding -nostartfiles boot.s kernel/main.c include/serial.c include/kmalloc.c include/queue.c include/timer.c include/shell.c include/syscall.c include/diskio.c include/ff.c include/task.c include/basic.c switch.s -o kern.elf scripts/link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	cp kern.bin /home/daniel/Arduino/riscv32imcpu_8266_5_spi_sram/data
	riscv32-unknown-elf-objcopy -O binary  kern.elf boot/kern.bin
	#export PATH=/opt/riscv32/bin:$PATH

CROSS_COMPILE = riscv32-unknown-elf-
CC              = $(CROSS_COMPILE)gcc
CFLAGS +=  -Os -nostartfiles -ffreestanding -march=rv32im -mabi=ilp32
OBJS += basic/exception.o
OBJS += basic/init.o
OBJS += basic/main.o
#OBJS += asm.s
ELF = basic/blink.elf
BIN = basic/blink.bin


blink:	$(OBJS)
	clear
	clear
	clear
	$(CC) $(CFLAGS) $(OBJS) -T basic/link.ld -o $(ELF)
	riscv32-unknown-elf-objcopy -O binary  basic/blink.elf basic/blink.bin
	riscv32-unknown-elf-objdump -d basic/blink.elf
	./elf basic/blink.elf


setjmp: 
	clear
	clear
	riscv32-unknown-elf-gcc -ffreestanding -nostartfiles boot.s kernel/setjmp.c include/serial.c include/kmalloc.c switch.s -o kern.elf scripts/link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	cp kern.bin /home/daniel/Arduino/riscv32imcpu_8266_5_spi_sram/data
	riscv32-unknown-elf-objcopy -O binary  kern.elf boot/setjmp.bin
	#export PATH=/opt/riscv32/bin:$PATH
interpreter: 
	clear
	clear
	riscv32-unknown-elf-gcc -ffreestanding -nostartfiles boot.s kernel/basic.c include/serial.c include/basic.c include/kmalloc.c include/diskio.c include/ff.c -o kern.elf scripts/link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	cp kern.bin /home/daniel/Arduino/riscv32imcpu_8266_5_spi_sram/data
	riscv32-unknown-elf-objcopy -O binary  kern.elf boot/basic.bin
	#export PATH=/opt/riscv32/bin:$PATH
task: 
	clear
	clear
	riscv32-unknown-elf-gcc -ffreestanding -nostartfiles boot.s example/kernel.c include/serial.c  include/diskio.c include/ff.c example/os_assert.c example/os_cbk.c example/os_event.c example/os_kernel.c example/os_msgqueue.c example/os_sem.c example/os_task.c -o kern.elf scripts/link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	cp kern.bin /home/daniel/Arduino/riscv32imcpu_8266_5_spi_sram/data
	cp kern.bin boot
task2:
	clear
	clear
	riscv32-unknown-elf-gcc -ffreestanding -nostartfiles boot.s example/test.c include/serial.c include/kmalloc.c  switch.s -o kern.elf scripts/link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	#riscv32-unknown-elf-objdump -d  kern.elf
	cp kern.bin /home/daniel/Arduino/riscv32imcpu_8266_5_spi_sram/data
	cp kern.bin boots
debug:
	riscv32-unknown-elf-objdump -d kern.elf
test:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -ffreestanding -nostartfiles app/main.c include/serial.c include/syscall.c -o app/app.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/app.elf bin/app.bin
	riscv32-unknown-elf-objdump -d app/app.elf
basic_inter:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -ffreestanding -nostartfiles app/basic.c include/serial.c include/syscall.c -o app/basic.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/basic.elf bin/basic.bin
	riscv32-unknown-elf-objdump -d app/basic.elf
cat:
	clear
	clear
	riscv32-unknown-elf-gcc -fpic -ffreestanding -nostartfiles app/cat.c include/serial.c include/syscall.c -o app/cat.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/cat.elf bin/cat.bin
	riscv32-unknown-elf-objdump -d app/cat.elf
echo:
	clear
	clear
	riscv32-unknown-elf-gcc -fpic -ffreestanding -nostartfiles app/echo.c include/serial.c include/syscall.c -o app/echo.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/echo.elf bin/echo.bin
	riscv32-unknown-elf-objdump -d app/echo.elf
dump:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -ffreestanding -nostartfiles app/dump.c include/serial.c include/syscall.c -o app/dump.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/dump.elf bin/dump.bin
	riscv32-unknown-elf-objdump -d app/dump.elf
loren:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -ffreestanding -nostartfiles app/loren.c include/serial.c include/syscall.c -o app/loren.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/loren.elf bin/loren.bin
	riscv32-unknown-elf-objdump -d app/loren.elf
list:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -ffreestanding -nostartfiles app/list.c include/serial.c include/syscall.c -o app/list.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/list.elf bin/list.bin
	riscv32-unknown-elf-objdump -d app/list.elf
static:
	clear
	clear
	riscv32-unknown-elf-gcc  -fpic -ffreestanding -nostartfiles app/static.c include/serial.c include/syscall.c -o app/static.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/static.elf bin/static.bin
	riscv32-unknown-elf-objdump -d app/static.elf
	riscv32-unknown-elf-objdump -s -j .got app/static.elf
static2:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -ffreestanding -nostartfiles  app/test.c include/serial.c -o app/test.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app/test.elf bin/test.bin
	riscv32-unknown-elf-objdump -d app/test.elf
	#riscv32-unknown-elf-objdump -s -j .sdata app/test.elf
run:
	./emu kern.bin 350000 1
to:
	clear
	clear
	./convert
toc:
	gcc -o convert convert.c