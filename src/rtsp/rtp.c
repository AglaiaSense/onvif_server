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
 *���ƣ�RtpWriteV
 *���ܣ���������
 *������ 
 			_ptConnect:	rtsp connect	
 
 			_ptIovec:	���õ�TIovec��

 			_iIovCnt:		_iIovCnt
            
 *����ֵ:	�ɹ�: �������ݵĳ���
 				ʧ��:TD_ERROR
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�Media_Gen_Session_ID
 *���ܣ�����ͬ����ʶԴ
 *��������
 
 *����ֵ:ͬ����ʶԴ
 *��ע:	
 *�޸���ʷ��
		��utils.c����ֲby qiaohaijun @2013.3.26
****************************************************/
unsigned int RtpGenSessionID(void)
{
	 struct timespec spec;
	 clock_gettime(CLOCK_REALTIME, &spec);
	 return (unsigned int)(spec.tv_nsec);
}

/****************************************************
 *���ƣ�RtpInitHeader
 *���ܣ���ʼ��RTPͷ��Ϣ
 *������ _pRtpHdr:����RTPͷ��Ϣ
 			_ucPT:payload type
 			 		

            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpInitJpegHeader
 *���ܣ���ʼ��JPEG��RTP�е�����JPEGͷ����
 *������  pJHdr:����ͷָ��

 *����ֵ:0
 *��ע:	2.0������ĵ�ʵʱ��ȡ
 *�޸���ʷ��
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
 *���ƣ�RtpInitRstHeader
 *���ܣ���ʼ��JPEG��RTP�еķֿ�ͷ����
 *������  pJRHdr:RTP�ķֿ��ͷָ��

 *����ֵ:0
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpInitDqtHeader
 *���ܣ���ʼ��JPEG��RTP�е�������ͷ����
 *������  pJQHdr:RTP��������ͷָ��

 *����ֵ:0
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpInitJpegHead
 *���ܣ���ʼ��JPEG��RTPͷ
 *������  pRTPJHdr:RTPͷָ��
 		
 *����ֵ:0
 *��ע:	ȥ������
 *�޸���ʷ��
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
 *���ƣ�RtpInitHeaderWithAecType
 *���ܣ�������������(��������/PT)��ʼ��RTPͷ
 *������ 
 			_pRtpHdr			:	����RTPͷ��Ϣ

			_iEncodeType 		: 	���뷽ʽ

            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpInitHeaderWithVecType
 *���ܣ�������������(��������/PT)��ʼ��RTPͷ
 *������ 
 			_pRtpHdr			:	����RTPͷ��Ϣ

			_iEncodeType 		: 	���뷽ʽ

            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpUnInitHeader
 *���ܣ���ʼ��RTPͷ��Ϣ
 *������ _pRtpHdr:����RTPͷ��Ϣ
 			 		

            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpBuildHeader
 *���ܣ����RTPͷ��Ϣ
 *������ _RtpHdrIN:����RTPͷ��Ϣ
 			_pRtpHdrOUT:���RTPͷ��Ϣ
 			_Marker:Maker
            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpBuildTcpFlag
 *���ܣ�RTSPͨ��TCP��ʽ����ʱ���RTPͷǰ��4���ֽ�
 *������ 
 			_pcBuffer			:		RTPͷǰ��4���ֽ�
 
 			_iInterleavedNo		:		_iInterleavedNo

 			_iPacketLen		:		����RTP���ĳ���(rtpͷ����+au_header/fu����+����Ƶ���ݳ���)
            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpBuildFuHeader
 *���ܣ�����2�ֽڵ�FU
 *������ 
 			_pcBuffer			:		FU
 
 			_ucFrameType		:		֡����

 			_iLastflag			:		�Ƿ�Ϊ���һ��
            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
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
		pcBuffer[0] |= 0x60;//NRI:0 11 00000 �˰����ݺ���Ҫ
	}
	else
	{
		pcBuffer[0] |= 0x40;//NRI: 0 10 00000�˰����ݱȽ���Ҫ
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
 *���ƣ�RtpInitIovec
 *���ܣ���ʼ��TIovec
 *������ 
 			_ptIovec:	��Ҫ����ʼ����_iIovCnt�ṹ��
 
 			_iIovCnt:		_iIovCnt

 			
            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpCombinIovPacket
 *���ܣ���TIovec��
 *������ 
 			_ptIovec:		δ����TIovec��
 
 			_iIovIndex:		_ptIovec���±�

 			_pSendData:		Ҫ���͵�����

 			_iSendDataLen:	Ҫ���͵����ݵĳ���
            
 *����ֵ:	void
 *��ע:	
 *�޸���ʷ��
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
 *���ƣ�RtpSendMediaPacket
 *���ܣ����������RTPͷ����
 *������ 
 			_ptRtspSession:	RTSPSession
 
 			_iAVflag:		����Ƶ��ʶλ
 			
 			_Marker:			Maker
 			
 			_pSendData1:		Ҫ���͵�����1
 			
 			_iSendDataLen1:	����1�ĳ���
 			
 			_pSendData2:		Ҫ���͵�����2
 			
 			_iSendDataLen2:	����2�ĳ���
 			
һ��RTP����tcp����
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ���ֽ�	|  RTPHeader   |    _pSendData1    	|      _pSendData2
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

--------------------------------------------------------------------
H264:
	1������SPS��PPS����Ϣʱ
	_pSendData1:			SPS��PPS
	_iSendDataLen1:		SPS����Ϣ�ĳ���
	_pSendData2:			NULL
	_iSendDataLen2:		0
	
	2��FU_Aģʽ(��Ƭ���ģʽ)
	_pSendData1:			fu indicator ��fu header
	_iSendDataLen1:		2���ֽڳ���
	_pSendData2:			h264����
	_iSendDataLen2:		h264���ݳ���

	3����һNALʱ
	_pSendData1:			h264����
	_iSendDataLen1:		h264���ݳ���
	_pSendData2:			NULL
	_iSendDataLen2:		0
--------------------------------------------------------------------
AAC:
	_pSendData1:			4�ֽڸ�au_header
	_iSendDataLen1:		4
	_pSendData2:			aac����
	_iSendDataLen2:		aac���ݳ���
--------------------------------------------------------------------
G711A��G711U��ADPCM:
	_pSendData1:			��Ƶ����
	_iSendDataLen1:		��Ƶ���ݳ���
	_pSendData2:			NULL
	_iSendDataLen2:		0
--------------------------------------------------------------------

//h264��Ƭģʽ
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ���ֽ�	|  RTPHeader   |FU:2�ֽ�(fu indicator ��fu header)|      FU���h264����
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//AAC��Ƶ����
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ���ֽ�	|  RTPHeader   |         4�ֽ�AU_header		     |      AAC����
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 			
            
 *����ֵ:	�������ݳ���
 *��ע:

 *�޸���ʷ��
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
		ptConnect = ptRtspSession->m_ptConnect;//ʹ��TCP��connect

		//���RTPͷǰ���4�ֽ�
		RtpBuildTcpFlag(cRtpTcpBuffer, iInterleavedNo, _iSendDataLen1 + _iSendDataLen2 + sizeof(TRtpHeader));

		//tcp
		RtpCombinIovPacket(tIovec, iNumber, cRtpTcpBuffer, 4);
		iNumber ++;
	}
	else if(ptRtspSession->m_iNetMode == NET_UDP)//UDP
	{
		//udp ����Ҫ4�ֽ�ֱ�ӷ�RTP��
	}

	//rtpͷ
	RtpBuildHeader(ptRtpHdrIN, &RtpHdrOUT, _Marker);		
	RtpCombinIovPacket(tIovec, iNumber, &RtpHdrOUT, sizeof(TRtpHeader));
	iNumber ++;
	
	//����1
	if(_pSendData1 != NULL && _iSendDataLen1 != 0)
	{
		RtpCombinIovPacket(tIovec, iNumber, _pSendData1, _iSendDataLen1);
		iNumber ++;
	}

	//����2
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
 *���ƣ�RtpSendMediaPacketFUA
 *���ܣ�FU_A��Ƭ����
 *�����		_ptRtspSession		:	Session�Ự
 			_iAVFlag			:	����Ƶ��ʶ
 			_pSendData			:	Ҫ��Ƭ���͵�����
 			_iSendDataLen		:	���ݵĳ���
  			
            
 *����ֵ:	�������ݳ���
 *��ע:

 *�޸���ʷ��
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

		if (iFirstPacket)//��һ��
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


