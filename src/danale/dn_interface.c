/*
 * 	file: dn_interface.c
 *	description: danale interface
 *	history: pangtaibao created at 20131017
 */
#include "dn_interface.h"
#include "dn_main.h"
#include "callee.h"
#include "DAp2pcmd.h"
#include "dcsconst.h"

#include "guiparameter.h"
#include "share_mem.h"
#include "onvif_sdk.h"
#include "onvif_shm_cmd.h"
#include "rtsp_data.h"
#include "proparser.h"
#include "iniparser.h"
#include "dictionary.h"
#include "mutex_lock.h"
#include "channel.h"
#include <time.h>

extern Dana_Session g_tSession[MAX_CHANNEL_NUM][MAX_CONNECT_NUM];
extern OnvifCtrl g_stOnvifCtrl;
extern int g_iConNum;

#define DANALE_VERSION "danale_v2.0.1"
#define OSD_MIDDLE_X	11
#define OSD_MIDDLE_Y	7
#define OSD_MAX_X	22
#define OSD_MAX_Y	18

/**************************************************
*功能:		获取网络参数
*参数:		_pReq	:	请求参数
*返回值:	0: 未改变 1: 参数有改变
*修改历史:20131211 v2.0.1 creat by ptb
**************************************************/
static int CheckNetworkInfo(void *_pSetReq)
{
	TGuiIpMsg *pstIpMsg = (TGuiIpMsg *)_pSetReq;
	TGuiIpMsg stGetIpMsg;
	int iRet = 0;

	if(pointer_valid_check(pstIpMsg))
	{
		return 0;
	}

	iRet = OnvifCmdGetip(&stGetIpMsg);
	if(TD_OK == iRet)
	{
		if(strcmp(pstIpMsg->m_strIp, stGetIpMsg.m_strIp)
		|| strcmp(pstIpMsg->m_strSubMsk, stGetIpMsg.m_strSubMsk)
		|| strcmp(pstIpMsg->m_strGetWay, stGetIpMsg.m_strGetWay)
		|| strcmp(pstIpMsg->m_strDNS, stGetIpMsg.m_strDNS))
		{
			return 1;
		}
	}
	
	return 0;
}

