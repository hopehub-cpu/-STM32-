#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"
#include "adc.h"
#include "usart.h"
#include "encoder.h"
#include "timer.h"
#include "OLED.h"
#include "DHT22.h"
#include "ESP8266.h"
#include "Serial.h"
#include "Control.h"
#include <stdio.h> 

int TargetVelocity_A=20;  //Aµç»ú±Õ»·¿ØÖÆÄ¿±êËÙ¶È
int TargetVelocity_B=20;  //Bµç»ú±Õ»·¿ØÖÆÄ¿±êËÙ¶È
float Velcity_Kp=1,  Velcity_Ki=3,  Velcity_Kd; //PID²ÎÊý
int encoder_A,encoder_B;  //±àÂëÆ÷¶ÁÈ¡±äÁ¿
//int Velocity_PWMA=0,Velocity_PWMB=0;  //pwmÉèÖÃ
u16 adcx;   //adc¼Ä´æÆ÷Öµ
float vcc;  //µçÔ´µçÑ¹Öµ
  
DHT22_Data dht22_data;
char display_str[32];
	
	void Display_Data(void) 
		{
     uint16_t adc_value = Get_adc_Average(6, 10);  // ??6,??10?
    float voltage = (float)adc_value * (3.3f / 4096.0f) * 5.545f;  // ????
    uint8_t battery_percent;
    
    // ???????(3S???)
    if(voltage >= 12.6f) {
        battery_percent = 100;
    } else if(voltage <= 10.0f) {
        battery_percent = 0;
    } else {
        battery_percent = (uint8_t)((voltage - 10.0f) * 100.0f / 2.6f);
    }
    sprintf(display_str, "Temp:%.1fC", dht22_data.temperature);
			OLED_ShowString(4, 3, display_str);
    //ÏÔÊ¾ÎÂ¶È
    
    sprintf(display_str, "Humi:%.1f%%", dht22_data.humidity);
			OLED_ShowString(4, 5, display_str);
    //ÏÔÊ¾Êª¶È
			
		 sprintf(display_str, "Battery:%3d%%",battery_percent);
    OLED_ShowString(3, 1, display_str);
   }

