/*******************************************************************************
 * File Name    :	Version.h
 * File Describe:	�汾��������汾��ض���
 * Relate Module:	Version.h
 * Writer		:   joneming
 * Date			:   2014-04-11
**********************************************************************************/
#ifndef __VERSION_H_
#define __VERSION_H_
/******************************************************************************/
/****************************************************/
/*                                                  */
/* Definitions                                      */
/*                                                  */
/****************************************************/
#define _STR(s)             #s					    //# ������ܽ���Ĳ���ת��Ϊ��˫���ŵ��ַ�����
#define STR(s)              _STR(s)
/****************************************************/
/*                                                  */
/* Function declarations                            */
/*                                                  */
/****************************************************/
#ifndef FIRMWARE_TIME
extern unsigned char FIRMWARE_TIME[];
#endif
/********************************************
 * Function: Version_UpdateSoftVersion
 * Describe: ���µ�ǰ����汾��
**********************************************/
void Version_UpdateSoftVersion(void);
/********************************************
 * Function: Version_GetFirmwareInformation
 * Describe: ��ȡ��ǰ�̼���Ϣ(�汾��+��������)
 * ����    : ����
**********************************************/
unsigned char Version_GetFirmwareInformation(unsigned char *buffer);
/********************************************
 * Function: Version_GetFirmwareInformation
 * Describe: ��ȡ��ǰӲ����Ϣ(�汾��)
 * ����    : ����
**********************************************/
unsigned char Version_GetHardwareInformation(unsigned char *buffer);
/********************************************
 * Function: Version_GetProductSeriesName
 * Describe: ��ȡ��ǰ��Ʒϵ������
 * ����    : ����
**********************************************/
unsigned char Version_GetProductSeriesName(char *buffer);
/********************************************
 * Function: Version_GetEquipmentCompany
 * Describe: ��ȡ��ǰ�豸��˾���
 * ����    : ����
**********************************************/
unsigned char Version_GetEquipmentCompany(char *buffer);
/********************************************
 * Function: Version_GetEquipmentSoftVer
 * Describe: ��ȡ��ǰ�豸�汾��
 * ����    : ����
**********************************************/
unsigned char Version_GetEquipmentSoftVer(char *buffer);
#endif//__VERSION_H_
/*******************************************************************************
 *                             end of module
 *******************************************************************************/
 
