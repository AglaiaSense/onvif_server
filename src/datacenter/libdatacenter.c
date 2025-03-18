#include <unistd.h>

#include "datapool.h"
#include "datacenter.h"
#include "libdatacenter.h"
//#include "rtmsg.h"

static TDataPool *g_ptDataPool = NULL;


// 数据缓存池初始化
int datacenter_PoolInit(int _iMaxSize)
{
//	RtMsg_WriteLog(3, " %s\n", "初始化datacenter 开始");
	if (NULL == g_ptDataPool)
	{
		g_ptDataPool = DataPoolCreate(_iMaxSize);
	}
	//zhangweiwei add 20111226 注册调试信息
//	RtMsg_Reg("datacenter", datacenter_RtMsgGetDebugInfo);
//	RtMsg_WriteLog(3, " %s\n", "初始化datacenter 结束");
	
	return g_ptDataPool != NULL ? TD_OK: TD_ERROR;
}

int datacenter_PoolUnint()
{
	if (g_ptDataPool)
	{
		DataPoolDestroy(g_ptDataPool);
		g_ptDataPool = NULL;
	}
	
	return TD_OK;
}

static TDataPackage *GetPackage(_TDataPackage *_ptPkg)
{
	return (TDataPackage *)(&_ptPkg->m_iDataSize);
}

static _TDataPackage *_GetPackage(TDataPackage *_ptPkg)
{
	//char *addr = ((char *)_ptPkg)  - (int)&(((_TDataPackage *)0)->m_iDataSize);
	char *addr = NULL;
	return (_TDataPackage *)addr;
}

// 创建一块内存
#if 0
TDataPackage *datacenter_CreatePackage(int _iSize)
{
	_TDataPackage *ptPkg = DataPackageCreate(_iSize, g_ptDataPool);

	return ptPkg ? GetPackage(ptPkg) : NULL;
}
#else
TDataPackage *datacenter_CreatePackage(int _iSize)
{
	int iCnt = 1;
	_TDataPackage *ptPkg = NULL;

	do  //创建不成功则等待一段时间
	{
		ptPkg = DataPackageCreate(_iSize, g_ptDataPool);

		if (ptPkg)
		{
			break;
		}
		usleep(100);
	} while (iCnt++ < 100);

	if (NULL == ptPkg)
	{
//		RtMsg_WriteLog(3, "alloc pkg error!\n");
		return NULL;
	}
	else
	{
		return GetPackage(ptPkg);
	}
}
#endif

// 添加内存引用记数 
void datacenter_PackageRefer(TDataPackage *_ptPkg)
{
	if (_ptPkg)
	{
		DataPackageRefer(_GetPackage(_ptPkg));
	}
}

// 减少内存引用记数
void datacenter_PackageRelease(TDataPackage *_ptPkg)
{
	if (_ptPkg)
	{
		DataPackageRelease(_GetPackage(_ptPkg));
	}
}

// 创建一个FIFO队列
TDataFIFO *datacenter_MakeFifo(int _iPri, int _iSize)
{
	return (TDataFIFO *)MakeFIFO(_iPri, _iSize);
}

// 销毁一个队列
void datacenter_DestroyFifo(TDataFIFO *_ptFifo)
{
	DestroyFIFO((_TDataFIFO *)_ptFifo);
}

void datacenter_ChgFifoPri(int _iPri, TDataFIFO *_ptFifo)
{
	ChgFIFOType(_iPri, (_TDataFIFO *)_ptFifo);
}

// 从队列获取一包数据
TDataPackage *datacenter_GetPackFromFifo(TDataFIFO *_ptFifo)
{
	_TDataPackage *ptPkg = GetData((_TDataFIFO *)_ptFifo);
	
	return ptPkg ? GetPackage(ptPkg) : NULL;
}

// 清空队列数据
void datacenter_ClearFifo(TDataFIFO *_ptFifo)
{
	ClearFIFO((_TDataFIFO *)_ptFifo);
}

void datacenter_ChgFifoType(int _iPri, TDataFIFO *_ptFifo)
{
	ChgFIFOType(_iPri, (_TDataFIFO *)_ptFifo);
}

// 创建一个数据分发中心
TDataCenter *datacenter_Create()
{
	return (TDataCenter *)DataCenterCreate();
}

// 销毁一个数据分发中心
void datacenter_Destroy(TDataCenter *_ptDc)
{
	DataCenterDestroy((_TDataCenter *)_ptDc);
}

// 绑定fifo到数据分发中心
int datacenter_BindFifo(TDataFIFO *_ptFifo, TDataCenter *_ptDc)
{
	if (_ptFifo && _ptDc)
	{
		DataCenterBindFIFO((_TDataFIFO *)_ptFifo, (_TDataCenter *)_ptDc);

		return TD_OK;
	}

	return TD_ERROR;
}

// 解绑定fifo到数据分发中心
int datacenter_UnBindFifo(TDataFIFO *_ptFifo, TDataCenter *_ptDc)
{
	if (_ptFifo && _ptDc)
	{
		DataCenterUnBindFIFO((_TDataFIFO *) _ptFifo, (_TDataCenter *)_ptDc);

		return TD_OK;
	}

	return TD_ERROR;
}

int datacenter_HasNoFifoBind(TDataCenter *_ptDc)
{
	_TDataCenter * ptDc = (_TDataCenter *)_ptDc;

	return (NULL == ptDc->m_ptFifo);
}

// 将数据包交与数据分发中心分发
int datacenter_PushPackage(TDataPackage *_ptPkg, TDataCenter *_ptDc)
{
	if (_ptPkg && _ptDc)
	{
		int iRet = PushData(_GetPackage(_ptPkg), (_TDataCenter *)_ptDc);

		return iRet > 0 ? TD_OK : TD_ERROR;
	}

	return TD_ERROR;
}



void datacenter_DebugMsg(char *_strMsg)
{
	DataPoolDebugMsg(_strMsg, g_ptDataPool);
}


int datacenter_GetFifoPackNum(TDataFIFO *_ptFifo)
{
	return  GetFifoPackNum((_TDataFIFO *)_ptFifo);
}



