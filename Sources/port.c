//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: port.c
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
static uint8_t g_fdd_track = 0;
static uint8_t g_fdd_track_valid = 0;
static uint8_t g_fdd_selected = 0;
static uint8_t g_fdd_motor_on = 0;
static uint8_t g_fdd_direction_outward = 0;
static uint8_t g_fdd_side = 0;
static uint8_t g_fdd_high_density = 1;
//<<AICUBE_USER_GLOBAL_DEFINE_END>>



////////////////////////////////////////
// P0口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT0_Init(void)
{
    SetP0nInitLevelHigh(PIN_ALL);       //设置P0初始化电平
    SetP0nQuasiMode(PIN_ALL);           //设置P0为准双向口模式
    SetP0nManualMode(PIN_ALL);          //设置P0手动配置端口模式

    DisableP0nPullUp(PIN_ALL);          //关闭P0内部上拉电阻
    DisableP0nPullDown(PIN_ALL);        //关闭P0内部下拉电阻
    EnableP0nSchmitt(PIN_ALL);          //使能P0施密特触发
    SetP0nSlewRateNormal(PIN_ALL);      //设置P0一般翻转速度
    SetP0nDrivingNormal(PIN_ALL);       //设置P0一般驱动能力
    SetP0nDigitalInput(PIN_ALL);        //使能P0数字信号输入功能

    //<<AICUBE_USER_PORT0_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT0_INITIAL_END>>
}

////////////////////////////////////////
// P1口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT1_Init(void)
{
    SetP1nInitLevelHigh(PIN_ALL);       //设置P1初始化电平
    SetP1nQuasiMode(PIN_ALL);           //设置P1为准双向口模式
    SetP1nManualMode(PIN_ALL);          //设置P1手动配置端口模式

    DisableP1nPullUp(PIN_ALL);          //关闭P1内部上拉电阻
    DisableP1nPullDown(PIN_ALL);        //关闭P1内部下拉电阻
    EnableP1nSchmitt(PIN_ALL);          //使能P1施密特触发
    SetP1nSlewRateNormal(PIN_ALL);      //设置P1一般翻转速度
    SetP1nDrivingNormal(PIN_ALL);       //设置P1一般驱动能力
    SetP1nDigitalInput(PIN_ALL);        //使能P1数字信号输入功能

    //<<AICUBE_USER_PORT1_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT1_INITIAL_END>>
}

////////////////////////////////////////
// P2口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT2_Init(void)
{
    SetP2nInitLevelHigh(PIN_ALL);       //设置P2初始化电平
    SetP2nQuasiMode(PIN_ALL);           //设置P2为准双向口模式
    SetP2nManualMode(PIN_ALL);          //设置P2手动配置端口模式

    DisableP2nPullUp(PIN_ALL);          //关闭P2内部上拉电阻
    DisableP2nPullDown(PIN_ALL);        //关闭P2内部下拉电阻
    EnableP2nSchmitt(PIN_ALL);          //使能P2施密特触发
    SetP2nSlewRateNormal(PIN_ALL);      //设置P2一般翻转速度
    SetP2nDrivingNormal(PIN_ALL);       //设置P2一般驱动能力
    SetP2nDigitalInput(PIN_ALL);        //使能P2数字信号输入功能

    //<<AICUBE_USER_PORT2_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT2_INITIAL_END>>
}

////////////////////////////////////////
// P3口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT3_Init(void)
{
    SetP3nInitLevelHigh(PIN_ALL);       //设置P3初始化电平
    SetP3nQuasiMode(PIN_7 | PIN_6 | PIN_5 | PIN_4 | PIN_3 | PIN_2); //设置P3.7,P3.6,P3.5,P3.4,P3.3,P3.2为准双向口模式
    SetP3nHighZInputMode(PIN_1 | PIN_0); //设置P3.1,P3.0为高阻输入模式
    SetP3nManualMode(PIN_ALL);          //设置P3手动配置端口模式

    DisableP3nPullUp(PIN_ALL);          //关闭P3内部上拉电阻
    DisableP3nPullDown(PIN_ALL);        //关闭P3内部下拉电阻
    EnableP3nSchmitt(PIN_ALL);          //使能P3施密特触发
    SetP3nSlewRateNormal(PIN_ALL);      //设置P3一般翻转速度
    SetP3nDrivingNormal(PIN_ALL);       //设置P3一般驱动能力
    SetP3nDigitalInput(PIN_ALL);        //使能P3数字信号输入功能

    //<<AICUBE_USER_PORT3_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT3_INITIAL_END>>
}

////////////////////////////////////////
// P4口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT4_Init(void)
{
    SetP4nInitLevelHigh(PIN_ALL);       //设置P4初始化电平
    SetP4nQuasiMode(PIN_ALL);           //设置P4为准双向口模式
    SetP4nManualMode(PIN_ALL);          //设置P4手动配置端口模式

    DisableP4nPullUp(PIN_ALL);          //关闭P4内部上拉电阻
    DisableP4nPullDown(PIN_ALL);        //关闭P4内部下拉电阻
    EnableP4nSchmitt(PIN_ALL);          //使能P4施密特触发
    SetP4nSlewRateNormal(PIN_ALL);      //设置P4一般翻转速度
    SetP4nDrivingNormal(PIN_ALL);       //设置P4一般驱动能力
    SetP4nDigitalInput(PIN_ALL);        //使能P4数字信号输入功能

    //<<AICUBE_USER_PORT4_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT4_INITIAL_END>>
}

