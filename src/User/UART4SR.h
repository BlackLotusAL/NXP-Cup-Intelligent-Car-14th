#ifndef _UART4SR_H_
#define _UART4SR_H_
#include "include.h"

void SendWare();
void SendArr(int Arr[], int Size);
void SendStr();
void ReceiveStr(s8 data);
void ReceiveStr_Init();

#endif