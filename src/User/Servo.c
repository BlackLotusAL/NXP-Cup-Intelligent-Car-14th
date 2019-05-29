#include "include.h"

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
    PID_servo.fdb = 0.0001 * pow(posError, 3) + 0.25 * posError;	
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
    FTM_PWM_Duty(FTM3, FTM_CH6, servoDuty);
}

/**
* @函数名: Servo_Test_Key
* @功  能: 舵机按键测试
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Servo_Test_Key()
{
    //舵机校准测试
    if(!KEY_Read(KEY1))           //如果KEY1按下，右转
    {
        if(servoDuty < 10000) servoDuty += 2;       
    }
    else if(!KEY_Read(KEY2))     //如果KEY2按下，左转
    {
        if(servoDuty > 4000)  servoDuty -= 2;       
    }
    else if(!KEY_Read(KEY0))     //如果KEY0按下，正中
    {
        servoDuty = SERVO_MIDDLE_DUTY;       
    }
    FTM_PWM_Duty(FTM3, FTM_CH6, servoDuty);
}

/**
* @函数名: Servo_Test_Key
* @功  能: 舵机蓝牙测试
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Servo_Test_UART()
{
    servoDuty = Constrain_int(servoDuty, SERVO_TURN_MOST_LEFT, SERVO_TURN_MOST_RIGHT);
    FTM_PWM_Duty(FTM3, FTM_CH6, servoDuty);
}
