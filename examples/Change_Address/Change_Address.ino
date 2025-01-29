#include <IO2.h>

// New address must be valid I2C address between 0x01 and 0x7F
#define NEW_ADDRESS 0x30

IO2 io2 = IO2();

void waitKey(char * msg) {
  Serial.print("Press Enter to ");
  Serial.println(msg);
  while(Serial.available() == 0);
  Serial.read();
}

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

  info();
}

void loop () {
  // Set new address
  waitKey("set new MOD-IO2 address");

  io2.setAddress(NEW_ADDRESS);
  if (io2.getError() != IO2_SUCCESS) {
	Serial.println();
    Serial.println("ERROR");
    Serial.println("Check if PGM1 is closed");
	Serial.println();
  } else {
    info();

    // Restore default address
    waitKey("restore default MOD-IO2 address");
    io2.setAddress(DEFAULT_ADDRESS);
    info();
  }

  waitKey("continue");
}
