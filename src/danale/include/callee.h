#ifndef __callee_h__
#define __callee_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "platform_adpt.h"
#include "dcsconst.h"
#include "mediatyp.h"
#include "DAp2pcmd.h"
#ifndef __rudp_h__
typedef struct _tagIOVEC {
	void *iov_base;
	unsigned int iov_len;
} IOVEC;
#endif

typedef struct storage_info{
	int type;		//云存储通知类型 
	char svr[32]; //云存储请求服务器 
	char etoken[72];//云存储token
	char user[32];	//云存储账号
	char passwd[32]; //云存储账号的密码
}DAStorageInfo;

typedef struct push_info{
	int type;//推送通知类型
	char svr[32];//推送服务器
}DAPushInfo;


typedef void* HTPCSESS;

typedef struct DAP2pCalleeCbFuncs {
	/* Called when a new connection comes in and the authentication is performing.
	 * Return TRUE if [user] is valid, and [password] is the password for the [user],
	 * otherwise, return FALSE */
	BOOL (*GetPassword)(const char *user, char password[32]);

	/* Called when a new session is established */
	int (*SessionCreated)(HTPCSESS hsess);

	/* Called when the session is closed by peer, or timeouted, or has problems in kernel.
	 * The callback clear user's data here, the session object itself is destroied by SDK(
	 * which means, don't call DAP2pSessCloseSession() here */
	void (*SessionAborted)(HTPCSESS hsess);

	/* Command handler
	 * pData is the parameter data of the cmd,
	 * trans_id is used for response */
	void (*CommandHandler)(HTPCSESS hsess, unsigned short cmd, unsigned int trans_id, BYTE *pData, int len);

	/* Called when device start,and will Initialize the information  */
	void (*GetStorageInfo)(struct storage_info *data);

	/* Called when Server inform device cloud storage about operation */
	void (*SetStorageInfo)(struct storage_info *data);

	/* Called when device start,and will Initialize the information*/
	void (*GetPushInfo)(struct push_info *data);
	
	/* Called when Server inform device alarm push about operation */
	void (*SetPushInfo)(struct push_info *data);

	BOOL (*SetNetworkInfo)(struct NetworkInfo *info);
} DAP2PCALLEECBFUNCS;

DWORD DAP2pCalleeGetVersion();

//initialize device basic information
int BaseInfoCalleeInitialize(struct CmdGetBaseInfo *pDev);

int DAP2pCalleeInitialize(const char *svr[], int n_svr, const char *sn, const DAP2PCALLEECBFUNCS *cbs);

///* signal termination
void DAP2pCalleeTerminate();

///* Cleanup resources
void DAP2pCalleeCleanup();

#define TPCGSF_LOCAL_READY	0x0001
#define TPCGSF_REMOTE_READY	0x0002
#define TPCGS_EXPIRED		1
#define TPCGS_INVALID_SN	2

typedef struct DAP2pCalleeState {
	int readyStates;	//combination of TPCGSF_xxx
	int state;		//TPCGS_xxx (invalid sn, expired service)
} DAP2PCALLEESTATE;

int DAP2pCalleeGetState(DAP2PCALLEESTATE *tpcState);

typedef struct _tagTpcSessInfo {
	int connType;	//DCS_CONNTYPE_xxx
	struct sockaddr_in peer_addr;
	int state;
} TPCSESSINFO;

struct push_storage_info{
	struct push_info push_info;	//推送相关信息
	struct storage_info storage_info;//云存储相关信息
};

int DAP2pSessGetInfo(HTPCSESS hsess, TPCSESSINFO *info);

#define DAP2P_LIVE_VIDEO_CHNO	0
#define DAP2P_LIVE_AUDIO_CHNO	1
#define DAP2P_PLAYBACK_VIDEO_CHNO	2
#define DAP2P_PLAYBACK_AUDIO_CHNO	3

///* Send media frame
//   for h264, frame is not nalu splitted, and start with 4 bytes sync bytes: 00 00 00 01
int DAP2pSessSendMediaFrame(HTPCSESS hSess, int chno, MEDIATYPE mt, DWORD timestamp, 
		BOOL isKeyFrame, BYTE *pFrame, int size, UINT maxWaitMs);
int DAP2pSessSendMediaFrameV(HTPCSESS hsess, int chno, MEDIATYPE mt, DWORD timestamp, 
		BOOL isKeyFrame, IOVEC *vec, UINT size, UINT maxWaitMs);

///* Send command request&response
//
//   Length per sending should be less than 1350 bytes.
//
int DAP2pSessSendRequest(HTPCSESS hsess, int chno, int cmd, const void *pData, UINT nDataLen, uint32 trans_id);
//   Sending of response should begin with 0 or multiple calls to DAP2pSessSendCmdResponse0(),
//   then ends with one DAP2pSessSendCmdResponseLast()
int DAP2pSessSendCmdResponse0(HTPCSESS hsess, int chno, int cmd, const void *pData, UINT nDataLen, uint32 trans_id);
int DAP2pSessSendCmdResponseLast(HTPCSESS hsess, int chno, int cmd, const void *pData, UINT nDataLen, uint32 trans_id);
#define DAP2pSessSendSimpleResponse(hsess, cmd, trans_id) DAP2pSessSendCmdResponseLast(hsess, 0, cmd, NULL, 0, trans_id)

int DAP2pSessSendEvent(HTPCSESS hsess, const void *pData, UINT nDataLen);

///* Send response with error indication, no extra data
//   status: only DCSS_xxx defined in dcsconst.h are allowed
int DAP2pSessSendResponseError(HTPCSESS hsess, int chno, int cmd, uint8 status, uint32 trans_id);

int DAP2pSessCloseSession(HTPCSESS hSess);

///* Associate a user's pointer to session
int DAP2pSessSetUserData(HTPCSESS hsess, void *pUser);

///* Get user's pointer associated with session
int DAP2pSessGetUserData(HTPCSESS hsess, void **ppUser);

//* Get device base information
int DABaseInfoCalleeget(struct CmdGetBaseInfo *pDev);


#define NOT_OPEN_STORAGE 	2001
#define NOT_OPEN_PUSH		2002

/*
* Function:send push req to appserver
* param:
	msgtype: message type, refer ot enum ALARM_MTYPE
	filepath: to be attach push file path
* return value: 0 ok 
*/
int DAP2pSendpushreq(ALARM_MTYPE msgtype, char *filepath);


/*
	Function: Send file to cloud storage server
	Param:	  
		type: 	NORMAL_FILE or ALARM_FILE, ALARM_FILE:timing record or snap 
					ALARM_FILE: alarm create file
					if type is not NORMAL_FILE or ALARM_FILE will return ERR_TYPE
		filepath: to send file path
	return :0 Ok 
	
*/	
#define ERR_TYPE 3

#define NORMAL_FILE 100
#define ALARM_FILE 101

int DAp2pSendFileToCloudsvr(int type,const char *filepath);

#ifdef __cplusplus
}
#endif

#endif

