/*
 *	project: onvif_v210
 *	file: main.c
 *	history: dongliqiang created at 20120129
 */




#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "common.h"
#include "onvifH.h"
#include "onvif.nsmap"
#include "onvif_sdk.h"
#include "proparser.h"
#include "gvar_mem.h"
#include "onvif_interface.h"
#include "wsaapi.h"
#include "onvif_shm_cmd.h"
#include "share_mem.h"
#include "onvif_heartbeat.h"
#include "macro.h"
#include "proc.h"
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

//firmware version
#define FIRMWARE_VER  "onvif_v2.3.1"

//全局变量
OnvifCtrl g_stOnvifCtrl;	//onvif server control

int tcp_svr_exit_flag;
int udp_svr_exit_flag;
int rtsp_svr_exit_flag;
int main_exit_flag;

extern SOAP_NMAC struct Namespace namespaces_probe[];
extern FILE* fpProc;
/**************************************************
*功能:		初始化配置文件,根据输出路
				数来添加config_onvif.ini内容
*参数:		_pThis
*返回值:	0：成功，1：失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int ProInit(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	dictionary *ptTmpDict  = NULL;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	//配置文件出厂时根据视频路数来补充配置文件
	if(access(CONFIG_ONVIF_PATH, F_OK) != 0)
	{
		if(TD_OK != BuildProfile(ptOnvifCtrl))
		{
			td_printf(0, "%s %d %s ModifyProfile error!\n", __FILE__, __LINE__, __FUNCTION__);	
			return TD_ERROR; 
		}

		//修改配置文件路数
		ptTmpDict = LoadProfile(CONFIG_ONVIF_PATH);
		if(NULL != ptTmpDict)
		{
			SetProStr(ptTmpDict, "media", 0, "profile_size", 0, NULL, ptOnvifCtrl->m_stNvsInfo.m_iVENum, CFG_REPLACE);
			SaveProfile(ptTmpDict, CONFIG_ONVIF_PATH);
			ReleaseProfile(ptTmpDict);
		}else {
			td_printf(0, "%s %d %s LoadProfile error!\n", __FILE__, __LINE__, __FUNCTION__);
			return TD_ERROR;
		}
	}
	
	//open config_onvif.ini
	ptOnvifCtrl->m_ptOnvifDict = LoadProfile(CONFIG_ONVIF_PATH);
	if(pointer_valid_check(ptOnvifCtrl->m_ptOnvifDict))
	{
		td_printf(0, "%s %d %s LoadProfile error!\n", __FILE__, __LINE__, __FUNCTION__);

		//文件加载为空,删除配置文件,重新生成 -- dlq modified at 20130627
		rename(CONFIG_ONVIF_PATH, CONFIG_ONVIF_PATH"_err");
		return TD_ERROR;
	}
	
	return TD_OK;
}

static int GetLocalIP(char *_pcIP)
{
	int iRet = 0;
	int iSocket = 0;
	struct ifreq ifr;
	struct sockaddr_in *sin;
	
	if(NULL == _pcIP)
	{
		return TD_ERROR;
	}
	
	iSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(iSocket < 0)
	{
		return TD_ERROR;
	}
	
	strcpy(ifr.ifr_name, "eth0");//ens33
	if(ioctl(iSocket, SIOCGIFADDR, &ifr) < 0)
	{
		close(iSocket);
		return TD_ERROR;
	}
	
	sin = (struct sockaddr_in *)&ifr.ifr_addr;
	strcpy(_pcIP, inet_ntoa(sin->sin_addr));
	
	close(iSocket);
	
	return iRet;
}

/**************************************************
*功能:		更新IP及UUID
*参数:		_pThis
*返回值:	0: 成功, !0: 失败
*修改历史:20130407 v2.3.0 creat by ptb
**************************************************/
static int UpdateIPAndUUID(OnvifCtrl *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	TGuiIpMsg stIpMsg   = {0};
	TNvsInfo *ptNvsInfo = NULL;
	char *pcTimeZone = NULL;
	char *pcTmp = NULL;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
	
	ptNvsInfo = &ptOnvifCtrl->m_stNvsInfo; 

	strncpy((char *)ptNvsInfo->m_cMultIP, MULT_ADDR, 31);
	ptNvsInfo->m_iLTcpPort   = GetProInt(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "httpport", 0, 8080);
	ptNvsInfo->m_iLEventPort = ptNvsInfo->m_iLTcpPort + 1;
	ptOnvifCtrl->m_stRtspServer.m_iLPort = GetProInt(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "rtspport", 0, 554);
	ptNvsInfo->m_iMultPort   = MULT_PORT;

	//UUID	
	pcTmp = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "uuid", 0, "null");
	if(!strcmp(pcTmp, "null"))
	{
		pcTmp = (char*)soap_wsa_rand_uuid(&ptOnvifCtrl->m_stSoapUdp);
		if(!pointer_valid_check(pcTmp))
		{
			strncpy((char *)ptOnvifCtrl->m_stNvsInfo.m_cUUID, pcTmp, 63);	
			td_printf(0, "%s: uuid gain: %s\n", __FUNCTION__, ptOnvifCtrl->m_stNvsInfo.m_cUUID);
		}
	
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "uuid", 0, (char *)ptOnvifCtrl->m_stNvsInfo.m_cUUID, 0, CFG_REPLACE);
		ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
	}
	else
	{
		if(strlen(pcTmp) < MAX_UUID_LEN)
		{
			strncpy((char *)ptOnvifCtrl->m_stNvsInfo.m_cUUID, pcTmp, 63);
			td_printf(0, "%s: uuid:%s\n", __FUNCTION__, ptOnvifCtrl->m_stNvsInfo.m_cUUID);
		}
	}

	//TimeZone
	pcTimeZone = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "timezone", 0, "WAUST-8");
	GetTimeZoneOffset(pcTimeZone);
	td_printf(0, "%s %d m_iTimeZone[%d]", __FUNCTION__, __LINE__, ptNvsInfo->m_iTimeZone);

	return TD_OK;
}

