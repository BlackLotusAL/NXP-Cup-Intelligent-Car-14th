#include "speed.h"

extern PID_value PID_left, PID_right;
extern int servoTurnDuty;
extern int setPulse, turnPulse, defaultPulse;
extern int getLeftPulse,  setLeftPulse,  leftDuty;
extern int getRightPulse, setRightPulse, rightDuty;
extern float getSpeed, turnAngle, turnRadius, turnSpeed;

extern float ADC_value[5];
extern int posStatus;
extern int goFlag;
extern KEY_value K1;

/**
* @函数名: Speed_Get
* @功  能: 编码器读取电机速度
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Speed_Get()        //获取正交解码速度
{
    getLeftPulse  = ftm_quad_get(ftm1);ftm_quad_clean(ftm1);
    getLeftPulse  = Constrain_int_output(getLeftPulse , 0, -50000, 50000);
    
    getRightPulse = ftm_quad_get(ftm2) * -1;ftm_quad_clean(ftm2);
    getRightPulse = Constrain_int_output(getRightPulse, 0, -50000, 50000);
}

/**
* @函数名: Speed_Set
* @功  能: 电机速度设置
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Speed_Set()
{
    //设置PID给定回馈
    PID_left.set = (float)setLeftPulse;
    PID_left.fdb = (float)getLeftPulse;
    PID_left.output_1 = (float)leftDuty;
    IncPID_Operation(&PID_left);        
    leftDuty = (int)PID_left.output;      
        
    //允许正反转
    if(leftDuty > 0)
    {
        ftm_pwm_duty(ftm0, ftm_ch0, 0);
        ftm_pwm_duty(ftm0, ftm_ch1, leftDuty);
    }
    else
    {
        ftm_pwm_duty(ftm0, ftm_ch0, -leftDuty);
        ftm_pwm_duty(ftm0, ftm_ch1, 0);
    }        

    //设置PID给定回馈   
    PID_right.set = (float)setRightPulse;     
    PID_right.fdb = (float)getRightPulse;
    PID_right.output_1 = (float)rightDuty;
    IncPID_Operation(&PID_right);
    rightDuty = (int)PID_right.output;
    
    //允许正反转            
    if(rightDuty > 0)
    {
        ftm_pwm_duty(ftm0, ftm_ch2, rightDuty);
        ftm_pwm_duty(ftm0, ftm_ch3, 0);
    }
    else
    {
        ftm_pwm_duty(ftm0, ftm_ch2, 0);
        ftm_pwm_duty(ftm0, ftm_ch3, -rightDuty);
    }        
}

/**
* @函数名: Speed_Turn
* @功  能: 电机转向速度差设置
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Speed_Turn()
{
    getSpeed = ENCODER_2_TRUE(getLeftPulse + getRightPulse) / 2.0;              //获取车当前速度
    
    turnAngle = 2.5293 * fabs((float)servoTurnDuty) + 0.1667;					//根据舵机转向占空比计算转向角度
    turnRadius = (CAR_LENGTH / sin(A2R(turnAngle))) - (CAR_WIDTH / 2.0);        //根据转向角度计算转弯半径
    turnSpeed = (getSpeed * CAR_WIDTH) / (2.0 * turnRadius);                    //根据当前速度计算转弯差速
    turnPulse = (int)TRUE_2_ENCODER(turnSpeed);                                 //根据转弯差速计算转弯差脉冲
    
    //电机差速配合舵机方向
    if(servoTurnDuty > 0)
    {
        turnPulse = -turnPulse;
    }
    setLeftPulse = setPulse + turnPulse;
    setRightPulse = setPulse - turnPulse;

}

/**
* @函数名: Speed_Test
* @功  能: 电机速度按键测试
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Speed_Test()
{
    if(!KEY_Read(KEY1))           //如果KEY1按下，加速
    {
        if(leftDuty < 400 && rightDuty < 400 )
        {
            leftDuty += 10;
            rightDuty += 10;                               
        }
    }
    else if(!KEY_Read(KEY2))     //如果KEY2按下，减速
    {
        if(leftDuty > 0 && rightDuty > 0)
        {
            leftDuty -= 10;
            rightDuty -= 10;
        }
    }
    else if(!KEY_Read(KEY0))     //如果KEY0按下，默认速度
    {
        leftDuty = 250;
        rightDuty = 250;                       
    }  
}

/**
* @函数名: Speed_Stop
* @功  能: 安全模式自动停车
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Speed_Stop()
{
    if(ADC_value[M0] < 50)
        
    {   
        goFlag = 0;
    }
    
    if(goFlag)
    {    
        setPulse = defaultPulse;
    }
    else
    {
        setPulse = 0;
        setLeftPulse = 0;
        setRightPulse = 0;
    }
}