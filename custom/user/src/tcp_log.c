#include <include.h>
#include "report.h"
#include "FCT_SelfCheck.h"

#define MAXCLIENT_NUM   5

u8	TerminalAuthorizationFlag=0;//�ն˼�Ȩ��־,��Ȩ�ɹ�Ϊ1,ʧ��Ϊ0
u8	Link1LoginCount = 0;//ע������ʹ�����ÿ��30�룬3�β��ɹ�����Ͽ����磬10���Ӻ�����
u8	Link1AuthorizationCount = 0;//��Ȩ����ʹ�����ÿ��30�룬10�β��ɹ���������Ȩ�룬����ע������
u8	Link1LoginAuthorizationState = 0;//��ȡ��Ȩ�룬����ע��������ͼ�Ȩ����Ͽ����磬��ʱ���������硢�ر����񼸸�״̬
u8	Link1LoginAuthorizationLastState = 0;//��һ��״̬
u16	Link1LoginAuthorizationDelayTime = 0;//ע���Ȩ��ʱʱ��
u8 Link1ManualCloseFlag = 0;//����1�ֶ��رձ�־,1Ϊ�ر�,0Ϊ����

//***************���ر���******************
static u16	Link1LoginAuthorizationDelayCount = 0;//��ʱ������1���Ӽ���1
static s32 TCP_error_time=0;//��ʼ��������TCP���������
bool TIMER_TCP_flag		=	FALSE;									//�ж�TCP��ѯ�Ƿ���(��ʼ��δ��)				
bool SIM_STATE_flag = FALSE;//SIM��״̬��־��TRUEλSIM��׼���ã�FALSE ΪSIM��û��׼����

u8 FCT_Test = 0;//FCT����λ

ServerContInfo ConnectInfo;


extern u32 battery_capacity;
extern u32 battery_voltage;

//TCP���
static u8 m_tcp_state = STATE_NW_GET_SIMSTATE;//TCP״̬������
/*****************************************************************
* timer param
******************************************************************/


static s32 timeout_90S_monitor = FALSE;
/*****************************************************************
* APN Param
******************************************************************/
SOC_IPVAL net_info={
		{
		"CMNET",    // APN name
    	"card",         // User name for APN
		"card",         // Password for APN
		0,
    	NULL,
    	NULL,
		},
		0,
		0,
		0
};

/*****************************************************************
* Server Param
******************************************************************/
#define SRVADDR_BUFFER_LEN  100
#define SEND_BUFFER_LEN     2048
#define RECV_BUFFER_LEN     2048

static u8 m_send_buf[SEND_BUFFER_LEN];
static u8 m_recv_buf[RECV_BUFFER_LEN];
static u64 m_nSentLen  = 0;      // Bytes of number sent data through current socket    

//static u8  m_SrvADDR[SRVADDR_BUFFER_LEN] = "momo.e-eye.cn";
//static u32 m_SrvPort = 11111;
static u8  m_ipaddress[5];  //only save the number of server ip, remove the comma

static s32 m_socketid = -1; 

static s32 m_remain_len = 0;     // record the remaining number of bytes in send buffer.
static char *m_pCurrentPos = NULL; 

/*****************************************************************
* GPRS and socket callback function
******************************************************************/
//This callback function is invoked by Ql_SocketConnect when the return value of Ql_SocketConnect is SOC_WOULDBLOCK
void socket_connect_callback(s32 socketId, s32 errCode, void* customParam );

// This callback function is invoked when the socket connection is disconnected by server or network.
void socket_close_callback(s32 socketId, s32 errCode, void* customParam );

//Accept a connection on a socket when the module is a server. This function is valid when the module is used as TCP server only
void socket_accept_callback(s32 listenSocketId, s32 errCode, void* customParam );

// This callback function is invoked when socket data arrives.
void socket_read_callback(s32 socketId, s32 errCode, void* customParam );

// This callback function is invoked in the following case:
// The return value is less than the data length to send when calling Ql_SOC_Send(), which indicates
// the socket buffer is full. Application should stop sending socket data till this callback function
// is invoked, which indicates application can continue to send data to socket.
void socket_write_callback(s32 socketId, s32 errCode, void* customParam );

//When the return value of Ql_GPRS_Activate is SOC_WOULDBLOCK, this callback function will be invoked later.
void GPRS_Actived_callback(u8 contexId, s32 errCode, void* customParam);

// This callback function is invoked when GPRS drops down.
void GPRS_Deactived_callback(u8 contextId, s32 errCode, void* customParam );

//This function is 
void GetIpByName_callback(u8 contexId, u8 requestId, s32 errCode,  u32 ipAddrCnt, u32* ipAddr);


ST_PDPContxt_Callback     callback_gprs_func = 
{
    GPRS_Actived_callback,
    GPRS_Deactived_callback
};
ST_SOC_Callback      callback_soc_func=
{
    socket_connect_callback,
    socket_close_callback,
    socket_accept_callback,
    socket_read_callback,    
    socket_write_callback
};


/*****************************************************************
* timer callback function
******************************************************************/
static void Timer_Callback(u32 timerId, void* param);


static void checkErr_AckNumber(s32 err_code)
{
    if(SOC_INVALID_SOCKET == err_code)
    {
        APP_DEBUG("<-- Invalid socket ID -->\r\n");
    }
    else if(SOC_INVAL == err_code)
    {
        APP_DEBUG("<-- Invalid parameters for ACK number -->\r\n");
    }
    else if(SOC_ERROR == err_code)
    {
        APP_DEBUG("<-- Unspecified error for ACK number -->\r\n");
    }
    else
    {
        // get the socket option successfully
    }
}

