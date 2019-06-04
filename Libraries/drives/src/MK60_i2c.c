/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_i2c
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 

#include "MK60_i2c.h"

unsigned char MasterTransmission;
unsigned char SlaveID;

I2C_Type * I2CN[4] = I2C_BASE_PTRS; //定义两个指针数组保存 I2CN 的地址



//启动信号
#define i2c_Start(NUM)             {I2CN[NUM]->C1 |= (I2C_C1_TX_MASK | I2C_C1_MST_MASK);}    //MST 由0变1，产生起始信号，TX = 1 进入发送模式

//停止信号
#define i2c_Stop(NUM)              {I2CN[NUM]->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK);}   //MST 由1变0，产生停止信号，TX = 0 进入接收模式

//重复启动
#define i2c_RepeatedStart(NUM)     {I2CN[NUM]->C1 |= I2C_C1_RSTA_MASK;}

//进入接收模式(应答,需要接收多个数据，接收最后一个字节前需要禁用应答i2c_DisableAck)
#define i2c_EnterRxMode(NUM)       {I2CN[NUM]->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);}  //

//进入接收模式(不应答,只接收一个字节)
#define i2c_PutinRxMode(NUM)       {I2CN[NUM]->C1 &= ~I2C_C1_TX_MASK; I2CN[NUM]->C1 |= I2C_C1_TXAK_MASK;}

//禁用应答(接收最后一个字节)
#define i2c_DisableAck(NUM)        {I2CN[NUM]->C1 |= I2C_C1_TXAK_MASK;}

//等待 I2C_S
#define i2c_Wait(NUM)              {while(( I2CN[NUM]->S & I2C_S_IICIF_MASK)==0); I2CN[NUM]->S |= I2C_S_IICIF_MASK;}
									

