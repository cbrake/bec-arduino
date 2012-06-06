// Alarm system monitor
// 2012-03-22 <cbrake@bec-systems.com> http://opensource.org/licenses/mit-license.php

// based on https://github.com/jcw/jeelib/tree/master/examples/RF12/rfRangeRX
// 2011-05-14 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Based on a sample implementation by Steve Evans (@tankslappa).

#include <JeeLib.h>

#define LED_PIN     9   // activity LED, comment out to disable
#define BEEP_PIN    5   // P2
#define GREEN_LED_PIN  7  // P4

MilliTimer beepTimer;
MilliTimer sensorAliveTimer;
MilliTimer heartbeatLed;
int sensor_alive_minutes = 0;

static void activityLed (byte on) {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, on);
}

enum state {
  STATE_INVALID,
  STATE_NORMAL,
  STATE_SENSOR_ERROR,
  STATE_ALARM,
};


int state = STATE_INVALID;

bool sensor_ok = true;
bool received_sensor_data = false;

char * state_desc[] = {
  "STATE:INVALID",
  "STATE:NORMAL",
  "STATE:SENSOR_ERROR",
  "STATE:ALARM"
};


void setup () {
  Serial.begin(57600);
  Serial.print("\n[BEC Alarm Monitor]\n");
  rf12_initialize('R', RF12_915MHZ);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);

  // blink green LED on startup
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, 1);
  delay(200);
  digitalWrite(GREEN_LED_PIN, 0);
  delay(200);
  digitalWrite(GREEN_LED_PIN, 1);
  delay(200);
  digitalWrite(GREEN_LED_PIN, 0);

  set_state(STATE_NORMAL);
}


void set_state(int new_state)
{
  if (new_state != state) {
    state = new_state;
    Serial.print(state_desc[new_state]);
    Serial.print("\n");
  }
}

void loop () {
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == 1) {
    sensor_ok = rf12_data[0];
    activityLed(1);
    delay(5);
    activityLed(0);
    Serial.print("R ");
    Serial.print(sensor_ok);
    Serial.print("\n");
    received_sensor_data = true;
    if (!sensor_ok) {
      // turn on alarm
      tone(BEEP_PIN, 2048, 500);
    } else {
      noTone(BEEP_PIN);
    }
  }

  if (sensorAliveTimer.poll(60000) && ++sensor_alive_minutes >= 3) {
    Serial.print("Checking if sensor is alive\n");
    switch (state) {
      case STATE_NORMAL: {
        if (!received_sensor_data) 
          set_state(STATE_SENSOR_ERROR);
        break;
      }
      case STATE_SENSOR_ERROR: {
        if (received_sensor_data)
          set_state(STATE_NORMAL);
        break;
      }                         
      case STATE_ALARM: {
        if (!received_sensor_data)
          set_state(STATE_SENSOR_ERROR);
        break;
      }
    }

    sensor_alive_minutes = 0;
    received_sensor_data = false;
  }


  // do the fast loop processing
  switch (state) {
    case STATE_NORMAL: {
      if (!sensor_ok) 
        set_state(STATE_ALARM);
        break;
    }
    case STATE_ALARM: {
      if (sensor_ok) 
        set_state(STATE_NORMAL);
      break;
    }
    case STATE_SENSOR_ERROR: {
      if (received_sensor_data)
        set_state(STATE_NORMAL);
    }
  }

  // beeper handling
  if (beepTimer.poll(1000)) {
    switch (state) {
      case STATE_NORMAL: {
        // do nothing
        noTone(BEEP_PIN);
        break;
      }
      case STATE_ALARM: {
        // long beep
        tone(BEEP_PIN, 2048, 500);
        break;
      }
      case STATE_SENSOR_ERROR: {
        // short beep
        tone(BEEP_PIN, 2048, 50);
      }
    }
  }

  if (heartbeatLed.poll(1000)) {
    digitalWrite(GREEN_LED_PIN, 1);
    delay(10);
    digitalWrite(GREEN_LED_PIN, 0);
  }
}

