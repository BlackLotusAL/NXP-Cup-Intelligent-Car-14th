#ifndef _fun_h
#define _fun_h
#include "headfile.h"

typedef struct
{
    int new;    //当前按键状态
    int old;    //上一个按键状态
    int cnt;    //按键次数计数
    int flag;   //按键状态标志
}KEY_value;

void SendWare();
void SendArr(int Arr[], int Size);
void SendStr();
void EMS_Correct_KEY_Operation(KEY_value *key);
void GO_KEY_Operation(KEY_value *key);
void Servo_Test_Key(KEY_value *key0, KEY_value *key1, KEY_value *key2);
float Constrain_float(float input, float min, float max);
int Constrain_int(int input, int min, int max);
int Constrain_int_output(int input, int output, int min, int max);

#endif