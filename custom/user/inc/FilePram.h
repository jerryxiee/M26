#ifndef __FILEPRAM_H__
#define __FILEPRAM_H__

//#define FRAM_BufferWrite(WriteAddr, pBuffer, NumBytesToWrite) 0// E2prom_WriteBytes(WriteAddr, pBuffer, NumBytesToWrite)

//#define FRAM_BufferRead(pBuffer,NumBytesToRead,ReadAddr)  0// E2prom_ReadBytes(ReadAddr,pBuffer,NumBytesToRead)

#define PRAM_FILE_NAME    "device_pram.txt"


#define PARAMTER_MAX_LEN                   25//ȡǰ�涨��Ĳ����������ֵ

/***********����ID���壬˳����PARAMETER_STRUCT����**********************/
#define E2_TERMINAL_HEARTBEAT_ID 			0x0001//����ID 0x0001,�ն��������ͼ��//��λ��//Ĭ��ֵ60
#define E2_MAIN_SERVER_APN_ID 				0x0010//����ID 0x0010,��������APN
#define E2_MAIN_SERVER_DIAL_NAME_ID 		0x0011//����ID 0x0011,������������ͨ�Ų����û���
#define E2_MAIN_SERVER_DIAL_PASSWORD_ID 	0x0012//����ID 0x0012,������������ͨ�Ų�������
#define E2_MAIN_SERVER_IP_ID 				0x0013//����ID 0x0013,����������ַ//IP������
#define E2_BACKUP_SERVER_APN_ID 			0x0014//����ID 0x0014,���ݷ�����APN//����ͨ�Ų��ŷ��ʵ�
#define E2_BACKUP_SERVER_DIAL_NAME_ID 		0x0015//����ID 0x0015,���ݷ���������ͨ�Ų����û���
#define E2_BACKUP_SERVER_DIAL_PASSWORD_ID 	0x0016//����ID 0x0016,���ݷ���������ͨ�Ų�������
#define E2_BACKUP_SERVER_IP_ID 				0x0017//����ID 0x0017,���ݷ�������ַ//IP������
#define E2_MAIN_SERVER_TCP_PORT_ID 			0x0018//����ID 0x0018,��������TCP�˿�
#define E2_SLEEP_REPORT_TIME_ID 			0x0027//����ID 0x0027,����ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ240
#define E2_EMERGENCY_REPORT_TIME_ID 		0x0028//����ID 0x0028,��������ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ10
#define	E2_ACCON_REPORT_TIME_ID				0x0029//����ID 0x0029,ȱʡʱ��㱨���,��λΪ��,Ĭ��Ϊ30
#define E2_CORNER_REPORT_ID 				0x0030//����ID 0x0030,�յ㲹���Ƕ�,��λ��/��//Ĭ��ֵΪ30
#define E2_MONITOR_SERVER_SMS_PHONE_ID      0x0043//���ƽ̨SMS�绰����
#define E2_SMS_TEXT_ALARM_PHONE_ID 			0x0044//SMS�ı������绰����
#define E2_ALARM_MASK_ID 					0x0050//����ID 0x0050,����������//bit9,11,12Ĭ������,LCD������,TTS����,IC��ģ�����Ĭ��Ϊ����
#define E2_MAX_SPEED_ID 					0x0055//����ID 0x0055,����ٶ�,��λkm/h,DWORD,Ĭ��ֵ130,��ͨ����Ϊ100
#define E2_OVER_SPEED_KEEP_TIME_ID 			0x0056//����ID 0x0056,���ٳ���ʱ��,��λΪ��,DWORD,Ĭ��Ϊ10��,��ͨ����Ϊ�����ϱ�
#define	E2_SET_CRASH_ALARM_ID				0x005D//����ID 0x005D,��ײ������������,��ײ���ٶ�Ĭ������Ϊ4g
#define	E2_SET_ROLLOVER_ALARM_ID	        0x005E//����ID 0x005E,�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ45 ��
#define	E2_CAR_TOTAL_MILE_ID				0x0080//����ID 0x0080��������̱������0.1km
#define E2_CAR_PROVINCE_ID 					0x0081//����ID 0x0081,�������ڵ�ʡ��ID
#define E2_CAR_CITY_ID 						0x0082//����ID 0x0082,�������ڵ�����ID
#define E2_CAR_PLATE_NUM_ID 				0x0083//����ID 0x0083,������ͨ�����Ű䷢�Ļ���������
#define E2_CAR_PLATE_COLOR_ID 				0x0084//������ID 0x0084,������ɫ//����JT/T415-2006��5.4.12,1:��ɫ,2:��ɫ,3:��ɫ,4:��ɫ,9:����

