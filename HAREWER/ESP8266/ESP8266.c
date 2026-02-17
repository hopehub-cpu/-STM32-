#include "ESP8266.h"
#include "delay.h"
#include "Serial.h"
#include "OLED.h"
#include <stdio.h>
#include <string.h>

void ESP8266_Init(void)
{
	  delay_init();
    // ????
//    Serial2_SendString("AT+RST\r\n");
//    delay_ms(1000);
//    
//    // ???STA??
//    Serial2_SendString("AT+CWMODE=1\r\n");
//    delay_ms(1000);
//    
//    // ??WiFi
//    Serial2_SendString("AT+CWJAP=\"AAA\",\"11111111\"\r\n");
//    delay_ms(5000);
    
    // ?????
	  Serial2_SendString("AT+CIPSTA_DEF=\"192.168.45.205\"\r\n");
	  delay_ms(3000);
    Serial2_SendString("AT+CIPMUX=1\r\n"); 
    delay_ms(5000);
    // ??TCP???,??8080
    Serial2_SendString("AT+CIPSERVER=1,8080\r\n");
    delay_ms(1000);
}

void ESP8266_SendData(char *data)
{
    char cmd[50];
    sprintf(cmd, "AT+CIPSEND=0,%d\r\n", strlen(data));
    Serial2_SendString(cmd);
    delay_ms(50);
    Serial2_SendString(data);
    delay_ms(50);
}

uint8_t ESP8266_GetRxFlag(void)
{
    return ESP8266_RxFlag;
}

void ESP8266_ClearRxFlag(void)
{
    ESP8266_RxFlag = 0;
}

char* ESP8266_GetRxBuffer(void)
{
    return ESP8266_RxBuffer;
}

uint16_t ESP8266_GetRxLength(void)
{
    return ESP8266_RxLength;
}
