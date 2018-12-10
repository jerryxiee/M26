#ifndef __GPIOCONTROL_H
#define __GPIOCONTROL_H
//**************�����*******************   
#include "ql_gpio.h"

#define GPRS_LED_PIN               			PINNAME_NETLIGHT//GPRSָʾ��  
#define RESET_KEY_PIN						PINNAME_DCD
#define WATCHDOG_PIN                        PINNAME_CTS//���Ź�
#define REMOTE_SHUTDOWN_PIN					PINNAME_PCM_SYNC//Զ�̹ػ�
#define OPEN_PULL_LOW_PIN					PINNAME_DTR	//����������
#define WDT_RESET_PIN						PINNAME_PCM_CLK//���Ź���λ��

#define WDT_RESET_PIN_SETHIGH()				Ql_GPIO_SetLevel(WDT_RESET_PIN, PINLEVEL_HIGH)//���Ź���λ������
#define WDT_RESET_PIN_SETLOW()				Ql_GPIO_SetLevel(WDT_RESET_PIN, PINLEVEL_LOW)//���Ź���λ������
#define REMOTE_SHUTDOWN()					Ql_GPIO_SetLevel(REMOTE_SHUTDOWN_PIN, PINLEVEL_HIGH)
#define HARD_FEED_DOG()						Ql_GPIO_SetLevel(WATCHDOG_PIN, PINLEVEL_LOW)//ι��
#define HARD_FEED_DOG1()					Ql_GPIO_SetLevel(WATCHDOG_PIN, PINLEVEL_HIGH)//ι��
#define RESET_KEY_PIN_HIGH()				Ql_GPIO_SetLevel(RESET_KEY_PIN, PINLEVEL_HIGH)//�ϵ�3s����

//****************�ṹ����*******************   
//�����,����ű��
typedef enum 
{  
    GPRS_LED=0,          //GPRS_LED,���
    RESET_KEY,			
    WATCHDOG,			//���Ź�
    REMOTE_POWERDOWN,
    OPEN_PULL_LOW,
    WDT_RESET,
    GPIO_OUT_IN_MAX,
}GPIO_OUT_IN_NUM;  


//****************��������*******************
void GpioOutInit(GPIO_OUT_IN_NUM IoNum,Enum_PinLevel level);
void GpioOutOn(GPIO_OUT_IN_NUM IoNum);
void GpioOutOff(GPIO_OUT_IN_NUM IoNum);
void GpioOutToggle(GPIO_OUT_IN_NUM IoNum);

void GpioInInit(GPIO_OUT_IN_NUM IoNum,Enum_PinLevel level);
u8 GpioInGetState(GPIO_OUT_IN_NUM IoNum);
u8 GpioOutGetState(GPIO_OUT_IN_NUM IoNum);

#endif /* __GPIOCONTROL_H */

