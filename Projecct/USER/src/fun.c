#include "fun.h"

extern PID_value PID_left, PID_right, PID_servo;
extern float ADC_value[5], ADC_min[5], ADC_max[5];
extern float posError, posError_H, posError_V, posError_I;
extern int posStatus;


extern int servoDuty, servoTurnDuty;
extern int setPulse, turnPulse, defaultPulse; 
extern int getLeftPulse,  setLeftPulse,  leftDuty;
extern int getRightPulse, setRightPulse, rightDuty;
extern float getSpeed, turnAngle, turnRadius, turnSpeed;

extern char sendStr[250], receiveStr[20];
extern int sendArr[50];
extern int strI;

extern KEY_value K0, K1, K2;
extern int goFlag;

/**
* @函数名: SendWare
* @功  能: 山外虚拟示波器
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void SendWare()
{
    uint8 cmd[2] = {0x03, 0xFC};
    //帧头
    uart_putchar(uart4, cmd[0]);
    uart_putchar(uart4, cmd[1]);
   
    //要发送的数
    sendArr[0] = getLeftPulse;
    sendArr[1] = setLeftPulse;
    SendArr(sendArr, 2); 
    
    //帧尾
    uart_putchar(uart4, cmd[1]);
    uart_putchar(uart4, cmd[0]);  
}

/**
* @函数名: SendArr
* @功  能: 发送数组 在山外虚拟示波器上显示
* @参  数: int Arr[]  发送的数组名
* @参  数: int Size   发送的数组长度
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void SendArr(int Arr[], int Size)
{
    for(uint32 i = 0; i < Size; i++)
    {
        uart_putchar(uart4, Arr[i]);
    }
}

/**
* @函数名: SendStr
* @功  能: 蓝牙发送数据到上位机
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void SendStr()
{
    //产生要发送的字符串
    sprintf(sendStr,"L2:%04d  L1:%04d  M0:%04d  R1:%04d  R2:%04d  PosSta:%02d  PosErrH:%04d  PosErrV:%04d  PosErrI:%04d  SerTurn:%03d  SerDuty:%04d  GetLPu:%03d  GetRPu:%03d  GetSp:%03d  TuAng:%03d  TuRad:%05d  TuSp:%03d  TuPu:%03d  SetLPu:%03d  SetRPu:%03d\r\n",\
                     (int)ADC_value[L2], (int)ADC_value[L1], (int)ADC_value[M0], (int)ADC_value[R1], (int)ADC_value[R2],\
	  				 (int)posStatus, (int)posError_H, (int)posError_V, (int)posError_I, servoTurnDuty, servoDuty, \
                     getLeftPulse, getRightPulse, (int)getSpeed, \
                     (int)turnAngle, (int)turnRadius, (int)turnSpeed, \
                     turnPulse, setLeftPulse, setRightPulse);

    //蓝牙发送字符串
    uart_putstr(uart4, (uint8 *)sendStr);
}

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
* @函数名: Servo_Test_Key
* @功  能: 舵机按键测试
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Servo_Test_Key(KEY_value *key0, KEY_value *key1, KEY_value *key2)
{
    //舵机校准测试
    key1->new = KEY_Read(KEY1);
    if(key1->new == 0 && key1->old == 1)
    {
        servoDuty += 1;//如果KEY1按下，右转
    }
    key1->old = key1->new;
	
    key2->new = KEY_Read(KEY2);
    if(key2->new == 0 && key2->old == 1)
    {
        servoDuty -= 1;//如果KEY2按下，左转
    }
    key2->old = key2->new;
	
    key0->new = KEY_Read(KEY0);
    if(key0->new == 0 && key0->old == 1)
    {
        servoDuty = SERVO_MIDDLE_DUTY;//如果KEY0按下，正中 
    }
    key0->old = key0->new;
	
    ftm_pwm_duty(ftm3, ftm_ch6, servoDuty);
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