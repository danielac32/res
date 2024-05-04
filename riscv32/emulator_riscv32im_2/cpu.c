#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "cpu.h"
#define blockEE 512
FILE *vm;
uint32_t block_store=1;
char buff[blockEE];

#define BLOCKBUFFER 16
uint32_t addr_store=1;
uint32_t addr_store2=1;
uint8_t buffer[BLOCKBUFFER];

uint16_t get_block(uint32_t addr){
     return (addr/BLOCKBUFFER) & 0xffff;
}

uint16_t get_point(uint32_t addr){
     return (addr % BLOCKBUFFER) & 0xffff;
}

void WriteFlash(uint32_t addr,uint8_t data){
  // fseek(vm, addr, SEEK_SET);
  // fwrite(&data,sizeof(data),1, vm);
  uint16_t block = get_block(addr);
  uint16_t point = get_point(addr);
  //uint16_t current = BLOCKBUFFER - point;
  
  buffer[point]=data;
  fseek(vm,block*sizeof(buffer),SEEK_SET);
  fwrite(buffer,sizeof(buffer),1,vm);
}

uint8_t ReadFlash(uint32_t addr){
 
  uint16_t block = get_block(addr);
  uint16_t point = get_point(addr);
 // uint16_t current = BLOCKBUFFER - point;
 
  fseek(vm,block*sizeof(buffer),SEEK_SET);
  fread(buffer, sizeof(buffer) ,1, vm); 
 
  return buffer[point];
}



uint8_t read_vm(uint32_t addr){
/*uint8_t i=0;
fseek(vm, addr, SEEK_SET);  
fread(&i, sizeof(char), 1, vm); 
return i; */
if(addr>67000){
    printf("read-------------error (fuera de memoria) %d\n",addr);
    exit(-1);
}
return ReadFlash(addr);
}
/*
uint8_t read_vm(uint32_t addr){
uint32_t block=0;
uint32_t res=0;
uint8_t mode=0;
block=(addr/blockEE);
if(block!=block_store)mode=0;
else mode=1;
block_store=block;
res=addr-(block * blockEE); //diferencia
if(mode==0){
   fseek(vm, block*blockEE, SEEK_SET);
   fread(buff, sizeof(char), blockEE, vm); 
   return buff[res];
}else return buff[res];

}
*/


void write_vm(uint32_t addr,uint8_t val){
/*fseek(vm, addr, SEEK_SET);
fwrite(&val,sizeof(val),1, vm);*/
if(addr>67000){
    printf("write----------error (fuera de memoria) %d\n",addr);
    exit(-1);
}
WriteFlash(addr,val);
}

uint32_t get_next(uint32_t counter){
uint8_t op[4];
for (int i = 0; i < 4; ++i)
{
	op[i]=read_vm(counter+i);
}
return (uint32_t)(op[3]<<24 | op[2]<<16 | op[1]<<8 | op[0]);
}

#define pc4 riscv.pc += 4

void w8m(uint32_t addr, uint8_t val){
     write_vm(addr,val);
}
uint8_t r8m(uint32_t addr){
	 return read_vm(addr);
}
void w16m(uint32_t addr, uint16_t val){
	 uint8_t dato[2];
	 dato[0] = val & 0xff;
	 dato[1] = (val >> 8) & 0xff;
	 fseek(vm, addr, SEEK_SET);
     fwrite(dato,1,2, vm);
	 /*for (int i = 0; i < 2; ++i)
	 {
	 	write_vm(addr + i, dato[i]);
	 }*/
}
uint16_t r16m(uint32_t addr){
     uint8_t dato[2];
     for (int i = 0; i < 2; ++i)
	 {
	 	dato[i]=read_vm(addr+i);
	 } 
	 return (dato[0] | dato[1] << 8);
}
void w32m(uint32_t addr, uint32_t val){
	 uint8_t dato[4];
	 dato[0] = val & 0xff;
	 dato[1] = (val >> 8) & 0xff;
	 dato[2] = (val >> 16) & 0xff;
	 dato[3] = (val >> 24) & 0xff;
	 fseek(vm, addr, SEEK_SET);
     fwrite(dato,1,4, vm);
	 /*for (int i = 0; i < 4; ++i)
	 {
	 	write_vm(addr + i, dato[i]);
	 }*/
}
uint32_t r32m(uint32_t addr){
    uint8_t dato[4];
    for (int i = 0; i < 4; ++i)
	{
	 	dato[i]=read_vm(addr+i);
	} 
	return dato[0] | (dato[1] << 8) | (dato[2]<< 16) | (dato[3] << 24);
}
void raise_exception(uint32_t cause,uint32_t tval)
{
   
}
/**************************************************/
//memory read