static void Timer_Callback(u32 timerId, void* param)
{
	s32 ret;
    if (TIMEOUT_90S_TIMER_ID == timerId)
    {
        APP_DEBUG("<--90s time out!!!-->\r\n");
        APP_DEBUG("<-- Close socket.-->\r\n");
        
        Ql_SOC_Close(m_socketid);
        m_socketid = -1;

        m_tcp_state = STATE_GPRS_DEACTIVATE;
		ConnectInfo.ipConnectFailureCnt++;

        timeout_90S_monitor = FALSE;
    }
    else if (TCP_TIMER_ID == timerId)
    {
        //APP_DEBUG("<--...........m_tcp_state=%d..................-->\r\n",m_tcp_state);
        switch (m_tcp_state)
        {
            case STATE_NW_GET_SIMSTATE://�ȴ�SIM��׼����
            {
                s32 simStat = 0;
				APP_DEBUG("<--SIM card status checking!-->\r\n");
				if((ConnectInfo.curIpType==MAIN_SERVER)&&(ConnectInfo.ipConnectFailureCnt>2))
					{
					Net_First_Value(MAIN_SERVER);
					}
					else if((ConnectInfo.curIpType==BACK_SERVER)&&(ConnectInfo.ipConnectFailureCnt>2))
						{
						Net_First_Value(MAIN_SERVER);
						}
		
                RIL_SIM_GetSimState(&simStat);
                if (simStat == SIM_STAT_READY)
                {
					ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);  
                    m_tcp_state = STATE_NW_QUERY_STATE;
                    APP_DEBUG("<--SIM card status is normal!-->\r\n");
                    if(0xaa == FCT_ReadSelfCheckFlg())
                    {
						APP_DEBUG("\r\nOK:SIMCARD CHACK OK\r\n");
						PUBLIC_SETBIT(FCT_Test,0);
                    }
                }
				else
				if (simStat == SIM_STAT_NOT_INSERTED)
                {APP_DEBUG("\r\nERROR:NOTINSERTED\r\n");
                    if(0xaa == FCT_ReadSelfCheckFlg())
                    {
						APP_DEBUG("\r\nERROR:NOTINSERTED\r\n");
                    }
                }
				else
				if (simStat == SIM_STAT_PIN_REQ)
                {APP_DEBUG("\r\nERROR:SIMCARD PIN OPEN\r\n");
                    if(0xaa == FCT_ReadSelfCheckFlg())
                    {
						APP_DEBUG("\r\nERROR:SIMCARD PIN OPEN\r\n");
                    }
                }
				else
				if (simStat == SIM_STAT_NOT_READY)
                {APP_DEBUG("\r\nERROR:SIMCARD NOT READY\r\n");
                    if(0xaa == FCT_ReadSelfCheckFlg())
                    {
						APP_DEBUG("\r\nERROR:SIMCARD NOT READY\r\n");
                    }
                }
				else
                {
                //    Ql_Timer_Stop(TCP_TIMER_ID);
                APP_DEBUG("\r\nERROR:SIMCARD UNNORMAL\r\n");
                    if(0xaa == FCT_ReadSelfCheckFlg())
                    {
						APP_DEBUG("\r\nERROR:SIMCARD UNNORMAL\r\n");
                    }
                }
                break;
            }        
            case STATE_NW_QUERY_STATE://�ȴ�GSM,SPRS����ע���
            {
                s32 creg = 0;
                s32 cgreg = 0;
                //Ql_NW_GetNetworkState(&creg, &cgreg);
                ret = RIL_NW_GetGSMState(&creg);
                ret = RIL_NW_GetGPRSState(&cgreg);
                APP_DEBUG("<--Network State:creg=%d,cgreg=%d-->\r\n",creg,cgreg);
                if((cgreg == NW_STAT_REGISTERED)||(cgreg == NW_STAT_REGISTERED_ROAMING))
                {
                    m_tcp_state = STATE_GPRS_REGISTER;
                }
				else
				{
					if(!timeout_90S_monitor)//��������ʱ��
					{
						Ql_Timer_Start(TIMEOUT_90S_TIMER_ID, TIMEOUT_90S_PERIOD, FALSE);
						timeout_90S_monitor = TRUE;
					}
				}
                break;
            }
            case STATE_GPRS_REGISTER://ע��GPRS��ػص�����
            {
                ret = Ql_GPRS_Register(0, &callback_gprs_func, NULL);
                if (GPRS_PDP_SUCCESS == ret)
                {
                	if(0xaa == FCT_ReadSelfCheckFlg())
                    {
						APP_DEBUG("\r\nOK:GPRS READY\r\n");
						PUBLIC_SETBIT(FCT_Test,1);
                    }
                    APP_DEBUG("<--Register GPRS callback function successfully.-->\r\n");
                    m_tcp_state = STATE_GPRS_CONFIG;
                }else if (GPRS_PDP_ALREADY == ret)
                {
                    APP_DEBUG("<--GPRS callback function has already been registered,ret=%d.-->\r\n",ret);
                    m_tcp_state = STATE_GPRS_CONFIG;
                }else
                {
                	if(0xaa == FCT_ReadSelfCheckFlg())
                    {
						APP_DEBUG("\r\nERROR:GPRS FAIL TO SIGN IN\r\n");
                    }
                    APP_DEBUG("<--Register GPRS callback function failure,ret=%d.-->\r\n",ret);
                }
                break;
            }
            case STATE_GPRS_CONFIG://����GPRS��ز���������APN ,�����û����������
            {
                ret = Ql_GPRS_Config(0, &net_info.m_GprsConfig);
                if (GPRS_PDP_SUCCESS == ret)
                {
                    APP_DEBUG("<--configure GPRS param successfully.-->\r\n");
                }else
                {
                    APP_DEBUG("<--configure GPRS param failure,ret=%d.-->\r\n",ret);
                }
                
                m_tcp_state = STATE_GPRS_ACTIVATE;
                break;
            }
            case STATE_GPRS_ACTIVATE://��������
            {
                m_tcp_state = STATE_GPRS_ACTIVATING;
                ret = Ql_GPRS_Activate(0);
                if (ret == GPRS_PDP_SUCCESS)//��������ɹ�
                {
                    APP_DEBUG("<--Activate GPRS successfully.-->\r\n");
                    m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
                }else
                if (ret == GPRS_PDP_WOULDBLOCK)//�ȴ���������ɹ�������ɹ�ϵͳ���Զ����ûص����� Callback_GPRS_Actived
                {  
                	if(!timeout_90S_monitor)//��������ʱ��
                      {
                        Ql_Timer_Start(TIMEOUT_90S_TIMER_ID, TIMEOUT_90S_PERIOD, FALSE);
                        timeout_90S_monitor = TRUE;
                      }
                     APP_DEBUG("<--Waiting for the result of GPRS activated.,ret=%d.-->\r\n",ret);
                    //waiting Callback_GPRS_Actived
                }else
                if (ret == GPRS_PDP_ALREADY)//���缤�������
                {
                    APP_DEBUG("<--GPRS has already been activated,ret=%d.-->\r\n",ret);
                    m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
                }else//error
                {
                    APP_DEBUG("<--Activate GPRS failure,ret=%d.-->\r\n",ret);
                    m_tcp_state = STATE_GPRS_ACTIVATE;
                }
                break;
            }
            case STATE_GPRS_GET_DNSADDRESS://��ȡ����DNS�����ַ
            {            
                u8 primaryAddr[16] = {0};
                u8 bkAddr[16] = {0};
                ret =Ql_GPRS_GetDNSAddress(0, (u32*)primaryAddr,  (u32*)bkAddr);
                if (ret == GPRS_PDP_SUCCESS)
                {
                    APP_DEBUG("<--Get DNS address successfully,primaryAddr=%d.%d.%d.%d,bkAddr=%d.%d.%d.%d-->\r\n",primaryAddr[0],primaryAddr[1],primaryAddr[2],primaryAddr[3],bkAddr[0],bkAddr[1],bkAddr[2],bkAddr[3]);            
                    m_tcp_state = STATE_GPRS_GET_LOCALIP;
                }else
                {
                     APP_DEBUG("<--Get DNS address failure,ret=%d.-->\r\n",ret);
                    m_tcp_state = STATE_GPRS_DEACTIVATE;
                }
                break;
            }
            case STATE_GPRS_GET_LOCALIP://��ȡ����IP��ַ
            {
                u8 ip_addr[5];
                Ql_memset(ip_addr, 0, 5);
                ret = Ql_GPRS_GetLocalIPAddress(0, (u32 *)ip_addr);
                if (ret == GPRS_PDP_SUCCESS)
                {
                    APP_DEBUG("<--Get Local Ip successfully,Local Ip=%d.%d.%d.%d-->\r\n",ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3]);
                    m_tcp_state = STATE_CHACK_SRVADDR;
                }else
                {
                    APP_DEBUG("<--Get Local Ip failure,ret=%d.-->\r\n",ret);
                }
                break;
            }
            case STATE_CHACK_SRVADDR://��������ip(����)��ַ
            {
                Ql_memset(m_ipaddress,0,5);
				//Net_First_Value(MAIN_SERVER);
				//APP_DEBUG("<--MAIN_SERVER checking!-->\r\n");
                ret = Ql_IpHelper_ConvertIpAddr(net_info.ip, (u32 *)m_ipaddress);
                if(ret == SOC_SUCCESS) // ip address, xxx.xxx.xxx.xxx
                {
                    APP_DEBUG("<--Convert Ip Address successfully,m_ipaddress=%d,%d,%d,%d-->\r\n",m_ipaddress[0],m_ipaddress[1],m_ipaddress[2],m_ipaddress[3]);
                    m_tcp_state = STATE_SOC_REGISTER;
                    
                }else  //domain name
                {
                    ret = Ql_IpHelper_GetIPByHostName(0, 0, net_info.ip, GetIpByName_callback);
                    if(ret == SOC_SUCCESS)
                    {
                        APP_DEBUG("<--Get ip by hostname successfully.-->\r\n");
                    }
                    else if(ret == SOC_WOULDBLOCK)
                    {
                        APP_DEBUG("<--Waiting for the result of Getting ip by hostname,ret=%d.-->\r\n",ret);
                        //waiting CallBack_getipbyname
                    }
                    else
                    {
                        APP_DEBUG("<--Get ip by hostname failure:ret=%d-->\r\n",ret);
                        if(ret == SOC_BEARER_FAIL)  
                        {
                             m_tcp_state = STATE_GPRS_DEACTIVATE;
                        }
                        else
                        {
                             m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
                        } 
                    }
                }
                break;
            }
            case STATE_SOC_REGISTER://ע������
            {
                ret = Ql_SOC_Register(callback_soc_func, NULL);
                if (SOC_SUCCESS == ret)
                {
                    APP_DEBUG("<--Register socket callback function successfully.-->\r\n");
                    m_tcp_state = STATE_SOC_CREATE;
                }else if (SOC_ALREADY == ret)
                {
                    APP_DEBUG("<--Socket callback function has already been registered,ret=%d.-->\r\n",ret);
                    m_tcp_state = STATE_SOC_CREATE;
                }else
                {
                    APP_DEBUG("<--Register Socket callback function failure,ret=%d.-->\r\n",ret);
                }
                break;
            }
            case STATE_SOC_CREATE://����һ��TCP����
            {
                m_socketid = Ql_SOC_Create(0, SOC_TYPE_TCP);
                if (m_socketid >= 0)
                {
                    APP_DEBUG("<--Create socket id successfully,socketid=%d.-->\r\n",m_socketid);
                    m_tcp_state = STATE_SOC_CONNECT;
                }else
                {
                    APP_DEBUG("<--Create socket id failure,error=%d.-->\r\n",m_socketid);
                }
                break;
            }
            case STATE_SOC_CONNECT://����һ��TCP����
            {
                m_tcp_state = STATE_IDLE;
                ret = Ql_SOC_Connect(m_socketid,(u32) m_ipaddress, net_info.port);
                if(ret == SOC_SUCCESS)
                {
                    APP_DEBUG("<--The socket is already connected.-->\r\n");
                    m_tcp_state = STATE_SOC_CLOSE;//����ر�״̬
					
					ret=Ql_Timer_Stop(TCP_TIMER_ID);//�ر�TCP���ӳ�ʼ����ʱ��
					if(ret <0)
					{
						//����������
						APP_DEBUG("\r\n<--failed!!,CLOSE TCP_TIMER_ID:(%d)fail ,ret = %d -->\r\n",TCP_TIMER_ID,ret);
					}
					else
					{
					 	APP_DEBUG("\r\n<--CloseTCP_TIMER_ID Success! -->\r\n");
					}
					//Net_First_Login();//���ӵ�¼����Ҫ�Ǵ���ע���Ȩ����
                }else if(ret == SOC_WOULDBLOCK)
                {
                      if (!timeout_90S_monitor)//start timeout monitor
                      {
                        Ql_Timer_Start(TIMEOUT_90S_TIMER_ID, TIMEOUT_90S_PERIOD, FALSE);
                        timeout_90S_monitor = TRUE;
                      }
                      APP_DEBUG("<--Waiting for the result of socket connection,ret=%d.-->\r\n",ret);
                      //waiting CallBack_getipbyname
                      
                }else //error
                {
                    APP_DEBUG("<--Socket Connect failure,ret=%d.-->\r\n",ret);
                    APP_DEBUG("<-- Close socket.-->\r\n");
                    Ql_SOC_Close(m_socketid);
                    m_socketid = -1;
                    
                    if(ret == SOC_BEARER_FAIL)  
                    {
                        m_tcp_state = STATE_GPRS_DEACTIVATE;
                    }
                    else
                    {
                        m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
                    }
                }
                break;
            }
            case STATE_IDLE:
            {
				;
                //m_tcp_state = STATE_SOC_SENDING;
                break;
            }  
			/*case STATE_SOC_LISTEN:
            {
                ret = Ql_SOC_Listen(m_socketid, MAXCLIENT_NUM);
                if(ret < 0)
                {
                    m_tcp_state = STATE_SOC_LISTEN;
                    APP_DEBUG("<--Set listen socket failure, ret=%d-->\r\n", ret);
                }else
                {
                    m_tcp_state = STATE_IDLE;
                    APP_DEBUG("<--Set listen socket successfully.-->\r\n");
                }
                break;
            }*/
			case STATE_SOC_CLOSE:
            {
				// �����·״̬????
                break;
            } 
            case STATE_GPRS_DEACTIVATE:
            {
				u8 updateFlg = GetUpdateFlag();
				if(updateFlg)return;
                m_tcp_state = STATE_NW_GET_SIMSTATE;//������tcp��ʼ������
                APP_DEBUG("<--Deactivate GPRS.-->\r\n");
                Ql_GPRS_Deactivate(0);
                break;
            }
            default:
                break;
        }    
    }
}

