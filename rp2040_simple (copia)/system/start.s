.syntax unified
.cpu cortex-m0plus
.thumb

.section .boot, "ax"

.byte 0x00, 0xb5, 0x32, 0x4b, 0x21, 0x20, 0x58, 0x60, 0x98, 0x68, 0x02, 0x21, 0x88, 0x43, 0x98, 0x60
.byte 0xd8, 0x60, 0x18, 0x61, 0x58, 0x61, 0x2e, 0x4b, 0x00, 0x21, 0x99, 0x60, 0x04, 0x21, 0x59, 0x61
.byte 0x01, 0x21, 0xf0, 0x22, 0x99, 0x50, 0x2b, 0x49, 0x19, 0x60, 0x01, 0x21, 0x99, 0x60, 0x35, 0x20
.byte 0x00, 0xf0, 0x44, 0xf8, 0x02, 0x22, 0x90, 0x42, 0x14, 0xd0, 0x06, 0x21, 0x19, 0x66, 0x00, 0xf0
.byte 0x34, 0xf8, 0x19, 0x6e, 0x01, 0x21, 0x19, 0x66, 0x00, 0x20, 0x18, 0x66, 0x1a, 0x66, 0x00, 0xf0
.byte 0x2c, 0xf8, 0x19, 0x6e, 0x19, 0x6e, 0x19, 0x6e, 0x05, 0x20, 0x00, 0xf0, 0x2f, 0xf8, 0x01, 0x21
.byte 0x08, 0x42, 0xf9, 0xd1, 0x00, 0x21, 0x99, 0x60, 0x1b, 0x49, 0x19, 0x60, 0x00, 0x21, 0x59, 0x60
.byte 0x1a, 0x49, 0x1b, 0x48, 0x01, 0x60, 0x01, 0x21, 0x99, 0x60, 0xeb, 0x21, 0x19, 0x66, 0xa0, 0x21
.byte 0x19, 0x66, 0x00, 0xf0, 0x12, 0xf8, 0x00, 0x21, 0x99, 0x60, 0x16, 0x49, 0x14, 0x48, 0x01, 0x60
.byte 0x01, 0x21, 0x99, 0x60, 0x01, 0xbc, 0x00, 0x28, 0x00, 0xd0, 0x00, 0x47, 0x12, 0x48, 0x13, 0x49
.byte 0x08, 0x60, 0x03, 0xc8, 0x80, 0xf3, 0x08, 0x88, 0x08, 0x47, 0x03, 0xb5, 0x99, 0x6a, 0x04, 0x20
.byte 0x01, 0x42, 0xfb, 0xd0, 0x01, 0x20, 0x01, 0x42, 0xf8, 0xd1, 0x03, 0xbd, 0x02, 0xb5, 0x18, 0x66
.byte 0x18, 0x66, 0xff, 0xf7, 0xf2, 0xff, 0x18, 0x6e, 0x18, 0x6e, 0x02, 0xbd, 0x00, 0x00, 0x02, 0x40
.byte 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x07, 0x00, 0x00, 0x03, 0x5f, 0x00, 0x21, 0x22, 0x00, 0x00
.byte 0xf4, 0x00, 0x00, 0x18, 0x22, 0x20, 0x00, 0xa0, 0x00, 0x01, 0x00, 0x10, 0x08, 0xed, 0x00, 0xe0
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0b, 0x8f, 0xd5


.section .vectors, "ax"
.align 2

.global __vectors, __VECTOR_TABLE
__VECTOR_TABLE:
__vectors:
.word _stack_top
.word irq_handler_reset


.text
.type irq_handler_reset,%function
.thumb_func
.global irq_handler_reset
irq_handler_reset:
ldr r0,=_stack_top
msr msp, r0
bl startup

.type isr_svcall, %function
.global isr_svcall
isr_svcall:
mrs r0, psp
blx svccall_handler_c
bx lr

.type pendsv_handler, %function
.global pendsv_handler
 
pendsv_handler:
    cpsid   i
    mrs r0, psp
    subs r0, #4
    mov r1, lr
    str r1, [r0]

    subs r0, #16
    stmia r0!, {r4,r5, r6, r7}
    
    mov r4, r8
    mov r5, r9
    mov r6, r10
    mov r7, r11
    subs r0, #32
    stmia r0!, {r4,r5, r6, r7}
    subs r0, #16 /* fix r0 to point to end of stack frame, 36 bytes from original r0 */

    pop {r1, r2, r3, r4, r5}
    mov r8, r1
    mov r9, r2
    mov r10, r3
    mov r11, r4
    mov r12, r5 /* r12 is ip */
    pop {r4, r5, r6, r7}       

    msr psr_nzcvq, ip
    cpsie   i
    pop {pc}

.global __piccolo_pre_switch
__piccolo_pre_switch:
    

    mrs ip, psr
    push {r4, r5, r6, r7, lr}
    mov r1, r8
    mov r2, r9
    mov r3, r10
    mov r4, r11
    mov r5, r12
    push {r1, r2, r3, r4, r5}    

    ldmia   r0!,{r4-r7}
    mov r8, r4
    mov r9, r5
    mov r10, r6
    mov r11, r7
    ldmia   r0!,{r4-r7}
    ldmia   r0!,{r1}
    mov lr, r1
    msr psp, r0 /* r0 is usertask_stack_start from activate(usertask_stack_start); */

    /* jump to user task */
    bx lr

.global __piccolo_task_init_stack
__piccolo_task_init_stack:
    mrs ip, psr
    push {r4, r5, r6, r7, lr}
    mov r1, r8
    mov r2, r9
    mov r3, r10
    mov r4, r11
    mov r5, r12
    push {r1, r2, r3, r4, r5}    

    /* switch to process stack */
    msr psp, r0
    movs r0, #3
    msr control, r0
    isb
    svc 0
    nop
    nop
    nop
    bx lr


 
