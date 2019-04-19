#include "include.h"

extern int goFlag;
/**
* @函数名: EMS_Correct_KEY_Operation
* @功  能: 电磁校准按键功能
* @参  数: KEY_value *key
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void EMS_Correct_KEY_Operation(KEY_value *key)
{
    //按键识别
    key->new = KEY_Read(KEY1);
    //if(key->new && (key->new ^ key->old))
    if(key->new == 0 && key->old == 1)
    {
        key->cnt++;
    }
    key->flag = key->cnt % 4;
    key->old = key->new;
    
    //4次按键为周期，第0次电磁校准初始化    亮LED0一个灯
    //               第1次水平电感校准      亮LED0，LED1，LED3三个灯               
    //               第2次垂直电感校准      亮LED1，LED3左右对角两个灯     
    //               第3次关闭电磁校准      灯全灭
    switch(key->flag)
    {
        case (EMS_CORRECT_MODE_IINT):
            LED_Ctrl(LED0, ON);         
            break;
        case (EMS_CORRECT_MODE_H):
            LED_Ctrl(LED1, ON);
            LED_Ctrl(LED3, ON);
            break;
        case (EMS_CORRECT_MODE_V):
            LED_Ctrl(LED0, OFF);
            break;
        case (EMS_CORRECT_MODE_OFF):
            LED_Ctrl(LEDALL, OFF);      //LED全灭
            break;
    }
}

/**
* @函数名: GO_KEY_Operation
* @功  能: 发车按键功能
* @参  数: KEY_value *key
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void GO_KEY_Operation(KEY_value *key)
{
    //按键识别
    key->new = KEY_Read(KEY2);
    if(key->new == 0 && key->old == 1)
    {
        goFlag = 1;
    }
    
    //2次按键为周期，goFlag = 0    停车 
    //               goFlag = 1    发车              

    if(goFlag)
    {
        LED_Ctrl(LED2, ON);
    }
    else
    {
        LED_Ctrl(LED2, OFF);
    }
    key->old = key->new;
}

/**
* @函数名: Constrain_float
* @功  能: float类型限幅
* @参  数: float input    输入值
* @参  数: float min      限幅下限
* @参  数: float max      限幅上限
* @返  回: input/min/max
* @简  例: 无
* @注  意：无
*/
float Constrain_float(float input, float min, float max) 
{
    return ((input < min) ? (min) : ((input > max) ? (max) : (input)));
}

/**
* @函数名: Constrain_int
* @功  能: int类型限幅
* @参  数: int input      输入值
* @参  数: int min        限幅下限
* @参  数: int max        限幅上限
* @返  回: input/min/max
* @简  例: 无
* @注  意：无
*/
int Constrain_int(int input, int min, int max) 
{
    return ((input < min) ? (min) : ((input > max) ? (max) : (input)));
}

/**
* @函数名: Constrain_int_output
* @功  能: int类型限幅
* @参  数: int input      输入值
* @参  数: int output     输出值
* @参  数: int min        限幅下限
* @参  数: int max        限幅上限
* @返  回: input/output
* @简  例: 无
* @注  意：input在限幅范围外输出output
*/
int Constrain_int_output(int input, int output, int min, int max) 
{
    return ((input < min) ? (output) : ((input > max) ? (output) : (input)));
}