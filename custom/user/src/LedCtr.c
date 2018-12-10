#include "include.h"
#include "report.h"
#include "FCT_SelfCheck.h"

static LED_CTRL_DEF ledCtrlHd;
static u16  LowVolThreshold = 0;//�͵�ѹ��ֵ

u32 battery_capacity;
u32 battery_voltage;

static u16 uart_busy_cnt = 1200;//����������120s������ͨѶ,������ڻ,���ֵά��1200

extern u8	TerminalAuthorizationFlag;//�ն˼�Ȩ��־,��Ȩ�ɹ�Ϊ1,ʧ��Ϊ0
/*
��������1�룬��2�룩��ʾ��ع��磻
��������0.1�룬��2.9�룩��ʾ��繩��
˵������Դ�ƿ���ʡ����
*/
void PowerLedFlashCtr(void)
{
	static u8 PowerLedCnt=0;
	PowerLedCnt++;
	if( Io_ReadAlarmBit( ALARM_BIT_POWER_SHUT))//�������˵���ǵ�ع���
		{
			if(PowerLedCnt<=5)
				{
					POWER_LED_ON();
				}
				else if(PowerLedCnt<=20)
				{
					POWER_LED_OFF();
				}
				else
				{
				PowerLedCnt=0;
				}
				
		}
		else
		{
			if(PowerLedCnt<=1)
				{
					POWER_LED_ON();
				}
				else if(PowerLedCnt<=29)
				{
					POWER_LED_OFF();
				}
				else
				{
				PowerLedCnt=0;
				}
		}

}
/*
��������1�룬��2�룩��ʾGPS����λ��
��������0.1�룬��2.9�룩��ʾGPS��λ;
��������0.1�룬��9.9�룩����ʾ����״̬��
��������ʾ���Ƕ�λģ����ϡ�
*/
void GpsLedFlashCtr(void)
{
	static u8 GpsLedCnt=0;
	GpsLedCnt++;
	if((1 == Io_ReadAlarmBit(ALARM_BIT_ANT_SHUT))
	 ||(1 == Io_ReadAlarmBit(ALARM_BIT_ANT_SHORT))
	 ||(1 == Io_ReadAlarmBit(ALARM_BIT_GNSS_FAULT)))
		{
			//APP_DEBUG("\r\n<-- GNSS ���� -->\r\n")
			GPS_LED_OFF();
		}
		else if(Io_ReadIoStatusBit(IO_STATUS_BIT_SLEEP)==1)
		{
			//APP_DEBUG("\r\n<-- ���� -->\r\n")
			if(GpsLedCnt<=1)
			{
				GPS_LED_ON();
			}
			else if(GpsLedCnt<=99)
			{
				GPS_LED_OFF();
			}
			else
			{
			GpsLedCnt=0;
			}
		}
		else if(0 == Io_ReadStatusBit(STATUS_BIT_NAVIGATION))
		{
			//APP_DEBUG("\r\n<-- ������ -->\r\n")
			if(GpsLedCnt<=5)
			{
				GPS_LED_ON();
			}
			else if(GpsLedCnt<=20)
			{
				GPS_LED_OFF();
			}
			else
			{
			GpsLedCnt=0;
			}
		}
		else //if(1 == Io_ReadStatusBit(STATUS_BIT_NAVIGATION))
		{
			//APP_DEBUG("\r\n<-- ���� -->\r\n")
			if(GpsLedCnt<=1)
			{
				GPS_LED_ON();
			}
			else if(GpsLedCnt<=29)
			{
				GPS_LED_OFF();
			}
			else
			{
			GpsLedCnt=0;
			}
		}
}
/*
��������0.1�룬��2.9�룩��ʾGPRS�������磻
��������1�룬��2�룩��ʾGSM��ʼ����
������ʾû��GSM�źš�
*/
void GprsLedFlashCtr(void)
{
	static u8  NetLedCnt=0;
	NetLedCnt++;
	if(TerminalAuthorizationFlag!=0)
	{
		if(NetLedCnt<=1)
		{
			GPRS_LED_ON();
		}
		else if(NetLedCnt<=29)
		{
			GPRS_LED_OFF();
		}
		else
		{
		NetLedCnt=0;
		}	
	}
	else
	{
		if(NetLedCnt<=10)
		{
			GPRS_LED_ON();
		}
		else if(NetLedCnt<=20)
		{
			GPRS_LED_OFF();
		}
		else
		{
		NetLedCnt=0;
		}	
	}
}

u16 uart_get_busy_status(void)
{
	return uart_busy_cnt;
}
void uart_busy_time_reduce(void)
{
	if((uart_busy_cnt)&&(uart_busy_cnt != 0xffff))
		uart_busy_cnt--;
}
void uart_set_default_mode(void)
{
	uart_busy_cnt = 1200;
}
void uart_set_idle_mode(void)
{
	uart_busy_cnt = 0xffff;
}

