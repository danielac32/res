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
#ifndef RamDisk_h
#define RamDisk_h
/**
 * \file
 * RamDisk class
 */
//#include <string.h>
//#include <avr/pgmspace.h>
#include <Print.h>
#include <FatStructs.h>
#include <FatApiConstants.h>
#include <RamBaseClass.h>
//------------------------------------------------------------------------------
/** RamDisk version YYYYMMDD */
#define RAM_DISK_VERSION 20140329
//------------------------------------------------------------------------------
/**
 * \typedef fat_t
 *
 * \brief Type for FAT entry
 */
typedef uint16_t fat_t;
//------------------------------------------------------------------------------
/** RamDiskParams version YYYYMMDD */
const uint16_t RAM_DISK_PARAMS_VERSION = 20140328;
/** \class RamDiskParams
 * \brief RamDiskParams file-system parameters stored at location zero.
 */
struct RamDiskParams {
           /** Version of the RamDiskParams structure. */
  uint32_t version;
           /** Size of the FAT in 512 byte blocks. */
  uint16_t fatSize;
           /** Number of data clusters in the volume. */
  uint16_t clusterCount;
           /** Size if the directory in 512 byte blocks */
  uint16_t dirSize;
           /** Shift that produces clusterSize.  
            *  clusterSize = 1 << clusterSizeShift*/
  uint8_t  clusterSizeShift;
}__attribute__((packed));
//------------------------------------------------------------------------------
/** \class RamDisk
 * \brief RamDisk implements a minimal Arduino RamDisk Library
 *
 * RamDisk does not support subdirectories or long file names.
 */
class RamDisk : public Print {
 public:
  /*
   * Public functions
   */
  /** create with file closed */
  RamDisk(void) : m_flags(0) {}
  /** \return The current cluster number. */
  fat_t curCluster(void) const {return m_curCluster;}
  bool close(void);
  /** \return The count of clusters in the volume. */
  static fat_t clusterCount(void) {return m_clusterCount;}
  /** \return The number of 512 byte blocks in a cluster */
  static uint8_t clusterSize(void) {return 1 << m_clusterSizeShift;}
  /** \return The current file position. */
  uint32_t curPosition(void) const {return m_curPosition;}
  /**
   * Set the date/time callback function
   *
   * \param[in] dateTime The user's callback function.  The callback
   * function is of the form:
   *
   * \code
   * void dateTime(uint16_t* date, uint16_t* time) {
   *   uint16_t year;
   *   uint8_t month, day, hour, minute, second;
   *
   *   // User gets date and time from GPS or real-time clock here
   *
   *   // return date using FAT_DATE macro to format fields
   *   *date = FAT_DATE(year, month, day);
   *
   *   // return time using FAT_TIME macro to format fields
   *   *time = FAT_TIME(hour, minute, second);
   * }
   * \endcode
   *
   * Sets the function that is called when a file is created or when
   * a file's directory entry is modified by sync(). All timestamps,
   * access, creation, and modify, are set when a file is created.
   * sync() maintains the last access date and last modify date/time.
   *
   * See the timestamp() function.
   */
  static void dateTimeCallback(
    void (*dateTime)(uint16_t* date, uint16_t* time)) {
    m_dateTime = dateTime;
  }
  /**
   * Cancel the date/time callback function.
   */
  static void dateTimeCallbackCancel(void) {m_dateTime = NULL;}
  bool dirEntry(dir_t* dir);
  int16_t fgets(char* str, int16_t num, char* delim = 0);
  /** \return The file's size in bytes. */
  uint32_t fileSize(void) const {return m_fileSize;}
  static bool format(RamType* dev, uint32_t totalBlocks, 
                     uint8_t dirBlocks, uint8_t clusterSize);
  static bool init(RamType* dev);
  /** return the count of free clusters. */
  static uint16_t freeClusterCount();
  /**
   * Checks the file's open/closed status for this instance of RamDisk.
   * \return The value true if a file is open otherwise false;
   */
  bool isOpen(void) const {return (m_flags & O_ACCMODE) != 0;}
  static void ls(Print* pr, uint8_t flags = 0);
  bool open(const char* fileName, uint8_t oflag);
  bool open(uint16_t entry, uint8_t oflag);
  static void printDirName(Print* pr, const dir_t& dir, uint8_t width);
  static void printFatDate(Print* pr, uint16_t fatDate);
  static void printFatTime(Print* pr, uint16_t fatTime);
  static void printTwoDigits(Print* pr, uint8_t v);
  int16_t read(void);
  int read(void* buf, size_t nbyte);
  static bool readDir(dir_t* dir, uint16_t* index,
                    uint8_t skip = (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY));

