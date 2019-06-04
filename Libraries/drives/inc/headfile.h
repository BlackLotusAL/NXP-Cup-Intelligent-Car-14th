/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		headfile
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/



#ifndef _headfile_h
#define _headfile_h


#include "MK60_port_cfg.h"
#include "common.h"
#include "misc.h"
#include "stdio.h"
#include "string.h"         //memcpy
#include "stdlib.h"         //malloc
#include "math.h"           //pow

//--------函数库--------
#include "MK60_rtc.h"
#include "MK60_gpio.h"
#include "MK60_systick.h"
#include "MK60_port.h"
#include "MK60_uart.h"
#include "MK60_pit.h"
#include "MK60_lptmr.h"
#include "MK60_ftm.h"
#include "MK60_adc.h"
#include "MK60_dac.h"
#include "MK60_flash.h"
#include "MK60_spi.h"
#include "MK60_i2c.h"
#include "MK60_wdog.h"
#include "MK60_dma.h"
#include "MK60_cmt.h"
#include "MK60_sdhc.h"

#include "isr.h"

//fatfs文件系统
#include "ff.h"

//--------逐飞科技产品例程库--------
#include "SEEKFREE_MT9V032.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_TSL1401.h"
#include "SEEKFREE_7725.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_28LCD.h"

//外部设备及自定义功能驱动的头文件
#include "define.h"
#include "ems.h"
#include "fun.h"
#include "init.h" 	
#include "key.h"
#include "led.h"
#include "pid.h"
#include "servo.h"
#include "speed.h"

#endif
