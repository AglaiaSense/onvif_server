/*
 * 	file: dn_interface.h
 *	description: dn interface
 *	history: pangtaibao created at 20131017
 */

#ifndef __DN_INTERFACE_H__
#define __DN_INTERFACE_H__

#include "DAp2pcmd.h"

#define CONFIG_SERVER_PATH		"./config_server.ini"

#define MAX_CONNECT_NUM		128		//���������
#define TD_TRUE		1
#define TD_FALSE	0

#define CIF		2		//352*288
#define VGA		7		//640*480

//��ֵ��ΪĿǰ����ʾ�ķֱ���
enum FBL_size_t{
	FBL_SIZE_D1,		//720 * 576(P) 720 * 480(N)
	FBL_SIZE_HD1,		//352 * 576(P) 352 * 480(N)
	FBL_SIZE_BCIF,		//720 * 288(P) 720 * 240(N)
	FBL_SIZE_CIF = 3,	//352 * 288(P) 352 * 240(N)
	FBL_SIZE_QCIF = 4,	//176 * 144(P) 176 * 120(N)
	FBL_SIZE_VGA = 5,	//640 * 480(N/P)
	FBL_SIZE_QVGA = 6,	//320 * 240(N/P)
	FBL_SIZE_SVCD,		//480 * 480(N/P)
	FBL_SIZE_QQVGA,		//160 * 128(N/P)
	FBL_SIZE_NR = 9,	//����
	FBL_SIZE_ND1 = 9,	//240 * 192
	FBL_SIZE_650TVL,	//928 * 576
	FBL_SIZE_720P = 12,	//1280 * 720
	FBL_SIZE_1_3M,		//1280 * 960
	FBL_SIZE_UXGA,		//1600 * 1200
	FBL_SIZE_1080P,		//1920 * 1080
	FBL_SIZE_WUXGA,		//1920 * 1200
	FBL_SIZE_2_5M,		//1872 * 1200
	FBL_SIZE_3M,		//2048 * 1536
	FBL_SIZE_5M,		//3744 * 1408
	FBL_SIZE_EXT_NR,	//ö�ٵ�ͼ�δ�С�������Ŀ
};

typedef enum{
	PTZ_UP = 1,				//��
	PTZ_DOWN,				//��
	PTZ_LEFT,				//��
	PTZ_RIGHT,				//��
	PTZ_TUPR,	//����
	PTZ_TUPL,	//�� ��
	PTZ_TDPR,	//����
	PTZ_TDPL,	//����
	PTZ_STOP,				//ֹͣ
	PTZ_ZOOMADD = 10,		//�䱶+
	PTZ_ZOOMCUT,			//�䱶-
	PTZ_IST,	//�仯ֹͣ
	PTZ_FOCUSADD = 13,		//����+
	PTZ_FOCUSCUT,			//����-
	PTZ_FST,	//����仯ֹͣ
	PTZ_IA_ON,	//��Ȧ�Զ�����
	PTZ_IRISADD = 17,		//��Ȧ+
	PTZ_IRISCUT,			//��Ȧ-
	PTZ_WP_ON,//��������
	PTZ_WP_OFF,//����ֹͣ
	PTZ_LP_ON,	//�ƹ⿪��
	PTZ_LP_OFF,//�ƹ�ֹͣ
	PTZ_AUTO_START = 23,	//Ѳ����ʼ
	PTZ_AUTO_STOP,			//Ѳ��ֹͣ
	PTZ_GOTO_PRESET,//ת��Ԥ���
	PTZ_START_TRACK_CRUISE,	//��ʼ�켣Ѳ��
	PTZ_STOP_TRACK_CRUISE,	//ֹͣ�켣Ѳ��
	PTZ_SET_PRESET,
	PTZ_POWER_ON,	//��Դ����
	PTZ_POWER_OFF,//��Դ�ر�
	PTZ_3D_POSITION,		//PTZ 3D��λ
	PTZ_3D_ROCKER,			//��3άҡ�˵���Ӧ
	PTZ_ASSISTANT_ON,		//�������ؿ�
	PTZ_ASSISTANT_OFF,		//�������ع�
	PTZ_SET_ALARMOUTPORTON,//�����������״̬��
	PTZ_SET_ALARMOUTPORTOFF,//�����������״̬��
}ENUM_PTZ;

void* Danale_FindSession(void *_phess);

int Danale_StartVideo(void *_phess, char *_pData);

int Danale_StopVideo(void *_phess);

int Danale_StartAudio(void *_phess);

int Danale_StopAudio(void *_phess);

int Danale_PTZControl(char *_pData);

int Danale_GetColor(struct dcs_video_color *_ptVideoColor);

int Danale_SetColor(char *_pData);

int Danale_SendMainStream(int _iKeyFrame, char *_pcData, int _iChn);

int Danale_SendSubStream(int _iKeyFrame, char *_pcData, int _iChn);

int Danale_BaseInfoInit(void *_pThis);

int Danale_ConfigInit();

int Danale_SetFlip(char *_pData);

int Danale_DataProcess();

int Danale_GetPuId();

int Danale_GetEncodeParam(void *_pData);

int Danale_SetEncodeParam(void *_pData);

int Danale_RebootOrRestore(void *_pData);

int Danale_GetNetworkInfo(void *_pGetInfo);

int Danale_SetNetworkInfo(void *_pReq, int _iFlag);

int Danale_GetOSDInfo(void *_pOsdInfo);

int Danale_SetOSDInfo(void *_ptSetOSD);

int Danale_GetTime(void *_pReq);

int Danale_SetTime(void *_pReq);

char* Danale_GetPsw();

int Danale_SetPsw(void* _pData);

int Danale_snapshot(int _iChn);

#endif