#define	E2_GPS_SET_MODE_ID					0x0090//����ID 0x0090,GNSS ��λģʽ��Ĭ��Ϊ0x02 ������λ

#define E2_PRAM_BASE_CUSTOM_ID           0xF200
#define E2_PRAM_BASE_CUSTOM_ID1          0x0001F000

#define E2_LOGINGPRS_PASSWORD_ID		(E2_PRAM_BASE_CUSTOM_ID+0x00)//����ID 0xF200,��Ȩ��
#define E2_MANUFACTURE_ID				(E2_PRAM_BASE_CUSTOM_ID+0x05)//����ID 0xF205���Զ��壬������ID��5�ֽ�	
#define E2_DEVICE_ID					(E2_PRAM_BASE_CUSTOM_ID+0x06)//����ID 0xF206���Զ��壬�����ն�ID��7�ֽ�
#define E2_DEVICE_PHONE_ID				(E2_PRAM_BASE_CUSTOM_ID+0x07)//����ID 0xF207���Զ��壬�ն��ֻ��ţ�6�ֽڣ�
#define E2_CAR_VIN_ID 					(E2_PRAM_BASE_CUSTOM_ID+0x08)//����ID 0xF208���Զ��壬����ʶ����(VIN)��17�ֽڣ�ASCII�룬
#define	E2_FIRMWARE_VERSION_ID			(E2_PRAM_BASE_CUSTOM_ID+0x0C)//����ID 0xF20C���Զ��壬�̼��汾��
#define E2_DEFINE_ALARM_MASK_WORD_ID	(E2_PRAM_BASE_CUSTOM_ID+0x43)//����ID 0xF243���Զ��壬�Զ��屨�������֣�DWORD
#define E2_DEFINE_HARD_VER_ID			(E2_PRAM_BASE_CUSTOM_ID1+0x04)//����ID 0x0001F004���Զ��壬�Զ���Ӳ���汾�ţ�DWORD
#define E2_SMS_ALARM_PHONE_NUM_ID		(E2_PRAM_BASE_CUSTOM_ID1+0x05)//����ID 0x0001F005���Զ��壬�Զ����û����ű����绰��DWORD
#define E2_SMS_OPERATE_PWD_ID			(E2_PRAM_BASE_CUSTOM_ID1+0x06)//����ID 0x0001F006���Զ��壬�Զ����û����Ų������룬DWORD
#define E2_LOW_VOL_THRESHOLD_ID			(E2_PRAM_BASE_CUSTOM_ID1+0x0f)//����ID 0x0001F00F���Զ��壬�Զ���͵�ѹ��ֵ��WORD
#define E2_DEVICE_BACKUP_ID             (E2_PRAM_BASE_CUSTOM_ID+0x2a)//����ID 0xF22a���Զ��壬�ն�ID�ű���
#define	E2_SIM_ICCID_NUMBER_ID          (E2_PRAM_BASE_CUSTOM_ID+0x7F)//����ID 0xF27F, �Զ��壬SIM����ICCID����,BCD[10],ֻ��

