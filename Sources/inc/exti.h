//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: exti.h
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#ifndef __EXTI_H__
#define __EXTI_H__


//<<AICUBE_USER_DEFINE_BEGIN>>
// 在此添加用户宏定义  
//<<AICUBE_USER_DEFINE_END>>



void EXTI0_Init(void);
void EXTI1_Init(void);



//<<AICUBE_USER_EXTERNAL_DECLARE_BEGIN>>
// 在此添加用户外部函数和外部变量声明
extern volatile uint16_t g_index_count;
extern volatile uint32_t g_index_period_ms;

void FDD_Flux_Reset(void);
void FDD_Flux_Start(void);
void FDD_Flux_Stop(void);
uint8_t FDD_Flux_IsCapturing(void);
uint16_t FDD_Flux_Available(void);
uint16_t FDD_Flux_GetOverflowCount(void);
uint8_t FDD_Flux_GetStats(uint16_t *count, uint16_t *min_value, uint16_t *max_value, uint16_t *avg_value);
uint16_t FDD_Flux_Peek(uint16_t *dst, uint16_t max_count);
uint16_t FDD_Flux_Read(uint16_t *dst, uint16_t max_count);
void FDD_Index_Reset(void);
uint8_t FDD_Index_Wait(uint16_t timeout_ms);
uint32_t FDD_Index_GetPeriodMs(void);
//<<AICUBE_USER_EXTERNAL_DECLARE_END>>


#endif
