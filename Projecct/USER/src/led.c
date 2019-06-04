#include "led.h"

void LED_Init(void)
{
   gpio_init(A17, GPO, 1);
   gpio_init(C0,  GPO, 1);
   gpio_init(D15, GPO, 1);
   gpio_init(E26, GPO, 1);
   gpio_init(C18, GPO, 1);
   gpio_init(C19, GPO, 1);
}

void LED_Ctrl(LEDn_e ledno, LEDs_e sta)
{
	switch(ledno) 
    {
		case LED0:
		if(sta==ON)			gpio_set(A17,0);
		else if(sta==OFF)	gpio_set(A17,1);
		else if(sta==RVS)	gpio_turn(A17);
		break;
    
		case LED1:
		if(sta==ON)			gpio_set(C0,0);
		else if(sta==OFF)	gpio_set(C0,1);
		else if(sta==RVS)	gpio_turn(C0);
		break;
    
		case LED2:
		if(sta==ON)			gpio_set(D15,0);
		else if(sta==OFF)	gpio_set(D15,1);
		else if(sta==RVS)	gpio_turn(D15);
		break; 
    
		case LED3:
		if(sta==ON)			gpio_set(E26,0);
		else if(sta==OFF)	gpio_set(E26,1);
		else if(sta==RVS)	gpio_turn(E26);
		break;
		
		case LED4:
		if(sta==ON)			gpio_set(C18,0);
		else if(sta==OFF)	gpio_set(C18,1);
		else if(sta==RVS)	gpio_turn (C18);
		break;
		
		case LED5:
		if(sta==ON)			gpio_set(C19,0);
		else if(sta==OFF)	gpio_set(C19,1);
		else if(sta==RVS)	gpio_turn(C19);
		break;
		
		case LEDALL:
		if(sta==ON) 
		{       
			gpio_set(A17,0);
			gpio_set(C0,0);
			gpio_set(D15,0);
			gpio_set(E26,0);
			gpio_set(C18,0);
			gpio_set(C19,0);
		}
		else if(sta==OFF)
		{ 
			gpio_set(A17,1);
			gpio_set(C0,1);
			gpio_set(D15,1);
			gpio_set(E26,1);
			gpio_set(C18,1);
			gpio_set(C19,1);
		}
		else if(sta==RVS)
		{       
			gpio_turn(A17);     
			gpio_turn(C0);      
			gpio_turn(D15);      
			gpio_turn(E26);
			gpio_turn(C18);      
			gpio_turn(C19);
		}
		break;
		
		default:
		break;    
    }   
}