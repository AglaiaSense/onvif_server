/*
 *	file: onvif_sdk.h
 *	description: define onvif server struct
 *	history: dongliqiang created at 20120209
 */

#ifndef __ONVIF_SDK_H__
#define __ONVIF_SDK_H__

#include "onvifH.h"
#include "dictionary.h"
#include "common.h"

#include "connection.h"
#include "channel.h"

//soap��
#define LINGER_TIMEOUT			(0)
#define ACCEPT_TIMEOUT			(0)
#define SEND_TIMEOUT			(10)
#define RECV_TIMEOUT			(10)
#define CONNECT_TIMEOUT			(10)
#define BACK_LOG				(100)
#define MAX_SCOPES_NUM			(16)
#define MAX_STRING_LEN			(1023)
#define MAX_STRING_LEN_MEM		(1024)
#define MAX_UUID_LEN			(64)
#define MAX_USER_NUM 			(16)
#define MAX_USERANDPWD_LEN		(16)
#define MIN_USERANDPWD_LEN		(4)
#define MAX_NAME_LEN			(64)
#define MAX_TMP_MEM				(2048)
#define SUBSCRIBEURL			"/Subscription?Index="
//#define MAX_KEEP_ALIVE		(1)

//#define MULT_CAST_PORT		(3702)
//#define MULT_CAST_IP			("239.255.255.250")
#define CONFIG_ONVIF_PATH		"./config_onvif.ini"
#define MAX_INDEX_NUM			(256)	//���������

#define MAX_CONSUMER_NUM		32
#define TOPIC_NUM				8

//			��1λ:video move, 
//			��2λ:port alarm, 
//			��3λ:video cover, 
//			��4λ:video lost,
typedef enum
{
	ALARM_MOVE = 0,
	ALARM_PORT,
	ALARM_COVER,
	ALARM_LOST
	
}TopicBitNo;// by qiaohaijun


typedef enum
{
	PORTALARMIN = 1,
	VIDEOMOVE,
	VIDEOLOST,
	VIDEOCOVER
}TopicType;

typedef enum
{
	DELETE = 0,
	INITIALIZE,
	CHANGE
}TopicState;

typedef enum
{
	AUTO        = 0,                    //�����˹�Ƭģʽ
	OPEN,
	CLOSE
}IrCutMode;

typedef struct
{
	int m_iChnMask;   //��λ��ʾͨ���ţ����Ϊ���ʾû��consumer
}ConsumerIndex;

typedef enum{
	CANCLED 		= 0,				//��ȡ������	
	SUBSCRIBED,							//�Ѷ��ķ���	
	PAUSE,								//��ͣ����	
	RESUME								//�ָ�����
}ConsumerState;

typedef struct{
	int				m_iSubTopicMask;		//by qiaohaijun 
	time_t			m_tCreateTime;		//����ʱ��
	time_t			m_tTerminateTime;	//��ֹʱ��
	ConsumerState	m_eState;			//consumer״̬��Ϣ
	TopicState		m_eMoveState;
	char			*pcEndPoint;		//EndPoint
}ConsumerInfo;

typedef struct{
	int				m_iEnable;			//ʹ�ܿ���
	int				m_iPullTopicMask;		//added by qiaohaijun @2013.12.06 for event
	int				m_iChnMask;			//added by qiaohaijun @2013.12.06
	TopicState		m_eMoveState;		//��ǰ״̬
	TopicState		m_ePortState;
	time_t			m_tCreateTime;		//����ʱ��
	time_t			m_tTerminateTime;	//��ֹʱ��
	char			m_cEndPoint[64];	//EndPoint
}PullMsgInfo;

//event manager�ṹ��
typedef struct{
	//�����ϴ�ģʽ
	ConsumerInfo    *ConsumerInfoList[MAX_CONSUMER_NUM];
	ConsumerIndex   ConsumerIndexList[TOPIC_NUM][MAX_CONSUMER_NUM];
	pthread_mutex_t  m_stEventLock;					//������

	int m_iAlarmInfo[MAX_CHANNEL_NUM][TOPIC_NUM];
	//�����ϴ�ģʽ
	PullMsgInfo		m_tPullMsg;
}EventManager;


//��Ƶ��ز���
typedef struct
{
	int	m_InputNorm[MAX_CHANNEL_NUM];   //��Ƶ��ʽ
}TVideoParam;

