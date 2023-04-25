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


void setup() {
  // put your setup code here, to run once:
  pinMode(TPIN, OUTPUT);

  Serial.begin(9600);

}

// By ADS protocol: To send a one, we should transmit 0b10.
// Holds transmission pin HIGH then LOW, each for a duration of (bit transmission period)/2
void send_one() {
  //Serial.println("Sending One");
  digitalWrite(TPIN, HIGH);
  delay(TIME_ADSMS / 2);
  digitalWrite(TPIN, LOW);
  delay(TIME_ADSMS / 2);
}

// By ADS protocol: To send a zero, we should transmit 0b01.
// Holds transmission pin LOW then HIGH, each for a duration of (bit transmission period)/2
void send_zero(){
  digitalWrite(TPIN, LOW);
  delay(TIME_ADSMS / 2);
  digitalWrite(TPIN, HIGH);
  delay(TIME_ADSMS / 2);
  digitalWrite(TPIN, LOW);
}

// Keeps transmission pin LOW for one bit transmission period.
void send_nothing() {
  digitalWrite(TPIN, LOW);
  delay(TIME_ADSMS);
  digitalWrite(TPIN, LOW);
}

// Sends the header described by ADS.
void send_ads_header(){
  send_one();
  send_one();
  send_nothing();
  send_zero();
  send_zero();
  send_nothing();
  send_nothing(); 
  send_nothing();
}


// Receives an 8 bit char, c, and transmits it using ADS standard.
// Sends bits from right to left:
// e.g. 'a' = 97 = 0110 0001 => 
// These bits would be transmitted in the following order: 1000 0110
// receiver is in charge of reversing the order of bits.
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

// Helper function for our hamming encoder.
// Returns the i'th bit from the right of char d.
char extract_bit(char d, int i){
  return (d >> i) & 0x1;
}

// Hamming Encoder.
// Takes in 4 bits from a byte, and returns the 8 bit hamming encoding.
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

// Transmits a char using ADS protocol.
// The 8 bit char is split into its four lower and upper bits, each of which
// is encoded using our hamming encoder. The ADS header is transmitted, then the hamming byte 
// corresponding to the upper bits, followed by the hamming byte corresponding to the lower bits.
void hamming_and_transmit_byte(char byte){
  char lower_bits = 0xf & byte;
  char upper_bits = (0xf0 & byte) >> 4;
  
  char hamming_lower = hamming_encode_nibble(lower_bits);
  char hamming_upper = hamming_encode_nibble(upper_bits);

  send_ads_header();   
  transmit(hamming_upper);
  transmit(hamming_lower);
}

// Sends a message using the ADS protocol with hamming error correction.
// For the provided char*, message, each byte of message is transmitted 
// one at a time using hamming_and_transmit_byte(char). That is, each byte
// will have its own preamble in the transmitted signal.
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
