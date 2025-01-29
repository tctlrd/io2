#include <IO2.h>

#define ANALOG_CHANNEL  AN0
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
  uint16_t adc = io2.analogRead(ANALOG_CHANNEL);
  checkError();

  Serial.print("Voltage = "); Serial.println(adc * 3.3 / 1023.0);
  delay(1000);
}
