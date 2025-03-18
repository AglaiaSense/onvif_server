/*
 *	file: onvif_epool.c
 *	description: onvif epool
 *	history: dongliqiang created at 20120815
 */

#include <unistd.h>
#include "epoll.h"
#include <sys/epoll.h>
#include "single_thread.h"
#include "string.h"
#include "common.h"
#include <stdio.h>
#include "proc.h"

static st_Thread g_stEpoolThread;
static int ep = -1;
static struct epoll_event events[MAX_EVENTS_EPOLL];
extern FILE* fpProc;

void * TskEpoll(void *arg)
{
 	char cName[64];
 	memset(cName,0,sizeof(cName));
 	strcpy(cName,__func__);
	ThreadInfoAdd(cName);
	func_info(-1);

	while(ep!=-1)
	{
		EpollProcessEvents();
	}
	return NULL;
}

int StartNetWork(int _iCoreCount)
{
	st_ThreadParam tThreadParam = {TskEpoll, NULL};
	int iRet = 0;
	
	if(EpollInit() != TD_OK)
	{
		return TD_ERROR;
	}
	
	ThreadInit(&g_stEpoolThread);
	iRet = ThreadCreate(&g_stEpoolThread, tThreadParam);
	if(iRet != TD_OK)
	{
		close(ep);
		return iRet;
	}
	ThreadDetach(&g_stEpoolThread);

	return TD_OK;
}

int StopNetWork()
{
	EpollDone();
	ThreadJoin(&g_stEpoolThread, -1);
	ThreadUnInit(&g_stEpoolThread);

	return TD_OK;
}

int EpollInit(void)
{
	if (ep == -1) 
	{
		ep = epoll_create(MAX_FD_EPOLL);

		if (ep == -1) 
		{
			return TD_ERROR;
		}
	}

	return TD_OK;
}

int EpollDone(void)
{
	if (close(ep) == -1) 
	{
		return TD_ERROR;
	}
	ep = -1;
	return TD_OK;
}

int  EpollAddConnection(TConnection *c,int _iType)
{
	struct epoll_event  ee = {0};

	if(MODE_IN == _iType)
	{
		ee.events = EPOLLIN;
	}
	else if(MODE_OUT == _iType)
	{
		ee.events = EPOLLOUT;
	}
	else if(MODE_IO == _iType)
	{
		ee.events = EPOLLIN | EPOLLOUT;
	}	
	else if(MODE_IOE == _iType)
	{
		ee.events = EPOLLIN | EPOLLOUT | EPOLLET;
	}
	else
	{
		ee.events = EPOLLIN | EPOLLOUT | EPOLLET;
	}

	ee.data.ptr = (void *)c;

	if(epoll_ctl(ep, EPOLL_CTL_ADD, c->m_iSocket, &ee) == -1) 
	{
		return TD_ERROR;
	}
	return TD_OK;

}

int EpollDelConnection(TConnection *c)
{
	int                 op;
	struct epoll_event  ee;

	op = EPOLL_CTL_DEL;
	ee.events = 0;
	ee.data.ptr = NULL;

	if (epoll_ctl(ep, op, c->m_iSocket, &ee) == -1) 
	{
		return TD_ERROR;
	}

	return TD_OK;
}

void EpollProcessEvents()
{
	int	i;
	int	nfds;
	TConnection * pThis =NULL;	
	nfds = epoll_wait(ep, events, MAX_EVENTS_EPOLL, -1);//等待epoll事件的发生
	if(nfds <= 0)
	{
		usleep(1);	//减小CPU占用率
		return ;
	}

	for(i = 0; i < nfds; i++)
	{	
		pThis = NULL;
		if(events[i].events & EPOLLIN)
		{
			pThis = (TConnection *)events[i].data.ptr;
			if(pThis == NULL)
				continue;
			//server
			if(pThis->m_iType == CONNECT_TYPE_SERVER)
			{
				if(NULL != pThis->m_AcceptEpoll)
				{
					pThis->m_AcceptEpoll(pThis);//TcpAcceptEpoll UdpRecvEpoll
				}
			}
			//client
			else if(pThis->m_iType == CONNECT_TYPE_CLIENT)
			{
				if(NULL != pThis->m_RecvEpoll)
				{
					pThis->m_RecvEpoll(pThis);
				}
			}
		}
		else if(events[i].events & EPOLLOUT)
		{
			pThis = (TConnection *)events[i].data.ptr;
			if(pThis == NULL)
				continue;
			
			if(NULL != pThis->m_SendEpoll)
			{
				pThis->m_SendEpoll(pThis);//TcpSendEpoll
			}
		}
		else
		{
			continue;
		}
	}

}

