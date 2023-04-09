#include "concurrency.h"

struct process_state{
  unsigned int sp;
  struct process_state *next;
  int started;
};

#define RED_P1 3
#define BLUE_P2 2
#define GREEN_END 4


// global variable indicating the head of the process list
process_t *current_process = NULL;

// global variable, temporary placeholder for the head of the process list
process_t *temp_current_process = NULL;

// global variable indicating the tail of the process list
process_t *last_process;

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
  insert_at_tail(ps);  
  interrupts();
  return 0;
}


void process_start(void){
  process_begin();
  current_process = temp_current_process;
}

unsigned int process_select(unsigned int cursp) {
  if (temp_current_process == NULL) {
    return 0;
  }
  // case if the process hasn't begun or terminated
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
  for(int i = 0; i < 100; i++){
    digitalWrite(3, HIGH);
    delay(100);
    digitalWrite(3, LOW); 
  }
  return;
}

void p2 (void)
{
/* process 2 here */
  for(int i = 0; i < 100; i++) {
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);  
  }
 return;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  if (process_create (p1, 64) < 0) {
    return;
  }

  if (process_create (p2, 64) < 0) {
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
  while (1) ;
}