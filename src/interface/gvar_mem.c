/*
 *	file: gvar_mem.c
 *	description: gloable variable memory init and uninit
 *	history: dongliqiang created at 20120227
 */

#include <stdlib.h>
#include "common.h"
#include "gvar_mem.h"
#include "proparser.h"
#include "iniparser.h"
#include "onvif_sdk.h"
#include "onvif_shm_cmd.h"
#include "onvifStub.h"
#include "macro.h"

/**************************************************
*功能:		检测临时分配的内存pTmpMem是否分配成功
*参数:		_pTmpMem:		临时内存指针
			_iNum:			内存指针个数
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120228 v1.0 creat by pangtb
	2013.12.19 modified by qiaohaijun
**************************************************/
int CheckMem(void **_ppTmpMem, int _iNum, int _iMemSize)
{
	int i = 0;

	if(_iNum > _iMemSize)
	{
		return TD_ERROR;
	}

	for(i = 0; i < _iNum; i++)
	{
		if(NULL == _ppTmpMem[i])
		{
			return TD_ERROR;
		}
	}

	return TD_OK;	
}

/**************************************************
*功能:		检测临时分配的内存pTmpMem是否分配成功
*参数:		_ppTmpMem:		内存地址
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120228 v1.0 creat by pangtb
**************************************************/
int FreeAllTmpMem(void **_ppTmpMem, int _iNum)
{
	int i = 0;
	
	for(i = 0; i < _iNum; i++)
	{
		if(NULL != _ppTmpMem[i])
		{
			free(_ppTmpMem[i]);
		}
	}

	return TD_OK;
}

int GMemInit__ns2__Probe(void *_pThis)
{
	struct d__ProbeMatchType *ptProbeMatch = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iSizeScope = 0;
	int iScopeLen = 0;
	int iTotalLen = 0;
	int iProfileNum = 1;
	int iNum  = 0;
	int i     = 0;
		
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	iSizeScope = ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes;
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp))
	{
		//malloc memeory
		ppTmpMem[iNum++] = calloc(1, sizeof(struct d__ProbeMatchesType));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct d__ProbeMatchType));

		//Address
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);

		//Types
		ppTmpMem[iNum++] = calloc(1, sizeof(char **));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);

		//Scopes
		ppTmpMem[iNum++] = calloc(1, sizeof(struct d__ScopesType));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MAX);

		//XAddrs
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return -1;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp             = (struct d__ProbeMatchesType *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch = (struct d__ProbeMatchType *)ppTmpMem[iNum++];

		ptProbeMatch = ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->ProbeMatch;

		//Address
		ptProbeMatch->wsa__EndpointReference.Address = (char *)ppTmpMem[iNum++];

		//Types
		ptProbeMatch->Types	   = (char **)ppTmpMem[iNum++];
		*(ptProbeMatch->Types) = (char *)ppTmpMem[iNum++];

		//Scopes
		ptProbeMatch->Scopes		 = (struct d__ScopesType *)ppTmpMem[iNum++];
		ptProbeMatch->Scopes->__item = (char *)ppTmpMem[iNum++];

		//XAddrs
		ptProbeMatch->XAddrs = (char *)ppTmpMem[iNum++];

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptProbeRsp->__sizeProbeMatch = iProfileNum;
		//Address
		memcpy(ptProbeMatch->wsa__EndpointReference.Address, (char*)ptOnvifCtrl->m_stNvsInfo.m_cUUID, MAX_UUID_LEN);		
		
		//Types
		strncpy(*ptProbeMatch->Types, "dn:NetworkVideoTransmitter", 26);

		//__item
		for(i = 0; i < iSizeScope; i++)
		{
			iScopeLen = strlen(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem);
			iTotalLen = strlen(ptProbeMatch->Scopes->__item);
			if((iTotalLen + iScopeLen) < MAX_STRING_LEN_MEM)
			{
				sprintf(ptProbeMatch->Scopes->__item, 
					"%s %s", 
					ptProbeMatch->Scopes->__item, 
					ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem);
			}
		}

		//td_printf(0, "========ProbeMatch Scope: %s\n", ptOnvifCtrl->m_stOnvifCfg.m_pdn__ProbeResponse->ProbeMatch->Scopes->__item);

		//XAddrs
		sprintf(ptProbeMatch->XAddrs, "http://%s:%d/onvif/device_service", ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort);

		//MetadataVersion
		ptProbeMatch->MetadataVersion = (enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "probematch_", i + 1, "pmt_metaversion", 0, 1);
	}

	return TD_OK;
}

