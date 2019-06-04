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
 
 
 
#ifndef _MK60_lptmr_h
#define _MK60_lptmr_h


#include "misc.h"
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_port_cfg.h"


typedef enum
{
    lpt0_alt1 = 1,      // A19
    lpt0_alt2 = 2       // C5
} LPT0_ALTn;

typedef enum
{
    LPT_Rising  = 0,    //上升沿触发
    LPT_Falling = 1     //下降沿触发
} LPT_CFG;


#define LPTMR_Flag_Clear()  (LPTMR0->CSR |= LPTMR_CSR_TCF_MASK)         //清除LPT比较标志位




void lptmr_pulse_init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg);
uint16 lptmr_pulse_get(void);
void lptmr_pulse_clean(void);


#endif
