//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: exti.c
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
static uint16_t xdata g_flux_buffer[FDD_FLUX_BUFFER_SIZE];
static volatile uint16_t g_flux_write = 0;
static volatile uint16_t g_flux_read = 0;
static volatile uint16_t g_flux_last_time = 0;
static volatile uint8_t g_flux_armed = 0;
static volatile uint8_t g_flux_capture_enable = 0;
static volatile uint16_t g_flux_overflow = 0;
volatile uint16_t g_index_count = 0;
volatile uint16_t g_last_index_time = 0;
static volatile uint32_t g_last_index_ms = 0;
volatile uint32_t g_index_period_ms = 0;
//<<AICUBE_USER_GLOBAL_DEFINE_END>>



////////////////////////////////////////
// 外部中断INT0初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void EXTI0_Init(void)
{
    INT0_FallingInt();                  //设置外部中断为下降沿中断
    INT0_SetIntPriority(3);             //设置中断为最高优先级
    INT0_EnableInt();                   //使能外部中断

    //<<AICUBE_USER_EXTI0_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_EXTI0_INITIAL_END>>
}

////////////////////////////////////////
// 外部中断INT1初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void EXTI1_Init(void)
{
    INT1_FallingInt();                  //设置外部中断为下降沿中断
    INT1_SetIntPriority(2);             //设置中断为较高优先级
    INT1_EnableInt();                   //使能外部中断

    //<<AICUBE_USER_EXTI1_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_EXTI1_INITIAL_END>>
}


////////////////////////////////////////
// 外部中断INT0中断服务程序
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void EXTI0_ISR(void) interrupt INT0_VECTOR
{
    //<<AICUBE_USER_EXTI0_ISR_CODE1_BEGIN>>
    // 在此添加中断函数用户代码  
    {
        uint16_t now;
        uint16_t next;

        if (!g_flux_capture_enable)
            return;

        now = ((uint16_t)TH0 << 8) | TL0;
        if (g_flux_armed)
        {
            next = g_flux_write + 1;
            if (next >= FDD_FLUX_BUFFER_SIZE)
                next = 0;

            if (next != g_flux_read)
            {
                g_flux_buffer[g_flux_write] = now - g_flux_last_time;
                g_flux_write = next;
            }
            else
            {
                g_flux_overflow++;
            }
        }
        else
        {
            g_flux_armed = 1;
        }
        g_flux_last_time = now;
    }
    //<<AICUBE_USER_EXTI0_ISR_CODE1_END>>
}

////////////////////////////////////////
// 外部中断INT1中断服务程序
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void EXTI1_ISR(void) interrupt INT1_VECTOR
{
    //<<AICUBE_USER_EXTI1_ISR_CODE1_BEGIN>>
    // 在此添加中断函数用户代码  
    {
        uint32_t now_ms;

        now_ms = g_ms_ticks;
        g_index_count++;
        g_last_index_time = ((uint16_t)TH0 << 8) | TL0;
        if (g_last_index_ms != 0)
            g_index_period_ms = now_ms - g_last_index_ms;
        g_last_index_ms = now_ms;
    }
    //<<AICUBE_USER_EXTI1_ISR_CODE1_END>>
}


//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码  
void FDD_Flux_Reset(void)
{
    uint8_t ea_save;

    ea_save = EA;
    DisableGlobalInt();
    g_flux_write = 0;
    g_flux_read = 0;
    g_flux_last_time = ((uint16_t)TH0 << 8) | TL0;
    g_flux_armed = 0;
    g_flux_overflow = 0;
    if (ea_save)
        EnableGlobalInt();
}

void FDD_Flux_Start(void)
{
    uint8_t ea_save;

    ea_save = EA;
    DisableGlobalInt();
    g_flux_capture_enable = 1;
    g_flux_armed = 0;
    g_flux_last_time = ((uint16_t)TH0 << 8) | TL0;
    if (ea_save)
        EnableGlobalInt();
}

void FDD_Flux_Stop(void)
{
    uint8_t ea_save;

    ea_save = EA;
    DisableGlobalInt();
    g_flux_capture_enable = 0;
    g_flux_armed = 0;
    if (ea_save)
        EnableGlobalInt();
}

uint8_t FDD_Flux_IsCapturing(void)
{
    return g_flux_capture_enable;
}

uint16_t FDD_Flux_Available(void)
{
    uint8_t ea_save;
    uint16_t write;
    uint16_t read;
    uint16_t available;

    ea_save = EA;
    DisableGlobalInt();
    write = g_flux_write;
    read = g_flux_read;

    if (write >= read)
        available = write - read;
    else
        available = FDD_FLUX_BUFFER_SIZE - read + write;

    if (ea_save)
        EnableGlobalInt();

    return available;
}

uint16_t FDD_Flux_GetOverflowCount(void)
{
    uint8_t ea_save;
    uint16_t overflow;

    ea_save = EA;
    DisableGlobalInt();
    overflow = g_flux_overflow;
    if (ea_save)
        EnableGlobalInt();

    return overflow;
}

uint16_t FDD_Flux_Read(uint16_t *dst, uint16_t max_count)
{
    uint8_t ea_save;
    uint16_t count;

    count = 0;
    while (count < max_count)
    {
        ea_save = EA;
        DisableGlobalInt();
        if (g_flux_read == g_flux_write)
        {
            if (ea_save)
                EnableGlobalInt();
            break;
        }

        dst[count] = g_flux_buffer[g_flux_read];
        g_flux_read++;
        if (g_flux_read >= FDD_FLUX_BUFFER_SIZE)
            g_flux_read = 0;
        if (ea_save)
            EnableGlobalInt();
        count++;
    }

    return count;
}

void FDD_Index_Reset(void)
{
    uint8_t ea_save;

    ea_save = EA;
    DisableGlobalInt();
    g_index_count = 0;
    g_last_index_ms = 0;
    g_index_period_ms = 0;
    g_last_index_time = ((uint16_t)TH0 << 8) | TL0;
    if (ea_save)
        EnableGlobalInt();
}

uint8_t FDD_Index_Wait(uint16_t timeout_ms)
{
    uint16_t start_count;
    uint32_t start_ms;

    start_count = g_index_count;
    start_ms = FDD_GetMillis();

    while ((uint32_t)(FDD_GetMillis() - start_ms) < timeout_ms)
    {
        if (g_index_count != start_count)
            return 1;
        WDT_Clear();
    }

    return 0;
}

uint32_t FDD_Index_GetPeriodMs(void)
{
    uint8_t ea_save;
    uint32_t period;

    ea_save = EA;
    DisableGlobalInt();
    period = g_index_period_ms;
    if (ea_save)
        EnableGlobalInt();

    return period;
}
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


