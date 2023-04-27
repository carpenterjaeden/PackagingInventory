#include <avr/io.h>
#include <util/delay.h>
#include "RFID.h"
#include <Wire.h>



void initRFID(){
    // initialize I2C interface
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
}

void RxIRQ_ISR() {
    // set address to GPIO (general purpose input output)
    Wire.beginTransmission(0x28);
    Wire.write(0x0A);
    Wire.endTransmission(false);

    // read output from GPIO
    Wire.requestFrom(0x28, 1, true);
    int val = Wire.read();
    val = (val >> RxIRQ_PIN) & 0x01; // RxIRQ_PIN is the pin on the microcontroller

    // checks if the interrupt is from the RFID sensor
    if (val == 1) {
        readRFIDTag();
    }

}

unsigned int readRFIDTag() {
    writeRegister(0x01, 0x00);
    // set address to CommandReg
    // reset to Idle Mode
    
    writeRegister(0x04, 0x7F);
    // set address to ComIrqReg
    // Clear interrupt request bits
    
    writeRegister(0x0A, 0x80);
    // set address to the FIFO Level Register
    // flush the FIFO buffer to remove old data


return 0;
}


void writeRegister(unsigned int reg, unsigned int val) {
    Wire.beginTransmission(0x28); // opens I2C with RC522 address (0x28)
    Wire.write(reg);    // loads register address
    Wire.write(val);    // loads value into address
    Wire.endTransmission(); // ends transmission to RC522
}
