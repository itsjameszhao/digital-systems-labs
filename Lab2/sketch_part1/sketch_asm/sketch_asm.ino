#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.println("The sum of 255 and 255 is: ");
  Serial.println(testasm(255, 255));
  Serial.println("The sum of 1 and 0 is: ");
  Serial.println(testasm(1, 0));
  Serial.println("The sum of 10 and 5 is: ");
  Serial.println(testasm(10, 5));
  Serial.println("The sum of 124 and 165 is: ");
  Serial.println(testasm(124, 165));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
