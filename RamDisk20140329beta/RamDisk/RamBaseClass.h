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
#ifndef RamBaseClass_h
#define RamBaseClass_h

// Using RamBaseClass takes more flash but allows RamDisk to be used with
// any derived class.  Set USE_RAM_BASE_CLASS to zero to save a little flash.
// This option may not be in future versions, only saves about 200 bytes.
#define USE_RAM_BASE_CLASS 1

#if USE_RAM_BASE_CLASS
class RamBaseClass;
typedef RamBaseClass RamType;
#else //  USE_RAM_BASE_CLASS
#include <M23LCV1024.h>
class M23LCV1024;
typedef M23LCV1024 RamType;
#endif  // USE_RAM_BASE_CLASS

//------------------------------------------------------------------------------
/** \class RamBaseClass
 * \brief RamBaseClass virtual base class for derived RAM classes.
 */
class RamBaseClass {
 public:
  /**
  * Read data from RAM.
   *
   * \param[in] address Location in RAM to be read.
   *   
   * \param[out] buf Pointer to the location that will receive the data.
   *
   * \param[in] nbyte Number of bytes to read.
   *
   * \return true for success or false for failure.
   */
  virtual bool read(uint32_t address, void *buf, size_t nbyte) = 0;
  /**
   * Write data to RAM.
   *
   * \param[in] address Location in RAM to be written.
   *   
   * \param[in] buf Pointer to the location of the data to be written.
   *
   * \param[in] nbyte Number of bytes to write.
   *
   * \return true for success or false for failure.
   */
  virtual bool write(uint32_t address, const void *buf, size_t nbyte) = 0;
};
#endif RamBaseClass_h