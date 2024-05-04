	.file	"syscall.c"
	.option pic
	.attribute arch, "rv32i2p0_m2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.comm	counter_timer,4,4
	.comm	mem,2048,4
	.comm	buff,80,4
	.comm	processlist,240,4
	.comm	alloc,500580,4
	.comm	core,20,4
	.align	2
	.globl	syscall
	.type	syscall, @function
syscall:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	sw	a2,-28(s0)
	sw	a3,-32(s0)
	lw	a7,-20(s0)
	lw	a0,-24(s0)
	lw	a1,-28(s0)
	lw	a2,-32(s0)
 #APP
# 17 "lib/syscall.c" 1
	ecall
# 0 "" 2
 #NO_APP
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	syscall, .-syscall
	.align	2
	.globl	SerialRead
	.type	SerialRead, @function
SerialRead:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,13
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L3
.L4:
 #APP
# 25 "lib/syscall.c" 1
	nop
# 0 "" 2
# 26 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L3:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L4
	li	a5,3145728
	addi	a5,a5,4
	lw	a5,0(a5)
	andi	a5,a5,0xff
	mv	a0,a5
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	SerialRead, .-SerialRead
	.align	2
	.globl	SerialAvailable
	.type	SerialAvailable, @function
SerialAvailable:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,12
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L7
.L8:
 #APP
# 36 "lib/syscall.c" 1
	nop
# 0 "" 2
# 37 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L7:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L8
	li	a5,3145728
	addi	a5,a5,4
	lw	a5,0(a5)
	mv	a0,a5
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	SerialAvailable, .-SerialAvailable
	.align	2
	.globl	exit_cpu
	.type	exit_cpu, @function
exit_cpu:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,10
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L11
.L12:
 #APP
# 47 "lib/syscall.c" 1
	nop
# 0 "" 2
# 48 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L11:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L12
	nop
	nop
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	exit_cpu, .-exit_cpu
	.align	2
	.globl	println
	.type	println, @function
println:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	addi	s0,sp,16
	li	a0,10
	call	write_char@plt
	nop
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	println, .-println
	.align	2
	.globl	write_char
	.type	write_char, @function
write_char:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	mv	a5,a0
	sb	a5,-17(s0)
	la	a5,buff
	lbu	a4,-17(s0)
	sb	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,4
	la	a4,buff
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,11
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L15
.L16:
 #APP
# 62 "lib/syscall.c" 1
	nop
# 0 "" 2
# 63 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L15:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L16
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	write_char, .-write_char
	.align	2
	.globl	mprint
	.type	mprint, @function
mprint:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	lw	a1,-20(s0)
	la	a0,buff
	call	strcpy@plt
	li	a5,3145728
	addi	a5,a5,4
	la	a4,buff
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,4
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L18
.L19:
 #APP
# 73 "lib/syscall.c" 1
	nop
# 0 "" 2
# 74 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L18:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L19
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	nop
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	mprint, .-mprint
	.align	2
	.globl	write_text
	.type	write_text, @function
write_text:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	li	a5,3145728
	addi	a5,a5,4
	lw	a4,-20(s0)
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,4
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L21
.L22:
 #APP
# 83 "lib/syscall.c" 1
	nop
# 0 "" 2
# 84 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L21:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L22
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	write_text, .-write_text
	.align	2
	.globl	yield
	.type	yield, @function
yield:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	li	a5,3145728
	addi	a5,a5,4
	lw	a4,-20(s0)
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,5
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L24
.L25:
 #APP
# 94 "lib/syscall.c" 1
	nop
# 0 "" 2
# 95 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L24:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L25
	nop
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	yield, .-yield
	.align	2
	.globl	kill_task
	.type	kill_task, @function
kill_task:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	li	a5,3145728
	addi	a5,a5,4
	lw	a4,-20(s0)
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,200
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L27
.L28:
 #APP
# 104 "lib/syscall.c" 1
	nop
# 0 "" 2
# 105 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L27:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L28
	nop
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	kill_task, .-kill_task
	.align	2
	.globl	exit_
	.type	exit_, @function
exit_:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,200
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L30
.L31:
 #APP
# 114 "lib/syscall.c" 1
	nop
# 0 "" 2
# 115 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L30:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L31
	nop
	nop
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	exit_, .-exit_
	.align	2
	.globl	closefile
	.type	closefile, @function
closefile:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,17
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L33
.L34:
 #APP
# 124 "lib/syscall.c" 1
	nop
# 0 "" 2
# 125 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L33:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L34
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	nop
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	closefile, .-closefile
	.align	2
	.globl	openfile
	.type	openfile, @function
openfile:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	li	a5,3145728
	addi	a5,a5,4
	lw	a4,-20(s0)
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,14
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L36
.L37:
 #APP
# 135 "lib/syscall.c" 1
	nop
# 0 "" 2
# 136 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L36:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L37
	li	a5,3145728
	addi	a5,a5,5
	lw	a5,0(a5)
	mv	a0,a5
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	openfile, .-openfile
	.align	2
	.globl	readfile
	.type	readfile, @function
readfile:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,6
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,15
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L40
.L41:
 #APP
# 146 "lib/syscall.c" 1
	nop
# 0 "" 2
# 147 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L40:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L41
	li	a5,3145728
	addi	a5,a5,6
	lw	a5,0(a5)
	andi	a5,a5,0xff
	mv	a0,a5
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	readfile, .-readfile
	.align	2
	.globl	filesize
	.type	filesize, @function
