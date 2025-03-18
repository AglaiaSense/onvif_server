/*
 *	file: rtp.h
 *	description: rtp
 *	history: dongliqiang created at 20130219
 */
 
#ifndef __RTP_H__
#define __RTP_H__

#include "connection.h"



#define MAX_PAYLOAD_LEN 	1400
#define MAX_BOX_LEN 		1400

#define FU_LEN 				2
#define NALU_HEADER_LEN 	4

#define SPS_FLAG 			7
#define PPS_FLAG 			8
#define IFRAME_FLAG 		5
#define PFRAME_FLAG 		1

#define V_FLAG 				0
#define A_FLAG 				1


#define RTP_VERSION 2
#define RTP_HDR_SET_VERSION(pHDR, val)  ((pHDR)->version = val)
#define RTP_HDR_SET_P(pHDR, val)        ((pHDR)->p       = val)
#define RTP_HDR_SET_X(pHDR, val)        ((pHDR)->x       = val) 
#define RTP_HDR_SET_CC(pHDR, val)       ((pHDR)->cc      = val)
#define RTP_HDR_SET_M(pHDR, val)        ((pHDR)->marker  = val)
#define RTP_HDR_SET_PT(pHDR, val)       ((pHDR)->pt      = val)
#define RTP_HDR_SET_SEQNO(pHDR, _sn)    ((pHDR)->seqno  = (_sn))
#define RTP_HDR_SET_TS(pHDR, _ts)       ((pHDR)->ts     = (_ts))
#define RTP_HDR_SET_SSRC(pHDR, _ssrc)    ((pHDR)->ssrc  = _ssrc)
#define RTP_HDR_LEN  sizeof(TRtpHeader)
#define NALU_SIZE 2

typedef enum
{
	FRAME_TYPE_I = 0,
	FRAME_TYPE_P,
	FRAME_TYPE_B
}FRAME_TYPE;

typedef struct RtpHeader
{
#if (BYTE_ORDER == LITTLE_ENDIAN)
	unsigned short 	cc      :4;   // CSRC count
	unsigned short 	x       :1;   // header extension flag
	unsigned short 	p       :1;   // padding flag
	unsigned short 	version :2;   // protocol version
	unsigned short 	pt      :7;   // payload type
	unsigned short 	marker  :1;   // marker bit
#elif (BYTE_ORDER == BIG_ENDIAN)
	unsigned short 	version :2;   // protocol version
	unsigned short 	p       :1;   // padding flag
	unsigned short 	x       :1;   // header extension flag
	unsigned short 	cc      :4;   // CSRC count
	unsigned short 	marker  :1;   // marker bit
	unsigned short 	pt      :7;   // payload type
#else
	#error YOU MUST DEFINE BYTE_ORDER == LITTLE_ENDIAN OR BIG_ENDIAN !
#endif
	unsigned short 	seqno  :16;   // sequence number
	unsigned int 	ts;           // timestamp in ms
	unsigned int 	ssrc;         // synchronization source
}__attribute__((packed)) TRtpHeader;

//�洢С��������BUG
//��JPEGͷ
typedef struct jpeghdr {
	unsigned int	tspec:8;	/* type-specific field */
	unsigned int	off:24;		/* fragment byte offset */   
	unsigned char	type;		/* id of jpeg decoder params */
	unsigned char	q;			/* quantization factor (or table id) */
	unsigned char	width;		/* frame width in 8 pixel blocks */
	unsigned char	height;		/* frame height in 8 pixel blocks */
}JpegMainHeader_t;

//��λ���ͷ
typedef struct jpeghdr_rst {
	unsigned short dri;
#if (BYTE_ORDER == LITTLE_ENDIAN)
	unsigned short count		:14;  
	unsigned short l			:1;   
	unsigned short f			:1;  
#elif (BYTE_ORDER == BIG_ENDIAN)
	unsigned short f			:1;   
	unsigned short l			:1;  
	unsigned short count		:14;
#else
#error YOU MUST DEFINE BYTE_ORDER == LITTLE_ENDIAN OR BIG_ENDIAN !  
#endif       
}JpegResetHeader_t;

//������ͷ
typedef struct jpeghdr_qtable {
	unsigned char  mbz;
	unsigned char  precision;
	unsigned short length;
}JpegQtableHeader_t;

//����JPEGͷ
typedef struct
{
	JpegMainHeader_t 		m_tJpegMainHead;
	JpegResetHeader_t 		m_tJpegResetHead;
	JpegQtableHeader_t 		m_tJpegQtableHead;
	unsigned char 			m_ucQtable[128]; 
	unsigned char 			m_ucHuftable[0x1A4];
	
}TRtpJpegHeader_t;



typedef struct RtpInfo
{
	TRtpHeader		m_tVRtpHeader;	
	TRtpHeader 		m_tARtpHeader;   
	TRtpJpegHeader_t	m_tJpegHeader;
} TRtpInfo;

typedef struct 
{
	char 			m_cMagic;			//'$'
	char 			m_cChanType;		//ͨ������:0-video rtp, 1-video rtcp, 2-audio rtp, 3-audio rtcp
	unsigned short 	m_usDataLen;		//���س���,���65535 bytes - 64k
}TRtspHeader;

//ע:
//hismall		:	sizeof(TFuHeader) == 4;
//hisiv100nptl	:	sizeof(TFuHeader) == 2;
typedef struct
{
	char			m_cIndicator;
	char			m_cHeader;
}TFuHeader;



int RtpInit(TConnection *_ptConnect, int _iNetType, void *_pUserData);

int RtpUnInit(TConnection *_ptConnect);

int RtpConnect(TConnection *_ptConnect, char *_pcServerIp, int _iServerPort);

