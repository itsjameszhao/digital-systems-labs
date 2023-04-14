//#import <RCSwitch.h>
#define TPIN 12

//RCSwitch mySwitch = RCSwitch();

#define DIT 1
#define DAH 3
#define INTRA 1
#define INTER 3
#define WORD 7

#define TIME 100

#define SHORT 250

#define LONG 500

#define PAUSE 100


static const char *alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};



void setup() {
  // put your setup code here, to run once:
  pinMode(TPIN, OUTPUT);
  //mySwitch.enableTransmit(TPIN);
  //mySwitch.setPulseLength(20000000);
  //mySwitch.setRepeatTransmit(1);


  Serial.begin(9600);

}

void send(const char* word){
  Serial.println("Sending ");
  for(const char* p = word; *p; p++){
    Serial.println(*p);
    char* morsePattern = alpha[*p - 97];
    Serial.println(morsePattern);
    //Serial.println(morsePattern);
    for(const char* m = morsePattern; *m; m++){
      digitalWrite(TPIN, HIGH);
      if(*m == '.'){
        delay(DIT*TIME);
      } else {
        delay(DAH*TIME);
      }
      digitalWrite(TPIN, LOW);
      delay(INTRA*TIME);
    }
    delay(INTER*TIME);
  }
  delay(WORD*TIME);

}

void loop() {
  // put your main code here, to run repeatedly:
  send("ooo");
  // int randSleep = random(5, 500);
  // digitalWrite(TPIN, LOW);
  // delay(randSleep);
  // digitalWrite(TPIN, HIGH);
  // delay(randSleep);
}