int GMemInit__ns3__PullMessages(void *_pThis)
{	
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iMaxMsgNum = 0;
	int iNum = 0;
	int i = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp))
	{
		iMaxMsgNum = ptOnvifCtrl->m_stNvsInfo.m_iVINum * TOPIC_NUM;

		//malloc memeory
 		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tev__PullMessagesResponse));
 		ppTmpMem[iNum++] = calloc(iMaxMsgNum, sizeof(struct wsnt__NotificationMessageHolderType));

		for(i = 0; i < iMaxMsgNum; i++)
		{
			ppTmpMem[iNum++] = calloc(1, sizeof(struct wsnt__TopicExpressionType));	
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);	
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);

			//Message any
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MAX);	
		}

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}
		
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp = (struct _tev__PullMessagesResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp->wsnt__NotificationMessage = (struct wsnt__NotificationMessageHolderType *)ppTmpMem[iNum++];

		for(i = 0; i < iMaxMsgNum; i++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp->wsnt__NotificationMessage[i].Topic = (struct wsnt__TopicExpressionType *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp->wsnt__NotificationMessage[i].Topic->__any   = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp->wsnt__NotificationMessage[i].Topic->Dialect = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp->wsnt__NotificationMessage[i].Message.__any  = (char *)ppTmpMem[iNum++];
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptPullMsgRsp->wsnt__NotificationMessage[i].Topic->Dialect, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
		}

	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
int GMemInit__ns4__CreatePullPointSubscription(void *_pThis)
{	
	struct _tev__CreatePullPointSubscriptionResponse *ptCreatePPSRsp = NULL;
	void *ppTmpMem[MEM_LEN_MINI]  = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iNum = 0;
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptCreatePPSRsp))
	{
		//malloc memeory
 		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tev__CreatePullPointSubscriptionResponse));					
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);	

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return -1;
		}
		
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptCreatePPSRsp = (struct _tev__CreatePullPointSubscriptionResponse *)ppTmpMem[iNum++];
		ptCreatePPSRsp = ptOnvifCtrl->m_stOnvifCfg.m_ptCreatePPSRsp;
		ptCreatePPSRsp->SubscriptionReference.Address = (char *)ppTmpMem[iNum++];

		//全局变量初始化
		memset(ptCreatePPSRsp->SubscriptionReference.Address, 0, MEM_LEN_MID);
		sprintf(ptCreatePPSRsp->SubscriptionReference.Address, "http://%s:%d/events/sub_pull", \
			(char*)ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLEventPort);
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120305 v1.0 creat by ptb
20131101 v2.3.1 modify by ptb
**************************************************/
int GMemInit__ns4__GetEventProperties(void *_pThis)
{	
	struct _tev__GetEventPropertiesResponse *ptEventProperRsp = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	char *pcTmp  = NULL;
	char *pcTmp1 = NULL;
	char *pcTmp2 = NULL;
	int iTopicExpNum = 2;
	int iTopicNum = 2;
	int iNum = 0;
	int i = 0;

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptEventPropertiesRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tev__GetEventPropertiesResponse));

		//TopicNamespaceLocation
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);

		//wstop__TopicSet
		ppTmpMem[iNum++] = calloc(1, sizeof(struct wstop__TopicSetType));				
		//any
		ppTmpMem[iNum++] = calloc(iTopicNum, sizeof(char *));
		for(i = 0; i < iTopicNum; i++)
		{
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MAX);
		}

		//wsnt__TopicExpressionDialect
		ppTmpMem[iNum++] = calloc(iTopicExpNum, sizeof(char *));
		for(i = 0; i < iTopicExpNum; i++)
		{
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		}

		//MessageContentFilterDialect
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);

		//MessageContentSchemaLocation
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}
		
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptEventPropertiesRsp = (struct _tev__GetEventPropertiesResponse *)ppTmpMem[iNum++];
		ptEventProperRsp = ptOnvifCtrl->m_stOnvifCfg.m_ptEventPropertiesRsp;

		//TopicNamespaceLocation
		ptEventProperRsp->TopicNamespaceLocation  = (char **)ppTmpMem[iNum++];
		*ptEventProperRsp->TopicNamespaceLocation = (char *)ppTmpMem[iNum++];

		//wstop__TopicSet
		ptEventProperRsp->wstop__TopicSet = (struct wstop__TopicSetType *)ppTmpMem[iNum++];
		ptEventProperRsp->wstop__TopicSet->__any = (char **)ppTmpMem[iNum++];
		for(i = 0; i < iTopicNum; i++)
		{
			*(ptEventProperRsp->wstop__TopicSet->__any + i) = (char *)ppTmpMem[iNum++];
		}
				
		//wsnt__TopicExpressionDialect
		ptEventProperRsp->wsnt__TopicExpressionDialect = (char **)ppTmpMem[iNum++];
		for(i = 0; i < iTopicExpNum; i++)
		{
			*(ptEventProperRsp->wsnt__TopicExpressionDialect + i) = (char *)ppTmpMem[iNum++];
		}

		//MessageContentFilterDialect
		ptEventProperRsp->MessageContentFilterDialect  = (char **)ppTmpMem[iNum++];
		*ptEventProperRsp->MessageContentFilterDialect = (char *)ppTmpMem[iNum++];

		//MessageContentSchemaLocation
		ptEventProperRsp->MessageContentSchemaLocation  = (char **)ppTmpMem[iNum++];
		*ptEventProperRsp->MessageContentSchemaLocation = (char *)ppTmpMem[iNum++];

		//初始化
		//TopicNamespaceLocation
		ptEventProperRsp->__sizeTopicNamespaceLocation = 1;
		pcTmp = "http://www.onvif.org/onvif/ver10/topics/topicns.xml";
		Strcpy(*ptEventProperRsp->TopicNamespaceLocation, pcTmp, MEM_LEN_MIN);
		
		ptEventProperRsp->wsnt__FixedTopicSet = xsd__boolean__true_;

		//wstop__TopicSet
		ptEventProperRsp->wstop__TopicSet->__size = iTopicNum;
		
		//pcTmp = "tns1:Device/Trigger/tnshik:AlarmIn";
		//pcTmp = "tns1:VideoSource/MotionAlarm";
		//pcTmp = "tns1:UserAlarm/VideoLost";
		//pcTmp = "tns1:VideoAnalytics/tnshik:MaskAlarm";
		//strcpy(pcTopicSet[0], "<tns1:UserAlarm wstop:topic=\"true\"><tns1:VideoMove wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription/>Name=\"VideoSourceConfigurationToken\" Type=\"tt:ReferenceToken\"</tt:Source><tt:Key><tt:SimpleItemDescription Name=\"VideoInNum\" Type=\"xs:integer\"/></tt:Key> <tt:Data><tt:SimpleItemDescription/>Name=\"AlarmStatus\" Type=\"xs:boolean\"</tt:Data></tt:MessageDescription></tns1:VideoMove></tns1:UserAlarm>");
		//strcpy(pcTopicSet[1], "<tns1:UserAlarm wstop:topic=\"true\"><tns1:VideoCover wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription/>Name=\"VideoSourceConfigurationToken\" Type=\"tt:ReferenceToken\"</tt:Source><tt:Key><tt:SimpleItemDescription Name=\"VideoInNum\" Type=\"xs:integer\"/></tt:Key> <tt:Data><tt:SimpleItemDescription/>Name=\"AlarmStatus\" Type=\"xs:boolean\"</tt:Data></tt:MessageDescription></tns1:VideoCover></tns1:UserAlarm>");
		//strcpy(pcTopicSet[2], "<tns1:UserAlarm wstop:topic=\"true\"><tns1:VideoLost wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription/>Name=\"VideoSourceConfigurationToken\" Type=\"tt:ReferenceToken\"</tt:Source><tt:Key><tt:SimpleItemDescription Name=\"VideoInNum\" Type=\"xs:integer\"/></tt:Key> <tt:Data><tt:SimpleItemDescription/>Name=\"AlarmStatus\" Type=\"xs:boolean\"</tt:Data></tt:MessageDescription></tns1:VideoLost></tns1:UserAlarm>");
		//strcpy(pcTopicSet[3], "<tns1:UserAlarm wstop:topic=\"true\"><tns1:PortInAlarm wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription/>Name=\"PortInNum\" Type=\"xs:integer\"</tt:Source><tt:Key><tt:SimpleItemDescription Name=\"PortInNum\" Type=\"xs:integer\"/></tt:Key> <tt:Data><tt:SimpleItemDescription/>Name=\"AlarmStatus\" Type=\"xs:boolean\"</tt:Data></tt:MessageDescription></tns1:PortInAlarm></tns1:UserAlarm>");

		//pcTmp = "<tns1:UserAlarm wstop:topic=\"true\"><tns1:MotionAlarm wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription/>Name=\"VideoSourceConfigurationToken\" Value=\"tt:ReferenceToken\"</tt:Source><tt:Data><tt:SimpleItemDescription/>Name=\"AlarmStatus\" Type=\"xs:boolean\"</tt:Data></tt:MessageDescription></tns1:MotionAlarm></tns1:UserAlarm>";

		//pcTmp1 = "<tns1:MediaControl wstop:topic=\"true\"><tns1:VideoSourceConfiguration wstop:topic=\"true\"><tt:MessageDescription IsProperty=\"true\"><tt:Source><tt:SimpleItemDescription Name=\"VideoSourceConfigurationToken\" Type=\"tt:ReferenceToken\" /></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"Config\" Type=\"tt:VideoSourceConfiguration\" /></tt:Data></tt:MessageDescription></tns1:VideoSourceConfiguration><tns1:VideoEncoderConfiguration wstop:topic=\"true\"><tt:MessageDescription IsProperty=\"true\"><tt:Source><tt:SimpleItemDescription Name=\"VideoEncoderConfigurationToken\" Type=\"tt:ReferenceToken\" /></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"Config\" Type=\"tt:VideoEncoderConfiguration\" /></tt:Data></tt:MessageDescription></tns1:VideoEncoderConfiguration></tns1:MediaControl>";
		//解决Avigilon平台报警问题修改20131206
		pcTmp1 = "<tns1:VideoSource wstop:topic=\"true\"><SignalLoss wstop:topic=\"true\"><tt:MessageDescription IsProperty=\"true\"><tt:Source><tt:SimpleItemDescription Name=\"Source\" Type=\"tt:ReferenceToken\" /></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"State\" Type=\"xs:boolean\" /></tt:Data></tt:MessageDescription></SignalLoss><MotionAlarm wstop:topic=\"true\"><tt:MessageDescription IsProperty=\"true\"><tt:Source><tt:SimpleItemDescription Name=\"Source\" Type=\"tt:ReferenceToken\" /></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"State\" Type=\"xs:boolean\" /></tt:Data></tt:MessageDescription></MotionAlarm><tns1:SignalStandardMismatch wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription Name=\"VideoSourceToken\" Type=\"tt:ReferenceToken\" /></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"State\" Type=\"xs:boolean\" /></tt:Data></tt:MessageDescription></tns1:SignalStandardMismatch></tns1:VideoSource>";
		pcTmp2 = "<tns1:RuleEngine wstop:topic=\"true\"><CellMotionDetector wstop:topic=\"true\"><Motion wstop:topic=\"true\"><tt:MessageDescription IsProperty=\"true\"><tt:Source><tt:SimpleItemDescription Name=\"VideoSourceConfigurationToken\" Type=\"tt:ReferenceToken\" /><tt:SimpleItemDescription Name=\"VideoAnalyticsConfigurationToken\" Type=\"tt:ReferenceToken\" /><tt:SimpleItemDescription Name=\"Rule\" Type=\"xs:string\" /></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"IsMotion\" Type=\"xs:boolean\" /></tt:Data></tt:MessageDescription></Motion></CellMotionDetector></tns1:RuleEngine>";
		//end
		//pcTmp4 = "<tns1:RecordingConfig wstop:topic=\"true\"><JobState wstop:topic=\"true\"><tt:MessageDescription IsProperty=\"true\"><tt:Source><tt:SimpleItemDescription Name=\"RecordingJobToken\" Type=\"tt:RecordingJobReference\" /></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"State\" Type=\"xs:String\" /><tt:ElementItemDescription Name=\"Information\" Type=\"tt:RecordingJobStateInformation\" /></tt:Data></tt:MessageDescription></JobState><RecordingJobConfiguration wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription Name=\"RecordingJobToken\" Type=\"tt:RecordingJobReference\" /></tt:Source><tt:Data><tt:ElementItemDescription Name=\"Configuration\" Type=\"tt:RecordingJobConfiguration\" /></tt:Data></tt:MessageDescription></RecordingJobConfiguration></tns1:RecordingConfig>";
		//pcTmp5 = "<tns1:VideoAnalytics wstop:topic=\"true\"><tns1:MaskAlarm wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription Name=\"VideoSourceToken\" Type=\"tt:ReferenceToken\" /></tt:Source><tt:Data><tt:SimpleItemDescription Name=\"State\" Type=\"xs:boolean\" /></tt:Data></tt:MessageDescription></tns1:MaskAlarm></tns1:VideoAnalytics>";
		//pcTmp6 = "<tns1:Device wstop:topic=\"true\"><tns1:HardwareFailure wstop:topic=\"true\"><tns1:HardDiskFull wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription Name=\"HardDiskNo\" Type=\"xs:int\" /></tt:Source></tt:MessageDescription></tns1:HardDiskFull><tns1:HardDiskUnformat wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription Name=\"HardDiskNo\" Type=\"xs:int\" /></tt:Source></tt:MessageDescription></tns1:HardDiskUnformat><tns1:HardDiskError wstop:topic=\"true\"><tt:MessageDescription><tt:Source><tt:SimpleItemDescription Name=\"HardDiskNo\" Type=\"xs:int\" /></tt:Source></tt:MessageDescription></tns1:HardDiskError></tns1:HardwareFailure><tns1:Network wstop:topic=\"true\"><tns1:EthernetBroken wstop:topic=\"true\" /><tns1:IPAddrConflict wstop:topic=\"true\" /></tns1:Network></tns1:Device>";
		//pcTmp7 = "<tns1:UserAlarm wstop:topic=\"true\"><tns1:IllegalAccess wstop:topic=\"true\" /></tns1:UserAlarm>";
		
		Strcpy(*ptEventProperRsp->wstop__TopicSet->__any, pcTmp1, MEM_LEN_MAX);
		Strcpy(*(ptEventProperRsp->wstop__TopicSet->__any + 1), pcTmp2, MEM_LEN_MAX);
		//Strcpy(*(ptEventProperRsp->wstop__TopicSet->__any + 2), pcTmp3, MEM_LEN_MAX);
		//Strcpy(*(ptEventProperRsp->wstop__TopicSet->__any + 3), pcTmp4, MEM_LEN_MAX);
		//Strcpy(*(ptEventProperRsp->wstop__TopicSet->__any + 4), pcTmp5, MEM_LEN_MAX);
		//Strcpy(*(ptEventProperRsp->wstop__TopicSet->__any + 5), pcTmp6, MEM_LEN_MAX);
		//Strcpy(*(ptEventProperRsp->wstop__TopicSet->__any + 6), pcTmp7, MEM_LEN_MAX);

		//wsnt__TopicExpressionDialect
		ptEventProperRsp->__sizeTopicExpressionDialect = iTopicExpNum;
		pcTmp = "http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete";
		Strcpy(*ptEventProperRsp->wsnt__TopicExpressionDialect, pcTmp, MEM_LEN_MIN);
		pcTmp = "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet";
		Strcpy(*(ptEventProperRsp->wsnt__TopicExpressionDialect + 1), pcTmp, MEM_LEN_MIN);

		//FilterDialect
		ptEventProperRsp->__sizeMessageContentFilterDialect = 1;
		pcTmp = "http://www.onvif.org/ver10/tev/messageContentFilter/ItemFilter";
		Strcpy(*ptEventProperRsp->MessageContentFilterDialect, pcTmp, MEM_LEN_MIN);

		//SchemaLocation
		ptEventProperRsp->__sizeMessageContentSchemaLocation = 1;
		pcTmp = "http://www.onvif.org/onvif/ver10/schema/onvif.xsd";
		Strcpy(*ptEventProperRsp->MessageContentSchemaLocation, pcTmp, MEM_LEN_MIN);
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120312 v1.0 creat by ptb
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__ns6__Subscribe(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MINI]  = {0};
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptSubscribeRsp))
	{
 		ppTmpMem[iNum++] = calloc(1, sizeof(struct _wsnt__SubscribeResponse));
		//Address
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);

		//wsnt__CurrentTime
		ppTmpMem[iNum++] = calloc(1, sizeof(time_t));

		//wsnt__TerminationTime
		ppTmpMem[iNum++] = calloc(1, sizeof(time_t));

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}
		
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptSubscribeRsp = (struct _wsnt__SubscribeResponse *)ppTmpMem[iNum++];
		//Address
		ptOnvifCtrl->m_stOnvifCfg.m_ptSubscribeRsp->SubscriptionReference.Address = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptSubscribeRsp->CurrentTime = (time_t *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptSubscribeRsp->TerminationTime = (time_t *)ppTmpMem[iNum++];
	}
		
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
int GMemInit__ns8__Notify(void *_pThis)
{	
	struct wsnt__NotificationMessageHolderType *pNotifyMsg = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MID] = {0};
	char *pcTmp = NULL;
	int iMsgNum = 0;
	int iNum = 0;
	int i = 0;
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptNotify))
	{
		//设置单次发送Notify消息的数量
		iMsgNum = TOPIC_NUM;
		
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _wsnt__Notify));	
		ppTmpMem[iNum++] = calloc(iMsgNum, sizeof(struct wsnt__NotificationMessageHolderType));	

		for(i = 0; i< iMsgNum; i++)
		{
			//SubscriptionReference
			ppTmpMem[iNum++] = calloc(1, sizeof(struct wsa5__EndpointReferenceType));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);

			//Topic
			ppTmpMem[iNum++] = calloc(1, sizeof(struct wsnt__TopicExpressionType));	
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);	
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);	

			//ProducerReference
			ppTmpMem[iNum++] = calloc(1, sizeof(struct wsa5__EndpointReferenceType));	
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);	

			//Message.__any
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MAX);	
		}
		
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNotify = (struct _wsnt__Notify *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNotify->NotificationMessage = (struct wsnt__NotificationMessageHolderType *)ppTmpMem[iNum++];

		pNotifyMsg = ptOnvifCtrl->m_stOnvifCfg.m_ptNotify->NotificationMessage;
		for(i = 0; i< iMsgNum; i++)
		{
			//SubscriptionReference
			pNotifyMsg[i].SubscriptionReference = (struct wsa5__EndpointReferenceType *)ppTmpMem[iNum++];
			pNotifyMsg[i].SubscriptionReference->Address = (char *)ppTmpMem[iNum++];

			//Topic
			pNotifyMsg[i].Topic = (struct wsnt__TopicExpressionType *)ppTmpMem[iNum++];
			pNotifyMsg[i].Topic->__any   = (char *)ppTmpMem[iNum++];
			pNotifyMsg[i].Topic->Dialect = (char *)ppTmpMem[iNum++];

			//ProducerReference
			pNotifyMsg[i].ProducerReference          = (struct wsa5__EndpointReferenceType *)ppTmpMem[iNum++];
			pNotifyMsg[i].ProducerReference->Address = (char *)ppTmpMem[iNum++];

			//Message
			pNotifyMsg[i].Message.__any = (char *)ppTmpMem[iNum++];
			
			//初始化
			pcTmp = "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet";
			Strcpy(pNotifyMsg[i].Topic->Dialect, pcTmp, MEM_LEN_MIN);
		}
		
		ptOnvifCtrl->m_stOnvifCfg.m_ptNotify->__sizeNotificationMessage = 0;

	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20130306 v2.3.0 modify by ptb
20131101 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetServices(void *_pThis)
{	
	struct tds__Service *ptService = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iServiceNum = 6;
	int iMajor = 2;
	int iMinor = 3;
	int iNum = 0;
	int i    = 0;

	char *pNameSpace[6] = {	"http://www.onvif.org/ver20/analytics/wsdl",
							"http://www.onvif.org/ver10/events/wsdl",
							"http://www.onvif.org/ver10/device/wsdl",
							"http://www.onvif.org/ver20/imaging/wsdl",
							"http://www.onvif.org/ver10/media/wsdl",
							"http://www.onvif.org/ver20/ptz/wsdl"};

	char *pXaddr[6] = {"analytics", "events", "device", "imaging", "media", "ptz"};
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptServicesRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetServicesResponse));
		ppTmpMem[iNum++] = calloc(iServiceNum, sizeof(struct tds__Service));

		for(i = 0; i < iServiceNum; i++)
		{
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
			ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__Service_Capabilities));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__OnvifVersion));
		}

		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptServicesRsp = (struct _tds__GetServicesResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptServicesRsp->Service = (struct tds__Service *)ppTmpMem[iNum++];

		ptService = ptOnvifCtrl->m_stOnvifCfg.m_ptServicesRsp->Service;

		for(i = 0; i < iServiceNum; i++)
		{
			ptService[i].Namespace = (char *)ppTmpMem[iNum++];
			ptService[i].XAddr	   = (char *)ppTmpMem[iNum++];
			ptService[i].Capabilities = (struct _tds__Service_Capabilities *)ppTmpMem[iNum++];
			ptService[i].Capabilities->__any = (char *)ppTmpMem[iNum++];
			ptService[i].Version = (struct tt__OnvifVersion *)ppTmpMem[iNum++];
		}

		//初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptServicesRsp->__sizeService = iServiceNum;
		for(i = 0; i < iServiceNum; i++)
		{
			Strcpy(ptService[i].Namespace, pNameSpace[i], MEM_LEN_MID);
			sprintf(ptService[i].XAddr, "http://%s:%d/%s", ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort, pXaddr[i]);
			ptService[i].Version->Major = iMajor;
			ptService[i].Version->Minor = iMinor;
		}
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20131101 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetServiceCapabilities(void *_pThis)
{	
	struct tds__DeviceServiceCapabilities *ptDevServCap = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptServiceCapRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetServiceCapabilitiesResponse));	
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tds__DeviceServiceCapabilities));	

		//Network 7
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tds__NetworkCapabilities));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	

		//Security 15
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tds__SecurityCapabilities));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	

		//System 11
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tds__SystemCapabilities));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));	
	}

	
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptServiceCapRsp = (struct _tds__GetServiceCapabilitiesResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptServiceCapRsp->Capabilities = (struct tds__DeviceServiceCapabilities *)ppTmpMem[iNum++];

	ptDevServCap = ptOnvifCtrl->m_stOnvifCfg.m_ptServiceCapRsp->Capabilities;

	//Network
	ptDevServCap->Network = (struct tds__NetworkCapabilities *)ppTmpMem[iNum++];
	ptDevServCap->Network->IPFilter           = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Network->ZeroConfiguration  = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Network->IPVersion6         = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Network->DynDNS             = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Network->Dot11Configuration = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Network->HostnameFromDHCP   = (enum xsd__boolean *)ppTmpMem[iNum++];

	//Security
	ptDevServCap->Security = (struct tds__SecurityCapabilities *)ppTmpMem[iNum++];
	ptDevServCap->Security->TLS1_x002e0          = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->TLS1_x002e1          = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->TLS1_x002e2          = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->OnboardKeyGeneration = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->AccessPolicyConfig   = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->DefaultAccessPolicy  = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->Dot1X                = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->RemoteUserHandling   = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->X_x002e509Token      = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->SAMLToken            = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->KerberosToken        = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->UsernameToken        = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->HttpDigest           = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->Security->RELToken             = (enum xsd__boolean *)ppTmpMem[iNum++];

	//System
	ptDevServCap->System = (struct tds__SystemCapabilities *)ppTmpMem[iNum++];
	ptDevServCap->System->DiscoveryResolve       = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->DiscoveryBye           = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->RemoteDiscovery        = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->SystemBackup           = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->SystemLogging          = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->FirmwareUpgrade        = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->HttpFirmwareUpgrade    = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->HttpSystemBackup       = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->HttpSystemLogging      = (enum xsd__boolean *)ppTmpMem[iNum++];
	ptDevServCap->System->HttpSupportInformation = (enum xsd__boolean *)ppTmpMem[iNum++];

	//初始化
	//Network
	*ptDevServCap->Network->IPFilter           = xsd__boolean__false_;
	*ptDevServCap->Network->ZeroConfiguration  = xsd__boolean__false_;
	*ptDevServCap->Network->IPVersion6         = xsd__boolean__false_;
	*ptDevServCap->Network->DynDNS             = xsd__boolean__false_;
	*ptDevServCap->Network->Dot11Configuration = xsd__boolean__false_;
	*ptDevServCap->Network->HostnameFromDHCP   = xsd__boolean__false_;

	//Security
	*ptDevServCap->Security->TLS1_x002e0          = xsd__boolean__false_;
	*ptDevServCap->Security->TLS1_x002e1          = xsd__boolean__false_;
	*ptDevServCap->Security->TLS1_x002e2          = xsd__boolean__false_;
	*ptDevServCap->Security->OnboardKeyGeneration = xsd__boolean__false_;
	*ptDevServCap->Security->AccessPolicyConfig   = xsd__boolean__false_;
	*ptDevServCap->Security->DefaultAccessPolicy  = xsd__boolean__false_;
	*ptDevServCap->Security->Dot1X                = xsd__boolean__false_;
	*ptDevServCap->Security->RemoteUserHandling   = xsd__boolean__false_;
	*ptDevServCap->Security->X_x002e509Token      = xsd__boolean__false_;
	*ptDevServCap->Security->SAMLToken            = xsd__boolean__false_;
	*ptDevServCap->Security->KerberosToken        = xsd__boolean__false_;
	*ptDevServCap->Security->UsernameToken        = xsd__boolean__true_;
	*ptDevServCap->Security->HttpDigest           = xsd__boolean__true_;
	*ptDevServCap->Security->RELToken             = xsd__boolean__false_;

	//System
	*ptDevServCap->System->DiscoveryResolve			= xsd__boolean__true_;
	*ptDevServCap->System->DiscoveryBye				= xsd__boolean__true_;
	*ptDevServCap->System->RemoteDiscovery			= xsd__boolean__true_;
	*ptDevServCap->System->SystemBackup				= xsd__boolean__false_;
	*ptDevServCap->System->SystemLogging			= xsd__boolean__false_;
	*ptDevServCap->System->FirmwareUpgrade			= xsd__boolean__false_;
	*ptDevServCap->System->HttpFirmwareUpgrade		= xsd__boolean__false_;
	*ptDevServCap->System->HttpSystemBackup			= xsd__boolean__false_;
	*ptDevServCap->System->HttpSystemLogging		= xsd__boolean__false_;
	*ptDevServCap->System->HttpSupportInformation	= xsd__boolean__false_;

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by pangtb
20130306 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetDeviceInformation(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MINI] = {0};
	int iNum = 0;
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetDeviceInformationResponse));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
	}

	
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assgin memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp = (struct _tds__GetDeviceInformationResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Manufacturer    = (char *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Model           = (char *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->FirmwareVersion = (char *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->SerialNumber    = (char *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->HardwareId      = (char *)ppTmpMem[iNum++];
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by sy
20131101 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetSystemDateAndTime(void *_pThis)
{	
	struct tt__SystemDateTime *ptSysTime = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MINI] = {0};
	int iNum = 0;
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptSystemTimeRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetSystemDateAndTimeResponse));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__SystemDateTime));
		//TimeZone
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__TimeZone));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		//UTCDateTime
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__DateTime));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Time));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Date));
		//LocalDateTime
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__DateTime));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Time));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Date));
	}

	
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assgin memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptSystemTimeRsp = (struct _tds__GetSystemDateAndTimeResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptSystemTimeRsp->SystemDateAndTime = (struct tt__SystemDateTime *)ppTmpMem[iNum++];

	ptSysTime = ptOnvifCtrl->m_stOnvifCfg.m_ptSystemTimeRsp->SystemDateAndTime;

	ptSysTime->TimeZone = (struct tt__TimeZone *)ppTmpMem[iNum++];
	ptSysTime->TimeZone->TZ = (char *)ppTmpMem[iNum++];

	ptSysTime->UTCDateTime = (struct tt__DateTime *)ppTmpMem[iNum++];
	ptSysTime->UTCDateTime->Time = (struct tt__Time *)ppTmpMem[iNum++];
	ptSysTime->UTCDateTime->Date = (struct tt__Date *)ppTmpMem[iNum++];

	ptSysTime->LocalDateTime  = (struct tt__DateTime *)ppTmpMem[iNum++];
	ptSysTime->LocalDateTime->Time = (struct tt__Time *)ppTmpMem[iNum++];
	ptSysTime->LocalDateTime->Date = (struct tt__Date *)ppTmpMem[iNum++];

	//初始化
	ptSysTime->DateTimeType    = tt__SetDateTimeType__Manual;
	ptSysTime->DaylightSavings = xsd__boolean__false_;

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by pangtb
            20131028 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetScopes(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	char *pcTmp = NULL;
	int iCount  = 0;
	int iSize   = 0;
	int iNum    = 0;
	int i       = 0;
	int itemp = 0;
	char *ptemp = NULL;

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp))
	{
		iSize = (enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scope_size", 0, 8);

		//calloc memory
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetScopesResponse));
		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__Scope));
		
		for(i = 0; i < iSize; i++)
		{
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
		}

		//check memory	
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			printf("warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}
	
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp = (struct _tds__GetScopesResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes = (struct tt__Scope *)ppTmpMem[iNum++];
		for(i = 0; i < iSize; i++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem = (char*)ppTmpMem[iNum++];
		}
		
		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->__sizeScopes = iSize;
		for(i = 0; i < MAX_INDEX_NUM; i++)
		{
			pcTmp =	GetProStr(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scope", i + 1, NULL);
			if(NULL != pcTmp)
			{
				strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeItem, pcTmp, MEM_LEN_MID);
				ptOnvifCtrl->m_stOnvifCfg.m_ptScopesRsp->Scopes[i].ScopeDef =	\
						(enum tt__ScopeDefinition)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "scopes", 0, "scopedef", i + 1, 0);
				iCount++;
			}
			
			if(iCount >= iSize)
			{
				break;
			}
		}
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20131028 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetDiscoveryMode(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;
		
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp))
	{
		//calloc memory
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetDiscoveryModeResponse));
	}

	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//average memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp = (struct _tds__GetDiscoveryModeResponse *)ppTmpMem[iNum++];

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp->DiscoveryMode = (enum tt__DiscoveryMode)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "discrymode", 0, tt__DiscoveryMode__Discoverable);
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by ptb
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tds__GetUsers(void *_pThis)
{	
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	char *pcTmpBuf = NULL;
	int iUserNum   = 0;
	int iNum 	   = 0;
	int i		   = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	if(NULL == ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp)
	{
		//从配置文件中读取用户数
		iUserNum = (enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "user_num", 0, 2);

		if(0 < iUserNum)
		{
			//malloc memeory
			ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetUsersResponse));
			ppTmpMem[iNum++] = calloc(iUserNum, sizeof(struct tt__User));
				
			for(i = 0; i < iUserNum; i++)
			{
				ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
				ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
			}

			
			if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
			{
				td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
				FreeAllTmpMem(ppTmpMem, iNum);
				return TD_ERROR;
			}
		
			//assign memory
			iNum = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp = (struct _tds__GetUsersResponse *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User = (struct tt__User *)ppTmpMem[iNum++];

			for(i = 0; i < iUserNum; i++)
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Username = (char *)ppTmpMem[iNum++];
				ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Password = (char *)ppTmpMem[iNum++];
			}

			//全局变量初始化
			ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser = iUserNum;
			for(i = 0; i < iUserNum; i++)
			{	
				//增加用户名为空的判断
				pcTmpBuf = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "user_", i + 1, "username", 0, "unknown");
				if(NULL != pcTmpBuf)
				{
					strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Username, pcTmpBuf);
				}

				//密码为空的判断
				pcTmpBuf = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "user_", i + 1, "password", 0, 0);
				if(NULL == pcTmpBuf)
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Password = NULL;
				}
				else
				{
					strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].Password , pcTmpBuf);
				}

				ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User[i].UserLevel = (enum tt__UserLevel)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "user_", i + 1, "userlevel", 0, 0);
			}
		}
		else if(0 == iUserNum)
		{
			//如果用户数等于0
			//malloc memeory
			ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetUsersResponse));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__User));
			
			
			if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
			{
				td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
				FreeAllTmpMem(ppTmpMem, iNum);
			
				return TD_ERROR;
			}

			//assign memory
			iNum = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp = (struct _tds__GetUsersResponse *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User = (struct tt__User *)ppTmpMem[iNum++];

			//全局变量初始化
			ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->__sizeUser  = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptUsersRsp->User		= NULL;
		}
		else
		{
			td_printf(0, "%s %d %s iUserNum:%d\n", __FILE__, __LINE__, __FUNCTION__, iUserNum); 
			return TD_ERROR;
		}
	}
	
	return SOAP_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20131028 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetWsdlUrl(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptWsdlUrlRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetWsdlUrlResponse));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
		
		//check memory
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}
		
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptWsdlUrlRsp = (struct _tds__GetWsdlUrlResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptWsdlUrlRsp->WsdlUrl = (char *)ppTmpMem[iNum++];

		//初始化
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptWsdlUrlRsp->WsdlUrl, "http://%s:%d/", \
			ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort);
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tds__GetCapabilities(void *_pThis)
{	
	struct tt__Capabilities *ptCapabilities = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iSupportVerNum = 3;
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptCapbilityRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetCapabilitiesResponse));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Capabilities));

		//Analytics
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__AnalyticsCapabilities));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);

		//Device
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__DeviceCapabilities));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__NetworkCapabilities));
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__SystemCapabilities));
		ppTmpMem[iNum++] = calloc(iSupportVerNum, sizeof(struct tt__OnvifVersion));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IOCapabilities));
		ppTmpMem[iNum++] = calloc(1, sizeof(int));
		ppTmpMem[iNum++] = calloc(1, sizeof(int));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__SecurityCapabilities));

		//Events
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__EventCapabilities));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);

		//Imaging
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ImagingCapabilities));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);

		//Media
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__MediaCapabilities));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__RealTimeStreamingCapabilities));
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));

		//PTZ
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PTZCapabilities));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptCapbilityRsp = (struct _tds__GetCapabilitiesResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptCapbilityRsp->Capabilities = (struct tt__Capabilities *)ppTmpMem[iNum++];

		ptCapabilities = ptOnvifCtrl->m_stOnvifCfg.m_ptCapbilityRsp->Capabilities;

		//Analytics
		ptCapabilities->Analytics = (struct tt__AnalyticsCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Analytics->XAddr = (char *)ppTmpMem[iNum++];

		//Device
		ptCapabilities->Device = (struct tt__DeviceCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Device->XAddr = (char *)ppTmpMem[iNum++];

		ptCapabilities->Device->Network  = (struct tt__NetworkCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Device->Network->IPFilter  = (enum xsd__boolean *)ppTmpMem[iNum++];
		ptCapabilities->Device->Network->ZeroConfiguration = (enum xsd__boolean *)ppTmpMem[iNum++];
		ptCapabilities->Device->Network->IPVersion6 = (enum xsd__boolean *)ppTmpMem[iNum++];
		ptCapabilities->Device->Network->DynDNS = (enum xsd__boolean *)ppTmpMem[iNum++];
		
		ptCapabilities->Device->System = (struct tt__SystemCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Device->System->SupportedVersions  = (struct tt__OnvifVersion *)ppTmpMem[iNum++];
		
		ptCapabilities->Device->IO = (struct tt__IOCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Device->IO->InputConnectors = (int *)ppTmpMem[iNum++];
		ptCapabilities->Device->IO->RelayOutputs = (int *)ppTmpMem[iNum++];

		ptCapabilities->Device->Security = (struct tt__SecurityCapabilities *)ppTmpMem[iNum++];

		//Events
		ptCapabilities->Events = (struct tt__EventCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Events->XAddr  = (char *)ppTmpMem[iNum++];

		//Imaging
		ptCapabilities->Imaging = (struct tt__ImagingCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Imaging->XAddr = (char *)ppTmpMem[iNum++];

		//Media
		ptCapabilities->Media = (struct tt__MediaCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Media->XAddr = (char *)ppTmpMem[iNum++];
		ptCapabilities->Media->StreamingCapabilities = (struct tt__RealTimeStreamingCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->Media->StreamingCapabilities->RTPMulticast = (enum xsd__boolean *)ppTmpMem[iNum++];
		ptCapabilities->Media->StreamingCapabilities->RTP_USCORETCP = (enum xsd__boolean *)ppTmpMem[iNum++];
		ptCapabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = (enum xsd__boolean *)ppTmpMem[iNum++];

		//PTZ
		ptCapabilities->PTZ = (struct tt__PTZCapabilities *)ppTmpMem[iNum++];
		ptCapabilities->PTZ->XAddr = (char *)ppTmpMem[iNum++];

		//全局变量初始化
		//Analytics
		sprintf(ptCapabilities->Analytics->XAddr, 
			"http://%s:%d/onvif/analytics", ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort);
		ptCapabilities->Analytics->RuleSupport            = xsd__boolean__true_;
		ptCapabilities->Analytics->AnalyticsModuleSupport = xsd__boolean__true_;

		//Device
		sprintf(ptCapabilities->Device->XAddr,
 			"http://%s:%d/onvif/device", ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort);
		*(ptCapabilities->Device->Network->IPFilter)          = xsd__boolean__false_;
		*(ptCapabilities->Device->Network->ZeroConfiguration) = xsd__boolean__false_;
		*(ptCapabilities->Device->Network->IPVersion6)        = xsd__boolean__false_;
		*(ptCapabilities->Device->Network->DynDNS)            = xsd__boolean__false_;

		ptCapabilities->Device->System->DiscoveryResolve = xsd__boolean__true_;
		ptCapabilities->Device->System->DiscoveryBye     = xsd__boolean__true_;
		ptCapabilities->Device->System->RemoteDiscovery  = xsd__boolean__true_;
		ptCapabilities->Device->System->SystemBackup     = xsd__boolean__false_;
		ptCapabilities->Device->System->SystemLogging    = xsd__boolean__false_;
		ptCapabilities->Device->System->FirmwareUpgrade  = xsd__boolean__false_;
		ptCapabilities->Device->System->SystemLogging    = xsd__boolean__false_;
		ptCapabilities->Device->System->__sizeSupportedVersions    = iSupportVerNum;
		ptCapabilities->Device->System->SupportedVersions[0].Major = 2;
		ptCapabilities->Device->System->SupportedVersions[0].Minor = 3;
		ptCapabilities->Device->System->SupportedVersions[1].Major = 2;
		ptCapabilities->Device->System->SupportedVersions[1].Minor = 0;
		ptCapabilities->Device->System->SupportedVersions[2].Major = 1;
		ptCapabilities->Device->System->SupportedVersions[2].Minor = 0;

		*(ptCapabilities->Device->IO->InputConnectors) = 0;

		if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
		{
			//DVR兼容NUUO平台
			*(ptCapabilities->Device->IO->RelayOutputs) = 1;
		}
		else
		{
			*(ptCapabilities->Device->IO->RelayOutputs) = ptOnvifCtrl->m_stNvsInfo.m_iAlarmOutNum;
		}

		//Events
		sprintf(ptCapabilities->Events->XAddr, "http://%s:%d/onvif/events", \
			ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLEventPort);
		ptCapabilities->Events->WSPullPointSupport = xsd__boolean__true_;
		ptCapabilities->Events->WSSubscriptionPolicySupport = xsd__boolean__true_;
		ptCapabilities->Events->WSPausableSubscriptionManagerInterfaceSupport = xsd__boolean__false_;

		//Imaging
		sprintf(ptCapabilities->Imaging->XAddr, "http://%s:%d/onvif/imaging", \
			ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort);

		//Media
		sprintf(ptCapabilities->Media->XAddr, "http://%s:%d/onvif/media", \
			ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort);
		*(ptCapabilities->Media->StreamingCapabilities->RTPMulticast) = xsd__boolean__false_;
		*(ptCapabilities->Media->StreamingCapabilities->RTP_USCORETCP) = xsd__boolean__true_;
		*(ptCapabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP) = xsd__boolean__true_;

		//PTZ
		sprintf(ptCapabilities->PTZ->XAddr, "http://%s:%d/onvif/ptz", \
			ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort);
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120.15 v1.0 creat by czl
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tds__GetHostname(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptHostnameRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetHostnameResponse));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__HostnameInformation));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);

		//check memory
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);		
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptHostnameRsp = (struct _tds__GetHostnameResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptHostnameRsp->HostnameInformation = (struct tt__HostnameInformation *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptHostnameRsp->HostnameInformation->Name = NULL;
		ptOnvifCtrl->m_stOnvifCfg.m_ptHostnameRsp->HostnameInformation->Name = (char *)ppTmpMem[iNum++];

		strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptHostnameRsp->HostnameInformation->Name, "HostName");
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by czl
            20131028 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetDNS(void *_pThis)
{	
	struct tt__DNSInformation *ptDNSInfo = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptDNSRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetDNSResponse));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__DNSInformation));

		//SearchDomain
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);

		//DNSFromDHCP
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IPAddress));
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_M);

		//DNSManual
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IPAddress));
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_M);

		//check memory
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptDNSRsp  = (struct _tds__GetDNSResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptDNSRsp->DNSInformation = (struct tt__DNSInformation *)ppTmpMem[iNum++];

		ptDNSInfo = ptOnvifCtrl->m_stOnvifCfg.m_ptDNSRsp->DNSInformation;

		ptDNSInfo->SearchDomain = (char **)ppTmpMem[iNum++];
		*(ptDNSInfo->SearchDomain) = (char *)ppTmpMem[iNum++];

		ptDNSInfo->DNSFromDHCP                 = (struct tt__IPAddress *)ppTmpMem[iNum++];
		ptDNSInfo->DNSFromDHCP->IPv4Address    = (char **)ppTmpMem[iNum++];
		*(ptDNSInfo->DNSFromDHCP->IPv4Address) = (char *)ppTmpMem[iNum++];

		ptDNSInfo->DNSManual                   = (struct tt__IPAddress *)ppTmpMem[iNum++];
		ptDNSInfo->DNSManual->IPv4Address      = (char **)ppTmpMem[iNum++];
		*(ptDNSInfo->DNSManual->IPv4Address)   = (char *)ppTmpMem[iNum++];

		//全局变量初始化
		ptDNSInfo->FromDHCP = xsd__boolean__false_;
		ptDNSInfo->__sizeSearchDomain = 1;
		strncpy(*(ptDNSInfo->SearchDomain), "local", 5);
		ptDNSInfo->__sizeDNSFromDHCP  = 1;
		ptDNSInfo->DNSFromDHCP->Type  = tt__IPType__IPv4;
		ptDNSInfo->__sizeDNSManual    = 1;
		ptDNSInfo->DNSManual->Type    = tt__IPType__IPv4;
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by czl
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tds__GetNTP(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	if(NULL == ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp)
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetNTPResponse));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__NTPInformation));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__NetworkHost));
		ppTmpMem[iNum++] = calloc(1, sizeof(char **));
		ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN_MEM);
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__NetworkHost));
		ppTmpMem[iNum++] = calloc(1, sizeof(char **));
		ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN_MEM);

		//check memory
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp                                              = (struct _tds__GetNTPResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation                              = (struct tt__NTPInformation *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPFromDHCP                 = (struct tt__NetworkHost *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPFromDHCP->IPv4Address    = (char **)ppTmpMem[iNum++];
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPFromDHCP->IPv4Address) = (char *)ppTmpMem[iNum++];

		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPManual                   = (struct tt__NetworkHost *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPManual->IPv4Address      = (char **)ppTmpMem[iNum++];
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPManual->IPv4Address)   = (char *)ppTmpMem[iNum++];

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->FromDHCP = xsd__boolean__false_;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->__sizeNTPFromDHCP = 1;
		strcpy(*ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPFromDHCP->IPv4Address, "No NTPFromDHCP");
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPFromDHCP->Type = tt__NetworkHostType__IPv4;

		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->__sizeNTPManual   = 1;
		strcpy(*ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPManual->IPv4Address, "10.1.1.1");
		ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->NTPManual->Type = tt__NetworkHostType__IPv4;
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的网络结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120314 v1.0 creat by czl
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tds__GetNetworkInterfaces(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	struct tt__NetworkInterface *ptNetwork = NULL;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetNetworkInterfacesResponse));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__NetworkInterface));

		//token
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);

		//Info
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__NetworkInterfaceInfo));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_M);
		ppTmpMem[iNum++] = calloc(1, sizeof(int));

		//IPv4
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IPv4NetworkInterface));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IPv4Configuration));

		//Manual
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PrefixedIPv4Address));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);

		//FromDHCP
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PrefixedIPv4Address));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		
		//check memory
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp = (struct _tds__GetNetworkInterfacesResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces = (struct tt__NetworkInterface *)ppTmpMem[iNum++];

		ptNetwork = ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces;

		//token
		ptNetwork->token = (char *)ppTmpMem[iNum++];

		//Info
		ptNetwork->Info            = (struct tt__NetworkInterfaceInfo *)ppTmpMem[iNum++];
		ptNetwork->Info->Name      = (char *)ppTmpMem[iNum++];
		ptNetwork->Info->HwAddress = (char *)ppTmpMem[iNum++];
		ptNetwork->Info->MTU       = (int *)ppTmpMem[iNum++];

		//IPv4
		ptNetwork->IPv4                            = (struct tt__IPv4NetworkInterface *)ppTmpMem[iNum++];
		ptNetwork->IPv4->Config                    = (struct tt__IPv4Configuration *)ppTmpMem[iNum++];
		ptNetwork->IPv4->Config->Manual            = (struct tt__PrefixedIPv4Address *)ppTmpMem[iNum++];
		ptNetwork->IPv4->Config->Manual->Address   = (char *)ppTmpMem[iNum++];
		ptNetwork->IPv4->Config->FromDHCP          = (struct tt__PrefixedIPv4Address *)ppTmpMem[iNum++];
		ptNetwork->IPv4->Config->FromDHCP->Address = (char *)ppTmpMem[iNum++];

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->__sizeNetworkInterfaces = 1;
		strcpy(ptNetwork->token, "net");
		ptNetwork->Enabled = xsd__boolean__true_;

		//Info
		strcpy(ptNetwork->Info->Name, "eth0");
		*(ptNetwork->Info->MTU) = 1500;

		//IPv4
		ptNetwork->IPv4->Enabled = xsd__boolean__true_;
		ptNetwork->IPv4->Config->__sizeManual = 1;
		memcpy(ptNetwork->IPv4->Config->Manual->Address, ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, 31);
		memcpy(ptNetwork->IPv4->Config->FromDHCP->Address, ptOnvifCtrl->m_stNvsInfo.m_cLocalIP, 31);
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by czl
20131115 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetNetworkProtocols(void *_pThis)
{	
	struct tt__NetworkProtocol *ptProtocols = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MINI] = {0};
	int iSize = 3;
	int iNum = 0;
	int i = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkProRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetNetworkProtocolsResponse));	
		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__NetworkProtocol));
		for(i = 0; i < iSize; i++)
		{
			ppTmpMem[iNum++] = calloc(1, sizeof(int));
		}

		//check memory
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkProRsp = (struct _tds__GetNetworkProtocolsResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkProRsp->NetworkProtocols = (struct tt__NetworkProtocol *)ppTmpMem[iNum++];

		ptProtocols = ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkProRsp->NetworkProtocols;

		for(i = 0; i < iSize; i++)
		{
			ptProtocols[i].Port = (int *)ppTmpMem[iNum++];
			
		}

		//全局变量初始化		
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkProRsp->__sizeNetworkProtocols = iSize;

		ptProtocols[0].__sizePort = 1;
		ptProtocols[0].Enabled	  = xsd__boolean__true_;
		ptProtocols[0].Name  = tt__NetworkProtocolType__HTTP;
		*ptProtocols[0].Port = ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort;

		ptProtocols[1].__sizePort = 0;
		ptProtocols[1].Enabled	  = xsd__boolean__false_;
		ptProtocols[1].Name  = tt__NetworkProtocolType__HTTPS;
		*ptProtocols[1].Port = 0;

		ptProtocols[2].__sizePort = 1;
		ptProtocols[2].Enabled	  = xsd__boolean__true_;
		ptProtocols[2].Name  = tt__NetworkProtocolType__RTSP;
		*ptProtocols[2].Port = ptOnvifCtrl->m_stRtspServer.m_iLPort;
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120224 v1.0 creat by czl
            20131028 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetNetworkDefaultGateway(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkGWRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetNetworkDefaultGatewayResponse));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__NetworkGateway));
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(1, MEM_LEN_M);

		//check memory
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkGWRsp                                 = (struct _tds__GetNetworkDefaultGatewayResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkGWRsp->NetworkGateway                 = (struct tt__NetworkGateway *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkGWRsp->NetworkGateway->IPv4Address    = (char **)ppTmpMem[iNum++];
		*(ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkGWRsp->NetworkGateway->IPv4Address) = (char *)ppTmpMem[iNum++];

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkGWRsp->NetworkGateway->__sizeIPv4Address = 1;
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20130306 v2.3.0 modify by ptb
20131009 v2.3.1 modify by ptb
**************************************************/
int GMemInit__tds__GetRelayOutputs(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	struct tt__RelayOutput *ptRelayOutput = NULL;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iRelayOutputCnt = 0;
	int iIdleState      = 0;
	int iNum = 0;
	int i    = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		//DVR兼容NUUO平台
		iRelayOutputCnt = 1;
	}
	else
	{
		iRelayOutputCnt = ptOnvifCtrl->m_stNvsInfo.m_iAlarmOutNum;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tds__GetRelayOutputsResponse));
		ppTmpMem[iNum++] = calloc(iRelayOutputCnt, sizeof(struct tt__RelayOutput));

		for(i = 0; i < iRelayOutputCnt; i++)
		{
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__RelayOutputSettings));
		}

		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp = (struct _tds__GetRelayOutputsResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs = (struct tt__RelayOutput *)ppTmpMem[iNum++];

		ptRelayOutput = ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->RelayOutputs;

		for(i = 0; i < iRelayOutputCnt; i++)
		{
			ptRelayOutput[i].token = (char *)ppTmpMem[iNum++];
			ptRelayOutput[i].Properties = (struct tt__RelayOutputSettings *)ppTmpMem[iNum++];
		}

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptRelayOutputRsp->__sizeRelayOutputs = iRelayOutputCnt;
		for(i = 0; i < iRelayOutputCnt; i++)
		{
			sprintf(ptRelayOutput[i].token, "relayoutput_%02d", i + 1);
			ptRelayOutput[i].Properties->Mode      = tt__RelayMode__Bistable;
			ptRelayOutput[i].Properties->DelayTime = 0;
		
			iIdleState = (OnvifCmdGetRelayOutputCnf(i + 1) == GUI_ALARM_OPEN_TYPE) ? tt__RelayIdleState__closed : tt__RelayIdleState__open;
			ptRelayOutput[i].Properties->IdleState = (enum tt__RelayIdleState)iIdleState;	
		}
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120418 v1.0 creat by ptb
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__timg10__GetImagingSettings(void *_pThis)
{	
	struct tt__ImagingSettings *ptImgSetting = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV10Rsp))
	{
		//malloc memeory
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _timg10__GetImagingSettingsResponse));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ImagingSettings));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__BacklightCompensation));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Exposure));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Rectangle));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FocusConfiguration));			
		ppTmpMem[iNum++] = calloc(1, sizeof(enum tt__IrCutFilterMode));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WideDynamicRange));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WhiteBalance));			

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV10Rsp = (struct _timg10__GetImagingSettingsResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV10Rsp->ImagingSettings = (struct tt__ImagingSettings *)ppTmpMem[iNum++];

		ptImgSetting = ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV10Rsp->ImagingSettings;

		ptImgSetting->BacklightCompensation = (struct tt__BacklightCompensation *)ppTmpMem[iNum++];
		ptImgSetting->Brightness = (float *)ppTmpMem[iNum++];
		ptImgSetting->ColorSaturation = (float *)ppTmpMem[iNum++];
		ptImgSetting->Contrast = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure = (struct tt__Exposure *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window = (struct tt__Rectangle *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window->bottom = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window->top    = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window->right  = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window->left   = (float *)ppTmpMem[iNum++];
		ptImgSetting->Focus = (struct tt__FocusConfiguration *)ppTmpMem[iNum++];
		ptImgSetting->IrCutFilter = (enum tt__IrCutFilterMode *)ppTmpMem[iNum++];
		ptImgSetting->Sharpness = (float *)ppTmpMem[iNum++];
		ptImgSetting->WideDynamicRange = (struct tt__WideDynamicRange *)ppTmpMem[iNum++];
		ptImgSetting->WhiteBalance = (struct tt__WhiteBalance *)ppTmpMem[iNum++];
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 	20120418 v1.0 creat by ptb
			20130227 v2.3.0 modify by ptb
**************************************************/
int GMemInit__timg10__GetOptions(void *_pThis)
{	
	struct tt__ImagingOptions *ptImgOptions = NULL;
	void *ppTmpMem[MEM_LEN_HUGE] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iIRcutModeNum = 3;
	int iModeNum = 2;
	int iVSNum = 0;
	int iNum   = 0;
	int i      = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	//从全局变量中获得当前的VideoSource数
	iVSNum = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _timg10__GetOptionsResponse));					
		//ImagingOptions
		ppTmpMem[iNum++] = calloc(iVSNum, sizeof(struct tt__ImagingOptions));						

		for(i = 0; i < iVSNum; i++)
		{
			//BacklightCompensation
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__BacklightCompensationOptions));	
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__WideDynamicMode));		
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			//Brightness
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));						

			//ColorSaturation
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));						

			//Contrast
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));						

			//Exposure
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ExposureOptions));					
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__ExposureMode));					
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__ExposurePriority));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			//Focus
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FocusOptions));					
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__AutoFocusMode));			
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			//IrCutFilterModes
			ppTmpMem[iNum++] = calloc(1, iIRcutModeNum * sizeof(enum tt__IrCutFilterMode));						

			//Sharpness
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));						

			//WideDynamicRange
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WideDynamicRangeOptions));			
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__WideDynamicMode));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			//WhiteBalance
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WhiteBalanceOptions));				
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__WhiteBalanceMode));				
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			
			if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
			{
				td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
				FreeAllTmpMem(ppTmpMem, iNum);
				return TD_ERROR;
			}
				
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp = (struct _timg10__GetOptionsResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions = (struct tt__ImagingOptions *)ppTmpMem[iNum++];
		ptImgOptions = ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV10Rsp->ImagingOptions;

		for(i = 0; i < iVSNum; i++)
		{
			//BacklightCompensation
			ptImgOptions[i].BacklightCompensation = (struct tt__BacklightCompensationOptions *)ppTmpMem[iNum++];
			ptImgOptions[i].BacklightCompensation->Mode = (enum tt__WideDynamicMode *)ppTmpMem[iNum++];
			ptImgOptions[i].BacklightCompensation->Level = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//Brightness
			ptImgOptions[i].Brightness = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//ColorSaturation
			ptImgOptions[i].ColorSaturation = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//Contrast
			ptImgOptions[i].Contrast = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//Exposure
			ptImgOptions[i].Exposure = (struct tt__ExposureOptions *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->Mode = (enum tt__ExposureMode *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->Priority = (enum tt__ExposurePriority *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->MinExposureTime = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->MaxExposureTime = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->MinGain = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->MaxGain = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->MinIris = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->MaxIris = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->ExposureTime = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->Gain = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Exposure->Iris = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//Focus
			ptImgOptions[i].Focus = (struct tt__FocusOptions *)ppTmpMem[iNum++];
			ptImgOptions[i].Focus->AutoFocusModes = (enum tt__AutoFocusMode *)ppTmpMem[iNum++];
			ptImgOptions[i].Focus->DefaultSpeed = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Focus->NearLimit = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].Focus->FarLimit = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//IrCutFilterModes
			ptImgOptions[i].IrCutFilterModes = (enum tt__IrCutFilterMode *)ppTmpMem[iNum++];

			//Sharpness
			ptImgOptions[i].Sharpness = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//WideDynamicRange
			ptImgOptions[i].WideDynamicRange = (struct tt__WideDynamicRangeOptions *)ppTmpMem[iNum++];
			ptImgOptions[i].WideDynamicRange->Mode = (enum tt__WideDynamicMode *)ppTmpMem[iNum++];
			ptImgOptions[i].WideDynamicRange->Level = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//WhiteBalance
			ptImgOptions[i].WhiteBalance = (struct tt__WhiteBalanceOptions *)ppTmpMem[iNum++];
			ptImgOptions[i].WhiteBalance->Mode = (enum tt__WhiteBalanceMode *)ppTmpMem[iNum++];
			ptImgOptions[i].WhiteBalance->YrGain = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOptions[i].WhiteBalance->YbGain = (struct tt__FloatRange *)ppTmpMem[iNum++];
		}

		//初始化全局变量
		for(i = 0; i < iVSNum; i++)
		{
			//BacklightCompensation
			ptImgOptions[i].BacklightCompensation->__sizeMode  = iModeNum;
			//*ptImgOptions[i].BacklightCompensation->Mode       = tt__BacklightCompensationMode__OFF;
			//*(ptImgOptions[i].BacklightCompensation->Mode + 1) = tt__BacklightCompensationMode__ON;
			ptImgOptions[i].BacklightCompensation->Level->Min  = VSIOBCOLEVMIN;
			ptImgOptions[i].BacklightCompensation->Level->Max  = VSIOBCOLEVMAX;

			//Brightness
			ptImgOptions[i].Brightness->Min = VSIOBCOBNMIN;
			ptImgOptions[i].Brightness->Max = VSIOBCOBNMAX;

			//ColorSaturation
			ptImgOptions[i].ColorSaturation->Min = VSIOBCOCSMIN;
			ptImgOptions[i].ColorSaturation->Max = VSIOBCOCSMAX;

			//Contrast
			ptImgOptions[i].Contrast->Min = VSIOBCOCTMIN;
			ptImgOptions[i].Contrast->Max = VSIOBCOCTMAX;

			//Exposure
			ptImgOptions[i].Exposure->__sizeMode           = iModeNum;
			//*ptImgOptions[i].Exposure->Mode                = tt__WideDynamicMode__OFF;
			//*(ptImgOptions[i].Exposure->Mode + 1)          = tt__WideDynamicMode__ON;
			ptImgOptions[i].Exposure->__sizeMode           = iModeNum;
			//*ptImgOptions[i].Exposure->Priority            = tt__ExposurePriority__LowNoise;
			//*(ptImgOptions[i].Exposure->Priority + 1)      = tt__ExposurePriority__FrameRate;
			ptImgOptions[i].Exposure->MinExposureTime->Min = VSIOEOMINETMIN;
			ptImgOptions[i].Exposure->MinExposureTime->Max = VSIOEOMINETMAX;
			ptImgOptions[i].Exposure->MaxExposureTime->Min = VSIOEOMAXETMIN;
			ptImgOptions[i].Exposure->MaxExposureTime->Max = VSIOEOMAXETMAX;
			ptImgOptions[i].Exposure->MinGain->Min 		  = VSIOEOMINGMIN;
			ptImgOptions[i].Exposure->MinGain->Max 		  = VSIOEOMINGMAX;
			ptImgOptions[i].Exposure->MaxGain->Min 		  = VSIOEOMAXGMIN;
			ptImgOptions[i].Exposure->MaxGain->Max 		  = VSIOEOMAXGMAX;
			ptImgOptions[i].Exposure->MinIris->Min 		  = VSIOEOMINIMIN;
			ptImgOptions[i].Exposure->MinIris->Max 		  = VSIOEOMINIMAX;
			ptImgOptions[i].Exposure->MaxIris->Min 		  = VSIOEOMAXIMIN;
			ptImgOptions[i].Exposure->MaxIris->Max         = VSIOEOMAXIMAX;
			ptImgOptions[i].Exposure->ExposureTime->Min    = VSIOEOETMIN;
			ptImgOptions[i].Exposure->ExposureTime->Max    = VSIOEOETMAX;
			ptImgOptions[i].Exposure->Gain->Min            = VSIOEOGMIN;
			ptImgOptions[i].Exposure->Gain->Max            = VSIOEOGMAX;
			ptImgOptions[i].Exposure->Iris->Min            = VSIOEOIMIN;
			ptImgOptions[i].Exposure->Iris->Max            = VSIOEOIMAX;

			//Focus
			ptImgOptions[i].Focus->__sizeAutoFocusModes  = iModeNum;
			*ptImgOptions[i].Focus->AutoFocusModes 		= tt__AutoFocusMode__AUTO;
			*(ptImgOptions[i].Focus->AutoFocusModes + 1) = tt__AutoFocusMode__MANUAL;
			ptImgOptions[i].Focus->DefaultSpeed->Min     = VSIOFODSMIN;
			ptImgOptions[i].Focus->DefaultSpeed->Max     = VSIOFODSMAX;
			ptImgOptions[i].Focus->NearLimit->Min        = VSIOFONLMIN;
			ptImgOptions[i].Focus->NearLimit->Max        = VSIOFONLMAX;
			ptImgOptions[i].Focus->FarLimit->Min         = VSIOFOFLMIN;
			ptImgOptions[i].Focus->FarLimit->Max         = VSIOFOFLMAX;

			//IrCutFilter
			ptImgOptions[i].__sizeIrCutFilterModes  = iIRcutModeNum;
			*ptImgOptions[i].IrCutFilterModes       = tt__IrCutFilterMode__ON;
			*(ptImgOptions[i].IrCutFilterModes + 1) = tt__IrCutFilterMode__OFF;
			*(ptImgOptions[i].IrCutFilterModes + 2) = tt__IrCutFilterMode__AUTO;

			//Sharpness
			ptImgOptions[i].Sharpness->Min = VSIOSNMIN;
			ptImgOptions[i].Sharpness->Max = VSIOSNMAX;
					
			//WideDynamicRange
			ptImgOptions[i].WideDynamicRange->__sizeMode  = iModeNum;
			*ptImgOptions[i].WideDynamicRange->Mode       = tt__WideDynamicMode__OFF;
			*(ptImgOptions[i].WideDynamicRange->Mode + 1) = tt__WideDynamicMode__ON;
			ptImgOptions[i].WideDynamicRange->Level->Min  = VSIOWDRLEVMIN;
			ptImgOptions[i].WideDynamicRange->Level->Max  = VSIOWDRLEVMAX;

			//WhiteBalance
			ptImgOptions[i].WhiteBalance->__sizeMode  = iModeNum;
			*ptImgOptions[i].WhiteBalance->Mode       = tt__WhiteBalanceMode__AUTO;
			*(ptImgOptions[i].WhiteBalance->Mode + 1) = tt__WhiteBalanceMode__MANUAL;
			ptImgOptions[i].WhiteBalance->YrGain->Min = VSIOWBYRGMIN;
			ptImgOptions[i].WhiteBalance->YrGain->Max = VSIOWBYRGMAX;
			ptImgOptions[i].WhiteBalance->YbGain->Min = VSIOWBYBGMIN;
			ptImgOptions[i].WhiteBalance->YbGain->Max = VSIOWBYBGMAX;
		}
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120418 v1.0 creat by ptb
            20131028 v2.3.1 modify by ptb
**************************************************/
int GMemInit__timg10__GetStatus(void *_pThis)
{	
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV10Rsp))
	{
		//malloc memeory
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _timg10__GetStatusResponse));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ImagingStatus));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FocusStatus));					

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV10Rsp = (struct _timg10__GetStatusResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV10Rsp->Status = (struct tt__ImagingStatus *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV10Rsp->Status->FocusStatus = (struct tt__FocusStatus *)ppTmpMem[iNum++];

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV10Rsp->Status->FocusStatus->Position = 0.0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV10Rsp->Status->FocusStatus->MoveStatus = tt__MoveStatus__UNKNOWN;
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120418 v1.0 creat by ptb
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__timg10__GetMoveOptions(void *_pThis)
{	
	struct tt__MoveOptions *ptMoveOptions = NULL;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV10Rsp))
	{
		//malloc memeory
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _timg10__GetMoveOptionsResponse));					
		//MoveOptions
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__MoveOptions));					
		//Absolute
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__AbsoluteFocusOptions));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

		//Relative
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__RelativeFocusOptions));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

		//Continuous
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ContinuousFocusOptions));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV10Rsp              = (struct _timg10__GetMoveOptionsResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV10Rsp->MoveOptions = (struct tt__MoveOptions *)ppTmpMem[iNum++];

		ptMoveOptions = ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV10Rsp->MoveOptions;

		ptMoveOptions->Absolute           = (struct tt__AbsoluteFocusOptions *)ppTmpMem[iNum++];
		ptMoveOptions->Absolute->Position = (struct tt__FloatRange *)ppTmpMem[iNum++];
		ptMoveOptions->Absolute->Speed    = (struct tt__FloatRange *)ppTmpMem[iNum++];

		ptMoveOptions->Relative           = (struct tt__RelativeFocusOptions *)ppTmpMem[iNum++];
		ptMoveOptions->Relative->Distance = (struct tt__FloatRange *)ppTmpMem[iNum++];
		ptMoveOptions->Relative->Speed    = (struct tt__FloatRange *)ppTmpMem[iNum++];

		ptMoveOptions->Continuous         = (struct tt__ContinuousFocusOptions *)ppTmpMem[iNum++];
		ptMoveOptions->Continuous->Speed  = (struct tt__FloatRange *)ppTmpMem[iNum++];

		//全局变量初始化
		ptMoveOptions->Absolute->Position->Max = 1.0;
		ptMoveOptions->Absolute->Position->Min = 0;
		ptMoveOptions->Absolute->Speed->Max    = 1.0;
		ptMoveOptions->Absolute->Speed->Min    = 0;

		ptMoveOptions->Relative->Distance->Max = 1.0;
		ptMoveOptions->Relative->Distance->Min = -1.0;
		ptMoveOptions->Relative->Speed->Max = 1.0;
		ptMoveOptions->Relative->Speed->Min = 0;

		ptMoveOptions->Continuous->Speed->Max = 1.0;
		ptMoveOptions->Continuous->Speed->Min = -1.0;
	}	
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120301 v1.0 creat by ptb
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__timg20__GetImagingSettings(void *_pThis)
{	
	struct tt__ImagingSettings20 *ptImgSetting = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MAX] = {0};
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV20Rsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _timg20__GetImagingSettingsResponse));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ImagingSettings20));			

		//BacklightCompensation
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__BacklightCompensation20));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			

		//Brightness
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			

		//ColorSaturation
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			

		//Contrast
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			

		//Exposure
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Exposure20));			
		ppTmpMem[iNum++] = calloc(1, sizeof(enum tt__ExposurePriority));			
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Rectangle));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			

		//Focus
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FocusConfiguration20));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			

		//IrCutFilter
		ppTmpMem[iNum++] = calloc(1, sizeof(enum tt__IrCutFilterMode));			

		//Sharpness
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			

		//WideDynamicRange
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WideDynamicRange20));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			

		//WhiteBalance
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WhiteBalance20));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));			
		ppTmpMem[iNum++] = calloc(1, sizeof(float));

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV20Rsp = (struct _timg20__GetImagingSettingsResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV20Rsp->ImagingSettings = (struct tt__ImagingSettings20 *)ppTmpMem[iNum++];

		ptImgSetting = ptOnvifCtrl->m_stOnvifCfg.m_ptImgSettingsV20Rsp->ImagingSettings;
		
		//BacklightCompensation
		ptImgSetting->BacklightCompensation = (struct tt__BacklightCompensation20 *)ppTmpMem[iNum++];
		ptImgSetting->BacklightCompensation->Level = (float *)ppTmpMem[iNum++];

		//Brightness
		ptImgSetting->Brightness = (float *)ppTmpMem[iNum++];

		//ColorSaturation
		ptImgSetting->ColorSaturation = (float *)ppTmpMem[iNum++];

		//Contrast
		ptImgSetting->Contrast = (float *)ppTmpMem[iNum++];

		//Exposure
		ptImgSetting->Exposure = (struct tt__Exposure20 *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Priority        = (enum tt__ExposurePriority *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window          = (struct tt__Rectangle *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window->bottom  = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window->top     = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window->right   = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Window->left    = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->MinExposureTime = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->MaxExposureTime = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->MinGain         = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->MaxGain         = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->MinIris         = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->MaxIris         = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->ExposureTime    = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Gain            = (float *)ppTmpMem[iNum++];
		ptImgSetting->Exposure->Iris            = (float *)ppTmpMem[iNum++];

		//Focus
		ptImgSetting->Focus = (struct tt__FocusConfiguration20 *)ppTmpMem[iNum++];
		ptImgSetting->Focus->DefaultSpeed = (float *)ppTmpMem[iNum++];
		ptImgSetting->Focus->NearLimit    = (float *)ppTmpMem[iNum++];
		ptImgSetting->Focus->FarLimit     = (float *)ppTmpMem[iNum++];

		//IrCutFilter
		ptImgSetting->IrCutFilter = (enum tt__IrCutFilterMode *)ppTmpMem[iNum++];

		//Sharpness
		ptImgSetting->Sharpness = (float *)ppTmpMem[iNum++];

		//WideDynamicRange
		ptImgSetting->WideDynamicRange = (struct tt__WideDynamicRange20 *)ppTmpMem[iNum++];
		ptImgSetting->WideDynamicRange->Level = (float *)ppTmpMem[iNum++];
		
		//WhiteBalance
		ptImgSetting->WhiteBalance = (struct tt__WhiteBalance20 *)ppTmpMem[iNum++];
		ptImgSetting->WhiteBalance->CrGain = (float *)ppTmpMem[iNum++];
		ptImgSetting->WhiteBalance->CbGain = (float *)ppTmpMem[iNum++];
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120305 v1.0 creat by ptb
			20130226 v2.3.0 modify by ptb
**************************************************/
int GMemInit__timg20__GetOptions(void *_pThis)
{	
	struct tt__ImagingOptions20 *ptImgOption = NULL;
	void *ppTmpMem[MEM_LEN_HUGE] = {0};//下面有for循环，其他地方类似，修改请注意calloc时是否越界，by qiaohaijun 2013.12.04
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iIRcutModeNum = 3;
	int iModeNum = 2;
	int iVSNum = 0;
	int iNum   = 0;
	int i      = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _timg20__GetOptionsResponse));					

		iVSNum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
		//ImagingOptions
		ppTmpMem[iNum++] = calloc(iVSNum, sizeof(struct tt__ImagingOptions20));						

		for(i = 0; i < iVSNum; i++)
		{
			//BacklightCompensation
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__BacklightCompensationOptions20));	
			//Mode
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__BacklightCompensationMode));		
			//Level
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			//Brightness
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));						

			//ColorSaturation
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));						

			//Contrast
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));						

			//Exposure
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ExposureOptions20));					
			//Mode
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__ExposureMode));					
			//Priority
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__ExposurePriority));					
			//MinExposureTime
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//MaxExposureTime
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//MinGain
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//MaxGain
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//MinIris
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//MaxIris
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//ExposureTime
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//Gain
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//Iris
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			//Focus
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FocusOptions20));					
			//AutoFocusModes
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__AutoFocusMode));			
			//DefaultSpeed
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//NearLimit
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//FarLimit
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			//IrCutFilterModes
			ppTmpMem[iNum++] = calloc(1, iIRcutModeNum * sizeof(enum tt__IrCutFilterMode));						

			//Sharpness
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));						

			//WideDynamicRange
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WideDynamicRangeOptions20));			
			//Mode
			ppTmpMem[iNum++] = calloc(1, iModeNum * sizeof(enum tt__WideDynamicMode));					
			//Level
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			//WhiteBalance
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WhiteBalanceOptions20));				
			//Mode
			ppTmpMem[iNum++] = calloc(1, iModeNum *sizeof(enum tt__WhiteBalanceMode));				
			//YrGain
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
			//YbGain
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

			
			if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
			{
				td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
				FreeAllTmpMem(ppTmpMem, iNum);
				return TD_ERROR;
			}
				
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp = (struct _timg20__GetOptionsResponse *)ppTmpMem[iNum++];
		//ImagingOptions
		ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions = (struct tt__ImagingOptions20 *)ppTmpMem[iNum++];

		ptImgOption = ptOnvifCtrl->m_stOnvifCfg.m_ptOptionsV20Rsp->ImagingOptions;

		for(i = 0; i < iVSNum; i++)
		{
			//BacklightCompensation
			ptImgOption[i].BacklightCompensation = (struct tt__BacklightCompensationOptions20 *)ppTmpMem[iNum++];
			ptImgOption[i].BacklightCompensation->Mode = (enum tt__BacklightCompensationMode *)ppTmpMem[iNum++];
			ptImgOption[i].BacklightCompensation->Level = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//Brightness
			ptImgOption[i].Brightness = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//ColorSaturation
			ptImgOption[i].ColorSaturation = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//Contrast
			ptImgOption[i].Contrast = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//Exposure
			ptImgOption[i].Exposure = (struct tt__ExposureOptions20 *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->Mode = (enum tt__ExposureMode *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->Priority = (enum tt__ExposurePriority *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->MinExposureTime = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->MaxExposureTime = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->MinGain = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->MaxGain = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->MinIris = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->MaxIris = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->ExposureTime = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->Gain = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Exposure->Iris = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//Focus
			ptImgOption[i].Focus = (struct tt__FocusOptions20 *)ppTmpMem[iNum++];
			ptImgOption[i].Focus->AutoFocusModes = (enum tt__AutoFocusMode *)ppTmpMem[iNum++];
			ptImgOption[i].Focus->DefaultSpeed = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Focus->NearLimit = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].Focus->FarLimit = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//IrCutFilterModes
			ptImgOption[i].IrCutFilterModes = (enum tt__IrCutFilterMode *)ppTmpMem[iNum++];

			//Sharpness
			ptImgOption[i].Sharpness = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//WideDynamicRange
			ptImgOption[i].WideDynamicRange = (struct tt__WideDynamicRangeOptions20 *)ppTmpMem[iNum++];
			ptImgOption[i].WideDynamicRange->Mode = (enum tt__WideDynamicMode *)ppTmpMem[iNum++];
			ptImgOption[i].WideDynamicRange->Level = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//WhiteBalance
			ptImgOption[i].WhiteBalance = (struct tt__WhiteBalanceOptions20 *)ppTmpMem[iNum++];
			ptImgOption[i].WhiteBalance->Mode = (enum tt__WhiteBalanceMode *)ppTmpMem[iNum++];
			ptImgOption[i].WhiteBalance->YrGain = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptImgOption[i].WhiteBalance->YbGain = (struct tt__FloatRange *)ppTmpMem[iNum++];
		}

		//初始化全局变量
		for(i = 0; i < iVSNum; i++)
		{
			//BacklightCompensation
			ptImgOption[i].BacklightCompensation->__sizeMode  = iModeNum;
			*ptImgOption[i].BacklightCompensation->Mode       = tt__BacklightCompensationMode__OFF;
			*(ptImgOption[i].BacklightCompensation->Mode + 1) = tt__BacklightCompensationMode__ON;
			ptImgOption[i].BacklightCompensation->Level->Min  = VSIOBCOLEVMIN;
			ptImgOption[i].BacklightCompensation->Level->Max  = VSIOBCOLEVMAX;

			//Brightness
			ptImgOption[i].Brightness->Min = VSIOBCOBNMIN;
			ptImgOption[i].Brightness->Max = VSIOBCOBNMAX;

			//ColorSaturation
			ptImgOption[i].ColorSaturation->Min = VSIOBCOCSMIN;
			ptImgOption[i].ColorSaturation->Max = VSIOBCOCSMAX;

			//Contrast
			ptImgOption[i].Contrast->Min = VSIOBCOCTMIN;
			ptImgOption[i].Contrast->Max = VSIOBCOCTMAX;

			//Exposure
			ptImgOption[i].Exposure->__sizeMode      = iModeNum;
			*ptImgOption[i].Exposure->Mode           = tt__ExposureMode__AUTO;
			*(ptImgOption[i].Exposure->Mode + 1)     = tt__ExposureMode__MANUAL;
			ptImgOption[i].Exposure->__sizeMode      = iModeNum;
			*ptImgOption[i].Exposure->Priority       = tt__ExposurePriority__LowNoise;
			*(ptImgOption[i].Exposure->Priority + 1) = tt__ExposurePriority__FrameRate;

			ptImgOption[i].Exposure->MinExposureTime->Min = VSIOEOMINETMIN;
			ptImgOption[i].Exposure->MinExposureTime->Max = VSIOEOMINETMAX;
			ptImgOption[i].Exposure->MaxExposureTime->Min = VSIOEOMAXETMIN;
			ptImgOption[i].Exposure->MaxExposureTime->Max = VSIOEOMAXETMAX;
			ptImgOption[i].Exposure->MinGain->Min = VSIOEOMINGMIN;
			ptImgOption[i].Exposure->MinGain->Max = VSIOEOMINGMAX;
			ptImgOption[i].Exposure->MaxGain->Min = VSIOEOMAXGMIN;
			ptImgOption[i].Exposure->MaxGain->Max = VSIOEOMAXGMAX;
			ptImgOption[i].Exposure->MinIris->Min = VSIOEOMINIMIN;
			ptImgOption[i].Exposure->MinIris->Max = VSIOEOMINIMAX;
			ptImgOption[i].Exposure->MaxIris->Min = VSIOEOMAXIMIN;
			ptImgOption[i].Exposure->MaxIris->Max = VSIOEOMAXIMAX;
			ptImgOption[i].Exposure->ExposureTime->Min = VSIOEOETMIN;
			ptImgOption[i].Exposure->ExposureTime->Max = VSIOEOETMAX;
			ptImgOption[i].Exposure->Gain->Min = VSIOEOGMIN;
			ptImgOption[i].Exposure->Gain->Max = VSIOEOGMAX;
			ptImgOption[i].Exposure->Iris->Min = VSIOEOIMIN;
			ptImgOption[i].Exposure->Iris->Max = VSIOEOIMAX;

			//Focus
			ptImgOption[i].Focus->__sizeAutoFocusModes 	= iModeNum;
			*ptImgOption[i].Focus->AutoFocusModes 		= tt__AutoFocusMode__AUTO;
			*(ptImgOption[i].Focus->AutoFocusModes + 1) = tt__AutoFocusMode__MANUAL;
			ptImgOption[i].Focus->DefaultSpeed->Min 	= VSIOFODSMIN;
			ptImgOption[i].Focus->DefaultSpeed->Max 	= VSIOFODSMAX;
			ptImgOption[i].Focus->NearLimit->Min 		= VSIOFONLMIN;
			ptImgOption[i].Focus->NearLimit->Max 		= VSIOFONLMAX;
			ptImgOption[i].Focus->FarLimit->Min 		= VSIOFOFLMIN;
			ptImgOption[i].Focus->FarLimit->Max 		= VSIOFOFLMAX;

			//IrCutFilter
			ptImgOption[i].__sizeIrCutFilterModes 		= iIRcutModeNum;
			*ptImgOption[i].IrCutFilterModes 			= tt__IrCutFilterMode__ON;
			*(ptImgOption[i].IrCutFilterModes + 1) 		= tt__IrCutFilterMode__OFF;
			*(ptImgOption[i].IrCutFilterModes + 2)		= tt__IrCutFilterMode__AUTO;

			//Sharpness
			ptImgOption[i].Sharpness->Min = VSIOSNMIN;
			ptImgOption[i].Sharpness->Max = VSIOSNMAX;

			//WideDynamicRange
			ptImgOption[i].WideDynamicRange->__sizeMode = iModeNum;
			*ptImgOption[i].WideDynamicRange->Mode 		= tt__WideDynamicMode__OFF;
			*(ptImgOption[i].WideDynamicRange->Mode + 1)= tt__WideDynamicMode__ON;
			ptImgOption[i].WideDynamicRange->Level->Min = VSIOWDRLEVMIN;
			ptImgOption[i].WideDynamicRange->Level->Max = VSIOWDRLEVMAX;

			//WhiteBalance
			ptImgOption[i].WhiteBalance->__sizeMode 	= iModeNum;
			*ptImgOption[i].WhiteBalance->Mode 			= tt__WhiteBalanceMode__AUTO;
			*(ptImgOption[i].WhiteBalance->Mode + 1) 	= tt__WhiteBalanceMode__MANUAL;
			ptImgOption[i].WhiteBalance->YrGain->Min 	= VSIOWBYRGMIN;
			ptImgOption[i].WhiteBalance->YrGain->Max 	= VSIOWBYRGMAX;
			ptImgOption[i].WhiteBalance->YbGain->Min 	= VSIOWBYBGMIN;
			ptImgOption[i].WhiteBalance->YbGain->Max 	= VSIOWBYBGMAX;
		}
	}

	return SOAP_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120314 v1.0 creat by ptb
**************************************************/
int GMemInit__timg20__GetStatus(void *_pThis)
{	
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iNum = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV20Rsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _timg20__GetStatusResponse));					
		//Status
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ImagingStatus20));					
		//FocusStatus20
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FocusStatus20));					
		
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV20Rsp = (struct _timg20__GetStatusResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV20Rsp->Status = (struct tt__ImagingStatus20 *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV20Rsp->Status->FocusStatus20 = (struct tt__FocusStatus20 *)ppTmpMem[iNum++];

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV20Rsp->Status->FocusStatus20->Position   = 0.0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV20Rsp->Status->FocusStatus20->MoveStatus = tt__MoveStatus__UNKNOWN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusV20Rsp->Status->FocusStatus20->Error      = NULL;
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120314 v1.0 creat by ptb
**************************************************/
int GMemInit__timg20__GetMoveOptions(void *_pThis)
{	
	struct tt__MoveOptions20 *ptMoveOption = NULL;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iNum = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV20Rsp))
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _timg20__GetMoveOptionsResponse));					
		//MoveOptions
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__MoveOptions20));					

		//Absolute
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__AbsoluteFocusOptions));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

		//Relative
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__RelativeFocusOptions20));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

		//Continuous
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ContinuousFocusOptions));					
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));					

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV20Rsp = (struct _timg20__GetMoveOptionsResponse *)ppTmpMem[iNum++];
		//MoveOptions
		ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV20Rsp->MoveOptions = (struct tt__MoveOptions20 *)ppTmpMem[iNum++];

		ptMoveOption = ptOnvifCtrl->m_stOnvifCfg.m_ptMoveOptionsV20Rsp->MoveOptions;
		
		//Absolute
		ptMoveOption->Absolute = (struct tt__AbsoluteFocusOptions *)ppTmpMem[iNum++];
		ptMoveOption->Absolute->Position = (struct tt__FloatRange *)ppTmpMem[iNum++];
		ptMoveOption->Absolute->Speed    = (struct tt__FloatRange *)ppTmpMem[iNum++];

		//Relative
		ptMoveOption->Relative = (struct tt__RelativeFocusOptions20 *)ppTmpMem[iNum++];
		ptMoveOption->Relative->Distance = (struct tt__FloatRange *)ppTmpMem[iNum++];
		ptMoveOption->Relative->Speed    = (struct tt__FloatRange *)ppTmpMem[iNum++];

		//Continuous
		ptMoveOption->Continuous = (struct tt__ContinuousFocusOptions *)ppTmpMem[iNum++];
		ptMoveOption->Continuous->Speed = (struct tt__FloatRange *)ppTmpMem[iNum++];

		//初始化
		ptMoveOption->Absolute->Position->Max = 1.0;
		ptMoveOption->Absolute->Position->Min = 0;
		ptMoveOption->Absolute->Speed->Max    = 1.0;
		ptMoveOption->Absolute->Speed->Min    = 0;
		ptMoveOption->Relative->Distance->Max = 1.0;
		ptMoveOption->Relative->Distance->Min = -1.0;
		ptMoveOption->Relative->Speed->Max    = 1.0;
		ptMoveOption->Relative->Speed->Min    = 0;
		ptMoveOption->Continuous->Speed->Max  = 1.0;
		ptMoveOption->Continuous->Speed->Min  = -1.0;
	}	
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tptz20__GetConfigurations(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM]  = {0};
	char *pcTmp = NULL;
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}	

	iSize = ptOnvifCtrl->m_stNvsInfo.m_iVINum;

	if(NULL == ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp)
	{
		//malloc memeory
 		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tptz20__GetConfigurationsResponse));					
 		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__PTZConfiguration));					

		for(i = 0; i < iSize; i++)
		{
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PTZSpeed));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Vector2D));
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Vector1D));
			ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
			ppTmpMem[iNum++] = (LONG64 *)calloc(1, sizeof(LONG64));
		}
		
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		//m_pwsnt__SubscribeResponse
		ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp = (struct _tptz20__GetConfigurationsResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration = (struct tt__PTZConfiguration *)ppTmpMem[iNum++];

		for(i = 0; i < iSize; i++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].Name      = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].token     = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].NodeToken = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultRelativePanTiltTranslationSpace = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultRelativeZoomTranslationSpace    = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultContinuousPanTiltVelocitySpace  = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultContinuousZoomVelocitySpace     = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed = (struct tt__PTZSpeed *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->PanTilt        = (struct tt__Vector2D *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->PanTilt->space = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->Zoom           = (struct tt__Vector1D *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->Zoom->space    = (char *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZTimeout = (LONG64 *)ppTmpMem[iNum++];
		}

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->__sizePTZConfiguration = iSize;
		for(i = 0; i < iSize; i++)
		{
			pcTmp = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzctoken_ch", i+1, "ptzcname", -1, NULL);
			if(NULL != pcTmp)
			{
				strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].Name, pcTmp);
			}
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].UseCount = 1;

			pcTmp = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzctoken_ch", i + 1, "ptzctoken", -1, NULL);
			if(NULL != pcTmp)
			{
				strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].token, pcTmp);
			}

			pcTmp = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzctoken_ch", i+1, "ptzcndtoken", -1, NULL);
			if(NULL != pcTmp)
			{
				strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].NodeToken, pcTmp);
			}

			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultRelativePanTiltTranslationSpace, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultRelativeZoomTranslationSpace, "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultContinuousPanTiltVelocitySpace, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultContinuousZoomVelocitySpace, "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");

			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->PanTilt->x = 1.0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->PanTilt->y = 1.0;
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->PanTilt->space, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");
			ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->Zoom->x = 1.0;
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZSpeed->Zoom->space, "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");

			//DefaultPTZTimeout单位是毫秒
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[i].DefaultPTZTimeout) = (enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "ptzctoken_ch", i+1, "ptzcdto", -1, 15) * 1000;
		}
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20130307 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tptz20__GetPresets(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM]  = {0};
	char *pName    = NULL;
	int iPresetNum[MAX_CHANNEL_NUM] = {0};
	int iSize      = 0;
	int iNum       = 0;
	int i = 0;
	int j = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stNvsInfo.m_iVINum;

	//确定每个通道的预置点个数
	for(i = 0; i < iSize; i++)
	{
		iPresetNum[i] = (enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", i + 1, "presetcnt", 0, 0);
		if(iPresetNum[i] > PRESET_MAX_NUM)
		{
			td_printf(0, "%s %d %s:Preset number is too many[chn = %d %d]!\n", __FILE__, __LINE__, __FUNCTION__, i, iPresetNum[i]);
			return TD_ERROR;
		}
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp))
	{
		//malloc memeory
 		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct _tptz20__GetPresetsResponse));
		
		for(i = 0; i < iSize; i++)
		{
			if(iPresetNum[i] > 0)
			{
				ppTmpMem[iNum++] = calloc(iPresetNum[i], sizeof(struct tt__PTZPreset));
				for(j = 0; j < iPresetNum[i]; j++)
				{
					ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
					ppTmpMem[iNum++] = calloc(1, MAX_STRING_LEN);
				}
			}
		}
		
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
		
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp = (struct _tptz20__GetPresetsResponse *)ppTmpMem[iNum++];
		for(i = 0; i < iSize; i++)
		{
			if(iPresetNum[i] > 0)
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[i].Preset = (struct tt__PTZPreset *)ppTmpMem[iNum++];
				for(j = 0; j < iPresetNum[i]; j++)
				{

					ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[i].Preset[j].Name  = (char *)ppTmpMem[iNum++];
					ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[i].Preset[j].token = (char *)ppTmpMem[iNum++];
				}
			}
		}

		//全局变量初始化
		for(i = 0; i < iSize; i++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[i].__sizePreset = iPresetNum[i];

			for(j = 0; j < iPresetNum[i]; j++)
			{
				pName = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "ptzpresets_ch", i + 1, "presetnum_", j + PRESET_HOME, NULL);
				if(!pointer_valid_check(pName))
				{
					memcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[i].Preset[j].Name, pName, MAX_STRING_LEN);
					sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptPresetsRsp[i].Preset[j].token, "presetnum_%02d", j + PRESET_HOME);
				}
			}
		}
	}	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20130221 v2.3 modify by ptb