void Socket_Restart(void)
{
	Ql_SOC_Close(m_socketid);
	Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
	m_tcp_state = STATE_NW_GET_SIMSTATE;//������tcp��ʼ������
}

//GPRS����ɹ��ص�����
void GPRS_Actived_callback(u8 contexId, s32 errCode, void* customParam)
{
    if(errCode == SOC_SUCCESS)
    {
        APP_DEBUG("<--CallBack: active GPRS successfully.-->\r\n");
        m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
    }else
    {
        APP_DEBUG("<--CallBack: active GPRS failure,errCode=%d-->\r\n",errCode);
        m_tcp_state = STATE_GPRS_ACTIVATE;
    }      
}


//������IP��ַת���ص�����
void GetIpByName_callback(u8 contexId, u8 requestId, s32 errCode,  u32 ipAddrCnt, u32* ipAddr)
{
    u8 i=0;
    u8* ipSegment = (u8*)ipAddr;
    
    APP_DEBUG("<-- %s:contexid=%d, requestId=%d,error=%d,num_entry=%d -->\r\n", __func__, contexId, requestId,errCode,ipAddrCnt);
    if (errCode == SOC_SUCCESS)
    {
        APP_DEBUG("<--CallBack: get ip by name successfully.-->\r\n");
        for(i=0;i<ipAddrCnt;i++)
        {
            ipSegment = (u8*)(ipAddr + i);
            APP_DEBUG("<--Entry=%d, ip=%d.%d.%d.%d-->\r\n",i,ipSegment[0],ipSegment[1],ipSegment[2],ipSegment[3]);
        }

        // Fetch the first ip address as the valid IP
        Ql_memcpy(m_ipaddress, ipAddr, 4);
        m_tcp_state = STATE_SOC_REGISTER;
    }
}

