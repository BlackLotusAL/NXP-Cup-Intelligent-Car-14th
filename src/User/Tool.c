#include "include.h"

extern int goFlag;

void EMS_Correct_KEY_Operation(KEY_value *key)
{
    key->new = KEY_Read(KEY1);
    if(key->new == 0 && key->old == 1)
    {
        key->cnt++;
    }
    key->flag = key->cnt % 4;
    key->old = key->new;
    
    switch(key->flag)
    {
        case (EMS_CORRECT_MODE_IINT):
            LED_Ctrl(LED0, ON);
            break;
        case (EMS_CORRECT_MODE_H):
            LED_Ctrl(LED1, ON);
            LED_Ctrl(LED3, ON);
            break;
        case (EMS_CORRECT_MODE_V):
            LED_Ctrl(LED0, OFF);
            break;
        case (EMS_CORRECT_MODE_OFF):
            LED_Ctrl(LEDALL, OFF);      //LED全灭
            break;
    }
}

void GO_KEY_Operation(KEY_value *key)
{
    key->new = KEY_Read(KEY2);
    if(key->new && (key->new ^ key->old))
    {
        goFlag = 1;
    }
    
    if(goFlag)
    {
        LED_Ctrl(LED2, ON);
    }
    else
    {
        LED_Ctrl(LED2, OFF);
    }
    key->old = key->new;
}

float Constrain_float(float input, float min, float max) 
{
    return ((input < min) ? (min) : ((input > max) ? (max) : (input)));
}

int Constrain_int(int input, int min, int max) 
{
    return ((input < min) ? (min) : ((input > max) ? (max) : (input)));
}

int Constrain_int_output(int input, int output, int min, int max) 
{
    return ((input < min) ? (output) : ((input > max) ? (output) : (input)));
}