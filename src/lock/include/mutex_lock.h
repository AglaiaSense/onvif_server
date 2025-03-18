/*
 *	file: mutex_lock.c
 *	description: mutex lock
 *	history: dongliqiang created at 20120815
 */
 
#ifndef __MUTEX_LOCK_H__
#define __MUTEX_LOCK_H__

#include "stdlib.h"
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <errno.h>
#endif

//#define DEBUG_LOCK		//just for debug
#ifdef DEBUG_LOCK
#include "debug_lock.h"
#endif

typedef struct  
{
#ifdef WIN32
	CRITICAL_SECTION m_tMutex;
#else
	pthread_mutex_t  m_tMutex;
#endif
	int   m_iInitFlag;
}TMutexLock;

void MutexLockInit(TMutexLock *_ptLock);

void MutexLockDestroy(TMutexLock * _ptLock);

void MutexLockLock(TMutexLock * _ptLock);

void MutexLockUnLock(TMutexLock * _ptLock);

int MutexLockTryLock(TMutexLock * _ptLock);

#endif

