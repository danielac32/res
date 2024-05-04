BINS = emu-rv32i test1

CROSS_COMPILE = /opt/riscv32/bin/riscv32-unknown-elf-
RV32I_CFLAGS = -march=rv32i -mabi=ilp32  -ffreestanding -nostdlib -nostartfiles -lgcc -fno-builtin  

CFLAGS = -O3 -Wall 
LDFLAGS = -lelf
CFLAGS2 = -I include  -ffreestanding -nostdlib -nostartfiles -lgcc -fno-builtin  
CFLAGS2 +=-O3   -march=rv32ima -mabi=ilp32

all: $(BINS)
	
emu-rv32i: emu-rv32i.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

test1: 
	riscv64-unknown-elf-gcc $(CFLAGS2) -T link.ld -o test1.elf entry.s test1.c 
	riscv64-unknown-elf-objcopy test1.elf -O binary test1.bin
	riscv64-unknown-elf-objdump -d test1.elf


#other:
#	$(CROSS_COMPILE)gcc -ffreestanding -nostartfiles multitasking/entry.s multitasking/vector.s multitasking/rv32.c multitasking/serial.c multitasking/kmalloc.c multitasking/task.c multitasking/exception.c -o rv32 multitasking/link.ld -march=rv32i -mabi=ilp32
#	/opt/riscv32/bin/riscv32-unknown-elf-objcopy -O binary rv32 test1.bin
#other2:
#	$(CROSS_COMPILE)gcc -ffreestanding -nostartfiles entry.s test1.c -o test1 link.ld -march=rv32i -mabi=ilp32
#	/opt/riscv32/bin/riscv32-unknown-elf-objcopy -O binary test1 test1.bin


check:
	 gcc -o emu-rv32i emu-rv32i.c 
	./emu-rv32i test1.bin

clean:
	$(RM) $(BINS)