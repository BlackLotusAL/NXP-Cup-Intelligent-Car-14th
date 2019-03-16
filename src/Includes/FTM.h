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

#ifndef _FTM_H_
#define _FTM_H_

/**********************************  FTM(引脚复用) ***************************************/

//      模块通道    端口          可选范围              建议
#define FTM0_CH0    PTC1        //PTC1、PTA3            PTA3不要用（与JLINK冲突）
#define FTM0_CH1    PTC2        //PTC2、PTA4
#define FTM0_CH2    PTC3        //PTC3、PTA5
#define FTM0_CH3    PTC4        //PTC4、PTA6
#define FTM0_CH4    PTD4        //PTD4、PTA7
#define FTM0_CH5    PTD5        //PTD5、PTA0            PTA0不要用（与JLINK冲突）
#define FTM0_CH6    PTD6        //PTD6、PTA1            PTA1不要用（与JLINK冲突）
#define FTM0_CH7    PTD7        //PTD7、PTA2            PTA2不要用（与JLINK冲突）

//      模块通道    端口          可选范围
#define FTM1_CH0    PTA8       //PTA8、PTA12、PTB0
#define FTM1_CH1    PTA9       //PTA9、PTA13、PTB1

//      模块通道    端口          可选范围
#define FTM2_CH0    PTA10       //PTA10、PTB18
#define FTM2_CH1    PTA11       //PTA11、PTB19

//      模块通道    端口          可选范围
#define FTM3_CH0    PTE5        // PTE5、PTD0
#define FTM3_CH1    PTE6        // PTE6、PTD1
#define FTM3_CH2    PTE7        // PTE7、PTD2
#define FTM3_CH3    PTE8        // PTE8、PTD3
#define FTM3_CH4    PTE9        // PTE9、PTC8
#define FTM3_CH5    PTE10       // PTE10、PTC9
#define FTM3_CH6    PTC10       // PTE11、PTC10
#define FTM3_CH7    PTC11       // PTE12、PTC11

//正交解码模块通道  端口          可选范围
#define FTM1_QDPHA  PTA12       //PTA8、PTA12、PTB0
#define FTM1_QDPHB  PTA13       //PTA9、PTA13、PTB1

#define FTM2_QDPHA  PTB18       //PTA10、PTB18
#define FTM2_QDPHB  PTB19       //PTA11、PTB19

/**********************************  FTM(引脚复用) ***************************************/


//定义FTM 通道号
typedef enum
{

    FTM_CH0,
    FTM_CH1,
    FTM_CH2,
    FTM_CH3,
    FTM_CH4,
    FTM_CH5,
    FTM_CH6,
    FTM_CH7,

} FTM_CHn_e;


typedef enum
{
    FTM_Rising,               //上升沿捕捉
    FTM_Falling,              //下降沿捕捉
    FTM_Rising_or_Falling     //跳变沿捕捉
} FTM_Input_cfg;

#define FTM_IRQ_EN(FTMn,CHn)        FTM_CnSC_REG(FTMN[FTMn],CHn) |= FTM_CnSC_CHIE_MASK       //开启 FTMn_CHn 中断
#define FTM_IRQ_DIS(FTMn,CHn)       FTM_CnSC_REG(FTMN[FTMn],CHn) &= ~FTM_CnSC_CHIE_MASK      //关闭 FTMn_CHn 中断
/*********************** PWM功能函数 **************************/
void FTM_PWM_Init(FTM_Type * ftmn, FTM_CHn_e ch, u16 mod, u16 cv);
void FTM_PWM_Duty(FTM_Type * ftmn, FTM_CHn_e ch, u16 duty);

/*********************** 输入捕捉及中断功能函数 **************************/
void FTM_Input_init(FTM_Type * ftmn, FTM_CHn_e ch, FTM_Input_cfg cfg);
void FTM0_interrupt(void);
void FTM1_interrupt(void);
void FTM2_interrupt(void);
/*********************** 正交解码功能函数 **************************/
void FTM_AB_Init(FTM_Type * ftmn);         //初始化FTM 的正交解码 功能
s16  FTM_AB_Get(FTM_Type * ftmn);          //获取FTM 正交解码 的脉冲数(负数表示反方向)


#endif



