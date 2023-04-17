
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "i2c.h"

#define wait_for_completion while(!(TWCR & (1 << TWINT)));

// InitI2C()
// - Wake up I2C module on mega 2560
// - Set prescaler TWPS to 1
// - Set two wire interface bit rate register TWBR
// - Enable two wire interface

void initI2C() {
  PRR0 &= ~(1<<PRTWI);  // wake up I2C module on AT2560 power management register
  TWSR |= (1 << TWPS0);  // prescaler power = 1
  TWSR &= ~(1 << TWPS1); // prescaler power = 1 

  TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)
  TWCR |= (1 << TWINT )| (1 << TWEN); // enable two wire interface
}

// StartI2C_Trans(unsigned char SLA)
// - Clear TWINT, initiate start condition, initiate enable
// - Wait for completion
// - Set two wire data register to the SLA + write bit
// - Trigger action: Clear TWINT and initiate enable
// - Wait for completion

void StartI2C_Trans(unsigned char SLA) {
// this function initiates a start condition and calls slave device with SLA
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // clear TWINT, intiate a start condition and enable
  wait_for_completion;
  TWDR = (SLA << 1); // slave address + write bit '0'
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:clear flag and enable TWI
  wait_for_completion;
}

// StopI2C_Trans()
// - Trigger action + stop condition
// - Write(unsigned char data)
// - Set two wire data register equal to incoming data
// - Trigger action
// - Wait for completion

void StopI2C_Trans() {
  // this function sends a stop condition to stop I2C transmission
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //trigger action:  send stop condition
}

// Read_from(unsigned char SLA, unsigned Char MEMADDRESS)
// - Start a transmission to the SLA
// - Write to the MEMADDRESS
// - Clear TWINT, initiate start condition, initiate enable
// - Wait for completion
// - Set two wire data register to the SLA + read bit
// - Trigger action + master acknowledge bit
// - Wait for completion
// - Trigger action
// - Wait for completion
// - Trigger action + stop condition

void Read_from(unsigned char SLA, unsigned char MEMADDRESS){
  // this function sets up reading from SLA at the SLA MEMADDRESS 
  StartI2C_Trans(SLA);
  write(MEMADDRESS);
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // restart to switch to read mode
  wait_for_completion;
  TWDR = (SLA << 1) | 0x01; // 7 bit address for slave plus read bit
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
  wait_for_completion;
  TWCR = (1<< TWINT) | (1 << TWEN);  // master can send a nack now
  wait_for_completion;
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Stop condition
// after this function is executed the TWDR register has the data from SLA that Master wants to read
}

void write(unsigned char data){
  // this function loads the data passed into the I2C data register and transmits
  TWDR = data; //load data into TWDR register
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:  clear flag and enable TWI
  wait_for_completion;
}

// unsigned char Read_data()
// - Return TWDR
unsigned char Read_data() {// Returns the last byte  from the data register
  return TWDR;
}


