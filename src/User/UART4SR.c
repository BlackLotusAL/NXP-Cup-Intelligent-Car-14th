#include "include.h"

extern PID_value PID_left, PID_right, PID_servo;
extern float ADC_value[5], ADC_min[5], ADC_max[5];
extern float posError, posError_H, posError_V;
extern int posStatus;


extern int servoDuty, servoTurnDuty;
extern int setPulse, turnPulse, defaultPulse; 
extern int getLeftPulse,  setLeftPulse,  leftDuty;
extern int getRightPulse, setRightPulse, rightDuty;
extern float getSpeed, turnAngle, turnRadius, turnSpeed;

extern char sendStr[250], receiveStr[20];
extern int sendArr[50];
extern int strI;

extern KEY_value K1, K2;
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
    UART_Put_Char(UART_4, cmd[0]);
    UART_Put_Char(UART_4, cmd[1]);
    
    //要发送的数
    sendArr[0] = (uint8)ADC_value[L2];
    sendArr[1] = (uint8)ADC_value[L1];
    sendArr[2] = (uint8)ADC_value[M0];
    sendArr[3] = (uint8)ADC_value[R1];
    sendArr[4] = (uint8)ADC_value[R2];
    SendArr(sendArr, 5); 
    
    //帧尾
    UART_Put_Char(UART_4, cmd[1]);
    UART_Put_Char(UART_4, cmd[0]);  
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
        UART_Put_Char(UART_4, Arr[i]);
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
    //生产要发送的字符串
    sprintf(sendStr,"<PosSta:%02d PosErr:%04d SerTurn:%03d SerDuty:%04d> <GetLPu:%03d GetRPu:%03d GetSp:%03d> <TuAng:%03d TuRad:%05d TuSp:%03d TuPu:%03d SetLPu:%03d SetRPu:%03d>\r\n",\
                     (int)posStatus, (int)posError, servoTurnDuty, servoDuty, \
                     getLeftPulse, getRightPulse, (int)getSpeed, \
                     (int)turnAngle, (int)turnRadius, (int)turnSpeed, \
                     turnPulse, setLeftPulse, setRightPulse);
    //蓝牙发送字符串
    UART_Put_Str(UART_4, (uint8 *)sendStr);
}

/**
* @函数名: ReceiveStr_Init
* @功  能: 接收字符串数组初始化
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void ReceiveStr_Init()
{
    for(int i = 0; i < 20; i++)
    {
        receiveStr[i] = 0;
    }
    strI = 0;
}

/**
* @函数名: ReceiveStr
* @功  能: 接收字符串
* @参  数: s8 data    中断收到的8位数据
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void ReceiveStr(s8 data)
{
    //读入数据
    receiveStr[strI] = data;
    
    //字符串当前位置移至下一位
    strI++;
    
    //如果接收数组已满则初始化数组
    if(strI >= 20)
    {
        ReceiveStr_Init();
    }
    
    //如果接受数据的前两位为\r\n（字符串结尾）
    if(receiveStr[strI - 2] == '\r' && receiveStr[strI - 1] == '\n')
    {
        //接收到的字符串与命令字符头比较，再读入相应的数字
        if(strI > 3 && strncmp(receiveStr, "SVD", 2) == 0)
        {
            servoDuty = (int)(receiveStr[3] - 0x30) * 1000 + (int)(receiveStr[4] - 0x30) * 100 + (int)(receiveStr[5] - 0x30) * 10 + (int)(receiveStr[6] - 0x30) * 1;
        }
        if(strI > 2 && strncmp(receiveStr, "DP", 2) == 0)
        {
            defaultPulse  = (int)(receiveStr[2] - 0x30) * 100  + (int)(receiveStr[3] - 0x30) * 10  + (int)(receiveStr[4] - 0x30) * 1;
        }
        if(strI > 4 && strncmp(receiveStr, "SVKP", 2) == 0)
        {
            PID_servo.KP  = (float)(receiveStr[4] - 0x30) * 10.0  + (float)(receiveStr[5] - 0x30) * 1.0  + (float)(receiveStr[6] - 0x30) * 0.1;
        }
        if(strI > 4 && strncmp(receiveStr, "SVKD", 2) == 0)
        {
            PID_servo.KD  = (float)(receiveStr[4] - 0x30) * 10.0  + (float)(receiveStr[5] - 0x30) * 1.0  + (float)(receiveStr[6] - 0x30) * 0.1;
        }
        
        //数组初始化
        ReceiveStr_Init();
    }
}