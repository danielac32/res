kern: 
	clear
	clear
	riscv32-unknown-elf-gcc -ffreestanding -nostartfiles main.c serial.c -o kern.elf link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	#export PATH=/opt/riscv32/bin:$PATH
dump:
	 riscv32-unknown-elf-objdump -d kern.elf
readelf:
	 riscv32-unknown-elf-readelf  -a kern.elf
run:
	./emu kern.bin 370000 1

