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
  
  io2.pinMode(GPIO0 | GPIO1, OUT);
  checkError();
}

void checkError() {
  if (io2.getError() != IO2_SUCCESS) {
    Serial.println("MOD-IO2 Error");
    while (true);
  }
}

void loop () {
  Serial.println("GPIO High");
  io2.digitalWrite(GPIO0 | GPIO1, HI);
  checkError();

  delay(500);

  Serial.println("GPIO Low");
  io2.digitalWrite(GPIO0 | GPIO1, LO);
  checkError();

  delay(500);
}
