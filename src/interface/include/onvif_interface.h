/*
 * 	file: onvif_interface.h
 *	description: onvif interface
 *	history: dongliqiang created at 20120229
 */

#ifndef __ONVIF_INTERFACE_H__
#define __ONVIF_INTERFACE_H__

#include "onvif_sdk.h"

/**************************************************
*功能:		多播发送hello消息
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
int SendHelloToClient(void *_pThis);

/**************************************************
*功能:		多播发送Bye消息
*参数:		_pThis:		全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
int SendByeToClient(void *_pThis);

/**************************************************
*功能:		验证用户信息
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120301 v1.0 creat by ptb
**************************************************/
int VerifyUserPsw(struct soap* ptSoap);

/**************************************************
*功能:		设置错误信息
*参数:		_ptSoap:		tcp soap
			_pcReason:		错误原因
			_pcSubStr:		subcode
			_pcSubSubStr:	subsubcode
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120301 v1.0 creat by ptb
**************************************************/
int soap_sender_fault_info(struct soap *_ptSoap, char *_pcReason, char *_pcSubStr, char *_pcSubSubStr);

/**************************************************
*功能:		设置错误信息
*参数:		_ptSoap:		tcp soap
			_pcReason:		错误原因
			_pcSubStr:		subcode
			_pcSubSubStr:	subsubcode
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120301 v1.0 creat by ptb
**************************************************/
int soap_receiver_fault_info(struct soap *_ptSoap, char *_pcReason, char *_pcSubStr, char *_pcSubSubStr);

/**************************************************
*功能:		检测消费者终止时间并发送Notify消息
*参数:		_pThis:			全局变量
			_pTopicSet:		订阅的主题
*返回值:	
*修改历史:
	20120325 v1.0 creat by ptb
	20131219 modified by qiaohaijun
**************************************************/
int NotifyConsumer();

/**************************************************
*功能:		增加用户信息
*参数:		_pThis			:			全局变量
			_tCreateTime	:			订阅时间
			_tTerminateTime	:			终止时间
			_pcEndPoint		:			订阅服务的地址
			_iConsumerId	:			消费者Index
			_iTopicMask 	:			订阅主题
			_iChnMask		:			当前主题通道号
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120325 v1.0 creat by ptb
**************************************************/
int AddConsumer(time_t _tCreateTime, time_t _tTerminateTime, const char *_pcEndPoint, int _iConsumerId, int _iTopicMask, int _iChnMask);

/**************************************************
*功能:		删除消费者信息
*参数:		_pThis			:			全局变量
			_pcTopicSet		:			订阅主题
			_iConsumerId	:			消费者Index
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120325 v1.0 creat by ptb
**************************************************/
int DeleteConsumer(int _iConsumerId);

int GetConsumerId(const char *_pcEndPoint);

int GetConsumerId_Ex(ConsumerInfo *_ptInfo);

/**************************************************
*功能:		获取空闲Index
*参数:		_pcEndPoint		:		消费者EndPoint
*返回值:	成功:消费者Index号 , 失败: <0
*修改历史:
	20120325 v1.0 creat by ptb
**************************************************/
int GetIdleId(void *_pThis);

/**************************************************
*功能:		查询链表中是否存在此_iConsumerId
*参数:		_pThis			:		全局变量
			_iConsumerId	:		需要查找的Index
*返回值:	成功:0 , 失败: -1
*修改历史:
	20120326 v1.0 creat by ptb
**************************************************/
int IsConsumer(int _iConsumerId);

/**************************************************
*功能:		更新消费者终止时间
*参数:		_pThis		    :			全局变量
			_iConsumerId	:			消费者Index
			_tTerminateTime	:			终止时间
			_iFlag			:			终止时间格式标识，
										1:在原时间基础上加上延时	
										0:给出明确的终止时间
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120325 v1.0 creat by ptb
**************************************************/
int UpdateConsumerTermTime(int _iConsumerId, time_t _tTerminateTime, int _iFlag);

/**************************************************
*功能:		验证用户信息
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120301 v1.0 creat by ptb
**************************************************/
int VerifyUserPsw(struct soap* ptSoap);

/**************************************************
*功能:		计时器线程函数
*参数:		_pThis:		全局变量
*返回值:	无
*修改历史:
	20120314 v1.0 creat by ptb
**************************************************/
void *ThreadTimer(void *_pThis);

