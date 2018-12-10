#include "include.h"
#include <stdio.h> 
#include "Report.h"
#include "Public.h"
//#include "Gps_App.h"
#include "RadioProtocol.h"
#include "GPIOControl.h"
#include "Io.h"
#include "Blind.h"
#include "Ril_cell.h"
#include "ql_memory.h"

extern u32 battery_capacity;
extern u32 battery_voltage;
extern s32 m_pwrOnReason;

//#define FLASH_BLIND_STEP_LEN                124

/********************���ر���*************************/
static u32 Link1ReportTime = 0;//����1λ�û㱨ʱ��������λ��
static u32 Link1ReportTimeCount = 0;//����1λ�û㱨ʱ�����
//static u32 Link1LastReportDistance = 0;//����1��һ���ϱ���ľ��룬��λ��
static u32 Link1ReportDistance = 0;//����1��һ���ϱ���ľ���
static u8 Link1ReportAckFlag = 0;//����1��һ��λ����ϢӦ���־��1��ʾ��Ӧ��0��ʾ��Ӧ��
static u8 Link1ReportBuffer[FLASH_BLIND_STEP_LEN] = {0};//����1λ���ϱ�����
static u8 Link1ReportBufferLen = 0;//����1λ���ϱ��������ݳ���
//static u32 ReportStrategy = 0;//�㱨����
//static u32 ReportScheme = 0;//�㱨����
//static u32 EmergencyReportTimeSpace = 0;//���������㱨ʱ����
//static u32 EmergencyReportDistanceSpace = 0;//���������㱨������
static u32 OverSpeedReportTimeSpace = 0;//���ٱ����㱨ʱ����
static u32 OverTimeReportTimeSpace = 0;//��ʱ�����㱨ʱ����
static u32 AccOnReportTimeSpace = 0;//ȱʡ�㱨ʱ����
//static u32 AccOnReportDistanceSpace = 0;//ȱʡ�㱨ʱ����
static u32 AccOffReportTimeSpace = 0;//���߻㱨ʱ����
//static u32 AccOffReportDistanceSpace = 0;//���߻㱨ʱ����
//static u32 UnloginReportTimeSpace = 0;//��ʻԱδ��¼�㱨ʱ����
//static u32 UnloginReportDistanceSpace = 0;//��ʻԱδ��¼�㱨ʱ����
static u32 TempTrackTimeSpace = 0;//��ʱ����ʱ����
static u32 TempTrackCount = 0;//��ʱ���ٴ���
static u32 ReportEventSwitch = 0;//λ���ϱ��¼���أ�bit0:Ϊ1��ʾ�������ٱ���λ���ϱ���bit1:Ϊ1��ʾ������ʱ����λ���ϱ�
u16 DelayAckCommand = 0;//��ʱӦ������
u8  DelayBuffer[300];//��ʱ���壬�ɰ�Ҫ��ʱ���͵����ݿ��������ע����󳤶�����
u8  DelayBufferLen;//��ʱ�������ݳ���

static u32 RTCWakeUpTimeOffset = 0;//RTC�����ϱ�ʱ��ƫ��

static ReportStatus_TypeDef	ReportStatus = ReportStatus_Off;

/********************ȫ�ֱ���*************************/
u16 Link1LastReportSerialNum = 0;//����1��һ��λ���ϱ�����ˮ��
u16 Link2LastReportSerialNum = 0;//����2��һ��λ���ϱ�����ˮ��
u32 Link1ReportTimeSpace = 0;//ͬLink1ReportTimeһ���������ⲿ����
u32 Link1ReportDistanceSpace = 0;//����1λ���ϱ�ʱ��������λ��

/********************�ⲿ����*************************/
extern TIME_T CurTime;

extern u16     CarControlAckSerialNum;//��������Ӧ����ˮ��
extern u8	DelaySendChannel ;//��ʱ����ͨ����
extern u8	TerminalAuthorizationFlag;//�ն˼�Ȩ��־,��Ȩ�ɹ�Ϊ1,ʧ��Ϊ0

/********************���غ�������*********************/
static void Report_GetLink1NextReportPoint(u32 *NextReportTime, u32 *NextReportDistance);//��ȡ��һ���ϱ���
//static u32 Report_GetCurDistance(void);//��ȡ��ǰ���ۼ���ʻ���
static u16 Report_GetPositionAdditionalInfo(u8 *pBuffer);//��ȡλ�ø�����Ϣ
static u16 Report_GetPositionStdAdditionalInfo(u8 *pBuffer);//��ȡ��׼��λ�ø�����Ϣ
static u16 Report_GetPositionEiAdditionalInfo(u8 *pBuffer);//��ȡ�����Զ����λ�ø�����Ϣ
static void Report_Get_Location(void);//��ȡ��γ����Ϣ


#define REPORT_TICKS     30000
/********************��������*************************/

//�ر�ϵͳ����ʱ��
void Stop_ReportTimer(void)
{
	 Ql_Timer_Stop(TIME_REPORT_TIME_ID);
}
//����ϵͳ����ʱ��
void Start_ReportTimer(void)
{
	Ql_Timer_Start(TIME_REPORT_TIME_ID, SYSTICK_TICKS, TRUE);
}
/******************************************************************************
*	���:��ʱ�����غ���
*	����:
*	����:��ʱId
*	����:��
******************************************************************************/
void Callback_ReportTimer(u32 timeId,void *param)
{

}

