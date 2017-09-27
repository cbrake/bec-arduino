/*
  Robot X Motor Control Program
  by Cliff and Miles Brake
  2015  
 */

#include <JeeLib.h>

int AENBL = 3;
int APHASE = A0;
int BENBL = 5;
int BPHASE = 4;

#define RUN HIGH
#define STOP LOW
#define FORWARD HIGH
#define REVERSE LOW

#undef DEBUG

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize digital pin 13 as an output.
  pinMode(AENBL, OUTPUT);
  pinMode(APHASE, OUTPUT);
  pinMode(BENBL, OUTPUT);
  pinMode(BPHASE, OUTPUT);
  rf12_initialize('R', RF12_915MHZ);
}

void motor_a(int speed) {
#ifdef DEBUG
  Serial.print("a: ");
  Serial.print(speed);
  Serial.print("\n");
#endif
  if (speed > -50 && speed < 50) {
    analogWrite(AENBL, 0);
    return;
  }
  
  if (speed > 255) speed = 255;
  if (speed < -255) speed = -255;
    
  if (speed > 0) {
    analogWrite(AENBL, speed);
    digitalWrite(APHASE, FORWARD);
  } else {
    analogWrite(AENBL, -speed);
    digitalWrite(APHASE, REVERSE);
  }
}

void motor_b(int speed) {
#ifdef DEBUG
  Serial.print("b: ");
  Serial.print(speed);
  Serial.print("\n");
#endif
  if (speed > -50 && speed < 50) {
    analogWrite(BENBL, 0);
    return;
  }

  if (speed > 255) speed = 255;
  if (speed < -255) speed = -255;
    
  if (speed > 0) {
    analogWrite(BENBL, speed);
    digitalWrite(BPHASE, FORWARD);
  } else {
    analogWrite(BENBL, -speed);
    digitalWrite(BPHASE, REVERSE);
  }
}

void motor_b(int run,  int direction) {
  digitalWrite(BENBL, run);
  digitalWrite(BPHASE, direction);
}  

struct packet {
  int up_down;
  int right_left;
}; 
  
// the loop function runs over and over again forever
void loop() {
  struct packet * p;
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == sizeof(*p)) {
    p = (packet*)rf12_data;
#ifdef DEBUG
    Serial.print("up_down: ");
    Serial.print(p->up_down);
    Serial.print("\n");
    Serial.print("right_left: ");
    Serial.print(p->right_left);
    Serial.print("\n");
#endif
    int speed = (p->up_down - 512)/2;
    int side = (p->right_left - 512)/2;
#ifdef DEBUG
    Serial.print("speed: ");
    Serial.print(speed);
    Serial.print("\n");
    Serial.print("side: ");
    Serial.print(side);
    Serial.print("\n");
#endif
    motor_a(speed+side);
    motor_b(speed-side);
  }
}
