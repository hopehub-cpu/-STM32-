#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "moto.h"
#define WHEEL_PWM_MIN       0       // ??PWM(???500)
#define WHEEL_PWM_MAX       7199    // ??PWM(???ARR=7199)
#define WHEEL_PWM_STEP      500     // ??????
#define WHEEL_PWM_DEFAULT   3000    // ??PWM
uint16_t servo_pwm = 770;       // ????PWM?(?????)
uint8_t  servo_direction = 0;   // 0:??, 1:??, 2:??
#define SERVO_PWM_MIN    500    // ???(??0°)
#define SERVO_PWM_MAX    1250   // ???(??180°)
#define SERVO_PWM_MID    807    // ??(90°)
#define SERVO_STEP       10     // ?10ms????(??,??????)

uint16_t wheel_pwm_value = WHEEL_PWM_DEFAULT;  // ??PWM???
uint8_t wheel_direction = 0;        // 0:??,1:??,2:??,3:??,4:??
uint8_t cutter_enable = 0;          // 0:??,1:??
uint8_t cutter_speed_level = 5;     // 0-10
int      Velocity_PWMA = 0,Velocity_PWMB=0;
void TIM1_UP_IRQHandler(void) //¶æ»ú¿ª»·¿ØÖÆ
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        // ---------- ????????(?10ms????)----------
        if (servo_direction == 1)   // ??
        {
            if (servo_pwm > SERVO_PWM_MIN + SERVO_STEP)
                servo_pwm -= SERVO_STEP;
            else
                servo_pwm = SERVO_PWM_MIN;   // ?????
            TIM_SetCompare4(TIM4, servo_pwm);
        }
        else if (servo_direction == 2) // ??
        {
            if (servo_pwm < SERVO_PWM_MAX - SERVO_STEP)
                servo_pwm += SERVO_STEP;
            else
                servo_pwm = SERVO_PWM_MAX;   // ?????
            TIM_SetCompare4(TIM4, servo_pwm);
        } else if (servo_direction == 3) // ??
        {
                servo_pwm =SERVO_PWM_MID;   // ?????
            TIM_SetCompare4(TIM4, servo_pwm);
        }
        // servo_direction == 0 ?,???
        
        // ---------- ?????????????(???,??)----------
        // encoder_A = Read_Encoder(4);
        // ...
    }
}
void Motor_SetSpeed(void)
{
    int base_speed = (int)wheel_pwm_value;
    
    switch (wheel_direction)
    {
        case 1:  // ??
            Velocity_PWMA= base_speed;
            Velocity_PWMB = base_speed;
            break;
        case 2:  // ??
            Velocity_PWMA = -base_speed;
            Velocity_PWMB = -base_speed;
            break;
        case 3:  // ??(??)
            Velocity_PWMA = -base_speed ;
            Velocity_PWMB = base_speed;
            break;
        case 4:  // ??
            Velocity_PWMA = base_speed;
            Velocity_PWMB = -base_speed;
            break;
        case 0:  // ??
					  Velocity_PWMA =0;
            Velocity_PWMB =0;
        default:
            Velocity_PWMA = 0;
            Velocity_PWMB = 0;
            break;
    }
    
    Set_PWM(Velocity_PWMA, Velocity_PWMB);
}
void Parse_Command(char *cmd)
{
    // ????????
    char *p = cmd;
    while (*p)
    {
        if (*p == '\r' || *p == '\n') { *p = '\0'; break; }
        p++;
    }
    if (strcmp(cmd, "&R1") == 0)
    {
        wheel_direction = 1;
        Motor_SetSpeed();
        printf("Forward\r\n");
    }
    else if (strcmp(cmd, "&R2") == 0)
    {
        wheel_direction = 2;
        Motor_SetSpeed();
        printf("Backward\r\n");
    }
    else if (strcmp(cmd, "&R3") == 0)
    {
        wheel_direction = 3;
        Motor_SetSpeed();
        printf("Left Turn\r\n");
    }
    else if (strcmp(cmd, "&R4") == 0)
    {
        wheel_direction = 4;
        Motor_SetSpeed();
        printf("Right Turn\r\n");
    }
    else if (strcmp(cmd, "&R0") == 0)
    { 
        wheel_direction = 0;
        Motor_SetSpeed();
        printf("Stop\r\n");
    }
    else if (strcmp(cmd, "&R7") == 0)   // ??
    {
        if (wheel_pwm_value + WHEEL_PWM_STEP <= WHEEL_PWM_MAX)
            wheel_pwm_value += WHEEL_PWM_STEP;
        else
            wheel_pwm_value = WHEEL_PWM_MAX;
        Motor_SetSpeed();
        printf("Speed Up, PWM=%d\r\n", wheel_pwm_value);
    }
    else if (strcmp(cmd, "&R8") == 0)   // ??
    {
        if (wheel_pwm_value >= WHEEL_PWM_STEP + WHEEL_PWM_MIN)
            wheel_pwm_value -= WHEEL_PWM_STEP;
        else
            wheel_pwm_value = WHEEL_PWM_MIN;
        Motor_SetSpeed();
        printf("Speed Down, PWM=%d\r\n", wheel_pwm_value);
    }
		// ¶æ»ú¿ØÖÆ
    else if (strcmp(cmd, "&R13") == 0)   // ????
    {
        servo_direction = 1;          // ??????
        printf("Servo Left Start\r\n");
    }
    else if (strcmp(cmd, "&R14") == 0)   // ????
    {
        servo_direction = 2;          // ??????
        printf("Servo Right Start\r\n");
    }
		else if (strcmp(cmd, "&R16") == 0)
    {
        wheel_direction = 0;
        Motor_SetSpeed();
			  servo_direction = 3;
			 
        printf("Stop\r\n");
    }
}
