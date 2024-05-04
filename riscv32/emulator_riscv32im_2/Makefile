all:
	gcc -o emu2 main.c cpu.c
	#cp emu ../kernel
	#cp emu ../
run:
	./emu2 rv32.bin
rv32:
	clear
	clear
	riscv32-unknown-elf-gcc -nostartfiles boot.s app.c -o rv32.elf link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin
	riscv32-unknown-elf-objdump --disassemble rv32.elf #-d app/rv32.elf
	riscv32-unknown-elf-readelf  -a rv32.elf