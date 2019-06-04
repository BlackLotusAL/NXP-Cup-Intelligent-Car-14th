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



#ifndef _MK60_gpio_h
#define _MK60_gpio_h

#include "common.h"

// 位带操作 内存偏移计算 详见 Cortex-M4 Generic User Guide 2.25 
// CM4中有2块bitband区域 0x2000_0000-0x200F_FFFF 映射至 0x2200_0000-0x23FF_FFFF
//                       0x4000_0000-0x4000_FFFF 映射至 0x4200_0000-0x43FF_FFFF

#define PAout(n)   BITBAND_REG(PTA->PDOR, n)
#define PAin(n)    BITBAND_REG(PTA->PDIR, n)
  
#define PBout(n)   BITBAND_REG(PTB->PDOR, n)
#define PBin(n)    BITBAND_REG(PTB->PDIR, n)

#define PCout(n)   BITBAND_REG(PTC->PDOR, n)
#define PCin(n)    BITBAND_REG(PTC->PDIR, n)

#define PDout(n)   BITBAND_REG(PTD->PDOR, n)
#define PDin(n)    BITBAND_REG(PTD->PDIR, n)

#define PEout(n)   BITBAND_REG(PTE->PDOR, n)
#define PEin(n)    BITBAND_REG(PTE->PDIR, n)




#define A_PDOR8(x)    *(uint8 *)(PTA_BASE+0x00+x) //定义A端口的8位输出地址
#define A_PSOR8(x)    *(uint8 *)(PTA_BASE+0x04+x) //定义A端口的8位置位地址
#define A_PCOR8(x)    *(uint8 *)(PTA_BASE+0x08+x) //定义A端口的8位清零地址
#define A_PTOR8(x)    *(uint8 *)(PTA_BASE+0x0C+x) //定义A端口的8位取反地址
#define A_PDIR8(x)    *(uint8 *)(PTA_BASE+0x10+x) //定义A端口的8位输入地址
#define A_PDDR8(x)    *(uint8 *)(PTA_BASE+0x14+x) //定义A端口的8位方向地址
                                                         
                                                         
#define B_PDOR8(x)    *(uint8 *)(PTB_BASE+0x00+x) //定义B端口的8位输出地址
#define B_PSOR8(x)    *(uint8 *)(PTB_BASE+0x04+x) //定义B端口的8位置位地址
#define B_PCOR8(x)    *(uint8 *)(PTB_BASE+0x08+x) //定义B端口的8位清零地址
#define B_PTOR8(x)    *(uint8 *)(PTB_BASE+0x0C+x) //定义B端口的8位取反地址
#define B_PDIR8(x)    *(uint8 *)(PTB_BASE+0x10+x) //定义B端口的8位输入地址
#define B_PDDR8(x)    *(uint8 *)(PTB_BASE+0x14+x) //定义B端口的8位方向地址


#define C_PDOR8(x)    *(uint8 *)(PTC_BASE+0x00+x) //定义C端口的8位输出地址
#define C_PSOR8(x)    *(uint8 *)(PTC_BASE+0x04+x) //定义C端口的8位置位地址
#define C_PCOR8(x)    *(uint8 *)(PTC_BASE+0x08+x) //定义C端口的8位清零地址
#define C_PTOR8(x)    *(uint8 *)(PTC_BASE+0x0C+x) //定义C端口的8位取反地址
#define C_PDIR8(x)    *(uint8 *)(PTC_BASE+0x10+x) //定义C端口的8位输入地址
#define C_PDDR8(x)    *(uint8 *)(PTC_BASE+0x14+x) //定义C端口的8位方向地址


#define D_PDOR8(x)    *(uint8 *)(PTD_BASE+0x00+x) //定义D端口的8位输出地址
#define D_PSOR8(x)    *(uint8 *)(PTD_BASE+0x04+x) //定义D端口的8位置位地址
#define D_PCOR8(x)    *(uint8 *)(PTD_BASE+0x08+x) //定义D端口的8位清零地址
#define D_PTOR8(x)    *(uint8 *)(PTD_BASE+0x0C+x) //定义D端口的8位取反地址
#define D_PDIR8(x)    *(uint8 *)(PTD_BASE+0x10+x) //定义D端口的8位输入地址
#define D_PDDR8(x)    *(uint8 *)(PTD_BASE+0x14+x) //定义D端口的8位方向地址


#define E_PDOR8(x)    *(uint8 *)(PTE_BASE+0x00+x) //定义E端口的8位输出地址
#define E_PSOR8(x)    *(uint8 *)(PTE_BASE+0x04+x) //定义E端口的8位置位地址
#define E_PCOR8(x)    *(uint8 *)(PTE_BASE+0x08+x) //定义E端口的8位清零地址
#define E_PTOR8(x)    *(uint8 *)(PTE_BASE+0x0C+x) //定义E端口的8位取反地址
#define E_PDIR8(x)    *(uint8 *)(PTE_BASE+0x10+x) //定义E端口的8位输入地址
#define E_PDDR8(x)    *(uint8 *)(PTE_BASE+0x14+x) //定义E端口的8位方向地址


//以下宏定义主要用于兼容老版本的摄像头采集代码，也可以删除下面的宏定义
//在摄像头采集代码中将下面的额宏定义换成上面的宏定义
#define A_IN_DATA(x)  *(uint8 *)(PTA_BASE+0x10+x)//定义PTA的8位输入端口地址
#define B_IN_DATA(x)  *(uint8 *)(PTB_BASE+0x10+x)//定义PTB的8位输入端口地址
#define C_IN_DATA(x)  *(uint8 *)(PTC_BASE+0x10+x)//定义PTC的8位输入端口地址
#define D_IN_DATA(x)  *(uint8 *)(PTD_BASE+0x10+x)//定义PTD的8位输入端口地址
#define E_IN_DATA(x)  *(uint8 *)(PTE_BASE+0x10+x)//定义PTE的8位输入端口地址


void    gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat);//初始化gpio
void    gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr);            //设置引脚方向
uint8   gpio_get(PTX_n ptx_n);                          //获取引脚状态
void    gpio_set(PTX_n ptx_n, uint8 dat);               //设置引脚状态
void    gpio_turn(PTX_n ptx_n);                         //翻转引脚状态

#endif
