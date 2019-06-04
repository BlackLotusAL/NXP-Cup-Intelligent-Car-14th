/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_32LCD.h
 * @brief      		2.8TFT液晶屏函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 * @note	
					TFT接线定义：
					------------------------------------ 
					    模块管脚       	单片机管脚
					    DB0-DB15        D0-D15
                        RS              E7
                        WR              E10
                        CS              E6
                        RST             E12
                        RD              E11
					------------------------------------ 
 ********************************************************************************************************************/
 
 
#ifndef _SEEKFREE_28LCD_H
#define _SEEKFREE_28LCD_H		

#include "headfile.h"


//定义是否使用横屏 		0,不使用.1,使用.
#define USE_HORIZONTAL  0	


//定义16位数据口，使用指针访问D口数据输出寄存器端口，强制为(uint16 *)类型即表示访问的是16位
#define LCD_DataPort    *(uint16 *)(PTD_BASE)     
//定义控制线IO口
#define  RS             E7
#define  WR             E10
#define  RD             E11
#define  CS             E6
#define  RST            E12
//定义控制线数据输出位寄存器
#define  LCD_RS         PEout(7)
#define  LCD_WR         PEout(10)
#define  LCD_RD         PEout(11)
#define  LCD_CS         PEout(6)
#define  LCD_RESET      PEout(12)






//LCD的画笔颜色和背景色	   
extern uint16  POINT_COLOR;//默认红色    
extern uint16  BACK_COLOR; //背景颜色.默认为白色
//LCD重要参数集
typedef struct  
{										    
	uint16 width;			//LCD 宽度
	uint16 height;			//LCD 高度
	uint16 id;				//LCD ID
	uint8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16	 wramcmd;		//开始写gram指令
	uint16  setxcmd;		//设置x坐标指令
	uint16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	


extern const uint8 timer[];
extern const unsigned char gImage_36[34602];
extern const uint8 unlocked[];
extern const uint8 locked[];
extern const uint8 timer_running[];
extern const uint8 timer_stop[];
extern const uint8 now_time[];
extern const uint8 lap_length[];
extern const uint8 fin_speed[];


//LCD参数  管理LCD重要参数
extern _lcd_dev lcddev;	

void LCD_Init(void); 
void LCD_Clear(uint16 Color);
void LCD_WR_DATA(int Data);
void LCD_WR_REG(int Reg);
void LCD_SetCursor(uint16 Xpos, uint16 Ypos);//设置光标位置
void LCD_SetWindows(uint16 xStar, uint16 yStar,uint16 xEnd,uint16 yEnd);//设置显示窗口
void LCD_DrawPoint(uint16 x,uint16 y);//画点
void LCD_WriteRAM_Prepare(void);
void LCD_SetParam(void);
void LCD_Display_16x8Str(uint16 x, uint16 y, int8 * str, uint16 color);
void LCD_Display_32x16Str(uint16 x, uint16 y, int8 * str, uint16 color);
void LCD_Display_16x8Num(uint16 x, uint16 y, int dat, uint8 number, uint16 color);
void LCD_Display_32x16Num(uint16 x, uint16 y, int dat, uint8 number, uint16 color);
void LCD_Displayimage032(uint8 *p);
void LCD_Displayimage7725(uint8 *p);
void LCD_Display_Chinese(uint16 x, uint16 y, uint8 number, uint8 size, const uint8 * p, uint16 color);
void LCD_Display_image(uint16 x, uint16 y, uint16 size_x, uint16 size_y, const uint8 *p);

#endif  
	 
	 



