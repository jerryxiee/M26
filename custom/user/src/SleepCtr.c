#include"include.h"

static u16 AccOnCount = 0;
static u16 AccOffCount = 0;

//ÿ���ϵ������ȡ�����߱�־
void SleepInit(void)
{
	u8 tmp=1;
	FRAM_BufferWrite(FRAM_SLEEP_FLAG_ADDR,&tmp,1); 
}
//��ȡ���߱�־
u8 ReadSleepFlag()
{
	u8 tmp=0;
	FRAM_BufferRead(&tmp,FRAM_SLEEP_FLAG_LEN,FRAM_SLEEP_FLAG_ADDR);
	APP_DEBUG("\r\n<--Read SLEEP Flag=%d-->\r\n",tmp);
	return tmp;
}

//д�����߱�־
void SetSleepFlag(u8 flag)
{
	u8 tmp[1];
	
	tmp[0]=flag;

	FRAM_BufferWrite(FRAM_SLEEP_FLAG_ADDR,tmp,1); 
	
	FRAM_BufferRead(tmp,FRAM_SLEEP_FLAG_LEN,FRAM_SLEEP_FLAG_ADDR);
	
	APP_DEBUG("\r\n<--Set SLEEP Flag=%d-->\r\n",tmp[0]);
	
}
/*********************************************************************
//��������	:Sleep_TimeTask(void)
//����		:���߶�ʱ����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:GNSS���أ�������GPSS_APP�У����ڸĺ���
*********************************************************************/
FunctionalState Sleep_TimeTask(void)
{
    u8 Acc = 0;
	static u16 EnterSleepCount=0;
    Acc = Io_ReadStatusBit(STATUS_BIT_ACC);
	//APP_DEBUG("\r\n<--Sleep Cont   AccOffCount=%d,AccOnCount=%d -->\r\n",AccOffCount,AccOnCount);
    if(1 == Acc)
    {
        AccOffCount = 0;
        if(AccOnCount<100)AccOnCount++;
        if(1 == AccOnCount)
        {
            Io_WriteIoStatusBit(IO_STATUS_BIT_SLEEP, RESET);
        	Ql_SleepDisable();
			APP_DEBUG("\r\n<--Exit SLEEP-->\r\n");
        }
        else
        if(2 == AccOnCount)
        {
        }
//        else
//        if(20 == AccOnCount)
//        {
//            CHARGE_ON(); 
//        }
    }
    else
    {
        AccOnCount = 0;
        if(AccOffCount<600)AccOffCount++;
		
        if(1 == AccOffCount)
        {
        	if(ReadSleepFlag()==2)
				{
					EnterSleepCount=60;
				}
			else
				{
					EnterSleepCount=300;
				}
			
        }
        else
        if(EnterSleepCount == AccOffCount)//����Ӻ�������ߣ����߻��ƣ���������֮ǰ����ģ��һ�Σ�Ȼ�����Ql_SleepEnable();��Ȼsleepģʽ����ģʽ�´��ڲ����Է���
        {
        	if(ReadSleepFlag()==1)
			{
				SetSleepFlag(2);
				APP_DEBUG("Sleep power down\r\n");
				Ql_PowerDown(1);
				//Ql_Sleep(500);
			}
		else if(ReadSleepFlag()==2)
			{
            	Io_WriteIoStatusBit(IO_STATUS_BIT_SLEEP, SET);
				SetSleepFlag(1);
				Ql_SleepEnable();
				APP_DEBUG("\r\n<--Enter SLEEP-->\r\n");
			}
		else{
			APP_DEBUG("\r\n<--Enter SLEEP ERROR-->\r\n");
			}
        }
    }
    /////////////////
    return ENABLE;
}




