/*
 *	file: media_audio.c
 *	description: send audio
 *	history: by qiaohaijun @2013.4.7
 */
#include "media_audio.h"
#include "rtp.h"
#include "rtsp_server.h"


/****************************************************
  *���ƣ�AACGetSampleRate
  *���ܣ���AAC�����л�ȡ��Ƶ������
  *������_pcAacData	 :	 AAC����
                    
  *����ֵ:	��Ƶ������ 
  *�޸���ʷ��
	 by qiaohaijun @2013.8.22
	 ........
 ****************************************************/
int AACGetSampleRate(char *_pcAacData)
{
	int iAacSampleRate[16] = {96000, 88200, 64000, 48000, 
							44100, 32000, 24000, 22050, 
							16000, 2000, 11025, 8000, 0, 0, 0, 0};

	int iIndex = 0;

#if 1
	int i;
	for(i = 0; i <3; i++)
	{
		td_printf(0, "_pcAacData[i] ==== %x", _pcAacData[i]);
	}
#endif

	iIndex = (_pcAacData[2]>>2)&0xf;

	td_printf(0, "iIndex ========= %d", iIndex);

	if(iIndex >= 0 && iIndex <= 15)
	{
		return iAacSampleRate[iIndex];
	}
	
	return 0;
}

 
 int AACGetLength(char *_pcAacData, int _iLength)
 {
	 int iAacSingleLength = 0;
	 char *pcAacData = _pcAacData;
	 if(_iLength > ADTS_HEADER_LENGTH_7)
	 {	 
		 //13bits
		 //FrameLength = (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
		 iAacSingleLength = ((int)(pcAacData[3]&0x3)<<11) + ((int)pcAacData[4]<<3) + ((pcAacData[5]&0xE0) >> 5);
	 }
	 return iAacSingleLength;
 }
 
 
 /****************************************************
  *���ƣ�AACBuildAuHeader
  *���ܣ�����au_header
  *������_pcAuHeader	 :	 au_header
                    _iLenWithOutAdts	 :	 ȥ��adtsͷ��AAC���ݳ���
  *����ֵ:	 
  *�޸���ʷ��
	 by qiaohaijun @2013.4.7
	 ........
 ****************************************************/
 int AACBuildAuHeader(char *_pcAuHeader, int _iLenWithOutAdts)
 {
	 char *pcAuHeader = _pcAuHeader;
 
	 pcAuHeader[0] = 0;
	 pcAuHeader[1] = 16;
	 pcAuHeader[2] = (_iLenWithOutAdts >> 5) & 0xff;
	 pcAuHeader[3] = ((_iLenWithOutAdts & 0x1f) << 3) & 0xff;
 
	 return TD_OK;
 }
 
 /****************************************************
  *���ƣ�AAC4SendAudio
  *���ܣ���װAAC���ݲ�����
  *������_pcAudioData:��������ָ��
                    _ptRtspSession:�ص��Ķ������
  *����ֵ:	 
  *�޸���ʷ��
	 by qiaohaijun @2013.4.7
	 ........
 ****************************************************/
 int AAC4SendAudio(void *_ptRtspSession, char *_pcAudioData, int _iAudioStreamSize)
 {
	 TRtspSession *ptRtspSession = NULL; 
	 TRtspServer *ptRtspServer   = NULL;
	 TRtpHeader *ptRtpHeader     = NULL;
	 TChannel *ptChannel 	     = NULL;
	 char *pcAudioData			 = NULL;
	 char cAuHeader[AU_HEADER_LENGTH] = {0};
	 int iAacSingleFrameSize = -1;
	 int iAacSizeWithoutAdts = -1;
	 int iAacTmpSize         = -1;
	 
	 if(_ptRtspSession == NULL || _pcAudioData == NULL)
	 {
		 return TD_ERROR;
	 }
	 
	 ptRtspSession = (TRtspSession *)_ptRtspSession;
	 pcAudioData   = _pcAudioData;
	 ptRtspServer  = (TRtspServer *)ptRtspSession->m_pServer;
	 ptChannel     = &(ptRtspServer->m_tChanel[ptRtspSession->m_iStream][ptRtspSession->m_iChan]);
	 ptRtpHeader   = &(ptRtspSession->m_tRtpInfo.m_tARtpHeader);
 
	 iAacTmpSize = _iAudioStreamSize;
 
	 while(iAacTmpSize > ADTS_HEADER_LENGTH_7)
	 {
		 iAacSingleFrameSize = AACGetLength(pcAudioData, iAacTmpSize);
		 iAacSizeWithoutAdts = iAacSingleFrameSize - ADTS_HEADER_LENGTH_7;
	 
		 AACBuildAuHeader(cAuHeader, iAacSizeWithoutAdts);
		 RtpSendMediaPacket(ptRtspSession, A_FLAG, 1, \
							 cAuHeader, AU_HEADER_LENGTH, \
							 pcAudioData + ADTS_HEADER_LENGTH_7, iAacSizeWithoutAdts);
							 
		 ptRtpHeader->ts += 1024;
		 
		 pcAudioData += iAacSingleFrameSize;
		 iAacTmpSize -= iAacSingleFrameSize;
	 }
	 
	 return TD_OK;
 }
 
 /****************************************************
  *���ƣ�ADPCM_DIV4SendAudio
  *���ܣ���װADPCM���ݲ�����
  *������_pcAudioData:��������ָ��
                    _ptRtspSession:�ص��Ķ������
  *����ֵ:	 
  *�޸���ʷ��
	 by qiaohaijun @2013.4.7
	 ........
 ****************************************************/
 int ADPCM_DIV4SendAudio(void *_ptRtspSession, char *_pcAudioData)
 {
	 TRtspSession *ptRtspSession = NULL; 
	 TRtspServer *ptRtspServer   = NULL;
	 TChannel *ptChannel 	     = NULL;
	 TRtpHeader *ptRtpHeader     = NULL;
	 char *pcAudioData			 = NULL;
	 int iAudioSingleFrameSize	 = -1;
	 int iAudioFrames			 = -1;
	 int i = 0;
	 
	 if(_ptRtspSession == NULL || _pcAudioData == NULL)
	 {
		 return TD_ERROR;
	 }
	 
	 ptRtspSession = (TRtspSession *)_ptRtspSession;
	 pcAudioData   = _pcAudioData;
	 ptRtspServer  = (TRtspServer *)ptRtspSession->m_pServer;
	 ptChannel     = &(ptRtspServer->m_tChanel[ptRtspSession->m_iStream][ptRtspSession->m_iChan]);
	 ptRtpHeader   = &(ptRtspSession->m_tRtpInfo.m_tARtpHeader);
	 iAudioFrames  = ptChannel->m_tFrameHeader.u16AFrames;
	 iAudioSingleFrameSize = ptChannel->m_tFrameHeader.u16AFrameSize;
	 
	 for(i = 0; i < iAudioFrames; i++)
	 {
		 //ȥ��4�ֽڵĺ�˼ͷ
		 RtpSendMediaPacket(ptRtspSession, A_FLAG, 1, pcAudioData + 4, iAudioSingleFrameSize - 4, NULL, 0);
		 ptRtpHeader->ts += (iAudioSingleFrameSize - 4) << 1;
		 pcAudioData += iAudioSingleFrameSize;
	 }
 
	 return TD_OK;
 }

 /****************************************************
  *���ƣ�G711ASendAudio
  *���ܣ���װG711A���ݲ�����
  *������_ptRtspSession 		       :	 session
			 _pcAudioData			 :	 ��Ƶ����ָ��
  *����ֵ:	 
  *�޸���ʷ��
	 by qiaohaijun @2013.4.7
	 ........
 ****************************************************/
 int G711ASendAudio(void *_ptRtspSession, char *_pcAudioData)
 {
	 TRtspSession *ptRtspSession = NULL; 
	 TRtspServer *ptRtspServer   = NULL;
	 TChannel *ptChannel         = NULL;
	 TRtpHeader *ptRtpHeader     = NULL;
	 char *pcAudioData			 = NULL;
	 int iAudioSingleFrameSize	 = -1;
	 int iAudioFrames			 = -1;
	 int i = 0;
	 
	 if(_ptRtspSession == NULL || _pcAudioData == NULL)
	 {
		 return TD_ERROR;
	 }
	 
	 ptRtspSession = (TRtspSession *)_ptRtspSession;
	 pcAudioData   = _pcAudioData;
	 ptRtspServer  = (TRtspServer *)ptRtspSession->m_pServer;
	 ptChannel     = &(ptRtspServer->m_tChanel[ptRtspSession->m_iStream][ptRtspSession->m_iChan]);
	 ptRtpHeader   = &(ptRtspSession->m_tRtpInfo.m_tARtpHeader);
	 iAudioFrames  = ptChannel->m_tFrameHeader.u16AFrames;
	 iAudioSingleFrameSize = ptChannel->m_tFrameHeader.u16AFrameSize;

	 for(i = 0; i < iAudioFrames; i++)
	 {
		 //ȥ��4�ֽڵĺ�˼ͷ
		 RtpSendMediaPacket(ptRtspSession, A_FLAG, 1, pcAudioData + 4, iAudioSingleFrameSize - 4, NULL, 0);
		 ptRtpHeader->ts += (iAudioSingleFrameSize - 4);
		 pcAudioData += iAudioSingleFrameSize;
	 }
 
	 return TD_OK;
 }
 
 /****************************************************
  *���ƣ�G711USendAudio
  *���ܣ���װG711U���ݲ�����
  *������_ptRtspSession 		       :	 session
			 _pcAudioData			 :	 ��Ƶ����ָ��
  *����ֵ:	 
  *�޸���ʷ��
	 by qiaohaijun @2013.4.7
	 ........
 ****************************************************/
 int G711USendAudio(void *_ptRtspSession, char *_pcAudioData)
 {
	 TRtspSession *ptRtspSession = NULL; 
	 TRtspServer *ptRtspServer   = NULL;
	 TChannel *ptChannel         = NULL;
	 TRtpHeader *ptRtpHeader     = NULL;
	 char *pcAudioData           = NULL;
	 int iAudioSingleFrameSize   = -1;
	 int iAudioFrames            = -1;
	 int i = 0;
	 
	 if(_ptRtspSession == NULL || _pcAudioData == NULL)
	 {
		 return TD_ERROR;
	 }
	 
	 ptRtspSession = (TRtspSession *)_ptRtspSession;
	 pcAudioData   = _pcAudioData;
	 ptRtspServer  = (TRtspServer *)ptRtspSession->m_pServer;
	 ptChannel     = &(ptRtspServer->m_tChanel[ptRtspSession->m_iStream][ptRtspSession->m_iChan]);
	 iAudioFrames  = ptChannel->m_tFrameHeader.u16AFrames;
	 iAudioSingleFrameSize = ptChannel->m_tFrameHeader.u16AFrameSize;
	 ptRtpHeader   = &(ptRtspSession->m_tRtpInfo.m_tARtpHeader);
	 
	 for(i = 0; i < iAudioFrames; i++)
	 {
		 //ȥ��4�ֽڵĺ�˼ͷ
		 RtpSendMediaPacket(ptRtspSession, A_FLAG, 1, pcAudioData + 4, iAudioSingleFrameSize - 4, NULL, 0);
		 ptRtpHeader->ts += (iAudioSingleFrameSize - 4);
		 pcAudioData += iAudioSingleFrameSize;
	 }
 
	 return TD_OK;
 }

