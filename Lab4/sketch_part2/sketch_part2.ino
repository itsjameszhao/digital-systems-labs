#include "concurrency.h"
#include <Adafruit_SSD1306.h>

#define RED_PIN 3
#define BLUE_PIN 2
#define GREEN_PIN 12
#define OLED_CS 8
#define OLED_DC 4 
#define OLED_RST 5
#define OLED_SI 6
#define OLED_CLK 7

/*
    In this sketch, we import the concurrency code from part 1. We also implement mutual exclusion locks.
    The general strategy for our locks is as follows: a lock has one field, held, that indicates whether
    someone holds the lock or not. In order to read the value of this field, a process must disable interrupts
    in order to ensure they are the only process reading this value. If they see the lock is held, they activate interrupts
    and yield their turn. They keep doing this until they acquire the lock, i.e., trying to acquire a lock blocks
    execution. If a process sees the lock is not held, they set the field, and reenable interrupts, and continue execution.

    We provide two tests for this. Both of our tests are based on the OLED display, since access to this must be exclusive
    to correctly display. The first test is a basic display of the process that currently holds the lock. If they hold the lock,
    they will display their `pid`. In these tests, p1 and p2, after a process calls lock_release, we make the processes sleep to
    provide some sense of fairness (otherwise, the same process will usually reaquire the lock immediately, there is nothing
    wrong with this, but its visually more pleasing to see processes alternate). We can add some fairness to locks
    by adding a queue structure to the lock, such that on attempting to acquire, a process can add themselves to the queue,
    and the `owner` of the lock can be stored as a field on the lock. However, this was not a requirement of our lock
    per the specifications of the lab. When this sketch is ran with only p1 and p2, the OLED will alternate showing 
    the numbers 1 and 2, 10 times each, until the program terminates.

    In the second test, we import and modify our code from lab3. To run this test, only p3 and p4 should be created and ran.
    In this test, each process will draw a ball on the OLED display, and control its motion to make it appear that 
    there are two balls bouncing around the screen. The way our code is written, one ball will move about twice as fast as the
    other, because we make one process sleep for twice as long as the other. It is also possible to have them move at the
    same speed, however, we preferred our current version because we saw it as a good demonstration of the differences in
    processing speeds of different process. Although both processes here run at the same speed, it can serve as a useful
    demonstration.

    One final note on our implementation, on acquiring and releasing a lock, we call a function cur_pid, that simply
    indicates which process currently holds the lock. This was useful for debugging and visualizing the runtime processing
    of our sketch. Our locks still work without this, so it is entirely optional.
*/


struct process_state{
  unsigned int sp;
  struct process_state *next;
  int started;
  int pid;
};


// OLED setup
Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);
int WIDTH = disp.width();
int HEIGHT = disp.height();

// Ball variables
const int ballRadius = 4;
// Position of ball
int xBall_1 = WIDTH / 2;
int yBall_1 = HEIGHT / 2;
// Direction of ball
int xDir_1 = 1;
int yDir_1 = 1;
// Position of ball
int xBall_2 = WIDTH / 2;
int yBall_2 = HEIGHT / 2;
// Direction of ball
int xDir_2 = -1;
int yDir_2 = -1;
int ballVelocity = 4;

const int DELAY_LOOP_MS = 5;


// global variable to assign process ids
int process_counter = 10;

// parameter to control the interval
int N = 5;

// global variable indicating the head of the process list
process_t *current_process = NULL;

// global variable, temporary placeholder for the head of the process list
process_t *temp_current_process = NULL;

// global variable indicating the tail of the process list
process_t *last_process;
const int NUM_DATA = 50;
int logging_array[NUM_DATA]; // value of the current process
int logging_index; //


// our own delay function
void delay_millis(int duration) {
  int end_time = millis() + duration;
  while(millis() < end_time) {
    ;
  }
}

lock_t *l; // The global lock

struct lock_state {
  int held;  
};

void lock_init(lock_t *l) {
    l->held = 0;
}

void lock_acquire(lock_t *l){ 
  noInterrupts();
  while(l->held != 0) {
    interrupts();
    noInterrupts();
  }
  l->held = 1;
  cur_pid(HIGH);
  interrupts();  
}

void lock_release(lock_t *l) {
  noInterrupts();
  l->held = 0;
  cur_pid(LOW);
  interrupts();  
}

void cur_pid(int high_low){
  if (current_process->pid == 10) {
    digitalWrite(BLUE_PIN, high_low);
  }
  else if (current_process->pid == 11) {
    digitalWrite(GREEN_PIN, high_low);
  }
}

// insert a new process struct at the end of the process list, or if there isn't anything just make it the head
void insert_at_tail(process_t *ps) {
  if (temp_current_process == NULL) {
    temp_current_process = ps;
    last_process = ps;
  }
  else {
    last_process->next = ps;
    last_process = ps;

  }
}

int process_create(void (*f) (void), int n) {
  noInterrupts();
  unsigned int sp = process_init(f, n);
  process_t *ps = process_malloc(sizeof(process_t));
  if (sp == 0 || ps == 0) {
    return -1;
  }
  ps->sp = sp;
  ps->next = NULL;
  ps->started = 0; // not started
  ps->pid = process_counter;
  process_counter++;
  insert_at_tail(ps);  
  interrupts();
  return 0;
}


void process_start(void){
  process_begin();
  current_process = temp_current_process;
}

