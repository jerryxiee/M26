#ifndef _SMS_H_
#define _SMS_H_

#include "ril_sms.h"

 
 //���ĺ���ɾ���ɹ�!
#define DEL_ALARM_PHONE_SUCCESS  	"4e2d5fc353f77801522096646210529fff01\0"
//��������ѻָ���Ϊ��ʼ���룡
#define SET_ALARM_PHONE_WRONG_PASSWORD 	"5bc6780195198befff0c5df26062590d62104e3a521d59cb5bc67801ff01\0"

 //��Ȩ�绰���óɹ����µ���Ȩ���뽫���ã�
#define SET_ALARM_PHONE_SUCCESS  "6388674375358bdd8bbe7f6e6210529fff0c65b076846388674353f778015c06542f7528ff01\0"
 //���������óɹ���
#define SET_IP_PORT_SUCCESS "670d52a156688bbe7f6e6210529fff01\0"
 //�������ö�ʱ�ϱ��ɹ�
#define SET_REPORT_TIME_SUCCESS  "4e0a4f2095f4969465f695f48bbe7f6e6210529fff0195f4969465f695f44e3a\0"
 //��ID�����óɹ���
#define SET_DEVIECE_ID_SUCCESS "0049004453f78bbe7f6e6210529fff01\0"
 //ָ����ճɹ���������
#define SET_REST_SUCCESS "63074ee463a565366210529fff0c53735c0691cd542fff01\0"
 //�ָ��͵�ɹ���
#define SET_OPEN_OIL_SUCCESS  "6062590d6cb98def6210529fff01\0"
 //���͵�ɹ���
#define SET_OFF_OIL_SUCCESS   "65ad6cb98def6210529fff01\0"
 //��ǰ��������͵�
#define ILLAGAL_SET_OIL  "5f53524d4e0d51418bb865ad6cb97535\0"
//ָ�֧�ֻ��ߴ�������ϸ�˶ԣ�
#define ILLAGAL_ORDER "63074ee44e0d652f63016216800595198befff0c8bf74ed47ec668385bf9ff01\0"


 /*****************************************************************************
  * FUNCTION
  *  SMS_Initialize
  *
  * DESCRIPTION
  *  Initialize SMS environment.
  *  
  * PARAMETERS
  *  VOID
  *
  * RETURNS
  *  TRUE:	This function works SUCCESS.
  *  FALSE: This function works FAIL!
  *****************************************************************************/
 bool SMS_Initialize(void);
/*****************************************************************************
 * FUNCTION
 *  Hdlr_RecvNewSMS
 *
 * DESCRIPTION
 *  The handler function of new received SMS.
 *  
 * PARAMETERS
 *  <nIndex>     The SMS index in storage,it starts from 1
 *  <bAutoReply> TRUE: The module should reply a SMS to the sender; 
 *               FALSE: The module only read this SMS.
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  1. This is an internal function
 *****************************************************************************/
void Hdlr_RecvNewSMS(u32 nIndex, bool bAutoReply);

void InitSmsData(void);
void UpSmsAlarmPhoNum(void);

void SMS_TextMode_Send(LIB_SMS_CharSetEnum SmsType,char *strPhNum,char *strTestMsg,u8 MsgLen);
extern u8 SmsOilFlag;
extern char SmsTextAlarmPhoNume[20];

/*
  * @brief  Ӧ�ò����CCID
  * @param  *p :ָ��ccid����
  * @retval None
  */
void  Modem_Api_CCID(u8 *p);
/**
  * @brief  ��ȡ�ն�SIM��ICCID��
  * @param 
  * @retval None.
  */
void MOD_GetIccidBcdValue(void);
/*
  * @brief  ��ѯmodem����
  * @param  None
  * @retval 
            bit0��0����֧��GPRSͨ�ţ�     1��֧��GPRSͨ�ţ�
            bit1��0����֧��CDMAͨ�ţ�     1��֧��CDMAͨ�ţ�
            bit2��0����֧��TD-SCDMAͨ�ţ� 1��֧��TD-SCDMAͨ�ţ�
            bit3��0����֧��WCDMAͨ�ţ�    1��֧��WCDMAͨ�ţ�
            bit4��0����֧��CDMA2000ͨ�ţ� 1��֧��CDMA2000ͨ�š�
            bit5��0����֧��TD-LTEͨ�ţ�   1��֧��TD-LTEͨ�ţ�
            bit7��0����֧������ͨ�ŷ�ʽ�� 1��֧������ͨ�ŷ�ʽ��  
  */
u8  Modem_Api_Type(void);


#endif
