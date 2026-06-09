//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: usblib.c
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
static void FDD_USB_SendText(const char *text);
static uint8_t FDD_USB_CmdEq(const char *cmd, const char *word);
static uint8_t FDD_USB_CmdStarts(const char *cmd, const char *word);
static uint8_t FDD_USB_ParseU8(const char *text, uint8_t *value);
static uint8_t FDD_USB_ParseTwoU8(const char *text, uint8_t *first, uint8_t *second);
static void FDD_USB_SendStatus(void);
static void FDD_USB_SendFluxInfo(void);
static void FDD_USB_SendFluxStats(void);
static void FDD_USB_SendFlux(void);
static void FDD_USB_SendFluxDrain(void);
static void FDD_USB_SendFluxPeek(void);
static void FDD_USB_SendFluxAscii(void);
static void FDD_USB_SendFluxPeekAscii(void);
static void FDD_USB_SendFluxDrainAscii(void);
static void FDD_USB_SendFluxAsciiSamples(const char *tag, uint16_t count);
static void FDD_USB_SendRpm(void);
static void FDD_USB_ReadyDrive(void);
static void FDD_USB_MotorOff(void);
static void FDD_USB_DriveSelectOn(void);
static void FDD_USB_DriveSelectOff(void);
static void FDD_USB_CaptureRev(void);
static void FDD_USB_SendHelp(void);
static char g_usb_cmd[32];
static char g_usb_text_resp[220];
static uint16_t g_usb_flux_samples[31];
static uint8_t g_usb_flux_resp[2 + 31 * 2];
//<<AICUBE_USER_GLOBAL_DEFINE_END>>



////////////////////////////////////////
// USB库初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void USBLIB_Init(void)
{
    usb_init();                         //初始化USB模块
    USB_SetIntPriority(0);              //设置中断为最低优先级
    set_usb_ispcmd("@STCISP#");         //设置USB不停电下载命令

    //<<AICUBE_USER_USBLIB_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_USBLIB_INITIAL_END>>
}

////////////////////////////////////////
// 等待USB配置完成函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void USBLIB_WaitConfiged(void)
{
    while (DeviceState != DEVSTATE_CONFIGURED) //等待USB完成配置
        WDT_Clear();                    //清看门狗定时器 (防止硬件自动使能看门狗)
}

////////////////////////////////////////
// USB设备接收数据处理程序
// 入口参数: 无
// 函数返回: 无
// bUsbOutReady：USB设备接收数据标志位
// OutNumber：USB设备接收到的数据长度
// UsbOutBuffer：保存USB设备接收到的数据
////////////////////////////////////////
void USBLIB_OUT_Done(void)
{
    if (bUsbOutReady)                   //查询是否有接收到USB主机发送数据
    {
        //<<AICUBE_USER_USBLIB_ISR_CODE1_BEGIN>>
        // 在此添加中断函数用户代码  
        FDD_USB_ProcessPacket(UsbOutBuffer, OutNumber);
        //<<AICUBE_USER_USBLIB_ISR_CODE1_END>>
        usb_OUT_done();                 //当前包的数据处理完成,通知USB主机可以发送下一包数据
    }
}



//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码  
static void FDD_USB_SendText(const char *text)
{
    USB_SendData((uint8_t *)text, strlen(text));
}

static uint8_t FDD_USB_CmdEq(const char *cmd, const char *word)
{
    while ((*cmd == ' ') || (*cmd == '\t'))
        cmd++;

    while (*word)
    {
        if (*cmd != *word)
            return 0;
        cmd++;
        word++;
    }

    return ((*cmd == 0) || (*cmd == '\r') || (*cmd == '\n') || (*cmd == ' ') || (*cmd == '\t'));
}

static uint8_t FDD_USB_CmdStarts(const char *cmd, const char *word)
{
    while ((*cmd == ' ') || (*cmd == '\t'))
        cmd++;

    while (*word)
    {
        if (*cmd != *word)
            return 0;
        cmd++;
        word++;
    }

    return 1;
}

