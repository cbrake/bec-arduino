/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

#define PIEZO_OUTPUT 12

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PIEZO_OUTPUT, OUTPUT);
}

void beep() {
    for (int i=0; i<250; i++) {
      digitalWrite(PIEZO_OUTPUT, HIGH);   // turn the LED on (HIGH is the voltage level)
      delayMicroseconds(86);                       // wait for a second
      digitalWrite(PIEZO_OUTPUT, LOW);    // turn the LED off by making the voltage LOW
      delayMicroseconds(85); // wait for a second
    }
}

// play tone for ~0.25s
void beep_nice() {
  for (int y=0; y<15; y++) {
    beep();
    delay(1000);
  }
}

void beep_wake_up() {
  for (int y=0; y<35; y++) {
    beep();
    delay(400);
  }
}

void beep_i_said_WAKE_UP() {
  while (1) {
    beep();
    delay(100);
  }
}

// the loop function runs over and over again forever
void loop() {
  beep_nice();
  beep_wake_up();
  beep_i_said_WAKE_UP();
}
