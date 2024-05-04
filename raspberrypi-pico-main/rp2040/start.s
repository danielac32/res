
#include "rp2040.h"
.syntax unified
.cpu cortex-m0plus
.thumb

.section .reset, "ax"

.type _entry_point,%function
.thumb_func
.global _entry_point
_entry_point:
    ldr r0,=__StackTop
    mov sp,r0
    b _reset_handler
 
.type _reset_handler,%function
.thumb_func
_reset_handler:
    
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    movs r2, #0
bss_loop:
cmp r0,r1
beq bss_finished
strb r2,[r0]
adds r0,r0,#1
b bss_loop
bss_finished:
movs r0,#0
mov lr ,r0
mov r12,sp
ldr r2 ,=main
blx r2
  
 

 

 

