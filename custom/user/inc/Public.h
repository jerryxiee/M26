#ifndef __PUBLIC_H_
#define __PUBLIC_H_


#define PUBLICSECS(x)	(unsigned long)((x) * SYSTICK_1SECOND)  //

#define STEP_VAL    (float)(10.0)
#define STEP_FLT    (float)(0.1)
#define SIGN_FLG    (float)(-1.0)
#define PUBLIC_CHECKBIT(X,N) ((X) & (1ul << (N)))	   //����ĳһλ
#define PUBLIC_SETBIT(X,N) (X)=((X) | (1ul<<(N)))       //����ĳһλ
#define PUBLIC_CLRBIT(X,N) (X)=((X) & (~(1ul<<(N))))    //���ĳһλ
////////////////////////////////////
#define PUBLIC_CHECKBIT_EX(X,N) (PUBLIC_CHECKBIT(X,N)?1:0)	   //����ĳһλ,����ֵֻ��0��1

/*************************************************************OK
** ��������: Public_GetSumVerify
** ��������: �����ݿ��ۼ���ͣ�ֻȡ���ֽ�
** ��ڲ���: Pdata���ݿ��׵�ַ,datalen����		 
** ���ڲ���: 
** ���ز���: У����
** ȫ�ֱ���: ��
** ����ģ��: ��
*************************************************************/
unsigned char Public_GetSumVerify(const unsigned char *Pdata,unsigned short datalen);
/*************************************************************
** ��������: Public_ConvertLongToBuffer
** ��������: ��unsigned longת��4λunsigned char,��λ��ǰ
** ��ڲ���: value:��Ҫת��������
** ���ڲ���: buf:ת��������׵�ַ
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
void Public_ConvertLongToBuffer(unsigned long value,unsigned char *buf);
/*************************************************************
** ��������: Public_ConvertAscToBCD
** ��������: ��asciii��ת��BCD��,(�������ֽڵ�asciii��ת��һ���ֽڵ�BCD��)
** ��ڲ���: destinĿ�����ݵ��׵�ַ,sourceԴ���ݵ��׵�ַ,len��Ҫת����Դ���ݳ���
** ���ڲ���: destinĿ�����ݵ��׵�ַ
** ���ز���: ��
** ȫ�ֱ���: destinʹ��ʱע��destin�Ŀռ����
** ����ģ��: 
** ��    ��: 0x31 0x32 0x33 0x34 0x35 0x36 =>0x12 34 56
*************************************************************/	
void Public_ConvertAscToBCD(unsigned char * destin,const unsigned char* source, unsigned char len);
/*************************************************************
** ��������: Public_ConvertBcdToAsc
** ��������: ��BCD���ַ���ÿһ���ֽ�ת��������AscII����ַ���
** ��ڲ���: pAscת��֮��������׵�ַ,pBcd��Ҫת����BCD�����׵�ַ��BcdLen:��Ҫת�������ݳ���
** ���ڲ���: pAscת��֮��������׵�ַ
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: ע��ռ����
** ��    ��: 0x12 34 56=>0x31 0x32 0x33 0x34 0x35 0x36
*************************************************************/	
void Public_ConvertBcdToAsc(unsigned char *pAsc, const unsigned char *pBcd, unsigned char BcdLen);
/*************************************************************
** ��������: Public_ConvertBcdToValue
** ��������: ��BCD���ַ���ת��������ֵ���ַ���
** ��ڲ���: pValueת��֮��������׵�ַ,pBcd��Ҫת����BCD�����׵�ַ��BcdLen:��Ҫת�������ݳ���
** ���ڲ���: pValueת��֮��������׵�ַ
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: ע��ռ����
** ��    ��: 0x12 34 56=>12 34 56
*************************************************************/	
void Public_ConvertBcdToValue(unsigned char *pValue, const unsigned char *pBcd, unsigned char BcdLen);
/*************************************************************
** ��������: Public_ConvertBufferToLong
** ��������: ��4λunsigned char,��λ��ǰ������ת��unsigned long;
             (��Public_ConvertLongToBuffer�����෴)
** ��ڲ���: buf:��Ҫת�������ݵ��׵�ַ
** ���ڲ���: 
** ���ز���: ת�����
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
unsigned long Public_ConvertBufferToLong(unsigned char *buf);
/*************************************************************
** ��������: Public_ConvertBufferToShort
** ��������: ��2λunsigned char,��λ��ǰ������ת��unsigned short;
** ��ڲ���: buf:��Ҫת�������ݵ��׵�ַ
** ���ڲ���: 
** ���ز���: ת�����
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
unsigned short Public_ConvertBufferToShort(unsigned char *buf);

/*************************************************************
** ��������: Public_ConvertShortToBuffer
** ��������: ��unsigned shortת��2λunsigned char,��λ��ǰ
** ��ڲ���: value:��Ҫת��������
** ���ڲ���: buf:ת��������׵�ַ
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
void Public_ConvertShortToBuffer(unsigned short value,unsigned char *buf);
/*************************************************************
** ��������: hextostr
** ��������: ��unsigned intת���ַ�����ʽ
** ��ڲ���: hex:��Ҫת��������len:ת���ĳ��ȣ�_strת����Ĵ��ָ��
** ���ڲ���: _str:ת��������׵�ַ��ת����ĳ���
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
unsigned char HexToStr(unsigned int hex,unsigned char len, void *_str);
/*************************************************************
** ��������: Public_CheckArrayValIsEqual
** ��������: �ж����������Ƿ����,
** ��ڲ���: array��Ҫ�жϵ�����1�׵�ַ,array1��Ҫ�жϵ�����2�׵�ַ,len��Ҫ�жϵĳ���
** ���ڲ���: ��
** ���ز���: 0���,1�����;
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
unsigned char Public_CheckArrayValIsEqual(const unsigned char *array,const unsigned char *array1,unsigned short len);
/*************************************************************
** ��������: Public_GetXorVerify
** ��������: �����ݿ�������ֻȡ���ֽ�
** ��ڲ���: Pdata���ݿ��׵�ַ,datalen����		 
** ���ڲ���: 
** ���ز���: У����
** ȫ�ֱ���: ��
** ����ģ��: ��
*************************************************************/
unsigned char Public_GetXorVerify(const unsigned char *Pdata,unsigned short datalen);
/*************************************************************
** ��������: Public_atof
** ��������: ���ַ���ת����ֵ
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: ��
** ����ģ��: ��
*************************************************************/
float Public_atof(const char*str); 
/*************************************************************
** ��������: Public_itoa
** ��������: ����ֵת���ַ���(�����Ǹ���)
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: ��
** ����ģ��: ��
*************************************************************/
/*********************************************************************
** �� �� ����
**********************************************************************/