uint8_t target_read_u8(uint8_t *pval, uint32_t addr)
{
  
  if(addr==KEYPRESSED){
     //*pval = (uint8_t)kbhit(); 
  }else if(addr==KEYREAD){
     //*pval = getchar();//fgetc(stdin);
  }else if(addr==ENABLE_TIMER){
     *pval=riscv.enable_timer;
  }else{
     *pval = r8m(addr);//read_vm(addr);
  }
    return 0;
}

uint8_t target_read_u16(uint16_t *pval, uint32_t addr)
{    

    *pval = r16m(addr);// read_vm(addr+0) | (read_vm(addr+1) << 8);
    
    return 0;
}

uint8_t target_read_u32(uint32_t *pval, uint32_t addr)
{
    if(addr==workregister1){
       *pval=riscv.wreg1;
    }else if(addr==workregister2){
       *pval=riscv.wreg2;
    }else if(addr==TIMER){
       *pval=riscv.timer;//riscv.timer;
    }else{
      *pval = r32m(addr);//read_vm(addr+0) | (read_vm(addr+1) << 8) | (read_vm(addr+2)<< 16) | (read_vm(addr+3) << 24);
    }
    return 0;
}
/**************************************************/
//memory write
uint8_t target_write_u8(uint32_t addr, uint8_t val)
{
    if (addr == UART_TX_ADDR) {
        printf("%c", val);
    }else if(addr==ENABLE_TIMER){
        riscv.enable_timer=val;
    }else{
       w8m(addr,val);// write_vm(addr,val);
    }
    return 0;
}

uint8_t target_write_u16(uint32_t addr, uint16_t val)
{
   
     //write_vm(addr ,val & 0xff);
     //write_vm(addr+1 , (val >> 8) & 0xff);
	w16m(addr,val);//
    return 0;
}

uint8_t target_write_u32(uint32_t addr, uint32_t val)
{    
    
    if(addr==workregister1){
       riscv.wreg1=val;
    }else if(addr==workregister2){
       riscv.wreg2=val;
    }else{
       //printf("writed memory\n");
       /*write_vm(addr+0 , val & 0xff);
       write_vm(addr+1 , (val >> 8) & 0xff);
       write_vm(addr+2 , (val >> 16) & 0xff);
       write_vm(addr+3 ,(val >> 24) & 0xff);*/
    	w32m(addr,val);//
    }

    return 0;
}
/**************************************************/
int32_t div32(int32_t a, int32_t b)
{
    if (b == 0) {
        return -1;
    } else if (a == ((int32_t)1 << (XLEN - 1)) && b == -1) {
        return a;
    } else {
        return a / b;
    }
}

uint32_t divu32(uint32_t a, uint32_t b)
{
    if (b == 0) {
        return -1;
    } else {
        return a / b;
    }
}

int32_t rem32(int32_t a, int32_t b)
{
    if (b == 0) {
        return a;
    } else if (a == ((int32_t)1 << (XLEN - 1)) && b == -1) {
        return 0;
    } else {
        return a % b;
    }
}

