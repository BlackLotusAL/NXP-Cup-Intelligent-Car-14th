#include "servo.h"

extern PID_value PID_servo;
extern int servoDuty, servoTurnDuty;
extern int posStatus;
extern float posError;
extern float ADC_value[5];
extern float posError, posError_H, posError_V, posError_I;

extern int goFlag;

/**
* @函数名: Pos_Get
* @功  能: 根据图像和电磁数据判断位置状态
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Pos_Get()
{
    //中间电感值最大
    if(ADC_value[M0] > ADC_value[L2] && ADC_value[M0] > ADC_value[R2])
    {
        posStatus = CAR_MIDDLE;
    }
    //最左侧电感值最大
    else if(ADC_value[L2] > ADC_value[M0] && ADC_value[L2] > ADC_value[R2])
    {
        posStatus = CAR_RIGHT;
        if(ADC_value[M0] < 400.0)
        {
             posStatus = CAR_VERY_RIGHT;
        }       
    }
    //最右侧电感值最大
    else if(ADC_value[R2] > ADC_value[M0] && ADC_value[R2] > ADC_value[L2])
    {
        posStatus = CAR_LEFT;
        if(ADC_value[M0] < 400.0)
        {
             posStatus = CAR_VERY_LEFT;
        }  
    }
    
    //差比和得出水平电感位置偏差
    posError_H = ((ADC_value[M0] - ADC_value[L2]) /(ADC_value[M0] + ADC_value[L2])) - ((ADC_value[M0] - ADC_value[R2]) / (ADC_value[M0] + ADC_value[R2]));  
    posError_H = posError_H * 100.0f;
    
    //差比和得出垂直电感位置偏差
    posError_V = ((ADC_value[M0] - ADC_value[L1]) /(ADC_value[M0] + ADC_value[L1])) - ((ADC_value[M0] - ADC_value[R1]) / (ADC_value[M0] + ADC_value[R1]));     
    posError_V = posError_V * 100.0f;
    
    //最终位置偏差
    posError = posError_H + 0.5 * posError_V;
}

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