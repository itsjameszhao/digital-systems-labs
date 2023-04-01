const int photocellPin = A0;
const int redPin = 9;
const int greenPin = 11; 
const int bluePin = 10;

void setup() {
  Serial.begin(9600);
}


// analogRead using IO mapped registers:
// references from: https://maker.pro/custom/tutorial/how-to-take-analog-readings-with-an-avr-microcontroller
// page 250-252 from: https://csl.yale.edu/~rajit/classes/eeng348/files/ATMega328.pdf
/*

  to have analog reads without explicitly calling analogRead, we can use the ADC and associated registers
  the process goes as such:
  we modify ADMUX register to configure the ADC for our reading
  
  set the ADEN bit in the ADCSRA register to enable the ADC

  with the ADC enabled, we can start a read by setting the ADSC bit of
  ADCSRA to a 1

  the read is ready when this bit turns into a 0, so we can keep polling it until that condition is met

  once zeroed, a read is ready, so we read the registers

  to read, we read register ADCH

*/

void loop() {
  // Read the photocell value
  int photocellValue = analogRead(photocellPin);

  // Convert and display the photocell value
  int mappedPhotocellValue = map(photocellValue, 0, 1023, 0, 255);
  Serial.println(photocellValue);
  Serial.println(mappedPhotocellValue);

  // Write the photocell value
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, mappedPhotocellValue);
  
  Serial.println(photocellValue);
  delay(1000);
}
