/****************************************************************************
[File information]
FileName = onvif_shm_cmd.c
Description = command  parse 
Author = tcf
Date = 20110215
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#include "onvifH.h"
#include "guitransport.h"
#include "share_mem.h"
#include "common.h"
#include "onvif_shm_cmd.h"
#include "proparser.h"
#include "macro.h"
#include "onvif_sdk.h"

static int g_iGuiParaTid = 0;
extern OnvifCtrl g_stOnvifCtrl;

/****************************************************
名称：   OnvifCmdInit
功能：   初始化命令通道ID
参数：  _iTid:命令通道ID
返回值:	    0:成功 			
修改历史:20120302 v1.0 created by czl
****************************************************/
int OnvifCmdInit(int _iTid)
{
	g_iGuiParaTid = _iTid;
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdRebootReq
功能:		向共享内存发送重启命令
参数:		无
返回值:	成功	:	0
			失败	: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdRebootReq()	
{
	char creqbuf[sizeof(TGuiSystemReSetReq)] = {0};
	TGuiSystemReSetReq *ptReq = (TGuiSystemReSetReq *) creqbuf;
	
	ptReq->m_iCode1 = GUI_SYS_SPEC_CODE1;
	ptReq->m_iCode2 = GUI_SYS_SPEC_CODE2;
	
	if(snd_rcv(g_iGuiParaTid, NULL, creqbuf, 0, sizeof(TGuiSystemReSetReq), GUI_SYS_RESET_REQ) != 0)
	{
		td_printf(0, "OnvifCmdRebootReq error \n");
		return TD_ERROR;
	}
	
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdSendVerNum
功能:		向共享内存发送onvif版本信息
参数:		_pcVersion:	版本信息字符串
返回值:	成功	:	0
			失败	: 	!0
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSendVerNum(char *_pcVersion)	
{
	alignedchar creqbuf[sizeof(TGuiStartUpMsg)] = {0};
	TGuiStartUpMsg *ptReq = (TGuiStartUpMsg *) creqbuf;
	TGuiStartUpRsp stRsp  = {0};
	
	if(pointer_valid_check(_pcVersion))
	{
		return TD_ERROR;
	}
	
	ptReq->m_iBncMode      = -1;
    ptReq->m_iVgaMode      = -1;
    ptReq->m_iMasterDspDev = -1;
    strncpy(ptReq->m_strGuiVer, _pcVersion, GUI_VER_LEN);
	
	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, creqbuf, sizeof(TGuiStartUpRsp), sizeof(TGuiStartUpMsg), GUI_START_UP_MSG) != 0)
	{
		td_printf(0, "OnvifCmdSendVerNum error \n");
		return TD_ERROR;
	}
	
	return stRsp.m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetDevNum
功能:		根据设备类型从共享内存
			获取设备数目
参数:		_itype	:	设备类型
返回值:	成功	:	设备数目
			失败	: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetDevNum(int _itype)
{
	alignedchar creqbuf[sizeof(TGuiGetDevNumReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiGetDevNumRsp)] = {0};
	TGuiGetDevNumReq *ptReq = (TGuiGetDevNumReq *) creqbuf;
	TGuiGetDevNumRsp *ptRsp = (TGuiGetDevNumRsp *) crspbuf;

	ptReq->m_iDevId = _itype;

	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiGetDevNumRsp), sizeof(TGuiGetDevNumReq), GUI_GET_DEV_NUM_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetDevNum error\n");
		return TD_ERROR;
	}
	
	return ptRsp->m_iNum;
}