//TCP���ӳɹ��ص�����
void socket_connect_callback(s32 socketId, s32 errCode, void* customParam )
{
s32 ret;
static u8 connect_fail_cnt = 0;
u32 csq, bit_err;

    if (errCode == SOC_SUCCESS)
    {
        if (timeout_90S_monitor) //stop timeout monitor
        {
           Ql_Timer_Stop(TIMEOUT_90S_TIMER_ID);
           timeout_90S_monitor = FALSE;
		   APP_DEBUG("<-- Close timeout_90S_monitor...-->\r\n");
        }
		if(0xaa == FCT_ReadSelfCheckFlg())
        {
			APP_DEBUG("\r\nOK:NORMAL NETWORK TEST NTP\r\n");
			PUBLIC_SETBIT(FCT_Test,2);
        }
        APP_DEBUG("<--Callback: socket connect successfully.-->\r\n");
        m_tcp_state = STATE_SOC_CLOSE;
		
		Ql_Timer_Stop(TCP_TIMER_ID);//�ر�TCP���ӳ�ʼ����ʱ��
		if(ret <0)
		{
			//����������
			APP_DEBUG("\r\n<--failed!!,CLOSE TCP_TIMER_ID:(%d)fail ,ret = %d -->\r\n",TCP_TIMER_ID,ret);
		}
		else
		{
			APP_DEBUG("\r\n<--CloseTCP_TIMER_ID Success! -->\r\n");
		}
		connect_fail_cnt = 0;
		Net_First_Login();//���ӵ�¼����Ҫ�Ǵ���ע���Ȩ����
    }else
    {
    	if(0xaa == FCT_ReadSelfCheckFlg())
        {
			APP_DEBUG("\r\nERROR:NETWORK TEST FAILURE NTP!\r\n");
        }
        APP_DEBUG("<--Callback: socket connect failure,(socketId=%d),errCode=%d-->\r\n",socketId,errCode);
        Ql_SOC_Close(socketId);
		Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
        m_tcp_state = STATE_SOC_CREATE;//;STATE_NW_GET_SIMSTATE
        APP_DEBUG("<--connect_fail_cnt=%d-->\r\n",connect_fail_cnt);
		ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);
		
		if(RIL_AT_SUCCESS == RIL_NW_GetSignalQuality((u32*)&csq, (u32*)&bit_err))
		{
			if((csq > 10)&&(csq <= 31))
			{
				if(connect_fail_cnt++ > 100)
				{
					connect_fail_cnt = 0;
					APP_DEBUG("<--100���㶼�����ϻ���������,connect_fail_cnt=%d-->\r\n",connect_fail_cnt);
					Blind_UpdateCirQueueInfo();
					Report_RtcWakeUpOffsetSave();
					Ql_Reset(0);
					//Ql_RIL_SendATCmd("AT+CFUN=1", 9, NULL, NULL, 0);
				}
			}
		}
       
    }
}