//写一个字节
#define i2c_write_byte(NUM,data)   {I2CN[NUM]->D = data; i2c_Wait(NUM);}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C初始化
//  @param      i2cn        I2C模块(i2c0,i2c1)
//  @param      baud        期望的波特率
//  @return                 实际的波特率
//  @return     void
//  @since      v1.0
//  Sample usage:           i2c_init(i2c0,400*1000);     // 初始化i2c0，期待的波特率为400k
//-------------------------------------------------------------------------------------------------------------------
uint32 i2c_init(I2Cn_e i2cn, uint32 baud)
{
    //查表 ICR 对应的  SCL_divider ，见 《K60P144M100SF2RM.pdf》第1468页的 I2C Divider and Hold Values
    uint16 ICR_2_SCL_divider[0x40]  =
    {
        20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68,
        48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128, 144, 160, 192, 240,
        160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960,
        640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
    };

    uint8 mult;
    
    uint16 scldiv =  0;  //最佳的分频系数

    //需要从 ICR_2_SCL_divider 里找到 与最佳分频系数scldiv最相近的 分频系数
    uint8 icr, n = 0x40;
    uint16 min_Dvalue = ~0, Dvalue;
    
    if(i2cn == i2c0)
    {
        // 开启时钟 
        SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;           //开启 I2C0时钟



        // 配置 i2c0功能的 GPIO 接口 
        if((I2C0_SCL_PIN == B0) || (I2C0_SCL_PIN == B2) || (I2C0_SCL_PIN == D8) )
        {
            port_init (I2C0_SCL_PIN, ALT2 | ODO | PULLUP );
        }
        else
            return 0;//错误直接返回

        if((I2C0_SDA_PIN == B1) || (I2C0_SDA_PIN == B3) || (I2C0_SDA_PIN == D9) )
            port_init (I2C0_SDA_PIN, ALT2 | ODO | PULLUP );
        else
            return 0;//错误直接返回
    }
    else
    {
        // 开启时钟 
        SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;           //开启 I2C1时钟



        // 配置 i2c1功能的 GPIO 接口 
        if(I2C1_SCL_PIN == E1)
            port_init (I2C1_SCL_PIN, ALT6 | ODO | PULLUP );
        else if(I2C1_SCL_PIN == C10)
            port_init (I2C1_SCL_PIN, ALT2 | ODO | PULLUP );
        else
            return 0;//错误直接返回

        if(I2C1_SDA_PIN == E0)
            port_init (I2C1_SDA_PIN, ALT6 | ODO | PULLUP );
        else if (I2C1_SDA_PIN == C11)
            port_init (I2C1_SDA_PIN, ALT2 | ODO | PULLUP );
        else
            return 0;//错误直接返回
    }

    // 设置频率

    // I2C baud rate = bus speed (Hz)/(mul × SCL divider)  即这里 50MHz/(1 ×128)=390.625kHz
    // SDA hold time = bus period (s) × mul × SDA hold value
    // SCL start hold time = bus period (s) × mul × SCL start hold value
    // SCL stop hold time = bus period (s) × mul × SCL stop hold value

    
    if(bus_clk_mhz*1000 <= 50000)mult = 0;         //bus 一分频
    else  if(bus_clk_mhz*1000 <= 100000)mult = 1;  //bus 二分频
    else      mult = 2;                       //bus 四分频

    scldiv =  bus_clk_mhz * 1000000 / ( (1<<mult) * baud );  //最佳的分频系数

    while(n)                                            //循环里逐个扫描，找出最接近的 分频系数
    {
        n--;
        Dvalue = abs(scldiv - ICR_2_SCL_divider[n]);
        if(Dvalue == 0)
        {
            icr = n;
            break;                                      //退出while循环
        }

        if(Dvalue < min_Dvalue)
        {
            icr = n;
            min_Dvalue = Dvalue;
        }
    }

    I2CN[i2cn]->F  = ( 0                        // I2C Frequency Divider register (I2Cx_F)  I2C分频寄存器   I2C最大波特率为 400k
                       | I2C_F_MULT(mult)        // 乘数因子 mul =  1<<MULT
                       | I2C_F_ICR(icr)          // 时钟速率 = ICR_2_SCL_divider[ICR] ，查表获得 ICR 与 SCL_divider 映射关系
                     );

    // 使能 I2C 
    I2CN[i2cn]->C1 = ( 0
                       | I2C_C1_IICEN_MASK       //使能I2C
                       //| I2C_C1_IICIE_MASK       //使能中断
                     );

    return (  bus_clk_mhz * 1000000 / ( (1<<mult) * ICR_2_SCL_divider[icr])  );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      I2C通信结束后需要调用的函数函数
//  @return     void
//  @since      v1.0
//  @note		如果通信失败，可尝试增大此延时值，确认是否延时导致的
//-------------------------------------------------------------------------------------------------------------------
void i2c_delay(void)
{
    volatile uint16 n = 400;     //注意，这个数据太小，会导致读取错误。

    while(n--);
}    

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取I2C设备指定地址寄存器的数据
//  @param      i2cn        I2C模块(i2c0、i2c1)
//  @param      SlaveID     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @return                 读取的寄存器值
//  @since      v1.0
//  Sample usage:       	uint8 value = i2c_read_reg(i2c0, 0x2D, 0x50);//读取0x50地址的数据，从机地址为0x2D
//-------------------------------------------------------------------------------------------------------------------
uint8 i2c_read_reg(I2Cn_e i2cn, uint8 SlaveID, uint8 reg)
{

    //先写入寄存器地址,再读取数据,因此此过程是 I2C 的复合格式,改变数据方向时需要重新启动
    //地址是低七位
    uint8 result;

    i2c_Start(i2cn);                                    //发送启动信号

    i2c_write_byte(i2cn, (SlaveID << 1) | MWSR);       //发送从机地址和写位

    i2c_write_byte(i2cn, reg);                          //发送从机里的寄存器地址

    i2c_RepeatedStart(i2cn);                            //复合格式，发送重新启动信号

    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );      //发送从机地址和读位

    i2c_PutinRxMode(i2cn);                              //进入接收模式(不应答,只接收一个字节)
    result = I2CN[i2cn]->D;                             //虚假读取一次，启动接收数据
    i2c_Wait(i2cn);                                     //等待接收完成

    i2c_Stop(i2cn);                                     //发送停止信号

    result = I2CN[i2cn]->D;                     //读取数据

    i2c_delay();                                        //必须延时一下，否则出错

    return result;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取I2C设备指定地址寄存器的数据
//  @param      i2cn        I2C模块(i2c0、i2c1)
//  @param      SlaveID     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @param      num         读取字节数
//  @param      addr        读取的数据存储的地址
//  @return     void
//  @since      v1.0
//  Sample usage:       	uint8 value = i2c_read_reg(i2c0, 0x2D, 0x50, 10, buf);//读取0x50地址的数据，从机地址为0x2D开始的10个字节
//-------------------------------------------------------------------------------------------------------------------

uint8 i2c_read_reg_bytes(I2Cn_e i2cn, uint8 SlaveID, uint8 reg, uint8 num, uint8 * addr)
{
    //先写入寄存器地址,再读取数据,因此此过程是 I2C 的复合格式,改变数据方向时需要重新启动
    //地址是低七位
    uint8 result;
    
    i2c_Start(i2cn);                                    //发送启动信号

    i2c_write_byte(i2cn, (SlaveID << 1) | MWSR);       //发送从机地址和写位

    i2c_write_byte(i2cn, reg);                          //发送从机里的寄存器地址

    i2c_RepeatedStart(i2cn);                            //复合格式，发送重新启动信号

    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );      //发送从机地址和读位

    i2c_EnterRxMode(i2cn);                              //进入接收模式(不应答,只接收一个字节)
    
    while(num--)
    {
        result = I2CN[i2cn]->D;                         //虚假读取一次，启动接收数据
        i2c_Wait(i2cn);                                 //等待接收完成
        *addr = I2CN[i2cn]->D;                         //读取数据
        i2c_delay();                                    //必须延时一下，否则出错
        addr++;
    }
    i2c_DisableAck(i2cn);
    i2c_PutinRxMode(i2cn);
    result = I2CN[i2cn]->D;                             //虚假读取一次，启动接收数据
    i2c_Wait(i2cn);                                     //等待接收完成
    i2c_Stop(i2cn);                                     //发送停止信号
    *addr = I2CN[i2cn]->D;                              //读取数据

    i2c_delay();                                        //必须延时一下，否则出错
    
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      写入一个字节数据到I2C设备指定寄存器地址
//  @param      i2cn        I2C模块(i2c0、i2c1)
//  @param      SlaveID     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @param      Data        数据
//  @return     void
//  @since      v1.0
//  Sample usage:       	i2c_write_reg(i2c0, 0x2D, 0x50,2);     //写入数据2到0x50地址，从机地址为0x2D
//-------------------------------------------------------------------------------------------------------------------
void i2c_write_reg(I2Cn_e i2cn, uint8 SlaveID, uint8 reg, uint8 Data)
{

    i2c_Start(i2cn);                                    //发送启动信号

    i2c_write_byte(i2cn, ( SlaveID << 1 ) | MWSR);      //发送从机地址和写位

    i2c_write_byte(i2cn, reg);                          //发送从机里的寄存器地址

    i2c_write_byte(i2cn, Data);                         //发送需要写入的数据

    i2c_Stop(i2cn);

    i2c_delay();                                        //延时太短的话，可能写出错
}