uint32_t remu32(uint32_t a, uint32_t b)
{
    if (b == 0) {
        return a;
    } else {
        return a % b;
    }
}

 uint32_t mulh32(int32_t a, int32_t b)
{
    return ((int64_t)a * (int64_t)b) >> 32;
}

 uint32_t mulhsu32(int32_t a, uint32_t b)
{
    return ((int64_t)a * (int64_t)b) >> 32;
}

 uint32_t mulhu32(uint32_t a, uint32_t b)
{
    return ((int64_t)a * (int64_t)b) >> 32;
}


 void dump_regs()
{
    uint32_t i ;
    printf("x0 zero: %08x\n", riscv.reg[0]);
    printf("x1 ra:   %08x\n", riscv.reg[1]);
    printf("x2 sp:   %08x\n", riscv.reg[2]);
    printf("x3 gp:   %08x\n", riscv.reg[3]);
    printf("x4 tp:   %08x\n", riscv.reg[4]);
    printf("x5 t0:   %08x\n", riscv.reg[5]);
    printf("x6 t1:   %08x\n", riscv.reg[6]);
    printf("x7 t2:   %08x\n", riscv.reg[7]);
    printf("x8 s0:   %08x\n", riscv.reg[8]);
    printf("x9 s1:   %08x\n", riscv.reg[9]);
    printf("x10 a0:  %08x\n", riscv.reg[10]);
    printf("x11 a1:  %08x\n", riscv.reg[11]);
    printf("x12 a2:  %08x\n", riscv.reg[12]);
    printf("x13 a3:  %08x\n", riscv.reg[13]);
    printf("x14 a4:  %08x\n", riscv.reg[14]);
    printf("x15 a5:  %08x\n", riscv.reg[15]);
    printf("x16 a6:  %08x\n", riscv.reg[16]);
    printf("x17 a7:  %08x\n", riscv.reg[17]);
    printf("x18 s2:  %08x\n", riscv.reg[18]);
    printf("x19 s3:  %08x\n", riscv.reg[19]);
    printf("x20 s4:  %08x\n", riscv.reg[20]);
    printf("x21 s5:  %08x\n", riscv.reg[21]);
    printf("x22 s6:  %08x\n", riscv.reg[22]);
    printf("x23 s7:  %08x\n", riscv.reg[23]);
    printf("x24 s8:  %08x\n", riscv.reg[24]);
    printf("x25 s9:  %08x\n", riscv.reg[25]);
    printf("x26 s10: %08x\n", riscv.reg[26]);
    printf("x27 s11: %08x\n", riscv.reg[27]);
    printf("x28 t3:  %08x\n", riscv.reg[28]);
    printf("x29 t4:  %08x\n", riscv.reg[29]);
    printf("x30 t5:  %08x\n", riscv.reg[30]);
    printf("x31 t6:  %08x\n", riscv.reg[31]);
    printf("***************************************\n");
        
    printf("***************************************\n");    
}

/***********************************************************/
//cpu
//#define next
//#define riscv.debug_inst

