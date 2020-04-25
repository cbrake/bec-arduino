
// relay plug is connected to Port 1
// see http://jeelabs.net/projects/hardware/wiki/JeeNode for information on how
// Use Arduino UNO as board
// JeeNode ports map to Arduino port names
// relay plug can be purchased from: http://moderndevice.com/product/relay-plug/
#define PIN_RELAY_4A  7 
#define PIN_RELAY_3D  6 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("\r\nSimple Relay Cycle\r\n");
  pinMode(PIN_RELAY_4A, OUTPUT);
  pinMode(PIN_RELAY_3D, OUTPUT);
}

int pulse_count = 0;
int stop = 500;

void loop() {
  if (pulse_count >= stop && false) {
    Serial.print("sent ");
    Serial.print(stop);
    Serial.print(" pulses\n");
    while(1) {}
  }
  Serial.print("Count: ");
  Serial.println(pulse_count);
  
  digitalWrite(PIN_RELAY_4A, HIGH);
  digitalWrite(PIN_RELAY_3D, HIGH);
  delay(10);
  digitalWrite(PIN_RELAY_4A, LOW);
  digitalWrite(PIN_RELAY_3D, LOW);
  //delayMicroseconds(490);
  delay(10);
  pulse_count++;
}
