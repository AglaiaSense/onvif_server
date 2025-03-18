/*
 *	file: common.c
 *	description: common funcs
 *	history: dongliqiang created at 20120207
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include <sys/sysinfo.h>
#include <limits.h> //added for UINT_MAX

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


VIDEOSIZE_INFO TVQCIF   = {{176,176},{144,112}};            // 0
VIDEOSIZE_INFO TVHCIF   = {{352,352},{144,128}};            // 1
VIDEOSIZE_INFO TVFCIF   = {{352,352},{288,240}};            // 2
VIDEOSIZE_INFO TVHD1    = {{704,704},{288,240}};            // 3
VIDEOSIZE_INFO TVFD1    = {{704,704},{576,480}};            // 4
VIDEOSIZE_INFO TVMD1    = {{720,720},{288,240}};            // 5
VIDEOSIZE_INFO TVQVGA   = {{320,320},{240,240}};            // 6
VIDEOSIZE_INFO TVFVGA   = {{640,640},{480,480}};            // 7
VIDEOSIZE_INFO TVHVGA   = {{640,640},{240,240}};            // 8
VIDEOSIZE_INFO TVF720P  = {{1280,1280},{720,720}};          // 9
VIDEOSIZE_INFO TVF960P  = {{1280,1280},{960,960}};          // 10
VIDEOSIZE_INFO TVF1200  = {{1600,1600},{1200,1200}};        // 11
VIDEOSIZE_INFO TVF1080P = {{1920,1920},{1080,1080}};        // 12
VIDEOSIZE_INFO TVFQXGA  = {{2048,2048},{1536,1536}};        // 13

VIDEOSIZE_INFO *PTVideoSizeInfo[]={&TVQCIF, &TVHCIF, &TVFCIF, &TVHD1, &TVFD1, &TVMD1, &TVQVGA, &TVFVGA, &TVHVGA, &TVF720P, &TVF960P, &TVF1200, &TVF1080P, &TVFQXGA, NULL};

/**************************************************
*¹¦ÄÜ:		»ñÈ¡ÊÓÆµ¿í¶ÈºÍ¸ß¶È
*²ÎÊý:		_iPicSize	:	Í¼Ïñ´óÐ¡Ë÷ÒýÖµ
            _iNorm		:	ÊÓÆµÖÆÊ½
			_u16Width	:	ÊÓÆµ¿í¶È
			_u16Height	:	ÊÓÆµ¸ß¶È
*·µ»ØÖµ:	0	:	³É¹¦
            0!	:	Ê§°Ü
*ÐÞ¸ÄÀúÊ·:20130311 v2.3.0 modify by ptb
**************************************************/
int GetVideoWidthHeight(int _iPicSize, int _iNorm, unsigned short* _u16Width, unsigned short* _u16Height)
{
	int iNorm = 0;

	//ÊÓÆµ´óÐ¡ºÏ·¨ÐÔÅÐ¶Ï
	if(_iPicSize < 0 || NULL == _u16Height || NULL == _u16Width)
	{
		return -1;
	}
	
	iNorm = (_iNorm == 1) ? 1:0;

	//ptb add at 120810
	//Éè±¸Ö§³Ö14¸ö·Ö±æÂÊ
	if(_iPicSize > 14)
	{
		_iPicSize = 0;
	}
	//end
	
	*_u16Width = PTVideoSizeInfo[_iPicSize]->m_u16VideoWidth[iNorm];
	*_u16Height = PTVideoSizeInfo[_iPicSize]->m_u16VideoHeight[iNorm];
	
	return 0;
}
#ifdef TD_DEBUG

#define gettid()   syscall(__NR_gettid)
 
