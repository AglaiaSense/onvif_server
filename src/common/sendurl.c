//*****************************************
//   文件名     : sendurl.c
//   文件描述: 发送URL地址
//	修改记录: modify at 20130527 by ptb                          
//*****************************************
#include "sendurl.h"
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include "onvif_sdk.h"

extern OnvifCtrl g_stOnvifCtrl;

void GetHttpDate(char *_hdr, char *_pBuf)
{
	struct tm tSaveTime;
	
	char ch[64] = {0};
	time_t now;
	
	now = time(NULL);
	gmtime_r(&now, &tSaveTime);
	
	strftime(ch, 64, "%a, %d %b %Y %H:%M:%S GMT", &tSaveTime);
	sprintf(_pBuf, "%s%s %s" SCRLF, _pBuf, _hdr, ch);
}

int SendUrl(int _isockfd)
{
	char send_buf[MAX_STRING_LEN_MEM] = {0};
	char html_buf[MAX_STRING_LEN_MEM] = {0};
	char chHtml[] = "<HTML>\r\n\
	<head>\r\n\
	<meta http-equiv=\"refresh\" content=\"0;url=http://%s:%d/index.html\">\r\n\
	</head>\r\n\
	</HTML>";

	sprintf(send_buf, "HTTP/1.0 200 OK" SCRLF);
	sprintf(send_buf, "%sContent-type: text/html" SCRLF, send_buf);

	GetHttpDate("Date:", send_buf);
	sprintf(send_buf, "%sConnection: close" SCRLF, send_buf);
	sprintf(send_buf, "%sAccept-Ranges: bytes" SCRLF, send_buf);

	GetHttpDate("Last-Modified", send_buf);
	sprintf(html_buf, chHtml, g_stOnvifCtrl.m_stNvsInfo.m_cLocalIP, g_stOnvifCtrl.m_stNvsInfo.m_iLWebPort);

	sprintf(send_buf, "%sContent-length:%d" SCRLF, send_buf, strlen(html_buf));
	sprintf(send_buf, "%s" SCRLF, send_buf);
	sprintf(send_buf, "%s%s" SCRLF, send_buf, html_buf);
	send(_isockfd,send_buf, strlen(send_buf), 0);

	return 0;
}

