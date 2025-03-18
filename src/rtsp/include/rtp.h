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

//存储小端数据有BUG
//主JPEG头
typedef struct jpeghdr {
	unsigned int	tspec:8;	/* type-specific field */
	unsigned int	off:24;		/* fragment byte offset */   
	unsigned char	type;		/* id of jpeg decoder params */
	unsigned char	q;			/* quantization factor (or table id) */
	unsigned char	width;		/* frame width in 8 pixel blocks */
	unsigned char	height;		/* frame height in 8 pixel blocks */
}JpegMainHeader_t;

//复位标记头
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

//量化表头
typedef struct jpeghdr_qtable {
	unsigned char  mbz;
	unsigned char  precision;
	unsigned short length;
}JpegQtableHeader_t;

//整个JPEG头
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
	char 			m_cChanType;		//通道类型:0-video rtp, 1-video rtcp, 2-audio rtp, 3-audio rtcp
	unsigned short 	m_usDataLen;		//负载长度,最大65535 bytes - 64k
}TRtspHeader;

//注:
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
 *名称：RtpGenSessionID
 *功能：产生同步标识源
 *参数：空
 
 *返回值:同步标识源
 *备注:	
 *修改历史：
		从utils.c中移植by qiaohaijun @2013.3.26
****************************************************/
extern unsigned int RtpGenSessionID(void);


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
int RtpInitHeaderWithAecType(TRtpHeader *_pRtpHdr, int _iEncodeType);

/****************************************************
 *名称：RtpInitHeaderWithVecType
 *功能：根据数据类型(编码类型/PT)初始化RTP头
 *参数： 
 			_pRtpHdr			:	输入RTP头信息

			_iEncodeType 		: 	编码方式

			_ptJpegHdr		:	Jpeg头

            
 *返回值:	无
 *备注:	
 *修改历史：
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpInitHeaderWithVecType(TRtpHeader *_pRtpHdr, int _iEncodeType, TRtpJpegHeader_t *_ptJpegHdr);


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
extern void RtpBuildHeader(TRtpHeader *_RtpHdrIN, TRtpHeader * _pRtpHdrOUT, bool _Marker);

#if 0
/****************************************************
 *名称：RtpSendMediaPacket
 *功能：将数据添加RTP头发送
 *参数： _ptRtspSession:RTSPSession
 			_RtpHdrIN:输入RTP头信息
 			_Marker:Maker
 			_pSendData1:要发送的数据1
 			_iSendDataLen1:数据1的长度
 			_pSendData2:要发送的数据2
 			_iSendDataLen2:数据2的长度
一包RTP数据
++++++++++++++++++++++++++++++++++++++++
   RTPHeader   |    _pSendData1    |      _pSendData2
++++++++++++++++++++++++++++++++++++++++
 			
            
 *返回值:	iRet:发送数据长度
 *备注:

 *修改历史：
	by qiaohaijun @2013.3.27
	........
****************************************************/
extern int RtpSendMediaPacket(void *_ptRtspSession, TRtpHeader *_RtpHdrIN, bool _Marker, void *_pSendData1, int _iSendDataLen1, void *_pSendData2, int _iSendDataLen2);
#endif

/****************************************************
 *名称：RtpBuildTcpFlag
 *功能：RTSP通过TCP方式连接时填充RTP头前的4个字节
 *参数： 
 			_pcBuffer:			RTP头前的4个字节
 
 			_iInterleavedNo:		_iInterleavedNo

 			_iPacketLen:			整个RTP包的长度
            
 *返回值:	无
 *备注:	
 *修改历史：
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpBuildTcpFlag(char *_pcBuffer, int _iInterleavedNo, int _iPacketLen);


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
void RtpInitIovec(TIovec *_ptIovec, int _iIovCnt);


/****************************************************
 *名称：RtpCombinIovPacket
 *功能：组TIovec包
 *参数： 
 			_ptIovec:		未填充的TIovec包
 
 			_iIovIndex:		_ptIovec的下标

 			_pSendData:		要发送的数据

 			_iSendDataLen:	要发送的数据的长度
            
 *返回值:	发送数据的长度
 *备注:	
 *修改历史：
	by qiaohaijun @2013.3.27
	........
****************************************************/
void RtpCombinIovPacket(TIovec *_ptIovec, int _iIovIndex, void *_pSendData, int _iSendDataLen);


#if 0
/****************************************************
 *名称：RtpWriteV
 *功能：发送数据
 *参数： 
 			_ptConnect:	rtsp connect	
 
 			_ptIovec:	填充好的TIovec包

 			_iIovCnt:		_iIovCnt
            
 *返回值:	发送数据的长度
 *备注:	
 *修改历史：
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpWriteV(TConnection *_ptConnect, TIovec *_ptIovec, int _iIovCnt);
#endif



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
 			
一包RTP数据
++++++++++++++++++++++++++++++++++++++++++++++++++++++
   RTPHeader   |    _pSendData1    |      _pSendData2
++++++++++++++++++++++++++++++++++++++++++++++++++++++
 			
            
 *返回值:	发送数据长度
 *备注:

 *修改历史：
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpSendMediaPacket(void *_ptRtspSession, int  _iAVflag, bool _Marker, 
						void *_pSendData1, int _iSendDataLen1,
						void *_pSendData2, int _iSendDataLen2);



/****************************************************
 *名称：RtpSendMediaPacketFUA
 *功能：FU_A分片发送
 *参数： _ptRtspSession	:	Session会话
 			_iAVFlag			:	音视频标识
 			_pSendData		:	要分片发送的数据
 			_iSendDataLen	:	数据的长度
  			
            
 *返回值:	发送数据长度
 *备注:

 *修改历史：
	by qiaohaijun @2013.3.27
	........
****************************************************/
int RtpSendMediaPacketFUA(void *_ptRtspSession,  int _iAVFlag, 
								void *_pSendData, int _iSendDataLen);




#endif