**************************************************/
int GMemInit__tptz20__GetStatus(void *_pThis)
{	
	void *ppTmpMem[MEM_LEN_MAX] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp))
	{
		//malloc memeory
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tptz20__GetStatusResponse));	

		iSize = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__PTZStatus)); 				

		for(i = 0; i < iSize; i++)
		{
			//Position
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PTZVector)); 				
			//tt__Vector2D
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Vector2D));					
			//tt__Vector1D
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Vector1D));					
			//MoveStatus
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PTZMoveStatus)); 				
			//PanTilt
			ppTmpMem[iNum++] = calloc(1, sizeof(enum tt__MoveStatus));	
			//Zoom
			ppTmpMem[iNum++] = calloc(1, sizeof(enum tt__MoveStatus));	
		}

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp = (struct _tptz20__GetStatusResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus = (struct tt__PTZStatus *)ppTmpMem[iNum++];

		for(i = 0; i < iSize; i++)
		{
			//Position
			ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].Position = (struct tt__PTZVector *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].Position->PanTilt = (struct tt__Vector2D *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].Position->Zoom = (struct tt__Vector1D *)ppTmpMem[iNum++];
			
			//MoveStatus
			ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].MoveStatus = (struct tt__PTZMoveStatus *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].MoveStatus->PanTilt = (enum tt__MoveStatus *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].MoveStatus->Zoom = (enum tt__MoveStatus *)ppTmpMem[iNum++];

			//全局变量初始化
			*ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].MoveStatus->PanTilt = tt__MoveStatus__IDLE;
			*ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].MoveStatus->Zoom    = tt__MoveStatus__IDLE;
			ptOnvifCtrl->m_stOnvifCfg.m_ptStatusRsp->PTZStatus[i].UtcTime              = time(NULL);
		}
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20130307 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tptz20__GetNodes(void *_pThis)
{	
	struct tt__PTZNode *ptPTZNode = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_HUGE]  = {0};
	int iAuxiliaryNum = 6;
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;
	int j     = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

		
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptNodesRsp))
	{
 		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tptz20__GetNodesResponse));					

		iSize = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
 		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__PTZNode));					

		for(i = 0; i < iSize; i++)
		{
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PTZSpaces));

			//AbsolutePanTiltPositionSpace
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Space2DDescription));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));

			//AbsoluteZoomPositionSpace
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Space1DDescription));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));

			//RelativePanTiltTranslationSpace
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Space2DDescription));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));

			//RelativeZoomTranslationSpace
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Space1DDescription));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));

			//ContinuousPanTiltVelocitySpace
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Space2DDescription));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));

			//ContinuousZoomVelocitySpace
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Space1DDescription));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));

			//PanTiltSpeedSpace
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Space1DDescription));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));

			//ZoomSpeedSpace
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Space1DDescription));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MID);					
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FloatRange));

			ppTmpMem[iNum++] = calloc(1, iAuxiliaryNum * sizeof(char *));

			//AuxiliaryCommands
			for(j = 0; j < iAuxiliaryNum; j++)
			{
				ppTmpMem[iNum++] = calloc(1, MEM_LEN_MINI);
			}
		}

		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}
		
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNodesRsp = (struct _tptz20__GetNodesResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptNodesRsp->PTZNode = (struct tt__PTZNode *)ppTmpMem[iNum++];

		ptPTZNode = ptOnvifCtrl->m_stOnvifCfg.m_ptNodesRsp->PTZNode;
		for(i = 0; i < iSize; i++)
		{
			ptPTZNode[i].token = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].Name = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces = (struct tt__PTZSpaces *)ppTmpMem[iNum++];

			//AbsolutePanTiltPositionSpace
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace = (struct tt__Space2DDescription *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->URI = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//AbsoluteZoomPositionSpace
			ptPTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace = (struct tt__Space1DDescription *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->URI = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//RelativePanTiltTranslationSpace
			ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace = (struct tt__Space2DDescription *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->URI = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//RelativeZoomTranslationSpace
			ptPTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace = (struct tt__Space1DDescription *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->URI = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//ContinuousPanTiltVelocitySpace
			ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace = (struct tt__Space2DDescription *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->URI = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange = (struct tt__FloatRange *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//ContinuousZoomVelocitySpace
			ptPTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace = (struct tt__Space1DDescription *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->URI = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//PanTiltSpeedSpace
			ptPTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace = (struct tt__Space1DDescription *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->URI = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->XRange = (struct tt__FloatRange *)ppTmpMem[iNum++];

			//ZoomSpeedSpace
			ptPTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace = (struct tt__Space1DDescription *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->URI = (char *)ppTmpMem[iNum++];
			ptPTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->XRange = (struct tt__FloatRange *)ppTmpMem[iNum++];
			
			ptPTZNode[i].AuxiliaryCommands = (char **)ppTmpMem[iNum++];
			for(j = 0; j < iAuxiliaryNum; j++)
			{
				*(ptPTZNode[i].AuxiliaryCommands + j) = (char *)ppTmpMem[iNum++];
			}
		}		

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptNodesRsp->__sizePTZNode = iSize;
		for(i = 0; i < iSize; i++)
		{
			sprintf(ptPTZNode[i].token, "node_%02d", i + 1);
			sprintf(ptPTZNode[i].Name, "node_%02d", i + 1);

			//水平坐标，范围: 0~36000
			//垂直坐标，范围: -1500~9000
			//变倍大小，范围: 10-500

			//SupportedPTZSpaces
			//AbsolutePanTiltPositionSpace
			ptPTZNode[i].SupportedPTZSpaces->__sizeAbsolutePanTiltPositionSpace = 1;
			strcpy(ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->URI, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
		#if 0
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Min = -1.0;
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Max = 1.0;
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Min = -1.0;
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Max = 1.0;
		#else
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Min = PTZ_ABS_X_MIN;
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Max = PTZ_ABS_X_MAX;
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Min = PTZ_ABS_Y_MIN;
			ptPTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Max = PTZ_ABS_Y_MAX;

		#endif

			//AbsoluteZoomPositionSpace
			ptPTZNode[i].SupportedPTZSpaces->__sizeAbsoluteZoomPositionSpace = 1;
			strcpy(ptPTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->URI, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
			ptPTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange->Min = PTZ_ABS_Z_MIN;
			ptPTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange->Max = PTZ_ABS_Z_MAX;

			//RelativePanTiltTranslationSpace
			ptPTZNode[i].SupportedPTZSpaces->__sizeRelativePanTiltTranslationSpace = 1;
			strcpy(ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->URI, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
			ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Min = -1.0;
			ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Max = 1.0;
			ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Min = -1.0;
			ptPTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Max = 1.0;

			//RelativeZoomTranslationSpace
			ptPTZNode[i].SupportedPTZSpaces->__sizeRelativeZoomTranslationSpace = 1;
			strcpy(ptPTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->URI, "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
			ptPTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange->Min = -1.0;
			ptPTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange->Max = 1.0;

			//ContinuousPanTiltVelocitySpace
			ptPTZNode[i].SupportedPTZSpaces->__sizeContinuousPanTiltVelocitySpace = 1;
			strcpy(ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->URI, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");
			ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange->Min = -1.0;
			ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange->Max = 1.0;
			ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange->Min = -1.0;
			ptPTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange->Max = 1.0;

			//ContinuousZoomVelocitySpace
			ptPTZNode[i].SupportedPTZSpaces->__sizeContinuousZoomVelocitySpace = 1;
			strcpy(ptPTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->URI, "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");
			ptPTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange->Min = -1.0;
			ptPTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange->Max = 1.0;

			//PanTiltSpeedSpace
			ptPTZNode[i].SupportedPTZSpaces->__sizePanTiltSpeedSpace = 1;
			strcpy(ptPTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->URI, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");
			ptPTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->XRange->Min = 0.0;
			ptPTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->XRange->Max = 1.0;

			//ZoomSpeedSpace
			ptPTZNode[i].SupportedPTZSpaces->__sizeZoomSpeedSpace = 1;
			strcpy(ptPTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->URI, "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
			ptPTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->XRange->Min = 0.0;
			ptPTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->XRange->Max = 1.0;

			ptPTZNode[i].MaximumNumberOfPresets = PRESET_MAX_NUM;
			ptPTZNode[i].HomeSupported = xsd__boolean__true_;
			ptPTZNode[i].__sizeAuxiliaryCommands = iAuxiliaryNum;

			strcpy(*ptPTZNode[i].AuxiliaryCommands, "AssOn");
			strcpy(*(ptPTZNode[i].AuxiliaryCommands + 1), "AssOff");
			strcpy(*(ptPTZNode[i].AuxiliaryCommands + 2), "LightOn");
			strcpy(*(ptPTZNode[i].AuxiliaryCommands + 3), "LightOff");
			strcpy(*(ptPTZNode[i].AuxiliaryCommands + 4), "WipeStart");
			strcpy(*(ptPTZNode[i].AuxiliaryCommands + 5), "WipeStop");
		}
	}

	return TD_OK;  
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20130307 v2.3.0 modify by ptb
**************************************************/
int GMemInit__tptz20__GetConfigurationOptions(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM]  = {0};
	int iNum = 0;
	int iSize = 0;
	int i = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stNvsInfo.m_iVINum;

	if(NULL == ptOnvifCtrl->m_stOnvifCfg.m_ptCfgOptionsRsp)
	{
		//malloc memeory
 		ppTmpMem[iNum++] = calloc(1, sizeof(struct _tptz20__GetConfigurationOptionsResponse));					
 		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__PTZConfigurationOptions));

		for(i = 0; i < iSize; i++)
		{
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__DurationRange)); 
		}

		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptCfgOptionsRsp = (struct _tptz20__GetConfigurationOptionsResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptCfgOptionsRsp->PTZConfigurationOptions = (struct tt__PTZConfigurationOptions *)ppTmpMem[iNum++];

		for(i = 0; i < iSize; i++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptCfgOptionsRsp->PTZConfigurationOptions[i].PTZTimeout = (struct tt__DurationRange *)ppTmpMem[iNum++];
		}

		//全局变量初始化
		for(i = 0; i < iSize; i++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptCfgOptionsRsp->PTZConfigurationOptions[i].PTZTimeout->Min = 10000;
			ptOnvifCtrl->m_stOnvifCfg.m_ptCfgOptionsRsp->PTZConfigurationOptions[i].PTZTimeout->Max = 100000;
			ptOnvifCtrl->m_stOnvifCfg.m_ptCfgOptionsRsp->PTZConfigurationOptions[i].Spaces = ptOnvifCtrl->m_stOnvifCfg.m_ptNodesRsp->PTZNode[i].SupportedPTZSpaces;
		}

	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120314 v1.0 creat by ptb
**************************************************/
int GMemInit__trt__GetVideoSources(void *_pThis)
{	
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iVSNum = 0;
	int iNum   = 0;
	int i      = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iVSNum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	td_printf(0, "%s %d %s iVSNum:%d\n", __FILE__, __LINE__, __FUNCTION__, iVSNum);

	if(NULL == ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp)
	{	
		//m_ptrt__GetVideoSourcesResponse
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetVideoSourcesResponse));				
		//VideoSources
		ppTmpMem[iNum++] = calloc(iVSNum, sizeof(struct tt__VideoSource));					

		for(i = 0; i < iVSNum; i++)
		{
			//malloc memeory
			//token
			ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));					

			//Resolution
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__VideoResolution));					

			//Imaging
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__ImagingSettings));					
				//BacklightCompensation
				ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__BacklightCompensation)); 				
				//Brightness
				ppTmpMem[iNum++] = calloc(1, sizeof(float));					
				//ColorSaturation
				ppTmpMem[iNum++] = calloc(1, sizeof(float));					
				//Contrast
				ppTmpMem[iNum++] = calloc(1, sizeof(float));					
				//Exposure
				ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Exposure));
					//Window
					ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__Rectangle));
						//bottom
						ppTmpMem[iNum++] = calloc(1, sizeof(float));					
						//top
						ppTmpMem[iNum++] = calloc(1, sizeof(float));					
						//right
						ppTmpMem[iNum++] = calloc(1, sizeof(float));					
						//left
						ppTmpMem[iNum++] = calloc(1, sizeof(float));
				//Focus
				ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__FocusConfiguration));					
				//IrCutFilter
				ppTmpMem[iNum++] = calloc(1, sizeof(enum tt__IrCutFilterMode)); 				
				//Sharpness
				ppTmpMem[iNum++] = calloc(1, sizeof(float));					
				//WideDynamicRange
				ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WideDynamicRange));					
				//WhiteBalance
				ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__WhiteBalance));					
		}
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		//m_ptrt__GetVideoSourcesResponse
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp = \
			(struct _trt__GetVideoSourcesResponse *)ppTmpMem[iNum++];

		//VideoSources
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources = \
			(struct tt__VideoSource *)ppTmpMem[iNum++];

		for(i = 0; i < iVSNum; i++)
		{
			//token
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token = \
				(char *)ppTmpMem[iNum++];

			//Resolution
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Resolution = \
				(struct tt__VideoResolution *)ppTmpMem[iNum++];
			
			//Imaging
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging = \
				(struct tt__ImagingSettings *)ppTmpMem[iNum++];
			//BacklightCompensation
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->BacklightCompensation = \
				(struct tt__BacklightCompensation *)ppTmpMem[iNum++];
			//Brightness
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Brightness = \
				(float *)ppTmpMem[iNum++];
			//ColorSaturation
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->ColorSaturation = \
				(float *)ppTmpMem[iNum++];
			//Contrast
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Contrast = \
				(float *)ppTmpMem[iNum++];
			//Exposure
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure = \
				(struct tt__Exposure *)ppTmpMem[iNum++];
			//Window
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window = \
				(struct tt__Rectangle *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window->bottom = \
				(float *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window->top = \
				(float *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window->right = \
				(float *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window->left = \
				(float *)ppTmpMem[iNum++];
			//Focus
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus = \
				(struct tt__FocusConfiguration *)ppTmpMem[iNum++];
			//IrCutFilter
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->IrCutFilter = \
				(enum tt__IrCutFilterMode *)ppTmpMem[iNum++];
			//Sharpness
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Sharpness = \
				(float *)ppTmpMem[iNum++];
			//WideDynamicRange
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WideDynamicRange = \
				(struct tt__WideDynamicRange *)ppTmpMem[iNum++];
			//WhiteBalance
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WhiteBalance = \
				(struct tt__WhiteBalance *)ppTmpMem[iNum++];
		}

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources = iVSNum;
		for(i = 0; i < iVSNum; i++)
		{
			//token
			sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].token, "vstoken_ch%02d", i + 1);

			//Framerate
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Framerate = VSFREAMRATE;

			//Resolution
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Resolution->Width  = VSVRWIDTH;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Resolution->Height = 240;//VSVRHEIGHT;

			//Imaging
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->BacklightCompensation->Mode  = tt__BacklightCompensationMode__OFF;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->BacklightCompensation->Level = 0;

			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Mode              = tt__ExposureMode__AUTO;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Priority          = tt__ExposurePriority__LowNoise;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window->bottom) = VSVRHEIGHT;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window->top)    = 0;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window->right)  = VSVRWIDTH;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Window->left)   = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MinExposureTime   = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MaxExposureTime   = 1;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MinGain           = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MaxGain           = 1;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MinIris           = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->MaxIris           = 1;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->ExposureTime      = 1;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Gain              = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Exposure->Iris              = 1;

			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->AutoFocusMode    = tt__AutoFocusMode__AUTO;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->DefaultSpeed     = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->NearLimit        = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Focus->FarLimit         = 0;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->IrCutFilter)          = tt__IrCutFilterMode__ON;
			*(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->Sharpness)            = 0;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WideDynamicRange->Mode  = tt__WideDynamicMode__OFF;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WideDynamicRange->Level = 1;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WhiteBalance->Mode      = tt__WhiteBalanceMode__AUTO;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WhiteBalance->CrGain    = 1;
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[i].Imaging->WhiteBalance->CbGain    = 1;
		}

	}	

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120315 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetAudioSources(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stNvsInfo.m_iAINum;

	if(iSize <= 0)
	{
		td_printf(0, "iSize:%d\n", iSize);
		return TD_ERROR;
	}

	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetAudioSourcesResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__AudioSource));

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
	}
	
	//check memory
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp = (struct _trt__GetAudioSourcesResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->AudioSources = (struct tt__AudioSource *)ppTmpMem[iNum++];

	for(i = 0; i< iSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->AudioSources[i].token = (char *)ppTmpMem[iNum++];
	}

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources = iSize;
	for(i = 0; i < iSize; i++)
	{
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->AudioSources[i].token, "astoken_ch%02d", i + 1);
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->AudioSources[i].Channels = ASCHN;
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120406 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetAudioOutputs(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM];
	int iAONum = 0;
	int iNum   = 0;
	int i      = 0;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iAONum = ptOnvifCtrl->m_stNvsInfo.m_iAONum;
	if(iAONum <= 0)
	{
		td_printf(0, "iAONum:%d\n", iAONum);
		return TD_ERROR;
	}

	//calloc memory
	iNum = 0;
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetAudioOutputsResponse));
	ppTmpMem[iNum++] = calloc(iAONum, sizeof(struct tt__AudioOutput));

	for(i = 0; i< iAONum; i++)
	{
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
	}

	//check memory
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
	}
	
	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp = (struct _trt__GetAudioOutputsResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp->AudioOutputs = (struct tt__AudioOutput *)ppTmpMem[iNum++];

	for(i = 0; i < iAONum; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp->AudioOutputs[i].token = (char *)ppTmpMem[iNum++];
	}

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp->__sizeAudioOutputs = iAONum;
	for(i = 0; i < iAONum; i++)
	{
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp->AudioOutputs[i].token, "aotoken_ch%02d", i + 1);
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120224 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetProfiles(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	char pcPro[MAX_INDEX_NUM][MAX_NAME_LEN] = {{0}};
	char *pTmpStr = NULL;
	int iProSize  = 0;
	int iCount    = 0;
	int iNum      = 0;
	int iSize     = 0;
	int i = 0;
	int j = 0;
	struct tt__VideoAnalyticsConfiguration *ptmp = NULL;
	struct tt__Profile *profiles = NULL;
	
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	//get profile number
	iProSize = 2;//(enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "media", 0, "profile_size", 0, 1);

	//malloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetProfilesResponse));
	ppTmpMem[iNum++] = calloc(iProSize, sizeof(struct tt__Profile));

	for(i = 0; i < iProSize; i++)
	{
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));

		//IPC支持海康设备报警added by qiaohaijun @2013.12.06
		ppTmpMem[iNum++] = (struct tt__VideoAnalyticsConfiguration *)calloc(1, sizeof(struct tt__VideoAnalyticsConfiguration));
		ppTmpMem[iNum++] = (struct tt__AnalyticsEngineConfiguration *)calloc(1, sizeof(struct tt__AnalyticsEngineConfiguration));
		ppTmpMem[iNum++] = (struct tt__Config *)calloc(1,  sizeof(struct tt__Config));
		ppTmpMem[iNum++] = (struct tt__ItemList *)calloc(1, sizeof(struct tt__ItemList));
		ppTmpMem[iNum++] = (struct _tt__ItemList_SimpleItem *)calloc(1, sizeof(struct _tt__ItemList_SimpleItem));
		ppTmpMem[iNum++] = (struct _tt__ItemList_ElementItem *)calloc(1, sizeof(struct _tt__ItemList_ElementItem));
		ppTmpMem[iNum++] = (struct tt__RuleEngineConfiguration *)calloc(1, sizeof(struct tt__RuleEngineConfiguration));
		ppTmpMem[iNum++] = (struct tt__Config *)malloc( sizeof(struct tt__Config));
		ppTmpMem[iNum++] = (struct tt__ItemList *)calloc(1, sizeof(struct tt__ItemList));
		ppTmpMem[iNum++] = (struct _tt__ItemList_SimpleItem *)calloc(4, sizeof(struct _tt__ItemList_SimpleItem));
		//end
	}

	//check memory
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp = (struct _trt__GetProfilesResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles = (struct tt__Profile *)ppTmpMem[iNum++];
	profiles = ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles;
	for(i = 0; i < iProSize; i++)
	{
		profiles[i].token = (char *)ppTmpMem[iNum++];
		profiles[i].Name = (char *)ppTmpMem[iNum++];
		profiles[i].fixed = (enum xsd__boolean *)ppTmpMem[iNum++];

		//for hikevent by qiaohaijun @2013.12.06
		profiles[i].VideoAnalyticsConfiguration = (struct tt__VideoAnalyticsConfiguration *)ppTmpMem[iNum++];
		profiles[i].VideoAnalyticsConfiguration->AnalyticsEngineConfiguration = (struct tt__AnalyticsEngineConfiguration *)ppTmpMem[iNum++];
		profiles[i].VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule = (struct tt__Config *)ppTmpMem[iNum++];
		profiles[i].VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters = (struct tt__ItemList *)ppTmpMem[iNum++];
		profiles[i].VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->SimpleItem = (struct _tt__ItemList_SimpleItem *)ppTmpMem[iNum++];
		profiles[i].VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->ElementItem = (struct _tt__ItemList_ElementItem *)ppTmpMem[iNum++];

		profiles[i].VideoAnalyticsConfiguration->RuleEngineConfiguration = (struct tt__RuleEngineConfiguration *)ppTmpMem[iNum++];
		profiles[i].VideoAnalyticsConfiguration->RuleEngineConfiguration->Rule = (struct tt__Config *)ppTmpMem[iNum++];
		profiles[i].VideoAnalyticsConfiguration->RuleEngineConfiguration->Rule->Parameters = (struct tt__ItemList *)ppTmpMem[iNum++];
		profiles[i].VideoAnalyticsConfiguration->RuleEngineConfiguration->Rule->Parameters->SimpleItem = (struct _tt__ItemList_SimpleItem *)ppTmpMem[iNum++];
		//end
	}

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->__sizeProfiles = iProSize;
	for(i = 0; i < MAX_INDEX_NUM; i++)
	{
		if(!FindProEntry(ptOnvifCtrl->m_ptOnvifDict, "profile_", i + 1))
		{
			continue;
		}
		else
		{
			sprintf(pcPro[i],"profile_%02d", i + 1);
			pTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, pcPro[i], 0, "protoken", 0, NULL);
			if(NULL != pTmpStr)
			{
				strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[iCount++].token, pTmpStr);
			}
			if(iCount >= iProSize)
			{
				break;
			}
		}
	}

	for(i = 0; i < iProSize; i++)
	{
		//name
		pTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token, 0, "proname", 0, NULL);
		if(NULL != pTmpStr)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].Name, pTmpStr);
		}
			td_printf(0, "%s,%d, iProSize = %d, ", __FILE__, __LINE__, iProSize);

		//fixed
		*ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].fixed =	\
			(enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token, 0, "fixed", 0, xsd__boolean__false_);

		//vsctoken
		pTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token, 0, "vsctoken", 0, NULL);
		if(NULL != pTmpStr)
		{
			iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations;
			for(j = 0; j < iSize; j++)
			{
				//获取匹配号j
				if(!strcmp(pTmpStr, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[j].token))
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].VideoSourceConfiguration = &ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[j];
					break;
				}
			}
		}

		#if 0
		//asctoken
		pTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token, 0, "asctoken", 0, NULL);
		if(NULL != pTmpStr)
		{
			iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations;
			for(j = 0; j < iSize; j++)
			{
				if(!strcmp(pTmpStr, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[j].token))
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].AudioSourceConfiguration = &ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[j];
					break;
				}
			}
		}
		#endif
		//vectoken
		pTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token, 0, "vectoken", 0, NULL);
		if(NULL != pTmpStr)
		{
			iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations;
			for(j = 0; j < iSize; j++)
			{
				if(!strcmp(pTmpStr, ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[j].token))
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].VideoEncoderConfiguration = &ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations[j];
					break;
				}
			}
		}
		#if 0
		//aectoken
		pTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token, 0, "aectoken", 0, NULL);
		if(NULL != pTmpStr)
		{
			iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations;
			for(j = 0; j < iSize; j++)
			{
				if(!strcmp(pTmpStr, ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[j].token))
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].AudioEncoderConfiguration = &ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[j];
					break;
				}
			}
		}
		#endif
		//ptzctoken
		pTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token, 0, "ptzctoken", 0, NULL);
		if(NULL != pTmpStr)
		{
			iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->__sizePTZConfiguration;
			for(j = 0; j < iSize; j++)
			{
				if(!strcmp(pTmpStr, ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[j].token))
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].PTZConfiguration = &ptOnvifCtrl->m_stOnvifCfg.m_ptConfigRsp->PTZConfiguration[j];
					break;
				}
			}
		}

		//mctoken
		pTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].token, 0, "mctoken", 0, NULL);
		if(NULL != pTmpStr)
		{
			iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations;
			for(j = 0; j < iSize; j++)
			{
				if(!strcmp(pTmpStr, ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[j].token))
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].MetadataConfiguration = &ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations[j];
					break;
				}
			}
		}

