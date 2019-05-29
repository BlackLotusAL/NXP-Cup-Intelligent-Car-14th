#include "include.h"

/**
* @函数名: Init_All
* @功  能: 初始化函数
* @参  数: 无
* @返  回: 无
* @简  例: 无
* @注  意：无
*/
void Init_All()
{
    PLL_Init(PLL200);                           //初始化PLLΪ200M,总线为40M
    
    LCD_Init();                                 //LCD初始化
    Draw_LQLogo();                              //显示LOGO 
    time_delay_ms(1000);                        //显示1s
    LCD_CLS();                                  //LCD清屏
    
    PIT_Init(PIT0, SPEED_CONTROL_CYCLE);        //速度控制周期
    PIT_Init(PIT1, SERVO_CONTROL_CYCLE);        //舵机控制周期
    PIT_Init(PIT2, EMS_COLLECT_CYCLE);          //电磁采集周期    
    //PIT_Init(PIT3, UART_SEND_CYCLE);            //蓝牙发数周期
    
    LED_Init();                                 //LED初始化 
    KEY_Init();                                 //按键初始化
    UART_Init(UART_4, 115200);                  //串口4初始化
    UART_Irq_En(UART_4);                        //开UART_4串口接收中断
    
    ADC_Init(ADC_1);                            //ADC初始化
    ADC_Start(ADC_1,ADC1_SE10,ADC_12bit);       //启动ADC转换
    ADC_Start(ADC_1,ADC1_SE11,ADC_12bit);       //启动ADC转换
    ADC_Start(ADC_1,ADC1_SE12,ADC_12bit);       //启动ADC转换
    ADC_Start(ADC_1,ADC1_SE13,ADC_12bit);       //启动ADC转换
    ADC_Start(ADC_1,ADC1_SE14,ADC_12bit);       //启动ADC转换
  
    FTM_AB_Init(FTM1);                          //开启正交解码
    FTM_AB_Init(FTM2);                          //开启正交解码

    //PWM初始化
    //舵机
    FTM_PWM_Init(FTM3,FTM_CH6,PWM_CYCLE(SERVO_PWM_FREQUENCY),0); //PTC10
    //电机驱动
    FTM_PWM_Init(FTM0,FTM_CH0,PWM_CYCLE(SPEED_PWM_FREQUENCY),0); //PTC1
    FTM_PWM_Init(FTM0,FTM_CH1,PWM_CYCLE(SPEED_PWM_FREQUENCY),0); //PTC2
    FTM_PWM_Init(FTM0,FTM_CH2,PWM_CYCLE(SPEED_PWM_FREQUENCY),0); //PTC3
    FTM_PWM_Init(FTM0,FTM_CH3,PWM_CYCLE(SPEED_PWM_FREQUENCY),0); //PTC4 
}