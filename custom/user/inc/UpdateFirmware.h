#ifndef  _UPDATAFIRMWARE_H_
#define  _UPDATAFIRMWARE_H_

/*
********************************************************************************
*                                STRUCT
********************************************************************************
*/
typedef struct {
    u8  flag;//ftpæ��־��������ʱ��0
    u8  ip[15];//ip��ַ
    u8  port[5];//�˿�
    u8  user[20];//����������
    u8  pass[20];//����������    
    u8  fpath[50];//�ļ�·��
    u8  fname[50];//�ļ���
    u16 pageAddr;
    u32 address;
}APP_FTP;


/*
  * @brief  ��ʼ����ftp�ļ�����
  * @param  None
  * @retval �����Ƿ�ִ������, 1Ϊ������0Ϊ���������ⲻ����
  */
u8 Net_Other_FtpStart(void);
/*********************************************************************
//��������	:Updata_SendResultNoteTimeTask
//����		:�����������֪ͨ��ƽ̨
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:
//���ú���	:
//�ж���Դ	:
//����		:
//��ע		:Ĭ���ǿ�����,1�����1��
*********************************************************************/
void Updata_SendResultNoteTimeTask(void);

u8 Net_Ftp_Parameter(u8 ParaType,u8 *tab,u8 tab_len);
void ftp_downfile_timer(void);
//��������־
u8 GetUpdateFlag(void);
//д������־
void SetUpdateFlag(u8 sta);

#endif

