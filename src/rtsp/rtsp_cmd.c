/*
 *	file: rtsp_cmd.h
 *	description: rtsp cmd process
 *	history: dongliqiang created at 20130228
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "macro.h" 
#include "rtsp_cmd.h"
#include "rtsp_data.h"
#include "media_h264.h"
#include "epoll.h"
#include "rtsp_heart.h"
#include "onvif_shm_cmd.h"
#include "media_base64.h"
#include "common.h"
#ifdef WITH_MD5
#include "md5.h"
#endif

#define PUT_16(p,v) ((p)[0]=((v)>>8)&0xff,(p)[1]=(v)&0xff)
#define PUT_32(p,v) ((p)[0]=((v)>>24)&0xff,(p)[1]=((v)>>16)&0xff,(p)[2]=((v)>>8)&0xff,(p)[3]=(v)&0xff)


//add by changguoxing
//eg. CSeq: 2
static int RtspGetValueByName(const char * _pMsg, char * _pName, char * _pValue)
{
	const char *pMsg = _pMsg;
	char *pName = _pName;
	char *pValue = _pValue;
	char *pPosCRLF = NULL;
	char *pPosCono = NULL;
	char *pPos = NULL;
	
	if(pMsg == NULL ||pName == NULL ||pValue == NULL)
	{
		td_printf(0, "%s, input param error.", __FUNCTION__);
		return -1;
	}

	pPos = strstr((char*)pMsg, pName);
	if(pPos == NULL)
	{
		td_printf(0, "%s, %d, failed.", __FUNCTION__, __LINE__);
		return -1;
	}

	pPosCono = strstr(pPos, ":");
	if(pPosCono == NULL)
	{
		td_printf(0, "%s, %d, failed.", __FUNCTION__, __LINE__);
		return -1;
	}
	pPosCono++;
	while(*pPosCono == ' ')
	{
		pPosCono++;
	}
	pPosCRLF = strstr(pPosCono, "\n");
	if(pPosCRLF == NULL)
	{
		td_printf(0, "%s, %d, failed.", __FUNCTION__, __LINE__);
		return -1;
	}

	memcpy(pValue, pPosCono, pPosCRLF-pPosCono);

	return 0;
}

//在psrc 的查找单词pneedle, pneedle后面必须是空格才返回正确，否则返回NULL.
static char* Mystrstr(const char* _pSrc,const char* _pNeedle)
{
	const char *pSrc = _pSrc;
	const char *pNeedle = _pNeedle;
	char c = '\0';
	char* pResult = NULL;
	
	if(NULL == pSrc || NULL == pNeedle)
	{
		return NULL;
	}

	pResult = strstr((char*)pSrc,pNeedle);
	if(pResult == NULL)
	{
		return NULL;
	}
	c = *(pResult+strlen(pNeedle));
	if(c != ' ')
	{
		return NULL;
	}
	return pResult;
}

//去掉base64编码中的\r\n，返回转换完之后的长度。
static int ChangeBase64Format(const char *_pcSrc, char *_pcDst)
{
	int i = 0;
	int j = 0;
	
	if(NULL == _pcSrc || NULL == _pcDst)
	{
		td_printf(0, "%s %d %s, input param error!!!\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	
	for(i=0; i<strlen(_pcSrc); i++)
	{
		if(*(_pcSrc+i) != '\r' && *(_pcSrc+i) != '\n')
		{
			_pcDst[j] = _pcSrc[i];
			j++;
		}
	}

	return strlen(_pcDst);
}


//judge http cmd
static int HttpCmdIsGet(const char *_pMsg)
{
	return Mystrstr(_pMsg, "GET") != NULL;
}

static int HttpCmdIsPost(const char *_pMsg)
{
	return Mystrstr(_pMsg, "POST") != NULL;
}
//end

//judge what cmd we recv
int RtspReqIsSetup(const char *_pMsg)
{
	return Mystrstr(_pMsg, "SETUP") != NULL;
}
int RtspReqIsPlay(const char *_pMsg)
{
	return Mystrstr(_pMsg, "PLAY") != NULL;
}
int RtspReqIsPause(const char *_pMsg)
{
	return Mystrstr(_pMsg, "PAUSE") != NULL;
}
int RtspReqIsOption(const char *_pMsg)
{
	return Mystrstr(_pMsg, "OPTIONS") != NULL;
}
int RtspReqIsDescribe(const char *_pMsg)
{
	return Mystrstr(_pMsg, "DESCRIBE") != NULL;
}
int RtspReqIsTearDown(const char *_pMsg)
{
	return Mystrstr(_pMsg, "TEARDOWN") != NULL;
}
int RtspReqIsSetParameter(const char *_pMsg)
{
	return Mystrstr(_pMsg, "SET_PARAMETER") != NULL;
}
//end

static int RtspParseCmdOption(const char *_pData, TRtspInfo* _ptRtspInfo)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	int iRet = -1;
	const char *pData = _pData;
	TRtspInfo *ptRtspInfo = _ptRtspInfo;
	char cValue[32] = {0};

	if(NULL == pData || NULL == ptRtspInfo)
	{
		return -1;
	}

	iRet = RtspGetValueByName(pData, "CSeq", cValue);
	if(iRet == 0)
	{
		ptRtspInfo->m_iSeq = atoi(cValue);
		td_printf(0, "RtspParseCmdOption, seq = %d", ptRtspInfo->m_iSeq);
	}

	ptRtspInfo->m_iRspCode = RTSP_RSP_200;
	ptRtspInfo->m_iAuthFlag = AUTH_YES;
	return iRet;
}

/*
dvr 满帧时传过来的帧率是2009，需要在函数内做转换。
*/
int RtspGetAVInfo(TChannel *_ptChannel)
{
	TChannel *ptChannel = _ptChannel;
	TGuiVideoEncodeCnf tTmpVEncodeCnf = {0};
	TRtspServer *ptServer = NULL;
	int iNorm = -1;
	VideoInCnf_EX tVInCnf = {0};

	if(NULL == ptChannel)
	{
		return -1;
	}
	ptServer = (TRtspServer *)ptChannel->m_pServer;
	//get norm
	OnvifCmdGetVideoInCnf(ptChannel->m_iChan + 1, &tVInCnf);
	//iNorm = ptServer->m_tNvsInfo.m_tVideoParam.m_InputNorm[ptChannel->m_iChan];
	iNorm = tVInCnf.m_iVideoMod;
	//td_printf(0, "%s, iNorm == %d", __FUNCTION__, iNorm);
	
	OnvifCmdGetVideoEncodeCnf(ptChannel->m_iChan + 1, &tTmpVEncodeCnf);
	ptChannel->m_tChanParam.m_tVEncCfg.m_iEncodeType = tTmpVEncodeCnf.m_iEncodeType;
	ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType = tTmpVEncodeCnf.m_iAudioEncodeType;
	//td_printf(0, "%s, get from kernel, framerate == %d", __FUNCTION__, tTmpVEncodeCnf.m_iFrameRate);
	// deal with FrameRate
	if(GUI_FULL_FRAME_RATE == tTmpVEncodeCnf.m_iFrameRate)
	{
		if(GUI_PAL_MODE == iNorm)
		{
			ptChannel->m_tChanParam.m_tVEncCfg.m_iFrameRate = PAL_MAXFRAMERATE;
		}
		else if(GUI_NTSC_MODE == iNorm)
		{
			ptChannel->m_tChanParam.m_tVEncCfg.m_iFrameRate = NTSC_MAXFRAMERATE;
		}
		else
		{
			ptChannel->m_tChanParam.m_tVEncCfg.m_iFrameRate = PAL_MAXFRAMERATE;
		}
	}
	else
	{
		ptChannel->m_tChanParam.m_tVEncCfg.m_iFrameRate = tTmpVEncodeCnf.m_iFrameRate;
	}

	//td_printf(0, "_ptRtspInfo->m_iRtspVideoType  == %d", ptChannel->m_tChanParam.m_tVEncCfg.m_iEncodeType);
	//td_printf(0, "_ptRtspInfo->m_iRtspAudioType === %d", ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType);
	//td_printf(0, "_ptRtspInfo->m_iVideoFrameRate == %d", ptChannel->m_tChanParam.m_tVEncCfg.m_iFrameRate);
	
	//放在channel.c里获取音频采样率by qiaohaijun @2013.08.22
	//ptChannel->m_tChanParam.m_iAudioSampleRate = OnvifCmdGetAudioRateCnf(ptChannel->m_iChan + 1);
#if 0
	//added by qiaohaijun @2013.4.17
	if(ptServer->m_tNvsInfo.m_iDeviceType == DEVICE_DVR)//DVR
	{
		ptChannel->m_tChanParam.m_iAudioSampleRate = OnvifCmdGetAudioRateCnf(ptChannel->m_iChan + 1);
	}
	else if(ptServer->m_tNvsInfo.m_iDeviceType == DEVICE_IPC)//IPC
	{
		ptChannel->m_tChanParam.m_iAudioSampleRate= 8000;//暂用8000,3518暂时获取不到音频采样率
	}
#else
	ptChannel->m_tChanParam.m_iAudioSampleRate= 8000;//暂用8000
#endif
	//end
	
	return 0;
}

/**************************************************
*功能:		通过Url获取通道号
*参数:		_pRtspUrl	:	rtsp url(rtsp://10.30.21.138:554/3/ 
								or http://10.30.21.138:554/3/)
*返回值:	成功		:	通道号(从1开始不关心端口号)
				失败		:	-1
*修改历史:20130827 v2.3 modify by ptb
**************************************************/
static int RtspGetChnoByUrl(char*_pRtspUrl)
{
	char *pRtspUrl = _pRtspUrl;
	char *pcPort = NULL;
	char *pcChno = NULL;
	int iChno = 1;			//默认值需为1.
	int iLen  = 5;			//strlen("rtsp:") == strlen("http:")==5 
	int iLen2 = 7;			//strlen("rtsp://") == strlen("http://")==7 
	
	if(NULL == pRtspUrl)
	{
		return -1;
	}
	
	pcPort = strchr(pRtspUrl + iLen, ':'); //跳过rtsp:的冒号。
	if(pcPort)
	{
		pcChno = strchr(pcPort , '/');
		td_printf(0, "pcChno = %s", pcChno);//  /x...x 或 /x...x/
		if(pcChno)
		{
			iChno = atoi(pcChno+1);
		}
	}
	else
	{
		//如果不带端口信息，再看有没有带通道号信息。rtsp://10.30.21.138/3/
		pcChno = strchr(pRtspUrl + iLen2, '/');
		if(pcChno)
		{
			iChno = atoi(pcChno+1);
		}
	}
	
	//容错处理
	if(iChno < 1)
	{
		iChno = 1;
	}

	return iChno;
}
#ifdef WITH_MD5
//by qiaohaijun for Authorization test...2013.12.13
//从收到的DESCRIBE信息中获取相应的auth信息
int parseAuthorizationHeader(const char *requestbuf,
					char *username,
					char *realm,
					char *nonce,
					char *uri,
					char *response)
{

	char *authbuf = NULL;
	char *fields = NULL;
	
	// Initialize the result parameters to default values:
	username[0] = realm[0] = nonce[0] = uri[0] = response[0] = '\0';

	authbuf = strstr(requestbuf, "Authorization: Digest ");
	if(!authbuf)
	{
		td_printf(0, "no Authorization....");
		return TD_ERROR;
	}

  	// Then, run through each of the fields, looking for ones we handle:
  	fields = authbuf + 22;

	while (*fields == ' ') fields++;

	char *parameter = (char *)malloc(strlen(fields) + 1);
	char *value = (char *)malloc(strlen(fields) + 1);
	if(!parameter || !value)
	{
		return TD_ERROR;
	}
	parameter[0] = '\0';

	while (1)
	{
		value[0] = '\0';
		if(sscanf(fields, "%[^=]=\"%[^\"]\"", parameter, value) != 2 &&
			sscanf(fields, "%[^=]=\"\"", parameter) != 1)
		{
			break;
		}

		td_printf(0, "parameter ===== %s, value ===== %s", parameter, value);
		
		if (strcmp(parameter, "username") == 0)
		{
			strcpy(username, value);
		}
		else if (strcmp(parameter, "realm") == 0)
		{
			strcpy(realm, value);
		}
		else if (strcmp(parameter, "nonce") == 0)
		{
			strcpy(nonce, value);
		}
		else if (strcmp(parameter, "uri") == 0)
		{
			strcpy(uri, value);
			
		}
		else if (strcmp(parameter, "response") == 0)
		{
			strcpy(response, value);
		}

		fields += strlen(parameter) + 2 /*="*/ + strlen(value) + 1 /*"*/;
		while (*fields == ',' || *fields == ' ') ++fields;

		// skip over any separating ',' and ' ' chars
		if (*fields == '\0' || *fields == '\r' || *fields == '\n') break;
	}

	if(username[0] == '\0'
	||realm[0] == '\0'
	||nonce[0] == '\0'
	||uri[0] == '\0'
	||response[0] == '\0')
	{
		return TD_ERROR;
	}
	

	free(parameter);
	free(value);
	parameter = NULL;
	value = NULL;
	
	return TD_OK;
}
//md5(md5(<username>:<realm>:<password>):<nonce>:md5(<cmd>:<url>))
int genResponse(char *_usname, 
					char *_realm, 
					char *_paswd, 
					char *_nonce, 
					char *_cmd, 
					char *_url,
					char *_response)
{
	char ha1buf[1024] = {0};
	char ha2buf[1024] = {0};
	unsigned char dataNeedCode[1024] = {0};
	
	sprintf((char *)dataNeedCode, "%s:%s:%s", _usname, _realm, _paswd);
	our_MD5Data(dataNeedCode, strlen((char *)dataNeedCode), ha1buf);
	dataNeedCode[0] = '\0';

	
	sprintf((char *)dataNeedCode, "%s:%s", _cmd, _url);
	our_MD5Data(dataNeedCode, strlen((char *)dataNeedCode), ha2buf);
	dataNeedCode[0] = '\0';

	sprintf((char *)dataNeedCode, "%s:%s:%s", ha1buf, _nonce, ha2buf);
	our_MD5Data(dataNeedCode, strlen((char *)dataNeedCode), _response);

	return 0;
}

