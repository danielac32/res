.global thread_switch
.global thread_start
thread_switch:
     addi sp,sp,-52
     sw  ra,0(sp)
     sw  s0,4(sp)
     sw  s1,8(sp)
     sw  s2,12(sp)
     sw  s3,16(sp)
     sw  s4,20(sp)
     sw  s5,24(sp)
     sw  s6,28(sp)
     sw  s7,32(sp)
     sw  s8,36(sp)
     sw  s9,40(sp)
     sw  s10,44(sp)
     sw  s11,48(sp)

     sw a0, 0(sp)

     lw a1, 0(sp)
 
     lw  ra,0(sp)
     lw  s0,4(sp)
     lw  s1,8(sp)
     lw  s2,12(sp)
     lw  s3,16(sp)
     lw  s4,20(sp)
     lw  s5,24(sp)
     lw  s6,28(sp)
     lw  s7,32(sp)
     lw  s8,36(sp)
     lw  s9,40(sp)
     lw  s10,44(sp)
     lw  s11,48(sp)
     addi sp,sp,52
    ret

thread_start:
     addi sp,sp,-52
     sw  ra,0(sp)
     sw  s0,4(sp)
     sw  s1,8(sp)
     sw  s2,12(sp)
     sw  s3,16(sp)
     sw  s4,20(sp)
     sw  s5,24(sp)
     sw  s6,28(sp)
     sw  s7,32(sp)
     sw  s8,36(sp)
     sw  s9,40(sp)
     sw  s10,44(sp)
     sw  s11,48(sp)
     sw a0, 0(sp)
     lw a1, 0(sp)

     jal thread_wrap
     ret
     
     
