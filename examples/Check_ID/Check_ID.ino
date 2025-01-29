#include <IO2.h>

// Control MOD-IO2 at address 0x21
IO2 io2 = IO2();

void info() {
  Serial.println();
  Serial.print("MOD-IO2 address is 0x"); Serial.println(io2.getAddress(), HEX);
  Serial.print("BoardID: 0x"); Serial.println(io2.readID(), HEX);
  Serial.print("FW Version: 0x"); Serial.println(io2.readRevision(), HEX);
  Serial.println();
}

void setup () {
  delay(500);
  Serial.begin(115200);
  delay(500);
  Serial.println();

  io2.readID();
  if (io2.getError() != IO2_SUCCESS) {
    Serial.println("MOD-IO2 is not responding at default I2C address");
    Serial.print("Detect... ");
    io2.detect();
    if (io2.getError() == IO2_NOT_FOUND) {
      Serial.println("MOD-IO2 not found");
      while (true);
    }
    Serial.print("located at 0x"); Serial.println(io2.getAddress(), HEX);
  }
}

void loop () {
  info();
  delay (1000);
}
