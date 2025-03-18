//*****************************************
//	 �ļ��� 	    : onvif_shm_cmd.h
//	 �ļ�����: �����ڴ�ͨ��
//                            ��غ���˵��
//	�޸ļ�¼ : modify at 20130527 by ptb
//*****************************************

#ifndef _ONVIF_SHM_CMD_H_
#define _ONVIF_SHM_CMD_H_

#include "guiparameter.h"

/*****************�ṹ���ض���**************************/	
typedef struct
{
	int m_iVideoMod;			//��Ƶ������ʽ
	int m_iBenginTm;			//��ʼʱ�䣬�ӵ���00:00��������
	int m_iEndTm;				//����ʱ��
	int m_iHue;					//ɫ���� 0-100
	int m_iLum;					//������ 0-100
	int m_iContrast;			//�Աȶ� 0-100
	int m_iSaturation;			//���Ͷ� 0-100
	int m_iGrayFlg;				//��ת������(0:��ɫ, 1:��ɫ)
} VideoInCnf_EX;

//��Ϣ���ͽṹ��
typedef struct
{
	unsigned short ProtocolVersion;		//Э��汾����ǰ�汾Ϊ0x0100
	unsigned short MsgType;				//��Ϣ����
	unsigned short TransactionNo;		//�����ţ�����16λ����ʾ�����е�һ������λ������ʾ����0������Ӧ��1��
	unsigned short PacketLength;		//��Ϣ�峤�ȣ�������Ϣͷ����
} __attribute__((packed)) MsgHeader;

//ʱ��
typedef struct
{
	int lCurrentTime;
} __attribute__((packed)) st_time;


/*****************��������**************************/	
int OnvifCmdInit(int _iTid);

int OnvifCmdRebootReq();

int OnvifCmdSendVerNum(char *_pcVersion);

int OnvifCmdHeartbeat();

int OnvifCmdGetip(TGuiIpMsg *_pstIpMsg);

int OnvifCmdSetip(TGuiIpMsg *_pstIpMsg);

int OnvifCmdIframeReq(int iChn);

int OnvifCmdStreamDisConnect(int _iChn);

int OnvifCmdSetTime(TGuiSetSysTimeReq *ptTime);

int OnvifCmdRebootPlatform();

int OnvifCmdGetVideoInCnf(int _iChn, VideoInCnf_EX *_PVInCnf);

int OnvifCmdSetAVEncodeCnf(int _iChn,const TGuiVideoEncodeCnf *_pstVEncodeCnf);

int OnvifCmdGetVideoEncodeCnf(int _iChn, TGuiVideoEncodeCnf * _pVEncodeCnf);

int OnvifCmdGetDevNum(int _itype);

int OnvifCmdGetPtzDecodeCnf(int _iChn, TGuiPtzDecodeCnf *_pPtzDCnf);

int OnvifCmdSetPtzDecodeCnf(int _iChn, const TGuiPtzDecodeCnf *_pstPtzDCnf);

int OnvifCmdGetMDAlarmCfg(int _iChn);

int OnvifCmdGetMDAlarmStatus(int _iChn);

int OnvifCmdSetVideoInCnf(int _iChn, VideoInCnf_EX *_pVInCnf);

int OnvifCmdSetPTZ(TGuiPtzLocalCode *pPtzLocalCode);

int OnvifCmdGetSystemMessage(TGuiGetSysMsgRsp *_pstDevMsg);

int OnvifCmdGetIspInfo(TGuiVideoISPInfo  *_ptpIspInfo);

int OnvifCmdSetIspInfo(TGuiVideoISPInfo *_ptIspInfo);

int OnvifCmdGetVideoChnSizeList(int _iChn, int  *_ptpVideoList, int _iMaxCnt);

int OnvifCmdGetNTP(char *_pNTPIP, int _iBufLen);

int OnvifCmdSetNTP(char *_pNTPIP);

int OnvifCmdGetProductModel();

int OnvifCmdGetHttpPort();

int OnvifCmdRestore();

int OnvifCmdGetOsd(int _iChn, TGuiVideoOverlayCnf *_ptGetOsd);

int OnvifCmdSetOsd(int _iChn, TGuiVideoOverlayCnf *_ptSetOsd);

int OnvifCmdGetSnapshot(int _iChn);

int OnvifCmdGetSnapshotEx(int _iChn, char *_pcNameBuffer);

int OnvifCmdGetChnType(int _iChn);

int OnvifCmdStreamConnect(int _iChn);

int OnvifCmdSetDVRVideoInCnf(int _iChn, VideoInCnf_EX* _pVInCnf);

int OnvifCmdGetAudioRateCnf(int _iChn);

int OnvifCmdSetAudioRateCnf(int _iChn, int _iAudioRate);

//RelayOutput
int OnvifCmdGetRelayOutputState(int _iChn);

int OnvifCmdSetRelayOutputState(int _iChn, int _iState);

int OnvifCmdGetRelayOutputCnf(int _iPort);

int OnvifCmdSetRelayOutputCnf(int _iPort, int _iType);

//¼�񲿷�
int OnvifCmdGetVRTemplate(int _iChn, TGuiGetVideoTemplateRsp *_pStVRTemplate);

int OnvifCmdSetVRTemplate(TGuiSetVideoTemplateReq *_pStVRTemplate);

int OnvifCmdGetVRStrategy(TGuiGetVideoStrategyRsp *_pGetVStrategy);

int OnvifCmdSetVRStrategy(TGuiGetVideoStrategyRsp *_pSetVStrategy);

int OnvifCmdGetVFileCount(TGuiCountVideoFileReq *_pCondition);

int OnvifCmdQueryVFile(TGuiQueryVideoFileReq *_pCondition);

int OnvifCmdSetFlip(int _iChn, int _iType);

int OnvifCmdGetPtzCompassCnf(int _iChn, TGuiGetPTZCompassRsp *_ptGetPTZCompassRsp);

int OnvifCmdSetPtzCompassCnf(TGuiSetPTZCompassReq *_ptSetPTZCompassReq);

#endif

