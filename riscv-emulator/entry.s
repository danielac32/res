.text
.global entry


entry:
 la sp, stack0
 li a0, 1024*4
 add sp, sp, a0

 #la sp,_sp
 la gp , _gp
 la t0, isr_vector 
 csrw mtvec, t0
 call main

 loop:
 j loop