/**************************************************
*功能:		手机连接视频处理
*参数:		_iQuality:		视频质量
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_PhoneHandle(int _iQuality, int _iStreamType)
{
	TGuiVideoEncodeCnf stSetVideoEncode = {0};
	int iStreamType = _iStreamType;
	int iVideoSize = 0;
	int iFrameRate = 0;
	int iBitRate   = 0;
	int iChn = 0;
	int iRet = 0;

	if(iStreamType < 0 || iStreamType > 2)
	{
		td_printf(0, "%s %d: Error iStreamType[%d]", __FUNCTION__, __LINE__, iStreamType);
		return DCSS_UNEXPECTED;
	}

	if(2 == iStreamType)
	{
		//标清--副码流
		iChn = 2;
		iVideoSize = CIF;
	}
	else
	{
		//自动、高清--主码流
		iChn = 1;
		iVideoSize = VGA;
	}
	td_printf(0, "=======Danale_PhoneHandle iChn[%d] iVideoSize[%d]", iChn, iVideoSize);
	if(OnvifCmdGetVideoEncodeCnf(iChn, &stSetVideoEncode))
	{
		td_printf(0, "%s %d Shm GetVideoEncodeCnf is error!", __FUNCTION__, __LINE__);
		return DCSS_UNEXPECTED;
	}
	
	switch(_iQuality)
	{
		case 1:
			{
				iFrameRate = 10;
				iBitRate   = 128;
				break;
			}
		case 2:
			{
				iFrameRate = 10;
				iBitRate   = 256;
				break;
			}
		case 3:
			{
				iFrameRate = 15;
				iBitRate   = 256;
				break;
			}
		case 4:
			{
				iFrameRate = 15;
				iBitRate   = 512;
				break;
			}
		default:
		{
			iFrameRate = 10;
			iBitRate   = 256;
			break;
		}
	}

	if(stSetVideoEncode.m_iPicSize != iVideoSize
	|| stSetVideoEncode.m_iFrameRate != iFrameRate
	|| stSetVideoEncode.m_iBitRate != iBitRate)
	{
		stSetVideoEncode.m_iPicSize   = iVideoSize;
		stSetVideoEncode.m_iFrameRate = iFrameRate;
		stSetVideoEncode.m_iBitRate   = iBitRate;
		
		td_printf(0, "set m_iFrameRate[%d]", stSetVideoEncode.m_iFrameRate);
		td_printf(0, "set m_iBitRate[%d]", stSetVideoEncode.m_iBitRate);
		td_printf(0, "set m_iPicSize[%d]", stSetVideoEncode.m_iPicSize);
		
		iRet = OnvifCmdSetAVEncodeCnf(iChn, &stSetVideoEncode);
		td_printf(0, "%s %d Shm SetAVEncodeCnf ret[%d]!", __FUNCTION__, __LINE__, iRet);
		if(iRet != 0)iRet = DCSS_UNEXPECTED;
	}

	return iRet;
}

/**************************************************
*功能:		连接视频处理
*参数:		_phess:		请求句柄
				_pData: 		请求数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
static int Danale_Connect(void *_phess, char *_pData)
{
	struct CmdStartVideoReq *ptStartVideoReq = (struct CmdStartVideoReq*)_pData;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iAllUseCnt  = 0;
	int iStreamType = 0;
	int iViNum = 0;
	int iChn = 0;
	int i = 0;
	
	if(NULL == ptStartVideoReq || NULL == _phess || NULL == ptOnvifCtrl)
	{
		return DCSS_UNEXPECTED;
	}
	
	td_printf(0xff0000, "channel:%d, mediaChn:%d, vstrm:%d", ptStartVideoReq->channel, ptStartVideoReq->mediaChn, ptStartVideoReq->vstrm);

	//Stream type
	iStreamType = ptStartVideoReq->vstrm - 1;//vstrm:1-自动，高清；2-标清
	if(iStreamType < 0)iStreamType = 0;
	if(iStreamType > 1)iStreamType = 1;

	//video chn
	iViNum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	iChn = ptStartVideoReq->channel;
	if(iChn < 0)iChn = 0;
	if(iChn >= iViNum)iChn = iViNum-1;
	
	iAllUseCnt = ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tGuiStream.m_iOnvifCnt + ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tGuiStream.m_iDanaleCnt; 
	if(0 == iAllUseCnt)
	{
		OnvifCmdIframeReq(iViNum * iStreamType + iChn + 1);//强制I 帧
		OnvifCmdStreamConnect(iViNum * iStreamType + iChn + 1);
		ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tGuiStream.m_ptGuiShmData = (TGuiStreamChannel *)get_chn_shmaddr(iViNum * iStreamType + iChn);
		ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_iStat = CHANNEL_CONNECTED;
	}
	
	ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tGuiStream.m_iDanaleCnt++;

	for(i = 0; i < MAX_CONNECT_NUM; i++)
	{
		//td_printf(0, "===============iVideoEnable[%d]", g_tSession[i].iVideoEnable[iChn]);
		if(TD_FALSE != g_tSession[iChn][i].iVideoEnable && _phess == g_tSession[iChn][i].hsess)
		{
			return 0;
		}

		if(TD_FALSE == g_tSession[iChn][i].iVideoEnable && NULL == g_tSession[iChn][i].hsess)
		{
			g_tSession[iChn][i].hsess   = _phess;
			g_tSession[iChn][i].iVideoEnable = iStreamType + 1;
			g_tSession[iChn][i].iChn = iChn;
			g_iConNum++;
			td_printf(0, "i[%d] hsess[%p]", i, g_tSession[iChn][i].hsess);
			break;
		}
	}
	
	return 0;
}

/**************************************************
*功能:		开始视频
*参数:		_phess:		请求句柄
				_pData:		请求数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_StartVideo(void *_phess, char *_pData)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct CmdStartVideoReq *req = (struct CmdStartVideoReq*)_pData;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	
	if(NULL == req)
	{
		return DCSS_UNEXPECTED;
	}
	
	if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType 
	&& DEVTYPE_PHONE == req->devType)
	{
		//目前暂只支持IPC
		Danale_PhoneHandle(req->quality, req->vstrm);
	}
	
	return Danale_Connect(_phess, _pData);
}

//add by aaron
void* Danale_FindSession(void *_phess)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	if(NULL == _phess || NULL == ptOnvifCtrl)
	{
		return NULL;
	}

	int iViNum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	int i,j;
	
	for(i=0; i<iViNum; i++)
	{
		for(j=0; j<MAX_CONNECT_NUM; j++)
		{
			if(g_tSession[i][j].hsess == _phess)
			{
				return &g_tSession[i][j];
			}
		}
	}
	return NULL;
}

/**************************************************
*功能:		停止视频
*参数:		_phess:		请求句柄
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_StopVideo(void *_phess)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iStreamType = -1;
	int iAllUseCnt = 0;
	int iChn = -1;
	int iViNum = 0;
				
	if(NULL == _phess || NULL == ptOnvifCtrl)
	{
		return DCSS_UNEXPECTED;
	}

	iViNum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	
	Dana_Session * pSession = (Dana_Session*)Danale_FindSession(_phess);
	if(NULL == pSession || TD_FALSE == pSession->iVideoEnable)
	{
		return DCSS_UNEXPECTED;
	}
	
	iChn = pSession->iChn;
	iStreamType = pSession->iVideoEnable - 1;
	pSession->iVideoEnable = TD_FALSE;
	pSession->iAudioEnable = TD_FALSE;
	pSession->hsess        = NULL;
	
	ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tGuiStream.m_iDanaleCnt--;
	iAllUseCnt = ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tGuiStream.m_iOnvifCnt + ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tGuiStream.m_iDanaleCnt;
	td_printf(0, "===========iAllUseCnt[%d] iChn[%d]", iAllUseCnt, iChn);
	if(iAllUseCnt <= 0)
	{
		OnvifCmdStreamDisConnect(iViNum*iStreamType + iChn + 1);
		//ptOnvifCtrl->m_stRtspServer.m_tChanel[iChn][0].m_tGuiStream.m_ptGuiShmData = NULL;
		ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tGuiStream.m_iDanaleCnt = 0;
		ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_iStat = CHANNEL_UNCONNECTED;
		//清空SDP信息
		memset(&ptOnvifCtrl->m_stRtspServer.m_tChanel[iStreamType][iChn].m_tSdpInfo, 0, sizeof(TSDPInfo));
	}
	g_iConNum--;
	if(g_iConNum < 0)
	{
		g_iConNum = 0;
	}

	return 0;
}

/**************************************************
*功能:		开始音频
*参数:		_phess:		请求句柄
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_StartAudio(void *_phess)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	
	if(NULL == _phess)
	{
		return DCSS_UNEXPECTED;
	}

	Dana_Session* pSession = (Dana_Session*)Danale_FindSession(_phess);
	if(NULL == pSession || TD_FALSE == pSession->iVideoEnable)
	{
		return DCSS_UNEXPECTED;
	}

	pSession->iAudioEnable = TD_TRUE;
	return 0;
}

/**************************************************
*功能:		停止音频
*参数:		_phess:		请求句柄
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_StopAudio(void *_phess)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	
	if(NULL == _phess)
	{
		return DCSS_UNEXPECTED;
	}
	
	Dana_Session* pSession = (Dana_Session*)Danale_FindSession(_phess);
	if(NULL == pSession)
	{
		return DCSS_UNEXPECTED;
	}

	pSession->iAudioEnable = TD_FALSE;
	return 0;
}

/**************************************************
*功能:		PTZ控制
*参数:		_pData:		请求数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_PTZControl(char *_pData)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct CmdPtzCtrlReq *req = (struct CmdPtzCtrlReq *)_pData;
	TGuiPtzLocalCode stPTZInfo = {0};
	int iEnable = 0;
		
	//td_printf(0, "#############%s %d:chn[%d] para1[%d] para2[%d]", __FUNCTION__, __LINE__, req->channel, req->para1, req->para2);
	printf("################PTZ code[%d] para1[%d] para1[%d]\n", req->code, req->para1, req->para2);
	//demo v2.0.1 变倍+ 125 - 127 光圈和焦距都不发码
	//demo v2.4    变倍+ 123 - 121 光圈+ 119 - 117 焦距+125 - 127
	switch(req->code)
	{
		case DAP2P_PTZ_STOP:
			{
				td_printf(0, "%s %d:PTZ cmd is STOP", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_STOP;
				stPTZInfo.m_iParam[0] = 0;		//X速度
				stPTZInfo.m_iParam[1] = 0;		//Y速度
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_MOVE_UP:
			{
				td_printf(0, "%s %d:PTZ cmd is UP", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_UP;
				stPTZInfo.m_iParam[0] = 0;		//X速度(0-63)
				stPTZInfo.m_iParam[1] = 40;		//Y速度(0-63)
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_MOVE_DOWN:
			{
				td_printf(0, "%s %d:PTZ cmd is DOWN", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_DOWN;
				stPTZInfo.m_iParam[0] = 0;		//X速度
				stPTZInfo.m_iParam[1] = 40;		//Y速度
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_MOVE_LEFT:
			{
				td_printf(0, "%s %d:PTZ cmd is LEFT", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_LEFT;
				stPTZInfo.m_iParam[0] = 40;		//X速度
				stPTZInfo.m_iParam[1] = 0;		//Y速度
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_MOVE_RIGHT:
			{
				td_printf(0, "%s %d:PTZ cmd is RIGHT", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_RIGHT;
				stPTZInfo.m_iParam[0] = 40;		//X速度
				stPTZInfo.m_iParam[1] = 0;		//Y速度
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_IRIS_IN:
			{
				td_printf(0, "%s %d:PTZ cmd is IRIS -", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_IRISCUT;
				//速度是定值
				stPTZInfo.m_iParam[0] = 1;
				stPTZInfo.m_iParam[1] = 1;
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_IRIS_OUT:
			{
				td_printf(0, "%s %d:PTZ cmd is IRIS +", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_IRISADD;
				//速度是定值
				stPTZInfo.m_iParam[0] = 1;
				stPTZInfo.m_iParam[1] = 1;
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_FOCUS_ON:
			{
				td_printf(0, "%s %d:PTZ cmd is FOCUS -", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_FOCUSCUT;
				//速度是定值
				stPTZInfo.m_iParam[0] = 50;
				stPTZInfo.m_iParam[1] = 50;

				#if 0
				stPTZInfo.m_iChn = 1;
				OnvifCmdSetPTZ(&stPTZInfo);

				//stop
				stPTZInfo.m_iCode = PTZ_STOP;
				stPTZInfo.m_iParam[0] = 0;
				stPTZInfo.m_iParam[1] = 0;
				#endif
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_FOCUS_OUT:
			{
				td_printf(0, "%s %d:PTZ cmd is FOCUS +", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_FOCUSADD;
				//速度是定值
				stPTZInfo.m_iParam[0] = 50;
				stPTZInfo.m_iParam[1] = 50;

				#if 0
				stPTZInfo.m_iChn = 1;
				OnvifCmdSetPTZ(&stPTZInfo);

				//stop
				stPTZInfo.m_iCode = PTZ_STOP;
				stPTZInfo.m_iParam[0] = 0;
				stPTZInfo.m_iParam[1] = 0;
				#endif
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_ZOOM_IN:
			{
				td_printf(0, "%s %d:PTZ cmd is ZOOM -\n", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_ZOOMCUT;
				//变倍速度是定值
				stPTZInfo.m_iParam[0] = 1;
				stPTZInfo.m_iParam[1] = 1;

				#if 0
				stPTZInfo.m_iChn = 1;
				OnvifCmdSetPTZ(&stPTZInfo);
				
				//stop
				stPTZInfo.m_iCode = PTZ_STOP;
				stPTZInfo.m_iParam[0] = 0;
				stPTZInfo.m_iParam[1] = 0;
				#endif
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_ZOOM_OUT:
			{
				td_printf(0, "%s %d:PTZ cmd is ZOOM +\n", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_ZOOMADD;
				//速度是定值
				stPTZInfo.m_iParam[0] = 1;
				stPTZInfo.m_iParam[1] = 1;

				#if 0
				stPTZInfo.m_iChn = 1;
				OnvifCmdSetPTZ(&stPTZInfo);

				//stop
				stPTZInfo.m_iCode = PTZ_STOP;
				stPTZInfo.m_iParam[0] = 0;
				stPTZInfo.m_iParam[1] = 0;
				#endif
				iEnable = TD_TRUE;
				break;
			}
		case DAP2P_PTZ_AUTO_SCAN:
			{
				td_printf(0, "%s %d:PTZ cmd is AUTO_SCAN", __FUNCTION__, __LINE__);
				stPTZInfo.m_iCode = PTZ_AUTO_START;
				//速度是定值
				stPTZInfo.m_iParam[0] = 0;
				stPTZInfo.m_iParam[1] = 0;
				iEnable = TD_TRUE;
				break;
			}
		default:
			{
				td_printf(0, "%s %d:PTZ cmd is UNKNOW[%d]", __FUNCTION__, __LINE__, req->code);
				break;
			}
	}

	if(TD_TRUE == iEnable)
	{
		stPTZInfo.m_iChn = 1;
		OnvifCmdSetPTZ(&stPTZInfo);
	}
	
	return 0;
}

/**************************************************
*功能:		获取视频亮度等值
*参数:		_ptVideoColor:	获取结果
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_GetColor(struct dcs_video_color *_ptVideoColor)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct dcs_video_color *ptGetVideoColor = _ptVideoColor;
	VideoInCnf_EX tVideoCnf;
	int iChn = 0;
	int iRet = 0;

	if(NULL == ptGetVideoColor)
	{
		return -1;
	}
	
	memset(&tVideoCnf, 0, sizeof(VideoInCnf_EX));

	iChn = 1;
	OnvifCmdGetVideoInCnf(iChn, &tVideoCnf);
	if(0 == iRet)
	{
		ptGetVideoColor->brightness = tVideoCnf.m_iLum * 100 / 255;
		ptGetVideoColor->contrast   = tVideoCnf.m_iContrast * 100 / 255;
		ptGetVideoColor->saturation = tVideoCnf.m_iSaturation * 100 / 255;
		ptGetVideoColor->hue 	    = tVideoCnf.m_iHue * 100 / 255;
	}
	
	return 0;
}

/**************************************************
*功能:		设置视频亮度等值
*参数:		_pData:		请求数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_SetColor(char *_pData)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct CmdSetVideoColorReq *req = (struct CmdSetVideoColorReq*)_pData;
	VideoInCnf_EX tGetVideoCnf;
	VideoInCnf_EX tSetVideoCnf;
	int iChn = 0;
	int iRet = 0;

	if(NULL == req)
	{
		return DCSS_UNEXPECTED;
	}
	
	memset(&tGetVideoCnf, 0, sizeof(VideoInCnf_EX));
	memset(&tSetVideoCnf, 0, sizeof(VideoInCnf_EX));
		
	//获取亮度等值
	iChn = 1;
	OnvifCmdGetVideoInCnf(iChn, &tGetVideoCnf);
	if(0 == iRet)
	{
		tSetVideoCnf.m_iVideoMod = tGetVideoCnf.m_iVideoMod;
		tSetVideoCnf.m_iGrayFlg  = tGetVideoCnf.m_iGrayFlg;
	}
	else
	{
		tSetVideoCnf.m_iVideoMod = 1;		// 1 --PAL;2 -- NTSC
		tSetVideoCnf.m_iGrayFlg  = 0;		// 0 -- 彩色; 1 -- 灰白
	}
	
	tSetVideoCnf.m_iBenginTm   = 0;
	tSetVideoCnf.m_iEndTm	   = 24 * 3600 - 1;//23:59:59
	tSetVideoCnf.m_iLum		   = req->color.brightness * 255 / 100;
	tSetVideoCnf.m_iContrast   = req->color.contrast * 255 / 100;
	tSetVideoCnf.m_iSaturation = req->color.saturation * 255 / 100;
	tSetVideoCnf.m_iHue		   = req->color.hue * 255 / 100;
		
	OnvifCmdSetVideoInCnf(iChn, &tSetVideoCnf);

	return 0;
}

/**************************************************
*功能:		获取OSD信息
*参数:		_pOsdInfo:	获取结果
*返回值:	0: 成功, !0: 失败
*修改历史:20131212 v2.0.1 creat by hwx
**************************************************/
int Danale_GetOSDInfo(void *_pOsdInfo)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct DAOSDInfo *pOsdInfo = (struct DAOSDInfo *)_pOsdInfo;
	TGuiVideoOverlayCnf stOsdInfo = {0};
	int iChn = 1;//写死
	
	if(NULL == pOsdInfo)
	{
		return -1;
	}
	if(OnvifCmdGetOsd(iChn , &stOsdInfo))
	{
		td_printf(0, "%s %d:Get OSD information failed.\n", __FUNCTION__, __LINE__);
		return -1;
	}
	
	//时间日期
	pOsdInfo->NeedDate=(!stOsdInfo.m_iTimeOverlayEnable);
	if(0 == pOsdInfo->NeedDate)
	{
		if(stOsdInfo.m_tTimeLoc.m_iXp >= 0 && stOsdInfo.m_tTimeLoc.m_iXp <= OSD_MIDDLE_X && \
		   stOsdInfo.m_tTimeLoc.m_iYp >= 0 && stOsdInfo.m_tTimeLoc.m_iYp <= OSD_MIDDLE_Y)
			pOsdInfo->DataPos = leftup;//左上
		else if(stOsdInfo.m_tTimeLoc.m_iXp >= 0 && stOsdInfo.m_tTimeLoc.m_iXp <= OSD_MIDDLE_X && \
		   stOsdInfo.m_tTimeLoc.m_iYp > OSD_MIDDLE_Y && stOsdInfo.m_tTimeLoc.m_iYp <= OSD_MAX_Y)
			pOsdInfo->DataPos = leftdown;//左下
		else if(stOsdInfo.m_tTimeLoc.m_iXp > OSD_MIDDLE_X && stOsdInfo.m_tTimeLoc.m_iXp <= OSD_MAX_X && \
		   stOsdInfo.m_tTimeLoc.m_iYp >= 0 && stOsdInfo.m_tTimeLoc.m_iYp <= OSD_MIDDLE_Y)
			pOsdInfo->DataPos = rightup;//右上
		else pOsdInfo->DataPos = rightdown;//右下
	}
	else pOsdInfo->DataPos = 4;
	
	//通道名
	pOsdInfo->NeedDevName=(!stOsdInfo.m_iNameOverlayEnable);
	if(0 == pOsdInfo->NeedDevName)
	{
		if(stOsdInfo.m_tNameLoc.m_iXp >= 0 && stOsdInfo.m_tNameLoc.m_iXp <= OSD_MIDDLE_X && \
		   stOsdInfo.m_tNameLoc.m_iYp >= 0 && stOsdInfo.m_tNameLoc.m_iYp <= OSD_MIDDLE_Y)
				pOsdInfo->DevNamePos = leftup;
		else if(stOsdInfo.m_tNameLoc.m_iXp >= 0 && stOsdInfo.m_tNameLoc.m_iXp <= OSD_MIDDLE_X && \
		   stOsdInfo.m_tNameLoc.m_iYp >= OSD_MIDDLE_Y && stOsdInfo.m_tNameLoc.m_iYp <= OSD_MAX_Y)
				pOsdInfo->DevNamePos = leftdown;
		else if(stOsdInfo.m_tNameLoc.m_iXp >= OSD_MIDDLE_X && stOsdInfo.m_tNameLoc.m_iXp <= OSD_MAX_X && \
		   stOsdInfo.m_tNameLoc.m_iYp >= 0 && stOsdInfo.m_tNameLoc.m_iYp <= OSD_MIDDLE_Y)
				pOsdInfo->DevNamePos = rightup;
		else pOsdInfo->DevNamePos = rightdown;
	}
	else pOsdInfo->DevNamePos = 4;
	
	//附加字符
	if(0 == strlen(stOsdInfo.m_strText))
	{
		pOsdInfo->OtherPos = 4;
	}
	else if(48 < strlen(stOsdInfo.m_strText))
	{
		td_printf(0,"Text is too long.\n");
		pOsdInfo->OtherPos = 4;
		return -1;
	}
	else
	{
		strncpy(pOsdInfo->OtherInfo,stOsdInfo.m_strText,strlen(stOsdInfo.m_strText));
		if(stOsdInfo.m_tTextLoc.m_iXp >= 0 && stOsdInfo.m_tTextLoc.m_iXp <= OSD_MIDDLE_X && \
		   stOsdInfo.m_tTextLoc.m_iYp >= 0 && stOsdInfo.m_tTextLoc.m_iYp <= OSD_MIDDLE_Y)
			pOsdInfo->OtherPos = leftup;
		else if(stOsdInfo.m_tTextLoc.m_iXp >= 0 && stOsdInfo.m_tTextLoc.m_iXp <= OSD_MIDDLE_X && \
		   stOsdInfo.m_tTextLoc.m_iYp >= OSD_MIDDLE_Y && stOsdInfo.m_tTextLoc.m_iYp <= OSD_MAX_Y)
			pOsdInfo->OtherPos= leftdown;
		else if(stOsdInfo.m_tTextLoc.m_iXp >= OSD_MIDDLE_X && stOsdInfo.m_tTextLoc.m_iXp <= OSD_MAX_X && \
		   stOsdInfo.m_tTextLoc.m_iYp >= 0 && stOsdInfo.m_tTextLoc.m_iYp <= OSD_MIDDLE_Y)
			pOsdInfo->OtherPos = rightup;
		else pOsdInfo->OtherPos= rightdown;
	}

	return 0;
}

