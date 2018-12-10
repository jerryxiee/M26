/********************************************************************************
 * File Name:           heartbeat.c
 * Function Describe:	������������������·����
 * Relate Module:       heartbeat.h
 * Writer:              Joneming
 * Date:                2015-05-12
 * Rewriter: 			
 * Date:				
 *******************************************************************************/

//********************************ͷ�ļ�************************************

#include "include.h"

//********************************���ر���**********************************
static u8  	HeartbeatTimeSpace = 0;//������ʱ����
static u32 	HeartbeatCount = 0;//����ʱ�����
//********************************��������**********************************
/*********************************************************************
//��������	:Heartbeat_ReportHeartbeat(void)
//����		:Heartbeat_ReportHeartbeat
//����		:�޲���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Heartbeat_ReportHeartbeat(void)
{    
    if(0==HeartbeatTimeSpace)return;	
    ///////////////
    HeartbeatCount++;
    ////////////////
    //if(Io_ReadIoStatusBit(IO_STATUS_BIT_SLEEP))return;//����״̬�Ͳ�������
    ////////////////
    if(HeartbeatCount<HeartbeatTimeSpace)return;
    ////////////////
    HeartbeatCount = 0;
    /////////////////////
    if(0==GetTerminalAuthorizationFlag())return;
    //////////////////////
    RadioProtocol_SendData(0x0002,NULL,0,02);
	
	APP_DEBUG("<--!!!��������!!!!-->\r\n");
    //RadioProtocol_TerminalHeartbeat();
}

/*********************************************************************
//��������	:Heartbeat_TimeTask(void)
//����		:ÿ��һ���������һ��������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
FunctionalState HeartBeat_TimeTask(void)
{ 
    Updata_SendResultNoteTimeTask();
    //Heartbeat_ReportHeartbeat();//����������
	return ENABLE;
}

/*********************************************************************
//��������	:HeartBeat_UpdatePram(void)
//����		:����������ʱ����
//����		:�޲���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void HeartBeat_UpdatePram(void)
{
	u8 Buffer[5] = {0};
	u8 PramLen;
	PramLen = EepromPram_ReadPram(E2_TERMINAL_HEARTBEAT_ID,Buffer);    
	if(E2_TERMINAL_HEARTBEAT_LEN == PramLen)
	{	
        HeartbeatTimeSpace = (u8)Public_ConvertBufferToLong(Buffer);
        /////////////////
        if(HeartbeatTimeSpace<10)HeartbeatTimeSpace=10;//��ֹ��������
        if(HeartbeatTimeSpace>180)HeartbeatTimeSpace=180;//
	}
	else
	{
		HeartbeatTimeSpace = 60;//Ĭ��ֵ,ʵ��Ӧ��ʹ��
	}
}

/*********************************************************************
//��������	:Heartbeat_ClrHeartbeatCount(void)
//����		:�������HeartbeatCount
//����		:�޲���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Heartbeat_ClrHeartbeatCount(void)
{
    HeartbeatCount = 0;
}
/*********************************************************************
//��������	:Heartbeat_GetHeartbeatTime(void)
//����		:�������HeartbeatCount
//����		:�޲���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
unsigned char Heartbeat_GetHeartbeatTime(void)
{
    return HeartbeatTimeSpace;
}
/*********************************************************************
//��������	:HeartBeat_ParamInit
//����		
//����		:�޲���
//���		:
*********************************************************************/
void HeartBeat_ParamInit(void)
{
    HeartbeatCount = 0;
    HeartBeat_UpdatePram();
    //SetTimerTask(TIME_HEART_BEAT,SECOND);
}
/******************************************************************************
**                            End Of File
******************************************************************************/