static uint8_t FDD_USB_ParseU8(const char *text, uint8_t *value)
{
    uint16_t v;
    uint8_t found;

    v = 0;
    found = 0;
    while ((*text == ' ') || (*text == '\t'))
        text++;

    while ((*text >= '0') && (*text <= '9'))
    {
        found = 1;
        v = (uint16_t)(v * 10 + (*text - '0'));
        if (v > 255)
            return 0;
        text++;
    }

    if (!found)
        return 0;

    *value = (uint8_t)v;
    return 1;
}

static uint8_t FDD_USB_ParseTwoU8(const char *text, uint8_t *first, uint8_t *second)
{
    while ((*text == ' ') || (*text == '\t'))
        text++;

    if (!FDD_USB_ParseU8(text, first))
        return 0;

    while ((*text >= '0') && (*text <= '9'))
        text++;
    while ((*text == ' ') || (*text == '\t') || (*text == ','))
        text++;

    return FDD_USB_ParseU8(text, second);
}

static void FDD_USB_SendStatus(void)
{
    uint8_t track;
    uint32_t period_ms;
    uint16_t rpm;

    if (!FDD_IO_GetTrack(&track))
        track = 255;
    period_ms = FDD_Index_GetPeriodMs();
    rpm = period_ms ? (uint16_t)(60000UL / period_ms) : 0;

    sprintf(g_usb_text_resp,
            "STAT T=%u SEL=%u MOT=%u DIR=%u SIDE=%u DEN=%u T0=%u WP=%u DC=%u IDX=%u IP=%lu RPM=%u CAP=%u FLUX=%u OVF=%u MS=%lu\r\n",
            track,
            FDD_IO_IsSelected(),
            FDD_IO_IsMotorOn(),
            FDD_IO_GetDirection(),
            FDD_IO_GetSide(),
            FDD_IO_GetDensity(),
            FDD_IO_IsTrack0(),
            FDD_IO_IsWriteProtected(),
            FDD_IO_IsDiskChanged(),
            g_index_count,
            period_ms,
            rpm,
            FDD_Flux_IsCapturing(),
            FDD_Flux_Available(),
            FDD_Flux_GetOverflowCount(),
            FDD_GetMillis());
    FDD_USB_SendText(g_usb_text_resp);
}

static void FDD_USB_SendFluxInfo(void)
{
    sprintf(g_usb_text_resp,
            "FLUX_INFO CAP=%u AVAIL=%u OVF=%u IP=%lu\r\n",
            FDD_Flux_IsCapturing(),
            FDD_Flux_Available(),
            FDD_Flux_GetOverflowCount(),
            FDD_Index_GetPeriodMs());
    FDD_USB_SendText(g_usb_text_resp);
}

static void FDD_USB_SendFluxStats(void)
{
    uint16_t count;
    uint16_t min_value;
    uint16_t max_value;
    uint16_t avg_value;

    if (!FDD_Flux_GetStats(&count, &min_value, &max_value, &avg_value))
    {
        FDD_USB_SendText("ERR FLUX_STATS\r\n");
        return;
    }

    sprintf(g_usb_text_resp,
            "FLUX_STATS N=%u MIN=%u MAX=%u AVG=%u OVF=%u\r\n",
            count,
            min_value,
            max_value,
            avg_value,
            FDD_Flux_GetOverflowCount());
    FDD_USB_SendText(g_usb_text_resp);
}

static void FDD_USB_SendRpm(void)
{
    uint32_t period_ms;
    uint16_t rpm;

    period_ms = FDD_Index_GetPeriodMs();
    rpm = period_ms ? (uint16_t)(60000UL / period_ms) : 0;
    sprintf(g_usb_text_resp, "RPM %u PERIOD %lu\r\n", rpm, period_ms);
    FDD_USB_SendText(g_usb_text_resp);
}

static void FDD_USB_ReadyDrive(void)
{
    uint32_t period_ms;
    uint16_t rpm;

    FDD_IO_Select(1);
    FDD_IO_Motor(1);
    FDD_Index_Reset();

    if (!FDD_Index_Wait(FDD_READY_TIMEOUT_MS))
    {
        FDD_USB_SendText("ERR READY NO_INDEX\r\n");
        return;
    }

    FDD_Index_Wait(FDD_READY_TIMEOUT_MS);
    period_ms = FDD_Index_GetPeriodMs();
    rpm = period_ms ? (uint16_t)(60000UL / period_ms) : 0;

    sprintf(g_usb_text_resp, "OK READY RPM=%u IP=%lu\r\n", rpm, period_ms);
    FDD_USB_SendText(g_usb_text_resp);
}