/**************************************************
*功能:		设置OSD信息
*参数:		_pData:	设置的参数指针
*返回值:	0: 成功, !0: 失败
*修改历史:20131213 v2.0 creat by hwx
**************************************************/
int Danale_SetOSDInfo(void *_ptSetOSD)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct DAOSDInfo *pddt = (struct DAOSDInfo *)_ptSetOSD;
	TGuiVideoOverlayCnf stOsdInfo = {0};
	int iChn = 1;//写死

	if(NULL == pddt)
	{
		return -1;
	}

	if(OnvifCmdGetOsd(iChn, &stOsdInfo))
	{
		td_printf(0,"OnvifCmdGetOsd ERROR!\n");
		return-1;
	}

	stOsdInfo.m_iTimeOverlayEnable = (int)(!pddt->NeedDate);
	stOsdInfo.m_iNameOverlayEnable = (int)(!pddt->NeedDevName);
	memset(stOsdInfo.m_strText , 0 , strlen(stOsdInfo.m_strText));
	strncpy(stOsdInfo.m_strText , pddt->OtherInfo , strlen(pddt->OtherInfo));

	//日期
	if(leftup == pddt->DataPos)
	{
		stOsdInfo.m_tTimeLoc.m_iXp = 0;
		stOsdInfo.m_tTimeLoc.m_iYp = 0;
	}
	else if(leftdown == pddt->DataPos)
	{
		stOsdInfo.m_tTimeLoc.m_iXp = 0;
		stOsdInfo.m_tTimeLoc.m_iYp = OSD_MAX_Y;
	}
	else if(rightup == pddt->DataPos)
	{
		stOsdInfo.m_tTimeLoc.m_iXp = OSD_MAX_X;
		stOsdInfo.m_tTimeLoc.m_iYp = 0;
	}
	else 
	{
		stOsdInfo.m_tTimeLoc.m_iXp = OSD_MAX_X;
		stOsdInfo.m_tTimeLoc.m_iYp = OSD_MAX_Y;
	}

	//通道名
	if(leftup == pddt->DevNamePos)
	{
		stOsdInfo.m_tNameLoc.m_iXp = 0;
		stOsdInfo.m_tNameLoc.m_iYp = 0;
	}
	else if(leftdown == pddt->DevNamePos)
	{
		stOsdInfo.m_tNameLoc.m_iXp = 0;
		stOsdInfo.m_tNameLoc.m_iYp = OSD_MAX_Y;
	}
	else if(rightup == pddt->DevNamePos)
	{
		stOsdInfo.m_tNameLoc.m_iXp = OSD_MAX_X;
		stOsdInfo.m_tNameLoc.m_iYp = 0;
	}
	else
	{
		stOsdInfo.m_tNameLoc.m_iXp = OSD_MAX_X;
		stOsdInfo.m_tNameLoc.m_iYp = OSD_MAX_Y;
	}

	//附加字符
	if(leftup == pddt->OtherPos)
	{
		stOsdInfo.m_tTextLoc.m_iXp = 0;
		stOsdInfo.m_tTextLoc.m_iYp = 0;
	}
	else if(leftdown == pddt->OtherPos)
	{
		stOsdInfo.m_tTextLoc.m_iXp = 0;
		stOsdInfo.m_tTextLoc.m_iYp = OSD_MAX_Y;
	}
	else if(rightup == pddt->OtherPos)
	{
		stOsdInfo.m_tTextLoc.m_iXp = OSD_MAX_X;
		stOsdInfo.m_tTextLoc.m_iYp = 0;
	}
	else
	{
		stOsdInfo.m_tTextLoc.m_iXp = OSD_MAX_X;
		stOsdInfo.m_tTextLoc.m_iYp = OSD_MAX_Y;
	}

	if(OnvifCmdSetOsd(iChn, &stOsdInfo))
	{
		td_printf(0,"OnvifCmdGetOsd ERROR!\n");
		return-1;
	}
	
	return 0;
}