filesize:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,4
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,6
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,16
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L44
.L45:
 #APP
# 157 "lib/syscall.c" 1
	nop
# 0 "" 2
# 158 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L44:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L45
	li	a5,3145728
	addi	a5,a5,6
	lw	a5,0(a5)
	mv	a0,a5
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	filesize, .-filesize
	.align	2
	.globl	get_time
	.type	get_time, @function
get_time:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,6
	sw	zero,0(a5)
	li	a5,3145728
	addi	a5,a5,5
	li	a4,18
	sw	a4,0(a5)
	li	a5,3145728
	addi	a5,a5,1
	li	a4,1
	sb	a4,0(a5)
	j	.L48
.L49:
 #APP
# 168 "lib/syscall.c" 1
	nop
# 0 "" 2
# 169 "lib/syscall.c" 1
	nop
# 0 "" 2
 #NO_APP
.L48:
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a5,a5,0xff
	bne	a5,zero,.L49
	li	a5,3145728
	addi	a5,a5,6
	lw	a5,0(a5)
	mv	a0,a5
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	get_time, .-get_time
	.align	2
	.globl	calls
	.type	calls, @function
calls:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	addi	s0,sp,16
	li	a5,3145728
	addi	a5,a5,1
	lbu	a5,0(a5)
	andi	a4,a5,0xff
	li	a5,1
	bne	a4,a5,.L69
	li	a5,3145728
	addi	a5,a5,5
	lw	a5,0(a5)
	li	a4,18
	bgtu	a5,a4,.L53
	li	a4,4
	bltu	a5,a4,.L54
	addi	a5,a5,-4
	li	a4,14
	bgtu	a5,a4,.L54
	slli	a4,a5,2
	lla	a5,.L56
	add	a5,a4,a5
	lw	a4,0(a5)
	lla	a5,.L56
	add	a5,a4,a5
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L56:
	.word	.L66-.L56
	.word	.L65-.L56
	.word	.L54-.L56
	.word	.L54-.L56
	.word	.L54-.L56
	.word	.L54-.L56
	.word	.L64-.L56
	.word	.L63-.L56
	.word	.L62-.L56
	.word	.L61-.L56
	.word	.L60-.L56
	.word	.L59-.L56
	.word	.L58-.L56
	.word	.L57-.L56
	.word	.L55-.L56
	.text
.L53:
	li	a4,200
	beq	a5,a4,.L67
	j	.L54
.L55:
	li	a5,3145728
	addi	a5,a5,6
	la	a4,counter_timer
	lw	a4,0(a4)
	sw	a4,0(a5)
	j	.L54
.L57:
	li	a3,0
	li	a2,0
	li	a1,0
	li	a0,17
	call	syscall@plt
	j	.L54
.L58:
	li	a3,0
	li	a2,0
	li	a1,0
	li	a0,16
	call	syscall@plt
	j	.L54
.L59:
	li	a3,0
	li	a2,0
	li	a1,0
	li	a0,15
	call	syscall@plt
	j	.L54
.L60:
	li	a5,3145728
	addi	a5,a5,4
	lw	a5,0(a5)
	li	a3,0
	li	a2,0
	mv	a1,a5
	li	a0,14
	call	syscall@plt
	j	.L54
.L64:
	li	a3,0
	li	a2,0
	li	a1,0
	li	a0,10
	call	syscall@plt
	j	.L54
.L62:
	li	a5,3145728
	addi	a5,a5,8
	lbu	a5,0(a5)
	andi	a4,a5,0xff
	li	a5,3145728
	addi	a5,a5,4
	sw	a4,0(a5)
	j	.L54
.L61:
	li	a5,3145728
	addi	a5,a5,9
	lbu	a5,0(a5)
	andi	a4,a5,0xff
	li	a5,3145728
	addi	a5,a5,4
	sw	a4,0(a5)
	j	.L54
.L63:
	li	a5,3145728
	addi	a5,a5,4
	lw	a5,0(a5)
	li	a3,0
	li	a2,0
	mv	a1,a5
	li	a0,11
	call	syscall@plt
	j	.L54
.L66:
	li	a5,3145728
	addi	a5,a5,4
	lw	a5,0(a5)
	li	a3,0
	li	a2,0
	mv	a1,a5
	li	a0,4
	call	syscall@plt
	j	.L54
.L65:
	li	a5,3145728
	addi	a5,a5,4
	lw	a5,0(a5)
	li	a3,0
	li	a2,0
	mv	a1,a5
	li	a0,5
	call	syscall@plt
	j	.L54
.L67:
	li	a5,3145728
	addi	a5,a5,4
	lw	a5,0(a5)
	ble	a5,zero,.L70
	li	a5,3145728
	addi	a5,a5,4
	lw	a5,0(a5)
	mv	a3,a5
	la	a4,processlist
	mv	a5,a3
	slli	a5,a5,1
	add	a5,a5,a3
	slli	a5,a5,4
	add	a5,a4,a5
	sw	zero,8(a5)
.L70:
	nop
.L54:
	li	a5,3145728
	addi	a5,a5,1
	sb	zero,0(a5)
.L69:
	nop
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	calls, .-calls
	.ident	"GCC: (GNU) 9.2.0"
