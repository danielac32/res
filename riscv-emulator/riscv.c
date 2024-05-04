#include <stdint.h>
#include <stdio.h>
 
#include <sys/ioctl.h>
#include <termios.h>

#include "cpu.h"
#include "riscv.h"
 
#define RA 1
#define SP 2
#define INTERRUPT 0x28 & ~1 

typedef enum { MODE_THREAD = 0, MODE_HANDLER } tMode;
#define EXC_RETURN          0xFFFFFFE0

int m_current_mode;
 
void rv32(uint32_t baseAddr)
{
     


    m_enable_mem_errors  = true;
    m_compliant_csr      = false;
    m_enable_rvm         = true;
    m_enable_rvc         = false;
    m_enable_rva         = false;
    m_enable_mtimecmp    = false;

    // Some memory defined
    _baseAddr=baseAddr;
    reset(_baseAddr);
    m_current_mode = MODE_THREAD;
    //printf("program counter: %08x\n", _baseAddr);
    step();
}


uint32_t irq_return(uint32_t pc){
  // EXC_RETURN value in PC
   // printf("\nirq_return\n");
    uint32_t ret;
    uint32_t sp = m_gpr[SP];
    m_current_mode = MODE_THREAD;

    switch (pc & 0xF){
            case 0xc:
            case 0xd:
                
            break;
            case 0x9:
            case 0x8:
    
               m_gpr[11] = read32(sp);
               sp += 4;
               m_gpr[10] = read32(sp);
               sp += 4;
               m_gpr[27] = read32(sp);
               sp += 4;
               m_gpr[26] = read32(sp);
               sp += 4;
               m_gpr[25] = read32(sp);
               sp += 4;
               m_gpr[24] = read32(sp);
               sp += 4;
               m_gpr[23] = read32(sp);
               sp += 4;
               m_gpr[22] = read32(sp);
               sp += 4;
               m_gpr[21] = read32(sp);
               sp += 4;
               m_gpr[20] = read32(sp);
               sp += 4;
               m_gpr[19] = read32(sp);
               sp += 4;
               m_gpr[18] = read32(sp);
               sp += 4;
               m_gpr[9] = read32(sp);
               sp += 4;
               m_gpr[8] = read32(sp);
               //sp += 4;
               //m_gpr[SP] = read32(sp);
               sp += 4;
               ret = read32(sp);
               sp += 4;
               m_gpr[SP] = sp;
             
               printf("%08x,%08x\n", m_gpr[11],ret);


               /*m_gpr[3] = read32(sp);
               sp += 4;
               m_gpr[4] = read32(sp);
               sp += 4;
               m_gpr[5] = read32(sp);
               sp += 4;
               m_gpr[6] = read32(sp);
               sp += 4;
               m_gpr[7] = read32(sp);
               sp += 4;
               m_gpr[8] = read32(sp);
               sp += 4;
               m_gpr[9] = read32(sp);
               sp += 4;
               m_gpr[10] = read32(sp);
               sp += 4;
               m_gpr[11] = read32(sp);
               sp += 4;
               m_gpr[12] = read32(sp);
               sp += 4;
               m_gpr[13] = read32(sp);
               sp += 4;
               m_gpr[14] = read32(sp);
               sp += 4;
               m_gpr[15] = read32(sp);
               sp += 4;
               m_gpr[16] = read32(sp);
               sp += 4;
               m_gpr[17] = read32(sp);
               sp += 4;
               m_gpr[18] = read32(sp);
               sp += 4;
               m_gpr[19] = read32(sp);
               sp += 4;               
               m_gpr[20] = read32(sp);
               sp += 4;
               m_gpr[21] = read32(sp);
               sp += 4;
               m_gpr[22] = read32(sp);
               sp += 4;
               m_gpr[23] = read32(sp);
               sp += 4;
               m_gpr[24] = read32(sp);
               sp += 4;
               m_gpr[25] = read32(sp);
               sp += 4;
               m_gpr[26] = read32(sp);
               sp += 4;
               m_gpr[27] = read32(sp);
               sp += 4;
               m_gpr[28] = read32(sp);
               sp += 4;               
               m_gpr[29] = read32(sp);
               sp += 4;
               m_gpr[30] = read32(sp);
               sp += 4;
               m_gpr[31] = read32(sp);
               sp += 4;
               m_gpr[RA] = read32(sp);//printf("%x,%x\n", sp,m_gpr[RA]);
               sp += 4;
               ret = read32(sp);//printf("%x,%x\n", sp,pc);
               sp += 4;
               m_gpr[SP] = read32(sp);//printf("%x,%x\n", sp,m_gpr[SP]);
               sp += 4;
               m_gpr[SP] = sp;
               */
            break;
            default:
              printf("error %x\n",pc );
              while(1);

    }
    return ret;
}

