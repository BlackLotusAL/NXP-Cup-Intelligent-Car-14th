/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_ftm
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_ftm_h
#define _MK60_ftm_h


#include "misc.h"
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_port_cfg.h"


typedef enum
{
    ftm0,
    ftm1,
    ftm2,
    ftm3,
} FTMn;


typedef enum
{
    ftm_ch0,
    ftm_ch1,
    ftm_ch2,
    ftm_ch3,
    ftm_ch4,
    ftm_ch5,
    ftm_ch6,
    ftm_ch7,
} FTM_CHn;

//------------------------ PWM ------------------------

#define FTM0_PRECISON 1000u     //定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON
#define FTM1_PRECISON 1000u     //定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON
#define FTM2_PRECISON 1000u     //定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON
#define FTM3_PRECISON 1000u     //定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON

//------------------------ 输入捕捉 ------------------------
//FTM 输入捕捉配置
typedef enum
{
    FTM_Rising=1,               //上升沿捕捉
    FTM_Falling=2,              //下降沿捕捉
    FTM_Rising_or_Falling=3     //跳变沿捕捉
} FTM_Input_cfg;


//------------------------ 输入捕捉中断标志位清除 ------------------------
#define FTM0_INPUT_FLAG_CLR(FTM_CHn)    FTM0->CONTROLS[FTM_CHn].CnSC &= ~FTM_CnSC_CHF_MASK;
#define FTM1_INPUT_FLAG_CLR(FTM_CHn)    FTM1->CONTROLS[FTM_CHn].CnSC &= ~FTM_CnSC_CHF_MASK;
#define FTM2_INPUT_FLAG_CLR(FTM_CHn)    FTM2->CONTROLS[FTM_CHn].CnSC &= ~FTM_CnSC_CHF_MASK;
#define FTM3_INPUT_FLAG_CLR(FTM_CHn)    FTM3->CONTROLS[FTM_CHn].CnSC &= ~FTM_CnSC_CHF_MASK;



//------------------------ 获取输入捕捉通道的中断标志位 ------------------------
//寄存器的低八位，代表着八个通道的中断标志位，该寄存器只能读不能写，最低位对应通道0
#define FTM0_INPUT_FLAG_READ            FTM0->STATUS
#define FTM1_INPUT_FLAG_READ            FTM1->STATUS
#define FTM2_INPUT_FLAG_READ            FTM2->STATUS
#define FTM3_INPUT_FLAG_READ            FTM3->STATUS


void ftm_pwm_init(FTMn ftmn, FTM_CHn ch, uint32 freq, uint32 duty);
void ftm_pwm_duty(FTMn ftmn, FTM_CHn ch, uint32 duty);
void ftm_pwm_freq(FTMn ftmn, uint32 freq);
void ftm_input_init(FTMn ftmn, FTM_CHn ch, FTM_Input_cfg cfg,uint32 freq_max);
uint16 ftm_input_get (FTMn ftmn, FTM_CHn ch);
void ftm_input_clean (FTMn ftmn);
void ftm_quad_init(FTMn ftmn);
int16 ftm_quad_get(FTMn ftmn);
void ftm_quad_clean(FTMn ftmn);

#endif
