#include "include.h"
#include "report.h"
//****************�궨��****************
//***************��������***************
//********************A�����״̬λ����************
static u16 CameraStatu=0;//A���¼�ǹ�����Ϣ
//********************������״̬λ����************
static u32	Status = 0;//32λ״̬λ,��Ӧ����λ����Ϣ�㱨���״̬λ����
static u32	ExtCarStatus = 0;//��չ�ĳ���״̬λ,��Ӧ����λ����Ϣ�㱨�����չ�����ź�״̬����
static u32	IoStatus = 0;//IO״̬λ,��Ӧ����λ����Ϣ�㱨�����չIO״̬����
static u32	Alarm = 0;//32λ����λ,��Ӧ����λ����Ϣ�㱨��ı���λ����
static u32	SelfDefineStatus = 0;//�Զ����1������ID0xE1
static u32	SelfDefineStatusMaskWord = 0;//�Զ����1������
//static u32  SelfDefineStatusHighValid = 0;//�Զ���1����Ч��־����ӦλΪ1��ʾ����Ч��0Ϊ����Ч
static u8	MicControl = 0;//��˷������
//********************������״̬λ��Ӧ���ⲿIO�ڸߵ���Ч����************
static u32	AlarmHighValid = 0xffffffff;  //�ߵ�ѹ��Ч״̬��־��ÿ��λ��Alarm��Ӧ��1Ϊ����Ч��0Ϊ����Ч
static u32	StatusHighValid = 0x0;        //�ߵ�ѹ��Ч״̬��־��ÿ��λ��IoStatus��Ӧ��1Ϊ����Ч��0Ϊ����Ч
//static u32	ExtCarStatusHighValid = 0x0;  //�ߵ�ѹ��Ч״̬��־��ÿ��λ��ExtCarStatus��Ӧ��1Ϊ����Ч��0Ϊ����Ч

//*********************ͨ��ģ�飬��λģ��ָʾ�ƿ���************
u32	GprsLedOnTime = 0;
u32	GprsLedOffTime = 0;
u32	GpsLedOnTime = 0;
u32	GpsLedOffTime = 0;
u8	GprsLedOnOffFlag = 0;//���������ر�־
u8	GpsLedOnOffFlag = 0;//���������ر�־
u8	GprsLedState = 0;//GPRS LED��״̬,0��������Ϩ��,1��������˸,2�����ϳ���
u8	GpsLedState = 0;//GPS LED��״̬,0��������Ϩ��,1��������˸,2�����ϳ���
//***************12/24V��Դ����Ӧ**************
static u16	PowerLowMaxValue = 0;//����0V
static u16	PowerShutValue = 50;//����5.0V

