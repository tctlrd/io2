#include <IO2.h>

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

  Serial.println();
  Serial.println("NOTE:This functionality is not available for boards revision C or later");
}

void loop() {
  uint8_t value;
  // possible values for the DAC on GPIO2 are from 0x00 to 0x1f)
  for (value=0; value<0x20; value++) {
    io2.setGPIO2_DAC(value);
    delay(2000);
  }
}
