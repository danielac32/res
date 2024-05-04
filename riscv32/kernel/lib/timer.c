#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include "syscall.h"
#include "lib.h"
#include "task.h"
#include "timer.h"

/*
void __attribute__ ((section(".interrupt"))) isr(){
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    calls();
    asm("nop");
    asm("lw      ra,12(sp)");
    asm("lw      s0,8(sp)");
    asm("addi    sp,sp,16");
    asm("uret");
}*/

void __attribute__ ((section(".interrupt"))) isr(){     
      
      asm("nop             \n\t"
          "lw  ra,12(sp)   \n\t"
          "lw  s0,8(sp)    \n\t"
          "addi  sp,sp,16  \n\t"
          "nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
          "nop             \n\t"
      );
      asm("addi  sp,sp,-120 \n\t"

          "sw    ra,116(sp) \n\t"
          "sw    t6,112(sp) \n\t"
          "sw    t5,108(sp) \n\t"
          "sw    t4,104(sp) \n\t"
          "sw    t3,100(sp) \n\t"

          "sw    s11,96(sp) \n\t"
          "sw    s10,92(sp) \n\t"
          "sw    s9,88(sp)  \n\t"
          "sw    s8,84(sp)  \n\t"
          "sw    s7,80(sp)  \n\t"
          "sw    s6,76(sp)  \n\t"
          "sw    s5,72(sp)  \n\t"
          "sw    s4,68(sp)  \n\t"
          "sw    s3,64(sp)  \n\t"
          "sw    s2,60(sp)  \n\t"

          "sw    a7,56(sp)  \n\t"
          "sw    a6,52(sp)  \n\t"
          "sw    a5,48(sp)  \n\t"
          "sw    a4,44(sp)  \n\t"
          "sw    a3,40(sp)  \n\t"
          "sw    a2,36(sp)  \n\t"
          "sw    a1,32(sp)  \n\t"
          "sw    a0,28(sp)  \n\t"

          "sw    s1,24(sp)  \n\t"
          "sw    s0,20(sp)  \n\t"

          "sw    t2,16(sp)  \n\t"
          "sw    t1,12(sp)  \n\t"
          "sw    t0,8(sp)   \n\t"
          "sw    tp,4(sp)   \n\t"

          "sw    gp,0(sp)   \n\t"
          //"addi  s0,sp,16  \n\t"
        );
      calls();
         
      asm("nop              \n\t"
          "lw    ra,116(sp) \n\t"
          "lw    t6,112(sp) \n\t"
          "lw    t5,108(sp) \n\t"
          "lw    t4,104(sp) \n\t"
          "lw    t3,100(sp) \n\t"

          "lw    s11,96(sp) \n\t"
          "lw    s10,92(sp) \n\t"
          "lw    s9,88(sp)  \n\t"
          "lw    s8,84(sp)  \n\t"
          "lw    s7,80(sp)  \n\t"
          "lw    s6,76(sp)  \n\t"
          "lw    s5,72(sp)  \n\t"
          "lw    s4,68(sp)  \n\t"
          "lw    s3,64(sp)  \n\t"
          "lw    s2,60(sp)  \n\t"

          "lw    a7,56(sp)  \n\t"
          "lw    a6,52(sp)  \n\t"
          "lw    a5,48(sp)  \n\t"
          "lw    a4,44(sp)  \n\t"
          "lw    a3,40(sp)  \n\t"
          "lw    a2,36(sp)  \n\t"
          "lw    a1,32(sp)  \n\t"
          "lw    a0,28(sp)  \n\t"

          "lw    s1,24(sp)  \n\t"
          "lw    s0,20(sp)  \n\t"

          "lw    t2,16(sp)  \n\t"
          "lw    t1,12(sp)  \n\t"
          "lw    t0,8(sp)   \n\t"
          "lw    tp,4(sp)   \n\t"
          
          "lw    gp,0(sp)   \n\t"
          "addi  sp,sp,120  \n\t"
          "uret             \n\t"
     ); 
    /*
    asm("nop \n\t"  
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
        "nop \n\t" 
    ); */ 
    /*asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    call();
    asm("nop");
    asm("lw      ra,12(sp)");
    asm("lw      s0,8(sp)");
    asm("addi    sp,sp,16");
    asm("uret");*/
}