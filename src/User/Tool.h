#ifndef _Tool_H_
#define _Tool_H_
#include "include.h"

typedef struct
{
    int new;    //当前按键状态
    int old;    //上一个按键状态
    int cnt;    //按键次数计数
    int flag;   //按键状态标志
}KEY_value;

void EMS_Correct_KEY_Operation(KEY_value *key);
void GO_KEY_Operation(KEY_value *key);
float Constrain_float(float input, float min, float max);
int Constrain_int(int input, int min, int max);
int Constrain_int_output(int input, int output, int min, int max);

#endif