// Transmit sketch used to report quality of reception.
// 2012-03-22 <cbrake@bec-systems.com> http://opensource.org/licenses/mit-license.php

// based on: https://github.com/jcw/jeelib/tree/master/examples/RF12/rfRangeTX
// 2011-05-14 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Based on a sample implementation by Steve Evans (@tankslappa).

#include <JeeLib.h>

#define LED_PIN     9   // activity LED, comment out to disable

const int PIN_REEDSWITCH1 = 4;  // P1
const int PIN_REEDSWITCH2 = 5;  // P2

byte outData, pending;
MilliTimer sendTimer;

static void activityLed (byte on) {
#ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, !on);
#endif
    // set reed switch pins to inputs and enable pullup
    pinMode(PIN_REEDSWITCH1, INPUT);
    pinMode(PIN_REEDSWITCH2, INPUT);
    digitalWrite(PIN_REEDSWITCH1, HIGH);
    digitalWrite(PIN_REEDSWITCH2, HIGH);
}

void setup() {
  Serial.begin(57600);
  Serial.print("\n[BEC Remote Alarm Sensor Node]");
  rf12_initialize('T', RF12_915MHZ);
}

void loop() {
  rf12_recvDone();

  if (sendTimer.poll(200)) {
    ++outData;
    pending = 1;
  }

  if (pending && rf12_canSend()) {
    activityLed(1);
    Serial.print("\ns1 = ");
    Serial.print(digitalRead(PIN_REEDSWITCH1));
    Serial.print(" s2 = ");
    Serial.print(digitalRead(PIN_REEDSWITCH2));
    rf12_sendStart(0, &outData, sizeof outData);
    pending = 0;
    delay(5);
    activityLed(0);
  }
}

