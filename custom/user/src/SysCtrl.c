#include "include.h"
#include "report.h"
#include "heartbeat.h"

//ϵͳ�߼���������
void proc_sysCtrl_task(s32 taskId)
{
    ST_MSG msg;
	FunctionalState ctr=ENABLE;
	u32 ttt;
	while(1)
	{
	
    Ql_OS_GetMessage(&msg);
    switch (msg.message)
	{
	    case MSG_ID_TIME_USER_FUNCTION:
		    {
				ttt=msg.param1;
			switch(msg.param1)
					{
						case Msg_Gps_TimeTask://GPS���ƴ���
							{
								//ctr=Gps_TimeTask();
							}
							break;
						case Msg_GpsMile_TimeTask://GPS��̼���
							{
								//ctr=GpsMile_TimeTask();
							}
							break;
						case Msg_Sleep_TimeTask://����
							{
								//ctr=Sleep_TimeTask();
							}
							break;
						case Heartbeat_TimeTask://����
							{
								HeartBeat_TimeTask();
							}
							break;
						case Msg_Link1Login_TimeTask://�ն�ע���Ȩ
						 	{
								ctr=Link1Login_TimeTask();
							}
						 	break;

						case Msg_Io_TimeTask://IO ״̬���
							{
								ctr=Io_TimeTask();	
							}
							break;
						case Msg_DelayTrigTimeTask://��ʱ����
							{
								ctr=DelayTrigTimeTask();
							}
							break;
						case Msg_SpeedMonitor_TimeTask://���ټ��
							{
								//ctr=SpeedMonitor_TimeTask();
							}
							break;

						 case Msg_ControlOil_TimeTask://����·
						 	{
								//ctr=ControlOil_TimeTask();
							}
						    break;
						 case Msg_Corner_TimeTask://�յ㲹��
					 		{
							    //ctr=Corner_TimeTask();
							}
						 	break;
						 case Msg_AreaManage_TimeTask://����
						 	{
						 		//ctr=AreaManage_TimeTask();
						 	}
						 	break;
						default:
							break;
					}
					if((ctr==DISABLE)&&(0<=ttt<=MAX_TIMETASK))
					{
						ClrTimerTask(ttt);
					}
			}
			break;
		case MSG_ID_EV_USER_FUNCTION:
			{
			switch(msg.param1)
					{
						case MSG_Gps_EvTask:
							{
								//Gps_EvTask();
								//APP_DEBUG("<-- Proc_sysCtrl_task Gps_EvTask-->\r\n");
							}
							break;
						case MSG_Report_EvTask:
							{
								//Report_UploadPositionInfo();//�ϱ�һ��λ����Ϣ
							}
							break;
					}
			}
        default:
            break;
	}
//		SoftWdg_Feed(BIT3);
	}
}



