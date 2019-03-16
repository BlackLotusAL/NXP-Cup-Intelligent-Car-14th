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


#ifndef __ADC_H__
#define __ADC_H__ 
/*
                        ADC对应引脚
----------------ADC0--------------------ADC1---------------

      ADC0_SE8-----PTB0         ADC1_SE4a-----PTE0 
      ADC0_SE9-----PTB1         ADC1_SE5a-----PTE1 
      ADC0_SE10----PTA7         ADC1_SE6a-----PTE2 
      ADC0_SE11----PTA8         ADC1_SE7a-----PTE3 
      ADC0_SE12----PTB2         ADC1_SE8------PTB0
      ADC0_SE13----PTB3         ADC1_SE9------PTB1
      ADC0_SE14----PTC0         ADC1_SE10-----PTB4
      ADC0_SE15----PTC1         ADC1_SE11-----PTB5
                                ADC1_SE12-----PTB6
      ADC0_SE16----ADC0_SE16    ADC1_SE13-----PTB7
                                ADC1_SE14-----PTB10
      ADC0_SE17----PTE24        ADC1_SE15-----PTB11
      ADC0_SE18----PTBE25       ADC1_SE16-----ADC1_SE16
                                ADC1_SE17-----PTA17

      ADC0_DM0-----ADC0_DM0     ADC1_DM0------ADC1_DM0
      ADC0_DM1-----ADC0_DM1     ADC1_DM1------ADC1_DM1

*/

typedef enum
{
    // ---------------------------------ADC0-------------------------
    ADC0_DP0  =0,
    ADC0_DP1  =1,
    PGA0_DP   =2,        //不支持ADC，需要配置 ADC PGA register 设置放大增益
    ADC0_DP3  =3,
    //软件触发不支持B通道
    ADC0_SE4b =4,      // PTC2     不支持软件ADC
    ADC0_SE5b =5,      // PTD1     不支持软件ADC
    ADC0_SE6b =6,      // PTD5     不支持软件ADC
    ADC0_SE7b =7,      // PTD6     不支持软件ADC

    ADC0_SE8  =8,       // PTB0
    ADC0_SE9  =9,       // PTB1
    ADC0_SE10 =10,      // PTA7
    ADC0_SE11 =11,      // PTA8
    ADC0_SE12 =12,      // PTB2
    ADC0_SE13 =13,      // PTB3
    ADC0_SE14 =14,      // PTC0
    ADC0_SE15 =15,      // PTC1
    ADC0_SE16 =16,      // ADC0_SE16
    ADC0_SE17 =17,      // PTE24
    ADC0_SE18 =18,      // PTE25
    ADC0_DM0  =19,      // ADC0_DM0
    ADC0_DM1  =20,      // ADC0_DM1
    ADC0_SE21 =21,      // 保留
    ADC0_SE22 =22,      // 保留
    DAC0_OUT,       // DAC0输出 不支持ADC
    RES2,           // 保留
    RES3,           // 保留
    Temp0_Sensor,   // Temperature Sensor,内部温度测量，可用ADC函数
    Bandgap0,       // 温度补偿结构带隙基准源   不支持ADC
    RES4,           // 保留
    VREFH0,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL0,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module0_Dis,    // 不支持 ADC

    // ---------------------------------ADC1-------------------------
    ADC1_DP0  =0,
    ADC1_DP1  =1,
    PGA1_DP   =2,        // 不支持 ADC
    ADC1_DP3  =3,
    ADC1_SE4a =4,      // PTE0
    ADC1_SE5a =5,      // PTE1
    ADC1_SE6a =6,      // PTE2
    ADC1_SE7a =7,      // PTE3

    ADC1_SE4b = ADC1_SE4a,  // PTC8    B通道 不支持软件ADC ,传递 ADC1_SE4b 到软件ADC函数里，会当作 ADC1_SE4a 处理。
    ADC1_SE5b = ADC1_SE5a,  // PTC9    B通道 不支持软件ADC
    ADC1_SE6b = ADC1_SE6a,  // PTC10   B通道 不支持软件ADC
    ADC1_SE7b = ADC1_SE7a,  // PTC11   B通道 不支持软件ADC

    ADC1_SE8  =8,       // PTB0
    ADC1_SE9  =9,       // PTB1
    ADC1_SE10 =10,      // PTB4
    ADC1_SE11 =11,      // PTB5
    ADC1_SE12 =12,      // PTB6
    ADC1_SE13 =13,      // PTB7
    ADC1_SE14 =14,      // PTB10
    ADC1_SE15 =15,      // PTB11
    ADC1_SE16 =16,      // ADC1_SE16
    ADC1_SE17 =17,      // PTA17
    VREF_OUTPUT,    // VREF Output
    ADC1_DM0  =19,       // ADC1_DM0
    ADC1_DM1  =20,       // ADC1_DM1
    RES5,           //保留
    RES6,
    DAC1_OUT,
    RES7,           //保留
    RES8,
    Temp1_Sensor,
    Bandgap1,       // 温度补偿结构带隙基准源   不支持ADC
    RES9,
    VREFH1,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL1,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module1_Dis,    // 不支持 ADC

} ADCn_Ch_e;

typedef enum  //ADC模块
{
    ADC_0,
    ADC_1,
} ADCn_e;

//精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


void ADC_Init(ADCn_e adc_n);

u16 ADC_Once(ADCn_e adc_n,ADCn_Ch_e adc_ch,ADC_nbit bit);
u16 ADC_Mid(ADCn_e adc_n,ADCn_Ch_e adc_ch,ADC_nbit bit);
u16 ADC_Ave(ADCn_e adc_n,ADCn_Ch_e adc_ch,ADC_nbit bit,u16 N);
void ADC_Start(ADCn_e adc_n,ADCn_Ch_e adc_ch,ADC_nbit bit);


#endif 
