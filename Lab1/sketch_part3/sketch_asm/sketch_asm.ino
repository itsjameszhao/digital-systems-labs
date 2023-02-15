#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.println("Fib(0) is");
  Serial.println(fib(0));
  Serial.println("Fib(1) is");
  Serial.println(fib(1));
  Serial.println("Fib(2) is");
  Serial.println(fib(2));
  Serial.println("Fib(3) is");
  Serial.println(fib(3));
  Serial.println("Fib(4) is");
  Serial.println(fib(4));
  Serial.println("Fib(10) is");
  Serial.println(fib(10));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
