/*
 * Piezo driver using STM32 Blue pill board
 * 
 * to use, open up serial console, and line ending to Carriage return.
 * command format:
 *   run <rising_us> <dwell_us> <falling_us> <duty_perc> <cmd_delay_us> <count>
 * example
 *   run 60 100 40 60
 */

#include "Cmd.h"

#define BAUD 9600

const int pwmOutPin = PA1; // pin10
HardwareTimer pwmtimer(2);

uint16 maxduty, duty;  
uint32 period, mypulse;

enum {
	STATE_IDLE,
	STATE_RISING,
	STATE_DWELL,
	STATE_FALLING,
	STATE_DELAY,
};

// variables used to track state of running program
int state = STATE_IDLE;
int current_count = 0;
int state_elapsed_time = 0;

// params
int rising_us = 0;
int dwell_us = 0;
int falling_us = 0;
int duty_perc = 0;
int cmd_delay_us = 0;
int count = 0;


void set_pwm(int duty) {
	//pwmtimer.pause();
	pwmtimer.setCompare(TIMER_CH2, duty);  //Pulse width
	pwmtimer.refresh();
	//pwmtimer.resume();
}

int handler_toggle = 0;

void set_state(int new_state) {
	state_elapsed_time = 0;
	state = new_state;
	int duty;

	// handle processing required at beging of each state
	switch (state) {
		case STATE_IDLE:
			pwmtimer.pause();
			break;
		case STATE_RISING:
			duty = calc_ramp_pwm(state_elapsed_time, rising_us, true, duty_perc);
			set_pwm(duty);
			pwmtimer.resume();
			break;
		case STATE_DWELL:
			set_pwm(duty_perc);
			break;
		case STATE_FALLING:
			duty = calc_ramp_pwm(state_elapsed_time, falling_us, false, duty_perc);
			set_pwm(duty);
			break;
		case STATE_DELAY:
			pwmtimer.pause();
			break;
	}
}

int calc_ramp_pwm(int current_time, int ramp_width, bool up, int duty_perc) {
	int t = up ? current_time : (ramp_width - current_time);
	if (up)
		t += 10;

	return duty_perc * t/(ramp_width + 10);
}

void test_calc_pwm_ramp() {
	int ramp_width = 60;
	int current_time = 0;

	Serial.println("test ramp up calculations (ramp time = 60, duty = 100%)");

	while (current_time <= ramp_width) {
		Serial.print("time ");
		Serial.print(current_time);
		Serial.print(" => ");
		Serial.print(calc_ramp_pwm(current_time, ramp_width, true, 100));
		Serial.println("%");
		current_time += 10;
	}

	Serial.println("test ramp down calculations (ramp time = 70, duty = 50%)");

	ramp_width = 70;
	current_time = 0;
	while (current_time <= ramp_width) {
		Serial.print("time ");
		Serial.print(current_time);
		Serial.print(" => ");
		Serial.print(calc_ramp_pwm(current_time, ramp_width, false, 50));
		Serial.println("%");
		current_time += 10;
	}
}

void handler(void) {
	state_elapsed_time += 10;
	handler_toggle = !handler_toggle;
	digitalWrite(PA2, handler_toggle);

	switch (state) {
		case STATE_IDLE:
			// do nothing
			break;
		case STATE_RISING:
			if (state_elapsed_time >= rising_us) {
				set_state(STATE_DWELL);
			} else {
				// ramp up PWM
				int duty = calc_ramp_pwm(state_elapsed_time, rising_us, true, duty_perc);
				set_pwm(duty);
			}
			break;
		case STATE_DWELL:
			if (state_elapsed_time >= dwell_us) {
				set_state(STATE_FALLING);
			}
			break;
		case STATE_FALLING:
			if (state_elapsed_time >= falling_us) {
				pwmtimer.pause();
				current_count += 1;
				if (current_count < count) {
					set_state(STATE_DELAY);
				} else {
					set_state(STATE_IDLE);
				}
			} else {
				// ramp down PWM
				int duty = calc_ramp_pwm(state_elapsed_time, falling_us, false, duty_perc);
				set_pwm(duty);
			}
			break;
		case STATE_DELAY:
			if (state_elapsed_time >= cmd_delay_us) {
				set_state(STATE_RISING);
			}
			break;
	}

	if (state == STATE_IDLE) {
		digitalWrite(PA3, 0);
	} else {
		digitalWrite(PA3, 1);
	}
}

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(BAUD);
	cmdInit(&Serial);

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(PC13, OUTPUT);
	pinMode(PA2, OUTPUT);  // toggle in handler
	pinMode(PA3, OUTPUT);  // high during command

	pwmtimer.pause();
	pwmtimer.setPrescaleFactor(1);        //Prescaler
	pwmtimer.setOverflow(98);            //Period width
	pwmtimer.refresh();
	pinMode(pwmOutPin, PWM);

	Timer3.pause();
	Timer3.setCount(0);
	Timer3.setOverflow(351);
	Timer3.attachCompare1Interrupt(handler);
	Timer3.resume();  

	cmdAdd("run", run);

	// this delay is required to give serial console time to open with Arduino env
	// may not be required for a normal serial console
	delay(600);

	Serial.println("Piezo driver");

	test_calc_pwm_ramp();
}

// the loop function runs over and over again forever
void loop() {
	/*
	digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(50);                       // wait for a second
	digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
	delay(50);                       // wait for a second
	if (state != STATE_IDLE) {
		Serial.print("state: ");
		Serial.println(state);
	}
	*/

	if (state == STATE_IDLE) {
		cmdPoll();
	}
}

void run(int arg_cnt, char **args)
{
	if (arg_cnt < 7) {
		Serial.println("Usage: run <rising_us> <dwell_us> <falling_us> <duty_perc> <cmd_delay_us> <count>");
		return;
	}

	rising_us = cmdStr2Num(args[1], 10);
	dwell_us = cmdStr2Num(args[2], 10);
	falling_us = cmdStr2Num(args[3], 10);
	duty_perc = cmdStr2Num(args[4], 10);
	cmd_delay_us = cmdStr2Num(args[5], 10);
	count = cmdStr2Num(args[6], 10);

	// we can't print stuff here because it messed up the timing due to serial activity

	/*
	Serial.print("rising: ");
	Serial.println(rising_us);
	Serial.print("dwell: ");
	Serial.println(dwell_us);
	Serial.print("falling: ");
	Serial.println(falling_us);
	Serial.print("duty: ");
	Serial.println(duty_perc);
	*/

	Timer3.pause();
	Timer3.setCount(0);
	current_count = 0;
	set_state(STATE_RISING);
	digitalWrite(PA3, 1);
	Timer3.resume();  
}
