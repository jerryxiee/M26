#include "include.h"
//#include "ComParam.h"

#define PARAM_UART_PORT UART_PORT1//���ڲ��������շ��Ĵ���
/*******************************************************************************
* Function Name  : ComParam_Error_Cmd
* Description    : �򴮿ڷ������ݴ���ʱ��Ӧ��֡����
*                  �ʹ���֡����
* Input          : - Cmd  : ��ʻ��¼��������              
* Output         : None
* Return         : None
*******************************************************************************/
void ComParam_Error_Cmd(u8 Cmd)
{
    u8 Buffer[5];
    Buffer[0] = 0x55;
    Buffer[1] = 0x7a;   
    if(Cmd < 0x80)                     /*�����������Ӧ��    */
    {
        Buffer[2] = 0xfa;        
    }
    else                                                /*�´��������Ӧ��    */
    {
        Buffer[2] = 0xfb;
    }
    Buffer[3] = 0;
    Buffer[4] = Public_GetXorVerify(Buffer, 4);
    Ql_UART_Write(PARAM_UART_PORT,Buffer,5);
}
/*******************************************************************************
* Function Name  : ComParam_Handle
* Description    : ������ʻ��¼�����ݴ���
* Input          : None              
* Output         : None
* Return         : None
*******************************************************************************/
void ComParam_DisposeParamCmd(u8 cmd ,u8 *pRec,u16 length)
{
    s16 nLen=0;                                      /*�����ݳ���             */
    u8  verify;                                      /*��������У���ֽ�       */
    u8	*pSend;                                      /*��������ָ��           */
    u8  buffer[200];
    u8 flag;
    flag=*(pRec+6);
    pSend = buffer;                                    /*��ʼ���������ݵ�ַ     */
    if(0 == flag)                       /*������                 */
    {
        E2ParamApp_DisposeReadParam(pSend+7, &nLen,pRec+7, length-1);
        nLen++;                              /*���϶���־             */
    }
    else //д����
    {
        verify=E2ParamApp_DisposeWriteParam(pRec+7, length-1);
        if(verify)return;
        nLen = 1;
    }
    /////////////////////////////////////
    *pSend     = 0x55;
    *(pSend+1) = 0x7a;
    *(pSend+2) = cmd;
    *(pSend+3) = nLen >> 8;              /*����                   */
    *(pSend+4) = nLen;
    *(pSend+5) = 0x12;
    *(pSend+6) = flag;                      /*��д��־                 */
    ///////////////
    verify = Public_GetXorVerify(pSend,nLen+6);
    /////////
    *(pSend+6+nLen) = verify;
    //ComParam_SendData(pSend, nLen+7);
    Ql_UART_Write(PARAM_UART_PORT,pSend,nLen+7);
}
/*******************************************************************************
* Function Name  : ComParam_DisposeRecvData
* Description    : ��ʻ��¼�����ݷ���
* Input          : - *pData  : ָ�����������
*                  - Length  : �������ݳ���  
* Output         : None
* Return         : - �յ�һ֡��������ȷ���ݷ����棬���򷵻ؼ�
*******************************************************************************/
unsigned char ComParam_DisposeRecvData(unsigned char *pRecvData,unsigned short Length)
{
    unsigned char verify,cmd;
    unsigned short len;
    if(Length<7)return 1;
    /////////////////
    if((0xAA != pRecvData[0])||(0x75 != pRecvData[1]))return 1;
    //////////////////
    cmd = pRecvData[2];
    ////////////
    len = Public_ConvertBufferToShort(&pRecvData[3]);
     ////////////////
    if((0xd0 != cmd)||(pRecvData[6] > 1))return 2;//���ǲ�����д
    ///////////
    if(len+7>Length)//���Ȳ���
    {
        ComParam_Error_Cmd(cmd);                      /*�򴮿ڷ��ʹ���֡����  */   
        return 3;
    }
    ///////////
    verify=Public_GetXorVerify(pRecvData, len+6);
    if(verify != pRecvData[len+6])//У�鲻��
    {
        ComParam_Error_Cmd(cmd);                      /*�򴮿ڷ��ʹ���֡����  */   
        return 4;
    }  
    /////////////////
    ComParam_DisposeParamCmd(cmd,pRecvData,len);
    ///////////////////
    return 0;
}

