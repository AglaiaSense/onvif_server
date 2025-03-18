/*
	media_mjpeg.h
	by qiaohaijun @2013.3.27
 */
#ifndef __RTPJPEG__H
#define __RTPJPEG__H

#include "rtp.h"
#include "rtsp_server.h"

#define RTP_JPEG_HEADLENGTH (sizeof(TRtpHeader) + sizeof(JpegMainHeader_t) + sizeof(JpegResetHeader_t))
#define RTP_JPEG_FIRSTHEADLENGTH (RTP_JPEG_HEADLENGTH + sizeof(JpegQtableHeader_t) + 128)

int MJpegSendVideo(TRtspSession *_ptSession, char *_pcData, int _iLen);
#endif


