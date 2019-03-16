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
#include "Lptmr.h"

void time_delay_ms(u16 ms)
{
  LPTMR_delay_ms(ms);
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_pulse_init                                                        
//功  能: LPTMR脉冲计数初始化                                                        
//参  数: LPT0_ALTn:LPTMR脉冲计数管脚
//        count   :LPTMR脉冲比较值
//        LPT_CFG :LPTMR脉冲计数方式：上升沿计数或下降沿计数
//返  回: 无                                                              
//简  例:  LPTMR_pulse_init(LPT0_ALT1,32768,LPT_Rising);                                
//-------------------------------------------------------------------------*
void LPTMR_Pulse_Init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg)
{

    // 开启模块时钟
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                        //使能LPT模块时钟

    //设置输入管脚
    if(altn == LPT0_ALT1)
    {
         PORTA_PCR19= PORT_PCR_MUX(6);
    }
    else if(altn == LPT0_ALT2)
    {
         PORTC_PCR5= PORT_PCR_MUX(4);
    }
    else                                    //不可能发生事件
    {
       ;  
    }

    // 清状态寄存器
    LPTMR0_CSR = 0x00;                                          //先关了LPT，这样才能设置时钟分频,清空计数值等


    //选择时钟源
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)           //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );


    // 设置累加计数值
    LPTMR0_CMR  =   LPTMR_CMR_COMPARE(count);                   //设置比较值

    // 管脚设置、使能中断
    LPTMR0_CSR  =  (0
                    | LPTMR_CSR_TPS(altn)       // 选择输入管脚 选择
                    | LPTMR_CSR_TMS_MASK        // 选择脉冲计数 (注释了表示时间计数模式)
                    | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    | LPTMR_CSR_TIE_MASK        //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_pulse_get                                                        
//功  能: 获取LPTMR脉冲计数值                                                        
//返  回: 脉冲计数值                                                              
//简  例: 无                                
//-------------------------------------------------------------------------*
uint16 LPTMR_Pulse_Get(void)
{
    uint16 data;
    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //已经溢出了
    {

        data = ~0;                          //返回 0xffffffff 表示错误
    }
    else
    {

        data = LPTMR0_CNR;
    }
    return data;
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_pulse_clean                                                        
//功  能: 清空LPTMR脉冲计数                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: 无                                
//-------------------------------------------------------------------------*
void LPTMR_Pulse_Clean(void)
{
    LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;     //禁用LPT的时候就会自动清计数器的值
    LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_delay_ms                                                        
//功  能: LPTMR延时函数（ms）                                                        
//参  数: ms    毫秒
//返  回: 无                                                              
//简  例: LPTMR_delay_ms(32);     // LPTMR 延时32ms                                
//-------------------------------------------------------------------------*
void LPTMR_delay_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;    //使能LPT模块时钟

    LPTMR0_CSR = 0x00;                      //先关了LPT，自动清计数器的值

    LPTMR0_CMR = ms;                        //设置比较值，即延时时间

    //选择时钟源
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)           //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    //| LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );

    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); //等待比较值与计数值相等，即时间到了

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;          //清除比较标志位

    return;
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_timing_ms                                                        
//功  能: LPTMR定时函数（ms）                                                        
//参  数: ms          LPTMR定时时间(0~65535)
//返  回: 无                                                              
//简  例: LPTMR_timing_ms(32);     // LPTMR 定时32ms                                
//-------------------------------------------------------------------------*
void LPTMR_timing_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;    //使能LPT模块时钟

    LPTMR0_CSR = 0x00;                      //先关了LPT，自动清计数器的值

    LPTMR0_CMR = ms;                        //设置比较值，即延时时间

    //选择时钟源
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)           //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    | LPTMR_CSR_TIE_MASK        //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );

   enable_irq(85);                           // 使能LPTMR中断
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_delay_us                                                        
//功  能: LPTMR延时函数（us）                                                        
//参  数: us          LPTMR延时时间(0~41942)
//返  回: 无                                                              
//简  例:  LPTMR_delay_us(32);     // LPTMR 延时32us                              
//-------------------------------------------------------------------------*
void LPTMR_delay_us(uint16 us)
{


    if(us == 0)
    {
        return;
    }

    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //使能 OSCERCLK


    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                        //使能LPT模块时钟

    LPTMR0_CSR = 0x00;                                          //先关了LPT，自动清计数器的值

    LPTMR0_CMR = (us * 50 + 16) / 32;                 //设置比较值，即延时时间

    //选择时钟源
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      | LPTMR_PSR_PRESCALE(4)     //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    //| LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );

    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); //等待比较值与计数值相等，即时间到了

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;          //清除比较标志位

    
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_timing_us                                                        
//功  能: LPTMR定时函数（us）                                                        
//参  数: us        LPTMR定时时间(0~41942)
//返  回: 无                                                              
//简  例: LPTMR_timing_us(32);     // LPTMR 定时32us                           
//-------------------------------------------------------------------------*
void LPTMR_timing_us(uint16 us)
{

    if(us == 0)
    {
        return;
    }


    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //使能 OSCERCLK


    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                        //使能LPT模块时钟

    LPTMR0_CSR = 0x00;                                          //先关了LPT，自动清计数器的值

    LPTMR0_CMR = (us * 50 + 16) / 32;                 //设置比较值，即延时时间

    //选择时钟源
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      | LPTMR_PSR_PRESCALE(4)     //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    | LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );

    enable_irq(85);                           // 使能LPTMR中断
}

