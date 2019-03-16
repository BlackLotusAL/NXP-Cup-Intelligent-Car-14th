#include "include.h"

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
