#ifndef __BLIND_H
#define __BLIND_H

#include "include.h"

#define	BLIND_SAVE_BY_QUEUE

#ifdef BLIND_SAVE_BY_QUEUE

#define VERIFY_MODE_HASH	(0)
#define VERIFY_MODE_CRC32	(1)

#define configVerifyMode	(VERIFY_MODE_CRC32)

typedef struct
{
	u32	dataLen;				//节点数据长度盲区节点长度+盲区数据长度
	//u32	dataAddr;				//节点数据地址
}BLIND_NODE;
typedef union
{
	u32 ulData;
	u8	ucData[4];
}UN_BLIND_DATA;
typedef struct
{
	UN_BLIND_DATA	head;					//队列头
	UN_BLIND_DATA	tail;					//队列尾
	UN_BLIND_DATA	queueLen;				//队列长度
	UN_BLIND_DATA	queueCapacity;			//队列容量
	UN_BLIND_DATA	startAddr;			//存储起始地址
	//u32	endAddr;				//存储结束地址
	UN_BLIND_DATA	dataSaveLen;			//盲区数据长度
	#if( VERIFY_MODE_CRC32 == configVerifyMode )
	UN_BLIND_DATA	crc32Verify;					//CRC32校验
	#elif( VERIFY_MODE_HASH == configVerifyMode )
	UN_BLIND_DATA	hashVerify;			//哈希校验
	#endif
}BLIND_SAVE_STU;

typedef BLIND_SAVE_STU	CirQueue_t;
typedef void * CirQueueHandle_t;

typedef enum {Q_SUCCESS = 0u,Q_FAILED = !Q_SUCCESS}QueueStatus;

#define BLIND_UP_OVERTIME					(10000)	// 盲区上报超时时间ms
#define	FLASH_BLIND_PACK_NUM				(5)//盲区打包数目
#define	FLASH_BLIND_AREA_SAVE_START_ADDR	(sizeof(BLIND_SAVE_STU))	//循环队列起始地址，前面预留的是存储节点信息的
#define	FLASH_BLIND_AREA_SAVE_END_ADDR		(FLASH_ONE_SECTOR_BYTES*FLASH_BLIND_END_SECTOR)
#define	FLASH_BLIND_AREA_SAVE_NUM			((FLASH_BLIND_AREA_SAVE_END_ADDR-FLASH_BLIND_AREA_SAVE_START_ADDR)/FLASH_BLIND_STEP_LEN-1)

void Blind_Init(void);

//创建一个信号量
int Blind_CreateSemaphore(void);
//给出信号量
void Blind_GiveSemaphore(void);
//获取信号量
void Blind_TakeSemaphore(bool wait);
//注册一个定时器用来控制盲区上报
void Blind_Timer_Register(void);
//更新节点信息
void Blind_UpdateCirQueueInfo(void);

#else

#define BLIND_STATE_INIT 	0
#define BLIND_STATE_SEARCH 	1
#define BLIND_STATE_REPORT 	2
#define BLIND_STATE_WAIT 	3
#define BLIND_STATE_ERROR 	4 

#define BLIND_REPORT_DELAY_TIME  400//单位为50ms，20秒内没收到应答继续重发，最多重发10次，仍然没收到应答，暂停补报

typedef struct
{
	u8 State;//状态，0:初始化状态;1:查找搜索状态;2:盲区汇报状态;3:空闲状态;4:等待状态;5:错误状态
	u8 ErrorCount;//错误步数计数，自检盲区区域累计超过有3条盲区数据将会擦除整个盲区
	s16 StartSector;//盲区存储区域开始扇区
	s16 EndSector;//盲区存储区域结束扇区
	s16 OldestSector;//最老时间的扇区位置
	s16 OldestStep;//最老时间的步数位置
	u32 OldestTimeCount;//最老时间的那条位置信息所对应的GPS时间
	s16 SectorStep;//一个扇区的总步数	
	s16 ReportSector;//当前盲区上报的扇区位置
	s16 ReportStep;//当前盲区上报的步数位置
	u32 ReportTimeCount;//当前盲区上报的那条位置信息所对应的GPS时间
	u8 ReportSendNum;//当前包盲区上报发送次数，每发送1次加1，收到平台应答时清0，超过10次未收到应答，等待10分钟，然后再重发
	u8 SaveEnableFlag;//盲区记录使能标志,1为使能，0为禁止
	s16 SaveSector;//当前记录的扇区位置
	s16 SaveStep;//当前记录的步数位置
	u32 SaveTimeCount;//当前记录的那条位置信息所对应的GPS时间 
	s16 LastReportSector[5];//上一次扇区上报的扇区（最多一次上报5条，因此需记录5个扇区，），收到平台应答时清0
	s16 LastReportStep[5];//上一次扇区上报的步数

}BLIND_STRUCT;

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
FunctionalState Blind_TimeTask(void);

/**************************************************************************
//函数名：Blind_Link1Erase
//功能：擦除连接1盲区存储区域
//输入：无
//输出：无
//返回值：无
//备注：检测到盲区记录的位置信息有误时（有误的条数大于50）会调用此函数，
//或者是远程FTP升级擦除区域时会调用此函数，因为盲区与远程固件升级共用一块区域
***************************************************************************/
void Blind_Link1Erase(void);

/**************************************************************************
//函数名：Blind_StartLink1Report
//功能：开启连接1盲区补报
//输入：无
//输出：无
//返回值：无
//备注：开启盲区补报，每次连接1鉴权成功需调用此函数
***************************************************************************/
void Blind_StartLink1Report(void);
#endif

//获取盲区上报状态
u8 Blind_GetReportState(void);

/**************************************************************************
//函数名：Blind_Link1Save
//功能：存储一条连接1的盲区数据
//输入：一条位置信息及长度
//输出：无
//返回值：0为成功，非0为失败，长度超出时会返回失败
//备注：一条盲区数据包括：时间4字节+属性1字节（预留）+校验和1字节+位置信息长度1字节+位置信息若干字节
***************************************************************************/
u8 Blind_Link1Save(u8 *pBuffer, u8 length, u8 attribute);

/**************************************************************************
//函数名：Blind_Link1ReportAck
//功能：连接1盲区补报应答处理
//输入：无
//输出：无
//返回值：无
//备注：收到连接1的盲区补报应答时需调用此函数
***************************************************************************/
void Blind_Link1ReportAck(void);

#endif
