#ifndef __timerq_h__
#define __timerq_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "basetype.h"

#define MAX_QUEUE_SIZE	10

typedef void (*SERVICEFUNC)(void *);

#define SLOW_QUEUE	0
#define FAST_QUEUE	1
int QueueServiceItem(int which, SERVICEFUNC func, void* data, int run_after/*milliseconds*/, const char *name);
void DequeueServiceItem(int which_queue, SERVICEFUNC func, void* data, BOOL bIgnoreData);

void StartTimerQueue();
void StopTimerQueue();

#ifdef __cplusplus
}
#endif

#endif

