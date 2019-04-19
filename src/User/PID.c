#include "include.h"

/**
* @函数名: IncPID_Operation
* @功  能: 增量式PID函数
* @参  数: PID_value *pid
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void IncPID_Operation(PID_value *pid)
{
    //计算当前误差
    pid->error = pid->set - pid->fdb;
    
    //计算中间变量AP,AI,AD;
    pid->AP = pid->error - pid->error_1;
    pid->AI = pid->error * pid->T;
    pid->AD = (pid->error - 2 * pid->error_1 + pid->error_2) / pid->T;
    
    //计算输出
    pid->output = pid->output_1 + pid->KP * pid->AP + pid->KI * pid->AI + pid->KD * pid->AD;
    
    //输出限幅
    pid->output = Constrain_float(pid->output, pid->output_min, pid->output_max);
      
    //为下次计算迭代 
    pid->output_1 = pid->output;
    pid->error_2 = pid->error_1;
    pid->error_1 = pid->error;
}

/**
* @函数名: PosPID_Operation
* @功  能: 位置式PID函数
* @参  数: PID_value *pid
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void PosPID_Operation(PID_value *pid)
{
    //计算当前误差
    pid->error = pid->set - pid->fdb;
    
    //计算所有误差的和
    //pid->error_sum = pid->error_sum + pid->error;
    
    //计算中间变量AP,AI,AD;
    pid->AP = pid->error;
    pid->AI = pid->error_sum * pid->T;
    pid->AD = (pid->error - pid->error_1) / pid->T;
    
    //计算输出
    pid->output = pid->KP * pid->AP + pid->KI * pid->AI + pid->KD * pid->AD;
    
    //输出限幅
    pid->output = Constrain_float(pid->output, pid->output_min, pid->output_max);
}

/**
* @函数名: PID_Init
* @功  能: PID    函数初始化
* @参  数: KP     比例参数
* @参  数: KI     积分参数
* @参  数: KD     微分参数
* @参  数: T      控制周期
* @参  数: min    限幅最小值
* @参  数: max    限幅最大值
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void PID_Init(PID_value *pid, float KP, float KI, float KD, float T, float min, float max)
{
    memset(pid, 0, sizeof(PID_value));
    pid->KP = KP;
    pid->KI = KI;
    pid->KD = KD;
    pid->T = T;
    pid->output_min = min;
    pid->output_max = max;
}
