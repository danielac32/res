.syntax unified
.cpu cortex-m0plus
.thumb

 

.global	disable
.global	restore
.global	enable

disable:
	mrs	r0, psr		/* Copy the CPSR into r0		*/
	cpsid	i		/* Disable interrupts			*/
	bx	lr		/* Return the CPSR			*/

 
restore:
	msr	psr_nzcvq, r0	/* Restore the CPSR			*/
	cpsie	i		
	bx 	lr		/* Return to caller			*/

 
enable:
	cpsie	i		/* Enable interrupts			*/
	bx	lr		/* Return				*/


.globl delay
.thumb_func
delay:
    subs r0,#1
    bne delay
    bx lr

.text
.align 4
.force_thumb
.global __aeabi_idiv
__aeabi_idiv:
	ldr r3, =0xD0000000
	str r0, [r3, #0x68]
	str r1, [r3, #0x6C]
	b 1f
1:	b 1f
1:	b 1f
1:	b 1f
1:	ldr r0, [r3, #0x70]
	bx lr


.text
.align 4
.force_thumb
.global __aeabi_idivmod
__aeabi_idivmod:
	ldr r3, =0xD0000000
	str r0, [r3, #0x68]
	str r1, [r3, #0x6C]
	b 1f
1:	b 1f
1:	b 1f
1:	b 1f
1:	ldr r0, [r3, #0x74]
	bx lr


.text
.align 4
.force_thumb
.global __aeabi_uidiv
__aeabi_uidiv:
	ldr r3, =0xD0000000
	str r0, [r3, #0x60]
	str r1, [r3, #0x64]
	b 1f
1:	b 1f
1:	b 1f
1:	b 1f
1:	ldr r0, [r3, #0x70]
	bx lr


.text
.align 4
.force_thumb
.global __aeabi_uidivmod
__aeabi_uidivmod:
	ldr r3, =0xD0000000
	str r0, [r3, #0x60]
	str r1, [r3, #0x64]
	b 1f
1:	b 1f
1:	b 1f
1:	b 1f
1:	ldr r0, [r3, #0x74]
	bx lr




.text
.align 0
.force_thumb
.syntax unified
.global __gnu_thumb1_case_uqi
.thumb_func
.type __gnu_thumb1_case_uqi,function
__gnu_thumb1_case_uqi:
push	{r1}
mov	r1, lr
lsrs	r1, r1, #1
lsls	r1, r1, #1
ldrb	r1, [r1, r0]
lsls	r1, r1, #1
add	lr, lr, r1
pop	{r1}
bx	lr
.size __gnu_thumb1_case_uqi, . - __gnu_thumb1_case_uqi

.text
.align 0
.force_thumb
.syntax unified
.global __gnu_thumb1_case_uhi
.thumb_func
.type __gnu_thumb1_case_uhi,function
__gnu_thumb1_case_uhi:
push	{r0, r1}
mov	r1, lr
lsrs	r1, r1, #1
lsls	r0, r0, #1
lsls	r1, r1, #1
ldrh	r1, [r1, r0]
lsls	r1, r1, #1
add	lr, lr, r1
pop	{r0, r1}
bx	lr
.size __gnu_thumb1_case_uhi, . - __gnu_thumb1_case_uhi
