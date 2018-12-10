//********************************ͷ�ļ�************************************

#include "include.h"
#include "report.h"
//********************************��������**********************************
#define SYS_USE_BIG_LITTLE_FLAG   0//ϵͳ���С�˱�־,0ΪС��
static u8 RadioProtocolRxBuffer[RADIOPROTOCOL_BUFFER_SIZE];//�����ջ��������ȫ��������ʱ,æ��־��0;�ڽ���ʱ,æ��־��1
static u8 RadioProtocolTxBuffer[RADIOPROTOCOL_BUFFER_SIZE];//ƽ̨���ݷ��ͻ�����
u8 ShareBuffer[RADIOPROTOCOL_BUFFER_SIZE];

//********************�ⲿģ����õı���*******************
extern u8	TerminalAuthorizationFlag;//�ն˼�Ȩ��־,��Ȩ�ɹ�Ϊ1,ʧ��Ϊ0
extern u16 DelayAckCommand;//��ʱӦ������

static unsigned char TerminalPhoneNumber[6] = {0};//�ն��ֻ���
static unsigned short RadioProtocolRxLen = 0;//RadioProtocolRxBuffer�������ݳ���
static unsigned short MessageSerialNum = 0;//��Ϣ��ˮ��
static unsigned short s_usRecvSerialNum=0;
static unsigned short s_usRecvMessageID=0;
static unsigned char s_ucCurRecvChanel = CHANNEL_DATA_1;	//����2����
unsigned char s_ucRecvChanel = CHANNEL_DATA_1;	//����2����
static unsigned char s_ucRecvTimeCnt = 0;//�����Ƿ���Ҫ�ȴ�
static unsigned char s_ucUpdataResultNoteAckFlag = 0;//�������֪ͨӦ���־,1Ϊ�յ�ƽ̨��ͨ��Ӧ����
u16     CarControlAckSerialNum = 0;//��������Ӧ����ˮ��
u8	DelaySendChannel = 0;//��ʱ����ͨ����
static MESSAGE_HEAD	RadioProtocolRxHead;//���յĶ��������ݾ�������õ���֡ͷ���ڴ˽ṹ����


//ע��Ӧ��
/*********************************************************************
//��������	:RadioProtocol_ServerGeneralAck(u8 *pBuffer, u16 BufferLen)
//����		:ƽ̨����������ͨ��Ӧ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8001
*********************************************************************/
ProtocolACK RadioProtocol_ServerGeneralAck(u8 *pBuffer, u16 BufferLen)
{
    u16	SerialNum;
    u16	ID;
    u8	Result;
    u8	*p;
    u8	HighByte;
    u8	LowByte;
    u8      Buffer[3] = {0};
    static u8	Count = 0;
    p = pBuffer;
	

    //�жϳ����Ƿ���ȷ
    if((5 != BufferLen))
    {
        return ACK_ERROR;
    }
    /////////////////////////////
    //��ȡӦ����ˮ��
    HighByte = *p++;
    LowByte = *p++;
    SerialNum = (HighByte << 8)|LowByte;
    if(0 == SerialNum)
    {
        //Ϊ���벻����
    }

    //��ȡӦ��ID
    HighByte = *p++;
    LowByte = *p++;
    ID = (HighByte << 8)|LowByte;

    //�ж��Ƿ�Ϊä������Ӧ��
    if(0x0704 == ID)
    {
        Blind_Link1ReportAck();
    }
    else
    if(0x200==ID)
    {
       Report_CmdAck();
    }
    //�ж��Ƿ�Ϊ��ȨӦ��
    else if(0x0102 == ID)
    {
        //��ȡ���
        Result = *p;
        if(0 == Result)
        {
            //�ɹ�
            Count++;
			APP_DEBUG("<-- ��Ȩ�ɹ�-->\r\n");
            //SetTerminalAuthorizationFlag(CHANNEL_DATA_1);
            //��ʾ��Ȩ�ɹ�
            //Blind_StartLink1Report();//����ä������
            //Report_UploadPositionInfo();
			
			//Report_SetReportStatus(ReportStatus_Init);
            //SetEvTask(MSG_Report_EvTask);//��Ȩ�ɹ��������ϱ�1��λ����Ϣ,dxl,2014.2.14
        }
        else
        {
			APP_DEBUG("<-- ��Ȩʧ��-->\r\n");
            //���־
            ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);
            ClearTerminalAuthorizationCode(CHANNEL_DATA_1);
        }
    }
    else if(0x0003 == ID)//�ն�ע��
    {
        //��ȡ���
        TerminalAuthorizationFlag = 0;
    }
    else if(0x0108 == ID)//�ն��������֪ͨ
    {
		s_ucUpdataResultNoteAckFlag=1;
    }
    else
    {
        if(Count >= 1)
        {
            //ȫ����ʾ
            //SetMainFullDisplayFlag();
            //������������ʾ
            //ClrDisplayControlFlag();
        }
        Count = 0;
        //��ȡ���
        Result = *p;

        if(0 == Result)
        {
            //�ɹ�
        }
        else if(1 == Result)
        {
            //ʧ��
        }
        else if(2 == Result)
        {
            //��Ϣ����
        }
        else if(3 == Result)
        {
            //��֧��
        }
        else if(4 == Result)//��������ȷ��
        {
            //����������˹�ȷ�ϵı���
            //�����������
            Io_WriteAlarmBit(ALARM_BIT_EMERGENCY, RESET);
            //���Σ��Ԥ��
            Io_WriteAlarmBit(ALARM_BIT_DANGER_PRE_ALARM, RESET);
            //����������򱨾�
            Io_WriteAlarmBit(ALARM_BIT_IN_OUT_AREA, RESET);
            //���������·����
            Io_WriteAlarmBit(ALARM_BIT_IN_OUT_ROAD, RESET);
            //�����·��ʻ�������㱨��
            Io_WriteAlarmBit(ALARM_BIT_LOAD_OVER_DRIVE, RESET);

            //����Ƿ����
            Io_WriteAlarmBit(ALARM_BIT_LAWLESS_ACC, RESET);
            //����Ƿ�λ��
            Io_WriteAlarmBit(ALARM_BIT_LAWLESS_MOVEMENT, RESET);
            //����Ƿ�������
            Io_WriteAlarmBit(ALARM_BIT_LAWLESS_OPEN_DOOR, RESET);

        }
    }
    
    if(0 == Buffer[0])
    {
    
    }
    return ACK_OK;
}

/*********************************************************************
//��������	:RadioProtocol_TerminalRegisterAck(u8 *pBuffer, u16 BufferLen)
//����		:�ն�ע��Ӧ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8100
*********************************************************************/
ProtocolACK RadioProtocol_TerminalRegisterAck(u8 *pBuffer, u16 BufferLen)
{
	u8	ucTmp[30],len;

	if((BufferLen < 3)||(BufferLen > LOGIN_PASSWORD_LEN+1))return ACK_ERROR;
	//�жϽ��
	if(0 != pBuffer[2])
    {
    APP_DEBUG("<-- ע��ʧ��-->\r\n");
        //RadioProtocol_TerminalLogout();
		RadioProtocol_SendData(0x0003,NULL,0,CHANNEL_DATA_1);//����		:�ն�ע��//��ע		:ָ��ID��0x0003
        return ACK_ERROR;
    }
    //��ȡ��Ȩ��
    APP_DEBUG("<-- ע��ɹ�����Ȩ��:%s��Ȩ�볤��%d-->\r\n",&pBuffer[3],BufferLen-3);
    EepromPram_WritePram(E2_LOGINGPRS_PASSWORD_ID, &pBuffer[3], BufferLen-3);
	len=EepromPram_ReadPram(E2_LOGINGPRS_PASSWORD_ID, ucTmp);
	ucTmp[len]='\0';
    APP_DEBUG("<-- ע��ɹ�����Ȩ��:%s��Ȩ�볤��%d-->\r\n",ucTmp,len);
	SetLoginAuthorizationState(LOGIN_SEND_AUTHORIZATION_CMD);	
	return ACK_OK;
}

/*********************************************************************
//��������	:RadioProtocol_PramSet(u8 *pBuffer, u16 BufferLen)
//����		:�����ն˲���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8103
*********************************************************************/
ProtocolACK RadioProtocol_PramSet(u8 *pBuffer, u16 BufferLen)
{

	s16	TotalPramCount;//��������
	//s16	PacketPramCount;//����������
	u8	*p;
	s16	length;
	u8 	AckResult = 0;
	
	p = pBuffer;
	
	//���Ȳ�������1024
	//��ȡ����
	length = BufferLen;

	//��ȡ��������
	TotalPramCount = *p++;
	length--;
	
	if(0 == TotalPramCount)
	{
		return ACK_ERROR;
	}
	RadioProtocol_WritePram(p, length);
	//Ӧ����
	AckResult = 0;

	return RadioProtocol_TerminalGeneralAck(AckResult);
	
}

