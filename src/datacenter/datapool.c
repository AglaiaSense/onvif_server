// 2010/05/31 duxiulong created


#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#ifdef MEMWATCH
#include "memwatch.h"
#endif

#ifdef TD_DEBUG
#define PKG_MEM_INIT_ALL	0
#endif

#include "datapool.h"

#define DataPackageLockInit(dpg)	InitDataLock((dpg)->m_tLock)
#define DataPackageLock(dpg)		LockDataLock((dpg)->m_tLock)
#define DataPackageUnLock(dpg)		UnLockDataLock((dpg)->m_tLock)

#define DataPoolLockInit(pool)		InitDataLock((pool)->m_tLock)
#define DataPoolLock(pool)			LockDataLock((pool)->m_tLock)
#define DataPoolUnLock(pool)		UnLockDataLock((pool)->m_tLock)

#define DataGroupLockInit(grp)		InitDataLock((grp)->m_tLock)
#define DataGroupLock(grp)			LockDataLock((grp)->m_tLock)
#define  DataGroupUnLock(grp)		UnLockDataLock((grp)->m_tLock)

#define DO_CACHE	1

// 调试信息，用于检测内存泄漏


#if DATA_POOL_DEBUG
static int g_iDataPoolPgAllocCnt = 0;
static int g_iDataPoolPgFreeCnt = 0;
static int g_iDataPoolPgReferCnt = 0;
static int g_iDataPoolPgReleaseCnt = 0;
static int g_iDataPoolPgReReleaseCnt = 0;
static int g_iDataPoolPgNullReleaseCnt = 0;

static int g_blPoolLostInit = 1;
static TDataLock g_tPoolTestLock;
#endif


static int g_iGrpSizes[] = {0, DPOOL_SIZE_LV0, DPOOL_SIZE_LV1, DPOOL_SIZE_LV2, DPOOL_SIZE_LV3,
	DPOOL_SIZE_LV4, DPOOL_SIZE_LV5, DPOOL_SIZE_LV6, DPOOL_SIZE_LV7, DPOOL_SIZE_LV8};


#define DP_DEFAULT_SIZE		(32 * 1024 * 1024) 

TDataPool *DataPoolCreate(int _iMaxMemSize)	//数据池创建
{
	TDataPool *ptPool = (TDataPool *)malloc(sizeof(TDataPool));
	int m_iGrpSize[] = {DGRP_SIZE_LV0, DGRP_SIZE_LV1, DGRP_SIZE_LV2, DGRP_SIZE_LV3,
		DGRP_SIZE_LV4, DGRP_SIZE_LV5, DGRP_SIZE_LV6, DGRP_SIZE_LV7, DGRP_SIZE_LV8};

	if (ptPool != NULL)
	{
		int i;
		
		memset(ptPool, 0, sizeof(*ptPool));
		ptPool->m_iSeq = 1;
		DataPoolLockInit(ptPool);

		for (i = 0; i < DPOOL_SIZE_LVS; i++)
		{
			TDataPackageGroup *ptGrp = &ptPool->m_tGrps[i];
			
			ptGrp->m_iMaxSize = m_iGrpSize[i];
			DataGroupLockInit(ptGrp);
		}

		ptPool->m_iMaxMemSize = (_iMaxMemSize <= 0) ? DP_DEFAULT_SIZE : _iMaxMemSize;
	}

	#if DATA_POOL_DEBUG
	if (g_blPoolLostInit)
	{
		InitDataLock(g_tPoolTestLock);
		g_blPoolLostInit = 0;
	}
	#endif

	return ptPool;
}

static void DataPackageGroupDestroy(TDataPackageGroup *_ptGrp)
{
	int i = 0;

	while (i < _ptGrp->m_iCnt)
	{
		free(_ptGrp->m_ptPkgs[i++]);
	}
}

void DataPoolDestroy(TDataPool *_ptPool)	//数据池销毁
{
	if (_ptPool != NULL)
	{
		int i;

		for (i = 0; i < DPOOL_SIZE_LVS; i++)
		{
			DataPackageGroupDestroy(_ptPool->m_tGrps + i);
		}

		free(_ptPool);
	}
}

