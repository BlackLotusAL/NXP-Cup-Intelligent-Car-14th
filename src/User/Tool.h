#ifndef _Tool_H_
#define _Tool_H_
#include "include.h"

typedef struct
{
    int new;
    int old;
    int cnt;
    int flag;  
}KEY_value;

void EMS_Correct_KEY_Operation(KEY_value *key);
void GO_KEY_Operation(KEY_value *key);
float Constrain_float(float input, float min, float max);
int Constrain_int(int input, int min, int max);
int Constrain_int_output(int input, int output, int min, int max);

#endif