#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

// 2010/05/31 duxiulong created
// ý�������ڴ�ع���

#include "datalock.h"

#define DPG_HEADE_SIZE	0


//#define byte unsigned char

typedef struct _TDataPackage
{
	void *m_pOwner;
	TDataLock m_tLock;		//���ݰ���
	int m_iRefCnt;			// ���ü���
	int m_iSeq;				//���к�
	int m_iMemSize;			// ���ڴ��С
	int m_iDataSize;			// ��ʵ�����ݳ���
	int m_iType;				// ��������
	#if 0
	int m_iHeadEnable;		// ��ͷ������Ч
	byte m_bHead[DPG_HEADE_SIZE];	//��ͷ��Ԥ��������ʹ��
	#endif
	unsigned char m_bData[0];			// ������
} _TDataPackage;


#define DPOOL_SIZE_LV0	(4 * 1024)
#define DPOOL_SIZE_LV1	(8 * 1024)
#define DPOOL_SIZE_LV2	(16 * 1024)
#define DPOOL_SIZE_LV3	(32 * 1024)
#define DPOOL_SIZE_LV4	(64 * 1024)
#define DPOOL_SIZE_LV5	(128 * 1024)
#define DPOOL_SIZE_LV6	(256 * 1024)
#define DPOOL_SIZE_LV7	(512 * 1024)
#define DPOOL_SIZE_LV8	(1024 * 1024)


#define DPOOL_MAX_SIZE DPOOL_SIZE_LV8

#define DPOOL_SIZE_LVS	9


#define DPOOL_GRP_SIZE	256

#define DGRP_SIZE_LV0	256
#define DGRP_SIZE_LV1	256
#define DGRP_SIZE_LV2	256
#define DGRP_SIZE_LV3	256
#define DGRP_SIZE_LV4	96
#define DGRP_SIZE_LV5	64
#define DGRP_SIZE_LV6	32
#define DGRP_SIZE_LV7	16
#define DGRP_SIZE_LV8	4


typedef struct
{
	int m_iCnt;
	int m_iMaxSize;
	TDataLock m_tLock;
	_TDataPackage *m_ptPkgs[DPOOL_GRP_SIZE];
} TDataPackageGroup;

typedef struct
{
	int m_iMaxMemSize;
	int m_iSeq;
	int m_iSizeAlloc;
	int m_iSizeAllocHistory;
	TDataLock m_tLock;
	TDataPackageGroup m_tGrps[DPOOL_SIZE_LVS];
}TDataPool;

TDataPool *DataPoolCreate(int _iMaxMemSize);	//���ݳش���
void DataPoolDestroy(TDataPool *_ptPool);	//���ݳ�����
_TDataPackage *DataPackageCreate(int _iSize, TDataPool *_ptPool); //���ݰ�����
void DataPackageRefer(_TDataPackage *_ptPkg);	//���ݰ�����
void DataPackageRelease(_TDataPackage *_ptPkg); //���ݰ�������

void DataPoolDebugMsg(char *_strMsg, TDataPool *_ptPool);

#define DATA_POOL_DEBUG	0

#endif