////////////////////////////////////////
// P5口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT5_Init(void)
{
    SetP5nInitLevelHigh(PIN_ALL);       //设置P5初始化电平
    SetP5nQuasiMode(PIN_ALL);           //设置P5为准双向口模式
    SetP5nManualMode(PIN_ALL);          //设置P5手动配置端口模式

    DisableP5nPullUp(PIN_ALL);          //关闭P5内部上拉电阻
    DisableP5nPullDown(PIN_ALL);        //关闭P5内部下拉电阻
    EnableP5nSchmitt(PIN_ALL);          //使能P5施密特触发
    SetP5nSlewRateNormal(PIN_ALL);      //设置P5一般翻转速度
    SetP5nDrivingNormal(PIN_ALL);       //设置P5一般驱动能力
    SetP5nDigitalInput(PIN_ALL);        //使能P5数字信号输入功能

    //<<AICUBE_USER_PORT5_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT5_INITIAL_END>>
}



//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码  
void FDD_IO_InitSafe(void)
{
    FDD_IO_WriteGate(0);
    FDD_IO_WriteDataIdle();
    FDD_IO_Motor(0);
    FDD_IO_Select(0);
    FDD_IO_SetDirection(0);
    FDD_IO_SetSide(0);
    FDD_IO_SetDensity(1);
    STEP = 1;
    g_fdd_track = 0;
    g_fdd_track_valid = 0;
    g_fdd_selected = 0;
    g_fdd_motor_on = 0;
    g_fdd_direction_outward = 0;
    g_fdd_side = 0;
    g_fdd_high_density = 1;
}

void FDD_IO_Select(uint8_t enable)
{
    DRVSEL = enable ? 0 : 1;
    g_fdd_selected = enable ? 1 : 0;
}

void FDD_IO_Motor(uint8_t enable)
{
    MOTOR = enable ? 0 : 1;
    g_fdd_motor_on = enable ? 1 : 0;
}

void FDD_IO_SetDirection(uint8_t outward)
{
    DIR = outward ? 1 : 0;
    g_fdd_direction_outward = outward ? 1 : 0;
}

void FDD_IO_StepPulse(void)
{
    STEP = 0;
    delay_us(FDD_STEP_PULSE_US);
    STEP = 1;
    FDD_DelayMs(FDD_STEP_SETTLE_MS);
}

void FDD_IO_SetSide(uint8_t side)
{
    SIDE = side ? 0 : 1;
    g_fdd_side = side ? 1 : 0;
}

void FDD_IO_SetDensity(uint8_t high_density)
{
    DENSEL = high_density ? 0 : 1;
    g_fdd_high_density = high_density ? 1 : 0;
}

void FDD_IO_WriteGate(uint8_t enable)
{
    WGATE = enable ? 0 : 1;
}

void FDD_IO_WriteDataIdle(void)
{
    WDATA = 1;
}

void FDD_IO_WriteDataActive(uint8_t active)
{
    WDATA = active ? 0 : 1;
}

uint8_t FDD_IO_Home(void)
{
    uint8_t i;

    FDD_IO_SetDirection(1);
    for (i = 0; i < FDD_HOME_MAX_STEPS; i++)
    {
        if (FDD_IO_IsTrack0())
        {
            g_fdd_track = 0;
            g_fdd_track_valid = 1;
            return 1;
        }
        FDD_IO_StepPulse();
    }

    if (FDD_IO_IsTrack0())
    {
        g_fdd_track = 0;
        g_fdd_track_valid = 1;
        return 1;
    }

    g_fdd_track_valid = 0;
    return 0;
}

uint8_t FDD_IO_Seek(uint8_t track)
{
    if (track > FDD_MAX_TRACK)
        return 0;

    if (!g_fdd_track_valid)
    {
        if (!FDD_IO_Home())
            return 0;
    }

    while (g_fdd_track < track)
    {
        FDD_IO_SetDirection(0);
        FDD_IO_StepPulse();
        g_fdd_track++;
    }

    while (g_fdd_track > track)
    {
        FDD_IO_SetDirection(1);
        FDD_IO_StepPulse();
        g_fdd_track--;
    }

    if ((track == 0) && !FDD_IO_IsTrack0())
    {
        g_fdd_track_valid = 0;
        return 0;
    }

    return 1;
}

void FDD_IO_InvalidateTrack(void)
{
    g_fdd_track_valid = 0;
}

uint8_t FDD_IO_GetTrack(uint8_t *track)
{
    if (!g_fdd_track_valid)
        return 0;

    *track = g_fdd_track;
    return 1;
}

uint8_t FDD_IO_IsSelected(void)
{
    return g_fdd_selected;
}

uint8_t FDD_IO_IsMotorOn(void)
{
    return g_fdd_motor_on;
}

uint8_t FDD_IO_GetDirection(void)
{
    return g_fdd_direction_outward;
}

uint8_t FDD_IO_GetSide(void)
{
    return g_fdd_side;
}

uint8_t FDD_IO_GetDensity(void)
{
    return g_fdd_high_density;
}

uint8_t FDD_IO_IsTrack0(void)
{
    return TRK0 ? 0 : 1;
}

uint8_t FDD_IO_IsWriteProtected(void)
{
    return WPT ? 0 : 1;
}

uint8_t FDD_IO_IsDiskChanged(void)
{
    return DSKCHG ? 0 : 1;
}

uint8_t FDD_IO_ReadIndex(void)
{
    return FDD_INDEX ? 0 : 1;
}

uint8_t FDD_IO_ReadData(void)
{
    return RDATA ? 0 : 1;
}
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


