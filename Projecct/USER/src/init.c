#include "init.h"

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
    DisableInterrupts;                  		//关闭中断
    get_clk();									//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
    
    OLED_Init();                                //OLED初始化
    OLED_Fill(0xff);                            
    systick_delay_ms(1000);						//显示1s
    OLED_Fill(0x00);                            //OLED清屏
    
    pit_init_ms(pit0, SPEED_CONTROL_CYCLE);		//速度控制中断
    pit_init_ms(pit1, SERVO_CONTROL_CYCLE);		//舵机控制中断
    pit_init_ms(pit2, EMS_COLLECT_CYCLE);		//电磁采集中断    
    //pit_init_ms(pit3, 1000);
	
	set_irq_priority(PIT0_IRQn,1);				//设置PIT0优先级
	set_irq_priority(PIT1_IRQn,1);				//设置PIT1优先级
	set_irq_priority(PIT2_IRQn,1);				//设置PIT2优先级
	set_irq_priority(PIT3_IRQn,1);				//设置PIT3优先级
	
	enable_irq(PIT0_IRQn);						//打开pit0的中断开关
	enable_irq(PIT1_IRQn);						//打开pit1的中断开关
	enable_irq(PIT2_IRQn);						//打开pit2的中断开关
	enable_irq(PIT3_IRQn);						//打开pit3的中断开关
    
    LED_Init();                                 //LED初始化 
    KEY_Init();                                 //按键初始化
    uart_init(uart3, 115200);					//初始换串口与电脑通信
    uart_init(uart4, 115200);					//串口4初始化

	//ADC初始化,启动ADC转换
    adc_init(ADC1_SE10);	adc_start(ADC1_SE10,ADC_12bit);                         
    adc_init(ADC1_SE11);	adc_start(ADC1_SE11,ADC_12bit);
    adc_init(ADC1_SE12);	adc_start(ADC1_SE12,ADC_12bit);
    adc_init(ADC1_SE13);	adc_start(ADC1_SE13,ADC_12bit);
    adc_init(ADC1_SE14);	adc_start(ADC1_SE14,ADC_12bit);

	//开启正交解码
    ftm_quad_init(ftm1);                          
    ftm_quad_init(ftm2);

    //舵机初始化
    ftm_pwm_init(ftm3,ftm_ch6,SERVO_PWM_FREQUENCY,0); //PTC10
	
    //电机驱动
    ftm_pwm_init(ftm0,ftm_ch0,SPEED_PWM_FREQUENCY,0); //PTC1
    ftm_pwm_init(ftm0,ftm_ch1,SPEED_PWM_FREQUENCY,0); //PTC2
    ftm_pwm_init(ftm0,ftm_ch2,SPEED_PWM_FREQUENCY,0); //PTC3
    ftm_pwm_init(ftm0,ftm_ch3,SPEED_PWM_FREQUENCY,0); //PTC4 
	
    EnableInterrupts;                   		//开启中断
}