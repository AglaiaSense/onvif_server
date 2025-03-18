/*
 *	file: task_process.c
 *	description: task process
 *	history: dongliqiang created at 20130228
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "task_process.h"
#include "rtsp_server.h"
#include "rtsp_cmd.h"
#include "rtsp_data.h"
#include "proc.h"

extern FILE* fpProc;

void *TaskCmdProcess(void *arg)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TRtspServer *ptServer = (TRtspServer *)arg;
	func_info(-1);

	while(ptServer->m_iStat == RTSP_INITED)
	{
		RtspCmdProcess(ptServer);
		usleep(10 * 1000);	//10ms
	}

	return NULL;
}
 
void *TaskDataProcess(void *arg)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TRtspServer *ptServer = (TRtspServer *)arg;
	func_info(-1);

	while(ptServer->m_iStat == RTSP_INITED)
	{
		RtspDataProcess(ptServer);
		usleep(10);
	}

	return NULL;
}

void *TaskHeartProcess(void *arg)
{
	 td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	 func_info(-1);

	 TRtspServer *ptServer = (TRtspServer *)arg;
	 
	 while(ptServer->m_iStat == RTSP_INITED)
	 {
		 RtspHeartProcess(ptServer);
		 sleep(1);
	 }
	 
	 return NULL;
}

