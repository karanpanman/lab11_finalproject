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
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;

unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned long int findGCD (unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a=b;
		b=c;
	}
	return 0;
}

typedef struct task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

enum Demo_States {shift} state;
int Demo_Tick(int state) {
	static unsigned char pattern = 0x80;
	static unsigned char row = 0xFE; 

	// Transitions
	switch (state) {
		case shift:	
			break;
		default:	
			state = shift;
			break;
	}	
	// Actions
	switch (state) {
		case shift:	
			if (row == 0xEF && pattern == 0x01) { // Reset demo 
				pattern = 0x80;		    
				row = 0xFE;
			} else if (pattern == 0x01) { // Move LED to start of next row
				pattern = 0x80;
				row = (row << 1) | 0x01;
			} else { // Shift LED one spot to the right on current row
				pattern >>= 1;
			}
			break;
		default:
	break;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
	return state;
}


unsigned short display[5][8];
int j;
int k;




enum Exer_States { out1, out2, out3, out4, out5 };
int exer_1(int state) {

        // Transitions
        switch (state) {
                case out1:
			state = out2;
                        break;

		case out2:
			state = out3;
			break;

		case out3:
			state = out4;
			break;

		case out4:
			state = out5;
			break;

		case out5:
			state = out1;
			break;

                default:
                        state = out1;
                        break;
        }
        // Actions
        switch (state) {
		unsigned char num;
                case out1:
			num = 0x00;
                	for ( k = 1; k < 9; k++){
                        	if (display[1][k] == 1){
					PORTD = 0xFE;
                                	num = num | (0x01 << (k - 1)) ;
                        	}
                	}
			PORTC = num;
                        break;

		case out2:
			num = 0x00;
                        for ( k = 1; k < 9; k++){
                                if (display[2][k] == 1){
					PORTD = 0xFD;
                                        num = num | (0x01 << (k - 1)) ;
                                }
                        }
			PORTC = num;
                        break;

		case out3:
			num = 0x00;
                        for ( k = 1; k < 9; k++){
                                if (display[3][k] == 1){
					PORTD = 0xFB;
                                        num = num | (0x01 << (k - 1)) ;
                                }
                        }
			PORTC = num;
                        break;

		case out4:
			num = 0x00;
                        for ( k = 1; k < 9; k++){
                                if (display[4][k] == 1){
					PORTD = 0xFE;
                                        num = num | (0x01 << (k - 1)) ;
                                }
                        }
			PORTC = num;
                        break;

		case out5:
			num = 0x00;
                        for ( k = 1; k < 9; k++){
                                if (display[5][k] == 1){
					PORTD = 0xEF;
					num = num | (0x01 << (k - 1)) ;
                                        
                                }
                        }
			PORTC = num;
                        break;


                default:
        	break;
        }


        return state;
}







int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

    /* Insert your solution below */
	//TimerSet(100);
	//TimerOn();
	//state = shift;
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	task1.state = out1;
	task1.period = 1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &exer_1;

	unsigned short i;
	unsigned long GCD = tasks[0] -> period;
        for (i = 1;i < numTasks; i++){
                GCD = findGCD(GCD, tasks[i]->period);
        }
	
	TimerSet(GCD);
	TimerOn();

	for( j = 1; j < 6; j++){
        	for ( k = 1; k < 9; k++){
                	display[j][k] = 0;
        	}
	}

	PORTD = 0xFF;
	display [1][1] = 1;
	display [2][6] = 1;
	display [3][8] = 1;
	display [5][4] = 1;
	display [5][5] = 1;
	display [5][6] = 1;

    while (1) {
	for ( i = 0; i < numTasks; i++) {
		if ( tasks[i] ->elapsedTime >= tasks[i]->period ) {
			tasks[i] -> state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i] -> elapsedTime += GCD;
	}
	while (!TimerFlag);
	TimerFlag = 0;
	
    }
    return 1;
}
