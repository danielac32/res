#if debug_arduino == 1
    #include <SPI.h>
    #include <SD.h>
    File boot;
    File vram;
    File OpenFile;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "cpu.h"
#if debug_arduino == 0
   FILE * filebin = NULL;
#endif

#define memory_read(addr) riscv.memory[addr]
#define memory_write(addr,val) riscv.memory[addr]=val
#define pc4 riscv.pc += 4


uint8_t read_vm(uint32_t addr){
       return memory_read(addr);
}

void write_vm(uint32_t addr,uint8_t val){
    memory_write(addr,val);
}

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
     for (int i = 0; i < 2; ++i)
     {
        write_vm(addr + i, dato[i]);
     }
}
uint16_t r16m(uint32_t addr){
     uint8_t dato[2];
     uint16_t res=0;
     for(int i = 0; i < 2; ++i){
         dato[i]=read_vm(addr+i);
     } 
     res= *(uint16_t *)dato;
     return res;
     //return (dato[0] | dato[1] << 8);
}
void w32m(uint32_t addr, uint32_t val){
     uint8_t dato[4];
     dato[0] = val & 0xff;
     dato[1] = (val >> 8) & 0xff;
     dato[2] = (val >> 16) & 0xff;
     dato[3] = (val >> 24) & 0xff;
     for (int i = 0; i < 4; ++i)
     {
        write_vm(addr + i, dato[i]);
     }
}
uint32_t r32m(uint32_t addr){
    uint8_t dato[4];
    uint32_t res=0;
    for (int i = 0; i < 4; ++i){
         dato[i]=read_vm(addr+i);
    } 
    res= *(uint32_t *)dato;
    return res;
    //return dato[0] | (dato[1] << 8) | (dato[2]<< 16) | (dato[3] << 24);
}

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
     *pval=riscv.enable_timer;
  }else if(addr==KEYPRESSED){
     #if debug_arduino == 1
        *pval = Serial.available();
     #else
        *pval = (uint8_t)kbhit(); 
     #endif
  }else if(addr==KEYREAD){
     #if debug_arduino == 1
        *pval = Serial.read();
     #else
        *pval = getchar();//fgetc(stdin);
     #endif
  }else{
     *pval =  r8m(addr);//memory_read(addr);
  }
    return 0;
}

uint8_t target_read_u16(uint16_t *pval, uint32_t addr)
{
    *pval = r16m(addr);//memory_read(addr+0) | (memory_read(addr+1) << 8);
    
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
             #if debug_arduino == 1
               *pval=millis();
             #else
               *pval=counter_timer;//riscv.timer;
             #endif
    }else{
      *pval = r32m(addr);//memory_read(addr+0) | (memory_read(addr+1) << 8) | (memory_read(addr+2)<< 16) | (memory_read(addr+3) << 24);
    }
    return 0;
}
/**************************************************/
//memory write
uint8_t target_write_u8(uint32_t addr, uint8_t val)
{
    if (addr == UART_TX_ADDR) {
        #if debug_arduino == 1
            Serial.write(val);
        #else
            printf("%c", val);  
        #endif
    }else if(addr==MODE_CPU){
             riscv.mode=val;
    }else if(addr==ENABLE_TIMER){
             riscv.enable_timer=val;
    }else{
         w8m(addr,val);//memory_write(addr,val);
    }
    return 0;
}

uint8_t target_write_u16(uint32_t addr, uint16_t val)
{
   
     //memory_write(addr ,val & 0xff);
     //memory_write(addr+1 , (val >> 8) & 0xff);
    
    w16m(addr,val);//
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
             riscv.wreg6=val;
    }else if(addr==workregister7){
             riscv.wreg7=val;
    }else{
       w32m(addr,val);//
    }

    return 0;
}

