#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define mcu 0

#define base_io 0xFA000

#define mode              (*(volatile uint8_t *)0xFA001)
#define reg               (*(volatile uint8_t *)0xFA002)
#define timer         (*(volatile uint32_t *)0xFA003)
#define enable_timer      (*(volatile uint8_t *)0xFA004)
#define keyAvailable   (*(volatile uint8_t *)0xFA005)
#define keyRead        (*(volatile uint8_t *)0xFA006)
#define led            (*(volatile uint8_t *)0xFA007)
#define workregister1 (*(volatile uint32_t *)0xFA008)
#define workregister2 (*(volatile uint32_t *)0xFA009)
#define workregister3 (*(volatile uint32_t *)0xFA00a)
#define workregister4 (*(volatile uint32_t *)0xFA00b)
#define workregister5 (*(volatile uint32_t *)0xFA00c)
#define workregister6 (*(volatile uint32_t *)0xFA00d)
#define workregister7 (*(volatile uint32_t *)0xFA00e)

#define _exit()                      \
register uint32_t a7 asm("a7")=25;   \
asm("ecall");

#define reg_uart(ch)                    \
register uint32_t a7 asm("a7")=24;      \
register uint32_t a0 asm("a0")=ch;      \
asm("ecall");

 //FILES
#define SYSCALL_READ   1
#define SYSCALL_READBUFF   2
#define SYSCALL_WRITE   3
#define SYSCALL_WRITEBUFF   4
#define SYSCALL_OPEN   5
#define SYSCALL_CLOSE   6
#define SYSCALL_SEEK   7 
#define SYSCALL_FSIZE   8 
#define SYSCALL_OPENDIR   9
#define SYSCALL_READDIR   10
#define SYSCALL_CLOSEDIR   11
 
 //MALLOC
#define SYSCALL_MALLOC   12
#define SYSCALL_FREE   13
 
  //TASK
#define SYSCALL_CREATETASK   14
#define SYSCALL_DELETETASK   15
#define SYSCALL_PID   16
#define SYSCALL_GETPID   17
#define SYSCALL_YIELD   (18)
#define SYSCALL_WAIT   19
#define SYSCALL_SLEEP   20
#define SYSCALL_EXIT   21
#define SYSCALL_KILL   22
  //SERIAL
#define SYSCALL_PRINTSTR   23
#define SYSCALL_PRINTCHAR   24

#define SYSCALL_EXIT2   25