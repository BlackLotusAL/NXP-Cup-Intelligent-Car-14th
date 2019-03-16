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
#include "vectors.h"

//中断向量表
typedef void (*vector_entry)(void);

#pragma location = ".intvec"
const vector_entry  __vector_table[] = //@ ".intvec" =
{
   VECTOR_000,           //初始化SP
   VECTOR_001,           //初始化PC
   VECTOR_002,
   VECTOR_003,
   VECTOR_004,
   VECTOR_005,
   VECTOR_006,
   VECTOR_007,
   VECTOR_008,
   VECTOR_009,
   VECTOR_010,
   VECTOR_011,
   VECTOR_012,
   VECTOR_013,
   VECTOR_014,
   VECTOR_015,
   VECTOR_016,
   VECTOR_017,
   VECTOR_018,
   VECTOR_019,
   VECTOR_020,
   VECTOR_021,
   VECTOR_022,
   VECTOR_023,
   VECTOR_024,
   VECTOR_025,
   VECTOR_026,
   VECTOR_027,
   VECTOR_028,
   VECTOR_029,
   VECTOR_030,
   VECTOR_031,
   VECTOR_032,
   VECTOR_033,
   VECTOR_034,
   VECTOR_035,
   VECTOR_036,
   VECTOR_037,
   VECTOR_038,
   VECTOR_039,
   VECTOR_040,
   VECTOR_041,
   VECTOR_042,
   VECTOR_043,
   VECTOR_044,
   VECTOR_045,
   VECTOR_046,
   VECTOR_047,
   VECTOR_048,
   VECTOR_049,
   VECTOR_050,
   VECTOR_051,
   VECTOR_052,
   VECTOR_053,
   VECTOR_054,
   VECTOR_055,
   VECTOR_056,
   VECTOR_057,
   VECTOR_058,
   VECTOR_059,
   VECTOR_060,
   VECTOR_061,
   VECTOR_062,
   VECTOR_063,
   VECTOR_064,
   VECTOR_065,
   VECTOR_066,
   VECTOR_067,
   VECTOR_068,
   VECTOR_069,
   VECTOR_070,
   VECTOR_071,
   VECTOR_072,
   VECTOR_073,
   VECTOR_074,
   VECTOR_075,
   VECTOR_076,
   VECTOR_077,
   VECTOR_078,
   VECTOR_079,
   VECTOR_080,
   VECTOR_081,
   VECTOR_082,
   VECTOR_083,
   VECTOR_084,
   VECTOR_085,
   VECTOR_086,
   VECTOR_087,
   VECTOR_088,
   VECTOR_089,
   VECTOR_090,
   VECTOR_091,
   VECTOR_092,
   VECTOR_093,
   VECTOR_094,
   VECTOR_095,
   VECTOR_096,
   VECTOR_097,
   VECTOR_098,
   VECTOR_099,
   VECTOR_100,
   VECTOR_101,
   VECTOR_102,
   VECTOR_103,
   VECTOR_104,
   VECTOR_105,
   VECTOR_106,
   VECTOR_107,
   VECTOR_108,
   VECTOR_109,
   VECTOR_110,
   VECTOR_111,
   VECTOR_112,
   VECTOR_113,
   VECTOR_114,
   VECTOR_115,
};
/******************************************************************************
* default_isr(void)
*
* Default ISR definition.
*
* In:  n/a
* Out: n/a
******************************************************************************/
void default_isr(void)
{
   /* ICSR register in System control block of Cortex M4 Core, low byte has active number */ 
   #define VECTORNUM                     (*(volatile uint8_t*)(0xE000ED04))

   printf("\n****default_isr entered on vector %d*****\r\n\n",VECTORNUM);
}
/******************************************************************************/
/* Generic interrupt handler for the PTA4 GPIO interrupt connected to an 
 * abort switch. 
 * NOTE: For true abort operation this handler should be modified
 * to jump to the main process instead of executing a return.
 */
void abort_isr(void)
{
   /* Write 1 to the PTA4 interrupt flag bit to clear the interrupt */
   PORTA_PCR4 |= PORT_PCR_ISF_MASK;    
  
   printf("\n****Abort button interrupt****\n\n");
   return;
}
/******************************************************************************/
/* Exception frame without floating-point storage 
 * hard fault handler in C,
 * with stack frame location as input parameter
 */
void 
hard_fault_handler_c(unsigned int * hardfault_args)
{
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;
    
    //Exception stack frame
    stacked_r0 = ((unsigned long) hardfault_args[0]);
    stacked_r1 = ((unsigned long) hardfault_args[1]);
    stacked_r2 = ((unsigned long) hardfault_args[2]);
    stacked_r3 = ((unsigned long) hardfault_args[3]);
    
    stacked_r12 = ((unsigned long) hardfault_args[4]);
    stacked_lr = ((unsigned long) hardfault_args[5]);
    stacked_pc = ((unsigned long) hardfault_args[6]);
    stacked_psr = ((unsigned long) hardfault_args[7]);
    
    printf ("[Hard fault handler]\n");
    printf ("R0 = %x\n", stacked_r0);
    printf ("R1 = %x\n", stacked_r1);
    printf ("R2 = %x\n", stacked_r2);
    printf ("R3 = %x\n", stacked_r3);
    printf ("R12 = %x\n", stacked_r12);
    printf ("LR = %x\n", stacked_lr);
    printf ("PC = %x\n", stacked_pc);
    printf ("PSR = %x\n", stacked_psr);
    printf ("BFAR = %x\n", (*((volatile unsigned long *)(0xE000ED38))));    //Bus fault address register
    printf ("CFSR = %x\n", (*((volatile unsigned long *)(0xE000ED28))));    //MemManager fault address register
    printf ("HFSR = %x\n", (*((volatile unsigned long *)(0xE000ED2C))));    //Hard fault status register
    printf ("DFSR = %x\n", (*((volatile unsigned long *)(0xE000ED30))));    //???
    printf ("AFSR = %x\n", (*((volatile unsigned long *)(0xE000ED3C))));    //Auxiliary fault status register
    
	for(;;)
	{
        /*infinite loop*/
	}	  
} 

/* End of "vectors.c" */
