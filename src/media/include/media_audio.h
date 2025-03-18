/*
 *	file: media_audio.h
 *	description: send audio
 *	history: by qiaohaijun @2013.4.7
 */

#ifndef __MEDIA_AUDIO_H__
#define __MEDIA_AUDIO_H__

#define	ADTS_HEADER_LENGTH_7	7
#define	ADTS_HEADER_LENGTH_9	9
#define	AU_HEADER_LENGTH		4

int AACGetSampleRate(char *_pcAacData);

int AACGetLength(char *_pcAacData, int _iLength);

int AACBuildAuHeader(char *_pcAuHeader, int _iLenWithOutAdts);

int AAC4SendAudio(void *_ptRtspSession, char *_pcAudioData, int _iAudioStreamSize);

int ADPCM_DIV4SendAudio(void *_ptRtspSession, char *_pcAudioData);

int G711ASendAudio(void *_ptRtspSession, char *_pcAudioData);

int G711USendAudio(void *_ptRtspSession, char *_pcAudioData);

#endif
