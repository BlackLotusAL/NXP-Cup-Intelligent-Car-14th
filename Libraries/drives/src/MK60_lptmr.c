/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_lptmr
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#include "MK60_lptmr.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      lptmr初始化
//  @param      altn            选择选择引脚模块
//  @param      count           脉冲比较值
//  @param      cfg             触发方式
//  @return     void
//  @since      v1.0
//  Sample usage:               lptmr_pulse_init(lpt0_alt2,0xFFFF,LPT_Falling);		//脉冲计数，计数到0XFFFF触发中断，下降沿计数
//-------------------------------------------------------------------------------------------------------------------
void lptmr_pulse_init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg)
{
    // 开启模块时钟
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;                   //使能LPT模块时钟
    
    //设置输入管脚    
    if(altn == lpt0_alt1)       port_init(A19, ALT6 | PULLUP);      //在A19上使用 ALT6
    else if(altn == lpt0_alt2)  port_init(C5, ALT3 | PULLUP);       //在C5上使用 ALT4
	

    // 清状态寄存器
    LPTMR0->CSR = 0x00;                                     //先关了LPT，这样才能设置时钟分频,清空计数值等

    //选择时钟源
    LPTMR0->PSR  =  ( 0
                      | LPTMR_PSR_PCS(1)                    //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      | LPTMR_PSR_PBYP_MASK                 //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)             //预分频值 = 2^(n+1) ,n = 0~ 0xF
                     );

    // 设置累加计数值
    LPTMR0->CMR  =   LPTMR_CMR_COMPARE(count);              //设置比较值
	
	LPTMR_Flag_Clear();
    // 管脚设置、使能中断
    LPTMR0->CSR  =  (0
                     | LPTMR_CSR_TPS(altn)       // 选择输入管脚 选择
                     | LPTMR_CSR_TMS_MASK        // 选择脉冲计数 (注释了表示时间计数模式)
                     | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                     | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                     | LPTMR_CSR_TIE_MASK        //中断使能
                     //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                    );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      返回当前计数值
//  @return     返回当前计数值
//  @since      v1.0
//  Sample usage:               uint16 dat = lptmr_pulse_get();		
//-------------------------------------------------------------------------------------------------------------------
uint16 lptmr_pulse_get(void)
{
    uint16 data;
    if(LPTMR0->CSR & LPTMR_CSR_TCF_MASK)	data = 0xFFFF;  //已经溢出了  返回 0xffff 表示错误
    else       								
    {
        LPTMR0->CNR = 0;//必须写入一个值才能正取读取
        data = LPTMR0->CNR;
    }
 
	return data;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      清除计数值
//  @return     void
//  @since      v1.0
//  Sample usage:               lptmr_pulse_clean();	
//-------------------------------------------------------------------------------------------------------------------
void lptmr_pulse_clean(void)
{
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;        //禁用LPT的时候就会自动清计数器的值
	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
}
