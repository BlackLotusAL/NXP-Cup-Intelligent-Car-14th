/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "common.h"

//-------------------------------------------------------------------------*
//函数名: stop                                                             *
//功  能: 设置CPU为STOP模式                                                *
//参  数: 无								   *
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void stop (void)
{
    //置位SLEEPDEEP来使能STOP模式
  //  SCB->SCR |= SCB_SCR_SLEEPDEEP_MASK;
    //进入STOP模式
  //  asm("WFI");
}

//-------------------------------------------------------------------------*
//函数名: wait                                                             *
//功  能: 设置CPU为WAIT模式                                                *
//参  数: 无								   *
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wait (void)
{
    //清SLEEPDEEP位来确定进入WAIT模式
   // SCB->SCR &= ~SCB_SCR_SLEEPDEEP_MASK;
    //进入WAIT模式
   // asm("WFI");
}

//-------------------------------------------------------------------------*
//函数名: write_vtor                                                       *
//功  能: 更改中断向量表偏移寄存器的值                                     *
//参  数: 要更改的值    						   *
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void write_vtor (int vtor)
{
    //写新值
    SCB->VTOR = vtor;
}

//-------------------------------------------------------------------------*
//函数名: enable_irq                                                       *
//功  能: 使能irq中断                                                      *
//参  数: irq:irq号       						   *
//返  回: 无                                                               *
//说  明: irq号不是中断向量号                                              *
//-------------------------------------------------------------------------*
void enable_irq (int irq)
{
  /*  int div;

    //确定irq号为有效的irq号
    if (irq > 91)	irq=91;

    //确定对应的NVICISER
    div = irq/32;

     switch (div)
    {
    	case 0x0:
              NVICICPR0 = 1 << (irq%32);
              NVICISER0 = 1 << (irq%32);
              break;
    	case 0x1:
              NVICICPR1 = 1 << (irq%32);
              NVICISER1 = 1 << (irq%32);
              break;
    	case 0x2:
              NVICICPR2 = 1 << (irq%32);
              NVICISER2 = 1 << (irq%32);
              break;
    } */
}

//-------------------------------------------------------------------------*
//函数名: disable_irq                                                      *
//功  能: 禁止irq中断                                                      *
//参  数: irq:irq号       						   *
//返  回: 无                                                               *
//说  明: irq号不是中断向量号                                              *
//-------------------------------------------------------------------------*
void disable_irq (int irq)
{
 /*   int div;

    //确定irq号为有效的irq号
    if (irq > 91)	irq=91;

    //确定对应的NVICISER
    div = irq/32;

    switch (div)
    {
    	case 0x0:
               NVICICER0 = 1 << (irq%32);
              break;
    	case 0x1:
              NVICICER1 = 1 << (irq%32);
              break;
    	case 0x2:
              NVICICER2 = 1 << (irq%32);
              break;
    }  */
}

//-------------------------------------------------------------------------*
//函数名: set_irq_priority                                                 *
//功  能: 设置irq中断和优先级                                              *
//参  数: irq:irq号         						   *
//        prio:优先级						           *
//返  回: 无                                                               *
//说  明: irq号不是中断向量号                                              *
//-------------------------------------------------------------------------*
void set_irq_priority (int irq, int prio)
{
 /*   uint8 *prio_reg;

    //确定irq号和优先级有效
    if (irq > 91)	irq=91;
    if (prio > 15)	prio=15;

    //确定对应的NVICISER
    prio_reg = (uint8 *)(((uint32)&NVICIP0) + irq);
    //设置优先级
    *prio_reg = ( (prio&0xF) << (8 - ARM_INTERRUPT_LEVEL_BITS) ); */
}

