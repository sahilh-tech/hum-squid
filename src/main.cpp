#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, world!");
}

void loop() {
  delay(1000);
  Serial.println("Hello, world!");
}
