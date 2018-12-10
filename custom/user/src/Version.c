/*******************************************************************************
 * File Name:           Version.c
 * Function Describe:   
 * Relate Module:		Version.h
 * Writer:              
 * Date:                
 * Rewriter:            
 * �޸�ʱ��             
 ******************************************************************************/


/*******************************************************************************

 ******************************************************************************/

#include "include.h"

//////////////////////////
#define SOFTVERSION                 02//�汾��:��λ:��ʾ��ǰ�汾��
/////////////////////////////////////////
#define VER_FIRMWARE_TIME           "201805121540"//����ʱ��
///////////////////////////////////////////////////
///////////////////////////////////////////////
#define HARDWARE_VERSION            "Ver:1.0"//Ӳ���汾��
//////////////////////////////////////////////////
#define NETWORK_TYPE                01//��ʾϵ��,��汾
#define NETWORK_MODE_NAME           "G"
#define PRODUCT_SERISE_NAME         "VTKG-22A2"//��Ʒϵ������:8���ֽ�//
/////////////////////////////////
//////////���º겻��Ҫ�޸�///////
//////////////////////////////////////////////
#ifdef OPEN_DEBUG_TEST_MODE        //���԰汾,�����޸�,��ֹ���˿����Ź�
#define SOFTVERSIONS                STR(01)//�汾��:��λ:��ʾ��ǰ�汾��
#else
#define SOFTVERSIONS                STR(SOFTVERSION)
#endif
#define NETWORK_TYPES               STR(NETWORK_TYPE)
#define FIRMWARE_VERSION            "4"NETWORK_TYPES""SOFTVERSIONS//�汾�Ź�5λ���֣�//�̼��汾��,2��ʾ702,01��ʾG��ͨ�ð汾,02��ʾC��ͨ�ð�(��Ϊģ��),04��ʾCͨ�ð�(����ģ��)
///////////////////////////////
#define FIRMWARE_INFORMATION        PRODUCT_SERISE_NAME"_Ver:"FIRMWARE_VERSION"_"VER_FIRMWARE_TIME//�̼���Ϣ(�����汾�ż�����)
///////////////////////////////////
#define HARDWARE_INFORMATION        HARDWARE_VERSION"_"NETWORK_MODE_NAME
//////////////////////////////////////////////
#define EQUIPMENT_COMPANY           "EI"
#define EQUIPMENT_SOFT_VER          "0"FIRMWARE_VERSION
#define EQUIPMENT_MANUFACTURER      "MANUFACTURER:"EQUIPMENT_COMPANY
#define EQUIPMENT_PRODUCT_MODEL     "PRODUCT MODEL:"PRODUCT_SERISE_NAME//8���ֽ�
#define EQUIPMENT_PCB_VERSION       "PCB VERSION:A "//��1���ո�
#define EQUIPMENT_SOFTWARE_VERSION  "SOFTWARE VERSION:"EQUIPMENT_SOFT_VER
#ifndef FIRMWARE_TIME
unsigned char FIRMWARE_TIME[]={VER_FIRMWARE_TIME};
#endif
/********************************************
 * Function: Version_UpdateSoftVersion
 * Describe: ���µ�ǰ����汾��
**********************************************/
void Version_UpdateSoftVersion(void)
{
    //Ϊ����ʱ����MANUFACTURER���������Ż�����ȥ���ˣ����������´���
    ////////////////////
    if(strncmp(EQUIPMENT_MANUFACTURER,EQUIPMENT_PRODUCT_MODEL,6)||strncmp(EQUIPMENT_PCB_VERSION,EQUIPMENT_SOFTWARE_VERSION,6))//һ������
    {
        //Public_SetOperateTimer(SysCtrl_DisposeTrackResetSystem,PUBLICSECS(20));//���º󣬷�ֹRTC������Ҫ�ȴ�20�������ϵͳʱ�����ж�
    }
}
/********************************************
 * Function: Version_GetFirmwareInformation
 * Describe: ��ȡ��ǰ�̼���Ϣ(�汾��+��������)
 * ����    : ����
**********************************************/
unsigned char Version_GetFirmwareInformation(unsigned char *buffer)
{
    strcpy((char *)buffer,FIRMWARE_INFORMATION);
    return strlen(FIRMWARE_INFORMATION);
}
/********************************************
 * Function: Version_GetFirmwareInformation
 * Describe: ��ȡ��ǰӲ����Ϣ(�汾��)
 * ����    : ����
**********************************************/
unsigned char Version_GetHardwareInformation(unsigned char *buffer)
{
    strcpy((char *)buffer,HARDWARE_VERSION);
    return strlen(HARDWARE_VERSION);
}
/********************************************
 * Function: Version_GetProductSeriesName
 * Describe: ��ȡ��ǰ��Ʒϵ������
 * ����    : ����
**********************************************/
unsigned char Version_GetProductSeriesName(char *buffer)
{
    strcpy((char *)buffer,PRODUCT_SERISE_NAME);
    return strlen(PRODUCT_SERISE_NAME);
}
/********************************************
 * Function: Version_GetEquipmentCompany
 * Describe: ��ȡ��ǰ�豸��˾���
 * ����    : ����
**********************************************/
unsigned char Version_GetEquipmentCompany(char *buffer)
{
    strcpy((char *)buffer,EQUIPMENT_COMPANY);
    return strlen(EQUIPMENT_COMPANY);
}
/********************************************
 * Function: Version_GetEquipmentSoftVer
 * Describe: ��ȡ��ǰ�豸�汾��
 * ����    : ����
**********************************************/
unsigned char Version_GetEquipmentSoftVer(char *buffer)
{
    strcpy((char *)buffer,EQUIPMENT_SOFT_VER);
    return strlen(EQUIPMENT_SOFT_VER);
}
/******************************************************************************
								EndModule
******************************************************************************/