/*********************************************************************
//��������	:RadioProtocol_PramSearch(u8 *pBuffer, u16 BufferLen)
//����		:��ѯ�ն˲���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8104
*********************************************************************/
ProtocolACK RadioProtocol_PramSearch(u8 *pBuffer, u16 BufferLen)
{

    u8 RadioShareBuffer[RADIOPROTOCOL_BUFFER_SIZE];
    u8	*p = NULL;
    u8	*p1 = NULL;
    u8	AckPramCount = 0;//����������
    s16	length = 0;
    
    //���Ȳ�������1024
    //��ȡӦ����ˮ��
    //AckSerialNum = RadioProtocolRxHead.SerialNum;
    //Ӧ����ˮ��д�뻺��
    p = RadioShareBuffer;
    *p++ = (s_usRecvSerialNum & 0xff00) >> 8;
    *p++ = s_usRecvSerialNum & 0xff;
    *p++;//Ԥ��1���ֽڴ洢Ӧ���������
    if(0 == BufferLen)//��ȡȫ���Ĳ���
    {
        AckPramCount=E2ParamApp_DisposeReadAllParam(p,&length);
    }
    else//��ȡָ���Ĳ���
    {
        p1 = pBuffer;//��1��Ϊ��Ҫ��ѯ�Ĳ����ܸ���
        p1++;//�Թ�
        AckPramCount = RadioProtocol_ReadPram(p, &length, p1, BufferLen-1);
    }
    //д�����������
    *(RadioShareBuffer+2) = AckPramCount;
    //���ȼ���Ӧ����ˮ��,Ӧ���������
    length += 3;
    //��Ϣͷ
    return RadioProtocol_SendData(0x0104,RadioShareBuffer,length,s_ucRecvChanel);
   
}
/*********************************************************************
//��������	:RadioProtocol_TerminalControl(u8 *pBuffer, u16 BufferLen)
//����		:�ն˿���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8105
*********************************************************************/
ProtocolACK RadioProtocol_TerminalControl(u8 *pBuffer, u16 BufferLen)
{
    u8 Command;
    u8	*p;
    s16	length;
    u8	AckResult;
    u8	ControlLinkWord;
    u8	Buffer[30];
    u8	ErrorFlag = 0;
	u32	i,j;
    u8	Count;
    u16	PramLen;
    ////////////
    AckResult = 1;
    p = pBuffer;
    length = BufferLen;

    //��ȡ������
    Command = *p++;
    length--;

    if(1 == Command)//��������
    {
        //��������Ϊ��URL��ַ;���ŵ�����;�����û���;��������;��ַ;TCP�˿�;
        //UDP�˿�;������ID;Ӳ���汾;�̼��汾;���ӵ�ָ��������ʱ��
        i = 0;
        j = 0;
        Count = 0;
        while(length > 2)
		{
					if(';' == *(p+i))//�ҵ��˷ָ��
					{	
						Count++;
						PramLen = i-j;
						switch(Count)
						{
		
							case 1://URL��ַ
							{
								if(Net_Ftp_Parameter(1,p+j,PramLen))
								{
									ErrorFlag =1;
								}
								break;
							}
							case 2://���ŵ�����
							{
								if(Net_Ftp_Parameter(2,p+j+1,PramLen-1))
								  {
									  ErrorFlag =1;
								  }
							   break;
							}
							case 3://�����û���
							{
								if(Net_Ftp_Parameter(3,p+j+1,PramLen-1))
								  {
									  ErrorFlag =1;
								  }
								break;
							}
							case 4://��������
							{
								if(Net_Ftp_Parameter(4,p+j+1,PramLen-1))
								  {
									  ErrorFlag =1;
								  } 					   
								break;
							}
							case 5://��ַ
							{
								if(Net_Ftp_Parameter(5,p+j+1,PramLen-1))//liamtsen
								{
									ErrorFlag =1;
								}
								break;
							}
							case 6://TCP�˿�
							{
								if(Net_Ftp_Parameter(6,p+j+1,PramLen-1))//liamtsen
								{
									ErrorFlag =1;
								}
								break;
							}
							case 7://UDP�˿�
							{
								//EepromPram_WritePram(E2_UPDATA_UDP_PORT_ID, p+j+1, PramLen-1); //dxl,2017.2.23,������
								//EepromPram_ReadPram(E2_UPDATA_UDP_PORT_ID, Buffer); 
		//						  if(Public_CheckArrayValIsEqual(p+j+1, Buffer,PramLen-1))
		//						  {
		//							  ErrorFlag =1;
		//						  }
								break;
							}
							case 8://������ID
							{
								//EepromPram_WritePram(E2_UPDATA_MANUFACTURE_ID, p+j+1, PramLen-1); //dxl,2017.2.23,������
								//EepromPram_ReadPram(E2_UPDATA_MANUFACTURE_ID, Buffer); 
		//						  if(Public_CheckArrayValIsEqual(p+j+1, Buffer,PramLen-1))
		//						  {
		//							  ErrorFlag =1;
		//						  }
								break;
							}
							case 9://Ӳ���汾
							{
								//EepromPram_WritePram(E2_UPDATA_HARDWARE_VERSION_ID, p+j+1, PramLen-1); //dxl,2017.2.23,������
								//EepromPram_ReadPram(E2_UPDATA_HARDWARE_VERSION_ID, Buffer);
		//						  if(Public_CheckArrayValIsEqual(p+j+1, Buffer,PramLen-1))
		//						  {
		//							  ErrorFlag =1;
		//						  }
								break;
							}
							case 10://�̼��汾
							{
								//EepromPram_WritePram(E2_UPDATA_FIRMWARE_VERSION_ID, p+j+1, PramLen-1); //dxl,2017.2.23,������
								//EepromPram_ReadPram(E2_UPDATA_FIRMWARE_VERSION_ID, Buffer);
		//						  if(Public_CheckArrayValIsEqual(p+j+1, Buffer,PramLen-1))
		//						  {
		//							  ErrorFlag =1;
		//						  }
								break;
							}
							default :
								break;
						}
						j = i;
					}
					i++;
					length--;	
					if(10 == Count)
					{
						break;
					}
				}
		        if(0 == ErrorFlag)//ֻ������ȫ����ȷ�˲ŻῪ��FTPԶ������,��Ӧ��
		        {
		            if(Net_Other_FtpStart())
		            {
		                AckResult = 0;
		                Buffer[0] = 3;//������־ ʧ�� 3
		                FRAM_BufferWrite(FRAM_FIRMWARE_UPDATA_FLAG_ADDR,Buffer,1);
						RadioProtocol_TerminalGeneralAck(AckResult);    
						Net_First_Close();
						 ftp_downfile_timer();
						 return ACK_OK;
					}
		        }
    }
    else if(2 == Command)//�����ն�����ָ��������
    {
        //��������Ϊ�����ӿ���;���ƽ̨��Ȩ��;���ŵ�����;�����û���;��������;��ַ;TCP�˿�;
        //UDP�˿�;���ӵ�ָ��������ʱ��
        //��ȡ���ӿ�����
        //ControlLinkWord = *p;
        AckResult = 0;
                
    }
	else if(3 == Command)//Ӳ���ػ�
    {
        
        AckResult = 0;
        DelayAckCommand = 12;//�ػ�
        //������ʱ����
        SetTimerTask(TIME_DELAY_TRIG,3*SECOND);
                
    }
	else
    if(4 == Command)//�ն˸�λ
    {
         AckResult = 0;
        //if(1 == Io_ReadStatusBit(STATUS_BIT_ACC))
        {            
            DelayAckCommand = 7;//���ߣ�����
        }
        //else
        //{            
        //    DelayAckCommand = 11;//���ߣ������������        
        //}
        //������ʱ����
        SetTimerTask(TIME_DELAY_TRIG,3*SECOND);   
	}
    else 
    if(5 == Command)//�ָ���������
    {
        AckResult = 0;
        EepromPram_DefaultSet();
        DelayAckCommand = 7;//���ߣ�����
        //������ʱ����
        SetTimerTask(TIME_DELAY_TRIG,3*SECOND);
	}
    
    RadioProtocol_TerminalGeneralAck(AckResult);    
	
    return ACK_OK;

}
/*********************************************************************
//��������	:RadioProtocol_TerminalAttribute(u8 *pBuffer, u16 BufferLen)
//����		:��ѯ�ն�����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8107
*********************************************************************/
/*
ProtocolACK RadioProtocol_TerminalAttribute(u8 *pBuffer, u16 BufferLen)
{
    u8	*p;
    u8	Buffer[150];
    u8	Buffer2[50];
    u8	Buffer3[50];
    u8	PramLen = 0;
    u8	SmsPhone[20];
    u8	channel = 0;
    u16	length = 0;
    u8	tmplen = 0;


    MESSAGE_HEAD	head;

    p = Buffer;
    length = 0;

    //��ȡ�ն����� Bytes:2 
    //Ĭ��Ϊ�����ڿ��ˡ�Σ��Ʒ�����ˣ����ͣ�һ���
    Buffer2[0] = 0;
    Buffer2[1] = 0x07;
  
    memcpy(p,Buffer2,2);
    p += 2;
    length += 2;

    PramLen = EepromPram_ReadPram(E2_MANUFACTURE_ID, Buffer2); 
    if(E2_MANUFACTURE_ID_LEN == PramLen)
    {
        memcpy(p,Buffer2,E2_MANUFACTURE_ID_LEN);
    }
    else
    {
        memcpy(p,"70108",5);
    }
    //////////////////
    p += 5; 
    length += 5;
    //////////////////////
    //��ȡ�ն��ͺ�
    memset(Buffer2,0,20);
    Version_GetProductSeriesName((char *)Buffer2);
    memcpy(p,Buffer2,20);
    p += 20;
    length += 20;
    //��ȡ�ն�ID
    PramLen = EepromPram_ReadPram(E2_DEVICE_ID, Buffer2);
    if(E2_DEVICE_ID_LEN == PramLen)//ȡ�ն�ID����7λ��Ϊ��¼ע����ն�ID
    {
        Public_ConvertBcdToAsc(Buffer3,&Buffer2[3],4);
    }
    else
    {

        memset(Buffer3,0,10);
        strcpy((char *)&Buffer3[1],"3100028");
    }
    ///��ȡ�ն�ID///////////////
    memcpy(p,&Buffer3[1],7);
    p += 7;
    length += 7;
    //�ն�SIM��ICCID BCD[10]  Bytes:10
    MOD_GetIccidBcdValue(Buffer2);
    memcpy(p,Buffer2,10 );
    p += 10 ;
    length += 10 ;

    //�ն�Ӳ���汾���� BYTE  Bytes:1
    tmplen = Version_GetHardwareInformation(Buffer2);
    *p++ = tmplen;
    length++;
    //�ն�Ӳ���汾
    memcpy(p,Buffer2,tmplen);//HUOYUN_DEBUG_OPEN����ƽ̨����ǰ����û�У�Ϊ��������
    p += tmplen;
    length += tmplen;
    //�ն˹̼��汾���� BYTE  Bytes:1
    tmplen = Version_GetFirmwareInformation(Buffer2);
    *p++ = tmplen;
    length++;
    //�ն˹̼��汾�� STRING
    memcpy(p,Buffer2,tmplen);
    p += tmplen ;
    length += tmplen;

    //GNSSģ������
   // if(E2_GPS_ATTRIBUTE_ID_LEN == EepromPram_ReadPram( E2_GPS_ATTRIBUTE_ID , Buffer2  ))
   // {
      //  *p = Buffer2[0];
      //  p++;
      //  length++;  
   // }
    //else
   // {
        *p = 0x03;//û�����ù��Ļ���֧��GPS+����
        p++;
        length++;  
   // }
    //ͨ��ģ������
    *p = 0x01;//GPRSͨѶ
    p++;
    length++; 

    //��ϢID
    head.MessageID = 0x0107;
    head.Attribute = (length & 0x3ff);

    //�ж��Ƿ�Ϊ����Ϣ
    if( CHANNEL_SMS == s_ucRecvChanel)
    {
        channel = CHANNEL_SMS;
    }

    if(SUCCESS == RadioProtocol_SendCmd(head, Buffer, length, channel))
    {
        return ACK_OK;
    } 
    else 
    {
        return ACK_ERROR;

    } 

}
*/
/*********************************************************************
//��������	:RadioProtocol_PositionSearch(u8 *pBuffer, u16 BufferLen)
//����		:λ����Ϣ��ѯ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8201
*********************************************************************/
ProtocolACK RadioProtocol_PositionSearch(u8 *pBuffer, u16 BufferLen)
{
	u16	length;
	u8	Buffer[150];
	u8	PramLen;
	
	if(0 != BufferLen)
	{
		return ACK_ERROR;
	}
	length = 0;
	//���Ӧ����ˮ��
	//AckSerialNum = RadioProtocolRxHead.SerialNum;
    Public_ConvertShortToBuffer(s_usRecvSerialNum, Buffer);
	length += 2;
	//��ȡλ����Ϣ,������Buffer��
	PramLen = Report_GetPositionInfo(Buffer+2);
	length += PramLen;
    
	//����λ�û㱨��Ϣ
	return RadioProtocol_SendData(0x0201,Buffer,length,s_ucRecvChanel);
	//��ϢID	
}