int RtspAuthorization(const char *requesbuf, TRtspSession *ptSession, char *_pcCMD)
{
	
	TRtspInfo *ptRtspInfo = &ptSession->m_tRtspInfo;
	TRtspServer *ptRtspServer = (TRtspServer *)ptSession->m_pServer;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)ptRtspServer->m_pUserData;
	int iRet = TD_OK;

	ptRtspInfo->m_pCmd = _pcCMD;
	
	char username[64] = {0};
	char realm[64] = {0};
	char nonce[64] = {0};
	char uri[512] = {0};
	char response[64] = {0};
	int i = 0;
	
	//获取校验头
	if(parseAuthorizationHeader(requesbuf, username, realm, nonce, uri, response) == TD_ERROR)
	{
		td_printf(0, "not authoriztion.............................");
		iRet =TD_ERROR;
		goto EXIT;
	}
	//校验

	if(strcmp(ptRtspServer->m_tRtspAuth.m_cNonce, nonce)
	||strcmp(ptRtspServer->m_tRtspAuth.m_cRealm, realm))
	{
		td_printf(0, "return nonce error or realm error.................");
		td_printf(0, "\nrecv nonce: %s\nlocal nonce: %s", nonce, ptRtspServer->m_tRtspAuth.m_cNonce);
		iRet =TD_ERROR;
		goto EXIT;
	}

	td_printf(0, "ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser ===== %d",  ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser);

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser; i++)
	{
		if(strcmp(username, ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Username))
		{
			continue;
		}

		char ha3buf[1024] = {0};

		genResponse(ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Username,
					ptRtspServer->m_tRtspAuth.m_cRealm,
					ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Password,
					ptRtspServer->m_tRtspAuth.m_cNonce,
					ptRtspInfo->m_pCmd,
					uri,
					ha3buf);

		td_printf(0, "response ==== %s", response);
		td_printf(0, "ha3buf ==== %s", ha3buf);

		if(strcmp(ha3buf, response))
		{
			continue;
		}
		else
		{
			iRet =TD_OK;
			goto EXIT;			
		}
	}

	if(i == ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser)
	{
		iRet =TD_ERROR;
		goto EXIT;
	}

EXIT:
	(iRet == TD_ERROR) ? (ptRtspInfo->m_iRspCode = RTSP_RSP_401) : (ptRtspInfo->m_iRspCode = RTSP_RSP_200);
	(iRet == TD_ERROR) ? (ptRtspInfo->m_iAuthFlag = AUTH_NO) : (ptRtspInfo->m_iAuthFlag = AUTH_YES);
	
	return iRet;
}

//end
#endif
/*
DESCRIBE rtsp://10.30.21.138/1 RTSP/1.0
CSeq: 2019086143
Accept: application/sdp
*/
static int RtspParseCmdDescribe(const char *_pData, TRtspSession* _ptSession)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	int iRet = -1;
	TRtspSession *ptSession = _ptSession;
	const char *pData     = _pData;
	TChannel *ptChannel   = NULL;
	char cValue[32] = {0};
	char *pTmpStr   = NULL;
	char *pStr      = NULL;
	int iAudioEncodeType = -1;
	int iVideoEncodeType = -1;
	int iShmChno = 1;				//shm chno's default must be 1. 
	int iViNum = 0;
	int iLen = 0;

	if(NULL == pData || NULL == ptSession)
	{
		td_printf(0, "%s, input param error.", __FUNCTION__);
		return -1;
	}
	
	TRtspInfo *ptRtspInfo = &ptSession->m_tRtspInfo;
	TRtspServer *ptServer = (TRtspServer *)ptSession->m_pServer;
	
	//get CSeq
	iRet = RtspGetValueByName(pData, "CSeq", cValue);
	if(0 != iRet)
	{
		td_printf(0, "%s, cmd DESCRIBE has no CSeq, error.", __FUNCTION__);
		return -1;
	}

	ptRtspInfo->m_iSeq = atoi(cValue);
	td_printf(0, "RtspParseCmdDescribe, seq = %d", ptRtspInfo->m_iSeq);
	
	//get rtsp url
	pStr = strstr((char*)pData, "rtsp://");
	if(NULL == pStr)
	{
		//130827 add for iNEX by ptb 
		pStr = strstr((char*)pData, "http://");
		if(NULL == pStr)
		{
			return -1;
		}
		//end
	}
	
	pTmpStr = pStr;
	while(*pTmpStr != ' ')
	{
		iLen++;
		pTmpStr++;
	}
	strncpy(ptRtspInfo->m_pcRtspUrl, pStr, iLen);
	td_printf(0, "url = %s", ptRtspInfo->m_pcRtspUrl);
	
	//get chno 
	iShmChno = RtspGetChnoByUrl(ptRtspInfo->m_pcRtspUrl);
	td_printf(0, "iShmChno = %d", iShmChno);
		
	//when we get iShmChno, we know chno and stream type.
	iViNum = ptServer->m_tNvsInfo.m_iVINum;
	td_printf(0, "vi num = %d", iViNum);
	
	if(iShmChno <= 0 || iShmChno > (MAX_STREAM * iViNum))
	{
		td_printf(0, "%s %d %s, iShmChno invalid!! iShmChno = %d.\n", __FILE__, __LINE__, __FUNCTION__, iShmChno);
		return -1;
	}
#ifdef WITH_MD5
	if(RtspAuthorization(pData, ptSession, "DESCRIBE"))
	{	
		return TD_OK;
	}
