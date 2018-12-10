/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_dma.h
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : This file contains all the functions prototypes for the
*                      DMA firmware library.
********************************************************************************

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASKSCHEDULE_H
#define __TASKSCHEDULE_H


//*********************��ʱʱ�䶨��*****************
#define SYSTICK_TICKS          25//������Ϣɨ������25ms
#define SYSTICK_0p1SECOND       4//100ms
#define SYSTICK_0p2SECOND       8//200ms  //zengliang add 
#define SYSTICK_0p5SECOND       20//500ms
#define SYSTICK_1SECOND         40	////1s
#define SYSTICK_10SECOND	    (400)//(10*SYSTICK_1SECOND)	//10s
#define SYSTICK_15SECOND	    (600)//(15*SYSTICK_1SECOND)	//15s
#define SYSTICK_20SECOND	    (800)//(10*SYSTICK_1SECOND)	//20s
#define SYSTICK_1MINUTE		    (2400)//(60*SYSTICK_1SECOND)	//1����
#define SYSTICK_10MINUTE	    (24000)//(600*SYSTICK_1SECOND)	//10����
#define SYSTICK_30MINUTE	    (72000)//(600*SYSTICK_1SECOND)	//30����
#define SYSTICK_1HOUR		    (144000)//(3600*SYSTICK_1SECOND)	//1Сʱ
#define	SECOND	SYSTICK_1SECOND



//*****************��Ϣ**************************
typedef enum
{
	Msg_Gps_TimeTask=0,					//1:GPS���ݴ���, 
	Msg_GpsMile_TimeTask,				//2:GPS��̼���,
	Heartbeat_TimeTask,					//����
	Msg_Sleep_TimeTask,					//5:����
	Msg_Link1Login_TimeTask,			//6:�ն�ע���Ȩ	 
	Msg_Io_TimeTask,					//7:IO ״̬���
	Msg_RadioProtocolResend_TimeTask, 	//8:����Э���ط�		
	Msg_DelayTrigTimeTask,				//9:��ʱ����
	Msg_SpeedMonitor_TimeTask,			//11:���ټ��,
	Msg_AreaManage_TimeTask,			//12:����
	Msg_ControlOil_TimeTask,			//13:����·
	Msg_SleepDeep_TimeTask,				//14:�������
	Msg_Polygon_TimeTask, 				//15:�����   
	Msg_Corner_TimeTask,				//17:�յ㲹��
}FUNCTION_MSG;

//******************ʱ������***********************
typedef enum 
{
    TIME_GPSREC=0,                    //1:GPS���ݴ��� 
    TIME_GPS_MILE,                  //2:GPS��̼���
    TIME_HEARTBEAT,                 //3:����
    TIME_SLEEP,                     //5:����
    TIME_LINK1_LOGIN,               //6:�ն�ע���Ȩ    
    TIME_IO_MONITOR,                //7:IO ״̬���
    TIME_RADIO_RESEND,              //8:����Э���ط������꣩
    TIME_DELAY_TRIG,                //9:��ʱ����
    TIME_SPEED_MONITOR,             //11:���ټ��   
    TIME_AREA,                      //12:����   
    TIME_OIL_CTRL,                  //13:����· 
    TIME_SLEEPDEEP,                 //14:�������
    TIME_POLYGON,                   //15:�����
    TIME_CORNER,                    //17:�յ㲹��
    MAX_TIMETASK                    //20
}TIMERTASK;



//*****************�¼�����**************************
typedef enum 
{
    MSG_Gps_EvTask=0,                       //2:GPS���ݴ���
    MSG_Report_EvTask,                          //3:�ϱ�һ��λ����Ϣ
    MSG_GetAkey_EvTask,              //4: ��ȡ��Ȩ������
    MSG_FtpUpdata_EvTask,                      //6: FTP��������
    MSG_MAX_EVENTASK

}EVENTTASK_MSG;

/*------------------------ Execute task by timer--------------------------------
Max task is 32. when power on dev this task is init.
Include 
------------------------------------------------------------------------------*/
#pragma pack(1)
typedef struct{
  FunctionalState TaskTimerState;       //task valid,1:valid;0:invalid
  u32 TaskTimerCount;   		//variables task time count
  u32 TaskExTimer;      		//set fixed param by independently funtion module
  FUNCTION_MSG operate;     //handle independtly funtion
}TIME_TASK;
#pragma pack()
/////
#pragma pack(1)
typedef struct{
  FunctionalState evTaskOnState;       //task valid,1:valid;0:invalid
  EVENTTASK_MSG operate;     //handle independtly funtion
}EVEN_TASK;
#pragma pack()
/* Private define ------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : InitTimerTask
* Description    : Initialize Schedule Timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InitTimerTask(void);

/*******************************************************************************
* Function Name  : TimerTaskScheduler
* Description    : Schedule Timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TimerTaskScheduler(void);

/*******************************************************************************
* Function Name  : SetTimerTask
* Description    : Setup timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetTimerTask(TIMERTASK task, u32 time);

/*******************************************************************************
* Function Name  : ClrTimerTask
* Description    : Cancel timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ClrTimerTask(TIMERTASK task);
/*******************************************************************************
* Function Name  : GetTimerTaskTimerState
* Description    : 
* Input          : None
* Output         : None
* Return         : TaskTimerState
*******************************************************************************/
FunctionalState GetTimerTaskTimerState(TIMERTASK task);
/*******************************************************************************
* Function Name  : InitEvTask
* Description    : Initialize Schedule Timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InitEvTask(void);
/*******************************************************************************
* Function Name  : SetEvTask
* Description    : Setup even task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetEvTask(u8 ev);
/*******************************************************************************
* Function Name  : EvTaskScheduler
* Description    : Schedule even task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EvTaskScheduler(void);

//����ʱ�䶨ʱ����ʼ��
void init_TaskScheduleTimer(void);
//�ر�ϵͳ����ʱ��
void Stop_TaskScheduleTimer(void);

#endif
/*******************************************************************************
 *                             end of module
 *******************************************************************************/


