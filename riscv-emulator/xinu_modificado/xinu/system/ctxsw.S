 
#  void ctxsw(char *olw_pregs, char *new_pregs);
 
#void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to);

#define __riscv_xlen 32
#define REGBYTES (__riscv_xlen / 8)
#define  RISCV_CONTEXT_SIZE (32*REGBYTES)

 


.global setjmp
setjmp:
sw	ra,0(a0)
sw	s0,4(a0)
sw	s1,8(a0)
sw	s2,12(a0)
sw	s3,16(a0)
sw	s4,20(a0)
sw	s5,24(a0)
sw	s6,28(a0)
sw	s7,32(a0)
sw	s8,36(a0)
sw	s9,40(a0)
sw	s10,44(a0)
sw	s11,48(a0)
sw	sp,52(a0)
sw  gp,56(a0)
sw  tp,60(a0)
sw  a7,64(a0)
sw  a6,68(a0)
sw  a5,72(a0)
sw  a4,76(a0)
sw  a3,80(a0)
sw  a2,84(a0)

li	a0,0
ret

.global longjmp
longjmp:
lw	ra,0(a0)
lw	s0,4(a0)
lw	s1,8(a0)
lw	s2,12(a0)
lw	s3,16(a0)
lw	s4,20(a0)
lw	s5,24(a0)
lw	s6,28(a0)
lw	s7,32(a0)
lw	s8,36(a0)
lw	s9,40(a0)
lw	s10,44(a0)
lw	s11,48(a0)
lw	sp,52(a0)
lw  gp,56(a0)
lw  tp,60(a0)
lw  a7,64(a0)
lw  a6,68(a0)
lw  a5,72(a0)
lw  a4,76(a0)
lw  a3,80(a0)
lw  a2,84(a0)

seqz	a0,a1
add	a0,a0,a1
ret



 # This Code derived from xv6-riscv (64bit)
# -- https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/swtch.S

# ============ MACRO ==================
.macro ctx_save base
        sw ra, 0(\base)
        sw sp, 4(\base)
        sw s0, 8(\base)
        sw s1, 12(\base)
        sw s2, 16(\base)
        sw s3, 20(\base)
        sw s4, 24(\base)
        sw s5, 28(\base)
        sw s6, 32(\base)
        sw s7, 36(\base)
        sw s8, 40(\base)
        sw s9, 44(\base)
        sw s10, 48(\base)
        sw s11, 52(\base)
        #sw a0, 56(\base)
        #sw a1, 60(\base)
        #sw a2, 64(\base)
        #sw a3, 68(\base)

.endm

.macro ctx_load base
        lw ra, 0(\base)
        lw sp, 4(\base)
        lw s0, 8(\base)
        lw s1, 12(\base)
        lw s2, 16(\base)
        lw s3, 20(\base)
        lw s4, 24(\base)
        lw s5, 28(\base)
        lw s6, 32(\base)
        lw s7, 36(\base)
        lw s8, 40(\base)
        lw s9, 44(\base)
        lw s10, 48(\base)
        lw s11, 52(\base)
        lw a0, 56(\base)
        lw a1, 60(\base)
        #lw a2, 64(\base)
        #lw a3, 68(\base)

.endm

# ============ Macro END   ==================
 
# Context switch
#
#   void sys_switch(struct context *old, struct context *new);
# 
# Save current registers in old. Load from new.

.global sys_switch
.align 4
sys_switch:
        ctx_save a0  # a0 => struct context *old
        ctx_load a1  # a1 => struct context *new
        ret          # pc=ra; swtch to new task (new->ra)
