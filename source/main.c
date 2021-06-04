/*	Name & E-mail: Karan Bhogal, kbhog001@ucr.edu
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: Stacker Game
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=V1IlX2fiThI
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
					PORTD = 0xF7;
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


unsigned int waitbool = 0;
enum bounce1 { b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, end };
int bounce(int state) {
	unsigned char tmpA = ~PINA;
	unsigned int cnt = 1;
        // Transitions
        switch (state) {

                case b1:
                        if ( tmpA == 0x01 ) { // Reset demo 
                                state = b5;
                        }
			else{
				state = b2;
			}
			break;
		
		case b2:
			if ( tmpA == 0x01 ) { // Reset demo 
                                state = b5;
                        }
                        else{
                                state = b3;
                        }
                        break;

		case b3:
			if ( tmpA == 0x01 ) {
				state = b5;
			}
			else{
				state = b4;
			}
			break;

		case b4:
			if ( tmpA == 0x01 ) { // Reset demo
                                state = b5;
                        }
                        else{
                                state = b1;
                        }
                        break;

		case b5:
			if (waitbool == 0) {
				state = b5;
			}
			else{
				cnt = cnt + 1;
				if (cnt >= 5){
					state = b6; 
				}
				else{
					state = b1;
				}
			}
			break;

		case b6:
			if ( tmpA == 0x01 ) { // Reset demo
                                if (cnt == 8){
                                        state = end;
                                }
				state = b5;
                        }
                        else{
                                state = b7;
                        }
                        break;

		case b7:
                        if ( tmpA == 0x01 ) { // Reset demo
                                if (cnt == 8){
                                        state = end;
                                }
				state = b5;
                        }
                        else{
                                state = b8;
                        }
                        break;

		case b8:
                        if ( tmpA == 0x01 ) { // Reset demo
                                if (cnt == 8){
                                        state = end;
                                }
				state = b5;
                        }
                        else{
                                state = b9;
                        }
                        break;

		case b9:
                        if ( tmpA == 0x01 ) { // Reset demo
                                if (cnt == 8){
                                        state = end;
                                }
				state = b5;
                        }
                        else{
                                state = b10;
                        }
                        break;

		case b10:
                        if ( tmpA == 0x01 ) { // Reset demo
				if (cnt == 7){
                                        state = end;
                                }
                                state = b5;
                        }
                        else{
                                state = b11;
                        }
                        break;

		case b11:
                        if ( tmpA == 0x01 ) { // Reset demo
				if (cnt >= 8){
					state = end;
				}
                                state = b5;
                        }
                        else{
                                state = b6;
                        }
                        break;

		case end:
			state = end;

                default:
                        state = b1;
                        break;
        }
        // Actions
        switch (state) {
                case b1:
			display[1][8] = 1;
			display[2][8] = 1;
			display[3][8] = 1;
			display[4][8] = 0;
			display[5][8] = 0;
                        break;

		case b2:
                        display[1][8] = 0;
                        display[2][8] = 1;
                        display[3][8] = 1;
                        display[4][8] = 1;
                        display[5][8] = 0;
                        break;

		case b3:
                        display[1][8] = 0;
                        display[2][8] = 0;
                        display[3][8] = 1;
                        display[4][8] = 1;
                        display[5][8] = 1;
                        break;

		case b4:
                        display[1][8] = 0;
                        display[2][8] = 1;
                        display[3][8] = 1;
                        display[4][8] = 1;
                        display[5][8] = 0;
                        break;

		case b5:
			/*display[1][8] = 0;
                        display[2][8] = 0;
                        display[3][8] = 0;
                        display[4][8] = 0;
                        display[5][8] = 0*/
			break;
		case b6:
                        display[1][8] = 1;
                        display[2][8] = 1;
                        display[3][8] = 0;
                        display[4][8] = 0;
                        display[5][8] = 0;
                        break;

		case b7:
                        display[1][8] = 0;
                        display[2][8] = 1;
                        display[3][8] = 1;
                        display[4][8] = 0;
                        display[5][8] = 0;
                        break;

		case b8:
                        display[1][8] = 0;
                        display[2][8] = 0;
                        display[3][8] = 1;
                        display[4][8] = 1;
                        display[5][8] = 0;
                        break;

		case b9:
                        display[1][8] = 0;
                        display[2][8] = 0;
                        display[3][8] = 0;
                        display[4][8] = 1;
                        display[5][8] = 1;
                        break;

		case b10:
                        display[1][8] = 0;
                        display[2][8] = 0;
                        display[3][8] = 1;
                        display[4][8] = 1;
                        display[5][8] = 0;
                        break;

		case b11:
                        display[1][8] = 0;
                        display[2][8] = 1;
                        display[3][8] = 1;
                        display[4][8] = 0;
                        display[5][8] = 0;
                        break;

		case end:
			if ( display[1][8] == 1) {
				if ( display[1][7] == 0){
					display[1][8] = 0;
				}
                        }
                        if ( display[2][8] == 1) {
                        	if ( display[2][7] == 0){
                                        display[2][8] = 0;
                                }
                        }
                        if ( display[3][8] == 1) {
				if ( display[3][7] == 0){
                                        display[3][8] = 0;
                                }
                        }
			if ( display[4][8] == 1) {
				if ( display[4][7] == 0){
                                        display[4][8] = 0;
                                }

                        }
			if ( display[5][8] == 1) {
				if ( display[5][7] == 0){
                                        display[5][8] = 0;
                                }
                        }
			
			break;

                default:
        	break;
        }

        return state;
}


