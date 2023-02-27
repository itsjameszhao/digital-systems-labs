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



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

void display(int row, int col) {
  // set all column pins to HIGH
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);

  // set all row pins to LOW
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);

  // turn on the specified LED
  digitalWrite(col, LOW);
  digitalWrite(row + 5, HIGH);
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
  display_ascii(82);
}
