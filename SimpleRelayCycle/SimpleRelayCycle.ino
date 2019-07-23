// sketch to control a Jeelabs relay plug to cycle test power to a Unit under test

// relay plug is connected to Port 3
// see http://jeelabs.net/projects/hardware/wiki/JeeNode for information on how
// Use Arduino UNO as board
// JeeNode ports map to Arduino port names
// relay plug can be purchased from: http://moderndevice.com/product/relay-plug/
#define PIN_RELAY_A  16      // main power
#define PIN_RELAY_D  6       // connected to push button switch
#define PIN_FET 7

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("\r\nSimple Relay Cycle\r\n");
  pinMode(PIN_RELAY_A, OUTPUT);
  pinMode(PIN_RELAY_D, OUTPUT);
  pinMode(PIN_FET, OUTPUT);
}

int pulse_count = 0;
int stop = 5000;

void loop() {
  
  if (pulse_count >= stop) {
    Serial.print("sent ");
    Serial.print(stop);
    Serial.print(" pulses\n");
    while(1) {}
  }
  
  digitalWrite(PIN_RELAY_A, HIGH);
  digitalWrite(PIN_FET, HIGH);
  //delayMicroseconds(490);
  digitalWrite(PIN_RELAY_A, LOW);
  digitalWrite(PIN_FET, LOW);
  //delayMicroseconds(490);
  pulse_count++;
}
