#export PATH=/opt/riscv32/bin:$PATH
task:
	/opt/riscv32/bin/riscv32-unknown-elf-gcc -ffreestanding -nostartfiles entry.s vector.s rv32.c serial.c kmalloc.c task.c exception.c -o rv32 link.ld -march=rv32im -mabi=ilp32
	/opt/riscv32/bin/riscv32-unknown-elf-objdump -d rv32
	/opt/riscv32/bin/riscv32-unknown-elf-objcopy -O binary rv32 rv32.bin
	cp rv32.bin /home/daniel/Arduino/riscv32_vm_2/data