static void FDD_USB_MotorOff(void)
{
    FDD_Flux_Stop();
    FDD_Flux_Reset();
    FDD_IO_WriteGate(0);
    FDD_IO_WriteDataIdle();
    FDD_IO_Select(0);
    FDD_IO_Motor(0);
    FDD_USB_SendText("OK MOTOR_OFF\r\n");
}

static void FDD_USB_DriveSelectOn(void)
{
    FDD_IO_Select(1);
    FDD_USB_SendText("OK DRIVE_SELECT_ON\r\n");
}

static void FDD_USB_DriveSelectOff(void)
{
    FDD_Flux_Stop();
    FDD_IO_WriteGate(0);
    FDD_IO_WriteDataIdle();
    FDD_IO_Select(0);
    FDD_USB_SendText("OK DRIVE_SELECT_OFF\r\n");
}

static void FDD_USB_SendHelp(void)
{
    FDD_USB_SendText("CMDS PING HELP SAFE STATUS READY MOTOR_OFF DRIVE_SELECT_ON DRIVE_SELECT_OFF MOTOR SELECT DIR STEP HOME SEEK TRACK_INVALIDATE SIDE DENSEL INDEX_RESET INDEX_WAIT RPM FLUX_RESET FLUX_CLEAR FLUX_START FLUX_STOP FLUX_INFO FLUX_STATS CAPTURE_REV CAPTURE_NEXT CAPTURE_TRACK CAPTURE_TS FLUX_READ FLUX_DRAIN FLUX_PEEK FLUX_READ_ASCII FLUX_PEEK_ASCII FLUX_DRAIN_ASCII\r\n");
}

static void FDD_USB_SendFlux(void)
{
    uint8_t i;
    uint8_t count;

    count = (uint8_t)FDD_Flux_Read(g_usb_flux_samples, 31);
    g_usb_flux_resp[0] = 'F';
    g_usb_flux_resp[1] = count;

    for (i = 0; i < count; i++)
    {
        g_usb_flux_resp[2 + i * 2] = (uint8_t)(g_usb_flux_samples[i] & 0xff);
        g_usb_flux_resp[3 + i * 2] = (uint8_t)(g_usb_flux_samples[i] >> 8);
    }

    USB_SendData(g_usb_flux_resp, 2 + count * 2);
}

static void FDD_USB_SendFluxDrain(void)
{
    uint8_t i;
    uint8_t count;
    uint16_t remaining;

    count = (uint8_t)FDD_Flux_Read(g_usb_flux_samples, 30);
    remaining = FDD_Flux_Available();
    g_usb_flux_resp[0] = 'D';
    g_usb_flux_resp[1] = count;
    g_usb_flux_resp[2] = (uint8_t)(remaining & 0xff);
    g_usb_flux_resp[3] = (uint8_t)(remaining >> 8);

    for (i = 0; i < count; i++)
    {
        g_usb_flux_resp[4 + i * 2] = (uint8_t)(g_usb_flux_samples[i] & 0xff);
        g_usb_flux_resp[5 + i * 2] = (uint8_t)(g_usb_flux_samples[i] >> 8);
    }

    USB_SendData(g_usb_flux_resp, 4 + count * 2);
}

static void FDD_USB_SendFluxPeek(void)
{
    uint8_t i;
    uint8_t count;

    count = (uint8_t)FDD_Flux_Peek(g_usb_flux_samples, 31);
    g_usb_flux_resp[0] = 'P';
    g_usb_flux_resp[1] = count;

    for (i = 0; i < count; i++)
    {
        g_usb_flux_resp[2 + i * 2] = (uint8_t)(g_usb_flux_samples[i] & 0xff);
        g_usb_flux_resp[3 + i * 2] = (uint8_t)(g_usb_flux_samples[i] >> 8);
    }

    USB_SendData(g_usb_flux_resp, 2 + count * 2);
}

static void FDD_USB_SendFluxAsciiSamples(const char *tag, uint16_t count)
{
    uint8_t i;

    sprintf(g_usb_text_resp, "%s N=%u", tag, count);
    FDD_USB_SendText(g_usb_text_resp);

    for (i = 0; i < count; i++)
    {
        sprintf(g_usb_text_resp, " %u", g_usb_flux_samples[i]);
        FDD_USB_SendText(g_usb_text_resp);
    }

    FDD_USB_SendText("\r\n");
}

