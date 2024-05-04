compile: 
	riscv32-unknown-elf-gcc -nostartfiles main.c lib/serial.c lib/syscall.c lib/timer.c driver/terminal.c  driver/cmd.c lib/task.c -o rv32.elf link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin
#compile2: 
#		riscv32-unknown-elf-gcc -nostartfiles boot.s basic2.c lib/serial.c lib/syscall.c lib/timer.c lib/task.c -o rv32.elf link.ld -march=rv32im -mabi=ilp32
#		riscv32-unknown-elf-objcopy -O binary  rv32.elf rv32.bin
dump:
	 riscv32-unknown-elf-objdump -d rv32.elf
readelf:
	 riscv32-unknown-elf-readelf  -a rv32.elf
run:
	./emu rv32.bin config.txt 
a:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles app/main.c lib/applib.c -o app/rv32.elf  app/link.ld -march=rv32im -mabi=ilp32 #--save-temps
	riscv32-unknown-elf-objcopy -O binary  app/rv32.elf app/app.bin
	riscv32-unknown-elf-objdump -d app/rv32.elf
	chmod 777 app.bin
	rm app.bin
	cp app/app.bin ./
b:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles app/main2.c lib/applib.c -o app/rv32.elf app/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  app/rv32.elf app/app3.bin
	riscv32-unknown-elf-objdump -d app/rv32.elf
	chmod 777 app3.bin
	rm app3.bin
	cp app/app3.bin ./

c:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles app/main3.c lib/applib.c -o app/rv32.elf app/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  app/rv32.elf app/app4.bin
	riscv32-unknown-elf-objdump -d app/rv32.elf
	chmod 777 app4.bin
	rm app4.bin
	cp app/app4.bin ./
d:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles app/main4.c lib/applib.c -o app/rv32.elf app/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  app/rv32.elf app/app5.bin
	riscv32-unknown-elf-objdump --disassemble app/rv32.elf #-d app/rv32.elf
	riscv32-unknown-elf-readelf  -a app/rv32.elf
	chmod 777 app5.bin
	rm app5.bin
	cp app/app5.bin ./

echo:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/echo.c lib/applib.c -o bin/rv32.elf app/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/echo.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d app/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
	chmod 777 bin/echo.bin
	#rm app5.bin
	#cp app/app5.bin ./

cat:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/cat.c lib/applib.c -o bin/rv32.elf app/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/cat.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d app/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
	chmod 777 bin/cat.bin
	#rm app5.bin
	#cp app/app5.bin ./
kill:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/kill.c lib/applib.c -o bin/rv32.elf app/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/kill.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d app/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
	chmod 777 bin/kill.bin
	#rm app5.bin
	#cp app/app5.bin ./
DUMP:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/dump.c lib/applib.c -o bin/rv32.elf app/link.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  bin/rv32.elf bin/dump.bin
	riscv32-unknown-elf-objdump --disassemble bin/rv32.elf #-d app/rv32.elf
	riscv32-unknown-elf-readelf  -a bin/rv32.elf
	chmod 777 bin/dump.bin
	#rm app5.bin
	#cp app/app5.bin ./
basic:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles app/basic.c -o app/rv32.elf app/link2.ld -march=rv32im -mabi=ilp32 
	riscv32-unknown-elf-objcopy -O binary  app/rv32.elf app/basic.bin
	riscv32-unknown-elf-objdump -d app/rv32.elf
	chmod 777 app/basic.bin
	#rm basic.bin
	#cp app/basic.bin ./