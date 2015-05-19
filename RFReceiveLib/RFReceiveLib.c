#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "RFReceiveLib.h"
#include <util/delay.h>

#define LED PB5


volatile uint8_t receiving;
volatile uint16_t timerValue; // TIMER VALUE

volatile uint16_t data = 0;
volatile uint8_t bitsReceived = 0;
volatile uint8_t receivedIntroBits = 0;
volatile uint8_t permOn = 0;

volatile uint8_t currentPulseLengthTicks = 0;

void (*dataHandler)(uint8_t, uint8_t);


#define true 1
#define false 0

void beginReceiving() {
	// todo init
	// initUSART();
	// writeString("bitch begin");

	initINT0Interrupt();
	initTimer();
}

ISR(INTERRUPT_VECTOR) {
	if (COMPARE_VALUE) { // rising edge

		refreshCurrentPulseLength();

		uint8_t pulseAmount = getPulseAmount();

		if (pulseAmount == 9) {
			startReceiving();
		}

		if (receiving) {
			if (pulseAmount == 1 || pulseAmount == 3) {
				data = data << 1;
				if (pulseAmount == 3) {  // LOGICAL ONE
					data |= 1;
				}

				if (++bitsReceived == 16) {
					receiving = false;
					dataHandler(data >> 8, data);
				}
			}
		}
	} else { // falling edge
		resetTimer();
	}
}

void setDataHandler(void (*newDataHandler)(uint8_t, uint8_t)) {
	dataHandler = newDataHandler;
}

void startReceiving() {
	bitsReceived = 0;
	receiving = true;
	data = 0;
}

void refreshCurrentPulseLength() {
	currentPulseLengthTicks = TCNT0;
}

uint8_t getPulseAmount() {
	if (checkFuzzy(PULSE_LENGTH_T * 3, currentPulseLengthTicks))
		return 9;
	if (checkFuzzy(PULSE_LENGTH_T * 2, currentPulseLengthTicks))
		return 3;
	if (checkFuzzy(PULSE_LENGTH_T * 1, currentPulseLengthTicks))
		return 1;
	return 0;
}

uint8_t checkFuzzy(int16_t target, int16_t value) {
	return (value > target - MAX_DEV && value < target + MAX_DEV);
}

void initINT0Interrupt() {
	#ifdef MCU_atmega328p
		DDRD &= ~(1 << PD2);		// change line for attiny85
	#endif
	#ifdef MCU_attiny
		DDRB &= ~(1 << PB2);		// change line for attiny85
	#endif


	EIMSK |= (1 << INT0);		// enable INT0
	EICRA |= (1 << ISC00);		// trigger on voltage change

	sei();				// set enable interrupt
}

void initTimer() {
	TCCR0B = TIM0PRESCALER; 

	sei();
}

void resetTimer() {
	TCNT0 = 0;
}