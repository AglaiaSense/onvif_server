/****************************************************************************
�ļ���:		guitransport.h
��	��:		ͼ�ν��������س������ݽ����ӿ�

�޸���־:
�޸���		�޸�����		�޸�˵��
==========  ==============  ===========================================
������		2008-09-16		������ʵ�ֻ�������
****************************************************************************/

#ifndef _GUI_TRANSPORT_H_
#define _GUI_TRANSPORT_H_

#define GUI_TX_NO_WAIT			0					//���ݴ��䲻�ȴ�
#define GUI_TX_WAIT_DONE		0x7FFFF000		//���ݴ���ȴ����
#define GUI_TX_WAIT_DEF		10					//Ĭ�ϳ�ʱʱ��

#define GUI_TX_SERVER			0	//������������(�������س���)
#define GUI_TX_CLIENT			1	//�ͻ���������(����ͼ�ν���)

#define GUI_TX_LOCAL_IP		0	//����IP
#define GUI_TX_MAIN_PORT		1	//������ͨ���˿�
#define GUI_TX_MSG_PORT		2	//��Ϣ����ͨ���˿�
#define GUI_TX_DATA_PORT		3	//͸�����ݡ��������ݴ���ͨ��

#define GUI_VIDEO_IN_CHN_CNT	32	//��Ƶ����ͨ����

#define GUI_VIDEO_FREE			0	//ͨ��δ¼��
#define GUI_VIDEO_HUM			1	//ͨ�����ڽ����ֶ�¼��
#define GUI_VIDEO_TM			2	//ͨ�����ڽ��ж�ʱ¼��
#define GUI_VIDEO_ALARM			3	//ͨ�����ڽ��б���¼��
#define GUI_VIDEO_DEF			4	//ͨ�����ڽ����Զ���¼��
#define GUI_VIDEO_NONE_HUM		11	//ͨ�����ڽ����ֶ�¼��(����Ƶ)
#define GUI_VIDEO_NONE_TM		12	//ͨ�����ڽ��ж�ʱ¼��(����Ƶ)
#define GUI_VIDEO_NONE_ALARM	13	//ͨ�����ڽ��б���¼��(����Ƶ)
#define GUI_VIDEO_NONE_DEF		14	//ͨ�����ڽ����Զ���¼��(����Ƶ)
#define GUI_VIDEO_NONE			-1	//ͨ������Ƶ����

typedef struct					//¼��״̬
{
	int m_iVideoSta[GUI_VIDEO_IN_CHN_CNT];	//��ͨ����Ӧ��¼��״̬
} TGuiVideoStatus;

extern TGuiVideoStatus *g_ptDvrVideoStaForGui; //¼��״̬������

#define GUI_ALARM_NONE			0	//�ޱ���
#define GUI_ALARM_PORT			1	//�˿ڱ���
#define GUI_ALARM_MD			2	//�ƶ���ⱨ��
#define GUI_ALARM_VLST			3	//��Ƶ��ʧ����
#define GUI_ALARM_VCVR			4	//��Ƶ�ڵ�����

typedef struct					//����״̬
{
	int m_iAlarmSta[GUI_VIDEO_IN_CHN_CNT];	//��ͨ����Ӧ�ı���״̬
	int m_iAlarmSound;			//����������ʾ(����ģ�����ò����)
	int m_iAlarmLed;			//ǰ��屨��״̬��
} TGuiAlarmStatus;

extern TGuiAlarmStatus *g_ptDvrAlarmStaForGui; //����״̬������

#define GUI_PRIVATE_DATA_SIZE	512
extern void *g_pGuiPrivateData;				 //U˽������

#define GUI_SYS_STA_STARTUP		0		//ϵͳ��������
#define GUI_SYS_STA_RUNNING		1		//ϵͳ��������
#define GUI_SYS_STA_REBOOT			2		//ϵͳ��������
#define GUI_SYS_STA_SHUTDOWN		3		//ϵͳ���ڹػ�

#define GUI_SYS_ERR_NONE			0		//�޴���
#define GUI_SYS_ERR_DISK_FULL		1		//������
#define GUI_SYS_ERR_DISK_NULL		2		//�޿��ô���
#define GUI_SYS_ERR_DISK_RW		3		//��д����ʧ��
#define GUI_SYS_ERR_ACCESS		4		//�Ƿ�����
#define GUI_SYS_ERR_IP_CONF		5		//IP��ַ��ͻ
#define GUI_SYS_ERR_NET_INTE		6		//�����ж�
#define GUI_SYS_ERR_VIDEOIN_MODE		7		//������ʽ����

#define GUI_SYS_STA_PCNT			4		//ϵͳ״̬��������
#define GUI_SYS_ERR_PCNT			4		

#define GUI_SHR_MEM_EXT			1
//m_cShmRsv�ֶ��ڴ��������ʼ��ַ
#define GUI_GPS_OFFSET            	0  //����GPS��ʾ��ƫ��GPSռ��36�ֽ�
#define GUI_ALM_SPIC_OFFSET			48		//��������������(4 * 3�ֽ�)
#define GUI_CBL_SHM_START			128    //����CBL��ƫ���ֽ� ռ��36�ֽ�
#define GUI_TEMP_SHM_START          256    //������ʪ�ȵ�ƫ��(256�ֽ�)
#define GUI_ALAR_STA_START			512		//���ڱ����������״̬(Ԥ��128�ֽ�)
#define GUI_AUDIO_STA_START			640		//������Ƶ����״̬(Ԥ��128�ֽ�)
#define GUI_SHR_DECODER_START       770     //���ںͽ�����ͨѶ(Ԥ��56�ֽ�)

#if 1
#define GUI_SHR_MEM_RSV			(4 * 1024)
#else
#define GUI_SHR_MEM_RSV				0
#endif

typedef struct						//GUI��ϵͳ״̬
{
	int m_iGuiPid;					//UI���̺�
	int m_iDvrPid;					//DVR��������̺�
	int m_iGuiLastActiveTm;		//UI����Ծʱ�䣬�൱��UI��������UI����
	int m_iSysUpdateTm;			//���������״̬��ʱ�䣬������������
	int m_iSysSta;					//ϵͳ״̬��������������
	int m_iParams[GUI_SYS_STA_PCNT];	//״̬��������������m_iSysSta����
	int m_iSysErr;					//ϵͳ�����ʶ,��������ά��
	int m_iSysErrTm;				//ϵͳ������ʱ��(time()ֵ)����������ά��
	int m_iSysErrMsg[GUI_SYS_ERR_PCNT]; //ϵͳ������չ��Ϣ����ʱ����
									//2011-8-10 ���һ������������ΪԤ��������
	//2011-10-21    by wanzy
	// �����ڶ��������������λ����Ϊ�ֶ�¼���Ƿ�ɹ���������10Ϊʧ�ܣ������Ϊ�ɹ���
	int m_iDvrErrNo;				//DVR������
	int m_iGuideFlg;				//���������б�ʶ,��UI��ʶ
	int m_iGuideEnFlg;				//�����򵼿����б�ʶ,��dvr��ʶ
	char m_cShmRsv[GUI_SHR_MEM_RSV];	 //Ԥ�������ڴ棬��������
} TGuiSysStatus;

extern TGuiSysStatus *g_ptGuiSysStatus;		//UI������ϵͳ״̬

typedef struct
{
	// �˿ڵ���ʵ״̬
	int m_iAlarmOutValue;		//��������˿�״̬1-32
	int m_iAlarmOutValue1;		//Ԥ��
	int m_iAlarmInValue;			//��������˿�״̬1-32
	int m_iAlarmInValue2;		//Ԥ��

	// �˿��Ƿ����ڱ���
	int m_iAlarmOut;				//�˿��������1-32
	int m_iAlarmOut1;
	int m_iAlarmIn;				//�˿����뱨��1-32
	int m_iAlarmIn1;

	// ��Ƶ��ʧ����
	int m_iVideoLost;				//��Ƶ��ʧ����1-32
	int m_iVideoLost1;			//Ԥ��

	// ��Ƶ�ƶ�����
	int m_iVideoMd;				//�ƶ���ⱨ��1-32
	int m_iVideoMd1;

	// ��Ƶ�ڵ�
	int m_iVideoCover;
	int m_iVideoCover1;
} TGuiAlarmInOutStatus;

#define g_ptGuiAlarmInOutStatus	((TGuiAlarmInOutStatus *)(g_ptGuiSysStatus->m_cShmRsv + GUI_ALAR_STA_START))

