#ifndef __DN_MAIN_H__
#define __DN_MAIN_H__

#define MAX_STREAM_NUM	2

typedef enum{
	STREAM_MAIN = 1,		//主码流
	STREAM_SUB				//子码流
}Stream_Type;

typedef struct
{
	void*	hsess;				//通道session
	int		iChn;				//通道号
	int		iVideoEnable;		//视频使能标志
	int		iAudioEnable;		//音频使能标志
}Dana_Session;

int dana_init(void *_pThis);

#endif
