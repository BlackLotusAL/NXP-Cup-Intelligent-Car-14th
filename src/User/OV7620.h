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
#ifndef __LQ_OV7620_H_
#define __LQ_OV7620_H_

#define LIN  320  //列
#define ROW  240  //行

#define L80   80
#define R60   60
 
#define OLEDV   80
#define OLEDH   60

extern void Seek_Becon_Dots(void);
extern u8 Calc_Offset(void);

extern unsigned char Image_data[ROW][LIN];
extern unsigned char Copy_Image[R60][L80];//拷贝后图像
extern unsigned char ROW_Count;
extern unsigned char FieldCounter;
extern unsigned char Is_SendPhoto;

extern void Copy_Arr(unsigned char aa[],unsigned char b[]);            //拷贝图像
extern void BinaryZation(unsigned char (*p)[L80]);  //二值化
extern void Peng_Fushi(unsigned char (*t)[L80],unsigned char (*temp)[L80],unsigned char b);
extern void Image_Filter(void);                               //一步到位函数，二值化，滤波

void OV7620_Init(void);
void SendPicture(void);


#endif