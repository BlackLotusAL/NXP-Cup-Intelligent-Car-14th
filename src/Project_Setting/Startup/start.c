//-------------------------------------------------------------------------*
// 文件名:start.c                                                          *
// 说  明: CPU启动后进行系统配置                                           *
//-------------------------------------------------------------------------*
#include "include.h"
//头文件
#include "common.h"
#include "wdog.h"
//#include "sysinit.h"

#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam" 

//内部函数声明
//-------------------------------------------------------------------------*
//函数名: common_startup                                                   *
//功  能: 复制中断向量表到RAM中                                            * 
//参  数: 无								   *	
//说  明: 将ROM中的初始化数据拷贝到RAM中                                   *
//-------------------------------------------------------------------------*
void common_startup(void);

//-------------------------------------------------------------------------*
//函数名: start                                                            *
//功  能: 系统启动                                                         * 
//参  数: 无								   *	
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void start(void)
{
    //关闭看门狗
    wdog_disable();		
    //复制中断向量表到RAM中
    common_startup();				
    //进入主函数
    
    //开启浮点支持功能
    SCB->CPACR |=((3UL << 10*2)|(3UL << 11*2));     /* set CP10 and CP11 Full Access */
    //MCM_ISCR = 0x9f000000;
    //__enable_FPU();
    main();
    /* No actions to perform after this so wait forever */
    while(1);

}


//-------------------------------------------------------------------------*
//函数名: common_startup                                                   *
//功  能: 复制中断向量表到RAM中                                            * 
//参  数: 无								   *	
//说  明: 将ROM中的初始化数据拷贝到RAM中                                   *
//-------------------------------------------------------------------------*
void common_startup(void)
{
    /* Declare a counter we'll use in all of the copy loops */
    uint32 n;
 
 
    /* Addresses for VECTOR_TABLE and VECTOR_RAM come from the linker file */  
    extern uint32 __VECTOR_TABLE[];
    extern uint32 __VECTOR_RAM[];

    /* Copy the vector table to RAM */
    if (__VECTOR_RAM != __VECTOR_TABLE)
    {
        for (n = 0; n < 0x410; n++)
            __VECTOR_RAM[n] = __VECTOR_TABLE[n];
    }
    /* Point the VTOR to the new copy of the vector table */
    write_vtor((uint32)__VECTOR_RAM);    
    
    /* Get the addresses for the .data section (initialized data section) */
    uint8* data_ram = __section_begin(".data");
    uint8* data_rom = __section_begin(".data_init");
    uint8* data_rom_end = __section_end(".data_init");
    
    /* Copy initialized data from ROM to RAM */
    n = data_rom_end - data_rom;
    while (n--)
      *data_ram++ = *data_rom++;
 
 
    /* Get the addresses for the .bss section (zero-initialized data) */
    uint8* bss_start = __section_begin(".bss");
    uint8* bss_end = __section_end(".bss");
    
    /* Clear the zero-initialized data section */
    n = bss_end - bss_start;
    while(n--)
      *bss_start++ = 0;    
    
    /* Get addresses for any code sections that need to be copied from ROM to RAM.
     * The IAR tools have a predefined keyword that can be used to mark individual
     * functions for execution from RAM. Add "__ramfunc" before the return type in
     * the function prototype for any routines you need to execute from RAM instead 
     * of ROM. ex: __ramfunc void foo(void);
     */
    uint8* code_relocate_ram = __section_begin("CodeRelocateRam");
    uint8* code_relocate = __section_begin("CodeRelocate");
    uint8* code_relocate_end = __section_end("CodeRelocate");
    
    /* Copy functions from ROM to RAM */
    n = code_relocate_end - code_relocate;
    while (n--)
      *code_relocate_ram++ = *code_relocate++;
}