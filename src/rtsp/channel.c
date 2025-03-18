/*
 *	file: channel.c
 *	description: channel process
 *	history: dongliqiang created at 20130228
 */

#include <string.h>
#include <unistd.h>
#include "common.h"
#include "channel.h"
#include "rtsp_data.h"
#include "media_h264.h"
#include "media_audio.h"
#include "debug.h"
#include "share_mem.h"

int inner_GetSpsInfo(TChannel *_ptChannel)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TChannel *ptChannel = _ptChannel;
	stFrameInfoPlate tFrameInfo = {0};
	char *pcData = NULL;
	int iDataLen = 0;
	int iCount = 0;
	TGuiStreamChannel *ptGuiShmStream = NULL;
	int iFrameRate = 0;
	int iGetSampleRate = 1;

	if(NULL == ptChannel)
	{
		td_printf(0, "%s %d %s, input param NULL!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	ptGuiShmStream = ptChannel->m_tGuiStream.m_ptGuiShmData;

	if(ptGuiShmStream->m_iBase == ptGuiShmStream->m_iTop)
	{
		//等待内核往共享内存放数据,由帧率计算等待时间。
		iFrameRate = ptChannel->m_tChanParam.m_tVEncCfg.m_iFrameRate;
		usleep(1000*1000 / iFrameRate);
		if(ptGuiShmStream->m_iBase == ptGuiShmStream->m_iTop)
		{
			td_printf(0, "warnning: %s %d %s, channel[%d] has no video\n", __FILE__, __LINE__, __FUNCTION__, ptChannel->m_iChan);
			//return TD_ERROR; //不返回，继续执行。
		}
	}

	//wait 3 seconds for I frame.
	while(iCount++ < 300) //1s==100
	{
		while(GetFrameInfo(ptChannel, &tFrameInfo) == TD_OK)
		{
			//print_frameinfo(&tFrameInfo);
			if(tFrameInfo.iDataLength > MAX_FRAME_SIZE 
				|| tFrameInfo.iDataLength <= 0 
				|| tFrameInfo.cFrameRate > 30)
			{
				td_printf(0, "warnning:%s %d %s, tFrameInfo error! iDataLength = %d, cFrameRate = %d", __FILE__, __LINE__, __FUNCTION__, tFrameInfo.iDataLength, tFrameInfo.cFrameRate);
				ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase = ptChannel->m_tGuiStream.m_ptGuiShmData->m_iTop;
			}
			else
			{
#if 0			
				td_printf(0, "%s %d %s m_ptGuiShmData size[%d], base[%d], top[%d]\n", __FILE__, __LINE__, __FUNCTION__, \
					ptChannel->m_tGuiStream.m_ptGuiShmData->m_iSize, \
					ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase, \
					ptChannel->m_tGuiStream.m_ptGuiShmData->m_iTop);
			
				td_printf(0, "inner_GetSpsInfo :\ncFrameType == %d, \ncFrameRate == %d, \niDataLength == %d", \
							tFrameInfo.cFrameType, \
							tFrameInfo.cFrameRate, \
							tFrameInfo.iDataLength);
#endif	
				pcData = ptChannel->m_tGuiStream.m_ptGuiShmData->m_cBuf + ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase;
				iDataLen = tFrameInfo.iDataLength;
				if(GetFrameHeader(pcData, iDataLen, &ptChannel->m_tFrameHeader))
				{
					td_printf(0, "%s %d %s inner_GetFrameHeader error!\n", __FILE__, __LINE__, __FUNCTION__);
				}
				else
				{

					//added by qiaohaijun @2013.08.22
					if(iGetSampleRate == 1)
					{
						//onvif不支持G711A/G711U/ADPCM   32k/48k
						//ptChannel->m_tChanParam.m_iAudioSampleRate = OnvifCmdGetAudioRateCnf(ptChannel->m_iChan + 1);

						if(ptChannel->m_tFrameHeader.u8ACoder == 0x16)
						{
							//ptChannel->m_tChanParam.m_iAudioSampleRate = OnvifCmdGetAudioRateCnf(ptChannel->m_iChan + 1);	//dlq modified at 20130925 for VLC time out -- aac have no share memory!

							if(ptChannel->m_tFrameHeader.u16AFrames > 0)
							{
								if(ptChannel->m_tChanParam.m_iAudioSampleRate != 48000 ||ptChannel->m_tChanParam.m_iAudioSampleRate != 32000)
								{
									ptChannel->m_tChanParam.m_iAudioSampleRate = AACGetSampleRate(pcData + sizeof(S_FrameHeader) + ptChannel->m_tFrameHeader.u32VStreamLen); 
								}
							}
							td_printf(0, "the aac audio sample rate is %d", ptChannel->m_tChanParam.m_iAudioSampleRate);
						}
						else
						{
							ptChannel->m_tChanParam.m_iAudioSampleRate = 8000;
						}
						iGetSampleRate = 0;
					}
					
					td_printf(0, "audio sample rate is %d", ptChannel->m_tChanParam.m_iAudioSampleRate);
					//end
					//td_printf(0, "%s %d %s, frame type = %d(0I,1P)\n", __FILE__, __LINE__, __FUNCTION__, ptChannel->m_tFrameHeader.u8FrameType);
					if(ptChannel->m_tFrameHeader.u8FrameType == FRAME_TYPE_I)
					{
						memset(&ptChannel->m_tSdpInfo, 0, sizeof(TSDPInfo));
						if(TD_OK == H264GetSpsPpsFromIFrame(&ptChannel->m_tSdpInfo, pcData + sizeof(S_FrameHeader), ptChannel->m_tFrameHeader.u32VStreamLen))
						{
							//we keep this I frame
							ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase -= sizeof(stFrameInfoPlate);
							//ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase += tFrameInfo.iDataLength;

							return TD_OK;
						}
					}
				}

				ptChannel->m_tGuiStream.m_ptGuiShmData->m_iBase += tFrameInfo.iDataLength;
			}
			
			
		}

		//td_printf(0, "%s %d %s, iCount = %d\n", __FILE__, __LINE__, __FUNCTION__, iCount);
		usleep(1000);
	}

	return TD_OK;    // if cannot get sps info, we still reply with no sps.
}

int ChannelInit(TChannel *_ptChannel)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TChannel *ptChannel = _ptChannel;

	if(NULL == ptChannel)
	{
		return TD_ERROR;
	}

	memset(ptChannel, 0, sizeof(TChannel));
	MutexLockInit(&ptChannel->m_tLock);
	ptChannel->m_iStat = CHANNEL_INITED;
	return TD_OK;
}

