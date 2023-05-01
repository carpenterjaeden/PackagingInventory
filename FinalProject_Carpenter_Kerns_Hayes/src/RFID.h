#ifndef RFID_H
#define RFID_H

void initRFID();
void readRFIDTag();
void writeRegister(unsigned int reg, unsigned int val);
unsigned int readRegister(unsigned int reg);
unsigned int transceiveData(unsigned int data);

#endif