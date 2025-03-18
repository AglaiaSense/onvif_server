/****************************************************************************
[File information]
FileName = hw_app.c
Description = common funtion 
Date = 2010-11
Funtion Desc
		int CopyFile(const char* src,const char* des)
****************************************************************************/
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>
#include <sys/shm.h>
#include "guitransport.h"
#include "share_mem.h"
#include "common.h"
#include "onvifH.h"
#include "onvif_shm_cmd.h"
#include "proparser.h"
#include "onvif_sdk.h"

pthread_mutex_t trans_lock = PTHREAD_MUTEX_INITIALIZER;
TGuiStreamChannel *channelSHaddr[MAX_CHANNEL_NUM * MAX_STREAM] = {NULL};

int snd_rcv(int _id, char *_rspbuf, char *_reqbuf, int _rspsize, int _reqsize, int _cmd)
{
	alignedchar creqbuf[sizeof(TGuiParamPdu) + _reqsize];
	alignedchar crspbuf[sizeof(TGuiParamPdu) + _rspsize];

	TGuiParamPdu *ptReqPdu = (TGuiParamPdu *) creqbuf;
	ptReqPdu->m_iCmd = _cmd;
	ptReqPdu->m_iLength = _reqsize;
	if(_reqbuf != NULL && _reqsize != 0)
	{
		memcpy(ptReqPdu->m_cPayload, _reqbuf, _reqsize);
	}
	pthread_mutex_lock(&trans_lock);
	if(GuiTxSend(_id, creqbuf, sizeof(TGuiParamPdu) + _reqsize, GUI_TX_WAIT_DEF) != (sizeof(TGuiParamPdu) + _reqsize))
	{
		td_printf(0,"GuiTxSend error\n");
		GuiTxReset(_id);
		pthread_mutex_unlock(&trans_lock);
		return -1;
	}

	if((_rspbuf == NULL) && (_rspsize <= 0))
	{
		pthread_mutex_unlock(&trans_lock);
		return 0;
	}

	if(GuiTxRcv(_id, crspbuf, sizeof(TGuiParamPdu) + _rspsize, GUI_TX_WAIT_DEF) != (sizeof(TGuiParamPdu) + _rspsize))
	{
		GuiTxReset(_id);
		pthread_mutex_unlock(&trans_lock);
		td_printf(0, "GuiTxRcv error\n");
		return -1;
	}
	
	TGuiParamPdu *ptRspPdu = (TGuiParamPdu *) crspbuf;

#if 0
	if(ptRspPdu->m_iCmd != (-1) * _cmd || ptRspPdu->m_iLength != _rspsize)
	{
		td_printf(0,"ptRspPdu->m_iCmd = %d\n",ptRspPdu->m_iCmd);
		td_printf(0,"ptRspPdu->m_iLength = %d\n",ptRspPdu->m_iLength);
		td_printf(0,"_rspsize = %d\n",_rspsize);
		GuiTxReset(_id);
		return -1;
	}
#else
	//modiefied by qiaohaijun 
	if(ptRspPdu->m_iLength != _rspsize)
	{
		td_printf(0, "the rsp size err!!");
		td_printf(0,"ptRspPdu->m_iCmd = %d\n",ptRspPdu->m_iCmd);
		td_printf(0,"ptRspPdu->m_iLength = %d\n",ptRspPdu->m_iLength);
		td_printf(0,"_rspsize = %d\n",_rspsize);
		GuiTxReset(_id);
		pthread_mutex_unlock(&trans_lock);		
		return -1;
	}
	if(ptRspPdu->m_iCmd != (-1) * _cmd)
	{
		if( ptRspPdu->m_iCmd != _cmd + 1)
		{
			td_printf(0, "the rsp cmd err!!");
			td_printf(0,"ptRspPdu->m_iCmd = %d\n",ptRspPdu->m_iCmd);
			td_printf(0,"ptRspPdu->m_iLength = %d\n",ptRspPdu->m_iLength);
			td_printf(0,"_rspsize = %d\n",_rspsize);
			GuiTxReset(_id);
			pthread_mutex_unlock(&trans_lock);	
			return -1;
		}
	}//end
#endif
	memcpy(_rspbuf, ptRspPdu->m_cPayload, _rspsize);
	pthread_mutex_unlock(&trans_lock);
	
	return 0;
}
int snd_rcv_variable(int _id, char *_rspbuf, char *_reqbuf, int _rspmaxsize, int _reqsize, int _cmd)
{
	alignedchar creqbuf[sizeof(TGuiParamPdu) + _reqsize];
	alignedchar crspbuf[sizeof(TGuiParamPdu) + _rspmaxsize];
	TGuiParamPdu *ptRspPdu = (TGuiParamPdu *) crspbuf;
	TGuiParamPdu *ptReqPdu = (TGuiParamPdu *) creqbuf;
	ptReqPdu->m_iCmd = _cmd;
	ptReqPdu->m_iLength = _reqsize;
	if(_reqbuf != NULL && _reqsize != 0)
	{
		memcpy(ptReqPdu->m_cPayload, _reqbuf, _reqsize);
	}
	pthread_mutex_lock(&trans_lock);
	if(GuiTxSend(_id, creqbuf, sizeof(TGuiParamPdu) + _reqsize, GUI_TX_WAIT_DEF) != (sizeof(TGuiParamPdu) + _reqsize))
	{
		td_printf(0,"GuiTxSend error\n");
		GuiTxReset(_id);
		pthread_mutex_unlock(&trans_lock);
		return -1;
	}

	if((_rspbuf == NULL) && (_rspmaxsize <= 0))
	{
		pthread_mutex_unlock(&trans_lock);
		return 0;
	}

	if(GuiTxRcv(_id, crspbuf, sizeof(TGuiParamPdu), GUI_TX_WAIT_DEF) != sizeof(TGuiParamPdu))
	{
		GuiTxReset(_id);
		pthread_mutex_unlock(&trans_lock);
		td_printf(0, "GuiTxRcv error\n");
		return -1;
	}
	if(ptRspPdu->m_iLength > 0)
	{
		if(GuiTxRcv(_id, crspbuf + sizeof(TGuiParamPdu), ptRspPdu->m_iLength, GUI_TX_WAIT_DEF) != ptRspPdu->m_iLength)
		{
			GuiTxReset(_id);
			pthread_mutex_unlock(&trans_lock);
			td_printf(0, "GuiTxRcv1 error\n");
			return -1;
		}
	}	
	
	if((ptRspPdu->m_iCmd == GUI_CMD_NOP) && (ptRspPdu->m_iLength == 0))
	{
		td_printf(0, "the kernel unsupport the cmd");
		pthread_mutex_unlock(&trans_lock);
		return -2;
	}
	else
	{
	#if 0
		if(ptRspPdu->m_iCmd != (-1) * _cmd || ptRspPdu->m_iLength > _rspmaxsize)
		{
			td_printf(0,"ptRspPdu->m_iCmd = %d\n",ptRspPdu->m_iCmd);
			td_printf(0,"ptRspPdu->m_iLength = %d\n",ptRspPdu->m_iLength);
			td_printf(0,"_rspmaxsize = %d\n",_rspmaxsize);
			GuiTxReset(_id);
			return -1;
		}
	#else
		//modiefied by qiaohaijun 
		if(ptRspPdu->m_iLength > _rspmaxsize)
		{
			td_printf(0, "the rsp size err!!");
			td_printf(0,"ptRspPdu->m_iCmd = %d\n",ptRspPdu->m_iCmd);
			td_printf(0,"ptRspPdu->m_iLength = %d\n",ptRspPdu->m_iLength);
			td_printf(0,"_rspsize = %d\n",_rspmaxsize);
			GuiTxReset(_id);
			pthread_mutex_unlock(&trans_lock);				
			return -1;
		}
		if(ptRspPdu->m_iCmd != (-1) * _cmd)
		{
			if( ptRspPdu->m_iCmd != _cmd + 1)
			{
				td_printf(0, "the rsp cmd err!!");
				td_printf(0,"ptRspPdu->m_iCmd = %d\n",ptRspPdu->m_iCmd);
				td_printf(0,"ptRspPdu->m_iLength = %d\n",ptRspPdu->m_iLength);
				td_printf(0,"_rspmaxsize = %d\n",_rspmaxsize);
				GuiTxReset(_id);
				pthread_mutex_unlock(&trans_lock);				
				return -1;
			}
		}//end
	#endif
		
		memcpy(_rspbuf, ptRspPdu->m_cPayload, ptRspPdu->m_iLength);
	}
	pthread_mutex_unlock(&trans_lock);

	return 0;
}
//---------------
int gui_transtream_init(int _iChn, void *_ptRsp)
{
	void *shmptr = NULL;
	int shmid = -1;
	TGuiStreamConnectRsp *ptRsp = (TGuiStreamConnectRsp *)_ptRsp;
	
	td_printf(0, "%s, %d, chn = %d, key = %d, size = %d", __FUNCTION__, __LINE__, _iChn, ptRsp->m_iKey, ptRsp->m_iSize);
	shmid = shmget(ptRsp->m_iKey, ptRsp->m_iSize, (IPC_CREAT | 0666));
	if(shmid == -1)
	{
		perror("shmget failed");
		return -1;
	}
	shmptr = (void *) (shmat(shmid, NULL, 0));
	if(shmptr == (void *)-1)
	{
		perror("shmat failed");
		return -1;
	}
	channelSHaddr[_iChn] = (TGuiStreamChannel *) (shmptr);	//保存共享内存首地址
	channelSHaddr[_iChn]->m_iBase = channelSHaddr[_iChn]->m_iTop;
	td_printf(0x12345678, "%s %d %s, shmptr[%d] = %p\n", __FILE__, __LINE__, __FUNCTION__, _iChn, channelSHaddr[_iChn]);
	
	return 0;
}

