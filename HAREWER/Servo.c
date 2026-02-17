#include "stm32f10x.h"                  // Device header
#include "PWM.h"


void Servo_Init(void)
{
void PWM_Init(void);						
}


void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);	
												
}