//TCP���ӹرջص�����
void socket_close_callback(s32 socketId, s32 errCode, void* customParam )
{
    m_nSentLen  = 0;
	
    ClearTerminalAuthorizationFlag(CHANNEL_DATA_1); 
	Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
	
    if (errCode == SOC_SUCCESS)
    {
        APP_DEBUG("<--CallBack: close socket successfully.-->\r\n"); 
		m_tcp_state = STATE_NW_GET_SIMSTATE;
    }else if(errCode == SOC_BEARER_FAIL)
    {   
        m_tcp_state = STATE_GPRS_DEACTIVATE;
        APP_DEBUG("<--CallBack: close socket failure,(socketId=%d,error_cause=%d)-->\r\n",socketId,errCode); 
    }else
    {
        m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
        APP_DEBUG("<--CallBack: close socket failure,(socketId=%d,error_cause=%d)-->\r\n",socketId,errCode); 
    }
	
}

void socket_accept_callback(s32 listenSocketId, s32 errCode, void* customParam )
{  
}


//TCP�յ���������ݻص����������յ�ƽ̨�����ݵ�ʱ�����øú���
void socket_read_callback(s32 socketId, s32 errCode, void* customParam )
{
    s32 ret;
    if(errCode)
    {
        APP_DEBUG("<--CallBack: socket read failure,(sock=%d,error=%d)-->\r\n",socketId,errCode);
        APP_DEBUG("<-- Close socket.-->\r\n");
        Ql_SOC_Close(socketId);
		Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
        m_socketid = -1;
        if(errCode == SOC_BEARER_FAIL)  
        {
            m_tcp_state = STATE_GPRS_DEACTIVATE;
        }
        else
        {
            m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
        }  
        return;
    }


    Ql_memset(m_recv_buf, 0, RECV_BUFFER_LEN);
    do
    {
        ret = Ql_SOC_Recv(socketId, m_recv_buf, RECV_BUFFER_LEN);

        if((ret < 0) && (ret != -2))
        //if(ret < 0)
        {
            APP_DEBUG("<-- Receive data failure,ret=%d.-->\r\n",ret);
            APP_DEBUG("<-- Close socket.-->\r\n");
            Ql_SOC_Close(socketId); //you can close this socket  
			Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
            m_socketid = -1;
            m_tcp_state = STATE_SOC_CREATE;
            break;
        }
        else if(ret == -2)
        {
            //wait next CallBack_socket_read
            break;
        }
        else if(ret < RECV_BUFFER_LEN)
        {
            APP_DEBUG("<--Receive data from sock(%d),len(%d):%s\r\n",socketId,ret,m_recv_buf);
            break;
        }else if(ret == RECV_BUFFER_LEN)
        {
            APP_DEBUG("<--Receive data from sock(%d),len(%d):%s\r\n",socketId,ret,m_recv_buf);
        }
    }while(1);
	
	if((ret>0))
	{
		RadioProtocol_CheckRecvData(m_recv_buf,ret);
	}
}



//���ͻص���������������һ������û�з�����ɣ�����øú���
void socket_write_callback(s32 socketId, s32 errCode, void* customParam )
{
    s32 ret;

    if(errCode)
    {
        APP_DEBUG("<--CallBack: socket write failure,(sock=%d,error=%d)-->\r\n",socketId,errCode);
        APP_DEBUG("<-- Close socket.-->\r\n");
        Ql_SOC_Close(socketId);
        m_socketid = -1;
        
		Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
		
        if(errCode == SOC_BEARER_FAIL)  
        {
            m_tcp_state = STATE_GPRS_DEACTIVATE;
        }
        else
        {
            m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
        }  
        return;
    }

    do
    {
        ret = Ql_SOC_Send(m_socketid, m_pCurrentPos, m_remain_len);
        APP_DEBUG("<--CallBack: Send data,socketid=%d,number of bytes sent=%d-->\r\n",m_socketid,ret);

        if(ret == m_remain_len)//send compelete
        {
            m_remain_len = 0;
            m_pCurrentPos = NULL;
            m_nSentLen += ret;
            //m_tcp_state = STATE_SOC_ACK;
            break;
         }
         else if((ret < 0) && (ret == SOC_WOULDBLOCK)) 
         {
            //you must wait CallBack_socket_write, then send data;     
            break;
         }
         else if(ret < 0)
         {
              APP_DEBUG("<--Send data failure,ret=%d.-->\r\n",ret);
              APP_DEBUG("<-- Close socket.-->\r\n");
              Ql_SOC_Close(socketId);//error , Ql_SOC_Close
              
			  Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
			  
              m_socketid = -1;

              m_remain_len = 0;
              m_pCurrentPos = NULL; 
              if(ret == SOC_BEARER_FAIL)  
              {
                  m_tcp_state = STATE_GPRS_DEACTIVATE;
              }
              else
              {
                  m_tcp_state = STATE_GPRS_GET_DNSADDRESS;
              }       
              break;
			  
        }
        else if(ret < m_remain_len)//continue send, do not send all data
        {
            m_remain_len -= ret;
            m_pCurrentPos += ret; 
            m_nSentLen += ret;
        }
     }while(1);
}


void GPRS_Deactived_callback(u8 contextId, s32 errCode, void* customParam )
{
	u8 updateFlg = GetUpdateFlag();
	if(updateFlg)return;
	ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);  
    if (errCode == SOC_SUCCESS)
    {
        APP_DEBUG("<--CallBack: deactived GPRS successfully.-->\r\n"); 
		//if(ReadSleepFlag()!=2)
			{
				ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);  
				Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
			    m_tcp_state = STATE_NW_GET_SIMSTATE;
			}

    }else
    {
        APP_DEBUG("<--CallBack: deactived GPRS failure,(contexid=%d,error_cause=%d)-->\r\n",contextId,errCode); 
    }
}

