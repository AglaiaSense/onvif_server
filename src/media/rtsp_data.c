/*
 *	file: media.c
 *	description: media process
 *	history: dongliqiang created at 20130228
 */
 
#include <stdio.h>
#include <string.h> 
#include <unistd.h>
#include "onvif_shm_cmd.h"
#include "rtsp_data.h"
#include "rtsp_cmd.h"//for session stat enum by qiaohaijun @2013.2.12
#include "media_h264.h"
#include "media_audio.h"
#include "debug.h"
#include "media_mjpeg.h"

#ifdef WITH_DANALE
#include "dn_interface.h"
#endif

#if 0
static int WriteFrameToFile(unsigned char*_data, int _iLen)
{
	char *cFilePath = "/dbg/test.dat";
	static FILE *pFile = NULL;
	int iRet = 0;

	if(access(cFilePath, W_OK))
	{
		if(pFile != NULL)
		{
			fflush(pFile);
			fclose(pFile);
		}
		pFile = fopen(cFilePath, "a+");
	}
	else
	{
		if(NULL == pFile)
		{
			pFile = fopen(cFilePath, "a+");
		}
	}
	
	if(NULL == pFile)
	{
		td_printf(0, "==========warnning:%s %d %s==========\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	
	iRet = fwrite(_data, _iLen, 1, pFile);
	if(1 != iRet)
	{
		td_printf(0, "==========%s %d %s: warnning:file write error!==========\n", __FILE__, __LINE__, __FUNCTION__);
		goto ERR;
	}
ERR:
	fflush(pFile);
//	fclose(pFile);
	return iRet;
}
#endif

int GetFrameInfo(TChannel *_ptChannel, stFrameInfoPlate *_ptFrameInfo)
{
	TChannel *ptChannel = _ptChannel;
	TGuiStreamChannel *ptGuiShmStream = NULL;
	stFrameInfoPlate *ptFrameInfo     = _ptFrameInfo;
	stFrameInfoPlate tTmpFrameInfo    = {0};

	if(NULL == ptChannel || NULL == ptFrameInfo)
	{
		return TD_ERROR;
	}

	ptGuiShmStream = ptChannel->m_tGuiStream.m_ptGuiShmData;

	if(ptGuiShmStream->m_iBase < 0 || ptGuiShmStream->m_iTop < 0)
	{
		td_printf(0, "==========%s %d %s warnning: m_iBase[%d] or m_iTop[%d]!==========\n", __FILE__, __LINE__, __FUNCTION__, ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase, ptChannel->m_tGuiStream.m_ptGuiShmData->m_iTop);
		ptGuiShmStream->m_iBase = 0;
		ptGuiShmStream->m_iTop  = 0;
		return TD_ERROR;
	}

	if(ptGuiShmStream->m_iTop > ptGuiShmStream->m_iBase)
	{
		memcpy(ptFrameInfo, ptGuiShmStream->m_cBuf + ptGuiShmStream->m_iBase, sizeof(stFrameInfoPlate));
		ptGuiShmStream->m_iBase += sizeof(stFrameInfoPlate);
	}
	else if(ptGuiShmStream->m_iTop < ptGuiShmStream->m_iBase)
	{
		if(ptGuiShmStream->m_iBase + sizeof(stFrameInfoPlate) <= ptGuiShmStream->m_iSize)
		{
			memcpy(&tTmpFrameInfo, ptGuiShmStream->m_cBuf + ptGuiShmStream->m_iBase, sizeof(stFrameInfoPlate));
			if(tTmpFrameInfo.cFrameType == 0xff)
			{
				// base开头到文件结尾的地方为无效填充数据
				memcpy(ptFrameInfo, ptGuiShmStream->m_cBuf, sizeof(stFrameInfoPlate));
				ptGuiShmStream->m_iBase = sizeof(stFrameInfoPlate);
			}
			else				
			{
				// 从base开头到文件结尾存在可用数据
				memcpy(ptFrameInfo, ptGuiShmStream->m_cBuf + ptGuiShmStream->m_iBase, sizeof(stFrameInfoPlate));
				ptGuiShmStream->m_iBase += sizeof(stFrameInfoPlate);
			}
		}
		else
		{
			// 假设从base到缓冲末尾不可以写下帧头数据,直接从开头拷贝
			memcpy(ptFrameInfo, ptGuiShmStream->m_cBuf, sizeof(stFrameInfoPlate));
			ptGuiShmStream->m_iBase = sizeof(stFrameInfoPlate);
		}
	}
	else
	{
		//td_printf(0, "%s %d %s top is equal base!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}

	return TD_OK;
}

int GetFrameHeader(char *_pcData, int _iLen, S_FrameHeader *_ptFrameHeader)
{
	S_FrameHeader *ptFrameHeader = _ptFrameHeader;
	char *pcData = _pcData;

	if(NULL == _pcData || NULL == ptFrameHeader)
	{
		return TD_ERROR;
	}

	if(_iLen < sizeof(S_FrameHeader))
	{
		return TD_ERROR;
	}

	memcpy(ptFrameHeader, pcData, sizeof(S_FrameHeader));
#if 0
	td_printf(0,"==========%s==========\n", __FUNCTION__);
	td_printf(0,"u32FrameID:%x\n",	 	ptFrameHeader->u32FrameID); 
	td_printf(0,"u32FrameSize:%u\n",	ptFrameHeader->u32FrameSize);	 
	td_printf(0,"u8FrameType:%u\n",  	ptFrameHeader->u8FrameType); 
	td_printf(0,"u32FrameNO:%u\n",	 	ptFrameHeader->u32FrameNO); 
	td_printf(0,"u32TimeStamp:%u\n",	ptFrameHeader->u32TimeStamp); 
	td_printf(0,"u32VStreamLen:%u\n",	ptFrameHeader->u32VStreamLen); 
	td_printf(0,"u16AFrames:%u\n",	 	ptFrameHeader->u16AFrames); 
	td_printf(0,"u16AFrameSize:%u\n",	ptFrameHeader->u16AFrameSize); 
	td_printf(0,"u8ACoder:%u\n",		ptFrameHeader->u8ACoder);
	td_printf(0,"====================\n\n");
#endif

	return TD_OK;		
}

int RtspDataProcess(TRtspServer *_ptServer)
{
	TRtspServer *ptServer = _ptServer;
	int i = 0;
	int j = 0;

	if(NULL == ptServer)
	{
		return TD_ERROR;
	}
	
	for(i = 0; i < MAX_STREAM; i++)
	{
		for(j = 0; j < MAX_CHANNEL_NUM; j++)
		{
			ProcessChanData(&ptServer->m_tChanel[i][j], i, j);
		}
	}

	return TD_OK;
}

int ProcessChanData(TChannel *_ptChannel, int _iStreamType, int _iChn)
{
	TChannel *ptChannel = _ptChannel;
	stFrameInfoPlate tFrameInfo = {0};
	int iKeyFrame = 0;
	int iRet = 0;

	if(NULL == ptChannel)
	{
		return TD_ERROR;
	}

	if(ptChannel->m_iStat != CHANNEL_CONNECTED)
	{
		iRet = IDLE;
		goto EXIT;
	}

	if(GetFrameInfo(ptChannel, &tFrameInfo))
	{
		//td_printf(0, "warnning: %s %d %s GetFrameInfo error!\n", __FILE__, __LINE__, __FUNCTION__);
		iRet = IDLE;
		goto EXIT;
	}
	
#ifdef WITH_DANALE
	if(ptChannel->m_tGuiStream.m_iDanaleCnt)
	{
		//td_printf(0, "========Process shm addr[%p]", ptChannel->m_tGuiStream.m_ptGuiShmData);
		iKeyFrame = (tFrameInfo.cFrameType == 0) ? 1 : 0;
		if(1 == _iStreamType)
		{
			Danale_SendSubStream(iKeyFrame, ptChannel->m_tGuiStream.m_ptGuiShmData->m_cBuf + ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase, _iChn);
		}
		else
		{
			Danale_SendMainStream(iKeyFrame, ptChannel->m_tGuiStream.m_ptGuiShmData->m_cBuf + ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase, _iChn);
		}
	}
#endif
	//print_frameinfo(&tFrameInfo);
	if(ptChannel->m_tGuiStream.m_iOnvifCnt)
	{
		ProcessAVData(ptChannel, ptChannel->m_tGuiStream.m_ptGuiShmData->m_cBuf + ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase, tFrameInfo.iDataLength);
	}
	ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase += tFrameInfo.iDataLength;

EXIT:
	return TD_OK;
}

int ProcessAVData(TChannel *_ptChannel, char *_pcData, int _iLen)
{
	TRtpHeader *ptAudioRtpHeader = NULL;
	TRtpHeader *ptVideoRtpHeader = NULL;
	TRtpJpegHeader_t *ptJpegRtpHeader = NULL;	
	TRtspSession *ptSession      = NULL;
	TRtspSession **pptSession    = NULL;
	TRtspServer *ptServer = NULL;
	DListNode *ptNode     = NULL;
	TChannel *ptChannel   = _ptChannel;
	char *pcData          = _pcData;
	int iVideoEncodeType  = -1;
	int iListSize         = 0;
	int iPos = 0;
	int iRet = 0;
	int i    = 0;

	if(NULL == ptChannel || NULL == pcData)
	{
		return TD_ERROR;
	}

	ptServer = (TRtspServer *)ptChannel->m_pServer;
	
	if(_iLen > MAX_FRAME_SIZE || _iLen <= 0)
	{
		td_printf(0, "warnning: %s %d %s _iLen[%d] is invalid!\n", __FILE__, __LINE__, __FUNCTION__, _iLen);
		return TD_ERROR;
	}
	
	if(GetFrameHeader(pcData, _iLen, &ptChannel->m_tFrameHeader))
	{
		td_printf(0, "%s %d %s inner_GetFrameHeader error!\n", __FILE__, __LINE__, __FUNCTION__);
	}

//	WriteFrameToFile(pcData + sizeof(S_FrameHeader), ptChannel->m_tFrameHeader.u32VStreamLen);

	adt_dlist_lock(ptServer->m_ptDListSession);
	iListSize = adt_dlist_size(ptServer->m_ptDListSession);
	ptNode = adt_dlist_head(ptServer->m_ptDListSession);
	for(i = 0; i < iListSize; i++)
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
			td_printf(0, "==========%s %d %s ptSession is NULL!==========\n", __FILE__, __LINE__, __FUNCTION__);
			iRet = TD_ERROR;
			goto EXIT;
		}

		ptSession = *pptSession;
		iRet = MutexLockTryLock(&ptSession->m_tLock);
		if(iRet != TD_OK)
		{
			continue;
		}
		
		if(ptSession->m_iChan == ptChannel->m_iChan && ptSession->m_iStream == ptChannel->m_iStream)
		{
			if(ptSession->m_iStat == SESSION_PLAY)
			{
				//每次置零保证发送给所有同一通道的连接的数据一致
				iPos = 0;
				iPos += sizeof(S_FrameHeader);

				ptAudioRtpHeader = &(ptSession->m_tRtpInfo.m_tARtpHeader);
				ptVideoRtpHeader = &(ptSession->m_tRtpInfo.m_tVRtpHeader);
				ptJpegRtpHeader = &(ptSession->m_tRtpInfo.m_tJpegHeader);
				iVideoEncodeType = -1;
		
				if(*(pcData + iPos) == 0xff)
				{
					iVideoEncodeType = GUI_MJPG;
				}
				else
				{
					iVideoEncodeType = GUI_H264;
				}
				
				if(ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType != ptChannel->m_tFrameHeader.u8ACoder\
				|| ptChannel->m_tChanParam.m_tVEncCfg.m_iEncodeType != iVideoEncodeType)
				{
					td_printf(0, "audio encode type changed!!!");
					ptChannel->m_tChanParam.m_tVEncCfg.m_iAudioEncodeType = ptChannel->m_tFrameHeader.u8ACoder;
					ptChannel->m_tChanParam.m_tVEncCfg.m_iEncodeType = iVideoEncodeType;

					RtpInitHeaderWithAecType(ptAudioRtpHeader, ptChannel->m_tFrameHeader.u8ACoder);
					RtpInitHeaderWithVecType(ptVideoRtpHeader, iVideoEncodeType, ptJpegRtpHeader);
				}
				
				//video
				if(ptSession->m_iSetupVideo)
				{
					ProcessVideo(ptSession, pcData + iPos, ptChannel->m_tFrameHeader.u32VStreamLen);
				}
				iPos += ptChannel->m_tFrameHeader.u32VStreamLen;
				
				//audio
				if(ptSession->m_iSetupAudio) //only if audio is requested, we send audio stream. 
				{
					ProcessAudio(ptSession, pcData + iPos, ptChannel->m_tFrameHeader.u16AFrameSize * ptChannel->m_tFrameHeader.u16AFrames);
				}
			}	
		}
		
		MutexLockUnLock(&ptSession->m_tLock);
		
		ptNode = adt_dlist_next(ptNode);
	}

EXIT:
	adt_dlist_unlock(ptServer->m_ptDListSession);
	return iRet;
}

int ProcessVideo(TRtspSession *_ptSession, char *_pcData, int _iLen)
{
	TRtspSession *ptSession = _ptSession;
	char *pcData = _pcData;
	TRtspServer *ptServer  = NULL;
	TChannel	*ptChannel = NULL;
	
	if(NULL == ptSession || NULL == pcData)
	{
		return TD_ERROR;
	}
	ptServer = (TRtspServer *)ptSession->m_pServer;

	if(ptServer == NULL)
	{
		return TD_ERROR;
	}
	
	ptChannel = &(ptServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan]);
	if(ptChannel == NULL)
	{
		return TD_ERROR;
	}

	ptSession->m_tRtpInfo.m_tVRtpHeader.ts = ptChannel->m_tFrameHeader.u32TimeStamp * 90;

	if(*pcData == 0xff)
	{
		//Mjpeg
		MJpegSendVideo(ptSession, pcData, _iLen);
	}
	else
	{
		//h264
		H264SendVideo(ptSession, pcData, _iLen);
	}

	return TD_OK;
}

