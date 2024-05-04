compile: 
	export PATH="$PATH:/opt/riscv32/bin"
	riscv32-unknown-elf-gcc -nostartfiles boot.s main.c -o rv32.elf link.ld -march=rv32im -mabi=ilp32
dump:
	 riscv32-unknown-elf-objdump -d rv32.elf
readelf:
	   riscv32-unknown-elf-readelf  -a rv32.elf
bin:
	riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin
run:
	./emu rv32.bin config.txt
