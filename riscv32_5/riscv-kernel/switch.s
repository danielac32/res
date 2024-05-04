
.global __strlen
.global thread_switch
.global thread_start


#************************************************************************
__strlen:	 
mv a1,zero
mv a2,a0
init:
lb	a1, 0(a0)
addi a0,a0,1
bne a1,zero,init
sub a0,a0,a2
ret

#************************************************************************

 
#************************************************************************


thread_start:
/*
sw    s11,68(sp)
sw    s10,64(sp)
sw    s9,60(sp)
sw    s8,56(sp)
sw    s7,52(sp)
sw    s6,48(sp)
sw    s5,44(sp)
sw    s4,40(sp)
sw    s3,36(sp)
sw    s2,32(sp)
sw    a7,28(sp)
sw    a6,24(sp)
sw    a5,20(sp)
sw    a4,16(sp)
sw    a3,12(sp)
sw    a2,8(sp)
sw    s1,4(sp)
sw    s0,0(sp)
*/
#sw   sp, 0(a0)  
lw   sp, 0(a1) 

ret


thread_switch:
#sw    ra,76(sp)
#sw    gp,72(sp)
sw    s11,68(sp)
sw    s10,64(sp)
sw    s9,60(sp)
sw    s8,56(sp)
sw    s7,52(sp)
sw    s6,48(sp)
sw    s5,44(sp)
sw    s4,40(sp)
sw    s3,36(sp)
sw    s2,32(sp)
sw    a7,28(sp)
sw    a6,24(sp)
sw    a5,20(sp)
sw    a4,16(sp)
sw    a3,12(sp)
sw    a2,8(sp)
sw    s1,4(sp)
sw    s0,0(sp)

sw   sp, 0(a0)

lw   sp, 0(a1)
 
#lw    ra,76(sp)
#lw    gp,72(sp)
lw    s11,68(sp)
lw    s10,64(sp)
lw    s9,60(sp)
lw    s8,56(sp)
lw    s7,52(sp)
lw    s6,48(sp)
lw    s5,44(sp)
lw    s4,40(sp)
lw    s3,36(sp)
lw    s2,32(sp)
lw    a7,28(sp)
lw    a6,24(sp)
lw    a5,20(sp)
lw    a4,16(sp)
lw    a3,12(sp)
lw    a2,8(sp)
lw    s1,4(sp)
lw    s0,0(sp)
ret

#************************************************************************
.global save_context
save_context:

sw    t6,100(sp) 
sw    t5,96(sp) 
sw    t4,92(sp) 
sw    t3,88(sp) 

sw    t2,84(sp)  
sw    t1,80(sp)  
sw    t0,76(sp)   
sw    tp,72(sp)   

sw    s11,68(sp)
sw    s10,64(sp)
sw    s9,60(sp)
sw    s8,56(sp)
sw    s7,52(sp)
sw    s6,48(sp)
sw    s5,44(sp)
sw    s4,40(sp)
sw    s3,36(sp)
sw    s2,32(sp)
sw    a7,28(sp)
sw    a6,24(sp)
sw    a5,20(sp)
sw    a4,16(sp)
sw    a3,12(sp)
sw    a2,8(sp)
sw    s1,4(sp)
sw    s0,0(sp)
sw    sp, 0(a0)
ret

.global restore_context
restore_context:
lw   sp, 0(a0)

lw    t6,100(sp) 
lw    t5,96(sp) 
lw    t4,92(sp) 
lw    t3,88(sp) 

lw    t2,84(sp)  
lw    t1,80(sp)  
lw    t0,76(sp)   
lw    tp,72(sp) 

lw    s11,68(sp)
lw    s10,64(sp)
lw    s9,60(sp)
lw    s8,56(sp)
lw    s7,52(sp)
lw    s6,48(sp)
lw    s5,44(sp)
lw    s4,40(sp)
lw    s3,36(sp)
lw    s2,32(sp)
lw    a7,28(sp)
lw    a6,24(sp)
lw    a5,20(sp)
lw    a4,16(sp)
lw    a3,12(sp)
lw    a2,8(sp)
lw    s1,4(sp)
lw    s0,0(sp)
ret


















#************************************************************************
.global save_context_kernel
save_context_kernel:
addi sp,sp,-80
sw    s11,68(sp)
sw    s10,64(sp)
sw    s9,60(sp)
sw    s8,56(sp)
sw    s7,52(sp)
sw    s6,48(sp)
sw    s5,44(sp)
sw    s4,40(sp)
sw    s3,36(sp)
sw    s2,32(sp)
sw    a7,28(sp)
sw    a6,24(sp)
sw    a5,20(sp)
sw    a4,16(sp)
sw    a3,12(sp)
sw    a2,8(sp)
sw    s1,4(sp)
sw    s0,0(sp)
ret

.global restore_context_kernel
restore_context_kernel:
lw    s11,68(sp)
lw    s10,64(sp)
lw    s9,60(sp)
lw    s8,56(sp)
lw    s7,52(sp)
lw    s6,48(sp)
lw    s5,44(sp)
lw    s4,40(sp)
lw    s3,36(sp)
lw    s2,32(sp)
lw    a7,28(sp)
lw    a6,24(sp)
lw    a5,20(sp)
lw    a4,16(sp)
lw    a3,12(sp)
lw    a2,8(sp)
lw    s1,4(sp)
lw    s0,0(sp)
addi sp,sp,80
ret