uint32_t irq_jmp(uint32_t pc){
  //printf("\nirq_jmp\n");

  uint32_t sp = m_gpr[SP];
  uint32_t temp = sp;
  sp -= 4;
  write32(sp,pc);
  //sp -= 4;
  //write32(sp,temp);
  sp -= 4;
  write32(sp,m_gpr[8]);
  sp -= 4;
  write32(sp,m_gpr[9]);
  sp -= 4;
  write32(sp,m_gpr[18]);
  sp -= 4;
  write32(sp,m_gpr[19]);
  sp -= 4;
  write32(sp,m_gpr[20]);
  sp -= 4;
  write32(sp,m_gpr[21]);
  sp -= 4;
  write32(sp,m_gpr[22]);
  sp -= 4;
  write32(sp,m_gpr[23]);
  sp -= 4;
  write32(sp,m_gpr[24]);
  sp -= 4;
  write32(sp,m_gpr[25]);
  sp -= 4;
  write32(sp,m_gpr[26]);
  sp -= 4;
  write32(sp,m_gpr[27]);
  sp -= 4;
  write32(sp,m_gpr[10]);
  sp -= 4;
  write32(sp,m_gpr[11]);
  m_gpr[SP] = sp;
  

  /*uint32_t sp = m_gpr[SP];
  
  uint32_t temp = sp;
  sp -= 4;
  write32(sp,temp);//printf("%x,%x\n", sp,temp);
  sp -= 4;
  write32(sp,pc);//printf("%x,%x\n", sp,pc);
  sp -= 4;
  write32(sp,m_gpr[RA]);//printf("%x,%x\n",sp,m_gpr[RA]);
  sp -= 4;
  write32(sp , m_gpr[31]);
  sp -= 4;
  write32(sp , m_gpr[30]);
  sp -= 4;
  write32(sp , m_gpr[29]);
  sp -= 4;
  write32(sp , m_gpr[28]);
          sp -= 4;
          write32(sp , m_gpr[27]);
          sp -= 4;
          write32(sp , m_gpr[26]);
          sp -= 4;
          write32(sp , m_gpr[25]);
          sp -= 4;
          write32(sp , m_gpr[24]);
          sp -= 4;
          write32(sp , m_gpr[23]);
          sp -= 4;
          write32(sp , m_gpr[22]);
          sp -= 4;
          write32(sp , m_gpr[21]);
          sp -= 4;
          write32(sp , m_gpr[20]);
          sp -= 4;
          write32(sp , m_gpr[19]);
          sp -= 4;
          write32(sp , m_gpr[18]);
          sp -= 4; 
          write32(sp , m_gpr[17]);
          sp -= 4;
          write32(sp , m_gpr[16]);
          sp -= 4;
          write32(sp , m_gpr[15]);
          sp -= 4;
          write32(sp , m_gpr[14]);
          sp -= 4;
          write32(sp , m_gpr[13]);
          sp -= 4;
          write32(sp , m_gpr[12]);
          sp -= 4;
          write32(sp , m_gpr[11]);
          sp -= 4;
          write32(sp , m_gpr[10]);
          sp -= 4;
          write32(sp , m_gpr[9]);
          sp -= 4;
          write32(sp , m_gpr[8]);
          sp -= 4;
          write32(sp , m_gpr[7]);
          sp -= 4;
          write32(sp , m_gpr[6]);
          sp -= 4;
          write32(sp , m_gpr[5]);
          sp -= 4;
  write32(sp , m_gpr[4]);
  sp -= 4;
  write32(sp , m_gpr[3]);
  m_gpr[SP] = sp;*/

  if(m_current_mode == MODE_HANDLER)m_gpr[RA] = 0xFFFFFFF1;   
  else{
    m_gpr[RA] = 0xFFFFFFF9;
  }
  m_current_mode = MODE_HANDLER;
  return INTERRUPT;
}


void step(){
printf("riscv32 emulator running\n");
 
m_csr_mip |= SR_IP_MEIP;



while (1){
   // Increment timer counter
    m_csr_mtime++;
    // Timer should generate a interrupt?
    // Limited internal timer, truncate to 32-bits
    m_csr_mtime &= 0xFFFFFFFF;
 
     if((m_pc & EXC_RETURN) == EXC_RETURN){
        m_pc = irq_return(m_pc);
     }

 execute();
     bool systick_irq = false;
     if (!(m_csr_mtime % value_timer))systick_irq = true;
       
     if (systick_irq && (m_current_mode == MODE_THREAD) && enable_timer){
           m_pc = irq_jmp(m_pc);
           systick_irq = false;
     }
    

        
}
	 
/*int c;
bool s=execute();
	while(s){
		s=execute();
        //scanf("%i",&c);
	}*/
}
 

void reset(uint32_t start_addr)
{
    m_pc        = start_addr;
    m_pc_x      = start_addr;
    m_load_res  = 0;

    for (int i=0;i<REGISTERS;i++)
        m_gpr[i] = 0;

    m_csr_mpriv    = PRIV_MACHINE;
    m_csr_msr      = 0;
    m_csr_mideleg  = 0;
    m_csr_medeleg  = 0;

    m_csr_mepc     = 0;
    m_csr_mie      = 0;
    m_csr_mip      = 0;
    m_csr_mcause   = 0;
    m_csr_mevec    = 0;
    m_csr_mtval    = 0;
    m_csr_mtime    = 0;
    m_csr_mtimecmp = 0;
    m_csr_mtime_ie = false;
    m_csr_mscratch = 0;

    m_csr_sepc     = 0;
    m_csr_sevec    = 0;
    m_csr_scause   = 0;
    m_csr_stval    = 0;
    m_csr_satp     = 0;
    m_csr_sscratch = 0;

    m_fault       = false;
    m_break       = false;
    m_trace       = 0;
    enable_timer=0;
    value_timer=0xFFFFFFFF;
}


uint32_t get_opcode(uint32_t address)
{
    // 2 byte aligned addresses are supported if RVC
    if (m_enable_rvc && (address & 2))
    {
        uint16_t opcode = ifetch16(address);
        return (ifetch16((address+2)) << 16) | opcode;
    }
    else
        return ifetch32(address);
}



int load(uint32_t pc, uint32_t address, uint32_t *result, int width, bool signedLoad)
{
    uint32_t physical = address;
    if (((address & 3) != 0 && width == 4) || ((address & 1) != 0 && width == 2))
    {
        exception(MCAUSE_MISALIGNED_LOAD, pc, address);
        return 0;
    }
    *result = 0;

     //get 0x4000300
    //ena 0x4000302

    if(address == 0x4000200){//enable timer
        *result = enable_timer;
        return 1;
    }else if(address == 0x4000204){// valor timer
        *result = value_timer;
        return 1;
    }else if(address == 0x4000300){// valor get
        //*result =  kch;//fgetc(stdin);//value_timer;
        return 1;
    }else if(address == 0x4000302){// valor ena
        //*result =_kbhit();
        return 1;
    }else if(address == 0x4000500){
        *result = m_current_mode;
        return 1;
    }

    switch (width)
            {
                case 4:
                    *result=read32(physical);
                    break;
                case 2:
                {
                    uint16_t dh = 0;
                    dh=read16(physical);
                    *result |= dh;

                    if (signedLoad && ((*result) & (1 << 15)))
                         *result |= 0xFFFF0000;
                }
                break;
                case 1:
                {
                    uint8_t db = 0;
                    db=read8(physical);
                    *result |= ((uint32_t)db << 0);

                    if (signedLoad && ((*result) & (1 << 7)))
                         *result |= 0xFFFFFF00;
                }
                break;
                default:
                    printf("error load\n");
                    exception(MCAUSE_FAULT_LOAD, pc, address);
                    return 0;
                    break;
            }
            return 1;
}

int store(uint32_t pc, uint32_t address, uint32_t data, int width)
{
    uint32_t physical = address;
      // Detect misaligned load
    if (((address & 3) != 0 && width == 4) || ((address & 1) != 0 && width == 2))
    {
        exception(MCAUSE_MISALIGNED_STORE, pc, address);
        return 0;
    }

    if (address==0x4000100 || address==0x4000301){
        printf("%c",(uint8_t)data & 0xff);
        return 1;
    }else if(address == 0x4000200){//enable timer
        enable_timer=(uint8_t)data;
        return 1;
    }else if(address == 0x4000204){// valor timer
        value_timer=data;
        return 1;
    }else if(address == 0x4000300 || address==0x4000302){
        return 1; 
    }
    //get 0x4000300
    //ena 0x4000302
    switch (width)
            {
                case 4:
                          write32(physical, data);
                    break;
                case 2:
                    write16(physical, data & 0xFFFF);
                    break;
                case 1:
                    write8(physical, data & 0xFF);
                    break;
                default:
                      printf("error store\n");
                      exception(MCAUSE_FAULT_STORE, pc, address);
                    return 0;
                    break;
            }
    return 1;
}


uint32_t access_csr2(uint32_t address, uint32_t data, bool set, bool clr)
{
    uint32_t result = 0;

#define CSR_STD(name, var_name) \
    case CSR_ ##name: \
    { \
        data       &= CSR_ ##name## _MASK; \
        result      = var_name & CSR_ ##name## _MASK; \
        if (set && clr) \
            var_name  = data; \
        else if (set) \
            var_name |= data; \
        else if (clr) \
            var_name &= ~data; \
    } \
    break;

#define CSR_CONST(name, value) \
    case CSR_ ##name: \
    { \
        result      = value; \
    } \
    break;


    switch (address & 0xFFF)
    {
        //--------------------------------------------------------
        // Simulation control
        //--------------------------------------------------------
        case CSR_DSCRATCH:
        case CSR_SIM_CTRL:
            switch (data & 0xFF000000)
            {
                case CSR_SIM_CTRL_EXIT:
                    
                    break;
                case CSR_SIM_CTRL_PUTC:
                   
                    break;
                case CSR_SIM_CTRL_GETC:
                    
                    break;
                case CSR_SIM_CTRL_TRACE:
                    
                    break;
                case CSR_SIM_PRINTF:
                
                break;
            }
         break;
        //--------------------------------------------------------
        // Standard - Machine
        //--------------------------------------------------------
        CSR_STD(MEPC,    m_csr_mepc)
        CSR_STD(MTVEC,   m_csr_mevec)
        CSR_STD(MCAUSE,  m_csr_mcause)
        CSR_STD(MSTATUS, m_csr_msr)
        CSR_STD(MIP,     m_csr_mip)
        CSR_STD(MIE,     m_csr_mie)
        CSR_CONST(MISA,  MISA_VALUE)
        CSR_STD(MIDELEG, m_csr_mideleg)
        CSR_STD(MEDELEG, m_csr_medeleg)
        CSR_STD(MSCRATCH,m_csr_mscratch)
        CSR_CONST(MHARTID,  MHARTID_VALUE)
        //--------------------------------------------------------
        // Standard - Supervisor
        //--------------------------------------------------------
        CSR_STD(SEPC,    m_csr_sepc)
        CSR_STD(STVEC,   m_csr_sevec)
        CSR_STD(SCAUSE,  m_csr_scause)
        CSR_STD(SIP,     m_csr_mip)
        CSR_STD(SIE,     m_csr_mie)
        CSR_STD(SATP,    m_csr_satp)
        CSR_STD(STVAL,   m_csr_stval)
        CSR_STD(SSCRATCH,m_csr_sscratch)
        CSR_STD(SSTATUS, m_csr_msr)
        //--------------------------------------------------------
        // Extensions
        //-------------------------------------------------------- 
        CSR_CONST(PMPCFG0, 0)
        CSR_CONST(PMPCFG1, 0)
        CSR_CONST(PMPCFG2, 0)
        CSR_CONST(PMPADDR0, 0)

         case CSR_MCYCLE:
            result      = m_csr_mtime;
            break;
        case CSR_MTIME:
            data       &= CSR_MTIME_MASK;
            result      = m_csr_mtime;

            // Non-std behaviour - write to CSR_TIME gives next interrupt threshold            
            if (set && data != 0)
            {
                m_csr_mtimecmp = data;

                // Clear interrupt pending
                m_csr_mip &= ~((m_csr_mideleg & SR_IP_STIP) ? SR_IP_STIP : SR_IP_MTIP);
            }
            break;
    
        case CSR_MTIMEH:
            result      = m_csr_mtime >> 32;
            break;
         // Non-std behaviour
        case CSR_MTIMECMP:
            data       &= CSR_MTIMECMP_MASK;
            result      = m_csr_mtimecmp & CSR_MTIMECMP_MASK;
            if (set && clr)
                m_csr_mtimecmp  = data;
            else if (set)
                m_csr_mtimecmp |= data;
            else if (clr)
                m_csr_mtimecmp &= ~data;

            // Enable mtimecmp interrupt
            if (set || clr)
                m_csr_mtime_ie = true;

            m_enable_mtimecmp = true;
            break;
        default:
            printf("*** CSR address not supported %08x [PC=%08x]\n", address, m_pc);
            break;
    }
    return result;
}

 


void exception(uint32_t cause, uint32_t pc, uint32_t badaddr)
{
    uint32_t deleg;
    uint32_t bit;
 
    // Interrupt
    if (cause >= MCAUSE_INTERRUPT)
    {
        deleg = m_csr_mideleg;
        bit   = 1 << (cause - MCAUSE_INTERRUPT);
    }
    // Exception
    else
    {
        deleg = m_csr_medeleg;
        bit   = 1 << cause;
    }

    // Exception delegated to supervisor mode
    if (m_csr_mpriv <= PRIV_SUPER && (deleg & bit))
    {
        uint32_t s = m_csr_msr;

        // Interrupt save and disable
        s &= ~SR_SPIE;
        s |= (s & SR_SIE) ? SR_SPIE : 0;
        s &= ~SR_SIE;

        // Record previous priviledge level
        s &= ~SR_SPP;
        s |= (m_csr_mpriv == PRIV_SUPER) ? SR_SPP : 0;

        // Raise priviledge to supervisor level
        m_csr_mpriv  = PRIV_SUPER;

        m_csr_msr    = s;
        m_csr_sepc   = pc;
        m_csr_scause = cause;
        m_csr_stval  = badaddr;
      

        // Set new PC
        m_pc = m_csr_sevec;
    }
    // Machine mode
    else
    {
        uint32_t s = m_csr_msr;

        // Interrupt save and disable
        s &= ~SR_MPIE;
        s |= (s & SR_MIE) ? SR_MPIE : 0;
        s &= ~SR_MIE;

        // Record previous priviledge level
        s &= ~SR_MPP;
        s |= (m_csr_mpriv << SR_MPP_SHIFT);

        // Raise priviledge to machine level
        m_csr_mpriv  = PRIV_MACHINE;

        m_csr_msr    = s;
        m_csr_mepc   = pc;
        m_csr_mcause = cause;
        m_csr_mtval  = badaddr;
        // Set new PC
        m_pc = m_csr_mevec;
    }
}


bool execute(void)
{

   
    uint32_t phy_pc = m_pc;

     // Misaligned PC
    if ((!m_enable_rvc && (m_pc & 3)) || (m_enable_rvc && (m_pc & 1)))
    {
        exception(MCAUSE_MISALIGNED_FETCH, m_pc, m_pc);
        return false;
    }
    
    
    // Get opcode at current PC
    uint32_t opcode = get_opcode(phy_pc);

    // Extract registers
    int rd          = (opcode & OPCODE_RD_MASK)  >> OPCODE_RD_SHIFT;
    int rs1         = (opcode & OPCODE_RS1_MASK) >> OPCODE_RS1_SHIFT;
    int rs2         = (opcode & OPCODE_RS2_MASK) >> OPCODE_RS2_SHIFT;

    // Extract immediates
    int typei_imm   = ((signed)(opcode & OPCODE_TYPEI_IMM_MASK)) >> OPCODE_TYPEI_IMM_SHIFT;
    int typeu_imm   = ((signed)(opcode & OPCODE_TYPEU_IMM_MASK)) >> OPCODE_TYPEU_IMM_SHIFT;
    int imm20       = typeu_imm << OPCODE_TYPEU_IMM_SHIFT;
    int imm12       = typei_imm;
    int bimm        = OPCODE_SBTYPE_IMM(opcode);
    int jimm20      = OPCODE_UJTYPE_IMM(opcode);
    int storeimm    = OPCODE_STYPE_IMM(opcode);
    int shamt       = ((signed)(opcode & OPCODE_SHAMT_MASK)) >> OPCODE_SHAMT_SHIFT;

    // Retrieve registers
    uint32_t reg_rd  = 0;
    uint32_t reg_rs1 = m_gpr[rs1];
    uint32_t reg_rs2 = m_gpr[rs2];
    uint32_t pc      = m_pc;

    bool take_exception = false;


    //printf( "%08x , %08x , sp: %08x ,%08x,%08x\n", pc, opcode,m_gpr[2],m_csr_mtval,m_csr_stval);

   if (opcode == 0)
    {
        printf("Bad instruction @ %x ,%x\n", phy_pc,m_gpr[1]);

        exception(MCAUSE_ILLEGAL_INSTRUCTION, pc, opcode);
        m_fault = true;
        take_exception = true;  
        printf("%x\n",m_pc );   
        while(1);   
    }
    else if ((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        //printf("%08x: andi r%d, r%d, %d\n", pc, rd, rs1, imm12);
         
        reg_rd = reg_rs1 & imm12;
        pc += 4;
    }
    else if ((opcode & INST_ORI_MASK) == INST_ORI)
    {
        //printf("%08x: ori r%d, r%d, %d\n", pc, rd, rs1, imm12);
         
        reg_rd = reg_rs1 | imm12;
        pc += 4;
    }
    else if ((opcode & INST_XORI_MASK) == INST_XORI)
    {
        //printf("%08x: xori r%d, r%d, %d\n", pc, rd, rs1, imm12);
         
        reg_rd = reg_rs1 ^ imm12;
        pc += 4;
    }
    else if ((opcode & INST_ADDI_MASK) == INST_ADDI)
    {
        //printf("%08x: addi r%d, r%d, %d\n", pc, rd, rs1, imm12);
        
        reg_rd = reg_rs1 + imm12;
        pc += 4;
    }
    else if ((opcode & INST_SLTI_MASK) == INST_SLTI)
    {
        //printf("%08x: slti r%d, r%d, %d\n", pc, rd, rs1, imm12);
       
        reg_rd = (signed)reg_rs1 < (signed)imm12;
        pc += 4;
    }
    else if ((opcode & INST_SLTIU_MASK) == INST_SLTIU)
    {
        //printf("%08x: sltiu r%d, r%d, %d\n", pc, rd, rs1, (unsigned)imm12);
        
        reg_rd = (unsigned)reg_rs1 < (unsigned)imm12;
        pc += 4;
    }
    else if ((opcode & INST_SLLI_MASK) == INST_SLLI)
    {
        //printf("%08x: slli r%d, r%d, %d\n", pc, rd, rs1, shamt);
        
        reg_rd = reg_rs1 << shamt;
        pc += 4;
    }
    else if ((opcode & INST_SRLI_MASK) == INST_SRLI)
    {
        //printf("%08x: srli r%d, r%d, %d\n", pc, rd, rs1, shamt);
        
        reg_rd = (unsigned)reg_rs1 >> shamt;
        pc += 4;
    }
    else if ((opcode & INST_SRAI_MASK) == INST_SRAI)
    {
        //printf("%08x: srai r%d, r%d, %d\n", pc, rd, rs1, shamt);
         
        reg_rd = (signed)reg_rs1 >> shamt;
        pc += 4;
    }
    else if ((opcode & INST_LUI_MASK) == INST_LUI)
    {
        //printf("%08x: lui r%d, 0x%x\n", pc, rd, imm20);
         
        reg_rd = imm20;
        pc += 4;
    }
    else if ((opcode & INST_AUIPC_MASK) == INST_AUIPC)
    {
        //printf("%08x: auipc r%d, 0x%x\n", pc, rd, imm20);
         
        reg_rd = imm20 + pc;
        pc += 4;
    }
    else if ((opcode & INST_ADD_MASK) == INST_ADD)
    {
        //printf("%08x: add r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        
        reg_rd = reg_rs1 + reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_SUB_MASK) == INST_SUB)
    {
        //printf("%08x: sub r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        reg_rd = reg_rs1 - reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_SLT_MASK) == INST_SLT)
    {
        //printf("%08x: slt r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        reg_rd = (signed)reg_rs1 < (signed)reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_SLTU_MASK) == INST_SLTU)
    {
        //printf("%08x: sltu r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        reg_rd = (unsigned)reg_rs1 < (unsigned)reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_XOR_MASK) == INST_XOR)
    {
        //printf("%08x: xor r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        reg_rd = reg_rs1 ^ reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_OR_MASK) == INST_OR)
    {
        //printf("%08x: or r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        reg_rd = reg_rs1 | reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_AND_MASK) == INST_AND)
    {
        //printf("%08x: and r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        reg_rd = reg_rs1 & reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_SLL_MASK) == INST_SLL)
    {
        //printf("%08x: sll r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        reg_rd = reg_rs1 << reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_SRL_MASK) == INST_SRL)
    {
        //printf("%08x: srl r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        reg_rd = (unsigned)reg_rs1 >> reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_SRA_MASK) == INST_SRA)
    {
        //printf("%08x: sra r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        
        reg_rd = (signed)reg_rs1 >> reg_rs2;
        pc += 4;
    }
    else if ((opcode & INST_JAL_MASK) == INST_JAL)
    {
        //printf("%08x: jal r%d, %d\n", pc, rd, jimm20);
         
        reg_rd = pc + 4;
        pc = pc + jimm20;

        
    }
    else if ((opcode & INST_JALR_MASK) == INST_JALR)
    {
        
        reg_rd = pc + 4;
        pc = (reg_rs1 + imm12) & ~1;
        
    }
    else if ((opcode & INST_BEQ_MASK) == INST_BEQ)
    {
        //printf("%08x: beq r%d, r%d, %d\n", pc, rs1, rs2, bimm);
         
        bool take_branch = (reg_rs1 == reg_rs2);
        if (take_branch)
            pc = pc + bimm;
        else
            pc += 4;

       
        // No writeback
        rd = 0;
      
    }
    else if ((opcode & INST_BNE_MASK) == INST_BNE)
    {
        //printf("%08x: bne r%d, r%d, %d\n", pc, rs1, rs2, bimm);
      

        bool take_branch = (reg_rs1 != reg_rs2);
        if (take_branch)
            pc = pc + bimm;
        else
            pc += 4;
        // No writeback
        rd = 0;
    }
    else if ((opcode & INST_BLT_MASK) == INST_BLT)
    {
        //printf("%08x: blt r%d, r%d, %d\n", pc, rs1, rs2, bimm);
        

        bool take_branch = ((signed)reg_rs1 < (signed)reg_rs2);
        if (take_branch)
            pc = pc + bimm;
        else
            pc += 4;

        

        // No writeback
        rd = 0;

     
    }
    else if ((opcode & INST_BGE_MASK) == INST_BGE)
    {
        //printf("%08x: bge r%d, r%d, %d\n", pc, rs1, rs2, bimm);
        
        bool take_branch = ((signed)reg_rs1 >= (signed)reg_rs2);
        if (take_branch)
            pc = pc + bimm;
        else
            pc += 4;

        
        // No writeback
        rd = 0;

                
    }
    else if ((opcode & INST_BLTU_MASK) == INST_BLTU)
    {
        //printf("%08x: bltu r%d, r%d, %d\n", pc, rs1, rs2, bimm);
        
        bool take_branch = ((unsigned)reg_rs1 < (unsigned)reg_rs2);
        if (take_branch)
            pc = pc + bimm;
        else
            pc += 4;

       

        // No writeback
        rd = 0;

                
    }
    else if ((opcode & INST_BGEU_MASK) == INST_BGEU)
    {
        //printf("%08x: bgeu r%d, r%d, %d\n", pc, rs1, rs2, bimm);
        

        bool take_branch = ((unsigned)reg_rs1 >= (unsigned)reg_rs2);
        if (take_branch)
            pc = pc + bimm;
        else
            pc += 4;

       

        // No writeback
        rd = 0;

                
    }
    else if ((opcode & INST_LB_MASK) == INST_LB)
    {
        //printf("%08x: lb r%d, %d(r%d)\n", pc, rd, imm12, rs1);
        
        if (load(pc, reg_rs1 + imm12, &reg_rd, 1, true))
            pc += 4;
        else
            return false;
    }
    else if ((opcode & INST_LH_MASK) == INST_LH)
    {
        //printf("%08x: lh r%d, %d(r%d)\n", pc, rd, imm12, rs1);
         
        if (load(pc, reg_rs1 + imm12, &reg_rd, 2, true))
            pc += 4;
        else
            return false;
    }
    else if ((opcode & INST_LW_MASK) == INST_LW)
    {
        
        //printf("%08x: lw r%d, %d(r%d)\n", pc, rd, imm12, rs1);
        if (load(pc, reg_rs1 + imm12, &reg_rd, 4, true))
            pc += 4;
        else
            return false;
    }
    else if ((opcode & INST_LBU_MASK) == INST_LBU)
    {
        //printf("%08x: lbu r%d, %d(r%d)\n", pc, rd, imm12, rs1);
         
        if (load(pc, reg_rs1 + imm12, &reg_rd, 1, false))
            pc += 4;
        else
            return false;
    }
    else if ((opcode & INST_LHU_MASK) == INST_LHU)
    {
        //printf("%08x: lhu r%d, %d(r%d)\n", pc, rd, imm12, rs1);
         
        if (load(pc, reg_rs1 + imm12, &reg_rd, 2, false))
            pc += 4;
        else
            return false;
    }
    else if ((opcode & INST_LWU_MASK) == INST_LWU)
    {
        //printf("%08x: lwu r%d, %d(r%d)\n", pc, rd, imm12, rs1);
         
        if (load(pc, reg_rs1 + imm12, &reg_rd, 4, false))
            pc += 4;
        else
            return false;
    }
    else if ((opcode & INST_SB_MASK) == INST_SB)
    {
        //printf("%08x: sb %d(r%d), r%d\n", pc, storeimm, rs1, rs2);
        
        if (store(pc, reg_rs1 + storeimm, reg_rs2, 1))
            pc += 4;
        else
            return false;

        // No writeback
        rd = 0;
    }
    else if ((opcode & INST_SH_MASK) == INST_SH)
    {
        //printf("%08x: sh %d(r%d), r%d\n", pc, storeimm, rs1, rs2);
        
        if (store(pc, reg_rs1 + storeimm, reg_rs2, 2))
            pc += 4;
        else
            return false;

        // No writeback
        rd = 0;
    }
    else if ((opcode & INST_SW_MASK) == INST_SW)
    {
        //printf("%08x: sw %d(r%d), r%d\n", pc, storeimm, rs1, rs2);
         
        if (store(pc, reg_rs1 + storeimm, reg_rs2, 4)){
            pc += 4;
        }else return false;

        // No writeback
        rd = 0;
    }
    else if (m_enable_rvm && (opcode & INST_MUL_MASK) == INST_MUL)
    {
        //printf("%08x: mul r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        
        reg_rd = (signed)reg_rs1 * (signed)reg_rs2;
        pc += 4;
    }
    else if (m_enable_rvm && (opcode & INST_MULH_MASK) == INST_MULH)
    {
        long long res = ((long long) (int)reg_rs1) * ((long long)(int)reg_rs2);
        
        //printf("%08x: mulh r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        reg_rd = (int)(res >> 32);
        pc += 4;
    }
    else if (m_enable_rvm && (opcode & INST_MULHSU_MASK) == INST_MULHSU)
    {
        long long res = ((long long) (int)reg_rs1) * ((unsigned long long)(unsigned)reg_rs2);
       
        //printf("%08x: mulhsu r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        reg_rd = (int)(res >> 32);
        pc += 4;
    }
    else if (m_enable_rvm && (opcode & INST_MULHU_MASK) == INST_MULHU)
    {
        unsigned long long res = ((unsigned long long) (unsigned)reg_rs1) * ((unsigned long long)(unsigned)reg_rs2);
         
        //printf("%08x: mulhu r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        reg_rd = (int)(res >> 32);
        pc += 4;
    }
    else if (m_enable_rvm && (opcode & INST_DIV_MASK) == INST_DIV)
    {
        //printf("%08x: div r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        
        if ((signed)reg_rs1 == INT32_MIN && (signed)reg_rs2 == -1)
            reg_rd = reg_rs1;
        else if (reg_rs2 != 0)
            reg_rd = (signed)reg_rs1 / (signed)reg_rs2;
        else
            reg_rd = (unsigned)-1;
        pc += 4;
    }
    else if (m_enable_rvm && (opcode & INST_DIVU_MASK) == INST_DIVU)
    {
        //printf("%08x: divu r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        
      
        if (reg_rs2 != 0)
            reg_rd = (unsigned)reg_rs1 / (unsigned)reg_rs2;
        else
            reg_rd = (unsigned)-1;
        pc += 4;
    }
    else if (m_enable_rvm && (opcode & INST_REM_MASK) == INST_REM)
    {
        //printf("%08x: rem r%d, r%d, r%d\n", pc, rd, rs1, rs2);
        

        if((signed)reg_rs1 == INT32_MIN && (signed)reg_rs2 == -1)
            reg_rd = 0;
        else if (reg_rs2 != 0)
            reg_rd = (signed)reg_rs1 % (signed)reg_rs2;
        else
            reg_rd = reg_rs1;
        pc += 4;
    }
    else if (m_enable_rvm && (opcode & INST_REMU_MASK) == INST_REMU)
    {
        //printf("%08x: remu r%d, r%d, r%d\n", pc, rd, rs1, rs2);
         
        if (reg_rs2 != 0)
            reg_rd = (unsigned)reg_rs1 % (unsigned)reg_rs2;
        else
            reg_rd = reg_rs1;
        pc += 4;
    }
    else if ((opcode & INST_ECALL_MASK) == INST_ECALL)
    {
        //printf("%08x: ecall\n", pc);
        exception(MCAUSE_ECALL_U + m_csr_mpriv, pc,0);
        take_exception   = true;
    }
    else if ((opcode & INST_EBREAK_MASK) == INST_EBREAK)
    {
        //printf("%08x: ebreak\n", pc);
        

        exception(MCAUSE_BREAKPOINT, pc,0);
        take_exception   = true;
        m_break          = true;
    }
    else if ((opcode & INST_MRET_MASK) == INST_MRET)
    {
        //printf("%08x: mret\n", pc);
        

        //assert(m_csr_mpriv == PRIV_MACHINE);

        uint32_t s        = m_csr_msr;
        uint32_t prev_prv = SR_GET_MPP(m_csr_msr);

        // Interrupt enable pop
        s &= ~SR_MIE;
        s |= (s & SR_MPIE) ? SR_MIE : 0;
        s |= SR_MPIE;

        // Set next MPP to user mode
        s &= ~SR_MPP;
        s |=  SR_MPP_U;

        // Set privilege level to previous MPP
        m_csr_mpriv   = prev_prv;
        m_csr_msr     = s;

        // Return to EPC
        pc = m_csr_mepc;
        //printf("%08x: mret\n", pc);
         
    }
    else if ((opcode & INST_SRET_MASK) == INST_SRET)
    {
        //printf("%08x: sret\n", pc);
        
        //assert(m_csr_mpriv == PRIV_SUPER);

        uint32_t s        = m_csr_msr;
        uint32_t prev_prv = (m_csr_msr & SR_SPP) ? PRIV_SUPER : PRIV_USER;

        // Interrupt enable pop
        s &= ~SR_SIE;
        s |= (s & SR_SPIE) ? SR_SIE : 0;
        s |= SR_SPIE;

        // Set next SPP to user mode
        s &= ~SR_SPP;

        // Set privilege level to previous MPP
        m_csr_mpriv   = prev_prv;
        m_csr_msr     = s;

        // Return to EPC
        pc = m_csr_sepc;
    }
    else if ((opcode & INST_CSRRW_MASK) == INST_CSRRW)
    {
        //DPRINTF(LOG_INST,("%08x: csrw r%d, r%d, 0x%x\n", pc, rd, rs1, imm12));
        //INST_STAT(ENUM_INST_CSRRW);
        reg_rd = access_csr2(imm12, reg_rs1, true, true);
        //if (imm12==CSR_MEPC)
       // {
         //   printf("aqui : %08x\n",reg_rs1 );
       // }
        pc += 4;
    }    
    else if ((opcode & INST_CSRRS_MASK) == INST_CSRRS)
    {
        //DPRINTF(LOG_INST,("%08x: csrs r%d, r%d, 0x%x\n", pc, rd, rs1, imm12));
        //INST_STAT(ENUM_INST_CSRRS);
        reg_rd = access_csr2(imm12, reg_rs1, true, false);
        pc += 4;
    }
    else if ((opcode & INST_CSRRC_MASK) == INST_CSRRC)
    {
        //DPRINTF(LOG_INST,("%08x: csrc r%d, r%d, 0x%x\n", pc, rd, rs1, imm12));
        //INST_STAT(ENUM_INST_CSRRC);
        reg_rd = access_csr2(imm12, reg_rs1, false, true);
        pc += 4;
    }
    else if ((opcode & INST_CSRRWI_MASK) == INST_CSRRWI)
    {
        //DPRINTF(LOG_INST,("%08x: csrwi r%d, %d, 0x%x\n", pc, rd, rs1, imm12));
        //INST_STAT(ENUM_INST_CSRRWI);
        reg_rd = access_csr2(imm12, rs1, true, true);
        pc += 4;
    }
    else if ((opcode & INST_CSRRSI_MASK) == INST_CSRRSI)
    {
        //DPRINTF(LOG_INST,("%08x: csrsi r%d, %d, 0x%x\n", pc, rd, rs1, imm12));
        //INST_STAT(ENUM_INST_CSRRSI);
        reg_rd = access_csr2(imm12, rs1, true, false);
        pc += 4;
    }
    else if ((opcode & INST_CSRRCI_MASK) == INST_CSRRCI)
    {
        //DPRINTF(LOG_INST,("%08x: csrci r%d, %d, 0x%x\n", pc, rd, rs1, imm12));
        //INST_STAT(ENUM_INST_CSRRCI);
        reg_rd = access_csr2(imm12, rs1, false, true);
        pc += 4;
    }
    else if ((opcode & INST_WFI_MASK) == INST_WFI)
    {
        //printf("%08x: wfi\n", pc);
        
        pc += 4;
    }
    else
    {
        exception(MCAUSE_ILLEGAL_INSTRUCTION, pc, opcode);
        m_fault =true;
        take_exception = true;
    }

    if (rd != 0 && !take_exception)
        m_gpr[rd] = reg_rd;




     /*if ((m_csr_mip & m_csr_mie)  && (m_csr_msr & SR_MIE)) {
         exception(MCAUSE_INTERRUPT + 0, m_pc,0);
         take_exception = true;
     }*/



    if (!take_exception)
        m_pc = pc;

    return true;
}



void      write8(uint32_t addr, uint8_t data){
	      /*if(addr >= 0x186a0){//ram
             memory[addr]=data;
	      }else{//flash
             memory[addr-_baseAddr]=data;
	      }*/
          memory[addr]=data;
}
uint8_t   read8(uint32_t addr){
	      /*if(addr >= 0x186a0){//ram
             return memory[addr];
	      }else{//flash
             return memory[addr-_baseAddr];
	      }*/
          return memory[addr];
}


void      write16(uint32_t address, uint16_t val){
	  uint8_t dato[2];
      dato[0] = val & 0xff;
      dato[1] = (val >> 8) & 0xff;

      for (int i = 0; i < 2; ++i){
           write8(address + i, dato[i]);
      }
}

uint16_t  read16(uint32_t address){
	 uint8_t dato[2];
     uint16_t res=0;
 
     for(int i = 0; i < 2; ++i){
         dato[i]=read8(address+i);
     } 
     res= *(uint16_t *)dato;
     return res;
}

void  write32(uint32_t address, uint32_t val){
	 uint8_t dato[4];
     dato[0] = val & 0xff;
     dato[1] = (val >> 8) & 0xff;
     dato[2] = (val >> 16) & 0xff;
     dato[3] = (val >> 24) & 0xff;
     //
     for (int i = 0; i < 4; ++i){
          write8(address + i, dato[i]);
     }
}

uint32_t  read32(uint32_t address){
	  uint8_t dato[4];
      uint32_t res=0;
	  for (int i = 0; i < 4; ++i){
           dato[i]=read8(address+i);
      } 
      res= *(uint32_t *)dato;
      return res;
}

uint32_t  ifetch32(uint32_t address){
	return read32(address); 
}
uint16_t  ifetch16(uint32_t address){
	return read16(address);
}