/**************************************************
*功能:		设置视频翻转
*参数:		_pData:		请求数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_SetFlip(char *_pData)
{
	struct CmdSetFlipReq *req = (struct CmdSetFlipReq*)_pData;
	printf("SET_FLIP: %d\n", req->flips);

	if(NULL == req)
	{
		return DCSS_UNEXPECTED;
	}
	
	switch(req->flips)
	{
		case 0:
			{
				//正置
				break;
			}
		case 1:
			{
				//水平翻转
				OnvifCmdSetFlip(1, GUI_VIDEO_TURN_H);
				break;
			}
		case 2:
			{
				//垂直翻转
				OnvifCmdSetFlip(1, GUI_VIDEO_TURN_V);
				break;
			}
		case 3:
			{
				//倒置
				OnvifCmdSetFlip(1, GUI_VIDEO_TURN_R);
				break;
			}
		default:
			{
				break;
			}
	}
	
	return 0;
}

static int Danale_PicQualityLv(int _iPicQualityLv)
{
	int iVspQuality = 6;
	
	switch(_iPicQualityLv)
	{
		case  4:
		case  5:
		case  6: break;
		case  7: 
		case  8: iVspQuality = 5;break;
		case  9: 
		case 10: iVspQuality = 4;break;
		case 11: 
		case 12: iVspQuality = 3;break;
		case 13: 
		case 14: iVspQuality = 2;break;
		default: break;
	}
	
	return iVspQuality;
}

static int Danale_PicQualityLvToKer(int _iPicQualityLv)
{
	int iVspQuality = 6;
	
	switch(_iPicQualityLv)
	{
		case  1: 
		case  2: iVspQuality = 14;break;
		case  3: iVspQuality = 12;break;
		case  4: iVspQuality = 10;break;
		case  5: iVspQuality = 8;break;
		case  6: iVspQuality = 6;break;
		default: break;
	}
	
	return iVspQuality;
}


/**************************************************
*功能:		获取视频参数
*参数:		_pData:		请求数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131210 v2.0.1 modify by ptb
**************************************************/
int Danale_GetEncodeParam(void *_pData)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct dcs_video_param_new *ptVideoParam = (struct dcs_video_param_new *)_pData;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiVideoEncodeCnf stEncodeRsp;
	VideoInCnf_EX stVideoIn;
	int iVINum = 0;
	int iRet = 0;
	int i = 0;
	
	if(NULL == ptVideoParam && NULL == ptOnvifCtrl)
	{
		return -1;
	}

	iVINum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;

	OnvifCmdGetVideoInCnf(1, &stVideoIn);

	for(i = 0; i < MAX_STREAM; i++)
	{
		memset(&stEncodeRsp, 0, sizeof(TGuiVideoEncodeCnf));
		iRet = OnvifCmdGetVideoEncodeCnf(i * iVINum + 1, &stEncodeRsp);
		if(TD_OK != iRet)
		{
			return iRet;
		}

		ptVideoParam[i].brmode  = i;
		ptVideoParam[i].fps     = stEncodeRsp.m_iFrameRate;
		ptVideoParam[i].gop     = stEncodeRsp.m_iIFrameRate;
		ptVideoParam[i].kbps    = stEncodeRsp.m_iBitRate;
		ptVideoParam[i].max_fps = (stVideoIn.m_iVideoMod==0 ? 25 : 30);
		ptVideoParam[i].quality = Danale_PicQualityLv(stEncodeRsp.m_iPicQualityLv);
		// 3-1280*720 
		ptVideoParam[i].res     = 2;
		ptVideoParam[i].save    = stEncodeRsp.m_iBitStreamType - 1;
		
		td_printf(0, "==================");
		td_printf(0, "GetEncodeParam i[%d]", i);
		td_printf(0, "GetEncodeParam fps[%d]", ptVideoParam[i].fps);
		td_printf(0, "GetEncodeParam gop[%d]", ptVideoParam[i].gop);
		td_printf(0, "GetEncodeParam kbps[%d]", ptVideoParam[i].kbps);
		td_printf(0, "GetEncodeParam max_fps[%d]", ptVideoParam[i].max_fps);
		td_printf(0, "GetEncodeParam save[%d]", ptVideoParam[i].save);
		td_printf(0, "GetEncodeParam quality[%d]", ptVideoParam[i].quality);
		td_printf(0, "==================");
	}

	return iRet;
}

