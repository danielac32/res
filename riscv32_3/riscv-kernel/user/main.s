	.file	"main.c"
	.option pic
	.attribute arch, "rv32i2p0_m2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
 #APP
	j main
 #NO_APP
	.comm	mem,2048,4
	.comm	buff,80,4
	.comm	counter_timer,4,4
	.align	2
	.globl	reverse
	.type	reverse, @function
reverse:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	zero,-20(s0)
	lw	a5,-40(s0)
	addi	a5,a5,-1
	sw	a5,-24(s0)
	j	.L2
.L3:
	lw	a5,-20(s0)
	lw	a4,-36(s0)
	add	a5,a4,a5
	lbu	a5,0(a5)
	sw	a5,-28(s0)
	lw	a5,-24(s0)
	lw	a4,-36(s0)
	add	a4,a4,a5
	lw	a5,-20(s0)
	lw	a3,-36(s0)
	add	a5,a3,a5
	lbu	a4,0(a4)
	sb	a4,0(a5)
	lw	a5,-24(s0)
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a4,-28(s0)
	andi	a4,a4,0xff
	sb	a4,0(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
	lw	a5,-24(s0)
	addi	a5,a5,-1
	sw	a5,-24(s0)
.L2:
	lw	a4,-20(s0)
	lw	a5,-24(s0)
	blt	a4,a5,.L3
	nop
	nop
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	reverse, .-reverse
	.align	2
	.globl	intToStr
	.type	intToStr, @function
intToStr:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	sw	zero,-20(s0)
	sb	zero,-21(s0)
	lw	a5,-36(s0)
	bne	a5,zero,.L5
	lw	a5,-40(s0)
	li	a4,48
	sb	a4,0(a5)
	li	a5,-1
	j	.L6
.L5:
	lw	a5,-36(s0)
	bge	a5,zero,.L8
	li	a5,1
	sb	a5,-21(s0)
	lw	a5,-36(s0)
	neg	a5,a5
	sw	a5,-36(s0)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
	j	.L8
.L9:
	lw	a4,-36(s0)
	li	a5,10
	rem	a5,a4,a5
	andi	a4,a5,0xff
	lw	a5,-20(s0)
	addi	a3,a5,1
	sw	a3,-20(s0)
	mv	a3,a5
	lw	a5,-40(s0)
	add	a5,a5,a3
	addi	a4,a4,48
	andi	a4,a4,0xff
	sb	a4,0(a5)
	lw	a4,-36(s0)
	li	a5,10
	div	a5,a4,a5
	sw	a5,-36(s0)
.L8:
	lw	a5,-36(s0)
	bne	a5,zero,.L9
	j	.L10
.L11:
	lw	a5,-20(s0)
	addi	a4,a5,1
	sw	a4,-20(s0)
	mv	a4,a5
	lw	a5,-40(s0)
	add	a5,a5,a4
	li	a4,48
	sb	a4,0(a5)
.L10:
	lw	a4,-20(s0)
	lw	a5,-44(s0)
	blt	a4,a5,.L11
	lw	a1,-20(s0)
	lw	a0,-40(s0)
	call	reverse@plt
	lbu	a5,-21(s0)
	beq	a5,zero,.L12
	lw	a5,-40(s0)
	li	a4,45
	sb	a4,0(a5)
.L12:
	lw	a5,-20(s0)
	lw	a4,-40(s0)
	add	a5,a4,a5
	sb	zero,0(a5)
	li	a5,0
.L6:
	mv	a0,a5
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	intToStr, .-intToStr
	.section	.rodata
	.align	2
.LC0:
	.string	"6545\n"
	.align	2
.LC1:
	.string	"8787\n"
	.align	2
.LC2:
	.string	"ormary\n"
	.align	2
.LC3:
	.string	"thread pid: "
	.align	2
.LC4:
	.string	"-- pointer: "
	.align	2
.LC5:
	.string	"veronica\n"
	.align	2
.LC6:
	.string	"geraldine\n"
	.align	2
.LC7:
	.string	"garcia\n"
	.align	2
.LC8:
	.string	"herrera\n"
	.align	2
.LC9:
	.string	"daniel quintero villegas app kernel test ' '\n"
	.align	2
.LC10:
	.string	"for\n"
	.align	2
.LC11:
	.string	"ingrese 'o' para salir \n"
	.align	2
.LC12:
	.string	"serial event ok\n"
	.align	2
.LC13:
	.string	"fail\n"
	.align	2
.LC14:
	.string	"        counter\n"
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-144
	sw	ra,140(sp)
	sw	s0,136(sp)
	addi	s0,sp,144
	sw	a0,-132(s0)
	sw	a1,-136(s0)
	lla	a5,.LC0
	sw	a5,-116(s0)
	lla	a5,.LC1
	sw	a5,-112(s0)
	lla	a5,.LC2
	sw	a5,-108(s0)
	lla	a0,.LC3
	call	mprint@plt
	addi	a5,s0,-104
	li	a2,4
	mv	a1,a5
	lw	a0,-132(s0)
	call	intToStr@plt
	addi	a5,s0,-104
	mv	a0,a5
	call	mprint@plt
	lla	a0,.LC4
	call	mprint@plt
	lw	a5,-136(s0)
	addi	a4,s0,-104
	li	a2,4
	mv	a1,a4
	mv	a0,a5
	call	intToStr@plt
	addi	a5,s0,-104
	mv	a0,a5
	call	mprint@plt
	call	println@plt
	sw	zero,-20(s0)
	j	.L14
.L15:
	lw	a5,-20(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	lw	a5,-100(a5)
	mv	a0,a5
	call	mprint@plt
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L14:
	lw	a4,-20(s0)
	li	a5,2
	ble	a4,a5,.L15
	lla	a0,.LC5
	call	mprint@plt
	lla	a0,.LC6
	call	mprint@plt
	lla	a0,.LC7
	call	mprint@plt
	lla	a0,.LC8
	call	mprint@plt
	addi	a5,s0,-104
	lla	a4,.LC9
	lw	t6,0(a4)
	lw	t5,4(a4)
	lw	t4,8(a4)
	lw	t3,12(a4)
	lw	t1,16(a4)
	lw	a7,20(a4)
	lw	a6,24(a4)
	lw	a0,28(a4)
	lw	a1,32(a4)
	lw	a2,36(a4)
	lw	a3,40(a4)
	sw	t6,0(a5)
	sw	t5,4(a5)
	sw	t4,8(a5)
	sw	t3,12(a5)
	sw	t1,16(a5)
	sw	a7,20(a5)
	sw	a6,24(a5)
	sw	a0,28(a5)
	sw	a1,32(a5)
	sw	a2,36(a5)
	sw	a3,40(a5)
	lhu	a4,44(a4)
	sh	a4,44(a5)
	addi	a5,s0,-104
	mv	a0,a5
	call	mprint@plt
	sw	zero,-20(s0)
	j	.L16
.L17:
	lla	a0,.LC10
	call	mprint@plt
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L16:
	lw	a4,-20(s0)
	li	a5,9
	ble	a4,a5,.L17
	lla	a0,.LC11
	call	mprint@plt
.L22:
	call	SerialAvailable@plt
	mv	a5,a0
	beq	a5,zero,.L18
	call	SerialRead@plt
	mv	a5,a0
	sb	a5,-21(s0)
	lbu	a4,-21(s0)
	li	a5,111
	bne	a4,a5,.L19
	lla	a0,.LC12
	call	mprint@plt
	j	.L23
.L19:
	lla	a0,.LC13
	call	mprint@plt
.L18:
	la	a5,counter_timer
	lw	a4,0(a5)
	li	a5,4096
	addi	a5,a5,904
	remu	a5,a4,a5
	bne	a5,zero,.L22
	lla	a0,.LC14
	call	mprint@plt
	j	.L22
.L23:
	lw	a0,-132(s0)
	call	kill_task@plt
	nop
	lw	ra,140(sp)
	lw	s0,136(sp)
	addi	sp,sp,144
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 9.2.0"
