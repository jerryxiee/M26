#ifndef _SLEEPCTR_H_
#define _SLEEPCTR_H_



//ÿ���ϵ������ȡ�����߱�־
void SleepInit(void);

//��ȡ���߱�־
u8 ReadSleepFlag();


//д�����߱�־
void SetSleepFlag(u8 flag);


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
//��ע		:
*********************************************************************/
FunctionalState Sleep_TimeTask(void);



#endif
