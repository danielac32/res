#include <stdint.h>
#include "reg.h"
 
#ifndef TIMER_H
#define TIMER_H

void __attribute__ ((section(".interrupt"))) isr();
void __attribute__ ((section(".svc"))) syscall_isr();

#endif