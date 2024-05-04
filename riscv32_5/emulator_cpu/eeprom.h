#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef EEPROM_H_
#define EEPROM_H_

FILE *eeprom_file;
#define EEPROM_SIZE 500000
#define EEPROM_END_ADDR EEPROM_SIZE
void eeprom_fill(uint32_t addr, unsigned char value, int size);
void eeprom_init(void);
void eeprom_write(uint32_t addr, unsigned char *buf, int size);
void eeprom_read(uint32_t addr, unsigned char *buf, int size);
 
#endif