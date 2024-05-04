all: 
	gcc -o emu main.c cpu.c
	cp emu ../riscv-kernel
	cp emu ../riscv-kernel2
run:
	./emu rv32.bin 100000
