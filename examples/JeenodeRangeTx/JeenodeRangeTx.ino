// Transmit sketch used to report quality of reception.
// 2012-03-22 <cbrake@bec-systems.com> http://opensource.org/licenses/mit-license.php

// based on: https://github.com/jcw/jeelib/tree/master/examples/RF12/rfRangeTX
// 2011-05-14 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Based on a sample implementation by Steve Evans (@tankslappa).

#include <JeeLib.h>

#define LED_PIN     9   // activity LED, comment out to disable

byte outData, pending;
MilliTimer sendTimer;

static void activityLed (byte on) {
#ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, !on);
#endif
}

void setup() {
  Serial.begin(57600);
  Serial.print("\n[JeenodeRangeTx]");
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
    rf12_sendStart(0, &outData, sizeof outData);
    pending = 0;
    delay(5);
    activityLed(0);
  }
}
