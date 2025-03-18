/*
 *	file: channel.h
 *	description: channel process
 *	history: dongliqiang created at 20130228
 */

#ifndef _CHANNEL__H_
#define _CHANNEL__H_

#include "mutex_lock.h"
#include "onvif_shm_cmd.h"

#define MAX_SPS_PPS_LEN				256

typedef enum
{
	CHANNEL_UNINITED = 0,
	CHANNEL_INITED,	
	CHANNEL_UNCONNECTED,
	CHANNEL_CONNECTED
}CHANNEL_STAT;

typedef struct
{
	unsigned int	u32FrameID ;	//标示ID,32位,固定为: 0x 00 00 00 02
	unsigned int	u32FrameSize;	//帧大小32位,整个帧大小,字节数
	unsigned char	u8FrameType;	//帧类型,8位,FRAME_I=0,  FRAME_P=1, FRAME_B=2 
	unsigned int	u32FrameNO;		//帧序号,32位,帧序号,循环递增
	unsigned int	u32TimeStamp;	//时间戳,32位,暂时不知道有什么用
	unsigned int	u32VStreamLen;	//视频数据长度,32位,视频数据大小
	unsigned short	u16AFrames;		//音频帧数,16位,该视频帧携带音频帧数
	unsigned short	u16AFrameSize;	//音频帧大小,16位,音频帧大小
	unsigned char	u8ACoder;		//音频压缩方式,8位,不压缩:0;    G711_A:0x01;    G711_U:0x02;    ADPCM_A :0x03;    G726:0x04;    AAC:0x16 //qiaohaijun add AAC
	unsigned short	reserved;		//16位,保留,32位对齐
}S_FrameHeader,*pS_FrameHeader;

typedef struct
{
	char 		m_pcSps[MAX_SPS_PPS_LEN];
	int 		m_iSpsLen;
	char 		m_pcPps[MAX_SPS_PPS_LEN];
	int 		m_iPpsLen;
	char 		m_pcPps2[MAX_SPS_PPS_LEN];
	int 		m_iPps2Len;
}TSDPInfo;

//gui stream
typedef struct
{
	int					m_iOnvifCnt;	//use count
	int					m_iDanaleCnt;	//use count
	TGuiStreamChannel*	m_ptGuiShmData;	//share mem stream
}TGuiStream;

//video param
typedef struct
{
	int					m_iAudioSampleRate;//音频采样率
	TGuiVideoEncodeCnf 	m_tVEncCfg;
}TChannelParam;

typedef struct
{
	int 				m_iStat;				//channel stat
	int					m_iChan;				//current channel
	int					m_iStream;				//current stream
	TChannelParam		m_tChanParam;			//channel param
	TGuiStream			m_tGuiStream;			//share memory
	TSDPInfo			m_tSdpInfo;				//sdp info
	S_FrameHeader		m_tFrameHeader;			//frame header
	void *				m_pServer;				//rtsp server
	TMutexLock   		m_tLock;				//unused
}TChannel;

int ChannelInit(TChannel *_ptChannel);

int ChannelUnInit(TChannel *_ptChannel);

int ChannelConnect(TChannel *_ptChannel);

int ChannelDisConnect(TChannel *_ptChannel);

int ChannelGetParam(TChannel *_ptChannel);

#endif