/****************************************************
名称:		OnvifCmdSetTime
功能:		设置系统时间
参数:		ptTime	:	时间结构体
返回值:	成功	:	0
			失败	: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetTime(TGuiSetSysTimeReq *ptTime)
{
	alignedchar creqbuf[sizeof(TGuiSetSysTimeReq)]  = {0};
	TGuiSetSysTimeReq *ptReq = (TGuiSetSysTimeReq *) creqbuf;
	
	ptReq->m_iTimeZone = ptTime->m_iTimeZone;
	ptReq->m_iYear     = ptTime->m_iYear;
	ptReq->m_iMon      = ptTime->m_iMon;
	ptReq->m_iDay      = ptTime->m_iDay;
	ptReq->m_iHour     = ptTime->m_iHour;
	ptReq->m_iMin      = ptTime->m_iMin;
	ptReq->m_iSec      = ptTime->m_iSec;

	if(snd_rcv(g_iGuiParaTid, NULL, creqbuf, 0, sizeof(TGuiSetSysTimeReq), GUI_SET_SYS_TIME_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetTime error\n");
		return TD_ERROR;
	}
	
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdGetip
功能:		获取网络相关参数
参数:		_pstIpMsg	:	网络参数结构体
返回值:	成功		:	0
			失败		: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetip(TGuiIpMsg *_pstIpMsg)
{
	alignedchar crspbuf[sizeof(TGuiGetIpRsp)] = {0};
	TGuiGetIpRsp *ptRsp = (TGuiGetIpRsp *) crspbuf;

	if(snd_rcv(g_iGuiParaTid, crspbuf, NULL, sizeof(TGuiGetIpRsp), 0, GUI_GET_IP_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetip error\n");
		return TD_ERROR;
	}
	
	_pstIpMsg->m_blIsHandSetDns = ptRsp->m_tIpMsg.m_blIsHandSetDns ;
	_pstIpMsg->m_iGetIpMethod   = ptRsp->m_tIpMsg.m_iGetIpMethod;
	strcpy(_pstIpMsg->m_strDNS, ptRsp->m_tIpMsg.m_strDNS);
	strcpy(_pstIpMsg->m_strGetWay , ptRsp->m_tIpMsg.m_strGetWay);
	strcpy(_pstIpMsg->m_strIp , ptRsp->m_tIpMsg.m_strIp);
	strcpy(_pstIpMsg->m_strMac,ptRsp->m_tIpMsg.m_strMac);
	strcpy(_pstIpMsg->m_strSecDNS, ptRsp->m_tIpMsg.m_strSecDNS);
	strcpy(_pstIpMsg->m_strSubMsk, ptRsp->m_tIpMsg.m_strSubMsk);
	
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdSetip
功能:		设置网络相关参数
参数:		_pstIpMsg	:	网络参数结构体
返回值:	成功		:	0
			失败		: 	!0
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetip(TGuiIpMsg *_pstIpMsg)
{
	alignedchar crspbuf[sizeof(TGuiSetIpRsp)] = {0};
	alignedchar creqbuf[sizeof(TGuiSetIpReq)] = {0};
	TGuiSetIpRsp *ptRsp = (TGuiSetIpRsp *) crspbuf;
	TGuiSetIpReq *ptReq = (TGuiSetIpReq *) creqbuf;
	
	ptReq->m_tIpMsg.m_blIsHandSetDns = _pstIpMsg->m_blIsHandSetDns;
	ptReq->m_tIpMsg.m_iGetIpMethod   = _pstIpMsg->m_iGetIpMethod;
	strcpy(ptReq->m_tIpMsg.m_strDNS, _pstIpMsg->m_strDNS);
	strcpy(ptReq->m_tIpMsg.m_strGetWay, _pstIpMsg->m_strGetWay);
	strcpy(ptReq->m_tIpMsg.m_strIp, _pstIpMsg->m_strIp);
	strcpy(ptReq->m_tIpMsg.m_strMac, _pstIpMsg->m_strMac);
	strcpy(ptReq->m_tIpMsg.m_strSecDNS, _pstIpMsg->m_strSecDNS);
	strcpy(ptReq->m_tIpMsg.m_strSubMsk, _pstIpMsg->m_strSubMsk);
	
	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiSetIpRsp), sizeof(TGuiSetIpReq), GUI_SET_IP_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetip error\n");
		return TD_ERROR;
	}
	
	_pstIpMsg->m_blIsHandSetDns = ptRsp->m_tIpMsg.m_blIsHandSetDns ;
	_pstIpMsg->m_iGetIpMethod   = ptRsp->m_tIpMsg.m_iGetIpMethod;
	strcpy(_pstIpMsg->m_strDNS, ptRsp->m_tIpMsg.m_strDNS);
	strcpy(_pstIpMsg->m_strGetWay , ptRsp->m_tIpMsg.m_strGetWay);
	strcpy(_pstIpMsg->m_strIp , ptRsp->m_tIpMsg.m_strIp);
	strcpy(_pstIpMsg->m_strMac,ptRsp->m_tIpMsg.m_strMac);
	strcpy(_pstIpMsg->m_strSecDNS, ptRsp->m_tIpMsg.m_strSecDNS);
	strcpy(_pstIpMsg->m_strSubMsk, ptRsp->m_tIpMsg.m_strSubMsk);
	
	return ptRsp->m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetPtzDecodeCnf
功能:		获取PTZ控制协议相关参数
参数:		_iChn		:	通道号(最小1)
			_pPtzDCnf	:	PTZ参数结构体
返回值:	成功		:	0
			失败		: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetPtzDecodeCnf(int _iChn, TGuiPtzDecodeCnf *_pPtzDCnf)
{
	alignedchar creqbuf[sizeof(TGuiGetPtzDecodeCnfReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiGetPtzDecodeCnfRsp)] = {0};
	TGuiGetPtzDecodeCnfReq *ptReq = (TGuiGetPtzDecodeCnfReq *) creqbuf;
	TGuiGetPtzDecodeCnfRsp *ptRsp = (TGuiGetPtzDecodeCnfRsp *) crspbuf;

	ptReq->m_iChn = _iChn;

	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiGetPtzDecodeCnfRsp), sizeof(TGuiGetPtzDecodeCnfReq), GUI_GET_PTZ_DECODE_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetPtzDecodeCnf error\n");
		return TD_ERROR;
	}
	
	_pPtzDCnf->m_iAddr      = ptRsp->m_tCnf.m_iAddr;
	_pPtzDCnf->m_iBitRate   = ptRsp->m_tCnf.m_iBitRate;
	_pPtzDCnf->m_iCheckType = ptRsp->m_tCnf.m_iCheckType;
	_pPtzDCnf->m_iComAttrEn = ptRsp->m_tCnf.m_iComAttrEn;
	_pPtzDCnf->m_iDataBits  = ptRsp->m_tCnf.m_iDataBits;
	_pPtzDCnf->m_iSportId   = ptRsp->m_tCnf.m_iSportId;
	_pPtzDCnf->m_iStopBits  = ptRsp->m_tCnf.m_iStopBits;
	strcpy(_pPtzDCnf->m_strProtocol, ptRsp->m_tCnf.m_strProtocol);

	return TD_OK;
}

/****************************************************
名称:		OnvifCmdSetPtzDecodeCnf
功能:		设置PTZ控制协议相关参数
参数:		_iChn		:	通道号(最小1)
			_pstPtzDCnf	:	PTZ参数结构体
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetPtzDecodeCnf(int _iChn, const TGuiPtzDecodeCnf *_pstPtzDCnf)
{
	alignedchar creqbuf[sizeof(TGuiSetPtzDecodeCnfReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiSetPtzDecodeCnfRsp)] = {0};
	TGuiSetPtzDecodeCnfReq *ptReq = (TGuiSetPtzDecodeCnfReq *) creqbuf;
	TGuiSetPtzDecodeCnfRsp *ptRsp = (TGuiSetPtzDecodeCnfRsp *) crspbuf;

	ptReq->m_iChn = _iChn;
	ptReq->m_tCnf.m_iAddr      = _pstPtzDCnf->m_iAddr;
	ptReq->m_tCnf.m_iBitRate   = _pstPtzDCnf->m_iBitRate;
	ptReq->m_tCnf.m_iCheckType = _pstPtzDCnf->m_iCheckType;
	ptReq->m_tCnf.m_iComAttrEn = _pstPtzDCnf->m_iComAttrEn;
	ptReq->m_tCnf.m_iDataBits  = _pstPtzDCnf->m_iDataBits;
	ptReq->m_tCnf.m_iSportId   = _pstPtzDCnf->m_iSportId;
	ptReq->m_tCnf.m_iStopBits  = _pstPtzDCnf->m_iStopBits;
	strcpy(ptReq->m_tCnf.m_strProtocol, _pstPtzDCnf->m_strProtocol);

	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiSetPtzDecodeCnfRsp), sizeof(TGuiSetPtzDecodeCnfReq), GUI_SET_PTZ_DECODE_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetPtzDecodeCnf error\n");
		return TD_ERROR;
	}
	
	return ptRsp->m_iRetCode;
}

/****************************************************
名称:		OnvifCmdIframeReq
功能:		请求I帧
参数:		_iChn		:	通道号(最小1)
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdIframeReq(int iChn)	
{
	alignedchar creqbuf[sizeof(TGuiForceIframeReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiForceIframeRsp)] = {0};
	TGuiForceIframeReq *ptReq = (TGuiForceIframeReq *) creqbuf;
	TGuiForceIframeRsp *ptRsp = (TGuiForceIframeRsp *) crspbuf;

	ptReq->m_iChn = iChn;

	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiForceIframeRsp), sizeof(TGuiForceIframeReq), GUI_FORCE_IFRAME_REQ) != 0)
	{
		td_printf(0, "OnvifCmdIframeReq error\n");
		return TD_ERROR;
	}

	td_printf(0, "i frame req ret code is %d", ptRsp->m_iRetCode);
	return ptRsp->m_iRetCode;
}

/****************************************************
名称:		OnvifCmdStreamConnect
功能:		请求视频
参数:		_iChn		:	通道号(最小1)
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdStreamConnect(int _iChn)
{
	alignedchar creqbuf[sizeof(TGuiStreamConnectReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiStreamConnectRsp)] = {0};
	TGuiStreamConnectReq *ptReq = (TGuiStreamConnectReq *) creqbuf;
	TGuiStreamConnectRsp *ptRsp = (TGuiStreamConnectRsp *) crspbuf;
	int iRet = -1; 

	ptReq->m_iChn = _iChn - 1;
	ptReq->m_iPid = getpid();

	//每次都要请求一次，向内核索要码流，若前端没视频，则base==top,有视频则绑定fifo，移动top.
	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiStreamConnectRsp), sizeof(TGuiStreamConnectReq), GUI_STREAM_CONNECT_REQ) != 0)
	{
		td_printf(0, "OnvifCmdStreamReq error\n");
		return TD_ERROR;
	}

	if(ptRsp->m_iKey < 0)
	{
		td_printf(0, "get shm key error. chno = %d, key = %d, 0x%x", _iChn -1 , ptRsp->m_iKey, ptRsp->m_iKey);
		return TD_ERROR;
	}

	if(NULL != (TGuiStreamChannel *)get_chn_shmaddr(ptReq->m_iChn))
	{
		return TD_OK;
	}
	
	iRet = gui_transtream_init(ptReq->m_iChn, ptRsp);
	if(iRet < 0)
	{
		td_printf(0,  "gui_transtream_init error %d\n", iRet);
		return TD_ERROR;
	}
	
	return iRet;
}

/****************************************************
名称:		OnvifCmdStreamDisConnect
功能:		断开视频
参数:		_iChn		:	通道号(最小1)
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdStreamDisConnect(int _iChn)
{
	alignedchar creqbuf[sizeof(TGuiStreamDisConnectReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiStreamDisConnectRsp)] = {0};
	TGuiStreamDisConnectReq *ptReq = (TGuiStreamDisConnectReq *) creqbuf;
	TGuiStreamDisConnectRsp *ptRsp = (TGuiStreamDisConnectRsp *) crspbuf;

	//ptReq->m_iChn = _iChn - 1;
	ptReq->m_iChn = _iChn; //changguoxing modified 20130506.
	ptReq->m_iPid = getpid();
	
	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiStreamDisConnectRsp), sizeof(TGuiStreamDisConnectReq), GUI_STREAM_DISCONNECT_REQ) != 0)
	{
		td_printf(0, "OnvifCmdStreamDisConnect error\n");
		return -1;
	}
	
	return ptRsp->m_iRetCode;
}

/****************************************************
名称:		OnvifCmdSetAVEncodeCnf
功能:		设置视频编码参数
参数:		_iChn			:	通道号(最小1)
			_pstVEncodeCnf	:	视频编码参数结构体
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetAVEncodeCnf(int _iChn, const TGuiVideoEncodeCnf *_pstVEncodeCnf)
{
	alignedchar creqbuf[sizeof(TGuiSetVideoEncodeCnfReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiSetVideoEncodeCnfRsp)] = {0};
	TGuiSetVideoEncodeCnfReq *ptReq = (TGuiSetVideoEncodeCnfReq *) creqbuf;
	TGuiSetVideoEncodeCnfRsp *ptRsp = (TGuiSetVideoEncodeCnfRsp *) crspbuf;

	ptReq->m_iChn = _iChn;
	ptReq->m_tCnf.m_iBitRate         = _pstVEncodeCnf->m_iBitRate;
	ptReq->m_tCnf.m_iBitStreamType   = _pstVEncodeCnf->m_iBitStreamType;
	ptReq->m_tCnf.m_iComplexType     = _pstVEncodeCnf->m_iComplexType;
	ptReq->m_tCnf.m_iEncodeType      = _pstVEncodeCnf->m_iEncodeType;
	ptReq->m_tCnf.m_iAudioEncodeType = _pstVEncodeCnf->m_iAudioEncodeType;
	ptReq->m_tCnf.m_iFrameRate       = _pstVEncodeCnf->m_iFrameRate;
	ptReq->m_tCnf.m_iIFrameRate      = _pstVEncodeCnf->m_iIFrameRate;
	ptReq->m_tCnf.m_iPicQualityLv    = _pstVEncodeCnf->m_iPicQualityLv;
	ptReq->m_tCnf.m_iPicSize         = _pstVEncodeCnf->m_iPicSize;
	td_printf(0xFF0000,"ptReq->m_tCnf.m_iPicQualityLv = %d",ptReq->m_tCnf.m_iPicQualityLv);
	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiSetVideoEncodeCnfRsp), sizeof(TGuiSetVideoEncodeCnfReq), GUI_SET_VENCODE_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetAVEncodeCnf error\n");
		return TD_ERROR;
	}
	return ptRsp->m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetVideoEncodeCnf
功能:		获取视频编码参数
参数:		_iChn		:	通道号(最小1)
			pVEncodeCnf	:	视频编码参数结构体
返回值:	成功		:	0
			失败		: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetVideoEncodeCnf(int iChn, TGuiVideoEncodeCnf * pVEncodeCnf)
{
	alignedchar creqbuf[sizeof(TGuiGetVideoEncodeCnfReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiGetVideoEncodeCnfRsp)] = {0};
	TGuiGetVideoEncodeCnfReq *ptReq = (TGuiGetVideoEncodeCnfReq *) creqbuf;
	TGuiGetVideoEncodeCnfRsp *ptRsp = (TGuiGetVideoEncodeCnfRsp *) crspbuf;

	ptReq->m_iChn =  iChn;

	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiGetVideoEncodeCnfRsp), sizeof(TGuiGetVideoEncodeCnfReq), GUI_GET_VENCODE_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetVideoEncodeCnf error %d\n", iChn);
		return TD_ERROR;
	}
	
	pVEncodeCnf->m_iAudioEncodeType = ptRsp->m_tCnf.m_iAudioEncodeType;
	pVEncodeCnf->m_iBitRate         = ptRsp->m_tCnf.m_iBitRate;
	pVEncodeCnf->m_iBitStreamType   = ptRsp->m_tCnf.m_iBitStreamType;
	pVEncodeCnf->m_iComplexType     = ptRsp->m_tCnf.m_iComplexType;
	pVEncodeCnf->m_iEncodeType      = ptRsp->m_tCnf.m_iEncodeType & 0x0f;
	pVEncodeCnf->m_iFrameRate       = ptRsp->m_tCnf.m_iFrameRate;
	pVEncodeCnf->m_iIFrameRate      = ptRsp->m_tCnf.m_iIFrameRate;
	pVEncodeCnf->m_iPicQualityLv    = ptRsp->m_tCnf.m_iPicQualityLv;
	pVEncodeCnf->m_iPicSize         = ptRsp->m_tCnf.m_iPicSize;

	td_printf(0,"GetpVEncodeCnf->m_iPicQualityLv = %d",pVEncodeCnf->m_iPicQualityLv);

	return TD_OK;
}

/****************************************************
名称:		OnvifCmdRebootPlatform
功能:		重启外挂
参数:		无
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdRebootPlatform()
{
	if(snd_rcv(g_iGuiParaTid, NULL, NULL, 0, 0, GUI_EXIT_MSG) != 0)
	{
		td_printf(0, "OnvifCmdRebootPlatform error\n");
		return TD_ERROR;
	}
	
	td_printf(0, "OnvifCmdRebootPlatform !!!\n");
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdHeartbeat
功能:		外挂心跳
参数:		无
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdHeartbeat()
{
	if(snd_rcv(g_iGuiParaTid, NULL, NULL, 0, 0, GUI_CMD_NOP) != 0)
	{
		td_printf(0, "OnvifCmdHeartbeat error\n");
		return TD_ERROR;
	}
	
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdGetVideoInCnf
功能:		获取视频输入参数
参数:		iChn			:	通道号(最小1)
			PVInCnf		:	视频参数结构体
返回值:	成功		:	0
			失败		: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetVideoInCnf(int _iChn, VideoInCnf_EX *_PVInCnf)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	alignedchar creqbuf[sizeof(TGuiGetVideoInCnfReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiGetVideoInCnfRsp)] = {0};
	TGuiGetVideoInCnfReq *ptReq = (TGuiGetVideoInCnfReq *) creqbuf;
	TGuiGetVideoInCnfRsp *ptRsp = (TGuiGetVideoInCnfRsp *) crspbuf;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	ptReq->m_iChn = _iChn;

	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiGetVideoInCnfRsp), sizeof(TGuiGetVideoInCnfReq), GUI_GET_VIDEOIN_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetVideoInCnf error %d\n", _iChn - 1);
		return TD_ERROR;
	}

	_PVInCnf->m_iVideoMod	= ptRsp->m_iVideMode;
	if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		_PVInCnf->m_iBenginTm   = ptRsp->m_tCnf[0].m_iBenginTm;
		_PVInCnf->m_iEndTm      = ptRsp->m_tCnf[0].m_iEndTm;
		_PVInCnf->m_iHue        = ptRsp->m_tCnf[0].m_iHue;
		_PVInCnf->m_iLum        = ptRsp->m_tCnf[0].m_iLum;
		_PVInCnf->m_iContrast   = ptRsp->m_tCnf[0].m_iContrast;
		_PVInCnf->m_iSaturation = ptRsp->m_tCnf[0].m_iSaturation;
		_PVInCnf->m_iGrayFlg    = ptRsp->m_tCnf[0].m_iGrayFlg;
	}
	else if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		_PVInCnf->m_iBenginTm   = ptRsp->m_tCnf[GUI_VIVEOIN_TM_CNF_CNT - 1].m_iBenginTm;
		_PVInCnf->m_iEndTm      = ptRsp->m_tCnf[GUI_VIVEOIN_TM_CNF_CNT - 1].m_iEndTm;
		_PVInCnf->m_iHue        = ptRsp->m_tCnf[GUI_VIVEOIN_TM_CNF_CNT - 1].m_iHue;
		_PVInCnf->m_iLum        = ptRsp->m_tCnf[GUI_VIVEOIN_TM_CNF_CNT - 1].m_iLum;
		_PVInCnf->m_iContrast   = ptRsp->m_tCnf[GUI_VIVEOIN_TM_CNF_CNT - 1].m_iContrast;
		_PVInCnf->m_iSaturation = ptRsp->m_tCnf[GUI_VIVEOIN_TM_CNF_CNT - 1].m_iSaturation;
		_PVInCnf->m_iGrayFlg    = ptRsp->m_tCnf[GUI_VIVEOIN_TM_CNF_CNT - 1].m_iGrayFlg;
	}
	else
	{
		td_printf(0, "%s %d : Device type[%d] is error!\n",__FUNCTION__, __LINE__, ptOnvifCtrl->m_stNvsInfo.m_iDeviceType);
		return TD_ERROR;
	}

	return TD_OK;
}

/****************************************************
名称:		OnvifCmdSetVideoInCnf
功能:		设置视频输入参数
参数:		_iChn		:	通道号(从1开始)
			_pVInCnf	:	视频输入参数结构体
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetVideoInCnf(int _iChn, VideoInCnf_EX *_pVInCnf)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	alignedchar cReqBuf[sizeof(TGuiSetVideoInCnfReq)] = {0};
	alignedchar cRspBuf[sizeof(TGuiSetVideoInCnfRsp)] = {0};
	TGuiSetVideoInCnfReq* ptReq = (TGuiSetVideoInCnfReq*)cReqBuf;
	TGuiSetVideoInCnfRsp *ptRsp = (TGuiSetVideoInCnfRsp*)cRspBuf;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	ptReq->m_iChn = _iChn;
	ptReq->m_iVideMode			   = _pVInCnf->m_iVideoMod;
	ptReq->m_tCnf[0].m_iBenginTm   = _pVInCnf->m_iBenginTm;
	ptReq->m_tCnf[0].m_iEndTm      = _pVInCnf->m_iEndTm;
	ptReq->m_tCnf[0].m_iHue 	   = _pVInCnf->m_iHue;
	ptReq->m_tCnf[0].m_iLum 	   = _pVInCnf->m_iLum;
	ptReq->m_tCnf[0].m_iContrast   = _pVInCnf->m_iContrast;
	ptReq->m_tCnf[0].m_iSaturation = _pVInCnf->m_iSaturation;
	ptReq->m_tCnf[0].m_iGrayFlg    = _pVInCnf->m_iGrayFlg;
	
	if(snd_rcv(g_iGuiParaTid, cRspBuf, cReqBuf, sizeof(TGuiSetVideoInCnfRsp), sizeof(TGuiSetVideoInCnfReq), GUI_SET_VIDEOIN_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetVideoInCnf chn %d error\n", _iChn);
		return TD_ERROR;
	}
	
	return ptRsp->m_iRetCode;
}

/****************************************************
名称:		OnvifCmdSetDVRVideoInCnf
功能:		设置DVR(临时值)视频输入参数
参数:		iChn			:	通道号(从1开始)
			pVInCnf		:	视频输入参数结构体
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetDVRVideoInCnf(int _iChn, VideoInCnf_EX* _pVInCnf)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	alignedchar cReqBuf[sizeof(TGuiChgVideoInCnfReq)] = {0};
	alignedchar cRspBuf[sizeof(TGuiChgVideoInCnfRsp)] = {0};
	TGuiChgVideoInCnfReq* ptReq = (TGuiChgVideoInCnfReq*)cReqBuf;
	TGuiChgVideoInCnfRsp* ptRsp = (TGuiChgVideoInCnfRsp*)cRspBuf;
		
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	ptReq->m_iChn        = _iChn;
	ptReq->m_iVideoMod   = _pVInCnf->m_iVideoMod;
	ptReq->m_iHue        = _pVInCnf->m_iHue;
	ptReq->m_iLum        = _pVInCnf->m_iLum;
	ptReq->m_iContrast   = _pVInCnf->m_iContrast;
	ptReq->m_iSaturation = _pVInCnf->m_iSaturation;
	ptReq->m_iGrayFlg    = _pVInCnf->m_iGrayFlg;

	if(snd_rcv(g_iGuiParaTid, cRspBuf, cReqBuf, sizeof(TGuiChgVideoInCnfRsp), sizeof(TGuiChgVideoInCnfReq), GUI_CHG_VIDEOIN_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetVideoInCnf chn %d error\n", _iChn);
		return TD_ERROR;
	}

	return ptRsp->m_iRetCode;
}

/****************************************************
名称:		OnvifCmdSetPTZ
功能:		发送PTZ控制码
参数:		pPtzLocalCode	:	PTZ控制参数
返回值:	成功			:	0
			失败			: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetPTZ(TGuiPtzLocalCode *pPtzLocalCode)
{
	alignedchar cReqBuf[sizeof(TGuiPtzLocalCode)] = {0};
	TGuiPtzLocalCode *pReq = (TGuiPtzLocalCode*)cReqBuf;

	//chn最小是1
	pReq->m_iChn      = pPtzLocalCode->m_iChn;
	pReq->m_iCode     = pPtzLocalCode->m_iCode;
	pReq->m_iParam[0] = pPtzLocalCode->m_iParam[0];
	pReq->m_iParam[1] = pPtzLocalCode->m_iParam[1];
	
	if(snd_rcv(g_iGuiParaTid, NULL, cReqBuf, 0, sizeof(TGuiPtzLocalCode), GUI_PTZ_LOCAL_CODE) != 0)
	{
		td_printf(0, "OnvifCmdSetPTZ error\n");
		return TD_ERROR;
	}

	return TD_OK;
}

/****************************************************
名称:		OnvifCmdGetSystemMessage
功能:		获取设备系统信息
参数:		_pstDevMsg		:	系统信息结构体
返回值:	成功			:	0
			失败			: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetSystemMessage(TGuiGetSysMsgRsp *_pstDevMsg)
{
	#if 0
	TGuiGetSysMsgRsp stRsp;
	
	memset(&stRsp, 0, sizeof(TGuiGetSysMsgRsp));
	
	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, NULL, sizeof(TGuiGetSysMsgRsp), 0, GUI_GET_SYS_MSG_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetPTZ error\n");
		return TD_ERROR;
	}
	
	if(pointer_valid_check(_pstDevMsg))
	{
		return TD_ERROR;
	}

	strcpy(_pstDevMsg->m_strDveName, stRsp.m_strDveName);
	strcpy(_pstDevMsg->m_strKernelVer, stRsp.m_strKernelVer);
	strcpy(_pstDevMsg->m_strManufacturer, stRsp.m_strManufacturer);
	strcpy(_pstDevMsg->m_strSN, stRsp.m_strSN);
	#endif
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdGetMDAlarmCfg
功能:		获取移动报警配置
参数:		_iChn		:	通道号(最小1)
返回值:	成功		:	移动报警使能标志
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetMDAlarmCfg(int _iChn)
{
	TGuiGetMdAlarmCnfReq stReq = {0};
	TGuiGetMdAlarmCnfRsp stRsp = {0};

	stReq.m_iChn = _iChn;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiGetMdAlarmCnfRsp), sizeof(TGuiGetMdAlarmCnfReq), GUI_GET_MD_ALARM_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetMDAlarmCfg error\n");
		return TD_ERROR;
	}

	return stRsp.m_tCnf.m_iEnable;
}

/****************************************************
名称:		OnvifCmdGetMDAlarmStatus
功能:		获取移动报警状态
参数:		_iChn		:	通道号
返回值:	成功		:	移动报警使能标志
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetMDAlarmStatus(int _iChn)
{
	if((g_ptGuiAlarmInOutStatus->m_iVideoMd & (1<<_iChn)))
	{
		return 1;
	}
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdGetVideoChnSizeList
功能:		获取视频分辨率列表
参数:		_iChn		:	通道号(最小1)
			_ptpVideoList:     分辨率列表
			_iMaxCnt      :     最大个数
返回值:	成功		:	支持的分辨率个数
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetVideoChnSizeList(int _iChn, int  *_ptpVideoList, int _iMaxCnt)
{
	#if 0
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	alignedchar stRspBuf[sizeof(TGuiGetChnVideoSizeRsp) + sizeof(TGuiVideoSizeItem) * MAX_VIDEOSIZE_LIST] = {0};
	TGuiGetChnVideoSizeReq stReq  = {0};
	TGuiGetChnVideoSizeRsp* ptRsp = (TGuiGetChnVideoSizeRsp *)stRspBuf;
	
	if(pointer_valid_check(ptOnvifCtrl) || pointer_valid_check(_ptpVideoList))
	{
		return TD_ERROR;
	}
	
	stReq.m_iChn = _iChn;
	
	if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		if(snd_rcv_variable(g_iGuiParaTid, (char *)&stRspBuf, (char *)&stReq, sizeof(TGuiGetChnVideoSizeRsp) + sizeof(TGuiVideoSizeItem) * MAX_VIDEOSIZE_LIST, sizeof(TGuiGetChnVideoSizeReq), GUI_GET_CHN_VIDEO_SIZE_REQ) != 0)
		{
			td_printf(0, "OnvifCmdGetVideoChnSizeList error\n");
			return TD_ERROR;
		}
		if(ptRsp->m_iCnt > _iMaxCnt)
		{
			return TD_ERROR;
		}
		memcpy(_ptpVideoList, stRspBuf + sizeof(TGuiGetChnVideoSizeRsp), ptRsp->m_iCnt * sizeof(TGuiVideoSizeItem));
	}
	else if (DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		//dvr共享内存分辨率列表为单个循环发送
		ptRsp->m_iCnt = snd_rcv_dvr(g_iGuiParaTid, _ptpVideoList, (char *)&stReq, sizeof(TGuiGetChnVideoSizeRsp), sizeof(TGuiGetChnVideoSizeReq), GUI_GET_CHN_VIDEO_SIZE_REQ);
		if(ptRsp->m_iCnt < 0)
		{
			td_printf(0, "OnvifCmdGetVideoChnSizeList error\n");
			return TD_ERROR;
		}
	}
	else
	{
		td_printf(0, "%s %d : Device type[%d] is error!\n",__FUNCTION__, __LINE__, ptOnvifCtrl->m_stNvsInfo.m_iDeviceType);
		return TD_ERROR;
	}

	return ptRsp->m_iCnt;
	#endif
	return 0;
}

/****************************************************
名称:		OnvifCmdGetIspInfo
功能:		获取高清参数配置
参数:		_ptpIspInfo	:     高清参数配置
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetIspInfo(TGuiVideoISPInfo  *_ptpIspInfo)
{
	TGuiGetVideoISPInfoRsp stRsp;
	int iRet = 0;

	memset(&stRsp, 0, sizeof(TGuiGetVideoISPInfoRsp));

	if((iRet = snd_rcv_variable(g_iGuiParaTid, (char *)&stRsp, NULL, sizeof(TGuiGetVideoISPInfoRsp), 0, GUI_GET_VIDEO_ISP_INFO_REQ)) != 0)
	{
		td_printf(0, "OnvifCmdGetIspInfo error\n");
		return iRet;
	}

	if(pointer_valid_check(_ptpIspInfo))
	{
		return TD_ERROR;
	}
	memcpy(_ptpIspInfo, &stRsp.m_tVideoISPInfo, sizeof(TGuiVideoISPInfo));

	return TD_OK;
}

/****************************************************
名称:		OnvifCmdSetIspInfo
功能:		设置高清参数配置
参数:		_ptpIspInfo	:     高清参数配置
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetIspInfo(TGuiVideoISPInfo *_ptIspInfo)
{
	TGuiSetVideoISPInfoReq stReq;
	TGuiSetVideoISPInfoRsp stRsp = {0};
	int iRet = 0;

	memset(&stReq, 0, sizeof(TGuiGetVideoISPInfoRsp));
	
	if(pointer_valid_check(_ptIspInfo))
	{
		return TD_ERROR;
	}
	memcpy(&stReq.m_tVideoISPInfo, _ptIspInfo, sizeof(TGuiVideoISPInfo));

	if((iRet = snd_rcv_variable(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiSetVideoISPInfoRsp), sizeof(TGuiSetVideoISPInfoReq), GUI_SET_VIDEO_ISP_INFO_REQ)) != 0)
	{
		td_printf(0, "OnvifCmdSetIspInfo error\n");
		return iRet;
	}
	
	return stRsp.m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetNTP
功能:		获取NTP信息
参数:		_pNTPIP		:	NTP数组
			_iBufLen		:     数组大小上限
返回值:	成功		:	0
			失败		: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetNTP(char *_pNTPIP, int _iBufLen)
{
	TGuiSetNtp stRsp;

	memset(&stRsp, 0, sizeof(TGuiSetNtp));

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, NULL, sizeof(TGuiSetNtp), 0, GUI_GET_NTP_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetNTP error\n");
		return TD_ERROR;
	}
	
	if(_iBufLen < sizeof(stRsp.m_strNtpIp))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(_pNTPIP))
	{
		return TD_ERROR;
	}
	memcpy(_pNTPIP, stRsp.m_strNtpIp, sizeof(stRsp.m_strNtpIp));

	return TD_OK;
}

/****************************************************
名称:		OnvifCmdSetNTP
功能:		设置NTP信息
参数:		_pNTPIP		:	NTP数组
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetNTP(char *_pNTPIP)
{
	TGuiSetNtp stReq;
	TGuiSetNtpRsp stRsp = {0};
	
	if(pointer_valid_check(_pNTPIP))
	{
		return TD_ERROR;
	}
	
	memset(&stReq, 0, sizeof(TGuiSetNtp));
	memcpy(stReq.m_strNtpIp, _pNTPIP, sizeof(stReq.m_strNtpIp));

	stReq.m_iport = 123;
	stReq.m_itime = 1;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiSetNtpRsp), sizeof(TGuiSetNtp), GUI_SET_NTP_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetNTP error\n");
		return TD_ERROR;
	}

	return stRsp.m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetProductModel
功能:		获取ProductModel
参数:		无
返回值:	成功		:	ProductModel
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetProductModel()
{
	#if 0
	TGuiGetHpmCodeRsp stReq;
	TGuiGetHpmCodeRsp stRsp;

	memset(&stReq, 0, sizeof(TGuiGetHpmCodeRsp));
	memset(&stRsp, 0, sizeof(TGuiGetHpmCodeRsp));

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiGetHpmCodeRsp), sizeof(TGuiGetHpmCodeRsp), GUI_GET_HPM_CODE_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetProductModel error\n");
		return TD_ERROR;
	}

	return stRsp.m_uiCode[0];
	#else
	return 123;
	#endif
}

/****************************************************
名称:		OnvifCmdGetHttpPort
功能:		获取设备网页端口
参数:		无
返回值:	成功		:	http端口号
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetHttpPort()
{
	TGuiGetNetCnfRsp stRsp;

	memset(&stRsp, 0, sizeof(TGuiGetNetCnfRsp));
	
	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, NULL, sizeof(TGuiGetNetCnfRsp), 0, GUI_GET_NET_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetHttpPort error\n");
		return -1;
	}
	
	return stRsp.m_tCnf.m_iHttpPort;
}

/****************************************************
名称:		OnvifCmdRestore
功能:		恢复出厂默认
参数:		无
返回值:	成功	:	0
			失败	: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdRestore()
{
	if(snd_rcv(g_iGuiParaTid, NULL, NULL, 0, 0, GUI_SET_RENEW_REQ) != 0)
	{
		td_printf(0, "OnvifCmdRestore error\n");
		return TD_ERROR;
	}

	return TD_OK;
}

/****************************************************
名称:		OnvifCmdSetOsd
功能:		设置字符叠加
参数:		_iChn	:	通道号(最小1)
			_stOsd	:	视频叠加请求
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetOsd(int _iChn, TGuiVideoOverlayCnf *_ptSetOsd)
{
	TGuiSetVideoOverlayCnfReq stReq = {0};
	TGuiSetVideoOverlayCnfRsp stRsp = {0};

	if(pointer_valid_check(_ptSetOsd))
	{
		return TD_ERROR;
	}

	stReq.m_iChn = _iChn;
	memcpy(&stReq.m_tCnf, _ptSetOsd, sizeof(TGuiVideoOverlayCnf));
	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiSetVideoOverlayCnfRsp), sizeof(TGuiSetVideoOverlayCnfReq), GUI_SET_VOVERLAY_CNF_REQ) != 0)
	{
		td_printf(0, "%s:ask for osd info errno!\n", __func__);
		return TD_ERROR;
	}
	
	return stRsp.m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetOsd
功能:		获取字符叠加
参数:		_iChn		:	通道号(最小1)
			_stOsdInfo	:	视频叠加响应
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetOsd(int _iChn, TGuiVideoOverlayCnf *_ptGetOsd)
{
	TGuiGetVideoOverlayCnfReq stReq = {0};
	TGuiGetVideoOverlayCnfRsp stRsp = {0};

	stReq.m_iChn = _iChn;
	
	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiGetVideoOverlayCnfRsp), sizeof(TGuiGetVideoOverlayCnfReq), GUI_GET_VOVERLAY_CNF_REQ) != 0)
	{
		td_printf(0, "%s:ask for osd info errno!\n", __func__);
		return TD_ERROR;
	}

	if(pointer_valid_check(_ptGetOsd))
	{
		return TD_ERROR;
	}
	memcpy(_ptGetOsd, &stRsp.m_tCnf, sizeof(TGuiVideoOverlayCnf));

	return TD_OK;
}

/****************************************************
名称:		OnvifCmdGetRelayOutputState
功能:		获取IO输出端口的状态
参数:		_iChn		:	通道号(最小1)
返回值:	成功		:	报警状态
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetRelayOutputState(int _iChn)
{
	TGuiManualGetAlarmOutReq stReq = {0};
	TGuiManualAlarmOutMsg stRsp = {0};

	stReq.m_iAlarmOutChn = _iChn;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiManualAlarmOutMsg), sizeof(TGuiManualGetAlarmOutReq), GUI_GET_MANUAL_ALARM_LST_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetRelayOutputState error\n");
		return TD_ERROR;
	}

	return stRsp.m_iStat;
}

/****************************************************
名称:		OnvifCmdSetRelayOutputState
功能:		设置IO输出端口的状态
参数:		_iChn		:	通道号(最小1)
			_iState		:	状态
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetRelayOutputState(int _iChn, int _iState)
{
	TGuiSetManualAlarmOutReq stReq = {0};
	TGuiSetManulAlarmOutRsp stRsp  = {0};

	stReq.m_iAlarmOutChn = _iChn;
	stReq.m_iStat = _iState;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiSetManulAlarmOutRsp), sizeof(TGuiSetManualAlarmOutReq), GUI_SET_MANUAL_ALARM_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetRelayOutputState error\n");
		return TD_ERROR;
	}

	return stRsp.m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetRelayOutputCnf
功能:		获取输出端口报警参数
参数:		_iPort		:	端口
返回值:	成功		:	报警类型
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetRelayOutputCnf(int _iPort)
{
	TGuiGetAlarmOutCnfReq stReq = {0};
	TGuiGetAlarmOutCnfRsp stRsp = {0};

	stReq.m_iPortId = _iPort;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiGetAlarmOutCnfRsp), sizeof(TGuiGetAlarmOutCnfReq), GUI_GET_ALARM_OUT_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetRelayOutputCnf error\n");
		return TD_ERROR;
	}

	return stRsp.m_tCnf.m_iType;
}

/****************************************************
名称:		OnvifCmdSetRelayOutputCnf
功能:		设置输出端口报警参数
参数:		_iPort		:	端口
			_iType		:	报警类型
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetRelayOutputCnf(int _iPort, int _iType)
{
	TGuiSetAlarmOutCnfReq stReq = {0};
	TGuiSetAlarmOutCnfRsp stRsp = {0};

	stReq.m_iPortId      = _iPort;
	stReq.m_tCnf.m_iType = _iType;
	
	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiSetAlarmOutCnfRsp), sizeof(TGuiSetAlarmOutCnfReq), GUI_SET_ALARM_OUT_CNF_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetRelayOutputCnf error\n");
		return TD_ERROR;
	}

	return stRsp.m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetSnapshot
功能:		抓拍
参数:		_iChn		:	通道号(最小1)
返回值:	成功		:	0
			失败		: 	0!
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetSnapshot(int _iChn)
{
	TGuiSnapshotReq	stSnapshotReq = {0};
 	TGuiSnapshotRsp	stSnapshotRsp = {0};

	stSnapshotReq.iChn = _iChn;

	if(snd_rcv(g_iGuiParaTid, (char *)&stSnapshotRsp, (char *)&stSnapshotReq, sizeof(TGuiSnapshotRsp), sizeof(TGuiSnapshotReq), GUI_SNAPSHOT_REQ))
	{		
		td_printf(0, "OnvifCmdGetSnapshot error, stSnapshotRsp.m_iRetCode == %d", stSnapshotRsp.m_iRetCode);
		return TD_ERROR;
	}
	
	return stSnapshotRsp.m_iRetCode;
}

/****************************************************
*功		能	: 	抓拍 
*参		数	: 	_iChn:通道号(最小1)          
					_pcNameBuffer:保存返回的图片名字
*返回	值		:	 
*修改历史	:	added by qhj 2012.11.01
****************************************************/
int OnvifCmdGetSnapshotEx(int _iChn, char *_pcNameBuffer)
{	
	TGuiSnapshotReqEx stSnapshotReqEx = {0}; 	
	TGuiSnapshotRspEx stSnapshotRspEx = {0};

	stSnapshotReqEx.iChn = _iChn;
	
	if(snd_rcv(g_iGuiParaTid, (char *)&stSnapshotRspEx, (char *)&stSnapshotReqEx, sizeof(TGuiSnapshotRspEx), sizeof(TGuiSnapshotReqEx), GUI_SNAPSHOT_REQ_EX))
	{				
		td_printf(0, "OnvifCmdGetSnapshotEX error");
		return -1;
	}
	
	if(pointer_valid_check(_pcNameBuffer))
	{
		return TD_ERROR;
	}
	memcpy(_pcNameBuffer, stSnapshotRspEx.m_cPicName, 64);

	return stSnapshotRspEx.m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetChnType
功能:		获取设备通道类型
参数:		_iChn	:	通道号(最小1)
返回值:	成功	:	通道类型
			失败	: 	-1
修改历史:	20130305 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetChnType(int _iChn)
{
	alignedchar creqbuf[sizeof(TGuiGetChnTypeReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiGetChnTypeRsp)] = {0};
	TGuiGetChnTypeReq *ptReq = (TGuiGetChnTypeReq *) creqbuf;
	TGuiGetChnTypeRsp *ptRsp = (TGuiGetChnTypeRsp *) crspbuf;

	ptReq->m_iChn = _iChn;

	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiGetChnTypeRsp), sizeof(TGuiGetChnTypeReq), GUI_GET_CHN_TYPE_REQ) != 0)
	{
		td_printf(0, "OnvifCmdGetChnType error\n");
		return TD_ERROR;
	}
	
	return ptRsp->m_iType;
}

