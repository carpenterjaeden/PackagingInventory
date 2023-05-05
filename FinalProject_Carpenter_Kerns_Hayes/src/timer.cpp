// Author: Jaeden Carpenter
// Net ID: carpenterjaeden
// Date: 3/20/23
// Assignment: Lab 3
//----------------------------------------------------------------------//

#include "timer.h"
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/


void initTimer1(){

    // sets CTC mode (010)
	TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |=  (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);

    // prescaler to 8
    TCCR1B &= ~(1 << CS10);
    TCCR1B |=  (1 << CS11);
    TCCR1B &= ~(1 << CS12);

    // set output compare to register 2
    OCR1AH = 0;
    OCR1AL = 2;
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay){
    unsigned int delayCount = 0;

    TCNT1 = 0;
    TIFR1 |= (1 << OCF1A);
    while (delayCount < delay) {
        if (TIFR1 & (1 << OCF1A)) {
            delayCount++;
            TIFR1 |= (1 << OCF1A);
        }
    }
}

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){

    // sets CTC mode (010)
	TCCR0A &= ~(1 << WGM00);
    TCCR0A |=  (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // prescaler 64
    TCCR0B |=  (1 << CS01) | (1 << CS00);
    TCCR0B &= ~(1 << CS02);
    OCR0A = 250;
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
    unsigned int delayCnt = 0;

    TCNT0 = 0;
    TIFR0 |= (1 << OCF0A);

    while (delayCnt < delay) {
        if (TIFR0 & (1 << OCF0A)) {
            delayCnt++;
            TIFRO |= (1 << OCF0A);
        }
    }
}