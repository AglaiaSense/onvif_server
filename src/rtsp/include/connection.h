/*
 *	file: connection.h
 *	description: connection functions
 *	history: dongliqiang created at 20120815
 */

#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <netinet/in.h>
#include <sys/uio.h>
#include "common.h"
#include "mutex_lock.h"
#include "libdatacenter.h"
#include "ringbuf.h"
#include "dlist.h"

#define LENGTH_IPV4			16

#define NET_TCP				0
#define	NET_UDP				1

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  	(-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR  		(-1)
#endif

#ifndef TD_DEBUG
#define CON_DBG_LOCK1(_pAddr, _pcString) 
#define CON_DBG_LOCK2(_pAddr, _pcString) 
#define CON_DBG_LOCK3(_pAddr, _pcString) 
#define CON_DBG_LOCK4(_pAddr, _pcString) 
#else
	//by qiaohaijun @2013.4.18  for debug the lock
	#ifdef CONECT_LOCK
		#define CON_DBG_LOCK1(_pAddr, _pcString) td_printf(0, "%s %s %d %p Wait Lock %s", __FILE__, __FUNCTION__, __LINE__, _pAddr, _pcString)
		#define CON_DBG_LOCK2(_pAddr, _pcString) td_printf(0, "%s %s %d %p Got Lock %s", __FILE__, __FUNCTION__, __LINE__, _pAddr, _pcString)
		#define CON_DBG_LOCK3(_pAddr, _pcString) td_printf(0, "%s %s %d %p Wait UN Lock %s", __FILE__, __FUNCTION__, __LINE__, _pAddr, _pcString)
		#define CON_DBG_LOCK4(_pAddr, _pcString) td_printf(0, "%s %s %d %p Got UN Lock %s", __FILE__, __FUNCTION__, __LINE__, _pAddr, _pcString)
	#else
		#define CON_DBG_LOCK1(_pAddr, _pcString)
		#define CON_DBG_LOCK2(_pAddr, _pcString)
		#define CON_DBG_LOCK3(_pAddr, _pcString)	
		#define CON_DBG_LOCK4(_pAddr, _pcString)
	#endif
#endif

#define SOCKET_ERROR_SUCCESS 0          //没有错误
#define SOCKET_ERROR_RCV 	(-1)
#define SOCKET_ERROR_SND  	(-2)

#define SEND_BUF_TCP 		(128*1024)			//64k -> 128k changguoxing modify 20130417
#define RECV_BUF_TCP 		(128*1024)			//128k
#define SEND_BUF_UDP 		(128*1024)			//64k -> 128K changguoxing modify 20130417
#define RECV_BUF_UDP 		(128*1024)			//128k
#define MAX_SEND_FIFO_NUM	(1024)				//128个
#define MAX_RECV_FIFO_NUM	(128)				//128个

#define SC_CLOSEING     	(-1)
#define SC_NONACTIVE    	(0)
#define SC_CONNECTING   	(1)
#define SC_CONNECTED    	(2)
#define SC_LISTENING		(3)


#define MAX_CONNECTION		(128)

typedef enum
{
	INITED = 0,
	UNINITED
}INITION;

typedef enum
{
	CONNECT_TYPE_SERVER = 0,
	CONNECT_TYPE_CLIENT
}CONNECT_TYPE;

typedef struct iovec TIovec;

typedef struct Connection  TConnection;

struct Connection
{
	int 			m_iSocket;
	int				m_iType;				//0 - server, 1 - client
	int				m_iNetType;				//0 - tcp, 1 - udp
	char 			m_pcLAddr[LENGTH_IPV4];	//local ip
	char 			m_pcRAddr[LENGTH_IPV4];	//remote ip
	int 			m_iLPort;				//local port
	int 			m_iRPort;				//remote port
	struct sockaddr_in m_tLocalAddr;
	struct sockaddr_in m_tDestAddr;
	int  			m_iStat;
	int 			m_iError;
	int  			m_iInited;

	TDataCenter *	m_ptRecvDc;				//接收缓存池
	TDataCenter *	m_ptSendDc;				//发送缓存池
	TDataFIFO *		m_ptRecvFifo;			//接收fifo
	TDataFIFO *		m_ptSendFifo;			//发送fifo
	TDataPackage *	m_ptRecvPkg;			//接收剩余数据
	int				m_iRecvPos;				//接收剩余数据位置
	TDataPackage *	m_ptSendPkg;			//发送剩余数据
	int				m_iSendPos;				//发送剩余数据位置
	TRingBuf *		m_ptTcpRingBuf;			//tcp滑块解析buf
	
	void *			m_pUserData;
	TMutexLock 		m_tLock; 
	
	int (*m_RecvEpoll)(TConnection* _ptConnect);
	int (*m_SendEpoll)(TConnection *_ptConnect);
	int (*m_AcceptEpoll)(TConnection *_ptConnect);
	int (*m_RecvBuffer)(TConnection *_ptConnect, unsigned char *_pcBuffer, int _iLen);
	int (*m_SendBuffer)(TConnection *_ptConnect,const unsigned char* _pcData, int _iLen);
	int (*m_ReadV)(TConnection *_ptConnect,  TIovec *_ptIovec, int _iIovCnt);
	int (*m_WriteV)(TConnection *_ptConnect,const TIovec *_ptIovec, int _iIovCnt);
	int (*m_ServerStart)(TConnection * _ptConnect);
	int (*m_ServerStop)(TConnection *_ptConnect);
	int (*m_Connect)(TConnection * _ptConnect, char *_pcSIP, int _iRPort);
	int (*m_Close)(TConnection *_ptConnect);
	int (*m_AcceptCallBack)(int _iSocket);
	int (*m_DisConnectCallBack)(TConnection * _ptConnect);
};

int inner_Close(TConnection *_ptConnect);

int inner_Error(TConnection *_ptConnect,int _iErrNum);

int inner_ConnectNoBlock(TConnection *_ptConnect);

int InitConnection(TConnection *_ptConnect, int _iType, int _iNetType, int _iLPort, void * _pData);

int UnInitConnection(TConnection *_ptConnect);

TConnection * CreatConnection(int _iSocket, int _iType, int _iNetType, void *_pUserData, void *DisConnectCallBack);

int DestroyConnection(TConnection *_ptConnect);

int TcpRecvEpoll(TConnection *_ptConnect);

int TcpSendEpoll(TConnection *_ptConnect);

int UdpRecvEpoll(TConnection *_ptConnect);

int UdpSendEpoll(TConnection *_ptConnect);

int TcpRecvBuffer(TConnection *_ptConnect, unsigned char *_pcBuffer, int _iLen);

int TcpSendBuffer( TConnection *_ptConnect, const unsigned char* _pcData, int _iLen);

int TcpAcceptEpoll( TConnection *_ptConnect);

int UdpRecvBuffer(TConnection *_ptConnect, unsigned char *_pcBuffer, int _iLen);

int UdpSendBuffer(TConnection *_ptConnect, const unsigned char* _pcData, int _iLen);

int UdpAcceptEpoll( TConnection *_ptConnect);

int ReadVBuffer(TConnection *_ptConnect, TIovec *_ptIovec, int _iIovCnt);

int WriteVBuffer(TConnection *_ptConnect,const TIovec *_ptIovec, int _iIovCnt);

int TcpConnect(TConnection * _ptConnect, char *_pcIP, int _iPort);

int UdpConnect(TConnection * _ptConnect, char *_pcIP, int _iPort);

int TcpServerStart(TConnection * _ptConnet);

int TcpServerStop(TConnection * _ptConnect);

//int CloseSocket(TConnection * _ptConnect);

int BindCallBack(TConnection * _ptConnect, void *AcceptCallBack, void *DisConnectCallBack, void *_pUserData);

int UnBindCallBack(TConnection * _ptConnect);

#endif
