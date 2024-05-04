/*
 * Swamp - cooperative multitasking operating system
 * Copyright (c) 2016 Andrey Skrypka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef SYSCFG_H
#define SYSCFG_H

#include <types.h>

struct syscfg
{
    volatile u32_t CFGR1;
    u32_t RESERVED;
    volatile u32_t EXTICR1;
    volatile u32_t EXTICR2;
    volatile u32_t EXTICR3;
    volatile u32_t EXTICR4;
    volatile u32_t CFGR2;
    u32_t RESERVED1[25];
    volatile u32_t IT_LINE_SR[32];
};

#define SYSCFG ((struct syscfg *)0x40010000)

#define SYSCFG_CFGR1_MEM_MODE ((u32_t)0x00000003)
#define SYSCFG_CFGR1_MEM_MODE_0 ((u32_t)0x00000001)
#define SYSCFG_CFGR1_MEM_MODE_1 ((u32_t)0x00000002)
#define SYSCFG_CFGR1_DMA_RMP ((u32_t)0x7F007F00)
#define SYSCFG_CFGR1_ADC_DMA_RMP ((u32_t)0x00000100)
#define SYSCFG_CFGR1_USART1TX_DMA_RMP ((u32_t)0x00000200)
#define SYSCFG_CFGR1_USART1RX_DMA_RMP ((u32_t)0x00000400)
#define SYSCFG_CFGR1_TIM16_DMA_RMP ((u32_t)0x00000800)
#define SYSCFG_CFGR1_TIM17_DMA_RMP ((u32_t)0x00001000)
#define SYSCFG_CFGR1_I2C_FMP_PB6 ((u32_t)0x00010000)
#define SYSCFG_CFGR1_I2C_FMP_PB7 ((u32_t)0x00020000)
#define SYSCFG_CFGR1_I2C_FMP_PB8 ((u32_t)0x00040000)
#define SYSCFG_CFGR1_I2C_FMP_PB9 ((u32_t)0x00080000)
#define SYSCFG_CFGR1_I2C_FMP_I2C1 ((u32_t)0x00100000)
#define SYSCFG_CFGR1_I2C_FMP_PA9 ((u32_t)0x00400000)
#define SYSCFG_CFGR1_I2C_FMP_PA10 ((u32_t)0x00800000)
#define SYSCFG_CFGR1_PA11_PA12_RMP ((u32_t)0x00000010)
#define SYSCFG_CFGR1_I2C_FMP_I2C2 ((u32_t)0x00200000)
#define SYSCFG_CFGR1_USART3_DMA_RMP ((u32_t)0x04000000)
#define SYSCFG_CFGR1_TIM16_DMA_RMP2 ((u32_t)0x00002000)
#define SYSCFG_CFGR1_TIM17_DMA_RMP2 ((u32_t)0x00004000)
#define SYSCFG_CFGR1_SPI2_DMA_RMP ((u32_t)0x01000000)
#define SYSCFG_CFGR1_USART2_DMA_RMP ((u32_t)0x02000000)
#define SYSCFG_CFGR1_I2C1_DMA_RMP ((u32_t)0x08000000)
#define SYSCFG_CFGR1_TIM1_DMA_RMP ((u32_t)0x10000000)
#define SYSCFG_CFGR1_TIM2_DMA_RMP ((u32_t)0x20000000)
#define SYSCFG_CFGR1_TIM3_DMA_RMP ((u32_t)0x40000000)
#define SYSCFG_CFGR1_IRDA_ENV_SEL ((u32_t)0x000000C0)
#define SYSCFG_CFGR1_IRDA_ENV_SEL_0 ((u32_t)0x00000040)
#define SYSCFG_CFGR1_IRDA_ENV_SEL_1 ((u32_t)0x00000080)

#define SYSCFG_CFGR2_LOCKUP_LOCK ((u32_t)0x00000001)
#define SYSCFG_CFGR2_SRAM_PARITY_LOCK ((u32_t)0x00000002)
#define SYSCFG_CFGR2_SRAM_PEF ((u32_t)0x00000100)
#define SYSCFG_CFGR2_PVD_LOCK ((u32_t)0x00000004)

#define SYSCFG_EXTICR1_EXTI0 ((u32_t)0x0000000F)
#define SYSCFG_EXTICR1_EXTI1 ((u32_t)0x000000F0)
#define SYSCFG_EXTICR1_EXTI2 ((u32_t)0x00000F00)
#define SYSCFG_EXTICR1_EXTI3 ((u32_t)0x0000F000)
#define SYSCFG_EXTICR1_EXTI0_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR1_EXTI0_PB ((u32_t)0x00000001)
#define SYSCFG_EXTICR1_EXTI0_PC ((u32_t)0x00000002)
#define SYSCFG_EXTICR1_EXTI0_PD ((u32_t)0x00000003)
#define SYSCFG_EXTICR1_EXTI0_PF ((u32_t)0x00000005)
#define SYSCFG_EXTICR1_EXTI1_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR1_EXTI1_PB ((u32_t)0x00000010)
#define SYSCFG_EXTICR1_EXTI1_PC ((u32_t)0x00000020)
#define SYSCFG_EXTICR1_EXTI1_PD ((u32_t)0x00000030)
#define SYSCFG_EXTICR1_EXTI1_PF ((u32_t)0x00000050)
#define SYSCFG_EXTICR1_EXTI2_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR1_EXTI2_PB ((u32_t)0x00000100)
#define SYSCFG_EXTICR1_EXTI2_PC ((u32_t)0x00000200)
#define SYSCFG_EXTICR1_EXTI2_PD ((u32_t)0x00000300)
#define SYSCFG_EXTICR1_EXTI2_PF ((u32_t)0x00000500)
#define SYSCFG_EXTICR1_EXTI3_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR1_EXTI3_PB ((u32_t)0x00001000)
#define SYSCFG_EXTICR1_EXTI3_PC ((u32_t)0x00002000)
#define SYSCFG_EXTICR1_EXTI3_PD ((u32_t)0x00003000)
#define SYSCFG_EXTICR1_EXTI3_PF ((u32_t)0x00005000)
#define SYSCFG_EXTICR1_EXTI0_PE ((u32_t)0x00000004)
#define SYSCFG_EXTICR1_EXTI1_PE ((u32_t)0x00000040)
#define SYSCFG_EXTICR1_EXTI2_PE ((u32_t)0x00000400)
#define SYSCFG_EXTICR1_EXTI3_PE ((u32_t)0x00004000)

#define SYSCFG_EXTICR2_EXTI4 ((u32_t)0x0000000F)
#define SYSCFG_EXTICR2_EXTI5 ((u32_t)0x000000F0)
#define SYSCFG_EXTICR2_EXTI6 ((u32_t)0x00000F00)
#define SYSCFG_EXTICR2_EXTI7 ((u32_t)0x0000F000)
#define SYSCFG_EXTICR2_EXTI4_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR2_EXTI4_PB ((u32_t)0x00000001)
#define SYSCFG_EXTICR2_EXTI4_PC ((u32_t)0x00000002)
#define SYSCFG_EXTICR2_EXTI4_PD ((u32_t)0x00000003)
#define SYSCFG_EXTICR2_EXTI4_PF ((u32_t)0x00000005)
#define SYSCFG_EXTICR2_EXTI5_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR2_EXTI5_PB ((u32_t)0x00000010)
#define SYSCFG_EXTICR2_EXTI5_PC ((u32_t)0x00000020)
#define SYSCFG_EXTICR2_EXTI5_PD ((u32_t)0x00000030)
#define SYSCFG_EXTICR2_EXTI5_PF ((u32_t)0x00000050)
#define SYSCFG_EXTICR2_EXTI6_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR2_EXTI6_PB ((u32_t)0x00000100)
#define SYSCFG_EXTICR2_EXTI6_PC ((u32_t)0x00000200)
#define SYSCFG_EXTICR2_EXTI6_PD ((u32_t)0x00000300)
#define SYSCFG_EXTICR2_EXTI6_PF ((u32_t)0x00000500)
#define SYSCFG_EXTICR2_EXTI7_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR2_EXTI7_PB ((u32_t)0x00001000)
#define SYSCFG_EXTICR2_EXTI7_PC ((u32_t)0x00002000)
#define SYSCFG_EXTICR2_EXTI7_PD ((u32_t)0x00003000)
#define SYSCFG_EXTICR2_EXTI7_PF ((u32_t)0x00005000)
#define SYSCFG_EXTICR2_EXTI4_PE ((u32_t)0x00000004)
#define SYSCFG_EXTICR2_EXTI5_PE ((u32_t)0x00000040)
#define SYSCFG_EXTICR2_EXTI6_PE ((u32_t)0x00000400)
#define SYSCFG_EXTICR2_EXTI7_PE ((u32_t)0x00004000)

#define SYSCFG_EXTICR3_EXTI8 ((u32_t)0x0000000F)
#define SYSCFG_EXTICR3_EXTI9 ((u32_t)0x000000F0)
#define SYSCFG_EXTICR3_EXTI10 ((u32_t)0x00000F00)
#define SYSCFG_EXTICR3_EXTI11 ((u32_t)0x0000F000)
#define SYSCFG_EXTICR3_EXTI8_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR3_EXTI8_PB ((u32_t)0x00000001)
#define SYSCFG_EXTICR3_EXTI8_PC ((u32_t)0x00000002)
#define SYSCFG_EXTICR3_EXTI8_PD ((u32_t)0x00000003)
#define SYSCFG_EXTICR3_EXTI8_PF ((u32_t)0x00000005)
#define SYSCFG_EXTICR3_EXTI9_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR3_EXTI9_PB ((u32_t)0x00000010)
#define SYSCFG_EXTICR3_EXTI9_PC ((u32_t)0x00000020)
#define SYSCFG_EXTICR3_EXTI9_PD ((u32_t)0x00000030)
#define SYSCFG_EXTICR3_EXTI9_PF ((u32_t)0x00000050)
#define SYSCFG_EXTICR3_EXTI10_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR3_EXTI10_PB ((u32_t)0x00000100)
#define SYSCFG_EXTICR3_EXTI10_PC ((u32_t)0x00000200)
#define SYSCFG_EXTICR3_EXTI10_PD ((u32_t)0x00000300)
#define SYSCFG_EXTICR3_EXTI10_PF ((u32_t)0x00000500)
#define SYSCFG_EXTICR3_EXTI11_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR3_EXTI11_PB ((u32_t)0x00001000)
#define SYSCFG_EXTICR3_EXTI11_PC ((u32_t)0x00002000)
#define SYSCFG_EXTICR3_EXTI11_PD ((u32_t)0x00003000)
#define SYSCFG_EXTICR3_EXTI11_PF ((u32_t)0x00005000)
#define SYSCFG_EXTICR3_EXTI8_PE ((u32_t)0x00000004)
#define SYSCFG_EXTICR3_EXTI9_PE ((u32_t)0x00000040)
#define SYSCFG_EXTICR3_EXTI10_PE ((u32_t)0x00000400)
#define SYSCFG_EXTICR3_EXTI11_PE ((u32_t)0x00004000)

#define SYSCFG_EXTICR4_EXTI12 ((u32_t)0x0000000F)
#define SYSCFG_EXTICR4_EXTI13 ((u32_t)0x000000F0)
#define SYSCFG_EXTICR4_EXTI14 ((u32_t)0x00000F00)
#define SYSCFG_EXTICR4_EXTI15 ((u32_t)0x0000F000)
#define SYSCFG_EXTICR4_EXTI12_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR4_EXTI12_PB ((u32_t)0x00000001)
#define SYSCFG_EXTICR4_EXTI12_PC ((u32_t)0x00000002)
#define SYSCFG_EXTICR4_EXTI12_PD ((u32_t)0x00000003)
#define SYSCFG_EXTICR4_EXTI12_PF ((u32_t)0x00000005)
#define SYSCFG_EXTICR4_EXTI13_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR4_EXTI13_PB ((u32_t)0x00000010)
#define SYSCFG_EXTICR4_EXTI13_PC ((u32_t)0x00000020)
#define SYSCFG_EXTICR4_EXTI13_PD ((u32_t)0x00000030)
#define SYSCFG_EXTICR4_EXTI13_PF ((u32_t)0x00000050)
#define SYSCFG_EXTICR4_EXTI14_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR4_EXTI14_PB ((u32_t)0x00000100)
#define SYSCFG_EXTICR4_EXTI14_PC ((u32_t)0x00000200)
#define SYSCFG_EXTICR4_EXTI14_PD ((u32_t)0x00000300)
#define SYSCFG_EXTICR4_EXTI14_PF ((u32_t)0x00000500)
#define SYSCFG_EXTICR4_EXTI15_PA ((u32_t)0x00000000)
#define SYSCFG_EXTICR4_EXTI15_PB ((u32_t)0x00001000)
#define SYSCFG_EXTICR4_EXTI15_PC ((u32_t)0x00002000)
#define SYSCFG_EXTICR4_EXTI15_PD ((u32_t)0x00003000)
#define SYSCFG_EXTICR4_EXTI15_PF ((u32_t)0x00005000)
#define SYSCFG_EXTICR4_EXTI12_PE ((u32_t)0x00000004)
#define SYSCFG_EXTICR4_EXTI13_PE ((u32_t)0x00000040)
#define SYSCFG_EXTICR4_EXTI14_PE ((u32_t)0x00000400)
#define SYSCFG_EXTICR4_EXTI15_PE ((u32_t)0x00004000)

#define SYSCFG_ITLINE0_SR_EWDG ((u32_t)0x00000001)

#define SYSCFG_ITLINE1_SR_PVDOUT ((u32_t)0x00000001)
#define SYSCFG_ITLINE1_SR_VDDIO2 ((u32_t)0x00000002)

#define SYSCFG_ITLINE10_SR_DMA1_CH2 ((u32_t)0x00000001)
#define SYSCFG_ITLINE10_SR_DMA1_CH3 ((u32_t)0x00000002)
#define SYSCFG_ITLINE10_SR_DMA2_CH1 ((u32_t)0x00000004)
#define SYSCFG_ITLINE10_SR_DMA2_CH2 ((u32_t)0x00000008)

#define SYSCFG_ITLINE11_SR_DMA1_CH4 ((u32_t)0x00000001)
#define SYSCFG_ITLINE11_SR_DMA1_CH5 ((u32_t)0x00000002)
#define SYSCFG_ITLINE11_SR_DMA1_CH6 ((u32_t)0x00000004)
#define SYSCFG_ITLINE11_SR_DMA1_CH7 ((u32_t)0x00000008)
#define SYSCFG_ITLINE11_SR_DMA2_CH3 ((u32_t)0x00000010)
#define SYSCFG_ITLINE11_SR_DMA2_CH4 ((u32_t)0x00000020)
#define SYSCFG_ITLINE11_SR_DMA2_CH5 ((u32_t)0x00000040)

#define SYSCFG_ITLINE12_SR_ADC ((u32_t)0x00000001)
#define SYSCFG_ITLINE12_SR_COMP1 ((u32_t)0x00000002)
#define SYSCFG_ITLINE12_SR_COMP2 ((u32_t)0x00000004)

#define SYSCFG_ITLINE13_SR_TIM1_BRK ((u32_t)0x00000001)
#define SYSCFG_ITLINE13_SR_TIM1_UPD ((u32_t)0x00000002)
#define SYSCFG_ITLINE13_SR_TIM1_TRG ((u32_t)0x00000004)
#define SYSCFG_ITLINE13_SR_TIM1_CCU ((u32_t)0x00000008)

#define SYSCFG_ITLINE14_SR_TIM1_CC ((u32_t)0x00000001)

#define SYSCFG_ITLINE15_SR_TIM2_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE16_SR_TIM3_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE17_SR_DAC ((u32_t)0x00000001)
#define SYSCFG_ITLINE17_SR_TIM6_GLB ((u32_t)0x00000002)

#define SYSCFG_ITLINE18_SR_TIM7_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE19_SR_TIM14_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE2_SR_RTC_ALRA ((u32_t)0x00000001)
#define SYSCFG_ITLINE2_SR_RTC_TSTAMP ((u32_t)0x00000002)
#define SYSCFG_ITLINE2_SR_RTC_WAKEUP ((u32_t)0x00000004)

#define SYSCFG_ITLINE20_SR_TIM15_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE21_SR_TIM16_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE22_SR_TIM17_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE23_SR_I2C1_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE24_SR_I2C2_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE25_SR_SPI1 ((u32_t)0x00000001)

#define SYSCFG_ITLINE26_SR_SPI2 ((u32_t)0x00000001)

#define SYSCFG_ITLINE27_SR_USART1_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE28_SR_USART2_GLB ((u32_t)0x00000001)

#define SYSCFG_ITLINE29_SR_USART3_GLB ((u32_t)0x00000001)
#define SYSCFG_ITLINE29_SR_USART4_GLB ((u32_t)0x00000002)
#define SYSCFG_ITLINE29_SR_USART5_GLB ((u32_t)0x00000004)
#define SYSCFG_ITLINE29_SR_USART6_GLB ((u32_t)0x00000008)
#define SYSCFG_ITLINE29_SR_USART7_GLB ((u32_t)0x00000010)
#define SYSCFG_ITLINE29_SR_USART8_GLB ((u32_t)0x00000020)

#define SYSCFG_ITLINE3_SR_FLASH_ITF ((u32_t)0x00000001)

#define SYSCFG_ITLINE30_SR_CAN ((u32_t)0x00000001)
#define SYSCFG_ITLINE30_SR_CEC ((u32_t)0x00000002)

#define SYSCFG_ITLINE4_SR_CRS ((u32_t)0x00000001)
#define SYSCFG_ITLINE4_SR_CLK_CTRL ((u32_t)0x00000002)

#define SYSCFG_ITLINE5_SR_EXTI0 ((u32_t)0x00000001)
#define SYSCFG_ITLINE5_SR_EXTI1 ((u32_t)0x00000002)

#define SYSCFG_ITLINE6_SR_EXTI2 ((u32_t)0x00000001)
#define SYSCFG_ITLINE6_SR_EXTI3 ((u32_t)0x00000002)

#define SYSCFG_ITLINE7_SR_EXTI4 ((u32_t)0x00000001)
#define SYSCFG_ITLINE7_SR_EXTI5 ((u32_t)0x00000002)
#define SYSCFG_ITLINE7_SR_EXTI6 ((u32_t)0x00000004)
#define SYSCFG_ITLINE7_SR_EXTI7 ((u32_t)0x00000008)
#define SYSCFG_ITLINE7_SR_EXTI8 ((u32_t)0x00000010)
#define SYSCFG_ITLINE7_SR_EXTI9 ((u32_t)0x00000020)
#define SYSCFG_ITLINE7_SR_EXTI10 ((u32_t)0x00000040)
#define SYSCFG_ITLINE7_SR_EXTI11 ((u32_t)0x00000080)
#define SYSCFG_ITLINE7_SR_EXTI12 ((u32_t)0x00000100)
#define SYSCFG_ITLINE7_SR_EXTI13 ((u32_t)0x00000200)
#define SYSCFG_ITLINE7_SR_EXTI14 ((u32_t)0x00000400)
#define SYSCFG_ITLINE7_SR_EXTI15 ((u32_t)0x00000800)

#define SYSCFG_ITLINE8_SR_TSC_EOA ((u32_t)0x00000001)
#define SYSCFG_ITLINE8_SR_TSC_MCE ((u32_t)0x00000002)

#define SYSCFG_ITLINE9_SR_DMA1_CH1 ((u32_t)0x00000001)

#endif