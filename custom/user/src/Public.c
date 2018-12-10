#include "include.h"


/*************************************************************OK
** ��������: Public_GetSumVerify
** ��������: �����ݿ��ۼ���ͣ�ֻȡ���ֽ�
** ��ڲ���: Pdata���ݿ��׵�ַ,datalen����		 
** ���ڲ���: 
** ���ز���: У����
** ȫ�ֱ���: ��
** ����ģ��: ��
*************************************************************/
unsigned char Public_GetSumVerify(const unsigned char *Pdata,unsigned short datalen)
{
    //�����ݿ��ۼ���ͣ�ֻȡ���ֽ�
    unsigned char sum = 0;
    unsigned short  i;
    for(i = 0; i < datalen; i++)
    {
        sum += Pdata[i];
    }
    return sum;	  
}
/*************************************************************
** ��������: Public_GetXorVerify
** ��������: �����ݿ�������ֻȡ���ֽ�
** ��ڲ���: Pdata���ݿ��׵�ַ,datalen����		 
** ���ڲ���: 
** ���ز���: У����
** ȫ�ֱ���: ��
** ����ģ��: ��
*************************************************************/
unsigned char Public_GetXorVerify(const unsigned char *Pdata,unsigned short datalen)
{
    //�����ݿ�������ֻȡ���ֽ�
    unsigned char Verify ;
    unsigned short  i;
    //////////////////
    Verify= Pdata[0];
    //SysCtrl_FeedDog();//ι��
    for(i = 1; i < datalen; i++)
    {
        Verify ^= Pdata[i];
    }
    //SysCtrl_FeedDog();//ι��
    return Verify;	  
}
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
void Public_ConvertAscToBCD(unsigned char * destin,const unsigned char* source, unsigned char len)
{
    unsigned char i;
    unsigned char chtemp,chhigh,chlow;
    for(i = 0; i < len; i+=2)
    {
        chtemp = *source++;
        chhigh = chtemp & 0x0F;
        chtemp = *source++;
        chlow = chtemp & 0x0F;
        chhigh <<= 4; 
        chhigh |= chlow;
        *destin++ = chhigh;
    }
}
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
void Public_ConvertBcdToAsc(unsigned char *pAsc, const unsigned char *pBcd, unsigned char BcdLen)
{
    unsigned char i;
    for(i=0; i<BcdLen; i++)
    {
        *(pAsc+2*i) = (*(pBcd+i) >> 4)+0x30;
        *(pAsc+2*i+1) = (*(pBcd+i) & 0x0f)+0x30;
    }
}
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
void Public_ConvertBcdToValue(unsigned char *pValue, const unsigned char *pBcd, unsigned char BcdLen)
{
    unsigned char	i;
    for(i=0; i<BcdLen; i++)
    {
        *(pValue+i) = (*(pBcd+i) >> 4)*10+(*(pBcd+i) & 0x0f);
    }
}

