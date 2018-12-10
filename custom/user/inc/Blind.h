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
	u32	dataLen;				//�ڵ����ݳ���ä���ڵ㳤��+ä�����ݳ���
	//u32	dataAddr;				//�ڵ����ݵ�ַ
}BLIND_NODE;
typedef union
{
	u32 ulData;
	u8	ucData[4];
}UN_BLIND_DATA;
typedef struct
{
	UN_BLIND_DATA	head;					//����ͷ
	UN_BLIND_DATA	tail;					//����β
	UN_BLIND_DATA	queueLen;				//���г���
	UN_BLIND_DATA	queueCapacity;			//��������
	UN_BLIND_DATA	startAddr;			//�洢��ʼ��ַ
	//u32	endAddr;				//�洢������ַ
	UN_BLIND_DATA	dataSaveLen;			//ä�����ݳ���
	#if( VERIFY_MODE_CRC32 == configVerifyMode )
	UN_BLIND_DATA	crc32Verify;					//CRC32У��
	#elif( VERIFY_MODE_HASH == configVerifyMode )
	UN_BLIND_DATA	hashVerify;			//��ϣУ��
	#endif
}BLIND_SAVE_STU;

typedef BLIND_SAVE_STU	CirQueue_t;
typedef void * CirQueueHandle_t;

typedef enum {Q_SUCCESS = 0u,Q_FAILED = !Q_SUCCESS}QueueStatus;

#define BLIND_UP_OVERTIME					(10000)	// ä���ϱ���ʱʱ��ms
#define	FLASH_BLIND_PACK_NUM				(5)//ä�������Ŀ
#define	FLASH_BLIND_AREA_SAVE_START_ADDR	(sizeof(BLIND_SAVE_STU))	//ѭ��������ʼ��ַ��ǰ��Ԥ�����Ǵ洢�ڵ���Ϣ��
#define	FLASH_BLIND_AREA_SAVE_END_ADDR		(FLASH_ONE_SECTOR_BYTES*FLASH_BLIND_END_SECTOR)
#define	FLASH_BLIND_AREA_SAVE_NUM			((FLASH_BLIND_AREA_SAVE_END_ADDR-FLASH_BLIND_AREA_SAVE_START_ADDR)/FLASH_BLIND_STEP_LEN-1)

void Blind_Init(void);

//����һ���ź���
int Blind_CreateSemaphore(void);
//�����ź���
void Blind_GiveSemaphore(void);
//��ȡ�ź���
void Blind_TakeSemaphore(bool wait);
//ע��һ����ʱ����������ä���ϱ�
void Blind_Timer_Register(void);
//���½ڵ���Ϣ
void Blind_UpdateCirQueueInfo(void);

#else

#define BLIND_STATE_INIT 	0
#define BLIND_STATE_SEARCH 	1
#define BLIND_STATE_REPORT 	2
#define BLIND_STATE_WAIT 	3
#define BLIND_STATE_ERROR 	4 

#define BLIND_REPORT_DELAY_TIME  400//��λΪ50ms��20����û�յ�Ӧ������ط�������ط�10�Σ���Ȼû�յ�Ӧ����ͣ����

typedef struct
{
	u8 State;//״̬��0:��ʼ��״̬;1:��������״̬;2:ä���㱨״̬;3:����״̬;4:�ȴ�״̬;5:����״̬
	u8 ErrorCount;//�������������Լ�ä�������ۼƳ�����3��ä�����ݽ����������ä��
	s16 StartSector;//ä���洢����ʼ����
	s16 EndSector;//ä���洢�����������
	s16 OldestSector;//����ʱ�������λ��
	s16 OldestStep;//����ʱ��Ĳ���λ��
	u32 OldestTimeCount;//����ʱ�������λ����Ϣ����Ӧ��GPSʱ��
	s16 SectorStep;//һ���������ܲ���	
	s16 ReportSector;//��ǰä���ϱ�������λ��
	s16 ReportStep;//��ǰä���ϱ��Ĳ���λ��
	u32 ReportTimeCount;//��ǰä���ϱ�������λ����Ϣ����Ӧ��GPSʱ��
	u8 ReportSendNum;//��ǰ��ä���ϱ����ʹ�����ÿ����1�μ�1���յ�ƽ̨Ӧ��ʱ��0������10��δ�յ�Ӧ�𣬵ȴ�10���ӣ�Ȼ�����ط�
	u8 SaveEnableFlag;//ä����¼ʹ�ܱ�־,1Ϊʹ�ܣ�0Ϊ��ֹ
	s16 SaveSector;//��ǰ��¼������λ��
	s16 SaveStep;//��ǰ��¼�Ĳ���λ��
	u32 SaveTimeCount;//��ǰ��¼������λ����Ϣ����Ӧ��GPSʱ�� 
	s16 LastReportSector[5];//��һ�������ϱ������������һ���ϱ�5����������¼5�������������յ�ƽ̨Ӧ��ʱ��0
	s16 LastReportStep[5];//��һ�������ϱ��Ĳ���

}BLIND_STRUCT;

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
FunctionalState Blind_TimeTask(void);

/**************************************************************************
//��������Blind_Link1Erase
//���ܣ���������1ä���洢����
//���룺��
//�������
//����ֵ����
//��ע����⵽ä����¼��λ����Ϣ����ʱ���������������50������ô˺�����
//������Զ��FTP������������ʱ����ô˺�������Ϊä����Զ�̹̼���������һ������
***************************************************************************/
void Blind_Link1Erase(void);

/**************************************************************************
//��������Blind_StartLink1Report
//���ܣ���������1ä������
//���룺��
//�������
//����ֵ����
//��ע������ä��������ÿ������1��Ȩ�ɹ�����ô˺���
***************************************************************************/
void Blind_StartLink1Report(void);
#endif

//��ȡä���ϱ�״̬
u8 Blind_GetReportState(void);

/**************************************************************************
//��������Blind_Link1Save
//���ܣ��洢һ������1��ä������
//���룺һ��λ����Ϣ������
//�������
//����ֵ��0Ϊ�ɹ�����0Ϊʧ�ܣ����ȳ���ʱ�᷵��ʧ��
//��ע��һ��ä�����ݰ�����ʱ��4�ֽ�+����1�ֽڣ�Ԥ����+У���1�ֽ�+λ����Ϣ����1�ֽ�+λ����Ϣ�����ֽ�
***************************************************************************/
u8 Blind_Link1Save(u8 *pBuffer, u8 length, u8 attribute);

/**************************************************************************
//��������Blind_Link1ReportAck
//���ܣ�����1ä������Ӧ����
//���룺��
//�������
//����ֵ����
//��ע���յ�����1��ä������Ӧ��ʱ����ô˺���
***************************************************************************/
void Blind_Link1ReportAck(void);

#endif
