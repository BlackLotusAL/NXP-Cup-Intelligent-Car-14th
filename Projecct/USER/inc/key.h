#ifndef _key_h
#define _key_h
#include "headfile.h"

//定义模块号
typedef enum
{
    KEY0 = 0,
    KEY1 = 1,
    KEY2 = 2,   
} KEYn_e;
typedef enum
{
    LOW = 0,  //亮
    HIGH = 1, //灭   
    FAIL = 0xff,
}KEYs_e;

void KEY_Init(void);
uint8 KEY_Read(KEYn_e keyno);

#endif 