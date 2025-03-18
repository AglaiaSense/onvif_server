/*
 *	file: rtp.c
 *	description: rtp
 *	history: dongliqiang created at 20130219
 */

#include <string.h>
#include "rtp.h"
#include "rtsp_server.h"
#include "rtsp_cmd.h"


unsigned short Myntols(unsigned short _usInput)
{
	unsigned short usOutput;
	usOutput = (_usInput<<8) + (_usInput>>8);
	return usOutput;
}
 
int RtpInit(TConnection *_ptConnect, int _iNetType, void *_pUserData)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;

	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}

	InitConnection(ptConnect, CONNECT_TYPE_SERVER, _iNetType, 0, _pUserData);

	return TD_OK;
}

int RtpUnInit(TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;

	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	
	UnInitConnection(ptConnect);

	return TD_OK;
}

int RtpConnect(TConnection *_ptConnect, char *_pcServerIp, int _iServerPort)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;

	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
		
	return ptConnect->m_Connect(ptConnect, _pcServerIp, _iServerPort);
}

int RtpDisconnect(TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;

	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}

	return ptConnect->m_Close(ptConnect);
}

int RtpRecv(TConnection *_ptConnect, unsigned char *_ptData, int _iLen)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	unsigned char *pcData = _ptData;

	if(NULL == ptConnect || NULL == pcData)
	{
		return TD_ERROR;
	}
	
	if(ptConnect->m_iStat != SC_CONNECTED || ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		return TD_ERROR;
	}

	return ptConnect->m_RecvBuffer(ptConnect, pcData, _iLen);
}

int RtpSend(TConnection *_ptConnect, unsigned char *_ptData, int _iLen)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	unsigned char *pcData = _ptData;

	if(NULL == ptConnect || NULL == pcData)
	{
		return TD_ERROR;
	}
	
	if(ptConnect->m_iStat != SC_CONNECTED || ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		return TD_ERROR;
	}

	return ptConnect->m_SendBuffer(ptConnect, pcData, _iLen);
}

int RtpReadV(TConnection *_ptConnect, TIovec *_ptIovec, int _iIovCnt)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	TIovec *ptIovec = _ptIovec;

	if(NULL == ptConnect || NULL == ptIovec)
	{
		return TD_ERROR;
	}
	
	if(ptConnect->m_iStat != SC_CONNECTED || ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		return TD_ERROR;
	}

	return ptConnect->m_ReadV(ptConnect, ptIovec, _iIovCnt);
}


/****************************************************
 *Ãû³Æ£ºRtpWriteV
 *¹¦ÄÜ£º·¢ËÍÊý¾Ý
 *²ÎÊý£º 
 			_ptConnect:	rtsp connect	
 
 			_ptIovec:	Ìî³äºÃµÄTIovec°ü

 			_iIovCnt:		_iIovCnt
            
 *·µ»ØÖµ:	³É¹¦: ·¢ËÍÊý¾ÝµÄ³¤¶È
 				Ê§°Ü:TD_ERROR
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by dongliqiang @2013.3.27
	........
****************************************************/
int RtpWriteV(TConnection *_ptConnect, const TIovec *_ptIovec, int _iIovCnt)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	const TIovec *ptIovec = _ptIovec;

	if(NULL == ptConnect || NULL == ptIovec)
	{
		return TD_ERROR;
	}

	if(ptConnect->m_iStat != SC_CONNECTED || ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		return TD_ERROR;
	}

	if(ptConnect->m_WriteV != NULL)
	{
		return ptConnect->m_WriteV(ptConnect, ptIovec, _iIovCnt);//WriteVBuffer
	}

	return TD_ERROR;
}