int td_printf(unsigned int handle, char* pszfmt, ...)
{
	 struct tm tm_now;
	 va_list struAp;
	 time_t now;
	 char format[512];
	 int ret = 0;
	 
	 if(pszfmt == NULL) 
	 {
		 return -1;
	 }

	 if((strlen(pszfmt)+3)>512)
	 {
		 return -2;
	 }
	 memset(format,0,sizeof(format)); 
	 strcpy(format,pszfmt);
	 if(format[strlen(pszfmt)-1]=='\n')
	 {
		 format[strlen(pszfmt)] = format[strlen(pszfmt)-2]=='\r' ?'\0':'\r';
	 }
	 else if(format[strlen(pszfmt)-1]=='\r')
	 {
		 format[strlen(pszfmt)] = format[strlen(pszfmt)-2]=='\n' ?'\0':'\n';
	 }
	 else
	 {
		 format[strlen(pszfmt)] ='\r';
		 format[strlen(pszfmt)+1] ='\n';
	 }
	 
	 now=time(&now);
	 localtime_r(&now, &tm_now);
	 if(0 == handle)
	 {	 
		 printf("[1;33m""[%04d/%02d/%02d %02d:%02d:%02d %ld]""[0;39m",
			 tm_now.tm_year+1900,
			 tm_now.tm_mon+1,
			 tm_now.tm_mday,
			 (tm_now.tm_hour)%24,
			 tm_now.tm_min,
			 tm_now.tm_sec,
			 gettid()/*getpid()*/);

		 va_start(struAp, pszfmt);
		 ret = vprintf(format, struAp);
		 va_end(struAp);
	 }
	 else if(0xFF0000 == handle)
	 {
		 printf("[1;31m""[%04d/%02d/%02d %02d:%02d:%02d]""[0;39m",
		 tm_now.tm_year+1900,
		 tm_now.tm_mon+1,
		 tm_now.tm_mday,
		 (tm_now.tm_hour)%24,
		 tm_now.tm_min,
		 tm_now.tm_sec);

		 va_start(struAp, pszfmt);
		 ret = vprintf(format, struAp);
		 va_end(struAp);
	 }
	 else if(SYS_INFO_PRINTF == handle)
	 {
	 	 #if 0	//dlq modified at 20120224
		 printf("[1;36m");
		 va_start(struAp, pszfmt);
		 ret = vprintf(format, struAp);
		 va_end(struAp);
		 printf("[0;39m\r\n");
		 #else
		 printf("[1;36m""[%04d/%02d/%02d %02d:%02d:%02d %ld]""[0;39m",
			 tm_now.tm_year+1900,
			 tm_now.tm_mon+1,
			 tm_now.tm_mday,
			 (tm_now.tm_hour)%24,
			 tm_now.tm_min,
			 tm_now.tm_sec,
			 gettid()/*getpid()*/);

		 va_start(struAp, pszfmt);
		 ret = vprintf(format, struAp);
		 va_end(struAp);
		 #endif
	 }
	 else
	 {
		 FILE *file;  
		 file = fopen("printf.txt","a+");
		 fprintf(file,"[%04d/%02d/%02d %02d:%02d:%02d]",
			 tm_now.tm_year+1900,
			 tm_now.tm_mon+1,
			 tm_now.tm_mday,
			 (tm_now.tm_hour)%24,
			 tm_now.tm_min,
			 tm_now.tm_sec);
	   
		 va_start(struAp, pszfmt);
		 ret = vfprintf(file,format, struAp);
		 va_end(struAp);
		 fclose(file);
	 }
	 return ret;
}

#endif

//---------------------------------------------------------------------------
static int OnCharLeft(const char* c1,const char* c2,char* c3)
{
	int i=0, j=0;

	char* cPos=NULL;
	cPos = strstr((char *)c1, (char *)c2);
	if(cPos != NULL)
	{		
		i=cPos-c1;
		for(j=0;j<i;j++)
			c3[j]=c1[j];
		
		c3[i]=0;
	}
	//dlq modified at 20120305
	else if(strlen(c1) > 0)
	{
		strcpy(c3, c1);
	}
	//end
	
	return 0;
}
//---------------------------------------------------------------------------
static int OnCharRight(const char* c1,const char* c2,char* c3)
{
	int i;
	char* cPos=NULL;
	i=strlen(c2);
	cPos = strstr((char *)c1,(char *)c2);
	if(cPos!=NULL)
	{
		memcpy(c3,cPos+i,strlen(cPos)-i);
		c3[strlen(cPos)-i]=0;
	}
	else
	{
		c3[0]=0;
	}

	return 0;
}

