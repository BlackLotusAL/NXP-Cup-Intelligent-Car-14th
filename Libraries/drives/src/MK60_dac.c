/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_dac
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#include "MK60_dac.h"

DAC_Type * const DACX[2] = DAC_BASE_PTRS; //定义两个指针数组保存 DACN 的地址

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      dacn         选择DAC通道
//  @param      volta        需要输出的电压
//  @return     void
//  @since      v1.0
//  Sample usage:               dac_init(dac0,1000);//输出电压等于(1000/(1<<12)*3.3)V
//-------------------------------------------------------------------------------------------------------------------
void dac_init(DACn dacn,uint16 volta)
{
    volta &= 0x0FFF; 
    
    // 使能时钟 
    SIM->SCGC2 |= (SIM_SCGC2_DAC0_MASK << dacn) ;    //使能DAC模块
    
    //  配置DAC寄存器  

    //配置DAC_C0寄存器
    DACX[dacn]->C0  = ( 0
                        |  DAC_C0_DACTRGSEL_MASK                //选择软件触发
                        |  DAC_C0_DACRFS_MASK                   //选择参考VDD电压(3.3V)
                        |  DAC_C0_DACEN_MASK                    //使能DAC模块
                      );

    //配置DAC_C1寄存器
    DACX[dacn]->C1 = ( 0 ) ;
    //配置DAC_C2寄存器
    DACX[dacn]->C2 = ( 0
                       | DAC_C2_DACBFRP(0)             //设置缓冲区读指针指向0
                     );

    DACX[dacn]->DAT[0].DATH = volta>>8;   //默认输出最低电压
    DACX[dacn]->DAT[0].DATL = (uint8)volta;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      改变输出电压
//  @param      dacn         选择DAC通道
//  @param      volta        需要输出的电压
//  @return     void
//  @since      v1.0
//  Sample usage:               dac_out(dac0,1000);//输出电压等于(1000/(1<<12)*3.3)V
//-------------------------------------------------------------------------------------------------------------------
void dac_out(DACn dacn, uint16 volta)
{
    volta &= 0x0FFF; 
    DACX[dacn]->DAT[0].DATH = volta>>8;   //默认输出最低电压
    DACX[dacn]->DAT[0].DATL = (uint8)volta;
}