unsigned int c1 = 1;
unsigned int c2 = 8;
unsigned int c3 = 1;
unsigned int i;
unsigned int m;
unsigned int n;

enum Slide_States { Wait, Press, Fall, Press2, Fall2, Press3, Press4, Press5, Press6  };
int Slide(int state) {
	unsigned char tmpA = ~PINA;



        // Transitions
        switch (state) {
                case Wait:
			if ( tmpA == 0x01 ){
				waitbool = 0;
				if (c3 == 1){
					state = Press;
				}
				else if (c3 == 2){
					c1 = 1;
					c2 = 8;
					state = Press2;
				}
				else if (c3 == 3){
					c1 = 1;
					c2 = 8;
					state = Press3;
				}
				else if (c3 == 4) {
					c1 = 1;
					c2 = 8;
					state = Press4;
				}
				else if (c3 == 5) {
					c1 = 1;
					c2 = 8;
					state = Press5;
				}
				else if (c3 == 6){
					c1 = 1;
					c2 = 8;
					state = Press6;
				}
				else if (c3 == 7){
					state = Fall;
				}
				
			}
			else{
				state = Wait;
			}
                        break;

		case Press:
			if ( c1 != 7 ){
				state = Press;
			}
			else {
				waitbool = 1;
				c3 = 2;
				state = Fall;

			}
			break;

		case Fall:
			state = Wait;
			break;

		case Press2:
			if ( c1 != 6 ){
                                state = Press2;
                        }
                        else {
                                waitbool = 1;
                                c3 = 3;
                                state = Fall;

                        }
			break;

		//case Fall2:
		//	state = Wait;
		//	break;

		case Press3:
                        if ( c1 != 5 ){
                                state = Press3;
                        }
                        else {
                                waitbool = 1;
                                c3 = 4;
                                state = Fall;

                        }
                        break;

		case Press4:
                        if ( c1 != 4 ){
                                state = Press4;
                        }
                        else {
                                waitbool = 1;
                                c3 = 5;
                                state = Fall;

                        }
                        break;

		case Press5:
                        if ( c1 != 3 ){
                                state = Press5;
                        }
                        else {
                                waitbool = 1;
                                c3 = 6;
                                state = Fall;

                        }
                        break;

		case Press6:
                        if ( c1 != 2 ){
                                state = Press6;
                        }
                        else {
                                waitbool = 1;
                                c3 = 7;
                                state = Fall;

                        }
                        break;

		/*case Press7:
                        waitbool = 1;
                        c3 = 8;
                        state = Fall;
                        break;
		*/
                default:
                        state = Wait;
                        break;
        }
        // Actions
        switch (state) {
                case Wait:
			break;

		case Press:
			if ( display[1][c2] == 1){
				i = 1;
				m = 2;
				n = 3;
			}

			else if ( display[2][c2] == 1){
				i = 2;
				m = 3;
				n = 4;
			}

			else if ( display[3][c2] == 1){
				i = 3;
				m = 4;
				n = 5;
			}
			display[i][c2] = 0;
			display[m][c2] = 0;
			display[n][c2] = 0;

			c2 = c2 - 1;
                        
			display[i][c2] = 1;
			display[m][c2] = 1;
			display[n][c2] = 1;
			
			c1++;			
			break;

		case Fall:
			if ( display[i][c2-1] == 0) {
				display[i][c2] = 0;
			}
			if ( display[m][c2-1] == 0) {
				display[m][c2] = 0;
			}
			if ( display[n][c2-1] == 0) {
				display[n][c2] = 0;
			}
			break;

		case Press2:
                        if ( display[1][c2] == 1){
                                i = 1;
                                m = 2;
                                n = 3;
                        }

                        else if ( display[2][c2] == 1){
                                i = 2;
                                m = 3;
                                n = 4;
                        }

                        else if ( display[3][c2] == 1){
                                i = 3;
                                m = 4;
                                n = 5;
                        }
                        display[i][c2] = 0;
                        display[m][c2] = 0;
                        display[n][c2] = 0;

                        c2 = c2 - 1;

                        display[i][c2] = 1;
                        display[m][c2] = 1;
                        display[n][c2] = 1;

                        c1++;
                        break;

		/*case Fall2:
                        if ( display[i][c2-1] == 0) {
                                display[i][c2] = 0;
                        }
                        if ( display[m][c2-1] == 0) {
                                display[m][c2] = 0;
                        }
                        if ( display[n][c2-1] == 0) {
                                display[n][c2] = 0;
                        }
                        break;*/


		case Press3:
                        if ( display[1][c2] == 1){
                                i = 1;
                                m = 2;
                                n = 3;
                        }

                        else if ( display[2][c2] == 1){
                                i = 2;
                                m = 3;
                                n = 4;
                        }

                        else if ( display[3][c2] == 1){
                                i = 3;
                                m = 4;
                                n = 5;
                        }
                        display[i][c2] = 0;
                        display[m][c2] = 0;
                        display[n][c2] = 0;

                        c2 = c2 - 1;

                        display[i][c2] = 1;
                        display[m][c2] = 1;
                        display[n][c2] = 1;

                        c1++;
                        break;

		/*case Fall3:
                        if ( display[i][c2-1] == 0) {
                                display[i][c2] = 0;
                        }
                        if ( display[m][c2-1] == 0) {
                                display[m][c2] = 0;
                        }
                        if ( display[n][c2-1] == 0) {
                                display[n][c2] = 0;
                        }
                        break;*/


		case Press4:
                        if ( display[1][c2] == 1){
                                i = 1;
                                m = 2;
                                n = 3;
                        }

                        else if ( display[2][c2] == 1){
                                i = 2;
                                m = 3;
                                n = 4;
                        }

                        else if ( display[3][c2] == 1){
                                i = 3;
                                m = 4;
                                n = 5;
                        }
                        display[i][c2] = 0;
                        display[m][c2] = 0;
                        display[n][c2] = 0;

                        c2 = c2 - 1;

                        display[i][c2] = 1;
                        display[m][c2] = 1;
                        display[n][c2] = 1;

                        c1++;
                        break;


		case Press5:
                        if ( display[1][c2] == 1){
                                i = 1;
                                m = 2;
                        }

                        else if ( display[2][c2] == 1){
                                i = 2;
                                m = 3;
                        }

                        else if ( display[3][c2] == 1){
                                i = 3;
                                m = 4;
                        }
			else if ( display[4][c2] == 1){
				i = 4;
				m = 5;
			}

                        display[i][c2] = 0;
                        display[m][c2] = 0;
                        //display[k][c2] = 0;

                        c2 = c2 - 1;

                        display[i][c2] = 1;
                        display[m][c2] = 1;
                        //display[k][c2] = 1;

                        c1++;
                        break;

		case Press6:
                        if ( display[1][c2] == 1){
                                i = 1;
                                m = 2;
                        }

                        else if ( display[2][c2] == 1){
                                i = 2;
                                m = 3;
                        }

                        else if ( display[3][c2] == 1){
                                i = 3;
                                m = 4;
                        }
                        else if ( display[4][c2] == 1){
                                i = 4;
                                m = 5;
                        }

                        display[i][c2] = 0;
                        display[m][c2] = 0;

                        c2 = c2 - 1;

                        display[i][c2] = 1;
                        display[m][c2] = 1;

                        c1++;
                        break;

		/*case Press7:
                        if ( display[1][c2] == 1){
                                i = 1;
                                m = 2;
                        }

                        else if ( display[2][c2] == 1){
                                i = 2;
                                m = 3;
                        }

                        else if ( display[3][c2] == 1){
                                i = 3;
                                m = 4;
                        }
                        else if ( display[4][c2] == 1){
                                i = 4;
                                m = 5;
                        }
                        display[i][c2] = 1;
                        display[m][c2] = 1;

                        c1++;
                        break;*/

                default:
        	break;
        }


        return state;
}




int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

    /* Insert your solution below */



	static task task1, task2, task3;
	task *tasks[] = { &task1, &task2, &task3 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	task1.state = b1;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &bounce;
	
	task2.state = Wait;
	task2.period = 100;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Slide;

	task3.state = out1;
	task3.period = 1;
	task3.elapsedTime = task3.period;
	task3.TickFct = &exer_1;

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
	display[2][1] = 1;
	display[3][1] = 1;
	display[4][1] = 1;
	display[5][1] = 0;


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