/*********************************************************************
//��������	:RadioProtocol_TempPositionControl(u8 *pBuffer, u16 BufferLen)
//����		:��ʱλ�ø��ٿ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8202
*********************************************************************/
ProtocolACK RadioProtocol_TempPositionControl(u8 *pBuffer, u16 BufferLen)
{

	u8	*p;
	u8	AckResult;
	u16 TimeSpace;//ʱ����
	u32 ValidTime;//��Ч��
	u8	Buffer[5];
	
	

	p = pBuffer;
	
	//if(6 != BufferLen),����������,��Ҫֹͣ���ٿ���ʱ��2�ֽ�,HUOYUN_DEBUG_OPEN����ʱ������
	//{
		//AckResult = 2;
		//return RadioProtocol_TerminalGeneralAck(AckResult);
	//}
	
	//д����ʱ����ʱ����
	TimeSpace = 0;
	TimeSpace |= *p << 8;
	Buffer[0] = *p;
	p++;
	TimeSpace |= *p;
	Buffer[1] = *p;
	p++;
	/*if(TimeSpace>30)//ʱ����̫�󷵻���Ϣ����
	{
		return RadioProtocol_TerminalGeneralAck(0x2);
	}*/
	FRAM_BufferWrite(FRAM_TEMP_TRACK_SPACE_ADDR, Buffer, FRAM_TEMP_TRACK_SPACE_LEN);
	Buffer[2] = Buffer[0];
	Buffer[3] = Buffer[1];
	Buffer[0] = 0;
	Buffer[1] = 0;
	EepromPram_WritePram(E2_ACCON_REPORT_TIME_ID, Buffer, E2_ACCON_REPORT_TIME_LEN);
	Report_UpdateOnePram(PRAM_TEMP_TRACK_TIME);
	if(6 == BufferLen)
	{
		//д����ʱ������Ч��
		ValidTime = 0;
		ValidTime |= *p++ << 24;
		ValidTime |= *p++ << 16;
		ValidTime |= *p++ << 8;
		ValidTime |= *p;
		ValidTime = ValidTime/TimeSpace;
		Buffer[0] = (ValidTime&0xff000000) >> 24;
		Buffer[1] = (ValidTime&0xff0000) >> 16;
		Buffer[2] = (ValidTime&0xff00) >> 8;
		Buffer[3] = ValidTime&0xff;
		FRAM_BufferWrite(FRAM_TEMP_TRACK_NUM_ADDR, Buffer, FRAM_TEMP_TRACK_NUM_LEN);
		Report_UpdateOnePram(PRAM_TEMP_TRACK_COUNT);

		Report_PramBackup();

		DelayAckCommand = 14;//�ϱ�
        SetTimerTask(TIME_DELAY_TRIG, 3*SECOND);
	}

	AckResult = 0;

	return RadioProtocol_TerminalGeneralAck(AckResult);
}

/*********************************************************************
//��������	:RadioProtocol_AlarmConfirmManual(u8 *pBuffer, u16 BufferLen)
//����		:�˹�ȷ�˱���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:  �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x8203
*********************************************************************/
ProtocolACK RadioProtocol_AlarmConfirmManual(u8 *pBuffer, u16 BufferLen)
{

  u8 *p =NULL;
  u8 AckResult;
  static u16 AlarmSerialNum;
  static u32 AlarmType;
  u8 flag = 0;  
		
  if( BufferLen != 6 )
  { 	
	  AckResult = 1;			  
  }
  else
  {


	  p = pBuffer;

	  /*** ��Ҫ�˹�ȷ�ϵı�����ˮ��, 0��ʾ����������Ϣ	***/
	  AlarmSerialNum = 0;
	  AlarmSerialNum = *p++;
	  AlarmSerialNum <<=8;
	  AlarmSerialNum += *p++;
		
	  /*** �˹�ȷ�ϱ�������  ***/
	  
	   AlarmType = 0;
	   AlarmType += *p++;
		
	   AlarmType <<= 8;
	   AlarmType += *p++;
		 
	   AlarmType <<= 8;    
	   AlarmType += *p++;
		
	   AlarmType <<= 8;
	   AlarmType += *p++;
	   
	   
	   if( AlarmType & ( 1<< 0 ) ) {
		Io_WriteAlarmBit( ALARM_BIT_EMERGENCY , RESET );	//0 1����������,�յ�Ӧ������
			flag = 0;
			FRAM_BufferWrite(FRAM_EMERGENCY_FLAG_ADDR, &flag, FRAM_EMERGENCY_FLAG_LEN);//�������洢��־
	   
	   }
	   
	  if( AlarmType & ( 1<< 3 ) ) {
		  Io_WriteAlarmBit( ALARM_BIT_DANGER_PRE_ALARM , RESET );		 //3	1��Σ��Ԥ��,�յ�Ӧ�������
	   
	   }
	   
	   
	  if( AlarmType & ( 1<< 20 ) ) {
		  Io_WriteAlarmBit( ALARM_BIT_IN_OUT_AREA , RESET );		//20	1����������,�յ�Ӧ�������
	   
	   }  
		
	  if( AlarmType & ( 1<< 21 ) ) {
		  Io_WriteAlarmBit( ALARM_BIT_IN_OUT_ROAD , RESET );		//21	1������·��,�յ�Ӧ������
	   
	   } 
		
		
	  if( AlarmType & ( 1<< 22 ) ) {
		  Io_WriteAlarmBit( ALARM_BIT_LOAD_OVER_DRIVE , RESET );			//22	1��·����ʻʱ�䲻��/����,�յ�Ӧ������
		
	   }
		
		
	  if( AlarmType & ( 1<< 27 ) ) {
		if(1 == Io_ReadAlarmBit(ALARM_BIT_LAWLESS_ACC))
		{
			 // LawlessAccAlarmEnableFlag = 0;
		  Io_WriteAlarmBit( ALARM_BIT_LAWLESS_ACC , RESET );		//27	1�������Ƿ����,�յ�Ӧ�������
			  
		}
		
	   }	
		
		
	  if( AlarmType & ( 1<< 28 ) ) {
		Io_WriteAlarmBit( ALARM_BIT_LAWLESS_MOVEMENT , RESET ); 		//28	1�������Ƿ�λ��,�յ�Ӧ�������
	   
	   }
	   
	   
	   AckResult = 0;		

  }

  return RadioProtocol_TerminalGeneralAck(AckResult);
}

