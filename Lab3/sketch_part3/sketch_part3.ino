#define OLED_CS 8
#define OLED_DC 4 
#define OLED_RST 5
#define OLED_SI 6
#define OLED_CLK 7
#include <Adafruit_SSD1306.h>

const int encoder0PinA = 2;
const int encoder0PinB = 3;
const int MIN_VAL = 0;
const int MAX_VAL = 255;
static unsigned long last_interrupt_time = 0;
static unsigned long ENCODER_DELAY_FILTER = 100;
int encoder0Pos = 128;

Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);

// Ball variables
const int _ballRadius = 4;
int _xBall = 0;
int _yBall = 0;
int _xDir = 1;
int _yDir = 1;
const int DELAY_LOOP_MS = 5;

void setup() {
  // For rotary
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  attachInterrupt(0, encoderAInterrupt, CHANGE); 
  attachInterrupt(1, encoderBInterrupt, CHANGE);
  Serial.begin(9600);
  Serial.println ("start reading encoder");
  Serial.println (encoder0Pos, DEC);

  // For LED
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
  disp.display();

  // Initialize ball to the center of the screeen
  _xBall = disp.width() / 2;
  _yBall = disp.height() / 2;

}

void loop() {
  disp.clearDisplay();
  // Update ball based on speed location

  int ballVelocity = (encoder0Pos - 128);
  
  _xBall += _xDir * ballVelocity;
  _yBall += _yDir * ballVelocity;

  // Check for ball bounce
  if(_xBall - _ballRadius <= 0 || _xBall + _ballRadius >= disp.width()){
    _xDir = _xDir * -1; // reverse x direction
  }
  
  if(_yBall - _ballRadius <= 0 || _yBall + _ballRadius >= disp.height()){
    _yDir = _yDir * -1; // reverse y direction
  }

  // Draw circle
  disp.drawCircle(_xBall, _yBall, _ballRadius, SSD1306_WHITE);
  
  // Render buffer to screen
  disp.display();

  if(DELAY_LOOP_MS > 0){
    delay(DELAY_LOOP_MS);
  }
}

void tryIncrementEncoder() {
  encoder0Pos = min(MAX_VAL, encoder0Pos + 1);
  Serial.println(encoder0Pos);
}

void tryDecrementEncoder() {
  encoder0Pos = max(MIN_VAL, encoder0Pos - 1);
  Serial.println(encoder0Pos);
}

void encoderAInterrupt(){
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > ENCODER_DELAY_FILTER) {
   doEncoderA();
  }
  last_interrupt_time = interrupt_time;
}

void encoderBInterrupt(){
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > ENCODER_DELAY_FILTER) {
   doEncoderB();
  }
  last_interrupt_time = interrupt_time;
}

void doEncoderA(){ 
  tryIncrementEncoder();
} 

void doEncoderB(){
  tryDecrementEncoder();
  //Serial.println (encoder0Pos, DEC);         // use for debugging - remember to comment out
}
