#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

//-----------------------------------------------------------------
// Prototypes:
//-----------------------------------------------------------------
void      spi_init(void);
void      spi_cs(uint32_t value);
unsigned char transfer(unsigned char data);
#endif
