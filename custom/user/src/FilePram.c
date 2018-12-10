#include "include.h"


//********************************�궨��************************************

#define EepromPramID_SIZE (sizeof(EepromPramAddress)/sizeof(EepromPramAddress[0]))


//********************************ȫ�ֱ���**********************************
/////////////////////
const unsigned long c_usEepromPramID[] = 
{
    E2_FIRMWARE_VERSION_ID,             //�汾��,���⴦��,����д,
    E2_TERMINAL_HEARTBEAT_ID,           //01����ID0x0001���ն��������ͼ������λ�룬Ĭ��ֵ60
    E2_MAIN_SERVER_APN_ID,              //02����ID0x0010����������APN
    E2_MAIN_SERVER_DIAL_NAME_ID,        //03����ID0x0011��������������ͨ�Ų����û���
    E2_MAIN_SERVER_DIAL_PASSWORD_ID,    //04����ID0x0012��������������ͨ�Ų�������
    E2_MAIN_SERVER_IP_ID,               //05����ID0x0013������������ַ��IP������
    E2_BACKUP_SERVER_APN_ID,			//06����ID0x0014,���ݷ�����APN//����ͨ�Ų��ŷ��ʵ�
    E2_BACKUP_SERVER_DIAL_NAME_ID, 		//07����ID0x0015,���ݷ���������ͨ�Ų����û���
 	E2_BACKUP_SERVER_DIAL_PASSWORD_ID, 	//08����ID0x0016,���ݷ���������ͨ�Ų�������
	E2_BACKUP_SERVER_IP_ID,				//09����ID0x0017,���ݷ�������ַ//IP������
    E2_MAIN_SERVER_TCP_PORT_ID,         //10����ID0x0018����������TCP�˿�
    E2_SLEEP_REPORT_TIME_ID,            //11����ID0x0027������ʱ�㱨ʱ��������λΪ�룬Ĭ��ֵΪ60
    E2_EMERGENCY_REPORT_TIME_ID,		//12����ID0x0028,��������ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ10
    E2_ACCON_REPORT_TIME_ID,            //13����ID0x0029,ȱʡʱ��㱨���,��λΪ�룬Ĭ��ֵΪ10 
	E2_CORNER_REPORT_ID, 				//14����ID0x0030,�յ㲹���Ƕ�,��λ��/��//Ĭ��ֵΪ30
	E2_MONITOR_SERVER_SMS_PHONE_ID,		//0x0043//���ƽ̨SMS�绰����
	E2_SMS_TEXT_ALARM_PHONE_ID,			//	0x0044//SMS�ı������绰����
	E2_ALARM_MASK_ID, 					//15����ID0x0050,����������//bit9,11,12Ĭ������,LCD������,TTS����,IC��ģ�����Ĭ��Ϊ����
	E2_MAX_SPEED_ID, 					//16����ID0x0055,����ٶ�,��λkm/h,DWORD,Ĭ��ֵ130,��ͨ����Ϊ100
	E2_OVER_SPEED_KEEP_TIME_ID, 		//17����ID0x0056,���ٳ���ʱ��,��λΪ��,DWORD,Ĭ��Ϊ10��,��ͨ����Ϊ�����ϱ�
	E2_SET_CRASH_ALARM_ID,				//18����ID0x005D,��ײ������������,��ײ���ٶ�Ĭ������Ϊ4g
	E2_SET_ROLLOVER_ALARM_ID,	        //19����ID0x005E,�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ45 ��
	E2_CAR_TOTAL_MILE_ID,				//20����ID0x0080��������̱������0.1km
	E2_CAR_PROVINCE_ID, 				//21����ID0x0081,�������ڵ�ʡ��ID
	E2_CAR_CITY_ID, 					//22����ID0x0082,�������ڵ�����ID
	E2_CAR_PLATE_NUM_ID, 				//23����ID0x0083,������ͨ�����Ű䷢�Ļ���������
	E2_CAR_PLATE_COLOR_ID, 				//24����ID0x0084,������ɫ//����JT/T415-2006��5.4.12,1:��ɫ,2:��ɫ,3:��ɫ,4:��ɫ,9:����
    E2_GPS_SET_MODE_ID,                 //25����ID0x0090 GNSS ��λģʽ��Ĭ��Ϊ������λ
	E2_LOGINGPRS_PASSWORD_ID,			//26����ID0xF200+0x00���Զ��壬��Ȩ��
	E2_MANUFACTURE_ID,					//27����ID0xF205���Զ��壬������ID��5�ֽ�	
    E2_DEVICE_ID,                       //28����ID0xF200+0x06���Զ��壬�����ն�ID��7�ֽ�
    E2_DEVICE_PHONE_ID,                 //29����ID0xF200+0x07���Զ��壬�ն��ֻ��ţ�6�ֽ�
    E2_CAR_VIN_ID,						//30����ID0xF208���Զ��壬����ʶ����(VIN)��17�ֽڣ�ASCII��
    E2_DEFINE_ALARM_MASK_WORD_ID,		//31����ID0xF243���Զ��壬�Զ��屨�������֣�DWORD
    E2_DEFINE_HARD_VER_ID,				//32����ID0x0001F0004���Զ��壬�Զ���Ӳ���汾�ţ�DWORD
    E2_SMS_ALARM_PHONE_NUM_ID,			//����ID 0x0001F005���Զ��壬�Զ����û����ű����绰��DWORD
	E2_SMS_OPERATE_PWD_ID,				//����ID 0x0001F006���Զ��壬�Զ����û����Ų������룬DWORD
	E2_LOW_VOL_THRESHOLD_ID,			//����ID 0x0001F00F���Զ��壬�Զ���͵�ѹ��ֵ��WORD
};
////////////////////////
const u8 EepromPramLength[] = 
{
    0,
    E2_TERMINAL_HEARTBEAT_LEN,          //01����ID0x0001���ն��������ͼ������λ�룬Ĭ��ֵ60
    E2_MAIN_SERVER_APN_LEN,             //02����ID0x0010����������APN
    E2_MAIN_SERVER_DIAL_NAME_LEN,       //03����ID0x0011��������������ͨ�Ų����û���
    E2_MAIN_SERVER_DIAL_PASSWORD_LEN,   //04����ID0x0012��������������ͨ�Ų�������
    E2_MAIN_SERVER_IP_LEN,              //05����ID0x0013������������ַ��IP������
	E2_BACKUP_SERVER_APN_LEN,			//06����ID0x0014,���ݷ�����APN//����ͨ�Ų��ŷ��ʵ�
	E2_BACKUP_SERVER_DIAL_NAME_LEN,		//07����ID0x0015,���ݷ���������ͨ�Ų����û���
	E2_BACKUP_SERVER_DIAL_PASSWORD_LEN,	//08����ID0x0016,���ݷ���������ͨ�Ų�������
	E2_BACKUP_SERVER_IP_LEN, 			//09����ID0x0017,���ݷ�������ַ//IP������
    E2_MAIN_SERVER_TCP_PORT_LEN,        //10����ID0x0018����������TCP�˿�
    E2_SLEEP_REPORT_TIME_LEN,           //11����ID0x0027������ʱ�㱨ʱ��������λΪ�룬Ĭ��ֵΪ60
	E2_EMERGENCY_REPORT_TIME_LEN,		//12����ID0x0028,��������ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ10
    E2_ACCON_REPORT_TIME_LEN,           //13����ID0x0029ȱʡʱ��㱨���,��λΪ�룬Ĭ��ֵΪ10
	E2_CORNER_REPORT_LEN, 				//14����ID0x0030,�յ㲹���Ƕ�,��λ��/��//Ĭ��ֵΪ30
	E2_MONITOR_SERVER_SMS_PHONE_LEN,    //����ID0x0043�����ƽ̨SMS�绰����
	E2_SMS_TEXT_ALARM_PHONE_LEN,		//0x0044//SMS�ı������绰����
	E2_ALARM_MASK_LEN,					//15����ID0x0050,����������//bit9,11,12Ĭ������,LCD������,TTS����,IC��ģ�����Ĭ��Ϊ����
	E2_MAX_SPEED_LEN, 					//16����ID0x0055,����ٶ�,��λkm/h,DWORD,Ĭ��ֵ130,��ͨ����Ϊ100
	E2_OVER_SPEED_KEEP_TIME_LEN,		//17����ID0x0056,���ٳ���ʱ��,��λΪ��,DWORD,Ĭ��Ϊ10��,��ͨ����Ϊ�����ϱ�
	E2_SET_CRASH_ALARM_LEN,				//18����ID0x005D,��ײ������������,��ײ���ٶ�Ĭ������Ϊ4g
	E2_SET_ROLLOVER_ALARM_LEN,			//19����ID0x005E,�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ45 ��
	E2_CAR_TOTAL_MILE_LEN,				//20����ID0x0080��������̱������0.1km
	E2_CAR_PROVINCE_LEN,				//21����ID0x0081,�������ڵ�ʡ��ID
	E2_CAR_CITY_LEN,					//22����ID0x0082,�������ڵ�����ID
	E2_CAR_PLATE_NUM_LEN, 				//23����ID0x0083,������ͨ�����Ű䷢�Ļ���������
	E2_CAR_PLATE_COLOR_LEN,				//24����ID0x0084,������ɫ//����JT/T415-2006��5.4.12,1:��ɫ,2:��ɫ,3:��ɫ,4:��ɫ,9:����
    E2_GPS_SET_MODE_LEN,                //25����ID0x0090 GNSS ��λģʽ��Ĭ��Ϊ������λ   
	E2_LOGINGPRS_PASSWORD_ID_LEN,		//26����ID0xF200+0x00���Զ��壬��Ȩ��
	E2_MANUFACTURE_ID_LEN,				//27����ID0xF205���Զ��壬������ID��5�ֽ�	
    E2_DEVICE_ID_LEN,                   //28����ID0xF200+0x06���Զ��壬�����ն�ID��7�ֽ�
    E2_DEVICE_PHONE_ID_LEN,             //29����ID0xF200+0x07���Զ��壬�ն��ֻ��ţ�6�ֽ�
    E2_CAR_VIN_ID_LEN,					//30����ID0xF208���Զ��壬����ʶ����(VIN)��17�ֽڣ�ASCII��
    E2_DEFINE_ALARM_MASK_WORD_ID_LEN,	//31����ID0xF243���Զ��壬�Զ��屨�������֣�DWORD
    E2_DEFINE_HARD_VER_ID_LEN,			//32����ID0x0001F0004���Զ��壬�Զ���Ӳ���汾�ţ�DWORD
	E2_SMS_ALARM_PHONE_NUM_ID_LEN,		//����ID 0x0001F005���Զ��壬�Զ����û����ű����绰�
	E2_SMS_OPERATE_PWD_ID_LEN,			//����ID 0x0001F006���Զ��壬�Զ����û����Ų�������
	E2_LOW_VOL_THRESHOLD_ID_LEN,		//����ID 0x0001F00F���Զ��壬�Զ���͵�ѹ��ֵ��WORD
};
//////////ֻ��Ϊ0��1////////////
const u8 EepromPramSwitch[] = 
{
    0,
    0,		//01����ID0x0001���ն��������ͼ������λ�룬Ĭ��ֵ60
    1,		//02����ID0x0010����������APN
    1,      //03����ID0x0011��������������ͨ�Ų����û���
    1,      //04����ID0x0012��������������ͨ�Ų�������
    1,		//05����ID0x0013������������ַ��IP������
	1,		//06����ID0x0014�����ݷ�����APN������ͨ�Ų��ŷ��ʵ�
	1,		//07����ID0x0015�����ݷ���������ͨ�Ų����û���
	1,		//08����ID0x0016�����ݷ���������ͨ�Ų�������
	1,		//09����ID0x0017�����ݷ�������ַ��IP������
    0,		//10����ID0x0018����������TCP�˿�
    0,		//11����ID0x0027������ʱ�㱨ʱ��������λΪ�룬Ĭ��ֵΪ60
    0,		//12����ID0x0028,��������ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ10
    0,		//13����ID0x0029��ȱʡʱ��㱨���,��λΪ�룬Ĭ��ֵΪ10  
	0,		//14����ID0x0030���յ㲹���Ƕ�,��λ��/�룬Ĭ��ֵΪ15
	1,		//0x0043//���ƽ̨SMS�绰����
	1,		//0x0044//SMS�ı������绰����
	0,		//15����ID0x0050�����������֣�Ĭ��Ϊȫ��
	0,		//16����ID0x0055������ٶ�
	0,		//17����ID0x0056�����ٳ���ʱ�䣬��λΪ�룬Ĭ��Ϊ10��
    0,		//18����ID0x005D,��ײ������������ 
    0,		//19����ID0x005E,�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ30 ��
	0,		//20����ID0x0080��������̱������0.1km
	0,		//21����ID0x0081���������ڵ�ʡ��ID
	0,		//22����ID0x0082���������ڵ�����ID
	1,		//23����ID0x0083��������ͨ�����Ű䷢�Ļ���������
	0,		//24����ID0x0084��������ɫ������JT/T415-2006��5.4.12
    0,      //25����ID0x0090 GNSS ��λģʽ��Ĭ��Ϊ������λ
	1,		//26����ID0xF200+0x00���Զ��壬��Ȩ��
	1,		//27����ID0xF200+0x05���Զ��壬������ID��5�ֽ�
	1,		//28����ID0xF200+0x06���Զ��壬�����ն�ID��7�ֽ�
	1,		//29����ID0xF200+0x07���Զ��壬�ն��ֻ��ţ�6�ֽ�
	1,		//30����ID0xF200+0x08���Զ��壬����ʶ����
	0,		//31����ID0xF200+0x43���Զ��壬�Զ��屨�������֣�DWORD
	1,		//32����ID0x0001F0004���Զ��壬�Զ���Ӳ���汾�ţ�DWORD
	1,		//����ID 0x0001F005���Զ��壬�Զ����û����ű����绰�
	1,		//����ID 0x0001F006���Զ��壬�Զ����û����Ų�������
	0,		//����ID 0x0001F00F���Զ��壬�Զ���͵�ѹ��ֵ��WORD
};
const u16 EepromPramAddress[] = 
{
    0,
    E2_TERMINAL_HEARTBEAT_ADDR,             //01����ID0x0001���ն��������ͼ������λ�룬Ĭ��ֵ60
    E2_MAIN_SERVER_APN_ADDR,                //02����ID0x0010����������APN  
    E2_MAIN_SERVER_DIAL_NAME_ADDR,          //03����ID0x0011��������������ͨ�Ų����û���
    E2_MAIN_SERVER_DIAL_PASSWORD_ADDR,      //04����ID0x0012��������������ͨ�Ų�������
    E2_MAIN_SERVER_IP_ADDR,                 //05����ID0x0013������������ַ��IP������  
	E2_BACKUP_SERVER_APN_ADDR,				//06����ID0x0014�����ݷ�����APN������ͨ�Ų��ŷ��ʵ�
	E2_BACKUP_SERVER_DIAL_NAME_ADDR,		//07����ID0x0015�����ݷ���������ͨ�Ų����û���
	E2_BACKUP_SERVER_DIAL_PASSWORD_ADDR,	//08����ID0x0016�����ݷ���������ͨ�Ų�������
	E2_BACKUP_SERVER_IP_ADDR,				//09����ID0x0017�����ݷ�������ַ��IP������
    E2_MAIN_SERVER_TCP_PORT_ADDR,           //10����ID0x0018����������TCP�˿� 
    E2_SLEEP_REPORT_TIME_ADDR,              //11����ID0x0027������ʱ�㱨ʱ��������λΪ�룬Ĭ��ֵΪ60    
    E2_EMERGENCY_REPORT_TIME_ADR,			//12����ID0x0028,��������ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ10
    E2_ACCON_REPORT_TIME_ADDR,              //13����ID0x0029 ȱʡʱ��㱨���,��λΪ�룬Ĭ��ֵΪ10  
    E2_CORNER_REPORT_ADDR,					//14����ID0x0030���յ㲹���Ƕ�,��λ��/�룬Ĭ��ֵΪ15
    E2_MONITOR_SERVER_SMS_PHONE_ADDR,       //����ID0x0043�����ƽ̨SMS�绰����
    E2_SMS_TEXT_ALARM_PHONE_ADDR,			//0x0044//SMS�ı������绰����
	E2_ALARM_MASK_ADDR,						//15����ID0x0050�����������֣�Ĭ��Ϊȫ��
	E2_MAX_SPEED_ADDR,						//16����ID0x0055������ٶ�
	E2_OVER_SPEED_KEEP_TIME_ADDR,			//17����ID0x0056�����ٳ���ʱ�䣬��λΪ�룬Ĭ��Ϊ10��
	E2_SET_CRASH_ALARM_ADDR,				//18����ID0x005D//��ײ������������ 
	E2_SET_ROLLOVER_ALARM_ADDR,				//19����ID0x005E//�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ30 ��
	E2_CAR_TOTAL_MILE_ADDR,					//20����ID0x0080��������̱������0.1km	
	E2_CAR_PROVINCE_ID_ADDR,				//21����ID0x0081���������ڵ�ʡ��ID
	E2_CAR_CITY_ID_ADDR,					//22����ID0x0082���������ڵ�����ID
	E2_CAR_PLATE_NUM_ADDR,					//23����ID0x0083��������ͨ�����Ű䷢�Ļ���������
	E2_CAR_PLATE_COLOR_ADDR,				//24����ID0x0084��������ɫ������JT/T415-2006��5.4.12
    E2_GPS_SET_MODE_ADDR,                   //25����ID0x0090,GNSS ��λģʽ��Ĭ��Ϊ������λ    
   	E2_LOGINGPRS_PASSWORD_ID_ADDR,			//26����ID0xF200+0x00���Զ��壬��Ȩ��
	E2_MANUFACTURE_ID_ADDR,					//27����ID0xF200+0x05���Զ��壬������ID��5�ֽ�
	E2_DEVICE_ID_ADDR,						//28����ID0xF200+0x06���Զ��壬�����ն�ID��7�ֽ�
	E2_DEVICE_PHONE_ID_ADDR,				//29����ID0xF200+0x07���Զ��壬�ն��ֻ��ţ�6�ֽ�
	E2_CAR_VIN_ID_ADDR,						//30����ID0xF200+0x08���Զ��壬����ʶ����
	E2_DEFINE_ALARM_MASK_WORD_ID_ADDR,		//31����ID0xF200+0x43���Զ��壬�Զ��屨�������֣�DWORD
	E2_DEFINE_HARD_VER_ID_ADDR,				//32����ID0x0001F0004���Զ��壬�Զ���Ӳ���汾�ţ�DWORD
	E2_SMS_ALARM_PHONE_NUM_ID_ADDR, 		//����ID 0x0001F005���Զ��壬�Զ����û����ű����绰
	E2_SMS_OPERATE_PWD_ID_ADDR,     		//����ID 0x0001F006���Զ��壬�Զ����û����Ų������� 
	E2_LOW_VOL_THRESHOLD_ID_ADDR,			//����ID 0x0001F00F���Զ��壬�Զ���͵�ѹ��ֵ��WORD
};

