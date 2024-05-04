

.data
str: .string "\n"
str1: .string "*****************************\n"
str2: .string "      SOC RISCV32 1.0        \n"

cad1: .space 20

.text 
boot:
#init sp,gp register
li sp,0x000fc70f
li gp,0x000fc710
#
la a0,str
li a7,4
ecall
la a0,str
li a7,4
ecall
la a0,str1
li a7,4
ecall
la a0,str2
li a7,4
ecall
la a0,str1
li a7,4
ecall
la a0,str
li a7,4
ecall
la a0,str
li a7,4
ecall

#readstring
#la a0,cad1
#li a1,20
#li a7,8
#ecall
#readint
#li a7,5
#ecall
j main