/****************************************************
 *功		能	: 	获取音频采样率
 *参		数	: 	_iChn:通道号(最小1)         
 *返回	值		:	
 *修改历史	:	by qiaohaijun @2013.3.29
****************************************************/
int OnvifCmdGetAudioRateCnf(int _iChn)
{
	alignedchar creqbuf[sizeof(TGuiGetAudioRateCnfReq)] = {0};
	alignedchar crspbuf[sizeof(TGuiGetAudioRateCnfRsp)] = {0};

	TGuiGetAudioRateCnfReq *ptReq = (TGuiGetAudioRateCnfReq *)creqbuf;
	TGuiGetAudioRateCnfRsp *ptRsp = (TGuiGetAudioRateCnfRsp *)crspbuf;

	ptReq->m_iChn = _iChn;

	if(snd_rcv(g_iGuiParaTid, crspbuf, creqbuf, sizeof(TGuiGetAudioRateCnfRsp), sizeof(TGuiGetAudioRateCnfReq), GUI_GET_AUDIO_RATE_CNF_REQ))
	{
		td_printf(0, "OnvifCmdGetAudioRateCnf error\n");
		return TD_ERROR;
	}

	td_printf(0, "#######tRspBuf.m_iAudioRate ==== %d", ptRsp->m_iAudioRate);

	return ptRsp->m_iAudioRate;
}

