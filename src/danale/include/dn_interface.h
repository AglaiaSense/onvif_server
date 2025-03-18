/*
 * 	file: dn_interface.h
 *	description: dn interface
 *	history: pangtaibao created at 20131017
 */

#ifndef __DN_INTERFACE_H__
#define __DN_INTERFACE_H__

#include "DAp2pcmd.h"

#define CONFIG_SERVER_PATH		"./config_server.ini"

#define MAX_CONNECT_NUM		128		//最大连接数
#define TD_TRUE		1
#define TD_FALSE	0

#define CIF		2		//352*288
#define VGA		7		//640*480

//赋值的为目前会显示的分辨率
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
	FBL_SIZE_NR = 9,	//备用
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
	FBL_SIZE_EXT_NR,	//枚举的图形大小种类的数目
};

typedef enum{
	PTZ_UP = 1,				//上
	PTZ_DOWN,				//下
	PTZ_LEFT,				//左
	PTZ_RIGHT,				//右
	PTZ_TUPR,	//右上
	PTZ_TUPL,	//左 上
	PTZ_TDPR,	//右下
	PTZ_TDPL,	//左下
	PTZ_STOP,				//停止
	PTZ_ZOOMADD = 10,		//变倍+
	PTZ_ZOOMCUT,			//变倍-
	PTZ_IST,	//变化停止
	PTZ_FOCUSADD = 13,		//焦距+
	PTZ_FOCUSCUT,			//焦距-
	PTZ_FST,	//焦点变化停止
	PTZ_IA_ON,	//光圈自动调整
	PTZ_IRISADD = 17,		//光圈+
	PTZ_IRISCUT,			//光圈-
	PTZ_WP_ON,//擦拭启动
	PTZ_WP_OFF,//擦拭停止
	PTZ_LP_ON,	//灯光开启
	PTZ_LP_OFF,//灯光停止
	PTZ_AUTO_START = 23,	//巡航开始
	PTZ_AUTO_STOP,			//巡航停止
	PTZ_GOTO_PRESET,//转到预设点
	PTZ_START_TRACK_CRUISE,	//开始轨迹巡航
	PTZ_STOP_TRACK_CRUISE,	//停止轨迹巡航
	PTZ_SET_PRESET,
	PTZ_POWER_ON,	//电源开启
	PTZ_POWER_OFF,//电源关闭
	PTZ_3D_POSITION,		//PTZ 3D定位
	PTZ_3D_ROCKER,			//对3维摇杆的响应
	PTZ_ASSISTANT_ON,		//辅助开关开
	PTZ_ASSISTANT_OFF,		//辅助开关关
	PTZ_SET_ALARMOUTPORTON,//设置输出报警状态开
	PTZ_SET_ALARMOUTPORTOFF,//设置输出报警状态关
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