void execute_instruction()
{
    uint32_t opcode, rd, rs1, rs2, funct3,insn;
    int32_t imm=0, cond, err;
    uint32_t addr, val, val2,temp;
    insn = get_next(riscv.pc); //(read_vm(riscv.pc+3)<<24 |read_vm(riscv.pc+2)<<16 |read_vm(riscv.pc+1)<<8 |read_vm(riscv.pc));//get_insn32(riscv.pc);
    opcode = insn & 0x7f;
    rd = (insn >> 7) & 0x1f;
    rs1 = (insn >> 15) & 0x1f;
    rs2 = (insn >> 20) & 0x1f;
  
   if(riscv.debug_inst){
    temp = riscv.pc + riscv.PROGRAM_COUNTER_START_VAL;
    printf("instruction: %08x , pc: %08x\n", insn,temp);

    }
    switch(opcode) {
    case 0x37: /* lui */
        if (rd != 0)
            riscv.reg[rd] = (int32_t)(insn & 0xfffff000);

        if(riscv.debug_inst){
        printf("lui\n");
        printf("rd: %i , rd val: %x\n", rd, riscv.reg[rd]);
        printf("rs1: %i, %i\n",rs1,riscv.reg[rs1]);
        printf("rs2: %i, %i\n",rs2,riscv.reg[rs2]);
        printf("imm: %i\n",imm );
        }
        pc4;
        break;
    case 0x17: /* auipc */
        if (rd != 0)
            riscv.reg[rd] = (int32_t)(riscv.pc + (int32_t)(insn & 0xfffff000));

        if(riscv.debug_inst){
        printf("auipc :rd :%08x reg: %08x \n", rd,riscv.reg[rd]);
        }
        pc4;
        break;
    case 0x6f: /* jal */
        imm = ((insn >> (31 - 20)) & (1 << 20)) |
              ((insn >> (21 - 1)) & 0x7fe) |
              ((insn >> (20 - 11)) & (1 << 11)) |
              (insn & 0xff000);
        
        
         imm = (imm << 11) >> 11;

        if (rd != 0)
            riscv.reg[rd] = riscv.pc + 4;

        
        riscv.pc = (int32_t)(riscv.pc + imm);

        if(riscv.debug_inst){
        printf("JAL :pc :%08x reg: %i im:%08x \n", riscv.pc,rd,imm);
        }
        break;
    case 0x67: /* jalr */
        imm = (int32_t)insn >> 20;
        val = riscv.pc + 4;
        riscv.pc = (int32_t)(riscv.reg[rs1] + imm) & ~1;
        if(riscv.debug_inst){
        printf("jalr :%08x reg: %i\n", riscv.pc,rs1);
        }
        if (rd != 0)
            riscv.reg[rd] = val;
        break;
    case 0x63:
        funct3 = (insn >> 12) & 7;
        switch(funct3 >> 1) {
        case 0: /* beq/bne */
            cond = (riscv.reg[rs1] == riscv.reg[rs2]);
            break;
        case 2: /* blt/bge */
            cond = ((int32_t)riscv.reg[rs1] < (int32_t)riscv.reg[rs2]);
            break;
        case 3: /* bltu/bgeu */
            cond = (riscv.reg[rs1] < riscv.reg[rs2]);
            break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        cond ^= (funct3 & 1);
        if (cond) {
            imm = ((insn >> (31 - 12)) & (1 << 12)) |
                  ((insn >> (25 - 5)) & 0x7e0) |
                  ((insn >> (8 - 1)) & 0x1e) |
                  ((insn << (11 - 7)) & (1 << 11));
            imm = (imm << 19) >> 19;
            riscv.pc = (int32_t)(riscv.pc + imm);
            break;
        }else pc4;
        if(riscv.debug_inst){
        printf("compare\n");
        }
        break;
    case 0x03: /* load */
        funct3 = (insn >> 12) & 7;
        imm = (int32_t)insn >> 20;
        addr = riscv.reg[rs1] + imm;

        switch(funct3) {
        case 0: /* lb */
        {
            uint8_t rval;
            if(riscv.debug_inst){
            printf("lb\n");
            }
            if (target_read_u8(&rval, addr)) {
                raise_exception(riscv.pending_exception, riscv.pending_tval);
                return;
            }
            val = (int8_t)rval;
        }
        break;
        case 1: /* lh */
        {
            uint16_t rval;
            if (target_read_u16(&rval, addr)) {
                raise_exception(riscv.pending_exception, riscv.pending_tval);
                return;
            }
            val = (int16_t)rval;
        }
        break;
        case 2: /* lw */
        {
            if(riscv.debug_inst){
            printf("lw\n");
            }
            uint32_t rval;
            if (target_read_u32(&rval, addr)) {
                raise_exception(riscv.pending_exception, riscv.pending_tval);
                return;
            }
            val = (int32_t)rval;
        }
        break;
        case 4: /* lbu */
        {
            if(riscv.debug_inst){
            printf("lbu\n");
            }
            uint8_t rval;

            if (target_read_u8(&rval, addr)) {
                raise_exception(riscv.pending_exception, riscv.pending_tval);
                return;
            }
            val = rval;
        }
        break;
        case 5: /* lhu */
        {
            uint16_t rval;
            if (target_read_u16(&rval, addr)) {
                raise_exception(riscv.pending_exception, riscv.pending_tval);
                return;
            }
            val = rval;
        }
        break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        if (rd != 0)
            riscv.reg[rd] = val;

        if(riscv.debug_inst){
        printf("addr: %x\n",addr );
        printf("rd: %i , rd val: %x\n", rd, riscv.reg[rd]);
        printf("rs1: %i, %i\n",rs1,riscv.reg[rs1]);
        printf("rs2: %i, %i\n",rs2,riscv.reg[rs2]);
        printf("imm: %i\n",imm );
        printf("val: %x\n", val);
        }
        pc4;
        break;
    case 0x23: /* store */
        funct3 = (insn >> 12) & 7;
        imm = rd | ((insn >> (25 - 5)) & 0xfe0);
        imm = (imm << 20) >> 20;
        addr = riscv.reg[rs1] + imm;
        val = riscv.reg[rs2];
        //printf("addr: %x\n",addr);
        switch(funct3) {
        case 0: /* sb */
            if (target_write_u8(addr, val)) {
                raise_exception(riscv.pending_exception, riscv.pending_tval);
                return;
            }
            if(riscv.debug_inst){
            printf("sb\n");
            }
            break;
        case 1: /* sh */
            if (target_write_u16(addr, val)) {
                raise_exception(riscv.pending_exception, riscv.pending_tval);
                return;
            }
            break;
        case 2: /* sw */
            if (target_write_u32(addr, val)) {
                raise_exception(riscv.pending_exception, riscv.pending_tval);
                return;
            } 
            if(riscv.debug_inst){
            printf("sw\n");
            }
            break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }

        if(riscv.debug_inst){
        printf("addr: %x\n",addr);
        printf("val: %x\n", val);
        printf("rd: %i\n", rd);
        printf("rs1: %i, %x\n",rs1,riscv.reg[rs1]);
        printf("rs2: %i, %x\n",rs2,riscv.reg[rs2]);
        printf("imm: %x\n",imm );
        }
        pc4;
        break;
    case 0x13:
        funct3 = (insn >> 12) & 7;
        imm = (int32_t)insn >> 20;
        switch(funct3) {
        case 0: /* addi */
            val = (int32_t)(riscv.reg[rs1] + imm) ;//& 0x0000ffff;

            if(riscv.debug_inst){
            printf("addi/li : val: %x , reg1: %x , reg2: %x imm: %x\n",val,rs1,rs2,imm);
            }
            break;
        case 1: /* slli */
            if ((imm & ~(XLEN - 1)) != 0) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            val = (int32_t)(riscv.reg[rs1] << (imm & (XLEN - 1)));
            break;
        case 2: /* slti */
            val = (int32_t)riscv.reg[rs1] < (int32_t)imm;
            break;
        case 3: /* sltiu */
            val = riscv.reg[rs1] < (uint32_t)imm;
            break;
        case 4: /* xori */
            val = riscv.reg[rs1] ^ imm;
            break;
        case 5: /* srli/srai */
            if ((imm & ~((XLEN - 1) | 0x400)) != 0) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            if (imm & 0x400)
                val = (int32_t)riscv.reg[rs1] >> (imm & (XLEN - 1));
            else
                val = (int32_t)((uint32_t)riscv.reg[rs1] >> (imm & (XLEN - 1)));
            break;
        case 6: /* ori */
            val = riscv.reg[rs1] | imm;
            break;
        default:
        case 7: /* andi */
            val = riscv.reg[rs1] & imm;
            break;
        }
        if (rd != 0)
            riscv.reg[rd] = val;

        pc4;
        break;
    case 0x33:
        imm = insn >> 25;
        val = riscv.reg[rs1];
        val2 = riscv.reg[rs2];
        if (imm == 1) {
            funct3 = (insn >> 12) & 7;
            switch(funct3) {
            case 0: /* mul */
                val = (int32_t)((int32_t)val * (int32_t)val2);
                break;
            case 1: /* mulh */
                val = (int32_t)mulh32(val, val2);
                break;
            case 2:/* mulhsu */
                val = (int32_t)mulhsu32(val, val2);
                break;
            case 3:/* mulhu */
                val = (int32_t)mulhu32(val, val2);
                break;
            case 4:/* div */
                val = div32(val, val2);
                break;
            case 5:/* divu */
                val = (int32_t)divu32(val, val2);
                break;
            case 6:/* rem */
                val = rem32(val, val2);
                break;
            case 7:/* remu */
                val = (int32_t)remu32(val, val2);
                break;
            default:
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
        } else {
            if (imm & ~0x20) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            funct3 = ((insn >> 12) & 7) | ((insn >> (30 - 3)) & (1 << 3));
            switch(funct3) {
            case 0: /* add */
                if(riscv.debug_inst){
                printf("add\n");
                }
                val = (int32_t)(val + val2);
                break;
            case 0 | 8: /* sub */
                val = (int32_t)(val - val2);
                break;
            case 1: /* sll */
                val = (int32_t)(val << (val2 & (XLEN - 1)));
                break;
            case 2: /* slt */
                val = (int32_t)val < (int32_t)val2;
                break;
            case 3: /* sltu */
                val = val < val2;
                break;
            case 4: /* xor */
                val = val ^ val2;
                break;
            case 5: /* srl */
                val = (int32_t)((uint32_t)val >> (val2 & (XLEN - 1)));
                break;
            case 5 | 8: /* sra */
                val = (int32_t)val >> (val2 & (XLEN - 1));
                break;
            case 6: /* or */
                val = val | val2;
                break;
            case 7: /* and */
                val = val & val2;
                break;
            default:
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
        }
        if (rd != 0)
            riscv.reg[rd] = val;

        pc4;
        break;
        case 0x73:
        funct3 = (insn >> 12) & 7;
        imm = insn >> 20;
        if (funct3 & 4)
            val = rs1;
        else
            val = riscv.reg[rs1];
        funct3 &= 3;
        
        
        if(imm==2){//uret
           /*if(riscv.debug_inst){
              printf("(ret)temp: %08x , pc: %08x , cycles: %08x\n",riscv.temp2,riscv.pc,riscv.cycles);
           }*/
           riscv.pc = riscv.temp2;
        }else{
      switch(funct3) {
                  case 0:
                    switch(riscv.reg[17]){
                          case 4://string 
                             while(read_vm(riscv.reg[10])!='\0'){
                                   printf("%c",read_vm(riscv.reg[10]));
                                   riscv.reg[10]++;
                             }
                          break;
                          case 5://readint
                            //scanf("%x",&riscv.reg[10]);
                            //printf("%x\n",riscv.reg[10]);
                          break;
                          case 8:///readstring
                            //printf("readstring:\n");
                            //printf("a0: %08x \n", riscv.reg[10]);
                            //printf("a1: %08x \n", riscv.reg[11]);
                            /*char *p;
                            p=(char*)malloc(riscv.reg[11]);
                            scanf("%s",p);
                            printf("%s\n",p);*/
                          break;
                          case 10://exit
                            printf("EXIT RISCV32\n");
                            riscv.running=0;
                          break;
                          case 11://print char
                             printf("%c",(char)riscv.reg[10]);
                          break;
                          case 12:
 
                          break;
                          case 13:
                             
                          break;
                          case 14://open file
                             
                          break;
                          case 15:
             
                          break;
                          case 16:
                
                          break;
                          case 17:
                
                          break;
                    }
                break;
           }
           pc4;
    }
       break;
    }
}





void init_soc(){
	int a=0;
	uint8_t data[32];
    uint32_t i=0,p=0,k=0;


	printf("iniciando cpu\n");
	init_variable();
	riscv.next=0;
    riscv.debug_inst=0;
    vm=fopen("vm.bin","r+");
     

    /*printf("dump flash  (%d)\n",50100);
  
    for(uint32_t i=0;i<50100;i++){
      if(!(i % 2))printf(" "); 
      if(!(i % 16))printf("    (%08x)\n",k); 
      if(!(i % 16))k+=16;
      printf("%02x",ReadFlash(i));  
    }
    printf("\n");
    i=0;
    p=0;
    k=0;

    for (uint16_t i = 4095; i < 4095+16; ++i)
    {
        printf("%02x ,",ReadFlash(i));  
    }
    printf("\n");

    for (uint16_t i = 4095; i < 4095+16; ++i)
    {
         WriteFlash(i,p);
         p+=1;
    }

    printf("\n");

    for (uint16_t i = 4095; i < 4095+16; ++i)
    {
        printf("%02x ,",ReadFlash(i));  
    }
    printf("\n");
*/
 

    while(riscv.running){
    	  if(riscv.next){
             scanf("%i",&a);
             if(a==10)dump_regs();
          }
          execute_instruction();
          if(riscv.enable_timer){
             if(!(riscv.cycles % 2500)){
             	 riscv.temp2 = riscv.pc;
                 riscv.pc = vector_interrupt;
                 riscv.cycles=0;
                 riscv.timer++;
             }
          }
          riscv.cycles++;
    }
    printf("end\n");
    fclose(vm);
}



