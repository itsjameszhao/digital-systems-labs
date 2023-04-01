#define OLED_CS 8
#define OLED_DC 4 
#define OLED_RST 5
#define OLED_SI 6
#define OLED_CLK 7
#include <Adafruit_SSD1306.h>


// ENCODER CODE GREATLY INSPIRED BY: https://forum.arduino.cc/t/trouble-reading-rotary-encoder/52571
// OLED PONG CODE GREATLY INSPIRED BY: https://github.com/makeabilitylab/arduino/blob/master/OLED/BallBounce/BallBounce.ino

const int encoder0PinA = 2;
const int encoder0PinB = 3;

// Used for velocity
// values (128, 255] => positive velocity, larger is greater magnitude
// values [0, 128] => negative velocity, smaller is greater magnitude
// i.e. absolute difference between encoder0Pos and 128 determines magnitude
const int MIN_VAL = 0;
const int MAX_VAL = 255;
int encoder0Pos = 128;

static unsigned long last_interrupt_time = 0;
static unsigned long ENCODER_DELAY_FILTER = 100;

// OLED setup
Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);
int WIDTH;
int HEIGHT;

// Ball variables
const int ballRadius = 4;
// Position of ball
int xBall = 0;
int yBall = 0;
// Direction of ball
int xDir = 1;
int yDir = 1;
const int DELAY_LOOP_MS = 5;
int MAX_VELOCITY = 8;

void setup() {
  // For rotary
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  attachInterrupt(0, encoderAInterrupt, CHANGE); 
  attachInterrupt(1, encoderBInterrupt, CHANGE);
  Serial.begin(9600);
  Serial.println ("start reading encoder");
  Serial.println (encoder0Pos, DEC);

  // For OLED
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
  disp.display();
  WIDTH = disp.width()
  HEIGHT = disp.height()

  // Initialize ball to the center of the screeen
  xBall = WIDTH / 2;
  yBall = HEIGHT / 2;

}

void loop() {
  disp.clearDisplay();
  // Update velocity based on encoder0Pos
  // Every turns to the encoder changes speed by one, capped at magnitude of MAX_VELOCITY
  int ballVelocity = (encoder0Pos - 128);
  // add a maximum speed
  if(ballVelocity < 0){
    ballVelocity = max(ballVelocity, MAX_VELOCITY * -1);
  } else {
    ballVelocity = min(ballVelocity, MAX_VELOCITY);
  }

  // little bonus if you made it all the way to 0 or 255: 50% speed boost for your efforts
  if(encoder0Pos == 0 || encoder0Pos == 255) {
    if(ballVelocity < 0){
        ballVelocity -= (MAX_VELOCITY / 2)
    } else {
        ballVelocity += (MAX_VELOCITY / 2)
    }
  }

  // Using velocity and directions, update position of ball
  xBall += xDir * ballVelocity;
  yBall += yDir * ballVelocity;

  // Check for ball bounce: xBall and yBall represent the center coordinates of the ball
  // check that center +/- radius does not come into contact with boundary, if it does, reverse directions (xDir & yDir),
  // not encoder0Pos
  if(xBall - ballRadius <= 0 || xBall + ballRadius >= WIDTH){
    xDir = xDir * -1;
  } 
  if(yBall - ballRadius <= 0 || yBall + ballRadius >= HEIGHT){
    yDir = yDir * -1;
  }

  // Draw circle
  disp.drawCircle(xBall, yBall, ballRadius, SSD1306_WHITE);
  
  // Render buffer to screen
  disp.display();

  if(DELAY_LOOP_MS > 0){
    delay(DELAY_LOOP_MS);
  }
}

// using interrupts allows us to be responsive to rotations
void encoderAInterrupt(){
  // Filter out spurious interrupts
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > ENCODER_DELAY_FILTER) {
   doEncoderA();
  }
  last_interrupt_time = interrupt_time;
}

void encoderBInterrupt(){
  // Filter out spurious interrupts
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > ENCODER_DELAY_FILTER) {
   doEncoderB();
  }
  last_interrupt_time = interrupt_time;
}

void doEncoderA(){ 
  // Simply increment the encoder when we interrupt
  encoder0Pos = min(MAX_VAL, encoder0Pos + 1);
  Serial.println(encoder0Pos);
} 

void doEncoderB(){
  // Simply decrement the encoder when we interrupt
  encoder0Pos = max(MIN_VAL, encoder0Pos - 1);
  Serial.println(encoder0Pos);
}
