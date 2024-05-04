// Create a text fine on the RamDisk an then copy the file to an SD.
// Warning this requires a new test version of SdFat.
#include <SdFat.h>
#include <SdFatUtil.h>
#include <M23LCV1024.h>
#include <RamDisk.h>

#define FILENAME "TEST.CSV"
const uint8_t RAM_CS_PIN = 9;
const uint8_t SD_CS_PIN = SS;

SdFat sd;
SdBaseFile sdFile;
M23LCV1024 ram;
RamDisk ramFile;
char buf[40];

void setup() {
  Serial.begin(9600);
  Serial.print(F("FreeRam: "));
  Serial.println(FreeRam());
  if (!sd.begin(SD_CS_PIN)) sd.errorHalt();
  // Initialize RAM 
  ram.begin(RAM_CS_PIN);

  Serial.print(F("Format (Y/N): "));
  while (!Serial.available());
  char c = toupper(Serial.read());
  Serial.println(c);

  if (c == 'Y') {
    // totalBlocks: 256
    // dirBlocks: 4  (64 entries)
    // clusterSizeBlocks: 1 (one 512 byte block per cluster)
    if (!RamDisk::format(&ram, 256, 4, 1)) {
      Serial.println(F("format fail"));
      return;
    }
  } else if (c != 'N') {
    Serial.println(F("Invalid entry"));
    return;
  }
  if (!RamDisk::init(&ram)) {
    Serial.println(F("init fail"));
    return;
  }
  // Remove old version.
  RamDisk::remove(FILENAME);
  if (!ramFile.open(FILENAME, O_CREAT | O_RDWR)) {
    Serial.println(F("open fail"));
    return;
  }
  Serial.println(F("Writing ramFile"));
  uint32_t m0 = micros();
  for (int i = 0; i < 1000; i++) {
    ramFile.print(micros() - m0);
    ramFile.write(",Line,");
    ramFile.println(i);
  }
  // like closing and opening file (need to update dir for ls).
  ramFile.sync();
  ramFile.rewind();
  
  RamDisk::ls(&Serial, LS_DATE | LS_SIZE);

  if (!sdFile.open(FILENAME, O_CREAT | O_RDWR | O_TRUNC)) {
    Serial.println(F("sdFile.open failed"));
    return;
  }
  Serial.println(F("Copying ramFile to sdFile"));
  int n;
  while ((n = ramFile.read(buf, sizeof(buf))) > 0) {
    if (sdFile.write(buf, n) != n) {
      Serial.println(F("sdFile.write failed"));
      return;
    }
  }
  ramFile.close();
  sdFile.close();
  sd.ls(&Serial, LS_DATE | LS_SIZE);

  Serial.println(F("Done"));
}
void loop() {}