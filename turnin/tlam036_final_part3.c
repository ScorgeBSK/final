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

enum States {init, waitReleaseOn, seq1, seq2, seq3, seq4, waitReleaseOff} state;
unsigned char led = 0;
void tick(){
	unsigned char button = ~PINA & 0x01;
	switch(state){
		case init:
			state = button ? waitReleaseOn : init;
			break;
		case waitReleaseOn:
			state = button ? waitReleaseOn : seq1;
			break;
		case seq1:
			state = button ? waitReleaseOff : seq2;
			break;
		case seq2:
			state = button ? waitReleaseOff : seq3;
			break;
		case seq3:
			state = button ? waitReleaseOff : seq4;
			break;
		case seq4:
			state = button ? waitReleaseOff : seq1;
			break;
		case waitReleaseOff:
			state = button ? waitReleaseOff : init;
			break;
		default:
			state = init;
			break;
	}

	switch(state){
		case init:
			led = 0;
			if(USART0_IsSendReady()){
				USART0_Send(led);
			}
			break;
		case waitReleaseOn:
			break;
		case seq1:
			led = 1;
			if(USART0_IsSendReady()){
				USART0_Send(led);
			}
			break;
		case seq2:
                        led = 2;
                        if(USART0_IsSendReady()){
                                USART0_Send(led);
                        }
                        break;
		case seq3:
                        led = 4;
                        if(USART0_IsSendReady()){
                                USART0_Send(led);
                        }
                        break;
		case seq4:
                        led = 8;
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
