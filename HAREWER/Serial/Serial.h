#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"

// ESP8266?????(??????)
extern char ESP8266_RxBuffer[200];
extern uint16_t ESP8266_RxLength;
extern uint8_t ESP8266_RxFlag;

void Serial2_Init(void);
void Serial2_SendByte(u8 byte);
void Serial2_SendString(char *String);
// ???????????

#endif