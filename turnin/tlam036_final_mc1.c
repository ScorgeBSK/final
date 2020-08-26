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

enum States {off, on} state;
unsigned char led = 0;
void tick(){
	switch(state){
		case off:
			state = on;
			break;
		case on:
			state = off;
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
		case on:
			led = 1;
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
    DDRB = 0xFF; PORTB = 0x00;

    initUSART0();

    TimerSet(1000);
    TimerOn();

    /* Insert your solution below */
    while (1) {
    	tick();
	while(!TimerFlag) {};
	TimerFlag = 0;
    }
    return 1;
}