//NVS������Ϣ
typedef struct{

	int          m_iVINum;         	//��Ƶ������

	int          m_iVENum;         	//��Ƶ������
		
	int          m_iVONum;         	//��Ƶ�����
	
	int          m_iAINum;         	//��Ƶ������
	
	int          m_iAONum;         	//��Ƶ�����

	int  		m_iAlarmInNum;      //����������

	int  		m_iAlarmOutNum;      //���������
		
	unsigned char			m_cLocalIP[32];		//����IP��ַ

	int			m_iLTcpPort;		//����onvif-TCP�˿�

	int          m_iLEventPort;		//���������˿�
	
	int          m_iLWebPort;		//�豸��ҳ�˿�

	unsigned char			m_cMultIP[32];		//�ಥ��ַ fixed "239.255.255.250"			

	int			m_iMultPort;		//�ಥ�˿�fixed 3702

	unsigned char			m_cUUID[64];		//uuid
	
	int 			m_iDeviceType;		//�豸����	0 - IPC	1 - DVR
	
	int 			m_iTimeZone;		//Ĭ�϶�����-8

	TVideoParam		m_tVideoParam;		// ��Ƶ��ز���
}TNvsInfo;


typedef struct
{
	char			m_cRealm[128];
	char			m_cNonce[36];
}TRtspAuth;

typedef struct
{
	int 				m_iStat;
	int	          		m_iLPort;				//rtsp server port
	TNvsInfo			m_tNvsInfo;				//nvs info
	TConnection			m_tHttpConnect;			//server http connect
	TConnection			m_tRtspConnect;			//server rtsp connect
	DList*				m_ptDListSession;		//server�˻Ự���
	TChannel			m_tChanel[MAX_STREAM][MAX_CHANNEL_NUM];
	void *				m_pUserData;			//user data
	TMutexLock   		m_tLock;				//server lock

	TRtspAuth		m_tRtspAuth;				//digest ������֤ by qiaohaijun

}TRtspServer;

typedef struct
{
	int		m_iCfgChangeFlag;			//�����ļ��ı��ʶ

	int		m_iSendHelloFlag;			//�ಥ����hello��ʶ

	int		m_iRebootFlag;		        //�豸����

	int		m_iPlatformRebootFlag;	//onvif����
	
	int		m_iFactoryDefaultFlag;		//�ָ���������

	int		m_iNotifyFlag;				//notify����ʹ��
	
	int		m_iIspFlag;					//�豸�Ƿ�֧�ָ������
	
	int		m_iAlarmFlag; 				//�б���Ϊ1���ޱ���Ϊ0.
}TFlag;

#ifdef WITH_DANALE
typedef struct
{
	char			m_cPuId[20];		//Danale ID��Ϣ
	int				m_iHeart;			//����
}TDanaleInfo;
#endif

