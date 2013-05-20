// Receive sketch used to report quality of reception.
// 2012-03-22 <cbrake@bec-systems.com> http://opensource.org/licenses/mit-license.php

// based on https://github.com/jcw/jeelib/tree/master/examples/RF12/rfRangeRX
// 2011-05-14 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Based on a sample implementation by Steve Evans (@tankslappa).

#include <JeeLib.h>

#define BUFFER_SIZE 64
#define DISPLAY_INTERVAL 500 // ms
#define LED_PIN     9   // activity LED, comment out to disable

//PortI2C myI2C (3);
MilliTimer statTimer;

byte timeBuf [BUFFER_SIZE]; // index is time slot, value is last packet
byte seqBuf [BUFFER_SIZE];  // index is last packet, value is time slot
char history [11];
byte lastSeq;

static void activityLed (byte on) {
#ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, !on);
#endif
}


static void gotPacket () {
  byte tenths = millis() / 100;
  // remember for each time slot what the last received packet was
  timeBuf[tenths % BUFFER_SIZE] = lastSeq;
  // remember for the last BUFFER_SIZE packets when they arrived
  seqBuf[lastSeq % BUFFER_SIZE] = tenths;
}

static byte recvCount (byte period) {
  // tenths and diff are bytes, so they are automatically modulo 256
  byte tenths = millis() / 100;
  byte n = 0;
  for (byte i = 0; i < sizeof seqBuf; ++i) {
    byte diff = tenths - seqBuf[i];
    if (diff <= period)
      ++n;
  }
  return n;
}

static void updateHistory () {
  for (byte i = 1; i < 10; ++i)
    history[10-i] = history[9-i];
  history[0] = '0';
  // tenths and diff are bytes, so they are automatically modulo 256
  byte tenths = millis() / 100;
  for (byte i = 0; i < DISPLAY_INTERVAL / 100; ++i) {
    byte pos = (tenths - i - 1) % BUFFER_SIZE;
    byte diff = lastSeq - timeBuf[pos];
    if (diff < 5)
      ++history[0];
  }
}

void setup () {
  Serial.begin(57600);
  Serial.print("\n[JeenodeRangeRx]");
  rf12_initialize('R', RF12_915MHZ);
}

void loop () {
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == 1) {
    lastSeq = rf12_data[0];
    gotPacket();
    activityLed(1);
    delay(5);
    activityLed(0);
    Serial.print("R ");
    Serial.print(lastSeq);
    Serial.print("\n");
  }
    
  if (statTimer.poll(DISPLAY_INTERVAL)) {
    // number of packets received in the last 5 seconds, as percentage
    //lcd3dig(0, 0, recvCount(50) * 2);
    // number of packets received in the last second, as percentage
    //lcd3dig(9, 0, recvCount(10) * 10);
    // show number of packets received in the last 10 display intervals
    updateHistory();
    //lcd.print(history);
  }
}