// 待优化
inline static int DataPackageSizeToGroup(int _iSize, int *_piNewSize)
{
	int i;


	for (i = 0; i < DPOOL_SIZE_LVS; i++)
	{
		if (_iSize > g_iGrpSizes[i] && _iSize <= g_iGrpSizes[i + 1])
		{
			*_piNewSize = g_iGrpSizes[i + 1];
			return i;
		}
	}

	return -1;
}


inline static _TDataPackage *DataPackageAlloc(int _iSize)
{
	_TDataPackage *ptPkg = (_TDataPackage *)malloc(sizeof(_TDataPackage) + _iSize);

	if (ptPkg != NULL)
	{
		#if PKG_MEM_INIT_ALL
		memset(ptPkg, 0, sizeof(_TDataPackage) + _iSize);
		#else
		memset(ptPkg, 0, sizeof(_TDataPackage));
		#endif
		ptPkg->m_iMemSize = _iSize;
		DataPackageLockInit(ptPkg);
	}

	return ptPkg;
}

#if DO_CACHE
_TDataPackage *DataPackageCreate(int _iSize, TDataPool *_ptPool)  //数据包创建
{
	int iSize;
	_TDataPackage *ptPkg = NULL;
	
	if (_ptPool != NULL)
	{
		int iGid = DataPackageSizeToGroup(_iSize, &iSize);

		if (iGid >= 0)
		{
			TDataPackageGroup *ptGrp = &_ptPool->m_tGrps[iGid];
			int iSeq;

			DataPoolLock(_ptPool);
			iSeq = _ptPool->m_iSeq++;
			if (iSeq >= 0x1FFFFFFF)
			{
				_ptPool->m_iSeq = 1;
			}
			DataPoolUnLock(_ptPool);

			DataGroupLock(ptGrp);
			if (ptGrp->m_iCnt > 0)
			{
				ptPkg = ptGrp->m_ptPkgs[--ptGrp->m_iCnt];
				DataGroupUnLock(ptGrp);
			}
			else if (_ptPool->m_iSizeAlloc < _ptPool->m_iMaxMemSize)
			{
				DataGroupUnLock(ptGrp);
				ptPkg = DataPackageAlloc(iSize);

				#if DATA_POOL_DEBUG
				g_iDataPoolPgAllocCnt += (ptPkg != NULL);
				#endif

				if (ptPkg)
				{
					DataPoolLock(_ptPool);
					_ptPool->m_iSizeAlloc += ptPkg->m_iMemSize;
					if (_ptPool->m_iSizeAlloc > _ptPool->m_iSizeAllocHistory)
					{
						_ptPool->m_iSizeAllocHistory = _ptPool->m_iSizeAlloc;
					}
					DataPoolUnLock(_ptPool);
				}
			}
			else
			{
				DataGroupUnLock(ptGrp);
			}
			

			if (ptPkg)
			{
				ptPkg->m_iDataSize = _iSize;
				ptPkg->m_pOwner = _ptPool;
				ptPkg->m_iRefCnt = 0;
				ptPkg->m_iSeq = iSeq;
				#if 0
				ptPkg->m_iHeadEnable = 0;
				#endif
			}
		}
	}

	return ptPkg;
}
#else
_TDataPackage *DataPackageCreate(int _iSize, TDataPool *_ptPool) 
{
	_TDataPackage *ptPkg = DataPackageAlloc(_iSize);

	if (ptPkg)
	{
		int iSeq;

		DataPoolLock(_ptPool);
		iSeq = _ptPool->m_iSeq++;
		if (iSeq >= 0x1FFFFFFF)
		{
			_ptPool->m_iSeq = 1;
		}
		DataPoolUnLock(_ptPool);
		
		ptPkg->m_iDataSize = _iSize;
		ptPkg->m_pOwner = _ptPool;
		ptPkg->m_iRefCnt = 0;
		ptPkg->m_iSeq = iSeq;
	}

	return ptPkg;
}
#endif

void DataPackageRefer(_TDataPackage *_ptPkg)	//数据包引用
{
	if (_ptPkg != NULL)
	{
		DataPackageLock(_ptPkg);
		_ptPkg->m_iRefCnt++;
		DataPackageUnLock(_ptPkg);
	}
}

