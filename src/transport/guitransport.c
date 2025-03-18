/*******************************************************************
本程序使用共享内存实现数据传输
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sysinfo.h>

#include "guitransport.h"

// 共享内存KEY文件
#define GUI_KEY_FILE_PATH			"guibmp/main/"
#define GUI_KEY_FILE				GUI_KEY_FILE_PATH"alarmSetting_1.bmp"
#define GUI_TX_BUF_SIZE			(1024 * 2)	//传输通道缓存大小

#define GUI_TX_UNIT_OPEND			1			//通道传输单元打开状态
#define GUI_TX_UNIT_CLOSED		2			//通道传输单元关闭状态
#define GUI_TX_UNIT_RESET			3			//通道传输单元重设状态
#if 0
#define GUI_TX_PAIR_CNT			2			//传输通道个数
#else
#define GUI_TX_PAIR_CNT			8			//20101126 添加到8个通道
#endif
#define GUI_TX_MAGIC_NO			20080922	//共享内存主标识
#define GUI_TX_RESET_BLOCK_SAFE 	0

#define DebugMsg(msg) printf("%s.%s.%d : %s\n", __FILE__, __FUNCTION__, __LINE__, msg)

TGuiVideoStatus *g_ptDvrVideoStaForGui = NULL; //录像状态保存区
TGuiAlarmStatus *g_ptDvrAlarmStaForGui = NULL; //报警状态保存区
TGuiSysStatus *g_ptGuiSysStatus = NULL; //系统状态
void *g_pGuiPrivateData = NULL;	//UI私有数据

typedef struct //传输缓存(单向)
{
	volatile int m_iStart;	//数据起始位置
	volatile int m_iEnd;	//数据结束位置
	char m_cBuf[GUI_TX_BUF_SIZE];	//数据缓存区
} TGuiTxBuffer;

typedef struct	 //传输单元
{
	int m_iRcvStat;	//接收方状态
	int m_iSndStat;	//发送方状态
	TGuiTxBuffer m_tBuf;	//传输缓存
} TGuiTxUnit;

typedef struct //双工传输单元
{
	int m_iUsed;			//单元启用标识位
	unsigned int m_uiIp;	//单元对应IP
	int m_iPort;			//单元对应端口
	int m_iLocked;			//锁定标识
	TGuiTxUnit *m_ptSendUnit;	//发送单元
	TGuiTxUnit *m_ptRcvUnit;	//接收单元
} TGuiTxPair;

typedef struct //共享内存数据传输器
{
	unsigned int m_uiMagicNo;	//特殊标识
	int m_iStat;				//当前状态
	TGuiTxPair m_tTxPairs[GUI_TX_PAIR_CNT * 2]; //传输单元
	TGuiTxUnit m_tTxUnits[GUI_TX_PAIR_CNT * 2]; //数据缓存单元
} TGuiTx;

static int g_iGuiShmId = -1;		//共享内存句柄
static TGuiTx *g_ptGuiTx = NULL;	//共享内存传输器对象

static int get_uptime(int* _msec)
{
	struct sysinfo info;
	sysinfo(&info);
	return (int)info.uptime;
}

//初始化传输装置
static void _GuiTxInit()
{
	DebugMsg("Init Gui Transport...");
	memset(g_ptGuiTx, 0, sizeof(g_ptGuiTx));
	g_ptGuiTx->m_uiMagicNo = GUI_TX_MAGIC_NO;
}

//创建共享内存传输器并初始化
int GuiTxInit()
{
	key_t tTheKey;
	int iInitFlg = 0;
	int iSize = sizeof(TGuiTx) + sizeof(TGuiAlarmStatus) + sizeof(TGuiVideoStatus) +sizeof(TGuiSysStatus) + GUI_PRIVATE_DATA_SIZE;

	tTheKey = ftok(GUI_KEY_FILE, 'a'); //创建关联key

	if (tTheKey < 0)
	{
		char strCmd[128];

		sprintf(strCmd, "mkdir -p %s", GUI_KEY_FILE_PATH);
		system(strCmd);
		sprintf(strCmd, "touch %s", GUI_KEY_FILE);
		system(strCmd);

		tTheKey = ftok(GUI_KEY_FILE, 'a');

		if (tTheKey < 0)
		{
			tTheKey = ftok("/dev/null", 'a');
		}

		if (tTheKey < 0)
		{
			return -1;
		}
	}

	//使用key创建或者打开共享内存
	g_iGuiShmId = shmget(tTheKey, iSize, 0666);
	if (g_iGuiShmId < 0)
	{
		g_iGuiShmId = shmget(tTheKey, iSize, IPC_CREAT | 0666);
		iInitFlg = 1;
	}
	if (g_iGuiShmId < 0)
	{
		DebugMsg("smd id < 0");
		return -1;
	}

	//共享内存连接到传输器
	g_ptGuiTx = (TGuiTx *)shmat(g_iGuiShmId, NULL, 0); 

	if (NULL != g_ptGuiTx)
	{
		g_ptDvrVideoStaForGui = (TGuiVideoStatus *)(g_ptGuiTx + 1);
		g_ptDvrAlarmStaForGui = (TGuiAlarmStatus *)(g_ptDvrVideoStaForGui + 1);
		g_ptGuiSysStatus = (TGuiSysStatus *)(g_ptDvrAlarmStaForGui + 1);
		g_pGuiPrivateData = (void *)(g_ptGuiSysStatus + 1);
	}

	if (1 == iInitFlg && NULL != g_ptGuiTx)
	{
		_GuiTxInit();

		memset(g_ptDvrAlarmStaForGui, 0, sizeof(TGuiAlarmStatus));
		memset(g_ptDvrVideoStaForGui, 0, sizeof(TGuiVideoStatus));
		memset(g_ptGuiSysStatus, 0, sizeof(TGuiSysStatus));
		memset(g_pGuiPrivateData, 0, GUI_PRIVATE_DATA_SIZE);
		g_ptGuiSysStatus->m_iSysUpdateTm = 0;
	}

	//操作失败删除创建的共享内存
	if (NULL == g_ptGuiTx)
	{
		shmctl(g_iGuiShmId, IPC_RMID, NULL);
		g_iGuiShmId = -1;
		return -1;
	}
	
	return 0;
}

void GuiTxUninit() //传输器析构
{
	DebugMsg("shmdt");
	if (g_ptGuiTx != NULL) //剥离共享内存
	{
		shmdt(g_ptGuiTx);
		g_ptGuiTx = NULL;
	}

	DebugMsg("shmctl rmid");
	if (g_iGuiShmId != -1) //删除共享内存
	{
		shmctl(g_iGuiShmId, IPC_RMID, NULL);
		g_iGuiShmId = -1;
	}
}

// 初始化传输单元
static void GuiTxUnitInit(TGuiTxUnit *_ptUnit, int _iSndStat, int _iRcvStat)
{
	_ptUnit->m_iSndStat = _iSndStat;
	_ptUnit->m_iRcvStat = _iRcvStat;
	_ptUnit->m_tBuf.m_iStart = 0;
	_ptUnit->m_tBuf.m_iEnd = 0;
}

// 创建一个数据传输通道
int GuiTxOpen(unsigned int _uiIp, int _iPort)
{
	int i;
	int iFreeId = -1;

	// 检查传输器是否初确初始化
	if (g_iGuiShmId < 0 || NULL == g_ptGuiTx)
	{
		//DebugMsg("shm not found");
		return -1;
	}

	// 查找未被占用的通道
	for (i = 0; i < GUI_TX_PAIR_CNT; i++)
	{
		TGuiTxPair *tpPair = &g_ptGuiTx->m_tTxPairs[i];

		if (0 == tpPair->m_iUsed)
		{
			iFreeId = i;
		}
		else if (tpPair->m_uiIp == _uiIp && tpPair->m_iPort == _iPort)
		{
			// 同一ip port对已经被占用
			//DebugMsg("");
			return -1;
		}
	}

	if (iFreeId >= 0) // 传输通道初始化
	{
		g_ptGuiTx->m_tTxPairs[iFreeId].m_iUsed = 1;
		g_ptGuiTx->m_tTxPairs[iFreeId].m_uiIp = _uiIp;
		g_ptGuiTx->m_tTxPairs[iFreeId].m_iPort = _iPort;
		g_ptGuiTx->m_tTxPairs[iFreeId].m_ptRcvUnit = g_ptGuiTx->m_tTxUnits + iFreeId;
		GuiTxUnitInit(g_ptGuiTx->m_tTxPairs[iFreeId].m_ptRcvUnit, GUI_TX_UNIT_CLOSED, GUI_TX_UNIT_OPEND);
		g_ptGuiTx->m_tTxPairs[iFreeId].m_ptSendUnit = g_ptGuiTx->m_tTxUnits + iFreeId + GUI_TX_PAIR_CNT;
		GuiTxUnitInit(g_ptGuiTx->m_tTxPairs[iFreeId].m_ptSendUnit, GUI_TX_UNIT_OPEND, GUI_TX_UNIT_CLOSED);
		g_ptGuiTx->m_tTxPairs[iFreeId].m_iLocked = 0;
	}
	
	return iFreeId;
}

// 连接到一个已打开的传输通道
static int _GuiTxConnect(unsigned int _uiIp, int _iPort, int _iReOpen)
{
	int i;
	int iFind = -1;
	int iTid;

	// 检查传输器是否初确初始化
	if (g_iGuiShmId < 0 || NULL == g_ptGuiTx)
	{
		return -1;
	}

	// 查找指定传输通道
	for (i = 0; i < GUI_TX_PAIR_CNT; i++)
	{
		TGuiTxPair *tpPair = &g_ptGuiTx->m_tTxPairs[i];

 		if (1 == tpPair->m_iUsed && tpPair->m_uiIp == _uiIp && tpPair->m_iPort == _iPort)
		{
			iFind = i;
		}
	}

	if (iFind < 0)
	{
		return -1;
	}

	iTid = GUI_TX_PAIR_CNT + iFind;

	if (0 == _iReOpen && 1 == g_ptGuiTx->m_tTxPairs[iTid].m_iUsed)
	{
		return -1;
	}

	// 新传输通道初始化
	g_ptGuiTx->m_tTxPairs[iTid].m_iUsed = 1;
	g_ptGuiTx->m_tTxPairs[iTid].m_ptRcvUnit = g_ptGuiTx->m_tTxUnits + iTid;
	g_ptGuiTx->m_tTxPairs[iTid].m_ptRcvUnit->m_tBuf.m_iStart = g_ptGuiTx->m_tTxPairs[iTid].m_ptRcvUnit->m_tBuf.m_iEnd = 0;
	g_ptGuiTx->m_tTxPairs[iTid].m_ptRcvUnit->m_iRcvStat = GUI_TX_UNIT_OPEND;
	g_ptGuiTx->m_tTxPairs[iTid].m_ptSendUnit = g_ptGuiTx->m_tTxUnits + iFind;
	g_ptGuiTx->m_tTxPairs[iTid].m_ptSendUnit->m_tBuf.m_iStart = g_ptGuiTx->m_tTxPairs[iTid].m_ptSendUnit->m_tBuf.m_iEnd = 0;
	g_ptGuiTx->m_tTxPairs[iTid].m_ptSendUnit->m_iSndStat = GUI_TX_UNIT_OPEND;
	#if !(GUI_TX_RESET_BLOCK_SAFE)
	g_ptGuiTx->m_tTxPairs[iTid].m_iLocked = 0;
	#endif

	return iTid;
}

int GuiTxConnect(unsigned int _uiIp, int _iPort)
{
	return _GuiTxConnect(_uiIp, _iPort, 0);
}

int GuiTxReConnect(unsigned int _uiIp, int _iPort)
{
	return _GuiTxConnect(_uiIp, _iPort, 1);
}

static void GuiTxReqReset(TGuiTxPair *_ptPair)
{
	int i = 0;
	TGuiTxUnit *ptUnit;
	
	while (_ptPair->m_iLocked > 0 && i++ < 300000)
	{
		usleep(10);
	}
	#if GUI_TX_RESET_BLOCK_SAFE
	_ptPair->m_iLocked++;
	#else
	_ptPair->m_iLocked = 1;
	#endif
	
	ptUnit = _ptPair->m_ptRcvUnit;
	ptUnit->m_iRcvStat = ptUnit->m_iSndStat = GUI_TX_UNIT_RESET;
	
	ptUnit = _ptPair->m_ptSendUnit;
	ptUnit->m_iRcvStat = ptUnit->m_iSndStat = -GUI_TX_UNIT_RESET;

	#if GUI_TX_RESET_BLOCK_SAFE
	_ptPair->m_iLocked--;
	#else
	_ptPair->m_iLocked = 0;
	#endif
}

static void GuiTxRspReset(TGuiTxPair *_ptPair)
{
	int i = 0;
	TGuiTxUnit *ptUnit;
	
	while (_ptPair->m_iLocked > 0 && i++ < 300000)
	{
		usleep(10);
	}
	#if GUI_TX_RESET_BLOCK_SAFE
	_ptPair->m_iLocked++;
	#else
	_ptPair->m_iLocked = 1;
	#endif
	
	ptUnit = _ptPair->m_ptRcvUnit;
	ptUnit->m_tBuf.m_iStart = 0;
	ptUnit->m_tBuf.m_iEnd = 0;
	ptUnit->m_iRcvStat = ptUnit->m_iSndStat = GUI_TX_UNIT_OPEND;
	
	ptUnit = _ptPair->m_ptSendUnit;
	ptUnit->m_tBuf.m_iStart = 0;
	ptUnit->m_tBuf.m_iEnd = 0;
	ptUnit->m_iRcvStat = ptUnit->m_iSndStat = GUI_TX_UNIT_OPEND;
	
	#if GUI_TX_RESET_BLOCK_SAFE
	_ptPair->m_iLocked--;
	#else
	_ptPair->m_iLocked = 0;
	#endif
}

// 数据传输
int GuiTxSend(int _iTid, const void *_pSndBuf, int _iSize, int _iTimeOut)
{
	int iLeft = _iSize;
	TGuiTxPair *ptPair = g_ptGuiTx->m_tTxPairs + _iTid;
	TGuiTxUnit *ptUnit;
	TGuiTxBuffer *ptBuf;
	const char *pcSrc = (const char *)_pSndBuf;
	int iStartTm = get_uptime(NULL);

	// 检查参数及传输器是否正常
	if (-1 == g_iGuiShmId || NULL == g_ptGuiTx || _iTid < 0 || NULL == _pSndBuf
		|| _iTid >= (GUI_TX_PAIR_CNT << 1) || ptPair->m_iUsed != 1)
	{
		return -1;
	}

	ptUnit = ptPair->m_ptSendUnit;
	ptBuf = &ptUnit->m_tBuf;

	if (ptUnit->m_iRcvStat != GUI_TX_UNIT_OPEND || ptUnit->m_iSndStat != GUI_TX_UNIT_OPEND)
	{
		if (-GUI_TX_UNIT_RESET == ptUnit->m_iRcvStat) //等待对方回应重设
		{
			int iTm = get_uptime(NULL);
			int iTmOut = 0;

			while (iTmOut < _iTimeOut || iTmOut < 0)
			{
				if (GUI_TX_UNIT_OPEND == ptUnit->m_iRcvStat)
				{
					break;
				}
				usleep(10);
				iTmOut = get_uptime(NULL) - iTm;
			}
			if (GUI_TX_UNIT_OPEND != ptUnit->m_iRcvStat)
			{
				return 0;
			}
		}
		else if (GUI_TX_UNIT_RESET == ptUnit->m_iRcvStat) //对方请求重设
		{
			GuiTxRspReset(ptPair);
		}
		else
		{
			return -1;
		}
	}

	while (iLeft > 0 && _iTimeOut >= get_uptime(NULL) - iStartTm) //发送全部数据结束
	{
		int iSize;
			
		while (GUI_TX_BUF_SIZE == ptBuf->m_iEnd) //等待接收方读走数据
		{
			usleep(10);
			if (ptUnit->m_iRcvStat != GUI_TX_UNIT_OPEND
				|| ptUnit->m_iSndStat != GUI_TX_UNIT_OPEND
				|| _iTimeOut < get_uptime(NULL) - iStartTm)
			{
					goto SendExit;
			}
		}
		iSize = GUI_TX_BUF_SIZE - ptBuf->m_iEnd;
		if (iSize > iLeft)
		{
			iSize = iLeft;
		}
		// 复制数据
		memcpy(ptBuf->m_cBuf + ptBuf->m_iEnd, pcSrc, iSize);
		ptBuf->m_iEnd += iSize;
		pcSrc += iSize;
		iLeft -= iSize;
	}

SendExit:
	return _iSize - iLeft;
}

// 接收数据
int GuiTxRcv(int _iTid, void *_pRcvBuf, int _iSize, int _iTimeOut)
{
	int iLeft = _iSize;
	TGuiTxPair *ptPair = g_ptGuiTx->m_tTxPairs + _iTid;
	TGuiTxUnit *ptUnit;
	TGuiTxBuffer *ptBuf;
	char *pcDst = (char *)_pRcvBuf;
	int iStartTm = get_uptime(NULL);

	if (-1 == g_iGuiShmId || NULL == g_ptGuiTx || _iTid < 0 || NULL == _pRcvBuf
		|| _iTid >= (GUI_TX_PAIR_CNT << 1) || ptPair->m_iUsed != 1)
	{
		return -1;
	}

	ptUnit = ptPair->m_ptRcvUnit;
	ptBuf = &ptUnit->m_tBuf;

	if (ptUnit->m_iRcvStat != GUI_TX_UNIT_OPEND || ptUnit->m_iSndStat != GUI_TX_UNIT_OPEND)
	{
		if (GUI_TX_UNIT_RESET == ptUnit->m_iRcvStat) //等待对方回应重设
		{
			return 0;
		}
		else if (-GUI_TX_UNIT_RESET == ptUnit->m_iRcvStat) //对方请求重设
		{
			GuiTxRspReset(ptPair);
		}
		else
		{
			return -1;
		}
	}
	
	while (iLeft > 0 && _iTimeOut >= get_uptime(NULL) - iStartTm) //接收完指定大小的数据
	{
		int iSize;
			
		while (ptBuf->m_iStart == ptBuf->m_iEnd) //等待数据
		{
			usleep(10);

			if (GUI_TX_BUF_SIZE == ptBuf->m_iStart)
			{
				ptBuf->m_iStart = 0;
				ptBuf->m_iEnd = 0;
			}
				
			if (ptUnit->m_iRcvStat != GUI_TX_UNIT_OPEND 
				|| ptUnit->m_iSndStat != GUI_TX_UNIT_OPEND
				|| _iTimeOut < get_uptime(NULL) - iStartTm)
			{
				goto RcvExit;
			}
		}
		iSize = ptBuf->m_iEnd - ptBuf->m_iStart;
		if (iSize > iLeft)
		{
			iSize = iLeft;
		}
		// 复制数据
		memcpy(pcDst, ptBuf->m_cBuf + ptBuf->m_iStart, iSize);
		pcDst += iSize;
		iLeft -= iSize;
		ptBuf->m_iStart += iSize;
	}

	// 重设缓存计数器
	if (GUI_TX_BUF_SIZE == ptBuf->m_iStart)
	{
		ptBuf->m_iStart = 0;
		ptBuf->m_iEnd = 0;
	}

RcvExit:
	return _iSize - iLeft;
}

// 关闭指定传输通道
void GuiTxClose(int _iTid)
{
	TGuiTxPair *ptPair = g_ptGuiTx->m_tTxPairs + _iTid;

	// 检测参数合法性及传输器状态
	if (_iTid < 0 || _iTid >= GUI_TX_PAIR_CNT * 2 || NULL == g_ptGuiTx)
	{
		return;
	}

	// 设置关闭状态
	ptPair->m_ptSendUnit->m_iSndStat = GUI_TX_UNIT_CLOSED;
	ptPair->m_ptRcvUnit->m_iRcvStat = GUI_TX_UNIT_CLOSED;
	ptPair->m_iUsed = 0;
}

// 等待有连接接入指定通道
void GuiTxWaitConnector(int _iTid, int _iTimeOut)
{
	int iStart = get_uptime(NULL);
	TGuiTxPair *ptPair;
	
	// 检测参数
	if (_iTid < 0 || _iTid >= GUI_TX_PAIR_CNT || NULL == g_ptGuiTx)
	{
		return;
	}
	
	ptPair = g_ptGuiTx->m_tTxPairs + _iTid;
	if (ptPair->m_iUsed != 1)
	{
		return;
	}

	// 等待连接接入
	while (GUI_TX_UNIT_OPEND != ptPair->m_ptSendUnit->m_iRcvStat ||
		GUI_TX_UNIT_OPEND != ptPair->m_ptRcvUnit->m_iSndStat)
	{
		usleep(100);

		// 超时不再等待
		if (_iTimeOut < get_uptime(NULL) - iStart)
		{
			break;
		}
	}
}

// 等待服务器方在指定ip与port对上打开传输通道
void GuiTxWaitServerStart(unsigned int _uiIp, int _iPort, int _iTimeOut)
{
	int i;
	int iFind = -1;
	int iStart = get_uptime(NULL);

	if (g_iGuiShmId < 0 || NULL == g_ptGuiTx)
	{
		return;
	}

	while (iFind == -1)
	{
		// 查找指定ip与port对上是否已建立起传输通道
		for (i = 0; i < GUI_TX_PAIR_CNT; i++)
		{
			TGuiTxPair *tpPair = &g_ptGuiTx->m_tTxPairs[i];

 			if (1 == tpPair->m_iUsed && tpPair->m_uiIp == _uiIp && tpPair->m_iPort == _iPort)
			{
				iFind = i;
				break;
			}
		}
		usleep(100);

		// 超时将不再等待
		if (_iTimeOut < get_uptime(NULL) - iStart)
		{
			break;
		}
	}
}

void GuiTxReset(int _iTid)
{
	TGuiTxPair *ptPair;

	// 检测参数
	if (_iTid < 0 || _iTid >= GUI_TX_PAIR_CNT * 2 || NULL == g_ptGuiTx)
	{
		return;
	}
	
	ptPair = g_ptGuiTx->m_tTxPairs + _iTid;
	if (ptPair->m_iUsed != 1)
	{
		return;
	}
	GuiTxReqReset(ptPair);
}

