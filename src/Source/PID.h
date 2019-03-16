#ifndef __PID_H__
#define __PID_H__

//PID计算需要的参数
typedef struct PID
{
    u16 set;          //系统待调节量的设定值 
    u16 fdb;          //系统待调节量的反馈值，就是传感器实际测量的值
    
    float KP;           //比例系数
    float KI;           //积分系数
    float KD;           //微分系数   
    float T;            //离散化系统的采样周期 
      
    float AP;           //变量代替三项的运算结果
    float AI;
    float AD;
      
    u16 error;        //当前偏差e(k)
    u16 error_1;      //前一步的偏差
    u16 error_2;      //前前一步的偏差
      
    u16 output;       //pid控制器的输出
    u16 output_1;     //pid的前一步输出
    u16 output_max;   //输出上限
    u16 output_min;   //输出下限   
}PID_value;

void PID_Operation(PID_value *pid);
u16 Constrain_u16(u16 input, u16 low, u16 high);

#endif