const int photocellPin = A0;
const int redPin = 9;
const int greenPin = 11; 
const int bluePin = 10;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read the photocell value
  int photocellValue = analogRead(photocellPin);

  // Convert and display the photocell value
  int mappedPhotocellValue = map(photocellValue, 0, 1023, 0, 255);
  Serial.println(photocellValue);
  Serial.println(mappedPhotocellValue);
  
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, mappedPhotocellValue);
  
  Serial.println(photocellValue);
  delay(1000);
}
