#include <IO2.h>

// Control MOD-IO2 at address 0x21
IO2 io2 = IO2();

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

void checkError() {
  if (io2.getError() != IO2_SUCCESS) {
    Serial.println("MOD-IO2 Error");
    while (true);
  }
}

void loop () {
  Serial.println("Relay1 ON    Relay2 OFF");
  io2.setRelay(RELAY1, ON);
  checkError();
  io2.setRelay(RELAY2, OFF);  
  checkError();

  delay(3000);

  Serial.println("Relay1 OFF   Relay2 ON");
  io2.setRelay(RELAY2, ON);
  checkError();
  io2.setRelay(RELAY1, OFF);  
  checkError();

  delay(3000);
}