/**************************************************
*功能:		创建内存空间
*参数:		_pThis
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int GVarInit(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl 	= (OnvifCtrl *)_pThis;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(UpdateIPAndUUID(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	//以下部分是ptOnvifCtrl成员的初始化部分
	if(GMemInit__tds__GetScopes(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	//必须在GetScopes之后
	if(GMemInit__ns2__Probe(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetDiscoveryMode(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetDeviceInformation(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetUsers(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetWsdlUrl(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetCapabilities(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetServiceCapabilities(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
#if 1
	if(GMemInit__tptz20__GetConfigurations(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}	

	if(GMemInit__tptz20__GetNodes(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tptz20__GetStatus(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	//这个必须在GMemInit__tptz__GetNodes 之后调用
	if(GMemInit__tptz20__GetConfigurationOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tptz20__GetPresets(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__ns4__GetEventProperties(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetVideoSources(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
	#endif
#if 0
	if(GMemInit__trt__GetAudioSources(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetAudioSourceConfigurations(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetAudioSourceConfigurationOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
	if(GMemInit__trt__GetAudioEncoderConfigurations(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
	if(GMemInit__trt__GetAudioEncoderConfigurationOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
	if(GMemInit__trt__GetAudioOutputs(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetAudioOutputConfigurations(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetAudioOutputConfigurationOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
#endif

	if(GMemInit__trt__GetVideoSourceConfigurations(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetVideoSourceConfigurationOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetVideoEncoderConfigurations(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetVideoEncoderConfigurationOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetMetadataConfigurations(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}	

	if(GMemInit__trt__GetMetadataConfigurationOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	//必须放到ptz,audio,video,metadata初始化之后
	if(GMemInit__trt__GetProfiles(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
#if 1
	if(GMemInit__timg10__GetOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__timg20__GetOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__timg10__GetStatus(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__timg20__GetStatus(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__timg10__GetMoveOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__timg20__GetMoveOptions(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__ns6__Subscribe(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__ns8__Notify(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__ns3__PullMessages(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__ns4__CreatePullPointSubscription(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
	#endif
	if(GMemInit__tds__GetNetworkInterfaces(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetHostname(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetDNS(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetNTP(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetNetworkProtocols(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetNetworkDefaultGateway(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__tds__GetSystemDateAndTime(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
#if 1
	if(GMemInit__tds__GetRelayOutputs(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}
#endif
	if(GMemInit__tds__GetServices(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__trt__GetStreamUri(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__timg10__GetImagingSettings(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	if(GMemInit__timg20__GetImagingSettings(ptOnvifCtrl))
	{
		DBG_HERE;
		return TD_ERROR;
	}

	return TD_OK;
}


/**************************************************
*功能:	获取编译时间
*参数:	
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
char *GetCompileDateTime(void)
{        
	static char CompileDateTime[128];
	
    const char szEnglishMonth[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char szTmpDate[40]={0};
    char szTmpTime[20]={0};
    char szMonth[4]={0};
    int iYear  = 0;
	int iMonth = 0;
	int iDay   = 0;
	int iHour  = 0;
	int iMin   = 0;
	int iSec   = 0;
    int i      = 0;

    //获取编译日期、时间
    sprintf(szTmpDate, "%s", __DATE__);    //"Sep 18 2010"
    sprintf(szTmpTime, "%s", __TIME__);    //"10:59:19"
    
    sscanf(szTmpDate, "%s %d %d", szMonth, &iDay, &iYear);
    sscanf(szTmpTime, "%d:%d:%d", &iHour, &iMin, &iSec);
            
    for(i = 0; i < 12; i++)
    {
        if(0 == strncmp(szMonth, szEnglishMonth[i], 3))
        {
            iMonth = i + 1;
            break;
        }
    }

    //td_printf(0, "%d,%d,%d,%d,%d,%d\n", iYear, iMonth, iDay, iHour, iMin, iSec);
    sprintf(CompileDateTime, "%04d%02d%02d", iYear, iMonth, iDay);

	return CompileDateTime;
}

/**************************************************
*功能:		获取共享内存网络参数信息，
				更新ONVIF全局变量
*参数:		_pThis			全局变量结构体指针
*返回值:	0: 成功, !0: 失败
*修改历史:
	20130129 v2.0 creat by ptb
**************************************************/
int UpdateNetworkInfo(OnvifCtrl *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	OnvifCfg *ptCfg = NULL;
	unsigned int mask;
	TGuiIpMsg stIpMsg;
	int iRet = 0;
	int i    = 0;

	if(NULL != ptOnvifCtrl)
	{
		//更新网络参数
		memset(&stIpMsg, 0, sizeof(TGuiIpMsg));
		
		ptCfg = &ptOnvifCtrl->m_stOnvifCfg;
		//Strcpy(ptCfg->m_ptNetworkInfoRsp->NetworkInterfaces->Info->HwAddress, stIpMsg.m_strMac, 31);
		//Strcpy(*(ptCfg->m_ptNetworkGWRsp->NetworkGateway->IPv4Address), stIpMsg.m_strGetWay, 31);
		//Strcpy(*(ptCfg->m_ptDNSRsp->DNSInformation->DNSManual->IPv4Address), stIpMsg.m_strDNS, 31); 
		//Strcpy(*(ptCfg->m_ptDNSRsp->DNSInformation->DNSFromDHCP->IPv4Address), stIpMsg.m_strDNS, 31); 
		//ptOnvifCtrl->m_stNvsInfo.m_iLWebPort = OnvifCmdGetHttpPort();

		//子网掩码
		#if 0
		mask = inet_addr(stIpMsg.m_strSubMsk);
		td_printf(0, "the mask is %x", mask);
		
		mask = ntohl(mask);
		
		for(i = 0; i < 32; i++)
		{
			if(!((mask >> (31 - i))& 1))
			{
				break;
			}
		}
		#endif
		td_printf(0, "the prefix is %d", i);
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces->IPv4->Config->Manual->PrefixLength   = i;
		ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces->IPv4->Config->FromDHCP->PrefixLength = i;

		//确定获取IP方式
		if(0 == stIpMsg.m_iGetIpMethod)
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->FromDHCP = xsd__boolean__true_;
			ptOnvifCtrl->m_stOnvifCfg.m_ptDNSRsp->DNSInformation->FromDHCP = xsd__boolean__true_;
			ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces->IPv4->Config->DHCP = xsd__boolean__true_;
		}
		else
		{
			ptOnvifCtrl->m_stOnvifCfg.m_ptNTPRsp->NTPInformation->FromDHCP = xsd__boolean__false_;
			ptOnvifCtrl->m_stOnvifCfg.m_ptDNSRsp->DNSInformation->FromDHCP = xsd__boolean__false_;
			ptOnvifCtrl->m_stOnvifCfg.m_ptNetworkInfoRsp->NetworkInterfaces->IPv4->Config->DHCP = xsd__boolean__false_;
		}
	}

	return iRet;
}


