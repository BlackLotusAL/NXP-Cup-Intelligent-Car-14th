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


void SendWare()
{
    uint8 cmd[2] = {0x03, 0xFC};
    UART_Put_Char(UART_4, cmd[0]);
    UART_Put_Char(UART_4, cmd[1]);
    
    sendArr[0] = (uint8)ADC_value[L2];
    sendArr[1] = (uint8)ADC_value[L1];
    sendArr[2] = (uint8)ADC_value[M0];
    sendArr[3] = (uint8)ADC_value[R1];
    sendArr[4] = (uint8)ADC_value[R2];
    SendArr(sendArr, 5); 
    
    UART_Put_Char(UART_4, cmd[1]);
    UART_Put_Char(UART_4, cmd[0]);  
}

void SendArr(int Arr[], int Size)
{
    for(uint32 i = 0; i < Size; i++)
    {
        UART_Put_Char(UART_4, Arr[i]);
    }
}

void SendStr()
{
    sprintf(sendStr,"M0:%04d  PosSta:%02d  PosErr_H:%04d  PosErr_V:%04d  PosErr:%04d  SerDuty:%04d  TuAng:%03d  SetLPu:%03d  SetRPu:%03d  GetLPu:%03d  GetRPu:%03d LDuty:%04d RDuty:%04d\r\n",\
                     (int)ADC_value[M0],\
                     (int)posStatus, (int)posError_H, (int)posError_V, (int)posError,\
                     servoDuty, (int)turnAngle,\
                     setLeftPulse, setRightPulse, getLeftPulse, getRightPulse,\
                     leftDuty, rightDuty);
   
    UART_Put_Str(UART_4, (uint8 *)sendStr);             //发送字符串
}

void ReceiveStr_Init()
{
    for(int i = 0; i < 20; i++)
    {
        receiveStr[i] = 0;
    }
    strI = 0;
}

void ReceiveStr(s8 data)
{
    receiveStr[strI] = data;
    strI++;
    if(strI >= 20)
    {
        ReceiveStr_Init();
    }
    if(receiveStr[strI - 2] == '\r' && receiveStr[strI - 1] == '\n')
    {
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

        ReceiveStr_Init();
    }
}