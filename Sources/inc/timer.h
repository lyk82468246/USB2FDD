//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: timer.h
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#ifndef __TIMER_H__
#define __TIMER_H__


//<<AICUBE_USER_DEFINE_BEGIN>>
// 在此添加用户宏定义  
//<<AICUBE_USER_DEFINE_END>>



void TIMER0_Init(void);
void TIMER11_Init(void);



//<<AICUBE_USER_EXTERNAL_DECLARE_BEGIN>>
// 在此添加用户外部函数和外部变量声明
extern volatile uint32_t g_ms_ticks;

uint16_t FDD_Timer0_Read16(void);
uint32_t FDD_GetMillis(void);
void FDD_DelayMs(uint16_t ms);
//<<AICUBE_USER_EXTERNAL_DECLARE_END>>


#endif