/**************************************************
*功能:		设置视频参数
*参数:		_pData:		请求数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131210 v2.0.1 modify by ptb
**************************************************/
int Danale_SetEncodeParam(void *_pData)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct CmdSetVideoParameterReq *pReq = (struct CmdSetVideoParameterReq*)_pData;

	if(NULL == pReq)
	{
		return DCSS_UNEXPECTED;
	}

	//int iChn = pReq->chnvid.channel;
	int iChn = (pReq->chnvid.video >> 28) & 0xF;
	td_printf(0xff0000, "Danale_SetEncodeParam iChn:%d", iChn);
	TGuiVideoEncodeCnf stEncode;
	OnvifCmdGetVideoEncodeCnf(iChn, &stEncode);

	stEncode.m_iBitRate = pReq->video_param.kbps;
	stEncode.m_iBitStreamType = pReq->video_param.save;
	stEncode.m_iFrameRate = pReq->video_param.fps;
	stEncode.m_iIFrameRate = pReq->video_param.gop;
	stEncode.m_iPicQualityLv = Danale_PicQualityLvToKer(pReq->video_param.quality);
	//stEncode.m_iPicSize = pReq->video_param.res;
	td_printf(0, "==================");
	td_printf(0, "stEncode.m_iBitRate:%d", stEncode.m_iBitRate);
	td_printf(0, "stEncode.m_iBitStreamType:%d", stEncode.m_iBitStreamType);
	td_printf(0, "stEncode.m_iFrameRate:%d", stEncode.m_iFrameRate);
	td_printf(0, "stEncode.m_iIFrameRate:%d", stEncode.m_iIFrameRate);
	td_printf(0, "stEncode.m_iPicQualityLv:%d", stEncode.m_iPicQualityLv);
	td_printf(0, "==================");
	OnvifCmdSetAVEncodeCnf(iChn, &stEncode);

	VideoInCnf_EX stVideoIn;
	OnvifCmdGetVideoInCnf(iChn, &stVideoIn);
	stVideoIn.m_iVideoMod = pReq->video_param.norm;
	OnvifCmdSetVideoInCnf(iChn, &stVideoIn);
	return 0;
}

