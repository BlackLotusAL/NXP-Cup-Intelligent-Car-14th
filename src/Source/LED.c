/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：LED.c
 * 功能             ：设置LED口工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2016-01-23
 * 实验平台         ：龙丘 k66F18核心板VG2
 * 开发环境         ：IAR 7.3及以上
 * 作者             ：CHIUSIR
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#include "include.h"


void LED_Init(void)
{
   GPIO_Init(PTB,17,GPO,0);
   GPIO_Init(PTB,16,GPO,0);
   GPIO_Init(PTB,18,GPO,1);
   GPIO_Init(PTB,19,GPO,1);
}

void LED_Ctrl(LEDn_e ledno, LEDs_e sta)
{
    switch(ledno) 
    {
    case LED0:
      if(sta==ON)        GPIO_Ctrl(PTB,16,0);
      else if(sta==OFF) GPIO_Ctrl(PTB,16,1);
      else if(sta==RVS) GPIO_Reverse (PTB, 16);
    break;
    
    case LED1:
      if(sta==ON)        GPIO_Ctrl(PTB,17,0);
      else if(sta==OFF) GPIO_Ctrl(PTB,17,1);
      else if(sta==RVS) GPIO_Reverse (PTB,17);
    break;
    
    case LED2:
      if(sta==ON)        GPIO_Ctrl(PTB,18,0);
      else if(sta==OFF) GPIO_Ctrl(PTB,18,1);
      else if(sta==RVS) GPIO_Reverse (PTB,18);
    break; 
    
    case LED3:
      if(sta==ON)        GPIO_Ctrl(PTB, 19,0);
      else if(sta==OFF) GPIO_Ctrl(PTB, 19,1);
      else if(sta==RVS) GPIO_Reverse (PTB, 19);
    break;
    case LEDALL:
      if(sta==ON) 
      {       
          GPIO_Ctrl(PTB,17,0);
          GPIO_Ctrl(PTB,18,0);
          GPIO_Ctrl(PTB,16,0);
          GPIO_Ctrl(PTB,19,0);
      }
      else if(sta==OFF)
      { 
          GPIO_Ctrl(PTB,17,1);
          GPIO_Ctrl(PTB,18,1);
          GPIO_Ctrl(PTB,16,1);
          GPIO_Ctrl(PTB,19,1);
      }
      else if(sta==RVS)
      {       
          GPIO_Reverse (PTB, 17);     
          GPIO_Reverse (PTB, 18);      
          GPIO_Reverse (PTB, 19);      
          GPIO_Reverse (PTB, 16);      
      }
    break;
    default:
    break;    
    }   
}