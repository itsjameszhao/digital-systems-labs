//#import <RCSwitch.h>
#define TPIN 12

//RCSwitch mySwitch = RCSwitch();

#define DIT 1
#define DAH 3
#define INTRA 1
#define INTER 3
#define WORD 7


#define TIME_ADSNS 16000 // Time for one bit to be transmitted, in microseconds
#define TIME_ADSMS 10

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

void send_morse(const char* word){
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

void send_one() {
  //Serial.println("Sending One");
  digitalWrite(TPIN, HIGH);
  delay(TIME_ADSMS / 2);
  digitalWrite(TPIN, LOW);
  delay(TIME_ADSMS / 2);
}

void send_zero(){
  //Serial.println("Sending Zero");
  digitalWrite(TPIN, LOW);
  delay(TIME_ADSMS / 2);
  digitalWrite(TPIN, HIGH);
  delay(TIME_ADSMS / 2);
  digitalWrite(TPIN, LOW);
}

void send_nothing() {
  digitalWrite(TPIN, LOW);
  delay(TIME_ADSMS);
  digitalWrite(TPIN, LOW);
}

void send_ads_header(){
  //Serial.println("Sending Header");
  send_one();
  send_one();
  send_nothing();
  send_zero();
  send_zero();
  send_nothing();
  send_nothing(); 
  send_nothing();
  //Serial.println("Header Finished"); 
}

void send_ads_char(const char c){
  //Serial.println("Sending character");  
  //Serial.println(c);
  send_ads_header();
  for(int i = 0; i < 8; i++){
    int bit = (c >> i) & 1;
    if (bit == 1){
      send_one();
    } 
    else {
      send_zero();
    }
  }    

}

void send_ads(const char *message) {
  Serial.println("Sending message");
  //Serial.println(message);
  for(char *p = message; *p != 0; p++){
    send_ads_char(*p);    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  send_ads("hello world!");
  // int randSleep = random(5, 500);
  // digitalWrite(TPIN, LOW);
  // delay(randSleep);
  // digitalWrite(TPIN, HIGH);
  // delay(randSleep);
}