#if 1		
		//IPC支持海康设备报警added by qiaohaijun @2013.12.06
		//请谨慎修改。
		ptmp = ptOnvifCtrl->m_stOnvifCfg.m_ptProfilesRsp->Profiles[i].VideoAnalyticsConfiguration;
		ptmp->Name = "VideoAnalyticsName";
		ptmp->UseCount = 2;
	//	ptmp->token = "VideoAnalyticsToken";
		ptmp->token = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[j].token;
	
	
		ptmp->AnalyticsEngineConfiguration->__sizeAnalyticsModule = 1;
		ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Name = "MyCellMotionModule";
	//	ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Name = "qiaohaijun err test";
		ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Type = "tt:CellMotionEngine";

		ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->__sizeElementItem = 1;
		ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->__sizeSimpleItem = 1;
		ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->SimpleItem->Name = "Sensitivity";//灵敏度?
		ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->SimpleItem->Value = "80";//灵敏度的范围?
		ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->ElementItem->Name = "Layout";
		//不可删除
		ptmp->AnalyticsEngineConfiguration->AnalyticsModule->Parameters->ElementItem->__any =\
		"<tt:CellLayout Columns=\"22\" Rows=\"18\">"
			"<tt:Transformation>"
				"<tt:Translate x=\"-1.000000\" y=\"-1.000000\" />"
				"<tt:Scale x=\"0.001563\" y=\"0.002778\" />"
			"</tt:Transformation>"
		"</tt:CellLayout>";
	

		ptmp->RuleEngineConfiguration->__sizeRule = 1;//几种报警方式
	//	ptmp->RuleEngineConfiguration->Rule->Name = "MyMotionDetectorRule";
		ptmp->RuleEngineConfiguration->Rule->Name = "qiaohaijun err test ";//报警名字，什么都行，不能为空
		ptmp->RuleEngineConfiguration->Rule->Type = "tt:CellMotionDetector";//何种报警方式

	#if 0
		ptmp->RuleEngineConfiguration->Rule->Parameters->__sizeSimpleItem = 4;
		ptmp->RuleEngineConfiguration->Rule->Parameters->SimpleItem[0].Name = "MinCount";
		ptmp->RuleEngineConfiguration->Rule->Parameters->SimpleItem[0].Value = "5";
		ptmp->RuleEngineConfiguration->Rule->Parameters->SimpleItem[1].Name = "AlarmOnDelay";	
		ptmp->RuleEngineConfiguration->Rule->Parameters->SimpleItem[1].Value = "10";
		ptmp->RuleEngineConfiguration->Rule->Parameters->SimpleItem[2].Name = "AlarmOffDelay";
		ptmp->RuleEngineConfiguration->Rule->Parameters->SimpleItem[2].Value = "10";
		ptmp->RuleEngineConfiguration->Rule->Parameters->SimpleItem[3].Name = "ActiveCells";
		ptmp->RuleEngineConfiguration->Rule->Parameters->SimpleItem[3].Value = "1wA=";
	#endif
