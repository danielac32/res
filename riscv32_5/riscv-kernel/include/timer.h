#include <stdint.h>
#include "reg.h"
 
#ifndef TIMER_H
#define TIMER_H

void __attribute__ ((section(".interrupt"))) isr();
uint32_t __attribute__ ((section(".svc"))) syscall_isr(uint32_t op,uint32_t x);

#endif