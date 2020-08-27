/*	Author: Trung Lam
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Final Project  Exercise #4 (Primary Microcontroller)
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "usart.h"

enum States {init, waitReleaseOn, playSeq, playSeqR, playBlink, waitReleaseOff} state;
const char sequence[4] = {0x01, 0x02, 0x04, 0x08};	//First Sequence
const char seqReverse[4] = {0x08, 0x04, 0x02, 0x01};	//Second Sequence
const char blinkingSeq[2] = {0x0F, 0x00};		//Third Sequence
unsigned char led = 0;
unsigned char i = 0;
unsigned char j = 0;
unsigned char k = 0;

void tick(){
	unsigned char button = ~PINA & 0x03;

	switch(state){
		case init:
			if(button == 0x01){
				state = waitReleaseOn;
			}
			else if(button == 0x02){
				state = waitReleaseOn;
			}
			else if(button == 0x03){
				state = waitReleaseOn;
			}
			else {
				state = init;
			}
			break;
		case waitReleaseOn:
			if(button == 0x01){
				state = playSeq;
			}
			else if(button == 0x02){
				state = playSeqR;
			}
			else if(button == 0x03){
				state = playBlink;
			}
			else {
				state = waitReleaseOn;
			}
			break;
		case playSeq:
			if(button == 0x01){
				state = waitReleaseOff;
			}
			else{
				state = playSeq;
				++i;
			}

			if(i >= 4){
				i = 0;
			}	
			break;
		case playSeqR:
                        if(button == 0x02){
                                state = waitReleaseOff;
                        }
                        else{
                                state = playSeqR;
                                ++j;
                        }

                        if(j >= 4){
                                j = 0;
                        }
                        break;
		case playBlink:
			if(button == 0x03){
				state = waitReleaseOff;
			}
			else{
				state = playBlink;
				++k;
			}

			if(k >= 2){
				k = 0;
			}
			break;
		case waitReleaseOff:
			if(button == 0x01){
				state = waitReleaseOff;
			}
			else if(button == 0x02){
				state = waitReleaseOff;
			}
			else if(button == 0x03){
				state = waitReleaseOff;
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
			led = 0;
			i = 0;
			j = 0;
			k = 0;
			if(USART0_IsSendReady()){
				USART0_Send(led);
			}
			break;
		case waitReleaseOn:
			break;
		case playSeq:
			led = sequence[i];
			if(USART0_IsSendReady()){
				USART0_Send(led);
			}
			break;
		case playSeqR:
                        led = seqReverse[j];
                        if(USART0_IsSendReady()){
                                USART0_Send(led);
                        }
                        break;
		case playBlink:
			led = blinkingSeq[k];
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
