/************************************************************************
//�������ƣ�Blind.c
//���ܣ���ģ��ʵ��ä���������ܡ�
//�汾�ţ�V0.1
//��Ȩ˵������Ȩ�����������������¼����������޹�˾
//�����ˣ�dxl
//����ʱ�䣺2014.10
//�汾��¼���汾��¼������汾���޸��ˡ��޸�ʱ�䡢�޸�ԭ���޸ļ�Ҫ˵��
//V0.1��ÿ�ΰ�5��λ����Ϣ���������ֻ���յ�ƽ̨Ӧ���ŻᲹ����һ����
//����һֱ������ǰ������෢��10�Σ�ÿ��20�롣��10�κ���Ȼû��Ӧ��
//���ȴ�30���Ӻ����ط���ǰ����
*************************************************************************/

/********************�ļ�����*************************/
#include <stdio.h>
#include <string.h>
#include "Blind.h"
#include "Public.h"
#include "RadioProtocol.h"
#include "report.h"


/********************�ⲿ����*************************/
extern u8 ShareBuffer[];
extern TIME_T CurTime;

#ifdef BLIND_SAVE_BY_QUEUE
static u8 reportAck = 0;
static CirQueueHandle_t pBlindCtrl = NULL;
static CirQueue_t BlindCtrlBackup;//����
static int iSemaphoreId;
static u8  blindProbe = 0;// ̽��

//���½ڵ���Ϣ
void Blind_UpdateCirQueueInfo(void);
QueueStatus Blind_IsCirQueueEmpty(CirQueueHandle_t xCirQueue);
static void Blind_NodeBackup(void);
static void Blind_NodeResume(void);

//ä�����ݴ���ϱ�
QueueStatus Blind_DataPackUploading(CirQueueHandle_t xCirQueue,u32 timeout);
//��ʼ��
void Blind_Init(void);

//ä�����ݴ洢�ṹ,ä����Ϣ�ǹ̶��洢��ַ��Ϊ�������趨
/*

____________________________________________________________________________
|ä����Ϣ|ä		��		��		��		ѭ		��		��		��	|
|_____________|_____________________________________________________________|
|			   |																|
|_____________|_____________________________________________________________|


*/
void proc_blind_task(s32 taskId)
{
	QueueStatus	upSta;
	u8 terminalAuthFlag;
	Blind_Init();
	while(1)
	{
		Blind_TakeSemaphore(TRUE); 	
		
		//ä���ϱ�
		terminalAuthFlag = GetTerminalAuthorizationFlag();
		APP_DEBUG("\r\n<-- terminalAuthFlag = %d -->\r\n",terminalAuthFlag);
		upSta = Blind_DataPackUploading(pBlindCtrl,BLIND_UP_OVERTIME);
		if(Q_SUCCESS == upSta)
		{
			APP_DEBUG("\r\n<-- ����ä���ϱ� ....-->\r\n");
		}
		else
		{
			CirQueue_t * const pxCirQueue = (CirQueue_t *)pBlindCtrl;
			APP_DEBUG("\r\n...ä���ϱ���� ...ʣ��ä������Ϊ:%d...head:%d...tail:%d\r\n", \
			pxCirQueue->queueLen.ulData,pxCirQueue->head.ulData,pxCirQueue->tail.ulData);
			//Blind_UpdateCirQueueInfo();
			Ql_Sleep(300000);//ÿ��5���Ӽ��һ�η�ֹ���µ�ä������
			if(terminalAuthFlag)
			{
				Blind_GiveSemaphore();
			}
		}
	}
}
/*********************************************************************************************************
*                                          Blind_DataSave
*
* Description : �洢ä������
*
* Argument(s) : pBuffer--->����ָ��
*			   Addr----->���ݵ�ַ
*			   length---->���ݳ���
*
* Return(s)   : none
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
void Blind_DataSave(u8 *pBuffer, u32 Addr,u32 length)
{
	sFLASH_WriteBuffer(pBuffer,Addr,length);//д��flash
}
/*********************************************************************************************************
*                                          Blind_IsCirQueueEmpty
*
* Description : �����пգ��������=0��Ϊ�գ���ʱû��Ԫ�ؿ��Գ���
*
* Argument(s) : ѭ������ָ��
*
* Return(s)   : ����״̬
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
QueueStatus Blind_IsCirQueueEmpty(CirQueueHandle_t xCirQueue)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	
  if (pxCirQueue->queueLen.ulData == 0)
  {
      return Q_SUCCESS;
  }
  return Q_FAILED;
}


/*********************************************************************************************************
*                                          Blind_IsCirQueueFull
*
* Description : �����������������=���������ʱ������Ԫ�����
*
* Argument(s) : ѭ������ָ��
*
* Return(s)   : ����״̬
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
QueueStatus Blind_IsCirQueueFull(CirQueueHandle_t xCirQueue)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	
  if (pxCirQueue->queueLen.ulData == pxCirQueue->queueCapacity.ulData)
  {
      return Q_SUCCESS;
  }
	return Q_FAILED;
}

/*********************************************************************************************************
*                                          Blind_GetCirQueueLength
*
* Description : ��ȡ���г���
*
* Argument(s) : ѭ������ָ��
*
* Return(s)   : ���г���
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int	Blind_GetCirQueueLength(CirQueueHandle_t xCirQueue)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	
	return	pxCirQueue->queueLen.ulData;
}

/*********************************************************************************************************
*                                          Blind_EnCirQueue
*
* Description : ��Ԫ�����
*
* Argument(s) : pBuffer:����ӵ�����ָ�� 
*								BufferLen:������ݳ���
*								pQ:ѭ������ָ��
*
* Return(s)   : ������ݳ���
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
QueueStatus Blind_EnCirQueue(u8 *pBuffer, u32 BufferLen,CirQueueHandle_t xCirQueue)
{
	u8* pBuf = pBuffer;
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;

	if((0 == BufferLen)||(NULL == pBuf))
	{
		return Q_FAILED;
	}
	
    if (Q_FAILED == Blind_IsCirQueueEmpty(xCirQueue))//ä����Ϊ��
	{
		if(pxCirQueue->tail.ulData == pxCirQueue->head.ulData)//���׷β
		{
		//ͷ��ǰ��,ä�����µ����ݸ�����
			pxCirQueue->head.ulData++;
			pxCirQueue->head.ulData = pxCirQueue->head.ulData % pxCirQueue->queueCapacity.ulData;
		}
	}
	//�洢����
	pxCirQueue->startAddr.ulData = FLASH_BLIND_AREA_SAVE_START_ADDR + pxCirQueue->tail.ulData*FLASH_BLIND_STEP_LEN;
	pxCirQueue->dataSaveLen.ulData = BufferLen;
	Ql_memmove(pBuf+4,pBuf,BufferLen);
	Ql_memcpy(pBuf,pxCirQueue->dataSaveLen.ucData,4);//���ݳ���ҲҪ�洢
	Blind_DataSave(pBuf,pxCirQueue->startAddr.ulData,BufferLen+4);
	APP_DEBUG("\r\n...д��ä����ַΪ:%dä������Ϊ:%d\r\n", pxCirQueue->startAddr.ulData,pxCirQueue->dataSaveLen.ulData);
	pxCirQueue->tail.ulData++;//����β������
	//��Ϊ�����ǻ��Σ�����tail��Ҫͨ��ȡģ��ʵ��ת�ص�0λ��
	pxCirQueue->tail.ulData = pxCirQueue->tail.ulData%pxCirQueue->queueCapacity.ulData;

	//�������û���ͼ�1������ͱ�������
	if (Q_FAILED == Blind_IsCirQueueFull(xCirQueue))
	{
	  pxCirQueue->queueLen.ulData++;
	}
	APP_DEBUG("\r\n...��ǰä������Ϊ:%d...head:%d...tail:%d\r\n", \
		pxCirQueue->queueLen.ulData,pxCirQueue->head.ulData,pxCirQueue->tail.ulData);
	//���½ڵ���Ϣ�ѽڵ���Ϣд��Flash,������Ϊ�˷�ֹ�쳣�ػ�����ä����ʧ���Ͼ�����ʤ��
	//if(0 == pxCirQueue->queueLen.ulData % 20)
	//Blind_UpdateCirQueueInfo();
	
	return Q_SUCCESS;
}

//����ä������
u8 Blind_Link1Save(u8 *pBuffer, u8 length, u8 attribute)
{
	Blind_EnCirQueue(pBuffer, length,pBlindCtrl);
	if(blindProbe)
	{
		blindProbe = 0;
	}
}

/*********************************************************************************************************
*                                          Blind_DeCirQueue
*
* Description : Ԫ�س���
*
* Argument(s) : pBuffer:���ӵ�����ָ�� 
*			   xCirQueue:ѭ������ָ��
*
* Return(s)   : �����е����ݳ���(�����峤��( 2�ֽ�)+ ������ĳ���)
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
u32 Blind_DeCirQueue(u8 *pBuffer,CirQueueHandle_t xCirQueue)
{
	BLIND_SAVE_STU temp;
	u8* pBuf = pBuffer;
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	if (Q_SUCCESS == Blind_IsCirQueueEmpty(xCirQueue))
	{
		//ǿ������
		//pxCirQueue->head.ulData = 0;
		//pxCirQueue->tail.ulData = 0;
	  	return 0;
	}
	
	//Ѱַ
	pxCirQueue->startAddr.ulData = FLASH_BLIND_AREA_SAVE_START_ADDR + pxCirQueue->head.ulData*FLASH_BLIND_STEP_LEN;
	//��ȡä�����ݳ���
	sFLASH_ReadBuffer(temp.dataSaveLen.ucData,pxCirQueue->startAddr.ulData,4);
	//APP_DEBUG("\r\n...��ȡä����ַΪ:%dä������Ϊ:%d\r\n", pxCirQueue->startAddr.ulData,temp.dataSaveLen.ulData);
	//��ȡä������
	*pBuf++ = temp.dataSaveLen.ucData[1];
	*pBuf++ = temp.dataSaveLen.ucData[0];
	sFLASH_ReadBuffer(pBuf,pxCirQueue->startAddr.ulData+4,temp.dataSaveLen.ulData);
	pxCirQueue->head.ulData++;
	//��Ϊ�����ǻ��Σ�����head��Ҫͨ��ȡģ��ʵ��ת�ص�0λ��
	pxCirQueue->head.ulData = pxCirQueue->head.ulData % pxCirQueue->queueCapacity.ulData;
	pxCirQueue->queueLen.ulData--; 
	
	return ( temp.dataSaveLen.ulData+2);//���������ֽڵ����ݳ���

}

typedef struct
{
	u16	itemNum;
	u32	blindDataLens;
}BLIND_PACK_INFO;
/*********************************************************************************************************
*                                          Blind_DataPack
*
* Description : ä�����ݴ��
*
* Argument(s) : pBuffer:���ӵ�����ָ�� 
*			   xCirQueue:ѭ������ָ��
*
* Return(s)   : �����������������ȵĽṹ��
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
BLIND_PACK_INFO Blind_DataPack(u8 *pBuffer,CirQueueHandle_t xCirQueue)
{
	u8 i;
	u8* pBuf = pBuffer;
	u32 dataLen=0;
	BLIND_PACK_INFO upInfo={0,0};
	//s32 ret;
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	
	for(i = 0;i < FLASH_BLIND_PACK_NUM;i++)
	{
		dataLen = Blind_DeCirQueue(pBuf,xCirQueue);
		pBuf += dataLen;
		if(dataLen)
		{
			upInfo.itemNum++;
		}
		upInfo.blindDataLens += dataLen;
	}
	
	return upInfo;
}


/*********************************************************************************************************
*                                          Blind_DataPackUploading
*
* Description : ä�����ݴ��
*
* Argument(s) : timeout:ä����ʱ���ص����
*			   xCirQueue:ѭ������ָ��
*
* Return(s)   : none
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
QueueStatus Blind_DataPackUploading(CirQueueHandle_t xCirQueue,u32 timeout)
{
	BLIND_PACK_INFO upInfo;
	s32	ret;
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	#if 1
	if (Q_SUCCESS == Blind_IsCirQueueEmpty(xCirQueue))
	{
		//ǿ������
		//pxCirQueue->head.ulData = 0;
		//pxCirQueue->tail.ulData = 0;
		//�������,�����ź���,����ä���ϱ�
		//�������,�����ź���,����ä���ϱ�
	  	return Q_FAILED;
	}
	#endif
	//backup
	Blind_NodeBackup();
	
	upInfo = Blind_DataPack(ShareBuffer+3,xCirQueue);
	
	ShareBuffer[0] = (u8)(upInfo.itemNum>>8);//�����ֽڱ�ʾ����
	ShareBuffer[1] = (u8)upInfo.itemNum;
	ShareBuffer[2] = 1;//0:������λ�������㱨;1:ä������
	//ShareBuffer[3] = (u8)(upInfo.blindDataLens>>8);//�����ֽ�λ�û㱨��Ϣ����
	//ShareBuffer[4] = (u8)upInfo.blindDataLens;
	APP_DEBUG("\r\n<-- Blind_Report %d   %d-->\r\n",upInfo.itemNum,upInfo.blindDataLens);
	RadioProtocol_PostionInformation_BulkUpTrans(ShareBuffer,upInfo.blindDataLens+3);

	//���½ڵ���Ϣ�ѽڵ���Ϣд��Flash,������Ϊ�˷�ֹ�쳣�ػ�����ä����ʧ���Ͼ�����ʤ��
	//Blind_UpdateCirQueueInfo();
	APP_DEBUG("\r\n...ʣ��ä������Ϊ:%d...head:%d...tail:%d\r\n", \
		pxCirQueue->queueLen.ulData,pxCirQueue->head.ulData,pxCirQueue->tail.ulData);
	// ����ä���ϱ��ص�������ʱ��,����
    //ret = Ql_Timer_Start(LOGIC_BLIND_REPORT_TMR_ID, timeout,FALSE);
	Ql_OS_SendMessage(main_task_id, MSG_ID_BLIND_CALLBACK, timeout, 0);
	blindProbe = 1;// ̽������
	return Q_SUCCESS;
}

static void Blind_NodeBackup(void)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)pBlindCtrl;

	Ql_memcpy((void*)&BlindCtrlBackup.head,(void*)&pxCirQueue->head,sizeof(BLIND_SAVE_STU));
}

static void Blind_NodeResume(void)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)pBlindCtrl;

	Ql_memcpy((void*)&pxCirQueue->head,(void*)&BlindCtrlBackup.head,sizeof(BLIND_SAVE_STU));
	//��ä���ϱ��ڼ���д��ä��Ҫ���⴦��
	if(0 == blindProbe)
	{
		if(pxCirQueue->tail.ulData == pxCirQueue->head.ulData)//���׷β
		{
		//ͷ��ǰ��,ä�����µ����ݸ�����
			pxCirQueue->head.ulData++;
			pxCirQueue->head.ulData = pxCirQueue->head.ulData % pxCirQueue->queueCapacity.ulData;
		}
		pxCirQueue->tail.ulData++;//����β������
		//��Ϊ�����ǻ��Σ�����tail��Ҫͨ��ȡģ��ʵ��ת�ص�0λ��
		pxCirQueue->tail.ulData = pxCirQueue->tail.ulData%pxCirQueue->queueCapacity.ulData;

		//�������û���ͼ�1������ͱ�������
		if (Q_FAILED == Blind_IsCirQueueFull(pBlindCtrl))
		{
		  pxCirQueue->queueLen.ulData++;
		}					
	}
}
//���½ڵ���Ϣ
void Blind_UpdateCirQueueInfo(void)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)pBlindCtrl;

	pxCirQueue->crc32Verify.ulData = Public_CRC_32( pxCirQueue->head.ucData, FLASH_BLIND_AREA_SAVE_START_ADDR - 4 );
	sFLASH_WriteBuffer(pxCirQueue->head.ucData,0,FLASH_BLIND_AREA_SAVE_START_ADDR);
}

/**************************************************************************
//��������Blind_Link1ReportAck
//���ܣ�����1ä������Ӧ����
//���룺��
//�������
//����ֵ����
//��ע���յ�����1��ä������Ӧ��ʱ����ô˺���
***************************************************************************/
void Blind_Link1ReportAck(void)
{
	reportAck = 1;
}

u8 Blind_GetLink1ReportAck(void)
{
	return reportAck;
}

void Blind_ClrLink1ReportAck(void)
{
	reportAck = 0;
}

//����һ���ź���
int Blind_CreateSemaphore(void)
{
	return Ql_OS_CreateSemaphore("MySemaphore", 1);
}
//�����ź���
void Blind_GiveSemaphore(void)
{
	Ql_OS_GiveSemaphore(iSemaphoreId);
}
//��ȡ�ź���
void Blind_TakeSemaphore(bool wait)
{
	Ql_OS_TakeSemaphore(iSemaphoreId,wait);
}

//ä���ϱ��ص�����
void Blind_Report_Callback_OnTimer(u32 timerId, void* param)
{
	static u8	shamLinkCount = 0;
	APP_DEBUG("Blind_Report_Callback_OnTimer......\r\n");
    if(LOGIC_BLIND_REPORT_TMR_ID == timerId)
	{
		if(0 == Blind_GetLink1ReportAck())//ä���ϱ�û��Ӧ
		{
			//resume
			Blind_NodeResume();
			//�������,�����ǲ���Ӧ��˵��������������ô��
			if(GetTerminalAuthorizationFlag())
			{
				if(shamLinkCount++ > 20)
				{					
					ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);
					Socket_Restart();//������tcp��ʼ������
					//Blind_UpdateCirQueueInfo();
					//Report_RtcWakeUpOffsetSave();
					//Ql_Reset(0);
				}
			}
		}
		else
		{
			shamLinkCount = 0;
			Blind_ClrLink1ReportAck();//���ä���ϱ�Ӧ���־
		}
		
		//�������,�����ź���,����ä���ϱ�
		if(GetTerminalAuthorizationFlag())
		{
			if (Q_SUCCESS == Blind_IsCirQueueEmpty(pBlindCtrl))
			{
				//���½ڵ���Ϣ�ѽڵ���Ϣд��Flash
				Blind_UpdateCirQueueInfo();
			}
			Blind_GiveSemaphore();
		}
		else
		{
			Blind_UpdateCirQueueInfo();
		}
		blindProbe = 0; //�������Ҫ��λ
	}
}
/*
�������������ʲк�����С��
���ʲк�����С�����ӷ�ʱ����ˮ�˼��ơ�
֦�����വ���٣����ĺδ��޷��ݡ�
ǽ����ǧǽ�����ǽ�����ˣ�ǽ�����Ц��
Ц�����������ģ�����ȴ�������ա�
*/
//��ȡä���ϱ�״̬
u8 Blind_GetReportState(void)
{
	return (u8)Blind_IsCirQueueEmpty(pBlindCtrl);	
}


/*********************************************************************************************************
*                                          Blind_CirQueueGenericCreate
*
* Description : ����һ��ѭ������
*
* Argument(s) : uxQueueLength:���е�����
*
* Return(s)   : ����Ķ���ָ��
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
CirQueueHandle_t Blind_CirQueueGenericCreate(const u32 uxQueueLength)
{
	CirQueue_t * pxCirQueue;

	pxCirQueue = (CirQueue_t *)Ql_MEM_Alloc(sizeof(CirQueue_t));
	if( pxCirQueue == NULL )
	{
		return NULL;
	}
	Ql_memset(pxCirQueue->head.ucData,0,FLASH_BLIND_AREA_SAVE_START_ADDR);
	pxCirQueue->startAddr.ulData = FLASH_BLIND_AREA_SAVE_START_ADDR;
	pxCirQueue->queueCapacity.ulData = uxQueueLength;

	return pxCirQueue;
}
/*
�������ɽ����
�����ѻ�֮ľ�����粻ϵ֮�ۡ�
����ƽ����ҵ�����ݻ������ݡ�
*/
//ע��һ����ʱ����������ä���ϱ�
void Blind_Timer_Register(void)
{	
	s32 ret = Ql_Timer_Register(LOGIC_BLIND_REPORT_TMR_ID, Blind_Report_Callback_OnTimer, NULL);
	APP_DEBUG("\r\nBlind_Timer_Register	%d......\r\n",ret);
}
/*********************************************************************************************************
*                                          Blind_Init
*
* Description : ä����ʼ��
*
* Argument(s) : 
*
* Return(s)   : 
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
void Blind_Init(void)
{
	BLIND_SAVE_STU temp;
	u32 crc32Verify;
	s32 ret;
	CirQueue_t * pxCirQueue;
	//����һ���ź���
	iSemaphoreId = Blind_CreateSemaphore();
	//����һ��������
	//iMutexId = Ql_OS_CreateMutex("MyMutex");
	//����ڵ�ռ�
	pBlindCtrl = Blind_CirQueueGenericCreate(FLASH_BLIND_AREA_SAVE_NUM);
	if(NULL == pBlindCtrl)
	{
		APP_DEBUG("blind init failed,System is about to restart......\r\n");	
		Ql_Reset(0);
	}
	APP_DEBUG("blind init success......\r\n");
	pxCirQueue = (CirQueue_t *)pBlindCtrl;
	
	sFLASH_ReadBuffer(temp.head.ucData,0,FLASH_BLIND_AREA_SAVE_START_ADDR);
	crc32Verify = Public_CRC_32( temp.head.ucData, FLASH_BLIND_AREA_SAVE_START_ADDR - 4 );
	//CRCУ��
	if(crc32Verify == temp.crc32Verify.ulData)
	{
		Ql_memcpy(pxCirQueue->head.ucData,temp.head.ucData,FLASH_BLIND_AREA_SAVE_START_ADDR);
		APP_DEBUG("Node check success......%04x    %04x...\r\n",crc32Verify,temp.crc32Verify.ulData);
	}
	else
	{
	//���½ڵ���Ϣ�ѽڵ���Ϣд��Flash
		Blind_UpdateCirQueueInfo();
	}
	//����һ��
	pxCirQueue->queueCapacity.ulData = FLASH_BLIND_AREA_SAVE_NUM;
	//���ͷ��ź���
	Blind_TakeSemaphore(FALSE);
}

#else

/********************���ر���*************************/
static BLIND_STRUCT Link1Blind;
static s16 Link1SearchSector;
static u16 Link1TimeCount = 0;
static u16 Link1WaitTimeCount = 0;



/********************���غ�������*********************/
static void Blind_Link1Init(void);//����1������ʼ��
static void Blind_Link1Search(void);//����1����ȷ��ä����¼���ϱ��ĳ�ʼλ��
static void Blind_Link1SearchSector(s16 SearchSector);//���ĳ�������е�ä����Ϣ
static u8 Blind_Link1Report(void);//����1�ϱ�һ��ä������
static u8 Blind_GetLink1ReportFlag(void);//��ȡ����1ä���ϱ���־


/********************��������*************************/
void proc_blind_task(s32 taskId)
{
	
	while(1)
	{
		Ql_Sleep(25);
		if(ProjectInitFinshFlag==1)		
		{
			Blind_TimeTask();
		}
//		SoftWdg_Feed(BIT2);
	}
}

//��ȡä���ϱ�״̬
u8 Blind_GetReportState(void)
{
	return Link1Blind.State;
}
/**************************************************************************
//��������Blind_TimeTask
//���ܣ�ʵ��ä����������
//���룺��
//�������
//����ֵ����
//��ע��ä��������ʱ����50ms����1�Σ������������Ҫ���ô˺�����
//���ڽ���FTP����ʱ��Ҫ�رո�������Ϊä����¼��̼�Զ����������һ��洢��
//ע�����˭�ر��˸�����˭��Ҫ����򿪸������ϵ�����Ĭ���Ǵ򿪵�
***************************************************************************/
FunctionalState Blind_TimeTask(void)
{
	/**************����1ä������****************/
	if(BLIND_STATE_INIT == Link1Blind.State)//��ʼ��������ر���
	{
		APP_DEBUG("\r\n<-- Blind_Link1Init -->\r\n");
		Blind_Link1Init();//��ɺ��Զ�ת��BLIND_STATE_SEARCH
	}
	else if(BLIND_STATE_SEARCH == Link1Blind.State)//�𲽲���ȷ����ǰ��ä������λ��
	{
	
		//APP_DEBUG("\r\n<-- Blind_Link1Search -->\r\n");
		Blind_Link1Search();//��ɺ��Զ�ת��BLIND_STATE_REPORT
	}
	else if(BLIND_STATE_REPORT == Link1Blind.State)//ä���ϱ�
	{
		Link1WaitTimeCount = 0;	
		if(0x01 == (0x01&GetTerminalAuthorizationFlag()))//ֻ������1����ʱ�Ų���
		{
			Link1TimeCount++;
			if(Link1TimeCount >= BLIND_REPORT_DELAY_TIME)//20��
			{
				Link1TimeCount = 0;	
				Link1Blind.ReportSendNum++;
				if((Link1Blind.ReportSendNum > 0)&&(Link1Blind.ReportSendNum <= 10))
				{
					if(0 != Link1Blind.LastReportSector[0])
					{
						Link1Blind.ReportSector = Link1Blind.LastReportSector[0];
						Link1Blind.ReportStep = Link1Blind.LastReportStep[0];
					}
					if(0 == Blind_Link1Report())//ä���ϱ�
					{
						//Blind_StartLink1Report();
						Link1Blind.State = BLIND_STATE_WAIT;//ת��ȴ�״̬
					}
				}
				else if(Link1Blind.ReportSendNum > 10)
				{
					Link1Blind.State = BLIND_STATE_WAIT;//ת��ȴ�״̬
				}
			}
		}
	}
	else if(BLIND_STATE_WAIT == Link1Blind.State)//�ȴ�
	{
		Link1WaitTimeCount++;
		if(Link1WaitTimeCount >= 36000)//30����
		{
			Link1WaitTimeCount = 0;
			Link1Blind.State = BLIND_STATE_INIT;
		}
	}
	else//�쳣
	{
		Link1Blind.State = BLIND_STATE_INIT;
	}
	
	return ENABLE;
}


