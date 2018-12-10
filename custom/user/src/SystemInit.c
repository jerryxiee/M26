#include "include.h"

/************************************************************
** ��������: System_ClearAllUserRecord
** ��������: 
** ��ڲ���:
** ���ڲ���:
************************************************************/ 
void System_ClearAllUserRecord(void)
{
	/*
    unsigned char buffer[30];    
    Area_EraseAllArea();//������Ρ�Բ������
    Polygon_ClearArea();//������������
    ClrRoute();                   //�����·
    TiredDrive_TiredDriverInfoInit();//��ʱ��ʻ��ؼ�ʻԱ��Ϣ��ʼ��   
    Report_EraseBlindArea();//����ä������   
    History_EraseTrackArea();//������ʷ�켣����    
    Public_ConvertLongToBuffer(0,buffer);
    EepromPram_WritePram(E2_CAR_TOTAL_MILE_ID, buffer,4);
    Temp_SaveDefaultParameter();
    EepromPram_UpdateVariable(E2_CAR_TOTAL_MILE_ID);
    FRAM_BufferWrite(FRAM_EMERGENCY_FLAG_ADDR, buffer, FRAM_EMERGENCY_FLAG_LEN);//�������洢��־
    */
}
/*********************************************************************
//��������	:EepromPram_DefaultServerParameter
//����		:
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void EepromPram_DefaultServerParameter(void)
{
    unsigned char Buffer[30];
    unsigned char BufferLen;
    strcpy((char *)Buffer,"momo.e-eye.cn");
    BufferLen=strlen((char *)Buffer);
    //////////////
    if(0 == EepromPram_WritePram(E2_MAIN_SERVER_IP_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_MAIN_SERVER_IP_ID, Buffer, BufferLen);
    }
    //////////////////////
    if(0 == EepromPram_WritePram(E2_BACKUP_SERVER_IP_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_BACKUP_SERVER_IP_ID, Buffer, BufferLen);
    }
    //0x0010       ��������APN
    strcpy((char *)Buffer,"cmnet");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_MAIN_SERVER_APN_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_MAIN_SERVER_APN_ID, Buffer, BufferLen);
    }   
    //0x0014 ���ݷ�����APN
    strcpy((char *)Buffer,"cmnet");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_BACKUP_SERVER_APN_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_BACKUP_SERVER_APN_ID, Buffer, BufferLen);
    }
    //��������TCP�˿�,Ĭ��Ϊ11111
    Public_ConvertLongToBuffer(11111, Buffer);
    if(0 == EepromPram_WritePram(E2_MAIN_SERVER_TCP_PORT_ID, Buffer, E2_MAIN_SERVER_TCP_PORT_LEN))
    {
        EepromPram_WritePram(E2_MAIN_SERVER_TCP_PORT_ID, Buffer, E2_MAIN_SERVER_TCP_PORT_LEN);
    }
    ///////////////
    //if(0 == EepromPram_WritePram(E2_MAIN_SERVER_UDP_PORT_ID, Buffer, E2_MAIN_SERVER_UDP_PORT_LEN))
    //{
    //    EepromPram_WritePram(E2_MAIN_SERVER_UDP_PORT_ID, Buffer, E2_MAIN_SERVER_UDP_PORT_LEN);
    //}
}

/*********************************************************************
//��������	:CheckFirstPowerOn
//����		����Ƿ�Ϊ��һ���ϵ�
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:����ֵΪ1����ʹ�ã�0���ǳ���ʹ��
//��ע		:
*********************************************************************/
u8 CheckFirstPowerOn(void)
{
    s32 handle=-1;
    u32	temp=0;
	s32 ret = 0;
	//Enum_FSStorage storage = Ql_FS_UFS;

	handle = Ql_FS_Open(PRAM_FILE_NAME,QL_FS_READ_WRITE);
	APP_DEBUG("\r\n<-- Creat PRAM_FILE_NAME File %d-->\r\n",handle);
	if(handle<0)//���ܶ�ȡ�������ļ�����Ϊ�ǵ�һ��ʹ��
		{  
            // format 
            //Ql_FS_Format(storage);
			handle = Ql_FS_Open(PRAM_FILE_NAME,QL_FS_CREATE_ALWAYS); //if file does not exist ,creat it
			APP_DEBUG("\r\n<-- Creat PRAM_FILE_NAME File %d-->\r\n",handle);
			//Ql_memset(ShareBuffer,0xff, RADIOPROTOCOL_BUFFER_SIZE);

			//while(temp<E2_PRAM_END_ADDR)
			//	{
		    //      ret = Ql_FS_Seek(handle,0,QL_FS_FILE_END);  //seek end 
          	//	  ret = Ql_FS_Write(handle, ShareBuffer, E2_PRAM_END_ADDR,&temp);
			//	  Ql_FS_Flush(handle); //fflushǿ�ƻ���д���ļ�
				 // temp++;
			//	}

		 Ql_FS_Close(handle);//close the file
			 return 1;
		}
	Ql_FS_Close(handle);//close the file
	return 0;
}

/*********************************************************************
//��������	:PowerOnUpdata
//����		:�ϵ���±���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:���µı����У�IO��GPS��λ�û㱨������ͷ
*********************************************************************/
void PowerOnUpdata(void)
{
	
    //����һ��ʹ�ñ�־
    if(CheckFirstPowerOn()==1)
    {
        EepromPram_DefaultSet();
        EepromPram_DefaultServerParameter();
		//Gps_PowerOnClearPosition();
		//SleepInit();
		//SetAccLineFlag(0);
    }
	CreatBlindFile();//����ä����ȡ�ļ�
    ///////////////
    //MemoryChipSelfCheck();
    ////////////////////////////    
    //����IO����
    Io_UpdataPram();

    //����������ʱ����
    HeartBeat_UpdatePram();

    //�����ֻ�����
    RadioProtocol_UpdateTerminalPhoneNum();    
    //////////////////


}

/*********************************************************************
//��������	:PeripheralInit
//����		:�����ʼ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:��ʼ���������У�GPS��GPRS��FRAM��FLASH��EEPROM������ͷ
*********************************************************************/
void PeripheralInit(void)
{

	//��ʼIIC�ӿ�
	//Mc20_IIC_Init();
	//��ʼ��BM250
	//init_bma250e();
    /////////////////
    //RadioProtocol_RecvQueueInit();
    /////////////////
    //////////////    
    //GPIO��ʼ��
    Io_Init(); 
	Led_Init();
	LowVol_UpdatePram();
	//�򿪵�ع���
	//VBATTN_ON();
	//BATTERYCHARGE_ON();
    /////////////
    //SPI_FLASH_AllUnprotect();    
    //E2prom_InitIO();
    //AD��ʼ��
    //Ad_Init(); 
	//�����12V���绹��24V����
    //Io_PowerOnDetectMainPower();
    /////////////////
    ///Communication_Init();
    //Sleep_Init();
    //////////////////
	init_TaskScheduleTimer();//��������ɨ�趨ʱ��
	
}


