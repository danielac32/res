#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "cpu.h"
#include "memory.h"
#include "eeprom.h"

 
#define BLOCK 256
 


uint8_t memory_read(uint32_t addr){
 
     /*uint8_t c;
	 eeprom_read(addr,&c,1);
     return c;*/
     return riscv.memory[addr];
}
void memory_write(uint32_t addr,uint8_t val){

      
    //eeprom_write(addr,&val,1);
   riscv.memory[addr]=val;
}




uint8_t read_vm(uint32_t addr){
       if(addr>space_reserved){
          printf("\n* error readed:  %08x\n" \
                 "pc: %08x\n"           \
                 "current op: %08x\n"   \
                 "after op: %08x *\n\n",     \
                 addr,riscv.pc,get_op(riscv.pc),get_op(riscv.pc-4));
          exit(-1);
       }
       return  memory_read(addr);
}

void write_vm(uint32_t addr,uint8_t val){
    if(addr>space_reserved){
          printf("\n* error writed:  %08x\n" \
                 "pc: %08x\n"           \
                 "current op: %08x\n"   \
                 "after op: %08x *\n\n",     \
                 addr,riscv.pc,get_op(riscv.pc),get_op(riscv.pc-4));
          exit(-1);
    }
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
}


uint8_t target_read_u8(uint8_t *pval, uint32_t addr)
{
  
  if(addr==FLAG_T0){
      *pval=riscv.flag;
  }else
  if(addr==MODE_CPU){
     *pval=riscv.mode;
  //}else if(addr==FLAGT0){
  //   *pval=riscv.flag;
  }else if(addr==ENABLE_TIMER){
     *pval=riscv.enable_timer;
      riscv.cycles=1;
      riscv.flag=1;
  }else if(addr==KEYPRESSED){
     *pval = (uint8_t)kbhit(); 
  }else if(addr==KEYREAD){
     *pval = getchar();
  }else{
     *pval =  r8m(addr);
  }
    return 0;
}

uint8_t target_read_u16(uint16_t *pval, uint32_t addr)
{
    *pval = r16m(addr);
    
    return 0;
}

uint8_t target_read_u32(uint32_t *pval, uint32_t addr)
{
    /*if(addr==workregister1){
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
             *pval=counter_timer;
    }else{*/
      *pval = r32m(addr);
    //}
    return 0;
}
/**************************************************/
//memory write
uint8_t target_write_u8(uint32_t addr, uint8_t val)
{
    if (addr == UART_TX_ADDR) {
        printf("%c", val);  
    }else if(addr==MODE_CPU){
        riscv.mode=val;
    }else if(addr==ENABLE_TIMER){
        riscv.enable_timer=val;
    //}else if(addr==FLAGT0){
    //    riscv.flag=val;
    }else{
        w8m(addr,val);//memory_write(addr,val);
    }
    return 0;
}

uint8_t target_write_u16(uint32_t addr, uint16_t val)
{
    w16m(addr,val);//
    return 0;
}

uint8_t target_write_u32(uint32_t addr, uint32_t val)
{    
    
    /*if(addr==workregister1){
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
    }else{*/
       w32m(addr,val);//
   // }

    return 0;
}

uint32_t get_op(uint32_t addr){
uint8_t dato[4];
for (int i = 0; i < 4; ++i)
{
    dato[i]=read_vm(addr+i);
}
return *(uint32_t *)dato;
}

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

