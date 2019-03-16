#ifndef _PID_H_
#define _PID_H_
#include "include.h"

typedef struct
{
    float set;          //系统待调节量的设定值 
    float fdb;          //系统待调节量的反馈值，就是传感器实际测量的值
    
    float KP;           //比例系数
    float KI;           //积分系数
    float KD;           //微分系数   
    float T;            //离散化系统的采样周期
      
    float AP;           //变量代替三项的运算结果
    float AI;
    float AD;
      
    float error;        //当前偏差e(k)
    float error_1;      //前一步的偏差
    float error_2;      //前前一步的偏差
    float error_sum;    //所有偏差的和
       
    float output;       //pid控制器的输出
    float output_1;     //pid的前一步输出
    float output_2;     //pid的前前一步输出
    float output_max;   //输出上限
    float output_min;   //输出下限   
}PID_value;

void IncPID_Operation(PID_value *pid);
void PosPID_Operation(PID_value *pid);
void PID_Init(PID_value *pid, float KP, float KI, float KD, float T, float min, float max);

#endif