/*
 *	file: rtsp_server.h
 *	description: rtsp server
 *	history: dongliqiang created at 20130228
 */
#ifndef __RTSP_SERVER_H__
#define __RTSP_SERVER_H__

#include "common.h"
#include "connection.h"
#include "channel.h"
#include "rtp.h"
#include "onvif_sdk.h"

#define HTTP_SESSION_COOKIE_LEN 		128

#define RTSP_MAX_MEM_POOL			(10 * 1024 * 1024)

#define WITH_SESSION_LOCK

typedef enum
{
	RTSP_INITED = 1,
	RTSP_UNINITED,
	RTSP_CONNECTED,
	RTSP_UNCONNECTED
}RTSP_STAT;

typedef struct
{
	int 			m_iSeq;					//消息序号
	int 			m_iCmdType;				//RTSP消息类型
	int 			m_iTrackId;				//track id.
	int 			m_iInterleavedANo; 		//interleaved audio no
	int 			m_iInterleavedVNo; 		//interleaved video no

	int			m_iRspCode;				//200, 401.....
	int			m_iAuthFlag;				//是否校验过
	char			*m_pCmd;				//保存命令, qiaohaijun @2013.12.13

	char 		m_pcRtspUrl[256];	
}TRtspInfo;

typedef struct
{
	int				m_iSessionID;			//session ID
	char 			m_pcHttpSessionCookie[HTTP_SESSION_COOKIE_LEN ]; //http sessionid
	int				m_iStat;				//连接状态, SESSION_STAT
	int				m_iChan;				//channel number , 0-MAX_CHANNEL_NUM
	int				m_iStream;				//0 - main stream; 1 - sub stram
	int				m_iNetMode;				//0 - tcp, 1 - udp
	TRtspInfo		m_tRtspInfo;			//rtsp info
	TRtpInfo		m_tRtpInfo;				//rtp header info
	TConnection *	m_ptConnect;			//rtsp connect		tcp发送和命令
	TConnection		m_tVRtpConnect;			//video rtp connect//udp数据
	TConnection		m_tVRtcpConnect;		//server video rtcp connect
	TConnection		m_tARtpConnect;			//audio rtp connect
	TConnection		m_tARtcpConnect;		//server audio rtcp connect
	void *			m_pServer;				//指示该session属于哪个rtsp server
	DListNode *		m_ptNode;				//指示自己属于哪个node
	unsigned int	m_uiHeartTime;			//heart time
	int 			m_iUsed;				//当前session是否可用
	int 			m_iSetupAudio;			//是否请求了audio
	int 			m_iSetupVideo;			//是否请求了video
	TMutexLock   	m_tLock;				//session lock
}TRtspSession;


int GetPort();

int FreePort(int _iPort);

int RtspServerInit(void *_pThis);

int RtspServerUnInit(TRtspServer *_ptServer);

int RtspServerStart(TRtspServer *_ptServer);

int RtspServerStop(TRtspServer *_ptServer);

int AcceptCallBack(int _iSocket);

int DisConnectCallBack(TConnection *_ptConnect);

int ReleaseRtspSession(TRtspSession *_ptSession);

void* ResetRtspSession(void *_pArg);

TRtspSession * FindSessionByHttpCookie(TRtspSession *_ptPostSession, char *_pcHttpCookie);


#endif
