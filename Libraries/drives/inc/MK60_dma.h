/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_dma
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 

 
#ifndef _MK60_dma_h
#define _MK60_dma_h


#include "misc.h"
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_port_cfg.h"

typedef enum
{
    //        禁用通道            
    Channel_Disabled        = 0,
    
    DMA_TSI                 = 1,
    //        UART            
    DMA_UART0_Rx            = 2,
    DMA_UART0_Tx            = 3,
    DMA_UART1_Rx            = 4,
    DMA_UART1_Tx            = 5,
    DMA_UART2_Rx            = 6,
    DMA_UART2_Tx            = 7,
    DMA_UART3_Rx            = 8,
    DMA_UART3_Tx            = 9,
    DMA_UART4_Rx_Tx         = 10,

    //        I2S            
    DMA_I2S0_Rx             = 12,
    DMA_I2S0_Tx             = 13,

    //        SPI            
    DMA_SPI0_Rx             = 14,
    DMA_SPI0_Tx             = 15,
    DMA_SPI1_Rx             = 16,
    DMA_SPI1_Tx             = 17,

    //        I2C            
    DMA_I2C0_I2C3           = 18,
    DMA_I2C1_I2C2           = 19,

    //        FTM            
    DMA_FTM0_CH0            = 20,
    DMA_FTM0_CH1            = 21,
    DMA_FTM0_CH2            = 22,
    DMA_FTM0_CH3            = 23,
    DMA_FTM0_CH4            = 24,
    DMA_FTM0_CH5            = 25,
    DMA_FTM0_CH6            = 26,
    DMA_FTM0_CH7            = 27,

    DMA_FTM1_CH0            = 28,
    DMA_FTM1_CH1            = 29,

    DMA_FTM2_CH0            = 30,
    DMA_FTM2_CH1            = 31,

    DMA_FTM3_CH0            = 32,
    DMA_FTM3_CH1            = 33,
    DMA_FTM3_CH2            = 34,
    DMA_FTM3_CH3            = 35,
    DMA_FTM3_CH4            = 36,
    DMA_FTM3_CH5            = 37,
    DMA_FTM3_CH6            = 38,
    DMA_FTM3_CH7            = 39,

    //     ADC/DAC/CMP/CMT    
    DMA_ADC0                = 40,
    DMA_ADC1                = 41,
    DMA_CMP0                = 42,
    DMA_CMP1                = 43,
    DMA_CMP2                = 44,
    DMA_DAC0                = 45,
    DMA_DAC1                = 46,
    DMA_CMT                 = 47,

    DMA_PDB                 = 48,

    DMA_PORTA              = 49,
    DMA_PORTB              = 50,
    DMA_PORTC              = 51,
    DMA_PORTD              = 52,
    DMA_PORTE              = 53,

    DMA_IEEE_TIME0         = 54,
    DMA_IEEE_TIME1         = 55,
    DMA_IEEE_TIME2         = 56,
    DMA_IEEE_TIME3         = 57,

    
    
    DMA_LPUART0_Rx         = 58,
    DMA_LPUART0_Tx         = 59,
    
    DMA_Always_EN3          = 60,
    DMA_Always_EN4          = 61,
    DMA_Always_EN5          = 62,
    DMA_Always_EN6          = 63,
} DMA_sources;


typedef enum
{
    DADDR_RECOVER = 0,        //恢复目的地址
    DADDR_KEEPON  = 1,        //目的地址保持不变
} DMA_cfg;

typedef enum      //DMA每次传输字节数
{
    DMA_BYTE1 = 0,
    DMA_BYTE2 = 1,
    DMA_BYTE4 = 2,
    DMA_BYTE16 = 4
} DMA_BYTEn;

typedef enum
{
    DMA_CH0,
    DMA_CH1,
    DMA_CH2,
    DMA_CH3,
    DMA_CH4,
    DMA_CH5,
    DMA_CH6,
    DMA_CH7,
    DMA_CH8,
    DMA_CH9,
    DMA_CH10,
    DMA_CH11,
    DMA_CH12,
    DMA_CH13,
    DMA_CH14,
    DMA_CH15
} DMA_CHn;

#define  DMA_IRQ_EN(DMA_CHn)    enable_irq((IRQn_Type)((IRQn_Type)DMA_CHn + DMA0_IRQn))               //允许DMA通道传输完成中断
#define  DMA_IRQ_DIS(DMA_CHn)   disable_irq((IRQn_Type)((IRQn_Type)DMA_CHn + DMA0_IRQn))              //禁止DMA通道传输完成中断

#define  DMA_IRQ_CLEAN(DMA_CHn) DMA0->INT|=(DMA_INT_INT0_MASK<<DMA_CHn)           //清除通道传输中断标志位

#define  DMA_EN(DMA_CHn)        DMA0->ERQ |= (DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //使能通道硬件DMA请求
#define  DMA_DIS(DMA_CHn)       DMA0->ERQ &=~(DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //禁止通道硬件DMA请求

#define  DMA_CHn_DIS(DMA_CHn)   DMAMUX->CHCFG[DMA_CHn] &= ~DMAMUX_CHCFG_SOURCE(Channel_Disabled)  //禁用通道



//DMA 重新配置，传输完毕后，一些参数会改变，需要重新赋值
__STATIC_INLINE void dma_repeat(DMA_CHn CHn,void *SADDR, void *DADDR,uint32 count)//使用__STATIC_INLINE为了将这段函数内嵌到使用该函数的地方，这样可以减少函数调用的时间
{
    DMA_IRQ_CLEAN(CHn);
    DMA0->TCD[CHn].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(count);  //当前主循环次数 当前循环次数耗尽后再次加载起始主循环次数
    DMA0->TCD[CHn].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(count);  //起始主循环次数

    DMA0->TCD[CHn].SADDR = (uint32)SADDR;                           // 设置  源地址
    DMA0->TCD[CHn].DADDR = (uint32)DADDR;                           // 设置目的地址

    DMA_EN(DMA_CH0);                                                //使能DMA 硬件请求

}


void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTX_n ptxn, DMA_BYTEn byten, uint32 count, uint32 cfg);



#endif
