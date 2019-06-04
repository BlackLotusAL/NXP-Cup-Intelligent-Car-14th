/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_adc
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_adc_h
#define _MK60_adc_h


#include "misc.h"
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_port_cfg.h"

typedef enum
{
    // ---------------------------------ADC0-------------------------
    ADC0_DP0,
    ADC0_DP1,
    PGA0_DP,        //不支持ADC，需要配置 ADC PGA register 设置放大增益
    ADC0_DP3,
    //正常情况B通道是不能使用的，如果需要使用找到adc_start函数 解除ADC_CFG2_MUXSEL_MASK注释    但是这样就只能使用B通道了
	//建议不要使用B通道，因为A通道数量远远多于B通道
    ADC0_SE4b,      // C2     
    ADC0_SE5b,      // D1     
    ADC0_SE6b,      // D5     
    ADC0_SE7b,      // D6     

    ADC0_SE8,       // B0
    ADC0_SE9,       // B1
    ADC0_SE10,      // A7
    ADC0_SE11,      // A8
    ADC0_SE12,      // B2
    ADC0_SE13,      // B3
    ADC0_SE14,      // C0
    ADC0_SE15,      // C1
    ADC0_SE16,      // ADC0_SE16
    ADC0_SE17,      // E24
    ADC0_SE18,      // E25
    ADC0_DM0,       // ADC0_DM0
    ADC0_DM1,       // ADC0_DM1
    RES0,           // 保留
    RES1,           // 保留
    DAC0_OUT,       // DAC0输出 不支持ADC
    RES2,           // 保留
    RES3,           // 保留
    Temp0_Sensor,   // Temperature Sensor,内部温度测量，可用ADC函数
    Bandgap0,       // 温度补偿结构带隙基准源   不支持ADC
    RES4,           // 保留
    VREFH0,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL0,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module0_Dis,    // 不支持 ADC

    // ---------------------------------ADC1-------------------------
    ADC1_DP0,
    ADC1_DP1,
    PGA1_DP,        // 不支持 ADC
    ADC1_DP3,
    ADC1_SE4a,      // E0
    ADC1_SE5a,      // E1
    ADC1_SE6a,      // E2
    ADC1_SE7a,      // E3
	
	//正常情况B通道是不能使用的，如果需要使用找到adc_start函数 解除ADC_CFG2_MUXSEL_MASK注释    但是这样就只能使用B通道了
	//建议不要使用B通道，因为A通道数量远远多于B通道
    ADC1_SE4b = ADC1_SE4a,  // C8     
    ADC1_SE5b = ADC1_SE5a,  // C9     
    ADC1_SE6b = ADC1_SE6a,  // C10    
    ADC1_SE7b = ADC1_SE7a,  // C11    

    ADC1_SE8,       // B0
    ADC1_SE9,       // B1
    ADC1_SE10,      // B4
    ADC1_SE11,      // B5
    ADC1_SE12,      // B6
    ADC1_SE13,      // B7
    ADC1_SE14,      // B10
    ADC1_SE15,      // B11
    ADC1_SE16,      // ADC1_SE16
    ADC1_SE17,      // A17
    VREF_OUTPUT,    // VREF Output
    ADC1_DM0,       // ADC1_DM0
    ADC1_DM1,       // ADC1_DM1
    RES5,           //保留
    RES6,
    DAC1_OUT,
    RES7,           //保留
    RES8,
    Temp1_Sensor,
    Bandgap1,       // 温度补偿结构带隙基准源   不支持ADC
    RES9,
    VREFH1,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL1,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module1_Dis,    // 不支持 ADC

} ADCn_Ch;

typedef enum  //ADC模块
{
	adc0,
	adc1
} ADCn;

//分辨率
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;

void adc_start(ADCn_Ch adcn_ch, ADC_nbit bit);
void adc_init(ADCn_Ch adcn_ch);
uint16 adc_once(ADCn_Ch adcn_ch, ADC_nbit bit);
uint16 adc_mid(ADCn_Ch adcn_ch, ADC_nbit bit);
uint16 adc_ave(ADCn_Ch adcn_ch, ADC_nbit bit, uint16 N);
void adc_stop(ADCn adcn);

#endif
