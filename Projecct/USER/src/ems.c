#include "ems.h"

extern float ADC_value[5], ADC_min[5], ADC_max[5];
extern float posError, posError_H, posError_V;
extern int posStatus;
extern KEY_value K1;

/**
* @函数名: EMS_Get
* @功  能: 读取电磁数据
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：ADC通道与电感编号的对应情况需自己测量调整
*/
void EMS_Get()
{
    //电磁信号采集
    ADC_value[L2] = (float)adc_ave(ADC1_SE10, ADC_12bit, 10); 
    ADC_value[L1] = (float)adc_ave(ADC1_SE11, ADC_12bit, 10);  
    ADC_value[M0] = (float)adc_ave(ADC1_SE12, ADC_12bit, 10);
    ADC_value[R1] = (float)adc_ave(ADC1_SE13, ADC_12bit, 10);
    ADC_value[R2] = (float)adc_ave(ADC1_SE14, ADC_12bit, 10);   
    

    if(K1.flag == EMS_CORRECT_MODE_OFF)    //如果电磁校准模式关闭
    {
        //归一化处理
        ADC_value[L2] = 1000.0 * (ADC_value[L2] - ADC_min[L2]) / (ADC_max[L2] - ADC_min[L2]);
        ADC_value[M0] = 1000.0 * (ADC_value[M0] - ADC_min[M0]) / (ADC_max[M0] - ADC_min[M0]);
        ADC_value[R2] = 1000.0 * (ADC_value[R2] - ADC_min[R2]) / (ADC_max[R2] - ADC_min[R2]);
        ADC_value[L1] = 1000.0 * (ADC_value[L1] - ADC_min[L1]) / (ADC_max[L1] - ADC_min[L1]);
        ADC_value[R1] = 1000.0 * (ADC_value[R1] - ADC_min[R1]) / (ADC_max[R1] - ADC_min[R1]);
        
        //限幅
        Constrain_float(ADC_value[L2], 1.0, 1000.0);
        Constrain_float(ADC_value[M0], 1.0, 1000.0);
        Constrain_float(ADC_value[L2], 1.0, 1000.0);
        Constrain_float(ADC_value[L1], 1.0, 1000.0); 
        Constrain_float(ADC_value[R1], 1.0, 1000.0);        
    }
}

/**
* @函数名: EMS_Correct
* @功  能: 读取电磁最大最小值用于之后的归一化处理
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：先初始化再读取最大最小值
*/
void EMS_Correct()
{
    switch(K1.flag)
    {
        //初始化电磁最大最小值
        case (EMS_CORRECT_MODE_IINT):
            ADC_max[L2] = 0,    ADC_max[L1] = 0,    ADC_max[M0] = 0,    ADC_max[R1] = 0,    ADC_max[R2] = 0;   
            ADC_min[L2] = 4095, ADC_min[L1] = 4095, ADC_min[M0] = 4095, ADC_min[R1] = 4095, ADC_min[R2] = 4095; 
            break;
         
        //读取水平电感的最大最小值
        case (EMS_CORRECT_MODE_H):
            if(ADC_value[M0] > ADC_max[M0])     ADC_max[M0] = ADC_value[M0];
            if(ADC_value[M0] < ADC_min[M0])     ADC_min[M0] = ADC_value[M0];
            
            if(ADC_value[L2] > ADC_max[L2])     ADC_max[L2] = ADC_value[L2];
            if(ADC_value[L2] < ADC_min[L2])     ADC_min[L2] = ADC_value[L2];
            
            if(ADC_value[R2] > ADC_max[R2])     ADC_max[R2] = ADC_value[R2];
            if(ADC_value[R2] < ADC_min[R2])     ADC_min[R2] = ADC_value[R2];
            break;   
            
        //读取垂直电感的最大最小值     
        case (EMS_CORRECT_MODE_V):
            if(ADC_value[L1] > ADC_max[L1])     ADC_max[L1] = ADC_value[L1];
            if(ADC_value[L1] < ADC_min[L1])     ADC_min[L1] = ADC_value[L1];
            
            if(ADC_value[R1] > ADC_max[R1])     ADC_max[R1] = ADC_value[R1];
            if(ADC_value[R1] < ADC_min[R1])     ADC_min[R1] = ADC_value[R1];
            break;
        
    }
}  


/**
* @函数名: Pos_Get
* @功  能: 根据电磁数据判断位置状态
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