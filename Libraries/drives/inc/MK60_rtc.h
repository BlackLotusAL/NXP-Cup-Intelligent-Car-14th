/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_rtc
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 

#ifndef _MK60_rtc_h
#define _MK60_rtc_h



#include "misc.h"
#include "common.h"
#include "stdlib.h"


#define rtc_alarm_irq_en()      RTC->IER |= RTC_IER_TAIE_MASK      //使能闹钟中断
#define rtc_alarm_irq_dis()     RTC->IER &= ~RTC_IER_TAIE_MASK     //禁止闹钟中断


#define rtc_overflow_irq_en()   RTC->IER |= RTC_IER_TOIE_MASK      //使能溢出中断
#define rtc_overflow_irq_dis()  RTC->IER &= ~RTC_IER_TOIE_MASK     //禁止溢出中断

#define rtc_invalid_irq_en()    RTC->IER |= RTC_IER_TIIE_MASK      //使能无效设置中断
#define rtc_invalid_irq_dis()   RTC->IER &= ~RTC_IER_TIIE_MASK     //禁止无效设置中断



void     rtc_init(void);                         //初始化

void     rtc_set_time(uint32 seconds);           //设置时间
uint32   rtc_get_time(void);                     //获取时间

uint8    rtc_set_alarm(uint32 alarm);            //设置闹钟
void     rtc_close_alarm(void);                  //关闭闹钟









#endif