int gui_transtream_uninit(int _iChn)
{
	void *shmptr = NULL;

	//detech
	td_printf(0, "%s %d %s iChn %d\n", __FILE__, __LINE__, __FUNCTION__, _iChn);
	shmptr = channelSHaddr[_iChn];
	if(NULL != shmptr)
	{
		shmdt(shmptr);
		channelSHaddr[_iChn] = NULL;
	}
	
	return 0;
}


int gui_trans_init()
{
	//func_info();
	int iTid = -1;
	int i = 0;
	
	if(-1 == GuiTxInit())
	{
		td_printf(0,  "GuiTxInit() failed!\n");
		return TD_ERROR;
	}

	GuiTxWaitServerStart(GUI_TX_LOCAL_IP, GUI_TX_MSG_PORT, GUI_TX_WAIT_DEF);
	iTid = GuiTxConnect(GUI_TX_LOCAL_IP, GUI_TX_MSG_PORT);
	while(iTid < 0 && i < 3)
	{
		iTid = GuiTxReConnect(GUI_TX_LOCAL_IP, GUI_TX_MSG_PORT);
		i++;
	}
	if(i == 3)
	{
		GuiTxClose(iTid);
		GuiTxUninit();
		return TD_ERROR;
	}
	return iTid;
}

int gui_datatrans_init()
{
	int iTid = -1;
	int i = 0;
	GuiTxWaitServerStart(GUI_TX_LOCAL_IP, GUI_TX_DATA_PORT, GUI_TX_WAIT_DEF);
	iTid = GuiTxConnect(GUI_TX_LOCAL_IP, GUI_TX_DATA_PORT);
	while(iTid < 0 && i < 3)
	{
		iTid = GuiTxReConnect(GUI_TX_LOCAL_IP, GUI_TX_DATA_PORT);
		i++;
	}
	if(i == 3)
	{
		GuiTxClose(iTid);
		GuiTxUninit();
		return -1;
	}
	return iTid;
}

