/*
 * Piezo driver using STM32 Blue pill board
 * 
 * to use, open up serial console, and line ending to Carriage return.
 * command format:
 *   run <rising_us> <dwell_us> <falling_us> <duty_perc>
 * example
 *   run 60 100 40 60
 */

#include <Cmd.h>

#define BAUD 9600

const int pwmOutPin = PA1; // pin10
HardwareTimer pwmtimer(2);

uint16 maxduty, duty;  
uint32 period, mypulse;

int state = 0;

// params
int rising_us = 0;
int dwell_us = 0;
int falling_us = 0;
int duty_perc = 0;

void handler(void) {
  digitalWrite(PA2, state);
  state = !state;
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(BAUD);
  cmdInit(&Serial);

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

  cmdAdd("run", run);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  delay(50);                       // wait for a second
  cmdPoll();
}

void run(int arg_cnt, char **args)
{
	if (arg_cnt < 5) {
		Serial.println("Usage: run <rising_us> <dwell_us> <falling_us> <duty_perc>");
		return;
	}

	rising_us = cmdStr2Num(args[1], 10);
	dwell_us = cmdStr2Num(args[2], 10);
	falling_us = cmdStr2Num(args[3], 10);
	duty_perc = cmdStr2Num(args[4], 10);

	Serial.print("rising: ");
	Serial.println(rising_us);
	Serial.print("dwell: ");
	Serial.println(dwell_us);
	Serial.print("falling: ");
	Serial.println(falling_us);
	Serial.print("duty: ");
	Serial.println(duty_perc);
}
