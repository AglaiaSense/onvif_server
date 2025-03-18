/*
 * 	file: onvif_interface.h
 *	description: onvif interface
 *	history: dongliqiang created at 20120229
 */

#ifndef __ONVIF_INTERFACE_H__
#define __ONVIF_INTERFACE_H__

#include "onvif_sdk.h"

/**************************************************
*����:		�ಥ����hello��Ϣ
*����:		_ptSoap:		tcp soap
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
int SendHelloToClient(void *_pThis);

/**************************************************
*����:		�ಥ����Bye��Ϣ
*����:		_pThis:		ȫ�ֱ���g_stOnvifCtrl
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
int SendByeToClient(void *_pThis);

/**************************************************
*����:		��֤�û���Ϣ
*����:		_ptSoap:		tcp soap
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120301 v1.0 creat by ptb
**************************************************/
int VerifyUserPsw(struct soap* ptSoap);

/**************************************************
*����:		���ô�����Ϣ
*����:		_ptSoap:		tcp soap
			_pcReason:		����ԭ��
			_pcSubStr:		subcode
			_pcSubSubStr:	subsubcode
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120301 v1.0 creat by ptb
**************************************************/
int soap_sender_fault_info(struct soap *_ptSoap, char *_pcReason, char *_pcSubStr, char *_pcSubSubStr);

/**************************************************
*����:		���ô�����Ϣ
*����:		_ptSoap:		tcp soap
			_pcReason:		����ԭ��
			_pcSubStr:		subcode
			_pcSubSubStr:	subsubcode
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120301 v1.0 creat by ptb
**************************************************/
int soap_receiver_fault_info(struct soap *_ptSoap, char *_pcReason, char *_pcSubStr, char *_pcSubSubStr);

/**************************************************
*����:		�����������ֹʱ�䲢����Notify��Ϣ
*����:		_pThis:			ȫ�ֱ���
			_pTopicSet:		���ĵ�����
*����ֵ:	
*�޸���ʷ:
	20120325 v1.0 creat by ptb
	20131219 modified by qiaohaijun
**************************************************/
int NotifyConsumer();

/**************************************************
*����:		�����û���Ϣ
*����:		_pThis			:			ȫ�ֱ���
			_tCreateTime	:			����ʱ��
			_tTerminateTime	:			��ֹʱ��
			_pcEndPoint		:			���ķ���ĵ�ַ
			_iConsumerId	:			������Index
			_iTopicMask 	:			��������
			_iChnMask		:			��ǰ����ͨ����
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120325 v1.0 creat by ptb
**************************************************/
int AddConsumer(time_t _tCreateTime, time_t _tTerminateTime, const char *_pcEndPoint, int _iConsumerId, int _iTopicMask, int _iChnMask);

/**************************************************
*����:		ɾ����������Ϣ
*����:		_pThis			:			ȫ�ֱ���
			_pcTopicSet		:			��������
			_iConsumerId	:			������Index
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120325 v1.0 creat by ptb
**************************************************/
int DeleteConsumer(int _iConsumerId);

int GetConsumerId(const char *_pcEndPoint);

int GetConsumerId_Ex(ConsumerInfo *_ptInfo);

/**************************************************
*����:		��ȡ����Index
*����:		_pcEndPoint		:		������EndPoint
*����ֵ:	�ɹ�:������Index�� , ʧ��: <0
*�޸���ʷ:
	20120325 v1.0 creat by ptb
**************************************************/
int GetIdleId(void *_pThis);

/**************************************************
*����:		��ѯ�������Ƿ���ڴ�_iConsumerId
*����:		_pThis			:		ȫ�ֱ���
			_iConsumerId	:		��Ҫ���ҵ�Index
*����ֵ:	�ɹ�:0 , ʧ��: -1
*�޸���ʷ:
	20120326 v1.0 creat by ptb
**************************************************/
int IsConsumer(int _iConsumerId);

/**************************************************
*����:		������������ֹʱ��
*����:		_pThis		    :			ȫ�ֱ���
			_iConsumerId	:			������Index
			_tTerminateTime	:			��ֹʱ��
			_iFlag			:			��ֹʱ���ʽ��ʶ��
										1:��ԭʱ������ϼ�����ʱ	
										0:������ȷ����ֹʱ��
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120325 v1.0 creat by ptb
**************************************************/
int UpdateConsumerTermTime(int _iConsumerId, time_t _tTerminateTime, int _iFlag);

/**************************************************
*����:		��֤�û���Ϣ
*����:		_ptSoap:		tcp soap
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120301 v1.0 creat by ptb
**************************************************/
int VerifyUserPsw(struct soap* ptSoap);

