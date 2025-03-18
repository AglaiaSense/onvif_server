#include <unistd.h>
#include <sys/syscall.h>
//#include "head.h"
#include "common.h"
#include "proc.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>

#define PTHDNUM 200
FILE* fpProc = NULL;
//extern TcpServer*		tsServer;
//extern int       g_iDiskIsFull;
//extern int       g_iDiskForJpegIsFull;

typedef struct func_info{
	int iPid;
	pthread_t iTid;
	int iPPid;
	char cFuncName[32];
}TFUNCINFO;
TFUNCINFO stFuncInfo[PTHDNUM];

int prc_Init(void)
{
	pthread_t tThread;
	if ((fpProc = fopen("/tmp/onvif", "w+")) == NULL)
	{
		td_printf(0xFF0000, "%s : Initial the proc module failed!", __FUNCTION__);
		return -1;
	}
	memset(&stFuncInfo, 0, sizeof(stFuncInfo));

	//pthread_create(&tThread, NULL, (void *)prc_Write, NULL);
	//pthread_detach(tThread);

	return 0;
}

int prc_Exit(void)
{
	if (fpProc != NULL)
	{
		fclose(fpProc);
		fpProc = NULL;
		return 0;
	}else
		return -1;
}

void* prc_Write(void* _param)
{	
	func_info(-1);
	while (1)
	{
		int i = 0;
		fseek(fpProc, 0, SEEK_SET);
		fprintf(fpProc, "PID\tPPID\tTID\tTName\n");
		for(i=0; i< PTHDNUM; i++){
			if( stFuncInfo[i].iTid > 0){
				fprintf(fpProc, "%d\t%d\t%lu\t%s\n", stFuncInfo[i].iPid,stFuncInfo[i].iPPid, stFuncInfo[i].iTid, stFuncInfo[i].cFuncName);
			}
		}
		//fprintf(fpProc, "\nchn\trec_cur\trec_pa\trec_reset\n");
		
		//for(i=0; i<mDev.m_u8VideoInput; i++)
		#if 0
		for(i=0; i<1; i++)
		{
			fprintf(fpProc, "%d\t%d\t%d\t%d\n", i, g_currentFile[i].m_FileType,g_recParam[i].m_RecordType, g_iReset[i]);
		}
		#endif
		//fprintf(fpProc, "\ng_iDiskIsFull\tg_iDiskForJpegIsFull\n");
		//fprintf(fpProc, "%d\t\t%d\n", g_iDiskIsFull, g_iDiskForJpegIsFull);
		fflush(fpProc);
		sleep(1);
	}
	return NULL;
}
void prc_ShowFuncInfo(const char *_cFuncName)
{
	if (fpProc != NULL)
	{
		int i=0;
		for(i=0; i< PTHDNUM; i++)
		{
			if(stFuncInfo[i].iTid == 0)
				break;
			else if(strcmp(stFuncInfo[i].cFuncName, _cFuncName)==0)
			{
				if(kill(stFuncInfo[i].iPid, 0) != 0) //by sunyong 20130402
					break;
			}
		}
		//stFuncInfo[i].iTid = pthread_self();
		stFuncInfo[i].iPid = syscall(224);  //by sunyong 20130402
		stFuncInfo[i].iPPid = getppid();
		strncpy(stFuncInfo[i].cFuncName, _cFuncName, 31);
	}
}
void prc_WriteFuncInfo(const char *_cFuncName, int iChn)
{
	char cString[32] = {0};

	if(iChn >= 0)
	{		
		sprintf(cString, "%s%d", _cFuncName, iChn);
	}
	else
	{
		sprintf(cString, "%s", _cFuncName);
	}
	prc_ShowFuncInfo(cString);
}