/****************************************************
 *功能		:	从dvr共享内存中获取分辨率列表
 *参数		: 	_id		     :	id号
                               _pVideoList  :     分辨率列表
 					_reqbuf       :     请求buf
                               _rspsize       :     响应数据的大小
                               _reqsize	     :      请求数据的大小
                               _cmd           :	命令码
 *返回值	:	成功       :      支持的分辨率个数
 					失败       :      -1
 *修改历史	:	create by ptb 20130217
****************************************************/
int snd_rcv_dvr(int _id, int *_pVideoList, char *_reqbuf, int _rspsize, int _reqsize, int _cmd)
{
	alignedchar creqbuf[sizeof(TGuiParamPdu) + _reqsize];
	alignedchar crspbuf[sizeof(TGuiParamPdu) + _rspsize];
	char cTmpBuf[sizeof(TGuiVideoSizeItem)] = {0};
	TGuiParamPdu *ptReqPdu = (TGuiParamPdu *) creqbuf;
	ptReqPdu->m_iCmd       = _cmd;
	ptReqPdu->m_iLength    = _reqsize;
	TGuiGetChnVideoSizeRsp *pVideoSizeRsp = NULL;
	TGuiVideoSizeItem *pVideoSizeItem     = NULL;
	TGuiParamPdu *pdu = NULL;
	int i = 0;

	if(_reqbuf != NULL && _reqsize != 0)
	{
		memcpy(ptReqPdu->m_cPayload, _reqbuf, _reqsize);
	}
	
	pthread_mutex_lock(&trans_lock);
	if(GuiTxSend(_id, creqbuf, sizeof(TGuiParamPdu) + _reqsize, GUI_TX_WAIT_DEF) != (sizeof(TGuiParamPdu)+ _reqsize))
	{
		td_printf(0,"GuiTxSend error\n");
		GuiTxReset(_id);
		pthread_mutex_unlock(&trans_lock);
		return -1;
	}

	if((_pVideoList == NULL) && (_rspsize <= 0))
	{
		pthread_mutex_unlock(&trans_lock);
		return 0;
	}

	if(GuiTxRcv(_id, crspbuf, sizeof(TGuiParamPdu) + _rspsize, GUI_TX_WAIT_DEF) != (sizeof(TGuiParamPdu) + _rspsize))
	{
		GuiTxReset(_id);
		pthread_mutex_unlock(&trans_lock);
		td_printf(0, "GuiTxRcv error\n");
		return -1;
	}
	
	pdu = (TGuiParamPdu *) crspbuf;

	if(pdu->m_iCmd != (-1) * _cmd || pdu->m_iLength != _rspsize)
	{
		td_printf(0,"ptRspPdu->m_iCmd = %d\n", pdu->m_iCmd);
		td_printf(0,"ptRspPdu->m_iLength = %d\n", pdu->m_iLength);
		td_printf(0,"_rspsize = %d\n",_rspsize);
		GuiTxReset(_id);
		pthread_mutex_unlock(&trans_lock);
		return -1;
	}

	pVideoSizeRsp = (TGuiGetChnVideoSizeRsp *)pdu->m_cPayload;
	
	if(pVideoSizeRsp->m_iCnt > MAX_VIDEOSIZE_LIST)
	{		
		td_printf(0,"pVideoSizeRsp->m_iCnt is %d", pVideoSizeRsp->m_iCnt);
		pthread_mutex_unlock(&trans_lock);
		return -1;
	}

	for(i = 0; i < pVideoSizeRsp->m_iCnt; i++)
	{
		memset(cTmpBuf, 0, sizeof(TGuiVideoSizeItem));
		if(GuiTxRcv(_id, cTmpBuf, sizeof(TGuiVideoSizeItem), GUI_TX_WAIT_DEF) != sizeof(TGuiVideoSizeItem))
		{
			GuiTxReset(_id);
			pthread_mutex_unlock(&trans_lock);
			td_printf(0, "GuiTxRcv error\n");
			return -1;
		}
		pVideoSizeItem = (TGuiVideoSizeItem *)cTmpBuf;
		_pVideoList[i] = pVideoSizeItem->m_iVideoSize;
	}
	
	pthread_mutex_unlock(&trans_lock);
	return pVideoSizeRsp->m_iCnt;
}

/************************************************************
 *功能		:	获取共享内存通道地址。
 *参数		: 	_iChno : 通道号(从0开始), 合法性由调用者保证。
 *返回值	:	成功返回共享内存通道地址, 可以为NULL。
*************************************************************/
char *get_chn_shmaddr(int _iChno)
{
	return (char *)channelSHaddr[_iChno];
}


