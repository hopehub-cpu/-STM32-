#ifndef __BATTERY_SIMPLE_H
#define __BATTERY_SIMPLE_H

#include "stm32f10x.h"

// ????
void Battery_Simple_Init(void);
void Battery_Simple_Update(void);
uint8_t Battery_Simple_GetPercentage(void);
void Battery_Simple_DisplayPercent(uint8_t line, uint8_t column);

#endif