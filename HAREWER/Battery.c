#include "Battery.h"
#include "adc.h"
#include "delay.h"
#include "OLED.h"
#include <stdio.h>

// ????
static uint8_t g_battery_percent = 0;

// ???????
void Battery_Simple_Init(void) {
    // ??ADC????
    adc_Init();
    printf("Battery monitor initialized (Simple Mode)\r\n");
}

// ?????????
void Battery_Simple_Update(void) {
    static uint32_t last_update = 0;
    uint32_t current_time = delay_get_tick();
    
    // ?2?????
    if(current_time - last_update < 2000) {
        return;
    }
    last_update = current_time;
    
    // ??ADC?(PA6????6)
    uint16_t adc_value = Get_adc_Average(6, 10);
    
    // ????(3S???,???5.545)
    const float VOLTAGE_DIVIDER_RATIO = 5.545f;
    float voltage = (float)adc_value * (3.3f / 4096.0f) * VOLTAGE_DIVIDER_RATIO;
    
    // ?????(3S???:10.0V-12.6V)
    const float VOLTAGE_FULL = 12.6f;
    const float VOLTAGE_EMPTY = 10.0f;
    
    if(voltage >= VOLTAGE_FULL) {
        g_battery_percent = 100;
    }
    else if(voltage <= VOLTAGE_EMPTY) {
        g_battery_percent = 0;
    }
    else {
        float percent = ((voltage - VOLTAGE_EMPTY) * 100.0f) / 
                       (VOLTAGE_FULL - VOLTAGE_EMPTY);
        g_battery_percent = (uint8_t)percent;
    }
    
    // ????????
    printf("Battery: ADC=%d, Voltage=%.2fV, Percent=%d%%\r\n", 
           adc_value, voltage, g_battery_percent);
}

// ?????????
uint8_t Battery_Simple_GetPercentage(void) {
    return g_battery_percent;
}

// ?OLED????????
void Battery_Simple_DisplayPercent(uint8_t line, uint8_t column) {
    char buffer[16];
    sprintf(buffer, "Battery:%3d%%", g_battery_percent);
    OLED_ShowString(line, column, buffer);
}