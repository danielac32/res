#export PATH=/opt/riscv32/bin:$PATH


all: 
	gcc -o emu main.c riscv.c
test:
	riscv64-unknown-elf-gcc  -I /usr/include -nostdlib -nostartfiles   entry.s vector.s rv32.c serial.c kmalloc.c -o rv32test link.ld -march=rv32im -mabi=ilp32
	riscv64-unknown-elf-objdump -d rv32test
	riscv64-unknown-elf-objcopy -O binary rv32test rv32test.bin


task:
	riscv64-unknown-elf-gcc -ffreestanding -nostartfiles multitasking/entry.s multitasking/vector.s multitasking/rv32.c multitasking/serial.c multitasking/kmalloc.c multitasking/task.c multitasking/exception.c -o rv32 multitasking/link.ld -march=rv32im -mabi=ilp32
	riscv64-unknown-elf-objdump -d rv32
	riscv64-unknown-elf-objcopy -O binary rv32 rv32.bin
	cp rv32.bin /home/daniel/Arduino/riscv32_vm_2/data

task3:
	clear
	clear
	clear
	riscv64-unknown-elf-gcc -ffreestanding -nostartfiles multitasking/entry.s multitasking/vector.s multitasking/rv32.c multitasking/serial.c multitasking/kmalloc.c multitasking/task.c multitasking/exception.c -o rv32 multitasking/link.ld -march=rv32im -mabi=ilp32
	riscv64-unknown-elf-objdump -d rv32
	riscv64-unknown-elf-objcopy -O binary rv32 rv32.bin

task4:
	clear
	clear
	clear
	riscv64-unknown-elf-gcc -ffreestanding -nostartfiles multitasking3/entry.s multitasking3/vector.s multitasking3/rv32.c multitasking3/serial.c multitasking3/kmalloc.c multitasking3/task.c multitasking3/exception.c multitasking3/queue.c multitasking3/kernel.c -o rv32 multitasking3/link.ld -march=rv32im -mabi=ilp32
	riscv64-unknown-elf-objdump -d rv32
	riscv64-unknown-elf-objcopy -O binary rv32 rv32.bin
task5:
	clearmyprintf
	clear
	clear
	riscv64-unknown-elf-gcc -ffreestanding -nostartfiles multitasking4/entry.s multitasking4/vector.s multitasking4/rv32.c multitasking4/serial.c multitasking4/kmalloc.c multitasking4/task.c multitasking4/exception.c multitasking4/lfs_util.c  multitasking4/lfs.c  -o rv32 multitasking4/link.ld -march=rv32im -mabi=ilp32
	riscv64-unknown-elf-objdump -d rv32
	riscv64-unknown-elf-objcopy -O binary rv32 rv32.bin

	
task2:
	riscv64-unknown-elf-gcc -I usr/include -ffreestanding -nostartfiles Round-Robin/entry.s  Round-Robin/switch.s Round-Robin/main.c Round-Robin/serial.c Round-Robin/kmalloc.c Round-Robin/scheduler.c Round-Robin/queue.c -o rv32 Round-Robin/link.ld -march=rv32im -mabi=ilp32 -ffreestanding -nostdlib -nostartfiles -lgcc -fno-builtin
	riscv64-unknown-elf-objdump -d rv32
	riscv64-unknown-elf-objcopy -O binary rv32 rv32.bin
run:
	./emu rv32.bin 100000 0