int main(void)
 {
	 OLED_Init();
	 SystemInit(); 
	 delay_init();
	 
	 DHT22_Init();
	 Motor_SetSpeed();
	 TIM1_UP_IRQHandler();
	 Serial2_Init();
	 ESP8266_Init();
	 void Parse_Command(char *cmd);
	 TIM4_PWM_Init(9999, 143);//¶æ»ú¿ØÖÆPWM
//	 u16 PWM=750;//¶æ»ú¿ØÖÆPWM
//	 u8 Direction=1;//¶æ»ú¿ØÖÆPWM
	 OLED_ShowString(1, 1, "DHT22 Monitor");
	 OLED_ShowString(2, 1, "Temp: ");
   OLED_ShowString(3, 1, "Humi: ");
	 OLED_ShowString(4, 1, "SOC:");
	 
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //ÖÐ¶ÏÓÅÏÈ¼¶·Ö×é	 
   //ÅäÖÃÏµÍ³Ê±ÖÓÎª72M   
      //ÑÓÊ±º¯Êý³õÊ¼»¯
	 uart_init(115200);		//´®¿Ú³õÊ¼»¯
//	 USART1_SendString("UART1 OK\r\n");
	 adc_Init();				//ADC1-PA6 ³õÊ¼»¯  Ö»ÓÐTB6612´øÎÈÑ¹Ä£¿é°æ²ÅÓÐ»ñÈ¡µç³ØµçÑ¹µÄ¹¦ÄÜ£¬TB6612FNG¾«¼ò°æÃ»ÓÐÕâ¸ö¹¦ÄÜ
	  
   Gpio_Init();    //³õÊ¼»¯µç»ú·½Ïò¿ØÖÆÒý½Å Aµç»ú AIN1-PB14 AIN2-PB15 , Bµç»ú BIN1-PB13 BIN2-PB12   
	  
   PWM_Int(7199,0);      //³õÊ¼»¯pwmÊä³ö 72000 000 /7199+1=10000HZ Aµç»ú ¶¨Ê±Æ÷3Í¨µÀ4 PB1 , Bµç»ú ¶¨Ê±Æ÷3Í¨µÀ3 PB0
//    Encoder_Init_Tim4();//Aµç»ú±àÂëÆ÷Òý½Å³õÊ¼»¯  AÏà-PB6 BÏà-PB7 
//	 Encoder_Init_Tim2();//Bµç»ú±àÂëÆ÷Òý½Å³õÊ¼»¯  AÏà-PA0 BÏà-PA1
	  
	 TIM1_10ms_Init();//10ms¶         ¨Ê±Æ÷1ÖÐ¶Ï£¬±àÂëÆ÷¶ÁÈ¡ÊýÖµºÍ¿ØÖÆÊ±ºòÊ¹ÓÃ£¬ÖÐ¶Ïº¯ÊýÔÚtimer.c

   while(1)
	 {
		//*****************¶æ»ú¿ØÖÆPWM***********************//
//		 delay_ms(100);	
//		 if(Direction)PWM=PWM+250;
//		else PWM=PWM-250;
//		if (PWM>1000) Direction=0;  //PWMÖµ1250´ú±í¶æ»úÎ»ÖÃ½Ó½ü180¶È
//		if (PWM<500)  Direction=1;   //PWMÖµ250´ú±í¶æ»úÎ»ÖÃ½Ó½ü0¶È
// 		TIM_SetCompare4(TIM4, PWM); //ÉèÖÃ´ý×°Èë²¶»ñ±È½Ï¼Ä´æÆ÷µÄÂö³åÖµ£¬Ïàµ±ÓÚ²»¶ÏÉèÖÃTIM_Pulse
//		                            //Ò²¼´ÉèÖÃÕ¼¿Õ±È£¬Êä³öµÄPWMÖµ
		 //*****************¶æ»ú¿ØÖÆPWM***********************//
		adcx=Get_adc_Average(ADC_Channel_6,10);  //»ñÈ¡adcµÄÖµ
		vcc=(float)adcx*(3.3*11/4096);    //Çóµ±Ç°µçÑ¹  µçÔ´µçÑ¹ÊÇadcÒý½ÅµçÑ¹µÄ11±¶		 
		 //OLEDÏÔÊ¾
		 if(DHT22_Read(&dht22_data)) 
			 {

            Display_Data();
        } else 
			 {

            OLED_ShowString(3, 11,"error");
        }
			 if (ESP8266_GetRxFlag())
        {
            char *cmd = ESP8266_GetRxBuffer();
            Parse_Command(cmd);
            ESP8266_ClearRxFlag();
        }
        
        delay_ms(10);
    }
	 }
//void TIM1_UP_IRQHandler(void)
//{
//  	static uint32_t counter = 0;
//    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
//    {
//        // Çå³ýÖÐ¶Ï±êÖ¾Î»
//        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//				
//			
//				/****************************************************/
//				//ÒÔÏÂÎª¿ª»·¿ØÖÆÄÚÈÝ£¬¿ª»·Óë±Õ»·¶þÕßÊ¹ÓÃÆäÒ»
//        encoder_A=Read_Encoder(4);	 	//Aµç»ú±àÂëÆ÷¶ÁÈ¡						
//				encoder_B=Read_Encoder(2);		//Bµç»ú±àÂëÆ÷¶ÁÈ¡
//				Velocity_PWMA=2000;						//Aµç»úPWMÉèÖÃ£¬ÕýÊýÕý×ª£¬¸ºÊý·´×ª
//				Velocity_PWMB=2000;		  					//Bµç»úPWMÉèÖÃ£¬ÕýÊýÕý×ª£¬¸ºÊý·´×ª
//				Set_PWM(Velocity_PWMA,Velocity_PWMB);   //¸ù¾ÝÉèÖÃµÄpwmÊýÖµ£¬¿ØÖÆ×ªÏòÒý½Å¸ßµÍµçÆ½ºÍpwm
//			
//        counter++;
//        if(counter >= 100) // Ã¿10ms¼Ó1£¬1Ãë¼ÆÊ±
//        {
//            counter = 0;
//            // Ã¿Ò»Ãë´òÓ¡Ò»´Î´®¿ÚÊý¾Ý
//						printf("¿ª»·¿ØÖÆ£¬µçÑ¹=%6.2f V\r\n",vcc);
//				  	printf("A±àÂëÆ÷=%d£¬A-PWM=%d£¬B±àÂëÆ÷=%d£¬B-PWM=%d\r\n",encoder_A,Velocity_PWMA,encoder_B,Velocity_PWMB);

//        }			
// 
//			}
//		}