/**************************************************
*¹¦ÄÜ:		Split a string like:"xxxx\txxxx\txxxx\t" to cDest array[]
*²ÎÊý:		cSource:		Ô´×Ö·û´®
*			cSplitChar:		·Ö½â·û
*			cDest:			Ä¿±êÊý×é
*·µ»ØÖµ:	Êý×é´óÐ¡
*ÐÞ¸ÄÀúÊ·:
	20120224 v1.0 creat by xxx
**************************************************/
int SplitStringToArray(char *cSource,char* cSplitChar,char** cDest)
{
    int i=0;

    while(strlen(cSource)>0)
    {
        OnCharLeft(cSource,cSplitChar,cDest[i]);
        OnCharRight(cSource,cSplitChar,cSource);
        ++i;
    }

    return i;
}

/**************************************************
*¹¦ÄÜ: 	 Ñ°ÕÒ×Ö·û´®µÄÊý×ÖÆðÊ¼Î»ÖÃ
*²ÎÊý: 	 cSource:		 Ô´×Ö·û´®
*			 cSplitChar:	 ·Ö½â·û
*			 cDest: 		 Ä¿±êÊý×é
*·µ»ØÖµ:	 Êý×é´óÐ¡
*ÐÞ¸ÄÀúÊ·:
 20120224 v1.0 creat by xxx
**************************************************/
char *FindNumPos(char *_cSource)
{
	char *pSrcStr = _cSource;
	if(NULL == pSrcStr)
	{
		return NULL;
	}

	while(pSrcStr)
	{
		if(*pSrcStr >= '0' && *pSrcStr <= '9')
		{
			return pSrcStr;
		}

		pSrcStr++;
	}

	return NULL;
}

/**************************************************
*¹¦ÄÜ: 	 ×ª»»floatÀàÐÍÖµ
*²ÎÊý: 	 _pfSrc    :	 Ô´Öµ
*		 _tSrcRange:	 Ô´·¶Î§
*		 _tDstRange: 	 Ä¿±ê·¶Î§
*·µ»ØÖµ:	 ×ª»»ºó´óÐ¡
*ÐÞ¸ÄÀúÊ·:20120224 v1.0 crby pangtb dlq
**************************************************/
float ExchangeValue(float _pfSrc, TFloatRange _tSrcRange, TFloatRange _tDstRange)
{
	float fRet = 0.0;
	float fSrcRange = 0.0;
	float fDstRange = 0.0;

	fSrcRange = _tSrcRange.Max - _tSrcRange.Min;
	fDstRange = _tDstRange.Max - _tDstRange.Min;
	if(fSrcRange > 0)
	{
		fRet = fDstRange / fSrcRange * (_pfSrc - _tSrcRange.Min) + _tDstRange.Min;
	}
	else if(fSrcRange < 0)
	{
		fRet = _tDstRange.Max -  fDstRange / fSrcRange * (_tSrcRange.Max - _pfSrc);
	}
	else
	{
		fRet = 0.0;
	}

	//td_printf(0, "%s %d %s fRet = %f\n", __FILE__, __LINE__, __FUNCTION__, fRet);
	
	return fRet;
}

char * base64_encode(unsigned char* pSrc, int iLen, char * pBuf, int iBufLen)
{
	static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char *ret, *dst;
	unsigned i_bits = 0;
	int i_shift = 0;
	int bytes_remaining = iLen;
	if (iLen >= UINT_MAX / 4 || iBufLen < iLen * 4 / 3 + 12)
  	{
		return NULL;
  	}
	ret = dst = pBuf;
	if (iLen) 
  	{  
  		// edge case
		while (bytes_remaining) 
		{
    		i_bits = (i_bits << 8) + *pSrc++;
			bytes_remaining--;
			i_shift += 8;
			do 
			{
				*dst++ = b64[(i_bits << 6 >> i_shift) & 0x3f];
				i_shift -= 6;
			} while (i_shift > 6 || (bytes_remaining == 0 && i_shift > 0));
		}
		while ((dst - ret) & 3)
		{
			*dst++ = '=';
		}
	}
	*dst = '\0';
	return ret;
}

