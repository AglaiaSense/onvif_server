
#include "single_thread.h"
#include "common.h"
#include "pthread.h"
#include "string.h"
#include "stdio.h"

pthread_man_t gPthreadInfo;

int ThreadInit(st_Thread *_pThread)
{
	if (!_pThread)
		return -1;

	_pThread->m_iThreadID = -1;
#ifdef WIN32
	_pThread->m_tThread = NULL;
#else
	_pThread->m_tThread = (pthread_t)NULL;
#endif
	return 0;
}

int ThreadUnInit( st_Thread *_pThread )
{
	if (!_pThread)
		return -1;
#ifdef WIN32
	if (_pThread->m_tThread)
		CloseHandle(_pThread->m_tThread);
#endif
#ifdef WIN32
	_pThread->m_tThread = NULL;
#else
	_pThread->m_tThread = (pthread_t)NULL;
#endif
	return 0;
}

int ThreadCreate(st_Thread *_pThread, st_ThreadParam _tParam)
{
	if (!_pThread)
		return -1;
#ifdef WIN32
	if (!_pThread->m_tThread)
	{
		_pThread->m_tThread =(HANDLE) _beginthreadex(NULL, 0, _tParam.start_address, _tParam.userdata, 0, &(_pThread->m_iThreadID));
		if (_pThread->m_tThread)
			return 0;
		else
			return -1;
	}
#else
	if (!_pThread->m_tThread)
		return pthread_create(&(_pThread->m_tThread), NULL, _tParam.start_address, _tParam.userdata);
#endif

	return -1;
}

int ThreadJoin(st_Thread *_pThread, int _iWaitTime)
{
	if (!_pThread)
		return -1;

#ifdef WIN32
	if (_pThread->m_tThread)
	{
		return WaitForSingleObject(_pThread->m_tThread, _iWaitTime);
	}
#else  
	if (_pThread->m_tThread)
	{
		return pthread_join(_pThread->m_tThread, NULL);
	}
#endif

	return -1;
}

void ThreadExit(void *_pArg)
{
#ifdef WIN32
#else  
#endif
}

int ThreadDetach(st_Thread *_pThread)
{
	if (!_pThread)
		return -1;

#ifdef WIN32
#else  
	if (_pThread->m_tThread)
	{
		return pthread_detach(_pThread->m_tThread);
	}
#endif

	return -1;
}

void ThreadInfoAdd(char * _cName)
{
	if(gPthreadInfo.m_iThreadNum >= MAX_THREAD_NUM)
	{
		return ;
	}
	gPthreadInfo.m_pThreadInfo[gPthreadInfo.m_iThreadNum].iTid = pthread_self();
	gPthreadInfo.m_pThreadInfo[gPthreadInfo.m_iThreadNum].iPid = getpid();
	gPthreadInfo.m_pThreadInfo[gPthreadInfo.m_iThreadNum].iPPid = getppid();
	gPthreadInfo.m_pThreadInfo[gPthreadInfo.m_iThreadNum].cFuncName[0]=0;
	strcpy(gPthreadInfo.m_pThreadInfo[gPthreadInfo.m_iThreadNum].cFuncName, _cName);
	gPthreadInfo.m_iThreadNum++;
	return;
}

void ThreadInfoDel(void)
{	
	int i=0;
	pthread_info_t pthreadinfo;
	memset(&pthreadinfo,0,sizeof(pthread_info_t));
	
	pthreadinfo.iTid = pthread_self();
	pthreadinfo.iPid = getpid();
	pthreadinfo.iPPid = getppid();
	pthreadinfo.cFuncName[0]=0;
	
	if(gPthreadInfo.m_iThreadNum == 0)
	{
		return ;
	}

	for(i=0;i<gPthreadInfo.m_iThreadNum;i++)
	{
		if(gPthreadInfo.m_pThreadInfo[i].iTid == pthreadinfo.iTid)
		{
			break;
		}
	}
	if(i<gPthreadInfo.m_iThreadNum)
	{
		memcpy(&gPthreadInfo.m_pThreadInfo[i],&gPthreadInfo.m_pThreadInfo[--gPthreadInfo.m_iThreadNum],sizeof(pthread_info_t));
	}
	return;
}


int  ThreadInfoGet(char *_cString ,int _iLen)
{
	int i = 0;
	int iLen = 0;
	if(NULL==_cString)
	{
		return iLen;
	}
	char * cString = _cString;
	char cPara[512];
	int iTmpLen = 0;
	
	for(i=0;i<gPthreadInfo.m_iThreadNum;i++)
	{
		memset(cPara,0,sizeof(cPara));
		sprintf(cPara, "%d\t%d\t%lu\t%s\n", \
			gPthreadInfo.m_pThreadInfo[i].iPid, \
			gPthreadInfo.m_pThreadInfo[i].iPPid, \
			gPthreadInfo.m_pThreadInfo[i].iTid, \
			gPthreadInfo.m_pThreadInfo[i].cFuncName);
		iTmpLen = strlen(cPara);
		if((iLen+iTmpLen)>_iLen)
		{
			return iLen;
		}
		strcat(cString,cPara);
		iLen = iLen+iTmpLen;
	}
	return iLen;
	
}

