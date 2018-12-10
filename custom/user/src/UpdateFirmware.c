#include"include.h"

#define URL_LEN 512
#define APN      "cmnet"
#define USERID   ""
#define PASSWD   ""

static APP_FTP         NetOther_FtpVal;//FTP��������
static u8 m_URL_Buffer[URL_LEN];

// ������ʼ�����ļ�
void ftp_downfile_timer(void)
{

    s32 strLen;
    ST_GprsConfig apnCfg;
    Ql_memcpy(apnCfg.apnName,   APN, Ql_strlen(APN));
    Ql_memcpy(apnCfg.apnUserId, USERID, Ql_strlen(USERID));
    Ql_memcpy(apnCfg.apnPasswd, PASSWD, Ql_strlen(PASSWD));

    //ftp://hostname/filePath/fileName:port@username:password
    strLen = Ql_sprintf(m_URL_Buffer, "ftp://%s%s%s:%s@%s:%s",NetOther_FtpVal.ip, NetOther_FtpVal.fpath, NetOther_FtpVal.fname,
    														  NetOther_FtpVal.port, NetOther_FtpVal.user, NetOther_FtpVal.pass);
    APP_DEBUG("\r\n<-- URL:%s -->\r\n",m_URL_Buffer);
    
    strLen=Ql_FOTA_StartUpgrade(m_URL_Buffer, &apnCfg, NULL);
	if(strLen==-1)
		{
			Ql_Reset(0);
		}
}

/*
  * @brief  ��ʼ����ftp�ļ�����
  * @param  None
  * @retval �����Ƿ�ִ������, 1Ϊ������0Ϊ���������ⲻ����
  */
u8 Net_Other_FtpStart(void)
{
    if(NetOther_FtpVal.flag == 0)
    {
    /*
        ClrTimerTask(TIME_CORNER);
    	ClrTimerTask(TIME_POSITION);
        ClrTimerTask(TIME_IO_MONITOR);
        ClrTimerTask(TIME_GPSREC);
     */
     //Stop_TaskScheduleTimer();
	// CtrGnssOFF();
        NetOther_FtpVal.flag = 1;//��λftp������־
        return 1;
    }
    return 0;
}
//��������־
u8 GetUpdateFlag(void)
{
	return NetOther_FtpVal.flag;
}

//д������־
void SetUpdateFlag(u8 sta)
{
	NetOther_FtpVal.flag = sta;
}

/*********************************************************************
//��������	:Updata_SendResultNoteTimeTask
//����		:�����������֪ͨ��ƽ̨
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:Ĭ���ǿ�����,1�����1��
*********************************************************************/
void Updata_SendResultNoteTimeTask(void)
{
    static u8 sucUpdataAckResult = 0;
    static u8 sucUpdataState = 0;
    static u8 sucUpdatacount = 0;    
    /////////////////////////
    if(0xff==sucUpdataState)return;
    /////////////////////////////
    switch(sucUpdataState)
    {
        case 0://��ȡ�����Ϣ
        {
            u8 Buffer[3] = {0};
            if(FRAM_BufferRead(Buffer,FRAM_FIRMWARE_UPDATA_FLAG_LEN,FRAM_FIRMWARE_UPDATA_FLAG_ADDR))
            {
            
				APP_DEBUG("\r\n<-- FRAM_FIRMWARE_UPDATA_FLAG :%d -->\r\n",Buffer[0]);
                //////////////
                sucUpdataState = 1;//������һ��
                //////////////
                if((1 == Buffer[0])||(3 == Buffer[0]))//1:��ʾƽ̨�·���Զ������ָ��,2:�����ɹ�,3:����ʧ��
                {
                    sucUpdataAckResult = 1;
                }
                else 
                if(2 == Buffer[0])
                {
                    sucUpdataAckResult = 0;
                }
                else
                {
                    sucUpdataState = 0xff;
                }                
            } 
            break;
        }
        case 1://����Ƿ����� 
        {
            if(GetTerminalAuthorizationFlag())
            {
                sucUpdatacount = 0;
                sucUpdataState++;
            }
            else
            {
                sucUpdatacount++;
                if(sucUpdatacount>120)
                {
                    sucUpdataState = 0xff;
                }
            }
            break;
        }
        case 2://�����������֪ͨ��ƽ̨
        {
            sucUpdataState++;
			APP_DEBUG("\r\n<-- FRAM_FIRMWARE_UPDATA_RESULT -->\r\n");
            RadioProtocol_FirmwareUpdataResultNote(0,sucUpdataAckResult);
            return ;
        }
        case 3://
        case 4://
        case 5://
            sucUpdataState++;//�������ˣ��ȴ�3���ƽ̨��Ӧ
            break;
        case 6://���Ӧ��
        {
            if(0==RadioProtocol_GetUpdataResultNoteAckFlag())
            {
                sucUpdataState = 2;
                sucUpdatacount++;
                if(sucUpdatacount < 3)break;
            }
            sucUpdatacount = 0;
            FRAM_BufferWrite(FRAM_FIRMWARE_UPDATA_FLAG_ADDR,&sucUpdatacount,1);//д�������´ξͲ��ᷢ�ͽ��֪ͨ�� dxl,2017.2.23,������
            sucUpdataState = 0xff;
            break;
        }
        default:
            sucUpdataState = 0xff;
            break;;
    }
    return ;
}


