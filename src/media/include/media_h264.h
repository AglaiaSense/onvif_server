/*
	media_h264.h
	by qiaohaijun @2013.3.27
 */


#ifndef __MEDIA_H264_H__
#define __MEDIA_H264_H__


#include "rtp.h"
#include "rtsp_server.h"


int H264GetSpsPpsFromIFrame(TSDPInfo *_ptSdpInfo, char *_pcData, int _iLen);

int H264SendSPS(TRtspSession *_ptSession, char *_pcData, int _iLen);

int H264SendPPS(TRtspSession *_ptSession, char *_pcData, int _iLen);

int H264SendPPS2(TRtspSession *_ptSession, char *_pcData, int _iLen);

int H264SendIFrame(TRtspSession *_ptSession, char *_pcData, int _iLen);

int H264SendPFrame(TRtspSession *_ptSession, char *_pcData, int _iLen);

int H264SendVideo(TRtspSession *_ptSession, char *_pcData, int _iLen);

#endif


