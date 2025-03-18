#ifndef _DATA_CENTER_H_
#define _DATA_CENTER_H_

//2010/05/31 duxiulong created
// ý�����ݷַ�����

#include "datapool.h"
#include "datalock.h"

enum
{
	FIFO_PRI_CYC = 1,	//���滷���������ʱѭ�����Ǿ�����
	FIFO_PRI_COM = 2,	//��ͨFIFO���������ʱ����������
	FIFO_PRI_SEQ = 3,	//���л�FIFO�������ݶ�ʧ���������ʱ������������
	FIFO_PRI_SEQ_NB = 4,	//���л�FIFO��������������
	FIFO_PRI_IF = 5
};

typedef struct _TDataFIFO
{
	struct _TDataFIFO *m_ptNext;
	TDataLock m_tLock;
	int m_iPri;			// ���ݴ������ȼ�
	int m_iBase;			// fifo baseָ��
	int m_iTop;			// fifo topָ��
	int m_iMaxPkgCnt;	//���ɻ������
	int m_iLastSeq;
	_TDataPackage *m_ptBuf[0];
} _TDataFIFO;

typedef struct
{
	TDataLock m_tLock;
	_TDataFIFO *m_ptFifo;
} _TDataCenter;	//����Դ

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
