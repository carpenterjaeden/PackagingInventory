// Author:      Jaeden Carpenter   
// Net ID:         carpenterjaeden
// Date: 3/20/23         
// Assignment:     Lab 3
//
// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PD0 and sets it into input mode
 */
void initSwitchPD0(){
  //data direction
DDRD &= ~(1 << PD0);
//port
PORTD |= (1 << PD0);

//Set the external interrupt control register A to 11 for INT0
EICRA |= (1<<ISC00) | (1<<ISC01);
//turn on INT0 in the external interrupt mask register to enable it
EIMSK |= (1<<INT0);




}
