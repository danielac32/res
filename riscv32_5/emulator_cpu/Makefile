all: 
	gcc -o emu main.c cpu.c memory.c syscall.c eeprom.c
	cp emu ../riscv-kernel
	cp emu ../small_riscv2
	cp emu ../riscv-kernel-2.0
run:
	./emu rv32.bin 100000 0