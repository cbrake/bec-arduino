// Receive sketch used to report quality of reception.
// 2012-03-22 <cbrake@bec-systems.com> http://opensource.org/licenses/mit-license.php

// based on https://github.com/jcw/jeelib/tree/master/examples/RF12/rfRangeRX
// 2011-05-14 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Based on a sample implementation by Steve Evans (@tankslappa).

#include <JeeLib.h>

#define LED_PIN     9   // activity LED, comment out to disable

static void activityLed (byte on) {
#ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, !on);
#endif
}

void setup () {
  Serial.begin(57600);
  Serial.print("\n[BEC Alarm Repeater]");
  rf12_initialize('S', RF12_915MHZ);
}

void send_payload(byte data)
{
  while (!rf12_canSend())
    rf12_recvDone();
  rf12_sendStart(0, &data, sizeof data);
}

void loop () {
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == 1) {
    byte lastSeq = rf12_data[0];
    activityLed(1);
    delay(5);
    activityLed(0);
    Serial.print("R ");
    Serial.print(lastSeq);
    Serial.print("\n");
    send_payload(lastSeq);
  }
}

