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

#ifndef __PLL_H__
#define __PLL_H__

extern uint32 core_clk_M;//单位MHZ
extern uint32 bus_clk_M;//单位MHZ

//PLL参数
typedef enum clk_option
{
    PLLUNULL ,
    PLL80    ,
    PLL90    ,
    PLL100   ,
    PLL120   ,
    PLL130   ,
    PLL140   ,
    PLL150   ,
    PLL160   ,
    PLL170   ,
    PLL180   ,
    PLL200   ,
    PLL220   ,
    PLL225   ,
    PLL230   ,
    PLL235   ,
    PLL237_5 , //实际为237.5M
} clk_option;



/*********************** PLL功能函数 **************************/
void PLL_Init(clk_option);     //锁相环初始化




#endif
