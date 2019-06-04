#ifndef __led_h
#define __led_h
#include "headfile.h"

//定义模块号
typedef enum
{
    LED0=0,
    LED1=1,
    LED2=2,
    LED3=3,
    LED4=4,
    LED5=5,
    LEDALL=6,//全部四个   
}LEDn_e;

typedef enum
{
    ON=0,  //亮
    OFF=1, //灭
    RVS=2, //反转  
}LEDs_e;

void LED_Init(void);
void LED_Ctrl(LEDn_e ledno, LEDs_e sta);

#endif 