/*********************************************************************
//��������	:RadioProtocol_CarControl(u8 *pBuffer, u16 BufferLen)
//����		:��������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8500
*********************************************************************/
ProtocolACK RadioProtocol_CarControl(u8 *pBuffer, u16 BufferLen) 
{
    u8 ControlFlag;
    u8 AckResult;
    ControlFlag = *pBuffer;
    if(PUBLIC_CHECKBIT(ControlFlag,6))
    {
        if(PUBLIC_CHECKBIT(ControlFlag,7))//����
        {
            //OilControl_OffOil();
        }
        else//ͨ��
        {
            //OilControl_OpenOil();
        }
    }
    else
    if(0x01 == (ControlFlag & 0x01))
    {
        //�������ż�������
        //CtrlLock(1);
    }
    else
    {
        //�������Ž�������
        //CtrlLock(0);
    }
    //���³�������Ӧ����ˮ��
    if(GetTimerTaskTimerState(TIME_DELAY_TRIG) == DISABLE)
    {
        CarControlAckSerialNum = RadioProtocolRxHead.SerialNum;
        DelayAckCommand = 0x0500;
        DelaySendChannel = s_ucRecvChanel;
        if(CHANNEL_SMS == DelaySendChannel)
        {
        }
        //������ʱ����
        SetTimerTask(TIME_DELAY_TRIG, PUBLICSECS(3));//2���Ӧ��,��Ҫ��Ϊ��ȷ������/�������״̬�ܲɼ���
        //ֻ��Ӧ��
        AckResult = 0;
        return RadioProtocol_TerminalGeneralAck(AckResult);
    }
    return ACK_OK;
}
/*********************************************************************
//��������	:RadioProtocol_SetRoundArea(u8 *pBuffer, u16 BufferLen)
//����		:����Բ������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8600
*********************************************************************/
ProtocolACK RadioProtocol_SetRoundArea(u8 *pBuffer, u16 BufferLen)
{
    u8 AckResult = 0;
    //AckResult = Area_SetRoundArea(pBuffer,BufferLen);
    return RadioProtocol_TerminalGeneralAck(AckResult);
}
/*********************************************************************
//��������	:RadioProtocol_DelRoundArea(u8 *pBuffer, u16 BufferLen)
//����		:ɾ��Բ������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8601
*********************************************************************/
ProtocolACK RadioProtocol_DelRoundArea(u8 *pBuffer, u16 BufferLen) //SSSS
{
    u8 AckResult = 0;
    //AckResult = Area_DelRoundArea(pBuffer,BufferLen);
    return RadioProtocol_TerminalGeneralAck(AckResult);
}
/*********************************************************************
//��������	:RadioProtocol_SetRectangleArea(u8 *pBuffer, u16 BufferLen)
//����		:���þ�������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8602
*********************************************************************/
ProtocolACK RadioProtocol_SetRectangleArea(u8 *pBuffer, u16 BufferLen)
{
    u8 AckResult = 0;
   	//AckResult = Area_SetRectArea(pBuffer,BufferLen);
    return RadioProtocol_TerminalGeneralAck(AckResult);
}
/*********************************************************************
//��������	:RadioProtocol_DelRectangleArea(u8 *pBuffer, u16 BufferLen)
//����		:ɾ����������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8603
*********************************************************************/
ProtocolACK RadioProtocol_DelRectangleArea(u8 *pBuffer, u16 BufferLen) //SSSS
{
    u8 AckResult = 0;
    //AckResult = Area_DelRectArea(pBuffer,BufferLen);
    return RadioProtocol_TerminalGeneralAck(AckResult);
}
/*********************************************************************
//��������	:RadioProtocol_SetPolygonArea(u8 *pBuffer, u16 BufferLen)
//����		:���ö��������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8604
*********************************************************************/
ProtocolACK RadioProtocol_SetPolygonArea(u8 *pBuffer, u16 BufferLen)
{
    u8	AckResult = 3;//�������ʱ��֧��
   // AckResult = Polygon_SetArea(pBuffer,BufferLen); 
    return RadioProtocol_TerminalGeneralAck(AckResult);
}
/*********************************************************************
//��������	:RadioProtocol_DelPolygonArea(u8 *pBuffer, u16 BufferLen)
//����		:ɾ�����������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8605
*********************************************************************/
ProtocolACK RadioProtocol_DelPolygonArea(u8 *pBuffer, u16 BufferLen)
{
    u8	AckResult = 3;//�������ʱ��֧��
    //AckResult = Polygon_DeleteArea(pBuffer,BufferLen);
    return RadioProtocol_TerminalGeneralAck(AckResult);
}


/*********************************************************************
//��������	:RadioProtocol_OriginalDataDownTrans(u8 *pBuffer, u16 BufferLen)
//����		:��������͸��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8900
*********************************************************************/
ProtocolACK RadioProtocol_OriginalDataDownTrans(u8 *pBuffer, u16 BufferLen)
{
/*
    u8 AckResult;
    //ֻ��Ӧ��,����ʱ����
    u8 flag;
    flag=*pBuffer;
    //////////////////////////
    #ifdef USE_EIEXPAND_PROTOCOL
    if(EIEXPAND_PROTOCOL_TRAN_TYPE == flag)//������չЭ��͸������
    {
        AckResult=EIExpand_RadioProtocolParse(pBuffer+1,BufferLen-1);
        if(0xff==AckResult)return ACK_OK;
    }
    #endif
    return RadioProtocol_TerminalGeneralAck(AckResult);
    */
        return ACK_OK;
}



