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
���ƣ�   OnvifCmdInit
���ܣ�   ��ʼ������ͨ��ID
������  _iTid:����ͨ��ID
����ֵ:	    0:�ɹ� 			
�޸���ʷ:20120302 v1.0 created by czl
****************************************************/
int OnvifCmdInit(int _iTid)
{
	g_iGuiParaTid = _iTid;
	return TD_OK;
}

/****************************************************
����:		OnvifCmdRebootReq
����:		�����ڴ淢����������
����:		��
����ֵ:	�ɹ�	:	0
			ʧ��	: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSendVerNum
����:		�����ڴ淢��onvif�汾��Ϣ
����:		_pcVersion:	�汾��Ϣ�ַ���
����ֵ:	�ɹ�	:	0
			ʧ��	: 	!0
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetDevNum
����:		�����豸���ʹӹ����ڴ�
			��ȡ�豸��Ŀ
����:		_itype	:	�豸����
����ֵ:	�ɹ�	:	�豸��Ŀ
			ʧ��	: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetTime
����:		����ϵͳʱ��
����:		ptTime	:	ʱ��ṹ��
����ֵ:	�ɹ�	:	0
			ʧ��	: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetip
����:		��ȡ������ز���
����:		_pstIpMsg	:	��������ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetip
����:		����������ز���
����:		_pstIpMsg	:	��������ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	!0
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetPtzDecodeCnf
����:		��ȡPTZ����Э����ز���
����:		_iChn		:	ͨ����(��С1)
			_pPtzDCnf	:	PTZ�����ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetPtzDecodeCnf
����:		����PTZ����Э����ز���
����:		_iChn		:	ͨ����(��С1)
			_pstPtzDCnf	:	PTZ�����ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdIframeReq
����:		����I֡
����:		_iChn		:	ͨ����(��С1)
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdStreamConnect
����:		������Ƶ
����:		_iChn		:	ͨ����(��С1)
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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

	//ÿ�ζ�Ҫ����һ�Σ����ں���Ҫ��������ǰ��û��Ƶ����base==top,����Ƶ���fifo���ƶ�top.
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
����:		OnvifCmdStreamDisConnect
����:		�Ͽ���Ƶ
����:		_iChn		:	ͨ����(��С1)
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetAVEncodeCnf
����:		������Ƶ�������
����:		_iChn			:	ͨ����(��С1)
			_pstVEncodeCnf	:	��Ƶ��������ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetVideoEncodeCnf
����:		��ȡ��Ƶ�������
����:		_iChn		:	ͨ����(��С1)
			pVEncodeCnf	:	��Ƶ��������ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdRebootPlatform
����:		�������
����:		��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdHeartbeat
����:		�������
����:		��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetVideoInCnf
����:		��ȡ��Ƶ�������
����:		iChn			:	ͨ����(��С1)
			PVInCnf		:	��Ƶ�����ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetVideoInCnf
����:		������Ƶ�������
����:		_iChn		:	ͨ����(��1��ʼ)
			_pVInCnf	:	��Ƶ��������ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetDVRVideoInCnf
����:		����DVR(��ʱֵ)��Ƶ�������
����:		iChn			:	ͨ����(��1��ʼ)
			pVInCnf		:	��Ƶ��������ṹ��
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetPTZ
����:		����PTZ������
����:		pPtzLocalCode	:	PTZ���Ʋ���
����ֵ:	�ɹ�			:	0
			ʧ��			: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
****************************************************/
int OnvifCmdSetPTZ(TGuiPtzLocalCode *pPtzLocalCode)
{
	alignedchar cReqBuf[sizeof(TGuiPtzLocalCode)] = {0};
	TGuiPtzLocalCode *pReq = (TGuiPtzLocalCode*)cReqBuf;

	//chn��С��1
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
����:		OnvifCmdGetSystemMessage
����:		��ȡ�豸ϵͳ��Ϣ
����:		_pstDevMsg		:	ϵͳ��Ϣ�ṹ��
����ֵ:	�ɹ�			:	0
			ʧ��			: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetMDAlarmCfg
����:		��ȡ�ƶ���������
����:		_iChn		:	ͨ����(��С1)
����ֵ:	�ɹ�		:	�ƶ�����ʹ�ܱ�־
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetMDAlarmStatus
����:		��ȡ�ƶ�����״̬
����:		_iChn		:	ͨ����
����ֵ:	�ɹ�		:	�ƶ�����ʹ�ܱ�־
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetVideoChnSizeList
����:		��ȡ��Ƶ�ֱ����б�
����:		_iChn		:	ͨ����(��С1)
			_ptpVideoList:     �ֱ����б�
			_iMaxCnt      :     ������
����ֵ:	�ɹ�		:	֧�ֵķֱ��ʸ���
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
		//dvr�����ڴ�ֱ����б�Ϊ����ѭ������
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
����:		OnvifCmdGetIspInfo
����:		��ȡ�����������
����:		_ptpIspInfo	:     �����������
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetIspInfo
����:		���ø����������
����:		_ptpIspInfo	:     �����������
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetNTP
����:		��ȡNTP��Ϣ
����:		_pNTPIP		:	NTP����
			_iBufLen		:     �����С����
����ֵ:	�ɹ�		:	0
			ʧ��		: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetNTP
����:		����NTP��Ϣ
����:		_pNTPIP		:	NTP����
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetProductModel
����:		��ȡProductModel
����:		��
����ֵ:	�ɹ�		:	ProductModel
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetHttpPort
����:		��ȡ�豸��ҳ�˿�
����:		��
����ֵ:	�ɹ�		:	http�˿ں�
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdRestore
����:		�ָ�����Ĭ��
����:		��
����ֵ:	�ɹ�	:	0
			ʧ��	: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetOsd
����:		�����ַ�����
����:		_iChn	:	ͨ����(��С1)
			_stOsd	:	��Ƶ��������
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetOsd
����:		��ȡ�ַ�����
����:		_iChn		:	ͨ����(��С1)
			_stOsdInfo	:	��Ƶ������Ӧ
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetRelayOutputState
����:		��ȡIO����˿ڵ�״̬
����:		_iChn		:	ͨ����(��С1)
����ֵ:	�ɹ�		:	����״̬
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetRelayOutputState
����:		����IO����˿ڵ�״̬
����:		_iChn		:	ͨ����(��С1)
			_iState		:	״̬
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetRelayOutputCnf
����:		��ȡ����˿ڱ�������
����:		_iPort		:	�˿�
����ֵ:	�ɹ�		:	��������
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdSetRelayOutputCnf
����:		��������˿ڱ�������
����:		_iPort		:	�˿�
			_iType		:	��������
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetSnapshot
����:		ץ��
����:		_iChn		:	ͨ����(��С1)
����ֵ:	�ɹ�		:	0
			ʧ��		: 	0!
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
*��		��	: 	ץ�� 
*��		��	: 	_iChn:ͨ����(��С1)          
					_pcNameBuffer:���淵�ص�ͼƬ����
*����	ֵ		:	 
*�޸���ʷ	:	added by qhj 2012.11.01
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
����:		OnvifCmdGetChnType
����:		��ȡ�豸ͨ������
����:		_iChn	:	ͨ����(��С1)
����ֵ:	�ɹ�	:	ͨ������
			ʧ��	: 	-1
�޸���ʷ:	20130305 v2.3.0 created by ptb
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
 *��		��	: 	��ȡ��Ƶ������
 *��		��	: 	_iChn:ͨ����(��С1)         
 *����	ֵ		:	
 *�޸���ʷ	:	by qiaohaijun @2013.3.29
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
 *��		��	: 	������Ƶ������
 *��		��	: 	_iChn:ͨ����(��С1)         
 *����	ֵ		:	
 *�޸���ʷ	:	by qiaohaijun @2013.5.7
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
 *��		��	: 	��ȡ¼��ģ�����
 *��		��	: 	_iChn               :ͨ����(��1��ʼ)    
 					_pStVRTemplate:��ȡ����ģ�����
 *����	ֵ	:	�ɹ�:    0
 					ʧ��:    !0
 *�޸���ʷ	:	20130522 v2.3.0 created by ptb
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
 *��		��	: 	����¼��ģ�����
 *��		��	: 	_pStVRTemplate:��Ҫ���õ�ģ��
 					����
 *����	ֵ	:	�ɹ�:    0
 					ʧ��:    !0
 *�޸���ʷ	:	20130522 v2.3.0 created by ptb
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
 *��		��	: 	��ȡ¼�����
 *��		��	: 	_pGetVStrategy:��ȡ����¼���
 					�Բ���
 *����	ֵ	:	�ɹ�:    0
 					ʧ��:    !0
 *�޸���ʷ	:	20130522 v2.3.0 created by ptb
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
 *��		��	: 	����¼�����
 *��		��	: 	_pSetVStrategy:��Ҫ���õ�¼���
 					�Բ���
 *����	ֵ	:	�ɹ�:    0
 					ʧ��:    !0
 *�޸���ʷ	:	20130522 v2.3.0 created by ptb
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
 *��		��	: 	ͳ��¼���ļ�����
 *��		��	: 	_pCondition:��ѯ����
 *����	ֵ	:	�ɹ�:    ¼���ļ�����
 					ʧ��:    0
 *�޸���ʷ	:	20130522 v2.3.0 created by ptb
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
 *��		��	: 	��ѯ¼���ļ�
 *��		��	: 	_pCondition:��ѯ����
 *����	ֵ	:	�ɹ�:    ��ѯ¼���ļ�����
 					ʧ��:    0
 *�޸���ʷ	:	20130522 v2.3.0 created by ptb
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
 *��		��	: 	��ʼ����¼���ļ�����
 *��		��	: 	IN: _ptVideoPlayReq:��������, 
 					OUT:_ptVideoPlayRsp:��Ӧ
 *��	��	ֵ	:	�ɹ�:    0
 					ʧ��:    -1
 *�� ��	��  ʷ	:	20130523 v2.3.0 created by changguoxing
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
 *��		��	: 	��ͣ����¼���ļ�����
 *��		��	: 	IN: TGuiVideoPlayPauseReq:�ط�¼��ı�ţ���1��ʼ��
 *��	��	ֵ	:	�ɹ�:    0
 					ʧ��:    -1
 *�� ��	��  ʷ	:	20130523 v2.3.0 created by changguoxing
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
 *��		��	: 	��������¼���ļ�����
 *��		��	: 	IN: TGuiVideoPlayCntnReq:�ط�¼��ı�ţ���1��ʼ��
 *��	��	ֵ	:	�ɹ�:    0
 					ʧ��:    -1
 *�� ��	��  ʷ	:	20130523 v2.3.0 created by changguoxing
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
 *��		��	: 	ֹͣ����¼���ļ�����
 *��		��	: 	IN: TGuiVideoPlayPauseReq:�ط�¼��ı�ţ���1��ʼ��
 *��	��	ֵ	:	�ɹ�:    0
 					ʧ��:    -1
 *�� ��	��  ʷ	:	20130523 v2.3.0 created by changguoxing
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
 *��		��	: 	N���ٶȲ���¼������
 *��		��	: 	IN: TGuiVideoPlayChgSpeedCmd:
 					m_iPlayId,�ط�¼��ı�ţ���1��ʼ��
 					m_iType,��n��ʾ��ǰ����xn����, -n��ʾ/n����
 *��	��	ֵ	:	�ɹ�:    0
 					ʧ��:    -1
 *�� ��	��  ʷ	:	20130523 v2.3.0 created by changguoxing
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
 *��		��	: 	��ţ����Ų���¼���ļ�����
 *��		��	: 	IN: TGuiVideoPlaySpeedCmd:
 					m_iPlayId,�ط�¼��ı�ţ���1��ʼ��
 					m_iType,��n��ʾn���ٲ���,-n��ʾ1/n���ٲ���(1, 0, -1����ʾ�����ٲ���)
 *��	��	ֵ	:	�ɹ�:    0
 					ʧ��:    -1
 *�� ��	��  ʷ	:	20130523 v2.3.0 created by changguoxing
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
 *��		��	: 	��������¼���ļ�����
 *��		��	: 	IN: TGuiVideoPlayStepCmd:
 					m_iPlayId,�ط�¼��ı�ţ���1��ʼ��
 					m_iType,��������(1:����, -1����)
 *��	��	ֵ	:	�ɹ�:    0
 					ʧ��:    -1
 *�� ��	��  ʷ	:	20130523 v2.3.0 created by changguoxing
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
����:		OnvifCmdSetFlip
����:		������Ƶ��ת
����:		_iChn		:	ͨ����
			_iType		:	��ת����
			����ֵ:	�ɹ�		:	0
			ʧ��		: 	-1
�޸���ʷ:	20130227 v2.3.0 created by ptb
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
����:		OnvifCmdGetPtzCompassCnf
����:		��ȡPTZ��������
����:		_iChn:ͨ����

			_ptGetPTZCompassRsp:������Ϣ

�޸���ʷ:	by qiaohaijun @2013.12.17
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
����:		OnvifCmdSetPtzCompassCnf
����:		����PTZ��������
����:		_ptSetPTZCompassReq:������Ϣ


�޸���ʷ:	by qiaohaijun @2013.12.17
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




