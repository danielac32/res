This is a very early version of a RamDisk library.  It provides a FAT style
file system on an external RAM device.
 
RamDisk is incomplete and likely has bugs.  I am releasing this version so
future users can provide comments and input.

Please post any comments in the Arduino forum topic where this file
was posted.

This library was developed using a Microchip 23LCV1024 1 Mbit SPI
serial SRAM with battery backup.

You must have working 23LCV1024 or 23LC1024 hardware or write your own
RAM device library.  You should be able to use other RAM devices by
writing a library with the RamBaseClass API.

Please read the html documentation. Go to the Classes tab and look
at the RamDisk and RamBaseClass.

You must install three libraries to try the examples included with RamDisk.

The RamDisk folder contains file-system code and three examples.

The 23LCV1024 folder contains the raw SRAM driver.

The SdFat folder contains a version of SdFat that can coexist with RamDisk.

There are three examples that demonstrate RamDisk features.

Start with HelloRamDisk.ino.  This example writes two lines of text to
a file and then reads the file and displays the text on the serial monitor.

Here is the output from HelloRamDisk:

Format (Y/N): N
1: Hello RamDisk!
2: Line to test fgets


RamDiskLoggerTest.ino will log 12,000 samples to RAM as fast as possible.
each sample consists of value from five analog pins.  A total of 60,000
values are written to a binary file.  Timing statistics and the first ten
samples in the log file are printed.

Here is output with pins 0, 2, 4 grounded. Pin 1 is 5 V and pin 3 is 3.3 V.

Format (Y/N): Y
clusterCount: 250
Starting test
t: 7.53 sec
minMicros: 60
maxMicros: 216
avgMicros: 66
sync micros: 156
filesize: 120000

pin0,pin1,pin2,pin3,pin4
0,1023,0,676,0
0,1023,0,676,0
0,1023,0,676,0
0,1023,0,676,0
0,1023,0,676,0
0,1023,0,676,0
0,1023,0,676,0
0,1023,0,676,0
0,1023,0,676,0
0,1023,0,676,0
Done


RamDiskToSdFat.ino creates a file with 1000 text lines in RAM.  It then
copies the text file to an SD file.

Here is the output from RamDiskToSdFat:

FreeRam: 1092
Format (Y/N): Y
Writing ramFile
TEST.CSV      2000-01-01 01:00:00 16746
Copying ramFile to sdFile
BENCH.DAT      2000-01-01 01:00:00 4999680
TEST.TXT       2000-01-01 01:00:00 16743
TEST.CSV       2000-01-01 01:00:00 16746
Done






