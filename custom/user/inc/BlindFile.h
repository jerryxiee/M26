#ifndef _BLINDFILE_H
#define _BLINDFILE_H

#define BLINDFILENAME   "BlindFile.txt"

#define	FLASH_ONE_SECTOR_BYTES	4096//ÿ������4096�ֽ�
#define	FLASH_BLIND_START_SECTOR            0//ä��,18����
#define FLASH_BLIND_END_SECTOR              8//500��ä��,Ԥ��1�����Ա��洢����
#define FLASH_BLIND_STEP_LEN                124


//����ä����ȡ�ļ�
void CreatBlindFile(void);


//д��ָ����ַ�����ֽ�
void sFLASH_ReadBuffer(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);

//��ָ����ַ��ȡ�����ֽ�
void sFLASH_WriteBuffer(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);

//������
void sFLASH_EraseSector(u32 SectorAddr);



#endif



