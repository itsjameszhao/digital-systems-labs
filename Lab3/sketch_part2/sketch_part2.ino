const int photocellPin = A0;
const int redPin = 9;
const int greenPin = 11; 
const int bluePin = 10;
const int echoPin = 12;
const int trigPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {
  // Read from Ultrasonic
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  int mappedDistance = map(distance, 0, 16, 0, 255);
  Serial.print("Distance is: ");
  Serial.println(distance);
  
  // Read from Photocell:
  int photocellValue = analogRead(photocellPin);
  int mappedPhotocellValue = map(photocellValue, 0, 1023, 0, 255);
  Serial.print("Mapped photocell value is: ");
  Serial.println(mappedPhotocellValue);
  Serial.print("Photocell value is: ");
  Serial.println(photocellValue);
  Serial.print("HIGh: ");
  Serial.println(HIGH);
  
  
  analogWrite(greenPin, 0);
  if (mappedDistance < 128) {
    analogWrite(greenPin, 0);
    analogWrite(redPin, 0);
    analogWrite(bluePin, mappedPhotocellValue);
  }
  else {
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(redPin, mappedPhotocellValue);
  }
  
  Serial.println(photocellValue);
  delay(1000);
}
