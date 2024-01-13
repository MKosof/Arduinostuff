#include <LiquidCrystal.h>

const uint d1=5,d2=4;

void setup() {
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);

}

void loop() {
  digitalWrite(d1, 1);
  delay(1);
  digitalWrite(d1, 0);
  digitalWrite(d2,1);
  delay(1);
  digitalWrite(d2,0);
  delay(498);

}