/*************************************************************
** ��������: Public_ConvertLongToBuffer
** ��������: ��unsigned longת��4λunsigned char,��λ��ǰ
** ��ڲ���: value:��Ҫת��������
** ���ڲ���: buf:ת��������׵�ַ
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
void Public_ConvertLongToBuffer(unsigned long value,unsigned char *buf)
{
    unsigned char i;
    for(i= 0; i<4; i++)
    {
        buf[3-i]=value>>(i<<3);
    }
}
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
unsigned long Public_ConvertBufferToLong(unsigned char *buf)
{
    unsigned long value;    
    value = 0;
    value |= buf[0]<<24;
    value |= buf[1]<<16;
    value |= buf[2]<<8;
    value |= buf[3];
    return value;
}
/*************************************************************
** ��������: Public_ConvertBufferToShort
** ��������: ��2λunsigned char,��λ��ǰ������ת��unsigned short;
** ��ڲ���: buf:��Ҫת�������ݵ��׵�ַ
** ���ڲ���: 
** ���ز���: ת�����
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
unsigned short Public_ConvertBufferToShort(unsigned char *buf)
{
    unsigned short value;
    value = 0;
    value |= buf[0]<<8;
    value |= buf[1];
    return value;
}

/*************************************************************
** ��������: Public_ConvertShortToBuffer
** ��������: ��unsigned shortת��2λunsigned char,��λ��ǰ
** ��ڲ���: value:��Ҫת��������
** ���ڲ���: buf:ת��������׵�ַ
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
void Public_ConvertShortToBuffer(unsigned short value,unsigned char *buf)
{
    buf[0] = value>>8;
    buf[1] = value&0xff;
}
/*************************************************************
** ��������: hextostr
** ��������: ��unsigned intת���ַ�����ʽ
** ��ڲ���: hex:��Ҫת��������len:ת���ĳ��ȣ�_strת����Ĵ��ָ��
** ���ڲ���: _str:ת��������׵�ַ��ת����ĳ���
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
unsigned char HexToStr(unsigned int hex,unsigned char len, void *_str)
{
   unsigned char i;
   unsigned int mask;
   unsigned int tmp;
   char *str = _str;
   
	*str = '\0';
	
   if((len==0)||(len>8))return 0; 
   mask	= 0x0000000F << ((len-1)*4);

	for(i=0;i<len;i++)
	{
		tmp = (mask >> (i*4) ) & hex;
		tmp >>= ((len-1)-i)*4;
		if(tmp<=9)
	 	{
	    	str[i]=tmp+'0';
	 	}
	 	else
	    	str[i]=tmp+'A'-10;
		str[i+1]='\0';
	}

   	return strlen(str);
}
/*************************************************************
** ��������: asctodec
** ��������: 
** ��ڲ���: 
** ���ڲ���:
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
int asctodec(u8 *asc, u8  len, signed char *_err)
{
	int retv=0;
	u8 i=0;
	u8 flg = 0;
	signed char err = 0;
	
	if(len==0)
	{
		err = 1;
	}
	else
	{
		if(*asc == '-')
		{
		  asc++;
		  flg = 1;
		}
		
		for(i = 0; i < len; i++)
		{
			if(*asc > '9' || *asc < '0')
			{
				err = 1;
				break;
			}
	
			retv *= 10;
	
			retv += *asc - '0';
	
			asc++;
		}
	
		if(flg)
		  retv = 0-retv;
	
	}
    
    if(_err != NULL)
	  *_err = err;
	
	return retv;
}


/*************************************************************
** ��������: asctohex
** ��������: 
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: ��
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/
unsigned long asctohex(u8 *asc, unsigned char len, signed char *_err)
{
	unsigned long retv=0;
	unsigned char i=0;
    signed char err = 0;
	
   	if(len==0)
	{
		err = 1;
	}
    else
	{
		for(i = 0; i < len; i++)
		{
			if(*asc >= '0' && *asc <= '9')
			{
			   retv <<= 4;
			   retv += *asc - '0';
			}
			else if(*asc >= 'a' && *asc <= 'f')
			{
			   retv <<= 4;
			   retv += *asc - 'a' + 10;
			}
			else if(*asc >= 'A' && *asc <= 'F')
			{
			   retv <<= 4;
			   retv += *asc - 'A' + 10;
			}
			else
			{
				err = 1;
				break;
			}
			asc++;
		}
	}

    if(_err != NULL)
	  *_err = err;
	
	return retv;
}

/*************************************************************
** ��������: Public_CheckArrayValIsEqual
** ��������: �ж����������Ƿ����,
** ��ڲ���: array��Ҫ�жϵ�����1�׵�ַ,array1��Ҫ�жϵ�����2�׵�ַ,len��Ҫ�жϵĳ���
** ���ڲ���: ��
** ���ز���: 0���,1�����;
** ȫ�ֱ���: 
** ����ģ��: 
*************************************************************/	
unsigned char Public_CheckArrayValIsEqual(const unsigned char *array,const unsigned char *array1,unsigned short len)
{
    unsigned short i;
    for(i =0; i <len; i++)
    {
        if(array[i]!=array1[i])return 1;
    }
    return 0;
}
/*************************************************************
** ��������: Public_atof
** ��������: ���ַ���ת����ֵ
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: ��
** ����ģ��: ��
*************************************************************/
float Public_atof(const char*str)
{
    float val,power;
    unsigned char sign=0,i;
    //ȥ���ַ����еĿո� 
    for(i=0;str[i]==' ';++i);
    //�ж������� ,����iָ����һ��λ�� 
    if(str[i] == '-')
    {
        sign = 1;
        i++;
    }
    else    
    if(str[i] == '+')
    {
        i++;
    }
    //ȡ���������������� 
    for(val=0.0;('0'<=str[i]&&str[i]<='9');++i)
    {
        val = STEP_VAL*val+ str[i]-'0';
    }
    ////////////////////
    if(str[i] == '.') //ȡС���� 
    {
        i++; //��һ��λ�� 
        //ȡС��������� 
        for(power=1.0;('0'<=str[i]&&str[i]<='9');++i)
        {
            val = STEP_VAL*val+str[i]-'0';
            power*=STEP_FLT; //float �Ŵ�ı��� 
        }
        val*=power;
    }
    /////////////
    if(sign)
    {
        val=SIGN_FLG*val;
    }
    ////////////////
    return val; 
} 
/*************************************************************
** ��������: Public_itoa
** ��������: ����ֵת���ַ���(�����Ǹ���)
** ��ڲ���: 
** ���ڲ���: 
** ���ز���: 
** ȫ�ֱ���: ��
** ����ģ��: ��
*************************************************************/
unsigned char Public_itoa(unsigned char *str,unsigned long val,unsigned char n)
{
    char temp[11];
    signed char i=0,j = 0;
    memset(temp,'0',n);
    do
    {
        temp[i] = val%10+'0';
        val/=10;
        i++;
    }while(val>0);
    //////////////////
    if(i<n)i = n;
    i--;
    ////////////
    while(i>=0)
    {
        str[j] = temp[i];
        j++;
        i--;
    };
    str[j] = '\0';
    return j;
}
 
