.cpu cortex-m0
.thumb
.global boot_entry
.globl vector_table
vector_table:
.thumb_func
.globl entry
entry:
    b reset
    .balign 4
    .word reset
    .word loop
    .word loop
    .word loop
    .word loop
    .word loop
    .word loop
    .word loop
    .word loop
    .word loop
    .word isr_svcall
    .word loop
    .word loop
    .word isr_pendsv
    .word isr_systick
 


.thumb_func
reset:
    ldr r1,=0xE000ED08 ;@ VTOR
    ldr r0,=vector_table
    str r0,[r1]

    ldr r0,=_estack
    msr msp,r0
    #bl boot_entry
    b loop

.thumb_func
loop:
    b loop


.align
.ltorg


;@ ----------------------------------
.balign 0x100

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl DELAY
.thumb_func
DELAY:
    sub r0,#1
    bne DELAY
    bx lr