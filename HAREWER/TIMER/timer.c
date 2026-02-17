#include "timer.h"
#include "encoder.h"
#include "pwm.h"
#include "usart.h"
#include "moto.h"
#include "stm32f10x.h"

//外部变量 extern说明改变量已在其它文件定义
extern int encoder_A,encoder_B;  //编码器读取变量
extern int Velocity_PWMA,Velocity_PWMB;  //pwm设置变量
extern int TargetVelocity_A;   //A电机闭环控制目标速度
extern int TargetVelocity_B;	 //B电机闭环控制目标速度
extern float vcc;  //电源电压值


void TIM1_10ms_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. 使能TIM1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    
    TIM_TimeBaseStructure.TIM_Period = 49;          // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;     // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    // 时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // 3. 使能TIM1更新中断
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    // 4. 配置TIM1中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 5. 使能TIM1
    TIM_Cmd(TIM1, ENABLE);
    
    // 6. 如果是高级定时器(TIM1/TIM8)，需要使能主输出
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

//定时器1中断服务函数
//void TIM1_UP_IRQHandler(void)
//{  
//  	static uint32_t counter = 0;
//    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
//    {
//        // 清除中断标志位
//        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//				
//			
//				/****************************************************/
//				//以下为开环控制内容，开环与闭环二者使用其一
//        encoder_A=Read_Encoder(4);		//A电机编码器读取						
//				encoder_B=Read_Encoder(2);		//B电机编码器读取
//				Velocity_PWMA=3000;						//A电机PWM设置，正数正转，负数反转
//				Velocity_PWMB=3000;							//B电机PWM设置，正数正转，负数反转
//				Set_PWM(Velocity_PWMA,Velocity_PWMB);   //根据设置的pwm数值，控制转向引脚高低电平和pwm
//			
//        counter++;
//        if(counter >= 100) // 每10ms加1，1秒计时
//        {
//            counter = 0;
//            // 每一秒打印一次串口数据
//						printf("开环控制，电压=%6.2f V\r\n",vcc);
//				  	printf("A编码器=%d，A-PWM=%d，B编码器=%d，B-PWM=%d\r\n",encoder_A,Velocity_PWMA,encoder_B,Velocity_PWMB);

//        }			
				/****************************************************/
				
				
				/****************************************************/
				//以下为闭环控制内容，开环与闭环二者使用其一
//				encoder_A=Read_Encoder(4);				//A电机编码器读取					
//				encoder_B=Read_Encoder(2);				//B电机编码器读取	
//				Velocity_PWMA=Velocity_A(TargetVelocity_A,encoder_A);		//PID闭环控制-输出PWM值
//				Velocity_PWMB=Velocity_B(TargetVelocity_B,encoder_B);		//PID闭环控制-输出PWM值
//				Set_PWM(Velocity_PWMA,Velocity_PWMB);			//根据设置的pwm数值，控制转向引脚高低电平和pwm
//			
//        counter++;
//        if(counter >= 100) // 每10ms加1，1秒计时
//        {
//            counter = 0;	
//						// 每一秒打印一次串口数据			
//						printf("闭环控制，电压=%6.2f V\r\n",vcc);
//						printf("A目标速度=%d，A编码器=%d，A-PWM=%d\r\n",TargetVelocity_A,encoder_A,Velocity_PWMA);
//						printf("B目标速度=%d，B编码器=%d，B-PWM=%d\r\n",TargetVelocity_B,encoder_B,Velocity_PWMB);
//        }
				/****************************************************/
//    
//    }
//}

