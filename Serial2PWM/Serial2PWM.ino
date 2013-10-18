/*
  Reads a binary byte on serial line and outputs
  value to PWM channel
 */
 
// the setup routine runs once when you press reset:
void setup() {                
  analogWrite(3, 0);
  Serial.begin(115200);
}

int incomingByte = 0;

// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    analogWrite(3, incomingByte);
  }
}
