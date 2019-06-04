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
 
 
 
#include "MK60_dma.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA初始化，由IO口请求传输输入端口的数据到内存
//  @param      DMA_CHn         通道号（DMA_CH0 ~ DMA_CH15）
//  @param      SADDR           源地址( (void * )&X_IN_DATA
//  @param      DADDR           目的地址
//  @param      PTxn            触发端口
//  @param      DMA_BYTEn       每次DMA传输字节数
//  @param      count           一个主循环传输字节数
//  @param      cfg             DMA传输配置，从DMA_cfg里选择
//  @return     void
//  @since      v1.0
//  Sample usage:   uint8 dat[10];
//                  dma_portx2buff_init(DMA_CH0, (void *)&C_IN_DATA(0), dat, A7, DMA_BYTE1, 10, DADDR_RECOVER);
//                  												//DMA初始化，源地址：C_IN_DATA(即C0-C7)，目的地址：dat,A7触发(默认上升沿)，每次传输1字节，共传输 10次 ，传输结束后恢复地址
//                  port_init(A7,ALT1 | DMA_FALLING);               //默认触发源是上升沿，此处改为 下降沿触发
//                  DMA_EN(DMA_CH0);                				//需要使能 DMA 后才能传输数据
//-------------------------------------------------------------------------------------------------------------------
void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTX_n ptxn, DMA_BYTEn byten, uint32 count, uint32 cfg)
{

    uint8 BYTEs = 1;
    uint8 i = DMA_BYTE1;
    uint8 n, tmp;
    uint8 ptxn_s;
    uint8 ptx0 = ((((uint32)SADDR) & 0x1C0) >> 6 ) << 5;
    
    while(i)//计算传输字节数
    {
        BYTEs *= 2;
        i--;
    }
    //DMA 寄存器 配置

    // 开启时钟 
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;                        //打开DMA模块时钟
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                    //打开DMA多路复用器时钟

    // 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) 
    DMA0->TCD[CHn].SADDR = 0;
    DMA0->TCD[CHn].SADDR = (uint32)SADDR;                   // 设置  源地址
    DMA0->TCD[CHn].DADDR = 0;                               
    DMA0->TCD[CHn].DADDR = (uint32)DADDR;                   // 设置目的地址
    DMA0->TCD[CHn].SOFF = 0;                                
    DMA0->TCD[CHn].SOFF = 0x00u;                            // 设置源地址偏移 = 0x0, 即不变
    DMA0->TCD[CHn].DOFF = 0;                                
    DMA0->TCD[CHn].DOFF = BYTEs;                            // 每次传输后，目的地址加 BYTEs
    DMA0->TCD[CHn].ATTR = 0;
    DMA0->TCD[CHn].ATTR = (0
                         | DMA_ATTR_SMOD(0x0)               // 源地址模数禁止  Source address modulo feature is disabled
                         | DMA_ATTR_SSIZE(byten)            // 源数据位宽 ：DMA_BYTEn  。    SSIZE = 0 -> 8-bit ，SSIZE = 1 -> 16-bit ，SSIZE = 2 -> 32-bit ，SSIZE = 4 -> 16-byte
                         | DMA_ATTR_DMOD(0x0)               // 目标地址模数禁止
                         | DMA_ATTR_DSIZE(byten)            // 目标数据位宽 ：DMA_BYTEn  。  设置参考  SSIZE
                        );
    DMA0->TCD[CHn].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(count);   //当前主循环次数 当前循环次数耗尽后再次加载起始主循环次数
    DMA0->TCD[CHn].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(count);   //起始主循环次数
    DMA0->CR &= ~DMA_CR_EMLM_MASK;                                // CR[EMLM] = 0
    DMA0->TCD[CHn].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(BYTEs); // 通道每次传输字节数，这里设置为BYTEs个字节。注：值为0表示传输4GB 
    
    
    // 配置 DMA 传输结束后的操作 
    DMA0->TCD[CHn].SLAST = 0;//调整  源地址的附加值,主循环结束后恢复  源地址
    DMA0->TCD[CHn].DLAST_SGA = (uint32)( (cfg & DADDR_KEEPON ) == 0 ? (-count)  : 0 ); //调整目的地址的附加值,主循环结束后恢复目的地址或者保持地址;
    DMA0->TCD[CHn].CSR = 0;
    DMA0->TCD[CHn].CSR = (0
                             | DMA_CSR_BWC(3)               //带宽控制,每读一次，eDMA 引擎停止 8 个周期（0不停止；1保留；2停止4周期；3停止8周期）
                             | DMA_CSR_DREQ_MASK            //主循环结束后停止硬件请求
                             | DMA_CSR_INTMAJOR_MASK        //主循环结束后产生中断
                            );
    DMA0->TCD[CHn].SLAST = 0;
    
    
    // 配置 DMA 触发源 


    DMAMUX0->CHCFG[CHn] = (0
                            | DMAMUX_CHCFG_ENBL_MASK                        // Enable routing of DMA request 
                            //| DMAMUX_CHCFG_TRIG_MASK                      // Trigger Mode: Periodic   PIT周期触发传输模式   通道1对应PIT1，必须使能PIT1，且配置相应的PIT定时触发 
                            | DMAMUX_CHCFG_SOURCE( PTX(ptxn) + DMA_PORTA)  // 通道触发传输源:    
                          );
    

    //配置触发源（默认是 上升沿触发）
    port_init(ptxn, ALT1 | DMA_RISING);

    //  配置输入源   
    
    n = (uint8)(((uint32)SADDR - ((uint32)(&A_IN_DATA(0)))) & 0x3f) * 8;       //最小的引脚号

    ptxn_s = ptx0 + n;
    tmp = ptxn_s + (BYTEs * 8 ) - 1;                                          //最大的引脚号
    while(ptxn_s <= tmp)
    {
        //由于这里是把端口数据传送到内存中，因此需要设置端口为输入状态
        gpio_init((PTX_n )ptxn_s, GPI, 0);
        port_init((PTX_n )ptxn_s , ALT1 | PULLDOWN );
        ptxn_s ++;
    }
    
    DMA_DIS(CHn);                                    //使能通道CHn 硬件请求
    DMA_IRQ_CLEAN(CHn);

    // 开启中断 
    //DMA_EN(CHn);                                    //使能通道CHn 硬件请求
    //DMA_IRQ_EN(CHn);                                //允许DMA通道传输
}


