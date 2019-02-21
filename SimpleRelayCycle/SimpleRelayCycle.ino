// sketch to control a Jeelabs relay plug to cycle test power to a Unit under test

// relay plug is connected to Port 3
// see http://jeelabs.net/projects/hardware/wiki/JeeNode for information on how
// JeeNode ports map to Arduino port names
// relay plug can be purchased from: http://moderndevice.com/product/relay-plug/
#define PIN_RELAY_A  16      // main power
#define PIN_RELAY_D  6       // connected to push button switch

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("\r\nSimple Relay Cycle\r\n");
  pinMode(PIN_RELAY_A, OUTPUT);
  pinMode(PIN_RELAY_D, OUTPUT);
}

int cycle_count = 0;

void loop() {
  // Power on 
  digitalWrite(PIN_RELAY_A, HIGH);
  delay(500);
  digitalWrite(PIN_RELAY_A, LOW);
  delay(500);
}
