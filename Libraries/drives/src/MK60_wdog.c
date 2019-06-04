/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_wdog
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
#include "MK60_wdog.h"

//内部函数声明
static void wdog_unlock(void);          //看门狗解锁


//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化看门狗，设置喂狗时间
//  @param      cnt             喂狗时间（单位为 ms）
//  @return     void
//  @since      v1.0
//  Sample usage:       		
//-------------------------------------------------------------------------------------------------------------------
void wdog_init_ms(uint32 ms)
{
    //计数时间最小为4个时钟周期（WDOG_TOVALL里说明）
    uint32 num = ms*100000;
    
    wdog_unlock();                                  //解锁看门狗，这样才能配置看门狗

    WDOG->PRESC = WDOG_PRESC_PRESCVAL(0);            //设置分频系数 = PRESCVAL +1(PRESCVAL取值范围为0~7)

    WDOG->TOVALH = num >> 16;                         //设置喂狗时间
    WDOG->TOVALL = (uint16)num;

    WDOG->STCTRLH = ( 0
                    | WDOG_STCTRLH_WDOGEN_MASK     //WDOGEN置位，使能 看门狗
                    //| WDOG_STCTRLH_CLKSRC_MASK   //看门狗时钟选择（0为 LPO ，1为bus 时钟）
                    | WDOG_STCTRLH_ALLOWUPDATE_MASK
                    | WDOG_STCTRLH_STOPEN_MASK
                    | WDOG_STCTRLH_WAITEN_MASK
                    );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启看门狗
//  @return     void
//  @since      v1.0
//  Sample usage:       		
//-------------------------------------------------------------------------------------------------------------------
void wdog_enable(void)
{
    wdog_unlock();                                  //解锁看门狗，这样才能配置看门狗

    WDOG->STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;       //WDOGEN置位，使能 看门狗
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      关闭看门狗
//  @return     void
//  @since      v1.0
//  Sample usage:       		
//-------------------------------------------------------------------------------------------------------------------
void wdog_disable(void)
{
    wdog_unlock();                                  //解锁看门狗，这样才能配置看门狗

    WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;      //WDOGEN清0，禁用 看门狗
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      喂狗
//  @return     void
//  @since      v1.0
//  Sample usage:       		
//-------------------------------------------------------------------------------------------------------------------
void wdog_feed(void)
{
    //此函数不能单步执行

    //WDOG_REFRESH 寄存器里描述，连续向此寄存器写入0xA602 、0xB480即可解锁，
    //中间不得超过20个时钟周期，因此需要先关总中断

    uint8 tmp = __get_BASEPRI();    //用于返回寄存器 PRIMASK 的值(1bit)
                                    //1表示关中断，0表示开中断

    //关闭总中断，否则有可能没法在 20个周期内连续写入 WDOG_UNLOCK
    DisableInterrupts;

    //更新 看门狗（喂狗）
    WDOG->REFRESH = 0xA602;
    WDOG->REFRESH = 0xB480;

    if(tmp == 0)
    {
        EnableInterrupts;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      解锁看门狗
//  @return     void
//  @since      v1.0
//  Sample usage:       		
//-------------------------------------------------------------------------------------------------------------------
void wdog_unlock(void)
{
    //此函数不能单步执行

    //WDOG_UNLOCK 寄存器里描述，连续向此寄存器写入0xC520、0xD928即可解锁，
    //中间不得超过20个时钟周期，因此需要先关总中断
    uint8 tmp = __get_BASEPRI();        //用于返回寄存器 PRIMASK 的值(1bit)
                                        //1表示关中断，0表示开中断

    //关闭总中断，否则有可能没法在 20个周期内连续写入 WDOG_UNLOCK
    DisableInterrupts;

    //解锁 看门狗
    WDOG->UNLOCK = 0xC520;
    WDOG->UNLOCK = 0xD928;

    if(tmp == 0)
    {
        EnableInterrupts;
    }
}