/*****************ID��Ӧ�ĳ���***************************/
#define E2_TERMINAL_HEARTBEAT_LEN 		4//����ID 0x0001,�ն��������ͼ��//��λ��//Ĭ��ֵ60
#define E2_MAIN_SERVER_APN_LEN 			25//����ID 0x0010,��������APN
#define E2_MAIN_SERVER_DIAL_NAME_LEN 		25//����ID 0x0011,������������ͨ�Ų����û���
#define E2_MAIN_SERVER_DIAL_PASSWORD_LEN 	25//����ID 0x0012,������������ͨ�Ų�������
#define E2_MAIN_SERVER_IP_LEN 			25//����ID 0x0013,����������ַ//IP������
#define E2_BACKUP_SERVER_APN_LEN 		25//����ID 0x0014,���ݷ�����APN//����ͨ�Ų��ŷ��ʵ�
#define E2_BACKUP_SERVER_DIAL_NAME_LEN 		25//����ID 0x0015,���ݷ���������ͨ�Ų����û���
#define E2_BACKUP_SERVER_DIAL_PASSWORD_LEN 	25//����ID 0x0016,���ݷ���������ͨ�Ų�������
#define E2_BACKUP_SERVER_IP_LEN 		25//����ID 0x0017,���ݷ�������ַ//IP������
#define E2_MAIN_SERVER_TCP_PORT_LEN 		4//����ID 0x0018,��������TCP�˿�
#define E2_SLEEP_REPORT_TIME_LEN 		4//����ID 0x0027,����ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ240
#define E2_EMERGENCY_REPORT_TIME_LEN		4//����ID0x0028����������ʱ�㱨ʱ��������λΪ�룬Ĭ��ֵΪ10
#define	E2_ACCON_REPORT_TIME_LEN		4//����ID 0x0029,ȱʡʱ��㱨���,��λΪ��,Ĭ��Ϊ30
#define E2_CORNER_REPORT_LEN 			4//����ID 0x0030,�յ㲹���Ƕ�,��λ��/��//Ĭ��ֵΪ30
#define E2_MONITOR_SERVER_SMS_PHONE_LEN		20	//����ID0x0043�����ƽ̨SMS�绰����
#define E2_SMS_TEXT_ALARM_PHONE_LEN     	20  //����ID0x0044��SMS�ı������绰����
#define E2_ALARM_MASK_LEN 			4//����ID 0x0050,����������//bit9,11,12Ĭ������,LCD������,TTS����,IC��ģ�����Ĭ��Ϊ����
#define E2_MAX_SPEED_LEN 			4//����ID 0x0055,����ٶ�,��λkm/h,DWORD,Ĭ��ֵ130,��ͨ����Ϊ100
#define E2_OVER_SPEED_KEEP_TIME_LEN 		4//����ID 0x0056,���ٳ���ʱ��,��λΪ��,DWORD,Ĭ��Ϊ10��,��ͨ����Ϊ�����ϱ�
#define	E2_SET_CRASH_ALARM_LEN			2//����ID 0x005D,��ײ������������,��ײ���ٶ�Ĭ������Ϊ4g
#define	E2_SET_ROLLOVER_ALARM_LEN	        2//����ID 0x005E,�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ45 ��
#define E2_CAR_TOTAL_MILE_LEN			4	//����ID0x0080��������̱������0.1km
#define E2_CAR_PROVINCE_LEN 			2//����ID 0x0081,�������ڵ�ʡ��ID
#define E2_CAR_CITY_LEN 			2//����ID 0x0082,�������ڵ�����ID
#define E2_CAR_PLATE_NUM_LEN 			25//����ID 0x0083,������ͨ�����Ű䷢�Ļ���������
#define E2_CAR_PLATE_COLOR_LEN 			1//������ID 0x0084,����ɫ//����JT/T415-2006��5.4.12,1:��ɫ,2:��ɫ,3:��ɫ,4:��ɫ,9:����
#define	E2_GPS_SET_MODE_LEN			1//����ID 0x0090,GNSS ��λģʽ��Ĭ��Ϊ0x02 ������λ

#define E2_LOGINGPRS_PASSWORD_ID_LEN		25//����ID 0xF200,��Ȩ��
#define E2_MANUFACTURE_ID_LEN			5//����ID 0xF205���Զ��壬������ID��5�ֽ�	
#define E2_DEVICE_ID_LEN			7//����ID 0xF206���Զ��壬�����ն�ID��7�ֽ�
#define E2_DEVICE_PHONE_ID_LEN			6//����ID 0xF207���Զ��壬�ն��ֻ��ţ�6�ֽڣ�
#define	E2_CAR_VIN_ID_LEN				17//����ID 0xF208����ʶ����(VIN)��17�ֽڣ�ASCII��
#define	E2_FIRMWARE_VERSION_ID_LEN		5//����ID 0xF20C���Զ��壬�̼��汾��
#define E2_DEVICE_BACKUP_ID_LEN			7//����ID 0xF22a���Զ��壬�����ն�ID��7�ֽ�
#define E2_DEFINE_ALARM_MASK_WORD_ID_LEN	4//����ID 0xF243���Զ��壬�Զ��屨�������֣�DWORD
#define E2_DEFINE_HARD_VER_ID_LEN			10//����ID0x0001F0004���Զ��壬�Զ���Ӳ���汾�ţ�DWORD
#define E2_SMS_ALARM_PHONE_NUM_ID_LEN		20	//����ID 0x0001F005���Զ��壬�Զ����û����ű����绰�
#define E2_SMS_OPERATE_PWD_ID_LEN     		6  //����ID 0x0001F006���Զ��壬�Զ����û����Ų�������
#define E2_LOW_VOL_THRESHOLD_ID_LEN			2//����ID 0x0001F00F���Զ��壬�Զ���͵�ѹ��ֵ��WORD

