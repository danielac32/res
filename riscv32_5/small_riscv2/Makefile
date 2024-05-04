kern: 
	clear
	clear
	riscv32-unknown-elf-gcc -ffreestanding -nostartfiles boot.s main.c serial.c kmalloc.c shell.c diskio.c ff.c -o kern.elf link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	cp kern.bin /home/daniel/Arduino/riscv32imcpu_8266_5_spi_sram/data
	#export PATH=/opt/riscv32/bin:$PATH
dump:
	 riscv32-unknown-elf-objdump -d kern.elf
readelf:
	 riscv32-unknown-elf-readelf  -a kern.elf
run:
	./emu kern.bin 370000 1
test:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -ffreestanding -nostartfiles app1.c serial.c -o app.elf linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app.elf app.bin
	riscv32-unknown-elf-objdump -d app.elf
to:
	./convert
toc:
	gcc -o convert convert.c