int ChannelUnInit(TChannel *_ptChannel)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TChannel *ptChannel = _ptChannel;

	if(NULL == ptChannel)
	{
		return TD_ERROR;
	}

	MutexLockDestroy(&ptChannel->m_tLock);
	ptChannel->m_iStat = CHANNEL_UNINITED;
	
	return TD_OK;
}

int ChannelConnect(TChannel *_ptChannel)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iShmNum = 0;
	TChannel *ptChannel = _ptChannel;
	TRtspServer *ptServer = NULL;
	int iViNum = 0;
	int iAllUseCnt = 0;

	if(NULL == ptChannel)
	{
		return TD_ERROR;
	}
	ptServer = (TRtspServer *)ptChannel->m_pServer;
	if(NULL == ptServer)
	{
		td_printf(0, "%s %d %s, input param error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	iViNum = ptServer->m_tNvsInfo.m_iVINum;
	
	iShmNum = ptChannel->m_iStream * iViNum + ptChannel->m_iChan + 1;
	if(iShmNum < 1 || iShmNum > (MAX_STREAM*iViNum))
	{
		td_printf(0, "%s %d %s, iShmNum invalid!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}

	//td_printf(0, "%s %d %s m_iUseCnt %d\n", __FILE__, __LINE__, __FUNCTION__, ptChannel->m_tGuiStream.m_iUseCnt);
	iAllUseCnt = ptChannel->m_tGuiStream.m_iOnvifCnt + ptChannel->m_tGuiStream.m_iDanaleCnt;
	if(iAllUseCnt <= 0)
	{
		if(OnvifCmdStreamConnect(iShmNum) != TD_OK)
		{
			td_printf(0, "%s %d %s OnvifCmdStreamReq iShmNum [%d] error!\n", __FILE__, __LINE__, __FUNCTION__, iShmNum);
			return TD_ERROR;
		}

		ptChannel->m_tGuiStream.m_ptGuiShmData = (TGuiStreamChannel *)get_chn_shmaddr(iShmNum - 1); 
		if(NULL == ptChannel->m_tGuiStream.m_ptGuiShmData)
		{
			return TD_ERROR;
		}
		ptChannel->m_tGuiStream.m_iOnvifCnt = 0;
		td_printf(0, "%s %d %s  iShmNum[%d] m_iChan[%d] m_iStream[%d] shm_data[%p]\n", __FILE__, __LINE__, __FUNCTION__, iShmNum, ptChannel->m_iChan, ptChannel->m_iStream, ptChannel->m_tGuiStream.m_ptGuiShmData);

		//IPC修改分辨率后不重启，对于不实时解sps的情况，需要所有通道断开后再连接。
		if(GUI_H264 == ptServer->m_tChanel[ptChannel->m_iStream][ptChannel->m_iChan].m_tChanParam.m_tVEncCfg.m_iEncodeType)
		{
			if(OnvifCmdIframeReq(iShmNum))
			{
				td_printf(0, "%s, OnvifCmdIframeReq failed. Ask for I frame failed.\n", __FUNCTION__);
				return TD_ERROR;
			}
			
			//get sps info
			if(inner_GetSpsInfo(ptChannel) != TD_OK)
			{
				td_printf(0, "%s %d %s inner_GetSpsInfo error!\n", __FILE__, __LINE__, __FUNCTION__);
				return TD_ERROR;
			}
		}
		ptChannel->m_iStat = CHANNEL_CONNECTED;//置为CHANNEL_CONNECTED状态后不能再请求sps.否则和data线程冲突。
	}
	else
	{
		#ifdef WITH_DANALE
		if(NULL != ptChannel->m_tGuiStream.m_ptGuiShmData)
		{
			ptChannel->m_iStat = CHANNEL_CONNECTED;
		}
		#endif
	}
	ptChannel->m_tGuiStream.m_iOnvifCnt++;
	
	return TD_OK;
}

int ChannelDisConnect(TChannel *_ptChannel)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iRet = 0;
	int iShmNum = 0;
	TChannel *ptChannel = _ptChannel;
	TRtspServer *ptServer = NULL;
	int iViNum = 0;
	int iAllUseCnt = 0;

	if(NULL == ptChannel)
	{
		td_printf(0, "%s %d %s, input param error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	ptServer = (TRtspServer *)ptChannel->m_pServer;
	if(NULL == ptServer)
	{
		td_printf(0, "%s %d %s, ptChannel->m_pServer is NULL, error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}

	if(ptChannel->m_iStat != CHANNEL_CONNECTED)
	{
		iRet = TD_OK;
		goto EXIT;
	}
	/*
	//changguoxing add,usecnt不能直接减1，因为当session还没连接成功时就被断开，则usecnt还没加1，
	//这里不加判断地减一会造成usecnt和实际session数不一致。我们在外层加上对session状态的判断。
	*/
	ptChannel->m_tGuiStream.m_iOnvifCnt--;
	td_printf(0, "%s %d %s m_iUseCnt %d\n", __FILE__, __LINE__, __FUNCTION__, ptChannel->m_tGuiStream.m_iOnvifCnt);

	iAllUseCnt = ptChannel->m_tGuiStream.m_iOnvifCnt + ptChannel->m_tGuiStream.m_iDanaleCnt;
	if(iAllUseCnt <= 0)
	{
		iViNum = ptServer->m_tNvsInfo.m_iVINum;
		iShmNum = ptChannel->m_iStream * iViNum + ptChannel->m_iChan + 1;
		//td_printf(0, "%s %d %s iShmNum %d\n", __FILE__, __LINE__, __FUNCTION__, iShmNum);
		if(OnvifCmdStreamDisConnect(iShmNum) != TD_OK)
		{
			td_printf(0, "%s %d %s OnvifCmdStreamDisConnect iShmNum [%d] error!\n", __FILE__, __LINE__, __FUNCTION__, iShmNum);
			iRet = TD_ERROR;
			goto EXIT;
		}
		
		ptChannel->m_tGuiStream.m_iOnvifCnt = 0;
		ptChannel->m_iStat = CHANNEL_UNCONNECTED;
		//清空SDP信息
		memset(&ptChannel->m_tSdpInfo, 0, sizeof(TSDPInfo));
	}
	
EXIT:
	return iRet;
}

int ChannelGetParam(TChannel *_ptChannel)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TChannel *ptChannel = _ptChannel;
	TRtspServer *ptServer = NULL;
	TGuiVideoEncodeCnf stVideoEncodeCnf = {0};
	int iChanNum = 0;
	int iRet = 0;
	
	if(NULL == ptChannel)
	{
		return TD_ERROR;
	}

	ptServer = (TRtspServer *)ptChannel->m_pServer;
	iChanNum = ptChannel->m_iStream * ptServer->m_tNvsInfo.m_iVINum + ptChannel->m_iChan;

	//get encoder param
	if(OnvifCmdGetVideoEncodeCnf(iChanNum + 1, &stVideoEncodeCnf))
	{
		td_printf(0, "%s %d %s OnvifCmdGetVideoEncodeCnf error!\n", __FILE__, __LINE__, __FUNCTION__);
		iRet = TD_ERROR;
		goto EXIT;
	}
	
	ptChannel->m_tChanParam.m_tVEncCfg = stVideoEncodeCnf;

	//get other param
EXIT:
	return TD_OK;
}