static void FDD_USB_SendFluxAscii(void)
{
    uint8_t count;

    count = (uint8_t)FDD_Flux_Read(g_usb_flux_samples, 8);
    FDD_USB_SendFluxAsciiSamples("FLUX_ASCII", count);
}

static void FDD_USB_SendFluxPeekAscii(void)
{
    uint8_t count;

    count = (uint8_t)FDD_Flux_Peek(g_usb_flux_samples, 8);
    FDD_USB_SendFluxAsciiSamples("FLUX_PEEK", count);
}

static void FDD_USB_SendFluxDrainAscii(void)
{
    uint8_t i;
    uint8_t count;
    uint16_t remaining;

    count = (uint8_t)FDD_Flux_Read(g_usb_flux_samples, 8);
    remaining = FDD_Flux_Available();
    sprintf(g_usb_text_resp, "FLUX_DRAIN N=%u REM=%u", count, remaining);
    FDD_USB_SendText(g_usb_text_resp);

    for (i = 0; i < count; i++)
    {
        sprintf(g_usb_text_resp, " %u", g_usb_flux_samples[i]);
        FDD_USB_SendText(g_usb_text_resp);
    }

    FDD_USB_SendText("\r\n");
}

static void FDD_USB_CaptureRev(void)
{
    FDD_Flux_Stop();
    FDD_Flux_Reset();
    FDD_Index_Reset();

    if (!FDD_Index_Wait(FDD_INDEX_TIMEOUT_MS))
    {
        FDD_USB_SendText("ERR CAPTURE NO_INDEX\r\n");
        return;
    }

    FDD_Flux_Start();
    if (!FDD_Index_Wait(FDD_CAPTURE_TIMEOUT_MS))
    {
        FDD_Flux_Stop();
        FDD_USB_SendText("ERR CAPTURE TIMEOUT\r\n");
        return;
    }
    FDD_Flux_Stop();

    sprintf(g_usb_text_resp,
            "OK CAPTURE FLUX=%u OVF=%u IP=%lu\r\n",
            FDD_Flux_Available(),
            FDD_Flux_GetOverflowCount(),
            FDD_Index_GetPeriodMs());
    FDD_USB_SendText(g_usb_text_resp);
}

