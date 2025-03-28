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
 *名称：RtpWriteV
 *功能：发送数据
 *参数： 
 			_ptConnect:	rtsp connect	
 
 			_ptIovec:	填充好的TIovec包

 			_iIovCnt:		_iIovCnt
            
 *返回值:	成功: 发送数据的长度
 				失败:TD_ERROR
 *备注:	
 *修改历史：
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
 *名称：Media_Gen_Session_ID
 *功能：产生同步标识源
 *参数：空
 
 *返回值:同步标识源
 *备注:	
 *修改历史：
		从utils.c中移植by qiaohaijun @2013.3.26
****************************************************/
unsigned int RtpGenSessionID(void)
{
	 struct timespec spec;
	 clock_gettime(CLOCK_REALTIME, &spec);
	 return (unsigned int)(spec.tv_nsec);
}

/****************************************************
 *名称：RtpInitHeader
 *功能：初始化RTP头信息
 *参数： _pRtpHdr:输入RTP头信息
 			_ucPT:payload type
 			 		

            
 *返回值:	无
 *备注:	
 *修改历史：
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
 *名称：RtpInitJpegHeader
 *功能：初始化JPEG的RTP中的描述JPEG头部分
 *参数：  pJHdr:描述头指针

 *返回值:0
 *备注:	2.0将长宽改到实时获取
 *修改历史：
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
 *名称：RtpInitRstHeader
 *功能：初始化JPEG的RTP中的分块头部分
 *参数：  pJRHdr:RTP的分块表头指针

 *返回值:0
 *备注:	
 *修改历史：
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
 *名称：RtpInitDqtHeader
 *功能：初始化JPEG的RTP中的量化表头部分
 *参数：  pJQHdr:RTP的量化表头指针

 *返回值:0
 *备注:	
 *修改历史：
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
 *名称：RtpInitJpegHead
 *功能：初始化JPEG的RTP头
 *参数：  pRTPJHdr:RTP头指针
 		
 *返回值:0
 *备注:	去掉长宽
 *修改历史：
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
 *名称：RtpInitHeaderWithAecType
 *功能：根据数据类型(编码类型/PT)初始化RTP头
 *参数： 
 			_pRtpHdr			:	输入RTP头信息

			_iEncodeType 		: 	编码方式

            
 *返回值:	无
 *备注:	
 *修改历史：
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
 *名称：RtpInitHeaderWithVecType
 *功能：根据数据类型(编码类型/PT)初始化RTP头
 *参数： 
 			_pRtpHdr			:	输入RTP头信息

			_iEncodeType 		: 	编码方式

            
 *返回值:	无
 *备注:	
 *修改历史：
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
 *名称：RtpUnInitHeader
 *功能：初始化RTP头信息
 *参数： _pRtpHdr:输入RTP头信息
 			 		

            
 *返回值:	无
 *备注:	
 *修改历史：
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
 *名称：RtpBuildHeader
 *功能：添加RTP头信息
 *参数： _RtpHdrIN:输入RTP头信息
 			_pRtpHdrOUT:输出RTP头信息
 			_Marker:Maker
            
 *返回值:	无
 *备注:	
 *修改历史：
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
 *名称：RtpBuildTcpFlag
 *功能：RTSP通过TCP方式连接时填充RTP头前的4个字节
 *参数： 
 			_pcBuffer			:		RTP头前的4个字节
 
 			_iInterleavedNo		:		_iInterleavedNo

 			_iPacketLen		:		整个RTP包的长度(rtp头长度+au_header/fu长度+音视频数据长度)
            
 *返回值:	无
 *备注:	
 *修改历史：
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
 *名称：RtpBuildFuHeader
 *功能：构建2字节的FU
 *参数： 
 			_pcBuffer			:		FU
 
 			_ucFrameType		:		帧类型

 			_iLastflag			:		是否为最后一包
            
 *返回值:	无
 *备注:	
 *修改历史：
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
		pcBuffer[0] |= 0x60;//NRI:0 11 00000 此包数据很重要
	}
	else
	{
		pcBuffer[0] |= 0x40;//NRI: 0 10 00000此包数据比较重要
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
 *名称：RtpInitIovec
 *功能：初始化TIovec
 *参数： 
 			_ptIovec:	需要被初始化的_iIovCnt结构体
 
 			_iIovCnt:		_iIovCnt

 			
            
 *返回值:	无
 *备注:	
 *修改历史：
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
 *名称：RtpCombinIovPacket
 *功能：组TIovec包
 *参数： 
 			_ptIovec:		未填充的TIovec包
 
 			_iIovIndex:		_ptIovec的下标

 			_pSendData:		要发送的数据

 			_iSendDataLen:	要发送的数据的长度
            
 *返回值:	void
 *备注:	
 *修改历史：
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
 *名称：RtpSendMediaPacket
 *功能：将数据添加RTP头发送
 *参数： 
 			_ptRtspSession:	RTSPSession
 
 			_iAVflag:		音视频标识位
 			
 			_Marker:			Maker
 			
 			_pSendData1:		要发送的数据1
 			
 			_iSendDataLen1:	数据1的长度
 			
 			_pSendData2:		要发送的数据2
 			
 			_iSendDataLen2:	数据2的长度
 			
一包RTP数据tcp发送
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 四字节	|  RTPHeader   |    _pSendData1    	|      _pSendData2
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

--------------------------------------------------------------------
H264:
	1、发送SPS、PPS等信息时
	_pSendData1:			SPS、PPS
	_iSendDataLen1:		SPS等信息的长度
	_pSendData2:			NULL
	_iSendDataLen2:		0
	
	2、FU_A模式(分片封包模式)
	_pSendData1:			fu indicator 和fu header
	_iSendDataLen1:		2个字节长度
	_pSendData2:			h264数据
	_iSendDataLen2:		h264数据长度

	3、单一NAL时
	_pSendData1:			h264数据
	_iSendDataLen1:		h264数据长度
	_pSendData2:			NULL
	_iSendDataLen2:		0
--------------------------------------------------------------------
AAC:
	_pSendData1:			4字节个au_header
	_iSendDataLen1:		4
	_pSendData2:			aac数据
	_iSendDataLen2:		aac数据长度
--------------------------------------------------------------------
G711A、G711U、ADPCM:
	_pSendData1:			音频数据
	_iSendDataLen1:		音频数据长度
	_pSendData2:			NULL
	_iSendDataLen2:		0
--------------------------------------------------------------------

//h264分片模式
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 四字节	|  RTPHeader   |FU:2字节(fu indicator 和fu header)|      FU后的h264数据
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//AAC音频数据
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 四字节	|  RTPHeader   |         4字节AU_header		     |      AAC数据
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 			
            
 *返回值:	发送数据长度
 *备注:

 *修改历史：
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
		ptConnect = ptRtspSession->m_ptConnect;//使用TCP的connect

		//填充RTP头前面的4字节
		RtpBuildTcpFlag(cRtpTcpBuffer, iInterleavedNo, _iSendDataLen1 + _iSendDataLen2 + sizeof(TRtpHeader));

		//tcp
		RtpCombinIovPacket(tIovec, iNumber, cRtpTcpBuffer, 4);
		iNumber ++;
	}
	else if(ptRtspSession->m_iNetMode == NET_UDP)//UDP
	{
		//udp 不需要4字节直接封RTP包
	}

	//rtp头
	RtpBuildHeader(ptRtpHdrIN, &RtpHdrOUT, _Marker);		
	RtpCombinIovPacket(tIovec, iNumber, &RtpHdrOUT, sizeof(TRtpHeader));
	iNumber ++;
	
	//数据1
	if(_pSendData1 != NULL && _iSendDataLen1 != 0)
	{
		RtpCombinIovPacket(tIovec, iNumber, _pSendData1, _iSendDataLen1);
		iNumber ++;
	}

	//数据2
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
 *名称：RtpSendMediaPacketFUA
 *功能：FU_A分片发送
 *参数�		_ptRtspSession		:	Session会话
 			_iAVFlag			:	音视频标识
 			_pSendData			:	要分片发送的数据
 			_iSendDataLen		:	数据的长度
  			
            
 *返回值:	发送数据长度
 *备注:

 *修改历史：
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

		if (iFirstPacket)//第一个
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