//led״̬�仯
void Led_Status_Change(u8 sta)
{
	ledCtrlHd.state = sta;
}
//LED״̬����
void LedControlCallback(u8 sta)
{
#if 0
	uart_busy_time_reduce();
	if(0 == uart_get_busy_status())
	{
		uart_set_idle_mode();
		{
			APP_DEBUG("\r\n...����˯��ģʽ....\r\n");
			GPRS_LED_OFF();
			Ql_SleepEnable();//����˯��ģʽ
		}
		return;
	}	
	if(0xffff == uart_get_busy_status())return;
	//APP_DEBUG("\r\n...˯��ģʽ������....%d\r\n",uart_busy_cnt);
#endif	
	switch(sta)
	{
		case LED_STATUS_POWER_ON:
			 LED_STATUS_POWER_NORMAL:
		{
 			GprsLedFlashCtr();
		}break;
		case LED_STATUS_POWER_DOWN:
		{
			GPRS_LED_OFF();
		}break;
		case LED_STATUS_POWER_LOW:
		{
			GPRS_LED_OFF();
		}break;
		case LED_STATUS_POWER_CHARGE:
		{
			GPRS_LED_ON();
		}break;
		default:break;
	}
}

void Led_Init(void)
{
	ledCtrlHd.state = LED_STATUS_POWER_NORMAL;
	ledCtrlHd.ledCtrlCB = LedControlCallback;
}

/*********************************************************************
//��������	:LowVol_UpdatePram(void)
//����		:���µ͵�ѹ��ֵ
//����		:�޲���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void LowVol_UpdatePram(void)
{
	u8 Buffer[5] = {0};
	u8 PramLen;
	PramLen = EepromPram_ReadPram(E2_LOW_VOL_THRESHOLD_ID,Buffer);    
	if(E2_LOW_VOL_THRESHOLD_ID_LEN == PramLen)
	{	
        LowVolThreshold = Public_ConvertBufferToShort(Buffer);
        /////////////////
        if(LowVolThreshold<3300)LowVolThreshold=3300;//��ֹ��������
        if(LowVolThreshold>3700)LowVolThreshold=3700;//
	}
	else
	{
		LowVolThreshold = 3500;//Ĭ��ֵ,ʵ��Ӧ��ʹ��
	}
}

//��ص�ѹֵС����ֵ��Ӳ���ػ�
void WatchLowVol(void)
{
	static u8 lowVolCnt = 0;
	static u8 csqCnt = 0;
	u32 temp,bit_err;
	if(battery_voltage<LowVolThreshold)
	{
		if(lowVolCnt++ > 5)
		{
			APP_DEBUG("\r\n<-- ��ѹֵ������ֵӲ���ػ� ��ǰ��ѹ:%d  ��ѹ��ֵ:%d-- >\r\n",battery_voltage,LowVolThreshold);
			//����ػ�֮ǰдä���ڵ���Ϣ��Flash
			Blind_UpdateCirQueueInfo();
			REMOTE_SHUTDOWN();
		}
	}
	else
	{
		lowVolCnt = 0;
	}
	if(csqCnt++ > 40)
	{csqCnt = 0;
		RIL_NW_GetSignalQuality((u32*)&temp, (u32*)&bit_err);
		//if(0xaa == FCT_ReadSelfCheckFlg())
		{
			APP_DEBUG("CSQ:%d\r\n",temp);
			APP_DEBUG("CELLVOLTAGE:%d\r\n",battery_voltage);
		}
	}
}

void proc_ledctr_task(s32 taskid)
{
	static u8 ResetPinPullHighFlg = 0;
	static u8 RstPinCnt = 0;
	static u8 LowVolCheck = 0;
	static u8 LowVolCnt = 0;
	static u8 LowVolResumeCnt = 0;
	
	while(1)
	{
		
		Ql_Sleep(100);
		
		if(ProjectInitFinshFlag==1)
		{
			if(0 == ResetPinPullHighFlg)
			{
				RstPinCnt++;
				if(RstPinCnt > 35)
				{
					ResetPinPullHighFlg = 1;
					RESET_KEY_PIN_HIGH();//Reset������
					APP_DEBUG("\r\nreset������........\r\n");
				}
			}
			
			//PowerLedFlashCtr();
			//GpsLedFlashCtr();
			ledCtrlHd.ledCtrlCB(ledCtrlHd.state);
			ReadTime();
			
			//���������в�����ѹ
			
			if((0 == GetUpdateFlag())&&(ReportStatus_Off != Report_GetReportStatus()))
			{
				if(LowVolCheck++ >= 10)//1sһ��
				{
					LowVolCheck = 0;
					RIL_GetPowerSupply(&battery_capacity, &battery_voltage);
					if(battery_voltage<3700)//С��3700mv
					{
						LowVolResumeCnt = 0;
						if(LowVolCnt++ > 10)
						{
							LowVolCnt = 10;
							Io_WriteSelfDefine2Bit(DEFINE_BIT_18, SET);
						}
					}
					else
					{
						LowVolCnt = 0;
						if(LowVolResumeCnt++ > 10)
						{
							LowVolResumeCnt = 10;
							Io_WriteSelfDefine2Bit(DEFINE_BIT_18, RESET);
						}
					}
					WatchLowVol();//����ѹ					
				}
			}
			//POWER_LED_ON();
			//Ql_Sleep(100);//��ʱһ��	
			//POWER_LED_OFF();
		}
//		SoftWdg_Feed(BIT4);
	}
}