void GsmDrop_callback(void)
{
	u8 updateFlg = GetUpdateFlag();
	if(updateFlg)return;
	ClearTerminalAuthorizationFlag(CHANNEL_DATA_1); 
	if(m_tcp_state == STATE_SOC_CLOSE)
		{
      		 APP_DEBUG("\r\n<--GsmDrop CallBack_GsmDrop-->\r\n"); 
			ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);  
			Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);//����TCP���ӳ�ʼ����ʱ��
			m_tcp_state = STATE_NW_GET_SIMSTATE;
		}

}
extern s32 m_pwrOnReason;
//////////////////////////////////////////////////////////////////////////////////////
//��Ȩ���
//////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief  ���ӵ�¼����Ҫ�Ǵ���ע���Ȩ����
  * @param  type: ���� 
            state: ��һ��ִ�е�״̬
  * @retval None
  */
void Net_First_Login( void)
{
	
    ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);  

	SetLoginAuthorizationState(LOGIN_READ_AUTHORIZATION_CODE);
	//����ע���Ȩ����
	//SetTimerTask(TIME_LINK1_LOGIN, SYSTICK_1SECOND); 
	SetTerminalAuthorizationFlag(CHANNEL_DATA_1);
	//��ʾ��Ȩ�ɹ�
	#ifndef BLIND_SAVE_BY_QUEUE
	Blind_StartLink1Report();//����ä������
	#endif
	RIL_GetPowerSupply(&battery_capacity, &battery_voltage);
	if(RTCPWRON != m_pwrOnReason)//����������ӻ��Ѿ��ϱ�һ��λ����Ϣ
	{
		Report_UploadPositionInfo();
		Report_ClearReportTimeCount();
		Report_UpdatePram();
		//��������
		ST_Time dateTime;
		u64 mkTime;
		u32 interval,temp;
		interval = Report_GetDefaultReportInterval();//��ȡ�ϱ����
		if(interval >= 30*60)
		{
			Ql_GetLocalTime(&dateTime);//��ȡʵʱʱ��
			mkTime = Ql_Mktime(&dateTime);			
			temp = mkTime%interval;//�����´���Ҫ�ϱ���ʱ��
			if(temp > 10*60)//����10���Ӿ���������˯���ػ�
			{
				mkTime += temp;
				mkTime -= 60;//����������ǰ60s����
				Ql_MKTime2CalendarTime(mkTime, &dateTime);
				dateTime.year -= 2000;
				RIL_Alarm_Create(&dateTime,0);
				//�ػ�ǰιһ�ι�
				GpioOutInit(WATCHDOG,PINLEVEL_HIGH);//���ó����
				HARD_FEED_DOG();
				Ql_Sleep(200);
				Ql_PowerDown(1);//�ػ�ģʽ
			}
		}
	}
	Report_SetReportStatus(ReportStatus_Init);
	APP_DEBUG("\r\n<--!!!���ӳɹ��ص�!!!!-->\r\n");
#ifdef BLIND_SAVE_BY_QUEUE
	Blind_GiveSemaphore();
	APP_DEBUG("\r\n<--!!!Blind_GiveSemaphore!!!!-->\r\n");
#endif
}

//��ʼ��TCP��ض�ʱ��
void init_tcp(void)
{
	Net_First_Value(MAIN_SERVER);//��ʼ��TCP�˿���ز���
    //register & start timer 
    Ql_Timer_Register(TCP_TIMER_ID, Timer_Callback, NULL);
    Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);

    Ql_Timer_Register(TIMEOUT_90S_TIMER_ID, Timer_Callback, NULL);
	
    timeout_90S_monitor = FALSE;

}

