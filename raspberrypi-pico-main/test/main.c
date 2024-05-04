#include <stdint.h>
#include "reg.h"
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void DELAY ( unsigned int );


void isr_svcall(){

}
void isr_pendsv(){

}

void isr_systick(){
PUT32(SIO_GPIO_OUT_XOR,1<<25);
GET32(STK_CSR);
}



static void clock_init ( void )
{
    PUT32(CLK_SYS_RESUS_CTRL_RW,0);
    //PUT32(CLK_REF_CTRL_RW,0);
    //PUT32(CLK_SYS_CTRL_RW,0);
    PUT32(XOSC_CTRL_RW,0xAA0);      //1 - 15MHZ
    PUT32(XOSC_STARTUP_RW,47);      //straight from the datasheet
    PUT32(XOSC_CTRL_SET,0xFAB000);  //enable
    while(1)
    {
        if((GET32(XOSC_STATUS_RW)&0x80000000)!=0) break;
    }
    PUT32(CLK_REF_CTRL_RW,2); //XOSC
    PUT32(CLK_SYS_CTRL_RW,0); //reset/clk_ref
    PUT32(CLK_PERI_CTRL_RW,(1<<11)|(4<<5));
}


 

int main ( void )
{

    clock_init();
 

    //release reset on IO_BANK0
    PUT32(RESETS_RESET_CLR,1<<5); //IO_BANK0
    //wait for reset to be done
    while(1)
    {
        if((GET32(RESETS_RESET_DONE_RW)&(1<<5))!=0) break;
    }

    //output disable
    PUT32(SIO_GPIO_OE_CLR,1<<25);
    //turn off pin 25
    PUT32(SIO_GPIO_OUT_CLR,1<<25);

    //set the function select to SIO (software controlled I/O)
    PUT32(IO_BANK0_GPIO25_CTRL_RW,5);

    //output enable
    PUT32(SIO_GPIO_OE_SET,1<<25);

  
 
    
    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,12000000-1);
    PUT32(STK_CVR,12000000-1);
    PUT32(STK_CSR,0x00000007);

    while(1)
    {

        /*//turn on the led
        PUT32(SIO_GPIO_OUT_SET,1<<25);
        do_delay(100);
        //turn off the led
        PUT32(SIO_GPIO_OUT_CLR,1<<25);
        do_delay(100);*/
        asm("wfe");
    }
    while(1);
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