/**************************************************
*功能:		设置高清参数(onvifv10版本)
*参数:		_iChan			:		通道号
                         _ptImgSetting  	:		图像参数结构体
*返回值:	0: 成功, !0: 失败
*修改历史:
	20130130 v2.0 creat by ptb
	20130219 v2.0 modify by ptb
**************************************************/
int SetIspParameter10(int _iChan, struct tt__ImagingSettings *_ptImgSetting);

/**************************************************
*功能:		设置高清参数(onvifv20版本)
*参数:		_iChan			:		通道号
                         _ptImgSetting  	:		图像参数结构体
*返回值:	0: 成功, !0: 失败
*修改历史:
	20130130 v2.0 creat by ptb
	20130219 v2.0 modify by ptb
**************************************************/
int SetIspParameter20(int _iChan, struct tt__ImagingSettings20 *_ptImgSetting);

/**************************************************
*功能:		设置视频输入参数(亮度、对比度、饱和度)
*参数:		_iChan			:		通道号
				_fBrightness		:		亮度
				_fContrast		:		对比度
				_fColorSaturation	:		色彩饱和度
*返回值:	0: 成功, !0: 失败
*修改历史:
	20130130 v2.0 creat by ptb
	20130219 v2.0 modify by ptb
**************************************************/
int SetVideoInputParameter(int _iChan, float _fBrightness, float _fContrast, float _fColorSaturation);

/**************************************************
*功能:		设置视频编码参数
*参数:		_iChan			:		通道号
				_ptConfiguration	:		视频编码结构体
*返回值:	0: 成功, !0: 失败
*修改历史:
	20130131 v2.0 creat by ptb
**************************************************/
int SetVideoEncodeParameter(int _iChan, struct tt__VideoEncoderConfiguration *_ptConfiguration);

/**************************************************
*功能:		匹配视频大小
*参数:		_piVideoSize		:	视频大小
				_ptResolution	:	分辨率宽、高
*返回值:	0		:	成功
				非0	:	失败
*修改历史:
			20130131 v2.0 creat by ptb
**************************************************/
int MatchVideoSize(int *_piVideoSize, struct tt__VideoResolution *_ptResolution);

/**************************************************
*功能:		获取共享内存更新高清参数，更新ONVIF全局变量
*参数:		_pThis	:	全局变量结构体指针
            _iChn	:	通道号	
*返回值:	0: 成功, !0: 失败
*修改历史:20130129 v2.0 creat by ptb
**************************************************/
int UpdateIspInfo();

/**************************************************
*功能:		获取共享内存视频参数，更新ONVIF全局变量
*参数:		_pThis	:		全局变量结构体指针
            _iChan	:		指定更新的通道
*返回值:	0: 成功, !0: 失败
*修改历史:20130129 v2.0 creat by ptb
**************************************************/
int UpdateVideoInfo(OnvifCtrl *_pThis, int _iChan);

/**************************************************
*功能:		获取共享内存视频参数，更新ONVIF全局变量
*参数:		_pThis	:		全局变量结构体指针
*返回值:	0: 成功, !0: 失败
*修改历史:	20130129 v2.0 creat by ptb
**************************************************/
int UpdateVideoInfoAll(OnvifCtrl *_pThis);

/**************************************************
*功能:		根据速度值确定PTZ方向
*参数:		_iSpeedX	:	X方向速度
            _iSpeedY	:	Y方向速度
*返回值:	成功		:	PTZ控制码
            失败		:	-1
*修改历史:20130222 v2.3 creat by ptb
**************************************************/
int GetPTZCode(int _iSpeedX, int _iSpeedY);

/**************************************************
*功能:		判断设备当前设备是否需要重启，
            兼容老设备修改分辨率等参数需要重启
*参数:		无
*返回值:	成功 :  0
            失败 : !0
*修改历史:20130320 v2.3.0 creat by ptb
**************************************************/
int VerifyDeviceRebootFlag();

/**************************************************
*功能:		获取当前空闲预置位号
*参数:		无
*返回值:	成功	:	预置位号
			      失败	:	-1
*修改历史:   20130425 v2.3.0 creat by ptb
**************************************************/
int GetIdlePresetNum();

void AlarmDetect();

time_t TdTime(time_t* _ptTimer);

int PaddingMessage(int _EventType, int _iChn, int _iTopicState, void *_pNotifyMsg, int *_piSize);

int ClearPullMsgInfo(void);

void GetTimeZoneOffset(char *_pcTimeZone);


#endif
 