//*****************�ڶ����֣���׼���ն��Ϸ�ָ��**********************
/*********************************************************************
//��������	:RadioProtocol_TerminalGeneralAck(u16 SerialNum, u16 MessageID, u8 Result)
//����		:�ն�ͨ��Ӧ��
//����		:Result:Ӧ����,0Ϊ�ɹ���1Ϊʧ�ܣ�2��Ϣ����3��֧��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:�ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x0001
*********************************************************************/
ProtocolACK RadioProtocol_TerminalGeneralAck(u8 AckResult)
{
	u8 Buffer[5];
	//��Ϣ����
	Buffer[0] = (s_usRecvSerialNum & 0xff00) >> 8;
	Buffer[1] = s_usRecvSerialNum;
	Buffer[2] = (s_usRecvMessageID & 0xff00) >> 8;
	Buffer[3] = s_usRecvMessageID;
	Buffer[4] = AckResult;
    return RadioProtocol_SendData(0x0001,Buffer,5,s_ucRecvChanel);
}
const char* c_terminalModel = "VTKG-21N";
/*********************************************************************
//��������	:RadioProtocol_TerminalRegister(void)
//����		:�ն�ע��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:  �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x0100
*********************************************************************/
ProtocolACK RadioProtocol_TerminalRegister(void)
{

	u8	*p=NULL;
	u8	*q=NULL;
	u8	Buffer[150] = {0};
	u8	Buffer2[30] = {0};
	u8	Buffer3[50] = {0};
	u8	PramLen = 0;
	u8	SmsPhone[20] = {0};
	u8	channel = 0;
	u16	length = 0;
	u8	BcdHigh = 0;
	u8	BcdLow = 0;
	u8	value = 0;
	MESSAGE_HEAD	head;
	u32	ID = 0;
	p = Buffer;
	length = 0;
	
	

	//��ȡʡ��ID
	PramLen = EepromPram_ReadPram(E2_CAR_PROVINCE_ID, Buffer2);
	if(2 == PramLen)
	{
		*p++ = Buffer2[0];
		*p++ = Buffer2[1];
		length += 2;
	}
	else
	{
		*p++ = 0;
		*p++ = 0;
		length += 2;
	}
	

	//��ȡ����ID
	PramLen = EepromPram_ReadPram(E2_CAR_CITY_ID, Buffer2);
	if(2 == PramLen)
	{
		*p++ = Buffer2[0];
		*p++ = Buffer2[1] & 0xff;
		length += 2;
	}
	else
	{
		*p++ = 0;
		*p++ = 0;
		length += 2;
	}

	//��ȡ������ID
	PramLen = EepromPram_ReadPram(E2_MANUFACTURE_ID, Buffer2); 
	if(5 == PramLen)
	{
		memcpy(p,Buffer2,5);
		p += 5;
		length += 5;
	}
	else
	{
		memcpy(p,"70108",5);
		p += 5;
		length += 5;
	}	
	//��ȡ�ն��ͺ�
    memset((char *)Buffer2,0,20);
    strcpy((char *)Buffer2,c_terminalModel);
	memcpy(p,Buffer2,20);
	p += 20;
	length += 20;
	//��ȡ�ն�ID
	PramLen = RadioProtocol_ReadDeviceID(p);
	p += 7;
	length +=7;
	//��ȡ������ɫ
	PramLen = EepromPram_ReadPram(E2_CAR_PLATE_COLOR_ID, Buffer2); //dxl,2017.2.24,������
    if(1 == PramLen)
    {
        *p++ = Buffer2[0];
        length++;

    }
    else
    {

         *p++ = 0;
         length++;
    }
    //��ȡ����,�ó��ƺ���ע��
    PramLen = EepromPram_ReadPram(E2_CAR_PLATE_NUM_ID, Buffer2); //dxl,2017.2.24,������
    if((PramLen < 20)&&(0 != PramLen))
    {
          memcpy(p,Buffer2,PramLen);
      length += PramLen;
          //p += PramLen;
          //*p = 0;
          //length++;
    }
    else
    {
          memcpy(p,"��b012346",9);
      length += 9;
          // p += 9;
          //*p = 0;
          //length++;
    }

	//��ϢID
	head.MessageID = 0x0100;
	//��Ϣ����
	head.Attribute = (length & 0x3ff);
	
	//��ȡע�����Ϣ����
	
	//�ж��Ƿ�Ϊ����Ϣ
	//if(CHANNEL_SMS == s_ucRecvChanel)
	//{
		//channel = CHANNEL_SMS;
	//}
	if(SUCCESS == RadioProtocol_SendCmd(head, Buffer, length, channel))
	{
		return ACK_OK;
	}
	else
	{
		return ACK_ERROR;
	}
	
}
/*********************************************************************
//��������	:RadioProtocol_TerminalQualify(void)
//����		:�ն˼�Ȩ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:   �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x0102
*********************************************************************/
ProtocolACK RadioProtocol_TerminalQualify(void)
{
	u8	Buffer[20];
	u8	length;

	//��ȡ��Ȩ��
	length = EepromPram_ReadPram(E2_LOGINGPRS_PASSWORD_ID, Buffer);
	Buffer[length]='\0';
    APP_DEBUG("<-- �ն˼�Ȩ����Ȩ��:%s��Ȩ�볤��%d-->\r\n",Buffer,length);

    return RadioProtocol_SendData(0x0102,Buffer,length,0);
}
/*********************************************************************
//��������	:RadioProtocol_FirmwareUpdataResultNote(u8 *pBuffer, u16 BufferLen)
//����		:Զ�̹̼��������֪ͨ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:  �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x0108
*********************************************************************/
ProtocolACK RadioProtocol_FirmwareUpdataResultNote(u8 UpdataType,u8 AckResult)
{
		u8 Buffer[5];
		u8 length;
		u8 SmsPhone[20];
		u8 channel = 0;
		MESSAGE_HEAD head;
		
		if( AckResult > 2 )
		{
			return ACK_ERROR;	 
		}
		
		//��ϢID
		head.MessageID = 0x0108;
		
		//��Ϣ����
		length = 2;
		head.Attribute = length;
		
		//��Ϣ����
		Buffer[0] = UpdataType;//��������
		Buffer[1] = AckResult; //�������
		
		//�ж��Ƿ�Ϊ����Ϣ
		if(CHANNEL_SMS == s_ucRecvChanel)
		{
			channel = CHANNEL_SMS;
		}
		
		if(SUCCESS == RadioProtocol_SendCmd(head, Buffer, length, channel))
		{
			return ACK_OK;
		}
		else
		{
			return ACK_ERROR;
		}

}

/*********************************************************************
//��������	:RadioProtocol_PostionInformationReport(u8 *pBuffer, u16 length)
//����		:λ����Ϣ�㱨
//����		:pBuffer:ָ�����ݣ�λ����Ϣ��ָ��;BufferLen:���ݵĳ���;
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:    �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x0200
*********************************************************************/
ProtocolACK  RadioProtocol_PostionInformationReport(u8 *pBuffer, u16 BufferLen)
{
	MESSAGE_HEAD	head;
	u8	SmsPhone[20];
	u8	channel = 0;
	u16	length;
	
	
	if(BufferLen >= 1024)
	{
		return ACK_ERROR;
	}
	length = BufferLen;
	//��ϢID
	head.MessageID = 0x0200;
	//��Ϣ����
	head.Attribute = (BufferLen & 0x3ff);
	//�ж��Ƿ�Ϊ����Ϣ
	//if(CHANNEL_SMS == s_ucRecvChanel)
	//{
		//channel = CHANNEL_SMS;
	//}
	if(SUCCESS == RadioProtocol_SendCmd(head, pBuffer, length, channel))
	{
		return ACK_OK;
	}
	else
	{
		return ACK_ERROR;
	}
}


/*********************************************************************
//��������	:RadioProtocol_PostionInformation_BulkUpTrans(u8 *pBuffer, u16 length)
//����		:��λ���������ϴ�
//����		:pBuffer:ָ�����ݣ�λ����Ϣ��ָ��;BufferLen:���ݵĳ���;
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		: �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		: ָ��ID��0x0704
//�μ�          : ҳ�� P45  <<��·���䳵�����Ƕ�λϵͳ�������ݳ����ն�ͨѶЭ�鼼���淶2013-01��>> 
*********************************************************************/
ProtocolACK  RadioProtocol_PostionInformation_BulkUpTrans(u8 *pBuffer, u16 BufferLen)
{
	MESSAGE_HEAD	head;
	//u8	SmsPhone[20];
	u8	channel = 0;
	u16	length;

	//�жϳ���
	if(BufferLen >= 1024)
	{
		return ACK_ERROR;
	}
	
	length = BufferLen;
	//��ϢID
	head.MessageID = 0x0704;
	//��Ϣ����
	head.Attribute = (BufferLen & 0x3ff);
        
	if(SUCCESS == RadioProtocol_SendCmd(head, pBuffer, length, channel))
	{
		return ACK_OK;
	}
	else
	{
		return ACK_ERROR;
	}
}
/*********************************************************************
//��������	:RadioProtocol_OriginalDataUpTrans(u8 Type, u32 TotalLen, u8 *pBuffer, u16 BufferLen)
//����		:��������͸��
//����		:pBuffer:ָ�����ݣ�͸�����ݣ���ָ��;BufferLen:��ǰ͸�����ݰ�����
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:    �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x0900
*********************************************************************/
ProtocolACK RadioProtocol_OriginalDataUpTrans(u8 Type, u8 *pBuffer, u16 BufferLen)
{
    unsigned char *buffer;
    u8 SmsPhone[20];
    u8 channel = 0;
    u16 length;
    MESSAGE_HEAD head;
    if(BufferLen >= 1023)
    {
        return ACK_ERROR;
    }
    buffer =ShareBuffer;
    //ָ���׵�ַ 
    memcpy(buffer+1,pBuffer,BufferLen);
    buffer[0] = Type;
    length = BufferLen+1;
    //��ϢID
    head.MessageID = 0x0900;
    //��Ϣ����
    head.Attribute = (length & 0x3ff);
    //�ж��Ƿ�Ϊ����Ϣ
    if(CHANNEL_SMS == s_ucRecvChanel)
    {
        channel = CHANNEL_SMS;
    }
    if(SUCCESS == RadioProtocol_SendCmd(head, buffer, length, channel))
    {
        return ACK_OK;
    }
    else
    {
        return ACK_ERROR;
    }
}
/*********************************************************************
//��������	:RadioProtocol_DataCompressUpTrans(u8 *pBuffer, u16 BufferLen)
//����		:����ѹ���ϴ�
//����		:pBuffer:ָ�����ݣ�ѹ�����ݣ���ָ��;BufferLen:ѹ�����ݳ���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:    �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x0901
*********************************************************************/
ProtocolACK RadioProtocol_DataCompressUpTrans(u8 *pBuffer, u16 BufferLen)
{
	
	u8	temp;
	u16 length;
	u8	SmsPhone[20];
	u8	channel = 0;
	u8	*p = NULL;
	u8	*p1 = NULL;
	u8	Buffer[1024];
	
	p = Buffer;
	p1 = p;
	if(NULL == p)
	{
		return ACK_ERROR;
	}
	//ѹ�����ݳ���
	temp = (BufferLen & 0xff000000) >> 24;
	*p++ = temp;
	temp = (BufferLen & 0xff0000) >> 16;
	*p++ = temp;
	temp = (BufferLen & 0xff00) >> 8;
	*p++ = temp;
	temp = BufferLen & 0xff;
	*p++ = temp;

	memcpy(p,pBuffer,BufferLen);
	p += BufferLen;
	length = BufferLen+4;

	MESSAGE_HEAD	head;

	//��ϢID
	head.MessageID = 0x0901;
	//��Ϣ����
	head.Attribute = (length & 0x3ff);
	
	//�ж��Ƿ�Ϊ����Ϣ
	if(CHANNEL_SMS == s_ucRecvChanel)
	{
		channel = CHANNEL_SMS;
	}
	if(SUCCESS == RadioProtocol_SendCmd(head, p1, length, channel))
	{
		return ACK_OK;
	}
	else
	{
		return ACK_ERROR;
	}
}

