
 

all:
	avr-gcc -DF_CPU=16000000UL -mmcu=atmega2560  -fno-builtin -c -Wall -Os    main.c -o kern.elf
	avr-objcopy -O ihex -R .eeprom kern.elf kern.hex
	avr-objdump -d kern.elf
flash:
	#avrdude -p atmega2560 -c arduino -P `ls -1 /dev/ttyUSB* /dev/ttyACM* 2>/dev/null | head -1` -b 115200 -D -U flash:w:kern.hex:i 
	avrdude -c arduino  -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:main.hex:i