void FDD_USB_ProcessPacket(uint8_t *buf, uint16_t len)
{
    uint8_t i;
    uint8_t value;
    uint8_t value2;

    if (len >= sizeof(g_usb_cmd))
        len = sizeof(g_usb_cmd) - 1;

    for (i = 0; i < len; i++)
    {
        if ((buf[i] >= 'a') && (buf[i] <= 'z'))
            g_usb_cmd[i] = buf[i] - 32;
        else
            g_usb_cmd[i] = buf[i];
    }
    g_usb_cmd[len] = 0;

    if (FDD_USB_CmdEq(g_usb_cmd, "PING"))
    {
        FDD_USB_SendText("PONG " FDD_FW_VERSION "\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "HELP"))
    {
        FDD_USB_SendHelp();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "SAFE"))
    {
        FDD_IO_InitSafe();
        FDD_USB_SendText("OK SAFE\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "READY"))
    {
        FDD_USB_ReadyDrive();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "MOTOR_OFF"))
    {
        FDD_USB_MotorOff();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "DRIVE_SELECT_ON"))
    {
        FDD_USB_DriveSelectOn();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "DRIVE_SELECT_OFF"))
    {
        FDD_USB_DriveSelectOff();
    }
    else if (FDD_USB_CmdStarts(g_usb_cmd, "MOTOR "))
    {
        FDD_IO_Motor(g_usb_cmd[6] == '1');
        FDD_USB_SendText("OK MOTOR\r\n");
    }
    else if (FDD_USB_CmdStarts(g_usb_cmd, "SELECT "))
    {
        FDD_IO_Select(g_usb_cmd[7] == '1');
        FDD_USB_SendText("OK SELECT\r\n");
    }
    else if (FDD_USB_CmdStarts(g_usb_cmd, "DIR "))
    {
        FDD_IO_SetDirection(g_usb_cmd[4] == '1');
        FDD_USB_SendText("OK DIR\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "STEP"))
    {
        FDD_IO_StepPulse();
        FDD_USB_SendText("OK STEP\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "HOME"))
    {
        if (FDD_IO_Home())
            FDD_USB_SendText("OK HOME\r\n");
        else
            FDD_USB_SendText("ERR HOME\r\n");
    }
    else if (FDD_USB_CmdStarts(g_usb_cmd, "SEEK "))
    {
        if (FDD_USB_ParseU8(&g_usb_cmd[5], &value) && FDD_IO_Seek(value))
            FDD_USB_SendText("OK SEEK\r\n");
        else
            FDD_USB_SendText("ERR SEEK\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "TRACK_INVALIDATE"))
    {
        FDD_IO_InvalidateTrack();
        FDD_USB_SendText("OK TRACK_INVALIDATE\r\n");
    }
    else if (FDD_USB_CmdStarts(g_usb_cmd, "SIDE "))
    {
        FDD_IO_SetSide(g_usb_cmd[5] == '1');
        FDD_USB_SendText("OK SIDE\r\n");
    }
    else if (FDD_USB_CmdStarts(g_usb_cmd, "DENSEL "))
    {
        FDD_IO_SetDensity(g_usb_cmd[7] == '1');
        FDD_USB_SendText("OK DENSEL\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "STATUS"))
    {
        FDD_USB_SendStatus();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "INDEX_RESET"))
    {
        FDD_Index_Reset();
        FDD_USB_SendText("OK INDEX_RESET\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "INDEX_WAIT"))
    {
        if (FDD_Index_Wait(FDD_INDEX_TIMEOUT_MS))
            FDD_USB_SendText("OK INDEX_WAIT\r\n");
        else
            FDD_USB_SendText("ERR INDEX_WAIT\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "RPM"))
    {
        FDD_USB_SendRpm();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_RESET"))
    {
        FDD_Flux_Reset();
        FDD_USB_SendText("OK FLUX_RESET\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_CLEAR"))
    {
        FDD_Flux_Reset();
        FDD_USB_SendText("OK FLUX_CLEAR\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_INFO"))
    {
        FDD_USB_SendFluxInfo();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_STATS"))
    {
        FDD_USB_SendFluxStats();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_START"))
    {
        FDD_Flux_Reset();
        FDD_Flux_Start();
        FDD_USB_SendText("OK FLUX_START\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_STOP"))
    {
        FDD_Flux_Stop();
        FDD_USB_SendText("OK FLUX_STOP\r\n");
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "CAPTURE_REV"))
    {
        FDD_USB_CaptureRev();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "CAPTURE_NEXT"))
    {
        FDD_USB_CaptureRev();
    }
    else if (FDD_USB_CmdStarts(g_usb_cmd, "CAPTURE_TRACK "))
    {
        if (FDD_USB_ParseU8(&g_usb_cmd[14], &value) && FDD_IO_Seek(value))
            FDD_USB_CaptureRev();
        else
            FDD_USB_SendText("ERR CAPTURE_TRACK\r\n");
    }
    else if (FDD_USB_CmdStarts(g_usb_cmd, "CAPTURE_TS "))
    {
        if (FDD_USB_ParseTwoU8(&g_usb_cmd[11], &value, &value2) && (value2 <= 1) && FDD_IO_Seek(value))
        {
            FDD_IO_SetSide(value2);
            FDD_USB_CaptureRev();
        }
        else
        {
            FDD_USB_SendText("ERR CAPTURE_TS\r\n");
        }
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_READ"))
    {
        FDD_USB_SendFlux();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_DRAIN"))
    {
        FDD_USB_SendFluxDrain();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_PEEK"))
    {
        FDD_USB_SendFluxPeek();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_READ_ASCII"))
    {
        FDD_USB_SendFluxAscii();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_PEEK_ASCII"))
    {
        FDD_USB_SendFluxPeekAscii();
    }
    else if (FDD_USB_CmdEq(g_usb_cmd, "FLUX_DRAIN_ASCII"))
    {
        FDD_USB_SendFluxDrainAscii();
    }
    else
    {
        FDD_USB_SendText("ERR CMD\r\n");
    }
}
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


