compile: 
	clear
	clear
	riscv32-unknown-elf-gcc -nostartfiles main.c lib/serial.c lib/kmalloc.c lib/timer.c -o kern.elf link2.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	#export PATH=/opt/riscv32/bin:$PATH
dump:
	 riscv32-unknown-elf-objdump -d kern.elf
readelf:
	 riscv32-unknown-elf-readelf  -a kern.elf
run:
	./emu kern.bin 100000