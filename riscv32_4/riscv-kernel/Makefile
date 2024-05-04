compile: 
	clear
	clear
	riscv32-unknown-elf-gcc -nostartfiles main.c lib/serial.c lib/syscall.c lib/timer.c lib/terminal.c  lib/cmd.c lib/task.c lib/klib.c -o kern.elf link2.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	#export PATH=/opt/riscv32/bin:$PATH
dump:
	 riscv32-unknown-elf-objdump -d kern.elf
readelf:
	 riscv32-unknown-elf-readelf  -a kern.elf
run:
	./emu kern.bin 100000
a:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/main.c lib/applib.c -o user/rv32.elf  user/link.ld -march=rv32im -mabi=ilp32 #--save-temps
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/app.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf

b:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/main2.c lib/applib.c -o user/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/app3.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf

c:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/main3.c lib/applib.c -o user/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/app4.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf

d:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/main4.c lib/applib.c -o user/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/app5.bin
	riscv32-unknown-elf-objdump --disassemble user/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a user/rv32.elf


echo:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/echo.c lib/nwlib.c -o bin/rv32.elf user/link2.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/echo.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d user/rv32.elf
	#riscv32-unknown-elf-readelf  -a bin/rv32.elf

cat:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/cat.c lib/nwlib.c -o bin/rv32.elf user/link2.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/cat.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf

kill:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/kill.c lib/nwlib.c -o bin/rv32.elf user/link2.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/kill.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf

DUMP:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/dump.c lib/nwlib.c -o bin/rv32.elf user/link2.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/dump.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf

basic:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/basic.c lib/nwlib.c -o user/rv32.elf user/link2.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/basic.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf

test:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/cputest.c -o user/rv32.elf user/link2.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/test.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf
	
compile2: 
	clear
	clear
	riscv32-unknown-elf-gcc -nostartfiles main2.c lib/serial.c -o rv32.elf link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin
compile3: 
	clear
	clear
	riscv32-unknown-elf-gcc -nostartfiles main3.c lib/serial.c -o rv32.elf link2.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin