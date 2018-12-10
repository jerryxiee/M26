/************************************************************************
//程序名称：Blind.c
//功能：该模块实现盲区补报功能。
//版本号：V0.1
//版权说明：版权属于深圳市伊爱高新技术开发有限公司
//开发人：dxl
//开发时间：2014.10
//版本记录：版本记录需包含版本、修改人、修改时间、修改原因、修改简要说明
//V0.1：每次把5条位置信息打包补报，只有收到平台应答后才会补报下一包，
//否则一直补报当前包，最多发送10次，每次20秒。若10次后仍然没有应答
//则会等待30分钟后再重发当前包。
*************************************************************************/

/********************文件包含*************************/
#include <stdio.h>
#include <string.h>
#include "Blind.h"
#include "Public.h"
#include "RadioProtocol.h"
#include "report.h"


/********************外部变量*************************/
extern u8 ShareBuffer[];
extern TIME_T CurTime;

#ifdef BLIND_SAVE_BY_QUEUE
static u8 reportAck = 0;
static CirQueueHandle_t pBlindCtrl = NULL;
static CirQueue_t BlindCtrlBackup;//副本
static int iSemaphoreId;
static u8  blindProbe = 0;// 探针

//更新节点信息
void Blind_UpdateCirQueueInfo(void);
QueueStatus Blind_IsCirQueueEmpty(CirQueueHandle_t xCirQueue);
static void Blind_NodeBackup(void);
static void Blind_NodeResume(void);

//盲区数据打包上报
QueueStatus Blind_DataPackUploading(CirQueueHandle_t xCirQueue,u32 timeout);
//初始化
void Blind_Init(void);

//盲区数据存储结构,盲区信息是固定存储地址，为检索而设定
/*

____________________________________________________________________________
|盲区信息|盲		区		数		据		循		环		存		储	|
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
		
		//盲区上报
		terminalAuthFlag = GetTerminalAuthorizationFlag();
		APP_DEBUG("\r\n<-- terminalAuthFlag = %d -->\r\n",terminalAuthFlag);
		upSta = Blind_DataPackUploading(pBlindCtrl,BLIND_UP_OVERTIME);
		if(Q_SUCCESS == upSta)
		{
			APP_DEBUG("\r\n<-- 继续盲区上报 ....-->\r\n");
		}
		else
		{
			CirQueue_t * const pxCirQueue = (CirQueue_t *)pBlindCtrl;
			APP_DEBUG("\r\n...盲区上报完成 ...剩余盲区条数为:%d...head:%d...tail:%d\r\n", \
			pxCirQueue->queueLen.ulData,pxCirQueue->head.ulData,pxCirQueue->tail.ulData);
			//Blind_UpdateCirQueueInfo();
			Ql_Sleep(300000);//每过5分钟检测一次防止有新的盲区产生
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
* Description : 存储盲区数据
*
* Argument(s) : pBuffer--->数据指针
*			   Addr----->数据地址
*			   length---->数据长度
*
* Return(s)   : none
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
void Blind_DataSave(u8 *pBuffer, u32 Addr,u32 length)
{
	sFLASH_WriteBuffer(pBuffer,Addr,length);//写入flash
}
/*********************************************************************************************************
*                                          Blind_IsCirQueueEmpty
*
* Description : 队列判空，如果长度=0则为空，这时没有元素可以出队
*
* Argument(s) : 循环队列指针
*
* Return(s)   : 队列状态
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
* Description : 队列判满，如果长度=容量，则此时不能有元素入队
*
* Argument(s) : 循环队列指针
*
* Return(s)   : 队列状态
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
* Description : 获取队列长度
*
* Argument(s) : 循环队列指针
*
* Return(s)   : 队列长度
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
* Description : 多元素入队
*
* Argument(s) : pBuffer:待入队的数据指针 
*								BufferLen:入队数据长度
*								pQ:循环队列指针
*
* Return(s)   : 入队数据长度
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
	
    if (Q_FAILED == Blind_IsCirQueueEmpty(xCirQueue))//盲区不为空
	{
		if(pxCirQueue->tail.ulData == pxCirQueue->head.ulData)//如果追尾
		{
		//头部前进,盲区被新的数据覆盖了
			pxCirQueue->head.ulData++;
			pxCirQueue->head.ulData = pxCirQueue->head.ulData % pxCirQueue->queueCapacity.ulData;
		}
	}
	//存储数据
	pxCirQueue->startAddr.ulData = FLASH_BLIND_AREA_SAVE_START_ADDR + pxCirQueue->tail.ulData*FLASH_BLIND_STEP_LEN;
	pxCirQueue->dataSaveLen.ulData = BufferLen;
	Ql_memmove(pBuf+4,pBuf,BufferLen);
	Ql_memcpy(pBuf,pxCirQueue->dataSaveLen.ucData,4);//数据长度也要存储
	Blind_DataSave(pBuf,pxCirQueue->startAddr.ulData,BufferLen+4);
	APP_DEBUG("\r\n...写入盲区地址为:%d盲区长度为:%d\r\n", pxCirQueue->startAddr.ulData,pxCirQueue->dataSaveLen.ulData);
	pxCirQueue->tail.ulData++;//队列尾部递增
	//因为队列是环形，所以tail需要通过取模来实现转回到0位置
	pxCirQueue->tail.ulData = pxCirQueue->tail.ulData%pxCirQueue->queueCapacity.ulData;

	//如果队列没满就加1，否则就保持满吧
	if (Q_FAILED == Blind_IsCirQueueFull(xCirQueue))
	{
	  pxCirQueue->queueLen.ulData++;
	}
	APP_DEBUG("\r\n...当前盲区条数为:%d...head:%d...tail:%d\r\n", \
		pxCirQueue->queueLen.ulData,pxCirQueue->head.ulData,pxCirQueue->tail.ulData);
	//更新节点信息把节点信息写入Flash,这里是为了防止异常关机导致盲区丢失，毕竟防不胜防
	//if(0 == pxCirQueue->queueLen.ulData % 20)
	//Blind_UpdateCirQueueInfo();
	
	return Q_SUCCESS;
}

//保存盲区数据
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
* Description : 元素出队
*
* Argument(s) : pBuffer:出队的数据指针 
*			   xCirQueue:循环队列指针
*
* Return(s)   : 出队列的数据长度(数据体长度( 2字节)+ 数据体的长度)
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
		//强制清零
		//pxCirQueue->head.ulData = 0;
		//pxCirQueue->tail.ulData = 0;
	  	return 0;
	}
	
	//寻址
	pxCirQueue->startAddr.ulData = FLASH_BLIND_AREA_SAVE_START_ADDR + pxCirQueue->head.ulData*FLASH_BLIND_STEP_LEN;
	//读取盲区数据长度
	sFLASH_ReadBuffer(temp.dataSaveLen.ucData,pxCirQueue->startAddr.ulData,4);
	//APP_DEBUG("\r\n...读取盲区地址为:%d盲区长度为:%d\r\n", pxCirQueue->startAddr.ulData,temp.dataSaveLen.ulData);
	//读取盲区数据
	*pBuf++ = temp.dataSaveLen.ucData[1];
	*pBuf++ = temp.dataSaveLen.ucData[0];
	sFLASH_ReadBuffer(pBuf,pxCirQueue->startAddr.ulData+4,temp.dataSaveLen.ulData);
	pxCirQueue->head.ulData++;
	//因为队列是环形，所以head需要通过取模来实现转回到0位置
	pxCirQueue->head.ulData = pxCirQueue->head.ulData % pxCirQueue->queueCapacity.ulData;
	pxCirQueue->queueLen.ulData--; 
	
	return ( temp.dataSaveLen.ulData+2);//加上两个字节的数据长度

}

typedef struct
{
	u16	itemNum;
	u32	blindDataLens;
}BLIND_PACK_INFO;
/*********************************************************************************************************
*                                          Blind_DataPack
*
* Description : 盲区数据打包
*
* Argument(s) : pBuffer:出队的数据指针 
*			   xCirQueue:循环队列指针
*
* Return(s)   : 数据项个数和数据项长度的结构体
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
* Description : 盲区数据打包
*
* Argument(s) : timeout:盲区定时器回调间隔
*			   xCirQueue:循环队列指针
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
		//强制清零
		//pxCirQueue->head.ulData = 0;
		//pxCirQueue->tail.ulData = 0;
		//如果在线,给出信号量,继续盲区上报
		//如果在线,给出信号量,继续盲区上报
	  	return Q_FAILED;
	}
	#endif
	//backup
	Blind_NodeBackup();
	
	upInfo = Blind_DataPack(ShareBuffer+3,xCirQueue);
	
	ShareBuffer[0] = (u8)(upInfo.itemNum>>8);//两个字节表示项数
	ShareBuffer[1] = (u8)upInfo.itemNum;
	ShareBuffer[2] = 1;//0:正常的位置批量汇报;1:盲区补报
	//ShareBuffer[3] = (u8)(upInfo.blindDataLens>>8);//两个字节位置汇报信息长度
	//ShareBuffer[4] = (u8)upInfo.blindDataLens;
	APP_DEBUG("\r\n<-- Blind_Report %d   %d-->\r\n",upInfo.itemNum,upInfo.blindDataLens);
	RadioProtocol_PostionInformation_BulkUpTrans(ShareBuffer,upInfo.blindDataLens+3);

	//更新节点信息把节点信息写入Flash,这里是为了防止异常关机导致盲区丢失，毕竟防不胜防
	//Blind_UpdateCirQueueInfo();
	APP_DEBUG("\r\n...剩余盲区条数为:%d...head:%d...tail:%d\r\n", \
		pxCirQueue->queueLen.ulData,pxCirQueue->head.ulData,pxCirQueue->tail.ulData);
	// 启动盲区上报回调函数定时器,单次
    //ret = Ql_Timer_Start(LOGIC_BLIND_REPORT_TMR_ID, timeout,FALSE);
	Ql_OS_SendMessage(main_task_id, MSG_ID_BLIND_CALLBACK, timeout, 0);
	blindProbe = 1;// 探针启动
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
	//在盲区上报期间又写过盲区要特殊处理
	if(0 == blindProbe)
	{
		if(pxCirQueue->tail.ulData == pxCirQueue->head.ulData)//如果追尾
		{
		//头部前进,盲区被新的数据覆盖了
			pxCirQueue->head.ulData++;
			pxCirQueue->head.ulData = pxCirQueue->head.ulData % pxCirQueue->queueCapacity.ulData;
		}
		pxCirQueue->tail.ulData++;//队列尾部递增
		//因为队列是环形，所以tail需要通过取模来实现转回到0位置
		pxCirQueue->tail.ulData = pxCirQueue->tail.ulData%pxCirQueue->queueCapacity.ulData;

		//如果队列没满就加1，否则就保持满吧
		if (Q_FAILED == Blind_IsCirQueueFull(pBlindCtrl))
		{
		  pxCirQueue->queueLen.ulData++;
		}					
	}
}
//更新节点信息
void Blind_UpdateCirQueueInfo(void)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)pBlindCtrl;

	pxCirQueue->crc32Verify.ulData = Public_CRC_32( pxCirQueue->head.ucData, FLASH_BLIND_AREA_SAVE_START_ADDR - 4 );
	sFLASH_WriteBuffer(pxCirQueue->head.ucData,0,FLASH_BLIND_AREA_SAVE_START_ADDR);
}

/**************************************************************************
//函数名：Blind_Link1ReportAck
//功能：连接1盲区补报应答处理
//输入：无
//输出：无
//返回值：无
//备注：收到连接1的盲区补报应答时需调用此函数
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

//创建一个信号量
int Blind_CreateSemaphore(void)
{
	return Ql_OS_CreateSemaphore("MySemaphore", 1);
}
//给出信号量
void Blind_GiveSemaphore(void)
{
	Ql_OS_GiveSemaphore(iSemaphoreId);
}
//获取信号量
void Blind_TakeSemaphore(bool wait)
{
	Ql_OS_TakeSemaphore(iSemaphoreId,wait);
}

//盲区上报回调函数
void Blind_Report_Callback_OnTimer(u32 timerId, void* param)
{
	static u8	shamLinkCount = 0;
	APP_DEBUG("Blind_Report_Callback_OnTimer......\r\n");
    if(LOGIC_BLIND_REPORT_TMR_ID == timerId)
	{
		if(0 == Blind_GetLink1ReportAck())//盲区上报没回应
		{
			//resume
			Blind_NodeResume();
			//如果在线,又老是不回应，说明是虚链接了特么的
			if(GetTerminalAuthorizationFlag())
			{
				if(shamLinkCount++ > 20)
				{					
					ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);
					Socket_Restart();//重新走tcp初始化流程
					//Blind_UpdateCirQueueInfo();
					//Report_RtcWakeUpOffsetSave();
					//Ql_Reset(0);
				}
			}
		}
		else
		{
			shamLinkCount = 0;
			Blind_ClrLink1ReportAck();//清除盲区上报应答标志
		}
		
		//如果在线,给出信号量,继续盲区上报
		if(GetTerminalAuthorizationFlag())
		{
			if (Q_SUCCESS == Blind_IsCirQueueEmpty(pBlindCtrl))
			{
				//更新节点信息把节点信息写入Flash
				Blind_UpdateCirQueueInfo();
			}
			Blind_GiveSemaphore();
		}
		else
		{
			Blind_UpdateCirQueueInfo();
		}
		blindProbe = 0; //无论如何要归位
	}
}
/*
《蝶恋花·花褪残红青杏小》
花褪残红青杏小。燕子飞时，绿水人家绕。
枝上柳绵吹又少，天涯何处无芳草。
墙里秋千墙外道。墙外行人，墙里佳人笑。
笑渐不闻声渐悄，多情却被无情恼。
*/
//获取盲区上报状态
u8 Blind_GetReportState(void)
{
	return (u8)Blind_IsCirQueueEmpty(pBlindCtrl);	
}


/*********************************************************************************************************
*                                          Blind_CirQueueGenericCreate
*
* Description : 创建一个循环队列
*
* Argument(s) : uxQueueLength:队列的容量
*
* Return(s)   : 申请的队列指针
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
《自题金山画像》
心似已灰之木，身如不系之舟。
问汝平生功业，黄州惠州儋州。
*/
//注册一个定时器用来控制盲区上报
void Blind_Timer_Register(void)
{	
	s32 ret = Ql_Timer_Register(LOGIC_BLIND_REPORT_TMR_ID, Blind_Report_Callback_OnTimer, NULL);
	APP_DEBUG("\r\nBlind_Timer_Register	%d......\r\n",ret);
}
/*********************************************************************************************************
*                                          Blind_Init
*
* Description : 盲区初始化
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
	//创建一个信号量
	iSemaphoreId = Blind_CreateSemaphore();
	//创建一个互斥量
	//iMutexId = Ql_OS_CreateMutex("MyMutex");
	//申请节点空间
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
	//CRC校验
	if(crc32Verify == temp.crc32Verify.ulData)
	{
		Ql_memcpy(pxCirQueue->head.ucData,temp.head.ucData,FLASH_BLIND_AREA_SAVE_START_ADDR);
		APP_DEBUG("Node check success......%04x    %04x...\r\n",crc32Verify,temp.crc32Verify.ulData);
	}
	else
	{
	//更新节点信息把节点信息写入Flash
		Blind_UpdateCirQueueInfo();
	}
	//保险一点
	pxCirQueue->queueCapacity.ulData = FLASH_BLIND_AREA_SAVE_NUM;
	//先释放信号量
	Blind_TakeSemaphore(FALSE);
}

#else

/********************本地变量*************************/
static BLIND_STRUCT Link1Blind;
static s16 Link1SearchSector;
static u16 Link1TimeCount = 0;
static u16 Link1WaitTimeCount = 0;



/********************本地函数声明*********************/
static void Blind_Link1Init(void);//连接1变量初始化
static void Blind_Link1Search(void);//连接1查找确定盲区记录、上报的初始位置
static void Blind_Link1SearchSector(s16 SearchSector);//检查某个扇区中的盲区信息
static u8 Blind_Link1Report(void);//连接1上报一包盲区数据
static u8 Blind_GetLink1ReportFlag(void);//获取连接1盲区上报标志


/********************函数定义*************************/
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

//获取盲区上报状态
u8 Blind_GetReportState(void)
{
	return Link1Blind.State;
}
/**************************************************************************
//函数名：Blind_TimeTask
//功能：实现盲区补报功能
//输入：无
//输出：无
//返回值：无
//备注：盲区补报定时任务，50ms调度1次，任务调度器需要调用此函数。
//当在进行FTP升级时需要关闭该任务，因为盲区记录与固件远程升级共用一块存储区
//注意如果谁关闭了该任务，谁就要负责打开该任务，上电启动默认是打开的
***************************************************************************/
FunctionalState Blind_TimeTask(void)
{
	/**************连接1盲区补报****************/
	if(BLIND_STATE_INIT == Link1Blind.State)//初始化所有相关变量
	{
		APP_DEBUG("\r\n<-- Blind_Link1Init -->\r\n");
		Blind_Link1Init();//完成后自动转入BLIND_STATE_SEARCH
	}
	else if(BLIND_STATE_SEARCH == Link1Blind.State)//逐步查找确定当前的盲区补报位置
	{
	
		//APP_DEBUG("\r\n<-- Blind_Link1Search -->\r\n");
		Blind_Link1Search();//完成后自动转入BLIND_STATE_REPORT
	}
	else if(BLIND_STATE_REPORT == Link1Blind.State)//盲区上报
	{
		Link1WaitTimeCount = 0;	
		if(0x01 == (0x01&GetTerminalAuthorizationFlag()))//只有连接1在线时才补报
		{
			Link1TimeCount++;
			if(Link1TimeCount >= BLIND_REPORT_DELAY_TIME)//20秒
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
					if(0 == Blind_Link1Report())//盲区上报
					{
						//Blind_StartLink1Report();
						Link1Blind.State = BLIND_STATE_WAIT;//转入等待状态
					}
				}
				else if(Link1Blind.ReportSendNum > 10)
				{
					Link1Blind.State = BLIND_STATE_WAIT;//转入等待状态
				}
			}
		}
	}
	else if(BLIND_STATE_WAIT == Link1Blind.State)//等待
	{
		Link1WaitTimeCount++;
		if(Link1WaitTimeCount >= 36000)//30分钟
		{
			Link1WaitTimeCount = 0;
			Link1Blind.State = BLIND_STATE_INIT;
		}
	}
	else//异常
	{
		Link1Blind.State = BLIND_STATE_INIT;
	}
	
	return ENABLE;
}


/**************************************************************************
//函数名：Blind_Link1Init
//功能：连接1变量初始化
//输入：无
//输出：无
//返回值：无
//备注：盲区补报定时任务需要调用此函数
***************************************************************************/
static void Blind_Link1Init(void)
{
	u8 i;

	Link1Blind.StartSector = FLASH_BLIND_START_SECTOR;
	Link1Blind.EndSector = FLASH_BLIND_END_SECTOR;

	Link1Blind.SectorStep = FLASH_ONE_SECTOR_BYTES/FLASH_BLIND_STEP_LEN;

	Link1Blind.OldestSector = Link1Blind.StartSector;
	Link1Blind.OldestStep = 0;
	//Link1Blind.OldestTimeCount = 0;//赋给最小值
	Link1Blind.OldestTimeCount = 0xFFFFFFFF;//赋给最小值
	Link1Blind.SaveSector = Link1Blind.StartSector;
	Link1Blind.SaveStep = 0;
	Link1Blind.SaveTimeCount = 0;//赋给最小值
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
//函数名：Blind_Link1Search
//功能：连接1查找确定盲区记录、上报的初始位置
//输入：无
//输出：无
//返回值：无
//备注：盲区补报定时任务需要调用此函数
***************************************************************************/
static void Blind_Link1Search(void)
{
	if((Link1SearchSector >= Link1Blind.StartSector)&&(Link1SearchSector < Link1Blind.EndSector))
	{
		Blind_Link1SearchSector(Link1SearchSector);//查找连接1某个扇区中的每条盲区信息
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
			//盲区区域为空
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
//函数名：Blind_Link1SearchSector
//功能：检查某个扇区中的盲区信息
//输入：扇区号
//输出：当前存储和补报盲区的位置
//返回值：无
//备注：
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
	sFLASH_ReadBuffer(&flag,Addr,1);//读整个扇区是否已补报标志
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

	Addr = SearchSector*FLASH_ONE_SECTOR_BYTES + (Link1Blind.SectorStep-1)*FLASH_BLIND_STEP_LEN;//读扇区的最后1步
	sFLASH_ReadBuffer(Buffer,Addr,4);
	ttCount = Public_ConvertBufferToLong(Buffer);
	if(0xffffffff != ttCount)
	{
		Gmtime(&tt,ttCount);
		if(SUCCESS == CheckTimeStruct(&tt))
		{
			sFLASH_ReadBuffer(Buffer,Addr,FLASH_BLIND_STEP_LEN);
			if(CHANNEL_DATA_1 != Buffer[4])//通道不正确
			{
				Link1Blind.ErrorCount++;
				if(Link1Blind.ErrorCount > 3)
				{
					Link1Blind.ErrorCount = 0;
					Blind_Link1Erase();//擦除所有盲区存储区域
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
					Blind_Link1Erase();//擦除所有盲区存储区域
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
				Blind_Link1Erase();//擦除所有盲区存储区域
				return ;
			}
		}
	}
	else
	{
		Addr = SearchSector*FLASH_ONE_SECTOR_BYTES;//读扇区的第1步
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
					if(CHANNEL_DATA_1 != Buffer[4])//通道不正确
					{
						Link1Blind.ErrorCount++;
						if(Link1Blind.ErrorCount > 3)
						{
							Link1Blind.ErrorCount = 0;
							Blind_Link1Erase();//擦除所有盲区存储区域
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
							Blind_Link1Erase();//擦除所有盲区存储区域
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
						Blind_Link1Erase();//擦除所有盲区存储区域
						return ;
					}
				}
			}
		}
	}

/*每个扇区每一步都检查，太浪费时间，优化后的只检查第1步和最后1步
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
				if(CHANNEL_DATA_1 != Buffer[4])//通道不正确
				{
					Link1Blind.ErrorCount++;
					if(Link1Blind.ErrorCount > 3)
					{
						Link1Blind.ErrorCount = 0;
						Blind_Erase(0xff);//擦除所有盲区存储区域
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
//函数名：Blind_Link1Report
//功能：连接1上报一包盲区数据
//输入：无
//输出：无
//返回值：无
//备注：盲区补报定时任务需要调用此函数
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
	u16 length = 3;//预留3个字节


	for(count=0; count<5; )//每包包含5条位置信息
	{
		if(0 == Blind_GetLink1ReportFlag())
		{
			break;
		}
		Addr = Link1Blind.ReportSector*FLASH_ONE_SECTOR_BYTES + Link1Blind.SectorStep*FLASH_BLIND_STEP_LEN + 1;
		sFLASH_ReadBuffer(&flag,Addr,1);//读整个扇区是否已补报标志
		if(0xaa == flag)//已补报过
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
				sFLASH_ReadBuffer(&flag,Addr,1);//读该条是否已补报标志
				if(0xaa == flag)//已补报过
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
							ShareBuffer[length] = 0;//长度高字节填补0
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
		ShareBuffer[0] = 0;//两个字节表示项数
		ShareBuffer[1] = count;
		ShareBuffer[2] = 1;//0:正常的位置批量汇报;1:盲区补报
		APP_DEBUG("\r\n<-- Blind_Report -->\r\n");
		RadioProtocol_PostionInformation_BulkUpTrans(ShareBuffer,length);
	}

	return 1;
}
/**************************************************************************
//函数名：Blind_Link1ReportAck
//功能：连接1盲区补报应答处理
//输入：无
//输出：无
//返回值：无
//备注：收到连接1的盲区补报应答时需调用此函数
***************************************************************************/
void Blind_Link1ReportAck(void)
{
	u8 i;
	u8 flag;
	u32 Addr;
	s16 Sector;

	Link1Blind.ReportSendNum = 0;
	Link1TimeCount = BLIND_REPORT_DELAY_TIME;//1秒后上报下一包

	Sector = Link1Blind.LastReportSector[0];

	for(i=0; i<5; i++)
	{
		if((Link1Blind.LastReportSector[i] >= Link1Blind.StartSector)
			&&(Link1Blind.LastReportSector[i] < Link1Blind.EndSector)
				&&(Link1Blind.LastReportStep[i] < Link1Blind.SectorStep))
		{
			Addr = Link1Blind.LastReportSector[i]*FLASH_ONE_SECTOR_BYTES+(Link1Blind.LastReportStep[i]+1)*FLASH_BLIND_STEP_LEN - 1;
			flag = 0xaa;
			sFLASH_WriteBuffer(&flag, Addr, 1);//标记该条盲区已上报

			if(Link1Blind.LastReportStep[i] >= (Link1Blind.SectorStep-1))
			{
				Addr = Sector*FLASH_ONE_SECTOR_BYTES+Link1Blind.SectorStep*FLASH_BLIND_STEP_LEN + 1;
				flag = 0xaa;
				sFLASH_WriteBuffer(&flag, Addr, 1);//标记该扇区的盲区已上报
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
//函数名：Blind_GetLink1ReportFlag
//功能：获取连接1盲区上报标志
//输入：无
//输出：无
//返回值：盲区上报标志
//备注：1表示需要上报，0表示不需要上报，终端每隔10分钟会自动检测一次看是否需要上报
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
//函数名：Blind_StartLink1Report
//功能：开启连接1盲区补报
//输入：无
//输出：无
//返回值：无
//备注：开启盲区补报，每次连接1鉴权成功需调用此函数
***************************************************************************/
void Blind_StartLink1Report(void)
{
	//if(BLIND_STATE_WAIT == Link1Blind.State)
	//{
		Link1Blind.State = BLIND_STATE_INIT;
	//}
}
/**************************************************************************
//函数名：Blind_Link1Erase
//功能：擦除连接1盲区存储区域
//输入：无
//输出：无
//返回值：无
//备注：检测到盲区记录的位置信息有误时（有误的条数大于50）会调用此函数，
//或者是远程FTP升级擦除区域时会调用此函数，因为盲区与远程固件升级共用一块区域
***************************************************************************/
void Blind_Link1Erase(void)
{
	u16 i;
	u32 Addr;
	
	Link1Blind.SaveEnableFlag = 0;//禁止存储
	Link1Blind.State = BLIND_STATE_INIT;

	for(i=Link1Blind.StartSector; i<Link1Blind.EndSector; i++)
	{
		Addr = i*FLASH_ONE_SECTOR_BYTES;
		sFLASH_EraseSector(Addr);
	}	
}
/**************************************************************************
//函数名：Blind_Link1Save
//功能：存储一条连接1的盲区数据
//输入：一条位置信息及长度
//输出：无
//返回值：0为成功，非0为失败，长度超出时会返回失败
//备注：一条盲区数据包括：时间4字节+属性1字节（预留）+校验和1字节+位置信息长度1字节+位置信息若干字节
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

	//if(ReportStatus_On != Report_GetReportStatus())return 1;//没上线不写盲区
	APP_DEBUG("\r\n<-- Blind_Link1Save -->\r\n");
	if((length > (FLASH_BLIND_STEP_LEN-9))||(length < 28)||(0 == Link1Blind.SaveEnableFlag))//每步最后两个字节用作错误标记，已上传标记
	{
		return 1;
	}
	
	if((Link1Blind.StartSector == Link1Blind.SaveSector)&&(0 == Link1Blind.SaveStep)&&(0 == Link1Blind.SaveTimeCount))//为空
	{
			
	}
	else//不为空，已存有数据
	{
		Sector = Link1Blind.SaveSector;
		Step = Link1Blind.SaveStep;
		APP_DEBUG("\r\n<-- Link1Blind.SaveSector=%d, Link1Blind.SaveStep=%d-->\r\n",Link1Blind.SaveSector,Link1Blind.SaveStep);
		Step--;//获取已存储的最新的那一步时间
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

		if(ERROR == CheckTimeStruct(&tt))//检查已存储的最新的那一步时间是否正确
		{
			sFLASH_EraseSector(Addr);
			Link1Blind.SaveStep = 0;
			
			APP_DEBUG("\r\n<-- Blind_Link1Save CheckTime SaveTime Error -->\r\n");
			return 1;
		}

		if(ERROR == CheckTimeStruct(&CurTime))//检查当前时间是否正确
		{
		
			APP_DEBUG("\r\n<-- Blind_Link1Save CheckTime CurrentTime Error-->\r\n");
			return 1;
		}

		TimeCount = ConverseGmtime(&CurTime);//比较当前时间与最新记录的那一步时间
		
		APP_DEBUG("<--Blind Ql_GetLocalTime(%d.%02d.%02d %02d:%02d:%02d) -->\n\r", CurTime.year, CurTime.month, CurTime.day, CurTime.hour, CurTime.min, CurTime.sec);
		if(TimeCount < ttCount)
		{
		
			APP_DEBUG("\r\n<-- Blind_Link1Save CheckTime CurrentTime less SaveTime Error-->\r\n");
			return 1;
		}
		
	}
	
	Addr = Link1Blind.SaveSector*FLASH_ONE_SECTOR_BYTES+Link1Blind.SaveStep*FLASH_BLIND_STEP_LEN;//检查当前存储的那一步的地址是否为空
	
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
				//等待一会儿
			}
			break;
		}
	}

	sum = 0;//计算校验和
	for(i=0; i<length; i++)
	{
		sum += *(pBuffer+i);
	}

	
	memcpy(Buffer+7,pBuffer,length);//拷贝位置信息至缓冲

	p = Buffer;//补填7字节的数据
	*p++ = (TimeCount&0xff000000) >> 24;
	*p++ = (TimeCount&0xff0000) >> 16;
	*p++ = (TimeCount&0xff00) >> 8;
	*p++ = TimeCount&0xff;
	*p++ = attribute;
	*p++ = sum;
	*p++ = length;

	sFLASH_WriteBuffer(Buffer,Addr,length+7);//写入flash

	Link1Blind.SaveStep++;//下移一步
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


















