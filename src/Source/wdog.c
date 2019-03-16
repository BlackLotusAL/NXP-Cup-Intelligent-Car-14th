/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "wdog.h"	//包含wdog.h头文件
       
//-------------------------------------------------------------------------*
//函数名: wdog_disable                                                     *
//功  能: 关闭看门狗模块                                                   * 
//参  数: 无			  					   *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_disable(void)
{
    //给看门狗模块解锁，以便写寄存器
    wdog_unlock();
    //关闭看门狗
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

//-------------------------------------------------------------------------*
//函数名: wdog_unlock                                                      *
//功  能: 看门狗解锁                                                       * 
//参  数: 无							           *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_unlock(void)
{
    /* 注意: 不要单步调试此程序!!! ，否则会引起CPU复位*/
    //关总中断
    DisableInterrupts;		
    //写解锁寄存器
    WDOG_UNLOCK = 0xC520;	
    //完成解锁
    WDOG_UNLOCK = 0xD928;	
    //开总中断
    EnableInterrupts;		
}
