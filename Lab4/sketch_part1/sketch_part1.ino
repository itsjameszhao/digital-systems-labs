#include "concurrency.h"
#define OLED_CS 8
#define OLED_DC 4 
#define OLED_RST 5
#define OLED_SI 6
#define OLED_CLK 7
#include <Adafruit_SSD1306.h>

struct process_state{
  unsigned int sp;
  struct process_state *next;
  int started;
  int pid;
};

#define RED_PIN 3
#define BLUE_PIN 2
#define GREEN_PIN 1

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
  if (logging_index < NUM_DATA && current_process != NULL) {
    logging_array[logging_index] = current_process->pid;
    logging_index++;
    // if (current_process->pid == 10) {
    //   // digitalWrite(2, HIGH);
    //   // delay(100);
    //   // digitalWrite(2, LOW); 
    // }  else if (current_process->pid == 11) {
    //   // digitalWrite(3, HIGH);
    //   // delay(100);
    //   // digitalWrite(3, LOW); 
    // }
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


void p1 (void)
{    
  /* process 1 here */
  for(int i = 0; i < 25; i++){
    digitalWrite(RED_PIN, HIGH);
    delay(100 * N);
    digitalWrite(RED_PIN, LOW);
    delay(200 * N); 
  }
  return;
}

void p2 (void)
{
/* process 2 here */
  delay(100 * N);
  for(int i = 0; i < 25; i++) {
    digitalWrite(GREEN_PIN, HIGH);
    delay(100 * N);
    digitalWrite(GREEN_PIN, LOW);
    delay(200 * N);  
  }
 return;
}

void p3 (void)
{
/* process 2 here */
  delay(200 * N);
  for(int i = 0; i < 25; i++) {
    digitalWrite(BLUE_PIN, HIGH);
    delay(100 * N);
    digitalWrite(BLUE_PIN, LOW);
    delay(200 * N);  
  }
 return;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  logging_index = 0;
  for (int i = 0; i < NUM_DATA; i++) {
    logging_array[i] = 0;
  }

  if (process_create (p1, 64) < 0) {
    return;
  }

  if (process_create (p2, 64) < 0) {
    return;
  }

  if (process_create (p3, 64) < 0) {
    return;
  }

}

void loop()
{
  process_start();
  /* if you get here, then all processes are either finished or
     there is deadlock */
  Serial.println("FINISHED");
  digitalWrite(4, HIGH); 
  // noInterrupts();
  Serial.println(logging_array[0]);
  Serial.println(logging_array[1]);
  Serial.println(logging_array[2]);
  Serial.println(logging_array[3]);
  Serial.println(logging_array[4]);
  Serial.println(logging_array[5]);
  Serial.println(logging_array[6]);
  Serial.println(logging_array[7]);
  Serial.println(logging_array[8]);
  Serial.println(logging_array[9]);


  // interrupts();
  while (1) {
    noInterrupts();
    Serial.println("IN WHILE LOOP");
  }
}