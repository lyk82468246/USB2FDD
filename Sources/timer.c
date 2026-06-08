//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: timer.c
// 文件描述: 
// 文件版本: V1.0
// 修改记录:
//   1. (2026-06-08) 创建文件
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#include "config.h"


//<<AICUBE_USER_INCLUDE_BEGIN>>
// 在此添加用户头文件包含  
//<<AICUBE_USER_INCLUDE_END>>


//<<AICUBE_USER_GLOBAL_DEFINE_BEGIN>>
// 在此添加用户全局变量定义、用户宏定义以及函数声明  
volatile uint32_t g_ms_ticks = 0;
//<<AICUBE_USER_GLOBAL_DEFINE_END>>



////////////////////////////////////////
// 定时器0初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void TIMER0_Init(void)
{
#define T0_PSCR                 (0)
#define T0_RELOAD               (65536 - 20)

    TIMER0_TimerMode();                 //设置定时器0为定时模式
    TIMER0_1TMode();                    //设置定时器0为1T模式
    TIMER0_Mode0();                     //设置定时器0为模式0 (16位自动重载模式)
    TIMER0_DisableGateINT0();           //禁止定时器0门控
    TIMER0_SetPrescale(T0_PSCR);        //设置定时器0的8位预分频
    TIMER0_SetReload16(T0_RELOAD);      //设置定时器0的16位重载值
    TIMER0_Run();                       //定时器0开始运行

    //<<AICUBE_USER_TIMER0_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    TIMER0_SetReload16(0);
    TH0 = 0;
    TL0 = 0;
    TIMER0_ClearFlag();
    //<<AICUBE_USER_TIMER0_INITIAL_END>>
}

////////////////////////////////////////
// 定时器11初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void TIMER11_Init(void)
{
#define T11_PSCR                (1)
#define T11_RELOAD              (65536 - (float)SYSCLK / (T11_PSCR + 1) * 1 / 1000) //定时周期1毫秒

    TIMER11_CLK_SYSCLK();               //定时器11使用系统时钟作为时钟源

    TIMER11_TimerMode();                //设置定时器11为定时模式
    TIMER11_1TMode();                   //设置定时器11为1T模式
    TIMER11_EnableInt();                //使能定时器11中断
    TIMER11_SetPrescale(T11_PSCR);      //设置定时器11的8位预分频
    TIMER11_SetReload16(T11_RELOAD);    //设置定时器11的16位重载值
    TIMER11_Run();                      //定时器11开始运行

    //<<AICUBE_USER_TIMER11_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_TIMER11_INITIAL_END>>
}


////////////////////////////////////////
// 定时器11中断服务程序
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void TIMER11_ISR(void) interrupt TMR11_VECTOR
{
    //<<AICUBE_USER_TIMER11_ISR_CODE1_BEGIN>>
    // 在此添加中断函数用户代码  
    TIMER11_ClearFlag();
    g_ms_ticks++;
    //<<AICUBE_USER_TIMER11_ISR_CODE1_END>>
}


//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码  
uint16_t FDD_Timer0_Read16(void)
{
    uint8_t ea_save;
    uint16_t value;

    ea_save = EA;
    DisableGlobalInt();
    value = ((uint16_t)TH0 << 8) | TL0;
    if (ea_save)
        EnableGlobalInt();

    return value;
}

uint32_t FDD_GetMillis(void)
{
    uint8_t ea_save;
    uint32_t value;

    ea_save = EA;
    DisableGlobalInt();
    value = g_ms_ticks;
    if (ea_save)
        EnableGlobalInt();

    return value;
}

void FDD_DelayMs(uint16_t ms)
{
    uint32_t start;

    start = FDD_GetMillis();
    while ((uint32_t)(FDD_GetMillis() - start) < ms)
        WDT_Clear();
}
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