#endif
////////////////////////////////////end
		
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120312 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetVideoSourceConfigurations(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	char *pcTmpStr = NULL;
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize = 2;//ptOnvifCtrl->m_stNvsInfo.m_iVINum;
	if(iSize <= 0)
	{
		td_printf(0,"%s %d %s error: vsc_num %d", __FILE__, __LINE__, __FUNCTION__, iSize);
		return TD_ERROR;
	}

	//malloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetVideoSourceConfigurationsResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__VideoSourceConfiguration));

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++]= calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++]= calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++]= calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++]= calloc(1, sizeof(struct tt__IntRectangle));
	}

	//check memory
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assgin memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp = (struct _trt__GetVideoSourceConfigurationsResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations = (struct tt__VideoSourceConfiguration *)ppTmpMem[iNum++];

	for(i = 0; i < iSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].Name = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].SourceToken = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].Bounds = (struct tt__IntRectangle *)ppTmpMem[iNum++];
	}

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations = iSize;
	for(i = 0; i < iSize; i++)
	{	
		//token
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].token, 	\
				"vsctoken_ch%02d", i + 1);
		//name
		pcTmpStr = \
				GetProStr(ptOnvifCtrl->m_ptOnvifDict, "vsctoken_ch", i + 1, "vscname", 0, NULL);
		if(NULL != pcTmpStr)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].Name, pcTmpStr);
		}

		//source token
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].SourceToken, "vstoken_ch%02d", i + 1);

		//usecount
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].UseCount = \
				(enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "vsctoken_ch", i + 1, "vscusecount", 0, 0);

		//Bounds
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].Bounds->x      = VSCBOUNDSX;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].Bounds->y      = VSCBOUNDSY;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].Bounds->width  = VSCBOUNDSW;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->Configurations[i].Bounds->height = VSCBOUNDSH;
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120224 v1.0 creat by xxx
**************************************************/
int GMemInit__trt__GetVideoEncoderConfigurations(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	struct tt__VideoEncoderConfiguration *ptConfig = NULL;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	char *pcTmpStr = NULL;
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	//iSize
	iSize = ptOnvifCtrl->m_stNvsInfo.m_iVENum;
	
	if(iSize <= 0)
	{
		td_printf(0, "%s %d %s iSize:%d\n", __FILE__, __LINE__, __FUNCTION__, iSize);
		return TD_ERROR;
	}
	
	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetVideoEncoderConfigurationsResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__VideoEncoderConfiguration));	

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++] = calloc(NAME_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(TOKEN_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__VideoResolution));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__VideoRateControl));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__H264Configuration));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__MulticastConfiguration));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IPAddress));
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(IPADDR_LEN, sizeof(char));
	}
	
	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp = (struct _trt__GetVideoEncoderConfigurationsResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations = (struct tt__VideoEncoderConfiguration*)ppTmpMem[iNum++];
	ptConfig = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->Configurations;

	for(i = 0; i < iSize; i++)
	{
		ptConfig[i].Name  = (char *)ppTmpMem[iNum++];
		ptConfig[i].token = (char *)ppTmpMem[iNum++];
		ptConfig[i].Resolution  = (struct tt__VideoResolution *)ppTmpMem[iNum++];
		ptConfig[i].RateControl = (struct tt__VideoRateControl *)ppTmpMem[iNum++];
		ptConfig[i].H264 = (struct tt__H264Configuration *)ppTmpMem[iNum++];
		ptConfig[i].Multicast = (struct tt__MulticastConfiguration *)ppTmpMem[iNum++];				
		ptConfig[i].Multicast->Address = (struct tt__IPAddress*)ppTmpMem[iNum++];					
		ptConfig[i].Multicast->Address->IPv4Address  = (char **)ppTmpMem[iNum++];					
		*ptConfig[i].Multicast->Address->IPv4Address = (char *)ppTmpMem[iNum++];					
	}

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations = iSize;

	for(i = 0; i < iSize; i++)
	{
		//token
		sprintf(ptConfig[i].token, "vectoken_ch%02d", i + 1);
		
		//name
		pcTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "vectoken_ch", i + 1, "vecname", 0, NULL);
		if(NULL != pcTmpStr)
		{
			strncpy(ptConfig[i].Name, pcTmpStr, NAME_LEN);
		}

		//vecusecount
		ptConfig[i].UseCount = (enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "vectoken_ch", i + 1, "vecusecount", 0, 0);
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120312 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetAudioSourceConfigurations(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	char *pcTmpStr = NULL;
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;

	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	iSize = ptOnvifCtrl->m_stNvsInfo.m_iAINum;
	if(iSize <= 0)
	{
		td_printf(0, "%s %d %s error iSize :%d\n", __FILE__, __LINE__, __FUNCTION__, iSize);
		return TD_ERROR;
	}
	
	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetAudioSourceConfigurationsResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__AudioSourceConfiguration));

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++]  = calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++]  = calloc(MAX_NAME_LEN, sizeof(char));	
		ppTmpMem[iNum++]  = calloc(MAX_NAME_LEN, sizeof(char));
	}

	//check memory
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp = (struct _trt__GetAudioSourceConfigurationsResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations = (struct tt__AudioSourceConfiguration *)ppTmpMem[iNum++];

	for(i = 0; i < iSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].Name = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].SourceToken = (char *)ppTmpMem[iNum++];
	}

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations = iSize;

	for(i = 0; i < iSize; i++)
	{
		//token
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].token, \
			"asctoken_ch%02d", i + 1);

		//name
		pcTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "asctoken_ch", i + 1, "ascname", 0, NULL);
		if(NULL != pcTmpStr)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].Name, pcTmpStr);
		}
		
		//SourceToken
		pcTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "asctoken_ch", i + 1, "ascsrctoken", 0, NULL);
		if(NULL != pcTmpStr)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].SourceToken, pcTmpStr);
		}

		//UseCount
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->Configurations[i].UseCount = \
			(enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "asctoken_ch", i + 1, "ascusecount", 0, 3);
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120312 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetAudioEncoderConfigurations(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	char *pcTmpStr = NULL;
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	//iSize
	iSize = ptOnvifCtrl->m_stNvsInfo.m_iAINum;
	if(iSize <= 0)
	{
		td_printf(0, "%s %d %s iSize:%d\n", __FILE__, __LINE__, __FUNCTION__, iSize);
		return TD_ERROR;
	}

	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetAudioEncoderConfigurationsResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__AudioEncoderConfiguration));

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__MulticastConfiguration));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IPAddress));
		ppTmpMem[iNum++] = calloc(1, sizeof(char *));
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));	
	}

	//check memory
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp = (struct _trt__GetAudioEncoderConfigurationsResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations = (struct tt__AudioEncoderConfiguration*)ppTmpMem[iNum++];

	for(i = 0; i < iSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Name = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast = (struct tt__MulticastConfiguration *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast->Address = (struct tt__IPAddress *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast->Address->IPv4Address = (char **)ppTmpMem[iNum++];
		*ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast->Address->IPv4Address = (char *)ppTmpMem[iNum++];
	}

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations = iSize;
	for(i = 0; i< iSize; i++)
	{
		//token
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].token, \
			"aectoken_ch%02d", i + 1);

		//name
		pcTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "aectoken_ch", i + 1, "aecname", 0, NULL);
		if(NULL != pcTmpStr)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Name, pcTmpStr);
		}

		//UseCont
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].UseCount = \
			(enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "aectoken_ch",  i + 1, "aecusecount", 0, 0);
		
		//Encoding
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Encoding       = tt__AudioEncoding__G711;
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Bitrate        = DEFAULT_AUDIOBITRATE;
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].SampleRate     = AUDIO_SAMPLE_8K;
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].SessionTimeout = AECST;

		//Multicast
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast->Address->Type = (enum tt__IPType)AECMULADDRTP;
		strcpy(*ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast->Address->IPv4Address, AECMULADDRIPV4);

		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast->Port      = AECMULPORT;
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast->TTL       = AECMULTTL;
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->Configurations[i].Multicast->AutoStart = (enum xsd__boolean)AECMULAS;
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120312 v1.0 creat by pangtb
20131028 v2.3.1 modify by ptb
**************************************************/
int GMemInit__trt__GetMetadataConfigurations(void *_pThis)
{	
	struct tt__MetadataConfiguration *ptConfig = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MAX] = {0};
	char *pcTmpStr = NULL;
	int iSize = 0;
	int iNum  = 0;
	int i = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp))
	{
		iSize = ptOnvifCtrl->m_stNvsInfo.m_iVINum;

		ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetMetadataConfigurationsResponse));
		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__MetadataConfiguration));

		for(i = 0; i < iSize; i++)
		{
			//Name
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
			//Token
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PTZFilter));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__EventSubscription));
			ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__MulticastConfiguration));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct wsnt__FilterType));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct _tt__EventSubscription_SubscriptionPolicy));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IPAddress));
			ppTmpMem[iNum++] = calloc(1, sizeof(char *));
			ppTmpMem[iNum++] = calloc(1, MEM_LEN_MIN);
		}

		//check memory
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}

		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp = (struct _trt__GetMetadataConfigurationsResponse*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations = (struct tt__MetadataConfiguration*)ppTmpMem[iNum++];

		ptConfig = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->Configurations;
		for(i = 0; i < iSize; i++)
		{
			ptConfig[i].Name = (char *)ppTmpMem[iNum++];
			ptConfig[i].token = (char *)ppTmpMem[iNum++];
			ptConfig[i].PTZStatus = (struct tt__PTZFilter*)ppTmpMem[iNum++];
			ptConfig[i].Events = (struct tt__EventSubscription*)ppTmpMem[iNum++];
			ptConfig[i].Analytics = (enum xsd__boolean*)ppTmpMem[iNum++];
			ptConfig[i].Multicast = (struct tt__MulticastConfiguration*)ppTmpMem[iNum++];
			ptConfig[i].Events->Filter = (struct wsnt__FilterType*)ppTmpMem[iNum++];
			ptConfig[i].Events->SubscriptionPolicy = (struct _tt__EventSubscription_SubscriptionPolicy*)ppTmpMem[iNum++];
			ptConfig[i].Multicast->Address = (struct tt__IPAddress*)ppTmpMem[iNum++];
			ptConfig[i].Multicast->Address->IPv4Address  = (char **)ppTmpMem[iNum++];
			*ptConfig[i].Multicast->Address->IPv4Address = (char *)ppTmpMem[iNum++];
		}

		//全局变量初始化
		ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations = iSize;
		for(i = 0; i < iSize; i++)
		{
			//token
			sprintf(ptConfig[i].token, "mctoken_ch%02d", i + 1);

			//name
			pcTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "mctoken_ch", i + 1, "mcname", 0, NULL);
			if(NULL != pcTmpStr)
			{
				strncpy(ptConfig[i].Name, pcTmpStr, MEM_LEN_MIN);
			}
			
			//PTZStatus
			ptConfig[i].PTZStatus->Status   = xsd__boolean__false_;
			ptConfig[i].PTZStatus->Position = xsd__boolean__false_;

			//Analytics-目前不支持智能分析
			*ptConfig[i].Analytics = (enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "mctoken_ch", i + 1, "mcanalytics", 0, xsd__boolean__false_);
			
			//Events
			//待定

			//Multicast
			ptConfig[i].Multicast->Address->Type = tt__IPType__IPv4;
			strncpy(*ptConfig[i].Multicast->Address->IPv4Address, MULT_ADDR, 32);
			ptConfig[i].Multicast->Port = 8002;
			ptConfig[i].Multicast->TTL  = 64;
			ptConfig[i].Multicast->AutoStart = xsd__boolean__false_;

			//SessionTimeout
			ptConfig[i].SessionTimeout = 60000;
		}

	}







	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120406 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetAudioOutputConfigurations(void *_pThis)
{	
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	char *pcTmpStr = NULL;
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stNvsInfo.m_iAONum;
	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetAudioOutputConfigurationsResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__AudioOutputConfiguration));

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(MAX_STRING_LEN, sizeof(char));
		ppTmpMem[iNum++] = calloc(MAX_STRING_LEN, sizeof(char));
	}

	//check memory
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp = (struct _trt__GetAudioOutputConfigurationsResponse *)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations = (struct tt__AudioOutputConfiguration *)ppTmpMem[iNum++];

	for(i = 0; i < iSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].Name = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].token = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].OutputToken = (char *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].SendPrimacy = (char *)ppTmpMem[iNum++];
	}

	//全局变量初始化
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->__sizeConfigurations = iSize;
	for(i = 0; i < iSize; i++)
	{
		//token
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].token, "aoctoken_ch%02d", i + 1);
		//name
		pcTmpStr = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "aoctoken_ch", i + 1, "aocname", 0, NULL);
		if(NULL != pcTmpStr)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].Name, pcTmpStr);
		}
		//use count
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].UseCount = \
			(enum xsd__boolean)GetProInt(ptOnvifCtrl->m_ptOnvifDict, "aoctoken_ch", i + 1, "aocusecount", 0, 1);
		//OutputToken
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].OutputToken, "aotoken_ch%02d", i + 1);

		//SendPrimacy
		strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].SendPrimacy, "unknown01", 10);

		//OutputLevel
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgRsp->Configurations[i].OutputLevel = 1;
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120316 v1.0 creat by pangtb
            20130306 v2.3.0 modify by ptb
