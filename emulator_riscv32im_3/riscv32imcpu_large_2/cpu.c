#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "cpu.h"


#define memory_read(addr) riscv.memory[addr]
#define memory_write(addr,val) riscv.memory[addr]=val
#define pc4 riscv.pc += 4

FILE * filebin = NULL;
void raise_exception(uint32_t cause,uint32_t tval)
{
   
}
/**************************************************/
//memory read

uint8_t target_read_u8(uint8_t *pval, uint32_t addr)
{
  
  if(addr==MODE_CPU){
     *pval=riscv.mode;
  }else if(addr==ENABLE_TIMER){
     *pval=riscv._enable_timer_;
  }else if(addr==KEYPRESSED){
     *pval = (uint8_t)kbhit(); 
  }else if(addr==KEYREAD){
     *pval = getchar();//fgetc(stdin);
  }else{
     *pval = memory_read(addr);
  }
    return 0;
}

uint8_t target_read_u16(uint16_t *pval, uint32_t addr)
{
    *pval = memory_read(addr+0) | (memory_read(addr+1) << 8);
    
    return 0;
}

uint8_t target_read_u32(uint32_t *pval, uint32_t addr)
{
  if(addr==workregister1){
     *pval=riscv.wreg1;
  }else if(addr==workregister2){
     *pval=riscv.wreg2;
    }else if(addr==workregister3){
       *pval=riscv.wreg3;
    }else if(addr==workregister4){
     *pval=riscv.wreg4;
    }else if(addr==workregister5){
     *pval=riscv.wreg5;
    }else if(addr==workregister6){
     *pval=riscv.wreg6;
    }else if(addr==workregister7){
     *pval=riscv.wreg7;
    }else if(addr==TIMER){
     *pval=counter_timer;//riscv.timer;
    }else{
      *pval = memory_read(addr+0) | (memory_read(addr+1) << 8) | (memory_read(addr+2)<< 16) | (memory_read(addr+3) << 24);
    }
    return 0;
}
/**************************************************/
//memory write
uint8_t target_write_u8(uint32_t addr, uint8_t val)
{
    if (addr == riscv.UART_TX_ADDR) {
        printf("%c", val);
    }else if(addr==MODE_CPU){
        riscv.mode=val;
    }else if(addr==ENABLE_TIMER){
        riscv._enable_timer_=val;
    }else{
         memory_write(addr,val);
    }
    return 0;
}

uint8_t target_write_u16(uint32_t addr, uint16_t val)
{
   
     memory_write(addr ,val & 0xff);
     memory_write(addr+1 , (val >> 8) & 0xff);
    return 0;
}

