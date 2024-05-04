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
#ifndef M23LCV1024_h
#define M23LCV1024_h
#include <Arduino.h>
#include <RamBaseClass.h>

// see RamBaseClass.h for more on USE_RAM_BASE_CLASS.
#if USE_RAM_BASE_CLASS
class M23LCV1024 : public RamBaseClass {
#else  // USE_RAM_BASE_CLASS
class M23LCV1024 {
#endif  // USE_RAM_BASE_CLASS
 public:
  bool begin(uint8_t csPin);
  bool read(uint32_t address, void *buf, size_t nbyte);
  bool write(uint32_t address, const void *buf, size_t nbyte);
 private:
  //----------------------------------------------------------------------------
  void csLow() {
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = 1 << SPI2X;
    digitalWrite(m_csPin, LOW);
  }
  //----------------------------------------------------------------------------
  void csHigh() {
    digitalWrite(m_csPin, HIGH);
  }
  //----------------------------------------------------------------------------
  static const uint8_t READ_DATA = 0X03;
  static const uint8_t WRITE_DATA = 0X02;
  static const uint8_t WRITE_MODE = 0X01;
  static const uint8_t SEQ_MODE = 0X40;
  uint8_t m_csPin;
};
#endif  // M23LCV1024_h