typedef struct
{
	int m_iInputBits;			//��Ƶ����״̬,bit0-bit32��ʽ��Ƶ0-31ͨ��,1��ʽ����Ƶ,0��ʽ����Ƶ
	int m_iInputBitsEx;			//������չ64·ʹ��
	unsigned char m_ucVolValue[64];	//����ֵ(����)
} TGuiAudioInputStatus;

#define g_ptGuiAudioInputStatus ((TGuiAudioInputStatus *)(g_ptGuiSysStatus->m_cShmRsv + GUI_AUDIO_STA_START))

typedef struct   //��ʪ����Ϣ
{
	int m_iDspEnable;  //�Ƿ���ʾ�ı�־
	float m_fTemp;       //�¶�
	float m_fHumidity;  //ʪ��
}TGuiTempMsg;
#define g_ptGuiTemperatureMsg ((TGuiTempMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_TEMP_SHM_START))

typedef struct	//������Ϣ
{
	int m_iSta;		//״̬
	int m_iUsed;		// ռ�ñ�ʶ(0��ʽδռ�ã�������ʽ���̺ţ�����ʹ��)
	int m_iUsedSize;	//�ѿ�¼��С(��λK)
	int m_iTotalSize;	//�����ܴ�С(��λK)
} TGuiCdSta;

#define GUI_CBL_MAX_CD		2
typedef struct	// ʵʱ��¼״̬��Ϣ
{
	int m_iStat;	// ��¼״̬
	TGuiCdSta m_tSCdSta[GUI_CBL_MAX_CD];
} TGuiCblSta;
#define g_ptGuiCblStat ((TGuiCblSta *)(g_ptGuiSysStatus->m_cShmRsv + GUI_CBL_SHM_START))

typedef struct			//����������������Ϣ��
{
	char m_iVoDev[4];	//VO�豸��
	char m_iChn[4];		//ͨ����
	char m_iAlarm[4];	//��������
} TGuiAlarmLinkSinglePicMsg;
#define g_ptGuiAlarmLinkSinglePicMsg ((TGuiAlarmLinkSinglePicMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_ALM_SPIC_OFFSET))

//������
typedef struct
{
	char  m_cDevNo;	    	//��ʾ�豸��	
	char  m_cDecChnCnt;  		//������������
	char  m_cDecCurChn;    		//��������ǰ����
	char m_cDecAlarmer[16]; 		//��������������λ��ʾ 0 ��Ƶ��ʧ 1��Ƶ�ƶ� 2�˿�
	char m_cDecSwitchStat[16];  	//�������л�״̬
	char m_cDecCtrlStat[16];    	//����������״̬
	char m_cLogoDspStat;         //Logo��ʾ
	char m_cComAttr;            //�������Ըı��־
	char m_cOtherAttr;          //�������Ըı��־ ����λ0��ʾ�ֱ���
}TGuiDecShrMsg;
#define g_ptGuiDecoderShrMsg ((TGuiDecShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_SHR_DECODER_START))

static inline int GetDvrErrNo()			//��ȡDVR�����룬��UI����
{
	int n = g_ptGuiSysStatus->m_iDvrErrNo;
	
	g_ptGuiSysStatus->m_iDvrErrNo = 0;
	return n;
}

static inline void SetDvrErrNo(int _iNo)			//����DVR��������dvr����
{
	g_ptGuiSysStatus->m_iDvrErrNo = _iNo;
}

/**********************************************************************
������:		GuiTxInit()
��	��:		���ݴ�������ʼ��
�Ρ���:		��
����ֵ:		0:�ɹ�, -1ʧ��
��	־:		2008-09-22����������������ʵ��
**********************************************************************/
int GuiTxInit();

/**********************************************************************
������:		GuiTxUninit()
��	��:		���ݴ���������ʼ��
�Ρ���:		��
����ֵ:		��
��	־:		2008-09-22����������������ʵ��
**********************************************************************/
void GuiTxUninit();

/**********************************************************************
������:		GuiTxOpen()
��	��:		��һ�����ݴ���ͨ��
�Ρ���:		_uiIp:		����IP��ַ
			_uiPort:	�����˿ں�
����ֵ:		������ʧ�ܣ�����ֵΪ����ͨ��������
��	־:		2008-09-16����������������ʵ��
			2008-92-22������������Ӳ���ip��port��
**********************************************************************/
int GuiTxOpen(unsigned int _uiIp, int _iPort);