/////////�Զ��������IDΪ����ID��1,��������һ���˹��޸�/////////////////////////////////////
#define	E2_SELF_DEFINE_ID_MAX               (E2_DEFINE_ALARM_MASK_WORD_ID+1)//�Զ��������ID,ע�⣺ÿ����������ʱ��Ҫ�����Ĳ���ID������






#define FRAM_BASE_ADDR               0//ǰ��һ���ֽڵ���������
#define EEPROM_PRAM_BASE_ADDR           100//ǰ��һ���ֽڵ���������
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//***************************************����������ļ�λ��*******************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
#define E2_PRAM_START_ADDR                  EEPROM_PRAM_BASE_ADDR
#define E2_PRAM_END_ADDR                    E2_PRAM_START_ADDR+E2_SMS_OPERATE_PWD_ID_ADDR+E2_SMS_OPERATE_PWD_ID_LEN+4
//��������ʼ��ַ

#define E2_TERMINAL_HEARTBEAT_ADDR          EEPROM_PRAM_BASE_ADDR//4�ֽ�		
//����ID0x0001���ն��������ͼ������λ�룬Ĭ��ֵ60

#define E2_MAIN_SERVER_APN_ADDR             (E2_TERMINAL_HEARTBEAT_ADDR+E2_TERMINAL_HEARTBEAT_LEN+1)//25�ֽ�		
//����ID0x0010����������APN

#define E2_MAIN_SERVER_DIAL_NAME_ADDR       (E2_MAIN_SERVER_APN_ADDR+E2_MAIN_SERVER_APN_LEN+2)		
//����ID0x0011��������������ͨ�Ų����û���

#define E2_MAIN_SERVER_DIAL_PASSWORD_ADDR   (E2_MAIN_SERVER_DIAL_NAME_ADDR+E2_MAIN_SERVER_DIAL_NAME_LEN+2)			
//����ID0x0012��������������ͨ�Ų�������

#define E2_MAIN_SERVER_IP_ADDR              (E2_MAIN_SERVER_DIAL_PASSWORD_ADDR+E2_MAIN_SERVER_DIAL_PASSWORD_LEN+2)	
//����ID0x0013������������ַ��IP������

#define E2_BACKUP_SERVER_APN_ADDR		(E2_MAIN_SERVER_IP_ADDR+E2_MAIN_SERVER_IP_LEN+2)				
//����ID0x0014�����ݷ�����APN������ͨ�Ų��ŷ��ʵ�

#define E2_BACKUP_SERVER_DIAL_NAME_ADDR		(E2_BACKUP_SERVER_APN_ADDR+E2_BACKUP_SERVER_APN_LEN+2)	
//����ID0x0015�����ݷ���������ͨ�Ų����û���

#define E2_BACKUP_SERVER_DIAL_PASSWORD_ADDR	(E2_BACKUP_SERVER_DIAL_NAME_ADDR+E2_BACKUP_SERVER_DIAL_NAME_LEN+2)		
//����ID0x0016�����ݷ���������ͨ�Ų�������

#define E2_BACKUP_SERVER_IP_ADDR		(E2_BACKUP_SERVER_DIAL_PASSWORD_ADDR+E2_BACKUP_SERVER_DIAL_PASSWORD_LEN+2)			
//����ID0x0017�����ݷ�������ַ��IP������

