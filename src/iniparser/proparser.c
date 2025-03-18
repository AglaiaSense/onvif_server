/*
 *	file: proparser.c
 *	description: profile parse
 *	history: dongliqiang created at 20120224
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include "proparser.h"
#include "iniparser.h"
#include "strlib.h"
#include "common.h"
#include "onvif_sdk.h"

#define OK            		0
#define ERROR 				-1
#define EXIST				-2
#define NOT_EXIST         	-3
#define PERMISSION_DENIED   -4

#define PROFIEL_TEMPLET  "./templet.ini"


static pthread_mutex_t g_stMutexLock;	//�����ļ�����������

/**************************************************
*����: 	 	�豸��һ������ʱ,��������·����ģ��
            ������Ӧ�������ļ�config_onvif.ini
*����: 	 	_pThis:    ȫ�ֱ���ָ��
*����ֵ:	0 - �ɹ�, !0 - ʧ��
*�޸���ʷ:20121018 v1.0 modify by ptb
**************************************************/
int BuildProfile(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	off_t fsize;
	char *pEnd 		= "[profile_end]\n";
	char pIndex[8];
	char *pMainStream = NULL;
	char *pSubStream  = NULL;
	char *pFlag1 	  = NULL;
	char *pFlag2 	  = NULL;
	char *pBuf		  = NULL;
	char *pTmp		  = NULL;
	int iMainStreamNum = 0;
	int iSubStreamNum  = 0;
	int fd_from1 = 0;
	int fd_to = 0;
	int iSize = 0;
	int iCnt  = 0;
	int i 	  = 0;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	iMainStreamNum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	iSubStreamNum  = ptOnvifCtrl->m_stNvsInfo.m_iVENum - ptOnvifCtrl->m_stNvsInfo.m_iVINum;

	fd_from1 = open(PROFIEL_TEMPLET, O_RDONLY);
	if(fd_from1 < 0)
	{
		td_printf(0, "open %s failed!", PROFIEL_TEMPLET);
		perror("open1 111");
		goto ERR;
	}

	fd_to = open(CONFIG_ONVIF_PATH, O_CREAT|O_RDWR, 00700);
	if(fd_to < 0)
	{
		perror("open2");
		goto ERR1;
	}

	fsize = lseek(fd_from1, 0, SEEK_END);
	pBuf  = (char *)calloc(1, fsize + 1);
	pMainStream = (char *)calloc(1, fsize + 1);
	pSubStream  = (char *)calloc(1, fsize + 1);

	lseek(fd_from1, 0, SEEK_SET);
	iCnt = read(fd_from1, pBuf, fsize);
	if(iCnt < 0)
	{
		perror("read");
		goto ERR3;
	}

	close(fd_from1);
	pBuf[iCnt] = '\0';

	pFlag1 = strstr(pBuf, "FLAG_1");
	pFlag2 = strstr(pBuf, "FLAG_2");

	if(NULL == pFlag1|| NULL == pFlag2)
	{
		goto ERR3;
	}

	iSize = pFlag1 - pBuf - 1;
	iCnt = write(fd_to, pBuf, iSize);
	if(iCnt < 0)
	{
		perror("write");
		goto ERR3;
	}

	//���������������ļ���Ϣ
	memcpy(pMainStream, pFlag1 + strlen("FLAG_1"), pFlag2 - (pFlag1 + strlen("FLAG_1")));
	pMainStream[pFlag2 - ( pFlag1 + strlen("FLAG_1"))] = '\0';
	for(i = 0; i < iMainStreamNum; i++)
	{
		sprintf(pIndex, "%02d", i + 1);
		strcpy(pBuf, pMainStream);
		
		while(1)
		{
			pTmp = strstr(pBuf, "XX");
			if(NULL == pTmp)
			{
				break;
			}
			memcpy(pTmp, pIndex, 2);
		}

		lseek(fd_to, 0, SEEK_END);
		iCnt = write(fd_to, pBuf, strlen(pBuf));
		if(iCnt != strlen(pBuf))
		{
			td_printf(0, "Main Stream write error!");
			perror("write1");
			goto ERR3;
		}
	}

	//���������������ļ���Ϣ
	strcpy(pSubStream, pFlag2 + strlen("FLAG_2"));
	for(i = 0; i < iSubStreamNum; i++)
	{
		sprintf(pIndex, "%02d", i + 1);
		strcpy(pBuf, pSubStream);
		
		while(1)
		{
			pTmp = strstr(pBuf, "XX");
			if(pTmp == NULL)
			{
				break;
			}
			memcpy(pTmp, pIndex, 2);
		}

		sprintf(pIndex, "%02d", i + 1 + iMainStreamNum);
		while(1)
		{
			pTmp = strstr(pBuf, "xx");
			if(pTmp == NULL)
			{
				break;
			}
			memcpy(pTmp, pIndex, 2);
		}

		lseek(fd_to, 0, SEEK_END);

		iCnt = write(fd_to, pBuf, strlen(pBuf));
		if(iCnt != strlen(pBuf))
		{
			td_printf(0, "Sub Stream write error!");
			perror("write2");
			goto ERR3;
		}
	}

	//���������ļ���β��Ϣ
	lseek(fd_to, 0, SEEK_END);
	iCnt = write(fd_to, pEnd, strlen(pEnd));
	if(iCnt != strlen(pEnd))
	{
		td_printf(0, "The config_onvif.ini end write error!");
		perror("write3");
		goto ERR3;
	}
	
	fsync(fd_to);
	close(fd_to);
	free(pBuf);
	free(pMainStream);
	free(pSubStream);

	return TD_OK;
ERR3:
	free(pBuf);
	free(pMainStream);
	free(pSubStream);
	close(fd_to);
	remove(CONFIG_ONVIF_PATH);

ERR1:
	close(fd_from1);
ERR:
	return TD_ERROR;
}