/**********************************************************************
������:		GuiTxWaitConnector()
��	��:		�ȴ��Ѵ򿪵Ĵ���ͨ�����Է����룬�������GuiOpen���漴����
			�ú���
�Ρ���:		iTid:		����ͨ�����
			iTimeOut:	��ʱʱ�䣬��
����ֵ:		��
��	־:		2008-92-22������������Ӳ���ip��port��
***********************************************************************/
void GuiTxWaitConnector(int _iTid, int _iTimeOut);


/**********************************************************************
������:		GuiTxWaitSeverStart()
��	��:		�ȴ��Է���ָ����IP��Port�ϴ�������ͨ�����������GuiTxConnect
			����ǰ���ô˽ӿ�
�Ρ���:		_iPort:		�˿�
			_uiIp:		ip��ַ
			_iTimeOut:	��ʱʱ��,��
����ֵ:		��
��	־:		2008-92-22������������Ӳ���ip��port��
***********************************************************************/
void GuiTxWaitServerStart(unsigned int _uiIp, int _iPort, int _iTimeOut);

/**********************************************************************
������:		GuiTxConnect()
��	��:		��һ�����ݴ���ͨ��
�Ρ���:		_uiIp:		�Է�IP��ַ
			_uiPort:	�Է��˿ں�
����ֵ:		������ʧ�ܣ�����ֵΪ����ͨ��������
��	־:		2008-09-22����������������ʵ��
**********************************************************************/
int GuiTxConnect(unsigned int _uiIp, int _iPort);

/**********************************************************************
������:		GuiTxReConnect()
��	��:		��һ�����ݴ���ͨ��(��������)
�Ρ���:		_uiIp:		�Է�IP��ַ
			_uiPort:	�Է��˿ں�
����ֵ:		������ʧ�ܣ�����ֵΪ����ͨ��������
��	־:		2008-09-22����������������ʵ��
**********************************************************************/
int GuiTxReConnect(unsigned int _uiIp, int _iPort);

/**********************************************************************
������:		GuiTxClose()
��	��:		�ر�ָ�����Ѵ򿪴���ͨ��
�Ρ���:		_iTid:		����ͨ��������
����ֵ:		��
��	־:		2008-09-16����������������ʵ��
**********************************************************************/
void GuiTxClose(int _iTid);

/**********************************************************************
������:		GuiTxSend()
��	��:		ͨ����������������
�Ρ���:		_iTid:		����ͨ�����
			_pSndBuf:	���������׵�ַ
			_iSize:		�������ݴ�С
			_iTimeOut:	��ʱ���ã���ΪGUI_TX_NO_WAIT, GUI_TX_WAIT_DONE��
						������ʽ��Ӧ��������ʱ
����ֵ:		������ʧ�ܣ�����ֵ��ʾʵ�ʴ����������
��	־:		2008-09-16����������������ʵ��
**********************************************************************/
int GuiTxSend(int _iTid, const void *_pSndBuf, int _iSize, int _iTimeOut);

/**********************************************************************
������:		GuiTxRcv()
��	��:		�Ӵ�������ȡ����
�Ρ���:		_iTid:		����ͨ�����
			_pRcvBuf:	���ջ����׵�ַ
			_iSize:		�������ݴ�С
			_iTimeOut:	��ʱ���ã���ΪGUI_TX_NO_WAIT, GUI_TX_WAIT_DONE��
						������ʽ��Ӧ��������ʱ
����ֵ:		������ʧ�ܣ�����ֵ��ʾʵ�ʶ�ȡ��������
��	־:		2008-09-16����������������ʵ��
**********************************************************************/
int GuiTxRcv(int _iTid, void *_pRcvBuf, int _iSize, int _iTimeOut);

/**********************************************************************
������:		GuiTxReset(int _iTid)
��	��:		���贫��ͨ��״̬
�Ρ���:		_iTid:		����ͨ�����
����ֵ:		��
��	־:		2008-09-16����������������ʵ��
**********************************************************************/
void GuiTxReset(int _iTid);

#endif