int RtpDisconnect(TConnection *_ptConnect);

int RtpRecv(TConnection *_ptConnect, unsigned char *_ptData, int _iLen);

int RtpSend(TConnection *_ptConnect, unsigned char *_ptData, int _iLen);

int RtpReadV(TConnection *_ptConnect, TIovec *_ptIovec, int _iIovCnt);


int RtpWriteV(TConnection *_ptConnect, const TIovec *_ptIovec, int _iIovCnt);

unsigned short Myntols(unsigned short _usInput);


/****************************************************
 *���ƣ�RtpGenSessionID
 *���ܣ�����ͬ����ʶԴ
 *��������
 
 *����ֵ:ͬ����ʶԴ
 *��ע:	
 *�޸���ʷ��
		��utils.c����ֲby qiaohaijun @2013.3.26
****************************************************/
extern unsigned int RtpGenSessionID(void);


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
int RtpInitHeaderWithAecType(TRtpHeader *_pRtpHdr, int _iEncodeType);

/****************************************************
 *���ƣ�RtpInitHeaderWithVecType
 *���ܣ�������������(��������/PT)��ʼ��RTPͷ
 *������ 
 			_pRtpHdr			:	����RTPͷ��Ϣ

			_iEncodeType 		: 	���뷽ʽ

			_ptJpegHdr		:	Jpegͷ

            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpInitHeaderWithVecType(TRtpHeader *_pRtpHdr, int _iEncodeType, TRtpJpegHeader_t *_ptJpegHdr);


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
extern void RtpBuildHeader(TRtpHeader *_RtpHdrIN, TRtpHeader * _pRtpHdrOUT, bool _Marker);

#if 0
/****************************************************
 *���ƣ�RtpSendMediaPacket
 *���ܣ����������RTPͷ����
 *������ _ptRtspSession:RTSPSession
 			_RtpHdrIN:����RTPͷ��Ϣ
 			_Marker:Maker
 			_pSendData1:Ҫ���͵�����1
 			_iSendDataLen1:����1�ĳ���
 			_pSendData2:Ҫ���͵�����2
 			_iSendDataLen2:����2�ĳ���
һ��RTP����
++++++++++++++++++++++++++++++++++++++++
   RTPHeader   |    _pSendData1    |      _pSendData2
++++++++++++++++++++++++++++++++++++++++
 			
            
 *����ֵ:	iRet:�������ݳ���
 *��ע:

 *�޸���ʷ��
	by qiaohaijun @2013.3.27
	........
****************************************************/
extern int RtpSendMediaPacket(void *_ptRtspSession, TRtpHeader *_RtpHdrIN, bool _Marker, void *_pSendData1, int _iSendDataLen1, void *_pSendData2, int _iSendDataLen2);
#endif

/****************************************************
 *���ƣ�RtpBuildTcpFlag
 *���ܣ�RTSPͨ��TCP��ʽ����ʱ���RTPͷǰ��4���ֽ�
 *������ 
 			_pcBuffer:			RTPͷǰ��4���ֽ�
 
 			_iInterleavedNo:		_iInterleavedNo

 			_iPacketLen:			����RTP���ĳ���
            
 *����ֵ:	��
 *��ע:	
 *�޸���ʷ��
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpBuildTcpFlag(char *_pcBuffer, int _iInterleavedNo, int _iPacketLen);


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
void RtpInitIovec(TIovec *_ptIovec, int _iIovCnt);


/****************************************************
 *���ƣ�RtpCombinIovPacket
 *���ܣ���TIovec��
 *������ 
 			_ptIovec:		δ����TIovec��
 
 			_iIovIndex:		_ptIovec���±�

 			_pSendData:		Ҫ���͵�����

 			_iSendDataLen:	Ҫ���͵����ݵĳ���
            
 *����ֵ:	�������ݵĳ���
 *��ע:	
 *�޸���ʷ��
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpCombinIovPacket(TIovec *_ptIovec, int _iIovIndex, void *_pSendData, int _iSendDataLen);


#if 0
/****************************************************
 *���ƣ�RtpWriteV
 *���ܣ���������
 *������ 
 			_ptConnect:	rtsp connect	
 
 			_ptIovec:	���õ�TIovec��

 			_iIovCnt:		_iIovCnt
            
 *����ֵ:	�������ݵĳ���
 *��ע:	
 *�޸���ʷ��
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpWriteV(TConnection *_ptConnect, TIovec *_ptIovec, int _iIovCnt);
#endif



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
 			
һ��RTP����
++++++++++++++++++++++++++++++++++++++++++++++++++++++
   RTPHeader   |    _pSendData1    |      _pSendData2
++++++++++++++++++++++++++++++++++++++++++++++++++++++
 			
            
 *����ֵ:	�������ݳ���
 *��ע:

 *�޸���ʷ��
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpSendMediaPacket(void *_ptRtspSession, int  _iAVflag, bool _Marker, 
						void *_pSendData1, int _iSendDataLen1,
						void *_pSendData2, int _iSendDataLen2);



/****************************************************
 *���ƣ�RtpSendMediaPacketFUA
 *���ܣ�FU_A��Ƭ����
 *������ _ptRtspSession	:	Session�Ự
 			_iAVFlag			:	����Ƶ��ʶ
 			_pSendData		:	Ҫ��Ƭ���͵�����
 			_iSendDataLen	:	���ݵĳ���
  			
            
 *����ֵ:	�������ݳ���
 *��ע:

 *�޸���ʷ��
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpSendMediaPacketFUA(void *_ptRtspSession,  int _iAVFlag, 
								void *_pSendData, int _iSendDataLen);




#endif

