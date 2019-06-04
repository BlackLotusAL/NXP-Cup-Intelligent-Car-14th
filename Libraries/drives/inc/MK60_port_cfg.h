/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_port_cfg
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_port_cfg_h
#define _MK60_port_cfg_h

#include "common.h"

//-----------------------------------  UART  -----------------------------------

//      模块通道        端口          可选范围                建议
#define UART0_RX_PIN    D6        //A1、A15、B16、D6          A1不要用（与Jtag冲突）
#define UART0_TX_PIN    D7        //A2、A14、B17、D7          A2不要用（与Jtag冲突）

#define UART1_RX_PIN    E1        //C3、E1
#define UART1_TX_PIN    E0        //C4、E0

#define UART2_RX_PIN    D2        //D2
#define UART2_TX_PIN    D3        //D3

#define UART3_RX_PIN    E5        //B10、C16、E5
#define UART3_TX_PIN    E4        //B11、C17、E4

#define UART4_RX_PIN    E25       //C14、E25
#define UART4_TX_PIN    E24       //C15、E24

//-----------------------------------  FTM  -----------------------------------

//      模块通道        端口      可选范围                建议
#define FTM0_CH0_PIN    C1        //C1、A3                A3不要用（与Jtag、SWD冲突）
#define FTM0_CH1_PIN    C2        //C2、A4    
#define FTM0_CH2_PIN    C3        //C3、A5    
#define FTM0_CH3_PIN    C4        //C4、A6    
#define FTM0_CH4_PIN    D4        //D4、A7    
#define FTM0_CH5_PIN    D5        //D5、A0                A0不要用（与Jtag、SWD冲突）
#define FTM0_CH6_PIN    D6        //D6、A1                A1不要用（与Jtag冲突）
#define FTM0_CH7_PIN    D7        //D7、A2                A2不要用（与Jtag冲突）


//      模块通道        端口      可选范围                建议
#define FTM1_CH0_PIN    A12       //A8、A12、B0
#define FTM1_CH1_PIN    A13       //A9、A13、B1

//      模块通道        端口      可选范围                建议
#define FTM2_CH0_PIN    B18       //A10、B18
#define FTM2_CH1_PIN    B19       //A11、B19

//      模块通道        端口      可选范围                建议
#define FTM3_CH0_PIN    E5        //D0 
#define FTM3_CH1_PIN    E6        //D1 
#define FTM3_CH2_PIN    E7        //D2 
#define FTM3_CH3_PIN    E8        //D3 
#define FTM3_CH4_PIN    E9        //C8 
#define FTM3_CH5_PIN    E10       //C9 
#define FTM3_CH6_PIN    C10       //C10
#define FTM3_CH7_PIN    E12       //C11          

//正交解码模块通道      端口      可选范围                建议
#define FTM1_QDPHA_PIN  A12       //A8、A12、B0
#define FTM1_QDPHB_PIN  A13       //A9、A13、B1

#define FTM2_QDPHA_PIN  B18       //A10、B18
#define FTM2_QDPHB_PIN  B19       //A11、B19


//-----------------------------------  I2C  -----------------------------------

//      模块通道        端口      可选范围                建议
#define I2C0_SCL_PIN    B2        // B0、B2、D8
#define I2C0_SDA_PIN    B3        // B1、B3、D9

#define I2C1_SCL_PIN    C10       // E1、C10
#define I2C1_SDA_PIN    C11       // E0、C11


//-----------------------------------  SPI  -----------------------------------
//如果不使用片选接口的话且需要片选接口用于做其他的时候需要注释掉，就不会进行初始化对应的管脚
//      模块通道        端口      可选范围              建议

#define SPI0_SCK_PIN    A15       // A15、C5、D1        全部都是 ALT2
#define SPI0_SOUT_PIN   A16       // A16、C6、D2        全部都是 ALT2
#define SPI0_SIN_PIN    A17       // A17、C7、D3        全部都是 ALT2

#define SPI0_PCS0_PIN   A14       // A14、C4、D0、      全部都是 ALT2
#define SPI0_PCS1_PIN   C3        // C3、D4             全部都是 ALT2
#define SPI0_PCS2_PIN   C2        // C2、D5             全部都是 ALT2
#define SPI0_PCS3_PIN   C1        // C1、D6             全部都是 ALT2
#define SPI0_PCS4_PIN   C0        // C0、               全部都是 ALT2
#define SPI0_PCS5_PIN   B23       // B23                ALT3


#define SPI1_SCK_PIN    B11       // E2、B11、          全部都是 ALT2
#define SPI1_SOUT_PIN   B16       // E1、B16、          全部都是 ALT2
#define SPI1_SIN_PIN    B17       // E3、B17、          全部都是 ALT2

#define SPI1_PCS0_PIN   B10       // E4、B10、          全部都是 ALT2
#define SPI1_PCS1_PIN   E0        // E0、B9、           全部都是 ALT2
#define SPI1_PCS2_PIN   E5        // E5、               全部都是 ALT2
#define SPI1_PCS3_PIN   E6        // E6、               全部都是 ALT2


#define SPI2_SCK_PIN    B21       // B21、D12           全部都是 ALT2
#define SPI2_SOUT_PIN   B22       // B22、D13           全部都是 ALT2
#define SPI2_SIN_PIN    B23       // B23、D14           全部都是 ALT2
#define SPI2_PCS0_PIN   B20       // B20、D11           全部都是 ALT2
#define SPI2_PCS1_PIN   D15       // D15                全部都是 ALT2


//-----------------------------------  CAN  -----------------------------------
#define CAN0_TX_PIN     A12       //A12、B18            全部都是 ALT2
#define CAN0_RX_PIN     A13       //A13、B19            全部都是 ALT2

#define CAN1_TX_PIN     E24       //E24、C17            全部都是 ALT2
#define CAN1_RX_PIN     E25       //E25、C16            全部都是 ALT2


#endif


