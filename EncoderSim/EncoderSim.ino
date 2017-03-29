/*
 * Encoder simulator
 * 
 * This can be used to simulate an encoder for testing.
 * 
 * enable 'f' to step forward, 'b' to step backward.
 * 
 * This example code is in the public domain.
 * 
 * Cliff Brake, 2017
 */


const int numSteps = 50;
const int A = 8;
const int B = 10;
const int periodUs = 500;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600);
  while(!Serial) {
    ; // wait for serial port to connect
  }

  Serial.println("Encoder simulator");
}

void forward() {
  for (int i=0; i < numSteps; i++) {
    digitalWrite(A, HIGH);
    delayMicroseconds(periodUs/4);
    digitalWrite(B, HIGH);
    delayMicroseconds(periodUs/4);
    digitalWrite(A, LOW);
    delayMicroseconds(periodUs/4);
    digitalWrite(B, LOW);
    delayMicroseconds(periodUs/4);
  }
}

void back() {
  for (int i=0; i < numSteps; i++) {
    digitalWrite(B, HIGH);
    delayMicroseconds(periodUs/4);
    digitalWrite(A, HIGH);
    delayMicroseconds(periodUs/4);
    digitalWrite(B, LOW);
    delayMicroseconds(periodUs/4);
    digitalWrite(A, LOW);
    delayMicroseconds(periodUs/4);
  }
}

// the loop function runs over and over again forever
void loop() {
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 'f') {
      Serial.print("stepping forward ");
      Serial.println(numSteps);
      forward();
    } else if (inByte == 'b') {
      Serial.print("stepping back ");
      Serial.println(numSteps);
      back();
    }
  }
}
