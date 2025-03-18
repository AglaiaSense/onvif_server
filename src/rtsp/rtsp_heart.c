/*
 *	file: rtsp_heart.c
 *	description: rtsp heart
 *	history: dongliqiang created at 20130228
 */

#include "rtsp_heart.h"
#include "rtsp_server.h"
#include "rtsp_cmd.h"

int RtspSessionHeart(TRtspSession *_ptSession)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspSession *ptSession = _ptSession;
	unsigned int now = 0;
	unsigned int bias = 0;

	if(NULL == ptSession)
	{
		td_printf(0, "%s %d %s, input param error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}

	now = get_uptime(NULL);
	bias = now - ptSession->m_uiHeartTime;

	switch(ptSession->m_iStat)
	{
		case SESSION_GET:
			if(bias > RTSP_CMD_TIMEOUT)
			{
				td_printf(0, "%s %d %s, GET timeout\n", __FILE__, __LINE__, __FUNCTION__);
				ReleaseRtspSession(ptSession);
			}
			break;

		//POST session的状态始终为POST,故此处不能省略，否则会default超时，但又不能计时，因为状态不变。退出仅由销毁GET线程连带销毁。
		case SESSION_POST:
			break;
			
		case SESSION_OPTIONS:
			if(bias > RTSP_CMD_TIMEOUT)
			{
				td_printf(0, "%s %d %s, OPTIONS timeout\n", __FILE__, __LINE__, __FUNCTION__);
				ReleaseRtspSession(ptSession);
			}
			break;

		case SESSION_DESCRIBE:
			if(bias > RTSP_CMD_TIMEOUT)
			{
				td_printf(0, "%s %d %s, DESCRIBE timeout\n", __FILE__, __LINE__, __FUNCTION__);
				ReleaseRtspSession(ptSession);
			}
			break;

		case SESSION_SETUP:
			if(bias > RTSP_CMD_TIMEOUT)
			{
				td_printf(0, "%s %d %s, SETUP timeout\n", __FILE__, __LINE__, __FUNCTION__);
				ReleaseRtspSession(ptSession);
			}
			break;

		case SESSION_PLAY:
			if(bias > RTSP_CLIENT_TIMEOUT)
			{
				td_printf(0, "%s %d %s, PLAY timeout\n", __FILE__, __LINE__, __FUNCTION__);
				ReleaseRtspSession(ptSession);
			}
			break;

		case SESSION_PAUSE:
			if(bias > RTSP_CMD_TIMEOUT)
			{
				td_printf(0, "%s %d %s, PAUSE timeout\n", __FILE__, __LINE__, __FUNCTION__);
				ReleaseRtspSession(ptSession);
			}
			break;

		case SESSION_TEARDOWN:
			if(bias > RTSP_CMD_TIMEOUT)
			{
				td_printf(0, "%s %d %s, TEARDOWN timeout\n", __FILE__, __LINE__, __FUNCTION__);
				ReleaseRtspSession(ptSession);
			}
			break;

		default:
			if(bias > RTSP_SERVER_TIMEOUT)
			{
				td_printf(0, "%s %d %s, default  RTSP_SERVER_TIMEOUT timeout, stat = %d\n", __FILE__, __LINE__, __FUNCTION__, ptSession->m_iStat);
				ReleaseRtspSession(ptSession);
			}
			break;
	}
	

	return TD_OK;
}
 
 int RtspHeartProcess(TRtspServer *_ptServer)
 {
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspServer *ptServer = _ptServer;
	DListNode* ptNode = NULL;
	TRtspSession *ptSession =NULL;
	TRtspSession **pptSession =NULL;
	int iListSize = 0;
	int iRet = 0;
	int i = 0;
 
	 if(NULL == ptServer)
	 {
	 	td_printf(0, "%s %d %s, input param error!\n", __FILE__, __LINE__, __FUNCTION__);
		 return TD_ERROR;
	 }
 
	 //heart process
	 adt_dlist_lock(ptServer->m_ptDListSession);
	iListSize = adt_dlist_size(ptServer->m_ptDListSession);
	ptNode = adt_dlist_head(ptServer->m_ptDListSession);
	for(i = 0; i < iListSize; i++)
	{
		if(NULL == ptNode)
		{
			td_printf(0, "==========warnning %s %d %s, session is null!\n", __FILE__, __LINE__, __FUNCTION__);
			iRet = TD_ERROR;
			goto EXIT;
		}
		
		pptSession = (TRtspSession**)adt_dlist_getdata(ptNode);
		if(NULL == pptSession)
		{
			td_printf(0, "==========warnning %s %d %s, session is null!\n", __FILE__, __LINE__, __FUNCTION__);
			iRet = TD_ERROR;
			goto EXIT;
		}
		ptSession = *pptSession;
		
		//td_printf(0, "%s %d %s, session lock start!\n", __FILE__, __LINE__, __FUNCTION__);
		iRet = MutexLockTryLock(&ptSession->m_tLock);
		if(iRet != TD_OK)
		{
			continue;
		}
		//after getting http GET cmd, we start count down hearttime.
		if(ptSession->m_iStat < SESSION_GET)
		{
			MutexLockUnLock(&ptSession->m_tLock);
			continue;
		}
		RtspSessionHeart(ptSession);

		MutexLockUnLock(&ptSession->m_tLock);
		
		ptNode = adt_dlist_next(ptNode);
	}

EXIT:
	adt_dlist_unlock(ptServer->m_ptDListSession);
	 return iRet;
 }

