#ifndef __CONTROL_H
#define __CONTROL_H
#define WHEEL_PWM_MIN       0
#define WHEEL_PWM_MAX       7199
#define WHEEL_PWM_STEP      500
#define WHEEL_PWM_DEFAULT   3000

// ---------- ??????(extern)----------
extern uint16_t wheel_pwm_value;   // ??PWM???
extern uint8_t  wheel_direction;   // 0:??,1:??,2:??,3:??,4:??
void Parse_Command(char *cmd);
void Motor_SetSpeed(void);
void TIM1_UP_IRQHandler(void);

#endif
