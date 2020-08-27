/*	Author: Trung Lam
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Final Project  Exercise #2 (Primary Microcontroller)
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "usart.h"

enum States {off, on, waitReleaseOn, waitReleaseOff} state;
unsigned char led = 0;
void tick(){
	unsigned char button = ~PINA & 0x01;
	switch(state){
		case off:
			state = button ? waitReleaseOn : off;
			break;
		case waitReleaseOn:
			state = button ? waitReleaseOn : on;
			break;
		case on:
			state = button ? waitReleaseOff : on;
			break;
		case waitReleaseOff:
			state = button ? waitReleaseOff : off;
			break;
		default:
			state = off;
			break;
	}

	switch(state){
		case off:
			led = 0;
			if(USART0_IsSendReady()){
				USART0_Send(led);
			}
			break;
		case waitReleaseOn:
			break;
		case on:
			led = 1;
			if(USART0_IsSendReady()){
				USART0_Send(led);
			}
			break;
		case waitReleaseOff:
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

    TimerSet(100);
    TimerOn();

    /* Insert your solution below */
    while (1) {
    	tick();
	while(!TimerFlag) {};
	TimerFlag = 0;
    }
    return 1;
}
