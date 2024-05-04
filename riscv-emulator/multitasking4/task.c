
#include "serial.h"
#include "kmalloc.h"
#include "context.h"
#include "task.h"
#include "riscv.h"
#include <string.h>
//extern void push();
//extern void pop();


void *get_psp(void) {
    void *ret;
    asm volatile ("mv   %0, sp" : "=r" (ret));
    return ret;
}

void set_psp(void* sp) {
    asm volatile ("mv sp, %0" : : "r" (sp));  
}

 

void __attribute__ ((section(".interrupt"))) isr(){  
 


//asm volatile("li a0,0xfffffffd\n"
 //            "jalr a0\n"
  //    );
}