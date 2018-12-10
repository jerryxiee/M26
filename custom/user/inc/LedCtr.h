#ifndef	__LEDCTR_H
#define	__LEDCTR_H


typedef enum 
{
	LED_STATUS_POWER_ON = 0,	//����
	LED_STATUS_POWER_NORMAL = LED_STATUS_POWER_ON,//����״̬
	LED_STATUS_POWER_DOWN,		//�ػ�
	LED_STATUS_POWER_LOW,		//�͹���
	LED_STATUS_POWER_CHARGE		//���
}LED_STATUS;
typedef struct
{
	u8 state;
	void(*ledCtrlCB)(u8 sta);
}LED_CTRL_DEF;


void Led_Init(void);
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
void LowVol_UpdatePram(void);

//led״̬�仯
void Led_Status_Change(u8 sta);

u16 uart_get_busy_status(void);
void uart_busy_time_reduce(void);
void uart_set_default_mode(void);

#endif