/**************************************************
*����:		��ʱ���̺߳���
*����:		_pThis:		ȫ�ֱ���
*����ֵ:	��
*�޸���ʷ:
	20120314 v1.0 creat by ptb
**************************************************/
void *ThreadTimer(void *_pThis);

/**************************************************
*����:		���ø������(onvifv10�汾)
*����:		_iChan			:		ͨ����
                         _ptImgSetting  	:		ͼ������ṹ��
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20130130 v2.0 creat by ptb
	20130219 v2.0 modify by ptb
**************************************************/
int SetIspParameter10(int _iChan, struct tt__ImagingSettings *_ptImgSetting);

/**************************************************
*����:		���ø������(onvifv20�汾)
*����:		_iChan			:		ͨ����
                         _ptImgSetting  	:		ͼ������ṹ��
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20130130 v2.0 creat by ptb
	20130219 v2.0 modify by ptb
**************************************************/
int SetIspParameter20(int _iChan, struct tt__ImagingSettings20 *_ptImgSetting);

/**************************************************
*����:		������Ƶ�������(���ȡ��Աȶȡ����Ͷ�)
*����:		_iChan			:		ͨ����
				_fBrightness		:		����
				_fContrast		:		�Աȶ�
				_fColorSaturation	:		ɫ�ʱ��Ͷ�
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20130130 v2.0 creat by ptb
	20130219 v2.0 modify by ptb
**************************************************/
int SetVideoInputParameter(int _iChan, float _fBrightness, float _fContrast, float _fColorSaturation);

/**************************************************
*����:		������Ƶ�������
*����:		_iChan			:		ͨ����
				_ptConfiguration	:		��Ƶ����ṹ��
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20130131 v2.0 creat by ptb
**************************************************/
int SetVideoEncodeParameter(int _iChan, struct tt__VideoEncoderConfiguration *_ptConfiguration);

/**************************************************
*����:		ƥ����Ƶ��С
*����:		_piVideoSize		:	��Ƶ��С
				_ptResolution	:	�ֱ��ʿ���
*����ֵ:	0		:	�ɹ�
				��0	:	ʧ��
*�޸���ʷ:
			20130131 v2.0 creat by ptb
**************************************************/
int MatchVideoSize(int *_piVideoSize, struct tt__VideoResolution *_ptResolution);

/**************************************************
*����:		��ȡ�����ڴ���¸������������ONVIFȫ�ֱ���
*����:		_pThis	:	ȫ�ֱ����ṹ��ָ��
            _iChn	:	ͨ����	
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:20130129 v2.0 creat by ptb
**************************************************/
int UpdateIspInfo();

/**************************************************
*����:		��ȡ�����ڴ���Ƶ����������ONVIFȫ�ֱ���
*����:		_pThis	:		ȫ�ֱ����ṹ��ָ��
            _iChan	:		ָ�����µ�ͨ��
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:20130129 v2.0 creat by ptb
**************************************************/
int UpdateVideoInfo(OnvifCtrl *_pThis, int _iChan);

/**************************************************
*����:		��ȡ�����ڴ���Ƶ����������ONVIFȫ�ֱ���
*����:		_pThis	:		ȫ�ֱ����ṹ��ָ��
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:	20130129 v2.0 creat by ptb
**************************************************/
int UpdateVideoInfoAll(OnvifCtrl *_pThis);

/**************************************************
*����:		�����ٶ�ֵȷ��PTZ����
*����:		_iSpeedX	:	X�����ٶ�
            _iSpeedY	:	Y�����ٶ�
*����ֵ:	�ɹ�		:	PTZ������
            ʧ��		:	-1
*�޸���ʷ:20130222 v2.3 creat by ptb
**************************************************/
int GetPTZCode(int _iSpeedX, int _iSpeedY);

/**************************************************
*����:		�ж��豸��ǰ�豸�Ƿ���Ҫ������
            �������豸�޸ķֱ��ʵȲ�����Ҫ����
*����:		��
*����ֵ:	�ɹ� :  0
            ʧ�� : !0
*�޸���ʷ:20130320 v2.3.0 creat by ptb
**************************************************/
int VerifyDeviceRebootFlag();

/**************************************************
*����:		��ȡ��ǰ����Ԥ��λ��
*����:		��
*����ֵ:	�ɹ�	:	Ԥ��λ��
			      ʧ��	:	-1
*�޸���ʷ:   20130425 v2.3.0 creat by ptb
**************************************************/
int GetIdlePresetNum();

void AlarmDetect();

time_t TdTime(time_t* _ptTimer);

int PaddingMessage(int _EventType, int _iChn, int _iTopicState, void *_pNotifyMsg, int *_piSize);

int ClearPullMsgInfo(void);

void GetTimeZoneOffset(char *_pcTimeZone);


#endif
 
