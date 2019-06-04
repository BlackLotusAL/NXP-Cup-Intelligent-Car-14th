/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_7725.c
 * @brief      		小钻风(二值化摄像头)函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 * @note
					OV7725接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SDA                 C17
						SCL                 C16
						场中断              C6
						像素中断            C18           
						数据口              C8-C15 
					------------------------------------ 
	
					默认分辨率是            160*120
					默认FPS                 50帧
 ********************************************************************************************************************/



#include "SEEKFREE_7725.h"



uint8 image_bin[OV7725_SIZE];                                   //定义存储接收图像的数组
uint8 image_dec[OV7725_H][OV7725_W];

uint8 OV7725_IDCode = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头内部寄存器初始化(内部使用，用户无需调用)
//  @param      NULL
//  @return     uint8			返回0则出错，返回1则成功
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 ov7725_reg_init(void)
{
    
    simiic_write_reg ( OV7725_DEV_ADD, OV7725_COM7, 0x80 );	//复位摄像头
    systick_delay_ms(50);
	OV7725_IDCode = simiic_read_reg( OV7725_DEV_ADD, OV7725_VER ,SCCB);
    if( OV7725_IDCode != OV7725_ID )    return 0;			//校验摄像头ID号


    if(OV7725_IDCode == OV7725_ID)
    {
        //ID号确认无误   然后配置寄存器
        simiic_write_reg(OV7725_DEV_ADD, OV7725_COM4         , 0xC1);  
        simiic_write_reg(OV7725_DEV_ADD, OV7725_CLKRC        , 0x01);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_COM2         , 0x03);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_COM3         , 0xD0);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_COM7         , 0x40);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_HSTART       , 0x3F);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_HSIZE        , 0x50);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_VSTRT        , 0x03);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_VSIZE        , 0x78);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_HREF         , 0x00);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_SCAL0        , 0x0A);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_AWB_Ctrl0    , 0xE0);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_DSPAuto      , 0xff);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl2    , 0x0C);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl3    , 0x00);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl4    , 0x00);
      
      
        if(OV7725_W == 80)              simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize    , 0x14);
        else if(OV7725_W == 160)        simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize    , 0x28);
        else if(OV7725_W == 240)        simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize    , 0x3c);
        else if(OV7725_W == 320)        simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize    , 0x50);
		
        if(OV7725_H == 60)              simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize    , 0x1E);
        else if(OV7725_H == 120)        simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize    , 0x3c);
        else if(OV7725_H == 180)        simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize    , 0x5a);
        else if(OV7725_H == 240)        simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize    , 0x78);
      
      
        simiic_write_reg(OV7725_DEV_ADD, OV7725_REG28        , 0x01);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_EXHCH        , 0x10);
		simiic_write_reg(OV7725_DEV_ADD, OV7725_EXHCL        , 0x1F);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM1         , 0x0c);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM2         , 0x16);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM3         , 0x2a);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM4         , 0x4e);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM5         , 0x61);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM6         , 0x6f);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM7         , 0x7b);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM8         , 0x86);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM9         , 0x8e);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM10        , 0x97);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM11        , 0xa4);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM12        , 0xaf);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM13        , 0xc5);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM14        , 0xd7);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM15        , 0xe8);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_SLOP         , 0x20);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_RADI      , 0x00);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEF      , 0x13);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_XC        , 0x08);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEFB     , 0x14);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEFR     , 0x17);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_CTR       , 0x05);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_BDBase       , 0x99);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_BDMStep      , 0x03);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_SDE          , 0x04);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_BRIGHT       , 0x00);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_CNST         , 0x40);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_SIGN         , 0x06);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_UVADJ0       , 0x11);
        simiic_write_reg(OV7725_DEV_ADD, OV7725_UVADJ1       , 0x02);
        return 1;
    }
    else        return 0;//错误
    
}





//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头采集程序初始化(内部使用，用户无需调用)
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ov7725_port_init(void)
{
    //DMA通道0初始化，OV7725_PCLK触发源(默认上升沿)，源地址为OV7725_DATAPORT，目的地址为：image_bin，每次传输1Byte
    dma_portx2buff_init(OV7725_DMA_CH, (void *)&OV7725_DATAPORT, (void *)image_bin, OV7725_PCLK, DMA_BYTE1, OV7725_DMA_NUM, DADDR_KEEPON);

    DMA_DIS(OV7725_DMA_CH);
    disable_irq(INTERRUPT_NUNBERS);                             //关闭中断
    DMA_IRQ_CLEAN(OV7725_DMA_CH);                               //清除通道传输中断标志位
    DMA_IRQ_EN(OV7725_DMA_CH);

    port_init(OV7725_PCLK, ALT1 | DMA_FALLING | PULLDOWN );     //PCLK
    port_init(OV7725_VSYNC, ALT1 | IRQ_RISING  | PULLDOWN | PF);//场中断，上拉，上降沿触发中断，带滤波

    enable_irq(INTERRUPT_NUNBERS);
	EnableInterrupts;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头初始化(调用之后设置好相关中断函数即可采集图像)
//  @param      NULL
//  @return     0
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 ov7725_init(void)
{
	IIC_init();
	ov7725_reg_init();                                          //摄像头寄存器配置
    ov7725_port_init();                                         //摄像头中断引脚及DMA配置
    return 0;
}





//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头场中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:					在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void ov7725_vsync(void) 
{
    uint8  n;                                                   //引脚号
	uint32 flag;
	
	flag = INTERRUPT_ISFR;
    INTERRUPT_ISFR  = 0xffffffff;                               //清中断标志位
            
    n = VSYNC_CHANNEL;                                          //场中断
    if(flag & (1 << n))                                         //场触发中断
    {
		INTERRUPT_ISFR = 1 <<  PCLK_CHANNEL;                    //清空PCLK标志位
        DMA0->TCD[OV7725_DMA_CH].DADDR = (uint32)image_bin;     //恢复DMA地址
        DMA_EN(OV7725_DMA_CH);                                  //使能DMA通道CHn 硬件请求
    }

}

uint8 ov7725_finish_flag = 0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头DMA完成中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:					在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void ov7725_dma(void)
{
    DMA_IRQ_CLEAN(OV7725_DMA_CH);                               //清除通道传输中断标志位
	ov7725_finish_flag = 1;										//采集完成标志位
}       




//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头数据解压函数
//  @param      *data1				源地址
//  @param      *data2				目的地址
//  @return     void
//  @since      v1.0
//  Sample usage:					Image_Decompression(da1,dat2[0]);//将一维数组dat1的内容解压到二维数组dat2里.
//-------------------------------------------------------------------------------------------------------------------
void Image_Decompression(uint8 *data1,uint8 *data2)
{
    uint8  temp[2] = {0,255};
    uint16 lenth = OV7725_SIZE;
    uint8  i = 8;

        
    while(lenth--)
    {
        i = 8;
        while(i--)
        {
            *data2++ = temp[(*data1 >> i) & 0x01];
        }
        data1++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头未解压图像发送至上位机查看图像
//  @param      *imgaddr			压缩图像数据地址
//  @param      *imgsize			图像大小(直接填写OV7725_SIZE)
//  @return     void
//  @since      v1.0
//  Sample usage:					调用该函数前请先初始化uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_7725(void *imgaddr, uint32_t imgsize)
{
    uart_putchar(uart2, 0x00);uart_putchar(uart2, 0xff);uart_putchar(uart2, 0x01);uart_putchar(uart2, 0x01);//发送四个字节命令
    uart_putbuff(uart2, (uint8_t *)imgaddr, imgsize);   //再发送图像
}




