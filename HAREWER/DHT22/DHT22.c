#include "DHT22.h"
#include "delay.h"
static void DHT22_GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DHT22_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT22_PORT, &GPIO_InitStructure);
    GPIO_SetBits(DHT22_PORT, DHT22_PIN);
}

void DHT22_Init(void) {
    DHT22_GPIO_Config();
}

static void DHT22_Set_Output(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT22_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT22_PORT, &GPIO_InitStructure);
}

static void DHT22_Set_Input(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT22_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(DHT22_PORT, &GPIO_InitStructure);
}

static uint8_t DHT22_Read_Byte(void) {
    uint8_t i, data = 0;
    
    for(i = 0; i < 8; i++) {
        while(GPIO_ReadInputDataBit(DHT22_PORT, DHT22_PIN) == 0);
        delay_us(40);
        
        if(GPIO_ReadInputDataBit(DHT22_PORT, DHT22_PIN) == 1) {
            data = (data << 1) | 0x01;
        } else {
            data = data << 1;
        }
        
        while(GPIO_ReadInputDataBit(DHT22_PORT, DHT22_PIN) == 1);
    }
    
    return data;
}

uint8_t DHT22_Read(DHT22_Data *data) {
    uint8_t buf[5];
    uint8_t i;
    // ????????
    DHT22_Set_Output();
    GPIO_ResetBits(DHT22_PORT, DHT22_PIN);
    delay_ms(1);
    GPIO_SetBits(DHT22_PORT, DHT22_PIN);
    delay_us(30);
    // ???????????
    DHT22_Set_Input();
    // ??DHT22??
    if(GPIO_ReadInputDataBit(DHT22_PORT, DHT22_PIN) == 1) return 0;
    delay_us(80);
    if(GPIO_ReadInputDataBit(DHT22_PORT, DHT22_PIN) == 0) return 0;
    delay_us(80);
    // ??40???
    for(i = 0; i < 5; i++) {
        buf[i] = DHT22_Read_Byte();
    }
    
    // ????
    if(buf[4] == (buf[0] + buf[1] + buf[2] + buf[3])) {
        data->humidity = (buf[0] * 256 + buf[1]) / 10.0;
        data->temperature = (buf[2] * 256 + buf[3]) / 10.0;
        return 1;
    }
    
    return 0;
}


    
