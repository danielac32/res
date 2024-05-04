kern: 
	clear
	clear
	riscv32-unknown-elf-gcc -nostartfiles kernel/main.c include/serial.c include/kmalloc.c include/timer.c include/shell.c include/syscall.c include/task.c switch.s -o kern.elf scripts/link.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  kern.elf kern.bin
	#export PATH=/opt/riscv32/bin:$PATH
dump:
	 riscv32-unknown-elf-objdump -d kern.elf
readelf:
	 riscv32-unknown-elf-readelf  -a kern.elf
run:
	./emu kern.bin 300000 0
echo:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/echo.c include/serial.c -o echo.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  echo.elf echo.bin
	cp echo.bin bin/
	riscv32-unknown-elf-objdump -d echo.elf
cat:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles bin/cat.c include/serial.c -o cat.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  cat.elf cat.bin
	cp cat.bin bin/
	riscv32-unknown-elf-objdump -d cat.elf

app1:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles examples/app1.c include/serial.c switch.s -o app1.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app1.elf app1.bin
	riscv32-unknown-elf-objdump -d app1.elf
app2:
	clear
	clear
	riscv32-unknown-elf-gcc -fpie -nostartfiles examples/app2.c include/serial.c switch.s -o app2.elf scripts/linkapp.ld -march=rv32im -mabi=ilp32
	riscv32-unknown-elf-objcopy -O binary  app2.elf app2.bin
	riscv32-unknown-elf-objdump -d app2.elf
core:
	gcc -o emu ../emulator_cpu/main.c ../emulator_cpu/cpu.c ../emulator_cpu/memory.c ../emulator_cpu/syscall.c
	#objdump -d emu
