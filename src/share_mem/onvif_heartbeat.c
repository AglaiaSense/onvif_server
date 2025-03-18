/*******************************************************************
FileName	   	:	 onvif_heartbeat.c
Description   	:  heartbeat
History		:  
             			20110215   create by tcf
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "onvif_shm_cmd.h"
#include "common.h"
#include "proc.h"

extern FILE* fpProc;


void *onvif_heartbeat(void *arg)
{
	int iRet = -1; 
	
#ifdef DEBUG_LV1
	char FileName[20] = "/proc/";
	char BufStatm[60];
	FILE *fp 		= NULL;
	char *p 		= NULL;
	int ReadSize 	= 0;
	int OldRSS 		= 0;
	int RSS 		= 0;
	int PID	 		= 0;
	int i 			= 0;

	PID = getpid();
	sprintf(FileName + 6, "%d", PID);
	strcat(FileName, "/statm");

	fp = fopen(FileName, "r + b");
	if(fp == NULL)
	{
		perror("proc");
	}
#endif
 
	func_info(-1);
	
	while(1)
	{
		iRet = OnvifCmdHeartbeat();
		if(iRet < 0)
		{
			td_printf(0,  "OnvifCmdHeartbeat error!\n");
		}
		sleep(1);
				
#ifdef DEBUG_LV1  

	    fseek(fp, 0, SEEK_SET);
		memset(BufStatm, 0, 60);
	    ReadSize = fread(BufStatm, sizeof(char), 60, fp);
	    
	    p = strstr(BufStatm, " ");
	    if(p != NULL)
	    {
			i = p - BufStatm;
			p = strstr(BufStatm + i, " ");
			if(p != NULL)
			{
				*p = '\0';
				RSS = atoi(BufStatm + i + 1);
			}
	    }

		if(RSS != OldRSS)
		{
		//    td_printf(0, "RSS is %d Inc is %d\n", RSS * 4, RSS - OldRSS);
		    OldRSS = RSS;
		}
		
#endif 
#ifdef DEBUG_MEM
		int i = 0;
		for(i = 0; i < mem_trace.trace_cnt; i++)
		{
			td_printf(0, "not release malloc is %s, call_line is %d address 0x%x, size %d\n", mem_trace.func_name[i], mem_trace.call_line[i], (int)mem_trace.address[i], mem_trace.size[i]);
		}
#endif

	}
	
#ifdef DEBUG_LV1
  fclose(fp);
#endif

	return NULL;
}
