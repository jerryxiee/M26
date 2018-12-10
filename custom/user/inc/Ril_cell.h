#ifndef __RIL_CELL_H__
#define __RIL_CELL_H__
#include "ql_type.h"
#include "ril_sim.h"

typedef struct 
{
 	char flg;
	u16 mcc; //���ұ���
	u16 mnc; //��Ӫ�̴���
	u16 lac; //��վ����
	u16 cellid; //С����
	s8 rxlev;//�� dBm Ϊ��λ�Ľ����źŵȼ�
	u8 ta;//ʱ����ǰ������Χ 0~63
}ST_QENG_info;



#endif