//����ʱ�䶨ʱ����ʼ��
void init_ReportTimer(void)
{ 	
	s32 ret;
	//ע��GPS��λ��ʱ��
	ret = Ql_Timer_Register(TIME_REPORT_TIME_ID, Callback_ReportTimer, NULL);
	if(ret <0)
    {
		//����������
        APP_DEBUG("\r\n<--failed!!,init_TaskScheduleTimer Ql_Timer_Register: timer(%d) fail ,ret = %d -->\r\n",TIME_REPORT_TIME_ID,ret);
    }
	Ql_Timer_Start(TIME_REPORT_TIME_ID, REPORT_TICKS, TRUE);
	APP_DEBUG("\r\n<--init_TaskScheduleTimer -->\r\n");
}
void Report_RtcWakeUpOffsetSave(void)
{
	if(RTCPWRON != m_pwrOnReason)
	{
		// д��RTC����ʱ��ƫ����
		FRAM_BufferWrite(FRAM_RTC_WAKEUP_ADDR,(u8*)&RTCWakeUpTimeOffset, FRAM_RTC_WAKEUP_LEN);
	}
}
void Report_RtcWakeUpOffsetResume(void)
{
	// �ָ�RTC����ʱ��ƫ����
	FRAM_BufferRead((u8*)&RTCWakeUpTimeOffset, FRAM_RTC_WAKEUP_LEN, FRAM_RTC_WAKEUP_ADDR);
	APP_DEBUG("\r\n...RTC����ʱ��ƫ����:%d...\r\n",RTCWakeUpTimeOffset);
}
/**************************************************************************
//��������Report_EnterPowerDownAfterReported
//���ܣ��ϱ���ɺ����ػ�ģʽ
//���룺��������
//�����
//����ֵ����
//��ע��
***************************************************************************/
void Report_EnterPowerDownAfterReported(void* param)
{
	BlindSavePara* blindSave = (BlindSavePara*)param;
	u32 upInterval = blindSave->interval;	
	//u8 updateFlg = GetUpdateFlag();

	//if(updateFlg)return;//���������򲻽���͹���ģʽ
	//if(TempTrackCount)return;//���������ʱ׷��״̬�򲻽���͹���ģʽ
	if(upInterval <= 5*60)
	{
		//Led_Status_Change(LED_STATUS_POWER_LOW);
		//Io_WriteIoStatusBit(IO_STATUS_BIT_SLEEP, SET);
		if(0xffff == uart_get_busy_status())
		{
			//APP_DEBUG("\r\n...�ϱ������˯��ģʽ....\r\n");
			//Ql_SleepEnable();//�ϱ����С��5���Ӿͽ���˯��ģʽ
		}
	}
	else
	{
		RIL_Alarm_Create(&blindSave->DateTime,0);
		APP_DEBUG("\r\nSet Alarm :\r\n%02d/%02d/%02d %02d:%02d:%02d\r\n",
			blindSave->DateTime.year, blindSave->DateTime.month,blindSave->DateTime.day, 
			blindSave->DateTime.hour, blindSave->DateTime.minute, blindSave->DateTime.second);
		//Led_Status_Change(LED_STATUS_POWER_DOWN);
		//�ػ�ǰιһ�ι�
		GpioOutInit(WATCHDOG,PINLEVEL_HIGH);//���ó����
		HARD_FEED_DOG();
		Ql_Sleep(200);
		//����֮ǰ����ä���ڵ���Ϣ
        Blind_UpdateCirQueueInfo();
		Report_RtcWakeUpOffsetSave();
		Ql_PowerDown(1);//�ػ�ģʽ
	}
}

static void *pBlindSave;

void Callback_BlindSave_OnTimer(u32 timerId, void* param)
{
	
	BlindSavePara* blindSaveInfo = (BlindSavePara*)param;
	u8 updateFlg = GetUpdateFlag();

	if(LOGIC_BLIND_SAVE_TMR_ID == timerId)
	{
		if(0 == Link1ReportAckFlag)
		{
			APP_DEBUG("\r\nCallback_BlindSave_OnTimerдä��....\r\n");
			Blind_Link1Save(blindSaveInfo->pBuf,blindSaveInfo->BufLen,CHANNEL_DATA_1);
		}
		if(updateFlg == 2)//Զ����������������...
		{
			ftp_downfile_timer();
			return;
		}
		else if(updateFlg == 1)//Զ��������...
		{
			return;
		}
		if(TempTrackCount)return;//���������ʱ׷��״̬�򲻽���͹���ģʽ
		#ifdef BLIND_SAVE_BY_QUEUE
		if((0 == Blind_GetReportState())||(0 == GetTerminalAuthorizationFlag()))
		#else
		if((BLIND_STATE_WAIT == Blind_GetReportState())||(0 == GetTerminalAuthorizationFlag()))
		#endif
		{
			APP_DEBUG("\r\nCallback_BlindSave_OnTimer....%d\r\n",timerId);
			Report_EnterPowerDownAfterReported(param);//��ʱ����ػ�ģʽ
		}
	}
}
/**************************************************************************
//��������Report_Blind_Callback_Init
//���ܣ�λ���ϱ���ä���ص�������ʼ��
//			ע��ä���ص����������ûص�����
//���룺��������
//�����
//����ֵ����
//��ע��
***************************************************************************/
void Report_Blind_Callback_Init(void)
{
	pBlindSave = Ql_MEM_Alloc(sizeof(BlindSavePara));
	if(NULL != pBlindSave)
	{
		Ql_Timer_Register(LOGIC_BLIND_SAVE_TMR_ID, Callback_BlindSave_OnTimer, pBlindSave);
		APP_DEBUG("\r\n<-- Blind Ql_Timer_Register -- >\r\n");
	}
	else
	{
		APP_DEBUG("\r\nMEM APPLY FAIL.....\r\n");
		Ql_Reset(0);
	}	
}

void proc_report_task(s32 taskId)
{
			
	while(TRUE)
	{
		Ql_Sleep(200);
		if(ProjectInitFinshFlag==1)
		{
			Report_TimeTask();
			//HeartBeat_TimeTask();
		}
//		SoftWdg_Feed(BIT1);
	}
}

/**************************************************************************
//��������GetNextReportTime
//���ܣ���ȡ������һ���ϱ���ʱ��
//���룺��
//�������
//����ֵ��
//��ע��
***************************************************************************/
u32 GetNextReportTime(void)
{
    return Link1ReportTime - Link1ReportTimeCount;
}
/**************************************************************************
//��������Report_GetReportInterval
//���ܣ���ȡȱʡ�㱨ʱ����
//���룺��
//�������
//����ֵ��
//��ע��
***************************************************************************/
u32 Report_GetDefaultReportInterval(void)
{
    return AccOffReportTimeSpace;
}

/**************************************************************************
//��������Report_GetReportStatus
//���ܣ���ȡ�ϱ�״̬
//���룺
//�����
//����ֵ��
//��ע��
***************************************************************************/
ReportStatus_TypeDef Report_GetReportStatus(void)
{
	return ReportStatus;
}
/**************************************************************************
//��������Report_SetReportStatus
//���ܣ���λ�ϱ�״̬
//���룺��������
//�����
//����ֵ����
//��ע��
***************************************************************************/
void Report_SetReportStatus(ReportStatus_TypeDef reportSta)
{
	ReportStatus = reportSta;
}

u8 Report_IsReady(u64 time)
{
	if(RTCPWRON != m_pwrOnReason)
	{
		if(Link1ReportTimeCount >= Link1ReportTime)
		{
			RTCWakeUpTimeOffset = time%Link1ReportTime;
			return 1;
		}
	}
	else
	{
		if(RTCWakeUpTimeOffset == time%Link1ReportTime)
			return 1;
	}
	return 0;
}
/**************************************************************************
//��������Report_TimeTask
//���ܣ�ʵ��λ�û㱨������ʵʱ���١��������������ٱ�������ʱ��������ʱ�����λ�û㱨��
//���룺��
//�������
//����ֵ��ʼ����ENABLE
//��ע��λ�û㱨��ʱ����1���ӽ���1�Σ������������Ҫ���ô˺���
***************************************************************************/
FunctionalState Report_TimeTask(void)
{
	u8 Link1ReportFlag = 0;//λ���ϱ���־��1��ʾҪ�ϱ���0��ʾ�����ϱ�
	u8 Ack = 0;
	u8 TempBuffer[6];
	//u8 ReportAddTimeCount = 0;
	u32 CurTimeCount,crc32Verify;
	static u32 LastTimeCount = 0;
	ST_Time dateTime;
	u64 mkTime;
	static u64 LastmkTime = 0;
	static u32 OnlineOvertimeCnt = 0;//���ڲ����߼�������ʱ�䵽�˽���ػ�ģʽ


	if(ReportStatus_Off == Report_GetReportStatus())
	{
		if(OnlineOvertimeCnt++ > 80)//��Լ30s
		{
			Report_SetReportStatus(ReportStatus_Init);//��Լһ����ת����һ��
		}
		return ENABLE;
	}
	else if(ReportStatus_Init == Report_GetReportStatus())
	{
		if(GetTerminalAuthorizationFlag())
		{
			OnlineOvertimeCnt = 0;
			Report_SetReportStatus(ReportStatus_On);   
		}
		else
		{
			Report_SetReportStatus(ReportStatus_Wait);
		}
		Report_Get_Location();//ȡ��γ����Ϣ
		Report_UpdatePram();
		LastTimeCount = ConverseGmtime(&CurTime);
		return ENABLE;
	}
	else if(ReportStatus_Wait == Report_GetReportStatus())
	{
	// ���ڲ����ߵĵ͹��Ĵ���,�б�Ҫ��???????
	/*
		if(OnlineOvertimeCnt++ > 3000)//��Լ10����
		{
			//��������
			u32 interval,temp;
			interval = Report_GetDefaultReportInterval();//��ȡ�ϱ����
			Ql_GetLocalTime(&dateTime);//��ȡʵʱʱ��
			mkTime = Ql_Mktime(&dateTime);			
			temp = mkTime%interval;//�����´���Ҫ�ϱ���ʱ��
			if(temp > 120)//����2����
			{
				mkTime += temp;
				mkTime -= 60;//����������ǰһ���ӿ���
				Ql_MKTime2CalendarTime(mkTime, &dateTime);
				dateTime.year -= 2000;
				RIL_Alarm_Create(&dateTime,0);
				APP_DEBUG("\r\nSet Alarm :\r\n%02d/%02d/%02d %02d:%02d:%02d\r\n",
				dateTime.year, dateTime.month,dateTime.day, 
				dateTime.hour, dateTime.minute, dateTime.second);
				//�ػ�ǰιһ�ι�
				GpioOutInit(WATCHDOG,PINLEVEL_HIGH);//���ó����
				HARD_FEED_DOG();
				Ql_Sleep(200);
				APP_DEBUG("<-- ��ʱ�����߽���ػ�ģʽ-- >\r\n");
				Ql_PowerDown(1);//�ػ�ģʽ
			}
		}*/
	}
	
	Report_GetLink1NextReportPoint(&Link1ReportTime,&Link1ReportDistance);

	if(0 != Link1ReportTime)//Link1ReportTime����0��ʾ��������ʱ�ϱ�
	{
		Ql_GetLocalTime(&dateTime);//��ȡʵʱʱ��
		mkTime = Ql_Mktime(&dateTime);
		if(mkTime == LastmkTime)return ENABLE;
		Link1ReportTimeCount++;
		LastmkTime = mkTime;
		if(Report_IsReady(mkTime))
		{
			//Ql_SleepDisable();
			Link1ReportFlag = 1;
			APP_DEBUG("\r\nreport time is :\r\n%02d/%02d/%02d %02d:%02d:%02d:%02d\r\n",
			dateTime.year, dateTime.month,dateTime.day, 
			dateTime.hour, dateTime.minute, dateTime.second,Link1ReportTime);
			if((TempTrackTimeSpace > 0)&&(TempTrackCount > 0))
			{
				TempTrackCount--;
                if(0 == TempTrackCount)
                {
                    TempTrackTimeSpace = 0;
                }
				Report_PramBackup();
				APP_DEBUG("<-- ��ʱ׷�ٴ���.... %d -- >\r\n", TempTrackCount);
			}
		}
		
	}

	if(1 == Link1ReportFlag)
	{	                
		APP_DEBUG("<-- Report PositionInfo -- >\r\n" );
		RIL_NW_GetCell();
		RIL_GetPowerSupply(&battery_capacity, &battery_voltage);
		Io_WriteIoStatusBit(IO_STATUS_BIT_SLEEP, RESET);
		Link1ReportBufferLen = Report_GetPositionInfo(Link1ReportBuffer);

		Ack = RadioProtocol_PostionInformationReport(Link1ReportBuffer,Link1ReportBufferLen);
		
		((pBlindSavePara)pBlindSave)->pBuf = Link1ReportBuffer;		
		((pBlindSavePara)pBlindSave)->BufLen = Link1ReportBufferLen;
		((pBlindSavePara)pBlindSave)->interval = Link1ReportTime;
		//��������
		//Ql_GetLocalTime(&dateTime);//��ȡʵʱʱ��
		//mkTime = Ql_Mktime(&dateTime);
		mkTime += Link1ReportTime;//��һ���ϱ�ʱ��
		mkTime -= 60;//����������ǰ60s����
		Ql_MKTime2CalendarTime(mkTime, &dateTime);
		dateTime.year -= 2000;
		Ql_memcpy(&((pBlindSavePara)pBlindSave)->DateTime,&dateTime,sizeof(ST_Time));
		if(GetUpdateFlag())//����Զ������������������...
		{
			SetUpdateFlag(1);
		}
		Ql_OS_SendMessage(main_task_id, MSG_ID_POWER_DOWN, REPORT_OVERTIME, 0);

		Link1ReportTimeCount = 0;
		Link1ReportAckFlag = 0;	
		Report_Get_Location();//ȡ��γ����Ϣ
	}
	return ENABLE;	
}
/*******************************************************************************
* Function Name  : DelayTrigTimeTask(void)
* Description    : ��ʱ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
FunctionalState DelayTrigTimeTask(void)
{
    u8 Buffer[FLASH_BLIND_STEP_LEN+20];
    u8 flag;
    u8 Len;
    u16 length;
    MESSAGE_HEAD head;

    if(0 == DelayAckCommand)
    {
        return DISABLE;
    }

    flag = 0;
    switch(DelayAckCommand)
    {
        case 0:
        {
            break;
        }
        case 0x0500://0x0500��������
        {
            //Ӧ����ˮ��
            length = 0;
            Buffer[0] = (CarControlAckSerialNum & 0xff00) >> 8;
            Buffer[1] = CarControlAckSerialNum & 0xff;
            length += 2;
            //��ȡλ����Ϣ,������Buffer��
            Len = Report_GetPositionInfo(Buffer+2);
            length += Len;
            //����λ�û㱨��Ϣ
            //��ϢID
            head.MessageID = 0x0500;
            head.Attribute = length;
            //����
            if(ERROR == RadioProtocol_SendCmd(head, Buffer, length, DelaySendChannel))
            {
                flag = 1;
            }
            break;
        }        
        case 2:
        {
            //Report_EraseBlindArea();//����ǰ����һ��ä��flash myh 130808
            // ����������������
            SetEvTask(MSG_FtpUpdata_EvTask);
            break;
        }
        case 0x0003://0x0003,�ն�ע��
        {
            TerminalAuthorizationFlag = 0;//�б����޸�
            break;
        }
        case 4:
        {
            //Communication_Init();
            break;
        }
        case 5:
        {
            //����
            Ql_Reset(0);
            break;
        }
        case 6:
        {
            // �������Ӽ��ƽ̨����
            //SetEvTask(EV_LINK_GOV_SERVER);
            break;
        }
        case 7:
        {
            //����֮ǰ����ä���ڵ���Ϣ
            Blind_UpdateCirQueueInfo();
            DelayAckCommand = 5;//��һ������
            flag = 1;
            break;
        }
        case 8:
        {
            //��Communication_TimeTask����
            break;
        }
        case 9:
        {
            //MOD_CloseMainIp();//ģ�������Ͽ�����
            DelayAckCommand = 10;//��һ���ǹر�ͨ��ģ������
            //������ʱ����
            SetTimerTask(TIME_DELAY_TRIG, 5*SECOND);
            TerminalAuthorizationFlag = 0;
            flag = 1;
            break;
        }
        case 10:
        {
            //dxl,2017.2.24,�޴˹���
            break;
        }
        case 11:
        {
            //MOD_CloseMainIp();//ģ�������Ͽ�����
            DelayAckCommand = 12;//��һ���ǹػ�
            //������ʱ����
            SetTimerTask(TIME_DELAY_TRIG, 5*SECOND);
            TerminalAuthorizationFlag = 0;
            flag = 1;
			//����
            Ql_Reset(0);
            break;
        }
        case 12:
        {
			APP_DEBUG("\r\n<-- Ӳ���ػ� -- >\r\n");
			//����ػ�֮ǰдä���ڵ���Ϣ��Flash
			Blind_UpdateCirQueueInfo();
			REMOTE_SHUTDOWN();
            break;
        }
        case 13:
        {
            if(ACK_OK != RadioProtocol_OriginalDataUpTrans(0x41, DelayBuffer, DelayBufferLen))
            {
                flag = 1;
            }
            break;
        }
		case 14:
        {
            Report_UploadPositionInfo();
			Report_ClearReportTimeCount();
            break;
        }
        default :break;
    }

    if(0 == flag)
    {
        DelayAckCommand  = 0;
        return DISABLE;
    }
    else
    {
        return ENABLE;
    }
}

/**************************************************************************
//��������Report_UpdatePram
//���ܣ�����λ�û㱨���б���
//���룺��
//�������
//����ֵ����
//��ע����Ҫ�Ǹ���λ�û㱨ģ����ʹ�õ��ı����������������֮ǰ����ô˺���
***************************************************************************/
void Report_UpdatePram(void)
{
  u8 Buffer[5];
  
  if(E2_SLEEP_REPORT_TIME_LEN == EepromPram_ReadPram(E2_SLEEP_REPORT_TIME_ID,Buffer))
  {
    AccOffReportTimeSpace = Public_ConvertBufferToLong(Buffer);
  }
  if(E2_ACCON_REPORT_TIME_LEN == EepromPram_ReadPram(E2_ACCON_REPORT_TIME_ID,Buffer))
  {
    /*AccOnReportTimeSpace*/TempTrackTimeSpace = Public_ConvertBufferToLong(Buffer);
  }

}
/**************************************************************************
//��������Report_CmdAck
//���ܣ�λ���ϱ�Ӧ����
//���룺ͨ����
//�������
//����ֵ����
//��ע���յ�λ���ϱ���ͨ��Ӧ��ʱ����ô˺���
***************************************************************************/
void Report_CmdAck(void)
{
    Link1ReportAckFlag = 1;
}
//׷����Ϣ����
void Report_PramBackup(void)
{
	u8 Buffer[10];
	u32 crc32Verify;

	Public_ConvertLongToBuffer(TempTrackCount,Buffer);	
	Public_ConvertShortToBuffer(TempTrackTimeSpace,Buffer+4);
	crc32Verify = Public_CRC_32( Buffer, 6 );
	Public_ConvertLongToBuffer(crc32Verify,Buffer+6);
	FRAM_BufferWrite(FRAM_TEMP_TRACK_NUM_ADDR,Buffer,10);
}
//�ϵ�ָ�׷����Ϣ
void Report_PramResume(void)
{
	u32 crc32Verify,crc32VerifyTmp;
	u8 Buffer[30];

	FRAM_BufferRead(Buffer, 10, FRAM_TEMP_TRACK_NUM_ADDR);
	crc32Verify = Public_CRC_32( Buffer, 6 );
	crc32VerifyTmp = Public_ConvertBufferToLong(Buffer+6);
	if(crc32Verify == crc32VerifyTmp)
	{
		if(FRAM_TEMP_TRACK_SPACE_LEN == FRAM_BufferRead(Buffer, 2, FRAM_TEMP_TRACK_SPACE_ADDR))
	    {
	        TempTrackTimeSpace = Public_ConvertBufferToShort(Buffer);
	    }
	    if(FRAM_TEMP_TRACK_NUM_LEN == FRAM_BufferRead(Buffer, 4, FRAM_TEMP_TRACK_NUM_ADDR))
	    {
	        TempTrackCount = Public_ConvertBufferToLong(Buffer);
	    }
		APP_DEBUG("\r\n...��ʱ׷��У��ͨ��%04x   %04x  %d   %d...\r\n",crc32Verify,crc32VerifyTmp,TempTrackTimeSpace,TempTrackCount);
	}
	else
	{
		APP_DEBUG("\r\n...��ʱ׷��У�鲻��ȷ...\r\n");
	}
	
}

/**************************************************************************
//��������Report_UpdateOnePram
//���ܣ�����ĳ������
//���룺��������
//������ò������Ͷ�Ӧ�ı���
//����ֵ����
//��ע�����ڻ�Զ������λ�û㱨��ز���ʱ��Ҫ���ô˺���
***************************************************************************/
void Report_UpdateOnePram(u8 type)
{
    u8 Buffer[30];
    u8 BufferLen;
    u32 TcpPort;
    
    switch(type)
    {
    case PRAM_TEMP_TRACK_TIME://��ʱ����ʱ����
        {
            if(FRAM_TEMP_TRACK_SPACE_LEN == FRAM_BufferRead(Buffer, 2, FRAM_TEMP_TRACK_SPACE_ADDR))
            {
                TempTrackTimeSpace = Public_ConvertBufferToShort(Buffer);
            }
            break;
        }
    case PRAM_TEMP_TRACK_COUNT://��ʱ���ٴ���
        {
            if(FRAM_TEMP_TRACK_NUM_LEN == FRAM_BufferRead(Buffer, 4, FRAM_TEMP_TRACK_NUM_ADDR))
            {
                TempTrackCount = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
#if 0
    case PRAM_EMERGENCY_REPORT_TIME://���������㱨ʱ����
        {
            if(E2_EMERGENCY_REPORT_TIME_LEN == EepromPram_ReadPram(E2_EMERGENCY_REPORT_TIME_ID,Buffer))
            {
                EmergencyReportTimeSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_EMERGENCY_REPORT_DISTANCE://���������㱨������
        {
            if(E2_EMERGENCY_REPORT_DISTANCE_LEN == EepromPram_ReadPram(E2_EMERGENCY_REPORT_DISTANCE_ID,Buffer))
            {
                EmergencyReportDistanceSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_OVER_SPEED_REPORT_TIME://���ٱ����㱨ʱ����
        {
            if(E2_OVERSPEED_ALARM_REPORT_TIME_ID_LEN == EepromPram_ReadPram(E2_OVERSPEED_ALARM_REPORT_TIME_ID,Buffer))
            {
                OverSpeedReportTimeSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_OVER_TIME_REPORT_TIME://��ʱ�����㱨ʱ����
        {
            if(E2_TIRED_DRIVE_REPORT_TIME_ID_LEN == EepromPram_ReadPram(E2_TIRED_DRIVE_REPORT_TIME_ID,Buffer))
            {
                OverTimeReportTimeSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_ACC_ON_REPORT_TIME://ȱʡ�㱨ʱ����
        {
            if(E2_ACCON_REPORT_TIME_LEN == EepromPram_ReadPram(E2_ACCON_REPORT_TIME_ID,Buffer))
            {
                AccOnReportTimeSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_ACC_ON_REPORT_DISTANCE://ȱʡ�㱨������
        {
            if(E2_ACCON_REPORT_DISTANCE_LEN == EepromPram_ReadPram(E2_ACCON_REPORT_DISTANCE_ID,Buffer))
            {
                AccOnReportDistanceSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_ACC_OFF_REPORT_TIME://���߻㱨ʱ����
        {
            if(E2_SLEEP_REPORT_TIME_LEN == EepromPram_ReadPram(E2_SLEEP_REPORT_TIME_ID,Buffer))
            {
                AccOffReportTimeSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_ACC_OFF_REPORT_DISTANCE://���߻㱨������
        {
            if(E2_SLEEP_REPORT_DISTANCE_LEN == EepromPram_ReadPram(E2_SLEEP_REPORT_DISTANCE_ID,Buffer))
            {
                AccOffReportDistanceSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_UNLOGIN_REPORT_TIME://��ʻԱδ��¼�㱨ʱ����
        {
            if(E2_DRIVER_UNLOGIN_REPORT_TIME_LEN == EepromPram_ReadPram(E2_DRIVER_UNLOGIN_REPORT_TIME_ID,Buffer))
            {
                UnloginReportTimeSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_UNLOGIN_REPORT_DISTANCE://��ʻԱδ��¼�㱨������
        {
            if(E2_DRIVER_UNLOGIN_REPORT_DISTANCE_LEN == EepromPram_ReadPram(E2_DRIVER_UNLOGIN_REPORT_DISTANCE_ID,Buffer))
            {
                UnloginReportDistanceSpace = Public_ConvertBufferToLong(Buffer);
            }
            break;
        }
    case PRAM_SECOND_LINK_ACC_ON_REPORT_TIME://����2 ACC ON�㱨ʱ����
        {
            if(E2_SECOND_ACC_ON_REPORT_TIME_ID_LEN == EepromPram_ReadPram(E2_SECOND_ACC_ON_REPORT_TIME_ID,Buffer))
            {
                Link2AccOnReportTimeSpace = Public_ConvertBufferToLong(Buffer);
            }
            
            break;
        }
    case PRAM_SECOND_LINK_ACC_OFF_REPORT_TIME://����2 ACC OFF�㱨ʱ����
        {
            
            if(E2_SECOND_ACC_OFF_REPORT_TIME_ID_LEN == EepromPram_ReadPram(E2_SECOND_ACC_OFF_REPORT_TIME_ID,Buffer))
            {
                Link2AccOffReportTimeSpace = Public_ConvertBufferToLong(Buffer);
            }
            
            break;
        }
    case PRAM_SECOND_LINK_OPEN_FLAG://����2 ������־
        {   
            BufferLen = EepromPram_ReadPram(E2_SECOND_MAIN_SERVER_IP_ID, Buffer);
            if(0 == BufferLen)
            {
                Link2OpenFlag = 0;
            }
            else
            {
                BufferLen = EepromPram_ReadPram(E2_SECOND_MAIN_SERVER_TCP_PORT_ID, Buffer);
                if(E2_SECOND_MAIN_SERVER_TCP_PORT_ID_LEN == BufferLen)
                {
                    TcpPort = Public_ConvertBufferToLong(Buffer);
                    if(0 == TcpPort)
                    {
                        Link2OpenFlag = 0;
                    }
                    else
                    {
                        Link2OpenFlag = 1;
                    }
                }
                else
                {
                    Link2OpenFlag = 0;
                }
        
            }
            
            break;
        }
    case PRAM_REPORT_EVENT_SWITCH://λ���ϱ��¼����
        {
            
            if(E2_REPORT_FREQ_EVENT_SWITCH_ID_LEN == EepromPram_ReadPram(E2_REPORT_FREQ_EVENT_SWITCH_ID,Buffer))
            {
                ReportEventSwitch = Public_ConvertBufferToLong(Buffer);
            }
            
            break;
        }
#endif    
        default:break;
    }
		
	Link1ReportTimeCount = 0;//���ò���ʱ����ʱ������0		
}

/**************************************************************************
//��������Report_UploadPositionInfo
//���ܣ��ϴ�һ��λ����Ϣ
//���룺ͨ����
//�������
//����ֵ��0�ϴ��ɹ���1�ϴ�ʧ��
//��ע����ǰ��λ����Ϣ==λ�û�����Ϣ+λ�ø�����Ϣ��λ����Ϣ�Ϊ150�ֽ�
***************************************************************************/
u8 Report_UploadPositionInfo(void)
{
	u8 Buffer[FLASH_BLIND_STEP_LEN+1];
	u8 BufferLen;
        
	BufferLen = Report_GetPositionInfo(Buffer);
        
        if(ACK_OK == RadioProtocol_PostionInformationReport(Buffer,BufferLen))
        {
          return 0;
        }
        else
        {//APP_DEBUG("\r\nReport_UploadPositionInfoдä��....\r\n");
          Blind_Link1Save(Buffer, BufferLen,CHANNEL_DATA_1); //CHANNEL_DATA_1==0x02
          return 1;
        }
}
/**************************************************************************
//��������Report_GetPositionInfo
//���ܣ���ȡ��ǰ��λ����Ϣ
//���룺��
//�����λ����Ϣ
//����ֵ��λ����Ϣ����
//��ע����ǰ��λ����Ϣ==λ�û�����Ϣ+λ�ø�����Ϣ��λ����Ϣ�Ϊ150�ֽ�
***************************************************************************/
u16 Report_GetPositionInfo(u8 *pBuffer)
{
	u8 *p = NULL;
	u16 length;

	p = pBuffer;
	length = 0;

	length = Report_GetPositionBasicInfo(p);//��ȡλ�û�����Ϣ
	p = p+length;

	length += Report_GetPositionAdditionalInfo(p);//��ȡλ�ø�����Ϣ

	return length;

}
static ST_LocInfo locinfo = {0};
//ȡ��γ��
static void Report_Callback_Location(s32 result, ST_LocInfo* loc_info)
{
	locinfo = *loc_info;
    APP_DEBUG("\r\n<-- Module location: latitude=%f, longitude=%f -->\r\n", loc_info->latitude, loc_info->longitude);
}
static void Report_Get_Location(void)
{
	
	/****************************************************************************
	* Definition for APN
	****************************************************************************/
	#define APN      "CMNET\0"
	#define USERID   ""
	#define PASSWD   ""
	
	s32 ret;
	u8  pdpCntxtId;

	// Set PDP context
	ret = Ql_GPRS_GetPDPContextId();
	APP_DEBUG("<-- The PDP context id available is: %d (can be 0 or 1)-->\r\n", ret);
	if (ret >= 0)
	{
	    pdpCntxtId = (u8)ret;
	} else {
    	APP_DEBUG("<-- Fail to get PDP context id, try to use PDP id(0) -->\r\n");
	    pdpCntxtId = 0;
	}

	ret = RIL_NW_SetGPRSContext(pdpCntxtId);
	APP_DEBUG("<-- Set PDP context id to %d -->\r\n", pdpCntxtId);
	if (ret != RIL_AT_SUCCESS)
	{
	    APP_DEBUG("<-- Ql_RIL_SendATCmd error  ret=%d-->\r\n",ret );
	}

	// Set APN
	ret = RIL_NW_SetAPN(1, APN, USERID, PASSWD);
	APP_DEBUG("<-- Set APN -->\r\n");

	// Request to get location
	{
		APP_DEBUG("<--Ql_Getlocation-->\r\n");
		ret = RIL_GetLocation(Report_Callback_Location);
		if(ret!=RIL_AT_SUCCESS)
		{
			APP_DEBUG("<-- Ql_Getlocation error  ret=%d-->\r\n",ret );
		}
	}

}

/**************************************************************************
//��������Report_GetPositionBasicInfo
//���ܣ���ȡλ�û�����Ϣ
//���룺��
//�����λ�û�����Ϣ
//����ֵ��λ�û�����Ϣ����
//��ע�����ȹ̶�Ϊ28�ֽڣ�λ�û�����Ϣ�������֡�״̬�֡�γ�ȡ����ȡ��̡߳��ٶȡ�����ʱ��
***************************************************************************/
u16 Report_GetPositionBasicInfo(u8 *pBuffer)
{
	u8 *p = NULL;
	u32 temp;
	u16 temp2;
	p = pBuffer;
	
	temp = Io_ReadAlarm();//������
	Public_ConvertLongToBuffer(temp,p);
	p += 4;

	temp = Io_ReadStatus();//״̬��
	Public_ConvertLongToBuffer(temp,p);
	p += 4;

	temp = (u32)(locinfo.latitude*1000000UL);//γ��
	Public_ConvertLongToBuffer(temp,p);
	p += 4;

	temp = (u32)(locinfo.longitude*1000000UL);//����
	Public_ConvertLongToBuffer(temp,p);
	p += 4;

	temp2 = 0;//�߳�
	Public_ConvertShortToBuffer(temp2,p);
	p += 2;

	temp2 = 0;//�ٶ�
	Public_ConvertShortToBuffer(temp2,p);
	p += 2;

	temp2 = 0;//����
	Public_ConvertShortToBuffer(temp2,p);
	p += 2;

	*p++ = Public_HEX2BCD(CurTime.year);//��ȡʱ��
	*p++ = Public_HEX2BCD(CurTime.month);
	*p++ = Public_HEX2BCD(CurTime.day);
	*p++ = Public_HEX2BCD(CurTime.hour);
	*p++ = Public_HEX2BCD(CurTime.min);
	*p++ = Public_HEX2BCD(CurTime.sec);
	APP_DEBUG("<--Report Ql_GetLocalTime(%d.%02d.%02d %02d:%02d:%02d) -->\n\r", CurTime.year, CurTime.month, CurTime.day, CurTime.hour, CurTime.min, CurTime.sec);

	return 28;
	
}
/**************************************************************************
//��������Report_ClearReportTimeCount
//���ܣ��ϱ�ʱ�����������0
//���룺��
//�������
//����ֵ����
//��ע���������ӵļ�����������0
***************************************************************************/
void Report_ClearReportTimeCount(void)
{
	Link1ReportTimeCount = 0;
}

/**************************************************************************
//��������Report_GetReportEventSwitch
//���ܣ���ȡReportEventSwitch��ֵ
//���룺��
//�������
//����ֵ����
//��ע��ReportEventSwitch��ֵ���ڲ���ID 0xF27D������ֵ
***************************************************************************/
u32 Report_GetReportEventSwitch(void)
{
	return ReportEventSwitch;
}
/**************************************************************************
//��������Report_GetOverSpeedReportTimeSpace
//���ܣ���ȡOverSpeedReportTimeSpace��ֵ
//���룺��
//�������
//����ֵ����
//��ע��OverSpeedReportTimeSpace��ֵ���ڲ���ID 0xF259������ֵ
***************************************************************************/
u32 Report_GetOverSpeedReportTimeSpace(void)
{
	return OverSpeedReportTimeSpace;
}
/**************************************************************************
//��������Report_GetOverTimeReportTimeSpace
//���ܣ���ȡOverTimeReportTimeSpace��ֵ
//���룺��
//�������
//����ֵ����
//��ע��OverTimeReportTimeSpace��ֵ���ڲ���ID 0xF277������ֵ
***************************************************************************/
u32 Report_GetOverTimeReportTimeSpace(void)
{
	return OverTimeReportTimeSpace;
}
/**************************************************************************
//��������Report_GetLink1NextReportPoint
//���ܣ���ȡ����1����һ��λ�û㱨�㣨�㱨ʱ��ͻ㱨���룩
//���룺��
//�������
//����ֵ����
//��ע��
***************************************************************************/
static void Report_GetLink1NextReportPoint(u32 *NextReportTime, u32 *NextReportDistance)
{
	
    if((TempTrackTimeSpace > 0)&&(TempTrackCount > 0))//��ʱ����
    {
        *NextReportTime = TempTrackTimeSpace;
	*NextReportDistance = 0;
	Link1ReportDistanceSpace = 0;
    }
	#ifdef DEVICE_USE_ZHB
	else
	{
		*NextReportTime = AccOffReportTimeSpace;
        *NextReportDistance = 0;
        Link1ReportDistanceSpace = 0;
	}
	#else
    else if(1 == Io_ReadStatusBit(STATUS_BIT_ACC))//ACC ON
    {
        *NextReportTime = AccOnReportTimeSpace;
        *NextReportDistance = 0;
        Link1ReportDistanceSpace = 0;
    }
    else//ACC OFF
    {
        *NextReportTime = AccOffReportTimeSpace;
        *NextReportDistance = 0;
        Link1ReportDistanceSpace = 0;				
    }
	#endif
}

/**************************************************************************
//��������Report_GetCurDistance
//���ܣ���ȡ��ǰ�ľ��루�ۼ���ʻ��̣�
//���룺��
//�������
//����ֵ�����루�ۼ���ʻ��̣�
//��ע����λ��
***************************************************************************/
//static u32 Report_GetCurDistance(void)
//{
  //dxl,2017.2.24,������

	//return 0;
//}
/**************************************************************************
//��������Report_GetPositionAdditionalInfo
//���ܣ���ȡλ�ø�����Ϣ
//���룺��
//�����λ�ø�����Ϣ
//����ֵ��λ�ø�����Ϣ����
//��ע��
***************************************************************************/
static u16 Report_GetPositionAdditionalInfo(u8 *pBuffer)
{
	u8 *p = NULL;
	u16 length;

	p = pBuffer;
	length = 0;

	length = Report_GetPositionStdAdditionalInfo(p);//��ȡ��׼��λ�ø�����Ϣ
	p = p+length;

	length += Report_GetPositionEiAdditionalInfo(p);//��ȡ�����Զ����λ�ø�����Ϣ

	return length;	
}
//û�õ�����ʱ����0
#define ReadMileSum()								(0)
#define Gps_ReadSpeed()								(0)
#define Area_GetInOutAreaAlarmSubjoinInfo(ptr)		(0)
#define Gps_ReadStaNum()							(0)


/**************************************************************************
//��������Report_GetPositionStdAdditionalInfo
//���ܣ���ȡ��׼��λ�ø�����Ϣ
//���룺��
//�������׼��λ�ø�����Ϣ
//����ֵ����׼��λ�ø�����Ϣ����
//��ע��������ϢID:0x01~0x04,0x11~0x13,0x25,0x2a,0x2b,0x30,0x31
***************************************************************************/
static u16 Report_GetPositionStdAdditionalInfo(u8 *pBuffer)
{
	u8 *p = NULL;
	u16 length;
	u32 temp;
	u16 temp2;
	u32 bit_err;

	p = pBuffer;
	length = 0;

/*

	*p++ = 0x01;//��̣�IDΪ0x01	
	*p++ = 4;
	temp = ReadMileSum();
	Public_ConvertLongToBuffer(temp,p);
	p += 4;
	length += 6;

//ID 03 02 û��

	*p++ = 0x03;//��ʻ��¼���ٶȣ�IDΪ0x03���˴����׼Ҫ��һ�������˴���
	*p++ = 2;//��������������������ѡ��GPS�ٶ�ʱ���������ٶȲ�Ϊ0ʹ�������ٶȣ���Ϊ0ʹ��GPS�ٶȣ�
	
	temp2 = 10*Gps_ReadSpeed();
	Public_ConvertShortToBuffer(temp2,p);
	p += 2;
	length += 4;

	///��ϢIDΪ0x12///�������򱨾�������Ϣ��8/////////////////
	temp2= Area_GetInOutAreaAlarmSubjoinInfo(p);//�������򱨾�;
	length +=temp2;
	p+=temp2;
	
//ID 25 û��

	*p++ = 0x25;//��չ�����ź�״̬��IDΪ0x25	
	*p++ = 4;
	temp = Io_ReadExtCarStatus();
	Public_ConvertLongToBuffer(temp,p);
	p += 4;
	length += 6;

	*p++ = 0x2a;//IO״̬��IDΪ0x2a	
	*p++ = 2;
	temp2 = Io_ReadIoStatus();
	if((1 == Io_ReadStatusBit(STATUS_BIT_ACC))&&(0x02 == (temp2&0x02)))//������ACC ON��SLEEP ON����ì�ܵ����ʱ��ACC ONΪ׼
	{
		temp2 = temp2&0xfc;
	}
	Public_ConvertShortToBuffer(temp2,p);
	p += 2;
	length += 4;
//2B û��

	//temp2 = Ad_GetValue(ADC_EXTERN1);dxl,2017.2.24,������
	//temp3 = Ad_GetValue(ADC_EXTERN2);dxl,2017.2.24,������
        temp2 = 0;
        temp3 = 0;
	*p++ = 0x2b;//ģ������IDΪ0x2b����16λΪģ����2����16λΪģ����1
	*p++ = 4;
	Public_ConvertShortToBuffer(temp3,p);
	p += 2;
	Public_ConvertShortToBuffer(temp2,p);
	p += 2;
	length += 6;
*/
	*p++ = 0x30;//���������ź�ǿ�ȣ�IDΪ0x30	
	*p++ = 1;
	RIL_NW_GetSignalQuality((u32*)&temp, (u32*)&bit_err);
	*p++ = (u8)temp>0?(u8)temp:0; 
	length += 3;

	//*p++ = 0x31;//��λ������IDΪ0x31	
	//*p++ = 1;
	//*p++ = Gps_ReadStaNum();
	//length += 3;
	
	return length;
	
}
/**************************************************************************
//��������Report_Get_BatteryInfo_Ext_0xE6
//���ܣ���ȡ��ص�ѹ�������ٷֱȣ����ֻ�е�ѹ��Ϣ����ֻ��2���ֽ�
//���룺pBuffer--->����ָ��mode:1  �е����ٷֱ���Ϣ,   0   û��
//����������Զ����λ�ø�����Ϣ
//����ֵ�������Զ����λ�ø�����Ϣ����
//��ע��
***************************************************************************/
static u32 Report_Get_BatteryInfo_Ext_0xE6(u8 *pBuffer,u8 mode)
{
	u8*p = pBuffer;

	*p++ = 0xE6;
	if(mode)
	{
		*p++ = 0x4;
	}
	else
	{
		*p++ = 0x2;
	}
	Public_ConvertShortToBuffer((unsigned short)(battery_voltage/10),p);
	Public_ConvertShortToBuffer((unsigned short)battery_capacity,p+2);

	return mode?(2+4):(2+2);
}
/**************************************************************************
//��������Report_GetPositionEiAdditionalInfo
//���ܣ���ȡ�����Զ����λ�ø�����Ϣ
//���룺��
//����������Զ����λ�ø�����Ϣ
//����ֵ�������Զ����λ�ø�����Ϣ����
//��ע��
***************************************************************************/
static u16 Report_GetPositionEiAdditionalInfo(u8 *pBuffer)
{
	u8 length;
	u8 *p = NULL;
	u32 temp;
	u8 VerifyCode;

	length = 0;
	p = pBuffer;

	p += 2;//�տ���������ID����־��1�ֽڣ�����1�ֽ�

	*p++ = 0xE1;//�Զ���״̬��2��IDΪ0xE1
	*p++ = 4;
	temp = Io_ReadSelfDefine2();
	Public_ConvertLongToBuffer(temp,p);
	p += 4;
	length += 6;

 	temp = Report_Get_BatteryInfo_Ext_0xE6(p,1);
	p += temp;
	length += temp;
	
	temp = PackgQENG_Info(p);//��ȡ��վ��Ϣ��ֻ���ڲ�������ʱ����ϴ���վ
	p += temp;
	length += temp;
	
	VerifyCode = Public_GetSumVerify(pBuffer+2,length);//У���
	*p++ = VerifyCode;
	length++;

	*pBuffer = 0xF0;//������չ������Ϣ��־
	*(pBuffer+1) = length;

	return length+2;
	
}





