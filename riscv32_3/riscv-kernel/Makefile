compile: 
	riscv32-unknown-elf-gcc -nostartfiles main.c lib/serial.c lib/syscall.c lib/timer.c lib/terminal.c  lib/cmd.c lib/task.c -o rv32.elf link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin
#compile2: 
#		riscv32-unknown-elf-gcc -nostartfiles boot.s basic2.c lib/serial.c lib/syscall.c lib/timer.c lib/task.c -o rv32.elf link.ld -march=rv32im -mabi=ilp32
#		riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin
dump:
	 riscv32-unknown-elf-objdump -d rv32.elf
readelf:
	 riscv32-unknown-elf-readelf  -a rv32.elf
run:
	./emu rv32.bin
a:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/main.c lib/applib.c -o user/rv32.elf  user/link.ld -march=rv32im -mabi=ilp32 #--save-temps
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/app.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf
	#chmod 777 app.bin
	#rm app.bin
	#cp user/app.bin ./
b:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/main2.c lib/applib.c -o user/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/app3.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf
	#chmod 777 app3.bin
	#rm app3.bin
	#cp user/app3.bin ./

c:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/main3.c lib/applib.c -o user/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/app4.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf
	#chmod 777 app4.bin
	#rm app4.bin
	#cp user/app4.bin ./
d:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/main4.c lib/applib.c -o user/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/app5.bin
	riscv32-unknown-elf-objdump --disassemble user/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a user/rv32.elf
	#chmod 777 app5.bin
	#rm app5.bin
	#cp user/app5.bin ./

echo:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/echo.c lib/applib.c -o bin/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/echo.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
	chmod 777 bin/echo.bin
	#rm app5.bin
	#cp user/app5.bin ./

cat:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/cat.c lib/applib.c -o bin/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/cat.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
	chmod 777 bin/cat.bin
	#rm app5.bin
	#cp user/app5.bin ./
kill:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/kill.c lib/applib.c -o bin/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/kill.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
	chmod 777 bin/kill.bin
	#rm app5.bin
	#cp user/app5.bin ./
DUMP:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/dump.c lib/applib.c -o bin/rv32.elf user/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/dump.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d user/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
	chmod 777 bin/dump.bin
	#rm app5.bin
	#cp user/app5.bin ./
basic:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles user/basic.c lib/nwlib.c -o user/rv32.elf user/link2.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  user/rv32.elf user/basic.bin
	riscv32-unknown-elf-objdump -d user/rv32.elf
	chmod 777 user/basic.bin
	#rm basic.bin
	#cp user/basic.bin ./
compile3: 
	clear
	clear
	riscv32-unknown-elf-gcc -nostartfiles main3.c lib/serial.c -o rv32.elf link2.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin