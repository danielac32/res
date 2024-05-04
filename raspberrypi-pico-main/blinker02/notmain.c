
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void DELAY ( unsigned int );

#define RESETS_BASE                 0x4000C000

//#define RESETS_RESET_RW        (RESETS_BASE+0x0+0x0000)
//#define RESETS_RESET_XOR       (RESETS_BASE+0x0+0x1000)
//#define RESETS_RESET_SET       (RESETS_BASE+0x0+0x2000)
#define RESETS_RESET_CLR       (RESETS_BASE+0x0+0x3000)

#define RESETS_RESET_DONE_RW   (RESETS_BASE+0x8+0x0000)
//#define RESETS_RESET_DONE_XOR  (RESETS_BASE+0x8+0x1000)
//#define RESETS_RESET_DONE_SET  (RESETS_BASE+0x8+0x2000)
//#define RESETS_RESET_DONE_CLR  (RESETS_BASE+0x8+0x3000)

#define SIO_BASE                    0xD0000000

//#define SIO_GPIO_OUT_RW             (SIO_BASE+0x10)
//#define SIO_GPIO_OUT_SET            (SIO_BASE+0x14)
#define SIO_GPIO_OUT_CLR            (SIO_BASE+0x18)
#define SIO_GPIO_OUT_XOR            (SIO_BASE+0x1C)

//#define SIO_GPIO_OE_RW              (SIO_BASE+0x20)
#define SIO_GPIO_OE_SET             (SIO_BASE+0x24)
#define SIO_GPIO_OE_CLR             (SIO_BASE+0x28)
//#define SIO_GPIO_OE_XOR             (SIO_BASE+0x2C)

#define PADS_BANK0_BASE             0x4001C000

#define PADS_BANK0_GPIO0_RW         (PADS_BANK0_BASE+0x04+0x0000)
#define PADS_BANK0_GPIO0_XOR        (PADS_BANK0_BASE+0x04+0x1000)
//#define PADS_BANK0_GPIO0_SET        (PADS_BANK0_BASE+0x04+0x2000)
//#define PADS_BANK0_GPIO0_CLR        (PADS_BANK0_BASE+0x04+0x3000)

#define PADS_BANK0_GPIO25_RW        (PADS_BANK0_BASE+0x68+0x0000)
#define PADS_BANK0_GPIO25_XOR       (PADS_BANK0_BASE+0x68+0x1000)
//#define PADS_BANK0_GPIO25_SET       (PADS_BANK0_BASE+0x68+0x2000)
//#define PADS_BANK0_GPIO25_CLR       (PADS_BANK0_BASE+0x68+0x3000)

#define IO_BANK0_BASE               0x40014000

#define IO_BANK0_GPIO0_CTRL_RW      (IO_BANK0_BASE+0x004+0x0000)
//#define IO_BANK0_GPIO0_CTRL_XOR     (IO_BANK0_BASE+0x004+0x1000)
//#define IO_BANK0_GPIO0_CTRL_SET     (IO_BANK0_BASE+0x004+0x2000)
//#define IO_BANK0_GPIO0_CTRL_CLR     (IO_BANK0_BASE+0x004+0x3000)

#define IO_BANK0_GPIO25_CTRL_RW     (IO_BANK0_BASE+0x0CC+0x0000)
//#define IO_BANK0_GPIO25_CTRL_XOR    (IO_BANK0_BASE+0x0CC+0x1000)
//#define IO_BANK0_GPIO25_CTRL_SET    (IO_BANK0_BASE+0x0CC+0x2000)
//#define IO_BANK0_GPIO25_CTRL_CLR    (IO_BANK0_BASE+0x0CC+0x3000)

#define CLOCKS_BASE                 0x40008000

#define CLK_REF_CTRL_RW             (CLOCKS_BASE+0x30+0x0000)
//#define CLK_REF_CTRL_XOR            (CLOCKS_BASE+0x30+0x1000)
//#define CLK_REF_CTRL_SET            (CLOCKS_BASE+0x30+0x2000)
//#define CLK_REF_CTRL_CLR            (CLOCKS_BASE+0x30+0x3000)

#define CLK_SYS_CTRL_RW             (CLOCKS_BASE+0x3C+0x0000)
//#define CLK_SYS_CTRL_XOR            (CLOCKS_BASE+0x3C+0x1000)
//#define CLK_SYS_CTRL_SET            (CLOCKS_BASE+0x3C+0x2000)
//#define CLK_SYS_CTRL_CLR            (CLOCKS_BASE+0x3C+0x3000)

#define CLK_PERI_CTRL_RW            (CLOCKS_BASE+0x48+0x0000)
//#define CLK_PERI_CTRL_XOR           (CLOCKS_BASE+0x48+0x1000)
//#define CLK_PERI_CTRL_SET           (CLOCKS_BASE+0x48+0x2000)
//#define CLK_PERI_CTRL_CLR           (CLOCKS_BASE+0x48+0x3000)

#define CLK_SYS_RESUS_CTRL_RW       (CLOCKS_BASE+0x78+0x0000)
//#define CLK_SYS_RESUS_CTRL_XOR      (CLOCKS_BASE+0x78+0x1000)
//#define CLK_SYS_RESUS_CTRL_SET      (CLOCKS_BASE+0x78+0x2000)
//#define CLK_SYS_RESUS_CTRL_CLR      (CLOCKS_BASE+0x78+0x3000)

#define XOSC_BASE                   0x40024000

#define XOSC_CTRL_RW                (XOSC_BASE+0x00+0x0000)
//#define XOSC_CTRL_XOR               (XOSC_BASE+0x00+0x1000)
#define XOSC_CTRL_SET               (XOSC_BASE+0x00+0x2000)
//#define XOSC_CTRL_CLR               (XOSC_BASE+0x00+0x3000)

#define XOSC_STATUS_RW              (XOSC_BASE+0x04+0x0000)
//#define XOSC_STATUS_XOR             (XOSC_BASE+0x04+0x1000)
//#define XOSC_STATUS_SET             (XOSC_BASE+0x04+0x2000)
//#define XOSC_STATUS_CLR             (XOSC_BASE+0x04+0x3000)

#define XOSC_STARTUP_RW             (XOSC_BASE+0x0C+0x0000)
//#define XOSC_STARTUP_XOR            (XOSC_BASE+0x0C+0x1000)
//#define XOSC_STARTUP_SET            (XOSC_BASE+0x0C+0x2000)
//#define XOSC_STARTUP_CLR            (XOSC_BASE+0x0C+0x3000)

#define UART0_BASE                  0x40034000

#define UART0_BASE_UARTDR_RW        (UART0_BASE+0x000+0x0000)
#define UART0_BASE_UARTFR_RW        (UART0_BASE+0x018+0x0000)
#define UART0_BASE_UARTIBRD_RW      (UART0_BASE+0x024+0x0000)
#define UART0_BASE_UARTFBRD_RW      (UART0_BASE+0x028+0x0000)
#define UART0_BASE_UARTLCR_H_RW     (UART0_BASE+0x02C+0x0000)
#define UART0_BASE_UARTCR_RW        (UART0_BASE+0x030+0x0000)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

 
// ROM
#define ROM_BASE                    0x00000000
// FLASH
#define XIP_BASE                    0x10000000
#define XIP_NOALLOC_BASE            0x11000000
#define XIP_NOCACHE_BASE            0x12000000
#define XIP_NOCACHE_NOALLOC_BASE    0x13000000
#define XIP_CTRL_BASE               0x14000000
#define XIP_SRAM_BASE               0x15000000
#define XIP_SRAM_END                0x15004000
#define XIP_SSI_BASE                0x18000000
// SRAM
#define SRAM_BASE                   0x20000000
#define SRAM_STRIPED_BASE           0x20000000
#define SRAM_STRIPED_END            0x20040000
#define SRAM4_BASE                  0x20040000
#define SRAM5_BASE                  0x20041000
#define SRAM_END                    0x20042000
#define SRAM0_BASE                  0x21000000
#define SRAM1_BASE                  0x21010000
#define SRAM2_BASE                  0x21020000
#define SRAM3_BASE                  0x21030000
// APB
#define SYSINFO_BASE                0x40000000
#define SYSCFG_BASE                 0x40004000
#define CLOCKS_BASE                 0x40008000
#define RESETS_BASE                 0x4000C000
#define PSM_BASE                    0x40010000
#define IO_BANK0_BASE               0x40014000
#define IO_QSPI_BASE                0x40018000
#define PADS_BANK0_BASE             0x4001C000
#define PADS_QSPI_BASE              0x40020000
#define XOSC_BASE                   0x40024000
#define PLL_SYS_BASE                0x40028000
#define PLL_USB_BASE                0x4002C000
#define BUSCTRL_BASE                0x40030000
#define UART0_BASE                  0x40034000
#define UART1_BASE                  0x40038000
#define SPI0_BASE                   0x4003C000
#define SPI1_BASE                   0x40040000
#define I2C0_BASE                   0x40044000
#define I2C1_BASE                   0x40048000
#define ADC_BASE                    0x4004C000
#define PWM_BASE                    0x40050000
#define TIMER_BASE                  0x40054000
#define WATCHDOG_BASE               0x40058000
#define RTC_BASE                    0x4005C000
#define ROSC_BASE                   0x40060000
#define VREG_AND_CHIP_RESET_BASE    0x40064000
#define TBMAN_BASE                  0x4006C000
// AHB
#define DMA_BASE                    0x50000000
#define USBCTRL_BASE                0x50100000
#define USBCTRL_DPRAM_BASE          0x50100000
#define USBCTRL_REGS_BASE           0x50110000
#define PIO0_BASE                   0x50200000
#define PIO1_BASE                   0x50300000
#define XIP_AUX_BASE                0x50400000
// SIO
#define SIO_BASE                    0xD0000000
// CM0+ internal peripherals
#define PPB_BASE                    0xE0000000


typedef unsigned int    uint;
typedef unsigned short  ushort;
typedef unsigned char   byte;

#define IO_RD(addr)            *((volatile uint*)(addr))
#define IO_WR(addr, data)      *((volatile uint*)(addr)) = (data)


#define RESET_ADC           (1 << 0)
#define RESET_BUSCTRL       (1 << 1)
#define RESET_DMA           (1 << 2)
#define RESET_I2C0          (1 << 3)
#define RESET_I2C1          (1 << 4)
#define RESET_IO_BANK0      (1 << 5)
#define RESET_IO_QSPI       (1 << 6)
#define RESET_JTAG          (1 << 7)
#define RESET_PADS_BANK0    (1 << 8)
#define RESET_PADS_QSPI     (1 << 9)
#define RESET_PIO0          (1 << 10)
#define RESET_PIO1          (1 << 11)
#define RESET_PLL_SYS       (1 << 12)
#define RESET_PLL_USB       (1 << 13)
#define RESET_PWM           (1 << 14)
#define RESET_RTC           (1 << 15)
#define RESET_SPI0          (1 << 16)
#define RESET_SPI1          (1 << 17)
#define RESET_SYSCFG        (1 << 18)
#define RESET_SYSINFO       (1 << 19)
#define RESET_TBMAN         (1 << 20)
#define RESET_TIMER         (1 << 21)
#define RESET_UART0         (1 << 22)
#define RESET_UART1         (1 << 23)
#define RESET_USBCTRL       (1 << 24)


void reset_set(uint hw);
void reset_release(uint hw);
void reset_release_wait(uint hw);

#define GPIO_FUNC_XIP           0
#define GPIO_FUNC_SPI           1
#define GPIO_FUNC_UART          2
#define GPIO_FUNC_I2C           3
#define GPIO_FUNC_PWM           4
#define GPIO_FUNC_SIO           5
#define GPIO_FUNC_PIO0          6
#define GPIO_FUNC_PIO1          7
#define GPIO_FUNC_CLK           8
#define GPIO_FUNC_USB           9
#define GPIO_FUNC_NULL          0xF

void gpio_init(uint gpio, uint func);
void gpio_dir(uint gpio, uint out);
void gpio_pullup(uint gpio, uint enable);
void gpio_pulldown(uint gpio, uint enable);
void gpio_set(uint gpio, uint value);
uint gpio_get(uint gpio);
void gpio_deinit(uint gpio);




struct resets_hw {
    uint reset;
    uint wdsel;
    uint reset_done;
};

#define rst ((volatile struct resets_hw*)RESETS_BASE)


void reset_set(uint hw) {
    rst->reset |= hw;
}

void reset_release(uint hw) {
    rst->reset &= (~hw);
}

void reset_release_wait(uint hw) {
    rst->reset &= (~hw);
    while (!(rst->reset_done & hw));
}



struct xosc_hw {
    uint ctrl;
    uint status;
    uint dormant;
    uint startup;
    uint count;
};

#define xosc    ((volatile struct xosc_hw*)XOSC_BASE)

#define CTRL_ENABLE             (0xFAB << 12)
#define CTRL_DISABLE            (0xD1E << 12)
#define CTRL_FREQ_1_15_MHZ      0xAA0

#define STATUS_STABLE           (1 << 31)
#define STATUS_BADWRITE         (1 << 24)
#define STATUS_ENABLED          (1 << 12)
#define STATUS_FREQ_1_15_MHZ    (0 << 0)

#define DORMANT_WAKE            0x77616B65
#define DORMANT_DORMANT         0x636F6D61


void xosc_init() {
    xosc->ctrl = CTRL_FREQ_1_15_MHZ;
    xosc->startup = (((12 * 1000000)/1000)+128)/256; // wait 1 ms
    xosc->ctrl |= CTRL_ENABLE;
    while (!(xosc->status & STATUS_STABLE));
}

void xosc_disable() {
    uint tmp = xosc->ctrl;
    tmp &= (0xFFF << 12);
    tmp |= CTRL_DISABLE;
    xosc->ctrl = tmp;
    while (xosc->status & STATUS_STABLE);
}

void xosc_dormant() {
    xosc->dormant = DORMANT_DORMANT;
    while (!(xosc->status & STATUS_STABLE));
}

struct sio_hw {
    uint cpuid;
    uint gpio_in;
    uint gpio_hi_in;
    uint unused;
    uint gpio_out;
    uint gpio_out_set;
    uint gpio_out_clr;
    uint gpio_out_xor;
    uint gpio_oe;
    uint gpio_oe_set;
    uint gpio_oe_clr;
    uint gpio_oe_xor;
};

struct io_bank0_hw {
    struct gpio {
        uint status;
        uint ctrl;
    } gpio[30];
};

struct pads_bank0_hw {
    uint voltage_select;
    uint gpio[30];
    uint swclk;
    uint swd;
};

#define sio     ((volatile struct sio_hw*)SIO_BASE)
#define io      ((volatile struct io_bank0_hw*)IO_BANK0_BASE)
#define pads    ((volatile struct pads_bank0_hw*)PADS_BANK0_BASE)

#define PADS_GPIO_OD        (1 << 7)
#define PADS_GPIO_IE        (1 << 6)
#define PADS_GPIO_PUE       (1 << 3)
#define PADS_GPIO_PDE       (1 << 2)


void gpio_init(uint gpio, uint func) {
    io->gpio[gpio].ctrl = func;
    pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
    sio->gpio_oe_clr = 1 << gpio;
}

void gpio_dir(uint gpio, uint out) {
    if (out) {
        sio->gpio_oe_set = 1 << gpio;
        pads->gpio[gpio] = 0;
    } else {
        sio->gpio_oe_clr = 1 << gpio;
        pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
    }
}

void gpio_set(uint gpio, uint value) {
    if (value)
        sio->gpio_out_set = 1 << gpio;
    else
        sio->gpio_out_clr = 1 << gpio;
}

uint gpio_get(uint gpio) {
    return sio->gpio_in & (1 << gpio);
}

void gpio_deinit(uint gpio) {
    pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
    sio->gpio_oe_clr = 1 << gpio;
}

void gpio_pullup(uint gpio, uint enable) {
    uint reg = pads->gpio[gpio];

    if (enable) {
        reg |= PADS_GPIO_PUE;
    } else {
        reg &= (~PADS_GPIO_PUE);
    }
    pads->gpio[gpio] = reg;
}

void gpio_pulldown(uint gpio, uint enable) {
    uint reg = pads->gpio[gpio];

    if (enable) {
        reg |= PADS_GPIO_PDE;
    } else {
        reg &= (~PADS_GPIO_PDE);
    }
    pads->gpio[gpio] = reg;
}



struct uart_hw {
    uint dr;
    uint rsr;
    uint unused1[4];
    uint fr;
    uint unuded2;
    uint ilpr;
    uint ibrd;
    uint fbrd;
    uint lcr_h;
    uint cr;
    uint ifls;
    uint imsc;
    uint ris;
    uint mis;
    uint icr;
};

#define uart ((volatile struct uart_hw*)UART0_BASE)

#define FR_TXFE     (1 << 7)
#define FR_RXFF     (1 << 6)
#define FR_TXFF     (1 << 5)
#define FR_RXFE     (1 << 4)
#define FR_BUSY     (1 << 3)

#define LCR_WLEN_5  (0 << 5)
#define LCR_WLEN_6  (1 << 5)
#define LCR_WLEN_7  (2 << 5)
#define LCR_WLEN_8  (3 << 5)
#define LCR_FEN     (1 << 4)

#define CR_RXE      (1 << 9)
#define CR_TXE      (1 << 8)
#define CR_UARTEN   (1 << 0)


void uart_init() {
    IO_WR(CLOCKS_BASE + 0x48, 0x880); // clk_peri enable
    IO_WR(CLOCKS_BASE + 0x30, 0x2);
    reset_release_wait(RESET_IO_BANK0);
    reset_release_wait(RESET_PADS_BANK0);
    reset_release_wait(RESET_UART0);

    uart->cr = 0;
    uart->ibrd = 6; // 115200
    uart->fbrd = 33;
    uart->lcr_h = LCR_WLEN_8 | LCR_FEN;
    uart->cr = CR_TXE | CR_RXE | CR_UARTEN;

    gpio_init(0, GPIO_FUNC_UART);
    gpio_init(1, GPIO_FUNC_UART);
    gpio_dir(0, 1);
}

uint uart_read(char *buff, uint len) {
    uint i;
    for (i = 0; i < len; ++i) {
        while (uart->fr & FR_RXFE);
        buff[i] = uart->dr & 0xFF;
    }
    return i;
}

uint uart_write(char *buff, uint len) {
    uint i;
    for (i = 0; i < len; ++i) {
        while (uart->fr & FR_TXFF);

        uart->dr = buff[i];
    }
    return i;
}

static void hex2str(char *str, int n) {
    int i, hb;
    for (i = 0; i < 8; ++i) {
        hb = n >> (7 - i) * 4;
        hb &= 0x0F;

        if (hb > 9)
            str[i] = (char)(hb + 'A' - 10);
        else
            str[i] = (char)(hb + '0');
    }
    str[8] = 0;
}

void uart_print(char *s) {
    while (*s) {
        uart_write(s, 1);
        ++s;
    }
}

void uart_hex(uint x) {
    char str[8 + 1];
    hex2str(str, x);
    uart_print(str);
}



void isr_svcall(){

}
void isr_pendsv(){

}

void isr_systick(){
 
}


int count=0;
int notmain ( void )
{
    xosc_init();
    uart_init();


    
    while(1)
    {
    

    uart_print("aquii\n");
 
       
      
    }
    return(0);
}

//-------------------------------------------------------------------------
//
// Copyright (c) 2021 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
