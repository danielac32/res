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
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <RamDisk.h>
//------------------------------------------------------------------------------
/** Macros for debug. */
#define DBG_FAIL_MACRO   //Serial.print(__FILE__);Serial.println(__LINE__)
#define TRACE //Serial.println(__LINE__)
//------------------------------------------------------------------------------
// volume info
bool     RamDisk::m_volumeValid = false;  // true if volume is valid
uint8_t  RamDisk::m_blocksPerCluster;     // must be power of 2
uint8_t  RamDisk::m_clusterSizeShift;     // Shift to multiply by clusterSize
uint16_t RamDisk::m_clusterOffsetMask;    // Mask to determine offset on cluster
uint16_t RamDisk::m_rootDirEntryCount;    // should be 512 for FAT16
fat_t    RamDisk::m_blocksPerFat;         // number of blocks in FAT
fat_t    RamDisk::m_clusterCount;         // total clusters in volume
uint16_t RamDisk::m_rootDirStartBlock;    // start of root dir
uint16_t RamDisk::m_dataStartBlock;       // start of data clusters
RamType* RamDisk::m_ramDev = 0;           // Raw device
//------------------------------------------------------------------------------
// callback function for date/time
void (*RamDisk::m_dateTime)(uint16_t* date, uint16_t* time) = NULL;
//------------------------------------------------------------------------------
// format 8.3 name for directory entry
static bool make83Name(const char* str, uint8_t* name) {
  uint8_t c;
  uint8_t n = 7;  // max index for part before dot
  uint8_t i = 0;
  // blank fill name and extension
  while (i < 11) name[i++] = ' ';
  i = 0;
  while ((c = *str++) != '\0') {
    if (c == '.') {
      if (n == 10) return false;  // only one dot allowed
      n = 10;  // max index for full 8.3 name
      i = 8;   // place for extension
    } else {
      // illegal FAT characters
      PGM_P p = PSTR("|<>^+=?/[];,*\"\\");
      uint8_t b;
      while ((b = pgm_read_byte(p++))) if (b == c) return false;
      // check length and only allow ASCII printable characters
      if (i > n || c < 0X21 || c > 0X7E) return false;
      // only upper case allowed in 8.3 names - convert lower to upper
      name[i++] = c < 'a' || c > 'z' ?  c : c + ('A' - 'a');
    }
  }
  // must have a file name, extension is optional
  return name[0] != ' ';
}
//==============================================================================
// RamDisk member functions
//------------------------------------------------------------------------------
bool RamDisk::addCluster(void) {
  // start search after last cluster of file or at cluster two in FAT
  fat_t freeCluster = m_curCluster ? m_curCluster : 1;
  for (fat_t i = 0; ; i++) {
    // return no free clusters
    if (i >= m_clusterCount) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // Fat has clusterCount + 2 entries
    if (freeCluster > m_clusterCount) freeCluster = 1;
    freeCluster++;
    fat_t value;
    if (!fatGet(freeCluster, &value)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (value == 0) break;
  }
  // mark cluster allocated
  if (!fatPut(freeCluster, FAT16EOC)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (m_curCluster != 0) {
    // link cluster to chain
    if (!fatPut(m_curCluster, freeCluster)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  } else {
    // first cluster of file so update directory entry
    m_flags |= F_FILE_DIR_DIRTY;
    m_firstCluster = freeCluster;
  }
  m_curCluster = freeCluster;
  return true;
  
 fail:
  return false;
}
//------------------------------------------------------------------------------
/**
 *  Close a file and write directory information.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include no file is open or an I/O error.
 */
bool RamDisk::close(void) {
  bool rtn = sync();
  m_flags = 0;
  return rtn;
}
//------------------------------------------------------------------------------
/**
 * Return a files directory entry
 *
 * \param[out] dir Location for return of the files directory entry.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
bool RamDisk::dirEntry(dir_t* dir) {
// isopen??//////////////////////////////////////////////////////////////////////////////
  if (!sync()) return false;
  return readDir(m_dirEntryIndex, dir);
}
//------------------------------------------------------------------------------
bool RamDisk::fatGet(fat_t cluster, fat_t* value) {
  if (cluster > (m_clusterCount + 1)) return false;
  uint32_t addr = (FAT_START_BLOCK << 9) + ((uint32_t)cluster << 1);
  return m_ramDev->read(addr, value, 2);
}
//------------------------------------------------------------------------------
bool RamDisk::fatPut(fat_t cluster, fat_t value) {
  if (cluster < 2 || cluster > (m_clusterCount + 1)) return false;
  uint32_t addr = (FAT_START_BLOCK << 9) + ((uint32_t)cluster << 1);
  return m_ramDev->write(addr, &value, 2);
}
//------------------------------------------------------------------------------
/**
 * Get a string from a file.
 *
 * fgets() reads bytes from a file into the array pointed to by \a str, until
 * \a num - 1 bytes are read, or a delimiter is read and transferred to \a str,
 * or end-of-file is encountered. The string is then terminated
 * with a null byte.
 *
 * fgets() deletes CR, '\\r', from the string.  This insures only a '\\n'
 * terminates the string for Windows text files which use CRLF for newline.
 *
 * \param[out] str Pointer to the array where the string is stored.
 * \param[in] num Maximum number of characters to be read
 * (including the final null byte). Usually the length
 * of the array \a str is used.
 * \param[in] delim Optional set of delimiters. The default is "\n".
 *
 * \return For success fgets() returns the length of the string in \a str.
 * If no data is read, fgets() returns zero for EOF or -1 if an error occurred.
 **/
int16_t RamDisk::fgets(char* str, int16_t num, char* delim) {
  char ch;
  int16_t n = 0;
  int16_t r = -1;
  while ((n + 1) < num && (r = read(&ch, 1)) == 1) {
    // delete CR
    if (ch == '\r') continue;
    str[n++] = ch;
    if (!delim) {
      if (ch == '\n') break;
    } else {
      if (strchr(delim, ch)) break;
    }
  }
  if (r < 0) {
    // read error
    return -1;
  }
  str[n] = '\0';
  return n;
}
//------------------------------------------------------------------------------
// free a cluster chain
bool RamDisk::freeChain(fat_t cluster) {
  while (1) {
    fat_t next;
    if (!fatGet(cluster, &next)) return false;
    if (!fatPut(cluster, 0)) return false;
    if (isEOC(next)) return true;
    cluster = next;
  }
}
//------------------------------------------------------------------------------
/**
 * \return The number of free clusters in the file-system.
 */
uint16_t RamDisk::freeClusterCount() {
  uint16_t free = 0;
  // Fat has clusterCount + 2 entries.  First two are dummy
  for (fat_t i = 2; i < (m_clusterCount + 2); i++) {
    fat_t f;
    if (!fatGet(i, &f)) return 0;
    if (f == 0) free++;
  }
  return free;
}
//------------------------------------------------------------------------------
/**
 * Format the RamDisk volume.
 *
 * \param[in] dev the raw RAM device.
 *
 * \param[in] totalBlocks total number of 512 byte blocks to be used
 *            in the volume.
 *
 * \param[in]  dirBlocks Number of 512 bytes to be allocated to the directory.
 *             Each block contains 16 directory entries.
 *
 * \param[in]  clusterSize number of blocks in a cluster.  clusterSize
 *             must be a power of two less than 128.
 *
 * \return true for success or false for failure.
 */
bool RamDisk::format(RamType* dev, uint32_t totalBlocks, 
                     uint8_t dirBlocks, uint8_t clusterSize) {
                     
  RamDiskParams params;
  
  if (dirBlocks == 0 || totalBlocks < (dirBlocks + 2 + clusterSize)
    || totalBlocks > 0X800000) {
    return false;
  }
 // determine shift that is same as multiply by clusterSize
  uint8_t clusterSizeShift = 0;
  for (uint8_t tmp = 1; clusterSize != tmp; clusterSizeShift++) {
    tmp <<= 1;
    if (tmp == 128) {
      return false;
    }
  }  
  uint16_t fatSize;
  for (fatSize = 1; fatSize < 256; fatSize++) {
    uint32_t s = fatSize*(clusterSize*256UL + 1) + dirBlocks - 1;
    if (s >= totalBlocks) {
      break;
    }
  }
  if (fatSize > 255) return false;
  uint16_t clusterCount = (totalBlocks - fatSize - dirBlocks - 1)/clusterSize;
  uint16_t dataStartBlock = FAT_START_BLOCK + fatSize + dirBlocks;
  fat_t fat[2];
  fat[0] = fat[1] = 0;  
  for (uint32_t add = 0; add < 512UL*dataStartBlock; add += sizeof(fat)) {
    if (!dev->write(add, fat, sizeof(fat))) return false;
  }
  params.version = RAM_DISK_PARAMS_VERSION;
  params.fatSize = fatSize;
  params.clusterCount = clusterCount;
  params.dirSize = dirBlocks;
  params.clusterSizeShift = clusterSizeShift;
  if (!dev->write(0, &params, sizeof(params))) return false;
  fat[0] = fat[1] = 0XFFFF;
  if (!dev->write(512, &fat, 4)) return false;
  return true;
}
//------------------------------------------------------------------------------
/**
 * Initialize the RamDisk volume.
 * \param[in] dev the raw RAM device.
 *
 * \return true for success or false for failure.
 */
bool RamDisk::init(RamType* dev) {
  RamDiskParams params;
  if (!dev->read(0, &params, sizeof(params))) return false;

  if (params.version != RAM_DISK_PARAMS_VERSION
    || params.clusterSizeShift > 6
    || params.fatSize == 0 || params.fatSize > 255
    || params.dirSize == 0 || params.dirSize > 255) {
    return false;
  }
  m_clusterSizeShift = params.clusterSizeShift;
  m_clusterOffsetMask = (1UL << (9 + m_clusterSizeShift)) - 1;
  m_blocksPerCluster = 1 << params.clusterSizeShift;
  m_blocksPerFat = params.fatSize;
  m_rootDirEntryCount = 16*params.dirSize;
  m_rootDirStartBlock = 1 + params.fatSize;
  m_dataStartBlock = 1 + params.fatSize + params.dirSize;
  m_clusterCount = params.clusterCount;
  m_ramDev = dev; 
  m_volumeValid = true;
  return true;
}
//------------------------------------------------------------------------------
/** List directory contents.
 *
 * \param[in] pr Print stream that list will be written to. 
 * 
 * \param[in] flags The inclusive OR of
 *
 * LS_DATE - %Print file modification date
 *
 * LS_SIZE - %Print file size.
 */
void RamDisk::ls(Print* pr, uint8_t flags) {
  dir_t d;
  for (uint16_t index = 0; readDir(&d, &index, DIR_ATT_VOLUME_ID); index++) {
    // print file name with possible blank fill
    printDirName(pr, d, flags & (LS_DATE | LS_SIZE) ? 14 : 0);

    // print modify date/time if requested
    if (flags & LS_DATE) {
       printFatDate(pr, d.lastWriteDate);
       pr->write(' ');
       printFatTime(pr, d.lastWriteTime);
    }
    // print size if requested
    if (DIR_IS_FILE(&d) && (flags & LS_SIZE)) {
      pr->write(' ');
      pr->print(d.fileSize);
    }
    pr->println();
  }
}
//------------------------------------------------------------------------------
/**
 * Open a file by file name.
 *
 * \note The file must be in the root directory and must have a DOS
 * 8.3 name.
 *
 * \param[in] fileName A valid 8.3 DOS name for a file in the root directory.
 *
 * \param[in] oflag Values for \a oflag are constructed by a bitwise-inclusive
 *  OR of flags from the following list
 *
 * O_READ - Open for reading.
 *
 * O_RDONLY - Same as O_READ.
 *
 * O_WRITE - Open for writing.
 *
 * O_WRONLY - Same as O_WRITE.
 *
 * O_RDWR - Open for reading and writing.
 *
 * O_APPEND - If set, the file offset shall be set to the end of the
 * file prior to each write.
 *
 * O_CREAT - If the file exists, this flag has no effect except as noted
 * under O_EXCL below. Otherwise, the file shall be created
 *
 * O_EXCL - If O_CREAT and O_EXCL are set, open() shall fail if the file exists.
 *
 * O_SYNC - Call sync() after each write.  This flag should not be used with
 * write(uint8_t), write_P(PGM_P), writeln_P(PGM_P), or the Arduino Print class.
 * These functions do character a time writes so sync() will be called
 * after each byte.
 *
 * O_TRUNC - If the file exists and is a regular file, and the file is
 * successfully opened and is not read only, its length shall be truncated to 0.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include the FAT volume has not been initialized,
 * a file is already open, \a fileName is invalid, the file does not exist,
 * is a directory, or can't be opened in the access mode specified by oflag.
 */
bool RamDisk::open(const char* fileName, uint8_t oflag) {
  dir_t dir;
  uint8_t dname[11];   // name formated for dir entry
  int16_t empty = -1;  // index of empty slot

  if (!m_volumeValid || isOpen()) return false;

  // error if invalid name
  if (!make83Name(fileName, dname)) return false;

  for (uint16_t index = 0; index < m_rootDirEntryCount; index++) {
    if (!readDir(index, &dir)) return false;

    if (dir.name[0] == DIR_NAME_FREE || dir.name[0] == DIR_NAME_DELETED) {
      // remember first empty slot
      if (empty < 0) empty = index;
      // done if no entries follow
      if (dir.name[0] == DIR_NAME_FREE) break;
    } else if (!memcmp(dname, dir.name, 11)) {
      // don't open existing file if O_CREAT and O_EXCL
      if ((oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) return false;
      // open existing file
      m_dirEntryIndex = index;
      return open(&dir, oflag);
    }
  }    
  // error if directory is full
  if (empty < 0) return false;

  // only create file if O_CREAT and O_WRITE
  if ((oflag & (O_CREAT | O_WRITE)) != (O_CREAT | O_WRITE)) return false;

  // initialize as empty file
  memset(&dir, 0, sizeof(dir_t));
  memcpy(dir.name, dname, 11);
  m_dirEntryIndex = empty;

  // set timestamps
  if (m_dateTime) {
    // call user function
    m_dateTime(&dir.creationDate, &dir.creationTime);
  } else {
    // use default date/time
    dir.creationDate = FAT_DEFAULT_DATE;
    dir.creationTime = FAT_DEFAULT_TIME;
  }
  dir.lastAccessDate = dir.creationDate;
  dir.lastWriteDate = dir.creationDate;
  dir.lastWriteTime = dir.creationTime;

  // Write entry
  if (!writeDir(empty, &dir)) return false;

  // open entry
  return open(&dir, oflag);
}
//------------------------------------------------------------------------------
/**
 * Open a file by file index.
 *
 * \param[in] index The root directory index of the file to be opened.  See \link
 *  RamDisk::readDir() readDir()\endlink.
 *
 * \param[in] oflag  See \link RamDisk::open(const char*, uint8_t)\endlink.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include the FAT volume has not been initialized,
 * a file is already open, \a index is invalid or is not the index of a
 * file or the file cannot be opened in the access mode specified by oflag.
 */
bool RamDisk::open(uint16_t index, uint8_t oflag) {
  dir_t dir;
  if (!m_volumeValid || isOpen()) return false;
  
  if (!readDir(index, &dir)) return false;

  // error if unused entry
  if (dir.name[0] == DIR_NAME_FREE || dir.name[0] == DIR_NAME_DELETED) {
    return false;
  }
  m_dirEntryIndex = index;
  return open(&dir, oflag);
}
//------------------------------------------------------------------------------
bool RamDisk::open(dir_t* dir, uint8_t oflag) {

  if ((oflag & O_TRUNC) && !(oflag & O_WRITE)) return false;
  
  // error if long name, volume label or subdirectory
  if ((dir->attributes & (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY)) != 0) {
    return false;
  }
  // don't allow write or truncate if read-only
  if (dir->attributes & DIR_ATT_READ_ONLY) {
    if (oflag & (O_WRITE | O_TRUNC)) return false;
  }

  m_curCluster = 0;
  m_curPosition = 0;
  m_fileSize = dir->fileSize;
  m_firstCluster = dir->firstClusterLow;
  m_flags = oflag & (O_ACCMODE | O_SYNC | O_APPEND);

  if (oflag & O_TRUNC ) return truncate(0);
  return true;
}
//------------------------------------------------------------------------------
/** %Print the name field of a directory entry in 8.3 format.
 *
 * \param[in] pr Print stream that name will be written to.  
 * \param[in] dir The directory structure containing the name.
 * \param[in] width Blank fill name if length is less than \a width.
 */
void RamDisk::printDirName(Print* pr, const dir_t& dir, uint8_t width) {
  uint8_t w = 0;
  for (uint8_t i = 0; i < 11; i++) {
    if (dir.name[i] == ' ') continue;
    if (i == 8) {
      pr->write('.');
      w++;
    }
    pr->write(dir.name[i]);
    w++;
  }
  if (DIR_IS_SUBDIR(&dir)) {
    pr->write('/');
    w++;
  }
  while (w < width) {
    pr->write(' ');
    w++;
  }
}
//------------------------------------------------------------------------------
/** %Print a directory date field.
 *
 *  Format is yyyy-mm-dd.
 *
 * \param[in] pr Print stream that field will be written to.  
 * \param[in] fatDate The date field from a directory entry.
 */
void RamDisk::printFatDate(Print* pr, uint16_t fatDate) {
  pr->print(FAT_YEAR(fatDate));
  pr->write('-');
  printTwoDigits(pr, FAT_MONTH(fatDate));
  pr->write('-');
  printTwoDigits(pr, FAT_DAY(fatDate));
}
//------------------------------------------------------------------------------
/** %Print a directory time field.
 *
 * Format is hh:mm:ss.
 *
 * \param[in] pr Print stream that field will be written to.  
 * \param[in] fatTime The time field from a directory entry.
 */
void RamDisk::printFatTime(Print* pr, uint16_t fatTime) {
  printTwoDigits(pr, FAT_HOUR(fatTime));
  pr->write(':');
  printTwoDigits(pr, FAT_MINUTE(fatTime));
  pr->write(':');
  printTwoDigits(pr, FAT_SECOND(fatTime));
}

//------------------------------------------------------------------------------
/** %Print a value as two digits.
 *
 * \param[in] pr Print stream that value will be written to.  
 * \param[in] v Value to be printed, 0 <= \a v <= 99
 */
void RamDisk::printTwoDigits(Print* pr, uint8_t v) {
  char str[3];
  str[0] = '0' + v/10;
  str[1] = '0' + v % 10;
  str[2] = 0;
  pr->print(str);
}
//------------------------------------------------------------------------------
/**
 * Read the next byte from a file.
 *
 * \return For success read returns the next byte in the file as an int.
 * If an error occurs or end of file is reached -1 is returned.
 */
int16_t RamDisk::read(void) {
  uint8_t b;
  return read(&b, 1) == 1 ? b : -1;
}
//------------------------------------------------------------------------------
/**
 * Read data from a file at starting at the current file position.
 *
 * \param[out] buf Pointer to the location that will receive the data.
 *
 * \param[in] nbyte Maximum number of bytes to read.
 *
 * \return For success read returns the number of bytes read.
 * A value less than \a nbyte, including zero, may be returned
 * if end of file is reached.
 * If an error occurs, read returns -1.  Possible errors include
 * read called before a file has been opened, the file has not been opened in
 * read mode, a corrupt file system, or an I/O error.
 */
int RamDisk::read(void* buf, size_t nbyte) {
  // convert void pointer to uin8_t pointer
  uint8_t* dst = reinterpret_cast<uint8_t*>(buf);

  // error if not open for read
  if (!(m_flags & O_READ)) return -1;

  // don't read beyond end of file
  if ((m_curPosition + nbyte) > m_fileSize) nbyte = m_fileSize - m_curPosition;

  // bytes left to read in loop
  size_t nToRead = nbyte;
  while (nToRead > 0) {
    uint16_t clusterOffset = m_curPosition & m_clusterOffsetMask;
    if (clusterOffset == 0) {
      // start next cluster
      if (m_curCluster == 0) {
        m_curCluster = m_firstCluster;
      } else {
        if (!fatGet(m_curCluster, &m_curCluster)) return -1;
      }
      // return error if bad cluster chain
      if (m_curCluster < 2 || isEOC(m_curCluster)) return -1;
    }
    // Max space in cluster.
    uint16_t n = (512 << m_clusterSizeShift) - clusterOffset;

    // Lesser of space in cluster and amount to read.
    if (n > nToRead) n = nToRead;
    
    uint32_t addr = clusterAddress(m_curCluster) + clusterOffset;  
   if (!m_ramDev->read(addr, dst, n)) return -1;

    m_curPosition += n;
    dst += n;
    nToRead -= n;
  }
  return nbyte;
}
//------------------------------------------------------------------------------
// make public??? ////////////////////////////////////////////////////////////////////////////////
bool RamDisk::readDir(uint16_t index, dir_t* dir) {
  if (index >= m_rootDirEntryCount) return false;
  uint32_t addr = (m_rootDirStartBlock << 9) + (index << 5);
  return m_ramDev->read(addr, dir, sizeof(dir_t));
}
//------------------------------------------------------------------------------
/**
 *  Read the next short, 8.3, directory entry.
 *
 *  Unused entries and entries for long names are skipped.
 *
 * \param[out] dir Location that will receive the entry.
 *
 * \param[in,out] index The search starts at \a index and \a index is
 * updated with the root directory index of the found directory entry.
 * If the entry is a file, it may be opened by calling
 * \link RamDisk::open(uint16_t, uint8_t) \endlink.
 *
 * \param[in] skip Skip entries that have these attributes. If \a skip
 * is not specified, the default is to skip the volume label and directories.
 *
 * \return The value one, true, is returned for success and the value zero,
 * false, is returned if an error occurs or the end of the root directory is
 * reached.  On success, \a entry is set to the index of the found directory
 * entry.
 */
bool RamDisk::readDir(dir_t* dir, uint16_t* index, uint8_t skip) {
  dir_t tmp;
  for (uint16_t i = *index; ; i++) {
    if (i >= m_rootDirEntryCount) return false;
    if (!readDir(i, &tmp)) return false;

    // done if beyond last used entry
    if (tmp.name[0] == DIR_NAME_FREE) return false;

    // skip deleted entry
    if (tmp.name[0] == DIR_NAME_DELETED) continue;

    // skip long names
    if ((tmp.attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME) continue;

    // skip if attribute match
    if (tmp.attributes & skip) continue;

    // return found index
    *index = i;
    break;
  }
  memcpy(dir, &tmp, sizeof(dir_t));
  return true;
}
//------------------------------------------------------------------------------
/**
 * Remove a file.  The directory entry and all data for the file are deleted.
 *
 * \note This function should not be used to delete the 8.3 version of a
 * file that has a long name. For example if a file has the long name
 * "New Text Document.txt" you should not delete the 8.3 name "NEWTEX~1.TXT".
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include the file is not open for write
 * or an I/O error occurred.
 */
bool RamDisk::remove(void) {
  // error if file is not open for write
  if (!(m_flags & O_WRITE)) return false;
  if (m_firstCluster) {
    if (!freeChain(m_firstCluster)) return false;
  }
  dir_t dir;
  if (!readDir(m_dirEntryIndex, &dir)) return false;
  dir.name[0] = DIR_NAME_DELETED;
  m_flags = 0;
  return writeDir(m_dirEntryIndex, &dir);
}
//------------------------------------------------------------------------------
/**
 * Remove a file.
 *
 * The directory entry and all data for the file are deleted.
 *
 * \param[in] fileName The name of the file to be removed.
 *
 * \note This function should not be used to delete the 8.3 version of a
 * file that has a long name. For example if a file has the long name
 * "New Text Document.txt" you should not delete the 8.3 name "NEWTEX~1.TXT".
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include the file is read only, \a fileName is not found
 * or an I/O error occurred.
 */
bool RamDisk::remove(const char* fileName) {
  RamDisk file;
  if (!file.open(fileName, O_WRITE)) return false;
  return file.remove();
}
//------------------------------------------------------------------------------
/**
 * Sets the file's read/write position.
 *
 * \param[in] pos The new position in bytes from the beginning of the file.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
bool RamDisk::seekSet(uint32_t pos) {
  // error if file not open or seek past end of file
  if (!isOpen() || pos > m_fileSize) return false;
  if (pos == 0) {
    // set position to start of file
    m_curCluster = 0;
    m_curPosition = 0;
    return true;
  }
  fat_t n = ((pos - 1) >> 9) >> m_clusterSizeShift;
  if (pos < m_curPosition || m_curPosition == 0) {
    // must follow chain from first cluster
    m_curCluster = m_firstCluster;
  } else {
    // advance from curPosition
    n -= ((m_curPosition - 1) >> 9) >> m_clusterSizeShift;
  }
  while (n--) {
    if (!fatGet(m_curCluster, &m_curCluster)) return false;
  }
  m_curPosition = pos;
  return true;
}
//------------------------------------------------------------------------------
/**
 *  The sync() call causes all modified data and directory fields
 *  to be written to the storage device.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include a call to sync() before a file has been
 * opened or an I/O error.
 */
bool RamDisk::sync(void) {
  if (m_flags & F_FILE_DIR_DIRTY) {
    dir_t dir;
    // cache directory entry
    if (!readDir(m_dirEntryIndex, &dir)) return false;

    // update file size and first cluster
    dir.fileSize = m_fileSize;
    dir.firstClusterLow = m_firstCluster;

    // set modify time if user supplied a callback date/time function
    if (m_dateTime) {
      m_dateTime(&dir.lastWriteDate, &dir.lastWriteTime);
      dir.lastAccessDate = dir.lastWriteDate;
    }
    m_flags &= ~F_FILE_DIR_DIRTY;
    return writeDir(m_dirEntryIndex, &dir);
  }
  return true;
}
//------------------------------------------------------------------------------
/**
 * The timestamp() call sets a file's timestamps in its directory entry.
 *
 * \param[in] flags Values for \a flags are constructed by a bitwise-inclusive
 * OR of flags from the following list
 *
 * T_ACCESS - Set the file's last access date.
 *
 * T_CREATE - Set the file's creation date and time.
 *
 * T_WRITE - Set the file's last write/modification date and time.
 *
 * \param[in] year Valid range 1980 - 2107 inclusive.
 *
 * \param[in] month Valid range 1 - 12 inclusive.
 *
 * \param[in] day Valid range 1 - 31 inclusive.
 *
 * \param[in] hour Valid range 0 - 23 inclusive.
 *
 * \param[in] minute Valid range 0 - 59 inclusive.
 *
 * \param[in] second Valid range 0 - 59 inclusive
 *
 * \note It is possible to set an invalid date since there is no check for
 * the number of days in a month.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
bool RamDisk::timestamp(uint8_t flags, uint16_t year, uint8_t month,
         uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
  if (!isOpen()
    || year < 1980
    || year > 2107
    || month < 1
    || month > 12
    || day < 1
    || day > 31
    || hour > 23
    || minute > 59
    || second > 59) {
      return false;
  }
  dir_t dir;
  if (!readDir(m_dirEntryIndex, &dir)) return false;

  uint16_t dirDate = FAT_DATE(year, month, day);
  uint16_t dirTime = FAT_TIME(hour, minute, second);
  if (flags & T_ACCESS) {
    dir.lastAccessDate = dirDate;
  }
  if (flags & T_CREATE) {
    dir.creationDate = dirDate;
    dir.creationTime = dirTime;
    // seems to be units of 1/100 second not 1/10 as Microsoft standard states
    dir.creationTimeTenths = second & 1 ? 100 : 0;
  }
  if (flags & T_WRITE) {
    dir.lastWriteDate = dirDate;
    dir.lastWriteTime = dirTime;
  }
  return writeDir(m_dirEntryIndex, &dir);
}
//------------------------------------------------------------------------------
/**
 * Truncate a file to a specified length.  The current file position
 * will be maintained if it is less than or equal to \a length otherwise
 * it will be set to end of file.
 *
 * \param[in] length The desired length for the file.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include file is read only, file is a directory,
 * \a length is greater than the current file size or an I/O error occurs.
 */
bool RamDisk::truncate(uint32_t length) {
  // error if file is not open for write
  if (!(m_flags & O_WRITE)) return false;

  if (length > m_fileSize) return false;

  // fileSize and length are zero - nothing to do
  if (m_fileSize == 0) return true;
  uint32_t newPos = m_curPosition > length ? length : m_curPosition;
  if (length == 0) {
    // free all clusters
    if (!freeChain(m_firstCluster)) return false;
    m_curCluster = m_firstCluster = 0;
  } else {
    fat_t toFree;
    if (!seekSet(length)) return false;
    if (!fatGet(m_curCluster, &toFree)) return false;
    if (!isEOC(toFree)) {
      // free extra clusters
      if (!fatPut(m_curCluster, FAT16EOC)) return false;
      if (!freeChain(toFree)) return false;
    }
  }
  m_fileSize = length;
  m_flags |= F_FILE_DIR_DIRTY;
  if (!sync()) return false;
  return seekSet(newPos);
}
//------------------------------------------------------------------------------
/**
 * Write data at the current position of an open file.
 *
 * \note Data is moved to the cache but may not be written to the
 * storage device until sync() is called.
 *
 * \param[in] buf Pointer to the location of the data to be written.
 *
 * \param[in] nbyte Number of bytes to write.
 *
 * \return For success write() returns the number of bytes written, always
 * \a nbyte.  If an error occurs, write() returns -1.  Possible errors include
 * write() is called before a file has been opened, the file has not been opened
 * for write, device is full, a corrupt file system or an I/O error.
 *
 */
int RamDisk::write(const void* buf, size_t nbyte) {
  size_t nToWrite = nbyte;
  const uint8_t* src = reinterpret_cast<const uint8_t*>(buf);

  // error if file is not open for write
  if (!(m_flags & O_WRITE)) goto writeErrorReturn;

  // go to end of file if O_APPEND
  if ((m_flags & O_APPEND) && m_curPosition != m_fileSize) {
    if (!seekEnd()) goto writeErrorReturn;
  }
  while (nToWrite > 0) {
    uint16_t clusterOffset = m_curPosition & m_clusterOffsetMask;
    if (clusterOffset == 0) {
      // start of new cluster
      if (m_curCluster != 0) {
        fat_t next;
        if (!fatGet(m_curCluster, &next)) goto writeErrorReturn;
        if (isEOC(next)) {
 
          // add cluster if at end of chain
          if (!addCluster()) goto writeErrorReturn;
        } else {
          m_curCluster = next;
        }
      } else {
        if (m_firstCluster == 0) {
          // allocate first cluster of file
          if (!addCluster()) goto writeErrorReturn;
        } else {
          m_curCluster = m_firstCluster;
        }
      }
    }
    // Max space in cluster.
    uint16_t n = (512 << m_clusterSizeShift) - clusterOffset;

    // Lesser of space in cluster and amount to write.
    if (n > nToWrite) n = nToWrite;
    
    uint32_t addr = clusterAddress(m_curCluster) + clusterOffset;    
    if (!m_ramDev->write(addr, src, n)) goto writeErrorReturn;

    m_curPosition += n;
    nToWrite -= n;
    src += n;
  }
  if (m_curPosition > m_fileSize) {
    // update fileSize and insure sync will update dir entry
    m_fileSize = m_curPosition;
    m_flags |= F_FILE_DIR_DIRTY;
  } else if (m_dateTime && nbyte) {
    // insure sync will update modified date and time
    m_flags |= F_FILE_DIR_DIRTY;
  }

  if (m_flags & O_SYNC) {
    if (!sync()) goto writeErrorReturn;
  }
  return nbyte;

 writeErrorReturn:
  writeError = true;
  return -1;
}
//------------------------------------------------------------------------------
/**
 * Write a byte to a file. Required by the Arduino Print class.
 *
 * \param[in] b byte to be written.
 *
 * Use RamDisk::writeError to check for errors.
 * \return one for success else zero.
 */
size_t RamDisk::write(uint8_t b) {
  return write(&b, 1) == 1 ? 1 : 0;
}
//------------------------------------------------------------------------------
/**
 * Write a string to a file. Used by the Arduino Print class.
 *
 * \param[in] str string to be written.
 *
 * Use RamDisk::writeError to check for errors.
 * \return number of bytes written or -1 if an error occurs.
 */
int16_t RamDisk::write(const char* str) {
  return write(str, strlen(str));
}
//------------------------------------------------------------------------------
/**
 * Write a PROGMEM string to a file.
 *
 * \param[in] str PROGMEM string to be written.
 *
 * Use RamDisk::writeError to check for errors.
 */
void RamDisk::write_P(PGM_P str) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) write(c);
}
//------------------------------------------------------------------------------
bool RamDisk::writeDir(uint16_t index, dir_t* dir) {
  if (index >= m_rootDirEntryCount) return false;
  uint32_t addr = (m_rootDirStartBlock << 9) + (index << 5);
  return m_ramDev->write(addr, dir, sizeof(dir_t));
}
//------------------------------------------------------------------------------
/**
 * Write a PROGMEM string followed by CR/LF to a file.
 *
 * \param[in] str PROGMEM string to be written.
 *
 * Use RamDisk::writeError to check for errors.
 */
void RamDisk::writeln_P(PGM_P str) {
  write_P(str);
  println();
}
