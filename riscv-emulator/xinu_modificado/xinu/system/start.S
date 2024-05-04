.text
.global entry


entry:
 la sp, _estack
 #li a0, 1024*4
 #add sp, sp, a0

 la gp , _gp
 la t0, isr_vector 
 csrw mtvec, t0
 call nulluser

 loop:
 j loop