/*********************************************************************
//��������	:RadioProtocol_TerminalRSA(u32 RSAe, u32 RSAn)
//����		:�ն�RSA��Կ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:  �ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:ָ��ID��0x0A00
*********************************************************************/
ProtocolACK RadioProtocol_TerminalRSA(u32 RSAe, u8 *pRSAn)
{
	u16	length;
	u8	Buffer[140];
    //////////////
    Public_ConvertLongToBuffer(RSAe, Buffer);
	//�洢nֵ
	memcpy(Buffer+4,pRSAn,128);

	length = 132;    
    return RadioProtocol_SendData(0x0a00,Buffer,length,0);	
}

//*****************�������֣��Զ����ƽ̨�·�ָ��**********************
//*****************���Ĳ��֣��Զ�����ն��Ϸ�ָ��**********************
//*****************���岿�֣�����ӿں���**********************
/*********************************************************************
//��������	:RadioProtocol_UpdateTerminalPhoneNum(void)
//����		:�����ն˵绰����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:�ϵ��ʼ��ʱ�����
*********************************************************************/
void RadioProtocol_UpdateTerminalPhoneNum(void)
{
        u8  	Buffer[13];
	u8	BufferLen;

	BufferLen = EepromPram_ReadPram(E2_DEVICE_ID, Buffer);
        if(E2_DEVICE_ID_LEN == BufferLen)
        {
             memcpy(TerminalPhoneNumber,Buffer+1,E2_DEVICE_ID_LEN-1);
			 //TerminalPhoneNumber[0] &= 0x0f;
        }
        else
        {

		//���ֻ����뱸������
		//BufferLen = EepromPram_ReadPram(E2_DEVICE_PHONE_BACKUP_ID, Buffer);
		//if(E2_DEVICE_PHONE_BACKUP_ID_LEN == BufferLen)
		//{
		//	memcpy(TerminalPhoneNumber,Buffer,E2_DEVICE_PHONE_BACKUP_ID_LEN);
		//}
		//else
		{
			TerminalPhoneNumber[0] = 0x01;
              		TerminalPhoneNumber[1] = 0x37;
              		TerminalPhoneNumber[2] = 0x60;
             	 	TerminalPhoneNumber[3] = 0x25;
              		TerminalPhoneNumber[4] = 0x00;
              		TerminalPhoneNumber[5] = 0x00;
		}

        }

}


/*********************************************************************
//��������	:RadioProtocol_CheckRecvData
//����		:�����ܵ�ƽ̨�����ݷ������׼��������֡����׼��������֡7E��ͷ7E��β
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:���ܵ�ƽ̨���ݺ���øú���
*********************************************************************/
void RadioProtocol_CheckRecvData(unsigned char *recvData,unsigned short recvDataLen)
{
    unsigned short iPosition[2],datalen=0;
    unsigned short i;
    unsigned char *buffer;
    unsigned char flag;
    buffer=recvData;
    flag =0;     
    for(i = 0; i<recvDataLen; i++)
    {
        if(0x7E==buffer[i])
        {
            datalen = i;
            iPosition[flag++] = i ;            
            if(2==flag)
            {
                datalen = iPosition[1]-iPosition[0]-1;
                if(datalen>RADIOPROTOCOL_BUFFER_SIZE)break;                
                s_ucRecvChanel = s_ucCurRecvChanel;
				if(datalen>=10)
				{
                    RadioProtocol_ProtocolParse(&buffer[iPosition[0]+1], datalen);
                    datalen = iPosition[1]+1;//
				}
                else
                {
                    datalen = iPosition[1];//
                }
                break;
            }
        }
    }
    //////////////////////
    if(0==flag)
    {
        RadioProtocolRxLen = 0;
        return;
    } 
    ////���Ƿ���ڶ������/////////////////////////
    if(0==datalen)return ;
    ////////////
    if(RadioProtocolRxLen>datalen&&RadioProtocolRxLen<=RADIOPROTOCOL_BUFFER_SIZE)
    {
        RadioProtocolRxLen -= datalen;
        memmove(RadioProtocolRxBuffer,&RadioProtocolRxBuffer[datalen],RadioProtocolRxLen); 
    }
    else
    {
        RadioProtocolRxLen = 0;
    }
}
/*********************************************************************
//��������	:RadioProtocol_AddRecvDataForParse
//����		:�������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:����Э����ջ��忽�������ݺ�����ô˺���,���½��յ�������
//		:������ͬʱ�յ��ü�������,����Ҫ��һ�����У��Ǽǽ������б�,
//		:Ȼ�󴥷���������
*********************************************************************/
void RadioProtocol_AddRecvDataForParse(unsigned char channel,unsigned char *recvData,unsigned short recvDataLen)
{
/*
    #if 0
    if(LZM_QueueDataFreeSpace(&s_stRadioRecvQueue)>recvDataLen)
    {
        s_ucCurRecvChanel = channel;
        LZM_QueueDataBatchInsert(&s_stRadioRecvQueue,recvData,recvDataLen);        
        if(DISABLE==GetTimerTaskTimerState(TIME_RADIO_PARSE))
        SetTimerTask(TIME_RADIO_PARSE, 1);
    }
    #else    
    if(RadioProtocolRxLen+recvDataLen<=RADIOPROTOCOL_BUFFER_SIZE)
    {
        s_ucCurRecvChanel = channel;
        memcpy(&RadioProtocolRxBuffer[RadioProtocolRxLen],recvData,recvDataLen);
        RadioProtocolRxLen +=recvDataLen; 
        s_ucRecvTimeCnt = 0;
        SetTimerTask(TIME_RADIO_PARSE, 1);
    }
    else
    {
        if(RadioProtocolRxLen)//
        {
            if(DISABLE==GetTimerTaskTimerState(TIME_RADIO_PARSE))
            SetTimerTask(TIME_RADIO_PARSE, 1);
        }
    }
    #endif
    */
}
//////////////////////////////////////////////////////////////////////
//�������
/////////////////////////////////////////////////////////////////////



