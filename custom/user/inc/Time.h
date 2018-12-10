#ifndef __TIME_H
#define __TIME_H
//ʱ��ṹ������
typedef struct
{
    s8 year;    //��:00~99
    s8 month;   //��:1~12
    s8 day;     //��:1~31
    s8 hour;    //ʱ:0~23
    s8 min;     //��:0~59
    s8 sec;     //��:0~59
}TIME_T;

#define START_YEAR      15//�б���ʱ��Ҫ�ſ�������Ӱ��Уʱ
#define END_YEAR        99
//ʱ��궨��
#define MIN_SECOND      60
#define HOUR_SECOND     0xE10
#define DAY_SECOND      0x15180
#define YEAR_SECOND     0x1E13380

//**********��������**********
void SetRtc(TIME_T *tt);
void ReadRtc(void);
u8 LeapYear(u8 year);
u16 YearDay(u8 yy,s8 mm,s8 dd);
u32 ConverseGmtime(TIME_T *tt);
void Gmtime(TIME_T *tt, u32 counter);
void RTC_Configuration(void);
void Num2NumStr(u32 Num, u8 *NumStr);
void RTC_GetCurTime(TIME_T *tt);
ErrorStatus CheckTimeStruct(TIME_T *tt);
void ReadTime(void);
/*************************************************************
** ��������: Public_CheckTimeIsInRange
** ��������: �ж�ָ��ʱ���Ƿ���ָ����Χ��
** ��ڲ���: curTimeָ��ʱ��,startTime��ʼʱ��,endTime����ʱ��
** ���ڲ���: 
** ���ز���: 1:�ڷ�Χ�ڣ�0:���ڷ�Χ�� 
** ȫ�ֱ���: ��
** ����ģ��: 
*************************************************************/
unsigned char Public_CheckTimeIsInRange(unsigned long curTime,unsigned long startTime,unsigned long endTime);

#endif

