#ifndef I2C_H
#define I2C_H

void InitI2C();
void StartI2C_Trans(unsigned char SLA);
void StopI2C_Trans();
void Read_from(unsigned char SLA, unsigned char MEMADDRESS);
void write(unsigned char data);
unsigned char Read_data();

#endif