/**************************************************
*����: 	 	���������ļ�
*����: 	 	_cProfile:		�����ļ�
*����ֵ:	dictionary
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
dictionary *LoadProfile(char *_pcProfile)
{
	dictionary *d;
	
	if(NULL == _pcProfile)
	{
		return NULL;
	}

	pthread_mutex_init(&g_stMutexLock, NULL);

	d = iniparser_load(_pcProfile);
	if(d != NULL)
	{
		if(FindProEntry(d, "profile_end", -1) != 1)
		{
			remove(CONFIG_ONVIF_PATH);
			return NULL;	
		}
	}

	return d;
}

/**************************************************
*����: 	 	�洢�����ļ�
*����: 	 	_cProfile:		�����ļ�
*����ֵ:	��
*�޸���ʷ:
	20120226 v1.0 creat by pangtb
**************************************************/
int SaveProfile(dictionary *_ptDict, char *_pcFilePath)
{
	td_printf(SYS_INFO_PRINTF, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	FILE *ptFile = NULL;
	char TmpFilePath[64];
	sprintf(TmpFilePath, "%s_tmp", _pcFilePath);

	pthread_mutex_lock(&g_stMutexLock);

	rename(_pcFilePath, TmpFilePath);
	
	ptFile = fopen(TmpFilePath, "w");
	if(NULL == ptFile)
	{
		pthread_mutex_unlock(&g_stMutexLock);
		return -1;
	}

	iniparser_dump_ini(_ptDict, ptFile);
	
	fflush(ptFile);
	fclose(ptFile);

	rename(TmpFilePath, _pcFilePath);
	
	pthread_mutex_unlock(&g_stMutexLock);

	td_printf(SYS_INFO_PRINTF, "%s %d %s end!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;	
}

/**************************************************
*����: 	 	�ͷ������ļ�
*����: 	 	_cProfile:		�����ļ�
*����ֵ:	��
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
void ReleaseProfile(dictionary *_ptDict)
{
	if(NULL == _ptDict)
	{
		return ;
	}

	iniparser_freedict(_ptDict);
	
	pthread_mutex_destroy(&g_stMutexLock);

}

/**************************************************
*����: 	 	��ȡ�����ļ���Ӧstring��Ϣ
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_Default:		Ĭ�Ϸ���ֵ
*����ֵ:	Ҫ��ȡ���ַ���
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
char *GetProStr(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, char *_cDefault)
{
	char *cRet = NULL;
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmp[512] = {0};

	if((NULL == _ptDict) || (NULL == _pcSec) || (NULL == _pcKey))
	{
	 	return _cDefault;
	}

	//section
	if(strlen(_pcSec) > 128 - 3)
	{
		return _cDefault;
	}

	if(_iSecNum > 0)
	{
		sprintf(cTmpSec, "%s%02d", _pcSec, _iSecNum);
	}
	else
	{
		strcpy(cTmpSec, _pcSec);
	}

	//key
	if(strlen(_pcKey) > 128 - 3)
	{
		return _cDefault;
	}

	if(_iKeyNum > 0)
	{
		sprintf(cTmpKey, "%s%02d", _pcKey, _iKeyNum);
	}
	else
	{
		strcpy(cTmpKey, _pcKey);
	}

	//combin
	if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
	{
		return _cDefault;
	}
	else
	{
		sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
	}

	pthread_mutex_lock(&g_stMutexLock);

	cRet = iniparser_getstring(_ptDict, cTmp, _cDefault);

	pthread_mutex_unlock(&g_stMutexLock);

	return cRet;
	 
}

/**************************************************
*����: 	 	��ȡ�����ļ���Ӧint��Ϣ
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_iDefault:		Ĭ�Ϸ���ֵ
*����ֵ:	Ҫ��ȡ��������
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
int GetProInt(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, int _iDefault)
{
	int iRet = 0;
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmp[512] = {0};

	if((NULL == _ptDict) || (NULL == _pcSec) || (NULL == _pcKey))
	{
	 	return _iDefault;
	}


	//section
	if(strlen(_pcSec) > 128 - 3)
	{
		return _iDefault;
	}

	if(_iSecNum > 0)
	{
		sprintf(cTmpSec, "%s%02d", _pcSec, _iSecNum);
	}
	else
	{
		strcpy(cTmpSec, _pcSec);
	}

	//key
	if(strlen(_pcKey) > 128 - 3)
	{
		return _iDefault;
	}

	if(_iKeyNum > 0)
	{
		sprintf(cTmpKey, "%s%02d", _pcKey, _iKeyNum);
	}
	else
	{
		strcpy(cTmpKey, _pcKey);
	}

	//combin
	if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
	{
		return _iDefault;
	}
	else
	{
		sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
	}

	pthread_mutex_lock(&g_stMutexLock);

	iRet = iniparser_getint(_ptDict, cTmp, _iDefault);

	pthread_mutex_unlock(&g_stMutexLock);

	return iRet;
}

/**************************************************
*����: 	 	��ȡ�����ļ���Ӧdouble��Ϣ
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_dDefault:		Ĭ�Ϸ���ֵ
*����ֵ:	Ҫ��ȡ��������
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
double GetProDouble(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, double _dDefault)
{
	double dRet = 0.0;
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmp[512] = {0};

	if((NULL == _ptDict) || (NULL == _pcSec) || (NULL == _pcKey))
	{
	 	return _dDefault;
	}


	//section
	if(strlen(_pcSec) > 128 - 3)
	{
		return _dDefault;
	}

	if(_iSecNum > 0)
	{
		sprintf(cTmpSec, "%s%02d", _pcSec, _iSecNum);
	}
	else
	{
		strcpy(cTmpSec, _pcSec);
	}

	//key
	if(strlen(_pcKey) > 128 - 3)
	{
		return _dDefault;
	}

	if(_iKeyNum > 0)
	{
		sprintf(cTmpKey, "%s%02d", _pcKey, _iKeyNum);
	}
	else
	{
		strcpy(cTmpKey, _pcKey);
	}

	//combin
	if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
	{
		return _dDefault;
	}
	else
	{
		sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
	}

	pthread_mutex_lock(&g_stMutexLock);

	dRet = iniparser_getdouble(_ptDict, cTmp, _dDefault);

	pthread_mutex_unlock(&g_stMutexLock);

	return dRet;
}

/**************************************************
*����: 	 	��ȡ�����ļ���Ӧbool��Ϣ
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_iDefault:		Ĭ�Ϸ���ֵ
*����ֵ:	Ҫ��ȡ��������
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
int GetProBool(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, int _iDefault)
{
	int bRet = 0;
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmp[512] = {0};

	if((NULL == _ptDict) || (NULL == _pcSec) || (NULL == _pcKey))
	{
	 	return _iDefault;
	}


	//section
	if(strlen(_pcSec) > 128 - 3)
	{
		return _iDefault;
	}

	if(_iSecNum > 0)
	{
		sprintf(cTmpSec, "%s%02d", _pcSec, _iSecNum);
	}
	else
	{
		strcpy(cTmpSec, _pcSec);
	}

	//key
	if(strlen(_pcKey) > 128 - 3)
	{
		return _iDefault;
	}

	if(_iKeyNum > 0)
	{
		sprintf(cTmpKey, "%s%02d", _pcKey, _iKeyNum);
	}
	else
	{
		strcpy(cTmpKey, _pcKey);
	}

	//combin
	if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
	{
		return _iDefault;
	}
	else
	{
		sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
	}

	pthread_mutex_lock(&g_stMutexLock);

	bRet = iniparser_getboolean(_ptDict, cTmp, _iDefault);

	pthread_mutex_unlock(&g_stMutexLock);

	return bRet;
}

/**************************************************
*����: 	 	���section�Ƿ����
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
*����ֵ:	����(1), ������(0)
*�޸���ʷ:
	20120224 v1.0 creat by sy
**************************************************/
int FindProEntry(dictionary *_ptDict, char *_pcSec, int _iSecNum)
{
	char cSec[64];
	
	if(_iSecNum < 0)
	{
		strcpy(cSec, _pcSec);
	}
	else
	{
		sprintf(cSec, "%s%02d", _pcSec, _iSecNum);
	}
	
	return iniparser_find_entry(_ptDict, cSec);
}

/**************************************************
*����: 	 	���������ļ���Ϣ.
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 ׷��ͨ���ŵ�val, _iValNum <= 0 ��׷��
			_iID:			�����ļ�����ID: CFG_ADD - ���, CFG_RM - ɾ��, CFG_REPLACE - �滻
*����ֵ:	0 - �ɹ�, -1 - ʧ��, -2 - ����, -3 - ������
*�޸���ʷ:
	20120227 v1.0 creat by pangtb
**************************************************/
int SetProStr(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, char *_pcVal, int _iValNum, int _iID)
{
	int iRet = 0;
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmpVal[128] = {0};
	char cTmp[512] = {0};
	
	if(NULL == _ptDict || NULL == _pcSec)
	{
		return -1;
	}

	//section
	if(strlen(_pcSec) > 128 - 3)
	{
		return -1;
	}

	if(_iSecNum > 0)
	{
		sprintf(cTmpSec, "%s%02d", _pcSec, _iSecNum);
	}
	else
	{
		strcpy(cTmpSec, _pcSec);
	}

	//key
	if(NULL != _pcKey)
	{
		if(strlen(_pcKey) > 128 - 3)
		{
			return -1;
		}

		if(_iKeyNum > 0)
		{
			sprintf(cTmpKey, "%s%02d", _pcKey, _iKeyNum);
		}
		else
		{
			strcpy(cTmpKey, _pcKey);
		}
	}

	//val
	if(NULL != _pcVal)
	{
		if(strlen(_pcVal) > 128 - 3)
		{
			return -1;
		}

		if(_iValNum > 0)
		{
			sprintf(cTmpVal, "%s%02d", _pcVal, _iValNum);
		}
		else
		{
			strcpy(cTmpVal, _pcVal);
		}
	}
	else
	{
		sprintf(cTmpVal, "%d", _iValNum);
	}

	//combin
	if(strlen(cTmpKey) > 0)
	{
		if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
		{
			return -1;
		}
		else
		{
			sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
		}
	}
	else
	{
		strcpy(cTmp, cTmpSec);
	}

	//process
	pthread_mutex_lock(&g_stMutexLock);
	
	//�жϲ���ID
	if(_iID == CFG_ADD)
	{
		iRet = iniparser_find_entry(_ptDict, cTmpSec);
		if(0 == iRet)	//section������
		{
			iRet = iniparser_add_entry(_ptDict, cTmpSec, NULL, NULL);
			if(iRet)
			{
				pthread_mutex_unlock(&g_stMutexLock);
	
				return -1; 
			}
		}
		else	//section����, ���key��val
		{
 			iRet = iniparser_add_entry(_ptDict,cTmpSec, cTmpKey, cTmpVal);
			if(iRet)
			{
				pthread_mutex_unlock(&g_stMutexLock);
				
				return -1;
			}
		}
	
	}
	
	else if(_iID == CFG_RM)
	{
		if(strlen(cTmpKey) <= 0)
		{
			iniparser_sec_unset(_ptDict, cTmp); //keyΪ��,ɾ��section�Լ����е�key,val����
		}
		else
		{
			iniparser_unset(_ptDict, cTmp);		//key�ǿ�,ɾ����Ӧkey��val
		}
	}
	
	else if(_iID == CFG_REPLACE)
	{
		if(NULL != cTmp)
		{
			iRet = iniparser_setstr(_ptDict, cTmp, cTmpVal);
			if(iRet)
			{
				pthread_mutex_unlock(&g_stMutexLock);
				return -1;
			}
		}
	}
	
	pthread_mutex_unlock(&g_stMutexLock);

	return 0;
}

/**************************************************
*����: 	 	���������ļ���Ϣ.
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 ׷��ͨ���ŵ�val, _iValNum <= 0 ��׷��
			_iID:			�����ļ�����ID: CFG_ADD - ���, CFG_RM - ɾ��, CFG_REPLACE - �滻
*����ֵ:	0 - �ɹ�, -1 - ʧ��, -2 - ����, -3 - ������
*�޸���ʷ:
	20120227 v1.0 creat by pangtb
**************************************************/
int SetProFloat(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, float _fVal, int _iID)
{
	int iRet = 0;
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmpVal[128] = {0};
	char cTmp[512] = {0};
	
	if(NULL == _ptDict || NULL == _pcSec)
	{
		return -1;
	}

	//section
	if(strlen(_pcSec) > 128 - 3)
	{
		return -1;
	}

	if(_iSecNum > 0)
	{
		sprintf(cTmpSec, "%s%02d", _pcSec, _iSecNum);
	}
	else
	{
		strcpy(cTmpSec, _pcSec);
	}

	//key
	if(NULL != _pcKey)
	{
		if(strlen(_pcKey) > 128 - 3)
		{
			return -1;
		}

		if(_iKeyNum > 0)
		{
			sprintf(cTmpKey, "%s%02d", _pcKey, _iKeyNum);
		}
		else
		{
			strcpy(cTmpKey, _pcKey);
		}
	}

	//val
	sprintf(cTmpVal, "%f", _fVal);

	//combin
	if(strlen(cTmpKey) > 0)
	{
		if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
		{
			return -1;
		}
		else
		{
			sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
		}
	}
	else
	{
		strcpy(cTmp, cTmpSec);
	}

	//process
	pthread_mutex_lock(&g_stMutexLock);
	
	//�жϲ���ID
	if(_iID == CFG_ADD)
	{
		iRet = iniparser_find_entry(_ptDict, cTmpSec);
		if(0 == iRet)	//section������
		{
			iRet = iniparser_add_entry(_ptDict, cTmpSec, NULL, NULL);
			if(iRet)
			{
				pthread_mutex_unlock(&g_stMutexLock);
	
				return -1; 
			}
		}
		else	//section����, ���key��val
		{
 			iRet = iniparser_add_entry(_ptDict,cTmpSec, cTmpKey, cTmpVal);
			if(iRet)
			{
				pthread_mutex_unlock(&g_stMutexLock);
				
				return -1;
			}
		}
	
	}
	
	else if(_iID == CFG_RM)
	{
		if(strlen(cTmpKey) <= 0)
		{
			iniparser_sec_unset(_ptDict, cTmp); //keyΪ��,ɾ��section�Լ����е�key,val����
		}
		else
		{
			iniparser_unset(_ptDict, cTmp);		//key�ǿ�,ɾ����Ӧkey��val
		}
	}
	
	else if(_iID == CFG_REPLACE)
	{
		if(NULL != cTmp)
		{
			iRet = iniparser_setstr(_ptDict, cTmp, cTmpVal);
			if(iRet)
			{
				pthread_mutex_unlock(&g_stMutexLock);
				return -1;
			}
		}
	}
	
	pthread_mutex_unlock(&g_stMutexLock);

	return 0;
}

/**************************************************
*����: 	 	ͨ��Point List���������ļ���Ϣ.
			ע��: ��ɾ��sectionʱ,section��Ӧ������Ҳ�ᱻ��ȫɾ��
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_ptList:		��Ϣ����
		 	_iID:			�����ļ�����ID: CFG_ADD - ���, CFG_RM - ɾ��, CFG_REPLACE - �滻
*����ֵ:	iRet: 0 - �ɹ�, -1 - ʧ��, -2 - ����, -3 - ������
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
int SetProStrList(dictionary *_ptDict, PointList *_ptList, int _iID)
{
	int iRet = 0;
	int i;
	PointList *ptList = NULL;

	if(NULL == _ptDict || NULL == _ptList)
	{
		return -1;
	}

	ptList = _ptList;

	for(i = 0; i < ptList->m_iCount; i++)
	{
		iRet = SetProStr(_ptDict, ptList->m_pcSec, ptList->m_iSecNum,	\
						ptList->m_pcKey[i], ptList->m_iKeyNum[i],	\
						ptList->m_pcVal[i], ptList->m_iKeyNum[i], _iID);
		if(iRet)
		{
			return -1;
		}
	}
	return iRet;
}

/**************************************************
*����: 	 	���_pcKeyΪNULL,��ȡsection���к���;���_pcKey��ΪNULL,���ȡ����key����
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_Default:		Ĭ�Ϸ���ֵ
*����ֵ:	iRet: <=0 - ʧ��, >0 - ���к���,��1��ʼ
*�޸���ʷ:
	20120308 v1.0 creat by pangtb
**************************************************/
int GetIdleNum(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iDefault)
{
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmp[512] = {0};
	int i;

	if(NULL == _ptDict || NULL == _pcSec)
	{
		return _iDefault;
	}

	//find idel section number
	if(NULL == _pcKey)
	{
		for(i = 0; i < MAX_INDEX_NUM; i++)
		{
			if(!FindProEntry(_ptDict, _pcSec, i + 1))	//����
			{
				return i + 1;
			}
		}

	}
	//find idel key number
	else	
	{
		//process section
		if(strlen(_pcSec) > 128 - 3)
		{
			return _iDefault;
		}
		
		if(_iSecNum > 0)
		{
			sprintf(cTmpSec, "%s%02d", _pcSec, _iSecNum);
		}
		else
		{
			strcpy(cTmpSec, _pcSec);
		}

		//key
		if(strlen(_pcKey) > 128 - 3)
		{
			return _iDefault;
		}

		for(i = 0; i < MAX_INDEX_NUM; i++)
		{
			sprintf(cTmpKey, "%s%02d", _pcKey, i + 1);
			
			//combin
			if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
			{
				return _iDefault;
			}
			else
			{
				sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
			}
			
			if(!iniparser_find_entry(_ptDict, cTmp))	//����
			{
				return i + 1;
			}
		}

	}

	return _iDefault;
	
}

/**************************************************
*����: 	 	��ȡval��Ӧkey����
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 ׷��ͨ���ŵ�val, _iValNum <= 0 ��׷��
*����ֵ:	iRet: <=0 - ʧ��, >0 - ���к���,��1��ʼ
*�޸���ʷ:
	20120308 v1.0 creat by pangtb
**************************************************/
int GetKeyNum(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, char *_pcVal, int _iValNum, int _iDefault)
{
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmpVal[128] = {0};
	char cTmp[512] = {0};
	char *pcTmpStr = NULL;
	int i;
	
	if(NULL == _ptDict || NULL == _pcSec)
	{
		return -1;
	}

	//section
	if(strlen(_pcSec) > 128 - 3)
	{
		return -1;
	}

	if(_iSecNum > 0)
	{
		sprintf(cTmpSec, "%s%02d", _pcSec, _iSecNum);
	}
	else
	{
		strcpy(cTmpSec, _pcSec);
	}
	
	//val
	if(strlen(_pcVal) > 128 - 3)
	{
		return -1;
	}

	if(_iValNum > 0)
	{
		sprintf(cTmpVal, "%s%02d", _pcVal, _iValNum);
	}
	else
	{
		strcpy(cTmpVal, _pcVal);
	}

	//key
	if(strlen(_pcKey) > 128 - 3)
	{
		return -1;
	}
	
	//find begin
	for(i = 0; i < MAX_INDEX_NUM; i++)
	{
		//key
		sprintf(cTmpKey, "%s%02d", _pcKey, i + 1);
		
		//combine
		if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
		{
			return _iDefault;
		}
		else
		{
			sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
		}

		pcTmpStr = iniparser_getstr(_ptDict, cTmp);
		if(NULL == pcTmpStr)
		{
			continue;
		}
		if(!strcmp(pcTmpStr, cTmpVal))
		{
			return i + 1;
		}

}

	return _iDefault;
}

/**************************************************
*����: 	 	_pcKeyΪ��,��ȡsec����;_pcKey�ǿ�,��ȡkey,val��Ӧ��key����
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_cKey:			key
		 	_iKeyNum:		key number
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 ׷��ͨ���ŵ�val, _iValNum <= 0 ��׷��
*����ֵ:	iRet: <=0 - ʧ��, >0 - ���к���,��1��ʼ
*�޸���ʷ:
	20120308 v1.0 creat by pangtb
**************************************************/
int GetSecNum(dictionary *_ptDict, char *_pcSec, char *_pcKey, int _iKeyNum, char *_pcVal, int _iValNum, int _iDefault)
{
	char cTmpSec[128] = {0};
	char cTmpKey[128] = {0};
	char cTmpVal[128] = {0};
	char cTmp[512] = {0};
	char *pcTmpStr = NULL;
	int i;
	
	if(NULL == _ptDict || NULL == _pcSec)
	{
		return _iDefault;
	}

	//1-index section num with nothing
	if(NULL == _pcKey)
	{
		for(i = 0; i < MAX_INDEX_NUM; i++)
		{
			if(FindProEntry(_ptDict, _pcSec, i))	//˳�����,section number��������
			{
				return i + 1;
			}
		}
	}
	//2-index key or section num with val
	else
	{
		//key
		if(strlen(_pcKey) > 128 - 3)
		{
			return _iDefault;
		}

		if(_iKeyNum > 0)
		{
			sprintf(cTmpKey, "%s%02d", _pcKey, _iKeyNum);
		}
		else
		{
			strcpy(cTmpKey, _pcKey);
		}

		//val
		if(NULL != _pcVal)
		{
			if(strlen(_pcVal) > 128 - 3)
			{
				return _iDefault;
			}

			if(_iValNum > 0)
			{
				sprintf(cTmpVal, "%s%02d", _pcVal, _iValNum);
			}
			else
			{
				strcpy(cTmpVal, _pcVal);
			}
		}

		//section
		if(strlen(_pcSec) > 128 - 3)
		{
			return _iDefault;
		}
			
		//find begin
		for(i = 0; i < MAX_INDEX_NUM; i++)
		{
			//sec
			sprintf(cTmpSec, "%s%02d", _pcSec, i + 1);
			
			//combine
			if(strlen(cTmpSec) + strlen(cTmpKey) > 512 - 2)
			{
				return _iDefault;
			}
			else
			{
				sprintf(cTmp, "%s:%s", cTmpSec, cTmpKey);
			}
			
			pcTmpStr = iniparser_getstr(_ptDict, cTmp);
			if(NULL == pcTmpStr)
			{
				continue;
			}
			
			if(!strcmp(pcTmpStr, cTmpVal))
			{
				return i + 1;
			}
		}
	}
	return _iDefault;
}

/****************************************************
 *����		:	�ҳ��ַ���(��:PT10S)�е�ʱ�䲢�Է���ʱ��(��λ:S)
 *����		:	_pcstrPT	:	Ҫת�����ַ���
 *����ֵ		: 	����:(ʱ����Ϊ��λ),	����(-1)
 *�޸���ʷ	:	20120314 v1.0 created by ptb
****************************************************/
int PTStrToTime(const char *_pcStrPT)
{
	int iTimeS = 0; //����ʱ�䣬��λ:S
	float fHour = 0.0;
	float fMin  = 0.0;
	float fSec  = 0.0;	
	char *pcTmpPT = NULL;
	char *pcTmpH  = NULL;
	char *pcTmpM  = NULL;
	char *pcTmpS  = NULL;
	
	pcTmpPT = strstr((char*)_pcStrPT, "PT");
	if(pcTmpPT == NULL)
	{
		return -1;
	}

	//hour
	pcTmpH = strchr((char*)_pcStrPT, 'H');
	if(pcTmpH != NULL)
	{
		fHour = atof(pcTmpPT + 2);
	}
	
	//minite
	pcTmpM = strchr((char*)_pcStrPT, 'M');
	if(pcTmpM != NULL)
	{
		if(pcTmpH)
		{
			fMin = atof(pcTmpH + 1);
		}
		else
		{
			fMin =	atof(pcTmpPT + 2);
		}
	}

	//second
	pcTmpS = strchr((char*)_pcStrPT, 'S');
	if(pcTmpS != NULL)
	{
		if(pcTmpM)
		{
			fSec = atof(pcTmpM + 1);
		}
		else
		{
			fSec = atof(pcTmpPT + 2);
		}
	}

	if((pcTmpH == NULL) && (pcTmpM == NULL) && (pcTmpS == NULL))
	{
		return -1;
	}
//	td_printf(0,"=====fHour:%f, fMin:%f, fSec:%f=====\n", fHour, fMin, fSec);
	iTimeS = fHour * 60.0 * 60.0 + fMin * 60.0 + fSec;
		
	return iTimeS;
}
/****************************************************
 *����		:	����Topic��Ϣ�����Ҫ���͵���Ϣ����
 *����		:	_pcTopicName:	Topic����
 				_pcTopicVal	:	Topicֵ
 				_pcTopicType:	Topic����
 				_iTopicNum	:	��ǰ֧�ֵ�Topic��

 *����ֵ		: 	����:������Ϣ�����ַ���,	����:NULL
 *�޸���ʷ	:	20120312 v1.0 created by ptb
****************************************************/
char* SendMsg(const char *_pcTopicName, const char *_pcTopicVal, const char *_pcTopicType, int _iTopicNum )
{
	char cTmpStr[MAX_STRING_LEN] = {0};
	int i = 0;

	for(i = 0; i < _iTopicNum; i++)
	{
		strcpy(cTmpStr, "<tns1:MediaControl wstop:topic=\"true\">");
		strcat(cTmpStr, "<tns1:VideoSourceConfiguration wstop:topic=\"true\">");
		strcat(cTmpStr, "<tt:MessageDescription IsProperty=\"true\">");
		strcat(cTmpStr, "<tt:Source><tt:SimpleItemDescription/>Name=\"VideoSourceConfigurationToken\"  Type=\"tt:ReferenceToken\"></tt:Source>");
		strcat(cTmpStr, "<tt:Data><tt:SimpleItemDescription/>Name=\"Config\" Type=\"tt:VideoSourceConfiguration\"</tt:Data>");
		strcat(cTmpStr, "</tt:MessageDescription>");
		strcat(cTmpStr, "</tns1:VideoSourceConfiguration>");
		strcat(cTmpStr, "</tns1:MediaControl>");
		
	}
	return NULL; 
}
#ifdef TEST_QIANG
int main(int argc, char *argv[])
{
	int iRet = 0;
	char *pcProfile = "./config_test.ini";
	char *pcTest = NULL;
	char cSec[128] = {0};
	char cKey[128] = {0};
	char cVal[128] = {0};
	int iSec = 0;
	int iKey = 0;
	PointList stList = {0};

	dictionary *ptDict = iniparser_load(pcProfile);
	if(NULL == ptDict)
	{
		printf("iniparser_load error!\n");

		return -1;
	}
#if 0
	//GetProStr test
	printf("========== GetProStr test ==========\n");

	//input user info
	printf("please input section:\n");
	scanf("%s", cSec);
	
	printf("please input section number:\n");
	scanf("%d", &iSec);
	
	printf("please input key:\n");
	scanf("%s", cKey);
	
	printf("please input key number:\n");
	scanf("%d", &iKey);
	
	pcTest = GetProStr(ptDict, cSec, iSec, cKey, iKey, NULL);
	if(NULL == pcTest)
	{
		printf("GetProStr error!\n");

		iRet = -1;
		goto EXIT;
	}

	printf("haha, result is %s\n\n", pcTest);
#endif
#if 0
	//SetProStr
	printf("========== add section test ==========\n");

	printf("please input the section name you want to add: \n");
	scanf("%s", cSec);
	stList.m_pcSec = cSec;
	stList.m_iCount++;

	iRet = SetProStrList(ptDict, &stList, CFG_ADD);
	if(iRet)
	{
		printf("SetProStr error! \n");

		goto EXIT;
	}
	
	SaveProfile(ptDict, pcProfile);	
	printf("========== add section test ok! ==========\n\n");

	//add key and value to section you slected.
	printf("========== add key test ==========\n");
	memset(&stList, 0, sizeof(PointList));

	printf("please input the section you want to add: \n");
	scanf("%s", cSec);
	stList.m_pcSec = cSec;

	printf("please input the key you want to add: \n");
	scanf("%s", cKey);
	stList.m_pcKey[stList.m_iCount] = cKey;

	printf("please input the value you want to add: \n");
	scanf("%s", cVal);
	stList.m_pcVal[stList.m_iCount] = cVal;
	stList.m_iCount++;

	iRet = SetProStrList(ptDict, &stList, CFG_ADD);
	if(iRet)
	{
		printf("SetProStr CFG_ADD error! \n");

		goto EXIT;
	}

	SaveProfile(ptDict, pcProfile);
	printf("========== add key test ok! ==========\n\n");
#endif

	//test replace function
	printf("========== replace key test ==========\n");
	memset(&stList, 0, sizeof(PointList));

	printf("please input the section you want to slect: \n");
	scanf("%s", cSec);
	stList.m_pcSec = cSec;
#if 0
	printf("please input the key you want to slect: \n");
	scanf("%s", cKey);
	stList.m_pcKey[stList.m_iCount] = cKey;

	printf("please input the val you want to relace: \n");
	scanf("%s", cVal);
	stList.m_pcVal[stList.m_iCount] = cVal;
#else
	printf("please input the val you want to relace: \n");
	scanf("%s", cVal);
	stList.m_pcVal[stList.m_iCount] = cVal;

#endif

	stList.m_iCount++;

	iRet = SetProStrList(ptDict, &stList, CFG_REPLACE);
	if(iRet)
	{
		printf("SetProStr CFG_REPLACE key error! \n");

		goto EXIT;
	}
	printf("SetProStr CFG_REPLACE key ok!\n");

	SaveProfile(ptDict, pcProfile);
	printf("========== replace key test ok! ==========\n\n");

	//test delete function

	//delete key
	printf("========== delete key test ==========\n");
	
	memset(&stList, 0, sizeof(PointList));
	printf("please input the section you want to slect: \n");
	scanf("%s", cSec);
	stList.m_pcSec = cSec;
	
	printf("please input the key you want to delete: \n");
	scanf("%s", cKey);
	stList.m_pcKey[stList.m_iCount] = cKey;
	stList.m_iCount++;
	
	iRet = SetProStrList(ptDict, &stList, CFG_RM);
	if(iRet)
	{
		printf("SetProStr CFG_RM key error! \n");

		goto EXIT;
	}
	printf("SetProStr CFG_RM key ok!\n");

	SaveProfile(ptDict, pcProfile);
	printf("========== replace key test ok! ==========\n\n");

	//delete section
	printf("========== delete section test ==========\n");
	
	memset(&stList, 0, sizeof(PointList));
	printf("please input the section you want to delete: \n");
	scanf("%s", cSec);
	stList.m_pcSec = cSec;
	stList.m_iCount++;

	iRet = SetProStrList(ptDict, &stList, CFG_RM);
	if(iRet)
	{
		printf("SetProStr CFG_RM error! \n");

		goto EXIT;
	}
	printf("SetProStr CFG_RM ok!\n");

	
	SaveProfile(ptDict, pcProfile);
	printf("========== delete section test ok! ==========\n\n");


EXIT:
	iniparser_freedict(ptDict);
	ptDict = NULL;

	return iRet;
}

#endif

