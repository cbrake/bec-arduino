#include <STM32Sleep.h>
#include <RTClock.h>

RTClock rt(RTCSEL_LSE);
long int alarmDelay = 5;

void setup() {
  // We have just started or woken up from sleep! System clock is set to 72MHz HSE.
  Serial.begin(9600);
  delay(1000);
  Serial.println("hi there");
  sleepAndWakeUp(STANDBY, &rt, alarmDelay);  
}

void loop() { }  // This is never run