#endif
	ptSession->m_iChan = iShmChno > iViNum ?  iShmChno - iViNum - 1 : iShmChno - 1;
	ptSession->m_iStream = iShmChno > iViNum  ? 1 : 0;
	
	td_printf(0, "%s, chno = %d, stream_type = %d", __FUNCTION__, ptSession->m_iChan, ptSession->m_iStream);

	ptChannel = &ptServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan];
	//get rtsp_info, like video type, audio type, framerate...!!!not deal with substream here
	RtspGetAVInfo(ptChannel);

	iAudioEncodeType = ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType;
	iVideoEncodeType = ptChannel->m_tChanParam.m_tVEncCfg.m_iEncodeType;
	td_printf(0, "iAudioEncodeType === %d(1:G711A, 2:G711U, 3:ADPCM), iVideoEncodeType === %d(1:h264, 2:mpg4, 3:mjpeg)", iAudioEncodeType, iVideoEncodeType);


	//初始化RTP头应该放在ChannelConnect前面，
	//ChannelConnect函数会置ptChannel->m_iStat = CHANNEL_CONNECTED状态
	//by  qiaohaijun
	RtpInitHeaderWithAecType(&(ptSession->m_tRtpInfo.m_tARtpHeader), iAudioEncodeType);
	RtpInitHeaderWithVecType(&(ptSession->m_tRtpInfo.m_tVRtpHeader), iVideoEncodeType, &(ptSession->m_tRtpInfo.m_tJpegHeader));

	//after getting chno, we request for stream.
	iRet = ChannelConnect(ptChannel);
	if(iRet < 0)
	{
		td_printf(0, "%s %d %s, ChannelConnect failed!\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
		
#if 0
	RtpInitHeaderWithAecType(&(ptSession->m_tRtpInfo.m_tARtpHeader), iAudioEncodeType);
	RtpInitHeaderWithVecType(&(ptSession->m_tRtpInfo.m_tVRtpHeader), iVideoEncodeType, &(ptSession->m_tRtpInfo.m_tJpegHeader));
#endif
/*
	td_printf(0, "\naudio == %d %d %d %d", \
								ptSession->m_tRtpInfo.m_tARtpHeader.seqno,\
								ptSession->m_tRtpInfo.m_tARtpHeader.pt,\
								ptSession->m_tRtpInfo.m_tARtpHeader.ssrc,\
								ptSession->m_tRtpInfo.m_tARtpHeader.ts);
	td_printf(0, "\nvideo == %d %d %d %d", \
								ptSession->m_tRtpInfo.m_tVRtpHeader.seqno,\
								ptSession->m_tRtpInfo.m_tVRtpHeader.pt,\
								ptSession->m_tRtpInfo.m_tVRtpHeader.ssrc,\
								ptSession->m_tRtpInfo.m_tVRtpHeader.ts);
	
*/
	return iRet;
}

static int RtspParseCmdSetup(const char *_pData, TRtspSession* _ptSession)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = _ptSession;
	const char *pData     = _pData;
	TRtspInfo *ptRtspInfo = NULL;
	char *pstrInterleaved = NULL;
	char *pstrClientPort  = NULL;
	char *pstrTrack       = NULL;
	char cTransport[128]  = {0};
	char cValue[32]       = {0};
	int iRet = -1;
	
	if(NULL == pData || NULL == ptSession)
	{
		return -1;
	}
	
	ptRtspInfo = &ptSession->m_tRtspInfo;
#ifdef WITH_MD5
	//OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SET_PARAMETER
	if(RtspAuthorization(pData, ptSession, "SETUP"))
	{	
		return TD_OK;
	}
#endif
	//get CSeq
	iRet = RtspGetValueByName(pData, "CSeq", cValue);
	td_printf(0, "RtspParseCmdSetup, RtspGetValueByName, iRet = %d", iRet);
	if(iRet == 0)
	{
		ptRtspInfo->m_iSeq = atoi(cValue);
		td_printf(0, "RtspParseCmdSetup, seq = %d", ptRtspInfo->m_iSeq);
	}

	//get trackId 
	pstrTrack = strstr((char*)pData, "trackID=");
	if(NULL == pstrTrack)
	{
		return -1;
	}

	sscanf(pstrTrack, "trackID=%d", &ptRtspInfo->m_iTrackId);
	td_printf(0, "ptRtspInfo->m_iTrackId = %d", ptRtspInfo->m_iTrackId);

	//get Transport
	iRet = RtspGetValueByName(pData, "Transport", cTransport);
	td_printf(0, "RtspParseCmdSetup, cTransport = %s", cTransport);

	//get NetMode
	if(NULL != strstr(cTransport, "RTP/AVP/TCP"))
	{
		ptSession->m_iNetMode = NET_TCP;
		//only tcp , get interleaved_no.
		if(NULL != strstr(cTransport, "interleaved="))
		{
			pstrInterleaved = strstr(cTransport, "interleaved=");
			
			if(ptRtspInfo->m_iTrackId == AUDIO_TRACK_ID)
			{
				sscanf(pstrInterleaved, "interleaved=%d-", &ptRtspInfo->m_iInterleavedANo);
				ptSession->m_iSetupAudio = 1;
			}
			else if(ptRtspInfo->m_iTrackId == VIDEO_TRACK_ID)
			{
				sscanf(pstrInterleaved, "interleaved=%d-", &ptRtspInfo->m_iInterleavedVNo);
				ptSession->m_iSetupVideo = 1;
			}
			td_printf(0, "m_iInterleavedANo = %d, m_iInterleavedVNo = %d", ptRtspInfo->m_iInterleavedANo, ptRtspInfo->m_iInterleavedVNo);
		}
	}
	else
	{
		ptSession->m_iNetMode = NET_UDP;
		//only when udp, do this. init udp connections after getting trackid and netmode and before getting client ports.
		if(ptRtspInfo->m_iTrackId == AUDIO_TRACK_ID)//audio
		{
			//get free port
			ptSession->m_tARtpConnect.m_iLPort = GetPort();
			if(ptSession->m_tARtpConnect.m_iLPort < RTP_SERVER_PORT_START)
			{
				td_printf(0, "%s %d %s, cannot get udp AUDIO port!!!\n", __FILE__, __LINE__, __FUNCTION__);
				return -1;
			}
			ptSession->m_tARtcpConnect.m_iLPort = ptSession->m_tARtpConnect.m_iLPort + 1;
			td_printf(0, "%s %d %s m_tARtpConnect.m_iLPort %d\n", __FILE__, __LINE__, __FUNCTION__, ptSession->m_tARtpConnect.m_iLPort);
			InitConnection(&ptSession->m_tARtpConnect, CONNECT_TYPE_CLIENT, ptSession->m_iNetMode, ptSession->m_tARtpConnect.m_iLPort, NULL);
			InitConnection(&ptSession->m_tARtcpConnect, CONNECT_TYPE_CLIENT, ptSession->m_iNetMode, ptSession->m_tARtcpConnect.m_iLPort, NULL);
			ptSession->m_iSetupAudio = 1;
		}
		else
		{
			//get free port
			ptSession->m_tVRtpConnect.m_iLPort = GetPort();
			if(ptSession->m_tVRtpConnect.m_iLPort < RTP_SERVER_PORT_START)
			{
				td_printf(0, "%s %d %s, cannot get udp VIDEO port!!!\n", __FILE__, __LINE__, __FUNCTION__);
				return -1;
			}
			ptSession->m_tVRtcpConnect.m_iLPort = ptSession->m_tVRtpConnect.m_iLPort + 1;
			td_printf(0, "%s %d %s m_tVRtpConnect.m_iLPort %d\n", __FILE__, __LINE__, __FUNCTION__, ptSession->m_tVRtpConnect.m_iLPort);
			InitConnection(&ptSession->m_tVRtpConnect, CONNECT_TYPE_CLIENT, ptSession->m_iNetMode, ptSession->m_tVRtpConnect.m_iLPort, NULL);
			InitConnection(&ptSession->m_tVRtcpConnect, CONNECT_TYPE_CLIENT, ptSession->m_iNetMode, ptSession->m_tVRtcpConnect.m_iLPort, NULL);
			ptSession->m_iSetupVideo = 1;
		}
		if(NULL != strstr(cTransport, "client_port="))
		{
			pstrClientPort = strstr(cTransport, "client_port=");
			if(ptRtspInfo->m_iTrackId == AUDIO_TRACK_ID)
			{
				sscanf(pstrClientPort, "client_port=%d-%d", &ptSession->m_tARtpConnect.m_iRPort, &ptSession->m_tARtcpConnect.m_iRPort);
			}
			else if(ptRtspInfo->m_iTrackId == VIDEO_TRACK_ID)
			{
				sscanf(pstrClientPort, "client_port=%d-%d", &ptSession->m_tVRtpConnect.m_iRPort, &ptSession->m_tVRtcpConnect.m_iRPort);
			}
			td_printf(0, "in parse setup, artp_port = %d, artcp_port = %d, vrtp_port = %d, vrtcp_port = %d",
				ptSession->m_tARtpConnect.m_iRPort, ptSession->m_tARtcpConnect.m_iRPort, ptSession->m_tVRtpConnect.m_iRPort, ptSession->m_tVRtcpConnect.m_iRPort);
		}
	}

	if(NET_UDP == ptSession->m_iNetMode )
	{
		//create udp socket after getting client ports.
		if(ptRtspInfo->m_iTrackId == AUDIO_TRACK_ID)//audio
		{
			//audio rtp
			strncpy(ptSession->m_tARtpConnect.m_pcRAddr, ptSession->m_ptConnect->m_pcRAddr, LENGTH_IPV4);
			strncpy(ptSession->m_tARtpConnect.m_pcLAddr, ptSession->m_ptConnect->m_pcLAddr, LENGTH_IPV4);
			//td_printf(0, "in parse, audio rtp local addr = %s", ptSession->m_tARtpConnect.m_pcLAddr);
			ptSession->m_tARtpConnect.m_Connect(&ptSession->m_tARtpConnect, ptSession->m_tARtpConnect.m_pcRAddr, ptSession->m_tARtpConnect.m_iRPort);

			//audio rtcp  UdpConnect
			strncpy(ptSession->m_tARtcpConnect.m_pcRAddr, ptSession->m_ptConnect->m_pcRAddr, LENGTH_IPV4);
			strncpy(ptSession->m_tARtcpConnect.m_pcLAddr, ptSession->m_ptConnect->m_pcLAddr, LENGTH_IPV4);
			ptSession->m_tARtcpConnect.m_Connect(&ptSession->m_tARtcpConnect, ptSession->m_tARtcpConnect.m_pcRAddr, ptSession->m_tARtcpConnect.m_iRPort);
		}
		else	//video
		{
			//video rtp
			strncpy(ptSession->m_tVRtpConnect.m_pcRAddr, ptSession->m_ptConnect->m_pcRAddr, LENGTH_IPV4);
			strncpy(ptSession->m_tVRtpConnect.m_pcLAddr, ptSession->m_ptConnect->m_pcLAddr, LENGTH_IPV4);
			ptSession->m_tVRtpConnect.m_Connect(&ptSession->m_tVRtpConnect, ptSession->m_tVRtpConnect.m_pcRAddr, ptSession->m_tVRtpConnect.m_iRPort);

			//video rtcp
			strcpy(ptSession->m_tVRtcpConnect.m_pcRAddr, ptSession->m_ptConnect->m_pcRAddr);
			strcpy(ptSession->m_tVRtcpConnect.m_pcLAddr, ptSession->m_ptConnect->m_pcLAddr);
			ptSession->m_tVRtcpConnect.m_Connect(&ptSession->m_tVRtcpConnect, ptSession->m_tVRtcpConnect.m_pcRAddr, ptSession->m_tVRtcpConnect.m_iRPort);
		}
	}

	return iRet;
}

static int RtspParseCmdPlay(const char *_pData, TRtspSession *_ptSession)
{
	int iRet = -1;
	char cValue[32] = {0};
	const char *pData = _pData;

	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = &ptSession->m_tRtspInfo;;

	if(NULL == pData || NULL == ptRtspInfo)
	{
		return -1;
	}
#ifdef WITH_MD5
	//OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SET_PARAMETER
	if(RtspAuthorization(pData, ptSession, "PLAY"))
	{	
		return TD_OK;
	}
#endif
	iRet = RtspGetValueByName(pData, "CSeq", cValue);
	td_printf(0, "RtspParseCmdPlay, RtspGetValueByName, iRet = %d", iRet);
	if(iRet == 0)
	{
		ptRtspInfo->m_iSeq = atoi(cValue);
		td_printf(0, "RtspParseCmdPlay, seq = %d", ptRtspInfo->m_iSeq);
	}

	return 0;
}

static int RtspParseCmdPause(const char *_pData, TRtspSession *_ptSession)
{
	int iRet = -1;
	char cValue[32] = {0};
	const char *pData = _pData;
	
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = &ptSession->m_tRtspInfo;

	if(NULL == pData || NULL == ptRtspInfo)
	{
		return -1;
	}

#ifdef WITH_MD5
	//OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SET_PARAMETER
	if(RtspAuthorization(pData, ptSession, "PAUSE"))
	{	
		return TD_OK;
	}
	
#endif
	iRet = RtspGetValueByName(pData, "CSeq", cValue);
	td_printf(0, "RtspParseCmdPause, RtspGetValueByName, iRet = %d", iRet);
	if(iRet == 0)
	{
		ptRtspInfo->m_iSeq = atoi(cValue);
		td_printf(0, "RtspParseCmdPause, seq = %d", ptRtspInfo->m_iSeq);
	}

	return 0;
}

static int RtspParseCmdTeardown(const char *_pData, TRtspSession *_ptSession)
{
	int iRet = -1;
	char cValue[32] = {0};
	const char *pData = _pData;
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = &ptSession->m_tRtspInfo;

	if(NULL == pData || NULL == ptRtspInfo)
	{
		return -1;
	}

#ifdef WITH_MD5
	//OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SET_PARAMETER
	if(RtspAuthorization(pData, ptSession, "TEARDOWN"))
	{	
		return TD_OK;
	}
#endif
	iRet = RtspGetValueByName(pData, "CSeq", cValue);
	td_printf(0, "RtspParseCmdTeardown, RtspGetValueByName, iRet = %d", iRet);
	if(iRet == 0)
	{
		ptRtspInfo->m_iSeq = atoi(cValue);
		td_printf(0, "RtspParseCmdTeardown, seq = %d", ptRtspInfo->m_iSeq);
	}

	return 0;
}

static int RtspParseCmdSetParameter(const char *_pData, TRtspSession *_ptSession)
{
	int iRet = -1;
	char cValue[32] = {0};
	const char *pData = _pData;
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = &ptSession->m_tRtspInfo;

	if(NULL == pData || NULL == ptRtspInfo)
	{
		return TD_ERROR;
	}
#ifdef WITH_MD5
	//OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SET_PARAMETER
	if(RtspAuthorization(pData, ptSession, "SET_PARAMETER"))
	{	
		return TD_OK;
	}
#endif
	
	iRet = RtspGetValueByName(pData, "CSeq", cValue);
	if(iRet == 0)
	{
		ptRtspInfo->m_iSeq = atoi(cValue);
	}

	return TD_OK;
}

//end

static int RtspMarkHeartTime(TRtspSession *_ptSession)
{
	TRtspSession *ptSession = _ptSession;
	if(NULL == ptSession)
	{
		return -1;
	}

	ptSession->m_uiHeartTime = get_uptime(NULL);
	//time((time_t*)&ptSession->m_uiHeartTime);

	return 0;
}

static int RtspSetRspCode(char *_pBuf, int _iRspCode)
{
	char *pBuf = _pBuf;
	char *pcRspText = NULL;
	ERtspCmd iRspCode = (ERtspCmd)_iRspCode;

	if(iRspCode == RTSP_RSP_200)
	{
		pcRspText = "OK";
	}
	else if(iRspCode == RTSP_RSP_400)
	{
		pcRspText = "Bad Request";
	}
	else if(iRspCode == RTSP_RSP_5XX)
	{
		pcRspText = "Param Error";
	}
	else if(iRspCode == RTSP_RSP_401)
	{
		pcRspText = "Unauthorized";
	}
	else
	{
		pcRspText = "Server Internal Error";
	}
	
	sprintf(pBuf, "%s %d %s" CRLF, "RTSP/1.0", iRspCode, pcRspText);
	return 0;
}

static int RtspSetCSeq(char *_pBuf, int _iCSeq)
{	
	sprintf(_pBuf, "%s%s %d" CRLF, _pBuf, "CSeq:", _iCSeq); 
	return 0;
}

