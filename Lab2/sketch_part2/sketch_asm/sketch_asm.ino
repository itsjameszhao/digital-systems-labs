#include "font5x7.h"

//int state;
//
//void setup() {
//  // put your setup code here, to run once
//  Serial.begin(9600);
//  Serial.println("Hello");
//  //pinMode(1, INPUT); 
////
//  int input[] = {8, 9, 10, 11, 12};
//  int output[] = {1,2,3,4,5,6,7};
//  for (int i = 0; i<7; i++) {
//    DDRD |= (1 << output[i]);
//  }
//  for (int i = 0; i<5; i++) {
//    DDRD |= (1 << input[i]);
//    PORTD &= (0 << input[i]);
//  }
//  PORTD |= (1<<2);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  //digitalWrite(2, HIGH);
//   PORTD |= (1<<2);
//
//  delay(10);
//  
//}

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
  // initialize digital pin LED_BUILTIN as an output.
  for(int i = 1; i <= 12; i++){
    pin_mode_high(i);
    //pinMode(i, OUTPUT);
  }
}

void display(int row, int col) {
  // set all column pins to HIGH
  for(int i = 1; i<=5; i++){
    digital_write(i, HIGH);
    //digitalWrite(i, HIGH);
  }

  // set all row pins to LOW
  for(int i = 6; i<=12; i++){
    digital_write(i, LOW);
    //digitalWrite(i, LOW);
  }

  // turn on the specified LED
  digital_write(col, LOW);
  digital_write(row + 5, HIGH);
  //digitalWrite(col, LOW);
  //digitalWrite(row + 5, HIGH);
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

void display_ascii_duration(int ascii_num, int microseconds){
  //millis()?
}


// the loop function runs over and over again forever
void loop() {
  display_ascii(49);
}
