//*********************************************
// ���ݷַ�����
// duxiulong 2011/03/01 create
//
// ˵��:
// 1 ʹ�ÿ�ǰ��Ҫ����datacneter_PoolInit()����
// 2 ÿ������Դ��Ӧһ��TDataCenter
// 3 ÿ����Ҫ�����ݽ��д����ҵ���Ӧһ��DataFIFO
// 4 ҵ����Ҫ����ĳ��Դ�����ݣ��轫��DataFIFO�󶨵���Ӧ��DataCenter
// 5 Դÿ����һ�����ݣ�ͨ��
//*********************************************
#ifndef _LIB_DATA_CENTER_H_
#define _LIB_DATA_CENTER_H_

//#include "public_def.h"
#include "common.h"

#define TDataCenter 	void
#define TDataFIFO 	void

#ifndef byte
//#define byte unsigned char
#endif

typedef struct
{
	int m_iDataSize;			// ��ʵ�����ݳ���
	int m_iType;				// ��������
	unsigned char m_bData[0];			// ������
} TDataPackage;

enum
{
	DC_FIFO_PRI_CYC = 1,	//���滷���������ʱѭ�����Ǿ�����
	DC_FIFO_PRI_COM = 2,	//��ͨFIFO���������ʱ����������
	DC_FIFO_PRI_SEQ = 3,	//���л�FIFO�������ݶ�ʧ���������ʱ������������
	DC_FIFO_PRI_SEQ_NB = 4,
	DC_FIFO_PRI_IF	= 5, //
};

// ���ݻ���س�ʼ��
int datacenter_PoolInit(int _iMaxSize);
int datacenter_PoolUnint();

// ����һ���ڴ�
TDataPackage *datacenter_CreatePackage(int _iSize);

// ����ڴ����ü��� 
void datacenter_PackageRefer(TDataPackage *_ptPkg);
// �����ڴ����ü���
void datacenter_PackageRelease(TDataPackage *_ptPkg);

// ����һ��FIFO����
TDataFIFO *datacenter_MakeFifo(int _iPri, int _iSize);
// ����һ������
void datacenter_DestroyFifo(TDataFIFO *_ptFifo);

void datacenter_ChgFifoPri(int _iPri, TDataFIFO *_ptFifo);


// �Ӷ��л�ȡһ������
TDataPackage *datacenter_GetPackFromFifo(TDataFIFO *_ptFifo);
// ��ն�������
void datacenter_ClearFifo(TDataFIFO *_ptFifo);

// ����һ�����ݷַ�����
TDataCenter *datacenter_Create();
// ����һ�����ݷַ�����
void datacenter_Destroy(TDataCenter *_ptDc);
// ��fifo�����ݷַ�����
int datacenter_BindFifo(TDataFIFO *_ptFifo, TDataCenter *_ptDc);
// ���fifo�����ݷַ�����
int datacenter_UnBindFifo(TDataFIFO *_ptFifo, TDataCenter *_ptDc);
// 
int datacenter_HasNoFifoBind(TDataCenter *_ptDc);

// �����ݰ��������ݷַ����ķַ�
int datacenter_PushPackage(TDataPackage *_ptPkg, TDataCenter *_ptDc);

void datacenter_DebugMsg(char *_strMsg);

//int datacenter_RtMsgGetDebugInfo(char *_strMsg, int _iLen)


int datacenter_GetFifoPackNum(TDataFIFO *_ptFifo);

#endif

