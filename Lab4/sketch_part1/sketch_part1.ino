#include "concurrency.h"

struct process_state{
  unsigned int sp;
  struct process_state *next;
  int started;
};

#define RED_P1 3
#define BLUE_P2 2
#define GREEN_END 4

typedef struct process_state process_t;

// global variable indicating the head of the process list
process_t *current_process = NULL;

// global variable indicating the tail of the process list
process_t *last_process;

// insert a new process struct at the end of the process list, or if there isn't anything just make it the head
void insert_at_tail(process_t *ps) {
  if (current_process == NULL) {
    current_process = ps;
    last_process = ps;
  }
  else {
    last_process->next = ps;
    last_process = ps;
  }
}

int process_create(void (*f) (void), int n) {
  unsigned int sp = process_init(f, n);
  process_t *ps = malloc(sizeof(process_t));
  if (sp == 0 || ps == 0) {
    return -1;
  }
  ps->sp = sp;
  ps->next = NULL;
  ps->started = 0; // not started

  insert_at_tail(ps);

  return 0;
}


void process_start(void){
  process_begin();
}

unsigned int process_select(unsigned int cursp){
  if (current_process == NULL) {
    return 0;
  }
  // case 1: cursp == 0
  if (cursp == 0){
    if (current_process->started == 0){
       current_process->started = 1;
       return current_process->sp;     
    }
    else {
      process_t *current_head = current_process;
      current_process = current_process->next;
      free(current_head);
      if(current_process != NULL){
        current_process->started = 1;
        return current_process-> sp;
      }
      else {
        current_process = NULL;
        last_process = NULL;      
        return 0;
      }
    }
  }
  // case 2: nonzero
  else {
    process_t* temp_head = current_process;
    current_process = current_process->next;
    temp_head->next = NULL;
    insert_at_tail(temp_head);
    current_process->started = 1;
    return current_process->sp;     
  }
}


void p1 (void)
{
  /* process 1 here */
  for(int i = 0; i < 100; i++){
    digitalWrite(3, HIGH);
    //digitalWrite(2, LOW);
    delay(100);
  }
  return;
}

void p2 (void)
{
/* process 2 here */
  for(int i = 0; i < 100; i++) {
    digitalWrite(2, HIGH);
    //digitalWrite(3, LOW);
    delay(100);
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
  //digitalWrite(2, LOW);
  //digitalWrite(3, LOW);
  while (1) ;
}