static int RtspSetDate(char *_pBuf)
{
	char *pBuf = _pBuf;
	char date[64] = {0};
	time_t now;
	struct tm tTime;

	now = time(NULL);
	localtime_r(&now, &tTime);
	strftime(date, 64, "%a, %d %b %Y %H:%M:%S GMT", &tTime);
	
	sprintf(pBuf, "%s%s %s" CRLF, pBuf, "Date:", date); 
	return 0;
}

static int RtspSetPublic(char *_pBuf)
{
	sprintf(_pBuf, "%s%s OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, SET_PARAMETER" CRLF, _pBuf, "Public:");	 
	return 0;
}

static int RtspSetServer(char *_pBuf)
{
	sprintf(_pBuf, "%sServer: AglaiaSense RTSP Server" CRLF, _pBuf); 
	return 0;
}
#ifdef WITH_MD5
static int RtspSetAuthenticate(char *_pBuf, char *_realm, char *_nonce)
{
	sprintf(_pBuf, "%sWWW-Authenticate: Digest realm=\"%s\","
					"nonce=\"%s\","
					"stale=\"FALSE\""CRLF,
					_pBuf,\
					_realm,\
					_nonce);
	return 0;
}
#endif
static int RtspSetBlankLine(char *_pBuf)
{
	sprintf(_pBuf, "%s" CRLF, _pBuf);	 
	return 0;
}

static int RtspReplyCmdOption(char *_pBuf, TRtspSession *_ptSession, int _iRspCode)
{
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = NULL;
	char *pBuf = _pBuf;

	if(NULL == pBuf || NULL == ptSession)
	{
		return -1;
	}
	
	ptRtspInfo = &ptSession->m_tRtspInfo;
	
	RtspSetRspCode(pBuf, _iRspCode);
	RtspSetCSeq(pBuf, ptRtspInfo->m_iSeq);
	RtspSetDate(pBuf);
	RtspSetPublic(pBuf);
	RtspSetServer(pBuf);
	RtspSetBlankLine(pBuf);

	RtspMarkHeartTime(ptSession);

	return 0;
}

//只在debug版打印
#ifdef TD_DEBUG
static void print_sps_pps_info(char *_pcSps, int _iSpsLen, char *_pcPps, int _iPpsLen)
{
	int i = 0;

	td_printf(0, "sps len = %d", _iSpsLen);
	for(i=0; i<_iSpsLen; i++)
	{
		
		printf("0x%02x ", _pcSps[i]);
	}
	printf("\n");

	td_printf(0, "pps len = %d", _iPpsLen);
	for(i=0; i<_iPpsLen; i++)
	{
		printf("0x%02x ", _pcPps[i]);
	}
	printf("\n");
}
#endif

int RtspGenSDPInfo(char *_pcSDPBuf, TRtspSession *_ptSession)
{
	TRtspSession *ptSession = _ptSession;
	TSDPInfo *ptSdpInfo = NULL;
	
	int iAudioType = -1;
	int iAudioSampleRate = 0;
	char base64SPS[MAX_SPS_PPS_LEN] = {0};
	char base64PPS[MAX_SPS_PPS_LEN]= {0};
	char base64PPS2[MAX_SPS_PPS_LEN]= {0};

	TRtspServer *ptServer = NULL;
	TChannel *ptChannel = NULL;
	
	if(NULL == ptSession || NULL == _pcSDPBuf)
	{
		return -1;
	}
	
	ptServer = (TRtspServer *)ptSession->m_pServer;
	if(NULL == ptServer)
	{
		return -1;
	}
	ptChannel = &ptServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan];

	sprintf(_pcSDPBuf, "v=0" CRLF);
	sprintf(_pcSDPBuf, "%so=- 0 0 IN IP4 %s" CRLF, _pcSDPBuf, ptSession->m_ptConnect->m_pcLAddr);
	sprintf(_pcSDPBuf, "%ss=SDP Descrption" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%si=SDP Description" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=type:broadcast" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=tool:AglaiaSense Streaming Media" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=x-qt-text-nam:session descriped" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=x-qt-text-inf:1" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%se=nightwatcher@126.com" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sc=IN IP4 %s" CRLF, _pcSDPBuf, ptSession->m_ptConnect->m_pcRAddr);
	sprintf(_pcSDPBuf, "%st=0 0" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=control:*" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=range:npt=0-" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=x-broadcastcontrol:TIME" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=x-copyright: AglaiaSense" CRLF, _pcSDPBuf);

	//modified by qiaohaijun @2013.4.14 today has a beautiful weather.
	if(ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType == G711A)
	{
		iAudioType = RTP_PT_ALAW;
		sprintf(_pcSDPBuf, "%sm=audio %d RTP/AVP %d" CRLF, _pcSDPBuf, 0, RTP_PT_ALAW);
		sprintf(_pcSDPBuf, "%sa=rtpmap:%d PCMA/8000/1"CRLF, _pcSDPBuf, RTP_PT_ALAW);
		sprintf(_pcSDPBuf, "%sa=control:trackID=%d" CRLF, _pcSDPBuf, AUDIO_TRACK_ID);
	}
	else if(ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType == G711U)
	{
		iAudioType = RTP_PT_ULAW;
		sprintf(_pcSDPBuf, "%sm=audio %d RTP/AVP %d" CRLF, _pcSDPBuf, 0, RTP_PT_ULAW);
		sprintf(_pcSDPBuf, "%sa=rtpmap:%d PCMU/8000/1"CRLF, _pcSDPBuf, RTP_PT_ULAW);
		sprintf(_pcSDPBuf, "%sa=control:trackID=%d" CRLF, _pcSDPBuf, AUDIO_TRACK_ID);
	}
	else if(ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType == ADPCM_DIV4)
	{
		iAudioType = RTP_PT_DVI4;
		sprintf(_pcSDPBuf, "%sm=audio %d RTP/AVP %d" CRLF, _pcSDPBuf, 0, RTP_PT_DVI4);
		//adpcm的待补充
		sprintf(_pcSDPBuf, "%sa=control:trackID=%d" CRLF, _pcSDPBuf, AUDIO_TRACK_ID);
	}
	else if(ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType == AAC4)
	{
		iAudioType = RTP_PT_AAC;
		iAudioSampleRate = ptChannel->m_tChanParam.m_iAudioSampleRate;
		
		sprintf(_pcSDPBuf, "%sm=audio %d RTP/AVP %d" CRLF, _pcSDPBuf, 0, RTP_PT_AAC);
		sprintf(_pcSDPBuf, "%sa=control:trackID=%d" CRLF, _pcSDPBuf, AUDIO_TRACK_ID);

		if(iAudioSampleRate == 48000)
		{
			sprintf(_pcSDPBuf, "%sa=rtpmap:%d mpeg4-generic/48000/2" CRLF, _pcSDPBuf, RTP_PT_AAC);
			//注意和AAC的RTP包头后面的AU-headers-length保持一致modified by qiaohaijun @2013.3.8
			sprintf(_pcSDPBuf, "%sa=fmtp:%d streamtype=5; profile-level-id=14; mode=AAC-lbr; config=1190; sizeLength=13; indexLength=3; indexDeltaLength=3" CRLF, _pcSDPBuf, RTP_PT_AAC);
		}
		else if(iAudioSampleRate == 32000)
		{
			sprintf(_pcSDPBuf, "%sa=rtpmap:%d mpeg4-generic/32000/2" CRLF, _pcSDPBuf, RTP_PT_AAC);
			sprintf(_pcSDPBuf, "%sa=fmtp:%d streamtype=5; profile-level-id=14; mode=AAC-lbr; config=1290; sizeLength=13; indexLength=3; indexDeltaLength=3" CRLF, _pcSDPBuf, RTP_PT_AAC);
		}
		else if(iAudioSampleRate == 44100)
		{
			td_printf(0, "warning: the audiosamplerate is not normal.");
			sprintf(_pcSDPBuf, "%sa=rtpmap:%d mpeg4-generic/44100/2" CRLF, _pcSDPBuf, RTP_PT_AAC);
			sprintf(_pcSDPBuf, "%sa=fmtp:%d streamtype=5; profile-level-id=14; mode=AAC-lbr; config=1210; sizeLength=13; indexLength=3; indexDeltaLength=3" CRLF, _pcSDPBuf, RTP_PT_AAC);
		}
		else
		{
			td_printf(0, "the aac audio rate does not support, the audio rate === %d", iAudioSampleRate);
		}
	}
	else 
	{
		td_printf(0, "the audio ecode type [%d] does not support!!", iAudioType);
	}
	//end
	
	td_printf(0 , "iAudioType === %d", iAudioType);
	//iAudioType = RTP_PT_ALAW; //changguoxing, 获取到的是-1，暂定死为G711A。


	if(ptChannel->m_tChanParam.m_tVEncCfg.m_iEncodeType == GUI_MJPG)
	{
		sprintf(_pcSDPBuf, "%sm=video 0 RTP/AVP %d" CRLF, _pcSDPBuf, RTP_PT_JPEG);
	}
	else if(ptChannel->m_tChanParam.m_tVEncCfg.m_iEncodeType == GUI_H264)
	{
		ptSdpInfo = &ptChannel->m_tSdpInfo;
		sprintf(_pcSDPBuf, "%sm=video %d RTP/AVP %d" CRLF, _pcSDPBuf, 0, RTP_PT_H264);
		sprintf(_pcSDPBuf, "%sa=rtpmap:%d H264/90000" CRLF, _pcSDPBuf, RTP_PT_H264);

		if((ptSdpInfo->m_iSpsLen > 0) &&  (ptSdpInfo->m_iPpsLen> 0))
		{
			#ifdef TD_DEBUG
			print_sps_pps_info(ptSdpInfo->m_pcSps, ptSdpInfo->m_iSpsLen, ptSdpInfo->m_pcPps, ptSdpInfo->m_iPpsLen);
			#endif
			
			base64_encode((unsigned char*)ptSdpInfo->m_pcSps, ptSdpInfo->m_iSpsLen, base64SPS, sizeof(base64SPS));
			base64_encode((unsigned char*)ptSdpInfo->m_pcPps, ptSdpInfo->m_iPpsLen, base64PPS, sizeof(base64PPS));
			/*
			packetization-mode: 表示支持的封包模式.
			当 packetization-mode 的值为 0 时或不存在时, 必须使用单一 NALU 单元模式.
			当 packetization-mode 的值为 1 时必须使用非交错(non-interleaved)封包模式.
			当 packetization-mode 的值为 2 时必须使用交错(interleaved)封包模式.
			*/
			sprintf(_pcSDPBuf, "%sa=fmtp:%d packetization-mode=1;profile-level-id=", _pcSDPBuf, RTP_PT_H264);
			if(ptSdpInfo->m_iPps2Len)
			{
				base64_encode((unsigned char*)ptSdpInfo->m_pcPps2, ptSdpInfo->m_iPps2Len, base64PPS2, sizeof(base64PPS2));
				sprintf(_pcSDPBuf, "%s%02X%02X%02X;sprop-parameter-sets=%s,%s,%s" CRLF, _pcSDPBuf, 
					ptSdpInfo->m_pcSps[1], ptSdpInfo->m_pcSps[2], ptSdpInfo->m_pcSps[3], base64SPS, base64PPS, base64PPS2);
			}
			else
			{
				sprintf(_pcSDPBuf, "%s%02X%02X%02X;sprop-parameter-sets=%s,%s" CRLF, _pcSDPBuf, 
					ptSdpInfo->m_pcSps[1], ptSdpInfo->m_pcSps[2], ptSdpInfo->m_pcSps[3], base64SPS, base64PPS);
			}
		}
	}
	else
	{
		td_printf(0, "the video ecode type [%d] does not support !!", ptChannel->m_tChanParam.m_tVEncCfg.m_iEncodeType);
	}

	sprintf(_pcSDPBuf, "%sa=recvonly" CRLF, _pcSDPBuf);
	sprintf(_pcSDPBuf, "%sa=control:trackID=%d" CRLF, _pcSDPBuf, VIDEO_TRACK_ID);
	sprintf(_pcSDPBuf, "%sa=framerate:%d" CRLF, _pcSDPBuf, ptChannel->m_tChanParam.m_tVEncCfg.m_iFrameRate);
	sprintf(_pcSDPBuf, "%sa=x-framerate:%d" CRLF, _pcSDPBuf, ptChannel->m_tChanParam.m_tVEncCfg.m_iFrameRate);

	return 0;
}

