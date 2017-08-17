/*
 * test PWM on STM32 Blue Pill board
*/

#define BAUD 9600

const int pwmOutPin = PA1; // pin10
HardwareTimer pwmtimer(2);

uint16 maxduty, duty;  
uint32 period, mypulse;

int state = 0;

void handler(void) {
  digitalWrite(PA2, state);
  state = !state;
}

// the setup function runs once when you press reset or power the board
void setup() {
  //Serial.begin(BAUD);
  //Serial.println("test");
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PC13, OUTPUT);
  pinMode(PA2, OUTPUT);

  pwmtimer.pause();
  pwmtimer.setPrescaleFactor(1);        //Prescaler
  pwmtimer.setOverflow(98);            //Period width
  pwmtimer.setCompare(TIMER_CH2, 50);  //Pulse width
  pwmtimer.refresh();
  pwmtimer.resume();
  pinMode(pwmOutPin, PWM);

  Timer3.pause();
  Timer3.setCount(0);
  Timer3.setOverflow(360);
  Timer3.attachCompare1Interrupt(handler);
  Timer3.resume();  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  //Serial.println("loop");
}
