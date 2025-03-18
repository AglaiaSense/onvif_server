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
	unsigned int	u32FrameID ;	//��ʾID,32λ,�̶�Ϊ: 0x 00 00 00 02
	unsigned int	u32FrameSize;	//֡��С32λ,����֡��С,�ֽ���
	unsigned char	u8FrameType;	//֡����,8λ,FRAME_I=0,  FRAME_P=1, FRAME_B=2 
	unsigned int	u32FrameNO;		//֡���,32λ,֡���,ѭ������
	unsigned int	u32TimeStamp;	//ʱ���,32λ,��ʱ��֪����ʲô��
	unsigned int	u32VStreamLen;	//��Ƶ���ݳ���,32λ,��Ƶ���ݴ�С
	unsigned short	u16AFrames;		//��Ƶ֡��,16λ,����Ƶ֡Я����Ƶ֡��
	unsigned short	u16AFrameSize;	//��Ƶ֡��С,16λ,��Ƶ֡��С
	unsigned char	u8ACoder;		//��Ƶѹ����ʽ,8λ,��ѹ��:0;    G711_A:0x01;    G711_U:0x02;    ADPCM_A :0x03;    G726:0x04;    AAC:0x16 //qiaohaijun add AAC
	unsigned short	reserved;		//16λ,����,32λ����
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
	int					m_iAudioSampleRate;//��Ƶ������
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

