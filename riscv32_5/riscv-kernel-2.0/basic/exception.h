#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <stdint.h>

#define int32_t         int
#define int16_t         short
#define int8_t          char
#define uint32_t        unsigned long
#define uint16_t        unsigned short
#define uint8_t         unsigned char

// Define the base addresses for peripherals
#define SYSTICK_BASE    ((uint32_t) 0xE000E010)
 
#define SYSTICK ((STK_type *) SYSTICK_BASE)

typedef struct
{
  uint32_t CSR;      /* SYSTICK control and status register,       Address offset: 0x00 */
  uint32_t RVR;      /* SYSTICK reload value register,             Address offset: 0x04 */
  uint32_t CVR;      /* SYSTICK current value register,            Address offset: 0x08 */
  uint32_t CALIB;    /* SYSTICK calibration value register,        Address offset: 0x0C */
} STK_type;
uint32_t count;
uint32_t get_tick();
 
 


#endif // __EXCEPTION_H__
