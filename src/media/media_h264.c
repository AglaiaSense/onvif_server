/*
	media_h264.c
	by qiaohaijun @2013.3.27
 */
 

#include <unistd.h>
#include <string.h>
#include "rtp.h"
#include "media_h264.h"
#include "rtsp_cmd.h"
#include "connection.h"

static char g_pcNaluHead[4] = {0, 0, 0, 1};



//搜索某数据串
static char *inner_SearchNum(char *chp_Src, unsigned int liv_SrcSize,const char *chp_Num, unsigned int liv_NumSize)
{
	unsigned int iM;
	unsigned int iN;
	if (liv_SrcSize < liv_NumSize)
	{
		return NULL;
	}
	for (iM = 0; iM < liv_SrcSize - liv_NumSize; iM++)
	{
		for(iN = 0; iN < liv_NumSize; iN++)
		{
			if (*(chp_Src + iM + iN) == *(chp_Num + iN))
			{
				if (iN == (liv_NumSize - 1))
				{
					return (char *)(chp_Src + iM);
				}
			} 
			else
			{
				break;
			}
		}
	}
	return NULL;
}


/****************************************************
名称：chp_SearchSPS
功能：搜索标准的SPS头
参数：  _pos:起始位置；
		_iLength:搜索范围数据长度
 			 		
返回值:	SPS的起始指针(NULL表示未搜索到)
备注:	
修改历史：
  20120305 v1.0 created by czl
	........
****************************************************/
static char *inner_SearchSPS(char *_pos, int _iLength)
{
	char *chp_SPSPos = NULL;
	char *chp_Pos = _pos;
	int iLength = _iLength;
	
	while(iLength > NALU_HEADER_LEN)
	{
		chp_SPSPos = inner_SearchNum(chp_Pos, iLength, g_pcNaluHead, NALU_HEADER_LEN);
		if(chp_SPSPos == NULL)
		{
			return NULL;
		}
		if((*(chp_SPSPos + NALU_HEADER_LEN)&0x1f) == SPS_FLAG)
		{
			return chp_SPSPos;
		}
		iLength -= (int)(chp_SPSPos - chp_Pos) + NALU_HEADER_LEN;
		chp_Pos = chp_SPSPos + NALU_HEADER_LEN;
	}
	
	return NULL;
}
/****************************************************
名称：chp_SearchPPS
功能：搜索标准的SPS头
参数：  _pos:起始位置；
		_iLength:搜索范围数据长度
 			 		
返回值:	PPS的起始指针(NULL表示未搜索到)
备注:	
修改历史：
  20120305 v1.0 created by czl
	........
****************************************************/
static char *inner_SearchPPS(char *_pos, int _iLength)
{
	char *chp_PPSPos = NULL;
	char *chp_Pos = _pos;
	int iLength = _iLength;
	while(iLength > NALU_HEADER_LEN)
	{
		chp_PPSPos = inner_SearchNum(chp_Pos, iLength, g_pcNaluHead, NALU_HEADER_LEN);
		if(chp_PPSPos == NULL)
		{
			return NULL;
		}
		if((*(chp_PPSPos + NALU_HEADER_LEN)&0x1f) == PPS_FLAG)
		{
			return chp_PPSPos;
		}
		iLength -= (int)(chp_PPSPos - chp_Pos) + NALU_HEADER_LEN;
		chp_Pos = chp_PPSPos + NALU_HEADER_LEN;
	}
	
	return NULL;
}
/****************************************************
名称：chp_SearchIFrame
功能：搜索标准的I帧头
参数：  _pos:起始位置；
		_iLength:搜索范围数据长度
 			 		
返回值:	I帧的起始指针(NULL表示未搜索到)
备注:	
修改历史：
  20120305 v1.0 created by czl
	........
****************************************************/
static char *inner_SearchIFrame(char *_pos, int _iLength)
{
	char *chp_IFramePos = NULL;
	char *chp_Pos = _pos;
	int iLength = _iLength;
	while(iLength > NALU_HEADER_LEN)
	{
		chp_IFramePos = inner_SearchNum(chp_Pos, iLength, g_pcNaluHead, NALU_HEADER_LEN);
		if(chp_IFramePos == NULL)
		{
			return NULL;
		}
		if((*(chp_IFramePos + NALU_HEADER_LEN)&0x1f) == IFRAME_FLAG)
		{
			return chp_IFramePos;
		}
		iLength -= (int)(chp_IFramePos - chp_Pos) + NALU_HEADER_LEN;
		chp_Pos = chp_IFramePos + NALU_HEADER_LEN;
	}
	
	return NULL;
}
/****************************************************
名称：chp_SearchPFrame
功能：搜索标准的P帧头
参数：  _pos:起始位置；
		_iLength:搜索范围数据长度
 			 		
返回值:	I帧的起始指针(NULL表示未搜索到)
备注:	
修改历史：
  20120418 v1.0 created by czl
	........
****************************************************/
static char *inner_SearchPFrame(char *_pos, int _iLength)
{
	char *chp_PFramePos = NULL;
	char *chp_Pos = _pos;
	int iLength = _iLength;
	while(iLength > NALU_HEADER_LEN)
	{
		chp_PFramePos = inner_SearchNum(chp_Pos, iLength, g_pcNaluHead, NALU_HEADER_LEN);
		if(chp_PFramePos == NULL)
		{
			return NULL;
		}
		if((*(chp_PFramePos + NALU_HEADER_LEN)&0x1f) == PFRAME_FLAG)
		{
			return chp_PFramePos;
		}
		iLength -= (int)(chp_PFramePos - chp_Pos) + NALU_HEADER_LEN;
		chp_Pos = chp_PFramePos + NALU_HEADER_LEN;
	}
	
	return NULL;
}