u8 Net_Ftp_Parameter(u8 ParaType,u8 *tab,u8 tab_len)
{
    u8  *p=NULL;
    u8 i;
    u8 len;//����˿ڵȳ���
    u8  num=0;//","����
    u8  numaddr[5]={0};//","�ĵ�ַ
    u8  addr=0;
    switch(ParaType)
    {
    case 1://URL��ַ
      {
        for(i=0;i<tab_len;i++)
        {
          if(tab[i] == ',')
          {
            numaddr[num] = i;
            num++;
          }
        }
        if(num != 3)
        {
          return 1;
        }
        //ftp�ļ�·��
        p = NetOther_FtpVal.fpath;
        len = sizeof(NetOther_FtpVal.fpath);
        memset(p,0,len); 
        memcpy(NetOther_FtpVal.fpath,tab,numaddr[0]); 
		APP_DEBUG("\r\n<-- NetOther_FtpVal fpath :%s -->\r\n",NetOther_FtpVal.fpath);
		
        //ftp�ļ���
        p = NetOther_FtpVal.fname;
        len = sizeof(NetOther_FtpVal.fname);
        memset(p,0,len);
        addr = numaddr[0] + 1;
        len  = numaddr[1] - numaddr[0] - 1;
        memcpy(p,tab+addr,len); 
		APP_DEBUG("\r\n<-- NetOther_FtpVal fname :%s -->\r\n",NetOther_FtpVal.fname);
		
        //ftp�û���
        p = NetOther_FtpVal.user;
        len = sizeof(NetOther_FtpVal.user);
        memset(p,0,len);
        addr = numaddr[1] + 1;
        len  = numaddr[2] - numaddr[1] - 1;    
        memcpy(p,tab+addr,len); 
		APP_DEBUG("\r\n<-- NetOther_FtpVal user :%s -->\r\n",NetOther_FtpVal.user);

        //ftp��½����
        p = NetOther_FtpVal.pass;
        len = sizeof(NetOther_FtpVal.pass);
        memset(p,0,len);
        addr = numaddr[2] + 1;
        len  = tab_len - addr;    
        memcpy(p,tab+addr,len);  
		APP_DEBUG("\r\n<-- NetOther_FtpVal pass :%s -->\r\n",NetOther_FtpVal.pass);
        break;
      }
    case 2://���ŵ�����
      {
         break;
      }
    case 3://�����û���
      {
        break;
      }
    case 4://��������
      {
        
        break;
      }
    case 5://ftp��ip��ַ
      {
        p = NetOther_FtpVal.ip;
        len = sizeof(NetOther_FtpVal.ip);
        memset(p,0,len);
        memcpy(NetOther_FtpVal.ip,tab,tab_len);
		APP_DEBUG("\r\n<-- NetOther_FtpVal ip :%s -->\r\n",NetOther_FtpVal.ip);
        break;
      }
    case 6://TCP�˿�
      {
        p = NetOther_FtpVal.port;
        len = sizeof(NetOther_FtpVal.port);
        memset(p,0,len);
        //sprintf((char*)NetOther_FtpVal.port,"%d",Public_ConvertBufferToShort(tab));
        Ql_sprintf((char*)NetOther_FtpVal.port,"%d",Public_ConvertBufferToShort(tab));
		//Ql_sprintf(m_URL_Buffer, "ftp://%s%s%s:%s@%s:%s",FTP_SVR_ADDR, FTP_SVR_PATH, FTP_FILENAME, FTP_SVR_PORT, FTP_USER_NAME, FTP_PASSWORD);
		//NetOther_FtpVal.port=((u32)tab[0]<<8)|(u32)tab[1];

		
		APP_DEBUG("\r\n<-- NetOther_FtpVal port :%s -->\r\n",NetOther_FtpVal.port);
		Ql_sprintf(m_URL_Buffer, "ftp://%s%s%s:%s@%s:%s",NetOther_FtpVal.ip, NetOther_FtpVal.fpath, NetOther_FtpVal.fname,
																	  NetOther_FtpVal.port, NetOther_FtpVal.user, NetOther_FtpVal.pass);
			APP_DEBUG("\r\n<-- URL:%s -->\r\n",m_URL_Buffer);

        break;
      }      
    }
    return 0;
}

