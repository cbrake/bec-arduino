


// connect relays to Port 3
#define PIN_RELAY_A  16      // main power
#define PIN_RELAY_D  6       // connected to push button switch

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.print("\n[Power Cycle Test Application]\n");
  pinMode(PIN_RELAY_A, OUTPUT);
  pinMode(PIN_RELAY_D, OUTPUT);
}

int cycle_count = 0;

void loop() {
  // Power on 
  Serial.print("Cycle test ");
  Serial.print(cycle_count);
  Serial.print("\n");
  digitalWrite(PIN_RELAY_A, HIGH);
  delay(1000);
  digitalWrite(PIN_RELAY_D, HIGH);
  delay(1000);
  digitalWrite(PIN_RELAY_D, LOW);
  Serial.print("switch toggled\n");  
  
  // delay till system is up and yank power
  delay(60000);
  Serial.print("yank power\n");
  digitalWrite(PIN_RELAY_A, LOW);
  // delay to let things bleed off
  delay(5*1000);
  cycle_count++;
}