/*************************************************************
** ��������: Eepromflash_ReadBytes
** ��������: ���ֽڲ���
** ��ڲ���: Address������ʼ��ַ,*data �������ݴ�ŵ�ַָ��,len �����ֽ���
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
u8 E2prom_ReadBytes(char* lpFileName,unsigned long Address, unsigned char *data, unsigned short len)
{
	s32 ret=0;
	s32 handle=-1;
	u32 hasreadenLen = 0;//�Ѿ�д���ļ����ֽ�������ʼֵΪ��

	handle = Ql_FS_Open(lpFileName,QL_FS_READ_ONLY);
	 if(handle<0)
	 {
		 APP_DEBUG("\r\n<--The %s file does not exist,handle:%d-->\r\n",lpFileName,handle);
		 handle = Ql_FS_Open(lpFileName,QL_FS_READ_ONLY);
		 if(handle<0)
		  {
			  APP_DEBUG("\r\n<--The %s file does not exist,handle:%d-->\r\n",lpFileName,handle);
			  return 0;
		  }
	 }
	 ret = Ql_FS_Seek(handle,Address,QL_FS_FILE_BEGIN); 
	 ret = Ql_FS_Read(handle, data, len, &hasreadenLen);
	 /*
	 APP_DEBUG("\r\n<--Eepromflash_ReadBytes  ret =%d  hasreadenLen =%d-->\r\n",ret,hasreadenLen);
	 if(len>5)
	 {
		APP_DEBUG("\r\n<--E2prom_ReadBytes len=%d Address=%d	Data=%s-->\r\n",len,Address,data);
 	}
 	else
 	{
 		APP_DEBUG("\r\n<--E2prom_ReadBytes len=%d Address=%d Data=%d",len,Address,data[0]);
 		u8 i;
 		for(i=1;i<len;i++)
 			{
 			APP_DEBUG("%d",data[i]);
 			}
 		APP_DEBUG("-->\r\n");
 	 }
 	 */
 	 Ql_FS_Close(handle);//close the file
	if(ret==QL_RET_OK)
	{
		return len;
	}
	else
	{
		return 0;
	}
}
/*************************************************************
** ��������: Eepromflash_WriteBytes
** ��������: д�ֽڲ���
** ��ڲ���: Addressд����ʼ��ַ,*data д�����ݴ�ŵ�ַָ��,len д���ֽ���
** ���ڲ���: 
** ���ز���: 1:�ɹ���0:ʧ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
unsigned char E2prom_WriteBytes(char* lpFileName,unsigned short Address, unsigned char *data,unsigned short len)
{
s32 ret=0;
s32 handle=-1;
u32 haswritenLen = 0;//�Ѿ�д���ļ����ֽ�������ʼֵΪ��

handle = Ql_FS_Open(lpFileName,QL_FS_READ_WRITE);
if(handle<0)
{
   APP_DEBUG("\r\n<--The %s file does not exist,handle:%d-->\r\n",lpFileName,handle);
   handle = Ql_FS_Open(lpFileName,QL_FS_READ_WRITE);
	if(handle<0)
	{
	   APP_DEBUG("\r\n<--The %s file does not exist,handle:%d-->\r\n",lpFileName,handle);
	   return 0;
	}
}
ret = Ql_FS_Seek(handle,Address,QL_FS_FILE_BEGIN);	//seek end 
ret = Ql_FS_Write(handle, data, len, &haswritenLen);
//APP_DEBUG("\r\n<--!! Function (E2prom_WriteBytes)  ret =%d  haswritenLen =%d-->\r\n",ret,haswritenLen);
//APP_DEBUG("%s",data);
/*
if(len>5)
{
APP_DEBUG("\r\n<--E2prom_WriteBytes len=%d Address=%d	Data=%s-->\r\n",len,Address,data);
}
else
{
	APP_DEBUG("\r\n<--E2prom_WriteBytes len=%d Address=%d Data=%d",len,Address,data[0]);
	u8 i;
	for(i=1;i<len;i++)
		{
		APP_DEBUG("-%d",data[i]);
		}
	APP_DEBUG("-->\r\n");
}
*/
Ql_FS_Flush(handle); //fflushǿ�ƻ���д���ļ�
Ql_FS_Close(handle);//close the file
return haswritenLen;
}

