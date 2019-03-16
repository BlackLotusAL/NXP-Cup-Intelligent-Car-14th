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


void KEY_Init(void)
{
   GPIO_Init(PTB,20,GPI,1);
   GPIO_Init(PTB,21,GPI,1);
   GPIO_Init(PTB,22,GPI,1);   
}


u8 KEY_Read(KEYn_e keyno)
{
    switch(keyno) 
    {
      case KEY0:
        return GPIO_Get(PTB20);
      break;
      
      case KEY1:
        return GPIO_Get(PTB21);
      break;
      
      case KEY2:
        return GPIO_Get(PTB22);
      break;
      default:
        return 0XFF;
           
    }
}

//

