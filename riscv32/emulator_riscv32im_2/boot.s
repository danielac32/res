

.data
str: .string "\n"
str1: .string "*****************************\n"
str2: .string "      SOC RISCV32IM 1.1        \n"

cad1: .space 20

.text 
boot:
#init sp,gp register
li sp,0xF9FE
li gp,0xFA00
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

j main
