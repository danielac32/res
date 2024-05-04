.global save_context
.global restore_context


save_context:
addi  sp,sp,-120
sw    ra,116(sp)
sw    t6,112(sp)
sw    t5,108(sp)
sw    t4,104(sp)
sw    t3,100(sp)
sw    s11,96(sp)
sw    s10,92(sp)
sw    s9,88(sp)
sw    s8,84(sp)
sw    s7,80(sp)
sw    s6,76(sp)
sw    s5,72(sp)
sw    s4,68(sp)
sw    s3,64(sp)
sw    s2,60(sp)
sw    a7,56(sp)
sw    a6,52(sp)
sw    a5,48(sp)
sw    a4,44(sp)
sw    a3,40(sp)
sw    a2,36(sp)
sw    a1,32(sp)
sw    a0,28(sp)
sw    s1,24(sp)
sw    s0,20(sp)
sw    t2,16(sp)
sw    t1,12(sp)
sw    t0,8(sp) 
sw    tp,4(sp) 
sw    gp,0(sp) 
ret

restore_context:
lw    ra,116(sp)
lw    t6,112(sp)
lw    t5,108(sp)
lw    t4,104(sp)
lw    t3,100(sp)

lw    s11,96(sp)
lw    s10,92(sp)
lw    s9,88(sp)
lw    s8,84(sp)
lw    s7,80(sp)
lw    s6,76(sp)
lw    s5,72(sp)
lw    s4,68(sp)
lw    s3,64(sp)
lw    s2,60(sp)
lw    a7,56(sp)
lw    a6,52(sp)
lw    a5,48(sp)
lw    a4,44(sp)
lw    a3,40(sp)
lw    a2,36(sp)
lw    a1,32(sp)
lw    a0,28(sp)
lw    s1,24(sp)
lw    s0,20(sp)
lw    t2,16(sp)
lw    t1,12(sp)
lw    t0,8(sp)
lw    tp,4(sp)
lw    gp,0(sp)
addi  sp,sp,120
ret