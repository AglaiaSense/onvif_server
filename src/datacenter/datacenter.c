#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef MEMWATCH
#include "memwatch.h"
#endif


#include "common.h"
#include "datacenter.h"

#define DataCenterLockInit(dc)		InitDataLock((dc)->m_tLock)
#define DataCenterLock(dc)			LockDataLock((dc)->m_tLock)
#define DataCenterUnLock(dc)		UnLockDataLock((dc)->m_tLock)

#define DataFIFOLockInit(f)			InitDataLock((f)->m_tLock)
#define DataFIFOLock(f)			LockDataLock((f)->m_tLock)
#define DataFIFOUnLock(f)			UnLockDataLock((f)->m_tLock)

_TDataCenter *DataCenterCreate()
{
	_TDataCenter *ptDc = (_TDataCenter *)malloc(sizeof(_TDataCenter));

	if (ptDc != NULL)
	{
		memset(ptDc, 0, sizeof(*ptDc));
		DataCenterLockInit(ptDc);
	}

	return ptDc;
}

void DataCenterDestroy(_TDataCenter *_ptDc)
{
	if (_ptDc != NULL)
	{
		free(_ptDc);
	}
}

void DataCenterBindFIFO(_TDataFIFO *_ptFifo, _TDataCenter *_ptDc)
{
	if (_ptFifo != NULL && _ptDc != NULL)
	{
		_TDataFIFO *ptFifo;
		
		DataCenterLock(_ptDc);
		DataFIFOLock(_ptFifo);

		if (_ptFifo->m_ptNext != NULL)
		{
			// should not come here
			goto EXIT;
		}

		ptFifo = _ptDc->m_ptFifo;
		if (ptFifo != NULL)
		{
			_ptFifo->m_ptNext = ptFifo;
			_ptDc->m_ptFifo = _ptFifo;
		}
		else
		{
			_ptDc->m_ptFifo = _ptFifo;
			_ptFifo->m_ptNext = NULL;
		}

	EXIT:
		DataFIFOUnLock(_ptFifo);
		DataCenterUnLock(_ptDc);
	}
}

void DataCenterUnBindFIFO(_TDataFIFO *_ptFifo, _TDataCenter *_ptDc)
{
	if (_ptFifo != NULL && _ptDc != NULL)
	{
		_TDataFIFO *ptPre = NULL;
		_TDataFIFO *ptFifo = NULL;
		_TDataFIFO *ptCur = NULL;
		
		DataCenterLock(_ptDc);
		DataFIFOLock(_ptFifo);

		for (ptCur = _ptDc->m_ptFifo; ptCur != NULL; ptCur = ptCur->m_ptNext)
		{
			if (ptCur == _ptFifo)
			{
				ptFifo = ptCur;
				break;
			}

			ptPre = ptCur;
		}

		if (ptFifo != NULL)
		{
			if (ptPre != NULL)
			{
				ptPre->m_ptNext = ptFifo->m_ptNext;
			}
			else
			{
				_ptDc->m_ptFifo = ptFifo->m_ptNext;
			}
		}

		_ptFifo->m_ptNext = NULL;

		DataFIFOUnLock(_ptFifo);
		DataCenterUnLock(_ptDc);
	}
}

