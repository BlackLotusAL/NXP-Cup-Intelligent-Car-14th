/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_spi
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#include "MK60_spi.h"


SPI_Type* SPIN[3] = SPI_BASE_PTRS; //定义三个指针数组保存 SPIx 的地址



#define SPI_TX_WAIT(SPIn)	while( ( SPIN[SPIn]->SR & SPI_SR_TXRXS_MASK ) == 1 ) //等待发送 完成
#define SPI_RX_WAIT(SPIn)   while( ( SPIN[SPIn]->SR & SPI_SR_RFDF_MASK ) == 0 )  //等待发送 FIFO为非空
#define SPI_EOQF_WAIT(SPIn) while( ( SPIN[SPIn]->SR & SPI_SR_EOQF_MASK ) == 0 )   //等待传输完成



//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI初始化
//  @param      spin            选择SPI模块   有spi0 spi1 spi2
//  @param      pcs             选择片选引脚
//  @param      master          选择主从模式
//  @param      baud            选择通信速率
//  @return     				返回真实波特率
//  @since      v1.0
//  Sample usage:       		uint32 baud = spi_init(spi0,SPI_PCS0, MASTER,10*1000*1000);              //初始化SPI,选择CS0,主机模式, 波特率为1M ,返回真实波特率到baud变量
//-------------------------------------------------------------------------------------------------------------------
uint32 spi_init(SPIn_e spin, SPI_PCSn_e pcs, SPI_CFG master,uint32 baud)
{
    uint8  br,pbr;
    uint32 clk = 0;
    uint32 Scaler[] = {2,4,6,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
    uint8  Prescaler[] = {2,3,5,7};
    uint32 fit_clk,fit_br=0,fit_pbr,min_diff =~0,diff;
    uint32 tmp;

    //计算CS到第一个SCK的时间
    uint8 pcssck,cssck,fit_pcssck,fit_cssck;

    //使能SPI模块时钟，配置SPI引脚功能
    if(spin == spi0)
    {
        SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;

        //进行管脚复用
        port_init(SPI0_SCK_PIN , ALT2  );
        port_init(SPI0_SOUT_PIN, ALT2  );
        port_init(SPI0_SIN_PIN , ALT2  );

        if(pcs & SPI_PCS0)
            port_init(SPI0_PCS0_PIN, ALT2  );

        if(pcs & SPI_PCS1)
            port_init(SPI0_PCS1_PIN, ALT2  );

        if(pcs & SPI_PCS2)
            port_init(SPI0_PCS2_PIN, ALT2  );

        if(pcs & SPI_PCS3)
            port_init(SPI0_PCS3_PIN, ALT2  );

        if(pcs & SPI_PCS4)
            port_init(SPI0_PCS4_PIN, ALT2  );

        if(pcs & SPI_PCS5)
            port_init(SPI0_PCS5_PIN, ALT3  );
    }
    else if(spin == spi1)
    {
        SIM->SCGC6 |= SIM_SCGC6_SPI1_MASK;

		
        port_init(SPI1_SCK_PIN , ALT2  );
        port_init(SPI1_SOUT_PIN, ALT2  );
        port_init(SPI1_SIN_PIN , ALT2  );

        if(pcs & SPI_PCS0)
            port_init(SPI1_PCS0_PIN, ALT2  );

        if(pcs & SPI_PCS1)
            port_init(SPI1_PCS1_PIN, ALT2  );

        if(pcs & SPI_PCS2)
            port_init(SPI1_PCS2_PIN, ALT2  );

        if(pcs & SPI_PCS3)
            port_init(SPI1_PCS3_PIN, ALT2  );
    }
    else if(spin == spi2)
    {
        SIM->SCGC3 |= SIM_SCGC3_SPI2_MASK;

        port_init(SPI2_SCK_PIN , ALT2  );
        port_init(SPI2_SOUT_PIN, ALT2  );
        port_init(SPI2_SIN_PIN , ALT2  );

        if(pcs & SPI_PCS0)
            port_init(SPI2_PCS0_PIN, ALT2  );

        if(pcs & SPI_PCS1)
            port_init(SPI2_PCS1_PIN, ALT2  );
    }
    else	return 0;//传递进来的 spi 模块有误，直接跳出函数

	
    SPIN[spin]->MCR = ( 0
                         | SPI_MCR_CLR_TXF_MASK     //清空 Tx FIFO 计数器
                         | SPI_MCR_CLR_RXF_MASK     //清空 Rx FIFO 计数器
                         | SPI_MCR_HALT_MASK        //停止SPI传输
                       );

    //-----------------------清标志位-------------------------
    SPIN[spin]->SR = (0
                       | SPI_SR_EOQF_MASK    //发送队列空了，发送完毕
                       | SPI_SR_TFUF_MASK    //传输FIFO下溢标志位，SPI为从机模式，Tx FIFO为空，而外部SPI主机模式启动传输，标志位就会置1，写1清0
                       | SPI_SR_TFFF_MASK    //传输FIFO满标志位。 写1或者DMA控制器发现传输FIFO满了就会清0。 0表示Tx FIFO满了
                       | SPI_SR_RFOF_MASK    //接收FIFO溢出标志位。
                       | SPI_SR_RFDF_MASK    //接收FIFO损耗标志位，写1或者DMA控制器发现传输FIFO空了就会清0。0表示Rx FIFO空
                      );


    //根据主从机模式设置工作模式。MCU提供最大主机频率是1/2主频，最大从机频率是1/4主频
    if(master == MASTER)
    {
        SPIN[spin]->MCR =  (0
                             |  SPI_MCR_MSTR_MASK        //Master,主机模式
                             |  SPI_MCR_PCSIS(pcs)
                             |  SPI_MCR_PCSIS_MASK
                            );

		get_clk();//获取内核时钟便于后面设置
		clk = bus_clk_mhz*1000000/baud;
        for(br = 0;br < 0x10;br++)
        {
            for(pbr = 0;pbr < 4;pbr++)
            {
                tmp = Scaler[br] * Prescaler[pbr];
                diff = abs(tmp - clk);
                if(min_diff > diff)
                {
                    //记住 最佳配置
                    min_diff = diff;
                    fit_br = br;
                    fit_pbr = pbr;

                    if(min_diff == 0)	break;//刚好匹配
                }
            }
			if(min_diff == 0)	break;//刚好匹配
        }

        fit_clk =  bus_clk_mhz *1000000 /(Scaler[fit_br] * Prescaler[fit_pbr]);

        //需要算一下 CS 到 CLK 的时间（波特率的半周期）,满足条件： (1<<(CSSCK + 1 ))*( 2*PCSSCK+1) = fclk /(2*  baud)
        clk =  bus_clk_mhz*1000000/fit_clk/2;
        min_diff = ~0;
        fit_cssck = 0x0F;
        fit_pcssck = 3;
        for(cssck = 0;cssck<=0x0F;cssck++)
        {
            tmp = 1<<(cssck+1) ;
            pcssck = (clk/tmp-1)/2;
            if(pcssck>3)continue;       //不能超过 3
            tmp = tmp * (2*pcssck+1);
            diff = abs(tmp-clk);
            if(min_diff > diff)
            {
                //记住 最佳配置
                min_diff = diff;
                fit_cssck = cssck;
                fit_pcssck = pcssck;

                if(min_diff == 0)	break;//刚好匹配
            }
        }
		
        SPIN[spin]->CTAR[0] = (0
                                //| SPI_CTAR_DBR_MASK    //双波特率 ，假设 DBR=1，CPHA=1，PBR=00，得SCK Duty Cycle 为 50/50
                                //| SPI_CTAR_CPHA_MASK   //数据在SCK上升沿改变（输出），在下降沿被捕捉（输入读取）。如果是0，则反之。  w25x16在上升沿读取数据；NRF24L01在上升沿读取数据
                                | SPI_CTAR_PBR(fit_pbr)        //波特率分频器 ，0~3 对应的分频值Prescaler为 2、3、5、7

                                | SPI_CTAR_PDT(0x00)     //延时因子为 PDT*2+1 ，这里PDT为3，即延时因子为7。PDT为2bit
                                | SPI_CTAR_BR(fit_br)         //波特率计数器值 ,当BR<=3,分频Scaler 为 2*（BR+1） ，当BR>=3，分频Scaler 为 2^BR  。BR为4bit
                                //SCK 波特率 = (Bus clk/Prescaler) x [(1+DBR)/Scaler ]          fSYS 为 Bus clock
                                //              50M / 2         x [ 1  /  2  ] = 25M   这里以最大的来算

                                //| SPI_CTAR_CPOL_MASK   //时钟极性，1表示 SCK 不活跃状态为高电平,   NRF24L01 不活跃为低电平
                                | SPI_CTAR_FMSZ(0x07)    //每帧传输 7bit+1 ，即8bit （FMSZ默认就是8）
                                // | SPI_CTAR_LSBFE_MASK //1为低位在前。

                                // 下面两个参数是调整 CS 信号来了到第一个CLK的时间
                                | SPI_CTAR_CSSCK(fit_cssck)    // x ：0~0x0F
                                | SPI_CTAR_PCSSCK(fit_pcssck)    //设置片选信号有效到时钟第一个边沿出现的延时的预分频值。tcsc延时预分频 2*x+1； x 0~3
                               );
    }
    else
    {
        //默认从机模式
        SPIN[spin]->CTAR_SLAVE[0] = (0
                                      | SPI_CTAR_SLAVE_FMSZ(0x07)
                                      | SPI_CTAR_SLAVE_CPOL_MASK
                                      | SPI_CTAR_SLAVE_CPHA_MASK
                                     );
    }


    SPIN[spin]->MCR &= ~SPI_MCR_HALT_MASK;     //启动SPI传输。1为暂停，0为启动

    return fit_clk;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI发送接收函数
//  @param      spin            选择SPI模块   有spi0 spi1 spi2
//  @param      pcs             选择片选引脚
//  @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
//  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      len             发送的字节数
//  @return     void				
//  @since      v1.0
//  Sample usage:       		spi_mosi(spi0,SPI_PCS0,buf,buf,1);    //发送buff的内容，并接收到buf里，长度为1字节
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi(SPIn_e spin, SPI_PCSn_e pcs, uint8 *modata, uint8 *midata, uint32 len)
{
    uint32 i = 0;
    do
    {
        
        //----------------清标志位----------------/
        SPIN[spin]->SR = (0
                           | SPI_SR_EOQF_MASK    //发送队列空了，发送完毕标志
                           | SPI_SR_TFUF_MASK    //传输FIFO下溢标志位，SPI为从机模式，Tx FIFO为空，而外部SPI主机模式启动传输，标志位就会置1，写1清0
                           | SPI_SR_TFFF_MASK    //传输FIFO满标志位。 写1或者DMA控制器发现传输FIFO满了就会清0。 0表示Tx FIFO满了
                           | SPI_SR_RFOF_MASK    //接收FIFO溢出标志位。
                           | SPI_SR_RFDF_MASK    //接收FIFO损耗标志位，写1或者DMA控制器发现传输FIFO空了就会清0。0表示Rx FIFO空
                          );


        //---------------- 清FIFO计数器 ----------------/
        SPIN[spin]->MCR    |=  (0
                                 | SPI_MCR_CLR_TXF_MASK  //写1清 Tx FIFO 计数器
                                 | SPI_MCR_CLR_RXF_MASK  //写1清 Rx FIFO 计数器
                                );

    }
    while( (SPIN[spin]->SR & SPI_SR_RFDF_MASK) );        //如果 Rx FIFO 非空，则清FIFO.

    //----------------发送len-1个数据----------------/                                                ;
    for(i = 0; i < (len - 1); i++)
    {
        SPIN[spin]->PUSHR = (0
                              | SPI_PUSHR_CTAS(0)             	//选择CTAR0寄存器
                              | SPI_PUSHR_CONT_MASK           	//1为 传输期间保持PCSn信号 ，即继续传输数据
                              | SPI_PUSHR_PCS(pcs)
                              | SPI_PUSHR_TXDATA(modata[i])     //要传输的数据
                             );

        while(!(SPIN[spin]->SR & SPI_SR_RFDF_MASK));        	//RFDF为1，Rx FIFO is not empty.
        if(midata != NULL)	midata[i] = (uint8)SPIN[spin]->POPR;//保存接收到的数据
        else				SPIN[spin]->POPR;

        SPIN[spin]->SR |= SPI_SR_RFDF_MASK;
    }

    //----------------发送最后一个数据----------------/
    SPIN[spin]->PUSHR = (0
                          | SPI_PUSHR_CTAS(0)                 	//选择CTAR0寄存器
                          | SPI_PUSHR_PCS(pcs)
                          | SPI_PUSHR_EOQ_MASK                	//1为 传输SPI最后的数据
                          | SPI_PUSHR_TXDATA(modata[i])
                         );

    SPI_EOQF_WAIT(spin);                                        //等待发送完成。(要及时把RX FIFO的东西清掉，不然这里就无限等待)

    while( !(SPIN[spin]->SR & SPI_SR_RFDF_MASK));           	//RFDF为1，Rx FIFO is not empty.
    if(midata != NULL)	midata[i] = (uint8)SPIN[spin]->POPR;    //保存接收到的数据
    else				SPIN[spin]->POPR;

    SPIN[spin]->SR |= SPI_SR_RFDF_MASK;                     	//写1清空RFDF，标记Rx FIFO 是空的
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI发送接收函数
//  @param      spin            选择SPI模块   有spi0 spi1 spi2
//  @param      pcs             选择片选引脚
//  @param      mocmd           发送的命令缓冲区地址(不需要接收则传 NULL)
//  @param      micmd           发送命令时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
//  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      cmdlen          发送命令的字节数
//  @param      len             发送数据的字节数
//  @return     void				
//  @since      v1.0
//  Sample usage:       		spi_mosi_cmd(spi0,SPI_PCS0,cmd,NULL,buff,buff,1,2);    //发送cmd/buff的内容，不接收cmd发送时的数据，接收buff发送时的数据到buff里，长度分别为1、2字节
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi_cmd(SPIn_e spin, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len)
{
    uint32 i = 0;
    do
    {
        //----------------清标志位----------------/
        SPIN[spin]->SR = (0
                           | SPI_SR_EOQF_MASK    //发送队列空了，发送完毕标志
                           | SPI_SR_TFUF_MASK    //传输FIFO下溢标志位，SPI为从机模式，Tx FIFO为空，而外部SPI主机模式启动传输，标志位就会置1，写1清0
                           | SPI_SR_TFFF_MASK    //传输FIFO满标志位。 写1或者DMA控制器发现传输FIFO满了就会清0。 0表示Tx FIFO满了
                           | SPI_SR_RFOF_MASK    //接收FIFO溢出标志位。
                           | SPI_SR_RFDF_MASK    //接收FIFO损耗标志位，写1或者DMA控制器发现传输FIFO空了就会清0。0表示Rx FIFO空
                          );

        //----------------清FIFO计数器----------------/
        SPIN[spin]->MCR |=  (0
                              | SPI_MCR_CLR_TXF_MASK      //写1清 Tx FIFO 计数器
                              | SPI_MCR_CLR_RXF_MASK      //写1清 Rx FIFO 计数器
                             );
    }
    while( (SPIN[spin]->SR & SPI_SR_RFDF_MASK));            	//如果 Rx FIFO 非空，则清FIFO.

    //----------------发送cmdlen个数据----------------/
    for(i = 0; i < cmdlen; i++)
    {
        SPIN[spin]->PUSHR = (0
                              | SPI_PUSHR_CTAS(0)             	//选择CTAR0寄存器
                              | SPI_PUSHR_CONT_MASK           	//1为 传输期间保持PCSn信号 ，即继续传输数据
                              | SPI_PUSHR_PCS(pcs)
                              | SPI_PUSHR_TXDATA(mocmd[i])    	//要传输的数据
                             );

        while(!(SPIN[spin]->SR & SPI_SR_RFDF_MASK));        	//RFDF为1，Rx FIFO is not empty.
        if(micmd != NULL)	micmd[i] = (uint8)SPIN[spin]->POPR; //保存接收到的数据
        else				SPIN[spin]->POPR;                   //读取FIFO数据(丢弃读取到的数据)

        SPIN[spin]->SR |= SPI_SR_RFDF_MASK;
    }

    //----------------发送len-1个数据----------------/
    for(i = 0; i < (len - 1); i++)
    {
        SPIN[spin]->PUSHR = (0
                              | SPI_PUSHR_CTAS(0)             	//选择CTAR0寄存器
                              | SPI_PUSHR_CONT_MASK           	//1为 传输期间保持PCSn信号 ，即继续传输数据
                              | SPI_PUSHR_PCS(pcs)
                              | SPI_PUSHR_TXDATA(modata[i])     //要传输的数据
                             );

        while(!(SPIN[spin]->SR & SPI_SR_RFDF_MASK));        	//RFDF为1，Rx FIFO is not empty.

        if(midata != NULL)	midata[i] = (uint8)SPIN[spin]->POPR;//保存接收到的数据
        else				SPIN[spin]->POPR;                   //读取FIFO数据(丢弃读取到的数据)

        SPIN[spin]->SR |= SPI_SR_RFDF_MASK;
    }
    //----------------发送最后一个数据----------------/
    SPIN[spin]->PUSHR = (0
                          | SPI_PUSHR_CTAS(0)          			//选择CTAR0寄存器
                          | SPI_PUSHR_PCS(pcs)
                          | SPI_PUSHR_EOQ_MASK         			//End Of Queue，1为 传输SPI最后的数据
                          | SPI_PUSHR_TXDATA(modata[i])
                         );

    SPI_EOQF_WAIT(spin);    									//要及时把RX FIFO的东西清掉，不然这里就无限等待

    while( !(SPIN[spin]->SR & SPI_SR_RFDF_MASK));    			//RFDF为1，Rx FIFO is not empty.
    if(midata != NULL)	midata[i] = (uint8)SPIN[spin]->POPR;    //保存接收到的数据
    else				SPIN[spin]->POPR;                       //读取FIFO数据(丢弃读取到的数据)

    SPIN[spin]->SR |= SPI_SR_RFDF_MASK;
}

