#ifndef __dcsconst_h__
#define __dcsconst_h__

//
// Status & Error code
//
#define DCSS_OK			0
#define DCSS_AUTH_FAILED	1//鉴权错误
#define DCSS_ERROR_TRANSID	2//?
#define DCSS_ERROR_PACKET	3//数据错误
#define DCSS_SESSION_CLOSED	4//会话已关闭
#define DCSS_ADDRESS_CHANGED	5	//for OP_DS_IAMHERE, server return a new address, then ipcam should report itself to the new server 
#define DCSS_INVALID_USER	6	//User not existed无效用户
#define DCSS_IPCAM_OFFLINE	7	//Offline
#define DCSS_INACTIVE		8	//Device not activated未激活
#define DCSS_UNEXPECTED		9//无法预料的错误
#define DCSS_INVALID_AC		10	//Wrong user name or password
#define DCSS_REGISTERED		11	//Register ipcam
#define DCSS_INVALID_KEY	12
#define DCSS_LACK_OF_RESOURCE	13//缺少资源
#define DCSS_NOT_ALLOWED	14//禁止操作
#define DCSS_RELAY_SERVER_UNVAILABLE	15
#define DCSS_UPDATE_FW		16
#define DCSS_UNRECOGNIZED_CMD	17//无效操作

#define DCSS_OPEN_STORAGE_NOTICE 	 20


#define DCSE_OK			0
#define DCSE_NO_RESPONSE	101
#define DCSE_ERROR_RESPONSE	102
#define DCSE_SOCKET_ERROR	103
#define DCSE_CONN_FAILED	104
#define DCSE_TIMEOUTED		105
#define DCSE_NO_DEV			106	//cann't be searched
#define DCSE_SESS_ABORTED	107

#define DCSE_INVALID_OBJECT	201
#define DCSE_INVALID_P2PCHNO	202
#define DCSE_P2PCHN_NOT_OPEN	203

//push error
#define PUSH_SYSTYPE_ERROR 300
#define PUSH_MSGTYPE_ERROR 301

//
// Connection Type
//
#define DCS_CONNTYPE_AUTO	0
#define DCS_CONNTYPE_LOCAL	1
#define DCS_CONNTYPE_P2P	2
#define DCS_CONNTYPE_RELAY	3

typedef enum 
{
   StunTypeUnknown=0,
   StunTypeBlocked=0,
   StunTypeOpen,

#ifndef RFC_STUN
   StunTypeIndependentFilter,
   StunTypeDependentFilter,
   StunTypePortDependedFilter,
   StunTypeDependentMapping,
#else
   StunTypeConeNat,
   StunTypeRestrictedNat,
   StunTypePortRestrictedNat,
   StunTypeSymNat,
#endif
   StunTypeFirewall,
   StunTypeFailure,
} NatType;

typedef enum
{
	NORMAL,
	MOTION_DETECT,
	IO_DETECT,
	SOUND_DETECT,
	COVER_ALARM,
	TIMING_CAPTURE,
	TIMING_RECORD,
	OTHER
}ALARM_MTYPE;


#endif

