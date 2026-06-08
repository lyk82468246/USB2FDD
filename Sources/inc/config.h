//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: config.h
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#ifndef __CONFIG_H__
#define __CONFIG_H__


//<<AICUBE_USER_DEFINE_BEGIN>>
// 在此添加用户宏定义
#define FDD_FW_VERSION          "FDD-0.1"
#define FDD_FLUX_BUFFER_SIZE    4096
#define FDD_STEP_PULSE_US       6
#define FDD_STEP_SETTLE_MS      4
#define FDD_HOME_MAX_STEPS      90
#define FDD_MAX_TRACK           83
#define FDD_INDEX_TIMEOUT_MS    300
#define FDD_CAPTURE_TIMEOUT_MS  500
//<<AICUBE_USER_DEFINE_END>>


#define __ENCODING              "UTF-8" //DO NOT DELETE or MODIFY


#define HIRC                    40000000UL
#define FOSC                    40000000UL
#define SYSCLK                  FOSC
#define MAIN_Fosc               FOSC

#include <ai8051u.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <intrins.h>

#include "def.h"
#include "ai8051u_def.h"
#include "ai_usb.h"


//<<AICUBE_USER_INCLUDE_BEGIN>>
// 在此添加用户头文件包含  
//<<AICUBE_USER_INCLUDE_END>>


#define DSKCHG                  P16
#define WPT                     P17
#define RDATA                   P32
#define FDD_INDEX               P33
#define TRK0                    P20
#define DENSEL                  P21
#define WDATA                   P22
#define WGATE                   P23
#define MOTOR                   P24
#define DRVSEL                  P50
#define DIR                     P25
#define STEP                    P26
#define SIDE                    P27


#include "port.h"
#include "timer.h"
#include "exti.h"
#include "usblib.h"

void SYS_Init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);



//<<AICUBE_USER_EXTERNAL_DECLARE_BEGIN>>
// 在此添加用户外部函数和外部变量声明  
//<<AICUBE_USER_EXTERNAL_DECLARE_END>>


#endif
