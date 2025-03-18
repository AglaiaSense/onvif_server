#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

// 2010/05/31 duxiulong created
// 媒体数据内存池管理

#include "datalock.h"

#define DPG_HEADE_SIZE	0


//#define byte unsigned char

typedef struct _TDataPackage
{
	void *m_pOwner;
	TDataLock m_tLock;		//数据包锁
	int m_iRefCnt;			// 引用计数
	int m_iSeq;				//序列号
	int m_iMemSize;			// 包内存大小
	int m_iDataSize;			// 包实际数据长度
	int m_iType;				// 数据类型
	#if 0
	int m_iHeadEnable;		// 包头数据有效
	byte m_bHead[DPG_HEADE_SIZE];	//包头，预留给网络使用
	#endif
	unsigned char m_bData[0];			// 包数据
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

TDataPool *DataPoolCreate(int _iMaxMemSize);	//数据池创建
void DataPoolDestroy(TDataPool *_ptPool);	//数据池销毁
_TDataPackage *DataPackageCreate(int _iSize, TDataPool *_ptPool); //数据包创建
void DataPackageRefer(_TDataPackage *_ptPkg);	//数据包引用
void DataPackageRelease(_TDataPackage *_ptPkg); //数据包解引用

void DataPoolDebugMsg(char *_strMsg, TDataPool *_ptPool);

#define DATA_POOL_DEBUG	0

#endif