**************************************************/
int GMemInit__trt__GetVideoSourceConfigurationOptions(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	int iVSNum = 0;
	int iSize  = 0;
	int iNum   = 0;
	int i = 0;
	int j = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize  = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgRsp->__sizeConfigurations;
	iVSNum = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->__sizeVideoSources;

	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetVideoSourceConfigurationOptionsResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__VideoSourceConfigurationOptions));

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRectangleRange));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
		ppTmpMem[iNum++] = calloc(iVSNum, sizeof(char **));
		for(j = 0; j < iVSNum; j++)
		{
			ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		}
	}
	
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp = (struct _trt__GetVideoSourceConfigurationOptionsResponse*)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options = (struct tt__VideoSourceConfigurationOptions*)ppTmpMem[iNum++];

	for(i = 0; i < iSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange = (struct tt__IntRectangleRange*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->XRange = (struct tt__IntRange*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->YRange = (struct tt__IntRange*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->WidthRange = (struct tt__IntRange*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->HeightRange = (struct tt__IntRange*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].VideoSourceTokensAvailable = (char **)ppTmpMem[iNum++];

		for(j = 0; j < iVSNum; j++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].VideoSourceTokensAvailable[j] = (char *)ppTmpMem[iNum++];
		}
	}
	
	//全局变量初始化
	for(i = 0; i < iSize; i++)
	{
		//XRange
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->XRange->Min = VSCOBRXMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->XRange->Max = VSCOBRXMAX;
			
		//YRange
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->YRange->Min = VSCOBRYMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->YRange->Max = VSCOBRYMAX;
			
		//WidthRange
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->WidthRange->Min = VSCOBRWMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->WidthRange->Max = VSCOBRWMAX;
			
		//HeightRange
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->HeightRange->Min = VSCOBRHMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].BoundsRange->HeightRange->Max = VSCOBRHMAX;
	
		//VideoSourceTokensAvailable
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].__sizeVideoSourceTokensAvailable = iVSNum;
		for(j = 0; j < iVSNum; j++)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourceCfgOptRsp->Options[i].VideoSourceTokensAvailable[j], \
				ptOnvifCtrl->m_stOnvifCfg.m_ptVideoSourcesRsp->VideoSources[j].token);
		}
		
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:   20120317 v1.0 creat by pangtb
            20130301 v2.3.0 modify by ptb
