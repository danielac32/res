#include "context.h"

/*
__attribute__ ((section(".irq_vectors"),aligned(4)))
void (* const irq_vectors[])(void) =
{
reset,
irq,
irq2,
isr_default,// eclic_bwei_handler
isr_default,// eclic_pmovi_handler
isr_default,// WWDGT interrupt
isr_default,// LVD from EXTI interrupt
isr_default,// Tamper interrupt
isr_default,// RTC global interrupt
isr_default,// FMC global interrupt
isr_default,// RCU global interrupt
isr_default,// EXTI Line0 interrupt
isr_default,// EXTI Line1 interrupt
isr_default,// EXTI Line2 interrupt
isr_default,// EXTI Line3 interrupt
isr_default,// EXTI Line4 interrupt
isr_default,// DMA0 channel0 global interrupt
isr_default,// DMA0 channel1 global interrupt
isr_default,// DMA0 channel2 global interrupt
isr_default,// DMA0 channel3 global interrupt
isr_default,// DMA0 channel4 global interrupt
isr_default,// DMA0 channel5 global interrupt

};

extern void entry();
void reset(){
entry();
}
void irq(){

}
void irq2(){

}

void isr_default(){

}*/