/**************************************************
*功能:		发送主码流视频
*参数:		_iKeyFrame:		是否是I帧
				_pcData:			视频数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_SendMainStream(int _iKeyFrame, char *_pcData, int _iChn)
{
	//td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	S_FrameHeader tFrameHeader = {0};
	int iKeyFrame = _iKeyFrame;
	char *pcData  = _pcData;
	int iHAudioLen = 4;
	int media_type = 0;
	int chno = 0;
	int iPos = 0;
	int i = 0;
	int j = 0;

	if(NULL == pcData || iKeyFrame < 0 || iKeyFrame > 1)
	{
		return -1;
	}
	#if 0
	if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		//IPC不处理主码流
		return TD_OK;
	}
	#endif
	memcpy(&tFrameHeader, pcData, sizeof(S_FrameHeader));
	//td_printf(0, "u32FrameID[%x] u32FrameNO[%d]", tFrameHeader.u32FrameID, tFrameHeader.u32FrameNO);
	
	for(i = 0; i < MAX_CONNECT_NUM; i++)
	{
		void * phsess = g_tSession[_iChn][i].hsess;
		if(!phsess)continue;
			//视频
			if(STREAM_MAIN == g_tSession[_iChn][i].iVideoEnable)
			{
				chno = DAP2P_LIVE_VIDEO_CHNO;
				media_type = MEDIATYPE_VIDEO_H264;
			DAP2pSessSendMediaFrame(phsess, chno, media_type, tFrameHeader.u32TimeStamp, iKeyFrame, (BYTE *)(_pcData + sizeof(S_FrameHeader)), tFrameHeader.u32VStreamLen, 0);
				iPos += tFrameHeader.u32VStreamLen + sizeof(S_FrameHeader);
			
				//音频
				if(TD_TRUE == g_tSession[_iChn][i].iAudioEnable)
				{
					chno = DAP2P_LIVE_AUDIO_CHNO;
					media_type = MEDIATYPE_AUDIO_G711A;
				
					for(j = 0; j < tFrameHeader.u16AFrames; j++)
					{
					DAP2pSessSendMediaFrame(phsess, chno, media_type, tFrameHeader.u32TimeStamp, iKeyFrame, (BYTE *)(_pcData + iPos + iHAudioLen), tFrameHeader.u16AFrameSize - iHAudioLen, 0);
						iPos += tFrameHeader.u16AFrameSize;
					}
				}
			}
		iPos = 0;
	}

	return 0;
}

/**************************************************
*功能:		发送副码流视频
*参数:		_iKeyFrame:		是否是I帧
				_pcData:			视频数据
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_SendSubStream(int _iKeyFrame, char *_pcData, int _iChn)
{
	S_FrameHeader tFrameHeader = {0};
	int iKeyFrame = _iKeyFrame;
	char *pcData  = _pcData;
	int iHAudioLen = 4;
	int media_type = 0;
	int chno = 0;
	int iPos = 0;
	int i = 0;
	int j = 0;
	
	if(NULL == pcData || iKeyFrame < 0 || iKeyFrame > 1)
	{
		return -1;
	}
		
	memcpy(&tFrameHeader, pcData, sizeof(S_FrameHeader));
	//td_printf(0, "%s %d:Start! u32FrameID[%x]", __FUNCTION__, __LINE__, tFrameHeader.u32FrameID);

	for(i = 0; i < MAX_CONNECT_NUM; i++)
	{
		void * phsess = g_tSession[_iChn][i].hsess;
		if(!phsess)continue;
			//视频
			if(g_tSession[_iChn][i].iVideoEnable > STREAM_MAIN)
			{
				//td_printf(0, "Send sub stream!");
				chno = DAP2P_LIVE_VIDEO_CHNO;
				media_type = MEDIATYPE_VIDEO_H264;
			DAP2pSessSendMediaFrame(phsess, chno, media_type, tFrameHeader.u32TimeStamp, iKeyFrame, (BYTE *)(_pcData + sizeof(S_FrameHeader)), tFrameHeader.u32VStreamLen, 0);
				iPos += tFrameHeader.u32VStreamLen + sizeof(S_FrameHeader);

				//音频
				if(TD_TRUE == g_tSession[_iChn][i].iAudioEnable)
				{
					chno = DAP2P_LIVE_AUDIO_CHNO;
					media_type = MEDIATYPE_AUDIO_G711A;
				
					for(j = 0; j < tFrameHeader.u16AFrames; j++)
					{
					DAP2pSessSendMediaFrame(phsess, chno, media_type, tFrameHeader.u32TimeStamp, iKeyFrame, (BYTE *)(_pcData + iPos + iHAudioLen), tFrameHeader.u16AFrameSize - iHAudioLen, 0);
						iPos += tFrameHeader.u16AFrameSize;
					}
				}
			}
		iPos = 0;
	}
	return 0;
}

/**************************************************
*功能:		基本信息初始化
*参数:		无
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_BaseInfoInit(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = _pThis;
	struct CmdGetBaseInfo pDev;
	char *pTmp = NULL;
	int iRet = 0;

	Strcpy(pDev.daID, ptOnvifCtrl->m_stDanaleInfo.m_cPuId, 20);
	Strcpy(pDev.versionApi, "version-2.0.1", 20);
	Strcpy(pDev.mainuser, "danale", 20);
	pTmp = ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->SerialNumber;
	Strcpy(pDev.sSn, pTmp + 6, 20);
	Strcpy(pDev.mac, ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces->Info->HwAddress, 32);
	Strcpy(pDev.name, ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Manufacturer, 20);
	Strcpy(pDev.version, DANALE_VERSION, 20);
	pDev.type  = ptOnvifCtrl->m_stNvsInfo.m_iDeviceType;
	pDev.chnum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	pDev.AllSize  = 0;
	pDev.FreeSize = 0;
	#if 0
	td_printf(0, "===========================");
	td_printf(0, "%s chnum[%d]", __FUNCTION__, pDev.chnum);
	td_printf(0, "%s sSn[%s]", __FUNCTION__, pDev.sSn);
	td_printf(0, "%s name[%s]", __FUNCTION__, pDev.name);
	td_printf(0, "%s version[%s]", __FUNCTION__, pDev.version);
	td_printf(0, "===========================");
	#endif
	iRet = BaseInfoCalleeInitialize(&pDev);
	return iRet;
}

/**************************************************
*功能:		获取PuId当成DanaID
*参数:		无
*返回值:	0: 成功, !0: 失败
*修改历史:20131017 v2.0.1 creat by ptb
**************************************************/
int Danale_GetPuId()
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	dictionary *pDic = NULL; 
	char *pTmp = NULL;

	pDic = iniparser_load(CONFIG_SERVER_PATH);
	if(pointer_valid_check(pDic))
	{
		td_printf(0, "%s %d LoadProfile config_server.ini error!\n", __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
	
	pTmp = (char *)iniparser_getstring(pDic, "commoninfo:deviceid", "UNKNOWDEVICE");
	if(pointer_valid_check(pTmp))
	{
		return TD_ERROR;
	}

	if(strlen(pTmp))
	{
		strncpy(ptOnvifCtrl->m_stDanaleInfo.m_cPuId, pTmp, 20);
		td_printf(0, "Get danale id[%s]", ptOnvifCtrl->m_stDanaleInfo.m_cPuId);
	}

	return 0;
}

/**************************************************
*功能:		恢复默认或系统重启
*参数:		_pReq	:	请求参数
*返回值:	0: 成功, !0: 失败
*修改历史:20131211 v2.0.1 creat by ptb
**************************************************/
int Danale_RebootOrRestore(void *_pReq)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct DAInitSet *req = (struct DAInitSet *)_pReq;
	
	if(req->Type)
	{
		if(OnvifCmdRestore() != TD_OK)return DCSS_UNEXPECTED;
	}
	else
	{
		if(OnvifCmdRebootReq() != TD_OK)return DCSS_UNEXPECTED;
	}

	return 0;
}