unsigned char dectostr(unsigned int decnum,void *_str,unsigned char _bit);

unsigned char Public_itoa(unsigned char *str,unsigned long val,unsigned char n);

#define Public_itoaEx(str,val)  Public_itoa(str,val,0)
/*************************************************************
** ��������: Public_BCD2HEX
** ��������: BCDת��ΪByte        
** ��ڲ���: val:��Ҫת��������
** ���ڲ���: ��
** ���ز���: ת�����
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
unsigned char Public_BCD2HEX(unsigned char val);    //BCDת��ΪByte
/*************************************************************
** ��������: asctodec
** ��������: 
** ��ڲ���: 
** ���ڲ���:
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
int asctodec(unsigned char *asc, unsigned char  len, signed char *_err);

/*************************************************************
** ��������: hextostr
** ��������: ��unsigned intת���ַ�����ʽ
** ��ڲ���: hex:��Ҫת��������len:ת���ĳ��ȣ�_strת����Ĵ��ָ��
** ���ڲ���: _str:ת��������׵�ַ��ת����ĳ���
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
unsigned long asctohex(unsigned char *asc, unsigned char len, signed char *_err);

/*************************************************************
** ��������: Public_HEX2BCD
** ��������: Byte��ת��ΪBCD��    
** ��ڲ���: val:��Ҫת��������
** ���ڲ���: ��
** ���ز���: ת�����
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
unsigned char Public_HEX2BCD(unsigned char val);    //Byte��ת��ΪBCD��
/*************************************************************
** ��������: Public_Mymemcpy
** ��������: src�����ݿ���des��,nΪ��������,flagΪ����(1:����,0:˳��)
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: ��
** ����ģ��:
*************************************************************/
void *Public_Mymemcpy(void *des,  const void *src, unsigned short len, unsigned char flag);

unsigned long Public_CRC_32( unsigned char * aData, unsigned long aSize );

#endif
