#include "font5x7.h"

int state;

void pin_mode_high(int pin){
  if(pin < 8) {
    DDRD |= (1 << pin);
  } else {
    DDRB |= (1 << (pin - 8));
  }
}

void digital_write(int pin, int high){
  if(pin < 8) {
    if(high == HIGH){
      PORTD |= (1 << pin);
    } else {
      PORTD &= ~(1 << pin);
    }
  } else {
    if(high == HIGH){
      PORTB |= (1 << (pin - 8));
    } else {
      PORTB &= ~(1 << (pin - 8));
    }
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  //Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  for(int i = 1; i <= 12; i++){
    if (i != 2) {
      pin_mode_high(i);
    } else {
      pin_mode_high(13);
    }
  }
  // put your setup code here, to run once
  
  //Serial.begin(9600);
  DDRD &= ~(1 << 2);
  PORTD |= (1 << 2);
  state = 0;
  attachInterrupt(digitalPinToInterrupt(2), incrementInterrupt, RISING);
  
}

void display(int row, int col) {
  // set all column pins to HIGH
  for(int i = 1; i<=5; i++){
    if (i != 2) {
      digital_write(i, HIGH);
    }
    else {
      digital_write(13, HIGH);
    }
  }

  // set all row pins to LOW
  for(int i = 6; i<=12; i++){
    digital_write(i, LOW);
  }

  // turn on the specified LED
  if (col != 2) {
    digital_write(col, LOW);
  }
  else {
    digital_write(13, LOW);
  }
  digital_write(row + 5, HIGH);
}

void display_ascii(int ascii_num) {
  int start_index = (ascii_num - 32) * 5; // 32 is the ascii value of ' ', the first printable character in Font5x7
  for (int col = 1; col <= 5; col++) {
    int font_data = pgm_read_byte(&Font5x7[start_index + col - 1]);
    for (int row = 1; row <= 7; row++) {
      if (font_data & (1 << (row - 1))) {
        display(row, 6 - col);
        delayMicroseconds(400); 
      }
    }
  }
}

int current_digit = 0;

// the loop function runs over and over again forever
void loop() {
  display_ascii(48 + current_digit);

  // put your main code here, to run repeatedly:
  /*
  int newstate = PIND & (1 << 2);
  if (state != newstate) {
    Serial.println("The PIND changed to :");
    Serial.println(newstate);
    state = newstate;
  }*/
  
  //delay(100);
}


void incrementInterrupt(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {
   current_digit = (current_digit + 1) % 10;
  }
  last_interrupt_time = interrupt_time;
}
