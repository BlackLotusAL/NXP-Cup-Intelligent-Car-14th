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

#ifndef __LPTMR_H__
#define __LPTMR_H__

/**
 *  @brief LPTMR脉冲计数输入管脚选项
 */
typedef enum
{
    //只有1、2管脚，并没有0、3管脚
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;

/**
 *  @brief LPTMR脉冲计数触发方式
 */
typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //上升沿触发
    LPT_Falling = 1     //下降沿触发
} LPT_CFG;

#define LPTMR_Flag_Clear()  (LPTMR0_CSR |= LPTMR_CSR_TCF_MASK)         //清除LPT比较标志位


extern void time_delay_ms(u16);


/*          用于延时         */
extern void     LPTMR_delay_ms(uint16 ms);      //延时(ms)
extern void     LPTMR_delay_us(uint16 us);      //延时(us)

/*       用于定时           */
extern void     LPTMR_timing_ms(uint16 ms);     //定时(ms)
extern void     LPTMR_timing_us(uint16 ms);     //定时(us)

/*        用于计时           */
extern void     LPTMR_time_start_ms(void);      //开始计时(ms)
extern uint32   LPTMR_time_get_ms(void);        //获取计时时间 

extern void     LPTMR_time_start_us(void);      //开始计时(ns)
extern uint32   LPTMR_time_get_us(void);        //获取计时时间 

extern void     LPTMR_time_close();             //关闭计时器


/*       用于脉冲计数        */
extern void     LPTMR_Pulse_Init  (LPT0_ALTn, uint16 count, LPT_CFG);   //计数器初始化设置
extern uint16   LPTMR_Pulse_Get   (void);                               //获取计数值
extern void     LPTMR_Pulse_Clean (void);                               //清空计数值


/*      中断复位函数模版    */
extern void LPTMR_test_handler(void);                                       //中断复位函数，仅供参考（需用户自行实现）
extern void LPTMR_Interrupt(void);

#endif 