/****************************************************
 *Ãû³Æ£ºMedia_Gen_Session_ID
 *¹¦ÄÜ£º²úÉúÍ¬²½±êÊ¶Ô´
 *²ÎÊý£º¿Õ
 
 *·µ»ØÖµ:Í¬²½±êÊ¶Ô´
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
		´Óutils.cÖÐÒÆÖ²by qiaohaijun @2013.3.26
****************************************************/
unsigned int RtpGenSessionID(void)
{
	 struct timespec spec;
	 clock_gettime(CLOCK_REALTIME, &spec);
	 return (unsigned int)(spec.tv_nsec);
}

/****************************************************
 *Ãû³Æ£ºRtpInitHeader
 *¹¦ÄÜ£º³õÊ¼»¯RTPÍ·ÐÅÏ¢
 *²ÎÊý£º _pRtpHdr:ÊäÈëRTPÍ·ÐÅÏ¢
 			_ucPT:payload type
 			 		

            
 *·µ»ØÖµ:	ÎÞ
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpInitHeader(TRtpHeader *_pRtpHdr, unsigned short _ucPT)
{
	_pRtpHdr->cc = 0;
	_pRtpHdr->marker = 0;
	_pRtpHdr->p = 0;
	_pRtpHdr->pt = _ucPT;
	_pRtpHdr->seqno = 0;
	_pRtpHdr->ts = 0;
	_pRtpHdr->version = RTP_VERSION;
	_pRtpHdr->x = 0;
	_pRtpHdr->ssrc = RtpGenSessionID();

	
	return 0;
}


/****************************************************
 *Ãû³Æ£ºRtpInitJpegHeader
 *¹¦ÄÜ£º³õÊ¼»¯JPEGµÄRTPÖÐµÄÃèÊöJPEGÍ·²¿·Ö
 *²ÎÊý£º  pJHdr:ÃèÊöÍ·Ö¸Õë

 *·µ»ØÖµ:0
 *±¸×¢:	2.0½«³¤¿í¸Äµ½ÊµÊ±»ñÈ¡
 *ÐÞ¸ÄÀúÊ·£º
	20110413 v1.0 edited by czl
	20120418 v2.0 edited by czl
	20130521 by qiaohaijun
	........
****************************************************/
int RtpInitJpegHeader(JpegMainHeader_t *pJHdr)
{
	pJHdr->width = 0;
	pJHdr->height = 0;
	pJHdr->type = 65;
	pJHdr->off = 0;
	pJHdr->q = 128;
	pJHdr->tspec = 0;
	return 0;
}
/****************************************************
 *Ãû³Æ£ºRtpInitRstHeader
 *¹¦ÄÜ£º³õÊ¼»¯JPEGµÄRTPÖÐµÄ·Ö¿éÍ·²¿·Ö
 *²ÎÊý£º  pJRHdr:RTPµÄ·Ö¿é±íÍ·Ö¸Õë

 *·µ»ØÖµ:0
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	20110413 v1.0 edited by czl
	20130521 by qiaohaijun
	........
****************************************************/
int RtpInitRstHeader(JpegResetHeader_t *pJRHdr)
{
	pJRHdr->count = 0x3ff;
	pJRHdr->f = 1;
	pJRHdr->l = 1;
	pJRHdr->dri = Myntols(0);//792;
	return 0;
}
/****************************************************
 *Ãû³Æ£ºRtpInitDqtHeader
 *¹¦ÄÜ£º³õÊ¼»¯JPEGµÄRTPÖÐµÄÁ¿»¯±íÍ·²¿·Ö
 *²ÎÊý£º  pJQHdr:RTPµÄÁ¿»¯±íÍ·Ö¸Õë

 *·µ»ØÖµ:0
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	20110413 v1.0 edited by czl
	20130521 by qiaohaijun
	........
****************************************************/
int RtpInitDqtHeader(JpegQtableHeader_t *pJQHdr)
{
	pJQHdr->mbz = 0;
	pJQHdr->precision = 0;
	pJQHdr->length = Myntols(128);
	return 0;
}
/****************************************************
 *Ãû³Æ£ºRtpInitJpegHead
 *¹¦ÄÜ£º³õÊ¼»¯JPEGµÄRTPÍ·
 *²ÎÊý£º  pRTPJHdr:RTPÍ·Ö¸Õë
 		
 *·µ»ØÖµ:0
 *±¸×¢:	È¥µô³¤¿í
 *ÐÞ¸ÄÀúÊ·£º
	20110413 v1.0 edited by czl
	20120418 v2.0 edited by czl
	20130521 by qiaohaijun
	........
****************************************************/
int RtpInitJpegHead(TRtpJpegHeader_t *_ptRTPJHdr)
{
	RtpInitJpegHeader(&_ptRTPJHdr->m_tJpegMainHead);
	RtpInitRstHeader(&_ptRTPJHdr->m_tJpegResetHead);
	RtpInitDqtHeader(&_ptRTPJHdr->m_tJpegQtableHead);
	return 0;
}