#define E2_MAIN_SERVER_TCP_PORT_ADDR        (E2_BACKUP_SERVER_IP_ADDR+E2_BACKUP_SERVER_IP_LEN+2)			
//����ID0x0018����������TCP�˿�

#define E2_SLEEP_REPORT_TIME_ADDR           (E2_MAIN_SERVER_TCP_PORT_ADDR+E2_MAIN_SERVER_TCP_PORT_LEN+2)		
//����ID0x0027������ʱ�㱨ʱ��������λΪ�룬Ĭ��ֵΪ60

#define E2_EMERGENCY_REPORT_TIME_ADR  (E2_SLEEP_REPORT_TIME_ADDR+E2_SLEEP_REPORT_TIME_LEN+1)
//����D0x0028,��������ʱ�㱨ʱ����//��λΪ��//Ĭ��ֵΪ10

#define	E2_ACCON_REPORT_TIME_ADDR           (E2_EMERGENCY_REPORT_TIME_ADR+E2_EMERGENCY_REPORT_TIME_LEN+1)		
//E2_PRAM_BASE_CUSTOM_ID+0x0029ȱʡʱ��㱨���,��λΪ��

#define E2_CORNER_REPORT_ADDR		(E2_ACCON_REPORT_TIME_ADDR+E2_ACCON_REPORT_TIME_LEN+1)	
//����ID0x0030���յ㲹���Ƕ�,��λ��/�룬Ĭ��ֵΪ15

#define E2_MONITOR_SERVER_SMS_PHONE_ADDR    (E2_CORNER_REPORT_ADDR+E2_CORNER_REPORT_LEN+1) 
//����ID0x0043�����ƽ̨SMS�绰����

#define E2_SMS_TEXT_ALARM_PHONE_ADDR		(E2_MONITOR_SERVER_SMS_PHONE_ADDR+E2_MONITOR_SERVER_SMS_PHONE_LEN+2)	
//����ID0x0044��SMS�ı������绰����

#define E2_ALARM_MASK_ADDR					(E2_SMS_TEXT_ALARM_PHONE_ADDR+E2_SMS_TEXT_ALARM_PHONE_LEN+2)
//����ID0x0050�����������֣�Ĭ��Ϊȫ��

#define E2_MAX_SPEED_ADDR			(E2_ALARM_MASK_ADDR+E2_ALARM_MASK_LEN+1)				
//����ID0x0055������ٶ�

#define E2_OVER_SPEED_KEEP_TIME_ADDR		(E2_MAX_SPEED_ADDR+E2_MAX_SPEED_LEN+1)			
//����ID0x0056�����ٳ���ʱ�䣬��λΪ�룬Ĭ��Ϊ10��

#define E2_SET_CRASH_ALARM_ADDR		                (E2_OVER_SPEED_KEEP_TIME_ADDR+E2_OVER_SPEED_KEEP_TIME_LEN+1)
//����ID0x005D//��ײ������������ 

#define E2_SET_ROLLOVER_ALARM_ADDR		        (E2_SET_CRASH_ALARM_ADDR+E2_SET_CRASH_ALARM_LEN+1)
//����ID0x005E//�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ30 ��

#define E2_CAR_TOTAL_MILE_ADDR			(E2_SET_ROLLOVER_ALARM_ADDR+E2_SET_ROLLOVER_ALARM_LEN+1)		
//����ID0x0080��������̱������0.1km

#define E2_CAR_PROVINCE_ID_ADDR			(E2_CAR_TOTAL_MILE_ADDR+E2_CAR_TOTAL_MILE_LEN+1)		
//����ID0x0081���������ڵ�ʡ��ID

#define E2_CAR_CITY_ID_ADDR			(E2_CAR_PROVINCE_ID_ADDR+E2_CAR_PROVINCE_LEN+1)			
//����ID0x0082���������ڵ�����ID

#define E2_CAR_PLATE_NUM_ADDR			(E2_CAR_CITY_ID_ADDR+E2_CAR_CITY_LEN+1)			
//����ID0x0083��������ͨ�����Ű䷢�Ļ���������

#define E2_CAR_PLATE_COLOR_ADDR			(E2_CAR_PLATE_NUM_ADDR+E2_CAR_PLATE_NUM_LEN+2)	
//����ID0x0084��������ɫ������JT/T415-2006��5.4.12

