#include <Arduino.h>

#define LED PC13


void setup() {
  pinMode(LED, 0x1);
}

void loop() {
  digitalWrite(LED, 0x1);
  delay(500);
  digitalWrite(LED, 0x0);
  delay(500);
}