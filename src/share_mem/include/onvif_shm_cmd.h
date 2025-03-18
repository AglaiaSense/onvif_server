//*****************************************
//	 文件名 	    : onvif_shm_cmd.h
//	 文件描述: 共享内存通信
//                            相关函数说明
//	修改记录 : modify at 20130527 by ptb
//*****************************************

#ifndef _ONVIF_SHM_CMD_H_
#define _ONVIF_SHM_CMD_H_

#include "guiparameter.h"

/*****************结构体重定义**************************/	
typedef struct
{
	int m_iVideoMod;			//视频输入制式
	int m_iBenginTm;			//开始时间，从当天00:00流过的秒
	int m_iEndTm;				//结束时间
	int m_iHue;					//色　调 0-100
	int m_iLum;					//亮　度 0-100
	int m_iContrast;			//对比度 0-100
	int m_iSaturation;			//饱和度 0-100
	int m_iGrayFlg;				//彩转黑类型(0:彩色, 1:灰色)
} VideoInCnf_EX;

//消息类型结构体
typedef struct
{
	unsigned short ProtocolVersion;		//协议版本，当前版本为0x0100
	unsigned short MsgType;				//消息类型
	unsigned short TransactionNo;		//事务编号，共有16位来表示，其中第一个比特位用来表示请求（0）和响应（1）
	unsigned short PacketLength;		//消息体长度，包括消息头长度
} __attribute__((packed)) MsgHeader;

//时钟
typedef struct
{
	int lCurrentTime;
} __attribute__((packed)) st_time;


/*****************函数声明**************************/	
int OnvifCmdInit(int _iTid);

int OnvifCmdRebootReq();

int OnvifCmdSendVerNum(char *_pcVersion);

int OnvifCmdHeartbeat();

int OnvifCmdGetip(TGuiIpMsg *_pstIpMsg);

int OnvifCmdSetip(TGuiIpMsg *_pstIpMsg);

int OnvifCmdIframeReq(int iChn);

int OnvifCmdStreamDisConnect(int _iChn);

int OnvifCmdSetTime(TGuiSetSysTimeReq *ptTime);

int OnvifCmdRebootPlatform();

int OnvifCmdGetVideoInCnf(int _iChn, VideoInCnf_EX *_PVInCnf);

int OnvifCmdSetAVEncodeCnf(int _iChn,const TGuiVideoEncodeCnf *_pstVEncodeCnf);

int OnvifCmdGetVideoEncodeCnf(int _iChn, TGuiVideoEncodeCnf * _pVEncodeCnf);

int OnvifCmdGetDevNum(int _itype);

int OnvifCmdGetPtzDecodeCnf(int _iChn, TGuiPtzDecodeCnf *_pPtzDCnf);

int OnvifCmdSetPtzDecodeCnf(int _iChn, const TGuiPtzDecodeCnf *_pstPtzDCnf);

int OnvifCmdGetMDAlarmCfg(int _iChn);

int OnvifCmdGetMDAlarmStatus(int _iChn);

int OnvifCmdSetVideoInCnf(int _iChn, VideoInCnf_EX *_pVInCnf);

int OnvifCmdSetPTZ(TGuiPtzLocalCode *pPtzLocalCode);

int OnvifCmdGetSystemMessage(TGuiGetSysMsgRsp *_pstDevMsg);

int OnvifCmdGetIspInfo(TGuiVideoISPInfo  *_ptpIspInfo);

int OnvifCmdSetIspInfo(TGuiVideoISPInfo *_ptIspInfo);

int OnvifCmdGetVideoChnSizeList(int _iChn, int  *_ptpVideoList, int _iMaxCnt);

int OnvifCmdGetNTP(char *_pNTPIP, int _iBufLen);

int OnvifCmdSetNTP(char *_pNTPIP);

int OnvifCmdGetProductModel();

int OnvifCmdGetHttpPort();

int OnvifCmdRestore();

int OnvifCmdGetOsd(int _iChn, TGuiVideoOverlayCnf *_ptGetOsd);

int OnvifCmdSetOsd(int _iChn, TGuiVideoOverlayCnf *_ptSetOsd);

int OnvifCmdGetSnapshot(int _iChn);

int OnvifCmdGetSnapshotEx(int _iChn, char *_pcNameBuffer);

int OnvifCmdGetChnType(int _iChn);

int OnvifCmdStreamConnect(int _iChn);

int OnvifCmdSetDVRVideoInCnf(int _iChn, VideoInCnf_EX* _pVInCnf);

int OnvifCmdGetAudioRateCnf(int _iChn);

int OnvifCmdSetAudioRateCnf(int _iChn, int _iAudioRate);

//RelayOutput
int OnvifCmdGetRelayOutputState(int _iChn);

int OnvifCmdSetRelayOutputState(int _iChn, int _iState);

int OnvifCmdGetRelayOutputCnf(int _iPort);

int OnvifCmdSetRelayOutputCnf(int _iPort, int _iType);

//录像部分
int OnvifCmdGetVRTemplate(int _iChn, TGuiGetVideoTemplateRsp *_pStVRTemplate);

int OnvifCmdSetVRTemplate(TGuiSetVideoTemplateReq *_pStVRTemplate);

int OnvifCmdGetVRStrategy(TGuiGetVideoStrategyRsp *_pGetVStrategy);

int OnvifCmdSetVRStrategy(TGuiGetVideoStrategyRsp *_pSetVStrategy);

int OnvifCmdGetVFileCount(TGuiCountVideoFileReq *_pCondition);

int OnvifCmdQueryVFile(TGuiQueryVideoFileReq *_pCondition);

int OnvifCmdSetFlip(int _iChn, int _iType);

int OnvifCmdGetPtzCompassCnf(int _iChn, TGuiGetPTZCompassRsp *_ptGetPTZCompassRsp);

int OnvifCmdSetPtzCompassCnf(TGuiSetPTZCompassReq *_ptSetPTZCompassReq);

#endif