//��Ȩ����
FunctionalState  Link1Login_TimeTask(void)
{
  u8  ucTmp[30];
  static u8 errorcount = 0;	
#ifdef MODEM_APP_SECOND_H
  if(CHANNEL_DATA_1 == (CHANNEL_DATA_1&GetTerminalAuthorizationFlag()))
#else
    if(GetTerminalAuthorizationFlag())
#endif
    {
      APP_DEBUG("<--!!!��Ȩ�ɹ��ر�����!!!!-->\r\n");
      Link1LoginAuthorizationState = LOGIN_CLOSE_TASK;
    }
  //////////////
  switch(Link1LoginAuthorizationState)
  {
  case LOGIN_READ_AUTHORIZATION_CODE://��ȡ��Ȩ��
    {
      Link1LoginCount = 0;
      errorcount = 0;
      Link1AuthorizationCount = 0;
      Link1LoginAuthorizationLastState = 0;
      Link1LoginAuthorizationDelayCount = 0;
      if(0 == EepromPram_ReadPram(E2_LOGINGPRS_PASSWORD_ID, ucTmp))//ʧ��
      {
        Link1LoginAuthorizationState = LOGIN_SEND_LOGIN_CMD;
		APP_DEBUG("<-- ��ȡ��Ȩ��ʧ��-->\r\n");
      }
      else//�ɹ�
      {
        Link1LoginAuthorizationState = LOGIN_SEND_AUTHORIZATION_CMD;
		APP_DEBUG("<-- ��ȡ��Ȩ��ɹ�����һ�����ͼ�Ȩ���� -->\r\n");
      }
      break;
    }
  case LOGIN_SEND_LOGIN_CMD://����ע������
    {
		APP_DEBUG("<-- ����ע������ -->\r\n");
      if(ACK_OK == RadioProtocol_TerminalRegister())
      {
      
	  APP_DEBUG("<-- ����ע������ɹ�-->\r\n");
       // Net_Other_Printf(PRTF_REGISTER);
        Link1LoginAuthorizationDelayCount = 0;
        Link1LoginAuthorizationDelayTime = 30;//ע���Ȩ��ʱʱ��
        Link1LoginAuthorizationLastState = LOGIN_SEND_LOGIN_CMD;//��һ��״̬
        Link1LoginCount++;
        if(Link1LoginCount >= 3)
        {
          Link1LoginAuthorizationState = LOGIN_CLOSE_GPRS;
        }
        else
        {
          Link1LoginAuthorizationState = LOGIN_DELAY;
        }
        errorcount = 0;
      }
      else
      {
      
	  APP_DEBUG("<-- ����ע������ʧ��-->\r\n");
        errorcount++;
        if(errorcount >= 200)
        {
          Link1LoginAuthorizationState = LOGIN_CLOSE_TASK;
        }
      }
      break;
    }
  case LOGIN_SEND_AUTHORIZATION_CMD://���ͼ�Ȩ����
    {
	APP_DEBUG("<-- ���ͼ�Ȩ����-->\r\n");	
      if(ACK_OK == RadioProtocol_TerminalQualify())
      {
      
	    APP_DEBUG("<-- ���ͼ�Ȩ����ɹ�-->\r\n");
        //Net_Other_Printf(PRTF_AUTHORIZATION);
        Link1LoginAuthorizationDelayCount = 0;//ע��������ʱ����������
        Link1LoginAuthorizationDelayTime = 30;//ע���Ȩ��ʱʱ��,��1����������ʱ
        Link1LoginAuthorizationLastState = LOGIN_SEND_AUTHORIZATION_CMD;//��һ��״̬
        Link1AuthorizationCount++;
        if(Link1AuthorizationCount >= 3)
        {
          Link1LoginAuthorizationState = LOGIN_SEND_LOGIN_CMD;
        }
        else
        {
          Link1LoginAuthorizationState = LOGIN_DELAY;
        }
        errorcount = 0;
      }
      else
      {
      
	  	APP_DEBUG("<-- ���ͼ�Ȩ����ʧ��-->\r\n");
        errorcount++;
        if(errorcount >= 200)
        {
          Link1LoginAuthorizationState = LOGIN_CLOSE_TASK;
        }
      }
      break;
    }
  case LOGIN_CLOSE_GPRS://�Ͽ�����
    {
	  APP_DEBUG("<-- ��Ȩʧ�ܶϿ����� -->\r\n");
      Net_First_Close();
      Link1ManualCloseFlag = 1;
      ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);//��Ȩ��־��0
      ClearTerminalAuthorizationCode(CHANNEL_DATA_1);
      Link1LoginAuthorizationDelayCount = 0;
      Link1LoginAuthorizationDelayTime = 10*60;//ע���Ȩ��ʱʱ�䣬���������ʱ10����
      Link1LoginAuthorizationLastState = LOGIN_RECONNECT_GPRS;
      Link1LoginAuthorizationState = LOGIN_DELAY;	
      break;
      
    }
  case LOGIN_DELAY://��ʱ
    {
      Link1LoginAuthorizationDelayCount++;
      if(Link1LoginAuthorizationDelayCount >= Link1LoginAuthorizationDelayTime)
      {
        Link1LoginAuthorizationDelayCount = 0;
        Link1LoginAuthorizationState = Link1LoginAuthorizationLastState;
      }
      break;
    }
  case LOGIN_RECONNECT_GPRS://��������
    {
	APP_DEBUG("<-- �������� -->\r\n");
      Link1LoginCount = 0;
      errorcount = 0;
      Link1AuthorizationCount = 0;
      Link1LoginAuthorizationLastState = 0;
      Link1LoginAuthorizationState = 0;
      Link1LoginAuthorizationDelayCount = 0; 
      Net_First_Open();
      Link1ManualCloseFlag = 0;
      Link1LoginAuthorizationState = LOGIN_CLOSE_TASK;
      break;
    }
  case LOGIN_CLOSE_TASK://�ر�����
    {
	  /*
      if(1 == Link1ManualCloseFlag)
      {
        Net_First_Open();
        Link1ManualCloseFlag = 0;
      }
      */
      Link1LoginCount = 0;
      errorcount = 0;
      Link1AuthorizationCount = 0;
      Link1LoginAuthorizationLastState = 0;
      Link1LoginAuthorizationState = 0;
      Link1LoginAuthorizationDelayCount = 0; 
	  //ClrTimerTask(Msg_Link1Login_TimeTask);
	  return DISABLE;
      break;
    }
  default:
    {
      Link1LoginAuthorizationState = LOGIN_CLOSE_TASK;
    }
    break;
  }
  return ENABLE;
}

/*********************************************************************
//��������	:ClearTerminalAuthorizationCode(u8 channel)
//����		:���ĳ��ͨ���ļ�Ȩ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:bit0:��ȱ��Ԥ��;bit1:��ʾCHANNEL_DATA_1��bit2:CHANNEL_DATA_2
//          :channel��ֵ����Ϊ��������ͨ��ֵ���������ǵ����
*********************************************************************/
void ClearTerminalAuthorizationCode(u8 channel)
{   
/*
    #ifdef MODEM_APP_SECOND_H
    if(CHANNEL_DATA_1 == (channel&CHANNEL_DATA_1))
    {
         System_ClearLoginGprsPassword();
    }
    //////////////
    if(CHANNEL_DATA_2 == (channel&CHANNEL_DATA_2))
    {
        for(i=0; i<20; i++)
        {
            E2prom_WriteByte(E2_SECOND_LOGINGPRS_PASSWORD_ID_ADDR+i,0xff);
        }
    }
    #else
    */
    System_ClearLoginGprsPassword();
    //#endif
}

/*********************************************************************
//��������	:System_ClearLoginGprsPassword
//����		:������Ȩ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void System_ClearLoginGprsPassword(void)
{
    unsigned char buffer[20];
    memset(buffer,0xff,20);
    E2prom_WriteBytes(PRAM_FILE_NAME,E2_LOGINGPRS_PASSWORD_ID_ADDR,buffer,20);
}
/*********************************************************************
//��������	:GetTerminalAuthorizationFlag(void)
//����		:��ȡ��Ȩ��־��ֵ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
u8 GetTerminalAuthorizationFlag(void)
{
	return 	TerminalAuthorizationFlag;
}
/*********************************************************************
//��������	:SetTerminalAuthorizationFlag(void)
//����		:��λ��Ȩ��־
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
void SetTerminalAuthorizationFlag(u8 channel)
{

    #ifdef MODEM_APP_SECOND_H
    if(CHANNEL_DATA_1 == (channel&CHANNEL_DATA_1))
    {
        TerminalAuthorizationFlag = TerminalAuthorizationFlag | CHANNEL_DATA_1;
    }
    
    if(CHANNEL_DATA_2 == (channel&CHANNEL_DATA_2))
    {
        TerminalAuthorizationFlag = TerminalAuthorizationFlag | CHANNEL_DATA_2;
    }
    #else
    TerminalAuthorizationFlag = 1;
   // g_stModemStatus.curOffLineTimeCnt = 0;
    #endif
	
}
/*********************************************************************
//��������	:ClearTerminalAuthorizationFlag(void)
//����		:�����Ȩ��־
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:
*********************************************************************/
void ClearTerminalAuthorizationFlag(u8 channel)
{
    #ifdef MODEM_APP_SECOND_H
    if(CHANNEL_DATA_1 == (channel&CHANNEL_DATA_1))
    {
        TerminalAuthorizationFlag = TerminalAuthorizationFlag & 0xfd;
    }
    if(CHANNEL_DATA_2 == (channel&CHANNEL_DATA_2))
    {
        TerminalAuthorizationFlag = TerminalAuthorizationFlag & 0xfb;
    }
    #else
    TerminalAuthorizationFlag = 0;
    #endif
}

