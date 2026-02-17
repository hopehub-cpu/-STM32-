#include  "pwm.h"

/**************************************************************************
函数功能：pwm初始化
入口参数：arr：设为一个时钟频率的最大值  psc： 预分频值
返回  值：无
**************************************************************************/
void PWM_Int(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;                //定义结构体GPIO_InitStructure
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;      //定义结构体TIM_TimeBaseStructure   
	TIM_OCInitTypeDef TIM_OCInitStructure;              //定义结构体TIM_OCInitStructure
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PB端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能定时器3
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //复用模式输出
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1; //PB0 、PB1
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;        //IO口速度
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //GPIO初始化
	
	
	TIM_TimeBaseStructure.TIM_Period = arr;                //设置下一个更新活动的自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;             //预分配值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;           //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode= TIM_OCMode_PWM1;             //PWM脉冲宽度调制1
	TIM_OCInitStructure.TIM_Pulse = 0;                           //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //设置TIM输出极性为高
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM3,ENABLE);//主输出使能
	
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);//使能预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);              //使能自动装载允许位
	TIM_Cmd(TIM3,ENABLE);//启动定时器3
	
	
}
void TIM4_PWM_Init(u16 arr, u16 psc)//舵机控制定时器5
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体	
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //定义一个PWM输出的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB时钟，在STM32中使用IO口前都要使能对应时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能通用定时器4时钟，PB9引脚对应TIM4_CH4
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; //引脚9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出模式，定时器功能为PB9引脚复用功能
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //定义该引脚输出速度为50MHZ
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化引脚GPIOB9
	 
	TIM_TimeBaseInitStrue.TIM_Period=arr; //计数模式为向上计数时，定时器从0开始计数，计数超过到arr时触发定时中断服务函数
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数，决定每一个计数的时长
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数模式：向上计数
	TIM_TimeBaseInitStrue.TIM_ClockDivision=0; //一般不使用，默认TIM_CKD_DIV1
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStrue); //根据TIM_TimeBaseInitStrue的参数初始化定时器TIM4
	
	TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM模式1，当定时器计数小于TIM_Pulse时，定时器对应IO输出有效电平
	TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //输出有效电平为高电平
	TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //使能PWM输出
	TIM_OCInitTypeStrue.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OC4Init(TIM4, &TIM_OCInitTypeStrue); //根据TIM_OCInitTypeStrue参数初始化定时器4通道4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable); //CH4预装载使能
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ARR预装载使能
	
	TIM_Cmd(TIM4, ENABLE); //使能定时器TIM4
}


void Set_PWM(int PWMA,int PWMB)
{
	if(PWMA>0){
		 GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		 GPIO_SetBits(GPIOB, GPIO_Pin_15);	    //正转 AIN1-PB14低电平  AIN2-PB15高电平
			    
	}else{
		 
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	 
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);	 
		PWMA=-PWMA;															//反转 AIN1-PB14高电平  AIN2-PB15低电平  pwm取绝对值
	}
	
	if(PWMB>0){
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);     //正转 BIN1-PB13高电平  BIN2-PB12低电平

	}else{
		GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
		GPIO_SetBits(GPIOB, GPIO_Pin_12);     	 		
		PWMB=-PWMB;															//反转 BIN1-PB13低电平  BIN2-PB12高电平  pwm取绝对值
	}
	
  TIM_SetCompare4(TIM3,PWMA);  //A电机设置pwm  PB1
	TIM_SetCompare3(TIM3,PWMB);	 //B电机设置pwm	 PB0
}

//void PWM_SetCompare2(uint16_t Compare)
//{
//	TIM_SetCompare4(TIM4, Compare);		//??CCR2??
//}