#if DO_CACHE
void DataPackageRelease(_TDataPackage *_ptPkg) //数据包解引用
{	
    if (_ptPkg != NULL)
	{
		int flg = 0;
		
		DataPackageLock(_ptPkg);
		
		_ptPkg->m_iRefCnt--;
		if (0 == _ptPkg->m_iRefCnt)
		{
			int iGid;
			int iSize;
			TDataPool *ptPool = (TDataPool *)_ptPkg->m_pOwner;
			TDataPackageGroup *ptGrp;

			iGid = DataPackageSizeToGroup(_ptPkg->m_iMemSize, &iSize);
			DataPackageUnLock(_ptPkg);

			if (iGid < 0)
			{
				// should not com here!
				#ifdef TD_DEBUG
				printf("data pkg error! addr = %p, size = %d\n", _ptPkg, _ptPkg->m_iMemSize);
				//abort();
				_TDataPackage *tmp = NULL;
				tmp->m_iMemSize = 0;
				#endif
				goto EXIT;
			}

			
			ptGrp = ptPool->m_tGrps + iGid;
			DataGroupLock(ptGrp);
			if (ptGrp->m_iCnt >= ptGrp->m_iMaxSize || ptGrp->m_iCnt >= DPOOL_GRP_SIZE)
			{
				DataGroupUnLock(ptGrp);
				flg = 1;
				DataPoolLock(ptPool);
				ptPool->m_iSizeAlloc -= _ptPkg->m_iMemSize;
				DataPoolUnLock(ptPool);
			}
			else
			{
				ptGrp->m_ptPkgs[ptGrp->m_iCnt++] = _ptPkg;
				DataGroupUnLock(ptGrp);
			}
			
		}
		else
		{
			DataPackageUnLock(_ptPkg);
		}

	EXIT:		
		
		if (flg)
		{
			free(_ptPkg);
		}
	}	
}
#else
void DataPackageRelease(_TDataPackage *_ptPkg) //数据包解引用
{	
	if (_ptPkg != NULL)
	{
		int flg = 0;

		DataPackageLock(_ptPkg);
		
		_ptPkg->m_iRefCnt--;
		if (0 == _ptPkg->m_iRefCnt)
		{
			flg = 1;
		}
		
		DataPackageUnLock(_ptPkg);
		
		if (flg)
		{
			free(_ptPkg);
		}
	}	
}
#endif

#if DATA_POOL_DEBUG
void GetDataPoolMemMsg(int *_piAllocCnt, int *_piFreeCnt, int *_piReferCnt, int *_piReleaseCnt,
	int *_piReRelaseCnt, int *_piNullReleaseCnt)
{
	*_piAllocCnt = g_iDataPoolPgAllocCnt;
	*_piFreeCnt = g_iDataPoolPgFreeCnt;
	*_piReferCnt = g_iDataPoolPgReferCnt;
	*_piReleaseCnt = g_iDataPoolPgReleaseCnt;
	*_piReRelaseCnt = g_iDataPoolPgReReleaseCnt;
	*_piNullReleaseCnt = g_iDataPoolPgNullReleaseCnt;
}
#endif

void DataPoolDebugMsg(char *_strMsg, TDataPool *_ptPool)
{
	if (_strMsg && _ptPool)
	{
		int i;
		char strBuf[256];
		
		sprintf(_strMsg, "data pool[%p] msg:\n", _ptPool);

		for (i = 0; i < DPOOL_SIZE_LVS; i++)
		{
			sprintf(strBuf, "group[%d] size = %dK, block = %d/%d, cache size = %dK\n",
				i, g_iGrpSizes[i + 1] >> 10, _ptPool->m_tGrps[i].m_iCnt, _ptPool->m_tGrps[i].m_iMaxSize,
				(g_iGrpSizes[i + 1] >> 10) * _ptPool->m_tGrps[i].m_iCnt);

			strcat(_strMsg, strBuf);
		}

		sprintf(strBuf, "Total pkg size = %dK/%dK/%dK\n\n", _ptPool->m_iSizeAlloc >> 10, _ptPool->m_iSizeAllocHistory >> 10, _ptPool->m_iMaxMemSize >> 10);
		strcat(_strMsg, strBuf);
	}
}