uint32_t get_op(uint32_t addr){
uint8_t dato[4];
//addr=(size_t)addr;//importante el size_t
/*dato[0] = read_vm(addr);
dato[1] = read_vm(addr+1);
dato[2] = read_vm(addr+2);
dato[3] = read_vm(addr+3);*/
for (int i = 0; i < 4; ++i)
{
    dato[i]=read_vm(addr+i);
}
return *(uint32_t *)dato;
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


void relative_jump(int32_t j,uint32_t *current){//jal , b,
if(j>0x7fffffff){         
   *current -= (uint32_t)0xffffffff-j;
}else{                      
   *current += (uint32_t)j;           
}            
}  
void jump(int32_t j,uint32_t *current){//jalr , return 
     *current = (uint32_t)j & ~1;
}


void execute_instruction()
{
    #if debug_arduino == 1
        char buff[100];
    #endif
    uint32_t opcode, rd, rs1, rs2, funct3,insn;
    int32_t imm=0, cond, err;
    uint32_t addr, val, val2,temp;
    
    
    insn= get_op(riscv.pc);

    opcode = insn & 0x7f;
    rd = (insn >> 7) & 0x1f;
    rs1 = (insn >> 15) & 0x1f;
    rs2 = (insn >> 20) & 0x1f;
    if(riscv.debug_inst){
       #if debug_arduino == 1
           sprintf(buff,"instruction: %08x , pc: %08x\n", insn,riscv.pc);
           Serial.print(buff);
       #else
           printf("instruction: %08x , pc: %08x\n", insn,riscv.pc);
       #endif
    }
    switch(opcode) {
    case 0x37: /* lui */
        if (rd != 0)
            riscv.reg[rd] = (int32_t)(insn & 0xfffff000);

        if(riscv.debug_inst){
           #if debug_arduino == 1
             sprintf(buff,"lui\n");
             sprintf(buff,"rd: %i , rd val: %x\n", rd, riscv.reg[rd]);
             Serial.print(buff);
           #else
             printf("lui\n");
             printf("rd: %i , rd val: %x\n", rd, riscv.reg[rd]);
           #endif
        }
        
        riscv.pc += 4;
        break;
    case 0x17: /* auipc */
        if (rd != 0)
            riscv.reg[rd] = (int32_t)(riscv.pc + (int32_t)(insn & 0xfffff000));

        
        riscv.pc += 4;
        break;
    case 0x6f: /* jal */
        imm = ((insn >> (31 - 20)) & (1 << 20)) |
              ((insn >> (21 - 1)) & 0x7fe) |
              ((insn >> (20 - 11)) & (1 << 11)) |
              (insn & 0xff000);
        
        
         imm = (imm << 11) >> 11;

        if (rd != 0)
            riscv.reg[rd] = riscv.pc + 4;

        if(riscv.debug_inst){
           #if debug_arduino == 1
               sprintf(buff,"jal: %x\n",riscv.pc + (uint32_t)imm);
               Serial.print(buff);
           #else
               printf("jal: %x\n",riscv.pc + (uint32_t)imm);
           #endif
        }
        //riscv.pc =  (int32_t)(riscv.pc + imm);
        //riscv.pc += imm;
        relative_jump(imm,&riscv.pc);
        
        /*if(imm>0x7fffffff)riscv.pc -= 0xffffffff-imm; 
        else riscv.pc += imm;*/
        
        if(riscv.debug_inst){
           #if debug_arduino == 1
              sprintf(buff,"jal: %x,%x\n",riscv.pc,imm);
           #else
              printf("jal: %x,%x\n",riscv.pc,imm);
           #endif
        }
        break;
    case 0x67: /* jalr */
        imm = (int32_t)insn >> 20;
        val = riscv.pc + 4;
        //riscv.pc = (int32_t)(riscv.reg[rs1] + imm) & ~1;
        
        jump(((riscv.reg[rs1] + imm)),&riscv.pc);
        
        if (rd != 0)
            riscv.reg[rd] = val;
        break;
    case 0x63:
        funct3 = (insn >> 12) & 7;
        switch(funct3 >> 1) {
        case 0: /* beq/bne */
            cond = (riscv.reg[rs1] == riscv.reg[rs2]);
              if(riscv.debug_inst){
                 #if debug_arduino == 1
                    sprintf(buff,"compare: %i , a4: %i , a5: %i\n",cond,riscv.reg[rs1],riscv.reg[rs2]);
                    Serial.print(buff);
                 #else
                    printf("compare: %i , a4: %i , a5: %i\n",cond,riscv.reg[rs1],riscv.reg[rs2]);
                 #endif
              }
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
            //riscv.pc = (int32_t)(riscv.pc + imm);
            ///riscv.pc += imm;
            
            relative_jump(imm,&riscv.pc);
        
            break;
        }else riscv.pc += 4;
        
        break;
    case 0x03: /* load */
        funct3 = (insn >> 12) & 7;
        imm = (int32_t)insn >> 20;
        addr = riscv.reg[rs1] + imm;

        switch(funct3) {
        case 0: /* lb */
        {
            uint8_t rval;
            
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
          #if debug_arduino == 1
            sprintf(buffer,"load");Serial.print(buff);
            sprintf(buffer,"addr: %x\n",addr );Serial.print(buff);
            sprintf(buffer,"rd: %i , rd val: %x\n", rd, riscv.reg[rd]);Serial.print(buff);
            sprintf(buffer,"rs1: %i, %i\n",rs1,riscv.reg[rs1]);Serial.print(buff);
            sprintf(buffer,"rs2: %i, %i\n",rs2,riscv.reg[rs2]);Serial.print(buff);
            sprintf(buffer,"imm: %i\n",imm );Serial.print(buff);
            sprintf(buffer,"val: %x\n", val);Serial.print(buff);

          #else
            printf("load");
            printf("addr: %x\n",addr );
            printf("rd: %i , rd val: %x\n", rd, riscv.reg[rd]);
            printf("rs1: %i, %i\n",rs1,riscv.reg[rs1]);
            printf("rs2: %i, %i\n",rs2,riscv.reg[rs2]);
            printf("imm: %i\n",imm );
            printf("val: %x\n", val);
          #endif
        }
        riscv.pc += 4;
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
            
            break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
       if(riscv.debug_inst){
          #if debug_arduino == 1
            sprintf(buff,"store");Serial.print(buff);
            sprintf(buff,"addr: %x\n",addr );Serial.print(buff);
            sprintf(buff,"rd: %i , rd val: %x\n", rd, riscv.reg[rd]);Serial.print(buff);
            sprintf(buff,"rs1: %i, %i\n",rs1,riscv.reg[rs1]);Serial.print(buff);
            sprintf(buff,"rs2: %i, %i\n",rs2,riscv.reg[rs2]);Serial.print(buff);
            sprintf(buff,"imm: %i\n",imm );Serial.print(buff);
            sprintf(buff,"val: %x\n", val);Serial.print(buff);
          #else
            printf("store");
            printf("addr: %x\n",addr );
            printf("rd: %i , rd val: %x\n", rd, riscv.reg[rd]);
            printf("rs1: %i, %i\n",rs1,riscv.reg[rs1]);
            printf("rs2: %i, %i\n",rs2,riscv.reg[rs2]);
            printf("imm: %i\n",imm );
            printf("val: %x\n", val);
          #endif
        }
        riscv.pc += 4;
        //printf("pc: %08x\n",riscv.pc ); 
        break;
    case 0x13:
        funct3 = (insn >> 12) & 7;
        imm = (int32_t)insn >> 20;
        switch(funct3) {
        case 0: /* addi */
            val = (int32_t)(riscv.reg[rs1] + imm) ;//& 0x0000ffff;
              if(riscv.debug_inst){
                 #if debug_arduino == 1
                    sprintf(buff,"addi/li : val: %x , imm: %x , reg: %x\n",val,imm,riscv.reg[rd]);
                    Serial.print(buff);
                 #else
                    printf("addi/li : val: %x , imm: %x , reg: %x\n",val,imm,riscv.reg[rd]);
                 #endif
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
        
        riscv.pc += 4;
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

        riscv.pc += 4;
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
           riscv.pc = riscv.temp2;
           if(riscv.debug_inst){
              #if debug_arduino == 1
                 sprintf(buff,"(ret)temp: %08x , pc: %08x , cycles: %08x\n",riscv.temp2,riscv.pc,riscv.cycles);
                 Serial.print(buff);
              #else
                 printf("(ret)temp: %08x , pc: %08x , cycles: %08x\n",riscv.temp2,riscv.pc,riscv.cycles);
              #endif
           }
        }else{
           switch(funct3) {
                  case 0:
                    switch(riscv.reg[17]){
                          case 4://string 
                             while(read_vm(riscv.reg[10])!='\0'){
                                   #if debug_arduino == 1
                                       Serial.write((char)read_vm(riscv.reg[10]));
                                   #else
                                       printf("%c",read_vm(riscv.reg[10]));
                                   #endif
                                   riscv.reg[10]++;
                             }
                          break;
                          case 5://readint
                               //printf("readint:\n");
                            //scanf("%x",&riscv.reg[10]);
                            //printf("%x\n",riscv.reg[10]);
                          break;
                          case 8:///readstring
                          
                          break;
                          case 10://exit
                            #if debug_arduino == 1
                                Serial.println(F("EXIT RISCV32"));
                            #else
                                printf("EXIT RISCV32\n");
                            #endif
                            riscv.running=0;
                          break;
                          case 11://print char
                             #if debug_arduino == 1
                               Serial.write((char)riscv.reg[10]);
                             #else
                               printf("%c",(char)riscv.reg[10]);
                             #endif
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
                              while(read_vm(riscv.reg[10])!='\0'){
                                   riscv.name[i]=read_vm(riscv.reg[10]);
                                   riscv.reg[10]++;
                                   i++;
                              }
                              riscv.name[i]='\0';
                              i=strlen(riscv.name);
                              //name[i-1]='\0';
                              //printf("file: %s\n",riscv.name);
                              #if debug_arduino == 1
                                  OpenFile = SD.open(riscv.namef);
                                  OpenFile.seek(0);
                                  if(OpenFile){
                                     riscv.wreg2=0;
                                  }else{
                                     riscv.wreg2=1;
                                  }
                              #else
                                  filebin = fopen(riscv.name, "rb");
                                  if(filebin == NULL){
                                     riscv.wreg2=1;
                                  }else{
                                     riscv.wreg2=0;
                                  }
                              #endif
                            }
                          break;
                          case 15:
                          {
                            #if debug_arduino == 1
                               if(OpenFile){
                                  riscv.wreg3=(uint8_t)OpenFile.read();  
                               }
                            #else
                               if(filebin!=NULL){
                                  riscv.wreg3=getc(filebin);  
                               }
                            #endif
                          }
                          break;
                          case 16:
                          {
                            #if debug_arduino == 1
                                if(OpenFile){
                                   riscv.wreg3=OpenFile.size();
                                }
                            #else
                                if(filebin!=NULL){
                                   fseek(filebin, 0, SEEK_END);
                                   riscv.wreg3 = ftell(filebin);
                                   rewind(filebin);
                                }
                            #endif
                          }
                          break;
                          case 17:
                               #if debug_arduino == 1
                                   if(OpenFile){
                                      OpenFile.close(); 
                                   }
                               #else
                                   if(filebin!=NULL){
                                      fclose(filebin);
                                   }
                               #endif
                         break;
                          case 24://read buffer
                               {
                                 uint32_t addr;
                                 uint32_t sizef;
                                 #if debug_arduino == 1
                                     if(OpenFile){
                                        addr=riscv.reg[10];//direccion 0 de la variable 
                                        sizef=riscv.reg[11];
                                        OpenFile.seek(0);
                                        for(uint32_t i=addr;i<sizef+addr;i++){
                                           write_vm(i,(uint8_t)OpenFile.read());
                                        }
                                     }
                                 #else
                                    if(filebin){
                                       addr=riscv.reg[10];//direccion 0 de la variable 
                                       sizef=riscv.reg[11];//tamaño del archivo

                                       fseek(filebin, 0, SEEK_SET);
                                       for(uint32_t i=addr;i<sizef+addr;i++){
                                           write_vm(i,(uint8_t)getc(filebin));
                                       }
                                    }
                                 #endif
                               }
                          break;
                    }
                break;
           }
           riscv.pc += 4;
        }
       break;
    }
    /******************************************/
    if(riscv.enable_timer){
        if(!(riscv.cycles % TIME_INTERRUPT)){
             if(riscv.debug_inst){
                #if debug_arduino == 1
                    sprintf(buff,"(jmp)temp: %08x , cycles: %08x\n",riscv.pc,riscv.cycles);
                    Serial.print(buff);
                #else
                    printf("(jmp)temp: %08x , cycles: %08x\n",riscv.pc,riscv.cycles);
                #endif
             } 
             riscv.temp2 = riscv.pc;
             
             riscv.pc = vector_interrupt;
             riscv.cycles=0;
             counter_timer++;
        } 
    }
    riscv.cycles++;
    /******************************************/
    if(riscv.debug_inst){
       #if debug_arduino == 1
           sprintf(buff,"(cycle: %08i, op: %08x , pc: %08x\n",riscv.cycles,insn,riscv.pc);
           Serial.print(buff);
       #else
           printf("(cycle: %08i, op: %08x , pc: %08x\n",riscv.cycles,insn,riscv.pc);
       #endif
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


int a;
void init_soc(){
  #if debug_arduino == 1
      Serial.begin(115200);
      pinMode(46, OUTPUT);
      pinMode(13,OUTPUT);
      //led
      pinMode(22,OUTPUT);
      pinMode(24,OUTPUT);
      pinMode(26,OUTPUT);
      pinMode(28,OUTPUT);
      digitalWrite(22,false);//amarillo
      digitalWrite(24,false);//verde

      while(!Serial);
      Serial.println(F("**************************************************"));
      Serial.print(F("EMULATOR RISCV32IMC CPU\n"));
      Serial.print(F("RUNNING INTO ARDUINO MEGA2560 16MHZ \n"));
      if (!SD.begin(46)) {
           Serial.println(F("initialization failed!"));
           return;
      }
      SPI.setClockDivider(SPI_CLOCK_DIV4);

  #else
      printf("RISCV32IM VIRTUAL CPU\n"); 
  #endif
  init_variable();
  riscv.next=0;
  riscv.debug_inst=0;

  while(riscv.running){

    if(riscv.next){
       #if debug_arduino == 1
           while(1){
              if(Serial.available()){
                 if(Serial.read()=='a')break; 
              }
           }
       #else
          scanf("%i",&a);
       #endif
    }
    execute_instruction();
  }
}