unsigned int process_select(unsigned int cursp) {
  // this if statement below was used only for debugging
  if (logging_index < NUM_DATA && current_process != NULL) {
    logging_array[logging_index] = current_process->pid;
    logging_index++;
  }

  if (cursp == 0){
    // process has not begun yet, begin the process and return its sp
    if (temp_current_process->started == 0){
      temp_current_process->started = 1;
      current_process = temp_current_process;
      return temp_current_process->sp;     
    }
    // process has ended, free the data structure and return next sp if one exists
    else {
      process_t *current_head = temp_current_process;
      temp_current_process = temp_current_process->next;
      free(current_head);
      // case if next is not null
      if(temp_current_process != NULL){
        temp_current_process->started = 1;
        current_process = temp_current_process;
        return temp_current_process-> sp;
      }
      // case if next is null
      else {
        temp_current_process = NULL;
        last_process = NULL;      
        current_process = temp_current_process;
        return 0;
      }
    }
  }
  // case 2: nonzero sp means 
  else {
    process_t* temp_head = temp_current_process;
    temp_head->sp = cursp;
    temp_current_process = temp_current_process->next;
    temp_head->next = NULL;
    insert_at_tail(temp_head);
    temp_current_process->started = 1;
    current_process = temp_current_process;
    return temp_current_process->sp;     
  }
}

// function that will display a provided string onto
// the OLED display for at least 500ms
void display_string(char* ch) {

  disp.clearDisplay();
  disp.setTextSize(1);
  disp.setTextColor(WHITE, BLACK);
  disp.setCursor(0, 0);
  disp.println(ch);
  disp.display();
  delay_millis(500);
}


// meant to be paired with p2
// attempts to write its `pid` onto OLED
void p1 (void)
{    
  for (int i = 0; i < 10; i++) {
    lock_acquire(l);
    display_string("1");
    lock_release(l);
    delay_millis(3);
  }
}

// meant to be paired with p1
// attempts to write its `pid` onto OLED
void p2 (void)
{
  for (int i = 0; i < 10; i++) {
    lock_acquire(l);
    display_string("2");
    lock_release(l);
    delay_millis(3);
  }
}

// meant to be paired with p4
// attempts to draw its own ball onto the screen, this is the `faster` process,
// only sleeps for 5ms after releasing the lock
void p3 (void)
{    
  while (1) {
    lock_acquire(l);
    disp.clearDisplay();
    // Using velocity and directions, update position of ball
    xBall_1 += xDir_1 * ballVelocity;
    yBall_1 += yDir_1 * ballVelocity;

    // Check for ball bounce: xBall and yBall represent the center coordinates of the ball
    // check that center +/- radius does not come into contact with boundary, if it does, reverse directions (xDir & yDir),
    // not encoder0Pos
    if(xBall_1 - ballRadius <= 0 || xBall_1 + ballRadius >= WIDTH){
      xDir_1 = xDir_1 * -1;
    } 
    if(yBall_1 - ballRadius <= 0 || yBall_1 + ballRadius >= HEIGHT){
      yDir_1 = yDir_1 * -1;
    }

    // Draw circle
    disp.drawCircle(xBall_1, yBall_1, ballRadius, SSD1306_WHITE);
    
    // Render buffer to screen
    disp.display();
    delay_millis(5);
    lock_release(l);
    delay_millis(5);
  }
}

// meant to be paired with p3
// attempts to draw its own ball onto the OLED, this is the `slower` process,
// sleeps for 10ms after releasing the lock
void p4 (void)
{
  while (1) {
    lock_acquire(l);
    disp.clearDisplay();
    // Using velocity and directions, update position of ball
    xBall_2 += xDir_2 * ballVelocity;
    yBall_2 += yDir_2 * ballVelocity;

    // Check for ball bounce: xBall and yBall represent the center coordinates of the ball
    // check that center +/- radius does not come into contact with boundary, if it does, reverse directions (xDir & yDir),
    // not encoder0Pos
    if(xBall_2 - ballRadius <= 0 || xBall_2 + ballRadius >= WIDTH){
      xDir_2 = xDir_2 * -1;
    } 
    if(yBall_2 - ballRadius <= 0 || yBall_2 + ballRadius >= HEIGHT){
      yDir_2 = yDir_2 * -1;
    }

    // Draw circle
    disp.drawCircle(xBall_2, yBall_2, ballRadius, SSD1306_WHITE);
    
    // Render buffer to screen
    disp.display();
    delay_millis(5);
    lock_release(l);
    delay_millis(10);
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  // For OLED
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
  disp.display();
  WIDTH = disp.width();
  HEIGHT = disp.height();

  l = malloc(sizeof(lock_t));
  lock_init(l);

  display_string("Starting setup");
  delay(2000);
//  if (process_create (p1, 64) < 0) {
//    digitalWrite(RED_PIN, HIGH);
//    return;
//  }
//
//  if (process_create (p2, 64) < 0) {
//    digitalWrite(RED_PIN, HIGH);
//    return;
//  }
  if (process_create (p3, 64) < 0) {
    digitalWrite(RED_PIN, HIGH);
    return;
  }

  if (process_create (p4, 64) < 0) {
    digitalWrite(RED_PIN, HIGH);
    return;
  }
}

void loop()
{
  process_start();
  /* if you get here, then all processes are either finished or
     there is deadlock */
  display_string("Finished");

  // interrupts();
  while (1) {
  }
}
