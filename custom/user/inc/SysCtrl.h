#ifndef __SYSCTRL__H_
#define __SYSCTRL__H_


////////////////////////////////////////////////////////////////////IP�����ṹ��
typedef struct
{
    ST_GprsConfig m_GprsConfig;//GPRS PDP context     
    u8  mode[4];//TCP��UDP    
    u8  ip[30]; //IP������
    u32  port;//�˿�
}SOC_IPVAL;//IP�����ṹ��



/**
  * @brief  ���ͻ������ݵ�����
  * @param    channel:����ͨ��
  * @param    srcdat :ָ��������
  * @param    srclen :�������ݳ���
  * @retval ���ط��ͽ��
  *                              0 :ʧ��  
  *                     ����srclen :�ɹ�
  */
u16 Communication_SndMsg2Svr(u8 channel, u8 * srcdat, u16 srclen);

/*
  * @brief  ������
  * @param  None
  * @retval None
  */
void Net_First_Open(void);
/*
  * @brief  �ر�����
  * @param  None
  * @retval None
  */
void Net_First_Close(void);




#endif