**************************************************/
int GMemInit__trt__GetVideoEncoderConfigurationOptions(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int piVideoList[MAX_VIDEOSIZE_LIST];
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	int iRJpegSize  = 0;
	int iRH264Size  = 0;
	int iVLNum      = 0;
	int iSize       = 0;
	int iNum        = 0;
	int i           = 0;
	int j           = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}
	
	iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations;
	if(iSize <= 0)
	{
		return TD_ERROR;
	}
		
	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetVideoEncoderConfigurationOptionsResponse));	
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__VideoEncoderConfigurationOptions));

	for(i = 0; i < iSize; i++)
	{
		//共享内存通信,获取分辨率列表
		iVLNum = 0;//OnvifCmdGetVideoChnSizeList(i + 1, piVideoList, MAX_VIDEOSIZE_LIST);
		#if 0
		for(j = 0; j < iVLNum; j++)
		{
			td_printf(0, "iChn:%d piVideoList:%d\n", i + 1, piVideoList[j]);
		}
		#endif
		if(iVLNum <= 0)
		{
			iRJpegSize = 1;
			iRH264Size = 1;
		}
		else
		{
			iRJpegSize = iVLNum;
			iRH264Size = iVLNum;
		}

		iRH264Size = 4;

		//QualityRange
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRectangleRange));	
#if 0
		//JPEG
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__JpegOptions));
		ppTmpMem[iNum++] = calloc(iRJpegSize, sizeof(struct tt__VideoResolution));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
