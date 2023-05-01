// Author:      Jaeden Carpenter   
// Net ID:         carpenterjaeden
// Date: 3/20/23         
// Assignment:     Lab 3
//
// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <Arduino.h>
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PD2 and sets it into input mode
 */
void initSwitchPD2(){
  //data direction
DDRD &= ~(1 << PD2);
//port
PORTD |= (1 << PD2);

//Set the external interrupt control register A to 11 for INT2
EICRA |= (1<<ISC20) | (1<<ISC21);
//turn on INT2 in the external interrupt mask register to enable it
EIMSK |= (1<<INT2);





}



