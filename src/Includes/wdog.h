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

#ifndef __WDOG_H__
#define __WDOG_H__

//-------------------------------------------------------------------------*
//函数名: wdog_disable                                                     *
//功  能: 关闭看门狗模块                                                   * 
//参  数: 无							       *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_disable(void);

//-------------------------------------------------------------------------*
//函数名: wdog_unlock                                                      *
//功  能: 看门狗解锁                                                       * 
//参  数: 无					             	       *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_unlock(void);

#endif 
