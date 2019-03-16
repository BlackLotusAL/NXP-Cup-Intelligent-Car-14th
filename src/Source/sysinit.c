//-------------------------------------------------------------------------*
// 文件名:sysinit.c                                                        *
// 说  明: 系统配置文件                                                    *
//-------------------------------------------------------------------------*

#include "sysinit.h"	//头文件
#include "include.h"
#include "PLL.h"

//全局变量声明
int core_clk_khz;
int core_clk_mhz;
int periph_clk_khz;

//-------------------------------------------------------------------------*
//函数名: sysinit                                                          *
//功  能: 系统设置                                                         * 
//参  数: 无						  	           *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void sysinit (void)
{
    //使能IO端口时钟    
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                              | SIM_SCGC5_PORTB_MASK
                              | SIM_SCGC5_PORTC_MASK
                              | SIM_SCGC5_PORTD_MASK
                              | SIM_SCGC5_PORTE_MASK );

    //开启系统时钟
    core_clk_mhz = pll_init(PLL180);
    //通过pll_init函数的返回值来计算内核时钟和外设时钟
    core_clk_khz = core_clk_mhz * 1000;
    periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
    //使能跟踪时钟，用于调试
    trace_clk_init();	
    //FlexBus时钟初始化
    fb_clk_init();

}

//-------------------------------------------------------------------------*
//函数名: trace_clk_init                                                   *
//功  能: 跟踪时钟初始化                                                   * 
//参  数: 无							  	   *	
//返  回: 无                                                               *
//说  明: 用于调试                                                         *
//-------------------------------------------------------------------------*
void trace_clk_init(void)
{
    //设置跟踪时钟为内核时钟
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;	
    //在PTA6引脚上使能TRACE_CLKOU功能
    PORTA_PCR6 = ( PORT_PCR_MUX(0x7));
}

//-------------------------------------------------------------------------*
//函数名: fb_clk_init                                                      *
//功  能: FlexBus时钟初始化                                                * 
//参  数: 无								   *	
//返  回: 无                                                               *
//说  明:                                                                  *
//-------------------------------------------------------------------------*
void fb_clk_init(void)
{
    //使能FlexBus模块时钟
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
    //在PTA6引脚上使能FB_CLKOUT功能
    PORTC_PCR3 = ( PORT_PCR_MUX(0x5));
}

//-------------------------------------------------------------------------*
//函数名: pll_init                                                         *
//功  能: pll初始化                                                        * 
//参  数: clk_option:时钟选项						   * 
//		  crystal_val:时钟值                                       *	
//返  回: 时钟频率值                                                       *
//说  明:                                                                  *
//-------------------------------------------------------------------------*
unsigned char pll_init(unsigned char clk_option, unsigned char crystal_val)
{
    unsigned char pll_freq;
    
    if (clk_option > 3) {return 0;}   //如果没有选择可用的选项则返回0
    if (crystal_val > 15) {return 1;} // 如果如果可用的晶体选项不可用则返回1
    
    //这里处在默认的FEI模式
    //首先移动到FBE模式
    #if (defined(K60_CLK) || defined(ASB817))
             MCG_C2 = 0;
    #else
             //使能外部晶振
             MCG_C2 = MCG_C2_RANGE(2) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
    #endif
    
    //初始化晶振后释放锁定状态的振荡器和GPIO
    SIM_SCGC4 |= SIM_SCGC4_LLWU_MASK;
    LLWU_CS |= LLWU_CS_ACKISO_MASK;
    
    //选择外部晶振，参考分频器，清IREFS来启动外部晶振
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);
    
    //等待晶振稳定	
    #if (!defined(K60_CLK) && !defined(ASB817))
    while (!(MCG_S & MCG_S_OSCINIT_MASK)){};  
    #endif
    
    //等待参考时钟状态位清零
    while (MCG_S & MCG_S_IREFST_MASK){}; 
    //等待时钟状态位显示时钟源来自外部参考时钟
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; 
    
    //进入FBE模式
    #if (defined(K60_CLK))
    MCG_C5 = MCG_C5_PRDIV(0x18);
    #else
    
    //配置PLL分频器来匹配所用的晶振
    MCG_C5 = MCG_C5_PRDIV(crystal_val); 
    #endif
    
    //确保MCG_C6处于复位状态，禁止LOLIE、PLL、和时钟控制器，清PLL VCO分频器
    MCG_C6 = 0x0;
    //选择PLL VCO分频器，系统时钟分频器取决于时钟选项
    switch (clk_option) {
    case 0:
      //设置系统分频器
      //MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
      set_sys_dividers(0,0,0,1);
      //设置VCO分频器，使能PLL为50MHz, LOLIE=0, PLLS=1, CME=0, VDIV=1
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(1); //VDIV = 1 (x25)
      pll_freq = 50;
      break;
    case 1:
      //设置系统分频器
      //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
      set_sys_dividers(0,1,1,3);
      //设置VCO分频器，使能PLL为100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(26); //VDIV = 26 (x50)
      pll_freq = 100;
      break;
    case 2:
      //设置系统分频器
      //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
      set_sys_dividers(0,1,1,3);
      //设置VCO分频器，使能PLL为96MHz, LOLIE=0, PLLS=1, CME=0, VDIV=24
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24); //VDIV = 24 (x48)
      pll_freq = 96;
      break;
    case 3:
      //设置系统分频器
      //MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
      set_sys_dividers(0,0,0,1);
      //设置VCO分频器，使能PLL为48MHz, LOLIE=0, PLLS=1, CME=0, VDIV=0
      MCG_C6 = MCG_C6_PLLS_MASK; //VDIV = 0 (x24)
      pll_freq = 48;
      break;
    }
    while (!(MCG_S & MCG_S_PLLST_MASK)){}; // wait for PLL status bit to set
    
    while (!(MCG_S & MCG_S_LOCK_MASK)){}; // Wait for LOCK bit to set
    
    //进入PBE模式
    
    //通过清零CLKS位来进入PEE模式
    // CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
    MCG_C1 &= ~MCG_C1_CLKS_MASK;
    
    //等待时钟状态位更新
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};
    
    //开始进入PEE模式
    
    return pll_freq;
}

//-------------------------------------------------------------------------*
//函数名: set_sys_dividers                                                 *
//功  能: 设置系系统分频器                                                 * 
//参  数: 预分频值   							   *	
//返  回: 无                                                               *
//说  明: 此函数必须放在RAM里执行，否则会产生错误e2448。当FLASH时钟分频改变* 
//        时，必须禁止FLASH的预取功能。在时钟分频改变之后，必须延时一小段时*
//	 间才可以从新使能预取功能。                                        * 
//-------------------------------------------------------------------------*
__ramfunc void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)
{
    uint32 temp_reg;
    uint8 i;
    //保存FMC_PFAPR当前的值
    temp_reg = FMC_PFAPR;
    
    //通过M&PFD置位M0PFD来禁止预取功能
    FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
                     | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
                     | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
    
    //给时钟分频器设置期望值  
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2) 
                      | SIM_CLKDIV1_OUTDIV3(outdiv3) | SIM_CLKDIV1_OUTDIV4(outdiv4);
    
    //等待分频器改变
    for (i = 0 ; i < outdiv4 ; i++)
    {}
    
    //从新存FMC_PFAPR的原始值
    FMC_PFAPR = temp_reg; 
    
    return;
}






