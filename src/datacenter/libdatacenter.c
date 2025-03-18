#include <unistd.h>

#include "datapool.h"
#include "datacenter.h"
#include "libdatacenter.h"
//#include "rtmsg.h"

static TDataPool *g_ptDataPool = NULL;


// ���ݻ���س�ʼ��
int datacenter_PoolInit(int _iMaxSize)
{
//	RtMsg_WriteLog(3, " %s\n", "��ʼ��datacenter ��ʼ");
	if (NULL == g_ptDataPool)
	{
		g_ptDataPool = DataPoolCreate(_iMaxSize);
	}
	//zhangweiwei add 20111226 ע�������Ϣ
//	RtMsg_Reg("datacenter", datacenter_RtMsgGetDebugInfo);
//	RtMsg_WriteLog(3, " %s\n", "��ʼ��datacenter ����");
	
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

// ����һ���ڴ�
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

	do  //�������ɹ���ȴ�һ��ʱ��
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

// ����ڴ����ü��� 
void datacenter_PackageRefer(TDataPackage *_ptPkg)
{
	if (_ptPkg)
	{
		DataPackageRefer(_GetPackage(_ptPkg));
	}
}

// �����ڴ����ü���
void datacenter_PackageRelease(TDataPackage *_ptPkg)
{
	if (_ptPkg)
	{
		DataPackageRelease(_GetPackage(_ptPkg));
	}
}

// ����һ��FIFO����
TDataFIFO *datacenter_MakeFifo(int _iPri, int _iSize)
{
	return (TDataFIFO *)MakeFIFO(_iPri, _iSize);
}

// ����һ������
void datacenter_DestroyFifo(TDataFIFO *_ptFifo)
{
	DestroyFIFO((_TDataFIFO *)_ptFifo);
}

void datacenter_ChgFifoPri(int _iPri, TDataFIFO *_ptFifo)
{
	ChgFIFOType(_iPri, (_TDataFIFO *)_ptFifo);
}

// �Ӷ��л�ȡһ������
TDataPackage *datacenter_GetPackFromFifo(TDataFIFO *_ptFifo)
{
	_TDataPackage *ptPkg = GetData((_TDataFIFO *)_ptFifo);
	
	return ptPkg ? GetPackage(ptPkg) : NULL;
}

// ��ն�������
void datacenter_ClearFifo(TDataFIFO *_ptFifo)
{
	ClearFIFO((_TDataFIFO *)_ptFifo);
}

void datacenter_ChgFifoType(int _iPri, TDataFIFO *_ptFifo)
{
	ChgFIFOType(_iPri, (_TDataFIFO *)_ptFifo);
}

// ����һ�����ݷַ�����
TDataCenter *datacenter_Create()
{
	return (TDataCenter *)DataCenterCreate();
}

// ����һ�����ݷַ�����
void datacenter_Destroy(TDataCenter *_ptDc)
{
	DataCenterDestroy((_TDataCenter *)_ptDc);
}

// ��fifo�����ݷַ�����
int datacenter_BindFifo(TDataFIFO *_ptFifo, TDataCenter *_ptDc)
{
	if (_ptFifo && _ptDc)
	{
		DataCenterBindFIFO((_TDataFIFO *)_ptFifo, (_TDataCenter *)_ptDc);

		return TD_OK;
	}

	return TD_ERROR;
}

// ���fifo�����ݷַ�����
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

// �����ݰ��������ݷַ����ķַ�
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



