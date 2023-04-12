#include "concurrency.h"

struct process_state{
  unsigned int sp;
  struct process_state *next;
  int started;
  int pid;
};

#define RED_PIN 3
#define BLUE_PIN 2
#define GREEN_PIN 1


/*
    In this sketch, we implement a basic model for concurrency. There are three global variables essential to make this happen.
    current_process, temp_current_process, and last_process. Since we use a linked list to represent our available processes,
    these global variables exist to help us ensure that we can always access the state of our linked list. 

    process_state: a structure to represent the state necessary for a process in our concurrency model. `sp` stores a given
    process's most recent stack pointer, `started` indicates whether a process has kicked off its execution or not (useful
    to determine if a process has terminated or not), `next` is a pointer to the next process in our queue

    process_create: in this function, we make two important function calls, process_init and process_malloc. The former
    is used to create the appropriate stack memory for the function that processes will call, and the latter is used to initialize
    the memory associated with a process's state. After allocating this memory, we add the process_state struct onto our linked list

    process_select: used to select the next process to execute. There are two main inputs to handle, zero and non-zero.
    With a zero input, a process has either terminated, or nothing is running. To differentiate between these two cases,
    we do some extra checking with the `started` flag in our process_state struct. We also handle special cases here such as
    no other process available for execution. We always update the value of current_process at the end of this function call.

    process_start: calls process_begin, which will indirectly call process_select, which handles all necessary cases

    TESTING: Our test uses an RGB LED and 3 processes. Each process controls a separate digital pin on the Arduino, so 
    no competition occurs (i.e. no mutual exclusion is needed). Each process will turn on their LED for 100ms * N, and turn it off
    for 200ms * N, making it seem that these lights alternate. However, since each LED is controlled by a single process, they
    are all independent of one another. These intervals can safely be changed to variable lengths for varying visual output.

    Another possible test would be to use the 5x7 led matrix, but functionally it would behave similarly to our current
    test: each process controls a set of pins, and they write to them at variable times.

    More complex tests implemented in part 2, but here it is safe to run any combination of these processes, all 3, only 2, or
    just 1, our model for concurrency allows all these as valid executions. More processes can also be built on top of 
    the current ones, as long as no resources are shared.
*/

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
// only works with temp_current_process, should not update current_process
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

// allocates memory, pushes struct onto our linked list
// disable interrupts through entire call for safety
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

// handles contexts switches, and handles all cases
// first process, process ended, no more processes, processes queued
unsigned int process_select(unsigned int cursp) {
  // if statement below only for debugging
  if (logging_index < NUM_DATA && current_process != NULL) {
    logging_array[logging_index] = current_process->pid;
    logging_index++;
  }

  // safety check: if nothing in queue, nothing to run
  if(temp_current_process == NULL){
    return 0;
  }

  // process has either terminated, or hasn't started
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
  // case 2: nonzero sp means current process hasn't terminated, switch to next in queue (could be itself)
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


// functions below turn on/off their LEDs at set intervals, intervals can safely be changed
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

  while (1) {
    Serial.println("IN WHILE LOOP");
  }
}
