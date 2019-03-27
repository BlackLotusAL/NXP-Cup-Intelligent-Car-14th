#ifndef _DEFINE_H_
#define _DEFINE_H_
#include "include.h"

#define SERVO_PWM_FREQUENCY (100)       //100HZ，10ms，脉冲宽度50000
#define SPEED_PWM_FREQUENCY (10000)     //10000HZ，0.1ms，脉冲宽度500
#define PWM_CYCLE(x) (5000000 / x)
//频率：40M/(2^3)/500=10,000HZ,每个脉冲为0.2us,周期为100us

#define SPEED_PIT_CYCLE         (10.0)          //速度控制的PIT周期，ms
#define SPEED_CONTROL_CYCLE     (10 * 1000)     //速度控制周期
#define SERVO_CONTROL_CYCLE     ( 5 * 1000)     //舵机控制周期
#define EMS_COLLECT_CYCLE       ( 1 * 1000)     //电磁采集周期
#define UART_SEND_CYCLE         (100 * 1000)    //蓝牙发数周期

#define FORWARD (1)                     //前进
#define STOP (0)                        //停止
#define RETREAT (-1)                    //后退

#define EMS_CORRECT_MODE_IINT   (0)     //初始化电磁信号校准
#define EMS_CORRECT_MODE_H      (1)     //开启电磁信号中间校准模式
#define EMS_CORRECT_MODE_V      (2)     //开启电磁信号两侧校准模式
#define EMS_CORRECT_MODE_OFF    (3)     //关闭电磁信号校准模式

#define CLB (-2)                        //车身非常靠左
#define CLS (-1)                        //车身偏左
#define CMZ (0)                         //车身正中
#define CRS (1)                         //车身偏右
#define CRB (2)                         //车身非常靠右

#define TLB (-2)                        //向左急弯
#define TLS (-1)                        //向左小弯
#define TMZ (0)                         //直道
#define TRS (1)                         //向右小弯
#define TRB (2)                         //向右急弯

#define CAR_LENGTH (200.0)                                                              //车身长，mm
#define CAR_WIDTH (155.0)                                                               //车身宽，mm
#define ENCODER_WHEEL_DIAMETER (18.1)                                                   //编码器轮直径，mm
#define LINE_NUMBER (512.0)                                                             //512线编码器
#define LENGTH_PER_PULSE (PI * ENCODER_WHEEL_DIAMETER / LINE_NUMBER)                    //每个脉冲对应的长度，mm
#define A2R(x) (PI * (x) / 180.0)                                                       //角度转换弧度
#define R2A(x) (180.0 * (x) / PI)                                                       //弧度转换角度
#define SPEED_PIT_CYCLE_PER_SECOND (1000.0 / SPEED_PIT_CYCLE)                           //每秒进行速度控制的次数
#define ENCODER_2_TRUE(x) ((x) * (SPEED_PIT_CYCLE_PER_SECOND * LENGTH_PER_PULSE))       //编码器读数转换真实速度，mm/s,setPulse=1，1.1cm/s
#define TRUE_2_ENCODER(x) ((x) / (SPEED_PIT_CYCLE_PER_SECOND * LENGTH_PER_PULSE))       //真实速度转换编码器读数
#define ADC_2_DISTANCE(x) (-0.00007 * pow((x), 3) + 0.0126 * pow((x), 2) - 1.0024 * pow((x), 1) + 41.316 * pow((x), 0)) //ADC读数转换成真实与中线距离，mm

#define SERVO_MIDDLE_DUTY (7050)        //舵机正中占空比
#define SERVO_MAX_TURN_DUTY (700)       //舵机最大转向占空比
#define SERVO_TURN_MOST_RIGHT (7750)    //舵机向最右转
#define SERVO_TURN_MOST_LEFT (6350)     //舵机向最左转

#define L2 (0)
#define L1 (1)
#define M0 (2)
#define R1 (3)
#define R2 (4)

#endif