static int RtspReplyCmdDescribe(char *_pBuf, TRtspSession *_ptSession, int _iRspCode)
{
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = NULL;
	char *pBuf = _pBuf;
	char cSDPBuf[1024] = {0};

	if(NULL == pBuf || NULL == ptSession)
	{
		return -1;
	}
	
	ptRtspInfo = &ptSession->m_tRtspInfo;

	
	RtspSetRspCode(pBuf, _iRspCode);
	RtspSetCSeq(pBuf, ptRtspInfo->m_iSeq);
	RtspSetDate(pBuf);
	RtspSetServer(pBuf);

#ifdef WITH_MD5
	TRtspServer *pRtspServer = (TRtspServer *)ptSession->m_pServer;

	if(_iRspCode == RTSP_RSP_401 || ptRtspInfo->m_iAuthFlag == AUTH_NO)
	{		
		RtspSetAuthenticate(pBuf, pRtspServer->m_tRtspAuth.m_cRealm, pRtspServer->m_tRtspAuth.m_cNonce);
		RtspSetBlankLine(pBuf);
	}
	else if(_iRspCode == RTSP_RSP_200 && ptRtspInfo->m_iAuthFlag == AUTH_YES)
#endif
	{
		sprintf(pBuf, "%s%s %s" CRLF, pBuf, "Content-Base:", ptRtspInfo->m_pcRtspUrl);
		sprintf(pBuf, "%s%s %s" CRLF, pBuf, "Content-Type:", "application/sdp");
		RtspGenSDPInfo(cSDPBuf, ptSession);
		sprintf(pBuf, "%s%s %d" CRLF, pBuf, "Content-Length:", strlen(cSDPBuf));	 
	
		RtspSetBlankLine(pBuf);

		//特别注意，SDP信息后面只有一个换行，不能再有空行。否则vlc2.05不识别。
		sprintf(pBuf, "%s%s", pBuf, cSDPBuf); //no CRLF.
	}
	
	RtspMarkHeartTime(ptSession);

	return 0;
}

static int RtspReplyCmdSetup(char *_pBuf, TRtspSession *_ptSession, int _iRspCode)
{
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = NULL;
	char *pBuf = _pBuf;

	if(NULL == pBuf || NULL == ptSession)
	{
		return -1;
	}

	ptRtspInfo = &ptSession->m_tRtspInfo;
	
	RtspSetRspCode(pBuf, _iRspCode);
	RtspSetCSeq(pBuf, ptRtspInfo->m_iSeq);
	RtspSetDate(pBuf);
	RtspSetServer(pBuf);

#ifdef WITH_MD5
	TRtspServer *pRtspServer = (TRtspServer *)ptSession->m_pServer;
	if(_iRspCode == RTSP_RSP_401 || ptRtspInfo->m_iAuthFlag == AUTH_NO)
	{		
		RtspSetAuthenticate(pBuf, pRtspServer->m_tRtspAuth.m_cRealm, pRtspServer->m_tRtspAuth.m_cNonce);
	}
	else if(_iRspCode == RTSP_RSP_200 && ptRtspInfo->m_iAuthFlag == AUTH_YES)
#endif
	{

		sprintf(pBuf, "%s%s %u;timeout=%d" CRLF, pBuf, "Session:", ptSession->m_iSessionID, RTSP_CLIENT_TIMEOUT);

		//audio
		if(ptRtspInfo->m_iTrackId == AUDIO_TRACK_ID)
		{
			if(ptSession->m_iNetMode == NET_TCP)
			{
				sprintf(pBuf, "%sTransport: RTP/AVP/TCP;unicast;interleaved=%d-%d;ssrc=%08X" CRLF, 
					pBuf,	ptRtspInfo->m_iInterleavedANo, ptRtspInfo->m_iInterleavedANo+1, 
					ptSession->m_tRtpInfo.m_tARtpHeader.ssrc);
			}
			else if(ptSession->m_iNetMode == NET_UDP)
			{
				sprintf(pBuf, "%sTransport: RTP/AVP;unicast;client_port=%u-%u;source=%s;server_port=%u-%u;ssrc=%08X" CRLF, 
					pBuf, ptSession->m_tARtpConnect.m_iRPort, ptSession->m_tARtcpConnect.m_iRPort, 
					ptSession->m_ptConnect->m_pcLAddr, ptSession->m_tARtpConnect.m_iLPort, 
					ptSession->m_tARtcpConnect.m_iLPort, ptSession->m_tRtpInfo.m_tARtpHeader.ssrc);
			}
		}
		//video
		else if(ptRtspInfo->m_iTrackId == VIDEO_TRACK_ID)
		{
			if(ptSession->m_iNetMode == NET_TCP)
			{
				sprintf(pBuf, "%sTransport: RTP/AVP/TCP;unicast;interleaved=%d-%d;ssrc=%08X" CRLF, 
					pBuf, ptRtspInfo->m_iInterleavedVNo, ptRtspInfo->m_iInterleavedVNo+1, 
					ptSession->m_tRtpInfo.m_tVRtpHeader.ssrc);
			}
			else if(ptSession->m_iNetMode == NET_UDP)
			{
				sprintf(pBuf, "%sTransport: RTP/AVP;unicast;client_port=%u-%u;destination=%s;source=%s;server_port=%u-%u;ssrc=%08X" CRLF, 
					pBuf, ptSession->m_tVRtpConnect.m_iRPort, ptSession->m_tVRtcpConnect.m_iRPort, 
					ptSession->m_ptConnect->m_pcRAddr, ptSession->m_ptConnect->m_pcLAddr, 
					ptSession->m_tVRtpConnect.m_iLPort, ptSession->m_tVRtcpConnect.m_iLPort, 
					ptSession->m_tRtpInfo.m_tVRtpHeader.ssrc);
			}
		}
		else
		{
			td_printf(0, "unregconized trackID...");
		}

		//changguoxing add 20131010
		sprintf(pBuf, "%sx-retransmit: our-retransmit" CRLF, pBuf);
		sprintf(pBuf, "%sCache-Control: must-revalidate" CRLF, pBuf);
		sprintf(pBuf, "%sx-transport-options: late-tolerance=1" CRLF, pBuf);
		sprintf(pBuf, "%sx-dynamic-rate: 1; rtt=16" CRLF, pBuf);

	}

	RtspSetBlankLine(pBuf);

	RtspMarkHeartTime(ptSession);

	return 0;
}

static int RtspReplyCmdPlay(char *_pBuf, TRtspSession *_ptSession, int _iRspCode)
{
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = NULL;
	TRtpInfo *ptRtpInfo = NULL;
	char *pBuf = _pBuf;

	if(NULL == pBuf || NULL == ptSession)
	{
		return -1;
	}
	ptRtspInfo = &ptSession->m_tRtspInfo;
	ptRtpInfo = &ptSession->m_tRtpInfo;

	
	
	RtspSetRspCode(pBuf, _iRspCode);
	RtspSetCSeq(pBuf, ptRtspInfo->m_iSeq);
	RtspSetDate(pBuf);
	RtspSetServer(pBuf);

#ifdef WITH_MD5
	TRtspServer *pRtspServer = (TRtspServer *)ptSession->m_pServer;
	if(_iRspCode == RTSP_RSP_401 || ptRtspInfo->m_iAuthFlag == AUTH_NO)
	{		
		RtspSetAuthenticate(pBuf, pRtspServer->m_tRtspAuth.m_cRealm, pRtspServer->m_tRtspAuth.m_cNonce);
	}
	else if(_iRspCode == RTSP_RSP_200 && ptRtspInfo->m_iAuthFlag == AUTH_YES)
#endif
	{

		sprintf(pBuf, "%sRange: npt=now-" CRLF, pBuf);
		sprintf(pBuf, "%sSession: %u" CRLF, pBuf, ptSession->m_iSessionID);

		sprintf(pBuf, "%sRTP-Info: url=%s/trackID=%d;seq=%d;rtptime=%d" CRLF, pBuf, 
			ptRtspInfo->m_pcRtspUrl, VIDEO_TRACK_ID, ptRtpInfo->m_tVRtpHeader.seqno, ptRtpInfo->m_tVRtpHeader.ts);
		
		
	}
	
	RtspSetBlankLine(pBuf);
	RtspMarkHeartTime(ptSession);

	return 0;
}

static int RtspReplyCmdPause(char *_pBuf, TRtspSession *_ptSession, int _iRspCode)
{
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = NULL;
	char *pBuf = _pBuf;

	if(NULL == pBuf || NULL == ptSession)
	{
		return -1;
	}
	ptRtspInfo = &ptSession->m_tRtspInfo;

	RtspSetRspCode(pBuf, _iRspCode);
	RtspSetCSeq(pBuf, ptRtspInfo->m_iSeq);
	RtspSetDate(pBuf);
	RtspSetServer(pBuf);

#ifdef WITH_MD5
	TRtspServer *pRtspServer = (TRtspServer *)ptSession->m_pServer;	
	if(_iRspCode == RTSP_RSP_401 || ptRtspInfo->m_iAuthFlag == AUTH_NO)
	{		
		RtspSetAuthenticate(pBuf, pRtspServer->m_tRtspAuth.m_cRealm, pRtspServer->m_tRtspAuth.m_cNonce);
	}
#endif

	
	RtspSetBlankLine(pBuf);

	RtspMarkHeartTime(ptSession);

	return 0;
}

static int RtspReplyCmdTeardown(char *_pBuf, TRtspSession *_ptSession, int _iRspCode)
{
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = NULL;
	char *pBuf = _pBuf;

	if(NULL == pBuf || NULL == ptSession)
	{
		return -1;
	}
	ptRtspInfo = &ptSession->m_tRtspInfo;
	
	
	RtspSetRspCode(pBuf, _iRspCode);
	RtspSetCSeq(pBuf, ptRtspInfo->m_iSeq);
	//changguoxing delete for Avigilon
	//RtspSetDate(pBuf);
	//RtspSetServer(pBuf);
#ifdef WITH_MD5
	TRtspServer *pRtspServer = (TRtspServer *)ptSession->m_pServer;
	
	if(_iRspCode == RTSP_RSP_401 || ptRtspInfo->m_iAuthFlag == AUTH_NO)
	{		
		RtspSetAuthenticate(pBuf, pRtspServer->m_tRtspAuth.m_cRealm, pRtspServer->m_tRtspAuth.m_cNonce);
	}
#endif
	RtspSetBlankLine(pBuf);

	RtspMarkHeartTime(ptSession);

	return 0;
}

static int RtspReplyCmdSetParameter(char *_pBuf, TRtspSession *_ptSession, int _iRspCode)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = NULL;
	char *pBuf = _pBuf;

	if(NULL == pBuf || NULL == ptSession)
	{
		 return -1;
	}
	ptRtspInfo = &ptSession->m_tRtspInfo;

	RtspSetRspCode(pBuf, _iRspCode);
	RtspSetCSeq(pBuf, ptRtspInfo->m_iSeq);
	RtspSetDate(pBuf);
	RtspSetServer(pBuf);
#ifdef WITH_MD5

	TRtspServer *pRtspServer = (TRtspServer *)ptSession->m_pServer;
	if(_iRspCode == RTSP_RSP_401 || ptRtspInfo->m_iAuthFlag == AUTH_NO)
	{		
		RtspSetAuthenticate(pBuf, pRtspServer->m_tRtspAuth.m_cRealm, pRtspServer->m_tRtspAuth.m_cNonce);
	}
#endif
	RtspSetBlankLine(pBuf);

	RtspMarkHeartTime(ptSession);

	return 0;
}