/**************************************************************************
//��������Blind_Link1Init
//���ܣ�����1������ʼ��
//���룺��
//�������
//����ֵ����
//��ע��ä��������ʱ������Ҫ���ô˺���
***************************************************************************/
static void Blind_Link1Init(void)
{
	u8 i;

	Link1Blind.StartSector = FLASH_BLIND_START_SECTOR;
	Link1Blind.EndSector = FLASH_BLIND_END_SECTOR;

	Link1Blind.SectorStep = FLASH_ONE_SECTOR_BYTES/FLASH_BLIND_STEP_LEN;

	Link1Blind.OldestSector = Link1Blind.StartSector;
	Link1Blind.OldestStep = 0;
	//Link1Blind.OldestTimeCount = 0;//������Сֵ
	Link1Blind.OldestTimeCount = 0xFFFFFFFF;//������Сֵ
	Link1Blind.SaveSector = Link1Blind.StartSector;
	Link1Blind.SaveStep = 0;
	Link1Blind.SaveTimeCount = 0;//������Сֵ
	Link1Blind.SaveEnableFlag = 0;
	Link1Blind.ReportSendNum = 0;
	for(i=0; i<5; i++)
	{
		Link1Blind.LastReportSector[i] = 0;
		Link1Blind.LastReportStep[i] = 0;
	}
	Link1Blind.ErrorCount = 0;
	Link1Blind.State = BLIND_STATE_SEARCH;

	Link1TimeCount = 0;
	Link1WaitTimeCount = 0;	
	Link1SearchSector = Link1Blind.StartSector;
}

/**************************************************************************
//��������Blind_Link1Search
//���ܣ�����1����ȷ��ä����¼���ϱ��ĳ�ʼλ��
//���룺��
//�������
//����ֵ����
//��ע��ä��������ʱ������Ҫ���ô˺���
***************************************************************************/
static void Blind_Link1Search(void)
{
	if((Link1SearchSector >= Link1Blind.StartSector)&&(Link1SearchSector < Link1Blind.EndSector))
	{
		Blind_Link1SearchSector(Link1SearchSector);//��������1ĳ�������е�ÿ��ä����Ϣ
	}
	else
	{
		Link1Blind.State = BLIND_STATE_INIT;
		return ;
	}
	
	if(Link1Blind.ErrorCount > 3)
	{
		Link1Blind.ErrorCount = 0;
		Blind_Link1Erase();
		return ;
	}

	Link1SearchSector++;
	if(Link1SearchSector >= Link1Blind.EndSector)
	{
		if((Link1Blind.StartSector == Link1Blind.SaveSector)&&(0 == Link1Blind.SaveStep)&&(0 == Link1Blind.SaveTimeCount))
		{
			//ä������Ϊ��
		}
		else
		{
			Link1Blind.SaveStep++;
			if(Link1Blind.SaveStep >= Link1Blind.SectorStep)
			{
				Link1Blind.SaveStep = 0;
				Link1Blind.SaveSector++;
				if(Link1Blind.SaveSector >= Link1Blind.EndSector)
				{
					Link1Blind.SaveSector = Link1Blind.StartSector;
				}
			}
		}
		Link1Blind.ReportSector = Link1Blind.OldestSector;
		Link1Blind.ReportStep = Link1Blind.OldestStep;
		Link1Blind.SaveEnableFlag = 1;
		Link1Blind.State = BLIND_STATE_REPORT;
	}
}


/**************************************************************************
//��������Blind_Link1SearchSector
//���ܣ����ĳ�������е�ä����Ϣ
//���룺������
//�������ǰ�洢�Ͳ���ä����λ��
//����ֵ����
//��ע��
***************************************************************************/
static void Blind_Link1SearchSector(s16 SearchSector)
{
	s16 i,j;
	u32 Addr;
	u8 flag;
	u8 Buffer[FLASH_BLIND_STEP_LEN+1];
	u8 sum;
	TIME_T tt;
	u32 ttCount;

	Addr = SearchSector*FLASH_ONE_SECTOR_BYTES + Link1Blind.SectorStep*FLASH_BLIND_STEP_LEN + 1;
	sFLASH_ReadBuffer(&flag,Addr,1);//�����������Ƿ��Ѳ�����־
	if(0xaa == flag)
	{
		Addr = SearchSector*FLASH_ONE_SECTOR_BYTES + (Link1Blind.SectorStep-1)*FLASH_BLIND_STEP_LEN;
		sFLASH_ReadBuffer(Buffer,Addr,4);
		ttCount = Public_ConvertBufferToLong(Buffer);
		Gmtime(&tt,ttCount);
		if(SUCCESS == CheckTimeStruct(&tt))
		{
			if(ttCount > Link1Blind.OldestTimeCount)
			{
				Link1Blind.OldestSector = SearchSector;
				Link1Blind.OldestStep = Link1Blind.SectorStep-1;
				Link1Blind.OldestTimeCount = ttCount;
			}
		}
		return ;
	}

	Addr = SearchSector*FLASH_ONE_SECTOR_BYTES + (Link1Blind.SectorStep-1)*FLASH_BLIND_STEP_LEN;//�����������1��
	sFLASH_ReadBuffer(Buffer,Addr,4);
	ttCount = Public_ConvertBufferToLong(Buffer);
	if(0xffffffff != ttCount)
	{
		Gmtime(&tt,ttCount);
		if(SUCCESS == CheckTimeStruct(&tt))
		{
			sFLASH_ReadBuffer(Buffer,Addr,FLASH_BLIND_STEP_LEN);
			if(CHANNEL_DATA_1 != Buffer[4])//ͨ������ȷ
			{
				Link1Blind.ErrorCount++;
				if(Link1Blind.ErrorCount > 3)
				{
					Link1Blind.ErrorCount = 0;
					Blind_Link1Erase();//��������ä���洢����
					return ;
				}	
			}
			sum = 0;
			for(j=0; j<Buffer[6]; j++)
			{
				sum += Buffer[j+7];
			}
			if((sum != Buffer[5])||(0 == Buffer[6]))
			{
				Link1Blind.ErrorCount++;
				if(Link1Blind.ErrorCount > 3)
				{
					Link1Blind.ErrorCount = 0;
					Blind_Link1Erase();//��������ä���洢����
					return ;
				}
			}
			else
			{
				/*
				APP_DEBUG("\r\n<-- Fun(Blind_Link1SearchSector) tt year=%d,month=%d,day=%d,hour=%d,min=%d,sec=%d -->\r\n",tt.year,tt.month,tt.day,tt.hour,tt.min,tt.sec);
				APP_DEBUG("\r\n<-- Fun(Blind_Link1SearchSector) Link1Blind.OldestTimeCount=0X%x -->\r\n",Link1Blind.OldestTimeCount);
				BlindTextDebug(Buffer,FLASH_BLIND_STEP_LEN);
				*/
				//if((ttCount > Link1Blind.OldestTimeCount)&&(0xaa == Buffer[FLASH_BLIND_STEP_LEN-1]))
				//if((ttCount > Link1Blind.OldestTimeCount))
				if((ttCount < Link1Blind.OldestTimeCount))
				{
					Link1Blind.OldestSector = SearchSector;
					Link1Blind.OldestStep = 0;
					Link1Blind.OldestTimeCount = ttCount;
				}

				if(ttCount > Link1Blind.SaveTimeCount)
				{
					Link1Blind.SaveSector = SearchSector;
					Link1Blind.SaveStep = Link1Blind.SectorStep-1;
					Link1Blind.SaveTimeCount = ttCount;
				}
			}
		}
		else
		{
			Link1Blind.ErrorCount++;
			if(Link1Blind.ErrorCount > 3)
			{
				Link1Blind.ErrorCount = 0;
				Blind_Link1Erase();//��������ä���洢����
				return ;
			}
		}
	}
	else
	{
		Addr = SearchSector*FLASH_ONE_SECTOR_BYTES;//�������ĵ�1��
		sFLASH_ReadBuffer(Buffer,Addr,4);
		ttCount = Public_ConvertBufferToLong(Buffer);
		if(0xffffffff != ttCount)
		{
			for(i=0; i<Link1Blind.SectorStep; i++)
			{
				Addr = SearchSector*FLASH_ONE_SECTOR_BYTES+i*FLASH_BLIND_STEP_LEN;
				sFLASH_ReadBuffer(Buffer,Addr,FLASH_BLIND_STEP_LEN);
				ttCount = Public_ConvertBufferToLong(Buffer);
				if(0xffffffff == ttCount)
				{
					break;
				}
				Gmtime(&tt,ttCount);
				if(SUCCESS == CheckTimeStruct(&tt))
				{
					if(CHANNEL_DATA_1 != Buffer[4])//ͨ������ȷ
					{
						Link1Blind.ErrorCount++;
						if(Link1Blind.ErrorCount > 3)
						{
							Link1Blind.ErrorCount = 0;
							Blind_Link1Erase();//��������ä���洢����
							return ;
						}	
					}
					sum = 0;
					for(j=0; j<Buffer[6]; j++)
					{
						sum += Buffer[j+7];
					}
					if((sum != Buffer[5])||(0 == Buffer[6]))
					{
						Link1Blind.ErrorCount++;
						if(Link1Blind.ErrorCount > 3)
						{
							Link1Blind.ErrorCount = 0;
							Blind_Link1Erase();//��������ä���洢����
							return ;
						}
					}
					else
					{
						if((ttCount > Link1Blind.OldestTimeCount)&&(0xaa == Buffer[FLASH_BLIND_STEP_LEN-1]))
						{
							Link1Blind.OldestSector = SearchSector;
							Link1Blind.OldestStep = i;
							Link1Blind.OldestTimeCount = ttCount;
						}

						if(ttCount > Link1Blind.SaveTimeCount)
						{
							Link1Blind.SaveSector = SearchSector;
							Link1Blind.SaveStep = i;
							Link1Blind.SaveTimeCount = ttCount;
						}
					}
				}
				else
				{
					Link1Blind.ErrorCount++;
					if(Link1Blind.ErrorCount > 3)
					{
						Link1Blind.ErrorCount = 0;
						Blind_Link1Erase();//��������ä���洢����
						return ;
					}
				}
			}
		}
	}

/*ÿ������ÿһ������飬̫�˷�ʱ�䣬�Ż����ֻ����1�������1��
	Link1Blind.ErrorCount = 0;
	for(i=0; i<Link1Blind.SectorStep; i++)
	{
		Addr = SearchSector*FLASH_ONE_SECTOR_BYTES + i*FLASH_BLIND_STEP_LEN;
		sFLASH_ReadBuffer(Buffer,Addr,4);
		ttCount = Public_ConvertBufferToLong(Buffer);
		if(0xffffffff != ttCount)
		{
			Gmtime(&tt,ttCount);
			if(SUCCESS == CheckTimeStruct(&tt))
			{
				sFLASH_ReadBuffer(Buffer,Addr,FLASH_BLIND_STEP_LEN);
				if(CHANNEL_DATA_1 != Buffer[4])//ͨ������ȷ
				{
					Link1Blind.ErrorCount++;
					if(Link1Blind.ErrorCount > 3)
					{
						Link1Blind.ErrorCount = 0;
						Blind_Erase(0xff);//��������ä���洢����
						break;
					}	
				}
				sum = 0;
				for(j=0; j<Buffer[6]; j++)
				{
					sum += Buffer[j+7];
				}
				if((sum != Buffer[5])||(0 == Buffer[6]))
				{
					Link1Blind.ErrorCount++;
				}
				else
				{
					if((ttCount > Link1Blind.OldestTimeCount)&&(0xaa == Buffer[FLASH_BLIND_STEP_LEN-1]))
					{
						Link1Blind.OldestSector = SearchSector;
						Link1Blind.OldestStep = i;
						Link1Blind.OldestTimeCount = ttCount;
					}

					if(ttCount > Link1Blind.SaveTimeCount)
					{
						Link1Blind.SaveSector = SearchSector;
						Link1Blind.SaveStep = i;
						Link1Blind.SaveTimeCount = ttCount;
					}
				}
			}
			else
			{
				Link1Blind.ErrorCount++;
			}
		}
	}
*/
	
}
/**************************************************************************
//��������Blind_Link1Report
//���ܣ�����1�ϱ�һ��ä������
//���룺��
//�������
//����ֵ����
//��ע��ä��������ʱ������Ҫ���ô˺���
***************************************************************************/
static u8 Blind_Link1Report(void)
{
	u32 Addr;
	u8 i;
	u8 count = 0;
	u8 flag;
	u8 Buffer[FLASH_BLIND_STEP_LEN+1];
	u8 BufferLen;
	u8 sum;
	u16 length = 3;//Ԥ��3���ֽ�


	for(count=0; count<5; )//ÿ������5��λ����Ϣ
	{
		if(0 == Blind_GetLink1ReportFlag())
		{
			break;
		}
		Addr = Link1Blind.ReportSector*FLASH_ONE_SECTOR_BYTES + Link1Blind.SectorStep*FLASH_BLIND_STEP_LEN + 1;
		sFLASH_ReadBuffer(&flag,Addr,1);//�����������Ƿ��Ѳ�����־
		if(0xaa == flag)//�Ѳ�����
		{
			Link1Blind.ReportSector++;
			Link1Blind.ReportStep = 0;
			if(Link1Blind.ReportSector >= Link1Blind.EndSector)
			{
				Link1Blind.ReportSector = Link1Blind.StartSector;
			}
		}
		else
		{
			for(;count<5;)
			{
				if(0 == Blind_GetLink1ReportFlag())
				{
					break;
				}
				Addr = Link1Blind.ReportSector*FLASH_ONE_SECTOR_BYTES + (Link1Blind.ReportStep+1)*FLASH_BLIND_STEP_LEN - 1;
				sFLASH_ReadBuffer(&flag,Addr,1);//�������Ƿ��Ѳ�����־
				if(0xaa == flag)//�Ѳ�����
				{
					Link1Blind.ReportStep++;
				}
				else
				{
					Addr = Link1Blind.ReportSector*FLASH_ONE_SECTOR_BYTES + Link1Blind.ReportStep*FLASH_BLIND_STEP_LEN;
					sFLASH_ReadBuffer(Buffer,Addr,FLASH_BLIND_STEP_LEN);
					BufferLen = Buffer[6];
					sum = 0;
					for(i=0; i<BufferLen; i++)
					{
						sum += Buffer[7+i];
					}
					if(Buffer[5] == sum)
					{
						if(((length+BufferLen+2) < RADIOPROTOCOL_BUFFER_SIZE)&&(BufferLen < (FLASH_BLIND_STEP_LEN-7)))
						{
							ShareBuffer[length] = 0;//���ȸ��ֽ��0
							memcpy(ShareBuffer+length+1,Buffer+6,BufferLen+1);
							length += (BufferLen+2);
							Link1Blind.LastReportSector[count] = Link1Blind.ReportSector;
							Link1Blind.LastReportStep[count] = Link1Blind.ReportStep;
							count++;
						}
						
					}
					Link1Blind.ReportStep++;
				}
				if(Link1Blind.ReportStep >= Link1Blind.SectorStep)
				{
					Link1Blind.ReportSector++;
					Link1Blind.ReportStep = 0;
					if(Link1Blind.ReportSector >= Link1Blind.EndSector)
					{
						Link1Blind.ReportSector = Link1Blind.StartSector;
					}
					break;
				}
			}
		}
	}
	
	if(0 == count)
	{
		return 0;
	}
	else
	{//APP_DEBUG("\r\n....fuck off.....\r\n");
		ShareBuffer[0] = 0;//�����ֽڱ�ʾ����
		ShareBuffer[1] = count;
		ShareBuffer[2] = 1;//0:������λ�������㱨;1:ä������
		APP_DEBUG("\r\n<-- Blind_Report -->\r\n");
		RadioProtocol_PostionInformation_BulkUpTrans(ShareBuffer,length);
	}

	return 1;
}
/**************************************************************************
//��������Blind_Link1ReportAck
//���ܣ�����1ä������Ӧ����
//���룺��
//�������
//����ֵ����
//��ע���յ�����1��ä������Ӧ��ʱ����ô˺���
***************************************************************************/
void Blind_Link1ReportAck(void)
{
	u8 i;
	u8 flag;
	u32 Addr;
	s16 Sector;

	Link1Blind.ReportSendNum = 0;
	Link1TimeCount = BLIND_REPORT_DELAY_TIME;//1����ϱ���һ��

	Sector = Link1Blind.LastReportSector[0];

	for(i=0; i<5; i++)
	{
		if((Link1Blind.LastReportSector[i] >= Link1Blind.StartSector)
			&&(Link1Blind.LastReportSector[i] < Link1Blind.EndSector)
				&&(Link1Blind.LastReportStep[i] < Link1Blind.SectorStep))
		{
			Addr = Link1Blind.LastReportSector[i]*FLASH_ONE_SECTOR_BYTES+(Link1Blind.LastReportStep[i]+1)*FLASH_BLIND_STEP_LEN - 1;
			flag = 0xaa;
			sFLASH_WriteBuffer(&flag, Addr, 1);//��Ǹ���ä�����ϱ�

			if(Link1Blind.LastReportStep[i] >= (Link1Blind.SectorStep-1))
			{
				Addr = Sector*FLASH_ONE_SECTOR_BYTES+Link1Blind.SectorStep*FLASH_BLIND_STEP_LEN + 1;
				flag = 0xaa;
				sFLASH_WriteBuffer(&flag, Addr, 1);//��Ǹ�������ä�����ϱ�
			}
		}
		
	}

	for(i=0; i<5; i++)
	{
		Link1Blind.LastReportSector[i] = 0;
		Link1Blind.LastReportStep[i] = 0;
	}
	
		
}
/**************************************************************************
//��������Blind_GetLink1ReportFlag
//���ܣ���ȡ����1ä���ϱ���־
//���룺��
//�������
//����ֵ��ä���ϱ���־
//��ע��1��ʾ��Ҫ�ϱ���0��ʾ����Ҫ�ϱ����ն�ÿ��10���ӻ��Զ����һ�ο��Ƿ���Ҫ�ϱ�
***************************************************************************/
static u8 Blind_GetLink1ReportFlag(void)
{
	s16 Sector;
	s16 Step;

	Sector = Link1Blind.SaveSector;
	Step = 	Link1Blind.SaveStep;
/*
	if(0 != Link1Blind.SaveTimeCount)
	{
		Step--;
		if(Step < 0)
		{
			Step = Link1Blind.SectorStep-1;
			Sector--;
			if(Sector < Link1Blind.StartSector)
			{
				Sector = Link1Blind.EndSector - 1;
			}
		}
	}
	*/
	if((Link1Blind.ReportSector == Sector)&&(Link1Blind.ReportStep >= Step))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/**************************************************************************
//��������Blind_StartLink1Report
//���ܣ���������1ä������
//���룺��
//�������
//����ֵ����
//��ע������ä��������ÿ������1��Ȩ�ɹ�����ô˺���
***************************************************************************/
void Blind_StartLink1Report(void)
{
	//if(BLIND_STATE_WAIT == Link1Blind.State)
	//{
		Link1Blind.State = BLIND_STATE_INIT;
	//}
}
/**************************************************************************
//��������Blind_Link1Erase
//���ܣ���������1ä���洢����
//���룺��
//�������
//����ֵ����
//��ע����⵽ä����¼��λ����Ϣ����ʱ���������������50������ô˺�����
//������Զ��FTP������������ʱ����ô˺�������Ϊä����Զ�̹̼���������һ������
***************************************************************************/
void Blind_Link1Erase(void)
{
	u16 i;
	u32 Addr;
	
	Link1Blind.SaveEnableFlag = 0;//��ֹ�洢
	Link1Blind.State = BLIND_STATE_INIT;

	for(i=Link1Blind.StartSector; i<Link1Blind.EndSector; i++)
	{
		Addr = i*FLASH_ONE_SECTOR_BYTES;
		sFLASH_EraseSector(Addr);
	}	
}
/**************************************************************************
//��������Blind_Link1Save
//���ܣ��洢һ������1��ä������
//���룺һ��λ����Ϣ������
//�������
//����ֵ��0Ϊ�ɹ�����0Ϊʧ�ܣ����ȳ���ʱ�᷵��ʧ��
//��ע��һ��ä�����ݰ�����ʱ��4�ֽ�+����1�ֽڣ�Ԥ����+У���1�ֽ�+λ����Ϣ����1�ֽ�+λ����Ϣ�����ֽ�
***************************************************************************/
u8 Blind_Link1Save(u8 *pBuffer, u8 length, u8 attribute)
{

	u8 Buffer[FLASH_BLIND_STEP_LEN+1] = {0};
	u8 i,j;
	u32 Addr;
	u32 TimeCount=0;
	u8 *p = NULL;
	u8 sum;
	s16 Sector;
	s16 Step;
	TIME_T tt;
	u32 ttCount;

	//if(ReportStatus_On != Report_GetReportStatus())return 1;//û���߲�дä��
	APP_DEBUG("\r\n<-- Blind_Link1Save -->\r\n");
	if((length > (FLASH_BLIND_STEP_LEN-9))||(length < 28)||(0 == Link1Blind.SaveEnableFlag))//ÿ����������ֽ����������ǣ����ϴ����
	{
		return 1;
	}
	
	if((Link1Blind.StartSector == Link1Blind.SaveSector)&&(0 == Link1Blind.SaveStep)&&(0 == Link1Blind.SaveTimeCount))//Ϊ��
	{
			
	}
	else//��Ϊ�գ��Ѵ�������
	{
		Sector = Link1Blind.SaveSector;
		Step = Link1Blind.SaveStep;
		APP_DEBUG("\r\n<-- Link1Blind.SaveSector=%d, Link1Blind.SaveStep=%d-->\r\n",Link1Blind.SaveSector,Link1Blind.SaveStep);
		Step--;//��ȡ�Ѵ洢�����µ���һ��ʱ��
		if(Step < 0)
		{
			Step = Link1Blind.SectorStep - 1;
			Sector--;
			if(Sector < Link1Blind.StartSector)
			{
				Sector = Link1Blind.EndSector - 1;
			}
		}
		Addr = Sector*FLASH_ONE_SECTOR_BYTES+Step*FLASH_BLIND_STEP_LEN;
		sFLASH_ReadBuffer(Buffer,Addr,4);
		ttCount = Public_ConvertBufferToLong(Buffer);
		Gmtime(&tt,ttCount);

		if(ERROR == CheckTimeStruct(&tt))//����Ѵ洢�����µ���һ��ʱ���Ƿ���ȷ
		{
			sFLASH_EraseSector(Addr);
			Link1Blind.SaveStep = 0;
			
			APP_DEBUG("\r\n<-- Blind_Link1Save CheckTime SaveTime Error -->\r\n");
			return 1;
		}

		if(ERROR == CheckTimeStruct(&CurTime))//��鵱ǰʱ���Ƿ���ȷ
		{
		
			APP_DEBUG("\r\n<-- Blind_Link1Save CheckTime CurrentTime Error-->\r\n");
			return 1;
		}

		TimeCount = ConverseGmtime(&CurTime);//�Ƚϵ�ǰʱ�������¼�¼����һ��ʱ��
		
		APP_DEBUG("<--Blind Ql_GetLocalTime(%d.%02d.%02d %02d:%02d:%02d) -->\n\r", CurTime.year, CurTime.month, CurTime.day, CurTime.hour, CurTime.min, CurTime.sec);
		if(TimeCount < ttCount)
		{
		
			APP_DEBUG("\r\n<-- Blind_Link1Save CheckTime CurrentTime less SaveTime Error-->\r\n");
			return 1;
		}
		
	}
	
	Addr = Link1Blind.SaveSector*FLASH_ONE_SECTOR_BYTES+Link1Blind.SaveStep*FLASH_BLIND_STEP_LEN;//��鵱ǰ�洢����һ���ĵ�ַ�Ƿ�Ϊ��
	
	APP_DEBUG("\r\n<-- Link1Blind.SaveSector=%d, Link1Blind.SaveStep=%d-->\r\n",Link1Blind.SaveSector,Link1Blind.SaveStep);
	
	sFLASH_ReadBuffer(Buffer,Addr,5);
	for(i=0; i<5; i++)
	{
		if(0xff != Buffer[i])
		{
			sFLASH_EraseSector(Addr);
			Link1Blind.SaveStep = 0;
			Addr = Link1Blind.SaveSector*FLASH_ONE_SECTOR_BYTES;
			for(j=0; j<200; j++)
			{
				//�ȴ�һ���
			}
			break;
		}
	}

	sum = 0;//����У���
	for(i=0; i<length; i++)
	{
		sum += *(pBuffer+i);
	}

	
	memcpy(Buffer+7,pBuffer,length);//����λ����Ϣ������

	p = Buffer;//����7�ֽڵ�����
	*p++ = (TimeCount&0xff000000) >> 24;
	*p++ = (TimeCount&0xff0000) >> 16;
	*p++ = (TimeCount&0xff00) >> 8;
	*p++ = TimeCount&0xff;
	*p++ = attribute;
	*p++ = sum;
	*p++ = length;

	sFLASH_WriteBuffer(Buffer,Addr,length+7);//д��flash

	Link1Blind.SaveStep++;//����һ��
	if(Link1Blind.SaveStep >= Link1Blind.SectorStep)
	{
		Link1Blind.SaveSector++;
		Link1Blind.SaveStep = 0;
		if(Link1Blind.SaveSector >= Link1Blind.EndSector)
		{
			Link1Blind.SaveSector = Link1Blind.StartSector;
		}
		Addr = Link1Blind.SaveSector*FLASH_ONE_SECTOR_BYTES;
		sFLASH_EraseSector(Addr);
	}
	return 0;
	
}

#endif


















