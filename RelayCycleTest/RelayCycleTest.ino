// sketch to control a Jeelabs relay plug to cycle test power to a Unit under test

// relay plug is connected to Port 3
// see http://jeelabs.net/projects/hardware/wiki/JeeNode for information on how
// JeeNode ports map to Arduino port names
#define PIN_RELAY_A  16      // main power
#define PIN_RELAY_D  6       // connected to push button switch

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.print("\r\n[Power Cycle Test Application]\r\n");
  pinMode(PIN_RELAY_A, OUTPUT);
  pinMode(PIN_RELAY_D, OUTPUT);
}

int cycle_count = 0;

void loop() {
  // Power on 
  Serial.print("Cycle test ");
  Serial.print(cycle_count);
  Serial.print("\r\n");
  digitalWrite(PIN_RELAY_A, HIGH);
  delay(1000);
  digitalWrite(PIN_RELAY_D, HIGH);
  delay(1000);
  digitalWrite(PIN_RELAY_D, LOW);
  Serial.print("switch toggled\r\n");  
  
  // delay till system is up and yank power
  delay(60000);
  Serial.print("yank power\r\n");
  digitalWrite(PIN_RELAY_A, LOW);
  // delay to let things bleed off
  delay(5*1000);
  cycle_count++;
}