/****************************************************
 *功		能	: 	设置音频采样率
 *参		数	: 	_iChn:通道号(最小1)         
 *返回	值		:	
 *修改历史	:	by qiaohaijun @2013.5.7
****************************************************/
int OnvifCmdSetAudioRateCnf(int _iChn, int _iAudioRate)
{
	TGuiSetAudioRateCnfReq stReq = {0};
	TGuiSetAudioRateCnfRsp stRsp = {0};

	stReq.m_iAudioRate = _iAudioRate;
	stReq.m_iChn = _iChn;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiSetAudioRateCnfRsp), sizeof(TGuiSetAudioRateCnfReq), GUI_SET_AUDIO_RATE_CNF_REQ))
	{
		td_printf(0, "OnvifCmdSetAudioRateCnf error\n");
		return TD_ERROR;
	}

	td_printf(0, "stRsp.m_iRetCode === %d", stRsp.m_iRetCode);
	return stRsp.m_iRetCode;
}

/****************************************************
 *功		能	: 	获取录像模板参数
 *参		数	: 	_iChn               :通道号(从1开始)    
 					_pStVRTemplate:获取到的模板参数
 *返回	值	:	成功:    0
 					失败:    !0
 *修改历史	:	20130522 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetVRTemplate(int _iChn, TGuiGetVideoTemplateRsp *_pStVRTemplate)
{
	TGuiGetVideoTemplateReq stReq = {0};
	TGuiGetVideoTemplateRsp stRsp = {0};
	int iReqLen = 0;
	int iRspLen = 0;
	int iCmd = 0;
	
	iCmd    = GUI_GET_VR_TEMPLATE_REQ;
	iReqLen = sizeof(TGuiGetVideoTemplateReq);
	iRspLen = sizeof(TGuiGetVideoTemplateRsp);

	stReq.m_iChn = _iChn;
	
	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, iRspLen, iReqLen, iCmd))
	{
		td_printf(0, "%s error\n", __FUNCTION__);
		return TD_ERROR;
	}

	_pStVRTemplate->m_iChn = stRsp.m_iChn;
	_pStVRTemplate->m_tTmp = stRsp.m_tTmp;

	return TD_OK;
}

/****************************************************
 *功		能	: 	设置录像模板参数
 *参		数	: 	_pStVRTemplate:需要设置的模板
 					参数
 *返回	值	:	成功:    0
 					失败:    !0
 *修改历史	:	20130522 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetVRTemplate(TGuiSetVideoTemplateReq *_pStVRTemplate)
{
	TGuiSetVideoTemplateReq stReq = {0};
	TGuiSetVideoTemplateRsp stRsp = {0};
	int iReqLen = 0;
	int iRspLen = 0;
	int iCmd = 0;

	if(pointer_valid_check(_pStVRTemplate))
	{
		return TD_ERROR;
	}
	
	iCmd    = GUI_SET_VR_TEMPLATE_REQ;
	iReqLen = sizeof(TGuiSetVideoTemplateReq);
	iRspLen = sizeof(TGuiSetVideoTemplateRsp);
	
	stReq.m_iChn = _pStVRTemplate->m_iChn;
	stReq.m_tTmp = _pStVRTemplate->m_tTmp;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, iRspLen, iReqLen, iCmd))
	{
		td_printf(0, "%s error\n", __FUNCTION__);
		return TD_ERROR;
	}

	return stRsp.m_iRetCode;
}

/****************************************************
 *功		能	: 	获取录像策略
 *参		数	: 	_pGetVStrategy:获取到的录像策
 					略参数
 *返回	值	:	成功:    0
 					失败:    !0
 *修改历史	:	20130522 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetVRStrategy(TGuiGetVideoStrategyRsp *_pGetVStrategy)
{
	TGuiGetVideoStrategyRsp stRsp;
	int iReqLen = 0;
	int iRspLen = 0;
	int iCmd = 0;

	if(pointer_valid_check(_pGetVStrategy))
	{
		return TD_ERROR;
	}
	iCmd    = GUI_GET_VR_STRATEGY_REQ;
	iReqLen = 0;
	iRspLen = sizeof(TGuiGetVideoStrategyRsp);

	memset(&stRsp, 0, iRspLen);

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, NULL, iRspLen, iReqLen, iCmd))
	{
		td_printf(0, "%s error\n", __FUNCTION__);
		return TD_ERROR;
	}
	
	_pGetVStrategy->m_tStrategy = stRsp.m_tStrategy;
		
	return TD_OK;
}

/****************************************************
 *功		能	: 	设置录像策略
 *参		数	: 	_pSetVStrategy:需要设置的录像策
 					略参数
 *返回	值	:	成功:    0
 					失败:    !0
 *修改历史	:	20130522 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetVRStrategy(TGuiGetVideoStrategyRsp *_pSetVStrategy)
{
	TGuiSetVideoStrategyReq stReq;
	TGuiSetVideoStrategyRsp stRsp = {0};
	int iReqLen = 0;
	int iRspLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_pSetVStrategy))
	{
		return TD_ERROR;
	}

	iCmd    = GUI_SET_VR_STRATEGY_REQ;
	iReqLen = sizeof(TGuiSetVideoStrategyReq);
	iRspLen = sizeof(TGuiSetVideoStrategyRsp);
	
	memset(&stReq, 0, iReqLen);
	stReq.m_tStrategy = _pSetVStrategy->m_tStrategy;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, iRspLen, iReqLen, iCmd))
	{
		td_printf(0, "%s error\n", __FUNCTION__);
		return TD_ERROR;
	}
 		
	return stRsp.m_iRetCode;
}

/****************************************************
 *功		能	: 	统计录像文件个数
 *参		数	: 	_pCondition:查询条件
 *返回	值	:	成功:    录像文件个数
 					失败:    0
 *修改历史	:	20130522 v2.3.0 created by ptb
****************************************************/
int OnvifCmdGetVFileCount(TGuiCountVideoFileReq *_pCondition)
{
	TGuiCountVideoFileReq stReq;
	TGuiCountVideoFileRsp stRsp = {0};
	int iReqLen = 0;
	int iRspLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_pCondition))
	{
		return TD_ERROR;
	}

	iCmd    = GUI_COUNT_VIDEO_FILE_REQ;
	iReqLen = sizeof(TGuiCountVideoFileReq);
	iRspLen = sizeof(TGuiCountVideoFileRsp);

	memset(&stReq, 0, iReqLen);
	stReq.m_tCondition = _pCondition->m_tCondition;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, iRspLen, iReqLen, iCmd))
	{
		td_printf(0, "%s error\n", __FUNCTION__);
		return TD_ERROR;
	}
 		
	return stRsp.m_iVideoFileCnt;
}

