/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_port
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 

#include "MK60_port.h"


static PORT_Type * const PORTX[] = PORT_BASE_PTRS;
static GPIO_Type * const GPIOX[] = GPIO_BASE_PTRS;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PORT初始化
//  @param      ptx_n           端口
//  @param      cfg             端口属性配置，如触发选项和上拉下拉选项
//  @return     void   
//  @since      v1.0
//  Sample usage:               port_init (A14, IRQ_RISING | PF | ALT1 | PULLUP );	//初始化 A14 管脚，上升沿触发中断，带无源滤波器，复用功能为GPIO并设置为输入 ，上拉电阻
//								set_irq_priority(PORTA_IRQn,1);						//设置优先级
//								enable_irq(PORTA_IRQn);								//打开PORTA中断开关
//								EnableInterrupts;									//打开总的中断开关
//-------------------------------------------------------------------------------------------------------------------
void port_init(PTX_n ptx_n,uint32 cfg)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    
    //使能端口时钟
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << ptx);
    
    //清除中断标志位
    PORTX[ptx]->ISFR = (uint32)1<<ptn;
    
    //设置复用功能为GPIO与配置端口功能
    PORTX[ptx]->PCR[ptn] = cfg;
    
    //设置端口为输入
    GPIOX[ptx]->PDDR &= ~(uint32)(1<<ptn);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PORT初始化
//  @param      ptx_n           端口
//  @param      cfg             端口属性配置，如触发选项和上拉下拉选项
//  @return     void   
//  @since      v1.0
//  Sample usage:               port_init_NoAlt (A14, IRQ_RISING | PF | ALT1 | PULLUP );//初始化 A14 管脚，上升沿触发中断，带无源滤波器，保留之前的MUX ，上拉电阻
//-------------------------------------------------------------------------------------------------------------------
void port_init_NoAlt(PTX_n ptx_n,uint32 cfg)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    
    //使能端口时钟
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << ptx);
           
    //配置端口功能
    PORTX[ptx]->PCR[ptn] = (PORTX[ptx]->PCR[ptn] & PORT_PCR_MUX_MASK) | cfg;
}
