/****************************************************************************************************
【平    台】龙邱K66FX智能车VC母板
【编    写】CHIUSIR
【E-mail  】chiusir@aliyun.com
【软件版本】V1.0
【最后更新】2016年08月24日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.3
【Target  】K66FX1M0VLQ18
【Crystal 】 50.000Mhz
【busclock】 40.000MHz
【pllclock】200.000MHz
=============================================================
接口定义：
-------------------------------------------------------------
LED测试GPIO
LED1--PTA17
LED2--PTC0
LED3--PTD15
LED4--PTE26
-------------------------------------------------------------
OLED测试GPIO
DC  --PTC13
RST --PTC14
MISO--PTC15
CLK --PTC16
-------------------------------------------------------------
串口       K66单片机接口     备注
GND        GND
TX         PTE24             接蓝牙的RX
RX         PTE25             接蓝牙的TX
-------------------------------------------------------------
正交解码模块通道        端口  
FTM1_QDPHA              PTA12       
FTM1_QDPHB              PTA13  
FTM2_QDPHA              PTB18       
FTM2_QDPHB              PTB19
-------------------------------------------------------------
PWM端口的定义           端口 
#define FTM3_CH6        PTC10       // PTE11、PTC10
#define FTM3_CH7        PTC11       // PTE12、PTC11

#define FTM0_CH0        PTC1        // PTC1、PTA3
#define FTM0_CH1        PTC2        // PTC2、PTA4
#define FTM0_CH2        PTC3        // PTC3、PTA5
#define FTM0_CH3        PTC4        // PTC4、PTA6
-------------------------------------------------------------
//ADC模块通道           端口 
ADC1_SE10 = 10          PTB4
ADC1_SE11 = 11          PTB5
ADC1_SE12 = 12          PTB6
ADC1_SE13 = 13          PTB7
ADC1_SE14 = 14          PTB10
=============================================================
调试历史：
------------------------------------------------------------------------------------------------------
修改历史
******************************************************************************************************/

#include "include.h" 
PID_value PID_left, PID_right, PID_servo;                //左轮PID参数结构体，右轮PID参数结构体，伺服轮PID参数结构体

float ADC_value[5], ADC_min[5], ADC_max[5];             //电感读数数组，电感最小值数组，电感最大值数组
float posError = 0, posError_H = 0, posError_V = 0;     //位置偏差，水平电感位置偏差，垂直电感位置偏差 
int posStatus = 0;                                      //位置状态

int servoDuty = SERVO_MIDDLE_DUTY, servoTurnDuty = 0;   //舵机占空比，舵机转向占空比
int setPulse = 0, turnPulse = 0, defaultPulse = 80;     //速度设定值（编码器读到的目标脉冲数），转向脉冲设定值，默认速度脉冲
int getLeftPulse  = 0, setLeftPulse  = 0, leftDuty  = 0;//读取左电机脉冲，设置左电机脉冲，设置左电机占空比          
int getRightPulse = 0, setRightPulse = 0, rightDuty = 0;//读取右电机脉冲，设置右电机脉冲，设置右电机占空比          

float getSpeed = 0, turnAngle = 0, turnRadius = 0, turnSpeed = 0;//读取速度，转向角度，转向半径，转向差速度；

char sendStr[250] = {0}, receiveStr[20] = {0};          //发送字符串，接收字符串
int sendArr[50] = {0};                                  //发送数组
int strI = 0;                                           //字符串当前位置

KEY_value K1, K2;                                       //声明K1,K2结构体
int goFlag = 0;                                         //发车标志

//主函数
void main(void)
{
    DisableInterrupts;                  //关闭中断
    Init_All();                         //初始化函数
    PID_Init(&PID_left , 3.0, 210.0, 0.0,  0.01, -200, 400);                                //初始化PID参数
    PID_Init(&PID_right, 3.0, 210.0, 0.0,  0.01, -200, 400);                                //初始化PID参数
    PID_Init(&PID_servo, 0.5,   0.0, 0.2, 0.005, -SERVO_MAX_TURN_DUTY, SERVO_MAX_TURN_DUTY);//初始化PID参数
    EnableInterrupts;                   //开启中断
    
    while(1)
    {       
        EMS_Correct_KEY_Operation(&K1); //电磁校准按键功能
        GO_KEY_Operation(&K2);          //发车按键功能

    }
}