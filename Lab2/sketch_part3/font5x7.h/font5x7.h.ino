#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);

  Serial.println("testasm TESTS BELOW");
  Serial.println("The testasm  of 255 and 1 is: ");
  Serial.println(testasm(255, 1));
  Serial.println("The testasm  of 1 and 255 is: ");
  Serial.println(testasm(1, 255));
  Serial.println("The testasm  of 255 and 255 is: ");
  Serial.println(testasm(255, 255));
  Serial.println("The testasm  of 2 and 3 is: ");
  Serial.println(testasm(2, 3));
  Serial.println("The testasm  of 3 and 2 is: ");
  Serial.println(testasm(3, 2));
  Serial.println("The testasm  of 10 and 10 is: ");
  Serial.println(testasm(10, 10));

  Serial.println("sumval TESTS BELOW");
  Serial.println("The sum  of 255 and 1 is: ");
  Serial.println(sumval(255, 1));


  Serial.println("diffval TESTS BELOW");
  Serial.println("The absolute difference  of 255 and 1 is: ");
  Serial.println(diffval(255, 1));
  Serial.println("The absolute difference  of 1 and 255 is: ");
  Serial.println(diffval(1, 255));
  Serial.println("The absolute difference  of 2 and 2 is: ");
  Serial.println(diffval(2, 2));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
