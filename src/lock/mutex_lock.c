/*
 *	file: mutex_lock.c
 *	description: mutex lock
 *	history: dongliqiang created at 20120815
 */

#include <stdio.h>
#include "mutex_lock.h"

void MutexLockInit(TMutexLock*  _ptLock)
{
#ifdef WIN32
	InitializeCriticalSection(&_ptLock->m_tMutex);
#else
	pthread_mutex_init(&_ptLock->m_tMutex, NULL);
#endif
	_ptLock->m_iInitFlag = 1;
}

void MutexLockDestroy(TMutexLock *_ptLock)
{
	if (!_ptLock || !_ptLock->m_iInitFlag)
		return;
#ifdef WIN32
	DeleteCriticalSection(&_ptLock->m_tMutex);
#else
	pthread_mutex_destroy(&_ptLock->m_tMutex);
#endif
	_ptLock->m_iInitFlag = 0;
}

void MutexLockLock(TMutexLock *_ptLock)
{
	if (!_ptLock || !_ptLock->m_iInitFlag)
		return;
#ifdef WIN32
	EnterCriticalSection(&_ptLock->m_tMutex);
#else
	pthread_mutex_lock(&_ptLock->m_tMutex);
#endif

}

void MutexLockUnLock(TMutexLock *_ptLock)
{
	if (!_ptLock || !_ptLock->m_iInitFlag)
		return;
#ifdef WIN32
	LeaveCriticalSection(&_ptLock->m_tMutex);
#else
	pthread_mutex_unlock(&_ptLock->m_tMutex);
#endif
}

int MutexLockTryLock(TMutexLock *_ptLock)
{
	if (!_ptLock || !_ptLock->m_iInitFlag)
		return -1;
#ifdef WIN32
	if (TryEnterCriticalSection(&_ptLock->m_tMutex))
	{
		return 0;
	}
	else
	{
		return 1;
	}
#else
	return pthread_mutex_trylock(&_ptLock->m_tMutex);
#endif
}

