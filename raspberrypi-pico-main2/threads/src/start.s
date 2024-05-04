.cpu cortex-m0plus
.thumb
.global reset
reset:
	ldr r0, =0x20040000
	mov sp, r0
	bl init
	b .

.align
.ltorg
.balign 0x100
