/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		system_MK60D10.h
 * @brief      		时钟初始化
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/

#ifndef SYSTEM_MK66_H_
#define SYSTEM_MK66_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
    

 
void Start (void);


void NMI_Handler(void);

#ifdef __cplusplus
}
#endif

#endif 
