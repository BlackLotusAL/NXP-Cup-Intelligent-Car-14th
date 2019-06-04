/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_uart
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#include "MK60_uart.h"




UART_Type * uart[5] = UART_BASE_PTRS;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      uart 初始化
//  @param      uartn           选择串口(uart0,uart1,uart2,uart3,uart4,uart5)
//  @param      baud            串口波特率
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_init(uart3,9600);   //初始化串口3为1位起始位、8位数据位、1位停止位、波特率9600
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UARTn uartn, uint32 baud)
{
    vuint32 uart_input_clk;
    vuint32 sbr,brfa;
    //进行管脚复用设置
    switch(uartn)
    {
        case uart0:
        {
            SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UART0 时钟
            if(UART0_RX_PIN == A1)                                                              port_init( UART0_RX_PIN, ALT2);
            else if((UART0_RX_PIN == A15) || (UART0_RX_PIN == B16) || (UART0_RX_PIN == D6) )    port_init( UART0_RX_PIN, ALT3);

            if(UART0_TX_PIN == A2)                                                              port_init( UART0_TX_PIN, ALT2);
            else if((UART0_TX_PIN == A14) || (UART0_TX_PIN == B17) || (UART0_TX_PIN == D7) )    port_init( UART0_TX_PIN, ALT3);
        }break;
        
        case uart1:
        {
            SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;      //使能 UART1 时钟
            if((UART1_RX_PIN == C3) || (UART1_RX_PIN == E1))                                    port_init( UART1_RX_PIN, ALT3);
            if((UART1_TX_PIN == C4) || (UART1_TX_PIN == E0))                                    port_init( UART1_TX_PIN, ALT3);
        }break;
        
        case uart2:
        {
            SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;      //使能 UART2 时钟
            if(UART2_TX_PIN == D3)                                                              port_init( UART2_TX_PIN, ALT3);
            if(UART2_RX_PIN == D2)                                                              port_init( UART2_RX_PIN, ALT3);
        }break;
        
        case uart3:
        {
            SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;      //使能 UART3 时钟
            if((UART3_RX_PIN == B10) || (UART3_RX_PIN == C16) || (UART3_RX_PIN == E5) )         port_init( UART3_RX_PIN, ALT3);
            if((UART3_TX_PIN == B11) || (UART3_TX_PIN == C17) || (UART3_TX_PIN == E4) )         port_init( UART3_TX_PIN, ALT3);
        }break;
        
        case uart4:
        {
            SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;      //使能 UART4 时钟
            if((UART4_RX_PIN == C14) || (UART4_RX_PIN == E25)  )                                port_init( UART4_RX_PIN, ALT3);
            if((UART4_TX_PIN == C15) || (UART4_TX_PIN == E24)  )                                port_init( UART4_TX_PIN, ALT3);
        }break;
        
        default:
        return;
    }
    
    //设置的时候，应该禁止发送和接收
    uart[uartn]->C2 &= ~(0
                         | UART_C2_TE_MASK
                         | UART_C2_RE_MASK);
    
    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    uart[uartn]->C1 |= (0
                        //| UART_C1_M_MASK                      //9 位或 8 位模式选择 : 0 为 8位 ，1 为 9位（注释了表示0，即8位） （如果是9位，位8在UARTx_C3里）
                        //| UART_C1_PE_MASK                     //奇偶校验使能（注释了表示禁用）
                        //| UART_C1_PT_MASK                     //校验位类型 : 0 为 偶校验 ，1 为 奇校验
                       );
    
    //计算波特率，串口0、1使用内核时钟，其它串口使用bus时钟
    if ((uartn == uart0) || (uartn == uart1)) uart_input_clk = core_clk_mhz * 1000000;  //内核时钟
    else                                            uart_input_clk = bus_clk_mhz * 1000000;   //bus时钟

    //UART 波特率 = UART 模块时钟 / (16 × (SBR[12:0] + BRFA/32))
    //不考虑 BRFA 的情况下， SBR = UART 模块时钟 / (16 * UART 波特率)
    sbr = (uint16)(uart_input_clk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR 是 13bit，最大为 0x1FFF

    //已知 SBR ，则 BRFA = UART 模块时钟 * 2/ UART 波特率 - 32 ×SBR[12:0]
    brfa = (uart_input_clk * 2 / baud)  - (sbr * 32);
    if(brfa>0x1F)   brfa = 0x1F;
    
    //写 SBR
    uart[uartn]->BDH &= ~UART_BDH_SBR_MASK;                     //清除原来波特率
    uart[uartn]->BDH |= UART_BDH_SBR(sbr>>8);                   //先写入SBR高位
    uart[uartn]->BDL  = UART_BDL_SBR((uint8)sbr);               //再写入SBR低位

    //写 BRFD
    uart[uartn]->C4 &= ~UART_C4_BRFA_MASK;
    uart[uartn]->C4 |= UART_C4_BRFA(brfa);

    //设置FIFO(FIFO的深度是由硬件决定的，软件不能设置)
    uart[uartn]->PFIFO |= (0
                           | UART_PFIFO_TXFE_MASK               //使能TX FIFO(注释表示禁止)
                           //| UART_PFIFO_TXFIFOSIZE(0)         //（只读）TX FIFO 大小，0为1字节，1~6为 2^(n+1)字节
                           | UART_PFIFO_RXFE_MASK               //使能RX FIFO(注释表示禁止)
                           //| UART_PFIFO_RXFIFOSIZE(0)         //（只读）RX FIFO 大小，0为1字节，1~6为 2^(n+1)字节
                           );

    // 允许发送和接收
    
    uart[uartn]->C2 |= (0
                        | UART_C2_TE_MASK                       //发送使能
                        | UART_C2_RE_MASK                       //接收使能
                        //| UART_C2_TIE_MASK                    //发送中断或DMA传输请求使能（注释了表示禁用）
                        //| UART_C2_TCIE_MASK                   //发送完成中断使能（注释了表示禁用）
                        //| UART_C2_RIE_MASK                    //接收满中断或DMA传输请求使能（注释了表示禁用）
                        );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送一个字节
//  @param      uartn           选择串口
//  @param      ch              要发送的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putchar(uart3,0x5a);   
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UARTn uartn, uint8 ch)
{
    while( !((uart[uartn]->S1) & UART_S1_TDRE_MASK) );  //等待发送缓冲区空
    uart[uartn]->D = (uint8)ch;                         //发送数据
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      uartn           选择串口
//  @param      *buff           要发送的数组地址
//  @param      len             发送长度
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putbuff(uart3,&a[0],sizeof(a));  
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff (UARTn uartn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uartn, *buff);
        buff++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      uartn           选择串口
//  @param      *str            要发送的字符串地址
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putstr(uart3,"i lvoe you"); 
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr (UARTn uartn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uartn, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口等待接收1个字符
//  @param      uartn           选择串口
//  @param      *str            接收的地址
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_getchar(uart3,&dat);
//-------------------------------------------------------------------------------------------------------------------
void uart_getchar (UARTn uartn, uint8 *ch) 
{
    while(!(uart[uartn]->S1 & UART_S1_RDRF_MASK));  //等待接收满了   
    *ch =  uart[uartn]->D;                          // 获取接收到的8位数据
    // 获取 9位数据时ch 应该是uint16 *类型的否则数据会溢出：
    // *ch =   ((( UARTx_C3_REG(UARTN[uartn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uartn]);  //返回9bit
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口查询接收1个字符
//  @param      uartn           选择串口
//  @param      *str            接收的地址
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_querychar(uart3,&dat);
//-------------------------------------------------------------------------------------------------------------------
char uart_querychar (UARTn uartn, uint8 *ch)
{
    if( uart[uartn]->RCFIFO )         //查询是否接受到数据
    {
        *ch  =   uart[uartn]->D;      //接受到8位的数据
        return  1;                    //返回 1 表示接收成功
    }
    return 0;                         //返回0表示接收失败
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启接收中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_rx_irq_en(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq_en(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_RIE_MASK;                                        //使能UART接收中断
    if(uartn<=3)enable_irq((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));       //使能IRQ中断
    else        enable_irq((IRQn_Type)(UART4_RX_TX_IRQn));                       //使能IRQ中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启发送完成中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_txc_irq_en(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_txc_irq_en(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_TCIE_MASK;                                       //使能UART发送完成中断
    if(uartn<=3)enable_irq((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));       //使能IRQ中断
    else        enable_irq((IRQn_Type)(UART4_RX_TX_IRQn));                      //使能IRQ中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁止接收中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_rx_irq_dis(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq_dis(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_RIE_MASK;                                       //禁止UART接收中断

    //如果发送中断还没有关，则不关闭IRQ
    if(!(uart[uartn]->C2 & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        if(uartn<=3)disable_irq((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));  //关IRQ中断
        else        disable_irq((IRQn_Type)(UART4_RX_TX_IRQn));                 //关IRQ中断
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁止发送完成中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_txc_irq_dis(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_txc_irq_dis(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_TCIE_MASK;                                      //禁止UART发送完成中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(uart[uartn]->C2 & UART_C2_RIE_MASK) )
    {
        if(uartn<=3)disable_irq((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));  //关IRQ中断
        else        disable_irq((IRQn_Type)(UART4_RX_TX_IRQn));                 //关IRQ中断
    }
}

//串口中断函数请到isr.c文件参考


//-------------------------------------------------------------------------------------------------------------------
//  @brief      重定义printf 到串口
//  @param      ch      需要打印的字节
//  @param      stream  数据流
//  @since      v1.0
//  @note       此函数由编译器自带库里的printf所调用
//-------------------------------------------------------------------------------------------------------------------
int fputc(int ch, FILE *stream)
{
    uart_putchar(DEBUG_PORT, (char)ch);
    return(ch);
}