/****************************************************
 *Ãû³Æ£ºRtpInitHeaderWithAecType
 *¹¦ÄÜ£º¸ù¾ÝÊý¾ÝÀàÐÍ(±àÂëÀàÐÍ/PT)³õÊ¼»¯RTPÍ·
 *²ÎÊý£º 
 			_pRtpHdr			:	ÊäÈëRTPÍ·ÐÅÏ¢

			_iEncodeType 		: 	±àÂë·½Ê½

            
 *·µ»ØÖµ:	ÎÞ
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpInitHeaderWithAecType(TRtpHeader *_pRtpHdr, int _iEncodeType)
{
	switch(_iEncodeType)
	{
		case G711A:
			RtpInitHeader(_pRtpHdr, RTP_PT_ALAW);
			break;
			
		case G711U:
			RtpInitHeader(_pRtpHdr, RTP_PT_ULAW);
			break;
			
		case ADPCM_DIV4:
			RtpInitHeader(_pRtpHdr, RTP_PT_DVI4);
			break;
			
		case AAC4:
			RtpInitHeader(_pRtpHdr, RTP_PT_AAC);
			break;
			
		default:
				;
	}

	return TD_OK;
}

/****************************************************
 *Ãû³Æ£ºRtpInitHeaderWithVecType
 *¹¦ÄÜ£º¸ù¾ÝÊý¾ÝÀàÐÍ(±àÂëÀàÐÍ/PT)³õÊ¼»¯RTPÍ·
 *²ÎÊý£º 
 			_pRtpHdr			:	ÊäÈëRTPÍ·ÐÅÏ¢

			_iEncodeType 		: 	±àÂë·½Ê½

            
 *·µ»ØÖµ:	ÎÞ
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpInitHeaderWithVecType(TRtpHeader *_pRtpHdr, int _iEncodeType, TRtpJpegHeader_t *_ptJpegHdr)
{
	switch(_iEncodeType)
	{
		case GUI_MJPG:	
			RtpInitHeader(_pRtpHdr, RTP_PT_JPEG);
			RtpInitJpegHead(_ptJpegHdr);
			break;
			
		case GUI_H264:	
			RtpInitHeader(_pRtpHdr, RTP_PT_H264);
			break;
			
		default:
				;
	}

	return TD_OK;
}	
		

/****************************************************
 *Ãû³Æ£ºRtpUnInitHeader
 *¹¦ÄÜ£º³õÊ¼»¯RTPÍ·ÐÅÏ¢
 *²ÎÊý£º _pRtpHdr:ÊäÈëRTPÍ·ÐÅÏ¢
 			 		

            
 *·µ»ØÖµ:	ÎÞ
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpUnInitHeader(TRtpHeader *_pRtpHdr)
{
	if(_pRtpHdr != NULL)
	{
		free(_pRtpHdr);
	}	
	return 0;
}


/****************************************************
 *Ãû³Æ£ºRtpBuildHeader
 *¹¦ÄÜ£ºÌí¼ÓRTPÍ·ÐÅÏ¢
 *²ÎÊý£º _RtpHdrIN:ÊäÈëRTPÍ·ÐÅÏ¢
 			_pRtpHdrOUT:Êä³öRTPÍ·ÐÅÏ¢
 			_Marker:Maker
            
 *·µ»ØÖµ:	ÎÞ
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpBuildHeader(TRtpHeader *_RtpHdrIN, TRtpHeader * _pRtpHdrOUT, bool _Marker)
{
	TRtpHeader * pRtpHdrOUT = _pRtpHdrOUT;
	RTP_HDR_SET_VERSION(pRtpHdrOUT, RTP_VERSION);
	RTP_HDR_SET_P(pRtpHdrOUT, 0);
	RTP_HDR_SET_X(pRtpHdrOUT, 0);
	RTP_HDR_SET_CC(pRtpHdrOUT, 0);
	RTP_HDR_SET_M(pRtpHdrOUT, _Marker);
	RTP_HDR_SET_PT(pRtpHdrOUT, _RtpHdrIN->pt);
	RTP_HDR_SET_SEQNO(pRtpHdrOUT, htons(_RtpHdrIN->seqno));
	RTP_HDR_SET_TS(pRtpHdrOUT, htonl(_RtpHdrIN->ts));
	RTP_HDR_SET_SSRC(pRtpHdrOUT, htonl(_RtpHdrIN->ssrc));
}


/****************************************************
 *Ãû³Æ£ºRtpBuildTcpFlag
 *¹¦ÄÜ£ºRTSPÍ¨¹ýTCP·½Ê½Á¬½ÓÊ±Ìî³äRTPÍ·Ç°µÄ4¸ö×Ö½Ú
 *²ÎÊý£º 
 			_pcBuffer			:		RTPÍ·Ç°µÄ4¸ö×Ö½Ú
 
 			_iInterleavedNo		:		_iInterleavedNo

 			_iPacketLen		:		Õû¸öRTP°üµÄ³¤¶È(rtpÍ·³¤¶È+au_header/fu³¤¶È+ÒôÊÓÆµÊý¾Ý³¤¶È)
            
 *·µ»ØÖµ:	ÎÞ
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpBuildTcpFlag(char *_pcBuffer, int _iInterleavedNo, int _iPacketLen)
{
	if(_pcBuffer != NULL)
	{
		_pcBuffer[0] = '$';
		_pcBuffer[1] = _iInterleavedNo;
		_pcBuffer[2] =( ((unsigned int)_iPacketLen) >> 8) & 0xff;
		_pcBuffer[3] = ((unsigned int)_iPacketLen) & 0xff;
		
	}
}

/****************************************************
 *Ãû³Æ£ºRtpBuildFuHeader
 *¹¦ÄÜ£º¹¹½¨2×Ö½ÚµÄFU
 *²ÎÊý£º 
 			_pcBuffer			:		FU
 
 			_ucFrameType		:		Ö¡ÀàÐÍ

 			_iLastflag			:		ÊÇ·ñÎª×îºóÒ»°ü
            
 *·µ»ØÖµ:	ÎÞ
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpBuildFuHeader(char *_pcBuffer, unsigned char _ucFrameType, int _iLastflag)
{
	
	char *pcBuffer = _pcBuffer;
	
	pcBuffer[0] = 0;
	pcBuffer[1] = 0;
		
	//FU Indicator:  F   NRI    Type 
	pcBuffer[0] = (0x1f & 28);//28:FU_A   
	
	if(_ucFrameType == FRAME_TYPE_I)
	{
		pcBuffer[0] |= 0x60;//NRI:0 11 00000 ´Ë°üÊý¾ÝºÜÖØÒª
	}
	else
	{
		pcBuffer[0] |= 0x40;//NRI: 0 10 00000´Ë°üÊý¾Ý±È½ÏÖØÒª
	}

	//FU Header : S E R Type
	pcBuffer[1] = 0x1f & 96;
	if (_iLastflag)
	{
		pcBuffer[1] |= 0x40;//E = 1
	}
	
	if(_ucFrameType == FRAME_TYPE_I)
	{
		pcBuffer[1]  |= 0x5;//type:I
	}
	else
	{
		pcBuffer[1]  |= 0x1;//type:P
	}
	
}


/****************************************************
 *Ãû³Æ£ºRtpInitIovec
 *¹¦ÄÜ£º³õÊ¼»¯TIovec
 *²ÎÊý£º 
 			_ptIovec:	ÐèÒª±»³õÊ¼»¯µÄ_iIovCnt½á¹¹Ìå
 
 			_iIovCnt:		_iIovCnt

 			
            
 *·µ»ØÖµ:	ÎÞ
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpInitIovec(TIovec *_ptIovec, int _iIovCnt)
{
	if(_ptIovec != NULL)
	{
		int i = 0;
		for(i = 0; i < _iIovCnt; i++)
		{
			_ptIovec[i].iov_base = NULL;
			_ptIovec[i].iov_len = 0;
		}
	}
}



/****************************************************
 *Ãû³Æ£ºRtpCombinIovPacket
 *¹¦ÄÜ£º×éTIovec°ü
 *²ÎÊý£º 
 			_ptIovec:		Î´Ìî³äµÄTIovec°ü
 
 			_iIovIndex:		_ptIovecµÄÏÂ±ê

 			_pSendData:		Òª·¢ËÍµÄÊý¾Ý

 			_iSendDataLen:	Òª·¢ËÍµÄÊý¾ÝµÄ³¤¶È
            
 *·µ»ØÖµ:	void
 *±¸×¢:	
 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpCombinIovPacket(TIovec *_ptIovec, int _iIovIndex, void *_pSendData, int _iSendDataLen)
{
	if(_ptIovec != NULL && _iIovIndex >= 0 && _pSendData != NULL && _iSendDataLen != 0)
	{
		if(_iIovIndex >= 0)
		{
			_ptIovec[_iIovIndex].iov_base = _pSendData;
			_ptIovec[_iIovIndex].iov_len  = _iSendDataLen;
		}
	}
}


/****************************************************
 *Ãû³Æ£ºRtpSendMediaPacket
 *¹¦ÄÜ£º½«Êý¾ÝÌí¼ÓRTPÍ··¢ËÍ
 *²ÎÊý£º 
 			_ptRtspSession:	RTSPSession
 
 			_iAVflag:		ÒôÊÓÆµ±êÊ¶Î»
 			
 			_Marker:			Maker
 			
 			_pSendData1:		Òª·¢ËÍµÄÊý¾Ý1
 			
 			_iSendDataLen1:	Êý¾Ý1µÄ³¤¶È
 			
 			_pSendData2:		Òª·¢ËÍµÄÊý¾Ý2
 			
 			_iSendDataLen2:	Êý¾Ý2µÄ³¤¶È
 			
Ò»°üRTPÊý¾Ýtcp·¢ËÍ
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ËÄ×Ö½Ú	|  RTPHeader   |    _pSendData1    	|      _pSendData2
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

--------------------------------------------------------------------
H264:
	1¡¢·¢ËÍSPS¡¢PPSµÈÐÅÏ¢Ê±
	_pSendData1:			SPS¡¢PPS
	_iSendDataLen1:		SPSµÈÐÅÏ¢µÄ³¤¶È
	_pSendData2:			NULL
	_iSendDataLen2:		0
	
	2¡¢FU_AÄ£Ê½(·ÖÆ¬·â°üÄ£Ê½)
	_pSendData1:			fu indicator ºÍfu header
	_iSendDataLen1:		2¸ö×Ö½Ú³¤¶È
	_pSendData2:			h264Êý¾Ý
	_iSendDataLen2:		h264Êý¾Ý³¤¶È

	3¡¢µ¥Ò»NALÊ±
	_pSendData1:			h264Êý¾Ý
	_iSendDataLen1:		h264Êý¾Ý³¤¶È
	_pSendData2:			NULL
	_iSendDataLen2:		0
--------------------------------------------------------------------
AAC:
	_pSendData1:			4×Ö½Ú¸öau_header
	_iSendDataLen1:		4
	_pSendData2:			aacÊý¾Ý
	_iSendDataLen2:		aacÊý¾Ý³¤¶È
--------------------------------------------------------------------
G711A¡¢G711U¡¢ADPCM:
	_pSendData1:			ÒôÆµÊý¾Ý
	_iSendDataLen1:		ÒôÆµÊý¾Ý³¤¶È
	_pSendData2:			NULL
	_iSendDataLen2:		0
--------------------------------------------------------------------

//h264·ÖÆ¬Ä£Ê½
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ËÄ×Ö½Ú	|  RTPHeader   |FU:2×Ö½Ú(fu indicator ºÍfu header)|      FUºóµÄh264Êý¾Ý
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//AACÒôÆµÊý¾Ý
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ËÄ×Ö½Ú	|  RTPHeader   |         4×Ö½ÚAU_header		     |      AACÊý¾Ý
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 			
            
 *·µ»ØÖµ:	·¢ËÍÊý¾Ý³¤¶È
 *±¸×¢:

 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpSendMediaPacket(void *_ptRtspSession, int  _iAVflag, bool _Marker, 
						void *_pSendData1, int _iSendDataLen1,
						void *_pSendData2, int _iSendDataLen2)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	TRtspSession 	*ptRtspSession 	= NULL;	
	TConnection 	*ptConnect 		= NULL;
	TRtpHeader 		*ptRtpHdrIN 	= NULL;
	TRtpHeader 		RtpHdrOUT;

	int iNumber 		= 0;
	int iWriteVLen 		= -1;
	int iInterleavedNo 	= -2;
	
	char 	cRtpTcpBuffer[4] = {0};
	TIovec 	tIovec[5];
	
	if(_ptRtspSession == NULL)
	{
		return TD_ERROR;
	}
	ptRtspSession = (TRtspSession *)_ptRtspSession;
	
	RtpInitIovec(tIovec, 5);
	
	if(_iAVflag == V_FLAG)//video
	{
		iInterleavedNo = ptRtspSession->m_tRtspInfo.m_iInterleavedVNo;//video rtp no, no+1: rtcp
		ptConnect = &(ptRtspSession->m_tVRtpConnect);
		ptRtpHdrIN = &(ptRtspSession->m_tRtpInfo.m_tVRtpHeader);
	}
	else if(_iAVflag == A_FLAG)//audio
	{	
		iInterleavedNo = ptRtspSession->m_tRtspInfo.m_iInterleavedANo;
		ptConnect = &(ptRtspSession->m_tARtpConnect);
		ptRtpHdrIN = &(ptRtspSession->m_tRtpInfo.m_tARtpHeader);
	}
	else
	{
		td_printf(0, "it is neither video nor audio, _iAVflag = [%d]", _iAVflag);
	}
	
	if(ptRtspSession->m_iNetMode == NET_TCP)//TCP
	{
		ptConnect = ptRtspSession->m_ptConnect;//Ê¹ÓÃTCPµÄconnect

		//Ìî³äRTPÍ·Ç°ÃæµÄ4×Ö½Ú
		RtpBuildTcpFlag(cRtpTcpBuffer, iInterleavedNo, _iSendDataLen1 + _iSendDataLen2 + sizeof(TRtpHeader));

		//tcp
		RtpCombinIovPacket(tIovec, iNumber, cRtpTcpBuffer, 4);
		iNumber ++;
	}
	else if(ptRtspSession->m_iNetMode == NET_UDP)//UDP
	{
		//udp ²»ÐèÒª4×Ö½ÚÖ±½Ó·âRTP°ü
	}

	//rtpÍ·
	RtpBuildHeader(ptRtpHdrIN, &RtpHdrOUT, _Marker);		
	RtpCombinIovPacket(tIovec, iNumber, &RtpHdrOUT, sizeof(TRtpHeader));
	iNumber ++;
	
	//Êý¾Ý1
	if(_pSendData1 != NULL && _iSendDataLen1 != 0)
	{
		RtpCombinIovPacket(tIovec, iNumber, _pSendData1, _iSendDataLen1);
		iNumber ++;
	}

	//Êý¾Ý2
	if(_pSendData2 != NULL && _iSendDataLen2 != 0)
	{
		RtpCombinIovPacket(tIovec, iNumber, _pSendData2, _iSendDataLen2);
		iNumber ++;
	}

	if(iNumber < 6 && ptConnect != NULL)
	{
		iWriteVLen = RtpWriteV(ptConnect, tIovec, iNumber);

		ptRtpHdrIN->seqno++;
	}
	else
	{
		td_printf(0, "ptRtspSession->m_ptConnect == [%p], iNumber == [%d]", ptRtspSession->m_ptConnect, iNumber);
	}

	return iWriteVLen;
	
}


/****************************************************
 *Ãû³Æ£ºRtpSendMediaPacketFUA
 *¹¦ÄÜ£ºFU_A·ÖÆ¬·¢ËÍ
 *²ÎÊý£		_ptRtspSession		:	Session»á»°
 			_iAVFlag			:	ÒôÊÓÆµ±êÊ¶
 			_pSendData			:	Òª·ÖÆ¬·¢ËÍµÄÊý¾Ý
 			_iSendDataLen		:	Êý¾ÝµÄ³¤¶È
  			
            
 *·µ»ØÖµ:	·¢ËÍÊý¾Ý³¤¶È
 *±¸×¢:

 *ÐÞ¸ÄÀúÊ·£º
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpSendMediaPacketFUA(void *_ptRtspSession,  int _iAVFlag, 
								void *_pSendData, int _iSendDataLen)
{
//	td_printf(0, "\n\n FU_A started...");
	
	TRtspSession 	*ptRtspSession 	= (TRtspSession *)_ptRtspSession;
	char 			*pcSendData		= (char *)_pSendData;
	int 			iSendDataLen 	= _iSendDataLen;
	
	TRtspServer 	*ptServer 		= NULL;
	TChannel 		*ptChannel 		= NULL;

	unsigned char 	ucFrameType;
	char 			ucFU[FU_LEN]	= {0};
	
	int iSinglePacketLen = MAX_PAYLOAD_LEN;
	int iFirstPacket 	= 1;//S
	int iLastPacket 	= 0;//E

	if(ptRtspSession == NULL || pcSendData == NULL || iSendDataLen <= 0)
	{
		return TD_ERROR;
	}
	
	ptServer = (TRtspServer *)ptRtspSession->m_pServer;
	ptChannel = &(ptServer->m_tChanel[ptRtspSession->m_iStream][ptRtspSession->m_iChan]);

	ucFrameType = ptChannel->m_tFrameHeader.u8FrameType;

	while(iSendDataLen > 0)
	{

		if(iSendDataLen <= MAX_PAYLOAD_LEN)
		{
			iLastPacket = 1;
			iSinglePacketLen = iSendDataLen;
		}

		RtpBuildFuHeader(ucFU, ucFrameType, iLastPacket);

		if (iFirstPacket)//µÚÒ»¸ö
		{
			ucFU[1]  |= 0x80;//S = 1;
		}
		
		RtpSendMediaPacket(ptRtspSession, _iAVFlag, iLastPacket, ucFU, FU_LEN, pcSendData + iFirstPacket, iSinglePacketLen - iFirstPacket);
	
		pcSendData 		+= iSinglePacketLen;		
		iSendDataLen 	-= iSinglePacketLen;
		
		iFirstPacket = 0;

	}

	//td_printf(0, "\nFU_A over\n\n");	
	return TD_OK;
	
}


