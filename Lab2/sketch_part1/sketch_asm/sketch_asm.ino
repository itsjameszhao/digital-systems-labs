int state;

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.println("The sum of 255 and 255 is: ");
  DDRD &= ~(1 << 2);
  PORTD |= (1 << 2);
  //pinMode (13, OUTPUT);
   

  state = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  int newstate = PIND & (1 << 2);
  if (state != newstate) {
    Serial.println("The PIND changed to :");
    Serial.println(newstate);
    state = newstate;
  }
  
  delay(100);
  
}
