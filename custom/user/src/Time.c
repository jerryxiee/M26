#include "include.h"

static u32 CurTimeID = 0;//��ǰʱ�䣬��ID����ʽ��ʾ

TIME_T CurTime;//��ǰʱ�䣬ÿ���Ӹ���һ�Σ���λ�û㱨�����и��£����õ���ϵͳʱ��

/*********************************************************************
//��������	:ReadTime(TIME_T *tt)
//����		:��ʵʱʱ��
//����		:
//���		:��ȡ�Ľ������ڽṹ��tt��
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:    
//��ע		:
*********************************************************************/
void ReadTime(void)
{
	ST_Time time;
	if(Ql_GetLocalTime(&time)!=NULL)//��ȡMC20ʱ��
		{
			CurTime.year=time.year%2000;//��
			CurTime.month=time.month;//��
			CurTime.day=time.day;//��
			CurTime.hour=time.hour;//ʱ
			CurTime.min=time.minute;//��
			CurTime.sec=time.second;//��
		//APP_DEBUG("<-- Ql_GetLocalTime(%d.%02d.%02d %02d:%02d:%02d timezone=%02d) -->\n\r", 
        //time.year, time.month, time.day, time.hour, time.minute, time.second, time.timezone);

		
		//APP_DEBUG("<-- Ql_GetLocalTime(%d.%02d.%02d %02d:%02d:%02d) -->\n\r", 
        //CurTime.year, CurTime.month, CurTime.day, CurTime.hour, CurTime.min, CurTime.sec);
		
		}
}


/*********************************************************************
//��������	:SetRtc(TIME_T *tt)
//����		:����RTC��������ֵ
//����		:tt,ʱ��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void SetRtc(TIME_T *tt)
{	
    //���RTC����ֵ
    CurTimeID = ConverseGmtime(tt);
    //д�뵽RTC������
    //RTC_SetCounter(CurTimeID);	
}
/*********************************************************************
//��������	:ReadRtc(TIME_T *tt)
//����		:��ʵʱʱ��
//����		:
//���		:��ȡ�Ľ������ڽṹ��tt��
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:    
//��ע		:
*********************************************************************/
void ReadRtc(void)
{     
	//��RTC��������ֵ
	//CurTimeID = RTC_GetCounter();
	//������ꡢ�¡��ա�ʱ���֡����ʾ��ʱ��        
	//Gmtime(&CurTime, CurTimeID);
}

/*********************************************************************
//��������	:RTC_GetCurTime
//����		:��ȡ��ǰʱ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:       
//��ע:
*********************************************************************/
void RTC_GetCurTime(TIME_T *tt)
{
    tt->year    = CurTime.year;
    tt->month   = CurTime.month;
    tt->day     = CurTime.day;
    tt->hour    = CurTime.hour;
    tt->min     = CurTime.min;
    tt->sec     = CurTime.sec;
}
/*********************************************************************
//��������	:LeapYear
//����		:�ж��Ƿ�Ϊ����
//����		:���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:�����귵��1���������귵��0
//��ע		:year��ȡֵΪ0-99���˺���ת����ʱ���������2000��
*********************************************************************/
u8 LeapYear(u8 year)
{
    unsigned short year2;	
    year2 = year + 2000;
    //////////////////
    if(year2%4  != 0)return 0;//����0//��������Ķ�
    ////////////////////
    if(year2%100!= 0)return 1;//Ϊ4�ı���������100�ı���,��������
    ///////////////
    if(year2%400!= 0)return 0;//Ϊ100�ı���������400�ı���,��������
    /////////////
    return 1;//��400�ı���������
}
/*********************************************************************
//��������	:YearDay
//����		:�����ꡢ�¡��գ������Ǹ���ĵڼ���
//����		:�ꡢ�¡���
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:���ظ���ĵڼ���
//��ע		:�˺���ת����ʱ���������2000��
*********************************************************************/
u16 YearDay(u8 yy,s8 mm,s8 dd)
{
    u16 TotalDay = 0;
    unsigned short jdays[] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    TotalDay += jdays[mm];
    TotalDay += dd;
    if(LeapYear(yy)&&(mm > 2))
        TotalDay++;
    return TotalDay;
}
/*********************************************************************
//��������	:CheckTimeStruct(TIME_T *tt)
//����		:���ʱ��ṹ���ڵ�ֵ�Ƿ���ȷ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:��ȷ������SUCCESS�����󷵻�ERROR
//��ע		:
*********************************************************************/
ErrorStatus CheckTimeStruct(TIME_T *tt)
{
    //����ꡢ�¡��ա�ʱ���֡���ʱ�����
    if((tt->year > END_YEAR)||(tt->year < START_YEAR)||
    (tt->month > 12)||(tt->month < 1)||
    (tt->day > 31)||(tt->day < 1)||
    (tt->hour > 23)||(tt->hour < 0)||
    (tt->min > 59)||(tt->min < 0)||
    (tt->sec > 59)||(tt->sec < 0))
    {
        return ERROR;
    }
    else
    {
        return SUCCESS;
    }
}
/*********************************************************************
//��������	:ConverseGmtime
//����		:�ѽṹ������ʱ��ת����32λ�޷��ŵ�ͳһ����ֵ
//����		:TIME_T �ṹ������ʱ��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:32λ�޷��ŵ�ͳһ����ֵ
//��ע		:�˺���ת����ʱ���������2000��
*********************************************************************/
u32 ConverseGmtime(TIME_T *tt)
{
    s16 i;
    s16 year;
    u32 TotalDay;
    u32 TotalSec;

    TotalDay = 0;
    TotalSec = 0;
    year = tt->year;

    if((year >= START_YEAR)&&(year <= END_YEAR))	//�ж����Ƿ���Ϸ�Χ
    {
        for(i = START_YEAR; i < tt->year; i++)	//����year��֮ǰ������
        {
            TotalDay += (365+LeapYear(i));
        }
        TotalDay += YearDay(tt->year, tt->month, tt->day);	//����year���������
        TotalSec = (TotalDay-1)*DAY_SECOND + tt->hour*HOUR_SECOND+ tt->min*60 + tt->sec;	//�����ܵ�����
    }
    return TotalSec;
}

