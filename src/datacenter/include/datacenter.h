#ifndef _DATA_CENTER_H_
#define _DATA_CENTER_H_

//2010/05/31 duxiulong created
// 媒体数据分发中心

#include "datapool.h"
#include "datalock.h"

enum
{
	FIFO_PRI_CYC = 1,	//缓存环，缓存溢出时循环覆盖旧数据
	FIFO_PRI_COM = 2,	//普通FIFO，缓存溢出时丢弃新数据
	FIFO_PRI_SEQ = 3,	//序列化FIFO，无数据丢失，缓存溢出时阻塞其它操作
	FIFO_PRI_SEQ_NB = 4,	//序列化FIFO，但不阻塞操作
	FIFO_PRI_IF = 5
};

typedef struct _TDataFIFO
{
	struct _TDataFIFO *m_ptNext;
	TDataLock m_tLock;
	int m_iPri;			// 数据处理优先级
	int m_iBase;			// fifo base指针
	int m_iTop;			// fifo top指针
	int m_iMaxPkgCnt;	//最大可缓存包数
	int m_iLastSeq;
	_TDataPackage *m_ptBuf[0];
} _TDataFIFO;

typedef struct
{
	TDataLock m_tLock;
	_TDataFIFO *m_ptFifo;
} _TDataCenter;	//数据源

_TDataCenter *DataCenterCreate();
void DataCenterDestroy(_TDataCenter *_ptDc);
void DataCenterBindFIFO(_TDataFIFO *_ptFifo, _TDataCenter *_ptDc);
void DataCenterUnBindFIFO(_TDataFIFO *_ptFifo, _TDataCenter *_ptDc);
int PushData(_TDataPackage *_ptPkg, _TDataCenter *_ptDc);
_TDataPackage *GetData(_TDataFIFO *_ptFifo);
void ClearFIFO(_TDataFIFO *_ptFifo);
_TDataFIFO *MakeFIFO(int _iPri, int _iSize);
void DestroyFIFO(_TDataFIFO *_ptFifo);
void ChgFIFOType(int _iPri, _TDataFIFO *_ptFifo);


int GetFifoPackNum( _TDataFIFO *_ptFifo);

#endif
