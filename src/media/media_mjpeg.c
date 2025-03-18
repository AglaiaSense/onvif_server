/*
	media_h264.c
	by qiaohaijun @2013.3.27
 */
 

#include <unistd.h>
#include <string.h>

#include "media_mjpeg.h"
#include "rtsp_cmd.h"
#include "connection.h"

/****************************************************
 *名称：MJpegGetLable
 *功能：处理获取JPEG标签名
 *参数：  _pBuf:数据输入指针
 			_plable:输出标签名


 *返回值:标签数据长度
 			-1 表示错误
 *备注:	
 *修改历史：
	20110413 v1.0 edited by czl
	........
****************************************************/
int MJpegGetLable(unsigned char *_pBuf, unsigned char *_pcLable)
{	
	unsigned short usLableLen = 0;
	if(*(_pBuf) == 0xFF)
	{
		if(0xD8 == *(_pBuf + 1))
		{
			*_pcLable = *(_pBuf + 1);
			return 0;
		}
		else
		{
			*_pcLable = *(_pBuf + 1);
			memcpy(&usLableLen, _pBuf + 2, sizeof(unsigned short));
			return (int)Myntols(usLableLen);
		}
	}
	else
	{
		return -1;
	}
}

/****************************************************
 *名称：MJpegDealFrameHead
 *功能：处理JPEG的头部信息
 *参数：  _pBuffer:输入JPEG存储地址
 			_pRHdr:输出的RTP的JPEG头信息

            
 *返回值:	输出处理后数据指针偏移量
 *备注:	
 *修改历史：
	20110413 v1.0 edited by czl
	........
****************************************************/
int MJpegDealFrameHead(char *_pBuffer, TRtpJpegHeader_t *_pRHdr)
{
	unsigned char ucLable;
	int iHeadOffset = 0;
	int iLableLength;
	unsigned char  *pcTmp = (unsigned char *)_pBuffer;
	unsigned short usHeight = 0;
	unsigned short usWidth = 0;
	unsigned short usHeighttmp = 0;
	unsigned short usWidthtmp = 0;
	while(1)
	{
		iLableLength = MJpegGetLable(pcTmp, &ucLable);
		pcTmp += 2;
		if(iLableLength < 0)
		{
			break;
		}
		switch(ucLable)
		{

			case 0xDD:
				memcpy(&(_pRHdr->m_tJpegResetHead.dri), pcTmp + 2, sizeof(unsigned short));
				break;
			case 0xDB:
				memcpy(_pRHdr->m_ucQtable, pcTmp + 2 + 1, 64);
				if(iLableLength < 128)
				{
					memcpy(_pRHdr->m_ucQtable + 64, pcTmp + 2 + 1, 64);//use
				}
				else
				{
					memcpy(_pRHdr->m_ucQtable + 64, pcTmp + 2 + 66, 64);//use
				}
				
				//memcpy(_pRHdr->m_ucQtable + 64, _pTmp + 2 + 70, 64);
				//iLableLength += 69;
				break;
			case 0xC4:
				*(_pRHdr->m_ucHuftable) = 0xff;
				*(_pRHdr->m_ucHuftable + 1) = 0xc4;
				memcpy(_pRHdr->m_ucHuftable + 2, pcTmp, 0x1A2);
				break;
			case 0xC0:
				memcpy(&usHeighttmp, pcTmp + 3, sizeof(unsigned short));
				memcpy(&usWidthtmp, pcTmp + 5, sizeof(unsigned short));
				usHeight = Myntols(usHeighttmp);
				usWidth = Myntols(usWidthtmp);
				_pRHdr->m_tJpegMainHead.height = (unsigned char)((usHeight>>3)&0xff);
				_pRHdr->m_tJpegMainHead.width = (unsigned char)((usWidth >> 3)&0xff);
				break;
			case 0xE0:
				;
			case 0xDA:
				;
			case 0xEF://hisi user data
				;
			case 0xD8:
				break;
			default:
				printf("the error is 0x%x\n", ucLable);
				return -1;//error deal
		}
		pcTmp += iLableLength;		
		iHeadOffset += 2 + iLableLength;
		//log_debug(0, "lablelenth=%d, headeroffset=%d, uch_Lable=%x", liv_LableLength, liv_HeadOffset, uch_Lable);
	}
	return iHeadOffset;
	
}


int MJpegSendPacket(TRtspSession *_ptSession, char *_pcData, int _iLen, int _iMarker, int _iFirstFlag, TRtpJpegHeader_t *_ptRtpJpegHdr)
{
	int iTmpLen = 0;

	if(_iFirstFlag)
	{
		iTmpLen = sizeof(JpegMainHeader_t) + sizeof(JpegResetHeader_t) + sizeof(JpegQtableHeader_t) + 128;
	}
	else
	{
		iTmpLen = sizeof(JpegMainHeader_t) + sizeof(JpegResetHeader_t);
	}
	return RtpSendMediaPacket(_ptSession, V_FLAG, _iMarker, ((char *)(_ptRtpJpegHdr)), iTmpLen, _pcData, _iLen);

}

int MJpegSendVideo(TRtspSession *_ptSession, char *_pcData, int _iLen)
{

	TRtspSession *ptSession = (TRtspSession *)_ptSession;
	TChannel *ptChannel = NULL;
	TRtspServer *ptServer = NULL;
	char *pcData = _pcData;
	
	TRtpJpegHeader_t *ptRtpJpegHdr = &(ptSession->m_tRtpInfo.m_tJpegHeader);
	
	ptServer = (TRtspServer *)ptSession->m_pServer;
	ptChannel = &(ptServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan]);

	int iHeadOffset = 0;
	int iJpegLeftLen = 0;
	char *pcTmp;
	int iFirstFlag = 1;
	int iPureJpegLen;
	unsigned short usOffset;

	int iMarker = 0;
	
	iHeadOffset = MJpegDealFrameHead(pcData, ptRtpJpegHdr);
	
	if(iHeadOffset < 0)
	{
		return -1;
	}
	
	iJpegLeftLen = _iLen - iHeadOffset;
		
	pcTmp = pcData + iHeadOffset;
	ptRtpJpegHdr->m_tJpegMainHead.off = usOffset = 0;
	
	while(iJpegLeftLen > 0)
	{
		iMarker = 0;
		iPureJpegLen = iFirstFlag ? MAX_PAYLOAD_LEN - RTP_JPEG_FIRSTHEADLENGTH : MAX_PAYLOAD_LEN -RTP_JPEG_HEADLENGTH;
		if(iPureJpegLen > iJpegLeftLen)
		{
			iPureJpegLen = iJpegLeftLen;
			iMarker = 1;
		}

		MJpegSendPacket(ptSession, pcTmp, iPureJpegLen, iMarker, iFirstFlag, ptRtpJpegHdr);
	
		iJpegLeftLen -= iPureJpegLen;
		usOffset += iPureJpegLen;
		ptRtpJpegHdr->m_tJpegMainHead.off  = Myntols(usOffset) << 8;
		iFirstFlag = 0;
		pcTmp += iPureJpegLen;
	}
	
	//动态更新量化表索引(大小范围为127-255)
	if(ptRtpJpegHdr->m_tJpegMainHead.q == 255)
	{
		ptRtpJpegHdr->m_tJpegMainHead.q = 127;
	}
	ptRtpJpegHdr->m_tJpegMainHead.q++;

	return 0;
	
}




















