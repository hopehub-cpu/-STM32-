#ifndef __PWM_H
#define	__PWM_H

#include "stm32f10x.h"
void PWM_Init(void);
void PWM_Int(u16 arr,u16 psc);
void Set_PWMA(int PWM);
void Set_PWMB(int PWM);
void Set_PWM(int PWMA,int PWMB);
void PWM_SetCompare2(uint16_t Compare);
void TIM4_PWM_Init(u16 arr, u16 psc);
#endif

