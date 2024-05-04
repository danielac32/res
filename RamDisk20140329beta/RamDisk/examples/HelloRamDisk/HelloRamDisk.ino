#include <M23LCV1024.h>
#include <RamDisk.h>
const uint8_t RAM_CS_PIN = 9;

M23LCV1024 ram;
RamDisk file;
char buf[40];

void setup() {
  Serial.begin(9600);
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
  if (!file.open("TEST.TXT", O_CREAT | O_RDWR)) {
    Serial.println(F("open fail"));
    return;
  }
  file.println("Hello RamDisk!");
  file.println("Line to test fgets");
  file.rewind();
  int lineNumber = 0;
  // Read file line at a time.
  while (file.fgets(buf, sizeof(buf)) > 0) {
    Serial.print(++lineNumber);
    Serial.print(F(": "));
    Serial.print(buf);
  }
  file.close();
}
void loop() {
}
