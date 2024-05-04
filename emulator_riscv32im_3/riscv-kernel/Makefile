cpu:
	gcc -o emu riscv32imcpu_large_2/main.c riscv32imcpu_large_2/cpu.c
	#chmod 777 riscv32imcpu_large_2/emu
	#cp riscv32imcpu_large_2/emu ../
run:
	./emu kern.bin config.txt


compile:
		riscv32-unknown-elf-gcc -nostartfiles main.c lib/serial.c lib/syscall.c lib/timer.c lib/terminal.c  lib/cmd.c lib/task.c -o rv32.elf link.ld -march=rv32im -mabi=ilp32
		riscv32-unknown-elf-objcopy -O binary  rv32.elf kern.bin
echo:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/echo.c lib/applib.c -o bin/rv32.elf bin/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/echo.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf  
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
cat:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/cat.c lib/applib.c -o bin/rv32.elf bin/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/cat.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf  
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
kill:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/kill.c lib/applib.c -o bin/rv32.elf bin/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/kill.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf  
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
DUMP:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/dump.c lib/applib.c -o bin/rv32.elf bin/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/dump.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf 
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
test:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/test.c -o bin/rv32.elf bin/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/test.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf 
	riscv32-unknown-elf-readelf  -a bin/rv32.elf