#ifndef _DATA_LOCK_H_
#define _DATA_LOCK_H_

// 2010/05/31 duxiulong created
// 数据锁实现

#define USE_SYS_MUTEX	1

#if USE_SYS_MUTEX
#include <pthread.h>

#define TDataLock 			pthread_mutex_t
#define InitDataLock(lck)		pthread_mutex_init(&(lck), NULL)
#define LockDataLock(lck)		pthread_mutex_lock(&(lck))
#define UnLockDataLock(lck)	pthread_mutex_unlock(&(lck))
#else
#include <unistd.h>

#define TDataLock	int
#define InitDataLock(lck)		(lck) = 0
#define LockDataLock(lck)		{while ((lck) > 0) {usleep(10);}; (lck)++;}
#define UnLockDataLock(lck)	(lck)--
#endif

#endif
