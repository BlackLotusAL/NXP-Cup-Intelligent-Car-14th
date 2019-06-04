/************************************************************
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
串口					K66单片机接口 
GND        				GND
TX         				PTE24		// 接蓝牙的RX
RX         				PTE25		// 接蓝牙的TX
-------------------------------------------------------------
正交解码模块通道		端口  
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
MT9V032接线定义：
------------------------------------ 
    模块管脚            单片机管脚
    SDA(51的RX)         E0			// UART1   
    SCL(51的TX)         E1			// UART1  
    场中断              D14			// VSYNC
    行中断              未使用不接	// HREF
    像素中断            D13			// PLCK
    数据口              D0-D7

    串口  
    波特率 115200 
    数据位 8 
    校验位 无
    停止位 1位
    流控   无
	串口连接注意事项：切勿使用蓝牙等无线串口连接
    RX					E4			// UART3
    TX                  E5			// UART3
=============================================================

分辨率是                188*120
摄像头参数设置可以到    SEEKFREE_MT9V032.h

总钻风-摄像头测试步骤：
1.下载程序到开发板
2.插上串口线或者USB转TTL
3.接好MT9V032模块接线
4.通电在TFT液晶上即可观看    
*************************************************************/  
#include "headfile.h"

PID_value PID_left, PID_right, PID_servo;					//左轮PID参数结构体，右轮PID参数结构体，伺服轮PID参数结构体

float ADC_value[5], ADC_min[5], ADC_max[5];					//电感读数数组，电感最小值数组，电感最大值数组
float posError = 0, posError_H = 0, posError_V = 0;			//位置偏差，水平电感位置偏差，垂直电感位置偏差 
int posStatus = 0;											//位置状态

int servoDuty = SERVO_MIDDLE_DUTY, servoTurnDuty = 0;		//舵机占空比，舵机转向占空比
int setPulse = 0, turnPulse = 0, defaultPulse = 60;			//速度设定值（编码器读到的目标脉冲数），转向脉冲设定值，默认速度脉冲
int getLeftPulse  = 0, setLeftPulse  = 60, leftDuty  = 0;	//读取左电机脉冲，设置左电机脉冲，设置左电机占空比          
int getRightPulse = 0, setRightPulse = 60, rightDuty = 0;	//读取右电机脉冲，设置右电机脉冲，设置右电机占空比          

float getSpeed = 0, turnAngle = 0, turnRadius = 0, turnSpeed = 0;//读取速度，转向角度，转向半径，转向差速度；

char sendStr[250] = {0}, receiveStr[20] = {0};				//发送字符串，接收字符串
int sendArr[50] = {0};										//发送数组
int strI = 0;												//字符串当前位置

KEY_value K0, K1, K2;										//声明K0,K1,K2结构体
int goFlag = 0;												//发车标志

int main(void)
{
    Init_All();                         	//初始化函数
    camera_init();							//摄像头初始化
	
	//初始化PID参数
    PID_Init(&PID_left , 5.0f, 1.5f, 0.0f, -600, 600);                                
    PID_Init(&PID_right, 5.0f, 1.5f, 0.0f, -600, 600);
    PID_Init(&PID_servo, 2.0f, 0.0f, 5.0f, -SERVO_MAX_TURN_DUTY, SERVO_MAX_TURN_DUTY);
	
	while(!goFlag)
	{
        EMS_Correct_KEY_Operation(&K1); //电磁校准按键功能
        GO_KEY_Operation(&K2);          //发车按键功能
	}
	
	while(1)
	{
		//SendStr();                    	//发送字符串
		if(mt9v032_finish_flag)
		{
			mt9v032_finish_flag = 0;
			//displayimage032(image[0]);
			seekfree_sendimg_032();
		}
	}
}