/****************************************************
 *功		能	: 	查询录像文件
 *参		数	: 	_pCondition:查询条件
 *返回	值	:	成功:    查询录像文件个数
 					失败:    0
 *修改历史	:	20130522 v2.3.0 created by ptb
****************************************************/
int OnvifCmdQueryVFile(TGuiQueryVideoFileReq *_pCondition)
{
	TGuiQueryVideoFileReq stReq;
	TGuiQueryVideoFileRsp stRsp = {0};
	int iReqLen = 0;
	int iRspLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_pCondition))
	{
		return TD_ERROR;
	}

	iCmd    = GUI_QUERY_VIDEO_FILE_REQ;
	iReqLen = sizeof(TGuiQueryVideoFileReq);
	iRspLen = sizeof(TGuiQueryVideoFileRsp);

	memset(&stReq, 0, iReqLen);
	stReq.m_iPageId      = _pCondition->m_iPageId;
	stReq.m_iRcntPerPage = _pCondition->m_iRcntPerPage;
	stReq.m_tCondition   = _pCondition->m_tCondition;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, iRspLen, iReqLen, iCmd))
	{
		td_printf(0, "%s error\n", __FUNCTION__);
		return TD_ERROR;
	}
 		
	return stRsp.m_iCnt;
}

/****************************************************************************
 *功		能	: 	开始播放录像文件请求
 *参		数	: 	IN: _ptVideoPlayReq:请求条件, 
 					OUT:_ptVideoPlayRsp:响应
 *返	回	值	:	成功:    0
 					失败:    -1
 *修 改	历  史	:	20130523 v2.3.0 created by changguoxing
****************************************************************************/
int OnvifCmdPlayVideoStart(TGuiVideoPlayReq *_ptVideoPlayReq, TGuiVideoPlayRsp *_ptVideoPlayRsp)
{
	int iReqLen = 0;
	int iRspLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_ptVideoPlayReq) || pointer_valid_check(_ptVideoPlayRsp))
	{
		td_printf(0, "%s %d, input param error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}

	iCmd     = GUI_PLAY_VIDEO_START_REQ;
	iReqLen = sizeof(TGuiVideoPlayReq);
	iRspLen = sizeof(TGuiVideoPlayRsp);

	if(snd_rcv(g_iGuiParaTid, (char *)_ptVideoPlayRsp, (char *)_ptVideoPlayReq, iRspLen, iReqLen, iCmd))
	{
		td_printf(0, "%s %d, error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
 		
	return TD_OK;
}

/****************************************************************************
 *功		能	: 	暂停播放录像文件请求
 *参		数	: 	IN: TGuiVideoPlayPauseReq:回放录像的编号，从1开始。
 *返	回	值	:	成功:    0
 					失败:    -1
 *修 改	历  史	:	20130523 v2.3.0 created by changguoxing
****************************************************************************/
int OnvifCmdPlayVideoPause(TGuiVideoPlayPauseReq *_ptVideoPauseReq)
{
	int iReqLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_ptVideoPauseReq))
	{
		td_printf(0, "%s %d, input param error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}

	iCmd     = GUI_PLAY_VIDEO_PAUSE_REQ;
	iReqLen = sizeof(TGuiVideoPlayPauseReq);

	if(snd_rcv(g_iGuiParaTid, NULL, (char *)_ptVideoPauseReq, 0, iReqLen, iCmd))
	{
		td_printf(0, "%s %d, error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
 		
	return TD_OK;
}

/****************************************************************************
 *功		能	: 	继续播放录像文件请求
 *参		数	: 	IN: TGuiVideoPlayCntnReq:回放录像的编号，从1开始。
 *返	回	值	:	成功:    0
 					失败:    -1
 *修 改	历  史	:	20130523 v2.3.0 created by changguoxing
****************************************************************************/
int OnvifCmdPlayVideoContinue(TGuiVideoPlayCntnReq *_ptVideoContReq)
{
	int iReqLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_ptVideoContReq))
	{
		td_printf(0, "%s %d, input param error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}

	iCmd     = GUI_PLAY_VIDEO_CONT_REQ;
	iReqLen = sizeof(TGuiVideoPlayCntnReq);

	if(snd_rcv(g_iGuiParaTid, NULL, (char *)_ptVideoContReq, 0, iReqLen, iCmd))
	{
		td_printf(0, "%s %d, error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
 		
	return TD_OK;
}

/****************************************************************************
 *功		能	: 	停止播放录像文件请求
 *参		数	: 	IN: TGuiVideoPlayPauseReq:回放录像的编号，从1开始。
 *返	回	值	:	成功:    0
 					失败:    -1
 *修 改	历  史	:	20130523 v2.3.0 created by changguoxing
****************************************************************************/
int OnvifCmdPlayVideoStop(TGuiVideoPlayStopReq*_ptVideoStopReq)
{
	int iReqLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_ptVideoStopReq))
	{
		td_printf(0, "%s %d, input param error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}

	iCmd     = GUI_PLAY_VIDEO_STOP_REQ;
	iReqLen = sizeof(TGuiVideoPlayStopReq);

	if(snd_rcv(g_iGuiParaTid, NULL, (char *)_ptVideoStopReq, 0, iReqLen, iCmd))
	{
		td_printf(0, "%s %d, error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
 		
	return TD_OK;
}

/****************************************************************************
 *功		能	: 	N倍速度播放录像请求
 *参		数	: 	IN: TGuiVideoPlayChgSpeedCmd:
 					m_iPlayId,回放录像的编号，从1开始。
 					m_iType,正n表示当前倍速xn倍速, -n表示/n倍速
 *返	回	值	:	成功:    0
 					失败:    -1
 *修 改	历  史	:	20130523 v2.3.0 created by changguoxing
****************************************************************************/
int OnvifCmdPlayVideoSpeedX(TGuiVideoPlayChgSpeedCmd*_ptVideoSpeedXReq)
{
	int iReqLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_ptVideoSpeedXReq))
	{
		td_printf(0, "%s %d, input param error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}

	iCmd     = GUI_PLAY_VIDEO_SPEEDX_REQ;
	iReqLen = sizeof(TGuiVideoPlayChgSpeedCmd);

	if(snd_rcv(g_iGuiParaTid, NULL, (char *)_ptVideoSpeedXReq, 0, iReqLen, iCmd))
	{
		td_printf(0, "%s %d, error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
 		
	return TD_OK;
}

/****************************************************************************
 *功		能	: 	快放，慢放播放录像文件请求
 *参		数	: 	IN: TGuiVideoPlaySpeedCmd:
 					m_iPlayId,回放录像的编号，从1开始。
 					m_iType,正n表示n倍速播放,-n表示1/n倍速播放(1, 0, -1均表示正常速播放)
 *返	回	值	:	成功:    0
 					失败:    -1
 *修 改	历  史	:	20130523 v2.3.0 created by changguoxing
****************************************************************************/
int OnvifCmdPlayVideoSpeed(TGuiVideoPlaySpeedCmd*_ptVideoSpeedReq)
{
	int iReqLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_ptVideoSpeedReq))
	{
		td_printf(0, "%s %d, input param error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}

	iCmd     = GUI_PLAY_VIDEO_SPEED_CMD;
	iReqLen = sizeof(TGuiVideoPlaySpeedCmd);

	if(snd_rcv(g_iGuiParaTid, NULL, (char *)_ptVideoSpeedReq, 0, iReqLen, iCmd))
	{
		td_printf(0, "%s %d, error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
 		
	return TD_OK;
}

/****************************************************************************
 *功		能	: 	单步播放录像文件请求
 *参		数	: 	IN: TGuiVideoPlayStepCmd:
 					m_iPlayId,回放录像的编号，从1开始。
 					m_iType,步进类型(1:步进, -1步退)
 *返	回	值	:	成功:    0
 					失败:    -1
 *修 改	历  史	:	20130523 v2.3.0 created by changguoxing
****************************************************************************/
int OnvifCmdPlayVideoStep(TGuiVideoPlayStepCmd*_ptVideoStepReq)
{
	int iReqLen = 0;
	int iCmd = 0;
	
	if(pointer_valid_check(_ptVideoStepReq))
	{
		td_printf(0, "%s %d, input param error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}

	iCmd     = GUI_PLAY_VIDEO_BY_STEP_REQ;
	iReqLen = sizeof(TGuiVideoPlayStepCmd);

	if(snd_rcv(g_iGuiParaTid, NULL, (char *)_ptVideoStepReq, 0, iReqLen, iCmd))
	{
		td_printf(0, "%s %d, error\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
 		
	return TD_OK;
}

/****************************************************
名称:		OnvifCmdSetFlip
功能:		设置视频翻转
参数:		_iChn		:	通道号
			_iType		:	翻转类型
			返回值:	成功		:	0
			失败		: 	-1
修改历史:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetFlip(int _iChn, int _iType)
{
	TSetVideoTurnTypeReq stReq;
	TSetVideoTurnTypeRsp stRsp;
	int iReqLen = 0;
	int iRspLen = 0;

	iReqLen = sizeof(TSetVideoTurnTypeReq);
	iRspLen = sizeof(TSetVideoTurnTypeRsp);

	memset(&stReq, 0, iReqLen);
	memset(&stRsp, 0, iRspLen);

	stReq.m_iChn  = _iChn;
	stReq.m_iType = _iType;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, iRspLen, iReqLen, GUI_SET_VIDEO_TURN_TYPE_REQ) != 0)
	{
		td_printf(0, "OnvifCmdSetFlip error\n");
		return TD_ERROR;
	}

	return stRsp.m_iRetCode;
}

/****************************************************
名称:		OnvifCmdGetPtzCompassCnf
功能:		获取PTZ绝对坐标
参数:		_iChn:通道号

			_ptGetPTZCompassRsp:返回信息

修改历史:	by qiaohaijun @2013.12.17
****************************************************/
int OnvifCmdGetPtzCompassCnf(int _iChn, TGuiGetPTZCompassRsp *_ptGetPTZCompassRsp)
{
	td_printf(0, "\n\n\n%s %d", __FUNCTION__, __LINE__);
	TGuiGetPTZCompassReq stReq = {0};
	TGuiGetPTZCompassRsp stRsp = {0};

	stReq.m_iChn  = _iChn;

	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)&stReq, sizeof(TGuiGetPTZCompassRsp), sizeof(TGuiGetPTZCompassReq), GUI_GET_PTZ_COMPASS_CNF_REQ))
	{
		td_printf(0, "OnvifCmdGetPtzCompassCnf error\n");
		return TD_ERROR;
	}

	td_printf(0, "stRsp.m_iXp ==== %d", stRsp.m_iXp);
	td_printf(0, "stRsp.m_iYp ==== %d", stRsp.m_iYp);
	td_printf(0, "stRsp.m_iZoom ===== %d", stRsp.m_iZoom);
	td_printf(0, "stRsp.m_iChn ==== %d", stRsp.m_iChn);
	

	memcpy(_ptGetPTZCompassRsp, &stRsp, sizeof(TGuiGetPTZCompassRsp));

	return TD_OK;
}


/****************************************************
名称:		OnvifCmdSetPtzCompassCnf
功能:		设置PTZ绝对坐标
参数:		_ptSetPTZCompassReq:请求信息


修改历史:	by qiaohaijun @2013.12.17
****************************************************/
int OnvifCmdSetPtzCompassCnf(TGuiSetPTZCompassReq *_ptSetPTZCompassReq)
{
	td_printf(0, "\n\n\n%s %d", __FUNCTION__, __LINE__);
	TGuiSetPTZCompassRsp stRsp = {0};

	td_printf(0, "sizeof(TGuiSetPTZCompassReq) ==== %d", sizeof(TGuiSetPTZCompassReq));
	
	if(snd_rcv(g_iGuiParaTid, (char *)&stRsp, (char *)_ptSetPTZCompassReq, sizeof(TGuiSetPTZCompassRsp), sizeof(TGuiSetPTZCompassReq), GUI_SET_PTZ_COMPASS_CNF_REQ))
	{
		td_printf(0, "OnvifCmdSetPtzCompassCnf error\n");
		return TD_ERROR;
	}

	td_printf(0, "stRsp.m_iRetCode ==== %d", stRsp.m_iRetCode);
	
	return stRsp.m_iRetCode;
}




