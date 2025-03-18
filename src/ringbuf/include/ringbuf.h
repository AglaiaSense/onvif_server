#ifndef _RING_BUF_H_
#define _RING_BUF_H_

//#include "public_def.h"
#include "common.h"

typedef struct
{
	void *m_ptSelftData;	//Ӧ�ò�����
} TRingBuf;

enum
{
	RINGBUF_WRITE_BLOCK = 0,	//�ռ䲻��ʱд����
	RINGBUF_WRITE_COVER = 1,	//�ռ䲻��ʱд����
};

// ����һ������buf
TRingBuf *ringbuf_Create(int _iBufSize, int _iType);

// ����һ������buf
void *ringbuf_Destroy(TRingBuf *_ptBuf);

// ����buf���ִ�ɶ�����
int ringbuf_DataSize(TRingBuf *_ptBuf);

// ����buf�ֿ�д����
int ringbuf_Capacity(TRingBuf *_ptBuf);

// д���ݵ�����buf
int ringbuf_Write(int _iSize, const void *_ptSrc, TRingBuf *_ptBuf);

// �ӻ���buf��ȡ����
int ringbuf_Read(int _iSize, void *_ptDst, TRingBuf *_ptBuf);

int ringbuf_Clear(TRingBuf *_ptBuf);

// zty 20120220
//���ƻ���buf����,��ɾ��
int ringbuf_Copy(int _iSize, void *_ptDst, TRingBuf *_ptBuf);

//ɾ������buf����
int ringbuf_Remove(int _iSize, TRingBuf *_ptBuf);
// zty add end

//��ȡring buf�е���ʼ���ݵ�ַ - dlq added at 20120919
unsigned char *ringbuf_GetData(TRingBuf *_ptBuf);
#endif