u8 FRAM_BufferWrite(unsigned short WriteAddr,unsigned char *pBuffer,unsigned short  NumBytesToWrite) 
{
	E2prom_WriteBytes(PRAM_FILE_NAME,WriteAddr, pBuffer, NumBytesToWrite);
}

u8 FRAM_BufferRead(unsigned char *pBuffer,unsigned short NumBytesToRead,unsigned long ReadAddr) 
{
	return	E2prom_ReadBytes(PRAM_FILE_NAME,ReadAddr,pBuffer,NumBytesToRead);
}

//********************************���ر���**********************************
/*************************************************************OK
** ��������: EepromPram_ConvertParamIDToIndex
** ��������: ���ַ�����������Ҫ���ҵļ�¼
** ��ڲ���: 
** ���ڲ���: ����Ҫ���ҵ�Index
** ���ز���: 
** ȫ�ֱ���: ��
** ����ģ��:
*************************************************************/
unsigned char EepromPram_ConvertParamIDToIndex(unsigned int ParamID)//
{
    unsigned char index=0;
    ///////////////
    for(index = 1; index<EepromPramID_SIZE;index++)
    {
        if(ParamID==c_usEepromPramID[index])return index;;//�ҵ�ID����
    }
	return 0;
}

/*********************************************************************
//��������	:EepromPram_WritePram
//����		:дeeprom����
//����		:PramID:����ID;pBuffer:ָ�����ݣ���д����ֽ���,д�������ֵ�Ļ�����ֽ�Ӧ��ǰ����ָ��;
//		    :BufferLen:���ݵĳ���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:д����ȷʱ�����صĳ��ȵ���BufferLen;д�����ʱ������0,��ʱ�ɳ����ٵ��øú���дһ��
*********************************************************************/
u16 EepromPram_WritePramEx(unsigned short index, u8 *pBuffer, u8 BufferLen)
{
    u8	VerifyByte;
    u8	sum;
    u8	len;
    u8	Buffer[52];
    u8	tmp[52];
    u8	PramType;//0Ϊֵ,1Ϊ�ַ���
    u16	Address;
    //////////////////
    if(0==index)//����ʶ��ID
    {
        return 0;//����ʱ����
    }
    //�жϳ����Ƿ���ȷ
    if((BufferLen > EepromPramLength[index])||(0 == BufferLen))
    {
        return 0;//���Ȼ����ID����
    }
    //////////////////////////////    
    //��У���//Ϊʵ�����ݵ��ۼӺ�
    VerifyByte = Public_GetSumVerify(pBuffer,BufferLen);
    //��ȡ��������
    PramType = EepromPramSwitch[index];
    //**************д����****************
    //��ȡ��ַ
    Address = EepromPramAddress[index];
	//APP_DEBUG("\r\n<--WritePramID Address=%d  PramType =%d-->\r\n",Address,PramType);
    //////////////////////
    len = 0;
    ////////////////
    if(PramType)//д�����ַ���,//��һ���ֽ�д���ַ����ĳ���
    {
        PramType = 1;
        Buffer[len++] = BufferLen;
    }
    ////////����д�뵽eeprom/////////////
    memcpy(&Buffer[len],pBuffer,BufferLen);
    len +=BufferLen;
    //д��У���ֽ�
    Buffer[len++] = VerifyByte;
    //���д�뵽eeprom
    //APP_DEBUG("����...%d..%d....%04x",index,BufferLen,Address);
	//if(index==36)
	//{
	//	Address = E2_TERMINAL_HEARTBEAT_ADDR;
	//}
    E2prom_WriteBytes(PRAM_FILE_NAME,Address,Buffer,len);
    //**************������****************
    if(BufferLen > 49)return BufferLen;//�ֽ������ڵ���30�ֽڵ�ֱ�ӷ���,��У��
    ///////////////
    //����д������ݣ����Ⱥ�У���ֽڳ���
    E2prom_ReadBytes(PRAM_FILE_NAME,Address+PramType, tmp, BufferLen+1);
	/*
		if(PramType==1)
			{
	 		APP_DEBUG("\r\n<--E2prom_ReadBytes len=%d Address=%d Data=%c",BufferLen+1,Address+PramType,tmp[0]);
 		u8 i;
 		for(i=1;i<BufferLen+1;i++)
 			{
 			APP_DEBUG("%c",tmp[i]);
 			}
 		APP_DEBUG("-->\r\n");
			}
		else
			{
	 		APP_DEBUG("\r\n<--E2prom_ReadBytes len=%d Address=%d Data=%d",BufferLen+1,Address+PramType,tmp[0]);
 		u8 i;
 		for(i=1;i<BufferLen+1;i++)
 			{
 			APP_DEBUG("-%d",tmp[i]);
 			}
 		APP_DEBUG("-->\r\n");
			}
			*/
    //����������ݵ�У���
    sum  = Public_GetSumVerify(tmp,BufferLen);
    //�Ƚ�У���
    if((sum == VerifyByte)&&(tmp[BufferLen]==VerifyByte))
    {
        return BufferLen;
    }
    ///////У�����,��д��һ��/////////
    E2prom_WriteBytes(PRAM_FILE_NAME,Address,Buffer,len);
    ////////////
    return 0; 
}
/*********************************************************************
//��������	:EepromPram_WritePram(u32 PramID, u8 *pBuffer, BufferLen)
//����		:дeeprom����
//����		:PramID:����ID;pBuffer:ָ�����ݣ���д����ֽ���,д�������ֵ�Ļ�����ֽ�Ӧ��ǰ����ָ��;
//		:BufferLen:���ݵĳ���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:д����ȷʱ�����صĳ��ȵ���BufferLen;д�����ʱ������0,��ʱ�ɳ����ٵ��øú���дһ��
*********************************************************************/
u16 EepromPram_WritePram(u32 PramID, u8 *pBuffer, u8 BufferLen)
{
    unsigned char index;
//APP_DEBUG("\r\n<--WritePramID:=%d-->\r\n",PramID);
    index = EepromPram_ConvertParamIDToIndex(PramID); 
//APP_DEBUG("\r\n<--WritePramID_INDEX:=%d-->\r\n",index);
    return EepromPram_WritePramEx(index,pBuffer,BufferLen);
}
/*********************************************************************
//��������	:EepromPram_ReadPram(u32 PramID, u8 *pBuffer)
//����		:��eeprom����
//����		:PramID:����ID;pBuffer:ָ�����ݣ��������ֽ���������������ֵ����ֽ���ǰ����ָ��;
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:����ֵ>0��ʾ��ȷ����������ֵ==0��ʾ������;
*********************************************************************/
u16 EepromPram_ReadPramEx(unsigned short index, u8 *pBuffer)
{
    u16	Address;
    u8 PramType;
    u8 length;    
    u8 sum;
    u8 Buffer[52];
    /////////////////////////
    if(0==index)return 0;//����ʱ����//����ʶ��ID
    ///////////////
    if(0 == EepromPramLength[index])//����ID��Ϊ������
    {
        return 0;
    }
    ///////////////
    Address = EepromPramAddress[index];
    ///////////////////
    PramType = EepromPramSwitch[index];
	
    if(PramType)
    {
    	//if(index == 36)
		//	Address = E2_TERMINAL_HEARTBEAT_ADDR;
        E2prom_ReadBytes(PRAM_FILE_NAME,Address, Buffer, 1);
        length = Buffer[0];//����
        //APP_DEBUG("����...%d..%d...%04x",index,length,Address);
        Address++;		
    }
    else
    {
        length = EepromPramLength[index];
    }
	
    ///////////////
    if(length > 49) return 0;   
    //�������ٶ�ȡ���ݺ�У���ֽ�
    E2prom_ReadBytes(PRAM_FILE_NAME,Address, Buffer, length+1);
    //����У���
    sum = Public_GetSumVerify(Buffer,length);
    if(sum == Buffer[length])//У����ȷ
    {//APP_DEBUG("\r\nУ����ȷ.....%d\r\n",length)
        memcpy(pBuffer,Buffer,length);
        return length;
    }
    ////////////
    pBuffer[0]=0;
    return 0;
}
/*********************************************************************
//��������	:EepromPram_ReadPram(u32 PramID, u8 *pBuffer)
//����		:��eeprom����
//����		:PramID:����ID;pBuffer:ָ�����ݣ��������ֽ���������������ֵ����ֽ���ǰ����ָ��;
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:����ֵ>0��ʾ��ȷ����������ֵ==0��ʾ������;
*********************************************************************/
u16 EepromPram_ReadPram(u32 PramID, u8 *pBuffer)
{
    unsigned char index;    
    index = EepromPram_ConvertParamIDToIndex(PramID);
    return EepromPram_ReadPramEx(index,pBuffer);
}
/*********************************************************************
//��������	:EepromPram_UpdateVariable(u16 PramID)
//����		:����PramID����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ǧ��ע��:EepromPram_UpdateVariable(E2_CAR_TOTAL_MILE_ID)���������ã�ֻ����������E2_CAR_TOTAL_MILE_ID�����              
*********************************************************************/
void EepromPram_UpdateVariable(u32 PramID)
{
	u8	Buffer[30];
	u8	BufferLen;
	u32	temp;
       
	switch(PramID)
	{
	case E2_TERMINAL_HEARTBEAT_ID:		
		{
			HeartBeat_UpdatePram();
			break;
		}//����ID0x0001���ն��������ͼ������λ�룬Ĭ��ֵ60
	case E2_MAIN_SERVER_APN_ID:	
                {
                        break;
                }//����ID0x0010����������APN
	//E2_MAIN_SERVER_DIAL_NAME_ID:			//����ID0x0011��������������ͨ�Ų����û���
	//E2_MAIN_SERVER_DIAL_PASSWORD_ID:		//����ID0x0012��������������ͨ�Ų�������
	//E2_MAIN_SERVER_IP_ID:				//����ID0x0013������������ַ��IP������
	//E2_BACKUP_SERVER_APN_ID:			//����ID0x0014�����ݷ�����APN������ͨ�Ų��ŷ��ʵ�
	//E2_BACKUP_SERVER_DIAL_NAME_ID:		//����ID0x0015�����ݷ���������ͨ�Ų����û���
	//E2_BACKUP_SERVER_DIAL_PASSWORD_ID:		//����ID0x0016�����ݷ���������ͨ�Ų�������
	//E2_BACKUP_SERVER_IP_ID:			//����ID0x0017�����ݷ�������ַ��IP������
	//E2_MAIN_SERVER_TCP_PORT_ID:			//����ID0x0018����������TCP�˿�
	//E2_MAIN_SERVER_UDP_PORT_ID:			//����ID0x0019����������UDP�˿�
	//E2_BACKUP_SERVER_TCP_PORT_ID:		//����ID0x001a�����ݷ�����TCP�˿�
	//E2_BACKUP_SERVER_UDP_PORT_ID:		//����ID0x001b�����ݷ�����UDP�˿�
	//E2_REGISTER_SMS_CENTER_NUM_ID:		//����ID0x001C��ע��������ĺ���
	//E2_PRIVILEGE_SMS_CENTER_NUM_ID:		//����ID0x001D�����ƶ��ź��루��Ȩ��
	//0,0,						//����ID0x001E~0x001f������
	case E2_SLEEP_REPORT_TIME_ID://0X0027 ����ACC OFF�㱨ʱ����
		{
			Report_UpdatePram();
			break;
		}
	case E2_EMERGENCY_REPORT_TIME_ID://0X0028 ���½�������ʱ�㱨ʱ����
		{
			//Report_UpdateOnePram(PRAM_EMERGENCY_REPORT_TIME);
			//Report_UpdatePram();
			break;
		}
	case E2_ACCON_REPORT_TIME_ID://0X0029 ����ACC ON�㱨ʱ����
		{
			Report_UpdatePram();
			//Report_UpdateOnePram(PRAM_ACC_ON_REPORT_TIME);
			break;
		}
	case E2_CORNER_REPORT_ID://����ID0x0030���յ㲹���Ƕ�,��λ��/�룬Ĭ��ֵΪ15
		{
			//Corner_UpdataPram();
			break;
		}
	case E2_ALARM_MASK_ID://����ID0x0050�����������֣�Ĭ��Ϊȫ��
	case E2_DEFINE_ALARM_MASK_WORD_ID://����ID���Զ��壬�Զ��屨�������֣�DWORD
		{
			Io_UpdataPram();
            //Photo_UpdatePram();
			break;
		}
	case E2_MAX_SPEED_ID://����ID0x0055������ٶ�
		{
             //Overspeed_UpdateParameter(); //����ʹ��
			break;
		}
	case E2_OVER_SPEED_KEEP_TIME_ID://����ID0x0056�����ٳ���ʱ�䣬��λΪ�룬Ĭ��Ϊ10��
		{
              //Overspeed_UpdateParameter();//����ʹ��

			break;
		}
        case E2_SET_CRASH_ALARM_ID:	//����ID0x005D,��ײ������������
		{
			//UpdataBmaAppPra();
			break;
		}
        case E2_SET_ROLLOVER_ALARM_ID:	//����ID0x005E,�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ30 �� 
		{
			//BMA250_UpdataRollOverAlarmParam();
			//UpdataBmaAppPra();
			break;
		}            
	case E2_CAR_TOTAL_MILE_ID:	//����ID0x0080��������̱������0.1km,ǧ��ע��:EepromPram_UpdateVariable(E2_CAR_TOTAL_MILE_ID)���������ã�ֻ����������E2_CAR_TOTAL_MILE_ID�����                                  			
		{
			//��ȡ�ո����õ����
			EepromPram_ReadPram(E2_CAR_TOTAL_MILE_ID,Buffer);//0.1����Ϊ��λ
			temp = 0;
			temp |= Buffer[0] << 24;
			temp |= Buffer[1] << 16;
			temp |= Buffer[2] << 8;
			temp |= Buffer[3];
			//���ó�ʼ���
			//GpsMile_SetPram(temp);
			
			break;
		}           
        case E2_GPS_SET_MODE_ID://����ID0x0090,GNSS ��λģʽ��Ĭ��Ϊ0x02 ������λ	
		{
              //GnssCheckChangePar();
			
			break;
		}
	case E2_MANUFACTURE_ID://����IDE2_PRAM_BASE_CUSTOM_ID+0x05���Զ��壬������ID��5�ֽ�				
		{
			BufferLen = EepromPram_ReadPram(E2_MANUFACTURE_ID, Buffer);
			if(0 == BufferLen)
			{
			
			}
			break;
		}
	case E2_DEVICE_ID://����IDE2_PRAM_BASE_CUSTOM_ID+0x06���Զ��壬�ն�ID��7�ֽ�
		{
			BufferLen = EepromPram_ReadPram(E2_DEVICE_ID, Buffer);
			if(0 == BufferLen)
			{
			
			}
			break;
		}
	case E2_DEVICE_PHONE_ID://����IDE2_PRAM_BASE_CUSTOM_ID+0x07���Զ��壬�ն��ֻ��ţ�6�ֽ�
		{
			//RadioProtocol_UpdateTerminalPhoneNum();
			break;
		}
	case E2_MONITOR_SERVER_SMS_PHONE_ID:
			{
				UpSmsMonitorPhoNum();
				break;
			}
	case E2_SMS_ALARM_PHONE_NUM_ID:
			{
				UpSmsAlarmPhoNum();
				break;
			}
		default:	
          break;
	}

}

