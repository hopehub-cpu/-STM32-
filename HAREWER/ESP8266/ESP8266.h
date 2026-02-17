#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"

void ESP8266_Init(void);
void ESP8266_SendData(char *data);
uint8_t ESP8266_GetRxFlag(void);
void ESP8266_ClearRxFlag(void);
char* ESP8266_GetRxBuffer(void);
uint16_t ESP8266_GetRxLength(void);

#endif