/**************************************************
*功能:		获取共享内存设备系统信息，
				更新ONVIF全局变量
*参数:		_pThis			全局变量结构体指针
*返回值:	0: 成功, !0: 失败
*修改历史:
	20130129 v2.0 creat by ptb
**************************************************/
int UpdateSysInfo(OnvifCtrl *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iProductModel = 0;
	int iOEMFlag = 0;
	char *pTmp = NULL;
	
	if(NULL != ptOnvifCtrl)
	{
		//软件版本信息
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->FirmwareVersion, "%s_%s", FIRMWARE_VER, GetCompileDateTime());
		//OnvifCmdSendVerNum(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->FirmwareVersion);
		td_printf(0, "The firmware version is %s", ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->FirmwareVersion);
		//序列号
		strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->SerialNumber, "ID0000000000000000000000", GUI_SYS_MSG_STR_LEN);
	#if 0
		//测试出报告时在配置文件里添加相应的字段和修改对应的信息即可， 暂时保留。by qiaohaijun
		strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptds__GetDeviceInformationResponse->SerialNumber, \
				GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "factoryID", 0, "ID0000000000000000000000"), MAX_STRING_LEN);

		strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptds__GetDeviceInformationResponse->HardwareId, \
				GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "prdctID", 0, "X0000"), MAX_STRING_LEN);

		strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptds__GetDeviceInformationResponse->Manufacturer, \
				GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "manufacturer", 0, "xxxxxx"), MAX_STRING_LEN);

		strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptds__GetDeviceInformationResponse->Model, \
				GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "model", 0, "xxxxxx"), MAX_STRING_LEN);
	#endif
		//硬件ID
		iProductModel = OnvifCmdGetProductModel();
		if(iProductModel < 0)
		{
			return TD_ERROR;
		}
		sprintf(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->HardwareId, "%X", iProductModel);
		
		//更新制造商信息
		pTmp = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "manufacturer", 0, "xxxxxx");
		if(NULL != pTmp)
		{
			strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Manufacturer, pTmp, strlen(pTmp));
			if(0 == strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Manufacturer, "xxxxxx"))
			{
				strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Manufacturer, "AglaiaSense Cam", 16);
			}
		}

		td_printf(0, "The manufacturer is %s", ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Manufacturer);
		
		//更新ProductModel
		pTmp = GetProStr(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "model", 0, "xxxxxx");
		strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Model, pTmp, strlen(pTmp));
		if(!strcmp(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Model, "xxxxxx"))
		{
			strncpy(ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Model, "GS500", 16);
		}
		td_printf(0, "The model is %s", ptOnvifCtrl->m_stOnvifCfg.m_ptDevInfoRsp->Model);	
	}

	return TD_OK;
}
#if 0
/**************************************************
*功能:		针对DVR与IPC不一致的参数
				进行更新
*参数:		_pThis			全局变量结构体指针
*返回值:	0: 成功, !0: 失败
*修改历史:
	20130129 v2.0 creat by ptb
**************************************************/
int UpdateDVRParam(OnvifCtrl *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iSize = 0;
	int i = 0;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	iSize = ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgRsp->__sizeConfigurations;
	if(iSize <= 0)
	{
		return TD_ERROR;
	}
	
	for(i = 0; i < iSize; i++)
	{
		//H264
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->GovLengthRange->Min = VECOH264GLRMIN_DVR;
		ptOnvifCtrl->m_stOnvifCfg.m_ptVideoEncoderCfgOptRsp->Options[i].H264->GovLengthRange->Max = VECOH264GLRMAX_DVR;
	}
	
	return TD_OK;
}
#endif
/**************************************************
*功能:		共享内存通信,更新全局变量g_stOnvifCtrl
*参数:		_pThis			g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
       20120224 v1.0 creat by pangtb
       20130129 v2.0 modify by ptb
**************************************************/
static int GVarUpdate(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	//更新视频参数信息
	if(TD_OK != UpdateVideoInfoAll(ptOnvifCtrl))
	{
		td_printf(0, "%s %d %s UpdateVideoInfoAll is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}

	//更新网络参数信息
	if(TD_OK != UpdateNetworkInfo(ptOnvifCtrl))
	{
		td_printf(0, "%s %d %s =====UpdateNetworkInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}

	//更新高清参数,DVR不支持高清参数
	if(DEVICE_IPC == ptOnvifCtrl->m_stNvsInfo.m_iDeviceType)
	{
		if(TD_OK != UpdateIspInfo())
		{
			td_printf(0, "%s %d %s :UpdateIspInfo is error!\n", __FILE__, __LINE__, __FUNCTION__);
		}
	}
	else
	{
		ptOnvifCtrl->m_stFlag.m_iIspFlag = 0;
	}

	//更新系统信息
	if(TD_OK != UpdateSysInfo(ptOnvifCtrl))
	{
		td_printf(0, "%s %d %s =====UpdateSysInfo is faild!\n", __FILE__, __LINE__, __FUNCTION__);
	}

	return TD_OK;
}
#if 0
/**************************************************
名称:		HeartBeatInit		
功能:		初始化心跳线程
参数:		_pThis
返回值:	0: 成功, !0: 失败
修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
int HeartBeatInit(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	pthread_t heartbeat;
	int iRet = 0;

	iRet = pthread_create(&heartbeat, NULL, onvif_heartbeat, NULL);
	pthread_detach(heartbeat);
	if(iRet < 0)
	{
		perror("heartbeat pthread create");
		return TD_ERROR;
	}

	return TD_OK;
}

/**************************************************
*功能:		共享内存初始化并更新全局变量g_stOnvifCtrl
*参数:		_pThis
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int ShmInit(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iProductModel = 0;
	int iRet = 0;
	int iTid = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}
	
	


	return TD_OK;
}

/**************************************************
*功能:		PTZ初始化
*参数:		_pThis
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int PtzInit(void *_pThis)
{
	pthread_t tTid;

	if(pthread_create(&tTid, NULL, PthreadCheckPtzTimeout, _pThis) < 0)
	{
		return TD_ERROR;
	}

	return TD_OK;
}
#endif
/**************************************************
*功能:		tcp soap init
*参数:		_pThis
*返回值:	>0: 成功
				0 >: 失败
*修改历史:20131031 v2.3.1 creat by ptb
**************************************************/
static int TcpInit(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	struct soap *ptTcpSoap = NULL;
	char *pcIpaddr = NULL;
	int iTcpPort = 0;
	int iRet = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	ptTcpSoap = &ptOnvifCtrl->m_stSoapTcp;
	pcIpaddr  = (char *)ptOnvifCtrl->m_stNvsInfo.m_cLocalIP;
	iTcpPort  = ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort;
	printf("ipaddr:%s, port:%d\n", pcIpaddr, iTcpPort);
	soap_init(ptTcpSoap);
	soap_set_mode(ptTcpSoap, SOAP_C_UTFSTRING);
	
	//soap 参数设置
	ptTcpSoap->socket_flags   = MSG_NOSIGNAL;//屏蔽SIGPIPE
	ptTcpSoap->bind_flags     = SO_REUSEADDR;
	ptTcpSoap->accept_timeout = ACCEPT_TIMEOUT;//accept超时
	ptTcpSoap->send_timeout   = SEND_TIMEOUT;
	ptTcpSoap->recv_timeout   = RECV_TIMEOUT;
	ptTcpSoap->accept_flags   = SO_LINGER;
	ptTcpSoap->linger_time    = LINGER_TIMEOUT;
	
	//soap bind
	iRet = soap_bind(ptTcpSoap, pcIpaddr, iTcpPort, BACK_LOG);
	
	return iRet;
}

/**************************************************
*功能:		udp soap init
*参数:		_pThis
*返回值:	>0: 成功
				0 >: 失败
*修改历史:20131031 v2.3.1 creat by ptb
**************************************************/
static int UdpInit(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl 	= (OnvifCtrl *)_pThis;
	struct soap *ptUdpSoap = NULL;
	struct ip_mreq stMreq;
	char *pcMultIp = NULL;
	int iMultPort = 0;
	int iRet = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	ptUdpSoap = &ptOnvifCtrl->m_stSoapUdp;
	pcMultIp  = (char *)ptOnvifCtrl->m_stNvsInfo.m_cMultIP;
	iMultPort = ptOnvifCtrl->m_stNvsInfo.m_iMultPort;

	//udp soap init
	soap_init1(ptUdpSoap, SOAP_IO_UDP);
	ptUdpSoap->socket_flags   = MSG_NOSIGNAL;		//屏蔽SIGPIPE
	ptUdpSoap->namespaces     = namespaces_probe;
	ptUdpSoap->bind_flags     = SO_REUSEADDR;
	ptUdpSoap->accept_timeout = ACCEPT_TIMEOUT;	//accept超时
	ptUdpSoap->send_timeout   = SEND_TIMEOUT;
	ptUdpSoap->accept_flags   = SO_LINGER;
	ptUdpSoap->linger_time    = LINGER_TIMEOUT;

	//soap bind
	iRet = soap_bind(ptUdpSoap, NULL, iMultPort, BACK_LOG);
	if(iRet < 0)
	{
		td_printf(0, "%s %d soap_bind error!\n", __FUNCTION__, __LINE__);
		return iRet;
	}
	
	//join muticast
	memset(&stMreq, 0, sizeof(stMreq));
	
	stMreq.imr_multiaddr.s_addr = inet_addr(pcMultIp);
	stMreq.imr_interface.s_addr = htonl(INADDR_ANY);
	iRet = setsockopt(ptUdpSoap->master, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMreq, sizeof(struct ip_mreq));
	if(iRet < 0)
	{
		td_printf(0, "%s %d %s setsockopt error!\n", __FILE__, __LINE__, __FUNCTION__);
		return iRet;
	}

	return iRet;
}

/**************************************************
*功能:		初始化gsoap网络
*参数:		_pThis
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int SoapInit(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	int iRet = 0;

	iRet = TcpInit(_pThis);
	if(iRet < 0)
	{
		printf("%s %d TcpInit error!\n", __FUNCTION__, __LINE__);
		return iRet; 
	}

	iRet = UdpInit(_pThis);
	if(iRet < 0)
	{
		printf("%s %d UdpInit error!\n", __FUNCTION__, __LINE__);
		return iRet; 
	}
#if 0
	iRet = EventInit(_pThis);
	if(iRet < 0)
	{
		printf("%s %d EventInit error!\n", __FUNCTION__, __LINE__);
		return iRet;
	}
	#endif
	return TD_OK;
}

/**************************************************
*功能:		反初始化gsoap网络
*参数:		_pThis			全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int SoapUnInit(void *_pThis)
{
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	if(NULL == ptOnvifCtrl)
	{
		return -1;
	}

	//destroy lock
	pthread_mutex_destroy(&ptOnvifCtrl->m_stMutexLock);

	return TD_OK;
}
#if 0
/**************************************************
*功能:		事件管理器初始化并更新全局变量g_stOnvifCtrl
*参数:		_pThis
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120324 v1.0 creat by czl
**************************************************/
static int EventMsgInit(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	pthread_t tid;
	int i = 0;
	int j = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	
	for(i = 0; i < MAX_CONSUMER_NUM; i++)
	{
		ptOnvifCtrl->m_stManager.ConsumerInfoList[i] = NULL;
	}

	for(j = 0; j < TOPIC_NUM; j++)
	for(i = 0; i < MAX_CONSUMER_NUM; i++)
	{
		ptOnvifCtrl->m_stManager.ConsumerIndexList[j][i].m_iChnMask = 0;
	}

	pthread_mutex_init(&ptOnvifCtrl->m_stManager.m_stEventLock, NULL);

	if(pthread_create(&tid, NULL, ThreadTimer, (void *)ptOnvifCtrl) < 0)
	{
		td_printf(0, "create ThreadTimer failed\n");
		return TD_ERROR;
	}

	pthread_detach(tid);

	return TD_OK;
}
#endif
/**************************************************
*功能:		系统初始化
*参数:		_pThis:		全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int Init(void *_pThis)
{
	td_printf(0, "%s %d %s start!  \n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iRet = 0;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	//全局变量初始化
	memset(ptOnvifCtrl, 0, sizeof(OnvifCtrl));

	//lock init
	pthread_mutex_init(&ptOnvifCtrl->m_stMutexLock, NULL);

	ptOnvifCtrl->m_stNvsInfo.m_iDeviceType = DEVICE_IPC;
	ptOnvifCtrl->m_stNvsInfo.m_iVINum = 2;
	ptOnvifCtrl->m_stNvsInfo.m_iVENum = ptOnvifCtrl->m_stNvsInfo.m_iVINum;// * MAX_STREAM;
	ptOnvifCtrl->m_stNvsInfo.m_iVONum = 0;

	//配置文件初始化
	if(ProInit(ptOnvifCtrl))
	{
		td_printf(0, "%s %d %s ProInit error!\n", __FILE__, __LINE__, __FUNCTION__);
		if(OnvifCmdRebootPlatform())
		{
			return TD_ERROR; 
		}
		exit(1);
		return TD_ERROR; 
	}


	GetLocalIP((char *)ptOnvifCtrl->m_stNvsInfo.m_cLocalIP);
	ptOnvifCtrl->m_stNvsInfo.m_iLTcpPort = 8080;

	//onvif结构体初始化
	if(GVarInit(ptOnvifCtrl))
	{
		td_printf(0, "%s %d %s GVarInit error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}

	//通过共享内存刷新全局变量
	if(GVarUpdate(ptOnvifCtrl))
	{
		td_printf(SYS_INFO_PRINTF, "%s %d %s GVarUpdate error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR;
	}
	#if 0
	//事件管理器初始化
	if(EventMsgInit(ptOnvifCtrl))
	{
		td_printf(SYS_INFO_PRINTF, "%s %d %s EventMsgInit error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR; 
	}
	
	//PTZ超时管理初始化
	if(PtzInit(ptOnvifCtrl))
	{
		td_printf(SYS_INFO_PRINTF, "%s %d %s PtzInit error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR; 
	}
#endif

	
	ptOnvifCtrl->camera_flag  = GetProInt(ptOnvifCtrl->m_ptOnvifDict, "device", 0, "camera_num", 0, 0);
	ptOnvifCtrl->m_rtsp_svr_ctrl[0].width  = GetProInt(ptOnvifCtrl->m_ptOnvifDict, "profile_01", 0, "resolution_width", 0, 1920);
	ptOnvifCtrl->m_rtsp_svr_ctrl[0].height = GetProInt(ptOnvifCtrl->m_ptOnvifDict, "profile_01", 0, "resolution_height", 0, 1080);
	ptOnvifCtrl->m_rtsp_svr_ctrl[1].width  = GetProInt(ptOnvifCtrl->m_ptOnvifDict, "profile_02", 0, "resolution_width", 0, 1920);
	ptOnvifCtrl->m_rtsp_svr_ctrl[1].height = GetProInt(ptOnvifCtrl->m_ptOnvifDict, "profile_02", 0, "resolution_height", 0, 1080);
	
	//soap网络初始化
	if(SoapInit(ptOnvifCtrl))
	{
		td_printf(SYS_INFO_PRINTF, "%s %d %s SoapInit error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR; 
	}
#if 0
	//rtsp
	if(RtspServerInit(ptOnvifCtrl))
	{
		td_printf(SYS_INFO_PRINTF, "%s %d %s RtspServerInit error!\n", __FILE__, __LINE__, __FUNCTION__);
		return TD_ERROR; 
	}
#endif

	td_printf(SYS_INFO_PRINTF, "%s %d %s success !\n", __FILE__, __LINE__, __FUNCTION__);
	return iRet;
}

/**************************************************
*功能:		系统反初始化,释放内存
*参数:		_pThis:		全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int UnInit(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	
	if(NULL == ptOnvifCtrl)
	{
		return TD_ERROR;
	}

	//soap uninit
	SoapUnInit(ptOnvifCtrl);

	//rtsp uninit
	//RtspServerUnInit(&ptOnvifCtrl->m_stRtspServer);

	return TD_OK;
}

/**************************************************
*功能:		soap tcp server 线程函数
*参数:		_pArg:		传入参数
*返回值:	
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
void *OnvifTcpServer(void *_pArg)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	SOAP_SOCKET iSockTcp;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pArg;
	struct soap *ptTcpSoap = NULL;
	func_info(-1);
	
	if(NULL == ptOnvifCtrl)
	{
		return NULL;
	}

	ptTcpSoap = &ptOnvifCtrl->m_stSoapTcp;

	//tcp循环扫描请求
	do
	{
		iSockTcp = soap_accept(ptTcpSoap);
		if(!soap_valid_socket(iSockTcp))
		{
			perror("accept failed!");
			td_printf(SYS_INFO_PRINTF, "%s %d %s socket accpet error:%d!\n", __FILE__, __LINE__, __FUNCTION__, iSockTcp);
			soap_print_fault(&ptOnvifCtrl->m_stSoapTcp, stderr);
			
			goto ERR;
		}
		td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
		//soap解析请求
		soap_serve(ptTcpSoap);
ERR:
		#ifdef WITH_TESTTOOL
		sleep(1);
		#endif

		usleep(10 * 1000);
		soap_destroy(ptTcpSoap);
		soap_end(ptTcpSoap);
	}while(tcp_svr_exit_flag);

	soap_done(ptTcpSoap);
	return NULL;
}

/**************************************************
*功能:		soap udp server 线程函数
*参数:		_pArg:		传入参数
*返回值:	
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
void *OnvifUdpServer(void *_pArg)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pArg;
	struct soap *ptUdpSoap = NULL;
	func_info(-1);
	
	if(NULL == ptOnvifCtrl)
	{
		return NULL;
	}

	ptUdpSoap = &ptOnvifCtrl->m_stSoapUdp;

	//udp循环扫描请求
	do
	{
		//soap解析请求
		soap_serve(ptUdpSoap);
		soap_destroy(ptUdpSoap);
		soap_end(ptUdpSoap);
	}while(udp_svr_exit_flag);

	soap_done(ptUdpSoap);
	return NULL;
}

void *start_rtsp_server_left(void *_pArg)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pArg;
	func_info(-1);
	
	if(NULL == ptOnvifCtrl)
	{
		return NULL;
	}

	char rtsp_server_start_cmd[256] = {0};

	sprintf(rtsp_server_start_cmd, "gst-variable-rtsp-server -p %d -u \"v4l2src device=/dev/video2 ! videocrop left=0 right=0 top=0 bottom=410 ! imxvideoconvert_g2d rotation=0 ! video/x-raw,width=%d,height=%d ! vpuenc_h264 ! rtph264pay name=pay0 pt=96\"", \
		ptOnvifCtrl->m_stRtspServer.m_iLPort, ptOnvifCtrl->m_rtsp_svr_ctrl[0].width, ptOnvifCtrl->m_rtsp_svr_ctrl[0].height);
	int status = system(rtsp_server_start_cmd);
    if (status == 0) {
		td_printf(0, "%s %d %s: rtsp server start success !\n", __FILE__, __LINE__, __FUNCTION__);
    } else {
		td_printf(0, "%s %d %s: rtsp server left exit !\n", __FILE__, __LINE__, __FUNCTION__);
    }

	return NULL;
}

void *start_rtsp_server_right(void *_pArg)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pArg;
	func_info(-1);
	
	if(NULL == ptOnvifCtrl)
	{
		return NULL;
	}

	char rtsp_server_start_cmd[256] = {0};

	sprintf(rtsp_server_start_cmd, "gst-variable-rtsp-server -p %d -u \"v4l2src device=/dev/video3 ! videocrop left=0 right=0 top=0 bottom=410 ! imxvideoconvert_g2d rotation=0 ! video/x-raw,width=%d,height=%d ! vpuenc_h264 ! rtph264pay name=pay0 pt=96\"", \
		ptOnvifCtrl->m_stRtspServer.m_iLPort+1, ptOnvifCtrl->m_rtsp_svr_ctrl[1].width, ptOnvifCtrl->m_rtsp_svr_ctrl[1].height);
	int status = system(rtsp_server_start_cmd);
    if (status == 0) {
		td_printf(0, "%s %d %s: rtsp server start success !\n", __FILE__, __LINE__, __FUNCTION__);
    } else {
		td_printf(0, "%s %d %s: rtsp server right exit !\n", __FILE__, __LINE__, __FUNCTION__);
    }

	return NULL;
}

#if 0

/**************************************************
*功能:		soap tcp server 线程函数
*参数:		_pArg:		传入参数
*返回值:	
*修改历史:    20130411 v2.3.0 creat by ptb
**************************************************/
void *OnvifEventServer(void *_pArg)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	SOAP_SOCKET iSockEvent;
	OnvifCtrl *ptOnvifCtrl = _pArg;
	struct soap *ptEventSoap = NULL;
	
	func_info(-1);
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return NULL;
	}

	ptEventSoap = &ptOnvifCtrl->m_stSoapEvent;

	//循环扫描请求
	do
	{
		iSockEvent = soap_accept(ptEventSoap);
		if(!soap_valid_socket(iSockEvent))
		{
			td_printf(SYS_INFO_PRINTF, "%s %d %s socket accpet error:%d!\n", __FILE__, __LINE__, __FUNCTION__, iSockEvent);
			soap_print_fault(&ptOnvifCtrl->m_stSoapEvent, stderr);
			goto ERR;
		}

		//soap解析请求
		soap_serve(ptEventSoap);

ERR:
		usleep(10 * 1000);
		soap_destroy(ptEventSoap);
		soap_end(ptEventSoap);
	}while(1);

	soap_done(ptEventSoap);
	return NULL;
}
//#endif
#endif

/**************************************************
*功能:		开启onvif服务
*参数:		_pThis:		onvif控制变量
*返回值:	0: 成功; -1: 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
static int StartServer(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	pthread_t m_threadTcp;
	pthread_t m_threadUdp;
	pthread_t m_threadEvent;

	pthread_t m_thread_rtsp;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iRet = 0;
	
	if(pointer_valid_check(ptOnvifCtrl))
	{
		return TD_ERROR;
	}

	//tcp server
	iRet = pthread_create(&m_threadTcp, NULL, (void* (*)(void*))OnvifTcpServer, (void *)ptOnvifCtrl);
	if(TD_OK != iRet)
	{
		td_printf(0, "%s %d %s soap_tcp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);	
		return TD_ERROR;
	}
	pthread_detach(m_threadTcp);

	//udp server
	iRet = pthread_create(&m_threadUdp, NULL, (void* (*)(void*))OnvifUdpServer, (void *)ptOnvifCtrl);
	if(TD_OK != iRet)
	{
		td_printf(0, "%s %d %s soap_udp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);	
		return TD_ERROR;
	}
	pthread_detach(m_threadUdp);

	//start rtsp server
	if (ptOnvifCtrl->camera_flag == 0 ) {
		iRet = pthread_create(&m_thread_rtsp, NULL, (void* (*)(void*))start_rtsp_server_left, (void *)ptOnvifCtrl);
		if(TD_OK != iRet)
		{
			td_printf(0, "%s %d %s start_rtsp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);	
			return TD_ERROR;
		}
		pthread_detach(m_thread_rtsp);

		iRet = pthread_create(&m_thread_rtsp, NULL, (void* (*)(void*))start_rtsp_server_right, (void *)ptOnvifCtrl);
		if(TD_OK != iRet)
		{
			td_printf(0, "%s %d %s start_rtsp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);	
			return TD_ERROR;
		}
		pthread_detach(m_thread_rtsp);
	}else if(ptOnvifCtrl->camera_flag == 1){
		iRet = pthread_create(&m_thread_rtsp, NULL, (void* (*)(void*))start_rtsp_server_left, (void *)ptOnvifCtrl);
		if(TD_OK != iRet)
		{
			td_printf(0, "%s %d %s start_rtsp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);	
			return TD_ERROR;
		}
		pthread_detach(m_thread_rtsp);
	}else if(ptOnvifCtrl->camera_flag == 2) {
		iRet = pthread_create(&m_thread_rtsp, NULL, (void* (*)(void*))start_rtsp_server_right, (void *)ptOnvifCtrl);
		if(TD_OK != iRet)
		{
			td_printf(0, "%s %d %s start_rtsp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);	
			return TD_ERROR;
		}
		pthread_detach(m_thread_rtsp);
	}

	return TD_OK;
}
#if 0

/**************************************************
*功能:		设备重启
*参数:		_pThis:		全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120302 v1.0 creat by pangtb
**************************************************/
static int SystemReboot(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	int iRet = 0;
	
	//send bye to muticast
	if(ptOnvifCtrl->m_stOnvifCfg.m_ptDiscModeRsp->DiscoveryMode == tt__DiscoveryMode__Discoverable)
	{
		SendByeToClient(ptOnvifCtrl);
	}
	
	iRet = OnvifCmdRebootReq();
	if(TD_OK == iRet)
	{
		//直接退出,以免再次onvif通信
		exit(0);	
	}

	return TD_OK;
}

/**************************************************
*功能:		onvif重启
*参数:		_pThis:		全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
	20131225 creat by huweixu
**************************************************/
static int PlatformReboot(void *_pThis)
{
	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);
	//OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;

	if(OnvifCmdRebootPlatform())
	{
		return TD_ERROR; 
	}
	exit(1);
	
	return TD_OK;
}
#endif
/**************************************************
*功能:		设备检测,监测配置文件,线程状态,设备重启等
*参数:		_pThis:		全局变量g_stOnvifCtrl
*返回值:	0: 成功, !0: 失败
*修改历史:
	20120302 v1.0 creat by pangtb
**************************************************/
int CheckDevice(void * _pThis)
{
	static int iFlag = 1;
	OnvifCtrl *ptOnvifCtrl = (OnvifCtrl *)_pThis;
	int iRet = 0;
	pthread_t m_thread_rtsp_left;
	pthread_t m_thread_rtsp_right;

	if(pointer_valid_check(ptOnvifCtrl))
	{
		td_printf(0xff0000, "CheckDevice ptOnvifCtrl=NULL");
		return TD_ERROR;
	}

	if(iFlag)
	{
		iFlag = 0;
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "exception_cnt", -1, "count", -1, NULL, 0, CFG_REPLACE);
		SaveProfile(ptOnvifCtrl->m_ptOnvifDict, CONFIG_ONVIF_PATH);
	}
	
	//发送hello消息
	if(TD_TRUE == ptOnvifCtrl->m_stFlag.m_iSendHelloFlag)
	{
		SendHelloToClient(ptOnvifCtrl);
		ptOnvifCtrl->m_stFlag.m_iSendHelloFlag = TD_FALSE;
	}
#if 0
	//恢复出厂设置
	if(TD_TRUE == ptOnvifCtrl->m_stFlag.m_iFactoryDefaultFlag)
	{
		sleep(1);
		if(TD_OK == OnvifCmdRestore())
		{
			exit(0);
		}
	}
	
	//设备重启
	if(TD_TRUE == ptOnvifCtrl->m_stFlag.m_iRebootFlag)
	{
		ptOnvifCtrl->m_stFlag.m_iRebootFlag = TD_FALSE;
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "exception_cnt", -1, "count", -1, NULL, 0, CFG_REPLACE);
		SaveProfile(ptOnvifCtrl->m_ptOnvifDict, CONFIG_ONVIF_PATH);
		SystemReboot(ptOnvifCtrl);
	}
	
	//onvif重启add by huweixu 20131225
	if(TD_TRUE == ptOnvifCtrl->m_stFlag.m_iPlatformRebootFlag)
	{
		ptOnvifCtrl->m_stFlag.m_iPlatformRebootFlag = TD_FALSE;
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "exception_cnt", -1, "count", -1, NULL, 0, CFG_REPLACE);
		SaveProfile(ptOnvifCtrl->m_ptOnvifDict, CONFIG_ONVIF_PATH);
		PlatformReboot(ptOnvifCtrl);
	}
	#endif

	if (ptOnvifCtrl->m_rtsp_svr_ctrl[0].restart_rtsp_flag) {
		sleep(3);

		//start rtsp server
		iRet = pthread_create(&m_thread_rtsp_left, NULL, (void* (*)(void*))start_rtsp_server_left, (void *)ptOnvifCtrl);
		if(TD_OK != iRet)
		{
			td_printf(0, "%s %d %s start_rtsp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);
			return TD_OK;	
		}
		pthread_detach(m_thread_rtsp_left);
		ptOnvifCtrl->m_rtsp_svr_ctrl[0].restart_rtsp_flag = 0;

		char tmp_buf[16] = {0};
		sprintf(tmp_buf, "%d", ptOnvifCtrl->m_rtsp_svr_ctrl[0].width);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "profile_01", -1, "resolution_width", -1, tmp_buf, 0, CFG_REPLACE);

		memset(tmp_buf, 0, 16);
		sprintf(tmp_buf, "%d", ptOnvifCtrl->m_rtsp_svr_ctrl[0].height);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "profile_01", -1, "resolution_height", -1, tmp_buf, 0, CFG_REPLACE);
		
		ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
	}


	if (ptOnvifCtrl->m_rtsp_svr_ctrl[1].restart_rtsp_flag) {
		sleep(3);

		//start rtsp server
		iRet = pthread_create(&m_thread_rtsp_right, NULL, (void* (*)(void*))start_rtsp_server_right, (void *)ptOnvifCtrl);
		if(TD_OK != iRet)
		{
			td_printf(0, "%s %d %s start_rtsp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);
			return TD_OK;
		}
		pthread_detach(m_thread_rtsp_right);
		ptOnvifCtrl->m_rtsp_svr_ctrl[1].restart_rtsp_flag = 0;

		char tmp_buf[16] = {0};
		sprintf(tmp_buf, "%d", ptOnvifCtrl->m_rtsp_svr_ctrl[1].width);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "profile_02", -1, "resolution_width", -1, tmp_buf, 0, CFG_REPLACE);

		memset(tmp_buf, 0, 16);
		sprintf(tmp_buf, "%d", ptOnvifCtrl->m_rtsp_svr_ctrl[1].height);
		SetProStr(ptOnvifCtrl->m_ptOnvifDict, "profile_02", -1, "resolution_height", -1, tmp_buf, 0, CFG_REPLACE);
		
		ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_TRUE;
	}

	//存储配置文件
	if(TD_TRUE == ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag)
	{
		SaveProfile(ptOnvifCtrl->m_ptOnvifDict, CONFIG_ONVIF_PATH);
		ptOnvifCtrl->m_stFlag.m_iCfgChangeFlag = TD_FALSE;
	}
	
	return TD_OK;
}
#if 0
static void block_bad_singals()
{
  	sigset_t   signal_mask;
	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
	
	if (pthread_sigmask (SIG_BLOCK, &signal_mask, NULL))
	{
    		td_printf(0,"block sigpipe error\n");
	}
}

