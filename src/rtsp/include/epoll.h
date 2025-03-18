/*
 *	file: epoll.h
 *	description: minisdk epool
 *	history: dongliqiang created at 20120815
 */

#ifndef __EPOOL_H__
#define __EPOOL_H__

#include "connection.h"

#define		MAX_EVENTS_EPOLL		200
#define		MAX_FD_EPOLL			200

typedef enum
{
	MODE_IN = 0,
	MODE_OUT,
	MODE_IO,
	MODE_IOE,
}EPOOL_TYPE;


int StartNetWork(int _iCoreCount);

int StopNetWork(void);

int EpollInit(void);

int EpollDone(void);

int EpollAddConnection(TConnection *c,int _iType);

int EpollDelConnection(TConnection *c);

void EpollProcessEvents(void);

#endif

