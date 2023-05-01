#include <avr/io.h>
#include <util/delay.h>
#include "RFID.h"
#include <Wire.h>
#include <Arduino.h>
#include "I2C.h"

/*      RFID DataSheet : 
        https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf
*/

volatile bool tag = false;

void initRFID(){
    // initialize I2C interface (SDA and SCL pins)
    Wire.begin();

    writeRegister(0x01, 0x00);
    // CommandReg register, Idle mode

    writeRegister(0x0C, 0x00);
    // TxControl register, Normal operation

    writeRegister(0x014, 0x80);
    // TModeReg register, Set timer to start when data is received

    writeRegister(0x2A, 0x00);
    // TxASKReg register, Disables "ask" prompt

    writeRegister(0x02, 0x01);
    // ComIEnReg register, Enables RxIRQ interrupt
    
    pinMode(PORTH6, OUTPUT);
    digitalWrite(PORTH6, LOW);
    // holds RST pin low while interrupt pin is initialized (RST is connected to pin9 / H6)

    pinMode(PORTD3, INPUT_PULLUP);
    //sets PIND3 as the RxIRQ_PIN
    
    attachInterrupt(PORTD3, RxIRQ_ISR, FALLING);
    //data direction
// DDRD &= ~(1 << PD3);
// //port
// PORTD |= (1 << PD3);

// //Set the external interrupt control register A to 11 for INT2
// EICRA |=  (1<<ISC31);
// EICRA &= ~(1<<ISC30);
// //turn on INT2 in the external interrupt mask register to enable it
// EIMSK |= (1<<INT3);

}

void RxIRQ_ISR() {
    // set address to GPIO (general purpose input output)
    return;
    Wire.beginTransmission(0x28);
    Wire.write(0x09);
    Wire.endTransmission(false);

    // read output from GPIO
    Wire.requestFrom(0x28, 1, true);
    int val = Wire.read();
    val = (val >> PORTD3) & 0x01; // assigns val with the value from the sensor's interrupt pin

    // checks if the interrupt is from the RFID sensor
    if (val == 1) {
        readRFIDTag();
    }

}

void readRFIDTag() {
    Serial.begin(9600);
    writeRegister(0x01, 0x00);
    // set address to CommandReg
    // reset to Idle Mode
    
    writeRegister(0x04, 0x7F);
    // set address to ComIrqReg
    // Clear interrupt request bits
    
    writeRegister(0x0A, 0x80);
    // set address to the FIFO Level Register
    // flush the FIFO buffer to remove old data

    writeRegister(0x01, 0xC0);
    // set address to CommandReg
    // mode set to be able to transfer and receive data from FIFO buffer

    unsigned int buffer[64];    // variable to store tag data
    unsigned int dataRead = 0;  // stores how many bytes read
    unsigned int dataCount = 0; // stores how many bytes available in the buffer

    while (dataRead < 64 && dataCount != 48) {
        dataCount = readRegister(0x0A); // stores number of available bytes in the buffer into "dataCount"
        for (unsigned int i = 0; i < dataCount; i++) {
            buffer[dataRead++] = transceiveData(0x00); // stores the data read from transceiveData into the buffer array
        }
    }

    for (unsigned int i = 0; i < dataRead; i++) {
        if (i >= 2 && i <= 5) { // *From datasheet* Tag ID is stored in buffer from 3rd to 6th byte 
            Serial.print(buffer[i], HEX); // prints tag ID to serial monitor in HEX format
        }
    }

    Serial.println();
}


void writeRegister(unsigned int reg, unsigned int val) {
    Wire.beginTransmission(0x28); // opens I2C with RC522 address (0x28)
    Wire.write(reg);    // loads register address
    Wire.write(val);    // loads value into address
    Wire.endTransmission(); // ends transmission to RC522
}

unsigned int readRegister(unsigned int reg) {
    Wire.beginTransmission(0x28); // open I2C connection
    Wire.write(reg & 0x7F); // mask read-write bit
    Wire.write(0x80); // enable read mode
    Wire.endTransmission();

    Wire.requestFrom(0x28, 1); // requests data to be read from the RC522
    unsigned int val = Wire.read(); // actually reads the value into "val"
    return val;
}

unsigned int transceiveData(unsigned int data) {
    Wire.beginTransmission(0x28); //open I2C connection
    Wire.write(0x0C);   // command to enable transmit or receiving of data from RC522
    Wire.write(data);   // saves the data that is being written or received
    Wire.endTransmission();

    Wire.requestFrom(0x28, 1);
    unsigned int output = Wire.read(); // reads the received data into output
    return output;
}