//-------------------------------------------------------------------------*
//函数名: LPTMR计时函数（ms,计时时间最大为65534ms）                                                      
//功  能: LPTMR计时函数                                                        
//参  数: 无
//返  回: 无                                                              
//简  例:          
/*                 void my_delay(uint32 time)
                    {
                        volatile uint32 i = time;
                        while(i--);

                    }


                    LPTMR_time_start_ms();

                    my_delay(600000);
                    i = LPTMR_time_get_ms();
                    if(i == ~0)
                    {
                        UART_Put_Str("\n计时时间超时");
                    }
                    else
                    {
                        UART_Put_Str("\n计时时间为：%dms",i);
                    }
 */
                        
//-------------------------------------------------------------------------*
void LPTMR_time_start_ms(void)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;    //使能LPT模块时钟

    LPTMR0_CSR = 0x00;                      //先关了LPT，自动清计数器的值

    LPTMR0_CMR = ~0;                        //设置比较值，即延时时间

    //选择时钟源
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)           //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    //| LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );
    
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_time_get_ms                                                        
//功  能: 获取LPTMR计时时间（ms）                                                        
//参  数: 无
//返  回: 计时时间（返回值为 ~0 表示计时超时，其他值在 0~ 65534ms 区间内 ）                                                              
//简  例: 参考 LPTMR_time_start_ms 的调用例子                          
//-------------------------------------------------------------------------*

uint32 LPTMR_time_get_ms(void)
{
    uint32 data;

    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //已经溢出了
    {

        data = ~0;                          //返回 0xffffffff 表示错误
    }
    else
    {
        data = LPTMR0_CNR;                  //返回计时时间(此值最大为 0xffff)
    }

    return data;
}

/*!
 *  @brief      关闭 LPTMR计时
 *  @since      v5.0
 */

//-------------------------------------------------------------------------*
//函数名: LPTMR_time_close                                                        
//功  能: 关闭 LPTMR计时                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: 无                        
//-------------------------------------------------------------------------*
void LPTMR_time_close()
{
    LPTMR0_CSR = 0x00;                                          //先关了LPT，自动清计数器的值，清空溢出标记
}



//-------------------------------------------------------------------------*
//函数名: LPTMR计时函数（us,计时时间最大为41942us）                                                      
//功  能: LPTMR计时函数                                                        
//参  数: 无
//返  回: 无                                                              
//简  例:          
/*                 void my_delay(uint32 time)
                    {
                        volatile uint32 i = time;
                        while(i--);

                    }

                    uint32 i;
                    LPTMR_time_start_us();

                    my_delay(5894);

                    i = LPTMR_time_get_us();
                    if(i == ~0)
                    {
                        UART_Put_Str("\n计时时间超时");
                    }
                    else
                    {
                        UART_Put_Str("\n计时时间为：%dus",i);
                    }
 */
                        
//-------------------------------------------------------------------------*
void LPTMR_time_start_us(void)
{


    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //使能 OSCERCLK

    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                        //使能LPT模块时钟

    LPTMR0_CSR = 0x00;                                          //先关了LPT，自动清计数器的值

    LPTMR0_CMR = ~0;                                            //设置比较值为最大值

    //选择时钟源
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      | LPTMR_PSR_PRESCALE(4)     //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    //| LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_time_get_us                                                        
//功  能: 获取LPTMR计时时间（us）                                                        
//参  数: 无
//返  回: 计时时间（返回值为 ~0 表示计时超时，其他值在 0~ 41942us 区间内 ）                                                              
//简  例: 参考 LPTMR_time_start_us 的调用例子                          
//-------------------------------------------------------------------------*
uint32 LPTMR_time_get_us(void)
{
    uint32 data;

    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //已经溢出了
    {

        data = ~0;                          //返回 0xffffffff 表示错误
    }
    else
    {

        data = (LPTMR0_CNR * 32) / 50; //进行单位换算
    }

    return data;
}



//-------------------------------------------------------------------------*
//函数名: LPTMR_interrupt                                                        
//功  能: LPTMR定时中断函数                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: 无                               
//-------------------------------------------------------------------------*
void LPTMR_Interrupt(void)
{
    LPTMR_Flag_Clear();

    //下面由用户添加实现代码
}