int PushData(_TDataPackage *_ptPkg, _TDataCenter *_ptDc)
{
	int iRet = 0;
	
	if (_ptPkg != NULL && _ptDc != NULL)
	{
		int iCnt;

		_TDataFIFO *ptFifo;
		
		DataCenterLock(_ptDc);

		DataPackageRefer(_ptPkg);

		for (ptFifo = _ptDc->m_ptFifo; ptFifo != NULL; ptFifo = ptFifo->m_ptNext)
		{
			DataFIFOLock(ptFifo);
			// dxl 20110504 防止数据包重复发送
			if (ptFifo->m_iLastSeq == _ptPkg->m_iSeq)
			{
				td_printf(0, "pkg proc twice!!!");
				goto NEXT;
			}
			iCnt = ptFifo->m_iTop - ptFifo->m_iBase;
			if (iCnt < ptFifo->m_iMaxPkgCnt)
			{
				int iIndx = (ptFifo->m_iTop++) % ptFifo->m_iMaxPkgCnt;
				ptFifo->m_ptBuf[iIndx] = _ptPkg;
				ptFifo->m_iLastSeq = _ptPkg->m_iSeq;
				DataPackageRefer(_ptPkg);
			}
			else if (FIFO_PRI_CYC == ptFifo->m_iPri)
			{
				int iIndx = (ptFifo->m_iTop++) % ptFifo->m_iMaxPkgCnt;
				_TDataPackage *ptOld = ptFifo->m_ptBuf[iIndx];

				DataPackageRelease(ptOld);
				
				ptFifo->m_ptBuf[iIndx] = _ptPkg;
				ptFifo->m_iLastSeq = _ptPkg->m_iSeq;
				DataPackageRefer(_ptPkg);	
				ptFifo->m_iBase++;

				td_printf(0, "fifo %p is full, do recover %p => %p\n", ptFifo, ptOld, _ptPkg);
			}
			else if (FIFO_PRI_SEQ == ptFifo->m_iPri)
			{
				int iIndx;
				
				do
				{
					DataFIFOUnLock(ptFifo); 
					usleep(10);
					DataFIFOLock(ptFifo);
					iCnt = ptFifo->m_iTop - ptFifo->m_iBase;
				} while (iCnt >= ptFifo->m_iMaxPkgCnt);

				iIndx = (ptFifo->m_iTop++) % ptFifo->m_iMaxPkgCnt;
				ptFifo->m_ptBuf[iIndx] = _ptPkg;
				ptFifo->m_iLastSeq = _ptPkg->m_iSeq;
				DataPackageRefer(_ptPkg);			
			}
			else if (FIFO_PRI_SEQ_NB == ptFifo->m_iPri)
			{
				//td_printf(0, "fifo seq no block!!!");
				iRet++;
			}
			else if (FIFO_PRI_IF == ptFifo->m_iPri)
			{
				int iIndx;
				_TDataPackage *ptPkg = NULL;

				while (ptFifo->m_iBase < ptFifo->m_iTop)
				{
					iIndx = ptFifo->m_iBase % ptFifo->m_iMaxPkgCnt;

					ptPkg = ptFifo->m_ptBuf[iIndx];

					if (0 == ptPkg->m_iType)
					{
						break;
					}
					else
					{
						DataPackageRelease(ptPkg);
						ptFifo->m_iBase++;
					}
				}

				if (ptFifo->m_iTop - ptFifo->m_iBase <  ptFifo->m_iMaxPkgCnt)
				{
					iIndx = (ptFifo->m_iTop++) % ptFifo->m_iMaxPkgCnt;
					ptFifo->m_ptBuf[iIndx] = _ptPkg;
					ptFifo->m_iLastSeq = _ptPkg->m_iSeq;
					DataPackageRefer(_ptPkg);
				}
			}
			#ifdef TD_DEBUG
			else
			{
			//	td_printf(0, "fifo %p is full, drop pkg %p", ptFifo, _ptPkg);
			}
			#endif

		NEXT:			
			DataFIFOUnLock(ptFifo);
		}

		DataPackageRelease(_ptPkg);
		
		DataCenterUnLock(_ptDc);
	}	

	return iRet;
}

_TDataPackage *GetData(_TDataFIFO *_ptFifo)
{
	_TDataPackage *ptPkg = NULL;

	if (_ptFifo != NULL)
	{
		DataFIFOLock(_ptFifo);

		if (_ptFifo->m_iTop > _ptFifo->m_iBase)
		{
			int iIndx = (_ptFifo->m_iBase++) % _ptFifo->m_iMaxPkgCnt;

			ptPkg = _ptFifo->m_ptBuf[iIndx];
			if (_ptFifo->m_iBase == _ptFifo->m_iTop)
			{
				_ptFifo->m_iBase = 0;
				_ptFifo->m_iTop = 0;
			}
		}
		
		DataFIFOUnLock(_ptFifo);
	}

	return ptPkg;
}

void ClearFIFO(_TDataFIFO *_ptFifo)
{
	if (_ptFifo != NULL)
	{
		int i;

		DataFIFOLock(_ptFifo);

		for (i = _ptFifo->m_iBase; i < _ptFifo->m_iTop; i++)
		{
			int iIndx = i % _ptFifo->m_iMaxPkgCnt;
			_TDataPackage *ptPkg = _ptFifo->m_ptBuf[iIndx];

			DataPackageRelease(ptPkg);
		}

		_ptFifo->m_iBase = 0;
		_ptFifo->m_iTop = 0;
		_ptFifo->m_iLastSeq = 0;

		DataFIFOUnLock(_ptFifo);
	}
}

_TDataFIFO *MakeFIFO(int _iPri, int _iSize)
{
	_TDataFIFO *ptFifo = (_TDataFIFO *)malloc(sizeof(_TDataFIFO) + _iSize * sizeof(_TDataPackage *));

	if (ptFifo != NULL)
	{
		memset(ptFifo, 0, sizeof(*ptFifo));

		ptFifo->m_iMaxPkgCnt = _iSize;
		ptFifo->m_iPri = _iPri;
		DataFIFOLockInit(ptFifo);
	}

	return ptFifo;
}

void DestroyFIFO(_TDataFIFO *_ptFifo)
{
	if (_ptFifo != NULL)
	{
		ClearFIFO(_ptFifo);
		free(_ptFifo);
	}
}

void ChgFIFOType(int _iPri, _TDataFIFO *_ptFifo)
{
	if (_ptFifo)
	{
		DataFIFOLock(_ptFifo);
		_ptFifo->m_iPri = _iPri;
		DataFIFOUnLock(_ptFifo);
	}
}

int GetFifoPackNum( _TDataFIFO *_ptFifo)
{
	if (_ptFifo)
	{
		return (_ptFifo->m_iTop - _ptFifo->m_iBase);
	}
	else
	{
		return 0;
	}
}