/*******************************************************************************
* Function Name  : SetLoginAuthorizationState
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetLoginAuthorizationState(u8 State)
{
    Link1LoginCount                 = 0;
    Link1AuthorizationCount         = 0;
    Link1LoginAuthorizationLastState= 0;
    Link1LoginAuthorizationState    = State;        
}

/**
  * @brief  ���ͻ������ݵ�����
  * @param    channel:����ͨ��
  * @param    srcdat :ָ��������
  * @param    srclen :�������ݳ���
  * @retval ���ط��ͽ��
  *                              0 :ʧ��  
  *                     ����srclen :�ɹ�
  */
u16 Communication_SndMsg2Svr(u8 channel, u8 * srcdat, u16 srclen)
{
    u16 len = 0;
	s32 ret;
	u64  ackNum = 0;
    if(channel == CHANNEL_SMS)
    {
        #ifdef EYE_USE_SMS
        len = srclen;
        //Modem_Api_SmsSend(SMS_PDU,NULL,srcdat,srclen);
        #endif
    }
//    #ifdef MODEM_APP_SECOND_H
//    else if(channel == CHANNEL_DATA_2)
//    {
//       	len = Net_Second_Send(srcdat,srclen);
//	  }
//    #endif
    else 
	if (m_socketid >= 0)
	///////////////////��һͨ��
    {
    
         ret = Ql_SOC_Send(m_socketid, (u8*)srcdat, srclen);
        if (ret == srclen)
        {
            APP_DEBUG("<-- Send socket data successfully. --> \r\n");
        }else{
            APP_DEBUG("<-- Fail to send socket data. --> \r\n");
            Ql_SOC_Close(m_socketid);
			m_socketid=-1;
            return 0;
        }
/*
        // Check ACK number
        do 
        {
            ret = Ql_SOC_GetAckNumber(m_SocketId, &ackNum);
            APP_DEBUG("<-- Current ACK Number:%llu/%d --> \r\n", ackNum, srclen);
            Ql_Sleep(500);
        } while (ackNum != srclen);
        */
        //APP_DEBUG("<-- Server has received all data --> \r\n");
        
    	return len=ret;
    }
	return 0;
}


/*
  * @brief  ������
  * @param  None
  * @retval None
  */
void Net_First_Open(void)
{

}
/*
  * @brief  �ر�����
  * @param  None
  * @retval None
  */
void Net_First_Close(void)
{
    s32 ret;
    //Close socket
    ret = Ql_SOC_Close(m_socketid);
    APP_DEBUG("<-- Close socket[%d], cause=%d --> \r\n", m_socketid, ret);
    //Deactivate GPRS
    APP_DEBUG("<-- Deactivating GPRS... -->\r\n");
	ret = Ql_GPRS_Deactivate(0);
    //ret = Ql_GPRS_DeactivateEx(pdpCntxtId, TRUE);
    APP_DEBUG("<-- Deactivated GPRS, cause=%d -->\r\n\r\n", ret);
}

/**
  * @brief  �ӻ�ȡ����
  * @param  type: ��������
  * @retval ���ز����ṹ 1->�ɹ�  0->�Ƿ�����
  */
u8 Net_First_Value(ServerType type )
{
    u8  tab[50]={0};
    u8  len;
    u32 id_ip;
    u16 temp;

	//////////////////////////////////////////////////////////////////��Ȩ��־��
    ClearTerminalAuthorizationFlag(CHANNEL_DATA_1);  
	
	ConnectInfo.curIpType=type;
	ConnectInfo.ipConnectFailureCnt=0;
	
    ////////////////////
    if(type==MAIN_SERVER)
    	{
	    len = EepromPram_ReadPram(E2_MAIN_SERVER_IP_ID, tab);    
	    if((0==len)||(len >= 30))
			{
			APP_DEBUG("<-- Net_First_Value Fail len=%d -->\r\n",len);
			return 0;
			}
	    memcpy(net_info.ip,tab,len); 
		net_info.ip[len]='\0';
		APP_DEBUG("<-- E2_MAIN_SERVER_IP_ID= %slen %d -->\r\n", net_info.ip,len);
    	}
	else
		{
	    len = EepromPram_ReadPram(E2_BACKUP_SERVER_IP_ID, tab);    
	    if((0==len)||(len >= 30))
			{
			APP_DEBUG("<-- Net_First_Value Fail len=%d -->\r\n",len);
			return 0;
			}
	    memcpy(net_info.ip,tab,len); 
		net_info.ip[len]='\0';
		APP_DEBUG("<-- E2_BACKUP_SERVER_IP_ID= %slen %d -->\r\n", net_info.ip,len);
		}
    //////////////////
    len = EepromPram_ReadPram(E2_MAIN_SERVER_TCP_PORT_ID, tab);    
    if((0==len)||(len >= 5))return 0;
    /////////////////////
    temp = Public_ConvertBufferToLong(tab); 
	/*
    #if 0
    if(temp)//modify by joneming
    {
        strcpy((char*)pstip->mode,"TCP");//ģʽ
    }
    else
    { 
        if(EepromPram_ReadPram(E2_MAIN_SERVER_UDP_PORT_ID, tab))
        {
            temp = Public_ConvertBufferToLong(tab);
            if(temp)
            {
                strcpy((char*)pstip->mode,"UDP");//ģʽ
            }
        }
    }
    #else
    strcpy((char*)pstip->mode,"TCP");//ģʽ
    #endif
    */
    //Public_itoaEx(net_info->port,temp);
    net_info.port=(u32)temp;
    ////////////////////
    return 1;//��������
}