uint8_t target_write_u32(uint32_t addr, uint32_t val)
{    
    
    if(addr==workregister1){
     riscv.wreg1=val;
    }else if(addr==workregister2){
     riscv.wreg2=val;
    }else if(addr==workregister3){
       riscv.wreg3=val;
    }else if(addr==workregister4){
     riscv.wreg4=val;
    }else if(addr==workregister5){
     riscv.wreg5=val;
    }else if(addr==workregister6){
       //printf("writed 6: %lx\n",addr);
     riscv.wreg6=val;
    }else if(addr==workregister7){
      //printf("writed 7: %lx\n",addr);
     riscv.wreg7=val;
    }else{
       //printf("writed memory\n");
       memory_write(addr+0 , val & 0xff);
       memory_write(addr+1 , (val >> 8) & 0xff);
       memory_write(addr+2 , (val >> 16) & 0xff);
       memory_write(addr+3 ,(val >> 24) & 0xff);
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
    for (i= riscv.base_sp; i > riscv.reg[2]; i-=4)
    {
        printf("sp: %08x\n", memory_read(i+0) | (memory_read(i+1) << 8) | (memory_read(i+2) << 16) | (memory_read(i+3) << 24) );
    }
    //printf("sp: %08lx\n", memory[0x0016e35c+0] | (memory[0x0016e35c+1] << 8) | (memory[0x0016e35c+2] << 16) | (memory[0x0016e35c+3] << 24) );
    
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
    insn = (memory_read(riscv.pc+3)<<24 |memory_read(riscv.pc+2)<<16 |memory_read(riscv.pc+1)<<8 |memory_read(riscv.pc));//get_insn32(riscv.pc);
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
           if(riscv.debug_inst){
              printf("(ret)temp: %08x , pc: %08x , cycles: %08x\n",riscv.temp2,riscv.pc,riscv.cycles);
           }
           riscv.pc = riscv.temp2;
        }else{
      switch(funct3) {
                  case 0:
                    if(riscv.debug_inst){
                       printf("ecall \n");
                       printf("a0: %08x \n", riscv.reg[10]);
                       printf("a7: %08x \n", riscv.reg[17]);
                    }
                
                    switch(riscv.reg[17]){
                          case 4://string 
                             while(memory_read(riscv.reg[10])!='\0'){
                                   printf("%c",memory_read(riscv.reg[10]));
                                   riscv.reg[10]++;
                             }
                          break;
                          case 5://readint
                               //printf("readint:\n");
                            scanf("%x",&riscv.reg[10]);
                            printf("%x\n",riscv.reg[10]);
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
                            riscv.__running=0;
                          break;
                          case 11://print char

                             printf("%c",(char)riscv.reg[10]);
                          break;
                          case 12:
                             //printf("ecall jump %x\n", riscv.reg[10]);
                             riscv.pc = riscv.reg[10]-4;
                          break;
                          case 13:
                             //printf("                         ecall kernel mode\n");
                          break;
                          case 14://open file
                             {
                               
                              int i=0;
                              int j=0;
                              strcpy(riscv.name,"");
                              while(memory_read(riscv.reg[10])!='\0'){
                                   riscv.name[i]=memory_read(riscv.reg[10]);
                                   riscv.reg[10]++;
                                   i++;
                              }
                              riscv.name[i]='\0';
                              i=strlen(riscv.name);
                              //name[i-1]='\0';
                              //printf("file: %s\n",riscv.name);
                              filebin = fopen(riscv.name, "rb");
                              if(filebin == NULL){
                                 riscv.wreg2=1;
                              }else{
                                 riscv.wreg2=0;
                              }
                            }
                          break;
              case 15:
                
                {
                  char c;
                  if(filebin!=NULL){
                        /*while ((c = getc(filebin)) != EOF){
                                
                    }*/
                  riscv.wreg3=getc(filebin);  
                  }
                }
              break;
              case 16:
                
                {
                  fseek(filebin, 0, SEEK_END);
                  riscv.wreg3 = ftell(filebin);
                  rewind(filebin);
                }
                            //printf("rcv cpu: size file\n");
              break;
              case 17:
                 fclose(filebin);
                 //printf("rcv cpu: close file\n");
              break;
                    }
                break;
           }
           pc4;
    }
       break;
    }
}
/***********************************************************/
 

int kbhit(){
struct timeval tv;
fd_set rdfs;
tv.tv_sec=0;
tv.tv_usec=0;
FD_ZERO(&rdfs);
FD_SET(STDIN_FILENO,&rdfs);
select(STDIN_FILENO+1,&rdfs,NULL,NULL,&tv);
return FD_ISSET(STDIN_FILENO,&rdfs);

}
/*
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
*/


int a;
void init_soc(){
  printf("RISCV32IM VIRTUAL CPU\n"); 
  riscv.pc=0x0000;
  counter_timer=0;
  uint32_t savetemp[XLEN];
  while(riscv.__running){

    if(riscv.next){
       scanf("%i",&a);
       if(a==10)dump_regs();
    }
    execute_instruction();
    if(riscv._enable_timer_){
        if(!(riscv.cycles % 2500)){
             if(riscv.debug_inst){
                printf("(jmp)temp: %08x , cycles: %08x\n",riscv.pc,riscv.cycles);
             } 
             riscv.temp2 = riscv.pc;
             
             riscv.pc = vector_interrupt;
             riscv.cycles=0;
             counter_timer++;
        } 
    }
    riscv.cycles++;
  }
}






 
