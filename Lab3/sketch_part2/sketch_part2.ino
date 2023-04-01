// based on provided example: https://www.instructables.com/Simple-Arduino-and-HC-SR04-Example/
// color changes at MAX_DISTANCE/2
// color value is static: either entirely green or red, determined by threshold mentioned above
// brightness control imported from part 1

const int photocellPin = A0;
const int redPin = 9;
const int greenPin = 11; 
const int bluePin = 10;
const int echoPin = 12;
const int trigPin = 13;
const int MAX_DISTANCE = 10;

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

  // in the code below, map could return values outside of the range (0, 255) but 
  // this does not cause unwanted behavior
  // edit: added floor and ceiling
  distance = duration / 58.2;
  if(distance > MAX_DISTANCE) {
    distance = MAX_DISTANCE;
  } else {
    distance = max(0, distance);
  }
  int mappedDistance = map(distance, 0, MAX_DISTANCE, 0, 255);
  
  // Read from Photocell:
  int photocellValue = analogRead(photocellPin);
  int mappedPhotocellValue = map(photocellValue, 0, 1023, 0, 255);
  
  if (mappedDistance < 128) {
    analogWrite(greenPin, mappedPhotocellValue);
    analogWrite(redPin, 0);
    analogWrite(bluePin, 0);
  }
  else {
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(redPin, mappedPhotocellValue);
  }
  
  delay(10);
}
