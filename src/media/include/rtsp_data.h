/*
 *	file: media.h
 *	description: media process
 *	history: dongliqiang created at 20130228
 */
#ifndef __RTSP_DATA_H__
#define __RTSP_DATA_H__

#include "rtsp_server.h"

#define IDLE 100
#define MAX_FRAME_SIZE		(1024 * 1024)

//video encode
typedef enum
{
	VIDEO_ENCODE_H264 = 0,
	VIDEO_ENCODE_MPEG4,
	VIDEO_ENCODE_MJPEG
}EVEncType;

//audio encode
typedef enum
{
	AUDIO_ENCODE_G711 = 0,
	AUDIO_ENCODE_G726,
	AUDIO_ENCODE_AAC
}EAEncType;


typedef enum RtspAudioType
{
	RTSP_AUDIO_TYPE_G711A,
	RTSP_AUDIO_TYPE_G711MU,
	RTSP_AUDIO_TYPE_ADPCM = 78
}ERtspAudioType;

typedef enum RtspVideoType
{
	RTSP_VIDEO_TYPE_H264,
	RTSP_VIDEO_TYPE_MJPEG
}ERtspVideoType;

typedef struct
{
	unsigned char cFrameType;	// ֡����0:I֡;1:P֡;0xff:��Ч���ݱ�ʶ;3---:����;
	unsigned char cFrameRate;	// ֡��
	unsigned char cReserve1;	// �����ֽ�1
	unsigned char cReserve2;	// �����ֽ�2
	int iDataLength;			// ֡���ݳ��� 
} stFrameInfoPlate, *pstFrameInfoPlate;

int GetFrameInfo(TChannel *_ptChannel, stFrameInfoPlate *_ptFrameInfo);

int GetFrameHeader(char *_pcData, int _iLen, S_FrameHeader *_ptFrameHeader);

int RtspDataProcess(TRtspServer *_ptServer);

int RtspHeartProcess(TRtspServer *_ptServer);

int ProcessChanData(TChannel *_ptChannel, int _iStreamType, int _iChn);

int ProcessAVData(TChannel *_ptChannel, char *_pcData, int _iLen);

int ProcessVideo(TRtspSession *_ptSession, char *_pcData, int _iLen);

int ProcessAudio(TRtspSession *_ptSession, char *_pcData, int _iLen);

int WriteFrameToFile(unsigned char*_data, int _iLen);

#endif

