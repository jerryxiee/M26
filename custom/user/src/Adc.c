#include "include.h"

#define POWERLOW     11700//����11.7VǷѹ
#define POWERSHUT    5000
#define POWERCHANGE  9

u32 Mc20AdcValue=0;
Enum_PinName adcPin = PIN_ADC0;

static u32 ADC_CustomParam = 1;
static void Callback_OnADCSampling(Enum_ADCPin adcPin, u32 adcValue, void *customParam)
{
	u32 mainPown;
	static u8 AdcConter=0;
	Mc20AdcValue=adcValue;
   // APP_DEBUG("<-- Callback_OnADCSampling: sampling voltage(mV)=%d  times=%d -->\r\n", adcValue, *((s32*)customParam))
    *((s32*)customParam) += 1;

    mainPown=adcValue*POWERCHANGE;
	//APP_DEBUG("<-- �����ѹ=%d -->\r\n",mainPown)
		
    if((mainPown<=POWERLOW)&&(mainPown>=POWERSHUT))
    {
    	AdcConter++;
		if(AdcConter>30)
			{
			AdcConter=0;
      		  Io_WriteAlarmBit(ALARM_BIT_POWER_LOW, SET);//����Ƿѹ��λ
      		  Io_WriteAlarmBit(ALARM_BIT_POWER_SHUT, RESET);//����������
			}
		//APP_DEBUG("<-- ����Ƿѹ -->\r\n")
    }
    else if(mainPown<POWERSHUT)
    {
    	AdcConter=0;
        Io_WriteAlarmBit(ALARM_BIT_POWER_LOW, RESET);//����Ƿѹ���
        Io_WriteAlarmBit(ALARM_BIT_POWER_SHUT, SET);//���������λ
		//APP_DEBUG("<-- ������� -->\r\n")
    }
	else
	{
		//APP_DEBUG("<-- �������� -->\r\n")
		AdcConter=0;
        Io_WriteAlarmBit(ALARM_BIT_POWER_SHUT, RESET);//����������
		Io_WriteAlarmBit(ALARM_BIT_POWER_LOW, RESET);//����Ƿѹ���
	}
}


/*********************************************************************
//��������	:Ad_GetValue
//����		:����AD���,��õ�ǰADֵ
//����		:adNum AD���,�ο�ADC_CHAN_TYPEö��.
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:��ǰ�ɼ�����ADֵ.
//��ע		:
*********************************************************************/
u32 Ad_GetValue(u8 Channel)
{
	return Mc20AdcValue*POWERCHANGE;//�õ�ʵ�ʵ�ѹ����λmV
}

void KillAdc(void)
{
    // Stop  sampling ADC
    Ql_ADC_Sampling(adcPin, FALSE);

}

void Ad_Init(void)
{

    // Register callback foR ADC
    APP_DEBUG("<-- Register callback for ADC -->\r\n")
    Ql_ADC_Register(adcPin, Callback_OnADCSampling, (void *)&ADC_CustomParam);

    // Initialize ADC (sampling count, sampling interval)
    APP_DEBUG("<-- Initialize ADC (sampling count=5, sampling interval=200ms) -->\r\n")
    Ql_ADC_Init(adcPin, 5, 200);

    // Start ADC sampling
    APP_DEBUG("<-- Start ADC sampling -->\r\n")
    Ql_ADC_Sampling(adcPin, TRUE);

    // Stop  sampling ADC
    //Ql_ADC_Sampling(adcPin, FALSE);
}


