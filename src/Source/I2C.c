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
#include "I2C.h"


volatile struct I2C_MemMap *I2Cx[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //定义两个指针数组保存 I2Cx 的地址


//-------------------------------------------------------------------------*
//函数名: IIC_init                                                        
//功  能: 初始化IIC                                                       
//参  数: i2cn:端口名 I2C0,I2C1                                                                                     
//返  回: 无                                                              
//简  例: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_Init(I2Cn i2cn)
{    
    if(i2cn == I2C_0)
    {
        /* 开启时钟 */
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //开启 I2C0时钟

        /* 配置 I2C0功能的 GPIO 接口 */
        if(I2C0_SCL == PTB0)
        {  
           PORTB_PCR0 = PORT_PCR_MUX(2);
           PORTB_PCR0 = PORTB_PCR0|(1<<5);//设置为开漏模式
        }
        else if(I2C0_SCL == PTB2)
        {
          PORTB_PCR2 = PORT_PCR_MUX(2);
          PORTB_PCR2 = PORTB_PCR2|(1<<5);//设置为开漏模式
        }
        else if(I2C0_SCL == PTD8)
        {
          PORTD_PCR8 = PORT_PCR_MUX(2);
          PORTD_PCR8 = PORTD_PCR8|(1<<5);//设置为开漏模式
        }
        else
            ;
        if(I2C0_SDA == PTB1)
        {
          PORTB_PCR1 = PORT_PCR_MUX(2);
          PORTB_PCR1 = PORTB_PCR1|(1<<5);//设置为开漏模式
        }
        else if(I2C0_SDA == PTB3)
        {  
          PORTB_PCR3 = PORT_PCR_MUX(2);
          PORTE_PCR3 = PORTE_PCR3|(1<<5);//设置为开漏模式
        }
        else if(I2C0_SDA == PTD9)
        {
          PORTD_PCR9 = PORT_PCR_MUX(2);
          PORTD_PCR9 = PORTD_PCR9|(1<<5);//设置为开漏模式
        }        
    }
    else
    {
        /* 开启时钟 */
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //开启 I2C1时钟

        /* 配置 I2C1功能的 GPIO 接口 */
        if(I2C1_SCL == PTE1)
        { 
          PORTE_PCR1 = PORT_PCR_MUX(6);
          PORTE_PCR1 = PORTE_PCR1|(1<<5);//设置为开漏模式
        }
        else if(I2C1_SCL == PTC10)
        {
          PORTC_PCR10 = PORT_PCR_MUX(2);
          PORTC_PCR10 = PORTC_PCR10|(1<<5);//设置为开漏模式
        }
        else
            ;

        if(I2C1_SDA == PTE0)
        { 
          PORTE_PCR0 = PORT_PCR_MUX(6);
          PORTE_PCR0 = PORTE_PCR0 | (1<<5);//设置成开漏模式   只设置SDA即可，两个都设置也没事
        }
        else if (I2C1_SDA == PTC11)
        {
          PORTC_PCR11 = PORT_PCR_MUX(2);
          PORTC_PCR11 = PORTC_PCR11 | (1<<5);//设置为开漏模式
        } 
    }

    /* 设置频率 */
    I2C_F_REG(I2Cx[i2cn])  = I2C_F_MULT(0) | I2C_F_ICR(0) ;  //0x29
    // MULT=00  即  mul = 1
    // ICR =14  ICR为Clock rate时钟频率
    // 从《k16 reference manual.pdf》P1460 可得：
    // ICR    SCL Divider   SDA Hold Value    SCL Hold (Start) Value    SCL Hold (Stop) Value
    //  29       384              33                   190                        193
    //  17       128              21                   58                         65
    // I2C baud rate = bus speed (Hz)/(mul × SCL divider)  即这里 90MHz/(1 ×128)=234.375kHz

    /* 使能 IIC1 */
    I2C_C1_REG(I2Cx[i2cn]) = I2C_C1_IICEN_MASK;
}

//-------------------------------------------------------------------------*
//函数名: I2C_StartTransmission                                                        
//功  能: 启动IIC传输                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Mode    :传输模式
//返  回: 无                                                              
//简  例: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_StartTransmission (I2Cn i2cn, u8 SlaveID, MSmode Mode)
{
    SlaveID = ( SlaveID << 1 ) | Mode ;            //确定写地址和读地址

    /* send start signal */
    I2C_Start(i2cn);

    /* send ID with W/R bit */
    I2C_write_byte(i2cn, SlaveID);
}

//-------------------------------------------------------------------------*
//函数名: Pause                                                        
//功  能: 延时                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: Pause;              
//-------------------------------------------------------------------------*
void Pause(void)
{
    u16 n;
    for(n = 1; n < 50000; n++)      //不可太小
    {
        asm("nop");
    }
}


//-------------------------------------------------------------------------*
//函数名: I2C_ReadAddr                                                        
//功  能: 读取IIC设备指定地址寄存器的数据                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Addr    :从机的寄存器地址
//返  回: result                                                              
//简  例: p[0]  = I2C_ReadAddr(i2cn,SlaveID,OUT_X_MSB_REG);              
//-------------------------------------------------------------------------*
u8 I2C_ReadAddr(I2Cn i2cn, u8 SlaveID, u8 Addr)
{
    u8 result;

    /* Send Slave Address */
    I2C_StartTransmission (i2cn, SlaveID, write);
    I2C_Wait(i2cn);

    /* Write Register Address */
    I2C_write_byte(i2cn, Addr);
    I2C_Wait(i2cn);

    /* Do a repeated start */
    I2C_RepeatedStart(i2cn);

    /* Send Slave Address */
    I2C_write_byte(i2cn, ( SlaveID << 1) | read );
    I2C_Wait(i2cn);

    /* Put in Rx Mode */
    I2C_PutinRxMode(i2cn);

    /* Turn off ACK since this is second to last byte being read*/
    I2C_DisableAck(i2cn); //不应答

    /* Dummy read 虚假读取*/
    result = I2C_D_REG(I2Cx[i2cn]);
    I2C_Wait(i2cn);

    /* Send stop since about to read last byte */
    I2C_Stop(i2cn);

    /* Read byte */
    result = I2C_D_REG(I2Cx[i2cn]);

    return result;
}


//-------------------------------------------------------------------------*
//函数名: I2C_ReadAddr                                                        
//功  能: 读取IIC设备指定地址寄存器的数据                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Addr    :从机的寄存器地址
//        Data    :数据
//返  回: 无                                                              
//简  例: I2C_WriteAddr(I2C1, SlaveAddress2100, CTRL_REG1_2100, 0x02);             
//-------------------------------------------------------------------------*
void I2C_WriteAddr(I2Cn i2cn, u8 SlaveID, u8 Addr, u8 Data)
{
    /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, write);    //启动传输
    I2C_Wait(i2cn);

    I2C_write_byte(i2cn, Addr);                    //写地址
    I2C_Wait(i2cn);

    I2C_write_byte(i2cn, Data);                    //写数据
    I2C_Wait(i2cn);

    I2C_Stop(i2cn);

    Pause();                                        //延时太短的话，可能写出错
}