/*********************************************************************
//��������	:Gmtime
//����		:��32λ�޷��ŵ�ͳһ����ֵת���ɽṹ������ʱ��
//����		:ָ��TIME_T���͵�ָ�룬����ֵ
//���		:ָ��TIME_T���͵�ָ��
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Gmtime(TIME_T *tt, u32 counter)
{
    s16 i;
    u32 sum ;
    u32 temp;
    u32 counter1 = 0;
    u8 flag;
    unsigned short jdays[] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    //////////////////
    if(counter > 0xBC191380)//������ʼ���һ����,����ת��
    {
        tt->year    = START_YEAR;
        tt->month   = 1;
        tt->day     = 1;
        tt->hour    = 0;
        tt->min     = 0;
        tt->sec     = 0;
        return;//������ʼ���һ����,����ת��
    }
    ///////////////////////
    sum = 0;
    temp = 0;    
    for(i=START_YEAR; sum<=counter&&i<END_YEAR; i++)   /* ������� */
    {
        sum += (LeapYear(i)*DAY_SECOND + YEAR_SECOND);
        if(sum <= counter)
        {
            temp = sum;
        }
    }
    ///////////////////////////
    tt->year = i-1;
    flag=LeapYear(tt->year);
    counter1=counter-temp;
    /////////////////////////
    sum=counter1;
    counter1=counter1%DAY_SECOND;
    temp =(sum-counter1)/DAY_SECOND;
    temp++;//TotalSec = (TotalDay-1)*DAY_SECOND + tt->hour*HOUR_SECOND+ tt->min*60 + tt->sec;	//�����ܵ�����
    for(i=12; i>0; i--)//
    {            
        if((temp>(jdays[i]+flag)&&i>2)||(temp>jdays[i]&&i<=2))            
        {
            tt->month=i;
            break;
        }
    }
    //////////////////////////////////////
    tt->day=temp-jdays[tt->month];
    if(tt->month>2)tt->day-=flag;
    ///////////
    tt->hour =counter1/HOUR_SECOND;
    ///////////////////////////
    counter1=counter1%HOUR_SECOND;
    /////////////////////
    tt->min =counter1/MIN_SECOND;
    /////////////
    tt->sec=counter1%MIN_SECOND;
    /////////////////////
}
/*********************************************************************
//��������	:
//����		:
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:       
//��ע:
*********************************************************************/
void Num2NumStr(u32 Num, u8 *NumStr)
{
    if(0 == Num)
    {
        *NumStr = 0;
        return ;
    }
    ////////////////////
    sprintf((char *)NumStr,"%02ul",Num);
    #if 0//modify by joneming
    u8 *p;
    u8 NumChar[10];
    u8 i;
    p = NumChar;
    *p = Num/1000000000 + 48;
    p++;
    *p = Num%1000000000/100000000 + 48;
    p++;
    *p = Num%100000000/10000000 + 48;
    p++;
    *p = Num%10000000/1000000 + 48;
    p++;
    *p = Num%1000000/100000 + 48;
    p++;
    *p = Num%100000/10000 + 48;
    p++;
    *p = Num%10000/1000 + 48;
    p++;
    *p = Num%1000/100 + 48;
    p++;
    *p = Num%100/10 + 48;
    p++;
    *p = Num%10 + 48;

    p = NumStr;
    i =0;
    while(NumChar[i] == '0')
    {
        i++;
    }
    if(10 == i)  //��NumֵΪ0ʱ
    {
        *p = '0';
        p++;
        *p = '0';
        p++;
        *p = '\0';
    }
    else if(9 == i) //��NumֵС��10ʱ
    {
        *p = '0';
        p++;
        *p = NumChar[i];
        p++;
        *p = '\0';
    }
    else //��Numֵ���ڵ���10ʱ
    {
        for(; i<10; i++,p++)
        {
            *p = NumChar[i];
        }
        *p = '\0';
    }
    #endif

}

/*************************************************************
** ��������: Public_CheckTimeIsInRange
** ��������: �ж�ָ��ʱ���Ƿ���ָ����Χ��
** ��ڲ���: curTimeָ��ʱ��,startTime��ʼʱ��,endTime����ʱ��
** ���ڲ���: 
** ���ز���: 1:�ڷ�Χ�ڣ�0:���ڷ�Χ�� 
** ȫ�ֱ���: ��
** ����ģ��: 
*************************************************************/
unsigned char Public_CheckTimeIsInRange(unsigned long curTime,unsigned long startTime,unsigned long endTime)
{
    if(startTime<endTime)//С��
    {
        if(startTime<=curTime&&curTime<=endTime)return 1;
    }
    else//����
    if(startTime>endTime)//
    {
        if(startTime<=curTime||curTime<=endTime)return 1;
    }
    else
    if(startTime==endTime)//����
    {
        if(startTime==curTime)return 1;
    }
    return 0;
}


