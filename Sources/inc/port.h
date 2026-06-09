//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: port.h
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#ifndef __PORT_H__
#define __PORT_H__


//<<AICUBE_USER_DEFINE_BEGIN>>
// 在此添加用户宏定义  
//<<AICUBE_USER_DEFINE_END>>



void PORT0_Init(void);
void PORT1_Init(void);
void PORT2_Init(void);
void PORT3_Init(void);
void PORT4_Init(void);
void PORT5_Init(void);



//<<AICUBE_USER_EXTERNAL_DECLARE_BEGIN>>
// 在此添加用户外部函数和外部变量声明
void FDD_IO_InitSafe(void);
void FDD_IO_Select(uint8_t enable);
void FDD_IO_Motor(uint8_t enable);
void FDD_IO_SetDirection(uint8_t outward);
void FDD_IO_StepPulse(void);
void FDD_IO_SetSide(uint8_t side);
void FDD_IO_SetDensity(uint8_t high_density);
void FDD_IO_WriteGate(uint8_t enable);
void FDD_IO_WriteDataIdle(void);
uint8_t FDD_IO_Home(void);
uint8_t FDD_IO_Seek(uint8_t track);
uint8_t FDD_IO_GetTrack(uint8_t *track);
uint8_t FDD_IO_IsSelected(void);
uint8_t FDD_IO_IsMotorOn(void);
uint8_t FDD_IO_GetDirection(void);
uint8_t FDD_IO_GetSide(void);
uint8_t FDD_IO_GetDensity(void);
uint8_t FDD_IO_IsTrack0(void);
uint8_t FDD_IO_IsWriteProtected(void);
uint8_t FDD_IO_IsDiskChanged(void);
uint8_t FDD_IO_ReadIndex(void);
uint8_t FDD_IO_ReadData(void);
//<<AICUBE_USER_EXTERNAL_DECLARE_END>>


#endif
