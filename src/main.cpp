#include <Arduino.h>

void setup() {



}

void loop() {
  pinMode(4, INPUT);
  delay(500);
  pinMode(4, OUTPUT);
  delay(50000);
}