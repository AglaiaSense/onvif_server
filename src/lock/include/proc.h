//#include "head.h"

//extern FILE* fpProc;
#ifndef __PROC_H__ 
#define __PROC_H__

int prc_Init(void);

int prc_Exit(void);

void* prc_Write(void* _param);

void prc_ShowFuncInfo(const char *_cFuncName);

void prc_WriteFuncInfo(const char *_cFuncName, int iChn);

#endif
