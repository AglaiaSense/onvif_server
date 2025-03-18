/*
 *	file: rtsp_cmd.h
 *	description: rtsp cmd process
 *	history: dongliqiang created at 20130228
 */

#ifndef __RTSP_CMD_H__
#define __RTSP_CMD_H__

#include "rtsp_server.h"


#ifndef TD_DEBUG
#define SESN_DBG_LOCK1(_pAddr, _pcString)
#define SESN_DBG_LOCK2(_pAddr, _pcString)
#define SESN_DBG_LOCK3(_pAddr, _pcString)	
#define SESN_DBG_LOCK4(_pAddr, _pcString)
#else
	#ifdef SESN_LOCK
		#define SESN_DBG_LOCK1(_pAddr, _pcString) td_printf(0, "%s %s %d %p Wait Lock %s", __FILE__, __FUNCTION__, __LINE__, _pAddr, _pcString)
		#define SESN_DBG_LOCK2(_pAddr, _pcString) td_printf(0, "%s %s %d %p Got Lock%s", __FILE__, __FUNCTION__, __LINE__, _pAddr, _pcString)
		#define SESN_DBG_LOCK3(_pAddr, _pcString) td_printf(0, "%s %s %d %p Wait UN Lock %s", __FILE__, __FUNCTION__, __LINE__, _pAddr, _pcString)
		#define SESN_DBG_LOCK4(_pAddr, _pcString) td_printf(0, "%s %s %d %p Got UN Lock %s", __FILE__, __FUNCTION__, __LINE__, _pAddr, _pcString)
	#else
		#define SESN_DBG_LOCK1(_pAddr, _pcString)
		#define SESN_DBG_LOCK2(_pAddr, _pcString)
		#define SESN_DBG_LOCK3(_pAddr, _pcString)	
		#define SESN_DBG_LOCK4(_pAddr, _pcString)
	#endif
#endif



#define RTSP_RSP_200 				200
#define RTSP_RSP_400 				400 	//客户端出错, 请求有语法错误或无法实现
#define RTSP_RSP_401				401	//Unauthorized
#define RTSP_RSP_5XX 				501 	//服务器端出错, 服务器无法满足合法的请求
#define RTP_SERVER_PORT_START		10000	//DST商讨可用10000 - 11000
#define MAX_SESSION_NUM				128		//SESSION连接数
#define MAX_PORT_INTERVAL			2		//端口间隔

#define AUDIO_TRACK_ID 			1
#define VIDEO_TRACK_ID				2

#define AUTH_YES					1
#define AUTH_NO						0

typedef struct
{
	int m_iUsed;
	int m_iPort;
}TRtspPort;

//rtsp cmd
typedef enum RtspCmd
{
	 RTSP_CMD_SETUP = 0,
	 RTSP_CMD_TEARDOWN,
	 RTSP_CMD_OPTIONS,
	 RTSP_CMD_PLAY,
	 RTSP_CMD_PAUSE,
	 RTSP_CMD_DESCRIBE,
	 RTSP_CMD_SETPARAMETER,
	 RTSP_CMD_LAST
} ERtspCmd;

typedef enum RtpPayloadType
{
	RTP_PT_ULAW = 0, 
	RTP_PT_GSM = 3, 
	RTP_PT_G723 = 4, 
	RTP_PT_DVI4 = 5, 
	RTP_PT_ALAW = 8, 
	RTP_PT_G722 = 9, 
	RTP_PT_S16BE_STEREO = 10, 
	RTP_PT_S16BE_MONO = 11, 
	RTP_PT_MPEGAUDIO = 14, 
	RTP_PT_JPEG = 26, 
	RTP_PT_H261 = 31, 
	RTP_PT_MPEGVIDEO = 32, 
	RTP_PT_MPEG2TS = 33, 
	RTP_PT_H263 = 34, 
	RTP_PT_H264 = 96,
	RTP_PT_G726 = 97,
	RTP_PT_AMR = 98,

	RTP_PT_AAC = 99,//by qiaohaijun @2013.1.22
	
	RTP_PT_INVALID = 127
}ERtpPayloadType;

typedef enum
{
	SESSION_CONNECTED = 1,
	SESSION_UNCONNECTED,
	SESSION_ACCEPTED,
	SESSION_GET,
	SESSION_POST,
	SESSION_OPTIONS,
	SESSION_DESCRIBE,
	SESSION_SETUP,
	SESSION_PLAY,
	SESSION_PAUSE,
	SESSION_TEARDOWN
}SESSION_STAT;

int RtspReqIsSetup(const char *_pMsg);

int RtspReqIsPlay(const char *_pMsg);

int RtspReqIsPause(const char *_pMsg);

int RtspReqIsOption(const char *_pMsg);

int RtspReqIsDescribe(const char *_pMsg);

int RtspReqIsTearDown(const char *_pMsg);

int RtspReqIsSetParameter(const char *_pMsg);

int RtspResponse(TRtspSession *_ptSession);

int RtspCmdProcess(TRtspServer *_ptServer);

int ParseRtspServer(TRtspServer *_ptServer);

int ParseHttpServer(TRtspServer *_ptServer);

int ParseRtcpServer(TRtspServer *_ptServer);

int ParseRtspSession(TRtspSession *_ptSession);

int ParseHttpSession(TRtspSession *_ptSession);

int ParseRtcpSession(TRtspSession *_ptSession);

int ParseHttpCmd(TRtspSession *_ptSession, const char *_pcData, int _iSize);

int ParseRtcpCmd(TRtspSession *_ptSession, const char *_pcData, int _iSize);

int ParseRtspCmd(TRtspSession *_ptSession, const char *_pcData, int _iSize);

#endif