//dlq modified at 20130802 for settime
int get_uptime(int* _msec)
{
	struct sysinfo info;
	sysinfo(&info);
	return (int)info.uptime;
}

/**************************************************
*¹¦ÄÜ:		É¾³ýÖ¸¶¨ÎÄ¼þ
*²ÎÊý:		_pcFilePath:		ÎÄ¼þÂ·¾¶
*·µ»ØÖµ:	0: ³É¹¦; -1: Ê§°Ü
*ÐÞ¸ÄÀúÊ·:20131011 v2.3.1 creat by huweixu
**************************************************/
int RemoveFile(char *_pcFilePath)
{
	int iSta = 0;
	
	iSta = access(_pcFilePath, F_OK);
	if(0 == iSta)	
	{
		td_printf(0, "The file exists.");

		remove(_pcFilePath);

		iSta = access(_pcFilePath,F_OK);
		if(0 == iSta)	
		{
			td_printf(0, "Delete onvifv10.bin fail.");
			return	TD_ERROR;
		}
		else	
		{
			td_printf(0, "Delete onvifv10.bin success.");
			return TD_OK;
		}
	}
	else	
	{
		td_printf(0, "The file onvifv10.bin doesn't exist.");
		return TD_OK;
	}
}

int Strcpy(char *_pcDest, const char *_pcSrc, int _iMaxLen)
{
	int iMaxLen = _iMaxLen;
	int iLen = 0;
	
	if(pointer_valid_check(_pcSrc) || pointer_valid_check(_pcDest))
	{
		return TD_ERROR;
	}

	//Ç§Íò×¢ÒâmemsetÊÇ·ñÔ½½ç£¬
	memset(_pcDest, 0, iMaxLen);

	iLen = strlen(_pcSrc);
	if(iLen <= iMaxLen - 1)
	{
		strncpy(_pcDest, _pcSrc, iLen);
	}
	else
	{
		strncpy(_pcDest, _pcSrc, iMaxLen - 1);
	}

	return TD_OK;
}

/**************************************************
*¹¦ÄÜ:		¼ì²âÉèÖÃµÄ¶Ë¿Ú¿ÉÓÃ
*²ÎÊý:		_ptSoap:		tcp soap
*·µ»ØÖµ:	0: ³É¹¦, !0: Ê§°Ü
*ÐÞ¸ÄÀúÊ·:20131114 v2.3.1 create by ptb
**************************************************/
int CheckPort(int _iPort)
{
	struct sockaddr_in stSockAddr;
	int iPort = _iPort;
	int iSocket = 0;
	int iRet = 0;

	if(iPort <= 0 || iPort > 65535)
	{
		return PROT_ILLEGAL;
	}
	
	//³¢ÊÔÓÃ»§ÉèÖÃµÄ¶Ë¿ÚºÅÊÇ·ñ¿ÉÒÔ°ó¶¨
	iSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(iSocket < 0)
	{
		perror("CheckPort: socket error");
		return TD_ERROR;
	}
	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
		
	stSockAddr.sin_addr.s_addr = INADDR_ANY;
	stSockAddr.sin_port   = htons(iPort);
	stSockAddr.sin_family = AF_INET;

	iRet = bind(iSocket, (struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in));
	if(0 != iRet)
	{
		perror("CheckPort: bind error");
		iRet = PROT_USED;
	}
	close(iSocket);

	return iRet;
}

/**************************************************
*¹¦ÄÜ:		¼ì²âÊÇ·ñ´æÔÚÊ±ÇøÎÄ¼þ
*²ÎÊý:		_ptSoap:		tcp soap
*·µ»ØÖµ:	0: ´æÔÚ
				!0: ²»´æÔÚ
*ÐÞ¸ÄÀúÊ·:20131114 v2.3.1 create by ptb
**************************************************/
int CheckTimeZoneFile()
{
	int iRet = 0;
	
	iRet = access("/etc/TZ", F_OK);

	return iRet;
}


