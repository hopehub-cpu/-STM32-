#ifndef _DHT22_H
#define _DHT22_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define DHT22_PORT GPIOA
#define DHT22_PIN  GPIO_Pin_8

typedef struct {
    float temperature;
    float humidity;
}
DHT22_Data;

void DHT22_Init(void);

uint8_t DHT22_Read(DHT22_Data *data);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

#endif