#endif
		//H264
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__H264Options));
		ppTmpMem[iNum++] = calloc(iRH264Size, sizeof(struct tt__VideoResolution));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
		ppTmpMem[iNum++] = calloc(VECOH264PS_SIZE, sizeof(enum tt__H264Profile));
	}

	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return -1;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp = (struct _trt__GetVideoEncoderConfigurationOptionsResponse*)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options = (struct tt__VideoEncoderConfigurationOptions*)ppTmpMem[iNum++];

	for(i = 0; i < iSize; i++)
	{
		//QualityRange
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].QualityRange = (struct tt__IntRange*)ppTmpMem[iNum++];
#if 0
		//JPEG
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG = (struct tt__JpegOptions*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG->ResolutionsAvailable = (struct tt__VideoResolution*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG->FrameRateRange = (struct tt__IntRange *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG->EncodingIntervalRange = (struct tt__IntRange *)ppTmpMem[iNum++];
#endif
		//H264
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264 = (struct tt__H264Options*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->ResolutionsAvailable = (struct tt__VideoResolution*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->GovLengthRange = (struct tt__IntRange *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->FrameRateRange = (struct tt__IntRange *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->EncodingIntervalRange = (struct tt__IntRange *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->H264ProfilesSupported  = (enum tt__H264Profile *)ppTmpMem[iNum++];
	}
	
	//全局变量初始化
	for(i = 0; i < iSize; i++)
	{
		//共享内存通信,获取分辨率列表
		iVLNum = OnvifCmdGetVideoChnSizeList(i + 1, piVideoList, MAX_VIDEOSIZE_LIST);
		if(iVLNum <= 0)
		{
			iRJpegSize = 1;
			//iRH264Size = 1;
		}
		else
		{
			iRJpegSize = iVLNum;
			//iRH264Size = iVLNum;
		}

		//QualityRange
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].QualityRange->Min = VECOQRMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].QualityRange->Max = VECOQRMAX;

		//size
		//ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG->__sizeResolutionsAvailable = iRJpegSize;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->__sizeResolutionsAvailable = iRH264Size;

		//分辨率列表

		//JPEG编码参数范围
		#if 0
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG->FrameRateRange->Min= VECOJPEGFRRMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG->FrameRateRange->Max= VECOJPEGFRRMAX;
			
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG->EncodingIntervalRange->Min= VECOJPEGEIRMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].JPEG->EncodingIntervalRange->Max= VECOJPEGEIRMAX;
		#endif
		//H264编码参数范围
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->GovLengthRange->Min= VECOH264GLRMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->GovLengthRange->Max= VECOH264GLRMAX;
	
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->FrameRateRange->Min= VECOH264FRRMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->FrameRateRange->Max= VECOH264FRRMAX;

		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->EncodingIntervalRange->Min= VECOH264EIRMIN;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->EncodingIntervalRange->Max= VECOH264EIRMAX;

		
		//H264ProfilesSupported
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->__sizeH264ProfilesSupported = VECOH264PS_SIZE;

		for(j = 0; j < VECOH264PS_SIZE; j++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->H264ProfilesSupported[j] = (enum tt__H264Profile)(VECOH264PSMODE_01 + j);
		}
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120318 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetAudioSourceConfigurationOptions(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	int iNum = 0;
	int i = 0;
	int j = 0;
	int iSize = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgRsp->__sizeConfigurations;

	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetAudioSourceConfigurationOptionsResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__AudioSourceConfigurationOptions));

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++] = calloc(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources, sizeof(char **));
		for(j = 0; j < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources; j++)
		{
			ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		}
	}
	
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgOptRsp = (struct _trt__GetAudioSourceConfigurationOptionsResponse*)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgOptRsp->Options = (struct tt__AudioSourceConfigurationOptions*)ppTmpMem[iNum++];

	for(i = 0; i < iSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgOptRsp->Options[i].InputTokensAvailable = (char **)ppTmpMem[iNum++];
		for(j = 0; j < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources; j++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgOptRsp->Options[i].InputTokensAvailable[j] = (char *)ppTmpMem[iNum++];
		}
	}
	
	//全局变量初始化
	for(i = 0; i < iSize; i++)
	{
		//InputTokensAvailable
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgOptRsp->Options[i].__sizeInputTokensAvailable = \
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources;

		for(j = 0; j < ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->__sizeAudioSources; j++)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourceCfgOptRsp->Options[i].InputTokensAvailable[j], \
				ptOnvifCtrl->m_stOnvifCfg.m_ptAudioSourcesRsp->AudioSources[j].token);
		}
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120318 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetAudioEncoderConfigurationOptions(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	int iAECOSize = 0;
	int iBLSize   = 0;
	int iSLSize   = 0;
	int iSize     = 0;
	int iNum      = 0;
	int i = 0;
	int j = 0;
	int k = 0;

	//改变宏的值一定要注意改变数组对应的成员
	int iAudioRateList[AUDIO_SAMPLE_NUM] = {AUDIO_SAMPLE_8K, AUDIO_SAMPLE_32K, AUDIO_SAMPLE_48K};
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgRsp->__sizeConfigurations;

	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetAudioEncoderConfigurationOptionsResponse));

	//仅支持一种音频配置
	iAECOSize = 1;
	ppTmpMem[iNum++] = calloc(iAECOSize, sizeof(struct tt__AudioEncoderConfigurationOptions));

	iBLSize = AECOBL_SIZE;
	iSLSize = AUDIO_SAMPLE_NUM;

	for(i = 0; i < iAECOSize; i++)
	{
		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__AudioEncoderConfigurationOption));
		for(j = 0; j < iSize; j++)
		{
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntList));
			ppTmpMem[iNum++] = calloc(iBLSize, sizeof(int));
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntList));
			ppTmpMem[iNum++] = calloc(iSLSize, sizeof(int));
		}
	}
	
	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp = (struct _trt__GetAudioEncoderConfigurationOptionsResponse*)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options = (struct tt__AudioEncoderConfigurationOptions*)ppTmpMem[iNum++];

	for(i = 0; i < iAECOSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options= (struct tt__AudioEncoderConfigurationOption*)ppTmpMem[iNum++];
		for(j = 0; j < iSize; j++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].BitrateList = (struct tt__IntList*)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].BitrateList->Items = (int *)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].SampleRateList = (struct tt__IntList*)ppTmpMem[iNum++];
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].SampleRateList->Items = (int *)ppTmpMem[iNum++];
		}
	}
	
	//全局变量初始化
	for(i = 0; i < iAECOSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options->__sizeOptions = iSize;
		for(j = 0; j < iSize; j++)
		{
			//Encoding
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].Encoding = tt__AudioEncoding__G711; 

			//BitrateList
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].BitrateList->__sizeItems    = iBLSize;
			
			for(k = 0; k < iBLSize; k++)
			{
				ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].BitrateList->Items[k] = AECOBL_01;
			}

			//SampleRateList modified by qiaohaijun @2013.5.7
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].SampleRateList->__sizeItems = iSLSize;
			if(DEVICE_DVR == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
			{
				for(k = 0; k < iSLSize; k++)
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].SampleRateList->Items[k] = iAudioRateList[k];
				}
			}
			else if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
			{
				for(k = 0; k < iSLSize; k++)
				{
					ptOnvifCtrl->m_stOnvifCfg.m_ptAudioEncoderCfgOptRsp->Options[i].Options[j].SampleRateList->Items[k] = AUDIO_SAMPLE_8K;
				}
			}
			//end
		}
	}

	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120318 v1.0 creat by pangtb
20131028 v2.3.1 modify by ptb
**************************************************/
int GMemInit__trt__GetMetadataConfigurationOptions(void *_pThis)
{	
	struct tt__MetadataConfigurationOptions *ptOptions = NULL;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MID] = {0};
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgOptionsRsp))
	{
		iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgRsp->__sizeConfigurations;

		ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetMetadataConfigurationOptionsResponse));
		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__MetadataConfigurationOptions));
		
		for(i = 0; i < iSize; i++)
		{
			ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__PTZStatusFilterOptions));
			ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
			ppTmpMem[iNum++] = calloc(1, sizeof(enum xsd__boolean));
		}
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}
		
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgOptionsRsp = (struct _trt__GetMetadataConfigurationOptionsResponse*)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgOptionsRsp->Options = (struct tt__MetadataConfigurationOptions*)ppTmpMem[iNum++];

		ptOptions = ptOnvifCtrl->m_stOnvifCfg.m_ptMetaCfgOptionsRsp->Options;
		for(i = 0; i < iSize; i++)
		{
			ptOptions[i].PTZStatusFilterOptions = (struct tt__PTZStatusFilterOptions*)ppTmpMem[iNum++];
			ptOptions[i].PTZStatusFilterOptions->PanTiltPositionSupported = (enum xsd__boolean *)ppTmpMem[iNum++];
			ptOptions[i].PTZStatusFilterOptions->ZoomPositionSupported = (enum xsd__boolean *)ppTmpMem[iNum++];
		}
		
		//全局变量初始化
		for(i = 0; i < iSize; i++)
		{
			//InputTokensAvailable
			ptOptions[i].PTZStatusFilterOptions->PanTiltStatusSupported	   = xsd__boolean__false_; 
			ptOptions[i].PTZStatusFilterOptions->ZoomStatusSupported       = xsd__boolean__false_; 
			*ptOptions[i].PTZStatusFilterOptions->PanTiltPositionSupported = xsd__boolean__false_; 
			*ptOptions[i].PTZStatusFilterOptions->ZoomPositionSupported	   = xsd__boolean__false_; 
		}
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120409 v1.0 creat by pangtb
**************************************************/
int GMemInit__trt__GetAudioOutputConfigurationOptions(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MAX_TMP_MEM] = {0};
	int iOTANum = 0;
	int iSize   = 0;
	int iNum    = 0;
	int i = 0;
	int j = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stNvsInfo.m_iAONum;
	iOTANum = ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp->__sizeAudioOutputs;

	//calloc memory
	ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetAudioOutputConfigurationOptionsResponse));
	ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__AudioOutputConfigurationOptions));

	for(i = 0; i < iSize; i++)
	{
		ppTmpMem[iNum++] = calloc(iOTANum, sizeof(char *));
		for(j = 0; j < iOTANum; j++)
		{
			ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		}
		
		ppTmpMem[iNum++] = calloc(AOCOSPO_SIZE, sizeof(char *));
		for(j = 0; j < AOCOSPO_SIZE; j++)
		{
			ppTmpMem[iNum++] = calloc(MAX_NAME_LEN, sizeof(char));
		}
		
		ppTmpMem[iNum++] = calloc(1, sizeof(struct tt__IntRange));
	}

	if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
	{
		td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
		FreeAllTmpMem(ppTmpMem, iNum);
		return TD_ERROR;
	}

	//assign memory
	iNum = 0;
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp = (struct _trt__GetAudioOutputConfigurationOptionsResponse*)ppTmpMem[iNum++];
	ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options = (struct tt__AudioOutputConfigurationOptions*)ppTmpMem[iNum++];

	for(i = 0; i < iSize; i++)
	{
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].OutputTokensAvailable = (char **)ppTmpMem[iNum++];
		for(j = 0; j < iOTANum; j++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].OutputTokensAvailable[j] = (char *)ppTmpMem[iNum++];
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].SendPrimacyOptions = (char **)ppTmpMem[iNum++];
		for(j = 0; j < AOCOSPO_SIZE; j++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].SendPrimacyOptions[j] = (char *)ppTmpMem[iNum++];
		}
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].OutputLevelRange = (struct tt__IntRange *)ppTmpMem[iNum++];
	}
	
	//全局变量初始化
	for(i = 0; i < iSize; i++)
	{
		//OutputTokensAvailable
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].__sizeOutputTokensAvailable = iOTANum;
		for(j = 0; j < iOTANum; j++)
		{
			strcpy(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].OutputTokensAvailable[j], ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputsRsp->AudioOutputs[j].token);
		}
		
		//SendPrimacyOptions
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].__sizeSendPrimacyOptions = AOCOSPO_SIZE;
		for(j = 0; j < AOCOSPO_SIZE; j++)
		{
			sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].SendPrimacyOptions[j], "unknown%02d", j + 1);
		}
		
		//OutputLevelRange
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].OutputLevelRange->Max = AOCOOLR_MAX;
		ptOnvifCtrl->m_stOnvifCfg.m_ptAudioOutputCfgOptRsp->Options[i].OutputLevelRange->Min = AOCOOLR_MIN;
	}
	
	return TD_OK;
}

/**************************************************
*功能:		初始化全局变量对应的结构体
*参数:		_pThis:		全局变量 g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史: 20120321 v1.0 creat by ptb
**************************************************/
int GMemInit__trt__GetStreamUri(void *_pThis)
{	
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	void *ppTmpMem[MEM_LEN_MIN] = {0};
	int iSize = 0;
	int iNum  = 0;
	int i     = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stNvsInfo.m_iVINum;
		
	if(pointer_valid_check(ptOnvifCtrl->m_stOnvifCfg.m_ptStreamUriRsp))
	{
		//malloc memeory
		ppTmpMem[iNum++] = calloc(1, sizeof(struct _trt__GetStreamUriResponse));					
		ppTmpMem[iNum++] = calloc(iSize, sizeof(struct tt__MediaUri));						

		for(i = 0; i < iSize; i++)
		{
			ppTmpMem[iNum++] = calloc(MEM_LEN_MID, sizeof(char));						
		}
		
		if(CheckMem(ppTmpMem, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0])))
		{
			td_printf(0, "warning:file:%s,line:%d, calloc err iNum == %d, buffersize === %d", __FILE__, __LINE__, iNum, sizeof(ppTmpMem)/sizeof(ppTmpMem[0]));
			FreeAllTmpMem(ppTmpMem, iNum);
			return TD_ERROR;
		}
				
		//assign memory
		iNum = 0;
		ptOnvifCtrl->m_stOnvifCfg.m_ptStreamUriRsp = (struct _trt__GetStreamUriResponse *)ppTmpMem[iNum++];
		ptOnvifCtrl->m_stOnvifCfg.m_ptStreamUriRsp->MediaUri = (struct tt__MediaUri *)ppTmpMem[iNum++];

		for(i = 0; i < iSize; i++)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptStreamUriRsp->MediaUri[i].Uri = (char *)ppTmpMem[iNum++];
		}
		
	}
	
	return TD_OK;
}