//****************�ⲿ����*****************
extern u8 GpsOnOffFlag;//GPS���ر�־,0ΪGPS��,1ΪGPS��
u32	AlarmMaskWord = 0;//����������,���ö�Ӧλ��ñ���λ�����ٱ���,�൱�ڸñ�������û����
///////////////////////
void Io_ScanTimeTask(void);
//****************��������*****************
/*********************************************************************
//��������	:Io_Init(void)
//����		:IO��ʼ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Io_Init(void)
{
//�������
    GpioOutInit(GPRS_LED,PINLEVEL_HIGH);
	GpioOutInit(RESET_KEY,PINLEVEL_LOW);
	GpioOutInit(WATCHDOG,PINLEVEL_HIGH);
    GpioOutInit(REMOTE_POWERDOWN,PINLEVEL_LOW);
	GpioOutInit(OPEN_PULL_LOW,PINLEVEL_LOW);
	GpioOutInit(WDT_RESET,PINLEVEL_HIGH);
}
//**********************��һ���֣���ʼ��****************
/*********************************************************************
//��������	:Io_UpdataPram(void)
//����		:���±���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Io_UpdataPram(void)
{    

	u8	Buffer[5];
	u8	BufferLen;
          
	//������������
	BufferLen = EepromPram_ReadPram(E2_ALARM_MASK_ID,Buffer);
	if(BufferLen == E2_ALARM_MASK_LEN)
	{
		AlarmMaskWord = Public_ConvertBufferToLong(Buffer);
	}

    //�Զ����1������
    BufferLen = EepromPram_ReadPram(E2_DEFINE_ALARM_MASK_WORD_ID,Buffer);
	if(BufferLen == E2_DEFINE_ALARM_MASK_WORD_ID_LEN)
	{
		SelfDefineStatusMaskWord = Public_ConvertBufferToLong(Buffer);
	}
	//����һ�ν�������״̬
	BufferLen = FRAM_BufferRead(Buffer, FRAM_EMERGENCY_FLAG_LEN, FRAM_EMERGENCY_FLAG_ADDR);
	if(1 == BufferLen)
	{
		if(1 == Buffer[0])
		{
			Io_WriteAlarmBit(ALARM_BIT_EMERGENCY, SET);
		}
		else
		{
			Io_WriteAlarmBit(ALARM_BIT_EMERGENCY, RESET);
		}
	}
	
	//��ȡ��λģʽ
	BufferLen = EepromPram_ReadPram(E2_GPS_SET_MODE_ID,Buffer);
	if(E2_GPS_SET_MODE_LEN == BufferLen)//bit0:GPS��λ;bit1:������λ;bit2:GLONASS��λ;bit3:Galileo��λ
	{
		if(0x01 == (0x01&Buffer[0]))
		{
			Io_WriteStatusBit(STATUS_BIT_GPS, SET);
		}
		else
		{
			Io_WriteStatusBit(STATUS_BIT_GPS, RESET);
		}
		
		if(0x02 == (0x02&Buffer[0]))
		{
			Io_WriteStatusBit(STATUS_BIT_COMPASS, SET);
		}
		else
		{
			Io_WriteStatusBit(STATUS_BIT_COMPASS, RESET);
		}
		
		if(0x04 == (0x04&Buffer[0]))
		{
			Io_WriteStatusBit( STATUS_BIT_GLONASS, SET);
		}
		else
		{
			Io_WriteStatusBit( STATUS_BIT_GLONASS, RESET);
		}
		
		if(0x08 == (0x08&Buffer[0]))
		{
			Io_WriteStatusBit(STATUS_BIT_GALILEO, SET);
		}
		else
		{
			Io_WriteStatusBit(STATUS_BIT_GALILEO, RESET);
		}		
	}
	
}

/*********************************************************************
//��������	:Io_ReadStatus
//����		:����׼״̬��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u32 Io_ReadStatus(void)
{
	return Status;
}
/*********************************************************************
//��������	:Io_ReadStatusBit(u8 index)
//����		:����׼״̬λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u8 Io_ReadStatusBit(STATUS_BIT bit)
{
	if(bit >= 32)return 0;
    return PUBLIC_CHECKBIT_EX(Status, bit);
}
/*********************************************************************
//��������	:Io_WriteStatusBit
//����		:д��׼״̬λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteStatusBit(STATUS_BIT bit, FlagStatus state)
{
	if(bit >= 32)return;
    if(SET==state)
    {
        PUBLIC_SETBIT(Status, bit);
    }
    else if(RESET==state)
    {
        PUBLIC_CLRBIT(Status, bit);
    }
}
/*********************************************************************
//��������	:Io_ReadExtCarStatus
//����		:����չ����״̬��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u32 Io_ReadExtCarStatus(void)
{
	return ExtCarStatus;
}
/*********************************************************************
//��������	:Io_ReadExtCarStatusBit(u8 index)
//����		:����չ����״̬λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u8 Io_ReadExtCarStatusBit(CAR_STATUS_BIT bit)
{
    if(bit >= 32)return 0;
    return PUBLIC_CHECKBIT_EX(ExtCarStatus, bit);
}
/*********************************************************************
//��������	:Io_WriteExtCarStatusBit
//����		:д��չ����״̬λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteExtCarStatusBit(CAR_STATUS_BIT bit, FlagStatus state)
{
	if(bit >= 32)return;
    if(SET==state)
    {
        PUBLIC_SETBIT(ExtCarStatus, bit);
    }
    else 
    if(RESET==state)
    {
        PUBLIC_CLRBIT(ExtCarStatus, bit);
    }
}
/*********************************************************************
//��������	:Io_ReadIoStatus
//����		:��IO״̬��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u16 Io_ReadIoStatus(void)
{
	return IoStatus;
}
/*********************************************************************
//��������	:Io_ReadIoStatusBit(u8 index)
//����		:��IO״̬λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u8 Io_ReadIoStatusBit(IO_STATUS_BIT bit)
{
    if(bit >= 16)return 0;
    return PUBLIC_CHECKBIT_EX(IoStatus, bit);
}
/*********************************************************************
//��������	:Io_WriteIoStatusBit
//����		:дIO״̬λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteIoStatusBit(IO_STATUS_BIT bit, FlagStatus state)
{
    if(bit >= 16)return;
    if(SET==state)
    {
        PUBLIC_SETBIT(IoStatus, bit);
    }
    else if(RESET==state)
    {
        PUBLIC_CLRBIT(IoStatus, bit);
    }
}
/*********************************************************************
//��������	:Io_ReadAlarm
//����		:��������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u32 Io_ReadAlarm(void)
{
    return (Alarm&((~AlarmMaskWord)));
}
/*********************************************************************
//��������	:Io_ReadAlarmMaskBit(u8 index)
//����		:����������λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:�ؼ�����λ��ӦΪ1��ֱ�ӷ���0���ؼ�����λ��ӦλΪ0���������α���λ
*********************************************************************/
u8 Io_ReadAlarmMaskBit(ALARM_BIT bit)
{
    if(bit >= 32)return 0;
    return PUBLIC_CHECKBIT_EX(AlarmMaskWord, bit);
}
/*********************************************************************
//��������	:Io_ReadSelfDefine2
//����		:���Զ���״̬��1
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u32 Io_ReadSelfDefine2(void)
{
	return (SelfDefineStatus&(~SelfDefineStatusMaskWord));
}
/*********************************************************************
//��������	:Io_ReadSelfDefine2Bit(u8 index)
//����		:���Զ����1��λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u8 Io_ReadSelfDefine2Bit(E_DEFINE_STATUS_BIT bit)
{
	u8	tmp = 0;

	if(bit < 32)
	{
		tmp = ((SelfDefineStatus&(~SelfDefineStatusMaskWord)) & (1<< bit))>> bit;
	}
	return tmp;
}
/*********************************************************************
//��������	:Io_ReadAlarmBit(u8 index)
//����		:������λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
u8 Io_ReadAlarmBit(ALARM_BIT bit)
{
	u8	tmp = 0;

	if(bit < 32)
	{
		tmp = ((Alarm&((~AlarmMaskWord))) & (1<< bit))>> bit;
	}
	return tmp;
}

/*********************************************************************
//��������	:Io_WriteAlarmBit
//����		:дĳһ����λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteAlarmBit(ALARM_BIT bit, FlagStatus state)
{
    if(bit >= 32)return;	
    if(SET==state)
    {
        PUBLIC_SETBIT(Alarm,bit);
    }
    else if(RESET==state)
    {
        PUBLIC_CLRBIT(Alarm,bit);
    }
}

/*********************************************************************
//��������	:Io_WriteSelfDefine2Bit
//����		:д�Զ����1��λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteSelfDefine2Bit(E_DEFINE_STATUS_BIT bit, FlagStatus state)
{
	if(bit >= 32)return;
    if(SET==state)
    {
        PUBLIC_SETBIT(SelfDefineStatus, bit);
    }
    else if(RESET==state)
    {
        PUBLIC_CLRBIT(SelfDefineStatus, bit);
    }
}
/*********************************************************************
//��������	:Io_WriteMicConrtolBit
//����		:дĳһ��˿���λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteMicConrtolBit(MIC_BIT bit, FlagStatus Status)
{
	if(bit >= MIC_BIT_MAX)return;		
    if(SET==Status)
    {
        PUBLIC_SETBIT(MicControl, bit);
    }
    else if(RESET==Status)
    {
        PUBLIC_CLRBIT(MicControl, bit);
    }
	if(0 == MicControl)
	{
		//MIC_BASE_OFF();//�ر�
	}
	else
	{
		//MIC_BASE_ON();//��
	}
}
/*********************************************************************
//��������	:Io_WriteSpeakerConrtolBit
//����		:дĳһ����������λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteSpeakerConrtolBit(SPEAKER_BIT bit, FlagStatus state)
{
/*
    if(bit >= SPEAKER_BIT_MAX)return;
    if(SET==state)
    {   
        PUBLIC_SETBIT(SpeakerControl, bit);
    }
    else 
    if(RESET==state)
    {
        PUBLIC_CLRBIT(SpeakerControl, bit);
    }
    if(0 == SpeakerControl)
    {
        SPEAKER_OFF();//�ر�
    }
    else
    {
        SPEAKER_ON();//��
    }
    */
}
/********************�������֣��ڲ�����*******************
/*********************************************************************
//��������	:Io_DetectStatus
//����		:�������״̬λ��IO����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Io_DetectStatus(GPIO_OUT_IN_NUM IoNum, u8 index)
{
	unsigned char checknum;
	unsigned char state;
	static u8 sucStatusHighCount[32] = {0};
	static u8 sucStatusLowCount[32] = {0};
	checknum = 10;
	state = GpioInGetState(IoNum);
	if(1 == state)
	{
		if(sucStatusHighCount[index] < checknum)
		{
			sucStatusHighCount[index]++;
		}
		sucStatusLowCount[index] = 0;
	}
	else
	{
		sucStatusHighCount[index] = 0;		
		if(sucStatusLowCount[index] <checknum)
		{
			sucStatusLowCount[index]++;
		}
	}
    if(sucStatusHighCount[index] >=checknum)
    {
        if(PUBLIC_CHECKBIT(StatusHighValid,index))
        {
            PUBLIC_SETBIT(Status,index);
        }
        else
        {
            PUBLIC_CLRBIT(Status,index);
        }
    }
	else if(sucStatusLowCount[index] >=checknum)
	{
        if(0==PUBLIC_CHECKBIT(StatusHighValid,index))
        {
            PUBLIC_SETBIT(Status,index);
        }
        else
        {
            PUBLIC_CLRBIT(Status,index);
        }
	}
}

/*********************************************************************
//��������	:Io_DetectBackPower
//����		:��ⱸ���ѹ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:50ms����һ��
*********************************************************************/
void Io_DetectBackPower(void)
{
    if(!Io_ReadAlarmBit(ALARM_BIT_POWER_SHUT))return;//����
	//����Ӵ���
}
/*********************************************************************
//��������	:Io_DetectMainPower
//����		:��������ѹ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:100ms����һ��
*********************************************************************/
void Io_DetectMainPower(void)
{
    #define MAIN_POWER_BUFFER_LEN     6
    static unsigned char sucCnt=0;
    static unsigned char sucAdValCount = 0;
    static unsigned char sucAdValCnt = 0;
    static unsigned short susBatValue[MAIN_POWER_BUFFER_LEN];
    //////////////////////////
    static unsigned char LowAlarmCount = 0;
    static unsigned char NoLowAlarmCount = 0;
    static unsigned char ShutAlarmCount = 0;
    static unsigned char NoShutAlarmCount = 0;    
    unsigned char AlarmCountMax;
    unsigned short AdMin = 0xffff;//ADת��ֵ��Сֵ
    unsigned short AdMax = 0;//ADת��ֵ���ֵ
    unsigned long AdSum = 0;//ADת��ֵ�ۼƺ�    
    unsigned char i;
    ////////////
    sucCnt++;
    sucCnt = 0;
    ////////////////////
    AlarmCountMax = 150;//ʵ��Ӧ��ʱʹ��
    susBatValue[sucAdValCnt] = Ad_GetValue(1); 
    sucAdValCnt++;
    if(sucAdValCnt>=MAIN_POWER_BUFFER_LEN)sucAdValCnt=0;
    /////////////////////////////
    if(sucAdValCount<MAIN_POWER_BUFFER_LEN)
    {
        sucAdValCount++;
        return;
    }
    AdSum = 0;
    for(i=0;i<sucAdValCount;i++)
    {
        if(AdMin > susBatValue[i])
        {
            AdMin = susBatValue[i];
        }
        if(AdMax < susBatValue[i])
        {
            AdMax = susBatValue[i];
        }
        AdSum += susBatValue[i];
    }
    //����ƽ��ֵ
    AdSum -= AdMin;
    AdSum -= AdMax;
    AdSum = (AdSum /(MAIN_POWER_BUFFER_LEN-2));//(MAIN_POWER_BUFFER_LEN-2)
    //ת����0.1VΪ��λ��ֵ
    AdSum = AdSum/100;
    //��λ�����Ƿѹ����
    if((AdSum < PowerLowMaxValue)&&(AdSum > PowerShutValue))
    {
        if(LowAlarmCount < AlarmCountMax)//ʵ��Ӧ��ȡֵ
        {
            LowAlarmCount++;
        }
        NoLowAlarmCount = 0;
    }
    else
    if(AdSum >(PowerLowMaxValue+2))
    {
        LowAlarmCount = 0;        			
        if(NoLowAlarmCount < AlarmCountMax)//ʵ��Ӧ��ȡֵ
        {
            NoLowAlarmCount++;
        }
    }
    if(LowAlarmCount >=AlarmCountMax)
    {
        Io_WriteAlarmBit(ALARM_BIT_POWER_LOW, SET);//����Ƿѹ��λ
        Io_WriteAlarmBit(ALARM_BIT_POWER_SHUT, RESET);//����������
    }
    else if(NoLowAlarmCount >=AlarmCountMax)
    {
        Io_WriteAlarmBit(ALARM_BIT_POWER_LOW, RESET);//����Ƿѹ���
    }
    //��λ������ϵ籨��
    if(AdSum < PowerShutValue)
    {
        if(ShutAlarmCount < AlarmCountMax)
        {
            ShutAlarmCount++;
        }
        NoShutAlarmCount = 0;
    }
    else
    if(AdSum >(PowerShutValue+10))
    {
        ShutAlarmCount = 0;        
        if(NoShutAlarmCount < AlarmCountMax)
        {
            NoShutAlarmCount++;
        }
    }
    if(ShutAlarmCount >=AlarmCountMax)
    {
        Io_WriteAlarmBit(ALARM_BIT_POWER_SHUT, SET);//���������λ
        Io_WriteAlarmBit(ALARM_BIT_POWER_LOW, RESET);//����Ƿѹ���
    }
    else if(NoShutAlarmCount >=AlarmCountMax)
    {
        Io_WriteAlarmBit(ALARM_BIT_POWER_SHUT, RESET);//����������
    }    
}
/*********************************************************************
//��������	:Io_PowerOnDetectMainPower
//����		:�ϵ��������ѹ,�ж���12V���绹��24V����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Io_PowerOnDetectMainPower(void)
{	
	u32	AdSum = 0;//ADת��ֵ�ۼƺ�
	Ql_Sleep(1000);//�ȴ�ADCת�����
    /////////////////////////
	AdSum = Ad_GetValue(1); 
	//����ƽ��ֵ
	//ת����0.1VΪ��λ��ֵ
	AdSum = AdSum/100;
	if(AdSum >= 200)//���ڵ���20V��Ϊ��24Vϵͳ,������12Vϵͳ
	{
		PowerLowMaxValue = 228;
		//PWR_TYPE_24V();
	}
	else
	{
		PowerLowMaxValue = 117;
		//PWR_TYPE_12V();
	}		
}

/*********************************************************************
//��������	:Io_WriteStatusHighValidBit
//����		:дStatusHighValidĳһ״̬λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteStatusHighValidBit(STATUS_BIT bit, FlagStatus state)
{
    if(bit >= 32)return;
    if(SET==state)
    {
        PUBLIC_SETBIT(StatusHighValid, bit);
    }
    else if(RESET==state)
    {
        PUBLIC_CLRBIT(StatusHighValid, bit);
    }
}

/*********************************************************************
//��������	:Io_WriteAlarmHighValidBit
//����		:дAlarmHighValidĳһ״̬λ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:ע���벻Ҫ���жϺ����ڵ��ô˺���
*********************************************************************/
void Io_WriteAlarmHighValidBit(ALARM_BIT bit, FlagStatus state)
{
	if(bit >= 32)return;
    if(SET==state)
    {
        PUBLIC_SETBIT(AlarmHighValid, bit);
    }
    else if(RESET==state)
    {
        PUBLIC_CLRBIT(AlarmHighValid, bit);
    }
}
/*********************************************************************
//��������	:Io_ScanIoStatusAndAlarm
//����		:IO������λ�������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:100ms����һ��
*********************************************************************/
void Io_ScanIoStatusAndAlarm(void)
{
    u8	Acc;
    //Io_DetectStatus(IO_ACC, STATUS_BIT_ACC);                   //���ACC
    //////////////////
    //Io_DetectStatus(IO_DOOR, STATUS_BIT_DOOR1);                   //��,����Ч
    ///////////////////////
    //Io_DetectMainPower();//��������Ƿ�����
    /////////////////
    Io_DetectBackPower();//��ⱸ��    
    ////////////////////////////
    Acc = Io_ReadStatusBit(STATUS_BIT_ACC);
}
/*********************************************************************
//��������	:Io_StatusAlarmAndPhoto
//����		:IO������λ�������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:100ms����һ��
*********************************************************************/
void Io_StatusAlarmAndPhoto(void)
{
      //zengliang add 2017-04-06
     static u32	LastStatus = 0;//��һ��IO״̬
     static u32	LastAlarm = 0;//��һ�α���״̬
     static u32	LastSelfDefineStatus = 0;//��һ�α���״̬
	 u32 Status,Alarm,SelfDefineStatus;
	 
	 Status = Io_ReadStatus();
	 Alarm = Io_ReadAlarm();
	 SelfDefineStatus = Io_ReadSelfDefine2();
	 if(ReportStatus_Off != Report_GetReportStatus()){
	     //״̬�仯�����ϱ�
	     if(LastStatus != Status)
	     {
	       Report_UploadPositionInfo();//�ϱ�һ��λ����Ϣ
	       APP_DEBUG("<--״̬�仯�ϱ�λ����Ϣ %d-->\r\n",Status);
	     }
	     //�������������ϱ�
	     if(LastAlarm != Alarm)
	     {
	       Report_UploadPositionInfo();//�ϱ�һ��λ����Ϣ
	       APP_DEBUG("<--����״̬�仯�ϱ�λ����Ϣ -->\r\n");
	     }
		 //��չ��������״̬λ
		 if(LastSelfDefineStatus != SelfDefineStatus)
	     {
	       Report_UploadPositionInfo();//�ϱ�һ��λ����Ϣ
	       APP_DEBUG("<--��չ��������״̬�仯�ϱ�λ����Ϣ -->\r\n");
	     }
	 }
     LastAlarm = Alarm;
     LastStatus = Status;
	 LastSelfDefineStatus = SelfDefineStatus;
}
/*********************************************************************
//��������	:Io_StatusAlarmAndPhoto
//����		:LED״̬����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:100ms����һ��
*********************************************************************/
void Io_DisposeLEDAndBeep(void)
{
/*
    u32	CurrentTime = 0;
    static u8 led_flash=0;
    static u8 last_led_mode;
    u8 led_mode=0;
    led_mode=Gps_ReadStatus()<<4|communicatio_GetMudulState(COMMUNICATE_STATE);
    if(last_led_mode!=led_mode)
    {
      GprsLedOnOffFlag=0;
    }
    last_led_mode=led_mode;
  
    if((1 == Io_ReadAlarmBit(ALARM_BIT_ANT_SHUT))
     ||(1 == Io_ReadAlarmBit(ALARM_BIT_ANT_SHORT))
     ||(1 == Io_ReadAlarmBit(ALARM_BIT_GNSS_FAULT)))
    {
      led_mode=0xff;
    }
    ///////////////
    //***********������*************
    CurrentTime = Timer_Val();        
    //*********GPRS LED����**********
    //if(0 == communicatio_GetMudulState(COMMUNICATE_STATE))
    if(led_mode==0x11)//����������˫��
    {
      //˫��
      if(led_flash<3)
      {
        led_flash++;
        if(1 == GprsLedOnOffFlag)//��
        {
          if((CurrentTime - GprsLedOnTime) >= 6)
          {
            GPS_LED_OFF();
            GPRS_LED_OFF();
          }
        }
        else//��
        {
          if((CurrentTime - GprsLedOffTime) >= 1)
          {
            GPS_LED_ON();
            GPRS_LED_ON();
          }
        } 
      }
      else
      {
        GpioOutOff(GPRS_LED);
        if((CurrentTime - GprsLedOnTime) >= 20)
        {
          led_flash=0;
          GPS_LED_OFF();
          GPRS_LED_OFF();
        }
      }    
    }
    else
    if(led_mode==0x01)//����δ��������
    {
    //����
        if(1 == GprsLedOnOffFlag)//��
        {
            if((CurrentTime - GprsLedOnTime) >= 8)
            {
              GPS_LED_OFF();
                GPRS_LED_OFF();
            }
        }
        else//��
        {
            if((CurrentTime - GprsLedOffTime) >= 8)
            {
              GPS_LED_ON();
                GPRS_LED_ON();
            }
        }
    }
    else
    if(led_mode==0x10)//����δ��������
    {
      //����
      if(1 == GprsLedOnOffFlag)//��
      {
        if((CurrentTime - GprsLedOnTime) >= 40)
        {
          GPS_LED_OFF();
          GPRS_LED_OFF();
        }
      }
      else//��
      {
        if((CurrentTime - GprsLedOffTime) >= 40)
        {
          GPS_LED_ON();
          GPRS_LED_ON();
        }
      }
    }
    else
    if(led_mode==0x00)//δ���ߡ�������Ϩ��
    {
      GPRS_LED_OFF();
//      //����
//      if(1 == GprsLedOnOffFlag)//��
//      {
//        if((CurrentTime - GprsLedOnTime) >= 200)
//        {
//          GPS_LED_OFF();
//          GPRS_LED_OFF();
//        }
//      }
//      else//��
//      {
//        if((CurrentTime - GprsLedOffTime) >= 200)
//        {
//          GPS_LED_ON();
//          GPRS_LED_ON();
//        }
//      }
    }
    else
    if(led_mode==0xFF)//���ϳ���
    {
      GPS_LED_ON();
      GPRS_LED_ON();
    }
    //*********GPS LED����**********
//    if((1 == Io_ReadAlarmBit(ALARM_BIT_ANT_SHUT))
//     ||(1 == Io_ReadAlarmBit(ALARM_BIT_ANT_SHORT))
//     ||(1 == Io_ReadAlarmBit(ALARM_BIT_GNSS_FAULT)))
//    {
//        GPS_LED_ON();
//    }
//    else 
//    if(0 == Gps_ReadStatus())//������
//    {
//        GPS_LED_OFF();
//    }
//    else 
//    if(1 == Gps_ReadStatus())
//    {
//        if(1 == GpsLedOnOffFlag)//��
//        {
//            if((CurrentTime - GpsLedOnTime) >= SYSTICK_1SECOND)
//            {
//                GPS_LED_OFF();
//            }
//        }
//        else//��
//        {
//            if((CurrentTime - GpsLedOffTime) >= SYSTICK_1SECOND)
//            {
//                GPS_LED_ON();
//            }
//        }
//    }
*/
}
/*********************************************************************
//��������	:Io_AccDetec
//����		:acc���
*********************************************************************/
#if 0
void Io_AccDetec( void)
{

  static u8 AccOnLinCont=0;
  static u8 AccOffLineCont=0;
	if(Ql_GPIO_GetLevel(ACC_DET_PIN)==1)
	{
		//APP_DEBUG("\r\n<--ACC_LINE HIGHT AccOnLinCont=%d-->\r\n",AccOnLinCont);
		AccOffLineCont=0;
		if(AccOnLinCont<201)AccOnLinCont++;
		
		if(AccOnLinCont==200)
		{

			Io_WriteStatusBit(STATUS_BIT_ACC, SET);//ACC_ON
			//SetAccLineFlag(1);
			BATTERYCHARGE_ON();
		}
		return;
	}
	else
	{
		AccOnLinCont=0;
		//if(ReadAccLineFlag()==1)
		{
			//APP_DEBUG("\r\n<--ACC_LINE LOWER AccOffLineCont=%d-->\r\n",AccOffLineCont);
				if(AccOffLineCont<201)AccOffLineCont++;
				
				if(AccOffLineCont==200)
					{
						//ShockTickCnt=0;
						Io_WriteStatusBit(STATUS_BIT_ACC, RESET);//ACC_OFF
						BATTERYCHARGE_OFF();
						return;
					}
				return ;
		}
	}

}
#endif
/*********************************************************************
//��������	:Io_ScanTimeTask
//����		:IO������λ�������ɨ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:100ms����һ��
*********************************************************************/
void Io_ScanTimeTask(void)
{
	//Io_AccDetec();
    //Io_ScanIoStatusAndAlarm();
    Io_StatusAlarmAndPhoto();
}
/*********************************************************************
//��������	:Io_TimeTask
//����		:IO������λ�������
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:50ms����һ��
*********************************************************************/
FunctionalState Io_TimeTask(void)
{
	Io_ScanTimeTask();
    return ENABLE;
}