int RtspResponse(TRtspSession *_ptSession)
{
	//td_printf(0, "%s %d %s start!seq = %d\n", __FILE__, __LINE__, __FUNCTION__, _ptSession->m_tRtspInfo.m_iSeq);
	TRtspSession *ptSession = _ptSession;
	char cRspBuf[1024] = {0};
	TConnection *ptConnect = NULL;
	TRtspInfo *ptRtspInfo = NULL;

	TIovec stIovec = {0};
	int iWrLen = 0;
	//add by cgx
	TChannel *ptChannel = NULL;
	TRtspServer *ptServer = NULL;
	int iViNum = -1;
	int iShmNum = -1;
	//add end

	if(NULL == ptSession->m_ptConnect || NULL == ptSession)
	{
		td_printf(0, "RtspResponse, input session null.");
		return TD_ERROR;
	}
	
	ptConnect = ptSession->m_ptConnect;
	ptRtspInfo = &ptSession->m_tRtspInfo;
	td_printf(0, "remote ip is %s", ptConnect->m_pcRAddr);
#ifndef WITH_MD5
	ptRtspInfo->m_iRspCode = RTSP_RSP_200;
#endif	

	if(ptRtspInfo->m_iCmdType == RTSP_CMD_OPTIONS)
	{
		RtspReplyCmdOption(cRspBuf, ptSession, ptRtspInfo->m_iRspCode);
	}
	else if(ptRtspInfo->m_iCmdType == RTSP_CMD_DESCRIBE)
	{
		RtspReplyCmdDescribe(cRspBuf, ptSession, ptRtspInfo->m_iRspCode);
	}
	else if(ptRtspInfo->m_iCmdType == RTSP_CMD_SETUP)
	{
		RtspReplyCmdSetup(cRspBuf, ptSession, ptRtspInfo->m_iRspCode);
	}
	else if(ptRtspInfo->m_iCmdType == RTSP_CMD_PLAY)
	{
		//changguoxing, modify for ts==0 in play reply.
		ptServer = (TRtspServer *)ptSession->m_pServer;
		ptChannel = &ptServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan];
		ptSession->m_tRtpInfo.m_tVRtpHeader.ts = ptChannel->m_tFrameHeader.u32TimeStamp*90;
		RtspReplyCmdPlay(cRspBuf, ptSession, ptRtspInfo->m_iRspCode);
	}
	else if(ptRtspInfo->m_iCmdType == RTSP_CMD_PAUSE)
	{
		RtspReplyCmdPause(cRspBuf, ptSession, ptRtspInfo->m_iRspCode);
	}
	else if(ptRtspInfo->m_iCmdType == RTSP_CMD_TEARDOWN)
	{
		RtspReplyCmdTeardown(cRspBuf, ptSession, ptRtspInfo->m_iRspCode);
	}
	else if(ptRtspInfo->m_iCmdType == RTSP_CMD_SETPARAMETER)
	{
		RtspReplyCmdSetParameter(cRspBuf, ptSession, ptRtspInfo->m_iRspCode);
	}
	else
	{
		td_printf(0, "unknown rtsp cmd...");
	}

	//to send rsp 
	td_printf(0, "rtsp rsp buffer = \n***************************************\n%s***************************************\n", cRspBuf);
	stIovec.iov_base = cRspBuf;
	stIovec.iov_len = strlen(cRspBuf);

	if(ptConnect->m_iStat != SC_CONNECTED || ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		td_printf(0, "==========warnning: m_iStat error!\n==========");
		return TD_ERROR;
	}
	
	iWrLen = ptConnect->m_WriteV(ptConnect, &stIovec, 1);
	//if reply failed, we return.
	if(iWrLen <= 0)
	{
		td_printf(0, "writev response failed. writev len = %d", iWrLen);
		return TD_ERROR;
	}

	//add by cgx
	if(ptRtspInfo->m_iCmdType == RTSP_CMD_PLAY)
	{
		ptServer = (TRtspServer *)ptSession->m_pServer;
		ptChannel = &ptServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan];
		iViNum = ptServer->m_tNvsInfo.m_iVINum;
		iShmNum = ptChannel->m_iStream * iViNum + ptChannel->m_iChan + 1;
		td_printf(0, "to ireq shmno = %d", iShmNum);
		OnvifCmdIframeReq(iShmNum);
	}

	//when teardown, we clean resources.
	if(ptRtspInfo->m_iCmdType == RTSP_CMD_TEARDOWN)
	{
		td_printf(0, "%s, %d, we get TEARDOWN, so we close the link.", __FUNCTION__, __LINE__);
		ReleaseRtspSession(ptSession);
	}
	
	return TD_OK;
}
//add end


