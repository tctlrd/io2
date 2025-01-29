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
  Serial.print("Start.. ");
}

void checkError() {
  if (io2.getError() != IO2_SUCCESS) {
    Serial.println("MOD-IO2 Error");
    while (true);
  }
}

void loop () {
  static uint8_t count = 0;
  static uint8_t duty = 0;
  static int8_t step = 1;

  if (count == 5) {
    io2.pwmOff(PWM1);
    checkError();
    Serial.println("Done");
    while (true);
  }

  io2.pwmSet(PWM1, duty);
  checkError();

  delay(5);

  duty += step;

  if (duty == 0xFF) {
    step = -1;
  } else if (duty == 0x00) {
    step = 1;
    count++;
    Serial.print(count);
    Serial.print(" ");
  }
}