int H264GetSpsPpsFromIFrame(TSDPInfo *_ptSdpInfo, char *_pcData, int _iLen)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TSDPInfo *ptSdpInfo = _ptSdpInfo;
	char *pcData = _pcData;
	char *pcSps = NULL;
	int iSpsLen = 0;
	char *pcPps = NULL;
	int iPpsLen = 0;
	char *pcPps2 = NULL;
	int iPps2Len = 0;
	char *pcNextNalu = NULL;
	int iPos = 0;

	if(NULL == ptSdpInfo || NULL == pcData)
	{
		return TD_ERROR;
	}

	//get sps
	pcSps = inner_SearchSPS(pcData + iPos, _iLen - iPos);
	if(NULL == pcSps)
	{
		td_printf(0, "%s %d %s, inner_SearchSPS failed!\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	iPos = pcSps - pcData;

	iPos += NALU_HEADER_LEN;
	pcPps = inner_SearchPPS(pcData + iPos, _iLen - iPos);
	if(NULL == pcPps)
	{
		td_printf(0, "%s %d %s, inner_SearchSPS failed!\n", __FILE__, __LINE__, __FUNCTION__);
		return -2;
	}

	iSpsLen = pcPps - pcSps - NALU_HEADER_LEN;

	//填充SDP 中的SPS信息
	memcpy(ptSdpInfo->m_pcSps, pcData + iPos, iSpsLen);
	ptSdpInfo->m_iSpsLen = iSpsLen;
	
	iPos += iSpsLen;

	//get pps
	iPos += NALU_HEADER_LEN;
	pcNextNalu = inner_SearchNum(pcData + iPos, _iLen - iPos, g_pcNaluHead, NALU_HEADER_LEN);
	if(NULL == pcNextNalu)
	{
		return -3;
	}
	iPpsLen = pcNextNalu - pcPps - NALU_HEADER_LEN;

	//填充SDP 中的PPS信息
	memcpy(ptSdpInfo->m_pcPps, pcData + iPos, iPpsLen);
	ptSdpInfo->m_iPpsLen = iPpsLen;
	
	iPos += iPpsLen;

	//get pps2
	iPos += NALU_HEADER_LEN;
	if((*(pcData + iPos)&0x1f) == PPS_FLAG)
	{
		pcPps2 = pcNextNalu;
		pcNextNalu = inner_SearchNum(pcData + iPos, _iLen - iPos, g_pcNaluHead, NALU_HEADER_LEN);
		if(NULL == pcNextNalu)
		{
			return -5;
		}
		iPps2Len = pcNextNalu - pcPps2 - NALU_HEADER_LEN;//changguoxing
		
		//填充SDP 中的PPS2信息
		memcpy(ptSdpInfo->m_pcPps2, pcData + iPos, iPps2Len);
		ptSdpInfo->m_iPps2Len = iPps2Len;
		iPos += iPps2Len;
	}
	
	return 0;
}

int H264SendSPS(TRtspSession *_ptSession, char *_pcData, int _iLen)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = (TRtspSession *)_ptSession;
	char *pcData = _pcData;
	char *pcSpsPos = NULL;
	int iSpsLen = 0;
	char *pcNextNalu = NULL;
	int iPos = 0;

	if(ptSession == NULL || pcData == NULL)
	{
		return 0;
	}

	//find sps
	pcSpsPos = inner_SearchSPS(pcData + iPos, _iLen - iPos);
	if(NULL == pcSpsPos)
	{
		td_printf(0, "%s %d %s warnning: pcSpsPos is not find!\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}

	iPos += (pcSpsPos - pcData);

	//find next nalu
	iPos += NALU_HEADER_LEN;
	pcNextNalu = inner_SearchNum(pcData + iPos, _iLen - iPos, g_pcNaluHead, NALU_HEADER_LEN);
	if(NULL == pcNextNalu)
	{
		td_printf(0, "%s %d %s warnning: pcNextNalu is not find!\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}
	
	iSpsLen = pcNextNalu - pcSpsPos - NALU_HEADER_LEN;
	iPos += iSpsLen;
	
	RtpSendMediaPacket(ptSession, V_FLAG, TD_FALSE, pcSpsPos + NALU_HEADER_LEN, iSpsLen, NULL, 0);
//	td_printf(0, "send SPS over....\n");
	return iPos;
}

int H264SendPPS(TRtspSession *_ptSession, char *_pcData, int _iLen)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
//	td_printf(0, "\nsend PPS started....");
	TRtspSession *ptSession = (TRtspSession *)_ptSession;
	char *pcData = _pcData;
	char *pcPpsPos = NULL;
	int iPpsLen = 0;
	char *pcNextNalu = NULL;
	int iPos = 0;
	
	if(NULL == ptSession || NULL == pcData)
	{
		return 0;
	}

	//find pps
	pcPpsPos = inner_SearchPPS(pcData + iPos, _iLen - iPos);
	if(NULL == pcPpsPos)
	{
		td_printf(0, "%s %d %s warnning: pcPpsPos is not find!\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}

	iPos += (pcPpsPos - pcData);

	//find next nalu
	iPos += NALU_HEADER_LEN;
	pcNextNalu = inner_SearchNum(pcData + iPos, _iLen - iPos, g_pcNaluHead, NALU_HEADER_LEN);
	if(NULL == pcNextNalu)
	{
		return 0;
	}
	
	iPpsLen = pcNextNalu - pcPpsPos - NALU_HEADER_LEN;
	iPos += iPpsLen;
	


	RtpSendMediaPacket(ptSession, V_FLAG, TD_FALSE, pcPpsPos + NALU_HEADER_LEN, iPpsLen, NULL, 0);

	//td_printf(0, "%s %d %s over!\n", __FILE__, __LINE__, __FUNCTION__);
	return iPos;	
}

int H264SendPPS2(TRtspSession *_ptSession, char *_pcData, int _iLen)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = (TRtspSession *)_ptSession;
	char *pcData = _pcData;
	char *pcPpsPos = NULL;
	int iPpsLen = 0;
	char *pcNextNalu = NULL;
	int iPos = 0;

	if(NULL == ptSession || NULL == pcData)
	{
		td_printf(0, "%s, input param error", __FUNCTION__);
		return 0;
	}

	//find pps
	pcPpsPos = inner_SearchPPS(pcData + iPos, _iLen - iPos);
	if(NULL == pcPpsPos)
	{
		//td_printf(0, "%s %d %s warnning: pcPps2Pos is not find!\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}

	iPos += (pcPpsPos - pcData);

	//find next nalu
	iPos += NALU_HEADER_LEN;
	pcNextNalu = inner_SearchNum(pcData + iPos, _iLen - iPos, g_pcNaluHead, NALU_HEADER_LEN);
	if(NULL == pcNextNalu)
	{
		td_printf(0, "%s, find next nalu failed.", __FUNCTION__);
		return 0;
	}
	
	iPpsLen = pcNextNalu - pcPpsPos - NALU_HEADER_LEN;
	iPos += iPpsLen;



	RtpSendMediaPacket(ptSession, V_FLAG, TD_FALSE, pcPpsPos + NALU_HEADER_LEN, iPpsLen, NULL, 0);

//	td_printf(0, "send PPS2 over....\n");
	return iPos;	
}

int H264SendIFrame(TRtspSession *_ptSession, char *_pcData, int _iLen)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	TRtspSession *ptSession = (TRtspSession *)_ptSession;
	char *pcData = _pcData;
	char *pcIFramePos = NULL;
	int iIFrameLen = 0;
	int iPos = 0;

	
	if(NULL == ptSession || NULL == pcData)
	{
		return 0;
	}
	
	//find I frame
	pcIFramePos = inner_SearchIFrame(pcData + iPos, _iLen - iPos);
	if(NULL == pcIFramePos)
	{
		td_printf(0, "==========%s %d %s warnning: pcIFramePos is not find==========\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}
	iPos += (pcIFramePos - pcData);
	iIFrameLen = _iLen - iPos;



	pcIFramePos += NALU_HEADER_LEN;
	iIFrameLen -= NALU_HEADER_LEN;

	//FU-A
	if(iIFrameLen > MAX_PAYLOAD_LEN)
	{
		RtpSendMediaPacketFUA(ptSession, V_FLAG, pcIFramePos, iIFrameLen);
	}
	//single NALU
	else if(iIFrameLen > 0)
	{

		RtpSendMediaPacket(ptSession, V_FLAG, 1, pcIFramePos, iIFrameLen, NULL, 0);
	}
	else
	{
		td_printf(0, "the Iframe data is wrong, iIFrameLen == %d",  iIFrameLen);
	}
	return iPos;	
}

int H264SendPFrame(TRtspSession *_ptSession, char *_pcData, int _iLen)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	TRtspSession *ptSession = (TRtspSession *)_ptSession;
	char *pcData = _pcData;
	char *pcPFramePos = NULL;
	int iPFrameLen = 0;
	int iPos = 0;

	if(NULL == ptSession || NULL == pcData)
	{
		td_printf(0, "%s %d %s , input param error!\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}

	//find P frame
	pcPFramePos = inner_SearchPFrame(pcData + iPos, _iLen - iPos);
	if(NULL == pcPFramePos)
	{
		return 0;
	}

	iPos += (pcPFramePos - pcData);
	iPFrameLen = _iLen - iPos;



	pcPFramePos += NALU_HEADER_LEN;
	iPFrameLen -= NALU_HEADER_LEN;

	//FU-A
	if(iPFrameLen > MAX_PAYLOAD_LEN)
	{
		RtpSendMediaPacketFUA(ptSession, V_FLAG, pcPFramePos, iPFrameLen);
	}	
	//single NALU
	else if(iPFrameLen > 0)
	{

		RtpSendMediaPacket(ptSession, V_FLAG, 1, pcPFramePos, iPFrameLen, NULL, 0);
	}
	else
	{
		td_printf(0, "the Pframe data is wrong, iPFrameLen == %d",  iPFrameLen);
	}
	
	//td_printf(0, "%s %d %s over!\n", __FILE__, __LINE__, __FUNCTION__);

	return iPos;	

}

int H264SendVideo(TRtspSession *_ptSession, char *_pcData, int _iLen)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TRtspSession *ptSession = (TRtspSession *)_ptSession;
	TChannel *ptChannel = NULL;
	TRtspServer *ptServer = NULL;
	char *pcData = _pcData;

	int iCount = 0;
	int iPos = 0;

	if(NULL == ptSession || NULL == pcData)
	{
		return TD_ERROR;
	}

	ptServer = (TRtspServer *)ptSession->m_pServer;
	ptChannel = &(ptServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan]);
		
	if(ptSession->m_iStat == SESSION_PLAY)
	{

		if(FRAME_TYPE_I == ptChannel->m_tFrameHeader.u8FrameType)
		{
			//td_printf(0, "\n\nsend I frame started....");
			//sps
			iCount = H264SendSPS(ptSession, pcData + iPos, _iLen - iPos);
			iPos += iCount;

			//pps
			iCount = H264SendPPS(ptSession, pcData + iPos, _iLen - iPos);
			iPos += iCount;

			//pps2
			iCount = H264SendPPS2(ptSession, pcData + iPos, _iLen - iPos);
			iPos += iCount;

			//I Frame
			iCount = H264SendIFrame(ptSession, pcData + iPos, _iLen - iPos);
			iPos += iCount;
			//td_printf(0, "\nsend I frame over\n\n");
		} 
		else if(FRAME_TYPE_P == ptChannel->m_tFrameHeader.u8FrameType)
		{
			//td_printf(0, "\n\nsend P frame started....");
			//p Frame
			iCount = H264SendPFrame(ptSession, pcData + iPos, _iLen - iPos);
			iPos += iCount;
			//td_printf(0, "\nsend P frame over\n\n");
		}
		else
		{
			td_printf(0, "%s %d %s warnning: frame type %d is not support!\n", __FILE__, __LINE__, __FUNCTION__, ptChannel->m_tFrameHeader.u8FrameType);
		}
		
	}
	return TD_OK;
}