/*********************************************************************
** �� �� ����
**********************************************************************/
u8 dectostr(u32 decnum,void *_str,u8 _bit)
{
   u8 pr;
   u8 i;
   u32 de;
   u8 flag=0;
   u32 temp;
   u8 bb;
   u8 seg;
   char *str = _str;
   

	//���ж�decnum����Чλ��
	temp = decnum;
	de=10;
	bb=1;
	for(i=0;i<10;i++)
	{
	 	if(temp/de>0)
			bb++;
		else
		 	break;
		de*=10;
	}
	seg=0;
   	if(_bit>bb)
	{
		for(i=0;i<(_bit-bb);i++)
		{
			str[seg]='0';
			seg++;	
		}
	}
	de=1000000000;
   	for(i=0;i<10;i++)
   	{
	 	pr=decnum/de%10;
	 	de/=10;
	 	if((pr==0)&&(flag==0)&&(de>0))
	 	continue;
	 	flag=1; 
	 	if(seg>128)return 0;
	 	str[seg]=pr+'0';
	 	seg++;
 	}
	str[seg]='\0'; //���Ͻ�����
	return seg;
}
 /*************************************************************
 ** ��������: Public_BCD2HEX
 ** ��������: BCDת��ΪByte 	   
 ** ��ڲ���: val:��Ҫת��������
 ** ���ڲ���: ��
 ** ���ز���: ת�����
 ** ȫ�ֱ���: 
 ** ����ģ��: 
 *************************************************************/
 unsigned char Public_BCD2HEX(unsigned char val)	//BCDת��ΪByte
 {
	 unsigned char temp;
	 temp=val&0x0f;
	 val>>=4;
	 val&=0x0f;
	 val*=10;
	 temp+=val;
	
	 return temp;
 }
 /*************************************************************
 ** ��������: Public_HEX2BCD
 ** ��������: Byte��ת��ΪBCD��    
 ** ��ڲ���: val:��Ҫת��������
 ** ���ڲ���: ��
 ** ���ز���: ת�����
 ** ȫ�ֱ���: 
 ** ����ģ��: 
 *************************************************************/
 unsigned char Public_HEX2BCD(unsigned char val)	//Byte��ת��ΪBCD��
 {
	 unsigned char i,j,k;
	 i=val/10;
	 j=val%10;
	 k=j+(i<<4);
	 return k;
 }


 /*************************************************************
 ** ��������: Public_Mymemcpy
 ** ��������: src�����ݿ���des��,nΪ��������,flagΪ����(1:����,0:˳��)
 ** ��ڲ���: 
 ** ���ڲ���: 
 ** ���ز���: 
 ** ȫ�ֱ���: ��
 ** ����ģ��:
 *************************************************************/