#define E2_GPS_SET_MODE_ADDR		                (E2_CAR_PLATE_COLOR_ADDR+E2_CAR_PLATE_COLOR_LEN+1)
//����ID0x0090//GNSS ��λģʽ��Ĭ��ΪE2_PRAM_BASE_CUSTOM_ID+0x ������λ


//////////////////////////////////////////////////////////////
//*************************�Զ������ID��ַ************************
#define E2_LOGINGPRS_PASSWORD_ID_ADDR			(E2_GPS_SET_MODE_ADDR+E2_GPS_SET_MODE_LEN+1)
//����IDE2_PRAM_BASE_CUSTOM_ID+0x00���Զ��壬�����ն�ID��7�ֽ�

#define E2_MANUFACTURE_ID_ADDR		(E2_LOGINGPRS_PASSWORD_ID_ADDR+E2_LOGINGPRS_PASSWORD_ID_LEN+2)
//����IDE2_PRAM_BASE_CUSTOM_ID+0x05��������ID��5�ֽ�	

#define E2_DEVICE_ID_ADDR			(E2_MANUFACTURE_ID_ADDR+E2_MANUFACTURE_ID_LEN+2)
//����IDE2_PRAM_BASE_CUSTOM_ID+0x06���Զ��壬�����ն�ID��7�ֽ�

#define E2_DEVICE_PHONE_ID_ADDR			(E2_DEVICE_ID_ADDR+E2_DEVICE_ID_LEN+2)
//����IDE2_PRAM_BASE_CUSTOM_ID+0x07���Զ��壬�ն��ֻ��ţ�6�ֽ�

#define E2_CAR_VIN_ID_ADDR			(E2_DEVICE_PHONE_ID_ADDR+E2_DEVICE_PHONE_ID_LEN+2)
//����ID 0xF208����ʶ����(VIN)��17�ֽڣ�ASCII��

#define	E2_FIRMWARE_VERSION_ID_ADR	(E2_CAR_VIN_ID_ADDR+E2_CAR_VIN_ID_LEN+2)
//����IDE2_PRAM_BASE_CUSTOM_ID 0x0C���Զ��壬�̼��汾��

#define E2_DEVICE_BACKUP_ID_ADDR			(E2_FIRMWARE_VERSION_ID_ADR+E2_FIRMWARE_VERSION_ID_LEN+2)
//����IDE2_PRAM_BASE_CUSTOM_ID+0x2a���Զ��壬�����ն�ID��7�ֽ�

#define	E2_DEFINE_ALARM_MASK_WORD_ID_ADDR		(E2_DEVICE_BACKUP_ID_ADDR+E2_DEVICE_BACKUP_ID_LEN+2)
//����IDE2_PRAM_BASE_CUSTOM_ID+0x43���Զ��壬����ʶ����

//32����ID0x0001F0004���Զ��壬�Զ���Ӳ���汾�ţ�DWORD
#define	E2_DEFINE_HARD_VER_ID_ADDR 		(E2_DEFINE_ALARM_MASK_WORD_ID_ADDR+E2_DEFINE_ALARM_MASK_WORD_ID_LEN+2)	

//����ID 0x0001F005���Զ��壬�Զ����û����ű����绰
#define	E2_SMS_ALARM_PHONE_NUM_ID_ADDR 		(E2_DEFINE_HARD_VER_ID_ADDR+E2_DEFINE_HARD_VER_ID_LEN+2)	

//����ID 0x0001F006���Զ��壬�Զ����û����Ų�������
#define E2_SMS_OPERATE_PWD_ID_ADDR     		(E2_SMS_ALARM_PHONE_NUM_ID_ADDR+E2_SMS_ALARM_PHONE_NUM_ID_LEN+2)  

//����ID 0x0001F00F���Զ��壬�Զ���͵�ѹ��ֵ��WORD
#define E2_LOW_VOL_THRESHOLD_ID_ADDR		(E2_SMS_OPERATE_PWD_ID_ADDR+E2_SMS_OPERATE_PWD_ID_LEN+2)

//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//***************************************�Զ�����������********************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
#define FRAM_OIL_CTRL_ADDR                      FRAM_BASE_ADDR //����·
#define FRAM_OIL_CTRL_ADDR_LEN                  1
///////////////////////////////////