//onvifͨ�����ýṹ��, ÿ��Э�齫��Ӧһ������.
//�������, ��ֱ����ӵ��ýṹ����.���ʱ��Ӧͬ��.
typedef struct{

	//======== IP configuration =======
	struct _tds__GetNetworkInterfacesResponse 			*m_ptNetworkInfoRsp;

	struct _tds__GetNetworkProtocolsResponse 			*m_ptNetworkProRsp;

	struct _tds__GetNetworkDefaultGatewayResponse 		*m_ptNetworkGWRsp;

	struct _tds__GetHostnameResponse 					*m_ptHostnameRsp;

	struct _tds__GetDNSResponse							*m_ptDNSRsp;

	struct _tds__GetNTPResponse 						*m_ptNTPRsp;

	//======== Device discovery  ======
	struct d__ProbeMatchesType 							*m_ptProbeRsp;

	struct _tds__GetDiscoveryModeResponse				*m_ptDiscModeRsp;

	//======== Device management  ====
	struct _tds__GetDeviceInformationResponse 			*m_ptDevInfoRsp;

	struct _tds__GetCapabilitiesResponse				*m_ptCapbilityRsp;

	struct _tds__GetScopesResponse 						*m_ptScopesRsp;
	
	struct _tds__GetWsdlUrlResponse                     *m_ptWsdlUrlRsp;
		
	struct _trt__GetProfilesResponse 					*m_ptProfilesRsp;

	struct _trt__GetStreamUriResponse					*m_ptStreamUriRsp;

	struct _tds__GetSystemDateAndTimeResponse 			*m_ptSystemTimeRsp;

	struct _tds__GetRelayOutputsResponse 				*m_ptRelayOutputRsp;

	//======== Event handing ========
	struct _tev__GetEventPropertiesResponse 			*m_ptEventPropertiesRsp;

	struct _wsnt__SubscribeResponse						*m_ptSubscribeRsp;

	struct _tev__CreatePullPointSubscriptionResponse    *m_ptCreatePPSRsp;

	struct _wsnt__Notify 								*m_ptNotify;

	struct _tev__PullMessagesResponse                   *m_ptPullMsgRsp;

	//======== Security  ===========
	struct _tds__GetUsersResponse						*m_ptUsersRsp;

	//======== Service   ===========
	struct _tds__GetServicesResponse 					*m_ptServicesRsp;

	struct _tds__GetServiceCapabilitiesResponse 		*m_ptServiceCapRsp;

	//Video
	struct _trt__GetVideoSourcesResponse						*m_ptVideoSourcesRsp;

	struct _trt__GetVideoSourceConfigurationsResponse			*m_ptVideoSourceCfgRsp;

	struct _trt__GetVideoEncoderConfigurationsResponse			*m_ptVideoEncoderCfgRsp;

	struct _trt__GetVideoSourceConfigurationOptionsResponse		*m_ptVideoSourceCfgOptRsp;
	
	struct _trt__GetVideoEncoderConfigurationOptionsResponse	*m_ptVideoEncoderCfgOptRsp;

	//Audio
	struct _trt__GetAudioSourcesResponse						*m_ptAudioSourcesRsp;
	
	struct _trt__GetAudioSourceConfigurationsResponse			*m_ptAudioSourceCfgRsp;

	struct _trt__GetAudioEncoderConfigurationsResponse			*m_ptAudioEncoderCfgRsp;

	struct _trt__GetAudioSourceConfigurationOptionsResponse		*m_ptAudioSourceCfgOptRsp;

	struct _trt__GetAudioEncoderConfigurationOptionsResponse	*m_ptAudioEncoderCfgOptRsp;

	struct _trt__GetAudioOutputsResponse						*m_ptAudioOutputsRsp;

	struct _trt__GetAudioOutputConfigurationsResponse			*m_ptAudioOutputCfgRsp;
	
	struct _trt__GetAudioOutputConfigurationOptionsResponse		*m_ptAudioOutputCfgOptRsp;

	//PTZ
	struct _tptz20__GetConfigurationsResponse 			*m_ptConfigRsp;

	struct _tptz20__GetConfigurationOptionsResponse 	*m_ptCfgOptionsRsp;

	struct _tptz20__GetNodesResponse 					*m_ptNodesRsp;

	struct _tptz20__GetStatusResponse 					*m_ptStatusRsp;

	struct _tptz20__GetPresetsResponse 					*m_ptPresetsRsp;
	
	//Image
	struct _timg10__GetImagingSettingsResponse 			*m_ptImgSettingsV10Rsp;

	struct _timg10__GetOptionsResponse 					*m_ptOptionsV10Rsp;

	struct _timg10__GetMoveOptionsResponse 				*m_ptMoveOptionsV10Rsp;

	struct _timg10__GetStatusResponse 					*m_ptStatusV10Rsp;

	struct _timg20__GetImagingSettingsResponse 			*m_ptImgSettingsV20Rsp;

	struct _timg20__GetOptionsResponse 					*m_ptOptionsV20Rsp;

	struct _timg20__GetMoveOptionsResponse 				*m_ptMoveOptionsV20Rsp;

	struct _timg20__GetStatusResponse 					*m_ptStatusV20Rsp;

	//Metadata
	struct _trt__GetMetadataConfigurationsResponse       *m_ptMetaCfgRsp;

	struct _trt__GetMetadataConfigurationOptionsResponse *m_ptMetaCfgOptionsRsp;
}OnvifCfg;


//onvif server
typedef struct{

	TNvsInfo		m_stNvsInfo;		//NVT��Ϣ
	
	struct soap 	m_stSoapTcp;		//tcp soap

	struct soap 	m_stSoapUdp;		//udp soap

	struct soap 	m_stSoapEvent;		//Event soap

	pthread_mutex_t	m_stMutexLock;		//tcp and udp mutex lock
	
	dictionary*		m_ptOnvifDict;		//onvif dictionary

	OnvifCfg		m_stOnvifCfg;		//onvifͨ�Žṹ��

	EventManager	m_stManager;		//event manager

	TRtspServer		m_stRtspServer;

	time_t          *m_pTimeout;		//ptz timeout

	TFlag			m_stFlag;			//onvif������Ҫ�ı�ʶ

#ifdef WITH_DANALE
	dictionary*		m_ptDict;			//config_server.ini

	TDanaleInfo		m_stDanaleInfo;		//Danaleƽ̨��Ϣ
#endif

}OnvifCtrl;

#endif
 
