#include "include.h"
//****************�궨��********************

//****************�ⲿ����*******************

//****************ȫ�ֱ���*******************
const u16 GPIO_OUT_IN_PIN[GPIO_OUT_IN_MAX] = 
{
    GPRS_LED_PIN,
	RESET_KEY_PIN,
    WATCHDOG_PIN,
    REMOTE_SHUTDOWN_PIN,
    OPEN_PULL_LOW_PIN,
    WDT_RESET_PIN
};
/****************��������*******************/

/****************��������******************
//��������	:GpioOutInit
//����		:��ʼ��GPIO�������
//����		:IoNum ����ֵ,�ο�GPIO_OUT_IN_NUMö��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void GpioOutInit(GPIO_OUT_IN_NUM IoNum,Enum_PinLevel level)
{
	s32 ret;
    // Initialize the GPIO pin (output high level, pull up)
    Ql_GPIO_Uninit(GPIO_OUT_IN_PIN[IoNum]);
    ret=Ql_GPIO_Init(GPIO_OUT_IN_PIN[IoNum], PINDIRECTION_OUT, level, PINPULLSEL_PULLDOWN);
	
	APP_DEBUG("\r\n<-- GpioOutInit PIN=%d ret%d -->\r\n",GPIO_OUT_IN_PIN[IoNum],ret);
  	//GPIO_Init(GPIO_OUT_IN_PORT[IoNum], &GPIO_InitStructure);
}
/****************��������******************
//��������	:GpioInInit
//����		:��ʼ����������
//����		:IoNum ����ֵ,�ο�GPIO_OUT_IN_NUMö��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void GpioInInit(GPIO_OUT_IN_NUM IoNum,Enum_PinLevel level)
{
	s32 ret;
	Ql_GPIO_Uninit(GPIO_OUT_IN_PIN[IoNum]);

    // Initialize the GPIO pin (output high level, pull up)
    ret=Ql_GPIO_Init(GPIO_OUT_IN_PIN[IoNum], PINDIRECTION_IN, level, PINPULLSEL_DISABLE);
	Ql_GPIO_SetDirection(GPIO_OUT_IN_PIN[IoNum],PINDIRECTION_IN);
	APP_DEBUG("\r\n<-- GpioInInit PIN=%d ret%d -->\r\n",GPIO_OUT_IN_PIN[IoNum],ret);
   //GPIO_Init(GPIO_OUT_IN_PORT[IoNum], &GPIO_InitStructure);
}

/****************��������******************
//��������	:GpioOutOn
//����		:����GPIO�������Ϊ��(������1)
//����		:IoNum ����ֵ,�ο�GPIO_OUT_IN_NUMö��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void GpioOutOn(GPIO_OUT_IN_NUM IoNum)
{
    Ql_GPIO_SetLevel(GPIO_OUT_IN_PIN[IoNum], PINLEVEL_HIGH);
 	// GPIO_OUT_IN_PORT[IoNum]->BSRR = GPIO_OUT_IN_PIN[IoNum];
}
/****************��������******************
//��������	:GpioOutOff
//����		:����GPIO�������Ϊ��(������0)
//����		:IoNum ����ֵ,�ο�GPIO_OUT_IN_NUMö��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void GpioOutOff(GPIO_OUT_IN_NUM IoNum)
{
    Ql_GPIO_SetLevel(GPIO_OUT_IN_PIN[IoNum], PINLEVEL_LOW);
   //GPIO_OUT_IN_PORT[IoNum]->BRR = GPIO_OUT_IN_PIN[IoNum];  
}

/****************��������******************
//��������	:GpioInGetState
//����		:��ȡ��������״̬
//����		:IoNum ����ֵ,�ο�GPIO_OUT_IN_NUMö��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:��ǰIO��״ֵ̬,0��1
//��ע		:
*********************************************************************/
u8 GpioInGetState(GPIO_OUT_IN_NUM IoNum)
{
	return Ql_GPIO_GetLevel(GPIO_OUT_IN_PIN[IoNum]);
    //return GPIO_ReadInputDataBit((GPIO_TypeDef *)GPIO_OUT_IN_PORT[IoNum], GPIO_OUT_IN_PIN[IoNum]);
}
/****************��������******************
//��������	:GpioOutGetState
//����		:��ȡ�������״̬
//����		:IoNum ���ֵ,�ο�GPIO_OUT_IN_NUMö��
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:��ǰIO��״ֵ̬,0��1
//��ע		:
*********************************************************************/
u8 GpioOutGetState(GPIO_OUT_IN_NUM IoNum)
{
	return Ql_GPIO_GetLevel(GPIO_OUT_IN_PIN[IoNum]);
    //return GPIO_ReadOutputDataBit((GPIO_TypeDef *)GPIO_OUT_IN_PORT[IoNum], GPIO_OUT_IN_PIN[IoNum]);
}

