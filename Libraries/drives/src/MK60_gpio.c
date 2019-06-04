/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_gpio
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/



#include "MK60_gpio.h"

static PORT_Type * const PORTX[] = PORT_BASE_PTRS;
static GPIO_Type * const GPIOX[] = GPIO_BASE_PTRS;



//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpio 初始化
//  @param      ptx_n           选择的引脚   
//  @param      ddr             引脚方向    GPI：输入 GPO：输出
//  @param      dat             引脚输出数据（如果设置为输出）
//  @return     void
//  @since      v1.0
//  Sample usage:               gpio_init(A5,GPO,0);   //初始化A5为gpio模式，设置为输出模式，初始化输出低电平
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    
    //使能端口时钟
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << ptx);
    
    //清除之前的复用功能
    PORTX[ptx]->PCR[ptn] &= ~(uint32)PORT_PCR_MUX_MASK;
    //设置复用功能为GPIO即普通IO口
    PORTX[ptx]->PCR[ptn] |= PORT_PCR_MUX(1);
    
    //设置端口方向
    if(ddr) GPIOX[ptx]->PDDR |= (uint32)(1<<ptn);
    else    GPIOX[ptx]->PDDR &= ~(uint32)(1<<ptn);
    
    //设置端口状态
    if(ddr)
    {
        if(dat) GPIOX[ptx]->PDOR |=  (uint32)(1<<ptn);
        else    GPIOX[ptx]->PDOR &= ~(uint32)(1<<ptn);   
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置引脚方向
//  @param      ptx_n           选择的引脚   
//  @param      ddr             引脚方向    GPI：输入 GPO：输出
//  @return     void
//  @since      v1.0
//  Sample usage:               gpio_ddr(A5,GPO);   //设置A5为输出模式
//-------------------------------------------------------------------------------------------------------------------
void gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    
    //设置端口方向
    if(ddr) GPIOX[ptx]->PDDR |= (uint32)(1<<ptn);
    else    GPIOX[ptx]->PDDR &= ~(uint32)(1<<ptn);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取引脚状态
//  @param      ptx_n           选择的引脚   
//  @return     管脚的状态      1为高电平，0为低电平
//  @since      v1.0
//  Sample usage:               uint8 status = gpio_get(A5);   //读取A5为状态
//-------------------------------------------------------------------------------------------------------------------
uint8 gpio_get(PTX_n ptx_n)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    
    //设置端口状态
    return ( (GPIOX[ptx]->PDIR >> ptn) & 0x1 );
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置引脚状态
//  @param      ptx_n           选择的引脚   
//  @param      dat             1为高电平，0为低电平
//  @return     void
//  @since      v1.0
//  Sample usage:               gpio_set(A5,0);   //设置A5为低电平
//-------------------------------------------------------------------------------------------------------------------
void gpio_set(PTX_n ptx_n, uint8 dat)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    
    //设置端口状态
    if(dat) GPIOX[ptx]->PDOR |=  (uint32)(1<<ptn);
    else    GPIOX[ptx]->PDOR &= ~(uint32)(1<<ptn);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      翻转引脚状态
//  @param      ptx_n           选择的引脚   
//  @return     void
//  @since      v1.0
//  Sample usage:               gpio_turn(A5);   //翻转A5
//-------------------------------------------------------------------------------------------------------------------
void gpio_turn(PTX_n ptx_n)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    
    //设置端口状态
    GPIOX[ptx]->PTOR = (uint32)(1<<ptn);
}


