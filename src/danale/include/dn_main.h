#ifndef __DN_MAIN_H__
#define __DN_MAIN_H__

#define MAX_STREAM_NUM	2

typedef enum{
	STREAM_MAIN = 1,		//������
	STREAM_SUB				//������
}Stream_Type;

typedef struct
{
	void*	hsess;				//ͨ��session
	int		iChn;				//ͨ����
	int		iVideoEnable;		//��Ƶʹ�ܱ�־
	int		iAudioEnable;		//��Ƶʹ�ܱ�־
}Dana_Session;

int dana_init(void *_pThis);

#endif