/**************************************************
*功能:		获取网络参数
*参数:		_pGetInfo	:	获取参数
*返回值:	0: 成功, !0: 失败
*修改历史:20131211 v2.0.1 creat by ptb
**************************************************/
int Danale_GetNetworkInfo(void *_pGetInfo)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	struct DANetInfo *ptNetInfo = (struct DANetInfo *)_pGetInfo;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiIpMsg stIpMsg;
	int iRet = 0;

	if(pointer_valid_check(ptNetInfo))
	{
		return TD_ERROR;
	}

	iRet = OnvifCmdGetip(&stIpMsg);
	if(TD_OK == iRet)
	{
		ptNetInfo->IpType  = (stIpMsg.m_iGetIpMethod == 1) ? 0 : 1;
		ptNetInfo->DnsType = stIpMsg.m_blIsHandSetDns;
		Strcpy(ptNetInfo->cIp, stIpMsg.m_strIp, 16);
		Strcpy(ptNetInfo->cNetMask, stIpMsg.m_strSubMsk, 16);
		Strcpy(ptNetInfo->cGateway, stIpMsg.m_strGetWay, 16);
		Strcpy(ptNetInfo->cMDns, stIpMsg.m_strDNS, 16);
		Strcpy(ptNetInfo->cSDns, stIpMsg.m_strSecDNS, 16);
		ptNetInfo->chttpPort = ptOnvifCtrl->m_stNvsInfo.m_iLWebPort;
	}
	
	#ifdef TD_DEBUG
	td_printf(0, "==============================");
	td_printf(0, "%s iRet[%d]", __FUNCTION__, iRet);
	td_printf(0, "%s IpType[%d]", __FUNCTION__, ptNetInfo->IpType);
	td_printf(0, "%s DnsType[%d]", __FUNCTION__, ptNetInfo->DnsType);
	td_printf(0, "%s cIp[%s]", __FUNCTION__, ptNetInfo->cIp);
	td_printf(0, "%s cNetMask[%s]", __FUNCTION__, ptNetInfo->cNetMask);
	td_printf(0, "%s cGateway[%s]", __FUNCTION__, ptNetInfo->cGateway);
	td_printf(0, "%s cMDns[%s]", __FUNCTION__, ptNetInfo->cMDns);
	td_printf(0, "%s cSDns[%s]", __FUNCTION__, ptNetInfo->cSDns);
	td_printf(0, "%s chttpPort[%d]", __FUNCTION__, ptNetInfo->chttpPort);
	td_printf(0, "==============================");
	#endif
	return iRet;
}



