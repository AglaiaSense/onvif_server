/*******************************************************************
������ʹ�ù����ڴ�ʵ�����ݴ���
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

// �����ڴ�KEY�ļ�
#define GUI_KEY_FILE_PATH			"guibmp/main/"
#define GUI_KEY_FILE				GUI_KEY_FILE_PATH"alarmSetting_1.bmp"
#define GUI_TX_BUF_SIZE			(1024 * 2)	//����ͨ�������С

#define GUI_TX_UNIT_OPEND			1			//ͨ�����䵥Ԫ��״̬
#define GUI_TX_UNIT_CLOSED		2			//ͨ�����䵥Ԫ�ر�״̬
#define GUI_TX_UNIT_RESET			3			//ͨ�����䵥Ԫ����״̬
#if 0
#define GUI_TX_PAIR_CNT			2			//����ͨ������
#else
#define GUI_TX_PAIR_CNT			8			//20101126 ��ӵ�8��ͨ��
#endif
#define GUI_TX_MAGIC_NO			20080922	//�����ڴ�����ʶ
#define GUI_TX_RESET_BLOCK_SAFE 	0

#define DebugMsg(msg) printf("%s.%s.%d : %s\n", __FILE__, __FUNCTION__, __LINE__, msg)

TGuiVideoStatus *g_ptDvrVideoStaForGui = NULL; //¼��״̬������
TGuiAlarmStatus *g_ptDvrAlarmStaForGui = NULL; //����״̬������
TGuiSysStatus *g_ptGuiSysStatus = NULL; //ϵͳ״̬
void *g_pGuiPrivateData = NULL;	//UI˽������

typedef struct //���仺��(����)
{
	volatile int m_iStart;	//������ʼλ��
	volatile int m_iEnd;	//���ݽ���λ��
	char m_cBuf[GUI_TX_BUF_SIZE];	//���ݻ�����
} TGuiTxBuffer;

typedef struct	 //���䵥Ԫ
{
	int m_iRcvStat;	//���շ�״̬
	int m_iSndStat;	//���ͷ�״̬
	TGuiTxBuffer m_tBuf;	//���仺��
} TGuiTxUnit;

typedef struct //˫�����䵥Ԫ
{
	int m_iUsed;			//��Ԫ���ñ�ʶλ
	unsigned int m_uiIp;	//��Ԫ��ӦIP
	int m_iPort;			//��Ԫ��Ӧ�˿�
	int m_iLocked;			//������ʶ
	TGuiTxUnit *m_ptSendUnit;	//���͵�Ԫ
	TGuiTxUnit *m_ptRcvUnit;	//���յ�Ԫ
} TGuiTxPair;

typedef struct //�����ڴ����ݴ�����
{
	unsigned int m_uiMagicNo;	//�����ʶ
	int m_iStat;				//��ǰ״̬
	TGuiTxPair m_tTxPairs[GUI_TX_PAIR_CNT * 2]; //���䵥Ԫ
	TGuiTxUnit m_tTxUnits[GUI_TX_PAIR_CNT * 2]; //���ݻ��浥Ԫ
} TGuiTx;

static int g_iGuiShmId = -1;		//�����ڴ���
static TGuiTx *g_ptGuiTx = NULL;	//�����ڴ洫��������

static int get_uptime(int* _msec)
{
	struct sysinfo info;
	sysinfo(&info);
	return (int)info.uptime;
}

//��ʼ������װ��
static void _GuiTxInit()
{
	DebugMsg("Init Gui Transport...");
	memset(g_ptGuiTx, 0, sizeof(g_ptGuiTx));
	g_ptGuiTx->m_uiMagicNo = GUI_TX_MAGIC_NO;
}

//���������ڴ洫��������ʼ��
int GuiTxInit()
{
	key_t tTheKey;
	int iInitFlg = 0;
	int iSize = sizeof(TGuiTx) + sizeof(TGuiAlarmStatus) + sizeof(TGuiVideoStatus) +sizeof(TGuiSysStatus) + GUI_PRIVATE_DATA_SIZE;

	tTheKey = ftok(GUI_KEY_FILE, 'a'); //��������key

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

	//ʹ��key�������ߴ򿪹����ڴ�
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

	//�����ڴ����ӵ�������
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

	//����ʧ��ɾ�������Ĺ����ڴ�
	if (NULL == g_ptGuiTx)
	{
		shmctl(g_iGuiShmId, IPC_RMID, NULL);
		g_iGuiShmId = -1;
		return -1;
	}
	
	return 0;
}

void GuiTxUninit() //����������
{
	DebugMsg("shmdt");
	if (g_ptGuiTx != NULL) //���빲���ڴ�
	{
		shmdt(g_ptGuiTx);
		g_ptGuiTx = NULL;
	}

	DebugMsg("shmctl rmid");
	if (g_iGuiShmId != -1) //ɾ�������ڴ�
	{
		shmctl(g_iGuiShmId, IPC_RMID, NULL);
		g_iGuiShmId = -1;
	}
}

// ��ʼ�����䵥Ԫ
static void GuiTxUnitInit(TGuiTxUnit *_ptUnit, int _iSndStat, int _iRcvStat)
{
	_ptUnit->m_iSndStat = _iSndStat;
	_ptUnit->m_iRcvStat = _iRcvStat;
	_ptUnit->m_tBuf.m_iStart = 0;
	_ptUnit->m_tBuf.m_iEnd = 0;
}

// ����һ�����ݴ���ͨ��
int GuiTxOpen(unsigned int _uiIp, int _iPort)
{
	int i;
	int iFreeId = -1;

	// ��鴫�����Ƿ��ȷ��ʼ��
	if (g_iGuiShmId < 0 || NULL == g_ptGuiTx)
	{
		//DebugMsg("shm not found");
		return -1;
	}

	// ����δ��ռ�õ�ͨ��
	for (i = 0; i < GUI_TX_PAIR_CNT; i++)
	{
		TGuiTxPair *tpPair = &g_ptGuiTx->m_tTxPairs[i];

		if (0 == tpPair->m_iUsed)
		{
			iFreeId = i;
		}
		else if (tpPair->m_uiIp == _uiIp && tpPair->m_iPort == _iPort)
		{
			// ͬһip port���Ѿ���ռ��
			//DebugMsg("");
			return -1;
		}
	}

	if (iFreeId >= 0) // ����ͨ����ʼ��
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

// ���ӵ�һ���Ѵ򿪵Ĵ���ͨ��
static int _GuiTxConnect(unsigned int _uiIp, int _iPort, int _iReOpen)
{
	int i;
	int iFind = -1;
	int iTid;

	// ��鴫�����Ƿ��ȷ��ʼ��
	if (g_iGuiShmId < 0 || NULL == g_ptGuiTx)
	{
		return -1;
	}

	// ����ָ������ͨ��
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

	// �´���ͨ����ʼ��
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

// ���ݴ���
int GuiTxSend(int _iTid, const void *_pSndBuf, int _iSize, int _iTimeOut)
{
	int iLeft = _iSize;
	TGuiTxPair *ptPair = g_ptGuiTx->m_tTxPairs + _iTid;
	TGuiTxUnit *ptUnit;
	TGuiTxBuffer *ptBuf;
	const char *pcSrc = (const char *)_pSndBuf;
	int iStartTm = get_uptime(NULL);

	// ���������������Ƿ�����
	if (-1 == g_iGuiShmId || NULL == g_ptGuiTx || _iTid < 0 || NULL == _pSndBuf
		|| _iTid >= (GUI_TX_PAIR_CNT << 1) || ptPair->m_iUsed != 1)
	{
		return -1;
	}

	ptUnit = ptPair->m_ptSendUnit;
	ptBuf = &ptUnit->m_tBuf;

	if (ptUnit->m_iRcvStat != GUI_TX_UNIT_OPEND || ptUnit->m_iSndStat != GUI_TX_UNIT_OPEND)
	{
		if (-GUI_TX_UNIT_RESET == ptUnit->m_iRcvStat) //�ȴ��Է���Ӧ����
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
		else if (GUI_TX_UNIT_RESET == ptUnit->m_iRcvStat) //�Է���������
		{
			GuiTxRspReset(ptPair);
		}
		else
		{
			return -1;
		}
	}

	while (iLeft > 0 && _iTimeOut >= get_uptime(NULL) - iStartTm) //����ȫ�����ݽ���
	{
		int iSize;
			
		while (GUI_TX_BUF_SIZE == ptBuf->m_iEnd) //�ȴ����շ���������
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
		// ��������
		memcpy(ptBuf->m_cBuf + ptBuf->m_iEnd, pcSrc, iSize);
		ptBuf->m_iEnd += iSize;
		pcSrc += iSize;
		iLeft -= iSize;
	}

SendExit:
	return _iSize - iLeft;
}

// ��������
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
		if (GUI_TX_UNIT_RESET == ptUnit->m_iRcvStat) //�ȴ��Է���Ӧ����
		{
			return 0;
		}
		else if (-GUI_TX_UNIT_RESET == ptUnit->m_iRcvStat) //�Է���������
		{
			GuiTxRspReset(ptPair);
		}
		else
		{
			return -1;
		}
	}
	
	while (iLeft > 0 && _iTimeOut >= get_uptime(NULL) - iStartTm) //������ָ����С������
	{
		int iSize;
			
		while (ptBuf->m_iStart == ptBuf->m_iEnd) //�ȴ�����
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
		// ��������
		memcpy(pcDst, ptBuf->m_cBuf + ptBuf->m_iStart, iSize);
		pcDst += iSize;
		iLeft -= iSize;
		ptBuf->m_iStart += iSize;
	}

	// ���軺�������
	if (GUI_TX_BUF_SIZE == ptBuf->m_iStart)
	{
		ptBuf->m_iStart = 0;
		ptBuf->m_iEnd = 0;
	}

RcvExit:
	return _iSize - iLeft;
}

// �ر�ָ������ͨ��
void GuiTxClose(int _iTid)
{
	TGuiTxPair *ptPair = g_ptGuiTx->m_tTxPairs + _iTid;

	// �������Ϸ��Լ�������״̬
	if (_iTid < 0 || _iTid >= GUI_TX_PAIR_CNT * 2 || NULL == g_ptGuiTx)
	{
		return;
	}

	// ���ùر�״̬
	ptPair->m_ptSendUnit->m_iSndStat = GUI_TX_UNIT_CLOSED;
	ptPair->m_ptRcvUnit->m_iRcvStat = GUI_TX_UNIT_CLOSED;
	ptPair->m_iUsed = 0;
}

// �ȴ������ӽ���ָ��ͨ��
void GuiTxWaitConnector(int _iTid, int _iTimeOut)
{
	int iStart = get_uptime(NULL);
	TGuiTxPair *ptPair;
	
	// ������
	if (_iTid < 0 || _iTid >= GUI_TX_PAIR_CNT || NULL == g_ptGuiTx)
	{
		return;
	}
	
	ptPair = g_ptGuiTx->m_tTxPairs + _iTid;
	if (ptPair->m_iUsed != 1)
	{
		return;
	}

	// �ȴ����ӽ���
	while (GUI_TX_UNIT_OPEND != ptPair->m_ptSendUnit->m_iRcvStat ||
		GUI_TX_UNIT_OPEND != ptPair->m_ptRcvUnit->m_iSndStat)
	{
		usleep(100);

		// ��ʱ���ٵȴ�
		if (_iTimeOut < get_uptime(NULL) - iStart)
		{
			break;
		}
	}
}

// �ȴ�����������ָ��ip��port���ϴ򿪴���ͨ��
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
		// ����ָ��ip��port�����Ƿ��ѽ�������ͨ��
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

		// ��ʱ�����ٵȴ�
		if (_iTimeOut < get_uptime(NULL) - iStart)
		{
			break;
		}
	}
}

void GuiTxReset(int _iTid)
{
	TGuiTxPair *ptPair;

	// ������
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