/*********************************************************************
//��������	:EepromPram_DefaultSet(void)
//����		:ϵͳ��1������ʱ��Ĭ������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ʹ�ûָ�Ĭ�ϳ�������Ҳ�Ὣ��������Ϊ������ʾ
*********************************************************************/
void EepromPram_DefaultSet(void)
{
    u32	temp=0;
	u8	Buffer[31]={0};
	u8	BufferLen=0;
    
    //0x0001	�ն��������ͼ��//��λ��//Ĭ��ֵ50//60����������
    temp = 50;//60����������
    Public_ConvertLongToBuffer(temp, Buffer);
    EepromPram_WritePram(E2_TERMINAL_HEARTBEAT_ID, Buffer, E2_TERMINAL_HEARTBEAT_LEN); 

	//0x0010	   ��������APN
	strcpy((char *)Buffer,"cmnet");
	BufferLen = strlen((char const *)Buffer);
	if(0 == EepromPram_WritePram(E2_MAIN_SERVER_APN_ID, Buffer, BufferLen))
	{
		EepromPram_WritePram(E2_MAIN_SERVER_APN_ID, Buffer, BufferLen);
	}
	//0x0011  ������������ͨ�Ų����û���
	strcpy((char *)Buffer,"card");
	BufferLen = strlen((char const *)Buffer);
	if(0 == EepromPram_WritePram(E2_MAIN_SERVER_DIAL_NAME_ID, Buffer, BufferLen))
	{
		EepromPram_WritePram(E2_MAIN_SERVER_DIAL_NAME_ID, Buffer, BufferLen);
	}
	//0x0012  ������������ͨ�Ų�������
	strcpy((char *)Buffer,"card");
	BufferLen = strlen((char const *)Buffer);
	if(0 == EepromPram_WritePram(E2_MAIN_SERVER_DIAL_PASSWORD_ID, Buffer, BufferLen))
	{
		EepromPram_WritePram(E2_MAIN_SERVER_DIAL_PASSWORD_ID, Buffer, BufferLen);
	}

	//0x0014 ���ݷ�����APN
	strcpy((char *)Buffer,"cmnet");
	BufferLen = strlen((char const *)Buffer);
	if(0 == EepromPram_WritePram(E2_BACKUP_SERVER_APN_ID, Buffer, BufferLen))
	{
		EepromPram_WritePram(E2_BACKUP_SERVER_APN_ID, Buffer, BufferLen);
	}
	//0x0015 ���ݷ���������ͨ�Ų����û���
	strcpy((char *)Buffer,"card");
	BufferLen = strlen((char const *)Buffer);
	if(0 == EepromPram_WritePram(E2_BACKUP_SERVER_DIAL_NAME_ID, Buffer, BufferLen))
	{
		EepromPram_WritePram(E2_BACKUP_SERVER_DIAL_NAME_ID, Buffer, BufferLen);
	}
	//0x0016 ���ݷ���������ͨ�Ų�������
	strcpy((char *)Buffer,"card");
	BufferLen = strlen((char const *)Buffer);
	if(0 == EepromPram_WritePram(E2_BACKUP_SERVER_DIAL_PASSWORD_ID, Buffer, BufferLen))
	{
		EepromPram_WritePram(E2_BACKUP_SERVER_DIAL_PASSWORD_ID, Buffer, BufferLen);
	}	 
    //0x0027	����ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ1200
    temp = 1200;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_SLEEP_REPORT_TIME_ID, Buffer, E2_SLEEP_REPORT_TIME_LEN))
    {
        EepromPram_WritePram(E2_SLEEP_REPORT_TIME_ID, Buffer, E2_SLEEP_REPORT_TIME_LEN);
    }

    //0x0028	��������ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ10
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_EMERGENCY_REPORT_TIME_ID, Buffer, E2_EMERGENCY_REPORT_TIME_LEN))
    {
        EepromPram_WritePram(E2_EMERGENCY_REPORT_TIME_ID, Buffer, E2_EMERGENCY_REPORT_TIME_LEN);
    }

    //0x0029	ȱʡʱ��㱨���,��λΪ��,Ĭ��Ϊ30
    temp = 30;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ACCON_REPORT_TIME_ID, Buffer, E2_ACCON_REPORT_TIME_LEN))
    {
        EepromPram_WritePram(E2_ACCON_REPORT_TIME_ID, Buffer, E2_ACCON_REPORT_TIME_LEN);
    }
    //0x0030	�յ㲹���Ƕ�,��λ��/��//Ĭ��ֵΪ30
    temp = 30;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_CORNER_REPORT_ID, Buffer, E2_CORNER_REPORT_LEN))
    {
        EepromPram_WritePram(E2_CORNER_REPORT_ID, Buffer, E2_CORNER_REPORT_LEN);
    }
    //0x0050	����������//bit9,11,12Ĭ������,LCD������,����ͷ����,IC��ģ�����,VSS�����쳣Ĭ��Ϊ����
    temp = 0x01001a00;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ALARM_MASK_ID, Buffer, E2_ALARM_MASK_LEN))
    {
        EepromPram_WritePram(E2_ALARM_MASK_ID, Buffer, E2_ALARM_MASK_LEN);
    }
    //0x0055	����ٶ�,��λkm/h,DWORD,Ĭ��ֵ100km/h
    temp = 100;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MAX_SPEED_ID, Buffer, E2_MAX_SPEED_LEN))
    {
        EepromPram_WritePram(E2_MAX_SPEED_ID, Buffer, E2_MAX_SPEED_LEN);
    }
    //0x0056	���ٳ���ʱ��,��λΪ��,DWORD,Ĭ��Ϊ10��,��ͨҪ�������ϱ�
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVER_SPEED_KEEP_TIME_ID, Buffer, E2_OVER_SPEED_KEEP_TIME_LEN))
    {
        EepromPram_WritePram(E2_OVER_SPEED_KEEP_TIME_ID, Buffer, E2_OVER_SPEED_KEEP_TIME_LEN);
    }
    //0x005D	��ײ������������,��ײʱ��Ĭ��Ϊ4ms,��ײ���ٶ�Ĭ������Ϊ4g
    temp = 0x2801;//4g,4ms
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_SET_CRASH_ALARM_ID, Buffer, E2_SET_CRASH_ALARM_LEN))
    {
        EepromPram_WritePram(E2_SET_CRASH_ALARM_ID, Buffer, E2_SET_CRASH_ALARM_LEN);
    }
    //0x005E	�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ45 ��
    temp = 45;//45��
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_SET_ROLLOVER_ALARM_ID, Buffer, E2_SET_ROLLOVER_ALARM_LEN))
    {
        EepromPram_WritePram(E2_SET_ROLLOVER_ALARM_ID, Buffer, E2_SET_ROLLOVER_ALARM_LEN);
    }
    //0x0080,�ۼ���ʻ�����0,������̱����, ��λ0.1km
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_CAR_TOTAL_MILE_ID, Buffer, E2_CAR_TOTAL_MILE_LEN))
    {
        EepromPram_WritePram(E2_CAR_TOTAL_MILE_ID, Buffer, E2_CAR_TOTAL_MILE_LEN);
    }

    //0x0090  GNSS ��λģʽ��Ĭ��Ϊ0x03 ������λ+GPS��ȫ������ƽ̨��������
    temp = 3;
    Buffer[0] = temp;//modify by joneming
    if(0 == EepromPram_WritePram(E2_GPS_SET_MODE_ID, Buffer, E2_GPS_SET_MODE_LEN))
    {
        EepromPram_WritePram(E2_GPS_SET_MODE_ID, Buffer, E2_GPS_SET_MODE_LEN);
    }
    //����ID 0xF205���Զ��壬������ID��5�ֽ�	������ID,Ĭ��Ϊ70108
    strcpy((char *)Buffer,"70108");
    if(0 == EepromPram_WritePram(E2_MANUFACTURE_ID, Buffer, E2_MANUFACTURE_ID_LEN))
    {
        EepromPram_WritePram(E2_MANUFACTURE_ID, Buffer, E2_MANUFACTURE_ID_LEN);
    }

	//0x0001f006 �û����Ų�������
	strcpy((char *)Buffer,"123456");
	BufferLen = strlen((char const *)Buffer);
	if(0 == EepromPram_WritePram(E2_SMS_OPERATE_PWD_ID, Buffer, BufferLen))
	{
		EepromPram_WritePram(E2_SMS_OPERATE_PWD_ID, Buffer, BufferLen);
	}

	//0x0001f00f �͵�ѹ�ػ���ѹ
	temp = 3500;//3500mv
    Public_ConvertShortToBuffer(temp, Buffer);
	if(0 == EepromPram_WritePram(E2_LOW_VOL_THRESHOLD_ID, Buffer, E2_LOW_VOL_THRESHOLD_ID_LEN))
	{
		EepromPram_WritePram(E2_LOW_VOL_THRESHOLD_ID, Buffer, E2_LOW_VOL_THRESHOLD_ID_LEN);
	}
}
/*************************************************************OK
** ��������: EepromPram_GetLenEepromPramID
** ��������: 
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: ��
** ����ģ��:
*************************************************************/
unsigned short EepromPram_GetLenEepromPramID(void)
{
    return EepromPramID_SIZE;
}


