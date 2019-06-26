#ifndef _define_h
#define _define_h
#include "headfile.h"

#define SERVO_PWM_FREQUENCY (100)       //100HZ，10ms，脉冲宽度50000
#define SPEED_PWM_FREQUENCY (10000)     //10KHZ，0.1ms，脉冲宽度500
#define PWM_CYCLE(x) (5000000 / x)
//频率：40M/(2^3)/500=10,000HZ,每个脉冲为0.2us,周期为100us

#define SPEED_PIT_CYCLE		(10.0f)		//速度控制的PIT周期，ms
#define SPEED_CONTROL_CYCLE (   10)		//速度控制周期
#define SERVO_CONTROL_CYCLE (    5)		//舵机控制周期
#define EMS_COLLECT_CYCLE   (    1)		//电磁采集周期

#define FORWARD					(1)		//前进
#define STOP					(0)		//停止
#define RETREAT					(-1)	//后退

#define EMS_CORRECT_MODE_IINT   (0)     //初始化电磁信号校准
#define EMS_CORRECT_MODE_H      (1)     //开启电磁信号中间校准模式
#define EMS_CORRECT_MODE_V      (2)     //开启电磁信号两侧校准模式
#define EMS_CORRECT_MODE_OFF    (3)     //关闭电磁信号校准模式

#define CAR_VERY_LEFT  			(-2)	//车身非常靠左
#define CAR_LEFT				(-1)	//车身偏左
#define CAR_MIDDLE				(0)		//车身正中
#define CAR_RIGHT				(1)		//车身偏右
#define CAR_VERY_RIGHT			(2)		//车身非常靠右

#define TURN_VERY_LEFT			(-2)	//向左急弯
#define TURN_LEFT				(-1)	//向左小弯
#define TURN_ZERO				(0)		//直道
#define TURN_RIGHT				(1)		//向右小弯
#define TURN_VERY_RIGHT			(2)		//向右急弯

#define PI (3.14159265358979f)
#define CAR_LENGTH (200.0f)																	//车身长，单位mm
#define CAR_WIDTH (155.0f)																	//车身宽，单位mm
#define ENCODER_WHEEL_DIAMETER (18.1f)														//编码器轮直径，单位mm
#define LINE_NUMBER (512.0f)																//512线编码器
#define LENGTH_PER_PULSE (PI * ENCODER_WHEEL_DIAMETER / LINE_NUMBER)						//每个脉冲对应的长度，单位mm
#define A2R(x) (PI * (x) / 180.0f)															//角度转弧度
#define R2A(x) (180.0f * (x) / PI)															//弧度转角度
#define SPEED_PIT_CYCLE_PER_SECOND (1000.0f / SPEED_PIT_CYCLE)								//每秒进行速度控制的次数
#define ENCODER_2_TRUE(x) ((x) * (SPEED_PIT_CYCLE_PER_SECOND * LENGTH_PER_PULSE))			//编码器读数转换真实速度，单位mm/s，即setPulse=1的速度是1.1cm/s
#define TRUE_2_ENCODER(x) ((x) / (SPEED_PIT_CYCLE_PER_SECOND * LENGTH_PER_PULSE))			//真实速度转换编码器读数
#define ADC_2_DISTANCE(x) (-0.00007f * pow((x), 3) + 0.0126f * pow((x), 2) - 1.0024f * pow((x), 1) + 41.316f * pow((x), 0))	//ADC读数转换成真实与中线距离，单位mm

#define SERVO_MIDDLE_DUTY (7130)		//舵机正中占空比
#define SERVO_MAX_TURN_DUTY (15)		//舵机最大转向占空比
#define SERVO_TURN_MOST_RIGHT (7145)	//舵机向最右转
#define SERVO_TURN_MOST_LEFT (7115)		//舵机向最左转

#define L2 (0)          //最左边电感编号0
#define L1 (1)          //最左边电感编号1
#define M0 (2)          //最左边电感编号2
#define R1 (3)          //最左边电感编号3
#define R2 (4)          //最左边电感编号4


#define BLACK0 (0x00)
#define WHITE1 (0xFF)
#endif