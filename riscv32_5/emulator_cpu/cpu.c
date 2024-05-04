
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cpu.h"
#include "memory.h"
#include "syscall.h"
#include "eeprom.h"

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

void raise_exception(uint32_t cause, uint32_t instruction){

}


#define save_context            \
for (int i = 0; i < 32; ++i){   \
     temp_reg[i]=riscv.reg[i];  \
}
#define restore_context         \
for (int i = 0; i < 32; ++i){   \
     riscv.reg[i]=temp_reg[i];  \
}

#define save_context2            \
for (int i = 0; i < 32; ++i){   \
     temp_reg2[i]=riscv.reg[i];  \
}
#define restore_context2         \
for (int i = 0; i < 32; ++i){   \
     riscv.reg[i]=temp_reg2[i];  \
}



void event_cycles(){
 if(!(riscv.cycles % TIME_INTERRUPT)){
      riscv.cycles=1;
      riscv.flag=1;
      call_isr();
 } 
 if(!riscv.flag)riscv.cycles++;  //
}

void call_isr(){
     riscv.temp2 = riscv.pc;
     save_context;
     jump(vector_interrupt,&riscv.pc);

     if(riscv.debug_inst){
        printf("(jmp)temp: %08x , cycles: %08x\n",riscv.pc,riscv.cycles);
     } 

    /*

      if(!(riscv.cycles % TIME_INTERRUPT)){
          
          riscv.temp2 = riscv.pc;
          jump(vector_interrupt,&riscv.pc);

          if(riscv.debug_inst){
                printf("(jmp)temp: %08x , cycles: %08x\n",riscv.pc,riscv.cycles);
          } 
          //riscv.cycles=0;
          counter_timer++;
      }
      riscv.cycles++;*/
}
void ret_isr(){
   restore_context;
   riscv.pc = riscv.temp2;
   if(riscv.debug_inst){
      printf("(ret)temp: %08x , pc: %08x , cycles: %08x\n",riscv.temp2,riscv.pc,riscv.cycles);
   }
}

void call_trap(){
     riscv.temp3 = riscv.pc;
     //save_context;
     jump(SYSCALL_INTERRUPT_POINTER,&riscv.pc);
}
void ret_trap(){
  // restore_context;
   riscv.pc = riscv.temp3;
}

void execute_instruction()
{
     
    uint32_t opcode, rd, rs1, rs2, funct3,insn;
    int32_t imm=0, cond, err;
    uint32_t addr, val, val2,temp;
    
    insn= get_op(riscv.pc);

    opcode = insn & 0x7f;
    rd = (insn >> 7) & 0x1f;
    rs1 = (insn >> 15) & 0x1f;
    rs2 = (insn >> 20) & 0x1f;
    if(riscv.debug_inst){
       printf("instruction: %08x , pc: %08x\n", insn,riscv.pc);
    }
    switch(opcode) {
    case 0x37: /* lui */
        if (rd != 0) riscv.reg[rd] = (int32_t)(insn & 0xfffff000);

        if(riscv.debug_inst){
           printf("lui\n");
           printf("rd: %i , val: %x\n", rd, riscv.reg[rd]);
        }
        
        riscv.pc += 4;
        break;
    case 0x17: /* auipc */
        if (rd != 0) riscv.reg[rd] = (int32_t)(riscv.pc + (int32_t)(insn & 0xfffff000));

        
        riscv.pc += 4;
        break;
    case 0x6f: /* jal */
        imm = ((insn >> (31 - 20)) & (1 << 20)) |
              ((insn >> (21 - 1)) & 0x7fe) |
              ((insn >> (20 - 11)) & (1 << 11)) |
              (insn & 0xff000);
        
        
         imm = (imm << 11) >> 11;

        if (rd != 0) riscv.reg[rd] = riscv.pc + 4;

        if(riscv.debug_inst){
           printf("jal: %x\n",riscv.pc + (uint32_t)imm);
        }
       
        relative_jump(imm,&riscv.pc);

        break;
    case 0x67: /* jalr */
        imm = (int32_t)insn >> 20;
        val = riscv.pc + 4;
        
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
                 printf("compare: %i , a4: %i , a5: %i\n",cond,riscv.reg[rs1],riscv.reg[rs2]);
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
            printf("load ");
            printf("addr: %x\n",addr );
            printf("rd: %i , rd val: %x\n", rd, riscv.reg[rd]);
            printf("rs1: %i, %i\n",rs1,riscv.reg[rs1]);
            printf("rs2: %i, %i\n",rs2,riscv.reg[rs2]);
            printf("imm: %i\n",imm );
            printf("val: %x\n", val);
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
            printf("store ");
            printf("addr: %x\n",addr );
            printf("rd: %i , rd val: %x\n", rd, riscv.reg[rd]);
            printf("rs1: %i, %i\n",rs1,riscv.reg[rs1]);
            printf("rs2: %i, %i\n",rs2,riscv.reg[rs2]);
            printf("imm: %i\n",imm );
            printf("val: %x\n", val);
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
                 printf("addi/li : val: %x , imm: %x , reg: %x\n",val,imm,rs1);
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
                val = val*val2;//(int32_t)((int32_t)val * (int32_t)val2);
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
        
        /*
         43c:   10200073            sret
         440:   00200073            uret
         444:   30200073            mret
                00000073            ecall
        */
         if(imm==0x102){
            riscv.pc = riscv.temp3;
            //sret
             //ret_trap();
         }else if (imm==0x002){
            //uret
          ret_isr();
            /*if(riscv.flag){
               ret_isr();
               //printf("return %i\n",riscv.reg[17] );
               riscv.flag=0;
            }*/
         }else if(imm==0x302){
            //mret

         }else if (imm==0x0000){
            //ecall
            uint32_t reg_a7=riscv.reg[17];
            syscall_handler(reg_a7);
            riscv.pc += 4;
         }
         
        /*if(imm==2){//uret
           if(riscv.flag){
              ret_isr();
              //printf("return %i\n",riscv.reg[17] );
              riscv.flag=0;
           }
        }else{
           switch(funct3) {
                  case 0:
                      {
                        uint32_t reg_a7=riscv.reg[17];

                      
                        syscall_handler(reg_a7);
                      }
                  break;
                  break;
           }
           riscv.pc += 4;
        }*/
       break;
    }
    /******************************************/
   /* if(riscv.enable_timer){
       riscv.enable_timer=0;
       call_isr();
    }*/
    if(riscv.enable_timer)event_cycles();
    //riscv.cycles++;
    /******************************************/
     
}

int a;
void init_soc(){
  
  printf("RISCV32IM VIRTUAL CPU (max memory: %d),(space: %d)\n",sizeof(riscv.memory),space_reserved); 
 
  init_variable();
  /*eeprom_init();
  
  printf("loading....\n");
  
  for (uint32_t i = 0; i < space_reserved; ++i)
  {
       uint8_t c=riscv.memory[i];
       eeprom_write(i,&c,1);
  }*/
  printf("running....\n");

  if (__debug__==1)
  {
     printf("enable debug\n");
     riscv.next=1;
     riscv.debug_inst=1;
  }else{
     riscv.next=0;
     riscv.debug_inst=0;
  }

  //printf("%i\n",riscv.debug_inst );
  riscv.running=1;
  while(riscv.running){

    if(riscv.next){
       scanf("%i",&a);
    }
    execute_instruction();
    
  }
  fclose(eeprom_file);
}


