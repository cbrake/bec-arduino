
#include <JeeLib.h>

void setup() {
  Serial.begin(115200);
  rf12_initialize('T', RF12_915MHZ);
}

struct packet {
  int up_down;
  int right_left;
};

MilliTimer sendTimer;

void loop() {  
  // put your main code here, to run repeatedly:
  rf12_recvDone();
  
  if (sendTimer.poll(50)) {
    struct packet p;
    p.up_down = analogRead(A0);
    p.right_left = analogRead(A1);
    Serial.print("up_down: ");
    Serial.print(p.up_down);
    Serial.print("\n");
    Serial.print("right_left: ");
    Serial.print(p.right_left);
    Serial.print("\n");
    if (rf12_canSend()) {
      rf12_sendStart(0, &p, sizeof p);
      Serial.print("s\n");
    }
  }
}