/*********************************************************************
//��������	:RadioProtocol_SendMessage(MESSAGE_HEAD	head, u8 *SrcData, u16 SrcLen, u8 SendCh)
//����		:������Ϣ
//����		:MESSAGE_HEAD	head:��Ϣͷ;SrcData:ָ����Ϣ���ָ��;
//		:SrcLen:��Ϣ�峤��;SendCh:����ͨ��,0ΪGPRS,1Ϊ����;
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:������һ��ָ���ͨ����ҪӦ��Ӧ��ʱ����øú�����
//		:�����Ϸ�һ����Ϣ����ʱҲ��Ҫ���øú�����
*********************************************************************/
ErrorStatus RadioProtocol_SendCmd(MESSAGE_HEAD	head, u8 *SrcData, u16 SrcLen, u8 SendCh )
{
    //u8	RadioProtocolTxBuffer[RADIOPROTOCOL_BUFFER_SIZE];
    u8	VerifyCode;
    u16	datalen;
    u16	length;
    //��Ȩ�ɹ���ŷ�����ָ��
    
    //�ն��ֻ���
    memcpy(head.PhoneNum,TerminalPhoneNumber,6);
    //��Ϣ��ˮ��
    MessageSerialNum++;
    head.SerialNum = MessageSerialNum;

    //MESSAGE_HEAD�ṹ����ֽ���
    datalen = RadioProtocol_HeadtoBytes(RadioProtocolTxBuffer+1,head);
    if(SrcLen > 0)
    {
        memcpy(RadioProtocolTxBuffer+1+datalen,SrcData,SrcLen);
        datalen +=SrcLen;
    }     
    VerifyCode=Public_GetXorVerify(RadioProtocolTxBuffer+1, datalen);
    RadioProtocolTxBuffer[1+datalen++] = VerifyCode;
    //ת��
    length = TransMean(RadioProtocolTxBuffer+1,datalen);
    length++;
    //����һͷһβ�ı�־λ
    RadioProtocolTxBuffer[0] = 0x7e;
    RadioProtocolTxBuffer[length] = 0x7e;
    length++;
    //����
    datalen = Communication_SndMsg2Svr(SendCh,RadioProtocolTxBuffer, length);
    if(length != datalen)
    {
        return ERROR;
    }
    return SUCCESS;
}
/*********************************************************************
//��������	:RadioProtocol_SendData
//��������	:
//��ע		:
*********************************************************************/
ProtocolACK RadioProtocol_SendData(u16 cmd, u8 *SrcData, u16 SrcLen, u8 SendCh)
{
    //u8	RadioProtocolTxBuffer[RADIOPROTOCOL_BUFFER_SIZE];
    MESSAGE_HEAD head; 
    u8	VerifyCode;
    u16	datalen;
    u16	length;
    //��Ȩ�ɹ���ŷ�����ָ��
    if(0 == GetTerminalAuthorizationFlag())
    {
        if(0x0100 == cmd)//ע��
        {
        }
        else 
        if(0x0102 == cmd)//��Ȩ
        {
        }
        else 
        if(0x0003 == cmd)//ע��
        {
        }
        else
        if((0x0001 == cmd)&&(CHANNEL_SMS == SendCh))//�������ն�Ӧ��
        {
        }
        else
        {
            return ACK_ERROR;
        }
    }
     //��ϢID
    head.MessageID = cmd;
    //��Ϣ����
    head.Attribute = SrcLen;
    //�ն��ֻ���
    memcpy(head.PhoneNum,TerminalPhoneNumber,6);
    //��Ϣ��ˮ��
    MessageSerialNum++;
    head.SerialNum = MessageSerialNum;

    //MESSAGE_HEAD�ṹ����ֽ���
    datalen = RadioProtocol_HeadtoBytes(RadioProtocolTxBuffer+1,head);
    if(SrcLen > 0)
    {
        memcpy(RadioProtocolTxBuffer+1+datalen,SrcData,SrcLen);
        datalen +=SrcLen;
    }     
    VerifyCode=Public_GetXorVerify(RadioProtocolTxBuffer+1, datalen);
    RadioProtocolTxBuffer[1+datalen++] = VerifyCode;
    //ת��
    length = TransMean(RadioProtocolTxBuffer+1,datalen);
    length++;
    //����һͷһβ�ı�־λ
    RadioProtocolTxBuffer[0] = 0x7e;
    RadioProtocolTxBuffer[length] = 0x7e;
    length++;
    //����
    datalen = Communication_SndMsg2Svr(SendCh,RadioProtocolTxBuffer, length);
    if(length != datalen)
    {
        return ACK_ERROR;
    }
    return ACK_OK;
}




