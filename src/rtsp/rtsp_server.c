/*
 *	file: rtsp_server.c
 *	description: rtsp server
 *	history: dongliqiang created at 20130228
 */
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <signal.h>
#include "rtsp_server.h"
#include "single_thread.h"
#include "task_process.h"
#include "epoll.h"
#include "onvif_sdk.h"
#include "rtsp_data.h"
#include "rtsp_cmd.h"
#include "proc.h"

TRtspServer *g_ptRtspServer = NULL;

TRtspSession *g_ptRtspSession = NULL;
static TMutexLock g_stSessionLock;  
extern FILE* fpProc;

static TRtspPort g_tPort[MAX_SESSION_NUM * MAX_PORT_INTERVAL];
static TMutexLock g_stPortLock;
 
static int inner_GenSessionID(void)
{
	 struct timespec spec;
	 
	 clock_gettime(CLOCK_REALTIME, &spec);
	 return (int)(spec.tv_nsec);
}


 
static int InitPort(int _iStartPort, int _iInterval, int _iConnectNum)
 {
	 int i = 0;
	 int j = 0;
	 int iPortNum = 0;
 
	 if(_iConnectNum > MAX_SESSION_NUM || _iInterval > MAX_PORT_INTERVAL)
	 {
		 return -1;
	 }
 
	 for(i = 0; i < _iConnectNum; i++)
	 {
	 	 for(j = 0; j < _iInterval; j++)
	 	 {
		 	 iPortNum = i * _iInterval + j;
			 g_tPort[iPortNum].m_iUsed = 0;
			 g_tPort[iPortNum].m_iPort = _iStartPort + iPortNum;
			 //td_printf(0, "==================iPortNum[%d] is %d\n", iPortNum, g_tPort[iPortNum].m_iPort);
		 }
	 }
 
	 return 0;
 }
 
int GetPort()
 {
	 int i;
	 static int iPortInit = 0;
	 int iPortNum = 0;
 
	 if(iPortInit == 0)
	 {
		 InitPort(RTP_SERVER_PORT_START, MAX_PORT_INTERVAL, MAX_SESSION_NUM);
		 iPortInit = 1;
	 }

	MutexLockLock(&g_stPortLock);
	 for(i = 0; i < MAX_SESSION_NUM; i++)
	 {
		 iPortNum = i * MAX_PORT_INTERVAL;
		 if(g_tPort[iPortNum].m_iUsed == 0)
		 {
			 g_tPort[iPortNum].m_iUsed = 1;
			 MutexLockUnLock(&g_stPortLock);
			 return g_tPort[iPortNum].m_iPort;
		 }
	 }
	 MutexLockUnLock(&g_stPortLock);
 
	 return -1;
 }
 
int FreePort(int _iPort)
 {
	 int iPort = _iPort;
	 int i;
	 int iPortNum = 0;
	 if(iPort < 0)
	 {
		 return -1;
	 }

	MutexLockLock(&g_stPortLock);
	 for(i = 0; i < MAX_SESSION_NUM; i++)
	 {
		 iPortNum = i * MAX_PORT_INTERVAL;
		 if(g_tPort[iPortNum].m_iPort == iPort)
		 {
			 g_tPort[iPortNum].m_iUsed = 0;
			 break;
		 }
	 }
	 MutexLockUnLock(&g_stPortLock);
 
	 return 0;
 } 
 

static DListNode * inner_AddRtspSession(TRtspServer *_ptServer, TRtspSession *_ptSession)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspServer *ptServer = _ptServer;
	TRtspSession *ptSession = _ptSession;
	DListNode *ptNode = NULL;

	if(NULL == ptServer || NULL == ptSession)
	{
		return NULL;
	}
	
	//add list node
	adt_dlist_lock(ptServer->m_ptDListSession);
	ptNode = adt_dlist_add(ptServer->m_ptDListSession, NULL, (void *)&ptSession);
	adt_dlist_unlock(ptServer->m_ptDListSession);
	td_printf(0, "%s %d %s end!\n", __FILE__, __LINE__, __FUNCTION__);
	return ptNode;
}

