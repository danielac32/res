/* Arduino RamDisk Library
 * Copyright (C) 2014 by William Greiman
 *
 * This file is part of the Arduino RamDisk Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with the Arduino RamDisk Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <M23LCV1024.h>
#ifndef __AVR__
#error AVR only
#endif
//------------------------------------------------------------------------------
inline uint8_t spiReceive() {
  SPDR = 0XFF;
  while (!(SPSR & (1 << SPIF)));
  return SPDR;
}
//------------------------------------------------------------------------------
static void spiReceive(uint8_t* buf, size_t n) {
  if (n-- == 0) return;
  SPDR = 0XFF;
  for (size_t i = 0; i < n; i++) {
    while (!(SPSR & (1 << SPIF)));
    uint8_t b = SPDR;
    SPDR = 0XFF;
    buf[i] = b;
  }
  while (!(SPSR & (1 << SPIF)));
  buf[n] = SPDR;
}
//------------------------------------------------------------------------------
inline void spiSend(uint8_t data) {
  SPDR = data;
  while (!(SPSR & (1 << SPIF)));
}
//------------------------------------------------------------------------------
static void spiSend(const uint8_t* buf , size_t n) {
  if (n == 0) return;
  SPDR = buf[0];
  if (n > 1) {
    uint8_t b = buf[1];
    size_t i = 2;
    while (1) {
      while (!(SPSR & (1 << SPIF)));
      SPDR = b;
      if (i == n) break;
      b = buf[i++];
    }
  }
  while (!(SPSR & (1 << SPIF)));
}
//------------------------------------------------------------------------------
inline void sendCmdAddress(uint8_t cmd, uint32_t address) {
 spiSend(cmd);
 spiSend(address >> 16);
 spiSend(address >> 8);
 spiSend(address);
} 
//------------------------------------------------------------------------------
bool M23LCV1024::begin(uint8_t csPin) {
 // set SS high - may be chip select for another SPI device
  digitalWrite(SS, HIGH);

  // SS must be in output mode even it is not chip select
  pinMode(SS, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  m_csPin = csPin;
  pinMode(m_csPin, OUTPUT);
  // Set sequential mode.
  csLow();
  spiSend(WRITE_MODE);
  spiSend(SEQ_MODE);
  csHigh();
}
//------------------------------------------------------------------------------
bool M23LCV1024::read(uint32_t address, void *buf, size_t nbyte) {
  csLow();
  sendCmdAddress(READ_DATA, address);
  spiReceive((uint8_t*)buf, nbyte);
  csHigh();
  return true;
}
//------------------------------------------------------------------------------
bool M23LCV1024::write(uint32_t address, const void *buf, size_t nbyte) {
  csLow();
  sendCmdAddress(WRITE_DATA, address);
  spiSend((const uint8_t*)buf, nbyte);
  csHigh();
  return true;
}