int ParseRtspCmd(TRtspSession *_ptSession, const char *_pcData, int _iSize)
{
	TRtspSession *ptSession = _ptSession;
	TRtspInfo *ptRtspInfo = &ptSession->m_tRtspInfo;
	const char *pcData = _pcData;
	int iSize = _iSize;
	//int iPos = 0; //记录分析数据的位置
	int iRet = -1;
	
	if(NULL == pcData || NULL == ptSession || NULL == ptRtspInfo || iSize <= 0)
	{
		td_printf(0, "%s %d %s, input param error!!!\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	
	if(RtspReqIsOption(pcData))
	{
		ptRtspInfo->m_iCmdType = RTSP_CMD_OPTIONS;
		iRet = RtspParseCmdOption(pcData, ptRtspInfo);
		if(iRet < 0)
		{
			td_printf(0, "%s %d %s warnning: parse cmd RTSP_CMD_OPTIONS failed!\n", __FILE__, __LINE__, __FUNCTION__);
			return 0;
		}
		//加判断是为了适应测试工具ODT，在PLAY之后发OPTIONS命令。20130527
		if(ptSession->m_iStat < SESSION_OPTIONS)
		{
			ptSession->m_iStat = SESSION_OPTIONS;
		}
	}
	else if(RtspReqIsDescribe(pcData))
	{
		ptRtspInfo->m_iCmdType = RTSP_CMD_DESCRIBE;
		iRet = RtspParseCmdDescribe(pcData, ptSession);
		if(iRet < 0)
		{
			td_printf(0, "%s %d %s warnning: parse cmd RTSP_CMD_DESCRIBE failed!\n", __FILE__, __LINE__, __FUNCTION__);
			return 0;
		}
		ptSession->m_iStat = SESSION_DESCRIBE;
	}
	else if(RtspReqIsSetup(pcData))
	{
		ptRtspInfo->m_iCmdType = RTSP_CMD_SETUP;
		iRet = RtspParseCmdSetup(pcData, ptSession);
		if(iRet < 0)
		{
			td_printf(0, "%s %d %s warnning: parse cmd RTSP_CMD_SETUP failed!\n", __FILE__, __LINE__, __FUNCTION__);
			return 0;
		}
		ptSession->m_iStat = SESSION_SETUP;
	}
	else if(RtspReqIsPlay(pcData))
	{
		ptRtspInfo->m_iCmdType = RTSP_CMD_PLAY;
		iRet = RtspParseCmdPlay(pcData, ptSession);
		if(iRet < 0)
		{
			td_printf(0, "%s %d %s warnning: parse cmd RTSP_CMD_PLAY failed!\n", __FILE__, __LINE__, __FUNCTION__);
			return 0;
		}
		ptSession->m_iStat = SESSION_PLAY;
	}
	else if(RtspReqIsPause(pcData))
	{
		ptRtspInfo->m_iCmdType = RTSP_CMD_PAUSE;
		iRet = RtspParseCmdPause(pcData, ptSession);
		if(iRet < 0)
		{
			td_printf(0, "%s %d %s warnning: parse cmd RTSP_CMD_PAUSE failed!\n", __FILE__, __LINE__, __FUNCTION__);
			return 0;
		}
		ptSession->m_iStat = SESSION_PAUSE;
	}
	else if(RtspReqIsTearDown(pcData))
	{
		 ptRtspInfo->m_iCmdType = RTSP_CMD_TEARDOWN;
		iRet = RtspParseCmdTeardown(pcData, ptSession);
		if(iRet < 0)
		{
			td_printf(0, "%s %d %s warnning: parse cmd RTSP_CMD_TEARDOWN failed!\n", __FILE__, __LINE__, __FUNCTION__);
			return 0;
		}
		ptSession->m_iStat = SESSION_TEARDOWN;
	}
	else if(RtspReqIsSetParameter(pcData))
	{
		ptRtspInfo->m_iCmdType = RTSP_CMD_SETPARAMETER;
		iRet = RtspParseCmdSetParameter(pcData, ptSession);
		if(iRet < 0)
		{
			td_printf(0, "%s %d %s warnning: parse cmd RTSP_CMD_TEARDOWN failed!\n", __FILE__, __LINE__, __FUNCTION__);
			return 0;
		}
	}
	else
	{
		td_printf(0, "%s %d %s warnning: cmd is unregnized!\n", __FILE__, __LINE__, __FUNCTION__);
		RtspMarkHeartTime(ptSession);
		return 0;
	}

	
	iRet = RtspResponse(ptSession);
	if(iRet < 0)
	{
		td_printf(0, "%s %d %s warnning: RtspResponse error!\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}

	return iSize;
}

int ParseBase64RtspCmd(TRtspSession *_ptSession, const char *_pcData, int _iSize)
{
	TRtspSession *ptGetSession = _ptSession;
	const char *pcData = _pcData;
	char buf[1024] = {0};
	char cRtspCmd[1024] = {0};
	int iRtspCmdSize = 0;

	if(NULL == pcData || NULL == ptGetSession || _iSize <= 0)
	{
		td_printf(0, "%s %d %s, input param error!!!\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	//td_printf(0, "base64 rtsp cmd:\n%s", pcData);
	
	//we have to get rid of "\r\n"s from pcData, or the rtsp cmd will be not complete.
	ChangeBase64Format(pcData, buf); //don't care ret value.
	
	iRtspCmdSize  = from64tobits(cRtspCmd, buf);
	if(iRtspCmdSize <= 0)
	{
		td_printf(0, "%s %d %s, decode base64 error!!!\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	//td_printf(0, "decoded rtsp cmd:\n%s", cRtspCmd);
	ParseRtspCmd(ptGetSession, cRtspCmd, iRtspCmdSize);

	return _iSize;
}


int ParseHttpConnect(TConnection *_ptConnect)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	
	if(ptConnect->m_iStat != SC_CONNECTED || ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		return TD_ERROR;
	}

	return TD_OK;
}

int ParseRtcpConnect(TConnection *_ptConnect)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	
	if(ptConnect->m_iStat != SC_CONNECTED || ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		return TD_ERROR;
	}

	return TD_OK;
}

//HTTP section
static int HttpParseCmdGet(const char *_pData, TRtspSession* _ptSession)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = _ptSession;
	const char *pData     = _pData;
	char cValue[1024] = {0};
	int iRet = -1;
	
	if(NULL == pData || NULL == ptSession)
	{
		return -1;
	}

	iRet = RtspGetValueByName(pData, "x-sessioncookie", cValue);
	if(0 != iRet)
	{
		td_printf(0, "%s, RtspGetValueByName error. iRet = %d", __FUNCTION__, iRet);
		return -1;
	}
	strncpy(ptSession->m_pcHttpSessionCookie, cValue, HTTP_SESSION_COOKIE_LEN -1);
	//td_printf(0, "HttpParseCmdGet, m_pcHttpSessionCookie = %s", ptSession->m_pcHttpSessionCookie);
	ptSession->m_iStat = SESSION_GET;

	return 0;
}

static int HttpParseCmdPost(TRtspSession* _ptSession, const char *_pData)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptPostSession = _ptSession;	
	const char *pData = _pData;

	char cValue[1024] = { 0 };
	TRtspSession *ptGetSession = NULL;
	char *pTemp = NULL;
	int iRet = -1;
	
	if(NULL == pData || NULL == ptPostSession)
	{
		return -1;
	}
	td_printf(0, "\n%s", pData);
	iRet = RtspGetValueByName(pData, "x-sessioncookie", cValue);
	if(0 != iRet)
	{
		td_printf(0, "%s, RtspGetValueByName failed.", __FUNCTION__);
		return -1;
	}
	ptGetSession = FindSessionByHttpCookie(ptPostSession, cValue);
	if(NULL == ptGetSession)
	{
		td_printf(0, "%s, FindSessionByHttpCookie failed.", __FUNCTION__);
		return -1;
	}
	strncpy(ptPostSession->m_pcHttpSessionCookie, cValue, HTTP_SESSION_COOKIE_LEN -1);
	//SESSION_POST should be set after session found.
	ptPostSession->m_iStat = SESSION_POST;
	pTemp = strstr((char*)pData, "\r\n\r\n");
	pTemp += strlen("\r\n\r\n");

	ParseBase64RtspCmd(ptGetSession, pTemp, strlen(pTemp));

	return 0;
}

int HttpGenGetReplyMsg(char *_pBuf, int _iRspCode)
{
	char *pBuf = _pBuf;
	char *pcRspText = NULL; 
	
	if(NULL == pBuf)
	{
		return -1;
	}

	if(_iRspCode == 200)
	{
		pcRspText = "OK";
	}

	sprintf(pBuf, "HTTP/1.0 %d %s" CRLF, _iRspCode, pcRspText);
	sprintf(pBuf, "%sCache-Control: no-store" CRLF, pBuf);
	RtspSetBlankLine(pBuf);

	return 0;
}

int HttpReplyGet(TRtspSession *_ptSession)
{
	TRtspSession *ptSession = _ptSession;
	TConnection *ptConnect = NULL;
	char cRspBuf[1024] = {0};	
	TIovec stIovec = {0};
	int iWrLen = 0;

	if(NULL == ptSession)
	{
		return -1;
	}
	ptConnect = ptSession->m_ptConnect;

	HttpGenGetReplyMsg(cRspBuf, 200);
	//GET之后超时收不到RTSP则销毁session.
	RtspMarkHeartTime(ptSession);
	
	stIovec.iov_base = cRspBuf;
	stIovec.iov_len = strlen(cRspBuf);
	

	iWrLen = ptConnect->m_WriteV(ptConnect, &stIovec, 1);
	//if reply failed, we return.
	if(iWrLen <= 0)
	{
		td_printf(0, "HttpResponse failed. writev len = %d", iWrLen);
		return -1;
	}

	return 0;
}
 
int ParseHttpCmd(TRtspSession *_ptSession, const char *_pcData, int _iSize)
{
	TRtspSession *ptSession = _ptSession;
	const char *pcData = _pcData;

	if(NULL == ptSession || NULL == pcData)
	{
		return -1;
	}
	//td_printf(0, "%s, ringbuf_GetData:\n%s\n", __FUNCTION__, pcData);
	
	if(HttpCmdIsGet(pcData))
	{
		HttpParseCmdGet(pcData, ptSession);
		//to reply
		HttpReplyGet(ptSession);
	}
	else if(HttpCmdIsPost(pcData))
	{
		HttpParseCmdPost(ptSession, pcData);//we don't care the return value.
	}
	
	return 0;
}
//HTTP end

int RtcpSendSR(TRtspSession *_ptSession, int _iInterleavedNo)
{
	TRtspSession *ptSession = _ptSession;
	int iInterleavedNo = _iInterleavedNo;
	struct timeval now;
	unsigned char  ucHeader[4] = {0};
	unsigned char  ucBuf[1024] = {0};
	unsigned int ntp_sec, ntp_usec;
	TRtpHeader *ptRtpHeader = NULL;
	char *pText = "AglaiaSense stream";
	TConnection *ptConnect = NULL;
	TIovec stIovec[2];
	int iRtcpLen = -1;
	int iRet = -1;

	if(NULL == ptSession)
	{
		td_printf(0, "%s, input param error.", __FUNCTION__);
		return -1;
	}
	ptConnect = ptSession->m_ptConnect;
	if(NULL == ptConnect)
	{
		td_printf(0, "%s, input param, ptConnect error.", __FUNCTION__);
		return -1;
	}

	if(iInterleavedNo == ptSession->m_tRtspInfo.m_iInterleavedANo+1)
	{
		ptRtpHeader = &ptSession->m_tRtpInfo.m_tARtpHeader;
	}
	else if(iInterleavedNo == ptSession->m_tRtspInfo.m_iInterleavedVNo+1)
	{
		ptRtpHeader = &ptSession->m_tRtpInfo.m_tVRtpHeader;
	}
	else
	{
		td_printf(0, "%s, iInterleavedNo error!", __FUNCTION__);
		return -1;
	}
	
#if 1
	gettimeofday( &now, NULL );
	ntp_sec = now.tv_sec + 0x83AA7E80;
	ntp_usec = (double)( (double)now.tv_usec * (double)0x4000000 ) / 15625.0;
	
	ucBuf[0] = 2 << 6; // version
	ucBuf[1] = 200;		// packet type is Sender Report
	PUT_16( ucBuf + 2, 6 ); // length in words minus one
	PUT_32( ucBuf + 4, ptRtpHeader->ssrc);
	PUT_32( ucBuf + 8, ntp_sec);
	PUT_32( ucBuf + 12, ntp_usec );
	PUT_32( ucBuf + 16, ptRtpHeader->ts);
	PUT_32( ucBuf + 20, 0);//packets send
	PUT_32( ucBuf + 24, 0);//bytes send
	ucBuf[28] = ( 2 << 6 ) | 1; // version; source count = 1
	ucBuf[29] = 202; // packet type is Source Description
	PUT_16( ucBuf + 30, 5 ); // length in words minus one
	PUT_32( ucBuf + 32, ptRtpHeader->ssrc);
	ucBuf[36] = 0x01; // field type is CNAME
	ucBuf[37] = strlen(pText); // text length
	memcpy( ucBuf + 38, pText, strlen(pText));

	iRtcpLen = 38 + strlen(pText);
		
	//send rtcp reply
	ucHeader[0] = '$';
	ucHeader[1] = iInterleavedNo;
	PUT_16(&ucHeader[2], iRtcpLen);
	stIovec[0].iov_base = ucHeader;
	stIovec[0].iov_len = 4;
	
	stIovec[1].iov_base = ucBuf;
	stIovec[1].iov_len = iRtcpLen;
	//iRet = ptConnect->m_WriteV(ptConnect, stIovec, 2);
	//td_printf(0, "%s, %d, to send tcp SR, sock = %d.", __FUNCTION__, __LINE__, ptConnect->m_iSocket);
	iRet = RtpWriteV(ptConnect, stIovec, 2);
	if(iRet <= 0)
	{
		td_printf(0, "%s, %d, writev failed, ret = %d.", __FUNCTION__, __LINE__, iRet);
		return -1;
	}
#endif
	return 0;
}
 
int ParseRtcpCmd(TRtspSession *_ptSession, const char *_pcData, int _iSize)
{
	TRtspSession *ptSession = _ptSession;
	const char *pcData = _pcData;
	int iSize = _iSize;
	int iPos = 0;	 //记录分析数据的位置
	int iInterleavedNo = -1;
	int iRtcpFlag = -1;
	int iRet = -1;

	if(NULL == pcData || NULL == ptSession)
	{
		td_printf(0, "%s, input param error.", __FUNCTION__);
		return -1;
	}
	#if 0
	//magic:1B, channel:1B, length:2B, rtcp header:1B, packet_type:1B, length:2B, sender ssrc:4B
	td_printf(0, "%s, pData[0] = %c", __FUNCTION__, pcData[0]);
	td_printf(0, "%s, pData[1] = %x", __FUNCTION__, pcData[1]);
	td_printf(0, "%s, pData[2] = %x", __FUNCTION__, pcData[2]);
	td_printf(0, "%s, pData[3] = %x", __FUNCTION__, pcData[3]);
	td_printf(0, "%s, pData[4] = %x", __FUNCTION__, pcData[4]);
	#endif
	
	//to parse rtcp 
	iInterleavedNo = (int)pcData[1];
	iRtcpFlag = (int)pcData[5];
	//td_printf(0, "iRtcpFlag = %d", iRtcpFlag);
	switch(iRtcpFlag)
	{
		//sr
		case 200: 
			break;
		//rr
		case 201: 
			iRet = RtcpSendSR(ptSession, iInterleavedNo);
			break;
		//sdes
		case 202:
			break;
		//bye
		case 203: 
			break;
		case 204:
			break;
		default:
			break;
	}

	if(-1 != iRet)
	{
		iPos = iSize;
	}	

	return iPos;
}

int UdpRtcpSendSR(TRtspSession *_ptSession, int _iIsVideo)
{
	TRtspSession *ptSession = _ptSession;
	struct timeval now;
	unsigned char  ucBuf[1024] = {0};
	unsigned int ntp_sec, ntp_usec;
	TRtpHeader *ptRtpHeader = NULL;
	char *pText = "AglaiaSense stream";
	TConnection *ptConnect = NULL;
	TIovec stIovec;
	int iRtcpLen = -1;
	int iRet = -1;

	if(NULL == ptSession)
	{
		td_printf(0, "%s, input param error.", __FUNCTION__);
		return -1;
	}

	if(_iIsVideo)
	{
		ptConnect = &ptSession->m_tVRtcpConnect;
		ptRtpHeader = &ptSession->m_tRtpInfo.m_tVRtpHeader;
	}
	else
	{
		ptConnect = &ptSession->m_tARtcpConnect;
		ptRtpHeader = &ptSession->m_tRtpInfo.m_tARtpHeader;
	}
	
	gettimeofday( &now, NULL );
	ntp_sec = now.tv_sec + 0x83AA7E80;
	ntp_usec = (double)( (double)now.tv_usec * (double)0x4000000 ) / 15625.0;
	
	ucBuf[0] = 2 << 6; // version
	ucBuf[1] = 200;		// packet type is Sender Report
	PUT_16( ucBuf + 2, 6 ); // length in words minus one
	PUT_32( ucBuf + 4, ptRtpHeader->ssrc);
	PUT_32( ucBuf + 8, ntp_sec);
	PUT_32( ucBuf + 12, ntp_usec );
	PUT_32( ucBuf + 16, ptRtpHeader->ts);
	PUT_32( ucBuf + 20, 0);//packets send
	PUT_32( ucBuf + 24, 0);//bytes send
	ucBuf[28] = ( 2 << 6 ) | 1; // version; source count = 1
	ucBuf[29] = 202; // packet type is Source Description
	PUT_16( ucBuf + 30, 5); // length in words minus one
	PUT_32( ucBuf + 32, ptRtpHeader->ssrc);
	ucBuf[36] = 0x01; // field type is CNAME
	ucBuf[37] = strlen(pText); // text length
	memcpy( ucBuf + 38, pText, strlen(pText));

	iRtcpLen = 38 + strlen(pText);
	
	stIovec.iov_base = ucBuf;
	stIovec.iov_len = iRtcpLen;
	iRet = ptConnect->m_WriteV(ptConnect, &stIovec, 1);
	if(iRet <= 0)
	{
		td_printf(0, "%s, writev failed. iRet = %d", __FUNCTION__, iRet);
		return -1;
	}

	return 0;
}

int ParseUdpRtcpCmd(TRtspSession *_ptSession, int _iIsVideo, const char *_pcData, int _iSize)
{
	 //td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = _ptSession;
	const char *pcData = _pcData;
	int iSize = _iSize;
	int iPos = 0;	 //记录分析数据的位置
	int iRtcpFlag = -1;
	int iRet = -1;

	if(NULL == pcData || NULL == ptSession)
	{
		td_printf(0, "%s, input param error.", __FUNCTION__);
		return -1;
	}
	#if 0
	//udp rtcp header:1B, packet_type:1B, length:2B, sender ssrc:4B
	#endif
	
	//to parse rtcp 
	iRtcpFlag = (int)pcData[1];
	//td_printf(0, "%s, iRtcpFlag = %d", __FUNCTION__, iRtcpFlag);
	switch(iRtcpFlag)
	{
		case 200: //sr
			break;
		case 201: //rr
			iRet = UdpRtcpSendSR(ptSession, _iIsVideo);
			break;
		case 202://sdes
			break;
		case 203: //bye
			break;
		case 204:
			break;
			
			default:
				break;
	}

	if(-1 != iRet)
	{
		iPos = iSize;
	}	

	return iPos;
}

static char *FindHttpCmd(const char *_pcData, int _iSize)
{
	const char *pcData = _pcData;
	int iSize = _iSize;
	char *pHttpCmdPos = NULL;
	
	if(NULL == pcData || iSize <= 0)
	{
		td_printf(0, "%s, input param error.", __FUNCTION__);
		return NULL;
	}

	pHttpCmdPos = strstr((char *)pcData, "GET");
	if(NULL != pHttpCmdPos)
	{
		return pHttpCmdPos;
	}
	pHttpCmdPos = strstr((char *)pcData, "POST");
	if(NULL != pHttpCmdPos)
	{
		return pHttpCmdPos;
	}

	return NULL;
}

static char *FindRtspCmd(const char *_pcData, int _iSize)
{
	const char *pcData = _pcData;
	int iSize = _iSize;
	char *pRtspCmdPos = NULL;
	
	if(NULL == pcData || iSize <= 0)
	{
		td_printf(0, "%s, input param error.", __FUNCTION__);
		return NULL;
	}

	pRtspCmdPos = strstr((char *)pcData, "OPTIONS");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "DESCRIBE");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "SETUP");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "PLAY");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "PAUSE");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "TEARDOWN");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "SET_PARAMETER");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "GET_PARAMETER");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "ANNOUNCE");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}
	pRtspCmdPos = strstr((char *)pcData, "RECORD");
	if(NULL != pRtspCmdPos)
	{
		return pRtspCmdPos;
	}

	return NULL;
}