//*****************�������֣�ģ���ڲ�����**********************
/*********************************************************************
//��������	:RadioProtocol_ProtocolParse(u8 *pBuffer, u16 BufferLen)
//����		:Э�����,����һ��ָ��
//����		:pBuffer:ָ������(������������)��ָ��;BufferLen:���ݵĳ���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:�ɹ�,����SUCCESS;ʧ��,����ERROR
//��ע		:ƽ̨�·�������������ɸú����������ú������������
//		:��7E��ͷ,7E��β��һ���ֽ�����
*********************************************************************/
ErrorStatus RadioProtocol_ProtocolParse(u8 *pBuffer, u16 BufferLen)
{
    unsigned char ParseBuffer[RADIOPROTOCOL_BUFFER_SIZE];
    u8 *p;
    u8 IDHighByte;
    u8 IDLowByte;
    //u8 Index;
    u8 offset;
    u16	length;
    u16	Attribute;
    //MESSAGE_HEAD RadioProtocolRxHead;
    //////////////////
    memcpy(ParseBuffer, pBuffer,BufferLen);    
    //������������0
    Heartbeat_ClrHeartbeatCount();
    ////////////////////
    //��ת��
    length = unTransMean(ParseBuffer, BufferLen);
    //���У����
    IDHighByte =Public_GetXorVerify(ParseBuffer, length-1);
    if(IDHighByte != ParseBuffer[length-1])//dxl,2013.11.30����У��,��ֹ��������
    {
        return ERROR;
    }
    //ָ����ջ���
    p = ParseBuffer;
    //��ȡ��ϢID�����������ֽ�������������ֽ���ǰ
    IDHighByte = *p++;
    IDLowByte  = *p++;
    //��ȡ��Ϣ������
    Attribute = Public_ConvertBufferToShort(p); 
    //////////////
    offset = 12;//�޷ְ���
    if(0x2000 == (Attribute&0x2000))//�зְ���
    {
        offset = 16;
    }
    ////////////////////
    length -= offset;//����Ϣͷ16�ֽ�
    length--;//��У����1�ֽ�
    ////////////////////
    if(length != (Attribute & 0x03ff))//�жϳ���
    {
        return ERROR;
    }
    ///��ϢID///////////////////
    s_usRecvMessageID = (IDHighByte << 8)|IDLowByte;
    //��Ϣ��ˮ��
    p += 8;//��Ϣ������2+�ն��ֻ���6
    //�ն��ֻ���
    //memcpy(RadioProtocolRxHead.PhoneNum,p,6);
    //p += 6;
    s_usRecvSerialNum = Public_ConvertBufferToShort(p);
    ////////////////////
    p = ParseBuffer+offset;
    switch(s_usRecvMessageID)
    {
        case 0x8001://ƽ̨����������ͨ��Ӧ��
            RadioProtocol_ServerGeneralAck(p, length);
            break;
        case 0x8100://�ն�ע��Ӧ��
            RadioProtocol_TerminalRegisterAck(p, length);
            break;  
        case 0x8103://�����ն˲���
            RadioProtocol_PramSet(p, length);
            break;
        case 0x8104://��ѯ�ն˲���
        case 0x8106://��ѯ�ն˲���
            RadioProtocol_PramSearch(p, length);
            break;
        case 0x8105://�ն˿���
            RadioProtocol_TerminalControl(p, length);
            break;
        case 0x8107:// ��ѯ�ն�����,//Add By Shigle 2013-04-01
            RadioProtocol_TerminalAttribute(p, length);	
            break;
        case 0x8201://λ����Ϣ��ѯ
            RadioProtocol_PositionSearch(p, length);
            break;
        case 0x8202://��ʱλ�ø��ٿ���
            RadioProtocol_TempPositionControl(p, length);
            break;
		case 0x8203://�˹�ȷ�ϱ���
			RadioProtocol_AlarmConfirmManual(p, length);
			break;
		case 0x8500://��������
			RadioProtocol_CarControl(p, length);
			break;
		case 0x8600://����Բ������
			RadioProtocol_SetRoundArea(p, length);
			break;
		case 0x8601://ɾ��Բ������
			RadioProtocol_DelRoundArea(p, length);
			break;	
		case 0x8602://���þ�������
			RadioProtocol_SetRectangleArea(p, length);
			break;	
		case 0x8603://���þ�������
			RadioProtocol_DelRectangleArea(p, length);
			break;	
		case 0x8604://���þ�������
			RadioProtocol_SetPolygonArea(p, length);
			break;		
		case 0x8605://���þ�������
			RadioProtocol_DelPolygonArea(p, length);
			break;	
        case 0x8900://��������͸��
            RadioProtocol_OriginalDataDownTrans(p, length);
            break;
        default: 
            RadioProtocol_TerminalGeneralAck(3);
            return ERROR;
    }
    return SUCCESS;
}
/*********************************************************************
//��������	:unTransMean
//����		:ʵ�ַ�ת�幦��
//		:0x7e----->0x7d���һ��0x02;0x7d----->0x7d���һ��0x01
//����		:����ǰ�ֽ���������ǰ�ֽ�������
//���		:�������ֽ���
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:ת��������ֽ�������
//��ע		:Э�������������ô˺������Ƚ��з�ת�壬Ȼ���ٽ�����
*********************************************************************/
u16 unTransMean(u8 *pBuffer, u16 BufferLen)
{
    u16	i;
    u16	NewLen;
    u8 temp;
    i = 0;
    NewLen = BufferLen;
    while(i < BufferLen)
    {
        if(0x7d == *pBuffer)
        {
            temp = *(pBuffer+1);
            if((0x02 == temp)||(0x01 == temp))
            {
                *pBuffer = 0x7c + temp;
                NewLen--;
                i++;
                memmove(pBuffer+1,pBuffer+2,BufferLen-i);
            }
        }
        i++;
        pBuffer++;
    }
    return NewLen;
}
/*********************************************************************
//��������	:TransMean
//����		:ʵ�ִ��ʱ��ת�崦����
//����		:ת��ǰ�ֽ�����ת��ǰ�ֽ�������
//���		:ת����ֽ���
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:ת����ֽ�������
//��ע		:0x7e----->0x7d���һ��0x02;0x7d----->0x7d���һ��0x01
*********************************************************************/
u16 TransMean(u8 *pBuffer, u16 BufferLen)
{
	u16	i;
	u16	NewLen;
	u8	temp;
	i = 0;
	NewLen = BufferLen;
	while(i < BufferLen)
	{
		if((0x7e == *pBuffer)||(0x7d == *pBuffer))
		{
            temp = (*pBuffer-0x7c);
			*pBuffer = 0x7d;
			memmove(pBuffer+2,pBuffer+1,BufferLen-i);
			pBuffer++;
			*pBuffer = temp;
			NewLen++;
		}
		i++;
		pBuffer++;
	}
	return NewLen;
}
/*********************************************************************
//��������	:RadioProtocol_HeadtoBytes(u8 *pBuffer, MESSAGE_HEAD head)
//����		:����Ϣͷ����ֽ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
u8 RadioProtocol_HeadtoBytes(u8 *pBuffer, MESSAGE_HEAD head)
{
	//u8	length;
	//u16	Attribute;

	//�����ֽ���ת���������ֽ���
	//��ϢID
	//head.MessageID = RadioProtocol_htons(head.MessageID);
	//��Ϣ������
	//Attribute = head.Attribute;
	//head.Attribute = RadioProtocol_htons(head.Attribute);
	//�ն��ֻ���
	//��Ϣ��ˮ��
	//head.SerialNum = RadioProtocol_htons(head.SerialNum);
	//�ж����޷�װ��,,�������������зְ���װ��
	//if(0x2000 == (Attribute & 0x2000))//�з�װ��
	//{
	//	head.TotalPackage = RadioProtocol_htons(head.TotalPackage);
	//	head.CurrentPackage = RadioProtocol_htons(head.CurrentPackage);
	//	length = 16;
	//}
	//else//û�з�װ��
	//{
		//length = 12;
	//}
    Public_ConvertShortToBuffer(head.MessageID, pBuffer);
    Public_ConvertShortToBuffer(head.Attribute, pBuffer+2);
    memcpy(pBuffer+4, head.PhoneNum, 6);
    Public_ConvertShortToBuffer(head.SerialNum, pBuffer+10);
	//memcpy((char *)pBuffer, (char *)&head, 12);
	return 12;
}
/*********************************************************************
//��������	:RadioProtocol_ntohs(u16 value)
//����		:�����ֽ���ת�����ֽ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
u16 RadioProtocol_ntohs(u16 value)
{
	u8	byte;
	u8	*p = NULL;
	u16	ConvertValue;

	p = (u8 *)&value;

    #if (1 == SYS_USE_BIG_LITTLE_FLAG)	
	{
		ConvertValue = value;
	}
	#else //�����ֽ���
	{
		ConvertValue = 0;	
		byte = *p++;
		ConvertValue |= byte << 8;
		byte = *p++;
		ConvertValue |= byte;	
	}
    #endif
	return ConvertValue;
	
}
/*********************************************************************
//��������	:RadioProtocol_ntoh(u32 value)
//����		:�����ֽ���ת�����ֽ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
u32 RadioProtocol_ntoh(u32 value)
{
	u8	byte;
	u32	ConvertValue;
	

	#if (1 == SYS_USE_BIG_LITTLE_FLAG)
	{
		ConvertValue = value;
	}
	#else //�����ֽ���
	{
		ConvertValue = 0;
		//��ȡ���ֽ�
		byte = (value & 0xff000000) >> 24;
		ConvertValue |= byte;
		//��ȡ�θ��ֽ�
		byte = (value & 0xff0000) >> 16;
		ConvertValue |= (byte << 8);
		//��ȡ���ֽ�
		byte = (value & 0xff00) >> 8;
		ConvertValue |= (byte << 16);
		//��ȡ���ֽ�
		byte = value;
		ConvertValue |= (byte << 24);
		
	}
    #endif
	return ConvertValue;
	
}
/*********************************************************************
//��������	:RadioProtocol_htons(u16 value)
//����		:�����ֽ���ת�����ֽ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
u16 RadioProtocol_htons(u16 value)
{
	u8	byte;
	u8	*p = NULL;
	u16	ConvertValue;

	p = (u8 *)&value;

	#if (1 == SYS_USE_BIG_LITTLE_FLAG)
	{
		ConvertValue = value;
	}
	#else //�����ֽ���
	{
		ConvertValue = 0;	
		byte = *p++;
		ConvertValue |= byte << 8;
		byte = *p++;
		ConvertValue |= byte;	
	}
    #endif
	return ConvertValue;
	
}
/*********************************************************************
//��������	:RadioProtocol_hton(u32 value)
//����		:�����ֽ���ת�����ֽ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
u32 RadioProtocol_hton(u32 value)
{
	u8	byte;
	u8	*p;
	u32	ConvertValue;

	p = (u8 *)&value;

	#if (1 == SYS_USE_BIG_LITTLE_FLAG)
	{
		ConvertValue = value;
	}
	#else //�����ֽ���
	{
		ConvertValue = 0;	
		byte = *p++;
		ConvertValue |= byte << 24;
		byte = *p++;
		ConvertValue |= byte << 16;
		byte = *p++;
		ConvertValue |= byte << 8;
		byte = *p++;
		ConvertValue |= byte;	
	}
    #endif
	return ConvertValue;
	
}
/*********************************************************************
//��������	:NullOperate(u8 *pBuffer, u16 BufferLen)
//����		:�ղ���,�����κ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:�ɹ�,����ACK_OK;ʧ�ܷ���ACK_ERROR;
//��ע		:
*********************************************************************/
ProtocolACK NullOperate(u8 *pBuffer, u16 BufferLen)
{
	return ACK_OK;
}
/*********************************************************************
//��������	:RadioProtocol_ReadDeviceID
//����		:ƽ̨��ȡ�ն�ID
//��ע		:
*********************************************************************/
unsigned char RadioProtocol_ReadDeviceID(unsigned char *data)
{
    u8	Buffer[30];
	u8	Buffer1[50];
    u8	PramLen;
    PramLen = EepromPram_ReadPram(E2_DEVICE_ID, Buffer);
    if(7 == PramLen)//ȡ�ն�ID����7λ��Ϊ��¼ע����ն�ID
    {
        Public_ConvertBcdToAsc(Buffer1,&Buffer[3],4);
   }
    else
    {
        //��ȡ���ݵ��ն�ID��
        strcpy((char *)&Buffer1[1],"3100028");
    }
    ///��ȡ�ն�ID///////////////
    memcpy(data,&Buffer1[1],7);
	//Buffer[PramLen]='\0';
    //APP_DEBUG("<-- ƽ̨��ȡ�ն�ID:%s-->\r\n",Buffer);
    return 7;
}



/*********************************************************************
//��������	:RadioProtocol_TerminalAttribute(u8 *pBuffer, u16 BufferLen)
//����		:��ѯ�ն�����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:ָ��ID��0x8107
*********************************************************************/
ProtocolACK RadioProtocol_TerminalAttribute(u8 *pBuffer, u16 BufferLen)
{
    u8 buffer[150];
    u8 tmplen;
    u16 length;
    
    length = 0;
    //��ȡ�ն����� Bytes:2 
    //Ĭ��Ϊ�����ڿ��ˡ�Σ��Ʒ�����ˣ����ͣ�һ���
    buffer[length++] = 0;
    buffer[length++] = 0x07;
    memcpy(&buffer[length],"70108",5);
    //////////////////
    length += 5;
    //////////////////////
    //��ȡ�ն��ͺ�
    memset(&buffer[length],0,20);
    Version_GetProductSeriesName((char *)&buffer[length]);
    length += 20;
    //��ȡ�ն�ID
    tmplen=RadioProtocol_ReadDeviceID(&buffer[length]);
    length += tmplen;
    //�ն�SIM��ICCID BCD[10]  Bytes:10
    Modem_Api_CCID(&buffer[length]);
    length += 10;

    //�ն�Ӳ���汾��Ϣ
    tmplen = Version_GetHardwareInformation(&buffer[length+1]);
    buffer[length++]= tmplen;
    length   +=tmplen;
    //�ն˹̼��汾��Ϣ
    tmplen = Version_GetFirmwareInformation(&buffer[length+1]);
    buffer[length++]= tmplen;
    length   +=tmplen;
    //GNSSģ������    
    buffer[length++] = 0x03;//û�����ù��Ļ���֧��GPS+����
    //ͨ��ģ������
    buffer[length++] = 0x01;//�ƶ�ͨѶ    
    return RadioProtocol_SendData(0x0107,buffer,length,s_ucRecvChanel);
}

/*********************************************************************
//��������	:RadioProtocol_GetCurRecvSerialNum
//����		:ȡ�õ�ǰ������ˮ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
unsigned short RadioProtocol_GetCurRecvSerialNum(void) 
{
    return s_usRecvSerialNum;
}

/*********************************************************************
//��������	:RadioProtocol_GetUpdataResultNoteAckFlag
//����		:ȡ���������֪ͨӦ���־
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:1Ϊ�յ�ƽ̨��ͨ��Ӧ����
//��ע		:
*********************************************************************/
unsigned char RadioProtocol_GetUpdataResultNoteAckFlag(void) 
{
    return s_ucUpdataResultNoteAckFlag;
}