void *Public_Mymemcpy(void *des,  const void *src, unsigned short len, unsigned char flag)
{
	char *su1 = (char *)des;
	const char *su2 = (const char *)src;
	unsigned short i;
	if(flag==0)
	{
	 memcpy(su1,su2,len);
	}
	else
	{
	 for(i=0 ;i<len; i++)
	 {
		 *(su1+len-1-i) = *(su2+i);
	 }	 
	}
	return (des);
}

const unsigned long CrcTable[256] =
{
 0X00000000L, 0X04c11db7L, 0X09823b6eL, 0X0d4326d9L, 
 0X130476dcL, 0X17c56b6bL, 0X1a864db2L, 0X1e475005L, 
 0X2608edb8L, 0X22c9f00fL, 0X2f8ad6d6L, 0X2b4bcb61L, 
 0X350c9b64L, 0X31cd86d3L, 0X3c8ea00aL, 0X384fbdbdL, 
 0X4c11db70L, 0X48d0c6c7L, 0X4593e01eL, 0X4152fda9L, 
 0X5f15adacL, 0X5bd4b01bL, 0X569796c2L, 0X52568b75L,
 0X6a1936c8L, 0X6ed82b7fL, 0X639b0da6L, 0X675a1011L, 
 0X791d4014L, 0X7ddc5da3L, 0X709f7b7aL, 0X745e66cdL, 
 0X9823b6e0L, 0X9ce2ab57L, 0X91a18d8eL, 0X95609039L, 
 0X8b27c03cL, 0X8fe6dd8bL, 0X82a5fb52L, 0X8664e6e5L, 
 0Xbe2b5b58L, 0Xbaea46efL, 0Xb7a96036L, 0Xb3687d81L, 
 0Xad2f2d84L, 0Xa9ee3033L, 0Xa4ad16eaL, 0Xa06c0b5dL,
 0Xd4326d90L, 0Xd0f37027L, 0Xddb056feL, 0Xd9714b49L, 
 0Xc7361b4cL, 0Xc3f706fbL, 0Xceb42022L, 0Xca753d95L, 
 0Xf23a8028L, 0Xf6fb9d9fL, 0Xfbb8bb46L, 0Xff79a6f1L, 
 0Xe13ef6f4L, 0Xe5ffeb43L, 0Xe8bccd9aL, 0Xec7dd02dL, 
 0X34867077L, 0X30476dc0L, 0X3d044b19L, 0X39c556aeL, 
 0X278206abL, 0X23431b1cL, 0X2e003dc5L, 0X2ac12072L, 
 0X128e9dcfL, 0X164f8078L, 0X1b0ca6a1L, 0X1fcdbb16L, 
 0X018aeb13L, 0X054bf6a4L, 0X0808d07dL, 0X0cc9cdcaL, 
 0X7897ab07L, 0X7c56b6b0L, 0X71159069L, 0X75d48ddeL, 
 0X6b93dddbL, 0X6f52c06cL, 0X6211e6b5L, 0X66d0fb02L, 
 0X5e9f46bfL, 0X5a5e5b08L, 0X571d7dd1L, 0X53dc6066L,
 0X4d9b3063L, 0X495a2dd4L, 0X44190b0dL, 0X40d816baL, 
 0Xaca5c697L, 0Xa864db20L, 0Xa527fdf9L, 0Xa1e6e04eL, 
 0Xbfa1b04bL, 0Xbb60adfcL, 0Xb6238b25L, 0Xb2e29692L, 
 0X8aad2b2fL, 0X8e6c3698L, 0X832f1041L, 0X87ee0df6L, 
 0X99a95df3L, 0X9d684044L, 0X902b669dL, 0X94ea7b2aL, 
 0Xe0b41de7L, 0Xe4750050L, 0Xe9362689L, 0Xedf73b3eL,
 0Xf3b06b3bL, 0Xf771768cL, 0Xfa325055L, 0Xfef34de2L, 
 0Xc6bcf05fL, 0Xc27dede8L, 0Xcf3ecb31L, 0Xcbffd686L, 
 0Xd5b88683L, 0Xd1799b34L, 0Xdc3abdedL, 0Xd8fba05aL, 
 0X690ce0eeL, 0X6dcdfd59L, 0X608edb80L, 0X644fc637L, 
 0X7a089632L, 0X7ec98b85L, 0X738aad5cL, 0X774bb0ebL, 
 0X4f040d56L, 0X4bc510e1L, 0X46863638L, 0X42472b8fL, 
 0X5c007b8aL, 0X58c1663dL, 0X558240e4L, 0X51435d53L, 
 0X251d3b9eL, 0X21dc2629L, 0X2c9f00f0L, 0X285e1d47L, 
 0X36194d42L, 0X32d850f5L, 0X3f9b762cL, 0X3b5a6b9bL, 
 0X0315d626L, 0X07d4cb91L, 0X0a97ed48L, 0X0e56f0ffL, 
 0X1011a0faL, 0X14d0bd4dL, 0X19939b94L, 0X1d528623L,
 0Xf12f560eL, 0Xf5ee4bb9L, 0Xf8ad6d60L, 0Xfc6c70d7L, 
 0Xe22b20d2L, 0Xe6ea3d65L, 0Xeba91bbcL, 0Xef68060bL, 
 0Xd727bbb6L, 0Xd3e6a601L, 0Xdea580d8L, 0Xda649d6fL, 
 0Xc423cd6aL, 0Xc0e2d0ddL, 0Xcda1f604L, 0Xc960ebb3L, 
 0Xbd3e8d7eL, 0Xb9ff90c9L, 0Xb4bcb610L, 0Xb07daba7L, 
 0Xae3afba2L, 0Xaafbe615L, 0Xa7b8c0ccL, 0Xa379dd7bL,
 0X9b3660c6L, 0X9ff77d71L, 0X92b45ba8L, 0X9675461fL, 
 0X8832161aL, 0X8cf30badL, 0X81b02d74L, 0X857130c3L, 
 0X5d8a9099L, 0X594b8d2eL, 0X5408abf7L, 0X50c9b640L, 
 0X4e8ee645L, 0X4a4ffbf2L, 0X470cdd2bL, 0X43cdc09cL, 
 0X7b827d21L, 0X7f436096L, 0X7200464fL, 0X76c15bf8L, 
 0X68860bfdL, 0X6c47164aL, 0X61043093L, 0X65c52d24L, 
 0X119b4be9L, 0X155a565eL, 0X18197087L, 0X1cd86d30L, 
 0X029f3d35L, 0X065e2082L, 0X0b1d065bL, 0X0fdc1becL, 
 0X3793a651L, 0X3352bbe6L, 0X3e119d3fL, 0X3ad08088L, 
 0X2497d08dL, 0X2056cd3aL, 0X2d15ebe3L, 0X29d4f654L, 
 0Xc5a92679L, 0Xc1683bceL, 0Xcc2b1d17L, 0Xc8ea00a0L,
 0Xd6ad50a5L, 0Xd26c4d12L, 0Xdf2f6bcbL, 0Xdbee767cL,
 0Xe3a1cbc1L, 0Xe760d676L, 0Xea23f0afL, 0Xeee2ed18L,
 0Xf0a5bd1dL, 0Xf464a0aaL, 0Xf9278673L, 0Xfde69bc4L,
 0X89b8fd09L, 0X8d79e0beL, 0X803ac667L, 0X84fbdbd0L,
 0X9abc8bd5L, 0X9e7d9662L, 0X933eb0bbL, 0X97ffad0cL,
 0Xafb010b1L, 0Xab710d06L, 0Xa6322bdfL, 0Xa2f33668L,
 0Xbcb4666dL, 0Xb8757bdaL, 0Xb5365d03L, 0Xb1f740b4L
};
unsigned long Public_CRC_32( unsigned char * aData, unsigned long aSize )
{
 unsigned long crc32 = 0;
 unsigned long tabItem;

 while(aSize--)
 {
	 tabItem=( crc32 >> 24 )^ *aData++;
	 crc32 = ( crc32 << 8 ) ^ CrcTable[tabItem];
 }
 return crc32;
}
//��ϣУ��
u32 HashPrime = 16777619;
u32 HashOffset = 0x811c9dc5;
u16 HashVerify(u8 *data,u16 lens)
{
	u16 idx;
	u32 hash = HashOffset;
	for(idx = 0;idx<lens;idx++)
	{
		hash = hash^(*data);
		hash = hash * HashPrime;
		data++;
	}
	return ((hash>>16)^(hash&0xffff));
}

