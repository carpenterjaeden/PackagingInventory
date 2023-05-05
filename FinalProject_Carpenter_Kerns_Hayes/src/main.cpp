// Author: Jaeden Carpenter, Sam Kerns, Malcolm Hayes
// Net ID: carpenterjaeden , s4k3 , malcolmhayes
// Date: 3/20/23
// Assignment: Final Project
//----------------------------------------------------------------------//
//
// 
// 
//----------------------------------------------------------------------//
/*
 * Pin layout should be as follows for the MFRC522 board:
 * Signal     Pin              Pin               
 *            Arduino Mega     MFRC522 board
 * -----------------------------------------------
 * Reset      46               RST
 * SPI SS     53               SDA
 * SPI MOSI   51               MOSI
 * SPI MISO   50               MISO
 * SPI SCK    52               SCK
 *
 * 
 */


#include <Arduino.h>
#include <avr/io.h>
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "I2C.h"
#include "spi2.h"
#include "RFID.h"
#include <MFRC522.h>
#include "lcd.h"
 
#define SS_PIN 53
#define RST_PIN 46
MFRC522 mfrc522(SS_PIN, RST_PIN);
// defines



// Define a set of states that can be used in the state machine using an enum.
typedef enum {incoming, outgoing} states;
states matrix = incoming;

//states that make sure each card is read only once before it it moved from the sensor
typedef enum {cardPresent, cardNotPresent} cardCheck;
cardCheck cardState = cardNotPresent;

typedef enum {wait_press, debounce_press, wait_release, debounce_release} debounce;
//define global variable for debounce states
volatile debounce dbState = wait_press;

unsigned int num = 0;



void setup(){

  Serial.begin(9600);  
  sei(); // Enable global interrupts.
  SPI_MASTER_Init();
	mfrc522.PCD_Init();	// Init MFRC522 card
  initTimer1();
  initSwitchPD2();
  initLCD();

  Serial.println("Ta");

}



void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (cardState == cardNotPresent){
      if (matrix == incoming) num++;
      else num--;
        Serial.println(num);
        cardState = cardPresent;
    } 

	}
  else if(!mfrc522.PICC_IsNewCardPresent()){
    cardState = cardNotPresent;
  }

    switch (matrix){
      case incoming:  // object is entering into the inventory
        moveCursor(0,0);
        writeString("Incoming");
        moveCursor(1,0);
        writeString("Amount: ");
        writeNum(num);
      break;
      case outgoing: // object is being removed from inventory
        moveCursor(0,0);
        writeString("Outgoing");
        moveCursor(1,0);
        writeString("Amount: ");
        writeNum(num);
      break;
      default:
        matrix = incoming;
      break;
    }



  //Switch case for debounce states
  switch(dbState){
  //do nothing while waiting
    case wait_press:
    break;
  //debounce press adds delay and goes to wait_release
    case debounce_press:
    delayMs(1);
    dbState = wait_release;
    break;
  //Do nothing while waiting
    case wait_release:
    break;
  //After release, delay and then go back to waiting for press
    case debounce_release:
    delayMs(1);
    dbState = wait_press;
    //switches states
    if (matrix == outgoing) {
      matrix = incoming;
    } else {
      matrix = outgoing;
    }
    break;

  }

	}



ISR(INT2_vect){

    //if INT0 is triggered for press
if (dbState == wait_press){
  dbState = debounce_press;
}
//if INT0 is triggered for release
else if (dbState == wait_release){
  //change motor state to counting
  dbState = debounce_release;
}
}
  




/* READ COORD FROM ACC using I2C
    //Read X
    Read_from(117, 59);
    unsigned int x = Read_data();
    Read_from(117, 60);
    x = (x<<8) + Read_data();
    //Read Y
    Read_from(117, 61);
    unsigned int y = Read_data();
    Read_from(117, 62);
    y = (y<<8) + Read_data();
    //Read Z
    Read_from(117, 63);
    unsigned int z = Read_data();
    Read_from(117, 64);
    z = (z<<8) + Read_data();
    //Serial prints
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
   //if reaches threshhold, trigger frown

    if ((y > 350) | (z > 350)){
      matrix = frown;
    }
*/

/* Switch-cases for using SPI and 8x8 display
case smile:
        displaySmile();
        muteflag = 0;
      break;
      case frown:
        displayFrown();
        //make the alarm chirp
        if (muteflag == 0){
        changeDutyCycle(350);
        }
        else{
          changeDutyCycle(0);
        }
      break;
      default:
      matrix = smile;
      break;
*/