static int inner_DelRtspSession(TRtspServer *_ptServer, TRtspSession *_ptSession)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspServer *ptServer = _ptServer;
	TRtspSession *ptSession = _ptSession;

	if(NULL == ptServer || NULL == ptSession)
	{
		td_printf(0, "%s %d %s, input param error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	
	adt_dlist_lock(ptServer->m_ptDListSession);
	adt_dlist_delete(ptServer->m_ptDListSession, ptSession->m_ptNode);
	ptSession->m_ptNode = NULL;
	adt_dlist_unlock(ptServer->m_ptDListSession);
	td_printf(0, "%s %d %s end!\n", __FILE__, __LINE__, __FUNCTION__);	
	return TD_OK;
}

#ifdef TD_DEBUG
static int GetFreeSessionCnt()
{
	int i = 0;
	int iCnt = 0;
	TRtspSession *ptSession = NULL;
	
	for(i = 0; i < MAX_SESSION_NUM; i++)
	{
		ptSession = g_ptRtspSession + i;
		if(ptSession->m_iUsed == 0)
		{
			iCnt++;
		}
	}

	return iCnt;
}
#endif

//根据HTTP POST的Session Cookie查找对应的GET的session.
TRtspSession * FindSessionByHttpCookie(TRtspSession *_ptPostSession, char *_pcHttpCookie)
{
	td_printf(0, "%s %d %s start", __FILE__, __LINE__, __FUNCTION__);
	
	int i = 0;
	TRtspSession *ptSession = NULL;
	TRtspSession *ptPostSession = _ptPostSession;

	if(NULL == _pcHttpCookie)
	{
		td_printf(0, "%s %d %s, input param error.", __FILE__, __LINE__, __FUNCTION__);
		return NULL;
	}

	//td_printf(0, "%s %d %s , IN cookie = %s", __FILE__, __LINE__, __FUNCTION__, _pcHttpCookie);
	
	MutexLockLock(&g_stSessionLock);
	for(i = 0; i < MAX_SESSION_NUM; i++)
	{
		ptSession = g_ptRtspSession + i;
		//td_printf(0, "%s %d %s , session[%d].cookie = %s", __FILE__, __LINE__, __FUNCTION__, i, ptSession->m_pcHttpSessionCookie);
		if(ptPostSession != ptSession && 1 == ptSession->m_iUsed  && ptSession->m_iStat > SESSION_ACCEPTED && strlen(ptSession->m_pcHttpSessionCookie) > 0 && 0 ==strncmp(ptSession->m_pcHttpSessionCookie, _pcHttpCookie, HTTP_SESSION_COOKIE_LEN))
		{
			td_printf(0, "%s %d %s, session[%d] matched!", __FILE__, __LINE__, __FUNCTION__, i);
			MutexLockUnLock(&g_stSessionLock);
			return ptSession;
		}
	}
	
	td_printf(0, "%s %d %s, nothing found!!!", __FILE__, __LINE__, __FUNCTION__);
	
	MutexLockUnLock(&g_stSessionLock);
	return NULL;
}

static TRtspSession * inner_GetSession()
{
	int i = 0;
	TRtspSession *ptSession = NULL;
	
	MutexLockLock(&g_stSessionLock);
	for(i = 0; i < MAX_SESSION_NUM; i++)
	{
		ptSession = g_ptRtspSession + i;
		if(ptSession->m_iUsed == 0)
		{
			td_printf(0, "%s %d %s, session[%d] %p\n", __FILE__, __LINE__, __FUNCTION__, i, ptSession);
			ptSession->m_iUsed = 1;
			td_printf(0, "%s %d %s, #######After Get, free session total = %d", __FILE__, __LINE__, __FUNCTION__, GetFreeSessionCnt());

			MutexLockUnLock(&g_stSessionLock);
			return ptSession;
		}
	}
	
	td_printf(0, "%s %d %s, #######After Get, free session total = %d", __FILE__, __LINE__, __FUNCTION__, GetFreeSessionCnt());
	
	MutexLockUnLock(&g_stSessionLock);
	return NULL;
}

static int inner_FreeSession(TRtspSession *_ptSession)
{
	int i = 0;
	TRtspSession *ptSession = _ptSession;
	TRtspSession *ptTempSession = NULL;

	if(pointer_valid_check(ptSession))
	{
		td_printf(0, "%s %d %s, input param error!\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}

	//session_id可以为0.
	if(1 != ptSession->m_iUsed)
	{
		td_printf(0, "%s %d %s, session is not used, cannot free.\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}

	MutexLockLock(&g_stSessionLock);
	for(i=0; i<MAX_SESSION_NUM; i++)
	{
		ptTempSession = g_ptRtspSession + i;
		if(ptTempSession->m_iSessionID == ptSession->m_iSessionID)
		{
			if(1 == ptTempSession->m_iUsed)
			{
				td_printf(0, "%s %d %s, session[%d] %p\n", __FILE__, __LINE__, __FUNCTION__, i, ptSession);
				ptTempSession->m_iUsed = 0;
				td_printf(0, "%s %d %s, $$$$$$$$$After Delete, free session total = %d", __FILE__, __LINE__, __FUNCTION__, GetFreeSessionCnt());
			
				MutexLockUnLock(&g_stSessionLock);
				return 0;
			}
			else
			{
				td_printf(0, "%s %d %s, session matched, but used state is error. used = %d", __FILE__, __LINE__, __FUNCTION__, ptTempSession->m_iUsed);
			}
		}
	}
	td_printf(0, "%s %d %s, $$$$$$$$$After Delete, free session total = %d", __FILE__, __LINE__, __FUNCTION__, GetFreeSessionCnt());
	MutexLockUnLock(&g_stSessionLock);
	
	return -1;
}
#ifdef WITH_MD5
//by qiaohaiju for Authenticate test, 2013.12.13
//生成noce值
int genNonce(char *nonceBuf)
{
	struct
	{
		struct timeval timestamp;
		unsigned counter;
	}seedData;

	gettimeofday(&seedData.timestamp, NULL);
	static unsigned suCounter = 0;
	seedData.counter = ++suCounter;

	our_MD5Data((unsigned char *)(&seedData), sizeof(seedData), nonceBuf);

	return 0;
}

int genRealm(char *realmBuf)
{
	if(realmBuf == NULL)
	{	
		return -1;
	}
	memcpy(realmBuf, "TINADY RTSP SERVER", strlen("TINADY RTSP SERVER"));
	realmBuf[strlen("TINADY RTSP SERVER")] = '\0';

	return 0;
}
#endif
int RtspServerInit(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	TRtspServer *ptServer  = NULL;
	int iRet = -1;
	int i = 0;
	int j = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	ptServer = &ptOnvifCtrl->m_stRtspServer;
	
	g_ptRtspServer = ptServer;
	
	//memory poll
	if (datacenter_PoolInit(RTSP_MAX_MEM_POOL))
	{
		td_printf(0, "init data pool error!\n");
		return TD_ERROR;
	}
	
	//save user data
	//ptServer->m_tNvsInfo  = ptOnvifCtrl->m_stNvsInfo;
	memcpy(&ptServer->m_tNvsInfo, &ptOnvifCtrl->m_stNvsInfo, sizeof(TNvsInfo));
	ptServer->m_pUserData = _pThis;

#ifdef WITH_MD5
	genRealm(ptServer->m_tRtspAuth.m_cRealm);
	genNonce(ptServer->m_tRtspAuth.m_cNonce);
#endif
	
	//init http connection
	iRet = InitConnection(&ptServer->m_tHttpConnect, CONNECT_TYPE_SERVER, NET_TCP, 9100, _pThis);
	if(iRet != TD_OK)
	{
		return TD_ERROR;
	}

	//bind http accept
	iRet = BindCallBack(&ptServer->m_tHttpConnect, (void *)AcceptCallBack, NULL, NULL);
	if(iRet != TD_OK)
	{
		UnInitConnection(&ptServer->m_tHttpConnect);
		return TD_ERROR;
	}
	
	//init rtsp connection
	iRet = InitConnection(&ptServer->m_tRtspConnect, CONNECT_TYPE_SERVER, NET_TCP, ptServer->m_iLPort, _pThis);
	if(iRet != TD_OK)
	{
		return TD_ERROR;
	}

	//bind rtsp accept
	iRet = BindCallBack(&ptServer->m_tRtspConnect, (void *)AcceptCallBack, NULL, NULL);
	if(iRet != TD_OK)
	{
		UnInitConnection(&ptServer->m_tRtspConnect);
		return TD_ERROR;
	}

	//creat list
	//ptServer->m_ptDListSession = adt_dlist_create(sizeof(TRtspSession));
	ptServer->m_ptDListSession = adt_dlist_create(sizeof(TRtspSession *));
	if(NULL == ptServer->m_ptDListSession)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	//init channel
	for(i = 0; i < MAX_STREAM; i++)
	{
		for(j = 0; j < MAX_CHANNEL_NUM; j++)
		{
			ChannelInit(&ptServer->m_tChanel[i][j]);
			ptServer->m_tChanel[i][j].m_iStream = i;
			ptServer->m_tChanel[i][j].m_iChan = j;
			ptServer->m_tChanel[i][j].m_pServer = ptServer;
		}
	}

	//init sessions
	MutexLockInit(&g_stSessionLock);
	g_ptRtspSession = (TRtspSession*)calloc(MAX_SESSION_NUM, sizeof(TRtspSession));
	if(NULL == g_ptRtspSession)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}
	for(i=0; i<MAX_SESSION_NUM; i++)
	{
		g_ptRtspSession[i].m_iStat = SESSION_UNCONNECTED;
		g_ptRtspSession[i].m_iUsed = 0;
		MutexLockInit(&g_ptRtspSession[i].m_tLock);
	}

	//init udp port lock
	MutexLockInit(&g_stPortLock);

	//ok
	ptServer->m_iStat = RTSP_INITED;
	
EXIT:	
	return TD_OK;
}

int RtspServerUnInit(TRtspServer *_ptServer)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TRtspServer *ptServer = _ptServer;
	int i,j;
	
	if(NULL == ptServer)
	{
		return TD_ERROR;
	}
	
	//save user data
	ptServer->m_pUserData = NULL;

	//uninit connection
	UnInitConnection(&ptServer->m_tRtspConnect);
	
	//unbind accept
	UnBindCallBack(&ptServer->m_tRtspConnect);

	//destroy list
	adt_dlist_destroy(ptServer->m_ptDListSession);

	//uninit channel
	for(i = 0; i < MAX_STREAM; i++)
	{
		for(j = 0; j < MAX_CHANNEL_NUM; j++)
		{
			ChannelUnInit(&ptServer->m_tChanel[i][j]);
		}
	}

	//uninit sessions
	MutexLockDestroy(&g_stSessionLock);
	if(g_ptRtspSession)
	{
		free(g_ptRtspSession);
		g_ptRtspSession = NULL;
	}

	//destroy port lock
	MutexLockDestroy(&g_stPortLock);

	ptServer->m_iStat = RTSP_UNINITED;

	//memory poll
	datacenter_PoolUnint();

	g_ptRtspServer = NULL;

	return TD_OK;
}

int RtspServerStart(TRtspServer *_ptServer)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspServer *ptServer = _ptServer;
	st_ThreadParam tCmdThreadParam	 = {TaskCmdProcess, ptServer};		//状态检测线程
	st_ThreadParam tDataThreadParam  = {TaskDataProcess, ptServer};		//数据处理线程
	st_ThreadParam tHeartThreadParam = {TaskHeartProcess, ptServer};	//心跳发送线程
	st_Thread tCmdTheadID;
	st_Thread tDataTheadID;
	st_Thread tHeartTheadID;
	int iRet = 0;
	
	if(pointer_valid_check(ptServer))
	{
		return TD_ERROR;
	}
	
	//epoll
	iRet = StartNetWork(1);
	if(iRet != TD_OK)
	{
		td_printf(0, "%s %d %s StartNetWork error!\n", __FILE__, __LINE__, __FUNCTION__);
		goto EXIT;
	}

	//cmd process
	ThreadInit(&tCmdTheadID);
	iRet = ThreadCreate(&tCmdTheadID, tCmdThreadParam);
	if(iRet != TD_OK)
	{
		td_printf(0, "%s %d %s: CmdThread create failed!\n", __FILE__, __LINE__, __FUNCTION__);
		goto EXIT;
	}
	ThreadDetach(&tCmdTheadID);

	//data process
	ThreadInit(&tDataTheadID);
	iRet = ThreadCreate(&tDataTheadID, tDataThreadParam);
	if(iRet != TD_OK)
	{
		td_printf(0, "%s %d %s: DataThread create failed!\n", __FILE__, __LINE__, __FUNCTION__);
		goto EXIT;
	}
	ThreadDetach(&tDataTheadID);

	//heart process
	ThreadInit(&tHeartTheadID);
	iRet = ThreadCreate(&tHeartTheadID, tHeartThreadParam);
	if(iRet != TD_OK)
	{
		td_printf(0, "%s %d %s: HeartThread create failed!\n", __FILE__, __LINE__, __FUNCTION__);
		goto EXIT;
	}
	ThreadDetach(&tHeartTheadID);
	
	//start http      HttpServerStart
	iRet = ptServer->m_tHttpConnect.m_ServerStart(&ptServer->m_tHttpConnect);
	if(iRet != TD_OK)
	{
		td_printf(0, "%s %d %s, http server error!\n", __FILE__, __LINE__, __FUNCTION__);
		goto EXIT;
	}
	td_printf(0, "%s %d %s, http server , sock = %d", __FILE__, __LINE__, __FUNCTION__, ptServer->m_tHttpConnect.m_iSocket);
	
	//start rtsp      TcpServerStart
	iRet = ptServer->m_tRtspConnect.m_ServerStart(&ptServer->m_tRtspConnect);
	if(iRet != TD_OK)
	{
		td_printf(0, "%s %d %s rtsp m_ServerStart error!\n", __FILE__, __LINE__, __FUNCTION__);
		goto EXIT;
	}
	td_printf(0, "%s %d %s, rtsp server , sock = %d", __FILE__, __LINE__, __FUNCTION__, ptServer->m_tRtspConnect.m_iSocket);

EXIT:

	return iRet;
}

int RtspServerStop(TRtspServer *_ptServer)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TRtspServer *ptServer = _ptServer;
	
	if(NULL == ptServer)
	{
		return TD_ERROR;
	}
	
	ptServer->m_tRtspConnect.m_ServerStop(&ptServer->m_tRtspConnect);

	//epoll stop
	StopNetWork();

	return TD_OK;
}

int AcceptCallBack(int _iSocket)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	int iSocket = _iSocket;
	TRtspServer *ptServer = g_ptRtspServer;
	TRtspSession *ptSession = NULL;
	DListNode* ptNode = NULL;
	int iRet = 0;

	if(iSocket < 0)
	{
		return TD_ERROR;
	}
	
	ptSession = inner_GetSession();
	if(NULL == ptSession)
	{
		td_printf(0, "%s %d %s, can not get free session!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	td_printf(0, "%s %d %s ptSession %p\n", __FILE__, __LINE__, __FUNCTION__, ptSession);
	
	//若该session还未释放完成，则直接返回，返回前需要inner_FreeSession。
	if(MutexLockTryLock(&ptSession->m_tLock) != TD_OK)
	{
		inner_FreeSession(ptSession);
		return TD_ERROR;
	}
	
	ptSession->m_ptConnect = CreatConnection(iSocket, CONNECT_TYPE_CLIENT, NET_TCP, ptSession, (void *)DisConnectCallBack);
	if(NULL == ptSession->m_ptConnect)
	{
		td_printf(0, "%s %d %s CreatConnection error!\n", __FILE__, __LINE__, __FUNCTION__);
		inner_FreeSession(ptSession);
		iRet = TD_ERROR;
		goto EXIT;
	}
	//if create connect successful, we add it to session list.
	ptNode = inner_AddRtspSession(ptServer, ptSession);
	//td_printf(0, "%s %d %s, session %p\n", __FILE__, __LINE__, __FUNCTION__, adt_dlist_getdata(ptNode));
	if(NULL == ptNode)
	{
		td_printf(0, "%s %d %s, inner_AddRtspSession error!\n", __FILE__, __LINE__, __FUNCTION__);
		inner_FreeSession(ptSession);
		iRet = TD_ERROR;
		goto EXIT;
	}
	ptSession->m_ptNode = ptNode;
	ptSession->m_iSessionID = inner_GenSessionID();
	ptSession->m_tRtpInfo.m_tARtpHeader.ssrc = inner_GenSessionID();//init ssrc
	ptSession->m_tRtpInfo.m_tVRtpHeader.ssrc = inner_GenSessionID();
	ptSession->m_pServer = ptServer;
	ptSession->m_iStat = SESSION_ACCEPTED;
	
EXIT:
	MutexLockUnLock(&ptSession->m_tLock);
	return iRet;
}

// reset http POST session resource...

static int CreateResetThread(TRtspSession *_ptSession)
{
	TRtspSession *ptSession = _ptSession;
	st_Thread tTheadID = {0};
	st_ThreadParam tThreadParam = {0};
	int iRet = 0;

	if(NULL == ptSession)
	{
		td_printf(0, "%s %d %s ptSession is null\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	tThreadParam.start_address = ResetRtspSession;
	tThreadParam.userdata = (void *)ptSession;
	ThreadInit(&tTheadID);
	iRet = ThreadCreate(&tTheadID, tThreadParam);
	if(iRet != TD_OK)
	{
		td_printf(0, "%s %d %s: Thread create failed!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	ThreadDetach(&tTheadID);

	return TD_OK;
}

int DisConnectCallBack(TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TConnection *ptConnect = _ptConnect;
	TRtspSession *ptSession = NULL;
	TRtspSession *ptGetSession = NULL;

	if(NULL == ptConnect)
	{
		td_printf(0, "%s %d %s ptSession is null\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	
	ptConnect->m_iStat = SC_CLOSEING;
	ptSession = (TRtspSession *)ptConnect->m_pUserData;
	ptGetSession = FindSessionByHttpCookie(ptSession, ptSession->m_pcHttpSessionCookie);
	//td_printf(0, "%s %d %s ptSession %p\n", __FILE__, __LINE__, __FUNCTION__, ptSession);
	CreateResetThread(ptSession);
	if(NULL != ptGetSession)
	{
		td_printf(0, "%s %d %s ptPostSession %p\n", __FILE__, __LINE__, __FUNCTION__, ptGetSession);
		CreateResetThread(ptGetSession);
	}
		
	return TD_OK;
}


int ReleaseRtspSession(TRtspSession *_ptSession)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = _ptSession;
	TRtspSession *ptGetSession = NULL;	
	
	if(NULL == ptSession)
	{
		td_printf(0, "%s %d %s ptSession is null\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	//td_printf(0, "%s %d %s ptSession %p\n", __FILE__, __LINE__, __FUNCTION__, ptSession);

	ptGetSession = FindSessionByHttpCookie(ptSession, ptSession->m_pcHttpSessionCookie);
	
	CreateResetThread(ptSession);
	if(NULL != ptGetSession)
	{
		td_printf(0, "%s %d %s ptPostSession %p\n", __FILE__, __LINE__, __FUNCTION__, ptGetSession);
		CreateResetThread(ptGetSession);
	}
		
	return TD_OK;
}

void* ResetRtspSession(void *_pArg)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = (TRtspSession *)_pArg;
	TRtspServer *ptServer = g_ptRtspServer;
	func_info(-1);
	
	if(NULL == ptSession)
	{
		td_printf(0, "%s %d %s, input param NULL!\n", __FILE__, __LINE__, __FUNCTION__);
		return NULL;
	}
	td_printf(0, "%s %d %s ptSession %p\n", __FILE__, __LINE__, __FUNCTION__, ptSession);

	MutexLockLock(&ptSession->m_tLock);

	if(ptSession->m_iStat == SESSION_UNCONNECTED)
	{
		MutexLockUnLock(&ptSession->m_tLock);
		goto EXIT;
	}

	//只有当请求视频后才断开。
	if(ptSession->m_iStat >= SESSION_DESCRIBE)
	{
		ChannelDisConnect(&ptServer->m_tChanel[ptSession->m_iStream][ptSession->m_iChan]);
	}
	
	//release connection
	if(ptSession->m_iNetMode == NET_UDP)
	{
		if(ptSession->m_iSetupVideo)
		{
			FreePort(ptSession->m_tVRtpConnect.m_iLPort);
			td_printf(0, "%s %d %s, to DestroyConnection 1", __FILE__, __LINE__, __FUNCTION__);
			DestroyConnection(&ptSession->m_tVRtpConnect);
			td_printf(0, "%s %d %s, to DestroyConnection 2", __FILE__, __LINE__, __FUNCTION__);
			DestroyConnection(&ptSession->m_tVRtcpConnect);
		}
		if(ptSession->m_iSetupAudio)
		{
			FreePort(ptSession->m_tARtpConnect.m_iLPort);
			td_printf(0, "%s %d %s, to DestroyConnection 3", __FILE__, __LINE__, __FUNCTION__);
			DestroyConnection(&ptSession->m_tARtpConnect);
			td_printf(0, "%s %d %s, to DestroyConnection 4", __FILE__, __LINE__, __FUNCTION__);
			DestroyConnection(&ptSession->m_tARtcpConnect);
		}
	}
	td_printf(0, "%s %d %s, to DestroyConnection 5", __FILE__, __LINE__, __FUNCTION__);
	DestroyConnection(ptSession->m_ptConnect);
	if(ptSession->m_ptConnect)
	{
		free(ptSession->m_ptConnect);
		ptSession->m_ptConnect = NULL;
	}
 
	//change session stat before release session lock.
	inner_FreeSession(ptSession);
	inner_DelRtspSession(ptServer, ptSession);
	ptSession->m_iSessionID = 0;
	ptSession->m_iChan = 0;
	ptSession->m_iStream = 0;
	ptSession->m_iNetMode = 0;
	ptSession->m_iSetupAudio = 0;
	ptSession->m_iSetupVideo = 0;
	memset(&ptSession->m_tRtpInfo, 0, sizeof(ptSession->m_tRtpInfo));
	memset(&ptSession->m_tRtspInfo, 0, sizeof(ptSession->m_tRtspInfo));
	ptSession->m_pServer = NULL;
	ptSession->m_uiHeartTime = 0;
	ptSession->m_pcHttpSessionCookie[0] = '\0';
	ptSession->m_iStat = SESSION_UNCONNECTED;
	
	MutexLockUnLock(&ptSession->m_tLock);
	
EXIT:
	td_printf(0, "%s %d %s end!\n", __FILE__, __LINE__, __FUNCTION__);
	return NULL;
}