/**************************************************
*功能:		获取网络参数
*参数:		_pReq	:	请求参数
*返回值:	0: 成功, !0: 失败
*修改历史:20131211 v2.0.1 creat by ptb
**************************************************/
int Danale_SetNetworkInfo(void *_pReq, int _iFlag)
{
	td_printf(0, "%s %d:Start!", __FUNCTION__, __LINE__);
	TGuiIpMsg stIpMsg;
	
	if(_iFlag)
	{
		struct DANetInfo *ptSetNetInfo = (struct DANetInfo *)_pReq;
		if(pointer_valid_check(ptSetNetInfo))
		{
			return DCSS_UNEXPECTED;
		}

		stIpMsg.m_iGetIpMethod = (ptSetNetInfo->IpType == 1) ? 0 : 1;
		stIpMsg.m_blIsHandSetDns = ptSetNetInfo->DnsType;
		Strcpy(stIpMsg.m_strIp, ptSetNetInfo->cIp, 32);
		Strcpy(stIpMsg.m_strSubMsk, ptSetNetInfo->cNetMask, 32);
		Strcpy(stIpMsg.m_strGetWay, ptSetNetInfo->cGateway, 32);
		Strcpy(stIpMsg.m_strDNS, ptSetNetInfo->cMDns, 32);
		Strcpy(stIpMsg.m_strSecDNS, ptSetNetInfo->cSDns, 32);
	}
	else
	{
		struct NetworkInfo *ptSetNetInfoEx = (struct NetworkInfo *)_pReq;
		if(pointer_valid_check(ptSetNetInfoEx))
		{
			return DCSS_UNEXPECTED;
		}

		stIpMsg.m_iGetIpMethod = (ptSetNetInfoEx->bDhcp == 1) ? 0 : 1;
		stIpMsg.m_blIsHandSetDns = 1;
		Strcpy(stIpMsg.m_strIp, ptSetNetInfoEx->cIp, 32);
		Strcpy(stIpMsg.m_strSubMsk, ptSetNetInfoEx->cNetMask, 32);
		Strcpy(stIpMsg.m_strGetWay, ptSetNetInfoEx->cGateway, 32);
		Strcpy(stIpMsg.m_strDNS, ptSetNetInfoEx->cMDns, 32);
		Strcpy(stIpMsg.m_strSecDNS, ptSetNetInfoEx->cSDns, 32);
	}
	
	if(0 == CheckNetworkInfo(&stIpMsg))
	{
		return DCSS_UNEXPECTED;
	}
	
	if(OnvifCmdSetip(&stIpMsg) != TD_OK)return DCSS_UNEXPECTED;
	
	return 0;
}

int Danale_GetTime(void *_pReq)
{
	struct DATimeInfo *pReq = (struct DATimeInfo *)_pReq;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == pReq || NULL == ptOnvifCtrl)
	{
		return -1;
	}

	if(ptOnvifCtrl->m_stNvsInfo.m_iTimeZone >= 0)
	{
		sprintf(pReq->gmtInfo, "GMT%02d:00", ptOnvifCtrl->m_stNvsInfo.m_iTimeZone);
	}
	else
	{
		sprintf(pReq->gmtInfo, "GMT%03d:00", ptOnvifCtrl->m_stNvsInfo.m_iTimeZone);
	}

	struct dcs_datetime now_date;
	struct dcs_datetime set_date;

	time_t tTime;
	struct tm stm;
	time(&tTime);
	localtime_r(&tTime, &stm);

	now_date.year = set_date.year = stm.tm_year + 1900;
	now_date.mon = set_date.mon = stm.tm_mon + 1;
	now_date.mday = set_date.mday = stm.tm_mday;
	now_date.hour = set_date.hour = stm.tm_hour;
	now_date.min = set_date.min = stm.tm_min;
	now_date.sec = set_date.sec = stm.tm_sec;
	now_date.dummy = set_date.dummy = 0;

	pReq->nowTime = now_date;
	pReq->timeMode = 1;
	pReq->timeArea = 3;
	pReq->SetTime = set_date;
	pReq->Interval = 1;

	OnvifCmdGetNTP(pReq->server, sizeof(pReq->server));
	return 0;
}

int Danale_SetTime(void *_pReq)
{
	struct DATimeInfo *pReq = (struct DATimeInfo *)_pReq;
	if(NULL == pReq)
	{
		return DCSS_UNEXPECTED;
	}
	
	if(pReq->timeMode == 2 || pReq->timeMode == 3)
	{
		TGuiSetSysTimeReq stTime;
		stTime.m_iYear = pReq->SetTime.year;
		stTime.m_iMon = pReq->SetTime.mon;
		stTime.m_iDay = pReq->SetTime.mday;
		stTime.m_iHour = pReq->SetTime.hour;
		stTime.m_iMin = pReq->SetTime.min;
		stTime.m_iSec = pReq->SetTime.sec;
		stTime.m_iTimeZone = -1;
		OnvifCmdSetTime(&stTime);

		OnvifCmdSetNTP("");//清空ntp
	}
	else if(pReq->timeMode == 4)
	{
		OnvifCmdSetNTP(pReq->server);
	}
	return 0;
}

char* Danale_GetPsw()
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	if(NULL == ptOnvifCtrl)
	{
		return NULL;
	}

	return GetProStr(ptOnvifCtrl->m_ptOnvifDict, "danale", 0, "password", 0, "admin");
}

int Danale_SetPsw(void* _pData)
{
	char *newPswd = (char*)_pData;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iRet = 0;

	if(NULL == newPswd || NULL == ptOnvifCtrl)
	{	
		//td_printf(0, "Danale_SetPsw NULL");
		return DCSS_UNEXPECTED;
	}
	
	iRet = SetProStr(ptOnvifCtrl->m_ptOnvifDict, "danale", 0, "password", 0, newPswd, 0, CFG_REPLACE);
	//td_printf(0xff0000, "Danale_SetPsw %s iRet = %d", newPswd, iRet);
	if(0 != iRet)
	{
		return DCSS_UNEXPECTED;
	}

	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = 1;
	return 0;
}

int Danale_snapshot(int _iChn)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	if(NULL == ptOnvifCtrl)return -1;

	int iViNum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	if(_iChn < 0)_iChn = 0;
	if(_iChn >= iViNum)_iChn = iViNum - 1;
	OnvifCmdGetSnapshot(_iChn + 1);
	return 0;
}

