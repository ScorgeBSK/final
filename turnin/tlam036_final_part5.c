/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "usart.h"

void set_PWM(double frequency){
	static double current_frequency;

	if(frequency != current_frequency){
		if(!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);

	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States {init, decrement, increment, reset, waitRelease} state;
unsigned char led = 0;
signed char i = 0;

void tick(){
	unsigned char button = ~PINA & 0x03;

	switch(state){
		case init:
			if(button == 0x01){
				state = decrement;
			}
			else if(button == 0x02){
				state = increment;
			}
			else if(button == 0x03){
				state = reset;
			}
			else {
				state = init;
			}
			break;
		case decrement:
			state = waitRelease;
			break;
		case increment:
			state = waitRelease;
			break;
		case reset:
			state = waitRelease;
			break;
		case waitRelease:
			if(button == 0x01){
				state = waitRelease;
			}
			else if(button == 0x02){
				state = waitRelease;
			}
			else if(button == 0x03){
				state = waitRelease;
			}
			else{
				state = init;
			}
			break;
		default:
			state = init;
			break;
	}

	switch(state){
		case init:
			break;
		case waitRelease:
			break;
		case decrement:
			led = --i;
			if(i < 0){
				led = 0;
			}
			if(USART0_IsSendReady()){
				USART0_Send(led);
			}
			break;
		case increment:
			led = ++i;
			if(i > 8){
				led = 8;
			}
                        if(USART0_IsSendReady()){
                                USART0_Send(led);
                        }
                        break;
		case reset:
			led = 0;
			if(USART0_IsSendReady()){
				USART0_Send(led);
			}
			break;
		default:
			break;
	}

	PORTB = led;

}

int main(void) {
    /* Insert DDR and PORT initializations */
    	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

    initUSART0();

    /* Insert your solution below */
    while (1) {
    	tick();
    }
    return 1;
}
