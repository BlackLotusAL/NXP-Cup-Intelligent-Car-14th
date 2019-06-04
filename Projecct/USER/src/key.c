#include "key.h"


void KEY_Init(void)
{
   gpio_init(B20,GPI,1);
   gpio_init(B21,GPI,1);
   gpio_init(B22,GPI,1);   
}

uint8 KEY_Read(KEYn_e keyno)
{
    switch(keyno) 
    {
      case KEY0:
        return gpio_get(B20);
      break;
      
      case KEY1:
        return gpio_get(B21);
      break;
      
      case KEY2:
        return gpio_get(B22);
      break;
	  
      default:
        return 0xff;   
    }
}