int ProcessAudio(TRtspSession *_ptSession, char *_pcAudioData, int _iAudioTotalFrameSize)
{
	TRtspSession	*ptSession		= NULL;	
	TRtspServer		*ptRtspServer	= NULL;
	TChannel		*ptChannel		= NULL;
	TRtpHeader		*ptRtpHeader	= NULL;
	char 			*pcAudioData 	= NULL;
	
	unsigned char ucAudioEncodeType;
	
	if(NULL == _ptSession || NULL == _pcAudioData || _iAudioTotalFrameSize <= 0)
	{
		return TD_ERROR;
	}

	ptSession	= (TRtspSession *)_ptSession;
	pcAudioData 	= _pcAudioData;
	
	ptRtspServer 	= (TRtspServer *)ptSession->m_pServer;
	ptChannel 		= &(ptRtspServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan]);
	ptRtpHeader	= &(ptSession->m_tRtpInfo.m_tARtpHeader);
	
	ucAudioEncodeType = ptChannel->m_tFrameHeader.u8ACoder;
	
	switch(ucAudioEncodeType)
	{
		case G711A:
			G711ASendAudio(ptSession, _pcAudioData);
			break;
			
		case G711U:
			G711USendAudio(ptSession, _pcAudioData);
			break;
			
		case ADPCM_DIV4:
			ADPCM_DIV4SendAudio(ptSession, _pcAudioData);
			break;
			
		case AAC4:
			AAC4SendAudio(ptSession, _pcAudioData, _iAudioTotalFrameSize);
			break;
			
		default :
			;
	}

	return TD_OK;
}

 