//by aaron
static void signal_handler(int sig, siginfo_t* info, void* p)
{
	static int iSig11En = 1;
	int iRet = 0;
	static int iSignalRebootFlag = 0;
	Dl_info stDlInfo; //stDlInfo结构体中有4个变量，另外两个是:执行文件名字和出错段地址
	ucontext_t *	pstUcontext = (ucontext_t *)p;
	void *pc = 0;
	char cLogBuf[128] = {0}; 
	FILE *fSigFile = NULL;

	if (11 == sig) // dxl 20110602 防止signal11反复记日志
	{
		if (iSig11En)
		{
			iSig11En = 0;
		}
		else
		{
			return;
		}
	}

	//dlq modified at 20130913 for segment fault info.
	if(iSignalRebootFlag == 0)
	{
		iSignalRebootFlag = 1;
		pc = (void *)pstUcontext->uc_mcontext.arm_pc;
		iRet = dladdr(pc, &stDlInfo);
		if(iRet != 0)
		{
			sprintf(cLogBuf, "NO.%d;P-%d;GP-%d;T-%lu;PC-%p<>FNC-%s;FN-%s;RelativeAddr:%d--%d\n", sig, info->si_pid, 
				getpid(), syscall(224), pc, stDlInfo.dli_sname, stDlInfo.dli_fname,(unsigned)(pc - stDlInfo.dli_saddr), iRet);
		}
		else
		{
			sprintf(cLogBuf, "NO.%d;P-%d;GP-%d;T-%lu;PC-%p<add_er>FNC-%s;FN-%s;RelativeAddr:%d--%d\n", sig, info->si_pid, 
				getpid(), syscall(224), pc, stDlInfo.dli_sname, stDlInfo.dli_fname,(unsigned)(pc - stDlInfo.dli_saddr), iRet);
		}
		
		fSigFile = fopen("log/onvif_signalinfo", "wb+");
		if(fSigFile != NULL)
		{
			fwrite(cLogBuf, strlen(cLogBuf), 1, fSigFile);
			fclose(fSigFile);
			system("cat /tmp/onvif >> log/onvif_signalinfo");
			system("date >> log/onvif_signalinfo");
		}
	}
	OnvifCmdRebootPlatform();
	exit(1);
}
//end

