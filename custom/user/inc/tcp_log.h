#ifndef __TCP_LOG__H_
#define __TCP_LOG__H_


#define TCP_TIMER_PERIOD     800
#define TIMEOUT_90S_PERIOD   90000

#define LOGIN_TIMER_ID_PERIOD 2000

/**************************************
	TCP��������״̬ 
*****************************************/
typedef enum{
    STATE_NW_GET_SIMSTATE,
    STATE_NW_QUERY_STATE,
    STATE_GPRS_REGISTER,
    STATE_GPRS_CONFIG,
    STATE_GPRS_ACTIVATE,
    STATE_GPRS_ACTIVATING,
    STATE_GPRS_GET_DNSADDRESS,
    STATE_GPRS_GET_LOCALIP,
    STATE_CHACK_SRVADDR,
    STATE_SOC_REGISTER,
    STATE_SOC_CREATE,
    STATE_SOC_CONNECT,
    STATE_SOC_CONNECTING,
    STATE_IDLE,
    STATE_SOC_LISTEN,
    STATE_SOC_CLOSE,
    STATE_GPRS_DEACTIVATE,
    STATE_TOTAL_NUM
}Enum_TCPSTATE;


//ע���Ȩ����״̬
typedef enum {
  LOGIN_READ_AUTHORIZATION_CODE=0,//��ȡ��Ȩ�룬ÿ��������������Ǵ�״̬
  LOGIN_SEND_LOGIN_CMD,//����ע������
  LOGIN_SEND_AUTHORIZATION_CMD,//���ͼ�Ȩ����
  LOGIN_CLOSE_GPRS,//�Ͽ�����
  LOGIN_DELAY,//��ʱ
  LOGIN_RECONNECT_GPRS,//��������
  LOGIN_CLOSE_TASK,//�ر�����
} LOGIN_STATE;  



typedef enum
{
	MAIN_SERVER=0,//��������
	BACK_SERVER,//���ݷ�����
}ServerType;

typedef struct
{
	ServerType curIpType;
	u8 ipConnectFailureCnt;
}ServerContInfo;

/**
  * @brief  ���ӵ�¼����Ҫ�Ǵ���ע���Ȩ����
  * @param  type: ���� 
            state: ��һ��ִ�е�״̬
  * @retval None
  */
void Net_First_Login(void);


FunctionalState  Link1Login_TimeTask(void);



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
void ClearTerminalAuthorizationCode(u8 channel);

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
void System_ClearLoginGprsPassword(void);

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
u8 GetTerminalAuthorizationFlag(void);


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
void SetTerminalAuthorizationFlag(u8 channel);


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
void ClearTerminalAuthorizationFlag(u8 channel);

/*******************************************************************************
* Function Name  : SetLoginAuthorizationState
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetLoginAuthorizationState(u8 State);

/**
  * @brief  �ӻ�ȡ����
  * @param  type: ��������
  * @retval ���ز����ṹ 1->�ɹ�  0->�Ƿ�����
  */
u8  Net_First_Value(ServerType type );


void GsmDrop_callback(void);


/*
  * @brief  �ر�����
  * @param  None
  * @retval None
  */
void Net_First_Close(void);

void Socket_Restart(void);

#endif

