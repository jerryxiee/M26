/********************************************************************
//��Ȩ˵��	:Shenzhen E-EYE Co. Ltd., Copyright 2009- All Rights Reserved.
//�ļ�����	:include.h		
//����		:
//�汾��	:
//������	:
//����ʱ��	:
//�޸���	:
//�޸�ʱ��	:
//�޸ļ�Ҫ˵��	:
//��ע		:
***********************************************************************/
#ifndef __INCLUDE__H_
#define __INCLUDE__H_
////////////////////////
#include "string.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_fs.h"
#include "ql_error.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "ql_timer.h"
#include "ql_adc.h"
#include "ql_time.h"
#include "ql_eint.h"
#include "ql_power.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_network.h"
#include "ril_location.h"
#include "ril_telephony.h"
#include "ril_system.h "
#include "ril_location.h "
#include "ril_alarm.h"


#include <stdio.h>
#include "custom_user_cfg.h"
#include "watchdog.h"

#include "version.h"
#include "FilePram.h"
#include "public.h"
#include "E2ParamApp.h"
#include "ComParam.h"

#include "RadioProtocol.h"
#include "sysctrl.h"
#include "tcp_log.h"
//#include "gps_drive.h"
#include "io.h"
#include "GPIOControl.h"
//#include "gps_app.h"
#include "time.h"
//#include "MileMeter.h"
#include "Taskschedule.h"
#include "Adc.h"
//#include "Corner.h"
#include "SystemInit.h"
//#include "OilControl.h"
#include "Blind.h"
#include "BlindFile.h"
//#include "bma250.h"
#include "UpdateFirmware.h"
#include "SleepCtr.h"
#include "Sms.h"
#include "ledctr.h"
//#include "AreaManage.h"

#define PUBLIC_CHECKBIT(X,N) ((X) & (1ul << (N)))	   //����ĳһλ
#define PUBLIC_SETBIT(X,N) (X)=((X) | (1ul<<(N)))       //����ĳһλ
#define PUBLIC_CLRBIT(X,N) (X)=((X) & (~(1ul<<(N))))    //���ĳһλ


#define DEBUG_ENABLE 1
#if DEBUG_ENABLE > 0
#define DEBUG_PORT  UART_PORT1
#define DBG_BUF_LEN   512
extern char DBG_BUFFER[];
#define APP_DEBUG(FORMAT,...) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
    Ql_sprintf(DBG_BUFFER,FORMAT,##__VA_ARGS__); \
    if (UART_PORT2 == (DEBUG_PORT)) \
    {\
        Ql_Debug_Trace(DBG_BUFFER);\
    } else {\
        Ql_UART_Write((Enum_SerialPort)(DEBUG_PORT), (u8*)(DBG_BUFFER), Ql_strlen((const char *)(DBG_BUFFER)));\
    }\
}
#else
#define APP_DEBUG(FORMAT,...) 
#endif

/////////////////////////
#endif
/******************************************************************************
**                            End Of File
******************************************************************************/

