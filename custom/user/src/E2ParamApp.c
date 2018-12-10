#include "include.h"
#include "FCT_SelfCheck.h"


/*******************************************************************************/
////////////////////////////////////////////
#define NEED_CONVERT_OLD_SELF_DEFINE_ID     //�������,��ת��֮ǰ�Զ������IDΪ0x0200��ʼ�Ĳ���
/////////////////////////////////////
//////////////////////////////////////////
extern const u8 EepromPramLength[];
extern const unsigned short c_usEepromPramID[];

/*************************************************************
** ��������: E2ParamApp_ConvertPramID
** ��������: ת���ɵ��Զ������ID(0x0200��ʼ)
** ��ڲ���: ����ID
** ���ڲ���: 
** ���ز���: �������Զ����ID(0xF200��ʼ)
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/

unsigned int E2ParamApp_ConvertPramID(unsigned int usParamID)
{
    unsigned int paramID;
    paramID=usParamID;
    //////////////////////
    if(0x0200<=usParamID&&usParamID<=0x23B)//
    {
        paramID=usParamID+0xF000;
    }
    ////////////////////////
    return paramID;
}

/*************************************************************
** ��������: E2ParamApp_ReadSpecialParam
** ��������: ���������
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
unsigned char E2ParamApp_ReadSpecialParam(unsigned short usParamID,unsigned char *data)
{
    if(E2_SIM_ICCID_NUMBER_ID == usParamID)//SIM����ICCID����
    {
        //Modem_Api_CCID(data);
        return 10;
    }
    return 0;
}

/*******************************************************************************
** ��������: E2ParamApp_CheckPhoneFormatData
** ��������: �ж������Ƿ�Ϊ�ֻ��Ÿ�ʽ,BCD[6]
** ��ڲ���: data�����׵�ַ,datalen���ݳ���
** ���ز���: 0���ֻ��Ÿ�ʽ,1�����ֻ��Ÿ�ʽ
** ȫ�ֱ���: ��
** ����ģ��: ��
*******************************************************************************/
unsigned char E2ParamApp_CheckPhoneFormatData(unsigned char *data,unsigned char datalen)
{
    u8 *p = NULL;
    u8  High4Bit = 0;//��4λ
    u8  Low4Bit = 0;//��4λ
    u8  i;
    if(NULL == p)
    {
        if(6 != datalen)
        {
            return 1;//��������
        }
        p = data;
        for(i=0; i<datalen; i++)
        {
            High4Bit = (*p&0xf0) >> 4;
            Low4Bit = *p&0x0f;
            if((High4Bit > 9)||(Low4Bit > 9))
            {
                return 1;//��ʽ����
            }
            p++;
        }
        return 0;
        
    }
    else
    {
        return 1;//���ش���
    }
}
extern u16 DelayAckCommand;
/*************************************************************
** ��������: E2ParamApp_DisposeWriteParam
** ��������: ����Э��д����
** ��ڲ���: pBuffer�����׵�ַ,BufferLen���ݳ���
** ���ڲ���: 
** ���ز���: ������
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
ProtocolACK E2ParamApp_DisposeWriteParam(u8 *pBuffer, u16 BufferLen)
{
    u8	*p;
    u8	Buffer[30];
    u8	ReadLen;
    u8	PramLen;
    s16	length;
    u32	PramID,prePramID;
    u16	index;

    p = pBuffer;
    length = BufferLen;

    //��ֻ���ǵ�ֵ�����
    while(length > 0)
    {
        //������ID
        prePramID=Public_ConvertBufferToLong(p);
		if(0x0000f238 == prePramID)//���ģʽ���⴦��
		{
			DelayAckCommand = 12;//�ػ�
        	//������ʱ����
        	SetTimerTask(TIME_DELAY_TRIG,3*SECOND);
			return ACK_OK;
		}
		if(0 == FCT_ReadSelfCheckFlg())//����ģʽ���л���FCTģʽ
		{
			if(0x0001ffff == prePramID)
			{
				APP_DEBUG("\r\nSYSTEM RESET INTO FCTMODE\r\n");
				Buffer[0] = 0; 
				FRAM_BufferWrite(FRAM_FCT_SELF_CHECK_ADDR,Buffer,FRAM_FCT_SELF_CHECK_LEN);
				DelayAckCommand = 5;//system reset
	        	//������ʱ����
	        	SetTimerTask(TIME_DELAY_TRIG,3*SECOND);
				return NOACK;
			}
		}
        p += 4;
        ////////////////////////
        PramID = E2ParamApp_ConvertPramID(prePramID);
        /////////////////////////
        index = EepromPram_ConvertParamIDToIndex(PramID);
        ///////////////
        //��ȡ��������
        PramLen = *p++;
		
        //////////////
        if(index)//����ID��ʶ���,��������
        {
            //������ID�Ͳ��������Ƿ���ȷ
            if(EepromPramLength[index] >= PramLen)
            {
                //���һЩ�ض��Ĳ����Ƿ���ȷ
                
                if((E2_SIM_ICCID_NUMBER_ID == PramID)//SIM����ICCID����,���ñ���,ֱ�Ӷ�
                 ||(E2_FIRMWARE_VERSION_ID == PramID))
                {
                    //����Ҫ����
                }
                else
                {      
                  if((E2_MAIN_SERVER_IP_ID == PramID)||                    
                    (E2_BACKUP_SERVER_IP_ID == PramID)||
                    (E2_MAIN_SERVER_TCP_PORT_ID == PramID)||
                    (E2_CAR_PROVINCE_ID == PramID)||
                    (E2_CAR_CITY_ID == PramID)||
                    (E2_CAR_PLATE_NUM_ID == PramID)||
                    (E2_CAR_PLATE_COLOR_ID == PramID)||
					(E2_DEVICE_PHONE_ID == PramID)||
                    (E2_DEVICE_ID == PramID))
                    {
                    	
                        //������Ȩ��,�´��������·�ע��ָ��
                        //ClearTerminalAuthorizationCode(CHANNEL_DATA_1);
						//SetAccLineFlag(0);
						//Net_First_Value(MAIN_SERVER);
						//Socket_Restart();
						//Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
                    }
				  	//APP_DEBUG("����...%d..%d..%04x",index,PramLen,PramID);
                    EepromPram_WritePramEx(index, p, PramLen);
                    //////////////////////// 
                    if(E2_DEVICE_ID == PramID)//д�뱸������,�ն�ID
                    {
                        EepromPram_WritePram(E2_DEVICE_BACKUP_ID, p, PramLen);
						EepromPram_WritePram(E2_DEVICE_PHONE_ID, p+1, E2_DEVICE_PHONE_ID_LEN);
                    }
					if(E2_SLEEP_REPORT_TIME_ID == PramID)
                    {
                        extern s32 m_pwrOnReason;
                    	m_pwrOnReason = PWRKEYPWRON;
                    }
                    //������д��Ĳ���                   
                    if(PramLen < 30)
                    { 
                        ReadLen = EepromPram_ReadPramEx(index, Buffer);
                        if(Public_CheckArrayValIsEqual(Buffer,p,ReadLen))//д����
                        {
                            return ACK_ERROR;//д����
                        }
                    }
                    //������Ӧ�ı���
                    EepromPram_UpdateVariable(PramID); 
                }
            }
            else//�������Ȳ���ȷ��
            {
                //return 0;
            }
        }
        else//����ID����ʶ��
        {
            return NOACK;;
        }
        ////////////////////
        length = length - 4 - 1 - PramLen;
        p += PramLen;
    }
    return ACK_OK;
}
/*************************************************************
** ��������: E2ParamApp_ReadOneParam
** ��������: ����Э�������
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
unsigned char E2ParamApp_ReadOneParam(unsigned short index,unsigned int PramID,unsigned char *data)
{
    u8 PramLen = 0;
    u8 Buffer[50];
	char ch[30] = {'\0'};
    if(E2_SIM_ICCID_NUMBER_ID == PramID)//SIM����ICCID����
    {
        //д����ID
        Public_ConvertLongToBuffer(PramID,data);
        data[4] = 10;//д��������
        Modem_Api_CCID(&data[5]);
        return 15;
    }
    else
    if((E2_FIRMWARE_VERSION_ID&0x0FFF)==(PramID&0x0FFF))//�汾��
    {
        Public_ConvertLongToBuffer(PramID,data);
		//data="V1.01";
        Version_GetEquipmentSoftVer((char *)&data[5]);
        data[4] = 6;//
        return 11;
    }
	else
    if(E2_DEFINE_HARD_VER_ID==PramID)//Ӳ���汾��
    {
        Public_ConvertLongToBuffer(PramID,data);
        PramLen = Version_GetHardwareInformation((char *)&data[5]);
        data[4] = PramLen;//
        return (5+PramLen);
    }
	else
    if(0x0001F010==PramID)//IMEI 0001F010
    {
        Public_ConvertLongToBuffer(PramID,data);
		if(RIL_AT_SUCCESS == RIL_GetIMEI(ch))
		{
			PramLen = Ql_strlen(ch)-2;//��ȥ0D  0A �س�����
	        Ql_strcpy(&data[5],ch+2);
        }
		data[4] = PramLen;//
        return (5+PramLen);
    }
	else
    if(0x0001F011==PramID)//IMSI 0001F011
    {
        Public_ConvertLongToBuffer(PramID,data);
		if(RIL_AT_SUCCESS == RIL_SIM_GetIMSI(ch))
        {
	        PramLen = Ql_strlen(ch)-2;//��ȥ0D  0A �س�����
	        Ql_strcpy(&data[5],ch+2);
        }
		data[4] = PramLen;//
        return (5+PramLen);
    }
	else
    if(0x0001F012==PramID)//SN 0001F012
    {
        Public_ConvertLongToBuffer(PramID,data);
		PramLen = Ql_GetUID(Buffer, sizeof(Buffer));
        Ql_memcpy(&data[5],Buffer,PramLen);
        data[4] = PramLen;//
        return (5+PramLen);
    }
	else
	if(0x0000f238 == PramID)//���ģʽ���⴦��
	{
		Public_ConvertLongToBuffer(PramID,data);//���ػ��˻�����ë��
		PramLen = 1;        
        data[4] = PramLen;
		data[5] = 0;
        return (5+1);
	}
    //////////////////////////
    PramLen = EepromPram_ReadPramEx(index, Buffer);
	//APP_DEBUG("\r\n��������...%d\r\n",PramLen)
    if(PramLen)
    {
        //д����ID
        Public_ConvertLongToBuffer(PramID,data);
        //д��������
        data[4] = PramLen;
        //д��������
        memcpy(&data[5],Buffer,PramLen);
        return PramLen+5;
    }
    return 0;
}
/*************************************************************
** ��������: E2ParamApp_DisposeReadAllParam
** ��������: ����Э�������
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: 
** ����ģ��: 
** ��    ע: ��ȡ�Ĳ����ܸ�����*pDstBuffer��ID�ţ�ID���ȣ�ID���ݵ�˳���ţ�*DstBufferLen�ܵĶ�ȡ���ȣ������������ܸ����ֽ�
           : ReadType:0Ϊ����(����)��ÿ��ֻ�ܶ�550�ֽڣ�1Ϊ���ߣ�GPRS����ÿ�οɶ�1000�ֽ�
*************************************************************/	
unsigned char E2ParamApp_DisposeReadAllParam(unsigned char *pDstBuffer,s16 *DstBufferLen)
{
    unsigned char *des;
    unsigned char paramNum,len;
    signed short length;
    unsigned short maxlen,index;
    unsigned long PramID;  
    length =0;
    paramNum =0;    
    des = pDstBuffer;
    maxlen = 256;
    //**************ƽָ̨���Ĳ���*****************
    for(index=0; index<EepromPram_GetLenEepromPramID(); index++)
    {
        PramID = c_usEepromPramID[index];
        len = E2ParamApp_ReadOneParam(index,PramID,des);
        if(len)
        {
            if(length+len>maxlen)break;
            des += len;
            length+=len;
            paramNum++;
        }
    }     
    ////////////////////
    *DstBufferLen=length;
    ////////////////
    return paramNum;
}
/*************************************************************
** ��������: E2ParamApp_DisposeReadParam
** ��������: ����Э�������
** ��ڲ���: ����Ҫ���Ĳ���ID�ŷ��뻺��pIDBuffer�У�ÿ��ID���ֽڣ�����ǰ��IDBufferLenΪ4��������
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: 
** ����ģ��: 
** ��    ע: ��ȡ�Ĳ����ܸ�����*pDstBuffer��ID�ţ�ID���ȣ�ID���ݵ�˳���ţ�*DstBufferLen�ܵĶ�ȡ���ȣ������������ܸ����ֽ�
           : ReadType:0Ϊ����(����)��ÿ��ֻ�ܶ�550�ֽڣ�1Ϊ���ߣ�GPRS����ÿ�οɶ�1000�ֽ�
*************************************************************/	
unsigned char E2ParamApp_DisposeReadParam(unsigned char *pDstBuffer,s16 *DstBufferLen,unsigned char *pIDBuffer,signed short IDBufferLen)
{
    unsigned char *des,*src;
    unsigned char paramNum,len;
    signed short length,datalen;
    unsigned short maxlen,index;
    unsigned long PramID,readParamID;    
    paramNum =0;
    length =0;
    des = pDstBuffer;    
    src = pIDBuffer;//    
    datalen =IDBufferLen;
    maxlen = 256;
    //////////////////
    while(datalen > 0)
    {
        PramID=Public_ConvertBufferToLong(src);        
        readParamID = E2ParamApp_ConvertPramID(PramID);
        index = EepromPram_ConvertParamIDToIndex(readParamID);
        //������
        len = E2ParamApp_ReadOneParam(index,PramID,des);
        if(len)
        {
            if(length+len>maxlen)break;
            des += len;
            length+=len;
            paramNum++;
        }
        src += 4;
        datalen -=4;
    }
    ////////////////////
    *DstBufferLen=length;
    ////////////////
    return paramNum;
}


