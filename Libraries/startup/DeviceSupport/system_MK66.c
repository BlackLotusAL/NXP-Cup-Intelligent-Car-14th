/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		system_MK60
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/

#include <stdint.h>
#include "common.h"
#include "MK60_port.h"

//-------------------------------------------------------------------------------------------------------------------
//  MK66FX_CLOCK 为 0  内核频率 180M  总线频率90M   flex_bus频率60M    flash频率25.7M
//  MK66FX_CLOCK 为 1  内核频率 200M  总线频率100M  flex_bus频率66.6M  flash频率28.5M
//  MK66FX_CLOCK 为 2  内核频率 220M  总线频率110M  flex_bus频率73.3M  flash频率31.4M
//-------------------------------------------------------------------------------------------------------------------
#define MK66FX_CLOCK    0



void Disable_Wdog(void)
{
    // Disable the WDOG module 
    // WDOG_UNLOCK: WDOGUNLOCK=0xC520 
    WDOG->UNLOCK = (uint16_t)0xC520u;     // Key 1 
    // WDOG_UNLOCK : WDOGUNLOCK=0xD928 
    WDOG->UNLOCK  = (uint16_t)0xD928u;    // Key 2 
    // WDOG_STCTRLH: ??=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,??=0,STNDBYEN=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 
    WDOG->STCTRLH = (uint16_t)0x01D2u;
}

void div_set(void)
{
    uint8  temp_c5;
    uint8  temp_c6;
    uint32 temp_clkdiv;
    

    switch(MK66FX_CLOCK)
    {
        case 0:
        {
            temp_clkdiv = (uint32_t)0x01260000u;
            temp_c5 = MCG_C5_PRDIV0(5-1);
            temp_c6 = MCG_C6_PLLS(1) | MCG_C6_VDIV0(36-16);
            
        }break;
        
        case 1:
        {
            temp_clkdiv = (uint32_t)0x01260000u;
            temp_c5 = MCG_C5_PRDIV0(5-1);
            temp_c6 = MCG_C6_PLLS(1) | MCG_C6_VDIV0(40-16);
            
        }break;
        
        case 2:
        {
            temp_clkdiv = (uint32_t)0x01260000u;
            temp_c5 = MCG_C5_PRDIV0(5-1);
            temp_c6 = MCG_C6_PLLS(1) | MCG_C6_VDIV0(44-16);
            
        }break;
        
        
        
        default:
        {
            temp_clkdiv = (uint32_t)0x01260000u;
            temp_c5 = MCG_C5_PRDIV0(5-1);
            temp_c6 = MCG_C6_PLLS(1) | MCG_C6_VDIV(36-16);
            
        }break;
        
    }

    SIM->CLKDIV1 = temp_clkdiv;                 
	MCG->C5 = temp_c5;                          
	MCG->C6 = temp_c6;                          
}


void SYSTEM_CLK(void)
{
	uint32 temp_reg;
    
    // Switch to FBE Mode
	SIM->CLKDIV1 = (uint32_t)0xffffffffu;    			// 预分频寄存器，先都设置为1
	//OSC->CR = (uint8_t)0x80u;                           // 启用外部晶振
	MCG->C2 &= ~MCG_C2_LP_MASK;							// 外部晶振是有源的时候不要置位MCG_C2_EREFS0_MASK ，这样才能使用A19引脚
	MCG->C2 |= MCG_C2_RANGE0(2);

	MCG->C1 = (uint8_t)0xBAu;                           // 选择外部时钟作为MCGOUTCLK的输入源，1536分频，FLL输入源为外部时钟，启用MCGIRCLK，内部参考时钟在停止模式下禁止
	MCG->C4 &= (uint8_t)~(uint8_t)0xE0u;
	MCG->C6 &= (uint8_t)( ~(uint8_t)MCG_C6_PLLS_MASK ); // MCG的输入源选择FLL
	while((MCG->S & MCG_S_IREFST_MASK) != 0u) {  }      // 等待，直到FLL的时钟源是外部时钟
	while((MCG->S & MCG_S_CLKST_MASK) != 0x08u) {  }    // 等待，MCGOUTCLK的时钟源被选择为外部时钟
	
    
    //保存寄存器值
    temp_reg = FMC->PFAPR;

    //设置M0PFD至M7PFD为1，禁用预取功能
    FMC->PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
                 | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
                 | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
    
	
    // Switch to PBE Mode 
	div_set();
	//恢复寄存器的值
    FMC->PFAPR = temp_reg; 
    
	while((MCG->S & MCG_S_PLLST_MASK) == 0u) {  }       // Wait until the source of the PLLS clock has switched to the PLL 
	while((MCG->S & MCG_S_LOCK0_MASK) == 0u) {  }       // Wait until locked 
	
	// Switch to PEE Mode 
	MCG->C1 = (uint8_t)0x1Au;
	while((MCG->S & 0x0Cu) != 0x0Cu) {  }               // Wait until output of the PLL is selected 
	while((MCG->S & MCG_S_LOCK0_MASK) == 0u) {  }       // Wait until locked
}

void Start (void) 
{
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));      // set CP10, CP11 Full Access
    
    Disable_Wdog();
    SYSTEM_CLK();
	gpio_init(A4,GPO,1);  //初始化为输出1，即 禁用了 NMI 中断
}



void NMI_Handler(void)
{
	
}


