#include "Serial.h"
#include <stdio.h>
#include <stdarg.h>

// ESP8266?????(????,?main??)
char ESP8266_RxBuffer[200];
uint16_t ESP8266_RxLength = 0;
uint8_t ESP8266_RxFlag = 0;

void Serial2_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

void Serial2_SendByte(u8 byte)
{
    USART_SendData(USART2, byte);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void Serial2_SendString(char *String)
{
    u8 i;
    for (i = 0; String[i] != '\0'; i++)
        Serial2_SendByte(String[i]);
}

// ??????(SendArray, SendNumber, Printf)???????,????

/*---------------------- ??2??:??+IPD?? ----------------------*/
void USART2_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint16_t i = 0;
    static uint8_t data_len = 0;
    static uint8_t channel = 0;
    uint8_t RxData;
    
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        RxData = USART_ReceiveData(USART2);
        
        if (RxState == 0)
        {
            if (RxData == '+') RxState = 1;
        }
        else if (RxState == 1)
        {
            if (RxData == 'I') RxState = 2;
            else RxState = 0;
        }
        else if (RxState == 2)
        {
            if (RxData == 'P') RxState = 3;
            else RxState = 0;
        }
        else if (RxState == 3)
        {
            if (RxData == 'D') RxState = 4;
            else RxState = 0;
        }
        else if (RxState == 4)  // ????
        {
            if (RxData == ',') RxState = 5;
            else RxState = 0;
        }
        else if (RxState == 5)  // ??channel
        {
            if (RxData >= '0' && RxData <= '9')
                channel = channel * 10 + (RxData - '0');
            else if (RxData == ',')
                RxState = 6;
            else RxState = 0;
        }
        else if (RxState == 6)  // ??????
        {
            if (RxData >= '0' && RxData <= '9')
                data_len = data_len * 10 + (RxData - '0');
            else if (RxData == ':')
            {
                RxState = 7;
                i = 0;
            }
            else RxState = 0;
        }
        else if (RxState == 7)  // ????
        {
            if (i < data_len)
            {
                ESP8266_RxBuffer[i++] = RxData;
            }
            if (i >= data_len)
            {
                ESP8266_RxBuffer[i] = '\0';
                ESP8266_RxLength = i;
                ESP8266_RxFlag = 1;
                RxState = 0;
                data_len = 0;
                channel = 0;
            }
        }
        
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}