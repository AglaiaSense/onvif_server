#ifndef __SINGLE_THREAD_H__
#define __SINGLE_THREAD_H__

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

#define MAX_THREAD_NUM 150

#ifdef getpid
#undef getpid
#define getpid() syscall(224)
#endif

#ifdef WIN32
typedef  unsigned THREAD_PROC;
#define THREAD_RETURN 0
#else
typedef void*     THREAD_PROC;
#define THREAD_RETURN (void *)0
#endif

typedef struct 
{
#ifdef WIN32 
	unsigned ( __stdcall *start_address )(void *);
#else
	void *(*start_address)(void *);
#endif
	void *userdata;
}st_ThreadParam;

typedef struct 
{
	unsigned int m_iThreadID;
#ifdef WIN32
	HANDLE m_tThread;
#else
	pthread_t m_tThread;
#endif
}st_Thread;

typedef struct pthread_info_s{
	int iPid;
	pthread_t iTid;
	int iPPid;
	char cFuncName[64];
}pthread_info_t;

typedef struct pthread_man_s
{
	pthread_info_t  m_pThreadInfo[MAX_THREAD_NUM];
	int 	m_iThreadNum;
}pthread_man_t;

int ThreadInit(st_Thread *_pThread);

int ThreadCreate(st_Thread *_pThread, st_ThreadParam _tParam);

void ThreadExit(void *_pArg);

int ThreadDetach(st_Thread *_pThread);

int ThreadJoin(st_Thread *_pThread, int _iWaitTime);

int ThreadUnInit(st_Thread *_pThread);

void ThreadInfoAdd(char * _cName);

void ThreadInfoDel(void);

int  GetThreadInfo(char *_cString ,int _iLen);

#endif
