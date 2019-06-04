/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_MPU6050.c
 * @brief      		MPU6050函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 * @note	
					MPU6050接线定义
					------------------------------------ 
						SDA                 C17
						SCL                 C16
					------------------------------------ 
 ********************************************************************************************************************/



#include "SEEKFREE_MPU6050.h"

int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;



//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void InitMPU6050(void)
{
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	   //解除休眠状态
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);      //125HZ采样率
    simiic_write_reg(MPU6050_DEV_ADDR, CONFIG, 0x04);          //
    simiic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);     //2000
    simiic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10);    //8g
	simiic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
    simiic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}




//内部使用用户无需调用
int16 GetData(uint8 REG_Address)
{
    uint8 L;   uint16 H ;
    H=simiic_read_reg(MPU6050_DEV_ADDR, REG_Address, IIC);
    L=simiic_read_reg(MPU6050_DEV_ADDR, REG_Address+1, IIC);
    return (H<<8)+L;   //合成数据
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_AccData(void)
{
    mpu_acc_x = GetData(ACCEL_XOUT_H)>>2;
    mpu_acc_y = GetData(ACCEL_YOUT_H)>>2;
    mpu_acc_z = GetData(ACCEL_ZOUT_H)>>2;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_Gyro(void)
{
    mpu_gyro_z = GetData(GYRO_XOUT_H)>>3;
    mpu_gyro_y = GetData(GYRO_YOUT_H)>>3;
    mpu_gyro_x = GetData(GYRO_ZOUT_H)>>3;
}