///////////////////////////////
//�������񴥷���־������1��ʾ������Զ������������2��ʾ�����ɹ���һ�ֽ�
//����3��ʾ����ʧ�ܣ�����4��ʾ�ѷ��͹�0x0108�����ˣ�ÿ���ϵ��Ȩ��ɺ���Ҫ��ȡ��ֵ��
#define FRAM_FIRMWARE_UPDATA_FLAG_ADDR          2//�������񴥷���־������1��ʾ������Զ������������2��ʾ�����ɹ���                                                    //����3��ʾ����ʧ�ܣ�����4��ʾ�ѷ��͹�0x0108�����ˣ�ÿ���ϵ��Ȩ��ɺ���Ҫ��ȡ��ֵ��
#define FRAM_FIRMWARE_UPDATA_FLAG_LEN           1

/////////////////////////////////////
#define FRAM_FIRMWARE_UPDATA_TYPE_ADDR          4//Զ����������,0Ϊ�ն�,9Ϊ���ؿ�����,Ŀǰֻ֧��������
#define FRAM_FIRMWARE_UPDATA_TYPE_LEN           1

#define FRAM_LAST_LOCATION_ADDR                 6	//��һ����Ч��λ��
#define FRAM_LAST_LOCATION_LEN                  11	//��һ����Ч��λ��

#define FRAM_MILEAGE_ADDR                       18	//����̴洢��ַ,��У���ֽ�,
#define FRAM_MILEAGE_LEN                        4	//����̴洢����,��У���ֽ�

#define FRAM_EMERGENCY_FLAG_ADDR				23	//����������һ��״̬
#define FRAM_EMERGENCY_FLAG_LEN					1

#define FRAM_ROLLOVER_BASS_ADDR					25	//�෭��׼�Ƕ�
#define FRAM_ROLLOVER_BASS_LEN					4

#define FRAM_SLEEP_FLAG_ADDR					30	//���߽����־
#define FRAM_SLEEP_FLAG_LEN						1

#define FRAM_ACC_LINE_FLAG_ADDR					32	//�Ƿ���ACC ��
#define FRAM_ACC_LINE_FLAG_LEN					1

#define FRAM_TEMP_TRACK_NUM_ADDR            	34 //��ʱ����ʣ�����
#define FRAM_TEMP_TRACK_NUM_LEN             	4

#define FRAM_TEMP_TRACK_SPACE_ADDR              38   //��ʱ���ټ��
#define FRAM_TEMP_TRACK_SPACE_LEN               2  

#define FRAM_TEMP_TRACK_VERIFY_ADDR              40   //crc32У��
#define FRAM_TEMP_TRACK_VERIFY_LEN               4  

#define FRAM_FCT_SELF_CHECK_ADDR              	44   //FCT�Լ��־
#define FRAM_FCT_SELF_CHECK_LEN               	1  

#define FRAM_RTC_WAKEUP_ADDR              		45   //RTC�����ϱ�ʱ��ƫ����
#define FRAM_RTC_WAKEUP_LEN               		4  

//////////////////////










//-------------------------------------------------------------------------------------------------
//------------------------------------��������-----------------------------------------------------
//-------------------------------------------------------------------------------------------------
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
unsigned char EepromPram_ConvertParamIDToIndex(unsigned int ParamID);
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
u16 EepromPram_WritePramEx(unsigned short index, u8 *pBuffer, u8 BufferLen);
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
u16 EepromPram_WritePram(u32 PramID, u8 *pBuffer, u8 BufferLen);
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
u16 EepromPram_ReadPramEx(unsigned short index, u8 *pBuffer);

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
void EepromPram_UpdateVariable(u32 PramID);
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
void EepromPram_DefaultSet(void);
/*************************************************************OK
** ��������: EepromPram_GetLenEepromPramID
** ��������: 
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: ��
** ����ģ��:
*************************************************************/
unsigned short EepromPram_GetLenEepromPramID(void);

unsigned char FRAM_BufferWrite(unsigned short WriteAddr,unsigned char *pBuffer,unsigned short  NumBytesToWrite);


unsigned char FRAM_BufferRead(unsigned char *pBuffer,unsigned short NumBytesToRead,unsigned long ReadAddr);


#endif
