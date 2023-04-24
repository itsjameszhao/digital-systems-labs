//#import <RCSwitch.h>
#define TPIN 13

//RCSwitch mySwitch = RCSwitch();

#define DIT 1
#define DAH 3
#define INTRA 1
#define INTER 3
#define WORD 7


#define TIME_ADSNS 16000 // Time for one bit to be transmitted, in microseconds
#define TIME_ADSMS 20

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

// receives an 8 bit char, and transmits it using ADS standard
// sends bits from right to left:
// e.g. 'a' = 97 = 0110 0001
// these bits would be transmitted in the following order: 1000 0110
// receiver is in charge of reversing the order of bits
void transmit(const char c){
  for(int i = 0; i < 8; i++){
    int bit = (c >> i) & 1;
    if(bit == 1){
      send_one();
    } else {
      send_zero();
    }
  }
}

char input[128];

void high_low_test(){
  while(true){
    digitalWrite(TPIN, HIGH);
    delay(10);
    digitalWrite(TPIN, LOW);
    delay(10);
  }
}

char extract_bit(char d, int pos){
  return (d >> pos) & 0x1;
}

char hamming_encode_nibble(char half_byte){
  char d = 0xf & half_byte;

  // hamming bits
  char h = 0;
  h |= ((extract_bit(d, 1) + extract_bit(d, 2) + extract_bit(d, 3)) & 1) << 2;
  h |= ((extract_bit(d, 0) + extract_bit(d, 2) + extract_bit(d, 3)) & 1) << 1;
  h |= (extract_bit(d, 0) + extract_bit(d, 1) + extract_bit(d, 3)) & 1;
  h &= 0x7;

  // parity
  char p = 0 ^ extract_bit(d, 0) ^ extract_bit(d, 1) ^ extract_bit(d, 2) ^ extract_bit(d, 3) ^ ((h >> 2) & 1) ^ ((h >> 1) & 1) ^ (h & 1);

  char encoded = (half_byte & 0xf);
  encoded |= (p << 7) | ((h & 1) << 6) | (((h >> 1) & 1) << 5) | (((h >> 2) & 1) << 4);

  return encoded;
}

// current transmission transmits bits right to left
// for(int i = 0; i < 8; i++) int bit = (c >> i) & 1
void hamming_and_transmit_byte(char byte){
  char lower_bits = 0xf & byte;
  char upper_bits = 0xf0 & byte;
  
  char hamming_lower = hamming_encode_nibble(lower_bits);
  char hamming_upper = hamming_encode_nibble(upper_bits);

  send_ads_header();   
  transmit(hamming_upper);
  transmit(hamming_lower);
}

void send(const char *message) {
  Serial.print("Sending message: ");
  Serial.println(message);
  for(char *p = message; *p != 0; p++){
    hamming_and_transmit_byte(*p);    
  }
}

// hamming h = 1101 0110
// 

// h as received = 0001 0110
// h = 0110 1000
// 0110 => 0110 0110
// 1000 => 1111 1000

// preamble 0110 0110 1111 1000
void loop() {
  // put your main code here, to run repeatedly:
  //high_low_test();
  // if(Serial.available()){
  //   Serial.readStringUntil('\n').toCharArray(input, 128);
  //   Serial.print("You typed: " );
  //   Serial.println(input);
  //   while(true) send_ads(input);
  // }
  send("hello world!");
  
}
