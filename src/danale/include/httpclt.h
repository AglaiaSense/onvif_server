#ifndef __httpclt_h__
#define __httpclt_h__

#include <openssl/ssl.h>
#include "platform_adpt.h"

#ifdef __cplusplus
extern "C" {
#endif
//-----------------------------------------------
//https.h
typedef struct _tagTcpConn {
	int sock;
	SSL *ssl;
} TCPCONN;

typedef struct _tagHttpConn {
	TCPCONN tconn;
	char host[64];
	int port;
} HTTPCONN;

#define CONN_E_TIMEOUT	-2
#define CONN_E_SOCKET	-3
#define CONN_E_PEER_CLOSED -4
#define CONN_E_RESOLVE	-5
#define CONN_E_CONNECT	-6
#define CONN_E_SSL	-7
#define CONN_E_GENERIC	-8

int tcp_connect(const char *host, int port, int timeout);

int ConnWaitRead(TCPCONN *conn, void *buff, int size, int wait_ms);
int ConnWrite(TCPCONN *conn, char *buff, int size);

void HttpConnInit(HTTPCONN *conn);
int HttpConnOpen(const char *url, HTTPCONN *conn);
void HttpConnClose(HTTPCONN *conn);

typedef enum { HTTP_METHOD_GET, HTTP_METHOD_PUT, HTTP_METHOD_POST } HTTPMETHOD;
int HttpSimpleSend(HTTPCONN *conn, HTTPMETHOD method, const char *user, const char *pswd, 
		const char *url, const char *param, const char *extra_headers);

#define RDSTAGE_STATUS		0
#define RDSTAGE_CONTENT_LENGTH	1
#define RDSTAGE_DATA		2
#define RDSTAGE_FINISHED	3
typedef int (* HTTPRESPDATACB)(int stage, char *pData, int len, void *pUserData);

#define EHTTP_HEADER	-101
#define EHTTP_URL	-102
int HttpRecvResponseWithCB(TCPCONN *conn, HTTPRESPDATACB cb, void *pUser, int timeout/*ms*/);

typedef struct _tagHttpResp {
	int status;
	char *content;
	int content_length;
} HTTPRESP;
int HttpRecvResponse(HTTPCONN *conn, HTTPRESP *resp, int timeout);

#define EHTTP_CREATE_FILE	-103
int HttpRecvFile(HTTPCONN *conn, const char *local, int timeout, HTTPRESP *resp);

int http_get(HTTPCONN *conn, const char *url, const char *extra_headers, int timeout, HTTPRESP *resp);
int http_get_file(HTTPCONN *conn, const char *url, const char *extra_headers, int timeout, const char *fname, HTTPRESP *resp);
int http_post(HTTPCONN *conn, const char *url, const char *param, const char *extra_headers, int timeout, HTTPRESP *resp);

#ifdef __cplusplus
}
#endif

#endif

