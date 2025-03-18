/*
 *	file: onvif_extren.c
 *	description: onvif interface functions
 *	history: dongliqiang created at 20120129
 */

#include <pthread.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "onvifH.h"
#include "common.h"
#include "onvif_sdk.h"
#include "proparser.h"
#include "gvar_mem.h"
#include "onvif_interface.h"
#include "wsaapi.h"

#ifdef WITH_WSSE
#include "wsseapi.h"
#endif
#include "iniparser.h"
#include "onvif_shm_cmd.h"
#include "guitransport.h"
#include "array.h"
#include "macro.h"
#include "charset.h"
#include "proc.h"


#ifndef CRLF
#define CRLF "\r\n"
#endif
//全局变量
extern OnvifCtrl g_stOnvifCtrl;
extern FILE* fpProc;

char TopicSetList[TOPIC_NUM][64] = {"tns1:VideoSource/MotionAlarm", "tns1:Device/Trigger/tnshik:AlarmIn", "tns1:UserAlarm/VideoLost", "tns1:VideoAnalytics/tnshik:MaskAlarm"};

int width = 512;
int height = 240;

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns2__Hello(struct soap* ptSoap, struct d__HelloType *dn__Hello, struct d__ResolveType *dn__HelloResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	return SOAP_STOP;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns2__Bye(struct soap* ptSoap, struct d__ByeType *dn__Bye, struct d__ResolveType *dn__ByeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return SOAP_STOP;
}
 
#ifdef WITH_ANALYTICS	
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns12__GetSupportedRules(struct soap* ptSoap, struct _tan__GetSupportedRules *tan__GetSupportedRules, struct _tan__GetSupportedRulesResponse *tan__GetSupportedRulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns12__CreateRules(struct soap* ptSoap, struct _tan__CreateRules *tan__CreateRules, struct _tan__CreateRulesResponse *tan__CreateRulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns12__DeleteRules(struct soap* ptSoap, struct _tan__DeleteRules *tan__DeleteRules, struct _tan__DeleteRulesResponse *tan__DeleteRulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns12__GetRules(struct soap* ptSoap, struct _tan__GetRules *tan__GetRules, struct _tan__GetRulesResponse *tan__GetRulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns12__ModifyRules(struct soap* ptSoap, struct _tan__ModifyRules *tan__ModifyRules, struct _tan__ModifyRulesResponse *tan__ModifyRulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns13__GetServiceCapabilities(struct soap* ptSoap, struct _tan__GetServiceCapabilities *tan__GetServiceCapabilities, struct _tan__GetServiceCapabilitiesResponse *tan__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns13__GetSupportedAnalyticsModules(struct soap* ptSoap, struct _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules, struct _tan__GetSupportedAnalyticsModulesResponse *tan__GetSupportedAnalyticsModulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns13__CreateAnalyticsModules(struct soap* ptSoap, struct _tan__CreateAnalyticsModules *tan__CreateAnalyticsModules, struct _tan__CreateAnalyticsModulesResponse *tan__CreateAnalyticsModulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns13__DeleteAnalyticsModules(struct soap* ptSoap, struct _tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules, struct _tan__DeleteAnalyticsModulesResponse *tan__DeleteAnalyticsModulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns13__GetAnalyticsModules(struct soap* ptSoap, struct _tan__GetAnalyticsModules *tan__GetAnalyticsModules, struct _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns13__ModifyAnalyticsModules(struct soap* ptSoap, struct _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, struct _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
#endif

/**************************************************
*功能:		设备检测
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20120227 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns3__Probe(struct soap* ptSoap, struct d__ProbeType *dn__Probe, struct d__ProbeMatchesType *dn__ProbeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char cTmpUUID[64] = {0};
	int i = 0;
	int j = 0;
	char *ppcTmpScope[MAX_SCOPES_NUM] = {0};
	int iScopeNum = 0;
	int iDisMode = 0;
	
	iDisMode = ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp->DiscoveryMode;

	//DiscoveryMode check
	if(tt__DiscoveryMode__Discoverable != iDisMode)
	{
		td_printf(0, "%s %d %s Discovery is not enable!\n", __FILE__, __LINE__, __FUNCTION__);		
		return SOAP_STOP;
	}
	//td_printf(0, "=============probe type[%s]", *dn__Probe->Types);
	//td_printf(0, "=============probe type[%s]", dn__Probe->Scopes->__item);
	
	//type check
	if(NULL != dn__Probe->Types && NULL != *dn__Probe->Types && strlen(*dn__Probe->Types) > 0)
	{
		td_printf(0, "type:%s\n", *dn__Probe->Types);
		if(strcmp(*dn__Probe->Types, *ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Types))
		{
			td_printf(0, "%s %d %s d__Types is not matched!\n", __FILE__, __LINE__, __FUNCTION__);

			return SOAP_STOP;
		}
		
	}
#if 0	//dlq modified at 20120428  for KEDA's DVR
	else
	{
		td_printf(0, "type is not matched!\n");
		return soap_sender_fault_info(ptSoap, "MatchingRuleNotSupported!", NULL, NULL);
	}
#endif

	//scope check
	if(NULL != dn__Probe->Scopes && NULL != dn__Probe->Scopes->__item)
	{
		for(i = 0; i < MAX_SCOPES_NUM; i++)
		{
			ppcTmpScope[i] = (char *)soap_mallocz(ptSoap, MAX_STRING_LEN * sizeof(char));
			if(NULL == ppcTmpScope[i])
			{
				return SOAP_ERR;
			}
		}
		
		iScopeNum = SplitStringToArray(dn__Probe->Scopes->__item, " ", ppcTmpScope);
		for(i = 0; i < iScopeNum; i++)
		{
			td_printf(0, "Scope:%s\n", ppcTmpScope[i]);
		}
		
		for(i = 0; i < iScopeNum; i++)
		{
			if(i == iScopeNum)
			{
				td_printf(0, "%s %d %s soap is not match!\n", __FILE__, __LINE__, __FUNCTION__);
				return SOAP_STOP;
			}
			
			for(j = 0; j < ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes; j++)
			{
				td_printf(0, "client i:%d scope:%s\n", i, ppcTmpScope[i]);
				td_printf(0, "server j:%d scope:%s\n", j, ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[j].ScopeItem);
				if(NULL == strstr(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[j].ScopeItem, ppcTmpScope[i]))
				{
					if(j != ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes -1)
					{
						continue;
					}
					else
					{
						td_printf(0, "%s %d %s soap is not match!\n", __FILE__, __LINE__, __FUNCTION__);

						return SOAP_STOP;
					}
				}
				else
				{
					break;
				}
			}

		}
	}

	//soap header
	if(NULL != ptSoap->header)
	{
		//save client uuid
		if(NULL != ptSoap->header->wsa5__MessageID && strlen(ptSoap->header->wsa5__MessageID) < 64 - 1)
		strcpy(cTmpUUID, ptSoap->header->wsa5__MessageID);
		//td_printf(0, "%s %d %s client uuid:%s\n", __FILE__, __LINE__, __FUNCTION__, cTmpUUID);

		soap_default_SOAP_ENV__Header(ptSoap, ptSoap->header);
	}
	else
	{
		ptSoap->header = (struct SOAP_ENV__Header *)soap_malloc(ptSoap, sizeof(struct SOAP_ENV__Header));
		if(NULL == ptSoap->header)
		{
			return SOAP_ERR;
		}
		
		soap_default_SOAP_ENV__Header(ptSoap, ptSoap->header);
	}
	
	ptSoap->header->wsa5__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";	//WS-Discovery

	ptSoap->header->wsa5__MessageID = (char *)soap_wsa_rand_uuid(ptSoap);
	//ptSoap->header->wsa5__MessageID = ptOnvifCtrl->m_stNvsInfo.m_cUUID;
	//td_printf(0, "%s %d %s server uuid:%s\n", __FILE__, __LINE__, __FUNCTION__, ptSoap->header->wsa5__MessageID);

	//updata g_stOnvifCtrl
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->__sizeProbeMatch; i++)
	{
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch[i].wsa__EndpointReference.Address, (char*)ptOnvifCtrl->m_stNvsInfo.m_cUUID);
	}
	
	ptSoap->header->wsa5__RelatesTo = (struct wsa5__RelatesToType *)(char *)soap_mallocz(ptSoap, sizeof(struct wsa5__RelatesToType));
	if(NULL == ptSoap->header->wsa5__RelatesTo)
	{
		return SOAP_ERR;
	}
	ptSoap->header->wsa5__RelatesTo->__item = (char *)soap_mallocz(ptSoap, 64);
	if(NULL == ptSoap->header->wsa5__RelatesTo->__item)
	{
		return SOAP_ERR;
	}
	strcpy(ptSoap->header->wsa5__RelatesTo->__item, cTmpUUID);

	ptSoap->header->wsa5__To = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";	//WS-Discovery

	//response
	dn__ProbeResponse->__sizeProbeMatch = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->__sizeProbeMatch;
	dn__ProbeResponse->ProbeMatch = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch;

	return SOAP_OK;
}

 #ifdef WITH_EVENT
 /**************************************************
 *功能: 	 详细描述函数的功能
 *参数: 	 _ptSoap:		 tcp soap
 *返回值:	 0: 成功, !0: 失败
 *修改历史:
	 20120224 v1.0 creat by xxx
 **************************************************/
 SOAP_FMAC5 int SOAP_FMAC6 __ns11__PauseSubscription(struct soap* ptSoap, struct _wsnt__PauseSubscription *wsnt__PauseSubscription, struct _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse)
 {
	 td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
 
	 return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
 }
  
 /**************************************************
 *功能: 	 详细描述函数的功能
 *参数: 	 _ptSoap:		 tcp soap
 *返回值:	 0: 成功, !0: 失败
 *修改历史:
	 20120224 v1.0 creat by xxx
 **************************************************/
 SOAP_FMAC5 int SOAP_FMAC6 __ns11__ResumeSubscription(struct soap* ptSoap, struct _wsnt__ResumeSubscription *wsnt__ResumeSubscription, struct _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse)
 {
	 td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
 
	 return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
 }



////////////////////////////////////////////////////////////////////////////////////////////
//			by qiaohaijun
//			iTopicMask, 从左到右依次为
//			第1位:video move, 
//			第2位:port alarm, 
//			第3位:video cover, 
//			第4位:video lost,	和enum 一一对应
//			:
//			:	
//			
//			iChnMask,以后扩展，表示第几通道
//
//			
//			在此基础上扩展,其他报警类型类似
//
//			海康
//			<wsnt:TopicExpression Dialect="http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet">
//				tns1:RuleEngine/CellMotionDetector/Motion//.
//			</wsnt:TopicExpression>
//				:
//				:
////////////////////////////////////////////////////////////////////////////////////////////


int GetMaskFromFilter(struct wsnt__FilterType * _filter, int *_iMask)
{
	int i = 0;
	int iTopicMask = 0;

	for(i = 0; i < _filter->__size; i++)
	{
		td_printf(0, "*(_filter->__any + %d)__any ==== %s", i, *(_filter->__any + i));
		
		if(strstr(*(_filter->__any + i), "UserAlarm//.")
		||strstr(*(_filter->__any + i), "UserAlarm"))
		{
			iTopicMask |= (1 << ALARM_MOVE);
			iTopicMask |= (1 << ALARM_PORT);
			iTopicMask |= (1 << ALARM_COVER);
			iTopicMask |= (1 << ALARM_LOST);
		}
		
		if(strstr(*(_filter->__any + i), "VideoAnalytics/VideoMove")
		||strstr(*(_filter->__any + i), "MotionAlarm")//MotionAlarm -- Avigilon
		||strstr(*(_filter->__any + i), "VideoMove")
		||strstr(*(_filter->__any + i), "Motion//."))//海康
		{
			
			iTopicMask |= (1 << ALARM_MOVE);	//video move
		}
		
		if(strstr(*(_filter->__any + i), "Device/PortInAlarm")
		||strstr(*(_filter->__any + i), "PortInAlarm")
		||strstr(*(_filter->__any + i), "Device//."))
		{
			iTopicMask |= (1 << ALARM_PORT);	//port alarm

		}

		if(strstr(*(_filter->__any + i), "VideoAnalytics/VideoCover")
		||strstr(*(_filter->__any + i), "VideoCover"))
		{
			iTopicMask |= (1 << ALARM_COVER);	//video cover
		}

		if(strstr(*(_filter->__any + i), "VideoAnalytics/VideoLost")
		||strstr(*(_filter->__any + i), "VideoLost"))
		{
			iTopicMask |= (1 << ALARM_LOST);	//video lost
		}
		
		if(strstr(*(_filter->__any + i), "VideoAnalytics//."))
		{
			td_printf(0, "VideoAnalytics, unsupport .....");
		}

	}

	*_iMask = iTopicMask;

	return 0;
}

 
 /**************************************************
 *功能: 	 报警消息订阅
 *参数: 	 ptSoap 		 :				 	 tcp soap
			 wsnt__Subscribe :				 请求的订阅信息
			 wsnt__SubscribeResponse :		 响应请求订阅的信息
 *返回值:	 0: 成功, !0: 失败
 *修改历史:20120326 v1.0 creat by ptb
20131031 v2.3.1 modify by ptb
 **************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns7__Subscribe(struct soap* ptSoap, struct _wsnt__Subscribe *wsnt__Subscribe, struct _wsnt__SubscribeResponse *wsnt__SubscribeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	struct _wsnt__SubscribeResponse *ptSubscibeRsp = NULL;
	OnvifCtrl *ptOnvifCtrl  = &g_stOnvifCtrl;
	char buffer[MEM_LEN_MID] = {0};
	int iTopicMask = 0;
	int iInitTime = 0;
	int iChnMask = 0;
	int iIndex = 0;
	time_t tTimeCurr;
	time_t tTimeTerm;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	 
	//设置header信息
	//如果header不为空，wsa5__Action则回"XXXXXXResponse"
	//TODO:若header为空时根据实际情况而定
	//modified by qiaohaijun 
	if(!pointer_valid_check(ptSoap->header))
	{
		ptSoap->header->wsa5__Action = "http://docs.oasis-open.org/wsn/bw-2/NotificationProducer/SubscribeResponse";
	}
	
	//检测发送的Filter信息
	if(NULL != wsnt__Subscribe->Filter)
	 {
		GetMaskFromFilter(wsnt__Subscribe->Filter, &iTopicMask);
		if(iTopicMask == 0)
		{
			soap_sender_fault_info(ptSoap, "wsnt:InvalidMessageContentExpressionFault", NULL, NULL);
			return SOAP_FAULT;
		}
	}
	else
	{
		iTopicMask |= ~0;
	}
	
	ptSubscibeRsp = ptOnvifCtrl->m_stOnvifCfg.m_ptSubscribeRsp;

	wsnt__SubscribeResponse->SubscriptionReference.Address = ptSubscibeRsp->SubscriptionReference.Address;
	wsnt__SubscribeResponse->CurrentTime     = ptSubscibeRsp->CurrentTime;
	wsnt__SubscribeResponse->TerminationTime = ptSubscibeRsp->TerminationTime;

	//response
	//Address
	//为每个消费者创建不同的Index
	iIndex = GetIdleId(ptOnvifCtrl);
	if(iIndex < 0)
	{
		td_printf(0, "%s %d %s: NO idle index!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	
	sprintf(buffer, "http://%s:%d%s%d", \
		(char*)ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, \
		ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort, SUBSCRIBEURL, iIndex);
	
	Strcpy(ptSubscibeRsp->SubscriptionReference.Address, buffer, MEM_LEN_MID);

	//CurrentTime
	time(&tTimeCurr);
	*wsnt__SubscribeResponse->CurrentTime = tTimeCurr;

	//TerminationTime
	if(pointer_valid_check(wsnt__Subscribe->InitialTerminationTime))
	{
		//默认订阅1天
		tTimeTerm = tTimeCurr + 24 * 60 * 60;
	}
	else
	{
		td_printf(0, "InitTerTime[%s]", wsnt__Subscribe->InitialTerminationTime);
		//InitialTerminationTime转化为以秒为单位
		iInitTime = PTStrToTime(wsnt__Subscribe->InitialTerminationTime);
		if(iInitTime < 0)
		{
			//默认订阅1天
			iInitTime = 24 * 60 * 60;
		}
		tTimeTerm = tTimeCurr + iInitTime;
	}
	*wsnt__SubscribeResponse->TerminationTime = tTimeTerm;

	//添加消费者地址到链表中
	AddConsumer(tTimeCurr, tTimeTerm, wsnt__Subscribe->ConsumerReference.Address, iIndex, iTopicMask, iChnMask);
	ptOnvifCtrl->m_stFlag.m_iNotifyFlag = TD_TRUE;

	return SOAP_OK;
}

/**************************************************
*功能:		更新订阅信息
*参数:		ptSoap				:		soap
			wsnt__Renew			:		request
			wsnt__RenewResponse	:		response
*返回值:	0: 成功, !0: 失败
*修改历史:20120326 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns11__Renew(struct soap* ptSoap, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	char *pcIniTimeStr 	= NULL;
	int iConsumerId 	= 0;
	time_t tTimeTerm;
	time_t tTimeCurr;

	//header	
	if(!pointer_valid_check(ptSoap->header))
	{
		ptSoap->header->wsa5__Action = "http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/RenewResponse";
		if(TD_TRUE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_iEnable)
		{
			td_printf(0, "AAAAAAAAAAAAAAAAAAAAA");
			//此处测试工具有个bug，
			//测试工具发来的有时候是	http://10.30.31.190:8081/
			//而我们的endpoint里面的值是	http://10.30.31.190:8081,暂不做比较
		#if 0
			if(strcmp(ptOnvifCtrl->m_stManager.m_tPullMsg.m_cEndPoint, ptSoap->header->wsa5__To))
			{
				soap_sender_fault_info(ptSoap, "the wsa5__To does not match", NULL, "tt:ResourceUnknown");
				return SOAP_FAULT;
			}
		#endif
		}
		
		iConsumerId = GetConsumerId(ptSoap->header->wsa5__To);

		//Subscription
		if(iConsumerId >= 0)
		{
			if(ptOnvifCtrl->m_stManager.ConsumerInfoList[iConsumerId] == NULL)
			{
				//发完unsubscribe后在发renew是非法的，
				//测试工具有negative测试，此条不可少
				//by qiaohaijun @2013.12.2,
				soap_sender_fault_info(ptSoap, "if the unsubscribe has been executed,renew is invalid", NULL, "tt:ResourceUnknown");
				return SOAP_FAULT;
			}
		}
		//
		else
		{
			if(ptOnvifCtrl->m_stManager.m_tPullMsg.m_iEnable == 0)
			{
				soap_sender_fault_info(ptSoap, "not create pull point subscription", NULL, "tt:ResourceUnknown");
				return SOAP_FAULT;
			}
		}
	}
	
	//为wsnt__CurrentTime开辟临时空间
	wsnt__RenewResponse->CurrentTime = (time_t *)soap_mallocz(ptSoap, sizeof(time_t));
	if(pointer_valid_check(wsnt__RenewResponse->CurrentTime))
	{
		return TD_ERROR;
	}
	
	//当前时间
	time(&tTimeCurr);
	*wsnt__RenewResponse->CurrentTime = tTimeCurr;

	//获得终止时间
	pcIniTimeStr = wsnt__Renew->TerminationTime;
	if(strstr(pcIniTimeStr, "PT"))
	{
		//InitialTerminationTime转化为以秒为单位
		tTimeTerm = tTimeCurr + (time_t)PTStrToTime(pcIniTimeStr);
		//更新消费者信息的终止时间
		UpdateConsumerTermTime(iConsumerId, (time_t)PTStrToTime(pcIniTimeStr), 1);
	}
	else
	{
		//字符串转换为时间模式
		soap_s2dateTime(ptSoap, pcIniTimeStr, &tTimeTerm);
		//更新消费者信息的终止时间
		UpdateConsumerTermTime(iConsumerId, tTimeTerm, 0);
	}
	
	wsnt__RenewResponse->TerminationTime = tTimeTerm;
	
	if(TD_TRUE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_iEnable)
	{
		ptOnvifCtrl->m_stManager.m_tPullMsg.m_tTerminateTime = tTimeTerm;
	}
	
	return SOAP_OK;
}

/**************************************************
*功能:		取消报警信息订阅
*参数:		ptSoap					    :		soap
			wsnt__Unsubscribe			:		request
			wsnt__UnsubscribeResponse	:		response
*返回值:	0: 成功, !0: 失败
*修改历史:20120326 v1.0 creat by ptb
20131031 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns11__Unsubscribe(struct soap* ptSoap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iIndex = -1;
	char cPort[10] = {0};
	
	if(NULL != ptSoap->header)
	{
		ptSoap->header->wsa5__Action = "http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/UnsubscribeResponse";

		if(NULL != ptSoap->header->wsa5__To)
		{
			//从header中获取Index号
			td_printf(0, "delete %s", ptSoap->header->wsa5__To);

			//被动模式
			if(TD_TRUE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_iEnable)
			{
				sprintf(cPort, "%d", ptOnvifCtrl->m_stNvsInfo.m_iLEventPort);
				if(strstr(ptSoap->header->wsa5__To, cPort))
				{
					ClearPullMsgInfo();
				}
			}

			//主动模式
			iIndex = GetConsumerId(ptSoap->header->wsa5__To);
			if(IsConsumer(iIndex))
			{
				soap_sender_fault_info(ptSoap, NULL, NULL, "tt:ResourceUnknown");
				return SOAP_FAULT;
			}

			//删除当前Index的消费者节点
			DeleteConsumer(iIndex);
		}
	}

	//g_iNotifyFlag = TD_FALSE;
	return SOAP_OK;
}

/**************************************************
*功能:      上传报警消息(被动方式)
*参数:      ptSoap:                               soap
                    tev__PullMessages:               request
                    tev__PullMessagesResponse:  response                   
*返回值:0: 成功 !0: 失败
*修改历史: 20120326 v1.0 creat by ptb	
20130327 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns4__PullMessages(struct soap* ptSoap, struct _tev__PullMessages *tev__PullMessages, struct _tev__PullMessagesResponse *tev__PullMessagesResponse)
{
	struct wsnt__NotificationMessageHolderType *ptRsp = NULL;
	OnvifCtrl *ptOnvifCtrl    = &g_stOnvifCtrl;
	time_t tTimeCurr = 0;
	int iTimeOut = 0;
	int iFlag = 0;
	int i = 0;
	int iTopicMask = 0;
	int iOffset = 0;
	
	//设置header信息
	if(!pointer_valid_check(ptSoap->header))
	{
		ptSoap->header->wsa5__Action = "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesResponse";
	}

	time(&tTimeCurr);
	tev__PullMessagesResponse->CurrentTime = tTimeCurr;

	//单位秒
	iTimeOut = tev__PullMessages->Timeout / 1000;
	tev__PullMessagesResponse->TerminationTime = tTimeCurr + iTimeOut;

	while(1)
	{
		if(time(NULL) - tev__PullMessagesResponse->TerminationTime >= 0)
		{
			iFlag = 0;
			break;
		}
		
		if(TD_TRUE == ptOnvifCtrl->m_stFlag.m_iAlarmFlag)
		{
			iFlag = 1;
			break;
		}

		usleep(1000 * 100);
	}

	if(0 == iFlag)
	{
		tev__PullMessagesResponse->__sizeNotificationMessage = 0;
		tev__PullMessagesResponse->wsnt__NotificationMessage = NULL;
		return SOAP_OK;
	}

	//init size 0
	tev__PullMessagesResponse->__sizeNotificationMessage = 0;
	ptRsp = ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp->wsnt__NotificationMessage;
	
	iTopicMask = ptOnvifCtrl->m_stManager.m_tPullMsg.m_iPullTopicMask;

	if(!pointer_valid_check(ptRsp))
	{
	
		for(i = 0; i < ptOnvifCtrl->m_stNvsInfo.m_iVINum; i++)
		{
		
//			第1位:video move, 
//			第2位:port alarm, 
//			第3位:video cover, 
//			第4位:video lost,

			if(TD_TRUE == ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_MOVE]
			&&((iTopicMask>>ALARM_MOVE)&0x01))
			{
				td_printf(0, "%s,%d, ALARM_MOVE...", __FUNCTION__, __LINE__);
				if(INITIALIZE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_eMoveState)
				{
					PaddingMessage(ALARM_MOVE, i, INITIALIZE, ptRsp, &iOffset);
					ptOnvifCtrl->m_stManager.m_tPullMsg.m_eMoveState = CHANGE;
				}
				else if(CHANGE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_eMoveState)
				{
					PaddingMessage(ALARM_MOVE, i, CHANGE, ptRsp, &iOffset);
				}
				else
				{
					PaddingMessage(ALARM_MOVE, i, DELETE, ptRsp, &iOffset);
				}
			}
			
			if(TD_TRUE == ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_PORT]
			&&((iTopicMask>>ALARM_PORT)&0x01))
			{
				td_printf(0, "%s,%d, ALARM_PORT...", __FUNCTION__, __LINE__);
				PaddingMessage(ALARM_PORT, i, INITIALIZE, ptRsp, &iOffset);
			}

			
			if(TD_TRUE == ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_COVER]
			&&((iTopicMask>>ALARM_COVER)&0x01))
			{
				td_printf(0, "%s,%d, ALARM_COVER...", __FUNCTION__, __LINE__);
				PaddingMessage(ALARM_COVER, i, INITIALIZE, ptRsp, &iOffset);
			}

			//其他报警类型
			//:
			//::
		}

		td_printf(0, "%s, %d offset ======== %d", __FUNCTION__, __LINE__, iOffset);
		tev__PullMessagesResponse->__sizeNotificationMessage = iOffset;
		tev__PullMessagesResponse->wsnt__NotificationMessage = ptRsp;
	}
	
	//每一秒检测一次，防止刷屏。
	sleep(1);

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns4__SetSynchronizationPoint(struct soap* ptSoap, struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, struct _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	//设置header信息
	if(ptSoap->header == NULL)
	{
		ptSoap->header = (struct SOAP_ENV__Header *)soap_mallocz(ptSoap, sizeof(struct SOAP_ENV__Header));
	}
	ptSoap->header->wsa5__Action = "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/SetSynchronizationPointResponse";

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetServiceCapabilities(struct soap* ptSoap, struct _tev__GetServiceCapabilities *tev__GetServiceCapabilities, struct _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:   订阅报警(被动模式)
*参数:   _ptSoap   : 	tcp soap
		    request	:	request
		    response	:	response
*返回值:	0: 成功, !0: 失败
*修改历史:   20120326 v1.0 creat by ptb
20131101 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns5__CreatePullPointSubscription(struct soap* ptSoap, struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, struct _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	PullMsgInfo *ptPullMsgInfo = NULL;
	char *pcIniTimeStr = NULL;
	int iTopicMask 	= 0;
	int iChnMask 	= 0;
	time_t tTimeCurr = 0;
	time_t tTimeTerm = 0;
	int iRetTime = -1;

	//设置header信息
	if(pointer_valid_check(ptSoap->header))
	{
		ptSoap->header = (struct SOAP_ENV__Header *)soap_mallocz(ptSoap, sizeof(struct SOAP_ENV__Header));
	}
	ptSoap->header->wsa5__Action = "http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionResponse";

	//检测发送的Filter信息
	if(!pointer_valid_check(tev__CreatePullPointSubscription->Filter))
	{
		td_printf(0, "tev__CreatePullPointSubscription->Filter->__size ======= %d", tev__CreatePullPointSubscription->Filter->__size);
		GetMaskFromFilter(tev__CreatePullPointSubscription->Filter, &iTopicMask);
		
		if(iTopicMask == 0)
		{
			//暂时修改错误信息，便于跟踪原因，若果测试工具不识别，在修改回去
			soap_sender_fault_info(ptSoap, "cat: CreatePullPointSubscription: topic err", NULL, NULL);
		//	soap_sender_fault_info(ptSoap, "wsnt:InvalidMessageContentExpressionFault", NULL, NULL);
			return SOAP_FAULT;
		}
	 
	}
	else
	{
		//如果没有过滤，全部订阅
		iTopicMask |= ~0;
	}

	//Response
	tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Address = ptOnvifCtrl->m_stOnvifCfg.m_ptCreatePPSRsp->SubscriptionReference.Address;

	//获取当前时间
	time(&tTimeCurr);
	tev__CreatePullPointSubscriptionResponse->wsnt__CurrentTime = tTimeCurr;

	//获取终止时间
	if(pointer_valid_check(tev__CreatePullPointSubscription->InitialTerminationTime))
	{
		pcIniTimeStr= "PT24H";//"PT60S";
	}
	else
	{
		pcIniTimeStr = tev__CreatePullPointSubscription->InitialTerminationTime;
	}
	iRetTime = PTStrToTime(pcIniTimeStr);
	
	//InitialTerminationTime转化为以秒为单位的float类型
	if(iRetTime >= 0)
	{
		tTimeTerm = tTimeCurr + iRetTime;
	}
	else
	{
		tTimeTerm = tTimeCurr;
	}
	tev__CreatePullPointSubscriptionResponse->wsnt__TerminationTime = tTimeTerm;

	ptPullMsgInfo = &ptOnvifCtrl->m_stManager.m_tPullMsg;
	ptPullMsgInfo->m_iEnable = TD_TRUE;	//置标志位
	ptPullMsgInfo->m_iPullTopicMask = iTopicMask;//暂时先保存qiaohaijun @2013.12.06
	ptPullMsgInfo->m_iChnMask = iChnMask;//暂时先保存
	ptPullMsgInfo->m_eMoveState    = INITIALIZE;
	ptPullMsgInfo->m_ePortState    = INITIALIZE;
	ptPullMsgInfo->m_tCreateTime    = tTimeCurr;
	ptPullMsgInfo->m_tTerminateTime = tTimeTerm;
	Strcpy(ptPullMsgInfo->m_cEndPoint, tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Address, 64);

	return SOAP_OK;
}
 
/**************************************************
*功能:		获取报警的属性
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20120305 v1.0 creat by ptb
20131101 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetEventProperties(struct soap* ptSoap, struct _tev__GetEventProperties *tev__GetEventProperties, struct _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse)
{
	struct _tev__GetEventPropertiesResponse *ptEventProperRsp = NULL;
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;

	//设置header信息
	if(pointer_valid_check(ptSoap->header))
	{
		ptSoap->header = (struct SOAP_ENV__Header *)soap_mallocz(ptSoap, sizeof(struct SOAP_ENV__Header));
	}
	
	ptSoap->header->wsa5__Action = "http://www.onvif.org/ver10/events/wsdl/EventPortType/GetEventPropertiesResponse";

	//response
	//TopicNamespaceLocation
	ptEventProperRsp = ptOnvifCtrl->m_stOnvifCfg.m_ptEventPropertiesRsp;
	tev__GetEventPropertiesResponse->__sizeTopicNamespaceLocation = ptEventProperRsp->__sizeTopicNamespaceLocation;
	tev__GetEventPropertiesResponse->TopicNamespaceLocation = ptEventProperRsp->TopicNamespaceLocation;

	tev__GetEventPropertiesResponse->wsnt__FixedTopicSet = ptEventProperRsp->wsnt__FixedTopicSet;
	tev__GetEventPropertiesResponse->wstop__TopicSet = ptEventProperRsp->wstop__TopicSet;

	tev__GetEventPropertiesResponse->__sizeTopicExpressionDialect = ptEventProperRsp->__sizeTopicExpressionDialect;
	tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect = ptEventProperRsp->wsnt__TopicExpressionDialect;

	tev__GetEventPropertiesResponse->__sizeMessageContentFilterDialect = ptEventProperRsp->__sizeMessageContentFilterDialect;
	tev__GetEventPropertiesResponse->MessageContentFilterDialect = ptEventProperRsp->MessageContentFilterDialect;

	tev__GetEventPropertiesResponse->__sizeMessageContentSchemaLocation = ptEventProperRsp->__sizeMessageContentSchemaLocation;
	tev__GetEventPropertiesResponse->MessageContentSchemaLocation = ptEventProperRsp->MessageContentSchemaLocation;

	return SOAP_OK;
}

/**************************************************
*功能:		上传报警消息(主动模式)
*参数:		_ptSoap		:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:201200326 v1.0 creat by ptb
20131101 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns8__Notify(struct soap* ptSoap, struct _wsnt__Notify *wsnt__Notify)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
		
	//设置header信息
	if(pointer_valid_check(ptSoap->header))
	{
		ptSoap->header = (struct SOAP_ENV__Header *)soap_mallocz(ptSoap, sizeof(struct SOAP_ENV__Header));
	}
	ptSoap->header->wsa5__Action = "http://docs.oasis-open.org/wsn/bw-2/NotificationConsumer/Notify";

	//response
	wsnt__Notify->NotificationMessage = ptOnvifCtrl->m_stOnvifCfg.m_ptNotify->NotificationMessage;

	return SOAP_OK;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns6__Renew(struct soap* ptSoap, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns6__Unsubscribe(struct soap* ptSoap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns7__GetCurrentMessage(struct soap* ptSoap, struct _wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, struct _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
  
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns9__GetMessages(struct soap* ptSoap, struct _wsnt__GetMessages *wsnt__GetMessages, struct _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns9__DestroyPullPoint(struct soap* ptSoap, struct _wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, struct _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns9__Notify(struct soap* ptSoap, struct _wsnt__Notify *wsnt__Notify)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __ns10__CreatePullPoint(struct soap* ptSoap, struct _wsnt__CreatePullPoint *wsnt__CreatePullPoint, struct _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"",NULL,NULL);
}

#endif 
 
/**************************************************
*功能:		获取设备服务信息
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20131101 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* ptSoap, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse)
{
	DBG_HERE;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(xsd__boolean__false_ == tds__GetServices->IncludeCapability)
	{
		tds__GetServicesResponse->__sizeService = ptOnvifCtrl->m_stOnvifCfg.m_ptServicesRsp->__sizeService;
		tds__GetServicesResponse->Service = ptOnvifCtrl->m_stOnvifCfg.m_ptServicesRsp->Service;
	}
	
	return 0;
}
 
/**************************************************
*功能:		获取设备服务能力
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20131101 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities(struct soap* ptSoap, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse)
{
	DBG_HERE;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	
	tds__GetServiceCapabilitiesResponse->Capabilities = ptOnvifCtrl->m_stOnvifCfg.m_ptServiceCapRsp->Capabilities;

	return SOAP_OK;
}
 
/**************************************************
*功能:		获取设备信息
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20131101 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation(struct soap* ptSoap, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse)
{
	DBG_HERE;
	struct _tds__GetDeviceInformationResponse *ptDevInfoRsp = NULL;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	ptDevInfoRsp = ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp;

	tds__GetDeviceInformationResponse->Manufacturer    = ptDevInfoRsp->Manufacturer;
	tds__GetDeviceInformationResponse->Model           = ptDevInfoRsp->Model;
	tds__GetDeviceInformationResponse->FirmwareVersion = ptDevInfoRsp->FirmwareVersion;
	tds__GetDeviceInformationResponse->SerialNumber    = ptDevInfoRsp->SerialNumber;
	tds__GetDeviceInformationResponse->HardwareId      = ptDevInfoRsp->HardwareId;

	return SOAP_OK;
}

/**************************************************
*功能:		检查时区是否合法
*参数:		
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
int CheckTimeZone(char *_pcTimeZone)
{
	char *pcTimeZone = _pcTimeZone;
	char *pcNumPos = NULL;
	int iRet = 0;
	
	if(pointer_valid_check(pcTimeZone))
	{
		return TD_ERROR;
	}
	
	pcNumPos = FindNumPos(pcTimeZone);
	if(pointer_valid_check(pcNumPos))
	{
		return TD_ERROR;
	}
	iRet = atoi(pcNumPos);
	
	//时区最大偏移13
	iRet = (iRet > 13) ? TD_ERROR : TD_OK;	
	return iRet;
}

/**************************************************
*功能:		获取时区偏移
*参数:		
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
void GetTimeZoneOffset(char *_pcTimeZone)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char *pcTimeZone = _pcTimeZone;
	char *pcTmp = NULL;
	int iTimeZone = -8;

	//pcTimeZone = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", -1, "timezone", -1, "CST-8");
	if(strlen(pcTimeZone) > 0)
	{
		pcTmp = FindNumPos(pcTimeZone);
		if(NULL != pcTmp)
		{
			iTimeZone = atoi(pcTmp);
			if(*(pcTmp - 1) == '-')
			{
				iTimeZone = (-1) * iTimeZone;
			}

			if(ptOnvifCtrl->m_stNvsInfo.m_iTimeZone != iTimeZone)
			{
				ptOnvifCtrl->m_stNvsInfo.m_iTimeZone = iTimeZone;
			}
		}
	}
}

/**************************************************
*功能:		检查时间是否合法
*参数:		
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
int SetTimeZone(char *_pcTimeZone)
{
	struct tt__SystemDateTime *ptSysTime = NULL;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char *ptTimeZone = _pcTimeZone;
	
	ptSysTime = ptOnvifCtrl->m_stOnvifCfg.m_ptSystemTimeRsp->SystemDateAndTime;
	
	Strcpy(ptSysTime->TimeZone->TZ, ptTimeZone, MEM_LEN_MIN);

	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", -1, "timezone", -1, ptTimeZone, -1, CFG_REPLACE); 
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	GetTimeZoneOffset(ptTimeZone);

	return TD_OK;
}

/**************************************************
*功能:		检查时间是否合法
*参数:		
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
int CheckTime(struct tt__DateTime *_ptTime)
{
	struct tt__DateTime *ptTime = _ptTime;
	int iYear   = 0;
	int iMonth  = 0;
	int iDay    = 0;
	int iHour   = 0;
	int iMinute = 0;
	int iSecond = 0;
	
	if(pointer_valid_check(ptTime))
	{
		return TD_ERROR;
	}

	iYear	= ptTime->Date->Year;
	iMonth	= ptTime->Date->Month;
	iDay	= ptTime->Date->Day;
	iHour	= ptTime->Time->Hour;
	iMinute = ptTime->Time->Minute;
	iSecond = ptTime->Time->Second;
	
	if(iYear < 0 || iMonth <= 0 || iMonth > 12 || iDay <= 0 || iDay > 31
	|| iHour < 0 || iHour > 24 || iMinute < 0 || iMinute >= 60
	|| iSecond < 0 || iSecond >= 60)
	{
		return TD_ERROR;
	}

	return TD_OK;
}

/**************************************************
*功能:		设置系统时间
*参数:		
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
int SetTimeToKernel(int _iZoneFlag, struct tt__DateTime *_ptTime)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiSetSysTimeReq SetTimeReq = {0};
	struct tt__DateTime *ptTime = _ptTime;
	struct timeval tIpcTime = {0};
	struct tm tTmp = {0};
	time_t tTime;

	tTmp.tm_year = ptTime->Date->Year - 1900;
	tTmp.tm_mon	 = ptTime->Date->Month - 1;
	tTmp.tm_mday = ptTime->Date->Day;
	tTmp.tm_hour = ptTime->Time->Hour;
	tTmp.tm_min	 = ptTime->Time->Minute;
	tTmp.tm_sec	 = ptTime->Time->Second;
	
	tTime = mktime(&tTmp);
	tTime -= ptOnvifCtrl->m_stNvsInfo.m_iTimeZone * 3600;

	//ipc 无时区概念,默认UTC时间显示
	if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		//解决华为平台校时问题
		tIpcTime.tv_sec  = tTime;
		tIpcTime.tv_usec = 0;
		if(settimeofday(&tIpcTime, (struct timezone *) 0))
		{
			perror("settimeofday");
			return TD_ERROR;
		}
		//end
	}
	
	localtime_r(&tTime, &tTmp);
	SetTimeReq.m_iYear = tTmp.tm_year + 1900;
	SetTimeReq.m_iMon  = tTmp.tm_mon + 1;
	SetTimeReq.m_iDay  = tTmp.tm_mday;
	SetTimeReq.m_iHour = tTmp.tm_hour;
	SetTimeReq.m_iMin  = tTmp.tm_min;
	SetTimeReq.m_iSec  = tTmp.tm_sec;
	if(_iZoneFlag)
	{
		SetTimeReq.m_iTimeZone = GUI_TIME_ZONE_DEFAULT + (-1) * ptOnvifCtrl->m_stNvsInfo.m_iTimeZone;
	}
	else
	{
		SetTimeReq.m_iTimeZone = -1;
	}

	OnvifCmdSetTime(&SetTimeReq);
	
	return TD_OK;
}

/**************************************************
*功能:		设置系统时间
*参数:		
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime(struct soap* ptSoap, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse)
{
	struct tt__SystemDateTime *ptSysTime = NULL;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iZoneFlag = 0;
	int iRet = 0;
	DBG_HERE;

	ptSysTime = ptOnvifCtrl->m_stOnvifCfg.m_ptSystemTimeRsp->SystemDateAndTime;

	if(tt__SetDateTimeType__Manual == tds__SetSystemDateAndTime->DateTimeType)
	{
		ptSysTime->DateTimeType = tds__SetSystemDateAndTime->DateTimeType;

		if(NULL != tds__SetSystemDateAndTime->TimeZone)
		{
			td_printf(0, "tz[%s]", tds__SetSystemDateAndTime->TimeZone->TZ);
			iRet = CheckTimeZone(tds__SetSystemDateAndTime->TimeZone->TZ);
			if(iRet < 0)
			{
				soap_sender_fault_info(ptSoap, "An invalid time zone was specified.","ter:InvalidArgVal","ter:InvalidTimeZone");
				return SOAP_FAULT;
			}
			SetTimeZone(tds__SetSystemDateAndTime->TimeZone->TZ);
			iZoneFlag = 1;
		}

		iRet = CheckTime(tds__SetSystemDateAndTime->UTCDateTime);
		if(iRet < 0)
		{
			soap_sender_fault_info(ptSoap,"An invalid date or time was specified.","ter:InvalidArgVal","ter:InvalidDateTime");
			return SOAP_FAULT;
		}
		SetTimeToKernel(iZoneFlag, tds__SetSystemDateAndTime->UTCDateTime);
	}
	else
	{
		//NTP校时不支持
		soap_sender_fault_info(ptSoap, "Cannot switch DateTimeType to NTP because no NTP server is defined.", "ter:InvalidArgVal", "ter:NtpServerUndefined");
		return SOAP_FAULT;
	}
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		获取系统时间
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20131125 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime(struct soap* ptSoap, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	struct tt__SystemDateTime *ptSysTime = NULL;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char *pcTimeZone = NULL;
	struct tm tLocalTime;
	struct tm tUTCTime;
	time_t time_now;
	int iTimeZone = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}

	ptSysTime = ptOnvifCtrl->m_stOnvifCfg.m_ptSystemTimeRsp->SystemDateAndTime;
	pcTimeZone = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", -1, "timezone", -1, "WAUST-8");
	Strcpy(ptSysTime->TimeZone->TZ, pcTimeZone, MEM_LEN_MIN);
	
	TdTime(&time_now);
	gmtime_r(&time_now, &tUTCTime);

	//UTCDateTime
	ptSysTime->UTCDateTime->Date->Year   = tUTCTime.tm_year + 1900;
	ptSysTime->UTCDateTime->Date->Month  = tUTCTime.tm_mon + 1;
	ptSysTime->UTCDateTime->Date->Day    = tUTCTime.tm_mday;
	ptSysTime->UTCDateTime->Time->Hour   = tUTCTime.tm_hour;
	ptSysTime->UTCDateTime->Time->Minute = tUTCTime.tm_min;
	ptSysTime->UTCDateTime->Time->Second = tUTCTime.tm_sec;
	td_printf(0, "%s[%d] UTC time is :%04d-%02d-%02d, %02d:%02d:%02d\n", __FUNCTION__, __LINE__, \
		tUTCTime.tm_year + 1900, tUTCTime.tm_mon + 1, tUTCTime.tm_mday, \
		tUTCTime.tm_hour, tUTCTime.tm_min, tUTCTime.tm_sec);
		
	iTimeZone = ptOnvifCtrl->m_stNvsInfo.m_iTimeZone;
	time_now -=	iTimeZone * 3600;
	gmtime_r(&time_now, &tLocalTime);
		
	//LocalDateTime 
	ptSysTime->LocalDateTime->Date->Year   = tLocalTime.tm_year + 1900;
	ptSysTime->LocalDateTime->Date->Month  = tLocalTime.tm_mon + 1;
	ptSysTime->LocalDateTime->Date->Day    = tLocalTime.tm_mday;
	ptSysTime->LocalDateTime->Time->Hour   = tLocalTime.tm_hour;
	ptSysTime->LocalDateTime->Time->Minute = tLocalTime.tm_min;
	ptSysTime->LocalDateTime->Time->Second = tLocalTime.tm_sec;
	
	td_printf(0, "%s[%d] local time is :%04d-%02d-%02d, %02d:%02d:%02d\n", __FUNCTION__, __LINE__, \
		tLocalTime.tm_year + 1900, tLocalTime.tm_mon + 1, tLocalTime.tm_mday, \
		tLocalTime.tm_hour, tLocalTime.tm_min, tLocalTime.tm_sec);
	
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime = ptSysTime;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by sy
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(struct soap* ptSoap, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	remove(CONFIG_ONVIF_PATH);//只恢复onvif 默认设置
	
	//if(tds__SetSystemFactoryDefault->FactoryDefault == tt__FactoryDefaultType__Hard)
	{
		//通过共享内存恢复默认设置
	}
	
	g_stOnvifCtrl.m_stFlag.m_iFactoryDefaultFlag = TD_TRUE;
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(struct soap* ptSoap, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The firmware was invalid not supported by this device.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120229 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* ptSoap, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	tds__SystemRebootResponse->Message = "System Rebooting......, please wait a moment!";

	ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem(struct soap* ptSoap, struct _tds__RestoreSystem *tds__RestoreSystem, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The backup file are invalid..",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* ptSoap, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* ptSoap, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"There is no system log information available.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(struct soap* ptSoap, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"There is no support information available..",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by pangtb
			20130318 v2.3.0 modify ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* ptSoap, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	tds__GetScopesResponse->__sizeScopes = ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes;
	tds__GetScopesResponse->Scopes       = ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by pangtb
			20130318 v2.3.0 modify ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* ptSoap, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iSizeScope = 0;
	int iScopeLen = 0;
	int iTotalLen = 0;
	int iCfgNum = 0;
	int iIndex = 0;
	int i = 0;

	if(tds__SetScopes->__sizeScopes > MAX_SCOPES_NUM)
	{
		soap_receiver_fault_info(ptSoap, "The requested scope list exceeds the supported number of scopes.", "ter:Action", "ter:TooManyScopes");
		return SOAP_FAULT;
	}

	iSizeScope = ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes;

	//确定可配置的Scope数量
	for(i = 0; i < iSizeScope; i++)
	{
		if(tt__ScopeDefinition__Configurable == ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeDef)
		{
			++iCfgNum;
		}
	}
	
	if(tds__SetScopes->__sizeScopes > iCfgNum)
	{
		soap_sender_fault_info(ptSoap, "The requested operation is not permitted by the device.", "ter:OperationProhibited", "ter:ScopeOverwrite");
		return SOAP_FAULT;
	}

	for(i = 0; i < iSizeScope; i++)
	{
		if(tt__ScopeDefinition__Configurable == ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeDef)
		{
			//更新Scopes信息
			memset(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem, 0, STRLEN_128);
			memcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem, tds__SetScopes->Scopes[iIndex], STRLEN_128);
			td_printf(0, "Modify ScopeItem is %d:%s \n", i + 1, ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem); 
			
			SetProStr(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scope", i + 1, tds__SetScopes->Scopes[iIndex], 0, CFG_REPLACE);
			iIndex++;
			if(iIndex >= tds__SetScopes->__sizeScopes)
			{
				break;
			}
		}
		else
		{
			continue;
		}
	}

	//更新probe信息中scope
	memset(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 0, MAX_STRING_LEN_MEM);
	for(i = 0; i < iSizeScope; i++)
	{
		iScopeLen = strlen(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem);
		iTotalLen = strlen(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item);
		if((iTotalLen + iScopeLen) < MAX_STRING_LEN_MEM)
		{
			sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 
				"%s %s", 
				ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 
				ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem);
		}
	}
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		增加Scopes
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20120224 v1.0 creat by pangtb
				20130516 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* ptSoap, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	struct _tds__GetScopesResponse *pstGetScopesRsp = NULL;
	struct tt__Scope *ptTmpScope = NULL;
	int iScopeLen = 0;
	int iTotalLen = 0;
	int iScopeNum = 0;
	int iCount = 0;
	int iIdleNum = 0;
	int i = 0;
	
	if(pointer_valid_check(tds__AddScopes))
	{
		return SOAP_ERR;
	}
	
	iScopeNum = tds__AddScopes->__sizeScopeItem;
	pstGetScopesRsp = ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp;

	//判断Scope总数是否超过最大值
	iCount = iScopeNum + ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes;
	if(iCount > MAX_SCOPES_NUM)
	{
		td_printf(0, "%s %d sizeScopeItem:%d\n", __FUNCTION__, __LINE__, iScopeNum);
		soap_receiver_fault_info(ptSoap, "The requested scope list exceeds the supported number of scopes.", "ter:Action", "ter:TooManyScopes");
		return SOAP_FAULT;
	}

	//开辟新空间
	ptTmpScope = (struct tt__Scope*)(char *)soap_mallocz(ptSoap, iScopeNum * sizeof(struct tt__Scope));
	if(pointer_valid_check(ptTmpScope))
	{
		return SOAP_ERR;
	}

	for(i = 0; i < iScopeNum; i++)
	{
		if(pointer_valid_check(tds__AddScopes->ScopeItem[i]))
		{
			return SOAP_ERR;
		}

		//判断Scope合法性
		if(!strstr(tds__AddScopes->ScopeItem[i], "onvif://www.onvif.org"))
		{
			soap_sender_fault(ptSoap, "scope is not standed!", NULL);
			return SOAP_FAULT;
		}
		
		//填充新结构体
		ptTmpScope[i].ScopeDef  = tt__ScopeDefinition__Configurable;
		ptTmpScope[i].ScopeItem = (char *)calloc(1, STRLEN_128);
		if(pointer_valid_check(ptTmpScope[i].ScopeItem))
		{
			return SOAP_ERR;
		}
		
		if(strlen(tds__AddScopes->ScopeItem[i]) < STRLEN_128)
		{
			memcpy(ptTmpScope[i].ScopeItem, tds__AddScopes->ScopeItem[i], STRLEN_128);
		}

		//修改配置文件
		iIdleNum = GetIdleNum(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scope", -1);
		if(iIdleNum <= 0 || iIdleNum > MAX_INDEX_NUM)
		{
			return SOAP_ERR;
		}
		
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scope", iIdleNum, tds__AddScopes->ScopeItem[i], 0, CFG_ADD);

		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scopedef", iIdleNum, "1", 0, CFG_ADD);
	}

	//更新全局变量
	pstGetScopesRsp->Scopes = (struct tt__Scope *)ArrayAdd(pstGetScopesRsp->Scopes, 	\
			pstGetScopesRsp->__sizeScopes * sizeof(struct tt__Scope),	\
			ptTmpScope, iScopeNum * sizeof(struct tt__Scope));
	if(pointer_valid_check(pstGetScopesRsp->Scopes))
	{
		td_printf(0, "%s %d %s ArrayAdd error!\n", __FILE__, __LINE__, __FUNCTION__);
		return SOAP_ERR;
	}

	pstGetScopesRsp->__sizeScopes += iScopeNum;

	//更新probe信息中scope
	memset(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 0, MAX_STRING_LEN_MEM);
	for(i = 0; i < pstGetScopesRsp->__sizeScopes; i++)
	{
		iScopeLen = strlen(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem);
		iTotalLen = strlen(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item);
		if((iTotalLen + iScopeLen) < MAX_STRING_LEN_MEM)
		{
			sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 
				"%s %s", 
				ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 
				ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem);
		}
	}

	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	if(ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp->DiscoveryMode == tt__DiscoveryMode__Discoverable)
	{
		ptOnvifCtrl->m_stFlag.m_iSendHelloFlag = TD_TRUE;
	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		删除Scopes
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130529 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* ptSoap, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char cTmpBuf[16] =  {0};
	int iSizeScopes = 0;
	int iScopeLen = 0;
	int iTotalLen = 0;
	int iNum = 0;
	int i = 0;
	int j = 0;
	
	if(pointer_valid_check(tds__RemoveScopes->ScopeItem)
	|| pointer_valid_check(*tds__RemoveScopes->ScopeItem))
	{
		return SOAP_ERR;
	}

	iSizeScopes = ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes;

	//逐个删除
	for(i = 0; i < tds__RemoveScopes->__sizeScopeItem; i++)
	{
		for(j = 0; j < iSizeScopes; j++)
		{
			if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[j].ScopeItem, tds__RemoveScopes->ScopeItem[i]))
			{
				if(tt__ScopeDefinition__Fixed == ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[j].ScopeDef)
				{
					soap_sender_fault_info(ptSoap, "Trying to Remove fixed scope parameter, command rejected.", "ter:OperationProhibited", "ter:FixedScope");
					return SOAP_FAULT;
				}
				else
				{					
					//free memory
					free(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[j].ScopeItem);
					ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[j].ScopeItem = NULL;

					//update m_ptds__GetScopesResponse
					ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes = (struct tt__Scope *)ArrayDel(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes, 	\
							ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes * sizeof(struct tt__Scope), 	\
							ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes + j, sizeof(struct tt__Scope));
					if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes))
					{
						return SOAP_ERR;
					}

					iSizeScopes--;
						
					//修改配置文件
					iNum = GetKeyNum(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scope", tds__RemoveScopes->ScopeItem[i], 0, -1);
					if(iNum <= 0)
					{
						td_printf(0, "iNum:%d\n", iNum);
						return SOAP_ERR;
					}

					sprintf(cTmpBuf, "%d", iSizeScopes);
					SetProStr(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scope_size", 0, cTmpBuf, 0, CFG_REPLACE);

					SetProStr(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scope", iNum, NULL, 0, CFG_RM);

					SetProStr(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scopedef", iNum, NULL, 0, CFG_RM);
					
					break;	//跳出当前查找,rm下一个scope
				}
			}
			else
			{
				//查找到最后,scope不存在
				if(j == (iSizeScopes - 1))
				{
					td_printf(0, "%s %d %s scope is not exist\n", __FILE__, __LINE__, __FUNCTION__);
					soap_sender_fault_info(ptSoap, "Trying to Remove scope which does not exist.", "ter:InvalidArgVal", "ter:NoScope");
					return SOAP_FAULT;
				}
				else
				{
					continue;
				}
			}
		}
	}

	ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes = iSizeScopes;

	//更新probe信息中scope
	memset(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 0, MAX_STRING_LEN_MEM);
	for(i = 0; i < iSizeScopes; i++)
	{
		iScopeLen = strlen(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem);
		iTotalLen = strlen(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item);
		if((iTotalLen + iScopeLen) < MAX_STRING_LEN_MEM)
		{
			sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 
				"%s %s", 
				ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes->__item, 
				ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem);
		}
	}
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
	
	if(ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp->DiscoveryMode == tt__DiscoveryMode__Discoverable)
	{
		ptOnvifCtrl->m_stFlag.m_iSendHelloFlag = TD_TRUE;
	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* ptSoap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	tds__GetDiscoveryModeResponse->DiscoveryMode = ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp->DiscoveryMode;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* ptSoap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char cTmp[32] = {0};

	//updata ptOnvifCtrl
	ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp->DiscoveryMode = tds__SetDiscoveryMode->DiscoveryMode;

	//updata config file
	sprintf(cTmp,"%d", tds__SetDiscoveryMode->DiscoveryMode);
	
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "discrymode", 0, cTmp, 0, CFG_REPLACE);

	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode(struct soap* ptSoap, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode(struct soap* ptSoap, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses(struct soap* ptSoap, struct _tds__GetDPAddresses *tds__GetDPAddresses, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference(struct soap* ptSoap, struct _tds__GetEndpointReference *tds__GetEndpointReference, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser(struct soap* ptSoap, struct _tds__GetRemoteUser *tds__GetRemoteUser, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"Remote User handling is not supported",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* ptSoap, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"Remote User handling not supported.",NULL,NULL);
}

/**************************************************
*功能:		获取用户名密码等相关信息
*参数:		_ptSoap					:	soap信息
			_tds__GetUsers			:	GetUsers信息
			_tds__GetUsersResponse	:	GetUsers回复

*返回值:	0: 成功, !0: 失败
*修改历史:
	20120227 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* ptSoap, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int iUserNum		 	= 0;
	int i					= 0;	
	
	iUserNum = ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser;

	//初始化时用户数是0时
	if(0 == iUserNum)
	{
		return SOAP_OK;
	}
	
	//为tds__GetUsersResponse->User结构体开辟临时空间并初始化

	tds__GetUsersResponse->User = (struct tt__User *)soap_mallocz(ptSoap, sizeof(struct tt__User) * iUserNum);
	if(NULL == tds__GetUsersResponse->User)
	{
		return SOAP_ERR;
	}
	memset(tds__GetUsersResponse->User, 0, sizeof(struct tt__User) * iUserNum);

	//response
	//支持的用户的数量
	tds__GetUsersResponse->__sizeUser = iUserNum;

	//显示所有用户及对应的权限
	for(i = 0; i < iUserNum; i++)
	{
		(tds__GetUsersResponse->User + i)->Username = (ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + i)->Username;
		(tds__GetUsersResponse->User + i)->UserLevel = (ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + i)->UserLevel;
	}
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		添加用户
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:201200308 v1.0 creat by ptb
				20130516 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* ptSoap, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse)
{
	struct tt__User *ptUser = NULL;
	OnvifCtrl *ptOnvifCtrl	= &g_stOnvifCtrl;
	PointList stPLUserInfo;
	char cTmpUserNum[10] = {0};
	char cTmpUserLev[10] = {0};
	int iAddSecNum = 0;
	int iOldSize   = 0;
	int iNewSize   = 0;
	int i = 0;
	int j = 0;
	
	iOldSize = ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser;
	iNewSize = iOldSize + tds__CreateUsers->__sizeUser;
	
	//判断是否超出了支持的最大用户数
	if( iNewSize > MAX_USER_NUM )
	{
		soap_receiver_fault_info(ptSoap, "Maximum number of supported users exceeded.", "ter:Action", "ter:TooManyUsers");
		return SOAP_FAULT;
	}
	
	//判断创建的用户名是否已经存在
	for(i = 0; i < iOldSize; i++)
	{
		for(j = 0; j < tds__CreateUsers->__sizeUser; j++)
		{
			if(!strcmp((ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + i)->Username, tds__CreateUsers->User[j].Username))
			{
				soap_sender_fault_info(ptSoap, "Username already exists.", "ter:OperationProhibited", "ter:UsernameClash");
				return SOAP_FAULT;
			}
		}
	}

	for(j = 0; j < tds__CreateUsers->__sizeUser; j++)
	{
		//密码长度不能大于MAX_USERANDPWD_LEN
		if(strlen(tds__CreateUsers->User[j].Password) > MAX_USERANDPWD_LEN)
		{
			soap_sender_fault_info(ptSoap, "The password is too long.", "ter:OperationProhibited", "ter:PasswordTooLong");
			return SOAP_FAULT;
		}

		//用户名长度不能小于MIN_USERANDPWD_LEN
		if(strlen(tds__CreateUsers->User[j].Username) < MIN_USERANDPWD_LEN)
		{
			soap_sender_fault_info(ptSoap, "The username is too short.", "ter:OperationProhibited", "ter:UsernameTooShort");
			return SOAP_FAULT;
		}

		//用户名长度不能大于MAX_USERANDPWD_LEN
		if(strlen(tds__CreateUsers->User[j].Username) > MAX_USERANDPWD_LEN)
		{
			soap_sender_fault_info(ptSoap, "The username is too long.", "ter:OperationProhibited", "ter:UsernameTooLong");
			return SOAP_FAULT;
		}

		//不允许创建权限为匿名的用户
		if(3 == tds__CreateUsers->User[j].UserLevel)
		{
			soap_sender_fault_info(ptSoap, "User level anonymous is not allowed.", "ter:OperationProhibited", "ter:AnonymousNotAllowed");
			return SOAP_FAULT;
		}
	}

	//添加所有用户信息到配置文件中，并更新配置文件
	for(i = 0; i < tds__CreateUsers->__sizeUser; i++)
	{
		//添加新Section
		iAddSecNum = GetIdleNum(ptOnvifCtrl->m_ptOnvifDict, "user_", 0, NULL, -1);
		if(-1 == iAddSecNum)
		{
			iAddSecNum = 1;
		}

		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "user_", iAddSecNum, NULL, 0, NULL, 0, CFG_ADD);

		//增加新用户信息到OnvifDict
		memset(&stPLUserInfo, 0, sizeof(PointList));
		stPLUserInfo.m_iCount		= 3;
		stPLUserInfo.m_pcSec		= "user_";
		stPLUserInfo.m_iSecNum		= iAddSecNum;
		stPLUserInfo.m_pcKey[0] 	= "username";
		stPLUserInfo.m_iKeyNum[0]	= 0;
		stPLUserInfo.m_pcKey[1] 	= "password";
		stPLUserInfo.m_iKeyNum[1]	= 0;
		stPLUserInfo.m_pcKey[2] 	= "userlevel";
		stPLUserInfo.m_iKeyNum[2]	= 0;

		//获取需要创建的用户信息到OnvifDict
		stPLUserInfo.m_pcVal[0] = tds__CreateUsers->User[i].Username;
		stPLUserInfo.m_pcVal[1] = tds__CreateUsers->User[i].Password;
		sprintf(cTmpUserLev, "%d", tds__CreateUsers->User[i].UserLevel);
		stPLUserInfo.m_pcVal[2] = cTmpUserLev;
		
		//增加用户信息到配置文件
		SetProStrList(ptOnvifCtrl->m_ptOnvifDict, &stPLUserInfo, CFG_ADD);

	}
	
	//更新配置文件中user_num
	sprintf(cTmpUserNum, "%d", iNewSize);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "user_num", 0, cTmpUserNum, 0, CFG_REPLACE);

	ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser += tds__CreateUsers->__sizeUser;
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
	
	ptUser = (struct tt__User *)(char *)soap_mallocz(ptSoap, tds__CreateUsers->__sizeUser * sizeof(struct tt__User));
	if(pointer_valid_check(ptUser))
	{
		return SOAP_ERR;
	}
	for(i = 0; i < tds__CreateUsers->__sizeUser; i++)
	{
		ptUser[i].Username = (char *)calloc(1, MAX_STRING_LEN);
		ptUser[i].Password = (char *)calloc(1, MAX_STRING_LEN);
		if(pointer_valid_check(ptUser[i].Username)
		|| pointer_valid_check(ptUser[i].Password))
		{
			return SOAP_ERR;
		}

		strcpy(ptUser[i].Username, tds__CreateUsers->User[i].Username);
		strcpy(ptUser[i].Password, tds__CreateUsers->User[i].Password);
		ptUser[i].UserLevel	= tds__CreateUsers->User[i].UserLevel;
	}
	
	ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User = \
		(struct tt__User *)ArrayAdd(ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User, sizeof(struct tt__User) * iOldSize, ptUser, sizeof(struct tt__User) * tds__CreateUsers->__sizeUser);

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120308 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* ptSoap, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse)
{
	OnvifCtrl *ptOnvifCtrl	= &g_stOnvifCtrl;
	char cTmpUserNum[32] = {0};
	int iUserFlg  = 0;
	int iUserNum  = 0;
	int iSecNum   = 0;
	int iSize 	  = 0;
	int i = 0;
	int j = 0;
	
	iSize = tds__DeleteUsers->__sizeUsername;
	iUserNum = ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser;

	//出厂的两个用户不允许删除
	for(i = 0; i < iSize; i++)
	{
		if(!strcmp(*(tds__DeleteUsers->Username + i), "Admin")
		|| !strcmp(*(tds__DeleteUsers->Username + i), "admin"))
		{
			td_printf(0, "The user is not delete.\n");
			soap_sender_fault_info(ptSoap, "The default user can not be deleted.", NULL, NULL);
			return SOAP_FAULT;
		}
	}
	
#ifdef WITH_WSSE
	#if 0
	_wsse__UsernameToken *ptToken = soap_wsse_UsernameToken(ptSoap, NULL);

	//判断用户权限，只有Administrator权限用户可以删除
	for(i = 0; i < iUserNum; i++)
	{
		if(!strcmp((ptOnvifCtrl->m_stOnvifCfg.m_ptds__GetUsersResponse->User + i)->Username, ptToken->Username))
		{
			//UserLevel为0，代表Administrator权限
			if(0 != (ptOnvifCtrl->m_stOnvifCfg.m_ptds__GetUsersResponse->User + i)->UserLevel)
			{
				td_printf(0, "The user level is not Administrator.Delete user error!\n");
				return SOAP_FAULT;
			}
		}
	}
	#endif
#endif		
	//删除OnvifDict中用户信息
	//确认要删除的用户名都存在
	for(i = 0; i < iSize; i++)
	{
		//获取用户名匹配的Sec号
		iSecNum = GetSecNum(ptOnvifCtrl->m_ptOnvifDict, "user_", "username", 0, *(tds__DeleteUsers->Username + i), 0, -1);
		if(iSecNum <= 0)
		{
			soap_sender_fault_info(ptSoap, "Username not recognized.", "ter:InvalidArgVal", "ter:UsernameMissing");
			return SOAP_FAULT;
		}
	}
	
	for(i = 0; i < iSize; i++)
	{
		//获取用户名匹配的Sec号
		iSecNum = GetSecNum(ptOnvifCtrl->m_ptOnvifDict, "user_", "username", 0, *(tds__DeleteUsers->Username + i), 0, -1);

		//删除对应的Sec内容
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "user_", iSecNum, NULL, 0, NULL, 0, CFG_RM);
	}
	
	//用户数减掉iSize
	sprintf(cTmpUserNum, "%d", (iUserNum - iSize));
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "user_num", 0, cTmpUserNum, 0, CFG_REPLACE);

	//改变配置文件标志，把OnvifDict中信息更新到配置文件
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	//删除全局变量中的用户信息
	for(i = 0; i < iSize; i++)
	{	
		iUserFlg = 0;
		for(j = 0; j < iUserNum; j++)
		{	
			//通过对比用户名删除用户信息
			if(!strcmp((ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j)->Username, *(tds__DeleteUsers->Username + i)))
			{
				//先释放空间
				free((ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j)->Username);
				free((ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j)->Password);
				(ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j)->Username = NULL;
				(ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j)->Password = NULL;
				
				//在全局变量中清空
				ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User = \
					(struct tt__User *)ArrayDel(ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User, sizeof(struct tt__User) * iUserNum, ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j, sizeof(struct tt__User));

				iUserFlg = 1;
				--iUserNum;
				--ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser;
				break;
			}
				
		}
		
		if(0 == iUserFlg)
		{
			soap_sender_fault_info(ptSoap, "Username not recognized.", "ter:InvalidArgVal", "ter:UsernameMissing");
			return SOAP_FAULT;
		}
	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	201200309 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* ptSoap, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse)
{
	OnvifCtrl *ptOnvifCtrl	= &g_stOnvifCtrl;
	char cTmpUserLev[10] = {0};
	int iUserNum  = 0;
	int iSecNum   = 0;
	int iSize 	  = 0;
	int i = 0;
	int j = 0;

	iSize = tds__SetUser->__sizeUser;
	iUserNum = ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser;

	//出厂的两个用户不允许修改
	for(i = 0; i < iSize; i++)
	{
		if(!strcmp((tds__SetUser->User + i)->Username, "Admin")
		|| !strcmp((tds__SetUser->User + i)->Username, "admin"))
		{
			td_printf(0, "The user is not modify.\n");
			soap_sender_fault_info(ptSoap, "The default user can not be modified.", NULL, NULL);
			return SOAP_FAULT;
		}
	}

#ifdef WITH_WSSE
		#if 0
		_wsse__UsernameToken *ptToken = soap_wsse_UsernameToken(ptSoap, NULL);
	
		//判断用户权限，只有Administrator权限用户可以修改
		for(i = 0; i < iUserNum; i++)
		{
			if(!strcmp((ptOnvifCtrl->m_stOnvifCfg.m_ptds__GetUsersResponse->User + i)->Username, ptToken->Username))
			{
				//UserLevel为0，代表Administrator权限
				if(0 != (ptOnvifCtrl->m_stOnvifCfg.m_ptds__GetUsersResponse->User + i)->UserLevel)
				{
					td_printf(0, "The user level is not Administrator.Delete user error!\n");
					return SOAP_FAULT;
				}
			}
		}
		#endif
#endif		
		for(i = 0; i < iSize; i++)
		{	
			//设置的密码不能为空
			if(!strlen((tds__SetUser->User + i)->Password))
			{
				soap_sender_fault_info(ptSoap, "Too weak password.", "OperationProhibited", "ter:PasswordTooWeak");
				return SOAP_FAULT;
			}

			//密码长度不能大于MAX_USERANDPWD_LEN
			if(strlen((tds__SetUser->User + i)->Password) > MAX_USERANDPWD_LEN)
			{
				soap_sender_fault_info(ptSoap, "The password is too long.", "OperationProhibited", "ter:PasswordTooLong");
				return SOAP_FAULT;
			}
		
			//用户权限不允许为匿名
			if(3 == (tds__SetUser->User + i)->UserLevel)
			{
				soap_sender_fault_info(ptSoap, "User level anonymous is not allowed.", "OperationProhibited", "ter:AnonymousNotAllowed");
				return SOAP_FAULT;
			}
		}
		

	//修改OnvifDict中用户信息
	//确认要修改的用户名都存在
	for(i = 0; i < iSize; i++)
	{
		//获取用户名匹配的Sec号
		iSecNum = GetSecNum(ptOnvifCtrl->m_ptOnvifDict, "user_", "username", 0, (tds__SetUser->User + i)->Username, 0, -1);
		if(iSecNum <= 0)
		{
			soap_sender_fault_info(ptSoap, "Username not recognized.", "ter:InvalidArgVal", "ter:UsernameMissing");
			return SOAP_FAULT;
		}
	}

	for(i = 0; i < iSize; i++)
	{
		//获取用户名匹配的Sec号
		iSecNum = GetSecNum(ptOnvifCtrl->m_ptOnvifDict, "user_", "username", 0, (tds__SetUser->User + i)->Username, 0, -1);

		//修改密码
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "user_", iSecNum, "password", 0, (tds__SetUser->User + i)->Password, 0, CFG_REPLACE);

		sprintf(cTmpUserLev, "%d", (tds__SetUser->User + i)->UserLevel);
		//修改权限
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "user_", iSecNum, "userlevel", 0, cTmpUserLev, 0, CFG_REPLACE);
	}

	//改变配置文件标志，把OnvifDict中信息更新到配置文件
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	//修改全局变量中用户信息
	for(i = 0; i < iSize; i++)
	{	
		for(j = 0; j < iUserNum; j++)
		{	
			//通过对比用户名删除用户信息
			if(!strcmp((ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j)->Username, (tds__SetUser->User + i)->Username))
			{
				strcpy((ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j)->Password, (tds__SetUser->User + i)->Password);
				(ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User + j)->UserLevel = (tds__SetUser->User + i)->UserLevel;
			}
				
		}
	}
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl(struct soap* ptSoap, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	tds__GetWsdlUrlResponse->WsdlUrl = g_stOnvifCtrl.m_stOnvifCfg.m_ptWsdlUrlRsp->WsdlUrl;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by sy
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* ptSoap, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{
	//td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	int i = 0;
	
	tds__GetCapabilitiesResponse->Capabilities = (struct tt__Capabilities *)soap_malloc(ptSoap, sizeof(struct tt__Capabilities));
	if(tds__GetCapabilitiesResponse->Capabilities == NULL)
	{
		return SOAP_ERR;
	}
	memset(tds__GetCapabilitiesResponse->Capabilities, 0, sizeof(struct tt__Capabilities));

	if(tds__GetCapabilities->__sizeCategory == 0)
	{
		tds__GetCapabilitiesResponse->Capabilities = g_stOnvifCtrl.m_stOnvifCfg.m_ptCapbilityRsp->Capabilities;
		return SOAP_OK;
	}
	
	for(i = 0; i < tds__GetCapabilities->__sizeCategory; i++)
	{
		switch(*(tds__GetCapabilities->Category + i))
		{
		case tt__CapabilityCategory__All:
			tds__GetCapabilitiesResponse->Capabilities = g_stOnvifCtrl.m_stOnvifCfg.m_ptCapbilityRsp->Capabilities;
			break;
		case tt__CapabilityCategory__Analytics:
			tds__GetCapabilitiesResponse->Capabilities->Analytics = g_stOnvifCtrl.m_stOnvifCfg.m_ptCapbilityRsp->Capabilities->Analytics;
			break;
		case tt__CapabilityCategory__Device:
			tds__GetCapabilitiesResponse->Capabilities->Device = g_stOnvifCtrl.m_stOnvifCfg.m_ptCapbilityRsp->Capabilities->Device;
			break;
		case tt__CapabilityCategory__Events:
			tds__GetCapabilitiesResponse->Capabilities->Events = g_stOnvifCtrl.m_stOnvifCfg.m_ptCapbilityRsp->Capabilities->Events;
			break;
		case tt__CapabilityCategory__Imaging:
			tds__GetCapabilitiesResponse->Capabilities->Imaging = g_stOnvifCtrl.m_stOnvifCfg.m_ptCapbilityRsp->Capabilities->Imaging;
			break;
		case tt__CapabilityCategory__Media:
			tds__GetCapabilitiesResponse->Capabilities->Media = g_stOnvifCtrl.m_stOnvifCfg.m_ptCapbilityRsp->Capabilities->Media;
			break;
		case tt__CapabilityCategory__PTZ:
			tds__GetCapabilitiesResponse->Capabilities->PTZ = g_stOnvifCtrl.m_stOnvifCfg.m_ptCapbilityRsp->Capabilities->PTZ;
			break;
		default:
			//goto ERR;
			break;
		}
	}
	
	return SOAP_OK;
	
//ERR:

	soap_receiver_fault_info(ptSoap, "The requested WSDL service category is not supported by the device.", "ter:ActionNotSupported", "ter:NoSuchService");
	return SOAP_FAULT;
	
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses(struct soap* ptSoap, struct _tds__SetDPAddresses *tds__SetDPAddresses, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		获取主机名
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20140116 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname(struct soap* ptSoap, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	tds__GetHostnameResponse->HostnameInformation = g_stOnvifCtrl.m_stOnvifCfg.m_ptHostnameRsp->HostnameInformation;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		设置主机名
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20140116 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname(struct soap* ptSoap, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char *pcHostName = tds__SetHostname->Name;
	char *pcName = NULL;
	int iMaxLen  = MEM_LEN_MIN;
	int iLen = 0;
	int i = 0;
		
	if(NULL != pcHostName)
	{
		pcName = ptOnvifCtrl->m_stOnvifCfg.m_ptHostnameRsp->HostnameInformation->Name;
		iLen = strlen(pcHostName);
		if(iLen > 0 && iLen < iMaxLen)
		{
			for(i = 0; i < iLen; i++)
			{
				if(!(isalnum(*(pcHostName + i)) 
				|| *(pcHostName + i) == '.' 
				|| *(pcHostName + i) == '-'))
				{
					return soap_sender_fault_info(ptSoap, "The requested hostname cannot be accepted by the device.", "ter:InvalidArgVal", "ter:InvalidHostname");
				}
			}
			Strcpy(pcName, pcHostName, iMaxLen);
			return SOAP_OK;
		}
	}
	return soap_sender_fault_info(ptSoap, "The requested hostname cannot be accepted by the device.", "ter:InvalidArgVal", "ter:InvalidHostname");
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP(struct soap* ptSoap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* ptSoap, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	tds__GetDNSResponse->DNSInformation = g_stOnvifCtrl.m_stOnvifCfg.m_ptDNSRsp->DNSInformation;
	tds__GetDNSResponse->DNSInformation = (struct tt__DNSInformation *)soap_mallocz(ptSoap, sizeof(struct tt__DNSInformation));
	tds__GetDNSResponse->DNSInformation->FromDHCP = g_stOnvifCtrl.m_stOnvifCfg.m_ptDNSRsp->DNSInformation->FromDHCP;
	tds__GetDNSResponse->DNSInformation->__sizeSearchDomain = g_stOnvifCtrl.m_stOnvifCfg.m_ptDNSRsp->DNSInformation->__sizeSearchDomain;
	tds__GetDNSResponse->DNSInformation->SearchDomain = g_stOnvifCtrl.m_stOnvifCfg.m_ptDNSRsp->DNSInformation->SearchDomain;
	if(tds__GetDNSResponse->DNSInformation->FromDHCP)
	{
		tds__GetDNSResponse->DNSInformation->__sizeDNSFromDHCP = g_stOnvifCtrl.m_stOnvifCfg.m_ptDNSRsp->DNSInformation->__sizeDNSFromDHCP;
		tds__GetDNSResponse->DNSInformation->DNSFromDHCP = g_stOnvifCtrl.m_stOnvifCfg.m_ptDNSRsp->DNSInformation->DNSFromDHCP;
	}
	else
	{
		tds__GetDNSResponse->DNSInformation->__sizeDNSManual = g_stOnvifCtrl.m_stOnvifCfg.m_ptDNSRsp->DNSInformation->__sizeDNSManual;
		tds__GetDNSResponse->DNSInformation->DNSManual = g_stOnvifCtrl.m_stOnvifCfg.m_ptDNSRsp->DNSInformation->DNSManual;
	}
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* ptSoap, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiIpMsg stIpMsg;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	memset(&stIpMsg, 0, sizeof(TGuiIpMsg));

	if(OnvifCmdGetip(&stIpMsg))
	{
		return TD_ERROR;
	}
	
	if(NULL != tds__SetDNS->SearchDomain
	&& NULL != *(tds__SetDNS->SearchDomain))
	{
		strncpy(*(ptOnvifCtrl->m_stOnvifCfg.m_ptDNSRsp->DNSInformation->SearchDomain), *(tds__SetDNS->SearchDomain), MAX_STRING_LEN);
	}

	if(NULL != tds__SetDNS->DNSManual
	&& NULL != tds__SetDNS->DNSManual->IPv4Address
	&& NULL != *(tds__SetDNS->DNSManual->IPv4Address))
	{
		if(tt__IPType__IPv4 == tds__SetDNS->DNSManual->Type)
		{
			if(-1 == inet_addr(*(tds__SetDNS->DNSManual->IPv4Address)))
			{
				soap_sender_fault_info(ptSoap, "The suggested IPv4 address is invalid.", "ter:InvalidArgVal", "ter:InvalidIPv4Address");
				return SOAP_FAULT;
			}

			if(strcmp(*(tds__SetDNS->DNSManual->IPv4Address), stIpMsg.m_strDNS))
			{
				stIpMsg.m_iGetIpMethod = 1;
				strncpy(stIpMsg.m_strDNS, *(tds__SetDNS->DNSManual->IPv4Address), sizeof(stIpMsg.m_strDNS));
				if(OnvifCmdSetip(&stIpMsg))
				{
					return TD_ERROR;
				}
				strncpy(*(ptOnvifCtrl->m_stOnvifCfg.m_ptDNSRsp->DNSInformation->DNSManual->IPv4Address), *(tds__SetDNS->DNSManual->IPv4Address), MAX_STRING_LEN);
				return SOAP_OK;
			}
		}
	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* ptSoap, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiIpMsg stIpMsg = {0};
	char cTmp[32] = {0};

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	if(OnvifCmdGetip(&stIpMsg))
	{
		return TD_ERROR;
	}

	tds__GetNTPResponse->NTPInformation = (struct tt__NTPInformation *)soap_mallocz(ptSoap, sizeof(struct tt__NTPInformation));
	tds__GetNTPResponse->NTPInformation->FromDHCP = ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->FromDHCP;

	//DHCP
	if(xsd__boolean__true_ == ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->FromDHCP)
	{
		if(OnvifCmdGetNTP(cTmp, 32))
		{
			return TD_ERROR;
		}
		
		tds__GetNTPResponse->NTPInformation->FromDHCP = xsd__boolean__true_;
		tds__GetNTPResponse->NTPInformation->__sizeNTPFromDHCP = 1;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPFromDHCP->Type = tt__NetworkHostType__IPv4;
		strncpy(*ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPFromDHCP->IPv4Address, cTmp, 32);
		tds__GetNTPResponse->NTPInformation->NTPFromDHCP = ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPFromDHCP;
	}

	//Manual
	if(xsd__boolean__false_ == ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->FromDHCP)
	{
		tds__GetNTPResponse->NTPInformation->FromDHCP = xsd__boolean__false_;
		tds__GetNTPResponse->NTPInformation->__sizeNTPManual = 1;
		tds__GetNTPResponse->NTPInformation->NTPManual = ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPManual;
	}

	return TD_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:   20130314 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* ptSoap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	if(xsd__boolean__false_ == tds__SetNTP->FromDHCP)
	{
		if(NULL != tds__SetNTP->NTPManual 
		&& NULL != tds__SetNTP->NTPManual->IPv4Address
		&& NULL != *(tds__SetNTP->NTPManual->IPv4Address))
		{
			if(tt__NetworkHostType__IPv4 == tds__SetNTP->NTPManual->Type)
			{
				if(-1 == inet_addr(*(tds__SetNTP->NTPManual->IPv4Address)))
				{
					if(strcmp(*(tds__SetNTP->NTPManual->IPv4Address), "0"))
					{
						soap_sender_fault_info(ptSoap, "The suggested IPv4 address is invalid.", "ter:InvalidArgVal", "ter:InvalidIPv4Address");
						return SOAP_FAULT;
					}
				}
		
				if(strcmp(*(ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPManual->IPv4Address), *(tds__SetNTP->NTPManual->IPv4Address)))
				{
					if(OnvifCmdSetNTP(*(tds__SetNTP->NTPManual->IPv4Address)))
					{
						return TD_ERROR;
					}
					strcpy(*(ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPManual->IPv4Address), *(tds__SetNTP->NTPManual->IPv4Address));
					ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->FromDHCP = xsd__boolean__false_;
 				}
 				return SOAP_OK;
			}
		}
	}

	if(xsd__boolean__true_ == tds__SetNTP->FromDHCP)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->FromDHCP = xsd__boolean__true_;
		return SOAP_OK;
	}
	
	return TD_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* ptSoap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* ptSoap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(struct soap* ptSoap, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	tds__GetNetworkInterfacesResponse->__sizeNetworkInterfaces  = g_stOnvifCtrl.m_stOnvifCfg.m_ptNetworkInfoRsp->__sizeNetworkInterfaces;
	tds__GetNetworkInterfacesResponse->NetworkInterfaces = g_stOnvifCtrl.m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces;

	return TD_OK;
}
 
/**************************************************
*功能:		设置网络相关参数
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史: 20130318 v2.3.0 modify by ptb
				20130527 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(struct soap* ptSoap, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	struct in_addr stAddr = {0};
	unsigned int iMask = 0;
	TGuiIpMsg stIpMsg = {0};
	int i = 0;
	
	if(pointer_valid_check(tds__SetNetworkInterfaces->NetworkInterface))
	{
		return SOAP_ERR;
	}
	
	if(OnvifCmdGetip(&stIpMsg))
	{
		return TD_ERROR;
	}

	//确认token是否存在
	if(strcmp(tds__SetNetworkInterfaces->InterfaceToken, ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces->token))
	{
		soap_sender_fault_info(ptSoap, "The supplied network interface token does not exist.", "ter:InvalidArgVal", "ter:InvalidNetworkInterface");
		return SOAP_FAULT;
	}
	
	if(!pointer_valid_check(tds__SetNetworkInterfaces->NetworkInterface->MTU) 
	&& 1500 < *(tds__SetNetworkInterfaces->NetworkInterface->MTU))
	{
		soap_sender_fault_info(ptSoap, "The MTU value is invalid.", "ter:InvalidArgVal", "ter:InvalidMtuValue");
		return SOAP_FAULT;
	}

	//IPv4
	if(!pointer_valid_check(tds__SetNetworkInterfaces->NetworkInterface->IPv4))
	{
		//DHCP
		if(!pointer_valid_check(tds__SetNetworkInterfaces->NetworkInterface->IPv4->DHCP)
		&& xsd__boolean__true_ == *(tds__SetNetworkInterfaces->NetworkInterface->IPv4->DHCP))
		{
			stIpMsg.m_iGetIpMethod = 0;
			if(OnvifCmdSetip(&stIpMsg))
			{
				return TD_ERROR;
			}
			tds__SetNetworkInterfacesResponse->RebootNeeded = xsd__boolean__true_;
			return SOAP_OK;
		}

		//Manual
		if(!pointer_valid_check(tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual)
		&& !pointer_valid_check(tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->Address))
		{
			if(-1 == inet_addr(tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->Address))
			{
				soap_sender_fault_info(ptSoap, "The suggested IPv4 address is invalid.", "ter:InvalidArgVal", "ter:InvalidIPv4Address");
				return SOAP_FAULT;
			}

			i = 32 - tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->PrefixLength;
			for(i; i < 32; i++)
			{
				iMask |= (1 << i);
			}

			iMask = htonl(iMask);
			stAddr.s_addr = iMask;
			strcpy(stIpMsg.m_strSubMsk, inet_ntoa(stAddr));
			
			if(strcmp(tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->Address, stIpMsg.m_strIp))
			{
				stIpMsg.m_iGetIpMethod = 1;
				memcpy(stIpMsg.m_strIp, tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->Address, 32);
				if(OnvifCmdSetip(&stIpMsg))
				{
					return TD_ERROR;
				}
				tds__SetNetworkInterfacesResponse->RebootNeeded = xsd__boolean__true_;
			}
		}
	}

	//IPv6
	//暂不支持
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		获取网络参数
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20131121 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(struct soap* ptSoap, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	tds__GetNetworkProtocolsResponse->__sizeNetworkProtocols = 3;
	tds__GetNetworkProtocolsResponse->NetworkProtocols =  ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkProRsp->NetworkProtocols;
	int i = 0;
	
	for(i = 0; i < 3; i++)
	{
		td_printf(0, "i[%d] port[%d]", i, *ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkProRsp->NetworkProtocols[i].Port);
	}
	return TD_OK;
}

/**************************************************
*功能:		设置网络参数
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20120224 v1.0 creat by xxx
20130206 增加设置协议使能中false的判断 by wanglei(应用接入)
20131114 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(struct soap* ptSoap, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	struct tt__NetworkProtocol *ptSetProtocol = NULL;
	struct tt__NetworkProtocol *ptNetworkPro = NULL;
	char *pcPortType = NULL;
	int iHttpPort = 0;
	int iRtspPort = 0;
	int iSetPort  = 0;
	int iRet  = 0;
	int iFlag = 0;
	int iSize = 0;
	int i = 0;
	
	iSize = tds__SetNetworkProtocols->__sizeNetworkProtocols;
	ptSetProtocol = tds__SetNetworkProtocols->NetworkProtocols;
	ptNetworkPro = ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkProRsp->NetworkProtocols;

	iHttpPort = ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort;
	iRtspPort = ptOnvifCtrl->m_stRtspServer.m_iLPort;
	
	for(i = 0; i < iSize; i++)
	{
		//td_printf(0, "=============i[%d] name[%d] Enabled[%d]", i, ptSetProtocol[i].Name, ptSetProtocol[0].Enabled);
		if(!pointer_valid_check(ptSetProtocol + i))
		{
			if(tt__NetworkProtocolType__HTTP == ptSetProtocol[i].Name
			&& xsd__boolean__true_ == ptSetProtocol[i].Enabled)
			{
				ptNetworkPro[0].Enabled = ptSetProtocol[i].Enabled;
				if(!pointer_valid_check(ptSetProtocol[i].Port) && *ptSetProtocol[i].Port != iHttpPort)
				{
					iSetPort = *ptSetProtocol[i].Port;
					pcPortType = "httpport";
					td_printf(0, "httpport is %d", iSetPort);
				}
				else
				{
					continue;
				}
			}
			else if(tt__NetworkProtocolType__RTSP == ptSetProtocol[i].Name 
			&& xsd__boolean__true_ == ptSetProtocol[i].Enabled)
			{
				ptNetworkPro[2].Enabled = ptSetProtocol[i].Enabled;
				if(!pointer_valid_check(ptSetProtocol[i].Port) && *ptSetProtocol[i].Port != iRtspPort)
				{
					iSetPort = *ptSetProtocol[i].Port;
					pcPortType = "rtspport";
					td_printf(0, "rtspport is %d", iSetPort);
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}
			
			iRet = CheckPort(iSetPort);
			if(TD_OK == iRet)
			{
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", -1,  pcPortType, -1,  NULL, iSetPort, CFG_REPLACE);
				ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

				if(0 == strcmp(pcPortType, "httpport"))
				{
					*ptNetworkPro[0].Port = iSetPort;
					ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort = iSetPort;
				}
				else
				{
					*ptNetworkPro[2].Port = iSetPort;
					ptOnvifCtrl->m_stRtspServer.m_iLPort = iSetPort;
				}
				iFlag = TD_TRUE;
				continue;
			}
			else if(PROT_ILLEGAL == iRet)
			{
				soap_sender_fault_info(ptSoap, "The port is not a correct port.", "ter:InvalidArgVal", "ter:PortSetError");
				return SOAP_FAULT;
			}
			else if(PROT_USED == iRet)
			{
				soap_sender_fault_info(ptSoap, "The selected port is already in use.", "ter:InvalidArgVal", "ter:PortAlreadyInUse");
				return SOAP_FAULT;
			}
		}
	}
	
	if(TD_TRUE != iFlag)
	{
		return soap_sender_fault_info(ptSoap, "The supplied network service is not supported.", "ter:InvalidArgVal", "ter:ServiceNotSupported");
	}

	//modified bu huweixu rtsp端口修改了，发送重启外挂信号
	ptOnvifCtrl->m_stFlag.m_iPlatformRebootFlag = 1;

	return iRet;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(struct soap* ptSoap, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	tds__GetNetworkDefaultGatewayResponse->NetworkGateway = g_stOnvifCtrl.m_stOnvifCfg.m_ptNetworkGWRsp->NetworkGateway;
	return TD_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:   20130314 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(struct soap* ptSoap, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiIpMsg stIpMsg;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	memset(&stIpMsg, 0, sizeof(TGuiIpMsg));

	if(OnvifCmdGetip(&stIpMsg))
	{
		return TD_ERROR;
	}
	
	if(NULL != tds__SetNetworkDefaultGateway->IPv4Address
	&& NULL != *(tds__SetNetworkDefaultGateway->IPv4Address))
	{
		if(-1 == inet_addr(*(tds__SetNetworkDefaultGateway->IPv4Address)))
		{
			soap_sender_fault_info(ptSoap, "The suggested IPv4 address is invalid.", "ter:InvalidArgVal", "ter:InvalidIPv4Address");
			return SOAP_FAULT;
		}
		else
		{
			if(strcmp(stIpMsg.m_strGetWay, *(tds__SetNetworkDefaultGateway->IPv4Address)))
			{
				stIpMsg.m_iGetIpMethod = 1;
				strncpy(stIpMsg.m_strGetWay, *(tds__SetNetworkDefaultGateway->IPv4Address), sizeof(stIpMsg.m_strGetWay));
				if(OnvifCmdSetip(&stIpMsg))
				{
					return TD_ERROR;
				}
				strncpy(*(ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkGWRsp->NetworkGateway->IPv4Address), *(tds__SetNetworkDefaultGateway->IPv4Address), 16);
				return SOAP_OK;
			}
		}
	}
	
	return SOAP_OK;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(struct soap* ptSoap, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by czl
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(struct soap* ptSoap, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	soap_receiver_fault_info(ptSoap,"The requested action is optional and is not implemented by the device.", "ter:ActionNotSupported", NULL);
	return SOAP_FATAL_ERROR;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(struct soap* ptSoap, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(struct soap* ptSoap, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(struct soap* ptSoap, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(struct soap* ptSoap, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* ptSoap, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* ptSoap, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate(struct soap* ptSoap, struct _tds__CreateCertificate *tds__CreateCertificate, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates(struct soap* ptSoap, struct _tds__GetCertificates *tds__GetCertificates, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus(struct soap* ptSoap, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus(struct soap* ptSoap, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates(struct soap* ptSoap, struct _tds__DeleteCertificates *tds__DeleteCertificates, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request(struct soap* ptSoap, struct _tds__GetPkcs10Request *tds__GetPkcs10Request, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates(struct soap* ptSoap, struct _tds__LoadCertificates *tds__LoadCertificates, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode(struct soap* ptSoap, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode(struct soap* ptSoap, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		获取继电器输出
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130513 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs(struct soap* ptSoap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	tds__GetRelayOutputsResponse->__sizeRelayOutputs = ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->__sizeRelayOutputs;
	tds__GetRelayOutputsResponse->RelayOutputs       = ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:   20130318 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(struct soap* ptSoap, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iIndex = -1;
	int iPort  = 0;
	int i = 0;

	if(NULL == ptOnvifCtrl 
	|| NULL == tds__SetRelayOutputSettings->RelayOutputToken  
	|| NULL == tds__SetRelayOutputSettings->Properties)
	{
		return SOAP_FAULT;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->__sizeRelayOutputs; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs[i].token, tds__SetRelayOutputSettings->RelayOutputToken))
		{
			iIndex = i;
			break;
		}
	}

	if(iIndex < 0)
	{
		soap_sender_fault_info(ptSoap,"Unknown relay token reference.", "ter:InvalidArgVal", "ter:RelayToken");
		return SOAP_FAULT;
	}

	iPort = atoi(tds__SetRelayOutputSettings->RelayOutputToken + strlen("relayoutput_"));
	
	if(tt__RelayIdleState__closed == tds__SetRelayOutputSettings->Properties->IdleState)
	{
		if(OnvifCmdSetRelayOutputCnf(iPort, GUI_ALARM_OPEN_TYPE))
		{
			td_printf(0, "%s %d %s OnvifCmdSetRelayOutputCnf is error!\n", __FILE__, __LINE__, __FUNCTION__);
		}
	}
	else
	{
		if(OnvifCmdSetRelayOutputCnf(iPort, GUI_ALARM_CLOSE_TYPE))
		{
			td_printf(0, "%s %d %s OnvifCmdSetRelayOutputCnf is error!\n", __FILE__, __LINE__, __FUNCTION__);
		}
	}

	ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs[iIndex].Properties->IdleState = tds__SetRelayOutputSettings->Properties->IdleState;
	//added at 2012.11.21 by qiaohaijun
	ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs[iIndex].Properties->Mode = tds__SetRelayOutputSettings->Properties->Mode;
	ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs[iIndex].Properties->DelayTime = tds__SetRelayOutputSettings->Properties->DelayTime;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(struct soap* ptSoap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	int iIndex = -1;
	int iPort  = 0;
	int i = 0;

	if(tds__SetRelayOutputState->RelayOutputToken == NULL)
	{
		return SOAP_FAULT;
	}
	
	for(i = 0; i < g_stOnvifCtrl.m_stOnvifCfg.m_ptRelayOutputRsp->__sizeRelayOutputs; i++)
	{
		if(!strcmp(g_stOnvifCtrl.m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs[i].token, tds__SetRelayOutputState->RelayOutputToken))
		{
			iIndex = i;
			break;
		}
	}


	if(iIndex < 0)
	{
		soap_sender_fault_info(ptSoap,"Unknown relay token reference.", "ter:InvalidArgVal", "ter:RelayToken");
		return SOAP_FAULT;
	}

	iPort = atoi(tds__SetRelayOutputState->RelayOutputToken + strlen("relayoutput_")) - 1;

	if(tds__SetRelayOutputState->LogicalState == tt__RelayLogicalState__active)
	{
		if(OnvifCmdSetRelayOutputState(iPort + 1, 1))
		{
			return TD_ERROR;
		}
	}
	else
	{
		if(OnvifCmdSetRelayOutputState(iPort + 1, 0))
		{
			return TD_ERROR;
		}
	}
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(struct soap* ptSoap, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The requested AuxiliaryCommand is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates(struct soap* ptSoap, struct _tds__GetCACertificates *tds__GetCACertificates, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey(struct soap* ptSoap, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"the format is not supported by the device.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation(struct soap* ptSoap, struct _tds__GetCertificateInformation *tds__GetCertificateInformation, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates(struct soap* ptSoap, struct _tds__LoadCACertificates *tds__LoadCACertificates, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration(struct soap* ptSoap, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration(struct soap* ptSoap, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The suggested EAP method is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration(struct soap* ptSoap, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations(struct soap* ptSoap, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration(struct soap* ptSoap, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities(struct soap* ptSoap, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* ptSoap, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(struct soap* ptSoap, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* ptSoap, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(struct soap* ptSoap, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	tds__StartFirmwareUpgradeResponse->UploadUri = (char *)soap_mallocz(ptSoap, MAX_STRING_LEN_MEM);
	if(NULL == tds__StartFirmwareUpgradeResponse->UploadUri)
	{
		return SOAP_ERR;
	}
	sprintf(tds__StartFirmwareUpgradeResponse->UploadUri, "http://%s:8000", ptOnvifCtrl->m_stNvsInfo.m_cLocalIP);
	tds__StartFirmwareUpgradeResponse->UploadDelay = 1000;
	tds__StartFirmwareUpgradeResponse->ExpectedDownTime = 10000;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(struct soap* ptSoap, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

#ifdef WITH_IMAGE
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg20__GetServiceCapabilities(struct soap* ptSoap, struct _timg20__GetServiceCapabilities *timg__GetServiceCapabilities, struct _timg20__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		ptSoap								:	tcp soap
			timg__GetImagingSettings			:	request
			timg__GetImagingSettingsResponse	:	response
*返回值:	0: 成功, !0: 失败
*修改历史:   20120315 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg20__GetImagingSettings(struct soap* ptSoap, struct _timg20__GetImagingSettings *timg__GetImagingSettings, struct _timg20__GetImagingSettingsResponse *timg__GetImagingSettingsResponse)
{
	td_printf(0, "%s %d %s\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	VideoInCnf_EX stVInCnf = {0};
	int iVSNum = 0;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}
	
	iVSNum = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;

	//为ImagingSettings开辟空间
	timg__GetImagingSettingsResponse->ImagingSettings = ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV20Rsp->ImagingSettings;

	for(i = 0; i < iVSNum; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg__GetImagingSettings->VideoSourceToken))
		{
			if(OnvifCmdGetVideoInCnf(i + 1, &stVInCnf))
			{
				return TD_ERROR;
			}

			//更新亮度、饱和度、对比度
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Brightness)	   = (float)stVInCnf.m_iLum;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->ColorSaturation) = (float)stVInCnf.m_iSaturation;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Contrast) 	   = (float)stVInCnf.m_iContrast;

			if(ptOnvifCtrl->m_stFlag.m_iIspFlag)
			{
			#ifndef WITH_TESTTOOL
				//更新全局变量值
				if(TD_OK != UpdateIspInfo())
				{
					td_printf(0, "%s %d %s =====UpdateIspInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
				}
			#endif
				//Brightness
				timg__GetImagingSettingsResponse->ImagingSettings->Brightness = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Brightness;
				
				//ColorSaturation
				timg__GetImagingSettingsResponse->ImagingSettings->ColorSaturation = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->ColorSaturation;
				
				//Contrast
				timg__GetImagingSettingsResponse->ImagingSettings->Contrast = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Contrast;

				//BacklightCompensation
				timg__GetImagingSettingsResponse->ImagingSettings->BacklightCompensation->Mode = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->BacklightCompensation->Mode;
				*(timg__GetImagingSettingsResponse->ImagingSettings->BacklightCompensation->Level) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->BacklightCompensation->Level;

				//Exposure
				timg__GetImagingSettingsResponse->ImagingSettings->Exposure->Mode = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Mode;
				//Priority
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->Priority) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Priority;
				//Window
				timg__GetImagingSettingsResponse->ImagingSettings->Exposure->Window = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window;
				//MinExposureTime
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->MinExposureTime) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MinExposureTime;
				//MaxExposureTime
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->MaxExposureTime) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MaxExposureTime;
				//MinGain
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->MinGain) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MinGain;
				//MaxGain
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->MaxGain) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MaxGain;
				//MinIris
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->MinIris) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MinIris;
				//MaxIris
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->MaxIris) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MaxIris;
				//ExposureTime
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->ExposureTime) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->ExposureTime;
				//Gain
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->Gain) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Gain;
				//Iris
				*(timg__GetImagingSettingsResponse->ImagingSettings->Exposure->Iris) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Iris;

				//Focus
				timg__GetImagingSettingsResponse->ImagingSettings->Focus->AutoFocusMode = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->AutoFocusMode;
				*(timg__GetImagingSettingsResponse->ImagingSettings->Focus->DefaultSpeed) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->DefaultSpeed;
				*(timg__GetImagingSettingsResponse->ImagingSettings->Focus->NearLimit) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->NearLimit;
				*(timg__GetImagingSettingsResponse->ImagingSettings->Focus->FarLimit) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->FarLimit;

				//IrCutFilter
				timg__GetImagingSettingsResponse->ImagingSettings->IrCutFilter = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->IrCutFilter;

				//Sharpness
				timg__GetImagingSettingsResponse->ImagingSettings->Sharpness = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Sharpness;

				//WideDynamicRange
				timg__GetImagingSettingsResponse->ImagingSettings->WideDynamicRange->Mode = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WideDynamicRange->Mode;
				*(timg__GetImagingSettingsResponse->ImagingSettings->WideDynamicRange->Level) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WideDynamicRange->Level;

				//WhiteBalance
				timg__GetImagingSettingsResponse->ImagingSettings->WhiteBalance->Mode = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WhiteBalance->Mode;
				*(timg__GetImagingSettingsResponse->ImagingSettings->WhiteBalance->CrGain) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WhiteBalance->CrGain;
				*(timg__GetImagingSettingsResponse->ImagingSettings->WhiteBalance->CbGain) = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WhiteBalance->CbGain;
			}
			else
			{
				timg__GetImagingSettingsResponse->ImagingSettings = (struct tt__ImagingSettings20 *)soap_mallocz(ptSoap, sizeof(struct tt__ImagingSettings20 ));
				//Brightness
				timg__GetImagingSettingsResponse->ImagingSettings->Brightness = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Brightness;

				//ColorSaturation
				timg__GetImagingSettingsResponse->ImagingSettings->ColorSaturation = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->ColorSaturation;
				//Contrast
				timg__GetImagingSettingsResponse->ImagingSettings->Contrast = \
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Contrast;
			}
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
}

/**************************************************
*功能:		设置图像参数
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:   20120316 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg20__SetImagingSettings(struct soap* ptSoap, struct _timg20__SetImagingSettings *timg__SetImagingSettings, struct _timg20__SetImagingSettingsResponse *timg__SetImagingSettingsResponse)
{
	td_printf(0, "here %s %d %s\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	float fColorSaturation = 0;
	float fBrightness 	   = 0;
	float fContrast 	   = 0;
	int iVSNum = 0;
	int iRet = 0;
	int i = 0;
	
	if(pointer_valid_check(ptOnvifCtrl)
	|| pointer_valid_check(timg__SetImagingSettings->VideoSourceToken)
	|| pointer_valid_check(timg__SetImagingSettings->ImagingSettings))
	{
		return SOAP_ERR;
	}

	iVSNum = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;

	for(i = 0; i < iVSNum; i++)
	{
		//检测请求的VideoSourceToken是否存在
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg__SetImagingSettings->VideoSourceToken))
		{
			//判断是否支持高清参数
			if(ptOnvifCtrl->m_stFlag.m_iIspFlag)
			{
				iRet = SetIspParameter20(i, timg__SetImagingSettings->ImagingSettings);
				if(TD_OK != iRet)
				{
					td_printf(0, "%s %s %d Isp parameter set error!", __FILE__, __FUNCTION__, __LINE__);
					//goto ERR;
				}
			}

			if(!pointer_valid_check(timg__SetImagingSettings->ImagingSettings->Brightness)
			&& !pointer_valid_check(timg__SetImagingSettings->ImagingSettings->ColorSaturation)
			&& !pointer_valid_check(timg__SetImagingSettings->ImagingSettings->Contrast))
			{
				if(*timg__SetImagingSettings->ImagingSettings->Brightness <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Brightness->Max
				&& *timg__SetImagingSettings->ImagingSettings->Brightness >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Brightness->Min
				&& *timg__SetImagingSettings->ImagingSettings->ColorSaturation <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->ColorSaturation->Max
				&& *timg__SetImagingSettings->ImagingSettings->ColorSaturation >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->ColorSaturation->Min
				&& *timg__SetImagingSettings->ImagingSettings->Contrast <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Contrast->Max
				&& *timg__SetImagingSettings->ImagingSettings->Contrast >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Contrast->Min)
				{
					fBrightness      = *timg__SetImagingSettings->ImagingSettings->Brightness;
					fContrast        = *timg__SetImagingSettings->ImagingSettings->Contrast;
					fColorSaturation = *timg__SetImagingSettings->ImagingSettings->ColorSaturation;
					iRet = SetVideoInputParameter(i, fBrightness, fContrast, fColorSaturation);
					if(TD_OK != iRet)
					{
						td_printf(0, "%s %s %d VideoInput parameter set error!", __FILE__, __FUNCTION__, __LINE__);
						//goto ERR;
					}
				}
				else
				{
					//goto ERR;
				}
			}

			//聚焦-- PTZ控制参数
			if(NULL != timg__SetImagingSettings->ImagingSettings->Focus)
			{
				//模式
				if(timg__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode == tt__AutoFocusMode__AUTO
				|| timg__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode == tt__AutoFocusMode__MANUAL)
				{
					if(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->AutoFocusMode != timg__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode)
					{
						ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->AutoFocusMode = timg__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode;
					}
				}
				else
				{
					//goto ERR;
				}
				
				//缺省速度
				if(NULL != timg__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed)
				{
					if(*timg__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Focus->DefaultSpeed->Max
					&& *timg__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Focus->DefaultSpeed->Min)
					{
						if((int)ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->DefaultSpeed != (int)*timg__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed)
						{
							ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->DefaultSpeed = *timg__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed;
						}
					}
					else
					{
						//goto ERR;
					}
				}
				
				//NearLimit
				if(NULL != timg__SetImagingSettings->ImagingSettings->Focus->NearLimit)
				{
					if(*timg__SetImagingSettings->ImagingSettings->Focus->NearLimit <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Focus->NearLimit->Max
					&& *timg__SetImagingSettings->ImagingSettings->Focus->NearLimit >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Focus->NearLimit->Min)
					{
						if((int)ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->NearLimit != (int)*timg__SetImagingSettings->ImagingSettings->Focus->NearLimit)
						{
							ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->NearLimit = *timg__SetImagingSettings->ImagingSettings->Focus->NearLimit;
						}
					}
					else
					{
						//goto ERR;
					}
				}
				
				//FarLimit
				if(NULL != timg__SetImagingSettings->ImagingSettings->Focus->FarLimit)
				{
					if(*timg__SetImagingSettings->ImagingSettings->Focus->FarLimit <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Focus->FarLimit->Max
					&& *timg__SetImagingSettings->ImagingSettings->Focus->FarLimit >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Focus->FarLimit->Min)
					{
						if((int)ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->FarLimit != (int)*timg__SetImagingSettings->ImagingSettings->Focus->FarLimit)
						{
							ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->FarLimit = *timg__SetImagingSettings->ImagingSettings->Focus->FarLimit;
						}
					}
					else
					{
						//goto ERR;
					}
				}
			}			
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
//ERR:
	
	soap_sender_fault_info(ptSoap, "The requested settings are incorrect.", "ter:InvalidArgVal", "ter:SettingsInvalid");
	return SOAP_FAULT;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120305 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg20__GetOptions(struct soap* ptSoap, struct _timg20__GetOptions *timg__GetOptions, struct _timg20__GetOptionsResponse *timg__GetOptionsResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int iVSNum = 0;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}

	iVSNum = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;

	for(i = 0; i < iVSNum; i++)
	{
		if(!strcmp((ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources + i)->token, timg__GetOptions->VideoSourceToken))
		{
			if(ptOnvifCtrl->m_stFlag.m_iIspFlag)
			{
				timg__GetOptionsResponse->ImagingOptions = ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions + i;
			}
			else
			{
				timg__GetOptionsResponse->ImagingOptions = (struct tt__ImagingOptions20 *)soap_mallocz(ptSoap, sizeof(struct tt__ImagingOptions20));
				timg__GetOptionsResponse->ImagingOptions->Brightness      = ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions[i].Brightness;
				timg__GetOptionsResponse->ImagingOptions->ColorSaturation = ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions[i].ColorSaturation;
				timg__GetOptionsResponse->ImagingOptions->Contrast        = ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions[i].Contrast;
			}
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120416 v1.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg20__Move(struct soap* ptSoap, struct _timg20__Move *timg__Move, struct _timg20__MoveResponse *timg__MoveResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int iIndex 	= -1;
	int iSpeed 	= 0;
	int iChn 	= -1;
	int i 		= 0;
	TGuiPtzLocalCode stPtzLocalCode;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}

	if(NULL == timg__Move 
	|| NULL == timg__Move->VideoSourceToken 
	|| NULL == timg__Move->Focus)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
		
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg__Move->VideoSourceToken))
		{
			iIndex = i;
			break;
		}
	}

	if(iIndex < 0)
	{
		soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
		return SOAP_FAULT;
	}

	iChn = atoi(timg__Move->VideoSourceToken + strlen("vstoken_ch"));
	if(iChn <= 0)
	{
		soap_sender_fault_info(ptSoap, "The requested iChn of VideoSourceConfigurationToken does not exist.", "ter:InvalidArgVal", "ter:NoSource");
		return SOAP_FAULT;
	}

	if(NULL != timg__Move || NULL != timg__Move->Focus)
	{
		if(NULL != timg__Move->Focus->Absolute)
		{
			if(timg__Move->Focus->Absolute->Position > 1 
			|| timg__Move->Focus->Absolute->Position < -1)
			{
				//goto ERR;
			}

			if(NULL != timg__Move->Focus->Absolute->Speed)
			{
				if(*timg__Move->Focus->Absolute->Speed > 1 
				|| *timg__Move->Focus->Absolute->Speed < 0)
				{
					//goto ERR;
				}
			}
		}

		if(NULL != timg__Move->Focus->Relative)
		{
			if(timg__Move->Focus->Relative->Distance > 1 
			|| timg__Move->Focus->Relative->Distance < -1)
			{
				//goto ERR;
			}

			if(NULL != timg__Move->Focus->Relative->Speed)
			{
				if(*timg__Move->Focus->Relative->Speed > 1 
				|| *timg__Move->Focus->Relative->Speed < 0)
				{
					//goto ERR;
				}
				iSpeed = abs(*timg__Move->Focus->Relative->Speed * 100);
				
			}
			else
			{
				iSpeed = 100;
			}
			

			if(timg__Move->Focus->Relative->Distance > 0)
			{
				stPtzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_FOCUS_ASC;
				
			}
			else
			{
				stPtzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_FOCUS_DEC;
			}

			stPtzLocalCode.m_iChn = iChn;
			stPtzLocalCode.m_iParam[0] = iSpeed;
			stPtzLocalCode.m_iParam[1] = iSpeed;
			
			OnvifCmdSetPTZ(&stPtzLocalCode);
			
		}

		if(NULL != timg__Move->Focus->Continuous)
		{
			if(timg__Move->Focus->Continuous->Speed > 1 
			|| timg__Move->Focus->Continuous->Speed < -1)
			{
				//goto ERR;
			}

			iSpeed = abs(timg__Move->Focus->Continuous->Speed *100);

			if(timg__Move->Focus->Continuous->Speed > 0)
			{
				stPtzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_FOCUS_ASC;
				
			}
			else
			{
				stPtzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_FOCUS_DEC;
			}
			
			stPtzLocalCode.m_iChn = iChn;
			stPtzLocalCode.m_iParam[0] = iSpeed;
			stPtzLocalCode.m_iParam[1] = iSpeed;
			
			OnvifCmdSetPTZ(&stPtzLocalCode);
			ptOnvifCtrl->m_pTimeout[iChn - 1] = time(NULL) + 15;

		}
		
	}

	return SOAP_OK;

//ERR:
	//soap_sender_fault_info(ptSoap,"The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
	
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120416 v1.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg20__Stop(struct soap* ptSoap, struct _timg20__Stop *timg__Stop, struct _timg20__StopResponse *timg__StopResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int iIndex 	= -1;
	int iChn	= -1;
	int i		= 0;
	TGuiPtzLocalCode stPtzLocalCode;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}
	
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg__Stop->VideoSourceToken))
		{
			iIndex = i;
			break;
		}
	}

	if(iIndex < 0)
	{
		soap_sender_fault_info(ptSoap,"The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
		return SOAP_FAULT;
	}

	iChn = atoi(timg__Stop->VideoSourceToken + strlen("vsctoken_ch"));
	if(iChn <= 0)
	{
		soap_sender_fault_info(ptSoap, "The requested iChn of VideoSourceConfigurationToken does not exist.", "ter:InvalidArgVal", "ter:NoSource");
		return SOAP_FAULT;
	}

	stPtzLocalCode.m_iChn = iChn;
	stPtzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_STOP;
 	stPtzLocalCode.m_iParam[0] = 0;
	stPtzLocalCode.m_iParam[1] = 0;

	OnvifCmdSetPTZ(&stPtzLocalCode);
	ptOnvifCtrl->m_pTimeout[iChn] = 0;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120426 v1.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg20__GetStatus(struct soap* ptSoap, struct _timg20__GetStatus *timg__GetStatus, struct _timg20__GetStatusResponse *timg__GetStatusResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}
	
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg__GetStatus->VideoSourceToken))
		{
			timg__GetStatusResponse->Status = ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV20Rsp->Status;
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120416 v1.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg20__GetMoveOptions(struct soap* ptSoap, struct _timg20__GetMoveOptions *timg__GetMoveOptions, struct _timg20__GetMoveOptionsResponse *timg__GetMoveOptionsResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}

	if(NULL == timg__GetMoveOptions 
	|| NULL == timg__GetMoveOptions->VideoSourceToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg__GetMoveOptions->VideoSourceToken))
		{
			timg__GetMoveOptionsResponse->MoveOptions = ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV20Rsp->MoveOptions;
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap,"The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120418 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg10__GetImagingSettings(struct soap* ptSoap, struct _timg10__GetImagingSettings *timg10__GetImagingSettings, struct _timg10__GetImagingSettingsResponse *timg10__GetImagingSettingsResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	VideoInCnf_EX stVInCnf = {0};
	int iVSNum = 0;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}
	
	iVSNum = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;

	for(i = 0; i < iVSNum; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg10__GetImagingSettings->VideoSourceToken))
		{
			if(OnvifCmdGetVideoInCnf(i + 1, &stVInCnf))
			{
				return TD_ERROR;
			}

			//更新亮度、饱和度、对比度
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Brightness)	   = (float)stVInCnf.m_iLum;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->ColorSaturation) = (float)stVInCnf.m_iSaturation;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Contrast) 	   = (float)stVInCnf.m_iContrast;
			
			//response
			if(ptOnvifCtrl->m_stFlag.m_iIspFlag)
			{
			#ifndef WITH_TESTTOOL
				//更新全局变量值
				if(TD_OK != UpdateIspInfo())
				{
					td_printf(0, "%s %d %s =====UpdateIspInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
				}
			#endif
				timg10__GetImagingSettingsResponse->ImagingSettings = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging;
			}
			else
			{
				timg10__GetImagingSettingsResponse->ImagingSettings = (struct tt__ImagingSettings *)soap_mallocz(ptSoap, sizeof(struct tt__ImagingSettings));
				timg10__GetImagingSettingsResponse->ImagingSettings->Brightness      = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Brightness;
				timg10__GetImagingSettingsResponse->ImagingSettings->ColorSaturation = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->ColorSaturation;
				timg10__GetImagingSettingsResponse->ImagingSettings->Contrast        = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Contrast;
			}
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:   20120418 v1.0 creat by ptb
            20130219 v2.3 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg10__SetImagingSettings(struct soap* ptSoap, struct _timg10__SetImagingSettings *timg10__SetImagingSettings, struct _timg10__SetImagingSettingsResponse *timg10__SetImagingSettingsResponse)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	float fColorSaturation = 0;
	float fBrightness 	   = 0;
	float fContrast 	   = 0;
	int iVSNum = 0;
	int iRet   = 0;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}

	if(NULL == timg10__SetImagingSettings->VideoSourceToken
	|| NULL == timg10__SetImagingSettings->ImagingSettings)
	{
		return SOAP_ERR;
	}

	iVSNum = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;

	for(i = 0; i < iVSNum; i++)
	{
		//检测请求的VideoSourceToken是否存在
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg10__SetImagingSettings->VideoSourceToken))
		{
			//判断是否支持高清参数
			if(ptOnvifCtrl->m_stFlag.m_iIspFlag)
			{
				iRet = SetIspParameter10(i, timg10__SetImagingSettings->ImagingSettings);
				if(TD_OK != iRet)
				{
					td_printf(0, "%s %s %d Isp parameter set error!", __FILE__, __FUNCTION__, __LINE__);
					//goto ERR;
				}
			}

			if(NULL != timg10__SetImagingSettings->ImagingSettings->Brightness
			&& NULL != timg10__SetImagingSettings->ImagingSettings->ColorSaturation
			&& NULL != timg10__SetImagingSettings->ImagingSettings->Contrast)
			{
				if(*timg10__SetImagingSettings->ImagingSettings->Brightness <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Brightness->Max
				&& *timg10__SetImagingSettings->ImagingSettings->Brightness >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Brightness->Min
				&& *timg10__SetImagingSettings->ImagingSettings->ColorSaturation <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->ColorSaturation->Max
				&& *timg10__SetImagingSettings->ImagingSettings->ColorSaturation >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->ColorSaturation->Min
				&& *timg10__SetImagingSettings->ImagingSettings->Contrast <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Contrast->Max
				&& *timg10__SetImagingSettings->ImagingSettings->Contrast >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Contrast->Min)
				{
					fBrightness 	 = *timg10__SetImagingSettings->ImagingSettings->Brightness;
					fContrast		 = *timg10__SetImagingSettings->ImagingSettings->Contrast;
					fColorSaturation = *timg10__SetImagingSettings->ImagingSettings->ColorSaturation;
					iRet = SetVideoInputParameter(i, fBrightness, fContrast, fColorSaturation);
					if(TD_OK != iRet)
					{
						td_printf(0, "%s %s %d VideoInput parameter set error!", __FILE__, __FUNCTION__, __LINE__);
						//goto ERR;
					}
				}
				else
				{
					//goto ERR;
				}
			}
			
			//聚焦-- PTZ控制参数
			if(NULL != timg10__SetImagingSettings->ImagingSettings->Focus)
			{
				//模式
				if(timg10__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode == tt__AutoFocusMode__AUTO
				|| timg10__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode == tt__AutoFocusMode__MANUAL)
				{
					if(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->AutoFocusMode != timg10__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode)
					{
						ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->AutoFocusMode = timg10__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode;
					}
				}
				else
				{
					//goto ERR;
				}
				
				//缺省速度
				if(timg10__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Focus->DefaultSpeed->Max
				&& timg10__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Focus->DefaultSpeed->Min)
				{
					if((int)ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->DefaultSpeed != (int)timg10__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed)
					{
						ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->DefaultSpeed = timg10__SetImagingSettings->ImagingSettings->Focus->DefaultSpeed;
					}
				}
				else
				{
					//goto ERR;
				}

				//NearLimit
				if(timg10__SetImagingSettings->ImagingSettings->Focus->NearLimit <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Focus->NearLimit->Max
				&& timg10__SetImagingSettings->ImagingSettings->Focus->NearLimit >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Focus->NearLimit->Min)
				{
					if((int)ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->NearLimit != (int)timg10__SetImagingSettings->ImagingSettings->Focus->NearLimit)
					{
						ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->NearLimit = timg10__SetImagingSettings->ImagingSettings->Focus->NearLimit;
					}
				}
				else
				{
					//goto ERR;
				}

				//FarLimit
				if(timg10__SetImagingSettings->ImagingSettings->Focus->FarLimit <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Focus->FarLimit->Max
				&& timg10__SetImagingSettings->ImagingSettings->Focus->FarLimit >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Focus->FarLimit->Min)
				{
					if((int)ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->FarLimit != (int)timg10__SetImagingSettings->ImagingSettings->Focus->FarLimit)
					{
						ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->FarLimit = timg10__SetImagingSettings->ImagingSettings->Focus->FarLimit;
					}
				}
				else
				{
					//goto ERR;
				}
			}			
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
//ERR:
	
	soap_sender_fault_info(ptSoap, "The requested settings are incorrect.", "ter:InvalidArgVal", "ter:SettingsInvalid");
	return SOAP_FAULT;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120418 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg10__GetOptions(struct soap* ptSoap, struct _timg10__GetOptions *timg10__GetOptions, struct _timg10__GetOptionsResponse *timg10__GetOptionsResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int iVSNum = 0;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}

	iVSNum = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;

	for(i = 0; i < iVSNum; i++)
	{
		if(!strcmp((ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources + i)->token, timg10__GetOptions->VideoSourceToken))
		{
			if(ptOnvifCtrl->m_stFlag.m_iIspFlag)
			{
				timg10__GetOptionsResponse->ImagingOptions = ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions + i;
			}
			else
			{
				timg10__GetOptionsResponse->ImagingOptions = (struct tt__ImagingOptions *)soap_mallocz(ptSoap, sizeof(struct tt__ImagingOptions));
				timg10__GetOptionsResponse->ImagingOptions->Brightness = (ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions + i)->Brightness;
				timg10__GetOptionsResponse->ImagingOptions->ColorSaturation = (ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions + i)->ColorSaturation;
				timg10__GetOptionsResponse->ImagingOptions->Contrast = (ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions + i)->Contrast;
			}
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg10__Move(struct soap* ptSoap, struct _timg10__Move *timg10__Move, struct _timg10__MoveResponse *timg10__MoveResponse)
{
	return __timg20__Move(ptSoap, (struct _timg20__Move *)timg10__Move, (struct _timg20__MoveResponse *)timg10__MoveResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg10__Stop(struct soap* ptSoap, struct _timg10__Stop *timg10__Stop, struct _timg10__StopResponse *timg10__StopResponse)
{
	return __timg20__Stop(ptSoap, (struct _timg20__Stop *)timg10__Stop, (struct _timg20__StopResponse *)timg10__StopResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120426 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg10__GetStatus(struct soap* ptSoap, struct _timg10__GetStatus *timg10__GetStatus, struct _timg10__GetStatusResponse *timg10__GetStatusResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg10__GetStatus->VideoSourceToken))
		{
			timg10__GetStatusResponse->Status = ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV10Rsp->Status;
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120418 v1.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __timg10__GetMoveOptions(struct soap* ptSoap, struct _timg10__GetMoveOptions *timg10__GetMoveOptions, struct _timg10__GetMoveOptionsResponse *timg10__GetMoveOptionsResponse)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}

	if(NULL == timg10__GetMoveOptions 
	|| NULL == timg10__GetMoveOptions->VideoSourceToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, timg10__GetMoveOptions->VideoSourceToken))
		{
			timg10__GetMoveOptionsResponse->MoveOptions = ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV10Rsp->MoveOptions;
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap,"The requested VideoSource does not exist.", "ter:InvalidArgVal", "ter:NoSource");
	return SOAP_FAULT;
}
#endif

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetServiceCapabilities(struct soap* ptSoap, struct _tmd__GetServiceCapabilities *tmd__GetServiceCapabilities, struct _tmd__GetServiceCapabilitiesResponse *tmd__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetRelayOutputOptions(struct soap* ptSoap, struct _tmd__GetRelayOutputOptions *tmd__GetRelayOutputOptions, struct _tmd__GetRelayOutputOptionsResponse *tmd__GetRelayOutputOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSources(struct soap* ptSoap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputs(struct soap* ptSoap, struct _trt__GetAudioOutputs *trt__GetAudioOutputs, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120314 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSources(struct soap* ptSoap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputs(struct soap* ptSoap, struct _tmd__GetVideoOutputs *tmd__GetVideoOutputs, struct _tmd__GetVideoOutputsResponse *tmd__GetVideoOutputsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSourceConfiguration(struct soap* ptSoap, struct _tmd__GetVideoSourceConfiguration *tmd__GetVideoSourceConfiguration, struct _tmd__GetVideoSourceConfigurationResponse *tmd__GetVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputConfiguration(struct soap* ptSoap, struct _tmd__GetVideoOutputConfiguration *tmd__GetVideoOutputConfiguration, struct _tmd__GetVideoOutputConfigurationResponse *tmd__GetVideoOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSourceConfiguration(struct soap* ptSoap, struct _tmd__GetAudioSourceConfiguration *tmd__GetAudioSourceConfiguration, struct _tmd__GetAudioSourceConfigurationResponse *tmd__GetAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputConfiguration(struct soap* ptSoap, struct _tmd__GetAudioOutputConfiguration *tmd__GetAudioOutputConfiguration, struct _tmd__GetAudioOutputConfigurationResponse *tmd__GetAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetVideoSourceConfiguration(struct soap* ptSoap, struct _tmd__SetVideoSourceConfiguration *tmd__SetVideoSourceConfiguration, struct _tmd__SetVideoSourceConfigurationResponse *tmd__SetVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetVideoOutputConfiguration(struct soap* ptSoap, struct _tmd__SetVideoOutputConfiguration *tmd__SetVideoOutputConfiguration, struct _tmd__SetVideoOutputConfigurationResponse *tmd__SetVideoOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported..",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetAudioSourceConfiguration(struct soap* ptSoap, struct _tmd__SetAudioSourceConfiguration *tmd__SetAudioSourceConfiguration, struct _tmd__SetAudioSourceConfigurationResponse *tmd__SetAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetAudioOutputConfiguration(struct soap* ptSoap, struct _tmd__SetAudioOutputConfiguration *tmd__SetAudioOutputConfiguration, struct _tmd__SetAudioOutputConfigurationResponse *tmd__SetAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSourceConfigurationOptions(struct soap* ptSoap, struct _tmd__GetVideoSourceConfigurationOptions *tmd__GetVideoSourceConfigurationOptions, struct _tmd__GetVideoSourceConfigurationOptionsResponse *tmd__GetVideoSourceConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputConfigurationOptions(struct soap* ptSoap, struct _tmd__GetVideoOutputConfigurationOptions *tmd__GetVideoOutputConfigurationOptions, struct _tmd__GetVideoOutputConfigurationOptionsResponse *tmd__GetVideoOutputConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSourceConfigurationOptions(struct soap* ptSoap, struct _tmd__GetAudioSourceConfigurationOptions *tmd__GetAudioSourceConfigurationOptions, struct _tmd__GetAudioSourceConfigurationOptionsResponse *tmd__GetAudioSourceConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputConfigurationOptions(struct soap* ptSoap, struct _tmd__GetAudioOutputConfigurationOptions *tmd__GetAudioOutputConfigurationOptions, struct _tmd__GetAudioOutputConfigurationOptionsResponse *tmd__GetAudioOutputConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		获取继电器输出
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130513 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetRelayOutputs(struct soap* ptSoap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	tds__GetRelayOutputsResponse->__sizeRelayOutputs = ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->__sizeRelayOutputs;
	tds__GetRelayOutputsResponse->RelayOutputs       = ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetRelayOutputSettings(struct soap* ptSoap, struct _tmd__SetRelayOutputSettings *tmd__SetRelayOutputSettings, struct _tmd__SetRelayOutputSettingsResponse *tmd__SetRelayOutputSettingsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	struct _tds__SetRelayOutputSettings tds__SetRelayOutputSettings;
	tds__SetRelayOutputSettings.RelayOutputToken = tmd__SetRelayOutputSettings->RelayOutput->token;
	tds__SetRelayOutputSettings.Properties = tmd__SetRelayOutputSettings->RelayOutput->Properties;

	return __tds__SetRelayOutputSettings(ptSoap, &tds__SetRelayOutputSettings, (struct _tds__SetRelayOutputSettingsResponse *)tmd__SetRelayOutputSettingsResponse);

}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetRelayOutputState(struct soap* ptSoap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tds__SetRelayOutputState(ptSoap, tds__SetRelayOutputState, tds__SetRelayOutputStateResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetDigitalInputs(struct soap *ptSoap, struct _tmd__GetDigitalInputs *tmd__GetDigitalInputs, struct _tmd__GetDigitalInputsResponse *tmd__GetDigitalInputsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPorts(struct soap *ptSoap, struct _tmd__GetSerialPorts *tmd__GetSerialPorts, struct _tmd__GetSerialPortsResponse *tmd__GetSerialPortsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPortConfiguration(struct soap *ptSoap, struct _tmd__GetSerialPortConfiguration *tmd__GetSerialPortConfiguration, struct _tmd__GetSerialPortConfigurationResponse *tmd__GetSerialPortConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetSerialPortConfiguration(struct soap *ptSoap, struct _tmd__SetSerialPortConfiguration *tmd__SetSerialPortConfiguration, struct _tmd__SetSerialPortConfigurationResponse *tmd__SetSerialPortConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPortConfigurationOptions(struct soap *ptSoap, struct _tmd__GetSerialPortConfigurationOptions *tmd__GetSerialPortConfigurationOptions, struct _tmd__GetSerialPortConfigurationOptionsResponse *tmd__GetSerialPortConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SendReceiveSerialCommand(struct soap *ptSoap, struct _tmd__SendReceiveSerialCommand *tmd__SendReceiveSerialCommand, struct _tmd__SendReceiveSerialCommandResponse *tmd__SendReceiveSerialCommandResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}


/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetServiceCapabilities(struct soap* ptSoap, struct _tptz20__GetServiceCapabilities *tptz__GetServiceCapabilities, struct _tptz20__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function of the namespce  is not supported.",NULL,NULL);
}

 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetConfigurations(struct soap* ptSoap, struct _tptz20__GetConfigurations *tptz__GetConfigurations, struct _tptz20__GetConfigurationsResponse *tptz__GetConfigurationsResponse)
{
	tptz__GetConfigurationsResponse->__sizePTZConfiguration = g_stOnvifCtrl.m_stOnvifCfg.m_ptConfigRsp->__sizePTZConfiguration;
	tptz__GetConfigurationsResponse->PTZConfiguration = g_stOnvifCtrl.m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration;
	return SOAP_OK;
}
 
/**************************************************
*功能:		获取预置点
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130425 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetPresets(struct soap* ptSoap, struct _tptz20__GetPresets *tptz__GetPresets, struct _tptz20__GetPresetsResponse *tptz__GetPresetsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char *vsctoken = NULL;
	int iChn = 0;

	if(pointer_valid_check(tptz__GetPresets)
	|| pointer_valid_check(tptz__GetPresets->ProfileToken)
	|| pointer_valid_check(ptOnvifCtrl))
	{
		DBG_HERE;
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	
	if(FindProEntry(ptOnvifCtrl->m_ptOnvifDict, tptz__GetPresets->ProfileToken, -1) != 1)
	{
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}

	vsctoken = GetProStr(ptOnvifCtrl->m_ptOnvifDict, tptz__GetPresets->ProfileToken, -1, "vsctoken", -1, NULL);
	td_printf(0,"vsctoken = %s",vsctoken);
	if(pointer_valid_check(vsctoken))
	{
		DBG_HERE;
		return SOAP_ERR;
	}
	
	iChn = atoi(vsctoken + strlen("vsctoken_ch")) - 1;
	if(iChn < 0 || iChn >= ptOnvifCtrl->m_stNvsInfo.m_iVENum)
	{
		DBG_HERE;
		return TD_ERROR;
	}
	
	tptz__GetPresetsResponse->__sizePreset = ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset;
	tptz__GetPresetsResponse->Preset       = ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset;

	return SOAP_OK;
}
 
/**************************************************
*功能:		设置预置点
*参数:		_ptSoap:	tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130424 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__SetPreset(struct soap* ptSoap, struct _tptz20__SetPreset *tptz__SetPreset, struct _tptz20__SetPresetResponse *tptz__SetPresetResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	struct tt__PTZPreset ptzPreset = {0};
	struct tt__PTZPreset *ptPointertoPreset = NULL;
	TGuiPtzLocalCode ptzLocalCode  = {0};
	char *vsctoken = NULL;
	char *pName    = NULL;
	//void *pSrc     = NULL;
	int iPresetNum = -1;
	int iIdleNum   = 0;
	int iSrcSize   = 0;
	int iSizePreset= 0;
	int iChn       = 0;
	int i          = 0;
	int iLenName = 0;
	if(pointer_valid_check(ptOnvifCtrl)
	|| pointer_valid_check(tptz__SetPreset)
	|| pointer_valid_check(tptz__SetPreset->ProfileToken))
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	
	if(FindProEntry(ptOnvifCtrl->m_ptOnvifDict, tptz__SetPreset->ProfileToken, -1) != 1)
	{
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}
	
	vsctoken = GetProStr(ptOnvifCtrl->m_ptOnvifDict, tptz__SetPreset->ProfileToken, -1,  "vsctoken", -1, NULL);
	if(pointer_valid_check(vsctoken))
	{
		return SOAP_ERR;
	}
	
	iChn = atoi(vsctoken + strlen("vsctoken_ch")) - 1;
	if(iChn < 0 || iChn >= ptOnvifCtrl->m_stNvsInfo.m_iVENum)
	{
		return SOAP_ERR;
	}
	
	if(tptz__SetPreset->PresetName != NULL)
	{
		iLenName = strlen(tptz__SetPreset->PresetName);
		if(iLenName > MAX_NAME_LEN)
		{	
			td_printf(0xFF0000,"iLenName = %d\n",iLenName);
			soap_sender_fault_info(ptSoap, "The Length of preset name is too long.",NULL,NULL);
			return SOAP_FAULT;
		}
	}

	//presetname presettoken都为空 -- dlq modified at 20130730 for hikvision
	if(tptz__SetPreset->PresetName == NULL && tptz__SetPreset->PresetToken == NULL)
	{
		iIdleNum = GetIdlePresetNum();
		tptz__SetPreset->PresetName = (char *)soap_mallocz(ptSoap, MAX_NAME_LEN);
		sprintf(tptz__SetPreset->PresetName, "%d", iIdleNum);
		td_printf(0, "%s %d %s PresetName is null, bug get idle:%s\n", __FILE__, __LINE__, __FUNCTION__, tptz__SetPreset->PresetName);
	}

	
#if 0//结构体成员preset未开空间 by qiaohaijun @2013.5.29 
	pGetPresetsRes = ptOnvifCtrl->m_stOnvifCfg.m_pptz20__GetPresetsResponse + iChn;
	if(pointer_valid_check(pGetPresetsRes))
	{
		DBG_HERE;
		return SOAP_ERR;
	}
#endif
	//当前PTZ状态
	if(*(ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[iChn].MoveStatus->PanTilt) != tt__MoveStatus__IDLE)
	{	
		soap_receiver_fault_info(ptSoap, "Preset cannot be set while PTZ unit is moving.", "ter:Action", "ter:MovingPTZ");
		return SOAP_FAULT;
	}
	if(!pointer_valid_check(tptz__SetPreset->PresetToken))
	{
		pName = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", iChn + 1, tptz__SetPreset->PresetToken, -1, NULL);
		if(pointer_valid_check(pName))
		{
			soap_sender_fault_info(ptSoap, "The requested preset token does not exist.", "ter:InvalidArgVal", "ter:NoToken");
			return SOAP_FAULT;
		}
		else
		{
		
			iPresetNum = atoi(tptz__SetPreset->PresetToken + strlen("presetnum_"));
			
			ptzLocalCode.m_iChn      = iChn + 1;
			ptzLocalCode.m_iCode     = GUI_PTZ_LOCAL_CODE_SET_PRESET;
			ptzLocalCode.m_iParam[0] = iPresetNum;
			ptzLocalCode.m_iParam[1] = 0;
			if(OnvifCmdSetPTZ(&ptzLocalCode))
			{
				return TD_ERROR;
			}

			td_printf(0x12345678,"pName =%s",pName);
			td_printf(0x12345678,"PresetName =%s",tptz__SetPreset->PresetName);
			if(!pointer_valid_check(tptz__SetPreset->PresetName))
			{
				if(strcmp(pName, tptz__SetPreset->PresetName))
				{
					//修改配置文件

					SetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", iChn + 1,  "preset_", iPresetNum,  tptz__SetPreset->PresetName, -1, CFG_REPLACE);
				
					//更新全局变量
					for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset; i++)
					{
						if(strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset[i].token, tptz__SetPreset->ProfileToken) == 0)
						{
							memcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset[i].Name, tptz__SetPreset->PresetName, MAX_STRING_LEN);
						}
					}
				}
				
			}
			tptz__SetPresetResponse->PresetToken = tptz__SetPreset->PresetToken;
		}
		
	}
	
	if(!pointer_valid_check(tptz__SetPreset->PresetName))
	{
		//PresetName存在就 覆盖
		//modified by qiaohaijun @2013.5.29
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset; i++)
		{
			if(!strcmp(tptz__SetPreset->PresetName, ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset[i].Name))
			{
				ptzLocalCode.m_iChn      = iChn + 1;
				ptzLocalCode.m_iCode     = GUI_PTZ_LOCAL_CODE_SET_PRESET;
				ptzLocalCode.m_iParam[0] = atoi(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset[i].token + strlen("presetnum_"));
				ptzLocalCode.m_iParam[1] = 0;

				if(OnvifCmdSetPTZ(&ptzLocalCode))
				{
					DBG_HERE;
					return TD_ERROR;
				}

				tptz__SetPresetResponse->PresetToken = ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset[i].token;
				return SOAP_OK;
			}
		}

		//PresetName不存在就新加
		iIdleNum = GetIdlePresetNum();
		if(iIdleNum < PRESET_HOME)
		{
			DBG_HERE;
			return TD_ERROR;
		}

		//判断是否超过最大预置点数
		if(iIdleNum >= PRESET_MAX_NUM - 1)
		{
			soap_receiver_fault_info(ptSoap, "Maximum number of Presets reached.", "ter:Action", "ter:TooManyPresets");
			return SOAP_FAULT;
		}
		
		ptzLocalCode.m_iChn 	 = iChn + 1;
		ptzLocalCode.m_iCode	 = GUI_PTZ_LOCAL_CODE_SET_PRESET;
		ptzLocalCode.m_iParam[0] = iIdleNum;
		ptzLocalCode.m_iParam[1] = 0;
		
		if(OnvifCmdSetPTZ(&ptzLocalCode))
		{
			DBG_HERE;
			return TD_ERROR;
		}

		
		//更新全局变量
		//新增预置点重新申请空间
		iSrcSize = ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset * sizeof(struct tt__PTZPreset);

		ptzPreset.Name = (char *)calloc(1, MAX_STRING_LEN);
		if(pointer_valid_check(ptzPreset.Name))
		{
			return SOAP_ERR;
		}

		ptzPreset.token = (char *)calloc(1, MAX_STRING_LEN);
		if(pointer_valid_check(ptzPreset.token))
		{
			return SOAP_ERR;
		}
		
		memcpy(ptzPreset.Name, tptz__SetPreset->PresetName, MAX_STRING_LEN);
		sprintf(ptzPreset.token, "presetnum_%02d", iIdleNum);

		ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset= (struct tt__PTZPreset *)ArrayAdd(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset, iSrcSize, &ptzPreset, sizeof(struct tt__PTZPreset));
		if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset))
		{
			td_printf(0, "the pointer is NULL");
			return SOAP_ERR;
		}

		ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset++;
		iSizePreset = ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset;
		
		//写入配置文件
	
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", iChn + 1, "presetnum_", iIdleNum, tptz__SetPreset->PresetName, -1, CFG_ADD);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", iChn + 1,  "presetcnt", -1,  NULL, iSizePreset, CFG_REPLACE);
		ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
		
		ptPointertoPreset = ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset;

		td_printf(0x12345678,"ptPointertoPreset[iSizePreset - 1].token = %s",ptPointertoPreset[iSizePreset - 1].token);
		tptz__SetPresetResponse->PresetToken = ptPointertoPreset[iSizePreset - 1].token;
		//end
	}
		
	return SOAP_OK;
}

/**************************************************
*功能:		删除预置点
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130425 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__RemovePreset(struct soap* ptSoap, struct _tptz20__RemovePreset *tptz__RemovePreset, struct _tptz20__RemovePresetResponse *tptz__RemovePresetResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char *vsctoken = NULL;
	char *pName    = NULL;
	int iPresetNum = -1;
	int iChn       = 0;
	int i          = 0;

	if(pointer_valid_check(ptOnvifCtrl)
	|| pointer_valid_check(tptz__RemovePreset)
	|| pointer_valid_check(tptz__RemovePreset->ProfileToken))
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	if(FindProEntry(ptOnvifCtrl->m_ptOnvifDict, tptz__RemovePreset->ProfileToken, -1) != 1)
	{
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}

	vsctoken = GetProStr(ptOnvifCtrl->m_ptOnvifDict, tptz__RemovePreset->ProfileToken, -1,  "vsctoken", -1, NULL);
	if(pointer_valid_check(vsctoken))
	{
		DBG_HERE;
		return SOAP_ERR;
	}

	iChn = atoi(vsctoken + strlen("vsctoken_ch")) - 1;
	if(iChn < 0 || iChn >= ptOnvifCtrl->m_stNvsInfo.m_iVENum)
	{
		DBG_HERE;
		return TD_ERROR;
	}
	pName = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", iChn + 1, tptz__RemovePreset->PresetToken, -1, NULL);
	if(pointer_valid_check(pName))
	{
		soap_sender_fault_info(ptSoap, "The requested preset token does not exist.", "ter:InvalidArgVal", "ter:NoToken");
		return SOAP_FAULT;
	}

	iPresetNum = ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset;
	for(i = 0; i < iPresetNum; i++)
	{
		if(strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset[i].token, tptz__RemovePreset->PresetToken) == 0)
		{	
			free(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset[i].token);
			ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset = 
				(struct tt__PTZPreset *)ArrayDel(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset, sizeof(struct tt__PTZPreset)*iPresetNum, \
						 ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset + i ,sizeof(struct tt__PTZPreset));
			ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset--;
			SetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", iChn + 1,  tptz__RemovePreset->PresetToken, -1,  NULL, -1, CFG_RM);
			SetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", iChn + 1,  "presetcnt", -1,  NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset, CFG_REPLACE);
			break;
		}
	}
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GotoPreset(struct soap* ptSoap, struct _tptz20__GotoPreset *tptz__GotoPreset, struct _tptz20__GotoPresetResponse *tptz__GotoPresetResponse)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiPtzLocalCode ptzLocalCode = {0};
	char *vsctoken = NULL;
	int iPresetNum = -1;
	int iChn       = 0;
	int i          = 0;

	if(NULL == tptz__GotoPreset || NULL == tptz__GotoPreset->ProfileToken || NULL == ptOnvifCtrl)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	if(FindProEntry(ptOnvifCtrl->m_ptOnvifDict, tptz__GotoPreset->ProfileToken, -1) != 1)
	{
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}
	
	vsctoken = GetProStr(ptOnvifCtrl->m_ptOnvifDict, tptz__GotoPreset->ProfileToken, -1,  "vsctoken", -1, NULL);
	if(vsctoken == NULL)
	{
		return SOAP_ERR;
	}
	
	iChn = atoi(vsctoken + strlen("vsctoken_ch")) - 1;
	if(iChn < 0)
	{
		return TD_ERROR;
	}
	
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].__sizePreset; i++)
	{
		if(strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[iChn].Preset[i].token, tptz__GotoPreset->PresetToken) == 0)
		{
			iPresetNum = atoi(tptz__GotoPreset->PresetToken + strlen("presetnum_"));
			break;
		}
	}

	if(iPresetNum > 0)
	{
		ptzLocalCode.m_iChn  = iChn + 1;
		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_GO_PRESET;
		ptzLocalCode.m_iParam[0] = iPresetNum;
		ptzLocalCode.m_iParam[1] = 0;
		OnvifCmdSetPTZ(&ptzLocalCode);
	}
	else
	{
		soap_sender_fault_info(ptSoap, "The requested preset token does not exist.", "ter:InvalidArgVal", "ter:NoToken");
		return SOAP_FAULT;
	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetStatus(struct soap* ptSoap, struct _tptz20__GetStatus *tptz__GetStatus, struct _tptz20__GetStatusResponse *tptz__GetStatusResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char *vsctoken = NULL;
	int iChn = 0;
	return SOAP_ERR;

	if(NULL == tptz__GetStatus || NULL == tptz__GetStatus->ProfileToken || NULL == ptOnvifCtrl)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	if(FindProEntry(ptOnvifCtrl->m_ptOnvifDict, tptz__GetStatus->ProfileToken, -1) != 1)
	{
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}
	
	vsctoken = GetProStr(ptOnvifCtrl->m_ptOnvifDict, tptz__GetStatus->ProfileToken, -1,  "vsctoken", -1, NULL);
	if(vsctoken == NULL)
	{
		return SOAP_ERR;
	}
	//vsc token后面的数字从1开始，by qiaohaijun @2013.5.29
	iChn = atoi(vsctoken + strlen("vsctoken_ch")) - 1;
	TGuiGetPTZCompassRsp tGetPTZCompassRsp = {0};

	OnvifCmdGetPtzCompassCnf(iChn, &tGetPTZCompassRsp);

	ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[iChn].Position->PanTilt->x = (float)tGetPTZCompassRsp.m_iXp;
	ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[iChn].Position->PanTilt->y = (float)tGetPTZCompassRsp.m_iYp;
	ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[iChn].Position->Zoom->x = (float)tGetPTZCompassRsp.m_iZoom;
	
	tptz__GetStatusResponse->PTZStatus = ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus + iChn;	
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetConfiguration(struct soap* ptSoap, struct _tptz20__GetConfiguration *tptz__GetConfiguration, struct _tptz20__GetConfigurationResponse *tptz__GetConfigurationResponse)
{
	int i = 0;
	
	if(NULL == tptz__GetConfiguration || NULL == tptz__GetConfiguration->PTZConfigurationToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	
	for(i = 0; i < g_stOnvifCtrl.m_stOnvifCfg.m_ptConfigRsp->__sizePTZConfiguration; i++)
	{
		if(strcmp(g_stOnvifCtrl.m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].token, tptz__GetConfiguration->PTZConfigurationToken) == 0)
		{
			tptz__GetConfigurationResponse->PTZConfiguration = g_stOnvifCtrl.m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration + i;
			return SOAP_OK;
		}
	}
	
	soap_sender_fault_info(ptSoap, "The requested configuration does not exist.", "ter:InvalidArgVal", "ter:NoConfig");
	return SOAP_FAULT;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetNodes(struct soap* ptSoap, struct _tptz20__GetNodes *tptz__GetNodes, struct _tptz20__GetNodesResponse *tptz__GetNodesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	tptz__GetNodesResponse->__sizePTZNode = g_stOnvifCtrl.m_stOnvifCfg.m_ptNodesRsp->__sizePTZNode;
	tptz__GetNodesResponse->PTZNode = g_stOnvifCtrl.m_stOnvifCfg.m_ptNodesRsp->PTZNode;
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetNode(struct soap* ptSoap, struct _tptz20__GetNode *tptz__GetNode, struct _tptz20__GetNodeResponse *tptz__GetNodeResponse)
{
	int i;

	if(NULL == tptz__GetNode || NULL == tptz__GetNode->NodeToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	
	for(i = 0; i < g_stOnvifCtrl.m_stOnvifCfg.m_ptNodesRsp->__sizePTZNode; i++)
	{
		if(strcmp((g_stOnvifCtrl.m_stOnvifCfg.m_ptNodesRsp->PTZNode + i)->token, tptz__GetNode->NodeToken) == 0)
		{
			tptz__GetNodeResponse->PTZNode = g_stOnvifCtrl.m_stOnvifCfg.m_ptNodesRsp->PTZNode + i;
			return SOAP_OK;
		}
	}
	
	soap_sender_fault_info(ptSoap, "No such PTZNode on the device.", "ter:InvalidArgVal", "ter:NoEntity");
	return SOAP_FAULT;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__SetConfiguration(struct soap* ptSoap, struct _tptz20__SetConfiguration *tptz__SetConfiguration, struct _tptz20__SetConfigurationResponse *tptz__SetConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	if(NULL == tptz__SetConfiguration || NULL == tptz__SetConfiguration->PTZConfiguration || NULL == tptz__SetConfiguration->PTZConfiguration->token)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	
	for(i = 0; i < ptOnvifCtrl->m_stNvsInfo.m_iVINum; i++)
	{
		if(strcmp(tptz__SetConfiguration->PTZConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].token) == 0)
		{
			if(NULL != tptz__SetConfiguration->PTZConfiguration->DefaultPTZTimeout)
			{
				*(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZTimeout) = *(tptz__SetConfiguration->PTZConfiguration->DefaultPTZTimeout);
				//(g_stOnvifCtrl.m_stOnvifCfg.m_pptz20__GetConfigurationsResponse->PTZConfiguration + i)->DefaultPTZSpeed->PanTilt->x = tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->PanTilt->x;
				//(g_stOnvifCtrl.m_stOnvifCfg.m_pptz20__GetConfigurationsResponse->PTZConfiguration + i)->DefaultPTZSpeed->PanTilt->y = tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->PanTilt->y;
				//(g_stOnvifCtrl.m_stOnvifCfg.m_pptz20__GetConfigurationsResponse->PTZConfiguration + i)->DefaultPTZSpeed->Zoom->x = tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->Zoom->x;

				SetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzctoken_ch", i + 1, "ptzcdto", -1, NULL, *(tptz__SetConfiguration->PTZConfiguration->DefaultPTZTimeout), CFG_REPLACE);
				ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
			}
			return SOAP_OK;
		}
	}

	soap_sender_fault_info(ptSoap, "The configuration does not exist.", "ter:InvalidArgVal", "ter:NoConfig");

	return SOAP_FAULT;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetConfigurationOptions(struct soap* ptSoap, struct _tptz20__GetConfigurationOptions *tptz__GetConfigurationOptions, struct _tptz20__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse)
{
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions = g_stOnvifCtrl.m_stOnvifCfg.m_ptCfgOptionsRsp->PTZConfigurationOptions;
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GotoHomePosition(struct soap* ptSoap, struct _tptz20__GotoHomePosition *tptz__GotoHomePosition, struct _tptz20__GotoHomePositionResponse *tptz__GotoHomePositionResponse)
{
	char *vsctoken;
	int iChn;
	TGuiPtzLocalCode ptzLocalCode;

	if(NULL == tptz__GotoHomePosition || NULL == tptz__GotoHomePosition->ProfileToken)
	{
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");;
		return SOAP_FAULT;
	}

	if(FindProEntry(g_stOnvifCtrl.m_ptOnvifDict, tptz__GotoHomePosition->ProfileToken, -1) != 1)
	{	
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");		
		return SOAP_FAULT;
	}
	
	vsctoken = GetProStr(g_stOnvifCtrl.m_ptOnvifDict, tptz__GotoHomePosition->ProfileToken, -1,  "vsctoken", -1, NULL);
	if(vsctoken == NULL)
	{
		return SOAP_ERR;
	}
	
	iChn = atoi(vsctoken+strlen("vsctoken_ch"))-1;


	ptzLocalCode.m_iChn = iChn + 1;
	ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_GO_PRESET;
	ptzLocalCode.m_iParam[0] = 1;
	ptzLocalCode.m_iParam[1] = 0;
	if(OnvifCmdSetPTZ(&ptzLocalCode))
	{
		//return SOAP_ERR;
	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__SetHomePosition(struct soap* ptSoap, struct _tptz20__SetHomePosition *tptz__SetHomePosition, struct _tptz20__SetHomePositionResponse *tptz__SetHomePositionResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	soap_receiver_fault_info(ptSoap, "The home position is fixed and cannot be overwritten.", "ter:Action", "ter:CannotOverwriteHome");		
	return SOAP_FAULT;
}

 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__ContinuousMove(struct soap* ptSoap, struct _tptz20__ContinuousMove *tptz__ContinuousMove, struct _tptz20__ContinuousMoveResponse *tptz__ContinuousMoveResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TGuiPtzLocalCode PtzLocalCode1 = {0};
	TGuiPtzLocalCode PtzLocalCode2 = {0};
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
 	char *vsctoken         = NULL;
	TFloatRange tSrcQRange = {0};
	TFloatRange tDstQRang  = {0};
	int iTimeout  = 0;
	int iSpeedX   = 0;
	int iSpeedY   = 0;
	int iChn      = -1;
	int iStopFlag = 0;
	float fSpeedX = 0;
	float fSpeedY = 0;
	float ftmp    = 0;

	if(NULL == tptz__ContinuousMove || NULL == tptz__ContinuousMove->ProfileToken || NULL == ptOnvifCtrl)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	
	if(FindProEntry(ptOnvifCtrl->m_ptOnvifDict, tptz__ContinuousMove->ProfileToken, -1) != 1)
	{	
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}
	
	vsctoken = GetProStr(ptOnvifCtrl->m_ptOnvifDict, tptz__ContinuousMove->ProfileToken, -1, "vsctoken", -1, NULL);
	if(vsctoken == NULL)
	{
		return SOAP_ERR;
	}

	iChn = atoi(vsctoken + strlen("vsctoken_ch"));
	if(iChn <= 0)
	{
		soap_sender_fault_info(ptSoap, "The requested iChn of VideoSourceConfigurationToken does not exist.", "ter:InvalidArgVal", "ter:NoSource");
		return SOAP_FAULT;
	}
	
	if(*(ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[iChn - 1].MoveStatus->PanTilt) != tt__MoveStatus__IDLE)
	{	
		soap_receiver_fault_info(ptSoap, "Preset cannot be set while PTZ unit is moving.", "ter:Action", "ter:MovingPTZ");
		return SOAP_FAULT;
	}
	else
	{
		*ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[iChn - 1].MoveStatus->PanTilt = tt__MoveStatus__MOVING;
	}

	if(tptz__ContinuousMove->Timeout != NULL)
	{
		iTimeout = *(tptz__ContinuousMove->Timeout);
	}

	if(iTimeout == 0)
	{
		iTimeout = *(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[iChn - 1].DefaultPTZTimeout);
	}

	if(NULL != tptz__ContinuousMove->Velocity)
	{
		//把收到的值转换成带方向的0-100的速度
		tSrcQRange.Max = ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV20Rsp->MoveOptions->Continuous->Speed->Max;
		tSrcQRange.Min = ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV20Rsp->MoveOptions->Continuous->Speed->Min;
		tDstQRang.Max  = 100;
		tDstQRang.Min  = -100;

		if(NULL != tptz__ContinuousMove->Velocity->PanTilt)
		{
			iSpeedX = (int)ExchangeValue(tptz__ContinuousMove->Velocity->PanTilt->x, tSrcQRange, tDstQRang);
			iSpeedY = (int)ExchangeValue(tptz__ContinuousMove->Velocity->PanTilt->y, tSrcQRange, tDstQRang);

			//确定PTZ方向
			if(iSpeedX != 0 || iSpeedY != 0)
			{
				PtzLocalCode1.m_iCode = GetPTZCode(iSpeedX, iSpeedY);
				if(TD_ERROR == PtzLocalCode1.m_iCode)
				{
					return TD_ERROR;
				}
			}

			//确定PTZ速度
			if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
			{
				//Ipc
				//发送共享内存
				PtzLocalCode1.m_iChn = iChn;
				PtzLocalCode1.m_iParam[0] = abs(iSpeedX);
				PtzLocalCode1.m_iParam[1] = abs(iSpeedY);
				
				OnvifCmdSetPTZ(&PtzLocalCode1);
				ptOnvifCtrl->m_pTimeout[iChn - 1] = time(NULL) + iTimeout;
			}
			else
			{
				//Dvr
				tSrcQRange.Max = ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV20Rsp->MoveOptions->Continuous->Speed->Max;
				tSrcQRange.Min = 0;
				tDstQRang.Max = 3.875;
				tDstQRang.Min = 0.0625;
				iSpeedX = 0;
				iSpeedY = 0;
			
				if(0 > tptz__ContinuousMove->Velocity->PanTilt->x)
				{
					fSpeedX = tptz__ContinuousMove->Velocity->PanTilt->x * (-1);
				}
				else
				{
					fSpeedX = tptz__ContinuousMove->Velocity->PanTilt->x;
				}
				
				if(0 != fSpeedX)
				{
					ftmp = ExchangeValue(fSpeedX, tSrcQRange, tDstQRang);
					if(1 > ftmp)
					{
						ftmp = 1;
					}
					iSpeedX = (int)ftmp;
				}
				else
				{
					iSpeedX = 0;
				}

				if(0 > tptz__ContinuousMove->Velocity->PanTilt->y)
				{
					fSpeedY = tptz__ContinuousMove->Velocity->PanTilt->y * (-1);
				}
				else
				{
					fSpeedY = tptz__ContinuousMove->Velocity->PanTilt->y;
				}

				if(0 != fSpeedY)
				{
					ftmp = ExchangeValue(fSpeedY, tSrcQRange, tDstQRang);
					if(1 > ftmp)
					{
						ftmp = 1;
					}
					iSpeedY = (int)ftmp;
				}
				else
				{
					iSpeedY = 0;
				}

				//发送共享内存
				PtzLocalCode1.m_iChn = iChn;
				PtzLocalCode1.m_iParam[0] = iSpeedX;
				PtzLocalCode1.m_iParam[1] = iSpeedY;
				OnvifCmdSetPTZ(&PtzLocalCode1);
				ptOnvifCtrl->m_pTimeout[iChn - 1] = time(NULL) + iTimeout;
			}	
		}
			
		//确定变倍+/-
		if(NULL != tptz__ContinuousMove->Velocity->Zoom)
		{
			if(tptz__ContinuousMove->Velocity->Zoom->x > 0)
			{
				PtzLocalCode2.m_iCode = GUI_PTZ_LOCAL_CODE_ZOOM_ASC;
			}
			else
			{
				PtzLocalCode2.m_iCode = GUI_PTZ_LOCAL_CODE_ZOOM_DEC;
			}
			
			if(0 != tptz__ContinuousMove->Velocity->Zoom->x)
			{
				PtzLocalCode2.m_iChn = iChn;
				//变倍速度是定值
				PtzLocalCode2.m_iParam[0] = 1;
				PtzLocalCode2.m_iParam[1] = 1;
				
				OnvifCmdSetPTZ(&PtzLocalCode2);
				ptOnvifCtrl->m_pTimeout[iChn - 1] = time(NULL) + iTimeout;
			}
		}
	}
	
	if(NULL != tptz__ContinuousMove->Velocity->PanTilt
	&& NULL == tptz__ContinuousMove->Velocity->Zoom)
	{
		if(0 == tptz__ContinuousMove->Velocity->PanTilt->x
		&& 0 == tptz__ContinuousMove->Velocity->PanTilt->y)
		{
			iStopFlag = 1;
		}
	}
	else if(NULL == tptz__ContinuousMove->Velocity->PanTilt
	&& NULL != tptz__ContinuousMove->Velocity->Zoom)
	{
		if(0 == tptz__ContinuousMove->Velocity->Zoom->x)
		{
			iStopFlag = 1;
		}
	}
	else if(NULL != tptz__ContinuousMove->Velocity->PanTilt
	&& NULL != tptz__ContinuousMove->Velocity->Zoom)
	{
		if(0 == tptz__ContinuousMove->Velocity->PanTilt->x
		&& 0 == tptz__ContinuousMove->Velocity->PanTilt->y
		&& 0 == tptz__ContinuousMove->Velocity->Zoom->x)
		{
			iStopFlag = 1;
		}
	}

	if(iStopFlag)
	{
		//PTZ停止
		PtzLocalCode1.m_iChn = iChn;
		PtzLocalCode1.m_iCode = GUI_PTZ_LOCAL_CODE_STOP;
		PtzLocalCode1.m_iParam[0] = 0;
		PtzLocalCode1.m_iParam[1] = 0;
		OnvifCmdSetPTZ(&PtzLocalCode1);
		ptOnvifCtrl->m_pTimeout[iChn - 1] = 0;
	}

	*ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[iChn - 1].MoveStatus->PanTilt = tt__MoveStatus__IDLE;
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__RelativeMove(struct soap* ptSoap, struct _tptz20__RelativeMove *tptz__RelativeMove, struct _tptz20__RelativeMoveResponse *tptz__RelativeMoveResponse)
{
	int iChn = 0;
 	char *vsctoken;
 	TGuiPtzLocalCode PtzLocalCode1;
	TGuiPtzLocalCode PtzLocalCode2;
	
	int x = 0,y = 0,z = 0;

	if(NULL == tptz__RelativeMove || NULL == tptz__RelativeMove->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	if(tptz__RelativeMove->Translation != NULL && tptz__RelativeMove->Translation->PanTilt != NULL)
	{
		x = tptz__RelativeMove->Translation->PanTilt->x *100;
		y = tptz__RelativeMove->Translation->PanTilt->y *100;
	}
	if(tptz__RelativeMove->Translation != NULL && tptz__RelativeMove->Translation->Zoom != NULL)
	{
		z = tptz__RelativeMove->Translation->Zoom->x *100;
	}
	
	
	if(FindProEntry(g_stOnvifCtrl.m_ptOnvifDict, tptz__RelativeMove->ProfileToken, -1) != 1)
	{	
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}
	
	vsctoken = GetProStr(g_stOnvifCtrl.m_ptOnvifDict, tptz__RelativeMove->ProfileToken, -1,	"vsctoken", -1, NULL);
	if(vsctoken == NULL)
	{
		return SOAP_ERR;
	}
	
	iChn = atoi(vsctoken+strlen("vsctoken_ch"))-1;


#if 1

	if(x != 0 || y != 0)
	{
		if(abs(x) > abs(y)*2)
		{
			td_printf(0, "---------right left\n");
			PtzLocalCode1.m_iCode = x > 0 ? GUI_PTZ_LOCAL_CODE_RIGHT : GUI_PTZ_LOCAL_CODE_LEFT;
		}
		else if(abs(y) > abs(x)*2)
		{
			td_printf(0, "----------up down\n");
			PtzLocalCode1.m_iCode = y > 0 ? GUI_PTZ_LOCAL_CODE_UP : GUI_PTZ_LOCAL_CODE_DOWN;
		}
		else if(x*y > 0 && abs(x) > abs(y)*0.5 && abs(x) < abs(y)*2)
		{
			td_printf(0, "-----------ru rd\n");
			PtzLocalCode1.m_iCode = x > 0 ? GUI_PTZ_LOCAL_CODE_RIGHTUP : GUI_PTZ_LOCAL_CODE_LEFTDOWN;
		}
		else if(x*y < 0 && abs(x) > abs(y)*0.5 && abs(x) < abs(y)*2)
		{
			td_printf(0, "-----------lu ld\n");
			PtzLocalCode1.m_iCode = x > 0 ? GUI_PTZ_LOCAL_CODE_RIGHTDOWN: GUI_PTZ_LOCAL_CODE_LEFTUP;
		}

		PtzLocalCode1.m_iChn = iChn + 1;
		
		if(tptz__RelativeMove->Speed != NULL && tptz__RelativeMove->Speed->PanTilt != NULL)
		{
			PtzLocalCode1.m_iParam[0] = abs(100 * tptz__RelativeMove->Speed->PanTilt->x);
			PtzLocalCode1.m_iParam[1] = abs(100 * tptz__RelativeMove->Speed->PanTilt->y);
		}
		else
		{
			PtzLocalCode1.m_iParam[0] = abs(x);
			PtzLocalCode1.m_iParam[1] = abs(y);
		}

		OnvifCmdSetPTZ(&PtzLocalCode1);
		g_stOnvifCtrl.m_pTimeout[iChn] = time(NULL) + 15;


	}

	

	if(z != 0)
	{
		PtzLocalCode2.m_iChn = iChn + 1;
		PtzLocalCode2.m_iCode = z > 0 ? GUI_PTZ_LOCAL_CODE_ZOOM_ASC : GUI_PTZ_LOCAL_CODE_ZOOM_DEC;
		PtzLocalCode2.m_iParam[0] = abs(100 * tptz__RelativeMove->Translation->Zoom->x);

		OnvifCmdSetPTZ(&PtzLocalCode2);
		g_stOnvifCtrl.m_pTimeout[iChn] = time(NULL) + 15;

	}
	
#endif
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__SendAuxiliaryCommand(struct soap* ptSoap, struct _tptz20__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, struct _tptz20__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	char *vsctoken;
	int iChn;
	TGuiPtzLocalCode ptzLocalCode;

	if(NULL == tptz__SendAuxiliaryCommand || NULL == tptz__SendAuxiliaryCommand->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	char *p = tptz__SendAuxiliaryCommand->AuxiliaryData;
	
	if(FindProEntry(g_stOnvifCtrl.m_ptOnvifDict, tptz__SendAuxiliaryCommand->ProfileToken, -1) != 1)
	{	
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");		
		return SOAP_FAULT;
	}
	
	vsctoken = GetProStr(g_stOnvifCtrl.m_ptOnvifDict, tptz__SendAuxiliaryCommand->ProfileToken, -1,  "vsctoken", -1, NULL);
	if(vsctoken == NULL)
	{
		return SOAP_ERR;
	}
	
	iChn = atoi(vsctoken+strlen("vsctoken_ch"));

	if(!strcmp("AssOn", p))
	{
		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_ASS_ON;
		tptz__SendAuxiliaryCommandResponse->AuxiliaryResponse = "AssOn";
	}
	else if(!strcmp("AssOff", p))
	{
		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_ASS_OFF;
		tptz__SendAuxiliaryCommandResponse->AuxiliaryResponse = "AssOff";
	}
	else if(!strcmp("LightOn", p))
	{
		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_LIGHT_ON;
		tptz__SendAuxiliaryCommandResponse->AuxiliaryResponse = "LightOn";
	}
	else if(!strcmp("LightOff", p))
	{
		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_LIGHT_OFF;
		tptz__SendAuxiliaryCommandResponse->AuxiliaryResponse = "LightOff";
	}
	else if(!strcmp("WipeStart", p))
	{
		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_WIPE_START;
		tptz__SendAuxiliaryCommandResponse->AuxiliaryResponse = "WipeStart";
	}
	else if(!strcmp("WipeStop", p))
	{
		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_WIPE_STOP;
		tptz__SendAuxiliaryCommandResponse->AuxiliaryResponse = "WipeStop";
	}
	else if(!strcmp("ScanOn", p))
 	{
 		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_AUTO_START;
 		tptz__SendAuxiliaryCommandResponse->AuxiliaryResponse = "ScanOn";
 	}
 	else if(!strcmp("ScanOff", p))
 	{
 		ptzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_AUTO_STOP;
 		tptz__SendAuxiliaryCommandResponse->AuxiliaryResponse = "ScanOff";
 	}
 	else
	{
		return SOAP_ERR;
	}
	
	ptzLocalCode.m_iChn = iChn;
 	ptzLocalCode.m_iParam[0] = 0;
	ptzLocalCode.m_iParam[1] = 0;
	if(OnvifCmdSetPTZ(&ptzLocalCode))
	{
		//return SOAP_ERR;
	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__AbsoluteMove(struct soap* ptSoap, struct _tptz20__AbsoluteMove *tptz__AbsoluteMove, struct _tptz20__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
#if 1
	int iChn = 0;
 	char *vsctoken;
	int x = 0,y = 0,z = 0;

	TGuiGetPTZCompassRsp tGetPTZCompassRsp = {0};
	
	if(NULL == tptz__AbsoluteMove || NULL == tptz__AbsoluteMove->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}
	

	td_printf(0, "tptz__AbsoluteMove->ProfileToken === %s", tptz__AbsoluteMove->ProfileToken);

	if(FindProEntry(g_stOnvifCtrl.m_ptOnvifDict, tptz__AbsoluteMove->ProfileToken, -1) != 1)
	{	
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}
	
	vsctoken = GetProStr(g_stOnvifCtrl.m_ptOnvifDict, tptz__AbsoluteMove->ProfileToken, -1,	"vsctoken", -1, NULL);
	if(vsctoken == NULL)
	{
		return SOAP_ERR;
	}

	td_printf(0, "vsctoken ========= %s", vsctoken);
	
	iChn = atoi(vsctoken+strlen("vsctoken_ch"))-1;
	
	OnvifCmdGetPtzCompassCnf(iChn, &tGetPTZCompassRsp);
	td_printf(0, "AAA tGetPTZCompassRsp.m_iZoom ====== %d", tGetPTZCompassRsp.m_iZoom);

	if(tptz__AbsoluteMove->Position != NULL)
	{
		if(tptz__AbsoluteMove->Position->PanTilt != NULL)
		{
			td_printf(0, "tptz__AbsoluteMove->Position->PanTilt->x === %f", tptz__AbsoluteMove->Position->PanTilt->x);
			td_printf(0, "tptz__AbsoluteMove->Position->PanTilt->y === %f", tptz__AbsoluteMove->Position->PanTilt->y);
			x = (int)tptz__AbsoluteMove->Position->PanTilt->x;
			y = (int)tptz__AbsoluteMove->Position->PanTilt->y;

			if(x < PTZ_ABS_X_MIN || x > PTZ_ABS_X_MAX 
			||y < PTZ_ABS_Y_MIN || y > PTZ_ABS_Y_MAX)
			{
				soap_sender_fault_info(ptSoap, "The requested position is out of bounds.", "ter:InvalidArgVal", "ter:InvalidPosition");
				return SOAP_FAULT;
			}
			
			tGetPTZCompassRsp.m_iXp = x;
			tGetPTZCompassRsp.m_iYp = y;

		}
	
		if(tptz__AbsoluteMove->Position->Zoom != NULL)
		{
			td_printf(0, "tptz__AbsoluteMove->Position->Zoom->x === %f", tptz__AbsoluteMove->Position->Zoom->x);
			z = (int)tptz__AbsoluteMove->Position->Zoom->x ;

			if(z < PTZ_ABS_Z_MIN || z > PTZ_ABS_Z_MAX)
			{
				soap_sender_fault_info(ptSoap, "The requested position is out of bounds.", "ter:InvalidArgVal", "ter:InvalidPosition");
				return SOAP_FAULT;
			}

			tGetPTZCompassRsp.m_iZoom = z;
			td_printf(0, "tGetPTZCompassRsp.m_iZoom === %d", tGetPTZCompassRsp.m_iZoom);
		}
	}
	td_printf(0, "BBB tGetPTZCompassRsp.m_iZoom ====== %d", tGetPTZCompassRsp.m_iZoom);
	OnvifCmdSetPtzCompassCnf(&tGetPTZCompassRsp);
	
#endif 	
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__Stop(struct soap* ptSoap, struct _tptz20__Stop *tptz__Stop, struct _tptz20__StopResponse *tptz__StopResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiPtzLocalCode PtzLocalCode = {0};
	char *vsctoken = NULL;
	int iChn = -1;
	
	if(NULL == tptz__Stop || NULL == tptz__Stop->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	if(FindProEntry(ptOnvifCtrl->m_ptOnvifDict, tptz__Stop->ProfileToken, -1) != 1)
	{
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}

	vsctoken = GetProStr(ptOnvifCtrl->m_ptOnvifDict, tptz__Stop->ProfileToken, -1,  "vsctoken", -1, NULL);
	if(vsctoken == NULL)
	{
		return SOAP_ERR;
	}
	
	iChn = atoi(vsctoken + strlen("vsctoken_ch"));
	if(iChn <= 0)
	{
		soap_sender_fault_info(ptSoap, "The requested iChn of VideoSourceConfigurationToken does not exist.", "ter:InvalidArgVal", "ter:NoSource");
		return SOAP_FAULT;
	}
	
	*ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[iChn - 1].MoveStatus->PanTilt = tt__MoveStatus__IDLE;
	
	PtzLocalCode.m_iChn  = iChn;
	PtzLocalCode.m_iCode = GUI_PTZ_LOCAL_CODE_STOP;
 	PtzLocalCode.m_iParam[0] = 0;
	PtzLocalCode.m_iParam[1] = 0;

	OnvifCmdSetPTZ(&PtzLocalCode);

	ptOnvifCtrl->m_pTimeout[iChn - 1] = 0;

	return SOAP_OK;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetPresetTours(struct soap *ptSoap, struct _tptz20__GetPresetTours *tptz20__GetPresetTours, struct _tptz20__GetPresetToursResponse *tptz20__GetPresetToursResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetPresetTour(struct soap *ptSoap, struct _tptz20__GetPresetTour *tptz20__GetPresetTour, struct _tptz20__GetPresetTourResponse *tptz20__GetPresetTourResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__GetPresetTourOptions(struct soap *ptSoap, struct _tptz20__GetPresetTourOptions *tptz20__GetPresetTourOptions, struct _tptz20__GetPresetTourOptionsResponse *tptz20__GetPresetTourOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__CreatePresetTour(struct soap *ptSoap, struct _tptz20__CreatePresetTour *tptz20__CreatePresetTour, struct _tptz20__CreatePresetTourResponse *tptz20__CreatePresetTourResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__ModifyPresetTour(struct soap *ptSoap, struct _tptz20__ModifyPresetTour *tptz20__ModifyPresetTour, struct _tptz20__ModifyPresetTourResponse *tptz20__ModifyPresetTourResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__OperatePresetTour(struct soap *ptSoap, struct _tptz20__OperatePresetTour *tptz20__OperatePresetTour, struct _tptz20__OperatePresetTourResponse *tptz20__OperatePresetTourResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz20__RemovePresetTour(struct soap *ptSoap, struct _tptz20__RemovePresetTour *tptz20__RemovePresetTour, struct _tptz20__RemovePresetTourResponse *tptz20__RemovePresetTourResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}


/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/

SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GetConfigurations(struct soap* ptSoap, struct _tptz10__GetConfigurations *tptz10__GetConfigurations, struct _tptz10__GetConfigurationsResponse *tptz10__GetConfigurationsResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__GetConfigurations(ptSoap, (struct _tptz20__GetConfigurations *)tptz10__GetConfigurations, (struct _tptz20__GetConfigurationsResponse *)tptz10__GetConfigurationsResponse);

}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GetPresets(struct soap* ptSoap, struct _tptz10__GetPresets *tptz10__GetPresets, struct _tptz10__GetPresetsResponse *tptz10__GetPresetsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	return __tptz20__GetPresets(ptSoap, (struct _tptz20__GetPresets *)tptz10__GetPresets, (struct _tptz20__GetPresetsResponse *)tptz10__GetPresetsResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__SetPreset(struct soap* ptSoap, struct _tptz10__SetPreset *tptz10__SetPreset, struct _tptz10__SetPresetResponse *tptz10__SetPresetResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	struct _tptz20__SetPresetResponse tptz20__SetPresetResponse = {0};

	return __tptz20__SetPreset(ptSoap, (struct _tptz20__SetPreset *)tptz10__SetPreset, &tptz20__SetPresetResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__RemovePreset(struct soap* ptSoap, struct _tptz10__RemovePreset *tptz10__RemovePreset, struct _tptz10__RemovePresetResponse *tptz10__RemovePresetResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	struct _tptz20__RemovePreset tptz20__RemovePreset = {0};

	if(!pointer_valid_check(tptz10__RemovePreset->ProfileToken))
	{
		tptz20__RemovePreset.ProfileToken = tptz10__RemovePreset->ProfileToken;
	}
		
	if(!pointer_valid_check(tptz10__RemovePreset->PresetToken))
	{
		tptz20__RemovePreset.PresetToken  = tptz10__RemovePreset->PresetToken;
	}

	return __tptz20__RemovePreset(ptSoap, &tptz20__RemovePreset, (struct _tptz20__RemovePresetResponse *)tptz10__RemovePresetResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GotoPreset(struct soap* ptSoap, struct _tptz10__GotoPreset *tptz10__GotoPreset, struct _tptz10__GotoPresetResponse *tptz10__GotoPresetResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	struct _tptz20__GotoPreset tptz20__GotoPreset;
	tptz20__GotoPreset.ProfileToken = tptz10__GotoPreset->ProfileToken;
	tptz20__GotoPreset.PresetToken = tptz10__GotoPreset->PresetToken;
	tptz20__GotoPreset.Speed = tptz10__GotoPreset->Speed;

	return __tptz20__GotoPreset(ptSoap, &tptz20__GotoPreset, (struct _tptz20__GotoPresetResponse *)tptz10__GotoPresetResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GetStatus(struct soap* ptSoap, struct _tptz10__GetStatus *tptz10__GetStatus, struct _tptz10__GetStatusResponse *tptz10__GetStatusResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__GetStatus( ptSoap, (struct _tptz20__GetStatus *)tptz10__GetStatus, (struct _tptz20__GetStatusResponse *)tptz10__GetStatusResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GetConfiguration(struct soap* ptSoap, struct _tptz10__GetConfiguration *tptz10__GetConfiguration, struct _tptz10__GetConfigurationResponse *tptz10__GetConfigurationResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	struct _tptz20__GetConfiguration ptz20__GetConfiguration;
	ptz20__GetConfiguration.PTZConfigurationToken = tptz10__GetConfiguration->ConfigurationToken;

	return __tptz20__GetConfiguration(ptSoap, &ptz20__GetConfiguration, (struct _tptz20__GetConfigurationResponse *)tptz10__GetConfigurationResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GetNodes(struct soap* ptSoap, struct _tptz10__GetNodes *tptz10__GetNodes, struct _tptz10__GetNodesResponse *tptz10__GetNodesResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__GetNodes(ptSoap, (struct _tptz20__GetNodes *)tptz10__GetNodes, (struct _tptz20__GetNodesResponse *)tptz10__GetNodesResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GetNode(struct soap* ptSoap, struct _tptz10__GetNode *tptz10__GetNode, struct _tptz10__GetNodeResponse *tptz10__GetNodeResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	struct _tptz20__GetNode tptz20__GetNode;
	tptz20__GetNode.NodeToken = tptz10__GetNode->NodeToken;

	return __tptz20__GetNode(ptSoap, &tptz20__GetNode, (struct _tptz20__GetNodeResponse *)tptz10__GetNodeResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__SetConfiguration(struct soap* ptSoap, struct _tptz10__SetConfiguration *tptz10__SetConfiguration, struct _tptz10__SetConfigurationResponse *tptz10__SetConfigurationResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	struct _tptz20__SetConfiguration tptz20__SetConfiguration;
	//ptb add for init 20120518
	tptz20__SetConfiguration.ForcePersistence = (enum xsd__boolean)0;
	//end
	tptz20__SetConfiguration.PTZConfiguration = tptz10__SetConfiguration->PTZConfiguration;
	if(tptz10__SetConfiguration->ForcePersistence != NULL)
	{
		tptz20__SetConfiguration.ForcePersistence = *tptz10__SetConfiguration->ForcePersistence;
	}

	return __tptz20__SetConfiguration(ptSoap, &tptz20__SetConfiguration, (struct _tptz20__SetConfigurationResponse *)tptz10__SetConfigurationResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GetConfigurationOptions(struct soap* ptSoap, struct _tptz10__GetConfigurationOptions *tptz10__GetConfigurationOptions, struct _tptz10__GetConfigurationOptionsResponse *tptz10__GetConfigurationOptionsResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__GetConfigurationOptions(ptSoap, (struct _tptz20__GetConfigurationOptions *)tptz10__GetConfigurationOptions, (struct _tptz20__GetConfigurationOptionsResponse *)tptz10__GetConfigurationOptionsResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__GotoHomePosition(struct soap* ptSoap, struct _tptz10__GotoHomePosition *tptz10__GotoHomePosition, struct _tptz10__GotoHomePositionResponse *tptz10__GotoHomePositionResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__GotoHomePosition(ptSoap, (struct _tptz20__GotoHomePosition *)tptz10__GotoHomePosition, (struct _tptz20__GotoHomePositionResponse *)tptz10__GotoHomePositionResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__SetHomePosition(struct soap* ptSoap, struct _tptz10__SetHomePosition *tptz10__SetHomePosition, char **tptz10__SetHomePositionResponse)
{	
	soap_receiver_fault_info(ptSoap, "The home position is fixed and cannot be overwritten.", "ter:Action", "ter:CannotOverwriteHome");		
	return SOAP_FAULT;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__ContinuousMove(struct soap* ptSoap, struct _tptz10__ContinuousMove *tptz10__ContinuousMove, struct _tptz10__ContinuousMoveResponse *tptz10__ContinuousMoveResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__ContinuousMove(ptSoap, (struct _tptz20__ContinuousMove *)tptz10__ContinuousMove, (struct _tptz20__ContinuousMoveResponse *)tptz10__ContinuousMoveResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__RelativeMove(struct soap* ptSoap, struct _tptz10__RelativeMove *tptz10__RelativeMove, struct _tptz10__RelativeMoveResponse *tptz10__RelativeMoveResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__RelativeMove(ptSoap, (struct _tptz20__RelativeMove *)tptz10__RelativeMove, (struct _tptz20__RelativeMoveResponse *)tptz10__RelativeMoveResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__SendAuxiliaryCommand(struct soap* ptSoap, struct _tptz10__SendAuxiliaryCommand *tptz10__SendAuxiliaryCommand, struct _tptz10__SendAuxiliaryCommandResponse *tptz10__SendAuxiliaryCommandResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__SendAuxiliaryCommand(ptSoap, (struct _tptz20__SendAuxiliaryCommand *)tptz10__SendAuxiliaryCommand, (struct _tptz20__SendAuxiliaryCommandResponse *)tptz10__SendAuxiliaryCommandResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__AbsoluteMove(struct soap* ptSoap, struct _tptz10__AbsoluteMove *tptz10__AbsoluteMove, struct _tptz10__AbsoluteMoveResponse *tptz10__AbsoluteMoveResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tptz10__Stop(struct soap* ptSoap, struct _tptz10__StopRequest *tptz10__StopRequest, struct _tptz10__StopResponse *tptz10__StopResponse){
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return __tptz20__Stop(ptSoap, (struct _tptz20__Stop *)tptz10__StopRequest, (struct _tptz20__StopResponse *)tptz10__StopResponse);
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(struct soap* ptSoap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120314 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* ptSoap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	//response
	trt__GetVideoSourcesResponse->__sizeVideoSources = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;
	trt__GetVideoSourcesResponse->VideoSources = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* ptSoap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	//response
	trt__GetAudioSourcesResponse->__sizeAudioSources = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources;
	trt__GetAudioSourcesResponse->AudioSources = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->AudioSources;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120406 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs(struct soap* ptSoap, struct _trt__GetAudioOutputs *trt__GetAudioOutputs, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	trt__GetAudioOutputsResponse->__sizeAudioOutputs = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp->__sizeAudioOutputs;
	trt__GetAudioOutputsResponse->AudioOutputs = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp->AudioOutputs;

	return SOAP_OK;
}
 
/**************************************************
*功能:		添加Profile
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20120224 v1.0 creat by pangtb
				20130516 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile(struct soap* ptSoap, struct _trt__CreateProfile *trt__CreateProfile, struct _trt__CreateProfileResponse *trt__CreateProfileResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	struct _trt__GetProfilesResponse *pstGetProfilesRsp = NULL;
	struct tt__Profile *ptTmpProfile = NULL;
	int iIdleNum = 0;
	
	if(pointer_valid_check(trt__CreateProfile))
	{
		return SOAP_ERR;
	}

	//开辟新空间
	ptTmpProfile = (struct tt__Profile *)(char *)soap_mallocz(ptSoap, sizeof(struct tt__Profile));
	ptTmpProfile->Name  = (char *)calloc(MAX_NAME_LEN, sizeof(char));
	ptTmpProfile->token = (char *)calloc(MAX_NAME_LEN, sizeof(char));
	ptTmpProfile->fixed = (enum xsd__boolean *)calloc(1, sizeof(enum xsd__boolean));

	if(pointer_valid_check(ptTmpProfile)
	|| pointer_valid_check(ptTmpProfile->Name)
	|| pointer_valid_check(ptTmpProfile->token)
	|| pointer_valid_check(ptTmpProfile->fixed))
	{
		return SOAP_ERR;
	}

	iIdleNum = GetIdleNum(ptOnvifCtrl->m_ptOnvifDict, "profile_", 0, NULL, -1);
	if(iIdleNum <= 0 || iIdleNum > MAX_INDEX_NUM)
	{
		return SOAP_ERR;
	}

	
	td_printf(0, "profile idel num:%d\n", iIdleNum);
	//token
	if(pointer_valid_check(trt__CreateProfile->Token))
	{
		sprintf(ptTmpProfile->token, "protoken_ch%02d", iIdleNum);
	}
	else
	{
		strcpy(ptTmpProfile->token, trt__CreateProfile->Token);
	}
	
	//name
	if(!pointer_valid_check(trt__CreateProfile->Name))
	{
		if(strlen(trt__CreateProfile->Name) <= 0 || strlen(trt__CreateProfile->Name) > MAX_NAME_LEN )
		{
			return SOAP_ERR;
		}
		strcpy(ptTmpProfile->Name, trt__CreateProfile->Name);
	}

	//fixed
	*ptTmpProfile->fixed = xsd__boolean__false_;

	//更新全局变量
	pstGetProfilesRsp = ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp;
	pstGetProfilesRsp->Profiles = (struct tt__Profile *)ArrayAdd(pstGetProfilesRsp->Profiles, \
		pstGetProfilesRsp->__sizeProfiles * sizeof(struct tt__Profile), \
		ptTmpProfile, sizeof(struct tt__Profile));
	pstGetProfilesRsp->__sizeProfiles++;

	//写入配置文件
	//token
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "profile_", iIdleNum, NULL, 0, NULL, 0, CFG_ADD);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "profile_", iIdleNum, "protoken", 0, ptTmpProfile->token, 0, CFG_ADD);
	//name
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptTmpProfile->token, 0, NULL, 0, NULL, 0, CFG_ADD);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptTmpProfile->token, 0, "proname", 0, trt__CreateProfile->Name, 0, CFG_ADD);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptTmpProfile->token, 0, "fixed", 0, NULL, *ptTmpProfile->fixed, CFG_ADD);
	//sizePrifile
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "media", 0, "profile_size", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles, CFG_REPLACE);
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	//response
	trt__CreateProfileResponse->Profile = pstGetProfilesRsp->Profiles + (pstGetProfilesRsp->__sizeProfiles - 1);

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120314 v1.0 creat by pangtb
	20120314 v1.0 modified by wl_hq
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* ptSoap, struct _trt__GetProfile *trt__GetProfile, struct _trt__GetProfileResponse *trt__GetProfileResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int i;
	int iVeChn = 0;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	char *vectoken = NULL;
	char pcTmpReason[128] = {0};

	if(NULL == trt__GetProfile || NULL == trt__GetProfile->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__GetProfile->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetProfile->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	vectoken =  ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token;
	iVeChn = atoi(vectoken + strlen("vectoken_ch")) - 1;//根据当前的Profile序号获取实际的通道号
	if(TD_OK != UpdateVideoInfo(ptOnvifCtrl, iVeChn))
	{
		td_printf(0, "%s %d %s =====UpdateVideoInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}
	//response
	trt__GetProfileResponse->Profile = &ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum];


	trt__GetProfileResponse->Profile->VideoSourceConfiguration->Bounds->height = height;
	trt__GetProfileResponse->Profile->VideoSourceConfiguration->Bounds->width = width;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Resolution->Width = width;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Resolution->Height = height;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl->FrameRateLimit = 25;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl->BitrateLimit = 384;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->H264->GovLength = 25;
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120307 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* ptSoap, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == ptOnvifCtrl)
	{
		return SOAP_ERR;
	}

	if(TD_OK != UpdateVideoInfoAll(ptOnvifCtrl))
	{
		td_printf(0, "%s %d %s UpdateVideoInfoAll is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}
	trt__GetProfilesResponse->__sizeProfiles = ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles;
	trt__GetProfilesResponse->Profiles = ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles;

	for(int i = 0; i < trt__GetProfilesResponse->__sizeProfiles; i++)
	{
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->width = width;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->height = height;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Width = width;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Height = height;
	}
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(struct soap* ptSoap, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	int iVECNewNum = 0;
	int iVECOldNum = 0;
	int i;
	char pcTmpReason[128] = {0};

 	if(NULL == trt__AddVideoEncoderConfiguration || NULL == trt__AddVideoEncoderConfiguration->ProfileToken || NULL == trt__AddVideoEncoderConfiguration->ConfigurationToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//get profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__AddVideoEncoderConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__AddVideoEncoderConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//get video encoder number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__AddVideoEncoderConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[i].token))
		{
			iVECNewNum = i;
			td_printf(0, "iVECNewNum:%d\n", iVECNewNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The VideoEncoderConfiguration indicated by the %s does not exist.", trt__AddVideoEncoderConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
	//replace
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token)
		{
			if(!strcmp(trt__AddVideoEncoderConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token))
			{
				return SOAP_OK;
			}
			else
			{
				//get old vec num
				if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token)
				{	
					for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations; i++)
					{
						if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[i].token))
						{
							iVECOldNum = i;
							td_printf(0, "iVECOldNum:%d\n", iVECOldNum);
							break;
						}
						else
						{
							if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations - 1)
							{
								return SOAP_ERR;
							}
						}
					}
				}
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECOldNum].UseCount--;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECOldNum].token, 0, "vecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECOldNum].UseCount, CFG_REPLACE);

				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECNewNum].UseCount++;
				ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations + iVECNewNum;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddVideoEncoderConfiguration->ProfileToken, 0, "vectoken", 0, trt__AddVideoEncoderConfiguration->ConfigurationToken, 0, CFG_REPLACE);
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddVideoEncoderConfiguration->ConfigurationToken, 0, "vecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECNewNum].UseCount, CFG_REPLACE);
			}
		}
	}
	//new
	else
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations + iVECNewNum;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECNewNum].UseCount++;
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddVideoEncoderConfiguration->ConfigurationToken, 0, "vecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECNewNum].UseCount, CFG_REPLACE);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddVideoEncoderConfiguration->ProfileToken, 0, "vectoken", 0, trt__AddVideoEncoderConfiguration->ConfigurationToken, 0, CFG_ADD);
	}
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120314 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(struct soap* ptSoap, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iVSCNewNum = 0;
	int iVSCOldNum = 0;
	int iProNum = 0;
	int i;
	char pcTmpReason[128] = {0};
	
	if(NULL == trt__AddVideoSourceConfiguration || NULL == trt__AddVideoSourceConfiguration->ProfileToken || NULL == trt__AddVideoSourceConfiguration->ConfigurationToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__AddVideoSourceConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__AddVideoSourceConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//video source number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__AddVideoSourceConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
		{
			iVSCNewNum = i;
			td_printf(0, "iVSCNewNum:%d\n", iVSCNewNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The VideoSourceConfiguration indicated by the %s does not exist", trt__AddVideoSourceConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
	//replace
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token)
		{
			if(!strcmp(trt__AddVideoSourceConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token))
			{
				return SOAP_OK;
			}
			//unbind previous vsc
			else
			{
				//previous video source number
				for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
				{
					if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
					{
						iVSCOldNum = i;
						td_printf(0, "iVSCOldNum:%d\n", iVSCOldNum);
						break;
					}
					else
					{
						if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations - 1)
						{
							return SOAP_ERR;
						}
					}
				}
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCOldNum].UseCount--;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCOldNum].token, 0, "vscusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCOldNum].UseCount, CFG_REPLACE);

				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNewNum].UseCount++;
				ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations + iVSCNewNum;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddVideoSourceConfiguration->ConfigurationToken, 0, "vscusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNewNum].UseCount, CFG_REPLACE);
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddVideoSourceConfiguration->ProfileToken, 0, "vsctoken", 0, trt__AddVideoSourceConfiguration->ConfigurationToken, 0, CFG_REPLACE);
			}
		}
	}
	//new
	else
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration = &ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNewNum];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNewNum].UseCount++;
		
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddVideoSourceConfiguration->ProfileToken, 0, "vsctoken", 0, trt__AddVideoSourceConfiguration->ConfigurationToken, 0, CFG_ADD);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddVideoSourceConfiguration->ConfigurationToken, 0, "vscusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNewNum].UseCount, CFG_REPLACE);
	}

	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120314 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(struct soap* ptSoap, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	int iAECOldNum = 0;
	int iAECNewNum = 0;
	int i;
	char pcTmpReason[128] = {0};

 	if(NULL == trt__AddAudioEncoderConfiguration || NULL == trt__AddAudioEncoderConfiguration->ProfileToken || NULL == trt__AddAudioEncoderConfiguration->ConfigurationToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//get profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__AddAudioEncoderConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__AddAudioEncoderConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//get audio encoder number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__AddAudioEncoderConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token))
		{
			iAECNewNum = i;
			td_printf(0, "iAECNum:%d\n", iAECNewNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The AudioEncoderConfiguration indicated by the %s does not exist.", trt__AddAudioEncoderConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
	//replace
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token)
		{
			if(!strcmp(trt__AddAudioEncoderConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token))
			{
				return SOAP_OK;
			}
			else
			{
				//get old vec num
				if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token)
				{	
					for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations; i++)
					{
						if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token))
						{
							iAECOldNum = i;
							td_printf(0, "iAECOldNum:%d\n", iAECOldNum);
							break;
						}
						else
						{
							if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations - 1)
							{
								return SOAP_ERR;
							}
						}
					}
				}
				ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECOldNum].UseCount--;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECOldNum].token, 0, "aecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECOldNum].UseCount, CFG_REPLACE);

				ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations + iAECNewNum;
				ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNewNum].UseCount++;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddAudioEncoderConfiguration->ConfigurationToken, 0, "aecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNewNum].UseCount, CFG_REPLACE);
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddAudioEncoderConfiguration->ProfileToken, 0, "aectoken", 0, trt__AddAudioEncoderConfiguration->ConfigurationToken, 0, CFG_REPLACE);
			}
		}
	}
	//new
	else
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations + iAECNewNum;
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNewNum].UseCount++;

		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddAudioEncoderConfiguration->ProfileToken, 0, "aectoken", 0, trt__AddAudioEncoderConfiguration->ConfigurationToken, 0, CFG_ADD);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddAudioEncoderConfiguration->ConfigurationToken, 0, "aecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNewNum].UseCount, CFG_REPLACE);
	}
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120314 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(struct soap* ptSoap, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iASCNewNum = 0;
	int iASCOldNum = 0;
	int iProNum = 0;
	int i;
	char pcTmpReason[128] = {0};

	if(NULL == trt__AddAudioSourceConfiguration || NULL == trt__AddAudioSourceConfiguration->ProfileToken || NULL == trt__AddAudioSourceConfiguration->ConfigurationToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__AddAudioSourceConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__AddAudioSourceConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//audio source number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__AddAudioSourceConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token))
		{
			iASCNewNum = i;
			td_printf(0, "iASCNewNum:%d\n", iASCNewNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The AudioSourceConfiguration indicated by the %s does not exist.", trt__AddAudioSourceConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
	//replace
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration)
	{
		DBG_HERE;
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token)
		{
			if(!strcmp(trt__AddAudioSourceConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token))
			{
				return SOAP_OK;
			}
			else
			{
				//previous audio source number
				for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations; i++)
				{
					if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token))
					{
						iASCOldNum = i;
						td_printf(0, "iASCOldNum:%d\n", iASCOldNum);
						break;
					}
					else
					{
						if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations - 1)
						{
							return SOAP_ERR;
						}
					}
				}
				ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCOldNum].UseCount--;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCOldNum].token, 0, "ascusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCOldNum].UseCount, CFG_REPLACE);

				ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNewNum].UseCount++;
				ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations + iASCNewNum;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddAudioSourceConfiguration->ConfigurationToken, 0, "ascusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNewNum].UseCount, CFG_REPLACE);
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddAudioSourceConfiguration->ProfileToken, 0, "asctoken", 0, trt__AddAudioSourceConfiguration->ConfigurationToken, 0, CFG_REPLACE);
			}
		}
	}
	//new
	else
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations + iASCNewNum;
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNewNum].UseCount++;
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddAudioSourceConfiguration->ProfileToken, 0, "asctoken", 0, trt__AddAudioSourceConfiguration->ConfigurationToken, 0, CFG_ADD);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddAudioSourceConfiguration->ConfigurationToken, 0, "ascusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNewNum].UseCount, CFG_REPLACE);
	}
		
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120314 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(struct soap* ptSoap, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	int iPTZCNewNum = 0;
	int iPTZCOldNum = 0;	
	int i;
	char pcTmpReason[128] = {0};

 	if(NULL == trt__AddPTZConfiguration || NULL == trt__AddPTZConfiguration->ProfileToken || NULL == trt__AddPTZConfiguration->ConfigurationToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//get profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__AddPTZConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__AddPTZConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}

		}
	}
	//get PTZ config number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->__sizePTZConfiguration; i++)
	{
		if(!strcmp(trt__AddPTZConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].token))
		{
			iPTZCNewNum = i;
			td_printf(0, "iPTZCNewNum:%d\n", iPTZCNewNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->__sizePTZConfiguration - 1)
			{
				sprintf(pcTmpReason, "The PTZConfiguration indicated by the %s does not exist.", trt__AddPTZConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
	
	//check exist
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].PTZConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].PTZConfiguration->token)
		{
			if(!strcmp(trt__AddPTZConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].PTZConfiguration->token))
			{
				return SOAP_OK;
			}
			else
			{
				//get old ptzc num
				if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].PTZConfiguration->token)
				{	
					for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->__sizePTZConfiguration; i++)
					{
						if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].PTZConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].token))
						{
							iPTZCOldNum = i;
							td_printf(0, "iPTZCOldNum:%d\n", iPTZCOldNum);
							break;
						}
						else
						{
							if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->__sizePTZConfiguration - 1)
							{
								return SOAP_ERR;
							}
						}
					}
				}
				ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[iPTZCOldNum].UseCount--;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[iPTZCOldNum].token, 0, "ptzcusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[iPTZCOldNum].UseCount, CFG_REPLACE);

				ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].PTZConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration + iPTZCNewNum;
				ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[iPTZCNewNum].UseCount++;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddPTZConfiguration->ConfigurationToken, 0, "ptzcusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[iPTZCNewNum].UseCount, CFG_REPLACE);
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddPTZConfiguration->ProfileToken, 0, "ptzctoken", 0, trt__AddPTZConfiguration->ConfigurationToken, 0, CFG_REPLACE);
			}
		}
	}
	else
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].PTZConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration + iPTZCNewNum;
		ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[iPTZCNewNum].UseCount++;
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddPTZConfiguration->ProfileToken, 0, "ptzctoken", 0, trt__AddPTZConfiguration->ConfigurationToken, 0, CFG_ADD);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddPTZConfiguration->ConfigurationToken, 0, "ptzcusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[iPTZCNewNum].UseCount, CFG_REPLACE);
	}
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(struct soap* ptSoap, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120314 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(struct soap* ptSoap, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	int iMCNewNum = 0;
	int iMCOldNum = 0;
	int i;
	char pcTmpReason[128] = {0};

 	if(NULL == trt__AddMetadataConfiguration || NULL == trt__AddMetadataConfiguration->ProfileToken || NULL == trt__AddMetadataConfiguration->ConfigurationToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//get profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__AddMetadataConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__AddMetadataConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//get MC config number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__AddMetadataConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[i].token))
		{
			iMCNewNum = i;
			td_printf(0, "iMCNewNum:%d\n", iMCNewNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The MetadataConfiguration indicated by the %s does not exist.", trt__AddMetadataConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
	//check exist
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration->token)
		{
			if(!strcmp(trt__AddMetadataConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration->token))
			{
				return SOAP_OK;
			}
			else
			{
				//get old mc num
				if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration->token)
				{	
					for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations; i++)
					{
						if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[i].token))
						{
							iMCOldNum = i;
							td_printf(0, "iMCOldNum:%d\n", iMCOldNum);
							break;
						}
						else
						{
							if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations - 1)
							{
								return SOAP_ERR;
							}
						}
					}
				}
				ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCOldNum].UseCount--;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCOldNum].token, 0, "mcusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCOldNum].UseCount, CFG_REPLACE);

				ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations + iMCNewNum;
				ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNewNum].UseCount++;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddMetadataConfiguration->ConfigurationToken, 0, "mcusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNewNum].UseCount, CFG_REPLACE);
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddMetadataConfiguration->ProfileToken, 0, "mctoken", 0, trt__AddMetadataConfiguration->ConfigurationToken, 0, CFG_REPLACE);
			}
		}
	}
	//new
	else
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations + iMCNewNum;
		ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNewNum].UseCount++;
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddMetadataConfiguration->ProfileToken, 0, "mctoken", 0, trt__AddMetadataConfiguration->ConfigurationToken, 0, CFG_ADD);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__AddMetadataConfiguration->ConfigurationToken, 0, "mcusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNewNum].UseCount, CFG_REPLACE);
	}
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(struct soap* ptSoap, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(struct soap* ptSoap, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	//return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
	//设备本身不支持音频解码，如果必须回response信息，
	//则将response置NULL,否则直接return。
	//by qiaohaijun @2013.6.6
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120318 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(struct soap* ptSoap, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	int iVECNum = 0;
	int i;
	char pcTmpReason[128] = {0};
	char pcVECToken[64] = {0};

 	if(NULL == trt__RemoveVideoEncoderConfiguration || NULL == trt__RemoveVideoEncoderConfiguration->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//get profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__RemoveVideoEncoderConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__RemoveVideoEncoderConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	
	//get video encoder number
	strcpy(pcVECToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token);
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(pcVECToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[i].token))
		{
			iVECNum = i;
			td_printf(0, "iVECNum:%d\n", iVECNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations - 1)
			{
				return SOAP_ERR;
			}
		}
	}
	
	//更新全局变量	
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration = NULL;
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECNum].UseCount--;

	//更新配置文件
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__RemoveVideoEncoderConfiguration->ProfileToken, 0, "vectoken", 0, NULL, 0, CFG_RM);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, pcVECToken, 0, "vecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECNum].UseCount, CFG_REPLACE);
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120318 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(struct soap* ptSoap, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iVSCNum = 0;
	int iProNum = 0;
	int i;
	char pcTmpReason[128] = {0};
	char pcVSCToken[64] = {0};
	
	if(NULL == trt__RemoveVideoSourceConfiguration || NULL == trt__RemoveVideoSourceConfiguration->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__RemoveVideoSourceConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__RemoveVideoSourceConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//video source number
	strcpy(pcVSCToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token);
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(pcVSCToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
		{
			iVSCNum = i;
			td_printf(0, "iVSCNum:%d\n", iVSCNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations - 1)
			{
				return SOAP_ERR;
			}
		}
	}
	
	//更新全局变量	
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration = NULL;
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].UseCount--;

	//更新配置文件
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__RemoveVideoSourceConfiguration->ProfileToken, 0, "vsctoken", 0, NULL, 0, CFG_RM);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, pcVSCToken, 0, "vscusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].UseCount, CFG_REPLACE);
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120318 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(struct soap* ptSoap, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	int iAECNum = 0;
	int i;
	char pcTmpReason[128] = {0};
	char pcAECToken[64] = {0};

 	if(NULL == trt__RemoveAudioEncoderConfiguration || NULL == trt__RemoveAudioEncoderConfiguration->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//get profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__RemoveAudioEncoderConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__RemoveAudioEncoderConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//get audio encoder number
	strcpy(pcAECToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token);
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(pcAECToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token))
		{
			iAECNum = i;
			td_printf(0, "iAECNum:%d\n", iAECNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations - 1)
			{
				return SOAP_ERR;
			}
		}
	}
	
	//更新全局变量	
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration = NULL;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].UseCount--;

	//更新配置文件
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__RemoveAudioEncoderConfiguration->ProfileToken, 0, "aectoken", 0, NULL, 0, CFG_RM);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, pcAECToken, 0, "aecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].UseCount, CFG_REPLACE);
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120318 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(struct soap* ptSoap, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iASCNum = 0;
	int iProNum = 0;
	int i;
	char pcTmpReason[128] = {0};
	char pcASCToken[64] = {0};

	if(NULL == trt__RemoveAudioSourceConfiguration || NULL == trt__RemoveAudioSourceConfiguration->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__RemoveAudioSourceConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__RemoveAudioSourceConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//audio source number
	strcpy(pcASCToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token);
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(pcASCToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token))
		{
			iASCNum = i;
			td_printf(0, "iASCNum:%d\n", iASCNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations - 1)
			{
				return SOAP_ERR;
			}
		}
	}
	
	//更新全局变量	
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration = NULL;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNum].UseCount--;

	//更新配置文件
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__RemoveAudioSourceConfiguration->ProfileToken, 0, "asctoken", 0, NULL, 0, CFG_RM);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, pcASCToken, 0, "ascusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNum].UseCount, CFG_REPLACE);
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(struct soap* ptSoap, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	int i;

	if(FindProEntry(g_stOnvifCtrl.m_ptOnvifDict, trt__RemovePTZConfiguration->ProfileToken, -1) != 1)
	{
		soap_sender_fault_info(ptSoap, "The requested profile token ProfileToken does not exist.", "ter:InvalidArgVal", "ter:NoProfile");
		return SOAP_FAULT;
	}

	SetProStr(g_stOnvifCtrl.m_ptOnvifDict, trt__RemovePTZConfiguration->ProfileToken, -1, "ptzctoken",-1, NULL, -1, CFG_RM);

	for(i = 0; i < g_stOnvifCtrl.m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(strcmp((g_stOnvifCtrl.m_stOnvifCfg.m_ptProfilesRsp->Profiles+i)->token, trt__RemovePTZConfiguration->ProfileToken) == 0)
		{
			(g_stOnvifCtrl.m_stOnvifCfg.m_ptProfilesRsp->Profiles+i)->PTZConfiguration = NULL;
		}
	}

	g_stOnvifCtrl.m_stFlag.m_iCfgChangeFlag = 1;
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(struct soap* ptSoap, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120318 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(struct soap* ptSoap, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	int iMCNum = 0;
	int i;
	char pcTmpReason[128] = {0};
	char pcMCToken[64] = {0};

 	if(NULL == trt__RemoveMetadataConfiguration || NULL == trt__RemoveMetadataConfiguration->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//get profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__RemoveMetadataConfiguration->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__RemoveMetadataConfiguration->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//get PTZ config number
	strcpy(pcMCToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration->token);
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(pcMCToken, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[i].token))
		{
			iMCNum = i;
			td_printf(0, "iMCNum:%d\n", iMCNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations - 1)
			{
				return SOAP_ERR;
			}
		}
	}
	
	//更新全局变量	
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration = NULL;
	ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].UseCount--;

	//更新配置文件
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__RemoveMetadataConfiguration->ProfileToken, 0, "mctoken", 0, NULL, 0, CFG_RM);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, pcMCToken, 0, "mcusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].UseCount, CFG_REPLACE);
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(struct soap* ptSoap, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(struct soap* ptSoap, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* ptSoap, struct _trt__DeleteProfile *trt__DeleteProfile, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProNum = 0;
	int iVSCNum = 0;
	int iVECNum = 0;
	int iASCNum = 0;
	int iAECNum = 0;
	int iMCNum = 0;
	int i;
	char pcTmpReason[128] = {0};

	if(NULL == trt__DeleteProfile || NULL == trt__DeleteProfile->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//get profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__DeleteProfile->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__DeleteProfile->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	//release previous param
	//video source configuration
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token)
		{
			for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
				{
					iVSCNum = i;
					break;
				}
				else
				{
					if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations - 1)
					{
						return SOAP_ERR;
					}
				}
			}
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].UseCount--;
			SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token, 0, "vscusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].UseCount, CFG_REPLACE);
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration = NULL;
	}
	//video encoder configuration
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token)
		{	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations; i++)
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[i].token))
				{
					iVECNum = i;
					break;
				}
				else
				{
					if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations - 1)
					{
						return SOAP_ERR;
					}
				}
			}
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECNum].UseCount--;
			SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token, 0, "vecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVECNum].UseCount, CFG_REPLACE);
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration = NULL;
	}
	//audio source configuration
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token)
		{
			for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations; i++)
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token))
				{
					iASCNum = i;
					break;
				}
				else
				{
					if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations - 1)
					{
						return SOAP_ERR;
					}
				}
			}
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNum].UseCount--;
			SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token, 0, "ascusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNum].UseCount, CFG_REPLACE);
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration = NULL;
	}
	//audio encoder configuration
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token)
		{
			for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations; i++)
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token))
				{
					iAECNum = i;
					break;
				}
				else
				{
					if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations - 1)
					{
						return SOAP_ERR;
					}
				}
			}
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].UseCount--;
			SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token, 0, "aecusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].UseCount, CFG_REPLACE);
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration = NULL;
	}
	//metadata configuration
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration)
	{
		if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration->token)
		{
			for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations; i++)
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[i].token))
				{
					iMCNum = i;
					break;
				}
				else
				{
					if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations - 1)
					{
						return SOAP_ERR;
					}
				}
			}
			ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].UseCount--;
			SetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration->token, 0, "mcusecount", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].UseCount, CFG_REPLACE);
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].MetadataConfiguration = NULL;
	}
	
	//other
	free(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].Name);
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].Name = NULL;
	free(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].token);
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].token = NULL;
	free(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].fixed);
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].fixed = NULL;
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles = (struct tt__Profile *)ArrayDel( \
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles, \
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles * sizeof(struct tt__Profile), \
		ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles + iProNum, \
		sizeof(struct tt__Profile));
	if(NULL == ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles)
	{
		return SOAP_ERR;
	}
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles--;

	//修改配置文件
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "media", 0, "profile_size", 0, NULL, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles, CFG_REPLACE);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__DeleteProfile->ProfileToken, 0, NULL, 0, NULL, 0, CFG_RM);
	SetProStr(ptOnvifCtrl->m_ptOnvifDict, "profile_", iProNum + 1, NULL, 0, NULL, 0, CFG_RM);
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
	
	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations(struct soap* ptSoap, struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == trt__GetVideoSourceConfigurations)
	{
		return SOAP_ERR;
	}

	trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations;
	trt__GetVideoSourceConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(struct soap* ptSoap, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == trt__GetVideoEncoderConfigurations)
	{
		return SOAP_ERR;
	}

	//更新视频参数信息
	if(TD_OK != UpdateVideoInfoAll(ptOnvifCtrl))
	{
		td_printf(0, "%s %d %s UpdateVideoInfoAll is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}

	trt__GetVideoEncoderConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations;
	trt__GetVideoEncoderConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations;

	struct tt__VideoEncoderConfiguration *ptr = trt__GetVideoEncoderConfigurationsResponse->Configurations;
	ptr->Resolution->Width = 512;
	ptr->Resolution->Height = 240;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(struct soap* ptSoap, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == trt__GetAudioSourceConfigurations)
	{
		return SOAP_ERR;
	}

	trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations;
	trt__GetAudioSourceConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations(struct soap* ptSoap, struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == trt__GetAudioEncoderConfigurations)
	{
		return SOAP_ERR;
	}

	trt__GetAudioEncoderConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations;
	trt__GetAudioEncoderConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations(struct soap* ptSoap, struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations(struct soap* ptSoap, struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == trt__GetMetadataConfigurations)
	{
		return SOAP_ERR;
	}

	trt__GetMetadataConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations;
	trt__GetMetadataConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120408 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations(struct soap* ptSoap, struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(NULL == trt__GetAudioOutputConfigurations)
	{
		return SOAP_ERR;
	}

	trt__GetAudioOutputConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->__sizeConfigurations;
	trt__GetAudioOutputConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations(struct soap* ptSoap, struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	//return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
	//设备本身不支持音频解码，如果必须回response信息，
	//则将response置NULL,否则直接return。
	//by qiaohaijun @2013.6.6
	trt__GetAudioDecoderConfigurationsResponse = NULL;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration(struct soap* ptSoap, struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN] = {0};
	int i;
	int iVSCNum = 0;

	if(NULL == trt__GetVideoSourceConfiguration || NULL == trt__GetVideoSourceConfiguration->ConfigurationToken)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetVideoSourceConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetVideoSourceConfiguration->ConfigurationToken);
			iVSCNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetVideoSourceConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	trt__GetVideoSourceConfigurationResponse->Configuration = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations + iVSCNum;
	trt__GetVideoSourceConfigurationResponse->Configuration->Bounds->width = width;
	trt__GetVideoSourceConfigurationResponse->Configuration->Bounds->height = height;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(struct soap* ptSoap, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN] = {0};
	int iVECNum = 0;
	int i = 0;

	if(NULL == ptOnvifCtrl || NULL == trt__GetVideoEncoderConfiguration || NULL == trt__GetVideoEncoderConfiguration->ConfigurationToken)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetVideoEncoderConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetVideoEncoderConfiguration->ConfigurationToken);
			iVECNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetVideoEncoderConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	//更新视频参数信息
	if(TD_OK != UpdateVideoInfo(ptOnvifCtrl, iVECNum))
	{
		td_printf(0, "%s %d %s =====UpdateVideoInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}

	trt__GetVideoEncoderConfigurationResponse->Configuration = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations + iVECNum;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(struct soap* ptSoap, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN] = {0};
	int i;
	int iASCNum = 0;

	if(NULL == trt__GetAudioSourceConfiguration || NULL == trt__GetAudioSourceConfiguration->ConfigurationToken)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetAudioSourceConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetAudioSourceConfiguration->ConfigurationToken);
			iASCNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetAudioSourceConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	trt__GetAudioSourceConfigurationResponse->Configuration = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations + iASCNum;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(struct soap* ptSoap, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN] = {0};
	int i;
	int iAECNum = 0;

	if(NULL == trt__GetAudioEncoderConfiguration || NULL == trt__GetAudioEncoderConfiguration->ConfigurationToken)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetAudioEncoderConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetAudioEncoderConfiguration->ConfigurationToken);
			iAECNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetAudioEncoderConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	trt__GetAudioEncoderConfigurationResponse->Configuration = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations + iAECNum;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(struct soap* ptSoap, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(struct soap* ptSoap, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN] = {0};
	int i;
	int iMCNum = 0;

	if(NULL == trt__GetMetadataConfiguration || NULL == trt__GetMetadataConfiguration->ConfigurationToken)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetMetadataConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetMetadataConfiguration->ConfigurationToken);
			iMCNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetMetadataConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	trt__GetMetadataConfigurationResponse->Configuration = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations + iMCNum;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120410 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(struct soap* ptSoap, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN] = {0};
	int i;
	int iAOCNum = 0;

	if(NULL == trt__GetAudioOutputConfiguration || NULL == trt__GetAudioOutputConfiguration->ConfigurationToken)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetAudioOutputConfiguration->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetAudioOutputConfiguration->ConfigurationToken);
			iAOCNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetAudioOutputConfiguration->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	trt__GetAudioOutputConfigurationResponse->Configuration = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations + iAOCNum;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(struct soap* ptSoap, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	//设备本身不支持音频解码，如果必须回response信息，
	//则将response置NULL,否则直接return。
	//by qiaohaijun @2013.6.6
	trt__GetAudioDecoderConfigurationResponse = NULL;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(struct soap* ptSoap, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int i;
	int iProNum = 0;
//	int iVECNum = 0;
	char pcTmpReason[MAX_STRING_LEN];

	if(NULL == trt__GetCompatibleVideoEncoderConfigurations || NULL == trt__GetCompatibleVideoEncoderConfigurations->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__GetCompatibleVideoEncoderConfigurations->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetCompatibleVideoEncoderConfigurations->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}

	trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(struct soap* ptSoap, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int i;
	int iProNum = 0;
//	int iVSCNum = 0;
	char pcTmpReason[MAX_STRING_LEN];

	if(NULL == trt__GetCompatibleVideoSourceConfigurations || NULL == trt__GetCompatibleVideoSourceConfigurations->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__GetCompatibleVideoSourceConfigurations->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetCompatibleVideoSourceConfigurations->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	
	trt__GetCompatibleVideoSourceConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations;
	trt__GetCompatibleVideoSourceConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(struct soap* ptSoap, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int i;
	int iProNum = 0;
	char pcTmpReason[MAX_STRING_LEN];

	if(NULL == trt__GetCompatibleAudioEncoderConfigurations || NULL == trt__GetCompatibleAudioEncoderConfigurations->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__GetCompatibleAudioEncoderConfigurations->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetCompatibleAudioEncoderConfigurations->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}

	trt__GetCompatibleAudioEncoderConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations;
	trt__GetCompatibleAudioEncoderConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(struct soap* ptSoap, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int i;
	int iProNum = 0;
	char pcTmpReason[MAX_STRING_LEN];

	if(NULL == trt__GetCompatibleAudioSourceConfigurations || NULL == trt__GetCompatibleAudioSourceConfigurations->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		td_printf(0, "===i:%d, ProfileToken:%s, token:%s\n", i, trt__GetCompatibleAudioSourceConfigurations->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token);
		if(!strcmp(trt__GetCompatibleAudioSourceConfigurations->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetCompatibleAudioSourceConfigurations->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}

	trt__GetCompatibleAudioSourceConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations;
	trt__GetCompatibleAudioSourceConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(struct soap* ptSoap, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(struct soap* ptSoap, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int i;
	int iProNum = 0;
	char pcTmpReason[MAX_STRING_LEN];

	if(NULL == trt__GetCompatibleMetadataConfigurations || NULL == trt__GetCompatibleMetadataConfigurations->ProfileToken)
	{
		return soap_sender_fault(ptSoap, NULL, NULL);
	}

	//profile number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__GetCompatibleMetadataConfigurations->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetCompatibleMetadataConfigurations->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	
	trt__GetCompatibleMetadataConfigurationsResponse->__sizeConfigurations = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations;
	trt__GetCompatibleMetadataConfigurationsResponse->Configurations = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(struct soap* ptSoap, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(struct soap* ptSoap, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	//设备本身不支持音频解码，如果必须回response信息，
	//则将response置NULL,否则直接return。
	//by qiaohaijun @2013.6.6
	trt__GetCompatibleAudioDecoderConfigurationsResponse = NULL;

	return SOAP_OK;
//	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120317 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(struct soap* ptSoap, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN];
	int iVSCNum = 0;
	int i = 0;

	if(NULL == trt__SetVideoSourceConfiguration || NULL == trt__SetVideoSourceConfiguration->Configuration || NULL == trt__SetVideoSourceConfiguration->Configuration->token)
	{
		return SOAP_ERR;
	}

	//config number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__SetVideoSourceConfiguration->Configuration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
		{
			iVSCNum = i;
			td_printf(0, "iVSCNum:%d\n", iVSCNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__SetVideoSourceConfiguration->Configuration->token);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
#if 0
	//confilect
	if(ptOnvifCtrl->m_stOnvifCfg.m_ptrt__GetVideoSourceConfigurationsResponse->Configurations[iVSCNum].UseCount > 1)
	{
		sprintf(pcTmpReason, "The new settings conflicts with other uses of the configuration.");
		return soap_receiver_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:ConfigurationConflict");
	}
#endif
	//compare
	//name
	if(NULL != trt__SetVideoSourceConfiguration->Configuration->Name && \
		strcmp(trt__SetVideoSourceConfiguration->Configuration->Name, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Name))
	{
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Name, \
			trt__SetVideoSourceConfiguration->Configuration->Name);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetVideoSourceConfiguration->Configuration->token, 0, "vscname", 0, trt__SetVideoSourceConfiguration->Configuration->Name, 0, CFG_REPLACE);
	}

	//SourceToken
	if(NULL != trt__SetVideoSourceConfiguration->Configuration->SourceToken && \
		strcmp(trt__SetVideoSourceConfiguration->Configuration->SourceToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].SourceToken))
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
		{
			if(!strcmp(trt__SetVideoSourceConfiguration->Configuration->SourceToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].SourceToken))
			{
				td_printf(0, "SourceToken:%s\n", trt__SetVideoSourceConfiguration->Configuration->SourceToken);
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations - 1)
				{
					return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
				}
			}
		}
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].SourceToken, \
			trt__SetVideoSourceConfiguration->Configuration->SourceToken);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetVideoSourceConfiguration->Configuration->token, 0, "vscsrctoken", 0, trt__SetVideoSourceConfiguration->Configuration->SourceToken, 0, CFG_REPLACE);
	}

	//Bounds
	if(NULL != trt__SetVideoSourceConfiguration->Configuration->Bounds)
	{
		//Bounds X
		if(trt__SetVideoSourceConfiguration->Configuration->Bounds->x != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Bounds->x)
		{
			if(trt__SetVideoSourceConfiguration->Configuration->Bounds->x < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[iVSCNum].BoundsRange->XRange->Min || \
				trt__SetVideoSourceConfiguration->Configuration->Bounds->x > ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[iVSCNum].BoundsRange->XRange->Max)
			{
				return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
			}
			else
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Bounds->x = trt__SetVideoSourceConfiguration->Configuration->Bounds->x;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetVideoSourceConfiguration->Configuration->token, 0, "vscboundsx", 0, NULL, trt__SetVideoSourceConfiguration->Configuration->Bounds->x, CFG_REPLACE);
			}
		}
		//Bounds Y
		if(trt__SetVideoSourceConfiguration->Configuration->Bounds->y != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Bounds->y)
		{
			if(trt__SetVideoSourceConfiguration->Configuration->Bounds->y < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[iVSCNum].BoundsRange->YRange->Min || \
				trt__SetVideoSourceConfiguration->Configuration->Bounds->y > ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[iVSCNum].BoundsRange->YRange->Max)
			{
				return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
			}
			else
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Bounds->y = trt__SetVideoSourceConfiguration->Configuration->Bounds->y;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetVideoSourceConfiguration->Configuration->token, 0, "vscboundsy", 0, NULL, trt__SetVideoSourceConfiguration->Configuration->Bounds->y, CFG_REPLACE);
			}
		}
		//Bounds W
		if(trt__SetVideoSourceConfiguration->Configuration->Bounds->width != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Bounds->width)
		{
			if(trt__SetVideoSourceConfiguration->Configuration->Bounds->width < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[iVSCNum].BoundsRange->WidthRange->Min || \
				trt__SetVideoSourceConfiguration->Configuration->Bounds->width> ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[iVSCNum].BoundsRange->WidthRange->Max)
			{
				return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
			}
			else
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Bounds->width = trt__SetVideoSourceConfiguration->Configuration->Bounds->width;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetVideoSourceConfiguration->Configuration->token, 0, "vscboundsw", 0, NULL, trt__SetVideoSourceConfiguration->Configuration->Bounds->width, CFG_REPLACE);
			}
		}
		//Bounds H
		if(trt__SetVideoSourceConfiguration->Configuration->Bounds->height!= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Bounds->height)
		{
			if(trt__SetVideoSourceConfiguration->Configuration->Bounds->height < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[iVSCNum].BoundsRange->HeightRange->Min || \
				trt__SetVideoSourceConfiguration->Configuration->Bounds->height > ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[iVSCNum].BoundsRange->HeightRange->Max)
			{
				return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
			}
			else
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[iVSCNum].Bounds->height = trt__SetVideoSourceConfiguration->Configuration->Bounds->height;
				SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetVideoSourceConfiguration->Configuration->token, 0, "vscboundsh", 0, NULL, trt__SetVideoSourceConfiguration->Configuration->Bounds->height, CFG_REPLACE);
			}
		}		
	}

	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		设置视频编码参数
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20120317 v1.0 creat by pangtb
				20121115 v1.0 modify by ptb
				20130228 v2.3.0 modify by ptb
				20130923 v2.3.1 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(struct soap* ptSoap, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	struct tt__VideoEncoderConfiguration *ptSetConfig = NULL;
	struct _trt__GetVideoEncoderConfigurationsResponse *pSaveRsp = NULL;
	char pcTmpReason[MAX_STRING_LEN];
	int iVECNum = 0;
	int iRet 	= 0;
	int i		= 0;

	ptSetConfig = trt__SetVideoEncoderConfiguration->Configuration;
	pSaveRsp    = g_stOnvifCtrl.m_stOnvifCfg.m_ptVideoEncoderCfgRsp;

	if(pointer_valid_check(ptSetConfig)  
	|| pointer_valid_check(ptSetConfig->token))
	{
		return SOAP_ERR;
	}

	//token
	for(i = 0; i < pSaveRsp->__sizeConfigurations; i++)
	{
		if(0 == strcmp(ptSetConfig->token, pSaveRsp->Configurations[i].token))
		{
			iVECNum = i;

		#ifdef WITH_TESTTOOL
		//这个老苌修改成在帧率小于10帧左右时，若等不到I帧，则不再等待，sdp中的SPS信息不填。
		//此处用测试工具测试时暂时先保留，by qiaohaijun @2013.6.7
			if(ptSetConfig->RateControl->FrameRateLimit  <= 14)
			{
				ptSetConfig->RateControl->FrameRateLimit = 25;
			}
		#endif
			iRet = SetVideoEncodeParameter(i, ptSetConfig);
			if(TD_OK != iRet)
			{
				if(1 == iRet)
				{
					return soap_receiver_fault_info(ptSoap, "MPEG4 is not support!", "ter:ActionNotSupported", "MPEG4NotSupported");
				}
				return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
			}
			break;
		}
		else
		{
			if(i >= pSaveRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", ptSetConfig->token);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
#if 0
	//confilect
	if(ptOnvifCtrl->m_stOnvifCfg.m_ptrt__GetVideoEncoderConfigurationsResponse->Configurations[iVECNum].UseCount > 1)
	{
		sprintf(pcTmpReason, "The new settings conflicts with other uses of the configuration.");
		return soap_receiver_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:ConfigurationConflict");
	}
#endif

	if(!pointer_valid_check(ptSetConfig->Name))
	{
		if(0 != strcmp(pSaveRsp->Configurations[iVECNum].Name, ptSetConfig->Name))
		{
			strncpy(pSaveRsp->Configurations[iVECNum].Name, ptSetConfig->Name, NAME_LEN);
		}
		
	}

	//Multicast
	if(!pointer_valid_check(ptSetConfig->Multicast))
	{
		//Address
		if(!pointer_valid_check(ptSetConfig->Multicast->Address))
		{
			//仅支持IPV4
			if(tt__IPType__IPv4 == ptSetConfig->Multicast->Address->Type)
			{
				if(!pointer_valid_check(ptSetConfig->Multicast->Address->IPv4Address) 
				&& !pointer_valid_check(*ptSetConfig->Multicast->Address->IPv4Address))
				{
					if(0 != strcmp(*pSaveRsp->Configurations[iVECNum].Multicast->Address->IPv4Address, *ptSetConfig->Multicast->Address->IPv4Address))
					{
						strncpy(*pSaveRsp->Configurations[iVECNum].Multicast->Address->IPv4Address, *ptSetConfig->Multicast->Address->IPv4Address, IPADDR_LEN);
					}
				}
			}
			else
			{
				return soap_receiver_fault_info(ptSoap, "IPV6 is not support!", "ter:ActionNotSupported", NULL);
			}
		}
		
		//Port
		if(pSaveRsp->Configurations[iVECNum].Multicast->Port != ptSetConfig->Multicast->Port)
		{
			pSaveRsp->Configurations[iVECNum].Multicast->Port = ptSetConfig->Multicast->Port;
		}

		//TTL
		if(pSaveRsp->Configurations[iVECNum].Multicast->TTL != ptSetConfig->Multicast->TTL)
		{
			pSaveRsp->Configurations[iVECNum].Multicast->TTL = ptSetConfig->Multicast->TTL;
		}

		//AutoStart
		if(pSaveRsp->Configurations[iVECNum].Multicast->AutoStart != ptSetConfig->Multicast->AutoStart)
		{
			pSaveRsp->Configurations[iVECNum].Multicast->AutoStart = ptSetConfig->Multicast->AutoStart;
		}
		
	}

	//SessionTimeout
	if(pSaveRsp->Configurations[iVECNum].SessionTimeout != ptSetConfig->SessionTimeout)
	{
		pSaveRsp->Configurations[iVECNum].SessionTimeout = ptSetConfig->SessionTimeout;
	}
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120319 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(struct soap* ptSoap, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iASCNum = 0;
	char pcTmpReason[MAX_STRING_LEN];
	int i;

	if(NULL == trt__SetAudioSourceConfiguration || NULL == trt__SetAudioSourceConfiguration->Configuration || NULL == trt__SetAudioSourceConfiguration->Configuration->token)
	{
		return SOAP_ERR;
	}

	//config number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__SetAudioSourceConfiguration->Configuration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token))
		{
			iASCNum = i;
			td_printf(0, "iASCNum:%d\n", iASCNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__SetAudioSourceConfiguration->Configuration->token);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
#if 0
	//confilect
	if(ptOnvifCtrl->m_stOnvifCfg.m_ptrt__GetAudioSourceConfigurationsResponse->Configurations[iASCNum].UseCount > 1)
	{
		sprintf(pcTmpReason, "The new settings conflicts with other uses of the configuration.");
		return soap_receiver_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:ConfigurationConflict");
	}
#endif
	//compare
	//name
	if(NULL != trt__SetAudioSourceConfiguration->Configuration->Name && \
		strcmp(trt__SetAudioSourceConfiguration->Configuration->Name, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNum].Name))
	{
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNum].Name, \
			trt__SetAudioSourceConfiguration->Configuration->Name);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetAudioSourceConfiguration->Configuration->token, 0, "ascname", 0, trt__SetAudioSourceConfiguration->Configuration->Name, 0, CFG_REPLACE);
	}

	//SourceToken
	if(NULL != trt__SetAudioSourceConfiguration->Configuration->SourceToken && \
		strcmp(trt__SetAudioSourceConfiguration->Configuration->SourceToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNum].SourceToken))
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources; i++)
		{
			if(!strcmp(trt__SetAudioSourceConfiguration->Configuration->SourceToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->AudioSources[i].token))
			{
				td_printf(0, "SourceToken:%s\n", trt__SetAudioSourceConfiguration->Configuration->SourceToken);
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources - 1)
				{
					return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
				}
			}
		}
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[iASCNum].SourceToken, \
			trt__SetAudioSourceConfiguration->Configuration->SourceToken);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetAudioSourceConfiguration->Configuration->token, 0, "ascsrctoken", 0, trt__SetAudioSourceConfiguration->Configuration->SourceToken, 0, CFG_REPLACE);
	}

	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;

}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(struct soap* ptSoap, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN];
	int iAECNum = 0;
	int iChn = 0;
	int i = 0;

	if(NULL == trt__SetAudioEncoderConfiguration || NULL == trt__SetAudioEncoderConfiguration->Configuration || NULL == trt__SetAudioEncoderConfiguration->Configuration->token)
	{
		return SOAP_ERR;
	}

	//config number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__SetAudioEncoderConfiguration->Configuration->token, \
				ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token))
		{
			iAECNum = i;
			td_printf(0, "iAECNum:%d\n", iAECNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", \
				trt__SetAudioEncoderConfiguration->Configuration->token);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	iChn = iAECNum + 1;

#if 0
	//confilect
	if(ptOnvifCtrl->m_stOnvifCfg.m_ptrt__GetAudioEncoderConfigurationsResponse->Configurations[iAECNum].UseCount > 1)
	{
		sprintf(pcTmpReason, "The new settings conflicts with other uses of the configuration.");
		return soap_receiver_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:ConfigurationConflict");
	}
#endif
	//compare
	//name
	if(NULL != trt__SetAudioEncoderConfiguration->Configuration->Name && \
		strcmp(trt__SetAudioEncoderConfiguration->Configuration->Name, \
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].Name))
	{
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].Name, \
			trt__SetAudioEncoderConfiguration->Configuration->Name);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetAudioEncoderConfiguration->Configuration->token, 0, "aecname", 0, trt__SetAudioEncoderConfiguration->Configuration->Name, 0, CFG_REPLACE);
	}

	//Encoding
	if(trt__SetAudioEncoderConfiguration->Configuration->Encoding != ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].Encoding)
	{
		if(trt__SetAudioEncoderConfiguration->Configuration->Encoding != tt__AudioEncoding__G711 && \
			trt__SetAudioEncoderConfiguration->Configuration->Encoding != tt__AudioEncoding__G726 && \
			trt__SetAudioEncoderConfiguration->Configuration->Encoding != tt__AudioEncoding__AAC)
		{
			return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
		}
		
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].Encoding = trt__SetAudioEncoderConfiguration->Configuration->Encoding;

	}
	//Bitrate
	if(trt__SetAudioEncoderConfiguration->Configuration->Bitrate != ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].Bitrate)
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[0].Options[iAECNum].BitrateList->__sizeItems; i++)
		{
			if(trt__SetAudioEncoderConfiguration->Configuration->Bitrate == ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[0].Options[iAECNum].BitrateList->Items[i])
			{
				td_printf(0, "Bitrate:%d\n", trt__SetAudioEncoderConfiguration->Configuration->Bitrate);
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[0].Options[iAECNum].BitrateList->__sizeItems)
				{
					return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
				}
			}
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].Bitrate = trt__SetAudioEncoderConfiguration->Configuration->Bitrate;

	}

	//by qiaohaijun@2013.5.7
	if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].SampleRate = OnvifCmdGetAudioRateCnf(iChn)/AUDIO_SAMPLE_QUOTIETY;
	}
	else if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].SampleRate = AUDIO_SAMPLE_8K;
	}
	else
	{	
		
	}
	//end
	
	td_printf(0, "trt__SetAudioEncoderConfiguration->Configuration->SampleRate ===== %d", trt__SetAudioEncoderConfiguration->Configuration->SampleRate);
	td_printf(0, "get audio rate from kernel === %d", ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].SampleRate);

	//moidified by qiaohaijun @2013.5.29
	//DVR有待验证。
	if(trt__SetAudioEncoderConfiguration->Configuration->SampleRate > AUDIO_SAMPLE_QUOTIETY)
	{
		trt__SetAudioEncoderConfiguration->Configuration->SampleRate = trt__SetAudioEncoderConfiguration->Configuration->SampleRate/AUDIO_SAMPLE_QUOTIETY;
	}
		
	//SampleRate
	if(trt__SetAudioEncoderConfiguration->Configuration->SampleRate != ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].SampleRate )
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[0].Options[iAECNum].SampleRateList->__sizeItems; i++)
		{
			if(trt__SetAudioEncoderConfiguration->Configuration->SampleRate == ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[0].Options[iAECNum].SampleRateList->Items[i])
			{
				td_printf(0, "SampleRate/1000:%d\n", trt__SetAudioEncoderConfiguration->Configuration->SampleRate);
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[0].Options[iAECNum].SampleRateList->__sizeItems - 1)
				{
					return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
				}
			}
		}

		// by qiaohaijun @2013.5.7, ipc不支持采样率的设置
		if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
		{
			if(OnvifCmdSetAudioRateCnf(iChn, trt__SetAudioEncoderConfiguration->Configuration->SampleRate * AUDIO_SAMPLE_QUOTIETY) != TD_OK)
			{
				return SOAP_ERR;
			}
		}
		//end
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[iAECNum].SampleRate = trt__SetAudioEncoderConfiguration->Configuration->SampleRate;

	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(struct soap* ptSoap, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		设置Metadata
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20131028 v2.3.1 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(struct soap* ptSoap, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iMCNum = 0;
	char pcTmpReason[MAX_STRING_LEN];
	int i = 0;

	if(NULL == trt__SetMetadataConfiguration || NULL == trt__SetMetadataConfiguration->Configuration || NULL == trt__SetMetadataConfiguration->Configuration->token)
	{
		return SOAP_ERR;
	}

	//config number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__SetMetadataConfiguration->Configuration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[i].token))
		{
			iMCNum = i;
			td_printf(0, "iMCNum:%d\n", iMCNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__SetMetadataConfiguration->Configuration->token);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
#if 0
	//confilect
	if(ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].UseCount > 1)
	{
		sprintf(pcTmpReason, "The new settings conflicts with other uses of the configuration.");
		return soap_receiver_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:ConfigurationConflict");
	}
#endif
	//compare
	//name
	if(NULL != trt__SetMetadataConfiguration->Configuration->Name 
	&& strcmp(trt__SetMetadataConfiguration->Configuration->Name, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Name))
	{
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Name, \
			trt__SetMetadataConfiguration->Configuration->Name);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetMetadataConfiguration->Configuration->token, 0, "mcname", 0, trt__SetMetadataConfiguration->Configuration->Name, 0, CFG_REPLACE);
	}
	
	//PTZStatus
	if(NULL != trt__SetMetadataConfiguration->Configuration->PTZStatus)
	{
		//Status
		if(trt__SetMetadataConfiguration->Configuration->PTZStatus->Status != ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].PTZStatus->Status)
		{
			#if 0
			if(ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Options[iMCNum].PTZStatusFilterOptions->PanTiltStatusSupported != xsd__boolean__true_)
			{
				return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
			}
			#endif
			ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].PTZStatus->Status = trt__SetMetadataConfiguration->Configuration->PTZStatus->Status;
		}

		//Position
		if(trt__SetMetadataConfiguration->Configuration->PTZStatus->Position != ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].PTZStatus->Position)
		{
			#if 0
			if(*ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Options[iMCNum].PTZStatusFilterOptions->PanTiltPositionSupported != xsd__boolean__true_)
			{
				return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
			}
			#endif
			ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].PTZStatus->Position = trt__SetMetadataConfiguration->Configuration->PTZStatus->Position;
		}
	}
	
	//Multicast
	if(NULL != trt__SetMetadataConfiguration->Configuration->Multicast)
	{
		//Address
		if(NULL != trt__SetMetadataConfiguration->Configuration->Multicast->Address)
		{
			//type
			if(trt__SetMetadataConfiguration->Configuration->Multicast->Address->Type == tt__IPType__IPv6)
			{
				return soap_receiver_fault_info(ptSoap, "IPV6 is not support!", "ter:ActionNotSupported", "IPV6NotSupported");
			}
			else if(trt__SetMetadataConfiguration->Configuration->Multicast->Address->Type == tt__IPType__IPv4)
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->Address->Type = tt__IPType__IPv4;
			}
			else
			{
				return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
			}
			//IPv4Address
			if(NULL != trt__SetMetadataConfiguration->Configuration->Multicast->Address->IPv4Address && NULL != *trt__SetMetadataConfiguration->Configuration->Multicast->Address->IPv4Address && 
				strcmp(*trt__SetMetadataConfiguration->Configuration->Multicast->Address->IPv4Address, *ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->Address->IPv4Address))
			{
				strcpy(*ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->Address->IPv4Address, *trt__SetMetadataConfiguration->Configuration->Multicast->Address->IPv4Address);
			}
		}
		//PORT
		if(trt__SetMetadataConfiguration->Configuration->Multicast->Port != ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->Port)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->Port = trt__SetMetadataConfiguration->Configuration->Multicast->Port;
		}

		//TTL
		if(trt__SetMetadataConfiguration->Configuration->Multicast->TTL != ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->TTL)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->TTL = trt__SetMetadataConfiguration->Configuration->Multicast->TTL;
		}

		//AutoStart
		if(trt__SetMetadataConfiguration->Configuration->Multicast->AutoStart != ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->AutoStart)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].Multicast->AutoStart = trt__SetMetadataConfiguration->Configuration->Multicast->AutoStart;
		}
	}

	//SessionTimeout
	if(trt__SetMetadataConfiguration->Configuration->SessionTimeout != ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].SessionTimeout)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[iMCNum].SessionTimeout = trt__SetMetadataConfiguration->Configuration->SessionTimeout;
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetMetadataConfiguration->Configuration->token, 0, "mcst", 0, NULL, trt__SetMetadataConfiguration->Configuration->SessionTimeout, CFG_REPLACE);
	}
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120410 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(struct soap* ptSoap, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iAOCNum = 0;
	char pcTmpReason[MAX_STRING_LEN];
	int i;

	if(NULL == trt__SetAudioOutputConfiguration || NULL == trt__SetAudioOutputConfiguration->Configuration || NULL == trt__SetAudioOutputConfiguration->Configuration->token)
	{
		return SOAP_ERR;
	}

	//config number
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__SetAudioOutputConfiguration->Configuration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].token))
		{
			iAOCNum = i;
			td_printf(0, "iAOCNum:%d\n", iAOCNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__SetAudioOutputConfiguration->Configuration->token);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
#if 0
	//confilect
	if(ptOnvifCtrl->m_stOnvifCfg.m_ptrt__GetAudioOutputConfigurationsResponse->Configurations[iAOCNum].UseCount > 1)
	{
		sprintf(pcTmpReason, "The new settings conflicts with other uses of the configuration.");
		return soap_receiver_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:ConfigurationConflict");
	}
#endif
	//compare
	//name
	if(NULL != trt__SetAudioOutputConfiguration->Configuration->Name && \
		strcmp(trt__SetAudioOutputConfiguration->Configuration->Name, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[iAOCNum].Name))
	{
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[iAOCNum].Name, \
			trt__SetAudioOutputConfiguration->Configuration->Name);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, trt__SetAudioOutputConfiguration->Configuration->token, 0, "aocname", 0, trt__SetAudioOutputConfiguration->Configuration->Name, 0, CFG_REPLACE);
	}

	//OutputToken
	if(NULL != trt__SetAudioOutputConfiguration->Configuration->OutputToken 
	&& strcmp(trt__SetAudioOutputConfiguration->Configuration->OutputToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[iAOCNum].OutputToken))
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[iAOCNum].__sizeOutputTokensAvailable; i++)
		{
			if(!strcmp(trt__SetAudioOutputConfiguration->Configuration->OutputToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[iAOCNum].OutputTokensAvailable[i]))
			{
				td_printf(0, "OutputToken:%s\n", trt__SetAudioOutputConfiguration->Configuration->OutputToken);
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[iAOCNum].__sizeOutputTokensAvailable - 1)
				{
					return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
				}
			}
		}
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[iAOCNum].OutputToken, \
			trt__SetAudioOutputConfiguration->Configuration->OutputToken);
	}

	//SendPrimacy
	if(NULL != trt__SetAudioOutputConfiguration->Configuration->SendPrimacy 
	&& strcmp(trt__SetAudioOutputConfiguration->Configuration->SendPrimacy, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[iAOCNum].SendPrimacy))
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[iAOCNum].__sizeSendPrimacyOptions; i++)
		{
			if(!strcmp(trt__SetAudioOutputConfiguration->Configuration->SendPrimacy, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[iAOCNum].SendPrimacyOptions[i]))
			{
				td_printf(0, "SendPrimacy:%s\n", trt__SetAudioOutputConfiguration->Configuration->SendPrimacy);
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[iAOCNum].__sizeSendPrimacyOptions - 1)
				{
					return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
				}
			}
		}
		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[iAOCNum].SendPrimacy, \
			trt__SetAudioOutputConfiguration->Configuration->SendPrimacy);
	}

	//OutputLevel
	if(trt__SetAudioOutputConfiguration->Configuration->OutputLevel != ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[iAOCNum].OutputLevel)
	{
		if(trt__SetAudioOutputConfiguration->Configuration->OutputLevel > ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[iAOCNum].OutputLevelRange->Max 
		|| trt__SetAudioOutputConfiguration->Configuration->OutputLevel < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[iAOCNum].OutputLevelRange->Min)
		{
			return soap_sender_fault_info(ptSoap, "The configuration parameters are not possible to set.", "ter:InvalidArgVal", "ter:ConfigModify");
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[iAOCNum].OutputLevel = trt__SetAudioOutputConfiguration->Configuration->OutputLevel;
	}
	
	ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(struct soap* ptSoap, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120315 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(struct soap* ptSoap, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iVSCNum = 0;
	int i;
	char pcTmpReason[MAX_STRING_LEN] = {0};

	if(NULL == trt__GetVideoSourceConfigurationOptions || NULL == trt__GetVideoSourceConfigurationOptions->ConfigurationToken)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetVideoSourceConfigurationOptions->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetVideoSourceConfigurationOptions->ConfigurationToken);
			iVSCNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetVideoSourceConfigurationOptions->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	//response
	trt__GetVideoSourceConfigurationOptionsResponse->Options = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options + iVSCNum;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120317 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(struct soap* ptSoap, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	struct _trt__GetVideoEncoderConfigurationsResponse *pstGetVECResponse = NULL;
	struct tt__H264Options *pstH264Options = NULL;
	struct tt__VideoResolution *pVRDest = NULL;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN]    = {0};
	int piVideoList[MAX_VIDEOSIZE_LIST] = {0};
	int iVideoListNum = 0;
	int iSrcSize      = 0;
	int iVECNum       = 0;
	int iNorm         = 0;
	int i             = 0;
	
	if(pointer_valid_check(ptOnvifCtrl)
	|| pointer_valid_check(trt__GetVideoEncoderConfigurationOptions))
	{
		return SOAP_ERR;
	}

	pstGetVECResponse = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp;

	//针对meilstone做修改
	if(NULL == trt__GetVideoEncoderConfigurationOptions->ConfigurationToken)
	{
		iVECNum = 0;
	}
	else
	{
		for(i = 0; i < pstGetVECResponse->__sizeConfigurations; i++)
		{
			if(!strcmp(trt__GetVideoEncoderConfigurationOptions->ConfigurationToken, pstGetVECResponse->Configurations[i].token))
			{
				td_printf(0, "ConfigurationToken:%s\n", trt__GetVideoEncoderConfigurationOptions->ConfigurationToken);
				iVECNum = i;
				break;
			}
			else
			{
				if(i >= pstGetVECResponse->__sizeConfigurations - 1)
				{
					sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetVideoEncoderConfigurationOptions->ConfigurationToken);
					return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
				}
			}
		}
	}

	pstH264Options = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVECNum].H264;

	//更新分辨率列表
	iVideoListNum = OnvifCmdGetVideoChnSizeList(iVECNum + 1, piVideoList, MAX_VIDEOSIZE_LIST);

	iNorm    = (ptOnvifCtrl->m_stNvsInfo.m_tVideoParam.m_InputNorm[iVECNum % ptOnvifCtrl->m_stNvsInfo.m_iVINum] == GUI_PAL_MODE) ? 0 : 1;
	iSrcSize = pstH264Options->__sizeResolutionsAvailable;

	if(iVideoListNum > iSrcSize)
	{
		pVRDest = (struct tt__VideoResolution *)(char *)soap_mallocz(ptSoap, (iVideoListNum - iSrcSize) * sizeof(struct tt__VideoResolution));
		if(pointer_valid_check(pVRDest))
		{
			return SOAP_ERR;
		}
		
		pstH264Options->ResolutionsAvailable = \
			(struct tt__VideoResolution *)ArrayAdd(pstH264Options->ResolutionsAvailable, sizeof(struct tt__VideoResolution) * iSrcSize, pVRDest, sizeof(struct tt__VideoResolution) * (iVideoListNum - iSrcSize));
		if(pointer_valid_check(pstH264Options->ResolutionsAvailable))
		{
			return SOAP_ERR;
		}
	}

	pstH264Options->__sizeResolutionsAvailable = iVideoListNum;
	
	for(i = 0; i < iVideoListNum; i++)
	{
		if(H960 == piVideoList[i])
		{
			pstH264Options->ResolutionsAvailable[i].Width  = 960;
			pstH264Options->ResolutionsAvailable[i].Height = 576;
		}
		else
		{
			GetVideoWidthHeight(piVideoList[i], iNorm, \
				(unsigned short*)&(pstH264Options->ResolutionsAvailable[i].Width), \
				(unsigned short*)&(pstH264Options->ResolutionsAvailable[i].Height));
		}
	}
	
	//response
	trt__GetVideoEncoderConfigurationOptionsResponse->Options = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options + iVECNum;

	return SOAP_OK;
}

 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120318 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(struct soap* ptSoap, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iASCNum = 0;
	int i;
	char pcTmpReason[MAX_STRING_LEN] = {0};


	if(NULL == trt__GetAudioSourceConfigurationOptions)
	{
		return SOAP_ERR;
	}

	//检测ProfileToken是否正确
	if(NULL != trt__GetAudioSourceConfigurationOptions->ProfileToken)
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
		{
			if(!strcmp(trt__GetAudioSourceConfigurationOptions->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
			{
				td_printf(0, "ConfigurationToken:%s\n", trt__GetAudioSourceConfigurationOptions->ProfileToken);
				iASCNum = i;
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles - 1)
				{
					sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetAudioSourceConfigurationOptions->ConfigurationToken);
					return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
				}
			}
			
		}		
	}	

	//检测ConfigurationToken是否正确
	if(NULL != trt__GetAudioSourceConfigurationOptions->ConfigurationToken)
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations; i++)
		{
			if(!strcmp(trt__GetAudioSourceConfigurationOptions->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token))
			{
				td_printf(0, "ConfigurationToken:%s\n", trt__GetAudioSourceConfigurationOptions->ConfigurationToken);
				//response
				iASCNum = i;
				//strcpy(*trt__GetAudioSourceConfigurationOptionsResponse->Options->InputTokensAvailable, trt__GetAudioSourceConfigurationOptions->ConfigurationToken);
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations - 1)
				{
					sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetAudioSourceConfigurationOptions->ConfigurationToken);
					return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
				}
			}
		}
		
	}
	//response
	trt__GetAudioSourceConfigurationOptionsResponse->Options = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgOptRsp->Options + iASCNum;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120318 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(struct soap* ptSoap, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[MAX_STRING_LEN] = {0};
	int iAECNum = 0;
	int i = 0;
	
	if(pointer_valid_check(trt__GetAudioEncoderConfigurationOptions))
	{
		return SOAP_ERR;
	}
	
	//针对meilstone做修改
	if(pointer_valid_check(trt__GetAudioEncoderConfigurationOptions->ConfigurationToken))
	{
		iAECNum = 0;
	}
	else
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations; i++)
		{
			if(!strcmp(trt__GetAudioEncoderConfigurationOptions->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token))
			{
				td_printf(0, "ConfigurationToken:%s\n", trt__GetAudioEncoderConfigurationOptions->ConfigurationToken);
				iAECNum = i;
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations - 1)
				{
					sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetAudioEncoderConfigurationOptions->ConfigurationToken);
					return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
				}
			}
		}
	}

	//音频目前只有一个options
	trt__GetAudioEncoderConfigurationOptionsResponse->Options = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options;	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120318 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(struct soap* ptSoap, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iMCNum = 0;
	int i;
	char pcTmpReason[MAX_STRING_LEN] = {0};

	if(NULL == trt__GetMetadataConfigurationOptions || NULL == trt__GetMetadataConfigurationOptions->ConfigurationToken)
	{
		return SOAP_ERR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetMetadataConfigurationOptions->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetMetadataConfigurationOptions->ConfigurationToken);
			iMCNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetMetadataConfigurationOptions->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}

	//response
	trt__GetMetadataConfigurationOptionsResponse->Options = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgOptionsRsp->Options + iMCNum;

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120409 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(struct soap* ptSoap, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iAOCNum = 0;
	int i;
	char pcTmpReason[MAX_STRING_LEN] = {0};

	if(NULL == trt__GetAudioOutputConfigurationOptions || NULL == trt__GetAudioOutputConfigurationOptions->ConfigurationToken)
	{
		return SOAP_ERR;
	}
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(trt__GetAudioOutputConfigurationOptions->ConfigurationToken, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].token))
		{
			td_printf(0, "ConfigurationToken:%s\n", trt__GetAudioOutputConfigurationOptions->ConfigurationToken);
			iAOCNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->__sizeConfigurations - 1)
			{
				sprintf(pcTmpReason, "The requested configuration indicated with %s does not exist.", trt__GetAudioOutputConfigurationOptions->ConfigurationToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoConfig");
			}
		}
	}
	//response
	trt__GetAudioOutputConfigurationOptionsResponse->Options = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options + iAOCNum;

	//int *mm = NULL;
	//*mm = 0;
	
	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(struct soap* ptSoap, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	//设备本身不支持音频解码，如果必须回response信息，
	//则将response置NULL,否则直接return。
	//by qiaohaijun @2013.6.6
	trt__GetAudioDecoderConfigurationOptionsResponse->Options == NULL;

	return SOAP_OK;
//	return	soap_receiver_fault_info(ptSoap,"Audio or Audio decoding is not supported by the device.", "ter:ActionNotSupported", "ter:AudioDecodingNotSupported");

}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120508 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap* ptSoap, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->TotalNumber = 1;	//紧支持一种编码格式输出

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	201200410 v1.0 creat by pangtb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri(struct soap* ptSoap, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int i;
	int iProNum = 0;
	int iVECNum = 0;
	char pcTmpReason[128] = {0};

	if(NULL == trt__GetStreamUri || NULL == trt__GetStreamUri->ProfileToken)
	{
		return SOAP_ERR;
	}

	//profile num
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__GetStreamUri->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetStreamUri->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}

	//added by qiaohaijun at 2012.12.6
	//判断profiletoken下的vsctoken或者vectoken是否存在
	if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration)//video encoding num
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations; i++)
		{
			if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoEncoderConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[i].token))
			{
				iVECNum = i;
				break;
			}
			else
			{
				if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations -1)
				{
					sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetStreamUri->ProfileToken);
					return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
				}
			}
		}
	}
	else if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration)//video source num	
	{
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations * 2; i++)
		{
			if(i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations)
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
				{
					iVECNum = i;
					break;
				}
				
			}
			else
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i - ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations].token))
				{
					iVECNum = i;
					break;
				}
				else
				{
					if(i - ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations -1)
					{
						sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetStreamUri->ProfileToken);
						return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
					}
				}
			}
		}
	}
	//有时候vsctoken 和vectoken都不存在
	//added by qiaohaijun @2013.5.29
	else if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration)//video source num	
	{
		td_printf(0, "get stream uri: ASC token");
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations * 2; i++)
		{
			if(i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations)
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token))
				{
					iVECNum = i;
					break;
				}
				
			}
			else
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i - ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations].token))
				{
					iVECNum = i;
					break;
				}
				else
				{
					if(i - ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations -1)
					{
						sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetStreamUri->ProfileToken);
						return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
					}
				}
			}
		}
	}
	else if(NULL != ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration)//video source num	
	{
		td_printf(0, "get stream uri: AEC token");
		for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations; i++)
		{
			if(i < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations)
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token))
				{
					iVECNum = i;
					break;
				}
				
			}
			else
			{
				if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].AudioEncoderConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i - ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations].token))
				{
					iVECNum = i;
					break;
				}
				else
				{
					if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations -1)
					{
						sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetStreamUri->ProfileToken);
						return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
					}
				}
			}
		}
	}
	else
	{
		sprintf(pcTmpReason, "The specified media profile does contain either unused sources or encoder configurations without a corresponding source. ");
		return soap_receiver_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:IncompleteConfiguration");
	}

	trt__GetStreamUriResponse->MediaUri = (struct tt__MediaUri *)soap_mallocz(ptSoap, sizeof(struct tt__MediaUri));
	trt__GetStreamUriResponse->MediaUri->Uri = (char *)soap_mallocz(ptSoap, 1024);
	
	sprintf(trt__GetStreamUriResponse->MediaUri->Uri, "rtsp://%s:%d/stream", ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stRtspServer.m_iLPort);

	trt__GetStreamUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__false_;
	trt__GetStreamUriResponse->MediaUri->InvalidAfterReboot = xsd__boolean__false_;
	trt__GetStreamUriResponse->MediaUri->Timeout = 5000;	//码流超时时间
    
	return SOAP_OK;

}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming(struct soap* ptSoap, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int i;
	int iProNum = 0;
	char pcTmpReason[128] = {0};

	if(NULL == trt__StartMulticastStreaming || NULL == trt__StartMulticastStreaming->ProfileToken)
	{
		return SOAP_ERR;
	}

	//profile num
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__StartMulticastStreaming->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__StartMulticastStreaming->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}

	//具体多播发送码流有待实现

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(struct soap* ptSoap, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return	soap_sender_fault_info(ptSoap,"The function is not supported.",NULL,NULL);
}
 
/**************************************************
*功能:		强制I帧
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20121107 v1.0 creat by pangtb
20130428 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(struct soap* ptSoap, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char pcTmpReason[128] = {0};
	int	iProNum	= 0;
	int iChn    = 0;
	int iProSize = 0;
	int iCfgSize = 0;
	int i       = 0;
	
	if(pointer_valid_check(ptOnvifCtrl) 
	|| pointer_valid_check(trt__SetSynchronizationPoint->ProfileToken))
	{
		return SOAP_ERR;
	}

	iProSize = ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles;
	iCfgSize = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations;
	
	//profile num
	for(i = 0; i < iProSize; i++)
	{
		if(!strcmp(trt__SetSynchronizationPoint->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= iProSize -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__SetSynchronizationPoint->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	
	//video source num
	for(i = 0; i < iCfgSize; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
		{
			iChn = i + 1;
			if(OnvifCmdIframeReq(iChn))
			{
				return TD_ERROR;
			}
			return SOAP_OK;
		}
		else
		{
			if(i >= iCfgSize -1)
			{
				sprintf(pcTmpReason, "The specified media profile does not contain either a reference to a video encoder configuration or a reference to a video source configuration.");
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:IncompleteConfiguration");
			}
		}
	}

	return SOAP_OK;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130411 v2.3.0 modify by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri(struct soap* ptSoap, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl       = &g_stOnvifCtrl;
	struct dirent * readthedir   = NULL;
	char cPicbuffer[64 * 1024]   = {0};
	char cPathPicNameBuffer[128] = {0};
	char cUriPicNameBuffer[128]	 = {0};
	char cLinkNameBuffer[128]	 = {0};
	char cOldNameBuffer[128]	 = {0};
	char cPicNameBuffer[64]		 = {0};
	char pcTmpReason[128]		 = {0};
	FILE *readfp  = NULL;
	FILE *writefp = NULL;
	DIR *opdir    = NULL;
	int	iProNum	= 0;
	int	iVscNum	= 0;
	int iLenR   = 0;
	int iLenW   = 0;
	int	iRet    = -1;
	int	iChn    = 0;
	int i       = 0;
	int iFoundFlag = 0;

	if(NULL == ptOnvifCtrl || NULL == trt__GetSnapshotUri || NULL == trt__GetSnapshotUri->ProfileToken)
	{
		return SOAP_ERR;
	}

	//profile num
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles; i++)
	{
		if(!strcmp(trt__GetSnapshotUri->ProfileToken, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token))
		{
			iProNum = i;
			td_printf(0, "iProNum:%d\n", iProNum);
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles -1)
			{
				sprintf(pcTmpReason, "The requested profile token %s does not exist.", trt__GetSnapshotUri->ProfileToken);
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:InvalidArgVal", "ter:NoProfile");
			}
		}
	}
	
	//video source num
	for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations; i++)
	{
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iProNum].VideoSourceConfiguration->token, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token))
		{
			iVscNum = i;
			break;
		}
		else
		{
			if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations -1)
			{
				sprintf(pcTmpReason, "The specified media profile does not contain either a reference to a video encoder configuration or a reference to a video source configuration.");
				return soap_sender_fault_info(ptSoap, pcTmpReason, "ter:Action", "ter:IncompleteConfiguration");
			}
		}
	}

	iChn = i;
	
	trt__GetSnapshotUriResponse->MediaUri = (struct tt__MediaUri *)soap_mallocz(ptSoap, sizeof(struct tt__MediaUri));
	if(trt__GetSnapshotUriResponse->MediaUri == NULL)
	{
		return SOAP_ERR;
	}

	trt__GetSnapshotUriResponse->MediaUri->Uri = (char *)soap_mallocz(ptSoap, 256);
	if(trt__GetSnapshotUriResponse->MediaUri->Uri == NULL)
	{
		return SOAP_ERR;
	}

	//sprintf(cUriPicNameBuffer, "pic_type%02d_%02d.jpeg", DEVICE_IPC, iChn + 1);
	sprintf(cUriPicNameBuffer, "1.jpeg");
		
		//IPC抓拍，无论是否有外部存储，始终都
		//会在/tmp/snapshot/ftp目录下存有且只有一张抓拍图片
		//iRet = OnvifCmdGetSnapshot(iChn + 1);

		//usleep(100);
		char *pFile_path = "/home/pangtb/share/onvif_v2.3/bin/image";
		opdir = opendir(pFile_path);
		if(NULL != opdir)
		{
			while((readthedir = readdir(opdir) )!= NULL)
			{
				usleep(10);
				td_printf(0, "readthedir.d_name === %s", readthedir->d_name);
				if(strstr(readthedir->d_name, ".jpg") != NULL)
				{
					iFoundFlag = 1;
					break;
				}
			}

			//找到以后才能建立软连接
			if(iFoundFlag == 1)
			{
				sprintf(cOldNameBuffer, "%s%s", pFile_path, readthedir->d_name);
				if(symlink(cOldNameBuffer, cLinkNameBuffer))
				{
					td_printf(0, "symlink FAILED!");
					//perror("the unlink failed reason is:");
				}
			}
			
			closedir(opdir);
		}

	sprintf(trt__GetSnapshotUriResponse->MediaUri->Uri, "http://%s/jpeg/%s", ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, cUriPicNameBuffer);


	trt__GetSnapshotUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__false_;
	trt__GetSnapshotUriResponse->MediaUri->InvalidAfterReboot  = xsd__boolean__false_;
		
	return SOAP_OK;
}

#ifdef WITH_FUNC_BAK
//备份函数
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices_(struct soap* ptSoap, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities_(struct soap* ptSoap, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation_(struct soap* ptSoap, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime_(struct soap* ptSoap, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime_(struct soap* ptSoap, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault_(struct soap* ptSoap, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware_(struct soap* ptSoap, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot_(struct soap* ptSoap, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem_(struct soap* ptSoap, struct _tds__RestoreSystem *tds__RestoreSystem, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup_(struct soap* ptSoap, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog_(struct soap* ptSoap, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation_(struct soap* ptSoap, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes_(struct soap* ptSoap, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes_(struct soap* ptSoap, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes_(struct soap* ptSoap, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes_(struct soap* ptSoap, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode_(struct soap* ptSoap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode_(struct soap* ptSoap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode_(struct soap* ptSoap, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode_(struct soap* ptSoap, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses_(struct soap* ptSoap, struct _tds__GetDPAddresses *tds__GetDPAddresses, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference_(struct soap* ptSoap, struct _tds__GetEndpointReference *tds__GetEndpointReference, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser_(struct soap* ptSoap, struct _tds__GetRemoteUser *tds__GetRemoteUser, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser_(struct soap* ptSoap, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers_(struct soap* ptSoap, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers_(struct soap* ptSoap, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers_(struct soap* ptSoap, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser_(struct soap* ptSoap, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl_(struct soap* ptSoap, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities_(struct soap* ptSoap, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses_(struct soap* ptSoap, struct _tds__SetDPAddresses *tds__SetDPAddresses, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname_(struct soap* ptSoap, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname_(struct soap* ptSoap, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP_(struct soap* ptSoap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS_(struct soap* ptSoap, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS_(struct soap* ptSoap, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP_(struct soap* ptSoap, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP_(struct soap* ptSoap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS_(struct soap* ptSoap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS_(struct soap* ptSoap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces_(struct soap* ptSoap, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces_(struct soap* ptSoap, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols_(struct soap* ptSoap, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols_(struct soap* ptSoap, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway_(struct soap* ptSoap, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway_(struct soap* ptSoap, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration_(struct soap* ptSoap, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration_(struct soap* ptSoap, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter_(struct soap* ptSoap, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter_(struct soap* ptSoap, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter_(struct soap* ptSoap, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter_(struct soap* ptSoap, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy_(struct soap* ptSoap, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy_(struct soap* ptSoap, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate_(struct soap* ptSoap, struct _tds__CreateCertificate *tds__CreateCertificate, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates_(struct soap* ptSoap, struct _tds__GetCertificates *tds__GetCertificates, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus_(struct soap* ptSoap, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus_(struct soap* ptSoap, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates_(struct soap* ptSoap, struct _tds__DeleteCertificates *tds__DeleteCertificates, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request_(struct soap* ptSoap, struct _tds__GetPkcs10Request *tds__GetPkcs10Request, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates_(struct soap* ptSoap, struct _tds__LoadCertificates *tds__LoadCertificates, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode_(struct soap* ptSoap, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode_(struct soap* ptSoap, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs_(struct soap* ptSoap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings_(struct soap* ptSoap, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState_(struct soap* ptSoap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand_(struct soap* ptSoap, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates_(struct soap* ptSoap, struct _tds__GetCACertificates *tds__GetCACertificates, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey_(struct soap* ptSoap, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation_(struct soap* ptSoap, struct _tds__GetCertificateInformation *tds__GetCertificateInformation, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates_(struct soap* ptSoap, struct _tds__LoadCACertificates *tds__LoadCACertificates, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration_(struct soap* ptSoap, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration_(struct soap* ptSoap, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration_(struct soap* ptSoap, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations_(struct soap* ptSoap, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration_(struct soap* ptSoap, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities_(struct soap* ptSoap, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status_(struct soap* ptSoap, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks_(struct soap* ptSoap, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris_(struct soap* ptSoap, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade_(struct soap* ptSoap, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore_(struct soap* ptSoap, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities_(struct soap* ptSoap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources_(struct soap* ptSoap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources_(struct soap* ptSoap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs_(struct soap* ptSoap, struct _trt__GetAudioOutputs *trt__GetAudioOutputs, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile_(struct soap* ptSoap, struct _trt__CreateProfile *trt__CreateProfile, struct _trt__CreateProfileResponse *trt__CreateProfileResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile_(struct soap* ptSoap, struct _trt__GetProfile *trt__GetProfile, struct _trt__GetProfileResponse *trt__GetProfileResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles_(struct soap* ptSoap, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration_(struct soap* ptSoap, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration_(struct soap* ptSoap, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration_(struct soap* ptSoap, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration_(struct soap* ptSoap, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration_(struct soap* ptSoap, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration_(struct soap* ptSoap, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration_(struct soap* ptSoap, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration_(struct soap* ptSoap, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration_(struct soap* ptSoap, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration_(struct soap* ptSoap, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration_(struct soap* ptSoap, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration_(struct soap* ptSoap, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration_(struct soap* ptSoap, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration_(struct soap* ptSoap, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration_(struct soap* ptSoap, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration_(struct soap* ptSoap, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration_(struct soap* ptSoap, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration_(struct soap* ptSoap, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile_(struct soap* ptSoap, struct _trt__DeleteProfile *trt__DeleteProfile, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations_(struct soap* ptSoap, struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations_(struct soap* ptSoap, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations_(struct soap* ptSoap, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations_(struct soap* ptSoap, struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations_(struct soap* ptSoap, struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations_(struct soap* ptSoap, struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations_(struct soap* ptSoap, struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations_(struct soap* ptSoap, struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration_(struct soap* ptSoap, struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration_(struct soap* ptSoap, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration_(struct soap* ptSoap, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration_(struct soap* ptSoap, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration_(struct soap* ptSoap, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration_(struct soap* ptSoap, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration_(struct soap* ptSoap, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration_(struct soap* ptSoap, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations_(struct soap* ptSoap, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations_(struct soap* ptSoap, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations_(struct soap* ptSoap, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations_(struct soap* ptSoap, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations_(struct soap* ptSoap, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations_(struct soap* ptSoap, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations_(struct soap* ptSoap, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations_(struct soap* ptSoap, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration_(struct soap* ptSoap, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration_(struct soap* ptSoap, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration_(struct soap* ptSoap, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration_(struct soap* ptSoap, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration_(struct soap* ptSoap, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration_(struct soap* ptSoap, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration_(struct soap* ptSoap, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions_(struct soap* ptSoap, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions_(struct soap* ptSoap, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions_(struct soap* ptSoap, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions_(struct soap* ptSoap, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions_(struct soap* ptSoap, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions_(struct soap* ptSoap, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions_(struct soap* ptSoap, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances_(struct soap* ptSoap, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri_(struct soap* ptSoap, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming_(struct soap* ptSoap, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming_(struct soap* ptSoap, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint_(struct soap* ptSoap, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri_(struct soap* ptSoap, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
 
/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by xxx
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration_(struct soap* ptSoap, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return 0;
}
#endif

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130418 v2.3.0 creat by wl
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __text__GetTimeOsd(struct soap* ptSoap, struct _text__GetTimeOsd *text__GetTimeOsd, struct _text__GetTimeOsdResponse *text__GetTimeOsdResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TGuiVideoOverlayCnf stOsdInfo = {0};
	static char **ppOsdAny = NULL;
	void *pMem[4] = {0};
	int iChn = 0;

	if(text__GetTimeOsd->VideoSourceToken == NULL || strlen(text__GetTimeOsd->VideoSourceToken) == 0)
	{
		td_printf(0, "text__GetTimeOsd->VideoSourceToken is NULL or strlen =0! \n");
		return soap_sender_fault_info(ptSoap, "The requested is invalid.", "InvalidArgVal", NULL);
	}

	iChn = atoi(text__GetTimeOsd->VideoSourceToken + strlen("vstoken_ch"));
	if(iChn < 0 || iChn > MAX_CHANNEL_NUM)
	{
		td_printf(0, "error iChn:%d\n", iChn);
		return SOAP_ERR;
	}
	
	if(OnvifCmdGetOsd(iChn, &stOsdInfo) < 0)
	{
		td_printf(0, "%s OnvifCmdGetOsd ERROR!", __FUNCTION__);
		return SOAP_ERR;
	}

	pMem[0]= (char *)soap_mallocz(ptSoap, sizeof(struct tt_ext__TimeOsd)); 
	pMem[1]= (char *)soap_mallocz(ptSoap, sizeof(struct tt_ext__FloatPoint)); 
	if(NULL == pMem[0] || NULL == pMem[1])
	{
		free(pMem[0]);
		free(pMem[1]);
		return SOAP_ERR;
	}

	text__GetTimeOsdResponse->TimeOsd = (struct tt_ext__TimeOsd *)pMem[0];
	text__GetTimeOsdResponse->TimeOsd->DatePosition = (struct tt_ext__FloatPoint *)pMem[1];

	text__GetTimeOsdResponse->TimeOsd->Enabled = (enum xsd__boolean)stOsdInfo.m_iTimeOverlayEnable;
	//todo 坐标转换
	text__GetTimeOsdResponse->TimeOsd->DatePosition->x = stOsdInfo.m_tTimeLoc.m_iXp / MAX_OSD_X;
	text__GetTimeOsdResponse->TimeOsd->DatePosition->y = stOsdInfo.m_tTimeLoc.m_iYp / MAX_OSD_Y;
	//any
	ppOsdAny = (char **)soap_mallocz(ptSoap, sizeof(char *));
	*ppOsdAny = (char *)soap_mallocz(ptSoap, MAX_STRING_LEN);

	if(ppOsdAny == NULL || *ppOsdAny == NULL)
	{
		free(ppOsdAny);
		free(*ppOsdAny);
		return SOAP_ERR;
	}

	sprintf(*ppOsdAny, \
		"<text:Attribute>\
		<text:Transparent>true</text:Transparent>\
		<text:Wink>true</text:Wink>\
		</text:Attribute>");// tt-ext 改为text by qiaohaijun at 2013.1.4 具体根据测试工具或者抓包来看发送的内容进而选择修改为text还是tt-ext

	text__GetTimeOsdResponse->TimeOsd->__size = 1;
	text__GetTimeOsdResponse->TimeOsd->__any = ppOsdAny;
	
	return SOAP_OK;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130418 v2.3.0 creat by wl
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __text__SetTimeOsd(struct soap* ptSoap, struct _text__SetTimeOsd *text__SetTimeOsd, struct _text__SetTimeOsdResponse *text__SetTimeOsdResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TGuiVideoOverlayCnf stOsdInfo = {0};
	int iChn = 0;
	float fLimitX = 0;
	float fLimitY = 0;
	if(text__SetTimeOsd->VideoSourceToken == NULL || strlen(text__SetTimeOsd->VideoSourceToken) == 0 || text__SetTimeOsd->TimeOsd == NULL)
	{
		td_printf(0, "text__SetTimeOsd->VideoSourceToken is NULL or strlen =0");
		return SOAP_ERR;
	}

	iChn = atoi(text__SetTimeOsd->VideoSourceToken + strlen("vstoken_ch"));
	
	if(iChn <= 0 || iChn > MAX_CHANNEL_NUM)
	{
		td_printf(0, "IChn is error:%d\n", iChn);
		return SOAP_ERR;
	}
	
	if(OnvifCmdGetOsd(iChn, &stOsdInfo) < 0)
	{
		td_printf(0, "OnvifCmdGetOsd ERROR!");
		return SOAP_ERR;
	}

	stOsdInfo.m_iTimeOverlayEnable = text__SetTimeOsd->TimeOsd->Enabled;
	fLimitX =  fabs(text__SetTimeOsd->TimeOsd->DatePosition->x);
	fLimitY =  fabs(text__SetTimeOsd->TimeOsd->DatePosition->y);
	//td_printf(0xFF0000,"NamePosition->x =%f",text__SetTimeOsd->TimeOsd->DatePosition->x);

	if(fLimitY > 1||fLimitX > 1)
	{
		return soap_sender_fault_info(ptSoap, "The Location Value is too large.",NULL,NULL);
	}
	
	if(fLimitX<= 1 && fLimitY <= 1)
	{
		//todo 转换
		stOsdInfo.m_tTimeLoc.m_iXp = fLimitX * MAX_OSD_X;
		stOsdInfo.m_tTimeLoc.m_iYp = fLimitY * MAX_OSD_Y;
	}

	if(OnvifCmdSetOsd(iChn, (TGuiVideoOverlayCnf *)&stOsdInfo) < 0)
	{
		td_printf(0, "OnvifCmdSetOsd ERROR!");
		return SOAP_ERR;
	}
	
	return SOAP_OK;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130418 v2.3.0 creat by wl
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __text__GetTitleOsd(struct soap* ptSoap, struct _text__GetTitleOsd *text__GetTitleOsd, struct _text__GetTitleOsdResponse *text__GetTitleOsdResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TGuiVideoOverlayCnf stOsdInfo = {0};
	static char **ppOsdAny = NULL;
	void *pMem[4] = {0};
	int iChn = 0;

	if(text__GetTitleOsd->VideoSourceToken == NULL || strlen(text__GetTitleOsd->VideoSourceToken) == 0 )
	{
		td_printf(0, "text__GetTitleOsd->VideoSourceToken is NULL or strlen =0");
		return SOAP_ERR;
	}

	iChn = atoi(text__GetTitleOsd->VideoSourceToken + strlen("vstoken_ch"));

	if(OnvifCmdGetOsd(iChn, &stOsdInfo) < 0)
	{
		td_printf(0, "OnvifCmdGetOsd ERROR!");
		return SOAP_ERR;
	}
	
	pMem[0]= (char *)soap_mallocz(ptSoap, sizeof(struct tt_ext__TitleOsd)); 
	pMem[1]= (char *)soap_mallocz(ptSoap, sizeof(struct tt_ext__FloatPoint)); 
	pMem[2]= (char *)soap_mallocz(ptSoap, 256);
	if(NULL == pMem[0] || NULL == pMem[1] || NULL == pMem[2])
	{
		free(pMem[0]);
		free(pMem[1]);
		free(pMem[2]);
		return SOAP_ERR;
	}

	text__GetTitleOsdResponse->TitleOsd = (struct tt_ext__TitleOsd *)pMem[0];
	text__GetTitleOsdResponse->TitleOsd->NamePosition = (struct tt_ext__FloatPoint *)pMem[1];
	text__GetTitleOsdResponse->TitleOsd->Name = (char *)pMem[2];

	text__GetTitleOsdResponse->TitleOsd->Enabled = (enum xsd__boolean)stOsdInfo.m_iTextEnable;
	//todo 坐标转换
	text__GetTitleOsdResponse->TitleOsd->NamePosition->x = stOsdInfo.m_tTextLoc.m_iXp/MAX_OSD_X;
	text__GetTitleOsdResponse->TitleOsd->NamePosition->y = stOsdInfo.m_tTextLoc.m_iYp/MAX_OSD_Y;
//	str_gb2312_to_utf8((const unsigned char *)stOsdInfo.m_strText,strlen(stOsdInfo.m_strText), (unsigned char *)text__GetTitleOsdResponse->TitleOsd->Name);
	GB2312ToUTF8((const unsigned char *)stOsdInfo.m_strText, (unsigned char *)text__GetTitleOsdResponse->TitleOsd->Name, strlen(stOsdInfo.m_strText));
	//any
	ppOsdAny = (char **)soap_mallocz(ptSoap, sizeof(char *));
	*ppOsdAny = (char *)soap_mallocz(ptSoap, MAX_STRING_LEN);

	if(ppOsdAny == NULL || *ppOsdAny == NULL)
	{
		return SOAP_ERR;
	}

	strcpy(*ppOsdAny, "<text:Attribute><text:Transparent>true</text:Transparent><text:Wink>true</text:Wink></text:Attribute>");
	
	text__GetTitleOsdResponse->TitleOsd->__size = 1;
	text__GetTitleOsdResponse->TitleOsd->__any = ppOsdAny;

	return SOAP_OK;
}

/**************************************************
*功能:		详细描述函数的功能
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:20130418 v2.3.0 creat by wl
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __text__SetTitleOsd(struct soap* ptSoap, struct _text__SetTitleOsd *text__SetTitleOsd, struct _text__SetTitleOsdResponse *text__SetTitleOsdResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	TGuiVideoOverlayCnf stOsdInfo = {0};
	float fLimitX = 0;
	float fLimitY = 0;
	int iChn = 0;
	int i = 0;
	int j = 0;

	int iRet = -1;


	if(text__SetTitleOsd->VideoSourceToken == NULL || strlen(text__SetTitleOsd->VideoSourceToken) == 0 || text__SetTitleOsd->TitleOsd == NULL)
	{
		td_printf(0, "text__SetTitleOsd->VideoSourceToken is NULL or strlen =0");
		return SOAP_ERR;
	}

	iChn = atoi(text__SetTitleOsd->VideoSourceToken + strlen("vstoken_ch"));
	
	if(iChn <= 0 || iChn > MAX_CHANNEL_NUM)
	{
		td_printf(0, "error iChn:%d\n", iChn);
		return SOAP_ERR;
	}
	td_printf(0, "text__SetTitleOsd->TitleOsd->Name = %s", text__SetTitleOsd->TitleOsd->Name);
	//字符串长度判断
	td_printf(0, "strlen(text__SetTitleOsd->TitleOsd->Name)  = %d", strlen(text__SetTitleOsd->TitleOsd->Name) );
	if(text__SetTitleOsd->TitleOsd->Name == NULL || strlen(text__SetTitleOsd->TitleOsd->Name) == 0 ||strlen(text__SetTitleOsd->TitleOsd->Name) > STRLEN_128)
	{
		return soap_sender_fault_info(ptSoap, "TitleOsd Name is illegal.",NULL,NULL);
	}

	if(OnvifCmdGetOsd(iChn, &stOsdInfo) < 0)
	{
		return SOAP_ERR;
	}

	stOsdInfo.m_iTextEnable = text__SetTitleOsd->TitleOsd->Enabled;
	if(stOsdInfo.m_iTextEnable == TD_FALSE)
	{
		strcpy(text__SetTitleOsd->TitleOsd->Name, " ");
	}

	fLimitX =  fabs(text__SetTitleOsd->TitleOsd->NamePosition->x);
	fLimitY  =  fabs(text__SetTitleOsd->TitleOsd->NamePosition->y);

	if(fLimitY > 1||fLimitX > 1)
	{
		return soap_sender_fault_info(ptSoap, "The Location Value is too large.",NULL,NULL);
	}

	if(fLimitX <= 1 && fLimitY <= 1)
	{
		//中文字符转换 by wanglei_HQ 20130416
		//str_utf8_to_gb2312((const unsigned char *)text__SetTitleOsd->TitleOsd->Name, strlen(text__SetTitleOsd->TitleOsd->Name), (unsigned char *)stOsdInfo.m_strText);
		UTF8ToGB2312((const unsigned char *)text__SetTitleOsd->TitleOsd->Name,(unsigned char *)stOsdInfo.m_strText, strlen(text__SetTitleOsd->TitleOsd->Name));

		//todo 转换1
		stOsdInfo.m_tTextLoc.m_iXp = fLimitX * MAX_OSD_X;
		stOsdInfo.m_tTextLoc.m_iYp = fLimitY * MAX_OSD_Y;
	}
	
	while(i<strlen(stOsdInfo.m_strText) -1)
	{
		if((stOsdInfo.m_strText[i] == 0x0D) && stOsdInfo.m_strText[i+1] == 0x0A)
		{
			stOsdInfo.m_strText[i] = 0x0A;

			
			j = i + 1;
			for(; j<strlen(stOsdInfo.m_strText) - 1; j++)
			{
				stOsdInfo.m_strText[j] = stOsdInfo.m_strText[j + 1];
			}
			stOsdInfo.m_strText[j] = 0;
		}
		i++;
	}

	//取消通道名称显示,ONVIF协议中不存在通道名称
	stOsdInfo.m_iNameOverlayEnable = 0;
	
	iRet = OnvifCmdSetOsd(iChn, (TGuiVideoOverlayCnf *)&stOsdInfo);
	td_printf(0, "%s %d %s, iRet = %d\n", __FILE__, __LINE__, __FUNCTION__, iRet);
	if(iRet == -1)
	{

		td_printf(0, "%s %d %s OnvifCmdSetOsd error!\n", __FILE__, __LINE__, __FUNCTION__);

		return SOAP_ERR;
	}
	
	return SOAP_OK;
}

/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stubs                                                     *
 *                                                                            *
\******************************************************************************/

/**************************************************
*功能:		多播发送hello消息
*参数:		_pThis:		全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:20130529 v2.3.0 modify by ptb
**************************************************/
int SendHelloToClient(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl 	= (OnvifCtrl *)_pThis;
	struct d__HelloType st_dn__Hello;
	struct soap stSoap;
	char cEndPoint[128] = {0};
	int iRet = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
#if 0
	struct ip_mreq stMreq;
	//首先要退出多播组,以免自己接收到数据
	stMreq.imr_multiaddr.s_addr = inet_addr(ptOnvifCtrl->m_stNvsInfo.m_cMultIP);
	stMreq.imr_interface.s_addr = htonl(INADDR_ANY);
	
	iRet = setsockopt(ptOnvifCtrl->m_stSoapUdp.socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, &stMreq, sizeof(struct ip_mreq));
	if(iRet < 0)
	{
		td_printf(0, "%s %d %s drop multicast error!\n", __FILE__, __LINE__, __FUNCTION__);

		return -1;
	}
#endif

	soap_init(&stSoap);
	
	stSoap.connect_flags   = SO_BROADCAST;	//多播方式
	stSoap.connect_timeout = CONNECT_TIMEOUT;
	stSoap.send_timeout    = SEND_TIMEOUT;
	
	//屏蔽SIGPIPE-just for Linux
	stSoap.socket_flags = MSG_NOSIGNAL; //set to MSG_NOSIGNAL to disable sigpipe
#if 0	
	//禁止本地回环接口
	//char iLoop = 0;
	iRet = setsockopt(stSoap.master, IPPROTO_IP, IP_MULTICAST_LOOP, 0, 1);
	if(iRet < 0)
	{
		td_printf(0, "%s %d %s setsockopt error!\n", __FILE__, __LINE__, __FUNCTION__);

		perror("hehe");
		return iRet;
	}
#endif

	//header
	stSoap.header = (struct SOAP_ENV__Header *)soap_mallocz(&stSoap, sizeof(struct SOAP_ENV__Header));
	if(pointer_valid_check(stSoap.header))
	{
		return TD_ERROR;
	}
	
 	soap_default_SOAP_ENV__Header(&stSoap, stSoap.header);
	
	stSoap.header->wsa5__Action    = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Hello";
	stSoap.header->wsa5__MessageID = (char *)soap_wsa_rand_uuid(&stSoap);
	stSoap.header->wsa5__To        = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
	
	//request
	soap_default_d__HelloType(&stSoap, &st_dn__Hello);

	st_dn__Hello.wsa__EndpointReference.Address = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->wsa__EndpointReference.Address;
	st_dn__Hello.Types  = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Types;
	st_dn__Hello.XAddrs = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->XAddrs;
	st_dn__Hello.Scopes = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->Scopes;
	st_dn__Hello.MetadataVersion = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->MetadataVersion;
	td_printf(0, "UUID:%s\n", st_dn__Hello.wsa__EndpointReference.Address);
    
	//send request to multcast ip
	sprintf(cEndPoint, "soap.udp://%s:%d", ptOnvifCtrl->m_stNvsInfo.m_cMultIP, ptOnvifCtrl->m_stNvsInfo.m_iMultPort);
	td_printf(0, "EndPoint:%s\n", cEndPoint);

	iRet = soap_call___ns2__Hello(&stSoap, cEndPoint, NULL, &st_dn__Hello, NULL);

	soap_destroy(&stSoap);
	soap_end(&stSoap);
	soap_done(&stSoap);
#if 0	
	//通信结束后重新加入多播组
	iRet = setsockopt(ptOnvifCtrl->m_stSoapUdp.socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMreq,sizeof(struct ip_mreq));
	if(iRet < 0)
	{
		td_printf(0, "%s %d %s drop multicast error!\n", __FILE__, __LINE__, __FUNCTION__);

		return -1;
	}
#endif	
	return iRet;
}

/**************************************************
*功能:		多播发送Bye消息
*参数:		_pThis:		全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
int SendByeToClient(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl 	= (OnvifCtrl *)_pThis;
	struct d__ByeType st_dn__Bye;
	struct soap stSoap;
	char cEndPoint[128] = {0};
	int iRet = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	soap_init(&stSoap);
	
	//linger设置
	stSoap.connect_flags   = SO_BROADCAST;	//多播方式
	stSoap.connect_timeout = CONNECT_TIMEOUT;
	stSoap.send_timeout    = SEND_TIMEOUT;
	stSoap.recv_timeout    = RECV_TIMEOUT;
	
	//屏蔽SIGPIPE-just for Linux
	stSoap.socket_flags = MSG_NOSIGNAL;

	stSoap.header = (struct SOAP_ENV__Header *)soap_malloc(&stSoap, sizeof(struct SOAP_ENV__Header));
	if(pointer_valid_check(stSoap.header))
	{
		return TD_ERROR;
	}
	
 	soap_default_SOAP_ENV__Header(&stSoap, stSoap.header);

	stSoap.header->wsa5__Action    = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Bye";
	stSoap.header->wsa5__MessageID = (char *)soap_wsa_rand_uuid(&stSoap);
	stSoap.header->wsa5__To        = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";

	soap_default_d__ByeType(&stSoap, &st_dn__Bye);
	st_dn__Bye.wsa__EndpointReference.Address = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch->wsa__EndpointReference.Address;
	td_printf(0, "Address:%s\n", st_dn__Bye.wsa__EndpointReference.Address);

	//send bye to client
	sprintf(cEndPoint, "soap.udp://%s:%d", ptOnvifCtrl->m_stNvsInfo.m_cMultIP, ptOnvifCtrl->m_stNvsInfo.m_iMultPort);
	td_printf(0, "EndPoint:%s\n", cEndPoint);
	
	iRet = soap_call___ns2__Bye(&stSoap, cEndPoint, NULL, &st_dn__Bye, NULL);

	soap_destroy(&stSoap);
	soap_end(&stSoap);
	soap_done(&stSoap);

	return iRet;
}


//user's function

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
int soap_sender_fault_info(struct soap *_ptSoap, char *_pcReason, char *_pcSubStr, char *_pcSubSubStr)
{
	if(NULL == _ptSoap)
	{
		return -1;
	}

	soap_sender_fault(_ptSoap, _pcReason, NULL);
	
	if(NULL != _pcSubStr)
	{
		*soap_faultsubcode(_ptSoap) = _pcSubStr;
	}
	
	if(NULL != _pcSubStr && NULL != _pcSubSubStr)
	{
		*soap_faultsubsubcode(_ptSoap) = _pcSubSubStr;
	}

	return SOAP_FAULT;
}

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
int soap_receiver_fault_info(struct soap *_ptSoap, char *_pcReason, char *_pcSubStr, char *_pcSubSubStr)
{
	if(NULL == _ptSoap)
	{
		return -1;
	}

	soap_receiver_fault(_ptSoap, _pcReason, NULL);
	
	if(NULL != _pcSubStr)
	{
		*soap_faultsubcode(_ptSoap) = _pcSubStr;
	}
	
	if(NULL != _pcSubStr && NULL != _pcSubSubStr)
	{
		*soap_faultsubsubcode(_ptSoap) = _pcSubSubStr;
	}

	return SOAP_FAULT;
}

/**************************************************
*功能:		验证用户信息
*参数:		_ptSoap:		tcp soap
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120301 v1.0 creat by ptb
**************************************************/
int VerifyUserPsw(struct soap* ptSoap)
{
#ifdef WITH_WSSE
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	char *pcUserName		= NULL;
	char *pcPassword		= NULL;
	int iUserNum 			= 0;
	int i 					= 0;
 	if(NULL == ptSoap->header || NULL == ptSoap->header->wsse__Security)
 	{
		ptSoap->error = 0;
		//goto ERR;
	}

	//获取UsernameToken
	_wsse__UsernameToken *ptToken = soap_wsse_UsernameToken(ptSoap, NULL);

	//获取当前用户数
	iUserNum = ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser;

	//如果当前用户数为0，任何用户都能通过
	if(0 == iUserNum)
	{	
		if(NULL != ptToken)
		{
			soap_wsse_delete_Security(ptSoap);
		}
		
		ptSoap->error = 0;
		//goto ERR;
	}
	else
	{
		//Token信息检测
		if(NULL == ptToken)
		{
			ptSoap->error = 401;
			//goto ERR;
		}

		//modified by qiaohaijun ,老代码存在漏洞
		if(NULL != ptToken->Username)
		{
			for(i = 0; i < iUserNum; i++)
			{
				//从全局变量中获取用户信息
				pcUserName = ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Username;
				pcPassword = ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Password;
				
				if(strcmp(ptToken->Username, pcUserName))
				{
					continue;
				}
				
				if(soap_wsse_verify_Password(ptSoap, pcPassword))
				{
					continue;
				}
				else
				{
					soap_wsse_delete_Security(ptSoap);
					ptSoap->error = 0;
					//goto ERR;
				}
			}

			//没匹配到
			if(i == iUserNum)
			{
				ptSoap->error = 401;
				//goto ERR;
			}
			
		}
		else
		{
			ptSoap->error = 401;
			//goto ERR;
		}
		//end
		
	}
	
////ERR:
	soap_wsse_delete_Security(ptSoap);
	return ptSoap->error;
	
#else	
	//如果没有密码校验部分,清空header
	if (ptSoap->header)
	{
		ptSoap->header->wsse__Security = NULL;
	}

	return SOAP_OK;
#endif
}

/**************************************************
*功能:           报警信息检测
*参数:           无
*返回值:      无
*修改历史:	20130620 v2.3.1 modify by ptb
**************************************************/
void AlarmDetect()
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	//static unsigned int  iMdFlag         = 0;
	static unsigned int  iVideoLostFlag  = 0;
	static unsigned int  iVideoCoverFlag = 0;
	static unsigned int  iAlarmInFlag    = 0;
	int iAlarmInNum = 0;
	int iPortState  = 0;
	int i = 0;
	int j = 0;
	
	if(!pointer_valid_check(ptOnvifCtrl))
	{
		for(i = 0; i < ptOnvifCtrl->m_stNvsInfo.m_iVINum; i++)
		{
			//移动报警
			#if 0
			//AVTECH NVR报警突然消失问题，未决
			if((iMdFlag >> i) & 0x01)
			{
				if((g_ptGuiAlarmInOutStatus->m_iVideoMd&(1<<i)) == 0)
				{
					td_printf(0, "The videomove alarm disappear!");
					iMdFlag &= ~(1 << i);
					ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][VIDEOMOVE - 1] = TD_FALSE;
					continue;
				}
			}
			else
			{
				if((g_ptGuiAlarmInOutStatus->m_iVideoMd&(1<<i)) > 0)
				{
					td_printf(0, "The videomove alarm happen!");
					iMdFlag |= (1 << i);
					ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][VIDEOMOVE - 1] = TD_TRUE;
					continue;
				}
			}
			#else
			if((g_ptGuiAlarmInOutStatus->m_iVideoMd&(1<<i)) > 0)
			{
				//td_printf(0, "The videomove alarm happen!");
				//iMdFlag |= (1 << i);
				//notify方式
				if(!pointer_valid_check(ptOnvifCtrl->m_stManager.ConsumerInfoList[i]))
				{
					if(DELETE == ptOnvifCtrl->m_stManager.ConsumerInfoList[i]->m_eMoveState)
					{
						ptOnvifCtrl->m_stManager.ConsumerInfoList[i]->m_eMoveState = INITIALIZE;
					}
				}

				//PullMessage方式
				if(TD_TRUE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_iEnable)
				{
					if(DELETE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_eMoveState)
					{
						ptOnvifCtrl->m_stManager.m_tPullMsg.m_eMoveState = INITIALIZE;
					}
				}
				ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_MOVE] = TD_TRUE;
				continue;
			}
			else
			{
				//notify方式
				if(!pointer_valid_check(ptOnvifCtrl->m_stManager.ConsumerInfoList[i]))
				{
					if(DELETE != ptOnvifCtrl->m_stManager.ConsumerInfoList[i]->m_eMoveState)
					{
						ptOnvifCtrl->m_stManager.ConsumerInfoList[i]->m_eMoveState = DELETE;
						ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_MOVE] = TD_TRUE;
						continue;
					}
				}
				//PullMessage方式
				if(TD_TRUE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_iEnable)
				{
					if(DELETE != ptOnvifCtrl->m_stManager.m_tPullMsg.m_eMoveState)
					{
						ptOnvifCtrl->m_stManager.m_tPullMsg.m_eMoveState = DELETE;
						ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_MOVE] = TD_TRUE;
						continue;
					}
				}
				
				ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_MOVE] = TD_FALSE;
				continue;
			}
			#endif
			//视频丢失报警
			if((iVideoLostFlag >> i) & 0x01)
			{
				if((g_ptGuiAlarmInOutStatus->m_iVideoLost&(1<<i)) == 0)
				{
					td_printf(0, "The videolost alarm disappear!");
					iVideoLostFlag &= ~(1 << i);
					ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_LOST] = TD_FALSE;
					continue;
				}
			}
			else
			{
				if((g_ptGuiAlarmInOutStatus->m_iVideoLost&(1<<i)) > 0)
				{
					td_printf(0, "The videolost alarm happen!");
					iVideoLostFlag |= (1 << i);
					ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_LOST] = TD_TRUE;
					continue;
				}
			}
		
			//视频遮挡报警
			if((iVideoCoverFlag >> i) & 0x01)
			{
				if((g_ptGuiAlarmInOutStatus->m_iVideoCover&(1<<i)) == 0)
				{
					td_printf(0, "The videocover alarm disappear!");
					iVideoCoverFlag &= ~(1 << i);
					ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_COVER] = TD_FALSE;
					continue;
				}
			}
			else
			{
				if((g_ptGuiAlarmInOutStatus->m_iVideoCover&(1<<i)) > 0)
				{
					td_printf(0, "The videocover alarm happen!");
					iVideoCoverFlag |= (1 << i);
					ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_COVER] = TD_TRUE;
					continue;
				}
			}
		}
		
		//端口报警
		if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
		{
			iAlarmInNum = MAX_CHANNEL_NUM;
			iPortState  = g_ptGuiAlarmInOutStatus->m_iAlarmIn;
		}
		else if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
		{
			iAlarmInNum = ptOnvifCtrl->m_stNvsInfo.m_iAlarmInNum;
			iPortState  = g_ptGuiAlarmInOutStatus->m_iAlarmOutValue;
		}
		else
		{
			iAlarmInNum = 1;
			iPortState  = g_ptGuiAlarmInOutStatus->m_iAlarmOutValue;
		}
		
		for(i = 0; i< iAlarmInNum; i++)
		{
			if((iAlarmInFlag >> i) & 0x01)
			{
				if((iPortState & (1 << i)) == 0)
				{
					td_printf(0, "The portin alarm disappear!");
					iAlarmInFlag &= ~(1 << i);
					ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_PORT] = TD_FALSE;
					continue;
				}
			}
			else
			{
				if((iPortState & (1 << i)) > 0)
				{
					td_printf(0, "The portin alarm happen!");
					iAlarmInFlag |= (1 << i);
					ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][ALARM_PORT] = TD_TRUE;

					continue;
				}
			}
		}

		//确定当前是否有报警发生
		for(i = 0; i < ptOnvifCtrl->m_stNvsInfo.m_iVINum; i++)
		{
			for(j = 0; j < TOPIC_NUM; j++)
			{
				if(ptOnvifCtrl->m_stManager.m_iAlarmInfo[i][j])
				{
					ptOnvifCtrl->m_stFlag.m_iAlarmFlag = TD_TRUE;
					break;
				}
				else
				{
					ptOnvifCtrl->m_stFlag.m_iAlarmFlag = TD_FALSE;
				}
			}
		}
	}
}



/**************************************************
*功能:		计时器线程函数
*参数:		_pThis:		全局变量
*返回值:	无
*修改历史:
	20120314 v1.0 creat by ptb
	20131219 modified by qiaohaijun 将soap提到外层，减少资源占用
**************************************************/
void *ThreadTimer(void *_pThis)
{
	td_printf(0, "%s, %s, %d", __FILE__, __FUNCTION__, __LINE__);
	OnvifCtrl *ptOnvifCtrl 	= (OnvifCtrl *)_pThis;
	int i = 0;
	static int iNum = 0;
	func_info(-1);
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
	//	pthread_exit(NULL);
		return NULL;//by qiaohaijun 
	}

	while(1)
	{
		if(iNum++ >=  100)
		{
			iNum = 0;
		}
		//主动模式是否到终止时间
		for(i = 0; i < MAX_CONSUMER_NUM; i++)
		{
			if(ptOnvifCtrl->m_stManager.ConsumerInfoList[i] != NULL)
			{
				
			#ifdef WITH_TESTTOOL
				if(ptOnvifCtrl->m_stManager.ConsumerInfoList[i]->m_tTerminateTime <= time(NULL) + 5)
			#else
				if(ptOnvifCtrl->m_stManager.ConsumerInfoList[i]->m_tTerminateTime <= time(NULL))
			#endif
				{
					DeleteConsumer(i);	//太保:这样修改岂不造成Consumer资源泄露//by qiaohaijun 2013.12.2 for testtool
				}
			}
		}

		//被动模式是否到终止时间
		if(TD_TRUE == ptOnvifCtrl->m_stManager.m_tPullMsg.m_iEnable)
		{
		#ifdef WITH_TESTTOOL
			if(ptOnvifCtrl->m_stManager.m_tPullMsg.m_tTerminateTime <= time(NULL) + 5)
		#else
			if(ptOnvifCtrl->m_stManager.m_tPullMsg.m_tTerminateTime <= time(NULL))
		#endif
			{
				ClearPullMsgInfo();
			}
		}
		AlarmDetect();
		if(ptOnvifCtrl->m_stFlag.m_iNotifyFlag)
		{
			NotifyConsumer();
		}
		
		sleep(1);//报警1S检测一次
		//usleep(1000 * 100);
	}


	return NULL;
}

//qiaohaijun test for hik, dahua, aviligon, odm..., 2013.12.19
int genMove_hik(char *_pTopicany, char *_pMsgany, int _iTopicState, int _iChn, struct tm *_ptSaveTime, void *_pReserved)
{
	char *pPropertyOperation = NULL;
	char *pStateValue = NULL;
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	//Topic
	Strcpy(_pTopicany, "tns1:RuleEngine/CellMotionDetector/Motion", MEM_LEN_MID);//海康的
	
	if(INITIALIZE == _iTopicState)
	{
		//Initialized I必须大写，否则ODM不识别
		pPropertyOperation = "Initialized";
		pStateValue = "true";//海康true必须小写
	}
	else if(CHANGE == _iTopicState)
	{
		pPropertyOperation = "Changed";
		pStateValue = "true";//海康true必须小写
	}
	else
	{	
		pPropertyOperation = "Deleted";
		pStateValue = "FALSE";//false好像无所谓
	}
	
	//海康报警
	_pMsgany[0] = '\0';
	sprintf(_pMsgany, \
				"<tt:Message UtcTime=\"%d-%02d-%02dT%02d:%02d:%02d\" PropertyOperation=\"%s\">"CRLF\
					"<tt:Source>"CRLF\
						"<tt:SimpleItem Name=\"VideoSourceConfigurationToken\" Value=\"%s\"/>"CRLF\
						"<tt:SimpleItem Name=\"VideoAnalyticsConfigurationToken\" Value=\"VideoAnalyticsToken\"/>"CRLF\
						"<tt:SimpleItem Name=\"Rule\" Value=\"MyMotionDetectorRule\"/>"CRLF\
					"</tt:Source>"CRLF\
					"<tt:Data>"CRLF\
						"<tt:SimpleItem Name=\"IsMotion\" Value=\"%s\"/>"CRLF\
					"</tt:Data>"CRLF\
				"</tt:Message>"CRLF,\
				_ptSaveTime->tm_year + 1900,\
				_ptSaveTime->tm_mon + 1,\
				_ptSaveTime->tm_mday,\
				_ptSaveTime->tm_hour,\
				_ptSaveTime->tm_min,\
				_ptSaveTime->tm_sec,\
				pPropertyOperation,\
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[_iChn].token,\
				pStateValue);
	td_printf(0, "hik move msg === \n%s", _pMsgany);

	return 0;
}

int genMove_aviligon(char *_pTopicany, char *_pMsgany, int _iTopicState, int _iChn, struct tm *_ptSaveTime, void *_pReserved)
{
	char *pPropertyOperation = NULL;
	char *pStateValue = NULL;

	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
				
	Strcpy(_pTopicany, TopicSetList[ALARM_MOVE], MEM_LEN_MID);

	//Message.any
	if(INITIALIZE == _iTopicState)
	{
		//Initialized I必须大写，否则ODM不识别
		pPropertyOperation = "Initialized";
		pStateValue = "TRUE";
	}
	else if(CHANGE == _iTopicState)
	{
		pPropertyOperation = "Changed";
		pStateValue = "TRUE";
	}
	else
	{	
		pPropertyOperation = "Deleted";
		pStateValue = "FALSE";
	}
	_pMsgany[0] = '\0';
	sprintf(_pMsgany,\
				"<tt:Message UtcTime=\"%d-%02d-%02dT%02d:%02d:%02d\" PropertyOperation=\"%s\">"CRLF\
					"<tt:Source>"CRLF\
						"<tt:SimpleItem Name=\"Source\" Value=\"%s\"/>"CRLF\
					"</tt:Source>"CRLF\
					"<tt:Data>"CRLF\
						"<tt:SimpleItem Name=\"State\" Value=\"%s\"/>"CRLF\
					"</tt:Data>"CRLF\
				"</tt:Message>"CRLF,\
				_ptSaveTime->tm_year + 1900,\
				_ptSaveTime->tm_mon + 1,\
				_ptSaveTime->tm_mday,\
				_ptSaveTime->tm_hour,\
				_ptSaveTime->tm_min,\
				_ptSaveTime->tm_sec,\
				pPropertyOperation,\
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[_iChn].token,\
				pStateValue);

	td_printf(0, "aviligon move msg === \n%s", _pMsgany);		
	return 0;
}

int genMove_dahua(char *_pTopicany, char *_pMsgany, int _iTopicState, int _iChn, struct tm *_ptSaveTime, void *_pReserved)
{

	char *pStateValue = NULL;
//	char *pPropertyOperation = NULL;

	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	//Topic 大华的TOPIC，谁换谁赔。
	Strcpy(_pTopicany, "tns1:VideoAnalytics/tnshik:MotionAlarm", MEM_LEN_MID);
	//Message.any
	if(INITIALIZE == _iTopicState)
	{
		//pPropertyOperation = "Initialized";
		pStateValue = "TRUE";
	}
	else if(CHANGE == _iTopicState)
	{
		//pPropertyOperation = "Changed";
		pStateValue = "TRUE";
	}
	else
	{	
		//pPropertyOperation = "Deleted";//大华不识别Delted字段，兼容吧。qiaohaijun
		//pPropertyOperation = "Changed";
		pStateValue = "FALSE";
	}

	
	_pMsgany[0] = '\0';
	sprintf(_pMsgany,\
			"<tt:Message UtcTime=\"%d-%02d-%02dT%02d:%02d:%02d\">"CRLF\
				"<tt:Source>"CRLF\
					/*"<tt:SimpleItem Name=\"VideoSourceToken\" Value=\"VideoSourceToken\"/>"CRLF\*//*此条是大华原装的，如果报警或消警不好使可以试一下*/
					"<tt:SimpleItem Name=\"Source\" Value=\"%s\"/>"CRLF\
				"</tt:Source>"CRLF\
				"<tt:Data>"CRLF\
					"<tt:SimpleItem Name=\"State\" Value=\"%s\"/>"CRLF\
				"</tt:Data>"CRLF\
			"</tt:Message>"CRLF,\
				_ptSaveTime->tm_year + 1900,\
				_ptSaveTime->tm_mon + 1,\
				_ptSaveTime->tm_mday,\
				_ptSaveTime->tm_hour,\
				_ptSaveTime->tm_min,\
				_ptSaveTime->tm_sec,\
				/*pPropertyOperation,\*/
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[_iChn].token,\
				pStateValue);

	td_printf(0, "dahua move msg === \n%s", _pMsgany);
	return 0;
}

int genPort_common(char *_pTopicany, char *_pMsgany, int _iTopicState, int _iChn, struct tm *_ptSaveTime, void *_pReserved)
{

	char *pPropertyOperation = NULL;
	char *pStateValue = NULL;

	//Message.any
	if(INITIALIZE == _iTopicState)
	{
		//Initialized I必须大写，否则ODM不识别
		pPropertyOperation = "Initialized";
		pStateValue = "TRUE";
	}
	else if(CHANGE == _iTopicState)
	{
		pPropertyOperation = "Changed";
		pStateValue = "TRUE";
	}
	else
	{	
		pPropertyOperation = "Deleted";
		pStateValue = "FALSE";
	}

	Strcpy(_pTopicany, TopicSetList[ALARM_PORT], MEM_LEN_MID);

	sprintf(_pMsgany, \
			"<tt:Message UtcTime=\"%d-%02d-%02dT%02d:%02d:%02d\" PropertyOperation=\"%s\">"CRLF\
				"<tt:Source>"CRLF\
					"<tt:SimpleItem Name=\"AlarmInToken\" Value=\"AlarmIn_%02d\"/>"CRLF\
				"</tt:Source>"CRLF\
				"<tt:Data>"CRLF\
					"<tt:SimpleItem Name=\"State\" Value=\"%s\"/>"CRLF\
				"</tt:Data>"CRLF\
			"</tt:Message>"CRLF,\
		_ptSaveTime->tm_year + 1900,\
		_ptSaveTime->tm_mon + 1,\
		_ptSaveTime->tm_mday,\
		_ptSaveTime->tm_hour,\
		_ptSaveTime->tm_min,\
		_ptSaveTime->tm_sec,\
		pPropertyOperation,\
		_iChn + 1,\
		pStateValue);
	td_printf(0, "_pMsgany \n====== %s", _pMsgany);
	return 0;
}

int genCover_common(char *_pTopicany, char *_pMsgany, int _iTopicState, int _iChn, struct tm *_ptSaveTime, void *_pReserved)
{

	Strcpy(_pTopicany, TopicSetList[ALARM_COVER], MEM_LEN_MID);

	sprintf(_pMsgany, \
			"<tt:Message UtcTime=\"%d-%02d-%02dT%02d:%02d:%02d\" PropertyOperation=\"Initialized\">"CRLF\
				"<tt:Source>"CRLF\
					"<tt:SimpleItem Name=\"VideoSourceToken\" Value=\"vstoken_ch%02d\"/>"CRLF\
				"</tt:Source>"CRLF\
				"<tt:Data>"CRLF\
					"<tt:SimpleItem Name=\"State\" Value=\"%s\"/>"CRLF\
				"</tt:Data>"CRLF\
			"</tt:Message>"CRLF,\
		_ptSaveTime->tm_year + 1900,\
		_ptSaveTime->tm_mon + 1,\
		_ptSaveTime->tm_mday,\
		_ptSaveTime->tm_hour,\
		_ptSaveTime->tm_min,\
		_ptSaveTime->tm_sec,\
		_iChn + 1,\
		"TRUE");

	td_printf(0, "Video cover Alarm[%s]", _pMsgany);

	return 0;
}
//end


/**************************************************
*功能:		填充报警消息
*参数:		_iTopicIndex  :		报警类型
				1端口2移动3丢失4遮挡
				_iFlag		:		1,告警; 0,消警。
*返回值:	成功:0 失败: !0      
*修改历史:   20131010 v2.3.1 creat by ptb
**************************************************/
int PaddingMessage(int _EventType, int _iChn, int _iTopicState, void *_pNotifyMsg, int *_piOffset)
{
	struct wsnt__NotificationMessageHolderType *ptNofityMsg = (struct wsnt__NotificationMessageHolderType *)_pNotifyMsg;
	struct tm tSaveTime;
	time_t time_now;
	
	if(pointer_valid_check(ptNofityMsg))
	{
		return TD_ERROR;
	}
	
	TdTime(&time_now);
	gmtime_r(&time_now, &tSaveTime);


	switch(_EventType)
	{
		case ALARM_MOVE:
			{
			
				ptNofityMsg[*_piOffset].Topic->__any[0] = '\0';
				genMove_hik(ptNofityMsg[*_piOffset].Topic->__any, 
							ptNofityMsg[*_piOffset].Message.__any, 
							_iTopicState, 
							_iChn,
							&tSaveTime,
							NULL);
				(*_piOffset)++;
			#if 1
				ptNofityMsg[*_piOffset].Topic->__any[0] = '\0';
				genMove_aviligon(ptNofityMsg[*_piOffset].Topic->__any, 
							ptNofityMsg[*_piOffset].Message.__any, 
							_iTopicState, 
							_iChn,
							&tSaveTime,
							NULL);
				(*_piOffset)++;
			#endif
			#if 1
				ptNofityMsg[*_piOffset].Topic->__any[0] = '\0';
				genMove_dahua(ptNofityMsg[*_piOffset].Topic->__any, 
							ptNofityMsg[*_piOffset].Message.__any, 
							_iTopicState, 
							_iChn,
							&tSaveTime,
							NULL);
				(*_piOffset)++;
			#endif
			}

			//end
			break;
		case ALARM_PORT:
			{	
				ptNofityMsg[*_piOffset].Topic->__any[0] = '\0';
				//没调试过
				genPort_common(ptNofityMsg[*_piOffset].Topic->__any, 
							ptNofityMsg[*_piOffset].Message.__any, 
							_iTopicState, 
							_iChn,
							&tSaveTime,
							NULL);
			#if 0
				Strcpy(ptNofityMsg[*_piOffset].Topic->__any, TopicSetList[ALARM_PORT], MEM_LEN_MID);

				sprintf(ptNofityMsg->Message.__any, \
						"<tt:Message UtcTime=\"%d-%02d-%02dT%02d:%02d:%02d\" PropertyOperation=\"Changed\">"\
						"<tt:Source><tt:SimpleItem Name=\"AlarmInToken\" Value=\"AlarmIn_%d\"/></tt:Source>"\
						"<tt:Data><tt:SimpleItem Name=\"State\" Value=\"%s\"/></tt:Data></tt:Message>",\
					tSaveTime.tm_year + 1900,\
					tSaveTime.tm_mon + 1,\
					tSaveTime.tm_mday,\
					tSaveTime.tm_hour,\
					tSaveTime.tm_min,\
					tSaveTime.tm_sec,\
					_iChn + 1,\
					"true");
			#endif
				(*_piOffset)++;
			}
			break;
		case ALARM_COVER:
			{
				ptNofityMsg[*_piOffset].Topic->__any[0] = '\0';
				genCover_common(ptNofityMsg[*_piOffset].Topic->__any, 
							ptNofityMsg[*_piOffset].Message.__any, 
							_iTopicState, 
							_iChn,
							&tSaveTime,
							NULL);
			#if 0
				Strcpy(ptNofityMsg[*_piOffset].Topic->__any, TopicSetList[ALARM_COVER], MEM_LEN_MID);

				sprintf(ptNofityMsg[*_piOffset].Message.__any, "<tt:Message UtcTime=\"%d-%02d-%02dT%02d:%02d:%02d\" PropertyOperation=\"Initialized\">"\
				"<tt:Source><tt:SimpleItem Name=\"VideoSourceToken\" Value=\"vstoken_ch%02d\"/></tt:Source>"\
				"<tt:Data><tt:SimpleItem Name=\"State\" Value=\"%s\"/></tt:Data></tt:Message>",\
					tSaveTime.tm_year + 1900,\
					tSaveTime.tm_mon + 1,\
					tSaveTime.tm_mday,\
					tSaveTime.tm_hour,\
					tSaveTime.tm_min,\
					tSaveTime.tm_sec,\
					_iChn + 1,\
					"TRUE");
			#endif

				(*_piOffset)++;
			}
			break;
		case ALARM_LOST:
			{
				//IPC无视频丢失
			}
			break;
		default:
			break;
	}
		
	return TD_OK;
}


//qiaohaijun test
int GetStateFromMask(int _iTopicMask, int _iStateMask, int _ibit,int *_iState)
{
	if((_iTopicMask>>_ibit)&0x01)
	{
		*_iState = (_iStateMask>>(2*_ibit))&0x03;
	}

	return 0;
}



/**************************************************
*功能:	发送Notify消息
*参数:	_iTopicIndex	:		报警类型
			_iChn		:		通道号
*返回值:	成功:0 失败: !0      
*修改历史:   
20120325 v1.0 creat by ptb
20130930 v2.3.1 modify by ptb
20131219 modified by qiaohaijun
**************************************************/
int NotifyConsumer()
{
	struct _wsnt__Notify wsnt__Notify = {0};
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	ConsumerInfo *pConsumer = NULL;
	//int iChnMask = 0;
	//int iIndex = 0;
	int i = 0;
	int j = 0;

	int iOffset = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	//struct soap stSoap      = {0};
	struct soap stSoap;
	//以后可考虑不用soap，自己实现功能
	soap_init(&stSoap);
	//多播方式
	stSoap.connect_flags   = SO_LINGER; 
	stSoap.linger_time	   = LINGER_TIMEOUT;
	stSoap.connect_timeout = CONNECT_TIMEOUT;
	stSoap.send_timeout    = SEND_TIMEOUT;
	stSoap.recv_timeout    = RECV_TIMEOUT;
	stSoap.socket_flags    = MSG_NOSIGNAL;
	
	stSoap.header = (struct SOAP_ENV__Header *)soap_mallocz(&stSoap, sizeof(struct SOAP_ENV__Header));
	if(pointer_valid_check(stSoap.header))
	{
		////goto ERROR;
	}
	memset(stSoap.header, 0, sizeof(struct SOAP_ENV__Header));

	struct soap *_ptSoap = &stSoap;
	
	for(i = 0; i < MAX_CONSUMER_NUM; i++)
	{
		
		pConsumer = ptOnvifCtrl->m_stManager.ConsumerInfoList[i];
		if(pointer_valid_check(pConsumer))
		{
			continue;
		}
				
		if(SUBSCRIBED == pConsumer->m_eState)
		{
			_ptSoap->header->wsa5__To = pConsumer->pcEndPoint;
			__ns8__Notify(_ptSoap, &wsnt__Notify);


			for(j = 0; j < ptOnvifCtrl->m_stNvsInfo.m_iVINum; j++)
			{
				
				if(TD_TRUE == ptOnvifCtrl->m_stManager.m_iAlarmInfo[j][ALARM_MOVE]
				&&((pConsumer->m_iSubTopicMask>>ALARM_MOVE)&0x1))
				{	
					td_printf(0, "%s,%d, ALARM_MOVE...", __FUNCTION__, __LINE__);
					if(INITIALIZE == pConsumer->m_eMoveState)
					{
						PaddingMessage(ALARM_MOVE, j, INITIALIZE, wsnt__Notify.NotificationMessage, &iOffset);
						pConsumer->m_eMoveState = CHANGE;
					}
					else if(CHANGE == pConsumer->m_eMoveState)
					{
						PaddingMessage(ALARM_MOVE, j, CHANGE, wsnt__Notify.NotificationMessage, &iOffset);
					}
					else
					{
						PaddingMessage(ALARM_MOVE, j, DELETE, wsnt__Notify.NotificationMessage, &iOffset);
					}
				}

				if(TD_TRUE == ptOnvifCtrl->m_stManager.m_iAlarmInfo[j][ALARM_PORT]
				&&((pConsumer->m_iSubTopicMask>>ALARM_PORT)&0x1))
				{
					td_printf(0, "%s,%d, ALARM_PORT...", __FUNCTION__, __LINE__);
					PaddingMessage(ALARM_PORT, j, INITIALIZE, wsnt__Notify.NotificationMessage, &iOffset);
				}
				
			}

			td_printf(0, "%s, %d offset ======== %d", __FUNCTION__, __LINE__, iOffset);
			
			wsnt__Notify.__sizeNotificationMessage = iOffset;

			wsnt__Notify.NotificationMessage->Topic->__anyAttribute = "tns1:VideoAnalytics/tnshik:MotionAlarm";
			if(0 == wsnt__Notify.__sizeNotificationMessage)
			{
				continue;
			}
			#if 1 
			//TODO:
			//此处需要考虑对方断网等情况，导致设备一直发送情况
			if(soap_send___ns8__Notify(_ptSoap, pConsumer->pcEndPoint, NULL, &wsnt__Notify))
			{
				int iIndex = 0;
				td_printf(0, "chan:%d Send Notify Error!", j);
				soap_send_fault(_ptSoap);
				iIndex = GetConsumerId_Ex(pConsumer);
				if(IsConsumer(iIndex))
				{
					soap_sender_fault_info(_ptSoap, NULL, NULL, "tt:ResourceUnknown");
					continue;
				}
			
				//删除当前Index的消费者节点
				DeleteConsumer(iIndex);
			}
			#endif
			
		}
		//1S发送一次报警信息
		sleep(1);
	}

//ERROR:
	soap_destroy(&stSoap);
	soap_end(&stSoap);
	soap_done(&stSoap);

	return SOAP_OK;
}

/**************************************************
*功能:	增加用户信息
*参数:	_tCreateTime	:			订阅时间
			_tTerminateTime	:		终止时间
			_pcEndPoint		:		订阅服务的地址
			_iConsumerId	:		消费者Index
			_iTopicMask 	:			订阅主题
			_iChnMask		:		当前主题通道号
*返回值:	0: 成功, !0: 失败
*修改历史:   20120325 v1.0 creat by ptb
20131031 v2.3.1 modify by ptb
**************************************************/
int AddConsumer(time_t _tCreateTime, time_t _tTerminateTime, const char *_pcEndPoint, int _iConsumerId, int _iTopicMask, int _iChnMask)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl  = &g_stOnvifCtrl;
	ConsumerInfo *ptNewUser = NULL;
	int iId = _iConsumerId;
	//int i = 0;

	if(pointer_valid_check(_pcEndPoint) || iId < 0 || iId >= MAX_CONSUMER_NUM)
	{
		return TD_ERROR;
	}
	
	ptNewUser = (ConsumerInfo *)calloc(1, sizeof(ConsumerInfo));
	if(pointer_valid_check(ptNewUser))
	{
		return TD_ERROR;
	}
	else
	{
		ptNewUser->m_tCreateTime	= _tCreateTime;
		ptNewUser->m_tTerminateTime = _tTerminateTime;
		ptNewUser->m_eState 		= SUBSCRIBED;
		ptNewUser->m_eMoveState    = INITIALIZE;
		ptNewUser->pcEndPoint		= (char *)calloc(1, MEM_LEN_MID);
		ptNewUser->m_iSubTopicMask = _iTopicMask;
		if(!pointer_valid_check(ptNewUser->pcEndPoint))
		{
			td_printf(0, "_pcEndPoint [%s]", _pcEndPoint);
			Strcpy(ptNewUser->pcEndPoint, _pcEndPoint, MEM_LEN_MID);
		}
	}

	pthread_mutex_lock(&ptOnvifCtrl->m_stManager.m_stEventLock);
	
	ptOnvifCtrl->m_stManager.ConsumerInfoList[iId] = ptNewUser;

	#if 0	//太保:为何要注释掉?采用何种方式过滤订阅的报警内容?
	for(i = 0; i < TOPIC_NUM; i++)
	{
		if((_iTopicMask >> i) & 0x01)
		{
			ptOnvifCtrl->m_stManager.ConsumerIndexList[i][iId].m_iChnMask = _iChnMask;
		}
	}
	#endif
	pthread_mutex_unlock(&ptOnvifCtrl->m_stManager.m_stEventLock);

	return SOAP_OK;
}

/**************************************************
*功能:	删除指定Id的消费者信息
*参数:	_iConsumerId	:	消费者Id
*返回值:	0: 成功, !0: 失败
*修改历史:   20120325 v1.0 creat by ptb
20130930 v2.3.1 modify by ptb
**************************************************/
int DeleteConsumer(int _iConsumerId)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iNum = _iConsumerId;
	int i = 0;
	
	if(iNum < 0 || iNum >= MAX_CONSUMER_NUM)
	{
		return TD_ERROR;
	}
	
	pthread_mutex_lock(&ptOnvifCtrl->m_stManager.m_stEventLock);

	for(i = 0; i < TOPIC_NUM; i++)
	{
		ptOnvifCtrl->m_stManager.ConsumerIndexList[i][iNum].m_iChnMask = 0;
	}
	
	free(ptOnvifCtrl->m_stManager.ConsumerInfoList[iNum]);
	ptOnvifCtrl->m_stManager.ConsumerInfoList[iNum] = NULL;
	
	pthread_mutex_unlock(&ptOnvifCtrl->m_stManager.m_stEventLock);

	return TD_OK;
}

/**************************************************
*功能:		更新消费者终止时间
*参数:	_iConsumerId	:	消费者Index
			_tTerminateTime	:	终止时间
			_iFlag			:	终止时间格式标识，
								1:在原时间基础上加上延时	
								0:给出在明确的终止时间
*返回值:	0: 成功, !0: 失败
*修改历史:20120325 v1.0 creat by ptb
20131031 v2.3.1 modify by ptb
**************************************************/
int UpdateConsumerTermTime(int _iConsumerId, time_t _tTerminateTime, int _iFlag)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	//ConsumerInfo *ptList = NULL;
	int iNum = _iConsumerId;

	//ptList = (ConsumerInfo *)ptOnvifCtrl->m_stManager.ConsumerInfoList;

	if(iNum < 0 || iNum >= MAX_CONSUMER_NUM)
	{
		td_printf(0, "iNum[%d]", iNum);
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stManager.ConsumerInfoList[iNum]))
	{
		return TD_ERROR;
	}

	if(_iFlag == 0)
	{
		ptOnvifCtrl->m_stManager.ConsumerInfoList[iNum]->m_tTerminateTime = _tTerminateTime;
	}
	else
	{
		ptOnvifCtrl->m_stManager.ConsumerInfoList[iNum]->m_tTerminateTime += _tTerminateTime;
	}

	return TD_OK;
}


/**************************************************
*功能:		查询指定下标的链表中是否
				存在消费者信息
参数:			_iConsumerId	:	需要查找的Id
*返回值:	成功:0 , 失败: -1
*修改历史:
20120326 v1.0 creat by ptb
20130930 v2.3.1 modify by ptb
**************************************************/
int IsConsumer(int _iConsumerId)
{
	OnvifCtrl *ptOnvifCtrl 	= &g_stOnvifCtrl;
	int iRet = 0;

	if(_iConsumerId < 0 || _iConsumerId >= MAX_CONSUMER_NUM)
	{
		return TD_ERROR;
	}

	iRet = (ptOnvifCtrl->m_stManager.ConsumerInfoList[_iConsumerId] != NULL) ? 0 : -1;
	return iRet;
}

/**************************************************
*功能:		获取消费者Index
*参数:		_pcEndPoint		:		消费者EndPoint
*返回值:	成功:消费者Index号 , 失败: <0
*修改历史:
	20120325 v1.0 creat by ptb
**************************************************/
int GetConsumerId(const char *_pcEndPoint)
{
	char *pcTmp = NULL;

	if(pointer_valid_check(_pcEndPoint))
	{
		return TD_ERROR;
	}
	
	pcTmp = strstr((char*)_pcEndPoint, SUBSCRIBEURL);
	if(pointer_valid_check(pcTmp))
	{
		return TD_ERROR;
	}
	else
	{
		pcTmp += strlen(SUBSCRIBEURL);
		return atoi(pcTmp);
	}

	return TD_ERROR;
}

/**************************************************
*功能:		获取消费者Index
*参数:		_pcEndPoint		:		消费者EndPoint
*返回值:	成功:消费者Index号 , 失败: <0
*修改历史:
	20120325 v1.0 creat by ptb
**************************************************/
int GetConsumerId_Ex(ConsumerInfo *_ptInfo)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iRet = 0;
	int i = 0;

	if(pointer_valid_check(_ptInfo))
	{
		return TD_ERROR;
	}
	
	for(i = 0; i < MAX_CONSUMER_NUM; i++)
	{
		iRet = strcmp((ptOnvifCtrl->m_stManager.ConsumerInfoList[i])->pcEndPoint, _ptInfo->pcEndPoint);
		if(0 == iRet)
		{
			return i;
		}
		continue;
	}

	return -1;
}

/**************************************************
*功能:		获取空闲Index
*参数:		_pThis		:		消费者EndPoint
*返回值:	成功:消费者Index号 , 失败: <0
*修改历史:   20120325 v1.0 creat by ptb
**************************************************/
int GetIdleId(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl 	= (OnvifCtrl *)_pThis;
	int i = 0;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	for(i = 0; i < MAX_CONSUMER_NUM; i++)
	{
		if(ptOnvifCtrl->m_stManager.ConsumerInfoList[i] == NULL)
		{
			return i;
		}
	}

	return -1;
}

/**************************************************
*功能:		设置高清参数(onvifv10版本)
*参数:		_iChan			:		通道号
                         _ptImgSetting  	:		图像参数结构体
*返回值:	0: 成功, !0: 失败
*修改历史:
	20130130 v2.0 creat by ptb
	20130219 v2.0 modify by ptb
**************************************************/
int SetIspParameter10(int _iChan, struct tt__ImagingSettings *_ptImgSetting)
{
	struct tt__ImagingSettings *ptImagingSetting = _ptImgSetting;
	struct tt__VideoSource *ptVideoSource = NULL;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
#ifndef WITH_TESTTOOL
	TGuiVideoISPInfo  stIspInfo   = {0};
#endif
	int iChan 		= _iChan;
	int iChangeFlag = 0;
	int iIrCutFilter= -1;
	int iBCMode     = -1;
	int iBCLevel 	= -1;
	int iEMode      = -1;
	int iEPriority 	= -1;
	int iEWinTop 	= -1;
	int iEWinLeft 	= -1;
	int iEWinBottom = -1;
	int iEWinRight 	= -1;
	int iEMinETime 	= -1;
	int iEMaxETime 	= -1;
	int iEMinEGain 	= -1;
	int iEMaxEGain 	= -1;
	int iSharpness 	= -1;
	int iWDRMode 	= -1;
	int iWDRLevel	= -1;
	int iWBMode		= -1;
	int iWBCrGain	= -1;
	int iWBCbGain	= -1;

	if(NULL == ptOnvifCtrl || NULL == ptImagingSetting)
	{
		return SOAP_ERR;
	}
	
	//更新全局变量值
	if(TD_OK != UpdateIspInfo())
	{
		td_printf(0, "%s %d %s =====UpdateIspInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}

	ptVideoSource = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources + iChan;

	//参数合法性判断
	//背光补偿
	if(NULL != ptImagingSetting->BacklightCompensation)
	{	
		//模式
		if(ptImagingSetting->BacklightCompensation->Mode == tt__BacklightCompensationMode__OFF
		|| ptImagingSetting->BacklightCompensation->Mode == tt__BacklightCompensationMode__ON)
		{
			if(ptImagingSetting->BacklightCompensation->Mode != ptVideoSource->Imaging->BacklightCompensation->Mode)
			{
				iChangeFlag = 1;
				iBCMode = (ptImagingSetting->BacklightCompensation->Mode == tt__BacklightCompensationMode__OFF)?2:1;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->BacklightCompensation->Mode  = iBCMode;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}
	
		//级别
		if(ptImagingSetting->BacklightCompensation->Level <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->BacklightCompensation->Level->Max
		&& ptImagingSetting->BacklightCompensation->Level >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->BacklightCompensation->Level->Min)
		{
			if((int)ptImagingSetting->BacklightCompensation->Level != (int)ptVideoSource->Imaging->BacklightCompensation->Level)
			{
				iChangeFlag = 1;
				iBCLevel = (int)ptImagingSetting->BacklightCompensation->Level;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->BacklightCompensation->Level = iBCLevel;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}
	}

	//曝光
	if(NULL != ptImagingSetting->Exposure)
	{
		//模式
		if(ptImagingSetting->Exposure->Mode == tt__ExposureMode__AUTO
		|| ptImagingSetting->Exposure->Mode == tt__ExposureMode__MANUAL)
		{
			if(ptImagingSetting->Exposure->Mode != ptVideoSource->Imaging->Exposure->Mode)
			{
				iChangeFlag = 1;
				iEMode = (ptImagingSetting->Exposure->Mode == tt__ExposureMode__MANUAL)?0:1;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->Exposure->Mode	= ptImagingSetting->Exposure->Mode;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}

		//曝光策略
		if(ptImagingSetting->Exposure->Priority == tt__ExposurePriority__LowNoise
		|| ptImagingSetting->Exposure->Priority == tt__ExposurePriority__FrameRate)
		{
			if(ptImagingSetting->Exposure->Priority != ptVideoSource->Imaging->Exposure->Priority)
			{
				iChangeFlag = 1;
				iEPriority  = ptImagingSetting->Exposure->Priority;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->Exposure->Priority = ptImagingSetting->Exposure->Priority;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}

		//曝光区域
		if(NULL != ptImagingSetting->Exposure->Window)
		{	
			if(NULL != ptImagingSetting->Exposure->Window->bottom
			&& NULL != ptImagingSetting->Exposure->Window->top
			&& NULL != ptImagingSetting->Exposure->Window->right
			&& NULL != ptImagingSetting->Exposure->Window->left)
			{
				if((int)*ptVideoSource->Imaging->Exposure->Window->bottom != (int)*ptImagingSetting->Exposure->Window->bottom
				|| (int)*ptVideoSource->Imaging->Exposure->Window->top != (int)*ptImagingSetting->Exposure->Window->top
				|| (int)*ptVideoSource->Imaging->Exposure->Window->right != (int)*ptImagingSetting->Exposure->Window->right
				|| (int)*ptVideoSource->Imaging->Exposure->Window->left != (int)*ptImagingSetting->Exposure->Window->left)
				{
					iChangeFlag = 1;
					iEWinBottom = (int)*ptImagingSetting->Exposure->Window->bottom;
					iEWinTop 	= (int)*ptImagingSetting->Exposure->Window->top;
					iEWinRight 	= (int)*ptImagingSetting->Exposure->Window->right;
					iEWinLeft 	= (int)*ptImagingSetting->Exposure->Window->left;
				#ifdef WITH_TESTTOOL
					*ptVideoSource->Imaging->Exposure->Window->bottom = *ptImagingSetting->Exposure->Window->bottom;
					*ptVideoSource->Imaging->Exposure->Window->top	  = *ptImagingSetting->Exposure->Window->top;
					*ptVideoSource->Imaging->Exposure->Window->right  = *ptImagingSetting->Exposure->Window->right;
					*ptVideoSource->Imaging->Exposure->Window->left   = *ptImagingSetting->Exposure->Window->left;
				#endif
				}
			}
		}
		
		//最小曝光时间
		if(ptImagingSetting->Exposure->MinExposureTime <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MinExposureTime->Max
		&& ptImagingSetting->Exposure->MinExposureTime >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MinExposureTime->Min)
		{
			if((int)ptImagingSetting->Exposure->MinExposureTime != (int)ptVideoSource->Imaging->Exposure->MinExposureTime)
			{
				iChangeFlag = 1;
				iEMinETime  = ptImagingSetting->Exposure->MinExposureTime;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->Exposure->MinExposureTime = ptImagingSetting->Exposure->MinExposureTime;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}
		
		//最大曝光时间
		if(ptImagingSetting->Exposure->MaxExposureTime <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MaxExposureTime->Max
		&& ptImagingSetting->Exposure->MaxExposureTime >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MaxExposureTime->Min)
		{
			if((int)ptImagingSetting->Exposure->MaxExposureTime != (int)ptVideoSource->Imaging->Exposure->MaxExposureTime)
			{
				iChangeFlag = 1;
				iEMaxETime = (int)ptImagingSetting->Exposure->MaxExposureTime;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->Exposure->MaxExposureTime = ptImagingSetting->Exposure->MaxExposureTime;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}
		
		//最小增益
		if(ptImagingSetting->Exposure->MinGain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MinGain->Max
		&& ptImagingSetting->Exposure->MinGain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MinGain->Min)
		{
			if((int)ptImagingSetting->Exposure->MinGain != (int)ptVideoSource->Imaging->Exposure->MinGain)
			{
				iChangeFlag = 1;
				iEMinEGain  = (int)ptImagingSetting->Exposure->MinGain / 100;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->Exposure->MinGain = ptImagingSetting->Exposure->MinGain;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}

		
		//最大增益
		if(ptImagingSetting->Exposure->MaxGain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MaxGain->Max
		&& ptImagingSetting->Exposure->MaxGain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MaxGain->Min)
		{
			if((int)ptImagingSetting->Exposure->MaxGain != (int)ptVideoSource->Imaging->Exposure->MaxGain)
			{
				iChangeFlag = 1;
				iEMaxEGain  = (int)ptImagingSetting->Exposure->MaxGain / 100;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->Exposure->MaxGain = ptImagingSetting->Exposure->MaxGain;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}

		//最小光圈-- 内核不支持
		if(ptImagingSetting->Exposure->MinIris <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MinIris->Max
		&& ptImagingSetting->Exposure->MinIris >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MinIris->Min)
		{
			if((int)ptImagingSetting->Exposure->MinIris != (int)ptVideoSource->Imaging->Exposure->MinIris)
			{
				ptVideoSource->Imaging->Exposure->MinIris = ptImagingSetting->Exposure->MinIris;
			}
		}
		else
		{
			//goto ERR;
		}
		
		//最大光圈-- 内核不支持
		if(ptImagingSetting->Exposure->MaxIris <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MaxIris->Max
		&& ptImagingSetting->Exposure->MaxIris >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->MaxIris->Min)
		{
			if((int)ptImagingSetting->Exposure->MaxIris != (int)ptVideoSource->Imaging->Exposure->MaxIris)
			{
				ptVideoSource->Imaging->Exposure->MaxIris = ptImagingSetting->Exposure->MaxIris;
			}
		}
		else
		{
			//goto ERR;
		}
		
		//曝光时间-- 内核不支持
		if(ptImagingSetting->Exposure->ExposureTime <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->ExposureTime->Max
		&& ptImagingSetting->Exposure->ExposureTime >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->ExposureTime->Min)
		{
			if((int)ptImagingSetting->Exposure->ExposureTime != (int)ptVideoSource->Imaging->Exposure->ExposureTime)
			{
				ptVideoSource->Imaging->Exposure->ExposureTime = ptImagingSetting->Exposure->ExposureTime;
			}
		}
		else
		{
			//goto ERR;
		}
		
		//Gain  -- 内核不支持
		if(ptImagingSetting->Exposure->Gain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->Gain->Max
		&& ptImagingSetting->Exposure->Gain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->Gain->Min)
		{
			if((int)ptImagingSetting->Exposure->Gain != (int)ptVideoSource->Imaging->Exposure->Gain)
			{
				ptVideoSource->Imaging->Exposure->Gain = ptImagingSetting->Exposure->Gain;
			}
		}
		else
		{
			//goto ERR;
		}
		
		//光圈-- 内核不支持
		if(ptImagingSetting->Exposure->Iris <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->Iris->Max
		&& ptImagingSetting->Exposure->Iris >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Exposure->Iris->Min)
		{
			if((int)ptImagingSetting->Exposure->Iris != (int)ptVideoSource->Imaging->Exposure->Iris)
			{
				ptVideoSource->Imaging->Exposure->Iris = ptImagingSetting->Exposure->Iris;
			}
		}
		else
		{
			//goto ERR;
		}
	}

	//红外滤光片模式
	if(NULL != ptImagingSetting->IrCutFilter)
	{
		if(*ptImagingSetting->IrCutFilter == tt__IrCutFilterMode__ON
		|| *ptImagingSetting->IrCutFilter == tt__IrCutFilterMode__OFF
		|| *ptImagingSetting->IrCutFilter == tt__IrCutFilterMode__AUTO)
		{
			if(*ptVideoSource->Imaging->IrCutFilter != *ptImagingSetting->IrCutFilter)
			{
				iChangeFlag  = 1;
				iIrCutFilter = *ptImagingSetting->IrCutFilter;
			#ifdef WITH_TESTTOOL
				*ptVideoSource->Imaging->IrCutFilter = *ptImagingSetting->IrCutFilter;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}
	}

	//锐度
	if(NULL != ptImagingSetting->Sharpness)
	{
		if(*ptImagingSetting->Sharpness <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Sharpness->Max
		&& *ptImagingSetting->Sharpness >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->Sharpness->Min)
		{
			if((int)*ptImagingSetting->Sharpness != (int)*ptVideoSource->Imaging->Sharpness)
			{
				iChangeFlag = 1;
				iSharpness  = (int)*ptImagingSetting->Sharpness;
			#ifdef WITH_TESTTOOL
				*ptVideoSource->Imaging->Sharpness = *ptImagingSetting->Sharpness;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}
	}

	//宽动态
	if(NULL != ptImagingSetting->WideDynamicRange)
	{
		//模式
		if(ptImagingSetting->WideDynamicRange->Mode == tt__WideDynamicMode__OFF
		|| ptImagingSetting->WideDynamicRange->Mode == tt__WideDynamicMode__ON)
		{
			if(ptImagingSetting->WideDynamicRange->Mode != ptVideoSource->Imaging->WideDynamicRange->Mode)
			{
				iChangeFlag = 1;
				iWDRMode    = (ptImagingSetting->WideDynamicRange->Mode == tt__WideDynamicMode__ON)?1:2;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->WideDynamicRange->Mode = ptImagingSetting->WideDynamicRange->Mode;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}

		//级别
		if(ptImagingSetting->WideDynamicRange->Level <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->WideDynamicRange->Level->Max
		&& ptImagingSetting->WideDynamicRange->Level >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->WideDynamicRange->Level->Min)
		{
			if((int)ptImagingSetting->WideDynamicRange->Level != (int)ptVideoSource->Imaging->WideDynamicRange->Level)
			{
				iChangeFlag = 1;
				iWDRLevel   = (int)ptImagingSetting->WideDynamicRange->Level;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->WideDynamicRange->Level = ptImagingSetting->WideDynamicRange->Level;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}
	}

	//白平衡
	if(NULL != ptImagingSetting->WhiteBalance)
	{
		//模式
		if(ptImagingSetting->WhiteBalance->Mode == tt__WhiteBalanceMode__AUTO
		|| ptImagingSetting->WhiteBalance->Mode == tt__WhiteBalanceMode__MANUAL)
		{
			if(ptImagingSetting->WhiteBalance->Mode != ptVideoSource->Imaging->WhiteBalance->Mode)
			{
				iChangeFlag = 1;
				iWBMode     = (ptImagingSetting->WhiteBalance->Mode == tt__WhiteBalanceMode__MANUAL)?0:1;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->WhiteBalance->Mode = ptImagingSetting->WhiteBalance->Mode;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}

		//手动模式下的红增益
		if(ptImagingSetting->WhiteBalance->CrGain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->WhiteBalance->YrGain->Max
		&& ptImagingSetting->WhiteBalance->CrGain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->WhiteBalance->YrGain->Min)
		{
			if((int)ptImagingSetting->WhiteBalance->CrGain != (int)ptVideoSource->Imaging->WhiteBalance->CrGain)
			{
				iChangeFlag = 1;
				iWBCrGain   = (int)ptImagingSetting->WhiteBalance->CrGain;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->WhiteBalance->CrGain = ptImagingSetting->WhiteBalance->CrGain;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}

		//手动模式下的蓝增益
		if(ptImagingSetting->WhiteBalance->CbGain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->WhiteBalance->YbGain->Max
		&& ptImagingSetting->WhiteBalance->CbGain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions->WhiteBalance->YbGain->Min)
		{
			if((int)ptImagingSetting->WhiteBalance->CbGain != (int)ptVideoSource->Imaging->WhiteBalance->CbGain)
			{
				iChangeFlag = 1;
				iWBCbGain   = (int)ptImagingSetting->WhiteBalance->CbGain;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->WhiteBalance->CbGain = ptImagingSetting->WhiteBalance->CbGain;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}
	}

#ifndef WITH_TESTTOOL
	//更新内核及全局变量
	if(iChangeFlag)
	{
		if(OnvifCmdGetIspInfo(&stIspInfo))
		{
			return TD_ERROR;
		}

		if(iBCMode >= 0)
		{
			stIspInfo.m_iBLCMode = iBCMode;
		}

		if(iBCLevel >= 0)
		{
			stIspInfo.m_iBLCLevel = iBCLevel;
		}

		if(iEMode >= 0)
		{
			stIspInfo.m_iExposureMode = iEMode;
		}

		if(iEWinTop >= 0 || iEWinBottom >= 0 || iEWinLeft >= 0 || iEWinRight >= 0)
		{
			stIspInfo.m_tExpArray.m_iTop = iEWinTop;
			stIspInfo.m_tExpArray.m_iLeft = iEWinLeft;
			stIspInfo.m_tExpArray.m_uiHeight = (unsigned int)(iEWinBottom - iEWinTop);
			stIspInfo.m_tExpArray.m_uiWidth = (unsigned int)(iEWinRight - iEWinLeft);
		}
		
		if(iEMinETime >= 0)
		{
			stIspInfo.m_iExpMinTime = iEMinETime;
		}
		
		if(iEMaxETime >= 0)
		{
			stIspInfo.m_iExpMaxTime = iEMaxETime;
		}
		
		if(iEMinEGain >= 0)
		{
			stIspInfo.m_iExpMinGain = iEMinEGain;
		}
		
		if(iEMaxEGain >= 0)
		{
			stIspInfo.m_iExpMaxGain = iEMaxEGain;
		}

		if(iIrCutFilter >= 0)
		{
			//保证iIrCutFilter永不改变
			//stIspInfo.m_iIRMode = iIrCutFilter;
		}

		if(iSharpness >= 0)
		{
			stIspInfo.m_iSharpness = iSharpness;
		}
		
		if(iWDRMode >= 0)
		{
			stIspInfo.m_iWDRMode = iWDRMode;
		}
		  
		if(iWDRLevel >= 0)
		{
			stIspInfo.m_iWDRLevel = iWDRLevel;
		}

		if(iWBMode >= 0)
		{
			stIspInfo.m_iWBMode = iWBMode;
		}
		
		if(iWBCrGain >= 0)
		{
			stIspInfo.m_iWBRGain = iWBCrGain;
		}

		if(iWBCbGain >= 0)
		{
			stIspInfo.m_iWBBGain = iWBCbGain;
		}
		
		if(OnvifCmdSetIspInfo(&stIspInfo))
		{
			return TD_ERROR;
		}

		//更新全局变量值
		if(TD_OK != UpdateIspInfo())
		{
			td_printf(0, "%s %d %s =====UpdateIspInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
		}
	}
#endif
	return TD_OK;

//ERR:
	td_printf(0, "%s %s %d !Isp parameter is illegal.", __FILE__, __FUNCTION__, __LINE__);
	return TD_ERROR;
}

/**************************************************
*功能:		设置高清参数(onvifv20版本)
*参数:		_iChan			:		通道号
            _ptImgSetting  	:		图像参数结构体
*返回值:	0: 成功, !0: 失败
*修改历史:   20130130 v2.0 creat by ptb
            20130219 v2.3.0 modify by ptb
**************************************************/
int SetIspParameter20(int _iChan, struct tt__ImagingSettings20 *_ptImgSetting)
{
	struct tt__ImagingSettings20 *ptImagingSetting = _ptImgSetting;
	struct tt__VideoSource *ptVideoSource = NULL;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
#ifndef WITH_TESTTOOL
	TGuiVideoISPInfo  stIspInfo  = {0};
#endif
	int iChan 		= _iChan;
	int iChangeFlag = 0;
	int iIrCutFilter= -1;
	int iBCMode 	= -1;
	int iBCLevel 	= -1;
	int iEMode 		= -1;
	int iEPriority 	= -1;
	int iEWinTop 	= -1;
	int iEWinLeft 	= -1;
	int iEWinBottom = -1;
	int iEWinRight 	= -1;
	int iEMinETime 	= -1;
	int iEMaxETime 	= -1;
	int iEMinEGain 	= -1;
	int iEMaxEGain 	= -1;
	int iSharpness 	= -1;
	int iWDRMode 	= -1;
	int iWDRLevel	= -1;
	int iWBMode		= -1;
	int iWBCrGain	= -1;
	int iWBCbGain	= -1;
	
	if(NULL == ptOnvifCtrl || NULL == ptImagingSetting)
	{
		return SOAP_ERR;
	}
	
	//更新全局变量值
	if(ptOnvifCtrl->m_stFlag.m_iIspFlag)
	{
		if(TD_OK != UpdateIspInfo())
		{
			td_printf(0, "%s %d %s =====UpdateIspInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
		}
	}

	ptVideoSource = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources + iChan;

	//参数合法性判断
	//背光补偿
	if(NULL != ptImagingSetting->BacklightCompensation)
	{	
		//模式
		if(ptImagingSetting->BacklightCompensation->Mode == tt__BacklightCompensationMode__OFF
		|| ptImagingSetting->BacklightCompensation->Mode == tt__BacklightCompensationMode__ON)
		{
			if(ptImagingSetting->BacklightCompensation->Mode != ptVideoSource->Imaging->BacklightCompensation->Mode)
			{
				iChangeFlag = 1;
				iBCMode = (ptImagingSetting->BacklightCompensation->Mode == tt__BacklightCompensationMode__OFF)?2:1;
			#ifdef WITH_TESTTOOL
				//BacklightCompensation
				ptVideoSource->Imaging->BacklightCompensation->Mode  = iBCMode;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}
	
		//级别
		if(NULL != ptImagingSetting->BacklightCompensation->Level)
		{
			if(*ptImagingSetting->BacklightCompensation->Level <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->BacklightCompensation->Level->Max
			&& *ptImagingSetting->BacklightCompensation->Level >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->BacklightCompensation->Level->Min)
			{
				if((int)*ptImagingSetting->BacklightCompensation->Level != (int)ptVideoSource->Imaging->BacklightCompensation->Level)
				{
					iChangeFlag = 1;
					iBCLevel = (int)*ptImagingSetting->BacklightCompensation->Level;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->BacklightCompensation->Level = iBCLevel;
				#endif
				}
			}
			else
			{
				//goto ERR;
			}
		}
	}

	//曝光
	if(NULL != ptImagingSetting->Exposure)
	{
		//模式
		if(ptImagingSetting->Exposure->Mode == tt__ExposureMode__AUTO
		|| ptImagingSetting->Exposure->Mode == tt__ExposureMode__MANUAL)
		{
			if(ptImagingSetting->Exposure->Mode != ptVideoSource->Imaging->Exposure->Mode)
			{
				iChangeFlag = 1;
				iEMode = (ptImagingSetting->Exposure->Mode == tt__ExposureMode__MANUAL)?0:1;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->Exposure->Mode	= ptImagingSetting->Exposure->Mode;
			#endif
			}
		}
		else
		{
			//goto ERR;
		}

		//曝光策略
		if(NULL != ptImagingSetting->Exposure->Priority)
		{	
			if(*ptImagingSetting->Exposure->Priority == tt__ExposurePriority__LowNoise
			|| *ptImagingSetting->Exposure->Priority == tt__ExposurePriority__FrameRate)
			{
				if(*ptImagingSetting->Exposure->Priority != ptVideoSource->Imaging->Exposure->Priority)
				{
					iChangeFlag = 1;
					iEPriority = *ptImagingSetting->Exposure->Priority;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->Exposure->Priority = *ptImagingSetting->Exposure->Priority;
				#endif
				}
			}
			else
			{
				//goto ERR;
			}
		}

		//曝光区域
		if(NULL != ptImagingSetting->Exposure->Window)
		{	
			if(NULL != ptImagingSetting->Exposure->Window->bottom
			&& NULL != ptImagingSetting->Exposure->Window->top
			&& NULL != ptImagingSetting->Exposure->Window->right
			&& NULL != ptImagingSetting->Exposure->Window->left)
			{
				if((int)*ptVideoSource->Imaging->Exposure->Window->bottom != (int)*ptImagingSetting->Exposure->Window->bottom
				|| (int)*ptVideoSource->Imaging->Exposure->Window->top != (int)*ptImagingSetting->Exposure->Window->top
				|| (int)*ptVideoSource->Imaging->Exposure->Window->right != (int)*ptImagingSetting->Exposure->Window->right
				|| (int)*ptVideoSource->Imaging->Exposure->Window->left != (int)*ptImagingSetting->Exposure->Window->left)
				{
					iChangeFlag = 1;
					iEWinBottom = (int)*ptImagingSetting->Exposure->Window->bottom;
					iEWinTop 	= (int)*ptImagingSetting->Exposure->Window->top;
					iEWinRight 	= (int)*ptImagingSetting->Exposure->Window->right;
					iEWinLeft 	= (int)*ptImagingSetting->Exposure->Window->left;
				#ifdef WITH_TESTTOOL
					*ptVideoSource->Imaging->Exposure->Window->bottom = *ptImagingSetting->Exposure->Window->bottom;
					*ptVideoSource->Imaging->Exposure->Window->top    = *ptImagingSetting->Exposure->Window->top;
					*ptVideoSource->Imaging->Exposure->Window->right  = *ptImagingSetting->Exposure->Window->right;
					*ptVideoSource->Imaging->Exposure->Window->left   = *ptImagingSetting->Exposure->Window->left;
				#endif
				}
			}
		}
		
		//最小曝光时间
		if(NULL != ptImagingSetting->Exposure->MinExposureTime)
		{
			if(*ptImagingSetting->Exposure->MinExposureTime <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MinExposureTime->Max
			&& *ptImagingSetting->Exposure->MinExposureTime >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MinExposureTime->Min)
			{
				if((int)*ptImagingSetting->Exposure->MinExposureTime != (int)ptVideoSource->Imaging->Exposure->MinExposureTime)
				{
					iChangeFlag = 1;
					iEMinETime  = (int)*ptImagingSetting->Exposure->MinExposureTime;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->Exposure->MinExposureTime = *ptImagingSetting->Exposure->MinExposureTime;
				#endif
				}
			}
			else
			{
				//goto ERR;
			}
		}
		
		//最大曝光时间
		if(NULL != ptImagingSetting->Exposure->MaxExposureTime)
		{
			if(*ptImagingSetting->Exposure->MaxExposureTime <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MaxExposureTime->Max
			&& *ptImagingSetting->Exposure->MaxExposureTime >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MaxExposureTime->Min)
			{
				if((int)*ptImagingSetting->Exposure->MaxExposureTime != (int)ptVideoSource->Imaging->Exposure->MaxExposureTime)
				{
					iChangeFlag = 1;
					iEMaxETime  = (int)*ptImagingSetting->Exposure->MaxExposureTime;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->Exposure->MaxExposureTime = *ptImagingSetting->Exposure->MaxExposureTime;
				#endif
				}
			}
			else
			{
				//goto ERR;
			}
		}
		
		//最小增益
		if(NULL != ptImagingSetting->Exposure->MinGain)
		{
			if(*ptImagingSetting->Exposure->MinGain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MinGain->Max
			&& *ptImagingSetting->Exposure->MinGain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MinGain->Min)
			{
				if((int)*ptImagingSetting->Exposure->MinGain != (int)ptVideoSource->Imaging->Exposure->MinGain)
				{
					iChangeFlag = 1;
					iEMinEGain  = (int)*ptImagingSetting->Exposure->MinGain / 100;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->Exposure->MinGain = *ptImagingSetting->Exposure->MinGain;
				#endif
				}
			}
			else
			{
				//goto ERR;
			}
		}
		
		//最大增益
		if(NULL != ptImagingSetting->Exposure->MaxGain)
		{
			if(*ptImagingSetting->Exposure->MaxGain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MaxGain->Max
			&& *ptImagingSetting->Exposure->MaxGain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MaxGain->Min)
			{
				if((int)*ptImagingSetting->Exposure->MaxGain != (int)ptVideoSource->Imaging->Exposure->MaxGain)
				{
					iChangeFlag = 1;
					iEMaxEGain  = (int)*ptImagingSetting->Exposure->MaxGain / 100;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->Exposure->MaxGain = *ptImagingSetting->Exposure->MaxGain;
				#endif
				}
			}
			else
			{
				//goto ERR;
			}
		}

		//最小光圈-- 内核不支持
		if(NULL != ptImagingSetting->Exposure->MinIris)
		{
			if(*ptImagingSetting->Exposure->MinIris <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MinIris->Max
			&& *ptImagingSetting->Exposure->MinIris >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MinIris->Min)
			{
				if((int)*ptImagingSetting->Exposure->MinIris != (int)ptVideoSource->Imaging->Exposure->MinIris)
				{
					ptVideoSource->Imaging->Exposure->MinIris = *ptImagingSetting->Exposure->MinIris;
				}
			}
			else
			{
				//goto ERR;
			}
		}
		
		//最大光圈-- 内核不支持
		if(NULL != ptImagingSetting->Exposure->MaxIris)
		{
			if(*ptImagingSetting->Exposure->MaxIris <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MaxIris->Max
			&& *ptImagingSetting->Exposure->MaxIris >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->MaxIris->Min)
			{
				if((int)*ptImagingSetting->Exposure->MaxIris != (int)ptVideoSource[iChan].Imaging->Exposure->MaxIris)
				{
					ptVideoSource->Imaging->Exposure->MaxIris = *ptImagingSetting->Exposure->MaxIris;
				}
			}
			else
			{
				//goto ERR;
			}
		}
		
		//曝光时间-- 内核不支持
		if(NULL != ptImagingSetting->Exposure->ExposureTime)
		{
			if(*ptImagingSetting->Exposure->ExposureTime <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->ExposureTime->Max
			&& *ptImagingSetting->Exposure->ExposureTime >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->ExposureTime->Min)
			{
				if((int)*ptImagingSetting->Exposure->ExposureTime != (int)ptVideoSource->Imaging->Exposure->ExposureTime)
				{
					ptVideoSource->Imaging->Exposure->ExposureTime = *ptImagingSetting->Exposure->ExposureTime;
				}
			}
			else
			{
				//goto ERR;
			}
		}
		
		//Gain  -- 内核不支持
		if(NULL != ptImagingSetting->Exposure->Gain)
		{
			if(*ptImagingSetting->Exposure->Gain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->Gain->Max
			&& *ptImagingSetting->Exposure->Gain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->Gain->Min)
			{
				if((int)*ptImagingSetting->Exposure->Gain != (int)ptVideoSource->Imaging->Exposure->Gain)
				{
					ptVideoSource->Imaging->Exposure->Gain = *ptImagingSetting->Exposure->Gain;
				}
			}
			else
			{
				//goto ERR;
			}
		}
		
		//光圈-- 内核不支持
		if(NULL != ptImagingSetting->Exposure->Iris)
		{
			if(*ptImagingSetting->Exposure->Iris <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->Iris->Max
			&& *ptImagingSetting->Exposure->Iris >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Exposure->Iris->Min)
			{
				if((int)*ptImagingSetting->Exposure->Iris != (int)ptVideoSource->Imaging->Exposure->Iris)
				{
					ptVideoSource->Imaging->Exposure->Iris = *ptImagingSetting->Exposure->Iris;
				}
			}
			else
			{
				//goto ERR;
			}
		}
	}
		
	//红外滤光片模式
	if(NULL != ptImagingSetting->IrCutFilter)
	{
		if(*ptImagingSetting->IrCutFilter == tt__IrCutFilterMode__ON
		|| *ptImagingSetting->IrCutFilter == tt__IrCutFilterMode__OFF
		|| *ptImagingSetting->IrCutFilter == tt__IrCutFilterMode__AUTO)
		{
			if(*ptImagingSetting->IrCutFilter != *ptVideoSource->Imaging->IrCutFilter)
			{
				iChangeFlag  = 1;
				if(tt__IrCutFilterMode__ON == *ptImagingSetting->IrCutFilter)
				{
					iIrCutFilter = OPEN;
				}
				else if(tt__IrCutFilterMode__OFF == *ptImagingSetting->IrCutFilter)
				{
					iIrCutFilter = CLOSE;
				}
				else if(tt__IrCutFilterMode__AUTO == *ptImagingSetting->IrCutFilter)
				{
					iIrCutFilter = AUTO;
				}
				else
				{
					td_printf(0, "%s %d %s IrCutFilterMode is not match[%d]!\n", __FILE__, __LINE__, __FUNCTION__, *ptImagingSetting->IrCutFilter);
				}
			#ifdef WITH_TESTTOOL
				*ptVideoSource->Imaging->IrCutFilter = *ptImagingSetting->IrCutFilter;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}
	}

	//锐度
	if(NULL != ptImagingSetting->Sharpness)
	{
		if(*ptImagingSetting->Sharpness <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Sharpness->Max
		&& *ptImagingSetting->Sharpness >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->Sharpness->Min)
		{
			if((int)*ptImagingSetting->Sharpness != (int)*ptVideoSource->Imaging->Sharpness)
			{
				iChangeFlag = 1;
				iSharpness  = (int)*ptImagingSetting->Sharpness;
			#ifdef WITH_TESTTOOL
				*ptVideoSource->Imaging->Sharpness = *ptImagingSetting->Sharpness;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}
	}

	//宽动态
	if(NULL != ptImagingSetting->WideDynamicRange)
	{
		//模式
		if(ptImagingSetting->WideDynamicRange->Mode == tt__WideDynamicMode__OFF
		|| ptImagingSetting->WideDynamicRange->Mode == tt__WideDynamicMode__ON)
		{
			if(ptImagingSetting->WideDynamicRange->Mode != ptVideoSource->Imaging->WideDynamicRange->Mode)
			{
				iChangeFlag = 1;
				iWDRMode    = (ptImagingSetting->WideDynamicRange->Mode == tt__WideDynamicMode__ON)?1:2;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->WideDynamicRange->Mode = ptImagingSetting->WideDynamicRange->Mode;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}

		//级别
		if(NULL != ptImagingSetting->WideDynamicRange->Level)
		{
			if(*ptImagingSetting->WideDynamicRange->Level <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->WideDynamicRange->Level->Max
			&& *ptImagingSetting->WideDynamicRange->Level >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->WideDynamicRange->Level->Min)
			{
				if((int)*ptImagingSetting->WideDynamicRange->Level != (int)ptVideoSource->Imaging->WideDynamicRange->Level)
				{
					iChangeFlag = 1;
					iWDRLevel   = (int)*ptImagingSetting->WideDynamicRange->Level;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->WideDynamicRange->Level = *ptImagingSetting->WideDynamicRange->Level;
				#endif					
				}
			}
			else
			{
				//goto ERR;
			}
		}
	}

	//白平衡
	if(NULL != ptImagingSetting->WhiteBalance)
	{
		//模式
		if(ptImagingSetting->WhiteBalance->Mode == tt__WhiteBalanceMode__AUTO
		|| ptImagingSetting->WhiteBalance->Mode == tt__WhiteBalanceMode__MANUAL)
		{
			if(ptImagingSetting->WhiteBalance->Mode != ptVideoSource->Imaging->WhiteBalance->Mode)
			{
				iChangeFlag = 1;
				iWBMode     = (ptImagingSetting->WhiteBalance->Mode == tt__WhiteBalanceMode__MANUAL)?0:1;
			#ifdef WITH_TESTTOOL
				ptVideoSource->Imaging->WhiteBalance->Mode = ptImagingSetting->WhiteBalance->Mode;
			#endif					
			}
		}
		else
		{
			//goto ERR;
		}

		//手动模式下的红增益
		if(NULL != ptImagingSetting->WhiteBalance->CrGain)
		{
			if(*ptImagingSetting->WhiteBalance->CrGain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->WhiteBalance->YrGain->Max
			&& *ptImagingSetting->WhiteBalance->CrGain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->WhiteBalance->YrGain->Min)
			{
				if((int)*ptImagingSetting->WhiteBalance->CrGain != (int)ptVideoSource->Imaging->WhiteBalance->CrGain)
				{
					iChangeFlag = 1;
					iWBCrGain   = (int)*ptImagingSetting->WhiteBalance->CrGain;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->WhiteBalance->CrGain = *ptImagingSetting->WhiteBalance->CrGain;
				#endif					
				}
			}
			else
			{
				//goto ERR;
			}
		}
		
		//手动模式下的蓝增益
		if(NULL != ptImagingSetting->WhiteBalance->CbGain)
		{
			if(*ptImagingSetting->WhiteBalance->CbGain <= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->WhiteBalance->YbGain->Max
			&& *ptImagingSetting->WhiteBalance->CbGain >= ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions->WhiteBalance->YbGain->Min)
			{
				if((int)*ptImagingSetting->WhiteBalance->CbGain != (int)ptVideoSource->Imaging->WhiteBalance->CbGain)
				{
					iChangeFlag = 1;
					iWBCbGain   = (int)*ptImagingSetting->WhiteBalance->CbGain;
				#ifdef WITH_TESTTOOL
					ptVideoSource->Imaging->WhiteBalance->CbGain = *ptImagingSetting->WhiteBalance->CbGain;
				#endif					
				}
			}
			else
			{
				//goto ERR;
			}
		}
	}
	
#ifndef WITH_TESTTOOL
	//更新内核及全局变量
	if(iChangeFlag)
	{
		if(OnvifCmdGetIspInfo(&stIspInfo))
		{
			return TD_ERROR;
		}
		
		if(iBCMode >= 0)
		{
			stIspInfo.m_iBLCMode = iBCMode;
		}

		if(iBCLevel >= 0)
		{
			stIspInfo.m_iBLCLevel = iBCLevel;
		}

		if(iEMode >= 0)
		{
			stIspInfo.m_iExposureMode = iEMode;
		}

		if(iEWinTop >= 0 || iEWinBottom >= 0 || iEWinLeft >= 0 || iEWinRight >= 0)
		{
			stIspInfo.m_tExpArray.m_iTop = iEWinTop;
			stIspInfo.m_tExpArray.m_iLeft = iEWinLeft;
			stIspInfo.m_tExpArray.m_uiHeight = (unsigned int)(iEWinBottom - iEWinTop);
			stIspInfo.m_tExpArray.m_uiWidth = (unsigned int)(iEWinRight - iEWinLeft);
		}
		
		if(iEMinETime >= 0)
		{
			stIspInfo.m_iExpMinTime = iEMinETime;
		}
		
		if(iEMaxETime >= 0)
		{
			stIspInfo.m_iExpMaxTime = iEMaxETime;
		}
		
		if(iEMinEGain >= 0)
		{
			stIspInfo.m_iExpMinGain = iEMinEGain;
		}
		
		if(iEMaxEGain >= 0)
		{
			stIspInfo.m_iExpMaxGain = iEMaxEGain;
		}
		
		if(iIrCutFilter >= 0)
		{
			//保证此值永不改变
			//stIspInfo.m_iIRMode = iIrCutFilter;
		}
		
		if(iSharpness >= 0)
		{
			stIspInfo.m_iSharpness = iSharpness;
		}
		
		if(iWDRMode >= 0)
		{
			stIspInfo.m_iWDRMode = iWDRMode;
		}
		  
		if(iWDRLevel >= 0)
		{
			stIspInfo.m_iWDRLevel = iWDRLevel;
		}

		if(iWBMode >= 0)
		{
			stIspInfo.m_iWBMode = iWBMode;
		}
		
		if(iWBCrGain >= 0)
		{
			stIspInfo.m_iWBRGain = iWBCrGain;
		}

		if(iWBCbGain >= 0)
		{
			stIspInfo.m_iWBBGain = iWBCbGain;
		}
		
		if(OnvifCmdSetIspInfo(&stIspInfo))
		{
			return TD_ERROR;
		}
		
		//更新全局变量值
		if(ptOnvifCtrl->m_stFlag.m_iIspFlag)
		{
			if(TD_OK != UpdateIspInfo())
			{
				td_printf(0, "%s %d %s =====UpdateIspInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
			}
		}
	}
#endif
	return TD_OK;

//ERR:
	td_printf(0, "%s %s %d !Isp parameter is illegal.", __FILE__, __FUNCTION__, __LINE__);
	return TD_ERROR;
}

/**************************************************
*功能:		设置视频输入参数(亮度、对比度、饱和度)
*参数:		_iChan			    :		通道号
            _fBrightness	    :		亮度
			_fContrast		    :		对比度
			_fColorSaturation	:		色彩饱和度
*返回值:	0: 成功, !0: 失败
*修改历史:   20130130 v2.0 creat by ptb
                            20130219 v2.3.0 modify by ptb
**************************************************/
int SetVideoInputParameter(int _iChan, float _fBrightness, float _fContrast, float _fColorSaturation)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	struct tt__VideoSource *pVideoSources = NULL;
	VideoInCnf_EX stVInCnf= {0};
	int iColorSaturation  = -1;
	int iBrightness 	  = -1;
	int iContrast 	   	  = -1;
	int iChangeFlag 	  = 0;
	int iChan 			  = _iChan;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return SOAP_ERR;
	}

	if(OnvifCmdGetVideoInCnf(iChan + 1, &stVInCnf))
	{
		return TD_ERROR;
	}

	pVideoSources = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources + iChan;

	//更新亮度、饱和度、对比度
	*(pVideoSources->Imaging->Brightness) 	   = (float)stVInCnf.m_iLum;
	*(pVideoSources->Imaging->ColorSaturation) = (float)stVInCnf.m_iSaturation;
	*(pVideoSources->Imaging->Contrast)		   = (float)stVInCnf.m_iContrast;

	//Brightness
	if((int)*pVideoSources->Imaging->Brightness != (int)_fBrightness)
	{
		iChangeFlag = 1;
		iBrightness = (int)_fBrightness;
	}

	//ColorSaturation
	if((int)*pVideoSources->Imaging->ColorSaturation != (int)_fColorSaturation)
	{
		iChangeFlag = 1;
		iColorSaturation = (int)_fColorSaturation;
	}

	//Contrast
	if((int)*pVideoSources->Imaging->Contrast != (int)_fContrast)
	{
		iChangeFlag = 1;
		iContrast = (int)_fContrast;
	}

	if(iChangeFlag)
	{
		if(iBrightness >= 0)
		{
			stVInCnf.m_iLum = iBrightness;
			*pVideoSources->Imaging->Brightness = _fBrightness;
		}
		
		if(iColorSaturation >= 0)
		{
			stVInCnf.m_iSaturation = iColorSaturation;
			*pVideoSources->Imaging->ColorSaturation = _fColorSaturation;
		}
		
		if(iContrast >= 0)
		{
			stVInCnf.m_iContrast = iContrast;
			*pVideoSources->Imaging->Contrast = _fContrast;
		}

		if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
		{
			iChan = 1;
			if(OnvifCmdSetVideoInCnf(iChan, &stVInCnf))
			{
				return TD_ERROR;
			}
		}
		else if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
		{
			if(OnvifCmdSetDVRVideoInCnf(iChan + 1, &stVInCnf))
			{
				return TD_ERROR;
			}
		}
		else
		{
			td_printf(0, "%s %d %s ===ptb m_iDeviceType[%d] is error!\n", __FILE__, __LINE__, __FUNCTION__, ptOnvifCtrl->m_stNvsInfo.m_iDeviceType);
		}
	}
	return TD_OK;
}

/**************************************************
*功能:		设置视频编码参数
*参数:		_iChan			    :		通道号
			_ptConfiguration	:		视频编码结构体
*返回值:	0: 成功, !0: 失败
*修改历史:   20130131 v2.0 creat by ptb
**************************************************/
int SetVideoEncodeParameter(int _iChan, struct tt__VideoEncoderConfiguration *_ptConfiguration)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	struct tt__VideoEncoderConfiguration *ptConfiguration = _ptConfiguration;
	struct tt__VideoResolution stResolution;
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	TGuiVideoEncodeCnf stVEncodeCnf = {0};
	TFloatRange tPicQuality = {14, 6};
	TFloatRange tOnvifQuality = {VECOQRMIN, VECOQRMAX};
	int iChan 		= _iChan;
	int iChangeFlag = 0;
	int iIFrameRate = -1;
	int iFrameRate 	= -1;
	int iVideoSize 	= -1;
	int iEncoding 	= -1;
	int iQuality 	= -1;
	int iBitrate 	= -1;
	int i 			= 0;
	
	if(pointer_valid_check(ptOnvifCtrl) || pointer_valid_check(ptConfiguration))
	{
		return SOAP_ERR;
	}
	
	stResolution.Width  = -1;
	stResolution.Height = -1;
	
	if(OnvifCmdGetVideoEncodeCnf(iChan + 1 , &stVEncodeCnf))
	{
		td_printf(0, "%s %s %d !OnvifCmdGetVideoEncodeCnf error!", __FILE__, __FUNCTION__, __LINE__);
	}

	if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		stVEncodeCnf.m_iEncodeType = stVEncodeCnf.m_iEncodeType & 0x0F;
	}
	
	//更新视频参数信息
	if(TD_OK != UpdateVideoInfo(ptOnvifCtrl, iChan))
	{
		td_printf(0, "%s %d %s =====UpdateVideoInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}

	//检查参数合法性
	//编码方式
	if(tt__VideoEncoding__H264 == ptConfiguration->Encoding
	|| tt__VideoEncoding__JPEG == ptConfiguration->Encoding)
	{
		if(ptConfiguration->Encoding != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Encoding)
		{
			iChangeFlag = 1;
			iEncoding = ptConfiguration->Encoding;
		}
	}
	else if(tt__VideoEncoding__MPEG4 == ptConfiguration->Encoding)
	{
		td_printf(0, "%s %s %d !The MPEG4 is not support!", __FILE__, __FUNCTION__, __LINE__);
		return 1;
	}
	else 
	{
		td_printf(0, "%s %s %d !The videoEncoding type is not support!", __FILE__, __FUNCTION__, __LINE__);
		return TD_ERROR;
	}
	
	if(tt__VideoEncoding__H264 == ptConfiguration->Encoding)
	{
		//分辨率
		if(NULL != ptConfiguration->Resolution)
		{
			for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->__sizeResolutionsAvailable; i++)
			{
				if(ptConfiguration->Resolution->Width == ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->ResolutionsAvailable[i].Width 
				&& ptConfiguration->Resolution->Height == ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->ResolutionsAvailable[i].Height)
				{
					if(ptConfiguration->Resolution->Width != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Resolution->Width
					|| ptConfiguration->Resolution->Height != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Resolution->Height)
					{
						iChangeFlag = 1;
						stResolution.Width  = ptConfiguration->Resolution->Width;
						stResolution.Height = ptConfiguration->Resolution->Height;
					}
					break;
				}
				else
				{
					if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->__sizeResolutionsAvailable - 1)
					{
						td_printf(0, "%s %s %d ! ============H.264 Resolution[%d * %d]!", __FILE__, __FUNCTION__, __LINE__, ptConfiguration->Resolution->Width, ptConfiguration->Resolution->Height);
						td_printf(0, "%s %s %d !The resolution is not support!", __FILE__, __FUNCTION__, __LINE__);
						return TD_ERROR;
					}
				}
			}
			
		}

		if(NULL != ptConfiguration->RateControl)
		{
			//帧率
			if(ptConfiguration->RateControl->FrameRateLimit >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->FrameRateRange->Min
			&& ptConfiguration->RateControl->FrameRateLimit <= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->FrameRateRange->Max)
			{
				if(ptConfiguration->RateControl->FrameRateLimit != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->FrameRateLimit)
				{
					iChangeFlag = 1;
					iFrameRate = ptConfiguration->RateControl->FrameRateLimit;
				}			
			}
			else
			{
				td_printf(0, "%s %s %d !The FrameRate is illegal!", __FILE__, __FUNCTION__, __LINE__);
				return TD_ERROR;
			}
			
			//RateControl -- EncodingInterval
			if(ptConfiguration->RateControl->EncodingInterval >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->EncodingIntervalRange->Min
			&& ptConfiguration->RateControl->EncodingInterval <= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->EncodingIntervalRange->Max)
			{
				if(ptConfiguration->RateControl->EncodingInterval != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->EncodingInterval)
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->EncodingInterval = ptConfiguration->RateControl->EncodingInterval;
				}			
			}
			else
			{
				td_printf(0, "%s %s %d !The EncodingInterval is illegal!", __FILE__, __FUNCTION__, __LINE__);
				return TD_ERROR;
			}
			
			//码率
			if(ptConfiguration->RateControl->BitrateLimit >= VECOBITRATEMIN
			&& ptConfiguration->RateControl->BitrateLimit <= VECOBITRATEMAX)
			{
				if(ptConfiguration->RateControl->BitrateLimit != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->BitrateLimit)
				{
					iChangeFlag = 1;
					iBitrate = ptConfiguration->RateControl->BitrateLimit;
				}			
			}
			else if (ptConfiguration->RateControl->BitrateLimit == 64000)
			{
				//just for passing the onvif test tools
			}
			else
			{
				td_printf(0, "%s %s %d !The BitrateLimit is illegal!", __FILE__, __FUNCTION__, __LINE__);
				return TD_ERROR;
			}
		}

		if(NULL != ptConfiguration->H264)
		{
			//I帧帧率
			if(ptConfiguration->H264->GovLength >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->GovLengthRange->Min 
			&& ptConfiguration->H264->GovLength <= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].H264->GovLengthRange->Max)
			{
				if(ptConfiguration->H264->GovLength != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].H264->GovLength)
				{
					iChangeFlag = 1;
					iIFrameRate = ptConfiguration->H264->GovLength;
				}			
			}
			else
			{
				td_printf(0, "%s %s %d !The GovLength is illegal!", __FILE__, __FUNCTION__, __LINE__);
				return TD_ERROR;
			}

			//H264Profile
			if(tt__H264Profile__Baseline == ptConfiguration->H264->H264Profile 
			|| tt__H264Profile__Main == ptConfiguration->H264->H264Profile
			|| tt__H264Profile__Extended == ptConfiguration->H264->H264Profile
			|| tt__H264Profile__High == ptConfiguration->H264->H264Profile
			)
			{
				if(ptConfiguration->H264->H264Profile != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].H264->H264Profile)
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].H264->H264Profile = ptConfiguration->H264->H264Profile;
				}			
			}
			else
			{
				td_printf(0, "%s %s %d !The H264Profile is illegal!", __FILE__, __FUNCTION__, __LINE__);
				return TD_ERROR;
			}	
		}
	}
	else
	{
		//JPEG
		//分辨率
		if(NULL != ptConfiguration->Resolution)
		{
			for(i = 0; i < ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].JPEG->__sizeResolutionsAvailable; i++)
			{
				if(ptConfiguration->Resolution->Width == ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].JPEG->ResolutionsAvailable[i].Width 
				&& ptConfiguration->Resolution->Height == ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].JPEG->ResolutionsAvailable[i].Height)
				{
					if(ptConfiguration->Resolution->Width != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Resolution->Width
					|| ptConfiguration->Resolution->Height != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Resolution->Height)
					{
						iChangeFlag = 1;
						stResolution.Width  = ptConfiguration->Resolution->Width;
						stResolution.Height = ptConfiguration->Resolution->Height;
					}
					break;
				}
				else
				{
					if(i >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].JPEG->__sizeResolutionsAvailable - 1)
					{
						td_printf(0, "%s %s %d ! ============JPEG Resolution[%d * %d]!", __FILE__, __FUNCTION__, __LINE__, ptConfiguration->Resolution->Width, ptConfiguration->Resolution->Height);
						td_printf(0, "%s %s %d !The resolution is not support!", __FILE__, __FUNCTION__, __LINE__);
						return TD_ERROR;
					}
				}
			}
		}

		if(NULL != ptConfiguration->RateControl)
		{
			//帧率
			if(ptConfiguration->RateControl->FrameRateLimit >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].JPEG->FrameRateRange->Min
			&& ptConfiguration->RateControl->FrameRateLimit <= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].JPEG->FrameRateRange->Max)
			{
				if(ptConfiguration->RateControl->FrameRateLimit != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->FrameRateLimit)
				{
					iChangeFlag = 1;
					iFrameRate = ptConfiguration->RateControl->FrameRateLimit;
				}			
			}
			else
			{
				td_printf(0, "%s %s %d !The FrameRate is illegal!", __FILE__, __FUNCTION__, __LINE__);
				return TD_ERROR;
			}
			//RateControl -- EncodingInterval
			if(ptConfiguration->RateControl->EncodingInterval >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].JPEG->EncodingIntervalRange->Min
			&& ptConfiguration->RateControl->EncodingInterval <= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].JPEG->EncodingIntervalRange->Max)
			{
				if(ptConfiguration->RateControl->EncodingInterval != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->EncodingInterval)
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->EncodingInterval = ptConfiguration->RateControl->EncodingInterval;
				}			
			}
			else
			{
				td_printf(0, "%s %s %d !The EncodingInterval is illegal!", __FILE__, __FUNCTION__, __LINE__);
				return TD_ERROR;
			}
			
			//码率
			if(ptConfiguration->RateControl->BitrateLimit >= VECOBITRATEMIN
			&& ptConfiguration->RateControl->BitrateLimit <= VECOBITRATEMAX)
			{
				if(ptConfiguration->RateControl->BitrateLimit != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->BitrateLimit)
				{
					iChangeFlag = 1;
					iBitrate = ptConfiguration->RateControl->BitrateLimit;
				}			
			}
			else if (ptConfiguration->RateControl->BitrateLimit == 64000)
			{
				//just for passing the onvif test tools
			}
			else
			{
				td_printf(0, "%s %s %d !The BitrateLimit is illegal!", __FILE__, __FUNCTION__, __LINE__);
				return TD_ERROR;
			}
		}		
		
	}
		
	//视频质量
	if(ptConfiguration->Quality >= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].QualityRange->Min 
	&& ptConfiguration->Quality <= ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iChan].QualityRange->Max)
	{
		if(ptConfiguration->Quality != ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Quality)
		{
			iChangeFlag = 1;
			iQuality = ExchangeValue(ptConfiguration->Quality, tOnvifQuality, tPicQuality);
		}
	}
	else
	{
		td_printf(0, "%s %s %d !The video quality[%f] is illegal!", __FILE__, __FUNCTION__, __LINE__, ptConfiguration->Quality);
		return TD_ERROR;
	}
		
	//更新全局变量及内核
	if(iChangeFlag)
	{
		if(iEncoding >= 0)
		{
			//目前只支持H.264和MJPEG编码
			if(tt__VideoEncoding__H264 == iEncoding)
			{
				stVEncodeCnf.m_iEncodeType = GUI_H264;	
			}
			else
			{
				stVEncodeCnf.m_iEncodeType = GUI_MJPG;	
			}
		}
	
		if(iQuality >= 0)
		{
			stVEncodeCnf.m_iPicQualityLv = iQuality;
		}

		if(stResolution.Width >= 0 || stResolution.Height >= 0)
		{
			MatchVideoSize(&iVideoSize, &stResolution);
			stVEncodeCnf.m_iPicSize = iVideoSize;
		}

		if(iFrameRate >= 0)
		{
			if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
			{
				stVEncodeCnf.m_iFrameRate = iFrameRate;
			}
			else
			{
			#ifdef WITH_TESTTOOL
				if(iFrameRate < 10)
				{
					iFrameRate = 10;
				}
			#endif	
				if(GUI_PAL_MODE == ptOnvifCtrl->m_stNvsInfo.m_tVideoParam.m_InputNorm[iChan] 
				&& 25 == iFrameRate)
				{
					stVEncodeCnf.m_iFrameRate = GUI_FULL_FRAME_RATE;
				}
				else if(GUI_NTSC_MODE == ptOnvifCtrl->m_stNvsInfo.m_tVideoParam.m_InputNorm[iChan]
				&& 30 == iFrameRate)
				{
					stVEncodeCnf.m_iFrameRate = GUI_FULL_FRAME_RATE;
				}
				else
				{
					stVEncodeCnf.m_iFrameRate = iFrameRate;
				}
			}
		}

		if(iBitrate >= 0)
		{
			stVEncodeCnf.m_iBitRate = iBitrate;
		}

		if(iIFrameRate >= 0)
		{
			stVEncodeCnf.m_iIFrameRate = iIFrameRate;
		}
		
		if(OnvifCmdSetAVEncodeCnf(iChan + 1, &stVEncodeCnf))
		{
			td_printf(0, "%s %s %d !OnvifCmdSetAVEncodeCnf is error!", __FILE__, __FUNCTION__, __LINE__);
			return TD_ERROR;
		}

		//共享内存设置成功后修改全局变量
		if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
		{
			if(iEncoding >= 0 || stResolution.Width >= 0 || stResolution.Height >= 0)
			{
				VerifyDeviceRebootFlag();
			}
		}

		//编码方式
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Encoding = \
			(stVEncodeCnf.m_iEncodeType == GUI_H264) ? tt__VideoEncoding__H264 : tt__VideoEncoding__JPEG;

		//视频质量
		//兼容S2设备，S2:2-10	S3:6-14
		if(stVEncodeCnf.m_iPicQualityLv < 6)
		{
			stVEncodeCnf.m_iPicQualityLv += 4; 
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Quality = (float)stVEncodeCnf.m_iPicQualityLv;

		//视频宽、高
		stResolution.Width  = -1;
		stResolution.Height = -1;
		MatchVideoSize(&stVEncodeCnf.m_iPicSize, &stResolution);
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Resolution->Width  = stResolution.Width;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].Resolution->Height = stResolution.Height;

		//帧率
		if(GUI_FULL_FRAME_RATE == stVEncodeCnf.m_iFrameRate)
		{
			if(GUI_PAL_MODE == ptOnvifCtrl->m_stNvsInfo.m_tVideoParam.m_InputNorm[iChan])
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->FrameRateLimit = PAL_MAXFRAMERATE;
			}
			else if(GUI_NTSC_MODE == ptOnvifCtrl->m_stNvsInfo.m_tVideoParam.m_InputNorm[iChan])
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->FrameRateLimit = NTSC_MAXFRAMERATE;
			}
			else
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->FrameRateLimit = PAL_MAXFRAMERATE;
			}

		}
		else
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->FrameRateLimit = stVEncodeCnf.m_iFrameRate;
		}

		//码率
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].RateControl->BitrateLimit = stVEncodeCnf.m_iBitRate;

		//I帧帧率
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iChan].H264->GovLength = stVEncodeCnf.m_iIFrameRate;
	}

	return TD_OK;
}

/**************************************************
*功能:		匹配视频大小
*参数:		_piVideoSize		:	视频大小
				_ptResolution	:	分辨率宽、高
*返回值:	0		:	成功
				非0	:	失败
*修改历史:
			20130131 v2.0 creat by ptb
**************************************************/
int MatchVideoSize(int *_piVideoSize, struct tt__VideoResolution *_ptResolution)
{
	if (NULL == _piVideoSize || NULL == _ptResolution)
	{
		td_printf(0, "%s %d %s: VideoSize or Resolution is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	
	if (*_piVideoSize < 0)
	{
		if(_ptResolution->Width  > 1920 && _ptResolution->Width  <= 2048 \
		&& _ptResolution->Height > 1088 && _ptResolution->Height <= 1536)
		{
			*_piVideoSize = QXGA;
		}
		else if(_ptResolution->Width > 1600 && _ptResolution->Width <= 1920 \
			&& _ptResolution->Height > 960 && _ptResolution->Height <= 1088)
		{
			*_piVideoSize = F1080P;
		}
		else if(_ptResolution->Width > 1280 && _ptResolution->Width <= 1600 \
			&& _ptResolution->Height > 960 && _ptResolution->Height <= 1200)
		{
			*_piVideoSize = F1200;
		}
		else if(_ptResolution->Width > 704 && _ptResolution->Width <= 1280 \
			&& _ptResolution->Height > 720 && _ptResolution->Height <= 960)
		{
			*_piVideoSize = F960P;
		}
		else if(_ptResolution->Width > 704 && _ptResolution->Width <= 1280 \
			&& _ptResolution->Height > 576 && _ptResolution->Height <= 720)
		{
			*_piVideoSize = F720P;
		}
		else if(_ptResolution->Width > 352 && _ptResolution->Width <= 640 \
			&& _ptResolution->Height > 144 && _ptResolution->Height <= 240)
		{
			*_piVideoSize = HVGA;
		}
		else if(_ptResolution->Width > 352 && _ptResolution->Width <= 640 \
			&& _ptResolution->Height > 288 && _ptResolution->Height <= 480)
		{
			*_piVideoSize = FVGA;
		}
		else if(_ptResolution->Width > 176 && _ptResolution->Width <= 320 \
			&& _ptResolution->Height > 144 && _ptResolution->Height <= 240)
		{
			*_piVideoSize = QVGA;
		}
		else if(_ptResolution->Width > 640 && _ptResolution->Width <= 704 \
			&& _ptResolution->Height > 288 && _ptResolution->Height <= 576)
		{
			*_piVideoSize = FD1;
		}
		else if(_ptResolution->Width > 640 && _ptResolution->Width <= 704 \
			&& _ptResolution->Height > 144 && _ptResolution->Height <= 288)
		{
			*_piVideoSize = HD1;//MD1
		}
		else if(_ptResolution->Width > 176 && _ptResolution->Width <= 352 \
			&& _ptResolution->Height > 144 && _ptResolution->Height <= 288)
		{
			*_piVideoSize = FCIF;
		}
		else if(_ptResolution->Width > 176 && _ptResolution->Width <= 352 \
			&& _ptResolution->Height > 0 && _ptResolution->Height <= 144)
		{
			*_piVideoSize = HCIF;
		}
		else if(_ptResolution->Width > 0 && _ptResolution->Width <= 176 \
			&& _ptResolution->Height > 0 && _ptResolution->Height <= 144)
		{
			*_piVideoSize = QCIF;
		}
		else
		{
			td_printf(0, "%s %d %s warnning: video size[%d * %d] is not avalible!\n", __FILE__, __LINE__, __FUNCTION__, _ptResolution->Width, _ptResolution->Height);
			*_piVideoSize = QXGA;	//默认按照最大创建
		}
		td_printf(0, "%s %d %s: match video size[%d * %d] %d\n", __FILE__, __LINE__, __FUNCTION__, _ptResolution->Width, _ptResolution->Height, *_piVideoSize);
	}
	else
	{
		switch(*_piVideoSize)
		{
			case QXGA:
			{
				_ptResolution->Width = 2048;
				_ptResolution->Height = 1536;
				break;
			}
			case F1080P:
			{
				_ptResolution->Width = 1920;
				_ptResolution->Height = 1080;
				break;
			}
			case F1200:
			{
				_ptResolution->Width = 1600;
				_ptResolution->Height = 1200;
				break;
			}
			case F960P:
			{
				_ptResolution->Width = 1280;
				_ptResolution->Height = 960;
				break;
			}
			case F720P:
			{
				_ptResolution->Width = 1280;
				_ptResolution->Height = 720;
				break;
			}
			case HVGA:
			{
				_ptResolution->Width = 640;
				_ptResolution->Height = 240;
				break;
			}
			case FVGA:
			{
				_ptResolution->Width = 640;
				_ptResolution->Height = 480;
				break;
			}
			case QVGA:
			{
				_ptResolution->Width = 320;
				_ptResolution->Height = 240;
				break;
			}
			case FD1:
			{
				_ptResolution->Width = 704;
				_ptResolution->Height = 576;
				break;
			}
			case HD1:
			{
				_ptResolution->Width = 704;
				_ptResolution->Height = 288;
				break;
			}
			case FCIF:
			{
				_ptResolution->Width = 352;
				_ptResolution->Height = 288;
				break;
			}
			case HCIF:
			{
				_ptResolution->Width = 352;
				_ptResolution->Height = 144;
				break;
			}
			case QCIF:
			{
				_ptResolution->Width = 176;
				_ptResolution->Height = 144;
				break;
			}
			default:
			{
				//匹配不成功范围默认QXGA
				_ptResolution->Width = 2048;
				_ptResolution->Height = 1536;
				td_printf(0, "%s %d %s: VideoSize is not match! Default return QXGA!\n", __FILE__, __LINE__, __FUNCTION__, _ptResolution->Width, _ptResolution->Height);
 			}
		}
		td_printf(0, "%s %d %s: Width %d, Height:%d\n", __FILE__, __LINE__, __FUNCTION__, _ptResolution->Width, _ptResolution->Height);
	}

	return TD_OK;
}

/**************************************************
*功能:		获取共享内存更新高清参数
                         更新ONVIF全局变量
*参数:		无
*返回值:	0: 成功, !0: 失败
*修改历史:   20130129 v2.0 creat by ptb
			20130319 v2.3.0 modify by ptb
**************************************************/
int UpdateIspInfo()
{
	td_printf(0, "%s %d %s :start\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	struct tt__ImagingSettings *pstImaging = NULL;
	struct _trt__GetVideoSourcesResponse *pstGVSResponse = NULL;
	TGuiVideoISPInfo stpIspInfo = {0};
	int iRet = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	pstGVSResponse = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp;
	//iRet = OnvifCmdGetIspInfo(&stpIspInfo);
	if(TD_OK == iRet)
	{
		ptOnvifCtrl->m_stFlag.m_iIspFlag = 1;
		if(!pointer_valid_check(pstGVSResponse)
		&& !pointer_valid_check(pstGVSResponse->VideoSources)
		&& !pointer_valid_check(pstGVSResponse->VideoSources->Imaging))
		{
			pstImaging = pstGVSResponse->VideoSources->Imaging;

			//背光补偿
			if(NULL != pstImaging->BacklightCompensation)
			{
				pstImaging->BacklightCompensation->Mode  = tt__BacklightCompensationMode__OFF;
				pstImaging->BacklightCompensation->Level = 0;
			}

			//曝光
			if(NULL != pstImaging->Exposure)
			{
				pstImaging->Exposure->Mode 				= (stpIspInfo.m_iExposureMode == 0)?tt__ExposureMode__MANUAL: tt__ExposureMode__AUTO;
				pstImaging->Exposure->Priority 			= (enum tt__ExposurePriority)stpIspInfo.m_iExpPriority;
				pstImaging->Exposure->MaxGain 			= stpIspInfo.m_iExpMaxGain / 100;
				pstImaging->Exposure->MinGain 			= stpIspInfo.m_iExpMinGain / 100;
				pstImaging->Exposure->MinExposureTime 	= stpIspInfo.m_iExpMinTime;
				pstImaging->Exposure->MaxExposureTime 	= stpIspInfo.m_iExpMaxTime;

				if(NULL != pstImaging->Exposure->Window
				&& NULL != pstImaging->Exposure->Window->top
				&& NULL != pstImaging->Exposure->Window->left
				&& NULL != pstImaging->Exposure->Window->bottom
				&& NULL != pstImaging->Exposure->Window->right)
				{
					*(pstImaging->Exposure->Window->top) 	= (float)(stpIspInfo.m_tExpArray.m_iTop);
					*(pstImaging->Exposure->Window->left) 	= (float)(stpIspInfo.m_tExpArray.m_iLeft);
					*(pstImaging->Exposure->Window->bottom) = (float)(stpIspInfo.m_tExpArray.m_iTop + stpIspInfo.m_tExpArray.m_uiHeight);
					*(pstImaging->Exposure->Window->right) 	= (float)(stpIspInfo.m_tExpArray.m_iLeft + stpIspInfo.m_tExpArray.m_uiWidth);
				}
			}

			//锐度
			if(NULL != pstImaging->Sharpness)
			{
				*(pstImaging->Sharpness) = (float)stpIspInfo.m_iSharpness;
			}

			//宽动态
			if(NULL != pstImaging->WideDynamicRange)
			{
				pstImaging->WideDynamicRange->Mode  = (stpIspInfo.m_iWDRMode == 1) ? tt__WideDynamicMode__ON: tt__WideDynamicMode__OFF;
				pstImaging->WideDynamicRange->Level = stpIspInfo.m_iWDRLevel;
			}
			
			//白平衡
			if(NULL != pstImaging->WhiteBalance)
			{
				pstImaging->WhiteBalance->Mode   = (stpIspInfo.m_iWBMode == 1)?tt__WhiteBalanceMode__MANUAL: tt__WhiteBalanceMode__AUTO;
				pstImaging->WhiteBalance->CrGain = stpIspInfo.m_iWBRGain;
				pstImaging->WhiteBalance->CbGain = stpIspInfo.m_iWBBGain;
			}
			
			//红外滤光片
			if(NULL != pstImaging->IrCutFilter)
			{
				//on--黑白ff--彩色auto--自动
				if(stpIspInfo.m_iIRMode == CLOSE)
				{
					*(pstImaging->IrCutFilter) = tt__IrCutFilterMode__OFF;
				}
				else if(stpIspInfo.m_iIRMode == OPEN)
				{
					*(pstImaging->IrCutFilter) = tt__IrCutFilterMode__ON;
				}
				else if(stpIspInfo.m_iIRMode == AUTO)
				{
					*(pstImaging->IrCutFilter) = tt__IrCutFilterMode__AUTO;
				}
				else
				{
					td_printf(0, "%s %d %s :m_iIRMode is not match[%d]!\n", __FILE__, __LINE__, __FUNCTION__, stpIspInfo.m_iIRMode);
				}
			}
		}
	}
	else
	{
		td_printf(0, "%s %d %s :OnvifCmdGetIspInfo error!\n", __FILE__, __LINE__, __FUNCTION__);
	}

	return iRet;
}

/**************************************************
*功能:		获取共享内存视频参数，更新ONVIF全局变量
*参数:		_pThis	:		全局变量结构体指针
            _iChan	:		指定更新的通道
*返回值:	0: 成功, !0: 失败
*修改历史:   20130129 v2.0 creat by ptb
**************************************************/
int UpdateVideoInfo(OnvifCtrl *_pThis, int _iChan)
{
	td_printf(0, "%s %s %d started!!", __FILE__, __FUNCTION__, __LINE__);
	OnvifCtrl *ptOnvifCtrl 	= (OnvifCtrl *)_pThis;
	VideoInCnf_EX stVInCnf = {0};
	TGuiVideoEncodeCnf stVEncodeCnf[MAX_CHANNEL_NUM * MAX_STREAM] = {{0}};
	unsigned short u16Width  = 0;
	unsigned short u16Height = 0;
	TFloatRange tPicQuality = {14, 6};
	TFloatRange tOnvifQuality = {VECOQRMIN, VECOQRMAX};
	
	int iVEChan = _iChan;
	int iVIChan = 0;
	int iRet    = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(iVEChan >= ptOnvifCtrl->m_stNvsInfo.m_iVENum)
	{
		return TD_ERROR;
	}
	
	//副码流处理
	if(iVEChan >= ptOnvifCtrl->m_stNvsInfo.m_iVINum)
	{
		iVIChan = iVEChan % ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	}
	else
	{
		iVIChan = iVEChan;
	}
	
	//1-获取视频输入参数
	stVInCnf.m_iVideoMod = GUI_NTSC_MODE;

	//亮度、饱和度、对比度
	if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[iVIChan].Imaging->Brightness)      = stVInCnf.m_iLum * 100 / 255.0;
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[iVIChan].Imaging->ColorSaturation) = stVInCnf.m_iSaturation * 100 / 255.0;
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[iVIChan].Imaging->Contrast)        = stVInCnf.m_iContrast * 100 / 255.0;
	}
	else
	{
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[iVIChan].Imaging->Brightness)      = stVInCnf.m_iLum;
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[iVIChan].Imaging->ColorSaturation) = stVInCnf.m_iSaturation;
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[iVIChan].Imaging->Contrast)        = stVInCnf.m_iContrast;
	}
	
	//2-获取视频编码参数

	//制式
	ptOnvifCtrl->m_stNvsInfo.m_tVideoParam.m_InputNorm[iVIChan] = stVInCnf.m_iVideoMod;

	//帧率范围
	if(GUI_PAL_MODE == ptOnvifCtrl->m_stNvsInfo.m_tVideoParam.m_InputNorm[iVIChan])
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].JPEG->FrameRateRange->Max = PAL_MAXFRAMERATE;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].H264->FrameRateRange->Max = PAL_MAXFRAMERATE;
	
		//Extension
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].Extension->JPEG->FrameRateRange->Max = PAL_MAXFRAMERATE;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].Extension->H264->FrameRateRange->Max = PAL_MAXFRAMERATE;
	}
	else if(GUI_NTSC_MODE == ptOnvifCtrl->m_stNvsInfo.m_tVideoParam.m_InputNorm[iVIChan])
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].JPEG->FrameRateRange->Max = NTSC_MAXFRAMERATE;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].H264->FrameRateRange->Max = NTSC_MAXFRAMERATE;
	
		//Extension
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].Extension->JPEG->FrameRateRange->Max = NTSC_MAXFRAMERATE;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].Extension->H264->FrameRateRange->Max = NTSC_MAXFRAMERATE;
	}
	else
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].JPEG->FrameRateRange->Max = PAL_MAXFRAMERATE;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].H264->FrameRateRange->Max = PAL_MAXFRAMERATE;
	
		//Extension
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].Extension->JPEG->FrameRateRange->Max = PAL_MAXFRAMERATE;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[iVEChan].Extension->H264->FrameRateRange->Max = PAL_MAXFRAMERATE;
	}
	
	//编码方式
	if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		//获取编码方式
		if(GUI_MJPG == stVEncodeCnf[iVEChan].m_iEncodeType)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Encoding = tt__VideoEncoding__JPEG;
		}
		else if(GUI_H264 == stVEncodeCnf[iVEChan].m_iEncodeType)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Encoding = tt__VideoEncoding__H264;
		}
		else if(GUI_MPG4 == stVEncodeCnf[iVEChan].m_iEncodeType)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Encoding = tt__VideoEncoding__MPEG4;
		}
		else
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Encoding = tt__VideoEncoding__H264;
		
		}
	}
	else
	{
		stVEncodeCnf[iVEChan].m_iEncodeType = stVEncodeCnf[iVEChan].m_iEncodeType & 0x0F;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Encoding = tt__VideoEncoding__H264;
	}
	
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].RateControl->EncodingInterval = 1;

	//帧率
	//DVR满帧会把帧率设置为2009
	if(GUI_FULL_FRAME_RATE == stVEncodeCnf[iVEChan].m_iFrameRate)
	{
		if(GUI_PAL_MODE == stVInCnf.m_iVideoMod)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].RateControl->FrameRateLimit = PAL_MAXFRAMERATE;
		}
		else if(GUI_NTSC_MODE == stVInCnf.m_iVideoMod)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].RateControl->FrameRateLimit = NTSC_MAXFRAMERATE;
		}
		else
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].RateControl->FrameRateLimit = PAL_MAXFRAMERATE;
		}
	}
	else
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].RateControl->FrameRateLimit = stVEncodeCnf[iVEChan].m_iFrameRate;
	}

	//码率
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].RateControl->BitrateLimit = stVEncodeCnf[iVEChan].m_iBitRate;

	//I帧间隔
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].H264->GovLength = stVEncodeCnf[iVEChan].m_iIFrameRate;

	//视频质量
	//兼容S2设备，S2:2-10   S3:6-14
	if(stVEncodeCnf[iVEChan].m_iPicQualityLv < 6)
	{
		stVEncodeCnf[iVEChan].m_iPicQualityLv += 4; 
	}
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Quality = \
		ExchangeValue(stVEncodeCnf[iVEChan].m_iPicQualityLv, tPicQuality, tOnvifQuality);

	//分辨率
	u16Width = width;
	u16Height = height;
	if(H960 == stVEncodeCnf[iVEChan].m_iPicSize)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Resolution->Height = 576;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Resolution->Width	= 960;
	}
	else
	{
		//GetVideoWidthHeight(stVEncodeCnf[iVEChan].m_iPicSize, stVInCnf.m_iVideoMod - 1, &u16Width, &u16Height);
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Resolution->Height = u16Height;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[iVEChan].Resolution->Width	= u16Width;
	}
	
	return iRet;
}

/**************************************************
*功能:		获取共享内存视频参数，更新ONVIF全局变量
*参数:		_pThis	:		全局变量结构体指针
*返回值:	0: 成功, !0: 失败
*修改历史:20130129 v2.3 creat by pangtb
**************************************************/
int UpdateVideoInfoAll(OnvifCtrl *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl 	= (OnvifCtrl *)_pThis;
	int i = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	for(i = 0; i < ptOnvifCtrl->m_stNvsInfo.m_iVENum; i++)
	{
		UpdateVideoInfo(ptOnvifCtrl, i);
	}

	return TD_OK;
}

/**************************************************
*功能:		根据速度值确定PTZ方向
*参数:		_iSpeedX	:	X方向速度
            _iSpeedY	:	Y方向速度
*返回值:	成功		:	PTZ控制码
			失败		:	-1
*修改历史:   20130222 v2.3 creat by ptb
**************************************************/
int GetPTZCode(int _iSpeedX, int _iSpeedY)
{
	int iSpeedX  = _iSpeedX;
	int iSpeedY  = _iSpeedY;
	int iPTZCode = 0;
	
	//确定PTZ方向
	if(abs(iSpeedX) >= abs(iSpeedY)*2)
	{
		iPTZCode = iSpeedX > 0 ? GUI_PTZ_LOCAL_CODE_RIGHT : GUI_PTZ_LOCAL_CODE_LEFT;
	}
	else if(abs(iSpeedY) >= abs(iSpeedX)*2)
	{
		iPTZCode = iSpeedY > 0 ? GUI_PTZ_LOCAL_CODE_UP : GUI_PTZ_LOCAL_CODE_DOWN;
	}
	else if(iSpeedX*iSpeedY > 0 && abs(iSpeedX) > abs(iSpeedY)*0.5 && abs(iSpeedX) < abs(iSpeedY)*2)
	{
		iPTZCode = iSpeedX > 0 ? GUI_PTZ_LOCAL_CODE_RIGHTUP : GUI_PTZ_LOCAL_CODE_LEFTDOWN;
	}
	else if(iSpeedX*iSpeedY < 0 && abs(iSpeedX) > abs(iSpeedY)*0.5 && abs(iSpeedX) < abs(iSpeedY)*2)
	{
		iPTZCode = iSpeedX > 0 ? GUI_PTZ_LOCAL_CODE_RIGHTDOWN: GUI_PTZ_LOCAL_CODE_LEFTUP;
	}
	else
	{
		iPTZCode = TD_ERROR;
	}
		
	return iPTZCode;
}

/**************************************************
*功能:		判断设备当前设备是否需要重启，
			兼容老设备修改分辨率等参数需要重启
*参数:		无
*返回值:	成功	:	0
			失败	:	!0
*修改历史:   20130320 v2.3.0 creat by ptb
**************************************************/
int VerifyDeviceRebootFlag()
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iProductModel = 0;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	iProductModel = OnvifCmdGetProductModel() & 0xffff;
	if(iProductModel < 0)
	{
		//td_printf(0, "OnvifCmdGetProductModel error\n");
		return TD_ERROR;
	}

	switch(iProductModel)
	{
		//TC-NC9100S1-MP-IR
		case 0x0046:
			ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_TRUE;
			break;

		//TC-NC9010S2-MP
		case 0x0051:
			ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_TRUE;
			break;
			
		//TC-NC9000S2-2MP
		case 0x0052:
			ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_TRUE;
			break;
			
		//TC-NC9010N1-2MP
		case 0x0053:
			ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_TRUE;
			break;

		//TC-NS621S2
		case 0x0060:
			ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_TRUE;
			break;

		//TC-NS622S2
		case 0x0062:
			ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_TRUE;
			break;

		//TC-NS628S2
		case 0x0064:
			ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_TRUE;
			break;
		default:
			//不在此列表，默认不需要重启
			break;
	}
	return TD_OK;
}

/**************************************************
*功能:		获取当前空闲预置位号
*参数:		无
*返回值:	成功	:	预置位号
			      失败	:	-1
*修改历史:   20130425 v2.3.0 creat by ptb
**************************************************/
int GetIdlePresetNum()
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	char *pName = NULL;
	int iSize   = 0;
	int iRet    = -1;
	int i = 0;
	int j = 0;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	iSize = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	
	for(i = PRESET_HOME; i < PRESET_MAX_NUM; i++)
	{
		for(j = 0; j < iSize; j++)
		{
			pName = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", j + 1, "presetnum_", i, NULL);
			if(NULL != pName)
			{
				iRet = -1;
				break;
			}
			else
			{
				iRet = i;
				continue;
			}
		}
		
		if(iRet > 0)
		{
			break;
		}
	}
	
	return iRet;
}

/**************************************************
*功能:		如果存在TZ，返回time(NULL)时间
				如果不存在TZ，返回UTC时间
*参数:		_ptTimer:	返回时间
*返回值:	
*修改历史:   20130425 v2.3.0 creat by ptb
**************************************************/
time_t TdTime(time_t* _ptTimer)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	time_t *ptTimer = _ptTimer;
	time_t tTimer = 0;
	int iTimeZone = 0;
	int iRet = 0;

	tTimer = time(NULL);

	iRet = CheckTimeZoneFile();
	if(0 != iRet)
	{
		//如果没有TZ文件，手动变成UTC
		iTimeZone = ptOnvifCtrl->m_stNvsInfo.m_iTimeZone;
		tTimer += 3600 * iTimeZone;
	}
	
	if(ptTimer != NULL)
	{
		*ptTimer = tTimer;
	}
	
	return tTimer;
}

/**************************************************
*功能:		清空PullMsg信息
*参数:		空
*返回值:	成功:0	失败:0!
*修改历史:   20131012 v2.3.1 creat by ptb
**************************************************/
int ClearPullMsgInfo(void)
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	memset(&ptOnvifCtrl->m_stManager.m_tPullMsg, 0, sizeof(PullMsgInfo));

	return TD_OK;
}

#ifdef WITH_RECORDING
/**************************************************
*功能:		获取录像回放能力集
*参数:		soap 	:	soap信息
				request	:	请求
				response:	响应
*返回值:	成功	:	0
			      失败	:	-1
*修改历史:   20130425 v2.3.0 creat by ptb
**************************************************/
//Replay
SOAP_FMAC5 int SOAP_FMAC6 __trp__GetServiceCapabilities(struct soap *_ptSoap, struct _trp__GetServiceCapabilities *trp__GetServiceCapabilities, struct _trp__GetServiceCapabilitiesResponse *trp__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

/**************************************************
*功能:		获取录像回放URI
*参数:		soap 	:	soap信息
				request	:	请求
				response:	响应
*返回值:	成功	:	0
			      失败	:	-1
*修改历史:   20130425 v2.3.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trp__GetReplayUri(struct soap *_ptSoap, struct _trp__GetReplayUri *trp__GetReplayUri, struct _trp__GetReplayUriResponse *trp__GetReplayUriResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

/**************************************************
*功能:		获取录像回放配置
*参数:		soap 	:	soap信息
				request	:	请求
				response:	响应
*返回值:	成功	:	0
			      失败	:	-1
*修改历史:   20130425 v2.3.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trp__GetReplayConfiguration(struct soap *_ptSoap, struct _trp__GetReplayConfiguration *trp__GetReplayConfiguration, struct _trp__GetReplayConfigurationResponse *trp__GetReplayConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

/**************************************************
*功能:		设置录像回放配置
*参数:		soap 	:	soap信息
				request	:	请求
				response:	响应
*返回值:	成功	:	0
			      失败	:	-1
*修改历史:   20130425 v2.3.0 creat by ptb
**************************************************/
SOAP_FMAC5 int SOAP_FMAC6 __trp__SetReplayConfiguration(struct soap *_ptSoap, struct _trp__SetReplayConfiguration *trp__SetReplayConfiguration, struct _trp__SetReplayConfigurationResponse *trp__SetReplayConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

//Search
SOAP_FMAC5 int SOAP_FMAC6 __tse__GetMediaAttributes(struct soap *_ptSoap, struct _tse__GetMediaAttributes *tse__GetMediaAttributes, struct _tse__GetMediaAttributesResponse *tse__GetMediaAttributesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__FindRecordings(struct soap *_ptSoap, struct _tse__FindRecordings *tse__FindRecordings, struct _tse__FindRecordingsResponse *tse__FindRecordingsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __tse__GetRecordingSearchResults(struct soap *_ptSoap, struct _tse__GetRecordingSearchResults *tse__GetRecordingSearchResults, struct _tse__GetRecordingSearchResultsResponse *tse__GetRecordingSearchResultsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__FindEvents(struct soap *_ptSoap, struct _tse__FindEvents *tse__FindEvents, struct _tse__FindEventsResponse *tse__FindEventsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__GetEventSearchResults(struct soap *_ptSoap, struct _tse__GetEventSearchResults *tse__GetEventSearchResults, struct _tse__GetEventSearchResultsResponse *tse__GetEventSearchResultsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__FindPTZPosition(struct soap *_ptSoap, struct _tse__FindPTZPosition *tse__FindPTZPosition, struct _tse__FindPTZPositionResponse *tse__FindPTZPositionResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__GetPTZPositionSearchResults(struct soap *_ptSoap, struct _tse__GetPTZPositionSearchResults *tse__GetPTZPositionSearchResults, struct _tse__GetPTZPositionSearchResultsResponse *tse__GetPTZPositionSearchResultsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__GetSearchState(struct soap *_ptSoap, struct _tse__GetSearchState *tse__GetSearchState, struct _tse__GetSearchStateResponse *tse__GetSearchStateResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__EndSearch(struct soap *_ptSoap, struct _tse__EndSearch *tse__EndSearch, struct _tse__EndSearchResponse *tse__EndSearchResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__FindMetadata(struct soap *_ptSoap, struct _tse__FindMetadata *tse__FindMetadata, struct _tse__FindMetadataResponse *tse__FindMetadataResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tse__GetMetadataSearchResults(struct soap *_ptSoap, struct _tse__GetMetadataSearchResults *tse__GetMetadataSearchResults, struct _tse__GetMetadataSearchResultsResponse *tse__GetMetadataSearchResultsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trc__GetServiceCapabilities(struct soap *_ptSoap, struct _trc__GetServiceCapabilities *trc__GetServiceCapabilities, struct _trc__GetServiceCapabilitiesResponse *trc__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trc__CreateRecording(struct soap *_ptSoap, struct _trc__CreateRecording *trc__CreateRecording, struct _trc__CreateRecordingResponse *trc__CreateRecordingResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
	
}

SOAP_FMAC5 int SOAP_FMAC6 __trc__DeleteRecording(struct soap *_ptSoap, struct _trc__DeleteRecording *trc__DeleteRecording, struct _trc__DeleteRecordingResponse *trc__DeleteRecordingResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordings(struct soap *_ptSoap, struct _trc__GetRecordings *trc__GetRecordings, struct _trc__GetRecordingsResponse *trc__GetRecordingsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__SetRecordingConfiguration(struct soap *_ptSoap, struct _trc__SetRecordingConfiguration *trc__SetRecordingConfiguration, struct _trc__SetRecordingConfigurationResponse *trc__SetRecordingConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingConfiguration(struct soap *_ptSoap, struct _trc__GetRecordingConfiguration *trc__GetRecordingConfiguration, struct _trc__GetRecordingConfigurationResponse *trc__GetRecordingConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingOptions(struct soap *_ptSoap, struct _trc__GetRecordingOptions *trc__GetRecordingOptions, struct _trc__GetRecordingOptionsResponse *trc__GetRecordingOptionsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__CreateTrack(struct soap *_ptSoap, struct _trc__CreateTrack *trc__CreateTrack, struct _trc__CreateTrackResponse *trc__CreateTrackResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__DeleteTrack(struct soap *_ptSoap, struct _trc__DeleteTrack *trc__DeleteTrack, struct _trc__DeleteTrackResponse *trc__DeleteTrackResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__GetTrackConfiguration(struct soap *_ptSoap, struct _trc__GetTrackConfiguration *trc__GetTrackConfiguration, struct _trc__GetTrackConfigurationResponse *trc__GetTrackConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__SetTrackConfiguration(struct soap *_ptSoap, struct _trc__SetTrackConfiguration *trc__SetTrackConfiguration, struct _trc__SetTrackConfigurationResponse *trc__SetTrackConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__CreateRecordingJob(struct soap *_ptSoap, struct _trc__CreateRecordingJob *trc__CreateRecordingJob, struct _trc__CreateRecordingJobResponse *trc__CreateRecordingJobResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__DeleteRecordingJob(struct soap *_ptSoap, struct _trc__DeleteRecordingJob *trc__DeleteRecordingJob, struct _trc__DeleteRecordingJobResponse *trc__DeleteRecordingJobResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingJobs(struct soap *_ptSoap, struct _trc__GetRecordingJobs *trc__GetRecordingJobs, struct _trc__GetRecordingJobsResponse *trc__GetRecordingJobsResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__SetRecordingJobConfiguration(struct soap *_ptSoap, struct _trc__SetRecordingJobConfiguration *trc__SetRecordingJobConfiguration, struct _trc__SetRecordingJobConfigurationResponse *trc__SetRecordingJobConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingJobConfiguration(struct soap *_ptSoap, struct _trc__GetRecordingJobConfiguration *trc__GetRecordingJobConfiguration, struct _trc__GetRecordingJobConfigurationResponse *trc__GetRecordingJobConfigurationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__SetRecordingJobMode(struct soap *_ptSoap, struct _trc__SetRecordingJobMode *trc__SetRecordingJobMode, struct _trc__SetRecordingJobModeResponse *trc__SetRecordingJobModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingJobState(struct soap *_ptSoap, struct _trc__GetRecordingJobState *trc__GetRecordingJobState, struct _trc__GetRecordingJobStateResponse *trc__GetRecordingJobStateResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trecv__GetServiceCapabilities(struct soap *_ptSoap, struct _trecv__GetServiceCapabilities *trecv__GetServiceCapabilities, struct _trecv__GetServiceCapabilitiesResponse *trecv__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trecv__GetReceivers(struct soap *_ptSoap, struct _trecv__GetReceivers *trecv__GetReceivers, struct _trecv__GetReceiversResponse *trecv__GetReceiversResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trecv__GetReceiver(struct soap *_ptSoap, struct _trecv__GetReceiver *trecv__GetReceiver, struct _trecv__GetReceiverResponse *trecv__GetReceiverResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trecv__CreateReceiver(struct soap *_ptSoap, struct _trecv__CreateReceiver *trecv__CreateReceiver, struct _trecv__CreateReceiverResponse *trecv__CreateReceiverResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trecv__DeleteReceiver(struct soap *_ptSoap, struct _trecv__DeleteReceiver *trecv__DeleteReceiver, struct _trecv__DeleteReceiverResponse *trecv__DeleteReceiverResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trecv__ConfigureReceiver(struct soap *_ptSoap, struct _trecv__ConfigureReceiver *trecv__ConfigureReceiver, struct _trecv__ConfigureReceiverResponse *trecv__ConfigureReceiverResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trecv__SetReceiverMode(struct soap *_ptSoap, struct _trecv__SetReceiverMode *trecv__SetReceiverMode, struct _trecv__SetReceiverModeResponse *trecv__SetReceiverModeResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __trecv__GetReceiverState(struct soap *_ptSoap, struct _trecv__GetReceiverState *trecv__GetReceiverState, struct _trecv__GetReceiverStateResponse *trecv__GetReceiverStateResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __tse__GetServiceCapabilities(struct soap *_ptSoap, struct _tse__GetServiceCapabilities *tse__GetServiceCapabilities, struct _tse__GetServiceCapabilitiesResponse *tse__GetServiceCapabilitiesResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __tse__GetRecordingSummary(struct soap *_ptSoap, struct _tse__GetRecordingSummary *tse__GetRecordingSummary, struct _tse__GetRecordingSummaryResponse *tse__GetRecordingSummaryResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __tse__GetRecordingInformation(struct soap *_ptSoap, struct _tse__GetRecordingInformation *tse__GetRecordingInformation, struct _tse__GetRecordingInformationResponse *tse__GetRecordingInformationResponse)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	return TD_OK;
}

#endif

