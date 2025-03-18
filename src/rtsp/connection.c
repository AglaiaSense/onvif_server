/*
 *	file: onvif_connection.c
 *	description: onvif connection functions
 *	history: dongliqiang created at 20120815
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <netinet/tcp.h>

#include "connection.h"
#include "epoll.h"
#include "common.h"
//#include "onvif_task_process.h"

int CloseSocket(TConnection * _ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection * ptConnect = _ptConnect;
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");

	inner_Close(ptConnect);
	
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);	
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	return TD_OK;
}

int inner_ConnectNoBlock(TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iSocket;
	TConnection *ptConnect = _ptConnect;
	if(ptConnect == NULL)
	{
		return TD_ERROR;
	}	
	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		return TD_ERROR;		
	}
	
	int opts = fcntl(iSocket , F_GETFL);
	if(-1 == opts)
	{
		return TD_ERROR;
	}
	opts = opts | O_NONBLOCK;
	if(fcntl(iSocket , F_SETFL , opts) < 0)
	{
		td_printf(0,"%s\n" , "fcntl F_SETFL is faild");
		return TD_ERROR;
	}

	return TD_OK;
}

int inner_Close(TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;

	if(ptConnect == NULL)
	{
		return TD_ERROR;
	}	
	
	//changguoxing,socket和stat都要放在disconnect里面处理。
	if(ptConnect->m_DisConnectCallBack)
	{
		ptConnect->m_DisConnectCallBack(ptConnect);//DisConnectCallBack
	}
	else
	{
		if(ptConnect->m_iStat == SC_CONNECTED)
		{
			EpollDelConnection(ptConnect);
		}
	}
	
	
	return TD_OK;
}

int inner_Error(TConnection *_ptConnect,int _iErrNum)
{
	TConnection *ptConnect = _ptConnect;
	if(ptConnect == NULL)
	{
		return TD_ERROR;
	}
	
	ptConnect->m_iError = _iErrNum;
	return inner_Close(ptConnect);
}

int inner_TcpKeepalive(int _iSock)
{
	int keepalive = 1; 
	int keepidle = 30;
	int keepinterval = 3;
	int keepcount = 3;
	
	setsockopt(_iSock, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
	setsockopt(_iSock, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
	setsockopt(_iSock, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
	setsockopt(_iSock, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));

    return 0;
}

int InitConnection(TConnection *_ptConnect, int _iType, int _iNetType, int _iLPort, void * _pData)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}

	MutexLockInit(&ptConnect->m_tLock);

	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");

	memset(ptConnect->m_pcRAddr, 0, LENGTH_IPV4);
	ptConnect->m_iInited   = INITED;
	ptConnect->m_iType     = _iType;
	ptConnect->m_iNetType  = _iNetType;
	ptConnect->m_iSocket   = INVALID_SOCKET;
	ptConnect->m_iError    = SOCKET_ERROR_SUCCESS;
	ptConnect->m_iRPort    = 0;
	ptConnect->m_iLPort    = _iLPort;
	ptConnect->m_iStat     = SC_NONACTIVE;
	ptConnect->m_pUserData = _pData;
	
	ptConnect->m_ptRecvPkg = NULL;
	ptConnect->m_iRecvPos  = 0;
	ptConnect->m_ptSendPkg = NULL;
	ptConnect->m_iSendPos  = 0;
	
	ptConnect->m_ptRecvDc   = datacenter_Create();
	ptConnect->m_ptSendDc   = datacenter_Create();
	ptConnect->m_ptRecvFifo = datacenter_MakeFifo(DC_FIFO_PRI_COM, MAX_RECV_FIFO_NUM);
	ptConnect->m_ptSendFifo = datacenter_MakeFifo(DC_FIFO_PRI_COM, MAX_SEND_FIFO_NUM);
	datacenter_BindFifo(ptConnect->m_ptRecvFifo, ptConnect->m_ptRecvDc);
	datacenter_BindFifo(ptConnect->m_ptSendFifo, ptConnect->m_ptSendDc);

	if(ptConnect->m_iNetType == NET_TCP)
	{
		ptConnect->m_RecvEpoll    = TcpRecvEpoll;
		ptConnect->m_SendEpoll    = TcpSendEpoll;
		ptConnect->m_AcceptEpoll  = TcpAcceptEpoll;
		ptConnect->m_RecvBuffer   = TcpRecvBuffer;
		ptConnect->m_SendBuffer   = TcpSendBuffer;
//		ptConnect->m_Connect	  = TcpConnect;		//TCP不需要
		ptConnect->m_ServerStart  = TcpServerStart;
		ptConnect->m_ServerStop   = TcpServerStop;
		ptConnect->m_Close        = CloseSocket;
		ptConnect->m_ptTcpRingBuf = ringbuf_Create(2 * RECV_BUF_TCP, RINGBUF_WRITE_COVER);
	}
	else if(ptConnect->m_iNetType == NET_UDP)
	{
		ptConnect->m_RecvEpoll = UdpRecvEpoll;
		ptConnect->m_SendEpoll = UdpSendEpoll;
//		ptConnect->m_AcceptEpoll	= UdpAcceptEpoll;
		ptConnect->m_RecvBuffer = UdpRecvBuffer;
		ptConnect->m_SendBuffer = UdpSendBuffer;
		ptConnect->m_Connect	= UdpConnect;
//		ptConnect->m_ServerStart	= UdpServerStart;
//		ptConnect->m_ServerStop = UdpServerStop;
		ptConnect->m_Close = CloseSocket;
	}

	//IOV方式不区分TCP UDP
	ptConnect->m_ReadV  = ReadVBuffer;
	ptConnect->m_WriteV = WriteVBuffer;
	
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	
	return TD_OK;
}

int UnInitConnection(TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	if(ptConnect->m_iInited != INITED)
	{
		td_printf(0, "==========warnning: %s %d %s==========not inited!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_OK;
	}

	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
	
	datacenter_Destroy(ptConnect->m_ptRecvDc);
	ptConnect->m_ptRecvDc = NULL;
	datacenter_Destroy(ptConnect->m_ptSendDc);
	ptConnect->m_ptSendDc = NULL;
	datacenter_DestroyFifo(ptConnect->m_ptRecvFifo);
	ptConnect->m_ptRecvFifo = NULL;
	datacenter_DestroyFifo(ptConnect->m_ptSendFifo);
	ptConnect->m_ptSendFifo = NULL;
	datacenter_PackageRelease(ptConnect->m_ptRecvPkg);
	ptConnect->m_ptRecvPkg = NULL;
	ptConnect->m_iRecvPos = 0;
	datacenter_PackageRelease(ptConnect->m_ptSendPkg);
	ptConnect->m_ptSendPkg = NULL;
	ptConnect->m_iSendPos = 0;

	if(ptConnect->m_iNetType == NET_TCP)
	{
		ptConnect->m_RecvEpoll 	= NULL;
		ptConnect->m_SendEpoll 	= NULL;
		ptConnect->m_RecvBuffer = NULL;
		ptConnect->m_SendBuffer = NULL;
		ptConnect->m_Connect	= NULL;
		ptConnect->m_Close 		= NULL;
		ringbuf_Destroy(ptConnect->m_ptTcpRingBuf);
		ptConnect->m_ptTcpRingBuf = NULL;
	}
	else if(ptConnect->m_iNetType == NET_UDP)
	{
		ptConnect->m_RecvEpoll 	= NULL;
		ptConnect->m_SendEpoll 	= NULL;
		ptConnect->m_RecvBuffer = NULL;
		ptConnect->m_SendBuffer = NULL;
		ptConnect->m_Connect	= NULL;
		ptConnect->m_Close 		= NULL;
	}
	
	//IOV方式不区分TCP UDP
	ptConnect->m_ReadV		= NULL;
	ptConnect->m_WriteV		= NULL;
	ptConnect->m_iInited    = UNINITED;
	ptConnect->m_iStat      = SC_NONACTIVE;
	ptConnect->m_iType 		= 0;
	ptConnect->m_iNetType	= NET_TCP;
	ptConnect->m_pUserData  = NULL;

	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	MutexLockDestroy(&ptConnect->m_tLock);

	return TD_OK;
}

int TcpRecvEpoll( TConnection *_ptConnect)
{
	//td_printf(0, "%s %d %s start! socket = %d\n", __FILE__, __LINE__, __FUNCTION__, _ptConnect->m_iSocket);
	int iRet = -1;
	int iSocket = 0;
	int iRcvlen = 0;
	char pcTmpBuf[RECV_BUF_TCP] = {0};
	TConnection *ptConnect = _ptConnect;
	TDataPackage *ptPackage = NULL;
	
	if(ptConnect == NULL)
	{
		return TD_ERROR;
	}
	
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
	
	if(ptConnect->m_iError != SOCKET_ERROR_SUCCESS || ptConnect->m_iStat != SC_CONNECTED)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iRcvlen = recv(iSocket, pcTmpBuf, RECV_BUF_TCP, 0);
	if(strstr(pcTmpBuf, "RTSP"))
	{
		td_printf(0, "\n#####################\n%s#####################\n", pcTmpBuf);
	}
	//td_printf(0, "%s %d %s iRcvLen = %d", __FILE__, __LINE__, __FUNCTION__, iRcvlen);
	if(iRcvlen < 0)
	{
		if((errno == EINTR) ||(errno == EAGAIN))
		{	
			iRet = TD_AGAIN;
			goto EXIT;
		}
		#ifdef TD_DEBUG
		perror("TcpRecvEpoll_________A, len < 0 ");
		#endif
		inner_Error(ptConnect, SOCKET_ERROR_RCV);
		iRet = TD_ERROR;
		goto EXIT;
	}
	else if(iRcvlen == 0)
	{
		#ifdef TD_DEBUG
		perror("TcpRecvEpoll_________B, len = 0 ");
		#endif
		inner_Error(ptConnect, SOCKET_ERROR_RCV);	
		iRet = TD_ERROR;
		goto EXIT;
	}
	else
	{
		while(iRcvlen > 0)
		{
			ptPackage = datacenter_CreatePackage(iRcvlen);
			if(NULL == ptPackage)
			{
				td_printf(0x12345678, "%s %d %s: warnning datacenter_CreatePackage error!\n");
				iRet= TD_ERROR;
				goto EXIT;
			}
			
//			td_printf(0, "pcTmpBuf = \n%s\n", pcTmpBuf);
			memcpy(ptPackage->m_bData, pcTmpBuf, iRcvlen);
			ptPackage->m_iDataSize = iRcvlen;
			datacenter_PushPackage(ptPackage, ptConnect->m_ptRecvDc);
#if 0
			//WriteFrameToFile(ptPackage->m_bData, ptPackage->m_iDataSize);
			TRtspDataHeader tRtspDataHeader = {0};
			memcpy(&tRtspDataHeader, ptPackage->m_bData, sizeof(TRtspDataHeader));
			td_printf(0, "==========%s==========\n", __FUNCTION__);
			td_printf(0, "%s ptPackage:%p\n", __FUNCTION__, ptPackage);
			td_printf(0, "%s iRcvlen:%d\n", __FUNCTION__, iRcvlen);
			td_printf(0, "%s TRtspDataHeader cMagic:%x\n", __FUNCTION__, tRtspDataHeader.cMagic);
			td_printf(0, "%s TRtspDataHeader cChanType:%d\n", __FUNCTION__, tRtspDataHeader.cChanType);
			td_printf(0, "%s TRtspDataHeader sDataLen:%u\n", __FUNCTION__, ntohs(tRtspDataHeader.sDataLen));
			td_printf(0, "====================\n\n");
#endif
			iRcvlen = recv(iSocket, pcTmpBuf, RECV_BUF_TCP, 0);
			//td_printf(0, "%s %d %s in while, iRcvLen = %d", __FILE__, __LINE__, __FUNCTION__, iRcvlen);
			if(iRcvlen < 0)
			{
				if((errno == EINTR) ||(errno == EAGAIN))
				{	
					iRet = TD_AGAIN;
					goto EXIT;
				}
				#ifdef TD_DEBUG
				perror("TcpRecvEpoll_________C, len < 0 ");
				#endif
				inner_Error(ptConnect, SOCKET_ERROR_RCV);
				iRet = TD_ERROR;
				goto EXIT;
			}
			else if(iRcvlen == 0)
			{
				#ifdef TD_DEBUG
				perror("TcpRecvEpoll_________D, len = 0 ");
				#endif
				inner_Error(ptConnect, SOCKET_ERROR_RCV);		
				iRet = TD_ERROR;
				goto EXIT;
			}
			else
			{
				//td_printf(0, "%s %d %s: recv again\n", __FILE__, __LINE__, __FUNCTION__);
			}
		}
	}
		
EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	return iRet;
}

int TcpSendEpoll( TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iRet = -1;
	int iSocket = 0;
	int iSendLen = 0;
	int iSendSize = 0;
	TDataPackage *ptSendPkg = NULL;
	TConnection *ptConnect = _ptConnect;

	if(ptConnect == NULL)
	{
		return TD_ERROR;
	}

	MutexLockLock(&ptConnect->m_tLock);

	if(ptConnect->m_iError != SOCKET_ERROR_SUCCESS || ptConnect->m_iStat != SC_CONNECTED)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	td_printf(0, "%s %d %s fifo num:%d\n", __FILE__, __LINE__, __FUNCTION__, datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo));
	while(datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo) > 0)
	{
		ptConnect->m_ptSendPkg = datacenter_GetPackFromFifo(ptConnect->m_ptSendFifo);
		ptConnect->m_iSendPos = 0;
		ptSendPkg = ptConnect->m_ptSendPkg;

		while(ptConnect->m_ptSendPkg != NULL)
		{
			iSendSize = ptSendPkg->m_iDataSize - ptConnect->m_iSendPos;
			iSendLen = send(iSocket, ptSendPkg->m_bData + ptConnect->m_iSendPos, iSendSize, 0);
			if(iSendLen <=0 )
			{
				if((errno == EINTR) ||(errno == EAGAIN))
				{	
					continue;
				}
				else
				{
					inner_Error(ptConnect, SOCKET_ERROR_SND);			
					iRet = TD_ERROR;
					goto EXIT; 
				}
			}
			else
			{
				if(iSendLen == iSendSize)
				{
					datacenter_PackageRelease(ptSendPkg);
					ptConnect->m_ptSendPkg = NULL;
					ptConnect->m_iSendPos = 0;
				}
				else
				{
					ptConnect->m_iSendPos += iSendLen;
				}
			}
		}
	}

EXIT:
	MutexLockUnLock(&ptConnect->m_tLock);
	return iRet;
}


int TcpAcceptEpoll( TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	int iRet = -1;
	int iNewSock = 0;

	if(ptConnect == NULL)
	{
		return TD_ERROR;
	}

	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
#ifdef TD_DEBUG
	//打印IP，调试用 i want to be a cat.
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	iNewSock = accept(ptConnect->m_iSocket, (struct sockaddr *)&addr, &addrlen);
	strcpy(ptConnect->m_pcRAddr, inet_ntoa(addr.sin_addr));
	
	td_printf(0, "#######remote ip.addr === %s", inet_ntoa(addr.sin_addr));
#else
	iNewSock = accept(ptConnect->m_iSocket, NULL, NULL);
#endif
	if(iNewSock < 0)
	{
		perror("accept errrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr:");
		iRet = TD_ERROR;
		goto EXIT;
	}

	if(ptConnect->m_AcceptCallBack != NULL)
	{
		iRet = ptConnect->m_AcceptCallBack(iNewSock);//AcceptCallBack
		if(iRet == TD_ERROR)
		{
			close(iNewSock);
			goto EXIT; 
		}
	}

EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	
	return iRet;
}

int UdpRecvEpoll(TConnection *_ptConnect)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iRet = -1;
	int iSocket = -1;
	int iRcvlen	= 0;
	struct sockaddr tServerAddr;	
	int iLen = sizeof(struct sockaddr);
	char pcTmpBuf[RECV_BUF_UDP];
	TConnection *ptConnect = _ptConnect;
	TDataPackage *ptPackage = NULL;

	if(ptConnect == NULL)
	{
		return TD_ERROR;
	}
	
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
	
	if(ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iRcvlen = recvfrom(iSocket, pcTmpBuf, RECV_BUF_UDP, MSG_DONTWAIT,(struct sockaddr*)&tServerAddr,(socklen_t *)&iLen);
	if(iRcvlen <= 0)
	{
		iRet = TD_ERROR;
		goto EXIT; 
	}
	else
	{
		ptPackage = datacenter_CreatePackage(iRcvlen);
		if(NULL == ptPackage)
		{
			iRet = TD_ERROR;
			goto EXIT;
		}
		memcpy(ptPackage->m_bData, pcTmpBuf, iRcvlen);
		ptPackage->m_iDataSize = iRcvlen;
		datacenter_PushPackage(ptPackage, ptConnect->m_ptRecvDc);
#if 0
		//WriteFrameToFile(ptPackage->m_bData, ptPackage->m_iDataSize);
		TRtspDataHeader tRtspDataHeader = {0};
		memcpy(&tRtspDataHeader, ptPackage->m_bData, sizeof(TRtspDataHeader));
		td_printf(0, "==========%s==========\n", __FUNCTION__);
		td_printf(0, "%s iRcvlen:%d\n", __FUNCTION__, iRcvlen);
		td_printf(0, "%s TRtspDataHeader cMagic:%x\n", __FUNCTION__, tRtspDataHeader.cMagic);
		td_printf(0, "%s TRtspDataHeader cChanType:%d\n", __FUNCTION__, tRtspDataHeader.cChanType);
		td_printf(0, "%s TRtspDataHeader sDataLen:%u\n", __FUNCTION__, ntohs(tRtspDataHeader.sDataLen));
		td_printf(0, "====================\n\n");
#endif	
	}
	
EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	return iRet;
}

int UdpSendEpoll( TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iRet = -1;
	int iAddrLen = 0;
	int iSocket = 0;
	int iSendLen = 0;
	int iSendSize = 0;
	TDataPackage *ptSendPkg = NULL;
	TConnection *ptConnect = _ptConnect;

	if(ptConnect == NULL)
	{
		return TD_ERROR;
	}
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");

	if(ptConnect->m_iError!= SOCKET_ERROR_SUCCESS)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	if(NULL != ptConnect->m_ptSendPkg)
	{
		ptSendPkg = ptConnect->m_ptSendPkg;
	}
	else
	{
		if(datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo) <= 0)
		{
			iRet = 0;
			goto EXIT;
		}
		ptConnect->m_ptSendPkg = datacenter_GetPackFromFifo(ptConnect->m_ptSendFifo);
		ptConnect->m_iSendPos = 0;
		ptSendPkg = ptConnect->m_ptSendPkg;
	}

	iSendSize = ptSendPkg->m_iDataSize - ptConnect->m_iSendPos;
	iAddrLen = sizeof(ptConnect->m_pcRAddr);
	iSendLen = sendto(iSocket, ptSendPkg->m_bData + ptConnect->m_iSendPos, iSendSize, 0, (struct sockaddr*)ptConnect->m_pcRAddr, iAddrLen);
	if(iSendLen == iSendSize)
	{
		datacenter_PackageRelease(ptSendPkg);
		ptConnect->m_ptSendPkg = NULL;
		ptConnect->m_iSendPos = 0;
		if(datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo) > 0)
		{
			ptConnect->m_ptSendPkg = datacenter_GetPackFromFifo(ptConnect->m_ptSendFifo);
			ptConnect->m_iSendPos = 0;
			iRet = TD_AGAIN;
			goto EXIT;
		}
		else
		{
			iRet = iSendLen;
			goto EXIT;		
		}
	}
	if(iSendLen < iSendSize)
	{
		ptConnect->m_iSendPos += iSendLen;
		iRet = TD_AGAIN;
		goto EXIT;
	}

	iRet = TD_ERROR;
	goto EXIT;
	
EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	return iRet;
}

int TcpRecvBuffer(TConnection *_ptConnect, unsigned char *_pcBuffer, int _iLen)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

int TcpSendBuffer( TConnection *_ptConnect, const unsigned char* _pcData, int _iLen)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iRet = 0;
	int iSocket = 0;
	int iSendLen = 0;
	TDataPackage *ptSendPkg = NULL;
	TConnection *ptConnect = _ptConnect;

	if(NULL == ptConnect || NULL == _pcData)
	{
		return TD_ERROR;
	}

	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");

	if(ptConnect->m_iError!= SOCKET_ERROR_SUCCESS)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	if(NULL == ptConnect->m_ptSendPkg)
	{
		iSendLen = send(iSocket, _pcData , _iLen, 0);
		if(iSendLen == _iLen)
		{
			iRet = iSendLen;
			goto EXIT;
		}
		if(iSendLen > 0)
		{
			ptSendPkg = datacenter_CreatePackage(_iLen - iSendLen);
			if(NULL == ptSendPkg)
			{
				iRet = iSendLen;
				goto EXIT;
			}
			memcpy(ptSendPkg->m_bData, _pcData + iSendLen, _iLen - iSendLen);
			ptSendPkg->m_iDataSize = _iLen - iSendLen;
			datacenter_PushPackage(ptSendPkg, ptConnect->m_ptSendDc);
			td_printf(0, "%s %d %s fifo num:%d\n", __FILE__, __LINE__, __FUNCTION__, datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo));
			ptConnect->m_ptSendPkg = ptSendPkg;
			ptConnect->m_iSendPos = 0;
			iRet = iSendLen;
			goto EXIT;
		}
		if((errno == EINTR) ||(errno == EAGAIN))
		{	
			iRet = TD_AGAIN;
			goto EXIT;
		}
		inner_Error(ptConnect, SOCKET_ERROR_SND);			
		iRet = TD_ERROR;
		goto EXIT; 
	}
	else
	{
		ptSendPkg = datacenter_CreatePackage(_iLen);
		if(NULL == ptSendPkg)
		{
			iRet = TD_ERROR;
			goto EXIT;		
		}
		memcpy(ptSendPkg->m_bData, _pcData, _iLen);
		ptSendPkg->m_iDataSize = _iLen;
		datacenter_PushPackage(ptSendPkg, ptConnect->m_ptSendDc);
		td_printf(0, "%s %d %s fifo num:%d\n", __FILE__, __LINE__, __FUNCTION__, datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo));
		iRet = _iLen;
		goto EXIT;
	}

EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	return iRet;
}

int UdpRecvBuffer(TConnection *_ptConnect, unsigned char *_pcBuffer, int _iLen)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

int UdpSendBuffer( TConnection *_ptConnect, const unsigned char* _pcData, int _iLen)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iRet = 0;
	int iSocket = 0;
	int iSendLen = 0;
	int iAddrLen = 0;
	TDataPackage *ptSendPkg = NULL;
	TConnection *ptConnect = _ptConnect;

	if(NULL == ptConnect || NULL == _pcData)
	{
		return TD_ERROR;
	}

	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");

	if(ptConnect->m_iError!= SOCKET_ERROR_SUCCESS)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}
	
	if(NULL == ptConnect->m_ptSendPkg)
	{
		iAddrLen = sizeof(ptConnect->m_tDestAddr);
		iSendLen = sendto(iSocket, _pcData , _iLen, 0, (struct sockaddr*)&ptConnect->m_tDestAddr, iAddrLen);
		if(iSendLen == _iLen)
		{
			iRet = iSendLen;
			goto EXIT;
		}
		if(iSendLen > 0)
		{
			ptSendPkg = datacenter_CreatePackage(_iLen - iSendLen);
			if(NULL == ptSendPkg)
			{
				iRet = TD_ERROR;
				goto EXIT;
			}
			memcpy(ptSendPkg->m_bData, _pcData + iSendLen, _iLen - iSendLen);
			ptSendPkg->m_iDataSize = _iLen - iSendLen;
			datacenter_PushPackage(ptSendPkg, ptConnect->m_ptSendDc);
			td_printf(0, "%s %d %s fifo num:%d\n", __FILE__, __LINE__, __FUNCTION__, datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo));
			ptConnect->m_ptSendPkg = ptSendPkg;
			ptConnect->m_iSendPos = 0;
			iRet = _iLen - iSendLen;
			goto EXIT;
		}
		iRet = TD_ERROR;
		goto EXIT; 
	}
	else
	{
		ptSendPkg = datacenter_CreatePackage(_iLen);
		if(NULL == ptSendPkg)
		{
			iRet = TD_ERROR;
			goto EXIT;
		}
		memcpy(ptSendPkg->m_bData, _pcData, _iLen);
		ptSendPkg->m_iDataSize = _iLen;
		datacenter_PushPackage(ptSendPkg, ptConnect->m_ptSendDc);
		td_printf(0, "%s %d %s fifo num:%d\n", __FILE__, __LINE__, __FUNCTION__, datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo));
		iRet = _iLen;
		goto EXIT;
	}
EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	return iRet;
}

int ReadVBuffer(TConnection *_ptConnect, TIovec *_ptIovec, int _iIovCnt)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iRet = -1;
	int iSocket = -1;
	TConnection *ptConnect = _ptConnect;
	TIovec *ptIovec = _ptIovec;
	
	if(ptConnect == NULL || ptIovec == NULL)
	{
		return TD_ERROR;
	}
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
	if(ptConnect->m_iError!= SOCKET_ERROR_SUCCESS)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iRet = readv(iSocket, ptIovec, _iIovCnt);
	if(iRet < 0)
	{	
		if((errno == EINTR) ||(errno == EAGAIN))
		{	
			iRet = TD_AGAIN;
			goto EXIT;
		}
		inner_Error(ptConnect, SOCKET_ERROR_SND);
		iRet = TD_ERROR;
		goto EXIT;
	}
	
EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");

	return iRet;
}

int WriteVBuffer(TConnection *_ptConnect, const TIovec *_ptIovec, int _iIovCnt)
{
//	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int i = 0;
	int iRet = -1;
	int iSocket = -1;
	TDataPackage *ptSendPkg = NULL;
	TConnection *ptConnect = _ptConnect;
	const TIovec *ptIovec = _ptIovec;
	int iWrLen = -1;
	int iTotalLen = 0;

	int iLeft = 0;//发了一部分的iovec所剩下的数据大小。
	int iTmpLen = 0;
	
	if(ptConnect == NULL || ptIovec == NULL)
	{
		return TD_ERROR;
	}

	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
	if(ptConnect->m_iError != SOCKET_ERROR_SUCCESS)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	iSocket = ptConnect->m_iSocket;
	if(iSocket == INVALID_SOCKET)
	{
		iRet = TD_ERROR;
		goto EXIT;
	}

	//计算iov buf总长度
	for(i=0; i<_iIovCnt; i++)
	{
		iTotalLen += ptIovec[i].iov_len;
	}


	//若缓存池为空，直接发送
	if(datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo) <= 0)
	{
		iWrLen = writev(iSocket, ptIovec, _iIovCnt);
//		td_printf(0, "%s %d %s iWrLen = %d\n", __FILE__, __LINE__, __FUNCTION__, iWrLen);
		if(iWrLen < 0) //发送失败，返回。
		{
			if((errno == EINTR) ||(errno == EAGAIN))
			{	
				ptSendPkg = datacenter_CreatePackage(iTotalLen - iWrLen);
				if(NULL == ptSendPkg)
				{
					iRet = TD_ERROR;
					goto EXIT;
				}
				ptSendPkg->m_iDataSize = 0;

				//save data
				for(i = 0; i<_iIovCnt; i++)
				{
					memcpy(ptSendPkg->m_bData + ptSendPkg->m_iDataSize, ptIovec[i].iov_base, ptIovec[i].iov_len);
					ptSendPkg->m_iDataSize += ptIovec[i].iov_len;
				}
				datacenter_PushPackage(ptSendPkg, ptConnect->m_ptSendDc);
				//td_printf(0, "%s %d %s fifo num:%d\n", __FILE__, __LINE__, __FUNCTION__, datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo));
				iRet = iWrLen;
				goto EXIT;
			}
			
			perror("wrtiev failed ");
			inner_Error(ptConnect, SOCKET_ERROR_SND);
			iRet = TD_ERROR;
			goto EXIT;
		}
		else if(iWrLen < iTotalLen)//若没有发完，则剩余数据写入缓存池。
		{
			for(i=0; i<_iIovCnt; i++)
			{
				iTmpLen += ptIovec[i].iov_len;
				if(iTmpLen > iWrLen)
				{
					iLeft = iTmpLen - iWrLen;
					break;
				}
			}
			ptSendPkg = datacenter_CreatePackage(iTotalLen - iWrLen);
			if(NULL == ptSendPkg)
			{
				iRet = TD_ERROR;
				goto EXIT;
			}
			ptSendPkg->m_iDataSize = 0;

			//先拷贝部分iov
			memcpy(ptSendPkg->m_bData, ptIovec[i].iov_base + ptIovec[i].iov_len - iLeft, iLeft);
			ptSendPkg->m_iDataSize += iLeft;//取实际用到的长度,即剩下的数据长度left
			
			i++;
			//再拷贝剩余所有iov数据。
			for(; i<_iIovCnt; i++)
			{
				memcpy(ptSendPkg->m_bData + ptSendPkg->m_iDataSize, ptIovec[i].iov_base, ptIovec[i].iov_len);
				ptSendPkg->m_iDataSize += ptIovec[i].iov_len;
			}
			datacenter_PushPackage(ptSendPkg, ptConnect->m_ptSendDc);
			//td_printf(0, "%s %d %s fifo num:%d\n", __FILE__, __LINE__, __FUNCTION__, datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo));

			iRet = iWrLen;
			goto EXIT;
		}
		else if(iWrLen == iTotalLen)
		{
			iRet = iWrLen;
			goto EXIT;
		}
		
	}
	//若缓存池不空，则把数据全部写入缓存池中。
	else  
	{
		ptSendPkg = datacenter_CreatePackage(iTotalLen);
		if(NULL == ptSendPkg)
		{
			iRet = TD_ERROR;
			goto EXIT;
		}
		ptSendPkg->m_iDataSize = 0;
		
		for(i=0; i<_iIovCnt; i++)
		{
			memcpy(ptSendPkg->m_bData + ptSendPkg->m_iDataSize, ptIovec[i].iov_base, ptIovec[i].iov_len);
			ptSendPkg->m_iDataSize += ptIovec[i].iov_len;
		}
		datacenter_PushPackage(ptSendPkg, ptConnect->m_ptSendDc);
		//td_printf(0, "%s %d %s fifo num:%d\n", __FILE__, __LINE__, __FUNCTION__, datacenter_GetFifoPackNum(ptConnect->m_ptSendFifo));
		iRet = iTotalLen;
		goto EXIT;
	}
	
EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");

	return iRet;
}


int UdpConnect(TConnection * _ptConnect, char *_pcIP, int _iPort)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	int iRet = -1;
	int iLen = 0;
	int cnt = 0;
	struct linger linger = { 0 };	
	TConnection * ptConnect = _ptConnect;
	
	if(NULL == ptConnect || NULL == _pcIP)
	{
		td_printf(0, "%s %d %s, input param error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
	
	signal(SIGPIPE, SIG_IGN);

	//创建SOCKET
	ptConnect->m_iSocket= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( ptConnect->m_iSocket < 0 )
	{
		td_printf(0, "%s %d %s, socket failed.!\n", __FILE__, __LINE__, __FUNCTION__);
		iRet = TD_ERROR;
		goto EXIT;
	}
		
	cnt = RECV_BUF_UDP;
	iRet = setsockopt(ptConnect->m_iSocket,SOL_SOCKET,SO_RCVBUF,(int *)&cnt,sizeof(int));
	cnt = SEND_BUF_UDP;
	iRet = setsockopt(ptConnect->m_iSocket,SOL_SOCKET,SO_SNDBUF,(int *)&cnt,sizeof(int));
	
	linger.l_onoff = 1;
	linger.l_linger = 0;
	iRet = setsockopt(ptConnect->m_iSocket ,SOL_SOCKET, SO_LINGER,(const char*)&linger, sizeof(linger));

	//bind
	if(ptConnect->m_iLPort >= 0)
	{
		memset(&ptConnect->m_tLocalAddr, 0, sizeof(struct sockaddr_in));
		ptConnect->m_tLocalAddr.sin_family = AF_INET;
		ptConnect->m_tLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		ptConnect->m_tLocalAddr.sin_port   = htons(ptConnect->m_iLPort);

		if(bind(ptConnect->m_iSocket, (struct sockaddr*)&ptConnect->m_tLocalAddr, sizeof(ptConnect->m_tLocalAddr)) < 0)
		{
			perror("bind faild");
			close(ptConnect->m_iSocket);
			ptConnect->m_iSocket = INVALID_SOCKET;
			iRet = TD_ERROR;
			goto EXIT;
		}
		
		//get actual port
		iLen = sizeof(ptConnect->m_tLocalAddr);
		if (getsockname(ptConnect->m_iSocket, (struct sockaddr *)&ptConnect->m_tLocalAddr, (socklen_t *)&iLen) == SOCKET_ERROR)
		{
			perror("getsockname faild");
			close(ptConnect->m_iSocket);
			ptConnect->m_iSocket = INVALID_SOCKET;
			iRet = TD_ERROR;
			goto EXIT;
		}
		ptConnect->m_iLPort = ntohs(ptConnect->m_tLocalAddr.sin_port);
	}

	//connect
	strncpy(ptConnect->m_pcRAddr, _pcIP, LENGTH_IPV4);
	ptConnect->m_iRPort = _iPort;
	memset(&ptConnect->m_tDestAddr, 0, sizeof(struct sockaddr_in));
	ptConnect->m_tDestAddr.sin_family = AF_INET;
	ptConnect->m_tDestAddr.sin_addr.s_addr =  inet_addr(ptConnect->m_pcRAddr);
	ptConnect->m_tDestAddr.sin_port =  htons(ptConnect->m_iRPort);

	if(((iRet = connect(ptConnect->m_iSocket,(struct sockaddr*)&ptConnect->m_tDestAddr, sizeof(struct sockaddr_in) )) < 0) && errno != EINPROGRESS)
	{
		perror("udp connect");
		close(ptConnect->m_iSocket);
		ptConnect->m_iSocket = INVALID_SOCKET;
		iRet = TD_ERROR;
		goto EXIT;
	}	

	datacenter_ClearFifo(ptConnect->m_ptRecvFifo);
	datacenter_ClearFifo(ptConnect->m_ptSendFifo);

	ptConnect->m_iError = SOCKET_ERROR_SUCCESS;	 
	ptConnect->m_iStat = SC_CONNECTED;
	iRet = TD_OK;
	inner_ConnectNoBlock(ptConnect);
	iRet = EpollAddConnection(ptConnect, MODE_IN);
	
	if(iRet!=0)
	{
		close(ptConnect->m_iSocket);
		ptConnect->m_iSocket = INVALID_SOCKET;
		ptConnect->m_iError = SOCKET_ERROR_SUCCESS;  
		ptConnect->m_iStat = SC_NONACTIVE;		
		iRet = TD_ERROR;
		goto EXIT; 	  
	}
	
EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	return iRet;
	
}

int TcpServerStart(TConnection * _ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int iRet = -1;
	int opt = 1;
	socklen_t len = sizeof(opt);
	TConnection * ptConnect = _ptConnect;
	struct linger so_linger = {0};

	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
	if(ptConnect->m_iSocket == INVALID_SOCKET)
	{
		ptConnect->m_iSocket = socket(AF_INET, SOCK_STREAM, 0);//SOCK_STREAM
		if(ptConnect->m_iSocket == -1)
		{
			td_printf(0, "TcpServerStart, socket error, errno = %d", errno);//EINTR
			iRet = TD_ERROR;
			goto EXIT;
		}

		//linger
		so_linger.l_onoff = 1;
		so_linger.l_linger = 0;
		iRet = setsockopt(ptConnect->m_iSocket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

		//reuseaddr
		setsockopt(ptConnect->m_iSocket, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	}
	td_printf(0, "TcpServerStart, ptConnect->m_iLPort = %d", ptConnect->m_iLPort);
	//bind to local addr
	if(ptConnect->m_iLPort > 0)
	{
		memset(&ptConnect->m_tLocalAddr, 0, sizeof(struct sockaddr_in));
		ptConnect->m_tLocalAddr.sin_family = AF_INET;
		ptConnect->m_tLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		ptConnect->m_tLocalAddr.sin_port = htons(ptConnect->m_iLPort);
	}
	iRet = bind(ptConnect->m_iSocket, (struct sockaddr*)&ptConnect->m_tLocalAddr, sizeof(ptConnect->m_tLocalAddr) );
	if(iRet < 0)
	{
		td_printf(0, "TcpServerStart, bind error, errno = %d", errno);
		inner_Close(ptConnect);
		goto EXIT;
	}

	iRet = listen(ptConnect->m_iSocket, MAX_CONNECTION);//最多128个连接。
	if(iRet < 0)
	{
		td_printf(0, "TcpServerStart, listen error, errno = %d", errno);
		inner_Close(ptConnect);
		goto EXIT;
	}

	ptConnect->m_iStat = SC_LISTENING;
	//start accept
	iRet = EpollAddConnection(ptConnect, MODE_IO);
	if(iRet < 0)
	{
		td_printf(0, "TcpServerStart, EpollAddConnection error, ret = %d", iRet);
		inner_Close(ptConnect);
		goto EXIT;
	}

	td_printf(0, "\n\n==================TcpServerStart, status is LISTENING...\n\n");

EXIT:
	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);	
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	return iRet;
}

int TcpServerStop(TConnection * _ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection * ptConnect = _ptConnect;
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");

	inner_Close(ptConnect);

	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	
	return TD_OK;
}

int BindCallBack(TConnection * _ptConnect, void *AcceptCallBack, void *DisConnectCallBack, void *_pUserData)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection * ptConnect = _ptConnect;
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");

	ptConnect->m_AcceptCallBack = (int (*)(int))AcceptCallBack;
	ptConnect->m_DisConnectCallBack = (int (*)(TConnection*))DisConnectCallBack;
	ptConnect->m_pUserData = _pUserData;

	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);	
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	
	return TD_OK;
}

int UnBindCallBack(TConnection * _ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection * ptConnect = _ptConnect;
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");

	ptConnect->m_AcceptCallBack = NULL;
	ptConnect->m_DisConnectCallBack = NULL;

	CON_DBG_LOCK3(ptConnect, "ptConnect");
	MutexLockUnLock(&ptConnect->m_tLock);	
	CON_DBG_LOCK4(ptConnect, "ptConnect");
	
	return TD_OK;
}


TConnection * CreatConnection(int _iSocket, int _iType, int _iNetType, void *_pUserData, void *DisConnectCallBack)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TConnection *ptConnect = NULL;
	int iSocket = _iSocket;
	int iRet = 0;
	int cnt = -1;
	struct sockaddr_in addr;
	socklen_t len = -1;
	struct linger so_linger = {0};	
	unsigned long ul = 1;
	
	if(iSocket < 0)
	{
		td_printf(0, "%s %d %s, input param error. iSocket = %d", __FILE__, __LINE__, __FUNCTION__, iSocket);
		return NULL;
	}

	ptConnect = (TConnection *)calloc(1, sizeof(TConnection));
	if(NULL == ptConnect)
	{
		td_printf(0, "%s %d %s calloc failed.", __FILE__, __LINE__, __FUNCTION__);
		return NULL;
	}
	
	InitConnection(ptConnect, _iType, _iNetType, 0, _pUserData);
	
	CON_DBG_LOCK1(ptConnect, "ptConnect");
	MutexLockLock(&ptConnect->m_tLock);
	CON_DBG_LOCK2(ptConnect, "ptConnect");
	
	ptConnect->m_iSocket = iSocket;
	ptConnect->m_DisConnectCallBack = (int (*)(TConnection*))DisConnectCallBack;

	//get ip and port
	len = sizeof(struct sockaddr_in);
	if (getpeername(ptConnect->m_iSocket, (struct sockaddr*)(&addr), &len) != 0)
	{
		perror("getpeername failed"); 
		MutexLockUnLock(&ptConnect->m_tLock);
		free(ptConnect);
		ptConnect = NULL;
		return NULL;
	}
	else
	{
		ptConnect->m_iRPort = ntohs(addr.sin_port);
		strncpy(ptConnect->m_pcRAddr, inet_ntoa(addr.sin_addr), LENGTH_IPV4);
	}
	
	len = sizeof(struct sockaddr_in);
	if (getsockname(ptConnect->m_iSocket, (struct sockaddr*)(&addr), &len) != 0)
	{
		perror("getsockname failed");  
		MutexLockUnLock(&ptConnect->m_tLock);
		free(ptConnect);
		ptConnect = NULL;
		return NULL;
	}
	else
	{
		ptConnect->m_iLPort = ntohs(addr.sin_port);
		strncpy(ptConnect->m_pcLAddr, inet_ntoa(addr.sin_addr), LENGTH_IPV4);
	}

	//set socket
	signal(SIGPIPE, SIG_IGN);

	//linger
	so_linger.l_onoff = 1;
	so_linger.l_linger = 0;
	iRet = setsockopt(ptConnect->m_iSocket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

	//设置为非阻塞模式
	ul = 1;
	ioctl(ptConnect->m_iSocket, FIONBIO, &ul);	 

	//buf
	cnt = RECV_BUF_TCP;
	iRet = setsockopt(ptConnect->m_iSocket,SOL_SOCKET,SO_RCVBUF,(int *)&cnt,sizeof(int));
	cnt = SEND_BUF_TCP;
	iRet = setsockopt(ptConnect->m_iSocket,SOL_SOCKET,SO_SNDBUF,(int *)&cnt,sizeof(int));
	
	inner_TcpKeepalive(ptConnect->m_iSocket);       //dongliqiang modified at 20131125
	datacenter_ClearFifo(ptConnect->m_ptRecvFifo);
	datacenter_ClearFifo(ptConnect->m_ptSendFifo);

	ptConnect->m_iError = SOCKET_ERROR_SUCCESS;  
	ptConnect->m_iStat = SC_CONNECTED;
	inner_ConnectNoBlock(ptConnect);
	iRet = EpollAddConnection(ptConnect, MODE_IOE);
	if(iRet!=0)
	{
		//close(ptConnect->m_iSocket); //we close socket when accept callback failed.
		MutexLockUnLock(&ptConnect->m_tLock);
		free(ptConnect);
		ptConnect = NULL;
		return NULL;	  
	}
	
	MutexLockUnLock(&ptConnect->m_tLock);
	td_printf(0, "%s %d %s end!\n", __FILE__, __LINE__, __FUNCTION__);
	return ptConnect;
}

int DestroyConnection(TConnection *_ptConnect)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TConnection *ptConnect = _ptConnect;
	if(NULL == ptConnect)
	{
		return TD_ERROR;
	}
	
	EpollDelConnection(ptConnect);
	
	if(ptConnect->m_iSocket != INVALID_SOCKET)
	{
		close(ptConnect->m_iSocket);
		ptConnect->m_iSocket = INVALID_SOCKET;
	}
	UnInitConnection(ptConnect);
	
	td_printf(0, "%s %d %s end!\n", __FILE__, __LINE__, __FUNCTION__);	
	return TD_OK;
}


