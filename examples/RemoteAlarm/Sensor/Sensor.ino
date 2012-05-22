// Transmit sketch used to report quality of reception.
// 2012-03-22 <cbrake@bec-systems.com> http://opensource.org/licenses/mit-license.php

// based on: https://github.com/jcw/jeelib/tree/master/examples/RF12/rfRangeTX
// 2011-05-14 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Based on a sample implementation by Steve Evans (@tankslappa).

#include <JeeLib.h>
#include <avr/sleep.h>

#define LED_PIN     9   // activity LED, comment out to disable

const int PIN_REEDSWITCH1 = 4;  // P1
const int PIN_REEDSWITCH2 = 5;  // P2

byte outData, pending;
MilliTimer sendTimer;

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

static void activityLed (byte on) {
#ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, !on);
#endif
}

// setup function
void setup() {
  Serial.begin(57600);
  Serial.print("\n[BEC Remote Alarm Sensor Node]");
  Serial.flush();
  rf12_initialize('T', RF12_915MHZ);

  // set reed switch pins to inputs and enable pullup
  pinMode(PIN_REEDSWITCH1, INPUT);
  pinMode(PIN_REEDSWITCH2, INPUT);
  digitalWrite(PIN_REEDSWITCH1, HIGH);
  digitalWrite(PIN_REEDSWITCH2, HIGH);
}

void debug_switch()
{
    Serial.print("\ns1 = ");
    Serial.print(digitalRead(PIN_REEDSWITCH1));
    Serial.print(" s2 = ");
    Serial.print(digitalRead(PIN_REEDSWITCH2));
}

bool check_switches()
{
  return digitalRead(PIN_REEDSWITCH1) && digitalRead(PIN_REEDSWITCH2);
}

void send_payload()
{
  rf12_sleep(RF12_WAKEUP);
  while (!rf12_canSend())
    rf12_recvDone();
  bool switch_state = check_switches();
  rf12_sendStart(0, &switch_state, sizeof switch_state);
  rf12_sendWait(SLEEP_MODE_STANDBY);
  rf12_sleep(RF12_SLEEP);
}

bool sent = false;

void loop() {
  //Serial.print("loop\n");
  //Serial.flush();
  //activityLed(1);
  //delay(50);
  //activityLed(0);
  
  if (!check_switches() && !sent) {
    send_payload();
    sent = true;
  }
  
  if (sendTimer.poll(30000)) {
    sent = false;
    //activityLed(1);
    // debug_switch();
    send_payload();
    //activityLed(0);
  }
  
  Sleepy::loseSomeTime(10000);
}