int ParseRtspSession(TRtspSession *_ptSession)
{
	//td_printf(0, "%s %d %s start!socket = %d\n", __FILE__, __LINE__, __FUNCTION__, _ptSession->m_ptConnect->m_iSocket);

	TRtspSession *ptSession = _ptSession;
	TConnection *ptConnect = NULL;
	TDataPackage *ptDataPackage = NULL;
	int iSize = 0;
	int iRet = 0;
	char * pData = NULL;
	char *pCmdPos = NULL;
	TRtspSession *ptGetSession = NULL;
	
	if(NULL == ptSession)
	{
		td_printf(0, "%s, %d, input param error.", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
	ptConnect = ptSession->m_ptConnect;
	if(NULL == ptConnect)
	{
		td_printf(0, "%s, %d, input param error.", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
	
	if(ptConnect->m_iStat != SC_CONNECTED || ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		//td_printf(0, "%s, ptConnect->m_iStat =%d, ptConnect->m_iError = %d", __FUNCTION__, ptConnect->m_iStat, ptConnect->m_iError);
		return TD_ERROR;
	}

	while(datacenter_GetFifoPackNum(ptConnect->m_ptRecvFifo) > 0)
	{
		ptDataPackage = datacenter_GetPackFromFifo(ptConnect->m_ptRecvFifo);
		if(NULL == ptDataPackage)
		{
			td_printf(0, "==========%s %d %s: tcp Data FIFO is error!==========\n", __FILE__, __LINE__, __FUNCTION__);
			iRet = TD_ERROR;
			goto EXIT;
		}

		//写入tcp ringbuf
		iSize =  ringbuf_Capacity(ptConnect->m_ptTcpRingBuf);
		if(ptDataPackage->m_iDataSize > iSize)
		{
			td_printf(0, "==========%s %d %s: tcp Data Package is too big!==========\n", __FILE__, __LINE__, __FUNCTION__);
			datacenter_PackageRelease(ptDataPackage);
			ptDataPackage = NULL;
			continue;
		}
		else
		{
			ringbuf_Write(ptDataPackage->m_iDataSize, ptDataPackage->m_bData, ptConnect->m_ptTcpRingBuf);
			datacenter_PackageRelease(ptDataPackage);
			ptDataPackage = NULL;
		}
		//解析ringbuf
		while((iSize = ringbuf_DataSize(ptConnect->m_ptTcpRingBuf)) > 0)
		{
			pData = (char *)ringbuf_GetData(ptConnect->m_ptTcpRingBuf);
			//td_printf(0, "==========%s %d %s ringbuf_GetData==========\n%s\n", __FILE__, __LINE__, __FUNCTION__, pData);
			//td_printf(0, "%s %d %s, size = %d", __FILE__, __LINE__, __FUNCTION__, iSize);
			
			if(pData[0] != '$') //rtsp, http cmd
			{
				//判断为合法的rtsp cmd.
				pCmdPos = FindRtspCmd(pData, iSize);
				if(NULL != pCmdPos)
				{
					//remove unrecognized chars
					if(pCmdPos - pData > 0)
					{
						ringbuf_Remove(pCmdPos - pData, ptConnect->m_ptTcpRingBuf);	 
					}
					//一个rtsp命令可能是两个epoll recv，需要判断\r\n\r\n来识别为一个完整的命令。
					if(NULL == strstr(pCmdPos, "\r\n\r\n"))
					{
						td_printf(0, "%s %d %s: we get long rtsp cmd.\n", __FILE__, __LINE__, __FUNCTION__);
						break;
					}
					pData[iSize] = '\0';
					ParseRtspCmd(ptSession, pData, iSize);
				}
				else //if http
				{
					pCmdPos = FindHttpCmd(pData, iSize);
					if(NULL != pCmdPos)
					{
						//remove unrecognized chars
						if(pCmdPos - pData > 0)
						{
							ringbuf_Remove(pCmdPos - pData, ptConnect->m_ptTcpRingBuf);	 
						}
						//怎么判断是一个完整的POST命令?
						pCmdPos = strstr(pCmdPos, "POST");
						if(NULL != pCmdPos)
						{
							pCmdPos = strstr(pCmdPos, "\r\n\r\n");
							pCmdPos += strlen("\r\n\r\n");
							if(strlen(pCmdPos) <= 0)
							{
								td_printf(0, "%s %d %s: we get long http cmd.\n", __FILE__, __LINE__, __FUNCTION__);
								break;
							}
						}
					
						pData[iSize] = '\0';
						ParseHttpCmd(ptSession, pData, iSize);
					}
					else
					{
						//when not '$', not RTSP cmd, not HTTP cmd, we assume it to be base64 RTSP cmd.
						pData[iSize] = '\0';
						//find get session
						ptGetSession = FindSessionByHttpCookie(ptSession, ptSession->m_pcHttpSessionCookie);
						if(NULL != ptGetSession)
						{
							ParseBase64RtspCmd(ptGetSession, pData, iSize);
						}
					}
				}
				//if not find cmd, we remove, if find, no matter parse ok or not, we remove this cmd anyway.
				ringbuf_Remove(iSize, ptConnect->m_ptTcpRingBuf);//after remove all data, when data come, write from beginning.	
			}
			else
			{
				RtspMarkHeartTime(ptSession);
				
				//等待$后面的内容, vlc rtcp保活先发1B的标识，再发rtcp数据。
				//20130614, ODM rtcp保活有时直接发60B的数据，有时分为两次发4B + 56B的数据。改为收到小于4B的数据任务还没收完。
				if(iSize <= 4)
				{
					break;
				}
				else
				{
					pData[iSize] = '\0'; // a RTCP packet data.
				}
				ParseRtcpCmd(ptSession, pData, iSize);
				ringbuf_Remove(iSize, ptConnect->m_ptTcpRingBuf);	
			}
		}
	}
	
EXIT:
	return iRet;
}

int ParseUdpRtcp(TRtspSession *_ptSession)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TRtspSession *ptSession = _ptSession;
	TConnection *ptARtcpConnect = NULL;
	TConnection *ptVRtcpConnect = NULL;
	TDataPackage *ptDataPackage = NULL;
	int iRet = 0;
	
	if(NULL == ptSession)
	{
		td_printf(0, "%s, %d, input param error.", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}

	ptARtcpConnect = &ptSession->m_tARtcpConnect;
	ptVRtcpConnect = &ptSession->m_tVRtcpConnect;
	
	//audio rtcp
	while(datacenter_GetFifoPackNum(ptARtcpConnect->m_ptRecvFifo) > 0)
	{
		//mark hearttime
		RtspMarkHeartTime(ptSession);
		
		ptDataPackage = datacenter_GetPackFromFifo(ptARtcpConnect->m_ptRecvFifo);
		if(NULL == ptDataPackage)
		{
			td_printf(0, "==========%s %d %s: udp Data FIFO is error!==========\n", __FILE__, __LINE__, __FUNCTION__);
			iRet = TD_ERROR;
			goto EXIT;
		}
		//td_printf(0, "%s, artcp datalen = %d", __FUNCTION__, ptDataPackage->m_iDataSize);
		ParseUdpRtcpCmd(ptSession, 0, (char *)ptDataPackage->m_bData, ptDataPackage->m_iDataSize);
		datacenter_PackageRelease(ptDataPackage);
	}
	//video rtcp
	while(datacenter_GetFifoPackNum(ptVRtcpConnect->m_ptRecvFifo) > 0)
	{
		//mark hearttime
		RtspMarkHeartTime(ptSession);
		
		ptDataPackage = datacenter_GetPackFromFifo(ptVRtcpConnect->m_ptRecvFifo);
		if(NULL == ptDataPackage)
		{
			td_printf(0, "==========%s %d %s: udp Data FIFO is error!==========\n", __FILE__, __LINE__, __FUNCTION__);
			iRet = TD_ERROR;
			goto EXIT;
		}
		//td_printf(0, "%s, vrtcp datalen = %d", __FUNCTION__, ptDataPackage->m_iDataSize);
		ParseUdpRtcpCmd(ptSession, 1, (const char*)ptDataPackage->m_bData, ptDataPackage->m_iDataSize);
		datacenter_PackageRelease(ptDataPackage);
	}

EXIT:
	return iRet;
}

int ParseRtspServer(TRtspServer *_ptServer)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TRtspServer *ptServer = _ptServer;
	int iSessionNum = 0;
	DListNode *ptNode = NULL;
	TRtspSession *ptSession = NULL;
	TRtspSession **pptSession = NULL;
	int iRet = 0;
	int i = 0;
	
	if(NULL == ptServer)
	{
		return TD_ERROR;
	}
	adt_dlist_lock(ptServer->m_ptDListSession);
	iSessionNum = adt_dlist_size(ptServer->m_ptDListSession);
	//td_printf(0, "%s %d %s iSessionNum %d\n", __FILE__, __LINE__, __FUNCTION__, iSessionNum);
	if(iSessionNum <= 0)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	ptNode = adt_dlist_head(ptServer->m_ptDListSession);
	for(i = 0; i < iSessionNum; i++)
	{
		if(NULL == ptNode)
		{
			td_printf(0, "%s %d %s ptNode warnning!\n", __FILE__, __LINE__, __FUNCTION__);
			iRet = TD_ERROR;
			goto EXIT;
		}
		
		pptSession = (TRtspSession**)adt_dlist_getdata(ptNode); 
		if(NULL == pptSession)
		{
			td_printf(0, "%s %d %s, find no session!\n", __FILE__, __LINE__, __FUNCTION__);
			iRet = TD_ERROR;
			goto EXIT;
		}
		ptSession = *pptSession;

		iRet = MutexLockTryLock(&ptSession->m_tLock);
		if(iRet != TD_OK)
		{
			continue;
		}
	
		ParseRtspSession(ptSession);
		ParseUdpRtcp(ptSession);

		SESN_DBG_LOCK3(ptSession, "ptSession");
		MutexLockUnLock(&ptSession->m_tLock);
		SESN_DBG_LOCK4(ptSession, "ptSession");
		
		ptNode = adt_dlist_next(ptNode);
	}

EXIT:
	adt_dlist_unlock(ptServer->m_ptDListSession);
	return iRet;
}

int ParseHttpServer(TRtspServer *_ptServer)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TRtspServer *ptServer = _ptServer;

	if(NULL == ptServer)
	{
		return TD_ERROR;
	}

	return TD_OK;
}

int ParseRtcpServer(TRtspServer *_ptServer)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TRtspServer *ptServer = _ptServer;
	
	if(NULL == ptServer)
	{
		return TD_ERROR;
	}

	return TD_OK;
}
	
int RtspCmdProcess(TRtspServer *_ptServer)
{
	//rtsp
	ParseRtspServer(_ptServer);
	//http
	ParseHttpServer(_ptServer);
	//rtcp
	ParseRtcpServer(_ptServer);

	return TD_OK;
}