  bool remove(void);
  static bool remove(const char* fileName);
  /** Sets the file's current position to zero. */
  void rewind(void) {m_curPosition = m_curCluster = 0;}
  /** \return The number of entries in the root directory. */
  static uint16_t rootDirEntryCount(void) {return m_rootDirEntryCount;}
  /**
   * Seek to current position plus \a pos bytes. See RamDisk::seekSet(). 
   *
   * \param[in] pos offset from current position.
   *
   * \return true for success or false for failure.
   */
  bool seekCur(int32_t pos) {return seekSet(m_curPosition + pos);}
  /** 
   * Seek to end of file.  See RamDisk::seekSet().
   *
   * \return true for success or false for failure.   
   */
  bool seekEnd(void) {return seekSet(m_fileSize);}
  bool seekSet(uint32_t pos);
  bool sync(void);
  bool timestamp(uint8_t flag, uint16_t year, uint8_t month, uint8_t day,
          uint8_t hour, uint8_t minute, uint8_t second);
  bool truncate(uint32_t size);
  /** RamDisk::writeError is set to true if an error occurs during a write().
   * Set RamDisk::writeError to false before calling print() and/or write()
   * and check for true after calls to write() and/or print().
   */
  bool writeError;
  int write(const void *buf, size_t nbyte);
  /**
   * Write for Adruino print.
   *
   * \note Data is moved to the cache but may not be written to the
   * storage device until sync() is called.
   *
   * \param[in] buf Pointer to the location of the data to be written.
   *
   * \param[in] size Number of bytes to write.
   *
   * \return For success write() returns the number of bytes written, always
   * \a size.  If an error occurs, write() returns -1 cast to size_t.
   */
  size_t write(const uint8_t *buf, size_t size) {
    return write((const void*)buf, size);}
  size_t write(uint8_t b);
  int16_t write(const char* str);
  void write_P(PGM_P str);
  void writeln_P(PGM_P str);
//------------------------------------------------------------------------------
 private:
  static const uint16_t FAT_START_BLOCK = 1;      // start of first FAT
  // Volume info
  static bool     m_volumeValid;        // true if volume has been initialized
  static uint8_t  m_blocksPerCluster;   // must be power of 2
  static uint8_t  m_clusterSizeShift;   // Shift to multiply by clusterSize
  static uint16_t m_clusterOffsetMask;  // Mask to determine offset on cluster
  static uint16_t m_rootDirEntryCount;  // should be 512 for FAT16
  static fat_t    m_blocksPerFat;       // number of blocks in one FAT
  static fat_t    m_clusterCount;       // total clusters in volume
  static uint16_t m_rootDirStartBlock;  // start of root dir
  static uint16_t m_dataStartBlock;     // start of data clusters
  static RamType* m_ramDev;

  // callback function for date/time
  static void (*m_dateTime)(uint16_t* date, uint16_t* time);

  // define fields in m_flags
  static uint8_t const F_OFLAG = O_ACCMODE | O_APPEND | O_SYNC;
  static uint8_t const F_FILE_DIR_DIRTY = 0X80;  // require sync directory entry

  uint8_t m_flags;          // see above for bit definitions
  int16_t m_dirEntryIndex;  // index of directory entry for open file
  fat_t m_firstCluster;     // first cluster of file
  uint32_t m_fileSize;      // fileSize
  fat_t m_curCluster;       // current cluster
  uint32_t m_curPosition;   // current byte offset

  static uint32_t clusterAddress(fat_t cluster) {
    uint32_t lba = (uint32_t)m_dataStartBlock 
                   + ((uint32_t)(cluster - 2) << m_clusterSizeShift);
    return lba << 9;  
  }
  static bool fatGet(fat_t cluster, fat_t* value);
  static bool fatPut(fat_t cluster, fat_t value);
  // end of chain test
  static bool isEOC(fat_t cluster) {return cluster >= 0XFFF8;}
  // allocate a cluster to a file
  bool addCluster(void);
  // free a cluster chain
  bool freeChain(fat_t cluster);
  bool open(dir_t* dir, uint8_t oflag);
  static bool readDir(uint16_t index, dir_t *dir);
  static bool writeDir(uint16_t index, dir_t *dir);
};
#endif  // RamDisk_h
