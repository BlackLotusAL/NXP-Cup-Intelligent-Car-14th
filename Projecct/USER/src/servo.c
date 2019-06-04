#include "servo.h"

extern PID_value PID_servo;
extern int servoDuty, servoTurnDuty;
extern int posStatus;
extern float posError;

extern int goFlag;

/**
* @函数名: Servo_Set
* @功  能: 舵机控制
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Servo_Set()
{ 
    //PID运算
    PID_servo.set = 0.0;
    PID_servo.fdb = (0.00005f * pow(posError, 3) + 0.2632f * posError) / 46.6667f;
    PosPID_Operation(&PID_servo);
    //设置舵机占空比
    servoTurnDuty = (int)PID_servo.output;
    servoDuty = SERVO_MIDDLE_DUTY - servoTurnDuty;
    
    //如果车身偏离中线较远，使用最大转向角度
    if(posStatus == CAR_VERY_LEFT)
    {
        servoDuty = SERVO_TURN_MOST_RIGHT;
    }
    else if(posStatus == CAR_VERY_RIGHT)
    {
        servoDuty = SERVO_TURN_MOST_LEFT;
    }
    
    //未发车时舵机居中
    if(!goFlag)
    {
        servoDuty = SERVO_MIDDLE_DUTY;
    }
    ftm_pwm_duty(ftm3, ftm_ch6, servoDuty);
}