void signal_Init(void)
{
	 struct sigaction act;

	 sigset_t* mask = &act.sa_mask;
	 act.sa_flags=SA_SIGINFO;	  /** 设置SA_SIGINFO 表示传递附加信息到触发函数 **/
	 act.sa_sigaction=signal_handler;

		
	 block_bad_singals();

	 //陶永亮 100814 在进行信号处理的时候屏蔽所有信号
	 sigemptyset(mask);   /** 清空阻塞信号 **/

	 //添加阻塞信号
	 sigaddset(mask, SIGABRT);
	 sigaddset(mask, SIGHUP);
	 sigaddset(mask, SIGQUIT);
	 sigaddset(mask, SIGILL);
	 sigaddset(mask, SIGTRAP);
	 sigaddset(mask, SIGIOT);
	 sigaddset(mask, SIGBUS);
	 sigaddset(mask, SIGFPE);
	 sigaddset(mask, SIGSEGV);

	 //安装信号处理函数
	 sigaction(SIGABRT,&act,NULL);
	 sigaction(SIGHUP,&act,NULL);
	 sigaction(SIGQUIT,&act,NULL);
	 sigaction(SIGILL,&act,NULL);
	 sigaction(SIGTRAP,&act,NULL);
	 sigaction(SIGIOT,&act,NULL);
	 sigaction(SIGBUS,&act,NULL);
	 sigaction(SIGFPE,&act,NULL);
	 sigaction(SIGSEGV,&act,NULL);
	 sigaction(SIGINT,&act,NULL);
	 //sigaction(SIGEMT,&act,NULL);
	 //sigaction(SIGTERM,&act,NULL);
	 //signal(SIGCHLD, signal_handler);
}
#endif

void signal_handler(int signal) {
	printf("Caught signal %d\n", signal);
    if (signal == SIGINT) {
        tcp_svr_exit_flag  = 0;
        udp_svr_exit_flag  = 0;
		rtsp_svr_exit_flag = 0;
		main_exit_flag = 0;
    }
}

int main(int argc, char *argv[])
{
	OnvifCtrl *ptOnvifCtrl = &g_stOnvifCtrl;
	
	signal(SIGINT, signal_handler);  // Register the signal handler
	tcp_svr_exit_flag = 1;
	udp_svr_exit_flag = 1;
	rtsp_svr_exit_flag = 1;
	main_exit_flag = 1;

	//init onvif server
	Init(ptOnvifCtrl);

	//开启onvif server
	if(StartServer(ptOnvifCtrl))
	{
		UnInit(ptOnvifCtrl);
		exit(1);
	}

	while(main_exit_flag)
	{
		sleep(1);

		CheckDevice(ptOnvifCtrl);
	}

	UnInit(ptOnvifCtrl);
	return 0;
}

