#include <stdint.h>

#ifndef SYSCALL_H_
#define SYSCALL_H_


 //FILES
uint8_t k_READ();
void k_READBUFF(uint8_t *buff,uint8_t size);
void k_WRITE(uint8_t val);
void k_WRITEBUFF(uint8_t *buff,uint8_t size); 
uint8_t k_OPEN(char *file);
void k_CLOSE();
void k_SEEK(uint32_t point);
uint32_t k_FSIZE(); 
uint8_t k_OPENDIR(char *dir);
//void k_READDIR();
//void k_CLOSEDIR();
uint32_t __svc(uint32_t x,uint32_t x2);

#endif