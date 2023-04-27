#include <avr/io.h>
#include <util/delay.h>
#include "RFID.h"
#include <Wire.h>



void initRFID(){
    // initialize I2C interface
    Wire.begin();

    // Configure the RC522 module to be on "standby" (waiting for tag)
    Wire.beginTransmission(0x28);

    Wire.write(0x01); // CommandReg register
    Wire.write(0x00); // Idle mode

    Wire.write(0x0C); // TxControl register
    Wire.write(0x00); // Force 100% modulation depth

    Wire.write(0x14); // TModeReg register
    Wire.write(0x80); // Set timer to start when data is received

    Wire.write(0x2A); // TxASKReg register
    Wire.write(0x00); // Disables "ask" prompt

    Wire.write(0x02); // ComIEnReg register
    Wire.write(0x01); // Enables RxIRQ interrupt

    Wire.endTransmission();
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
        unsigned int data[16];
        readRFIDTag();
    }

}

void readRFIDTag() {
    
}