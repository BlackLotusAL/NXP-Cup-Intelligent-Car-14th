#include "include.h"

extern PID_value PID_servo;
extern int servoDuty, servoTurnDuty;
extern int posStatus;
extern float posError;

extern int goFlag;

void Servo_Set()
{ 
    PID_servo.set = 0.0;
    PID_servo.fdb = 0.0001 * pow(posError, 3) + 0.2632 * posError;
    PosPID_Operation(&PID_servo);
    servoTurnDuty = (int)PID_servo.output;
    servoDuty = SERVO_MIDDLE_DUTY - servoTurnDuty;
    
    if(posStatus == CLB)
    {
        servoDuty = SERVO_TURN_MOST_RIGHT;
    }
    else if(posStatus == CRB)
    {
        servoDuty = SERVO_TURN_MOST_LEFT;
    }
    
    if(!goFlag)
    {
        servoDuty = SERVO_MIDDLE_DUTY;
    }
    FTM_PWM_Duty(FTM3, FTM_CH6, servoDuty);
}

void Servo_Test_Key()
{
    //舵机校准测试
    if(!KEY_Read(KEY1))           //如果KEY1按下
    {
        if(servoDuty < 10000) servoDuty += 2;       
    }
    else if(!KEY_Read(KEY2))     //如果KEY2按下
    {
        if(servoDuty > 4000)  servoDuty -= 2;       
    }
    else if(!KEY_Read(KEY0))     //如果KEY0按下
    {
        servoDuty = SERVO_MIDDLE_DUTY;       
    }
    FTM_PWM_Duty(FTM3, FTM_CH6, servoDuty);
}

void Servo_Test_UART()
{
    servoDuty = Constrain_int(servoDuty, SERVO_TURN_MOST_LEFT, SERVO_TURN_MOST_RIGHT);
    FTM_PWM_Duty(FTM3, FTM_CH6, servoDuty);
}
