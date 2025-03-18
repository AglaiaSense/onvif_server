/*************************************************************************************************************
/// @brief this is file for example
/// @file  calless_test.c
/// @author liu
/// @version v1.0.2
/// @date  2013-3-1
*************************************************************************************************************/
#include "guiparameter.h"
#include "dn_main.h"
#include "dictionary.h"
#include "dn_interface.h"
#include "onvif_sdk.h"
#include "callee.h"
#include "DAp2pcmd.h"
#include "errdefs.h"
#include "onvif_shm_cmd.h"
#include "proc.h"
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

extern FILE* fpProc;

Dana_Session g_tSession[MAX_CHANNEL_NUM][MAX_CONNECT_NUM];
int g_iConNum = 0;

#define VIDEO	0x01
#define AUDIO	0x02

//the struct  
typedef struct _MyData {
	HTPCSESS hSess;     //the se

	BOOL	bPlayback;	//simulate playback
	DWORD	dwMediaMask;	//bit 0: send video; bit 1: send audio

	BOOL bRun;
	PA_HTHREAD thd;
} MYDATA;

void PlayAudio(MEDIATYPE fmt, BYTE *pData, int len) 
{
	td_printf(0x12345678, "PlayAudio");
}

int Snapshot(int channel, int quality, BYTE **ppBuff, UINT *size)
{
	Danale_snapshot(channel);
	char *pcDir = "/tmp/snapshot/ftp";
	DIR * pDir = opendir(pcDir);
	if(!pDir)
	{
		return -1;
	}

	struct dirent *pPic = NULL;
	while((pPic = readdir(pDir)))
	{
		if(strlen(pPic->d_name) > 2)break;
	}

	if(!pPic)
	{
		return -1;
	}
	
	char cPicName[64] = {0};
	sprintf(cPicName, "%s/%s", pcDir, pPic->d_name);
	td_printf(0xff0000, "cPicName:%s", cPicName);
	
	FILE *fp = fopen(cPicName, "rb");
	if(fp)
	{
		fseek(fp, 0, SEEK_END);
		*size = ftell(fp);
		rewind(fp);

		*ppBuff = malloc(*size);
		fread(*ppBuff, 1, *size, fp);

		fclose(fp);
		return 0;
	}
	return -1;
}

//----------------------------------
//       callback functions 
//----------------------------------
//if change password, only copy your new password to the param password;NOte:must platform also changed too;
//the user is default admin ,and  the password you will need get from the ipc
BOOL get_password(const char *user, char password[32])
{
	char *pPwd = Danale_GetPsw();
	if(NULL == pPwd)
	{
		strncpy(password, "admin", 31);
	}
	else
	{
		td_printf(0x12345678, "Danale_GetPsw:%s", pPwd);
		strncpy(password, pPwd, 31);
	}
	
	return TRUE;
}

//when receive the command and our server will callback this function control your ipc
void handle_command(HTPCSESS hsess, uint16 cmd, uint32 trans_id, BYTE *pData, int len)
{
	int err = 0;
	td_printf(0x12345678, "===================Handle_command recive: %x\n", cmd);
	
	switch(cmd)
	{
		case CMD_START_VIDEO:
		{ 
			err = Danale_StartVideo(hsess, (char *)pData);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_START_AUDIO:
		{ 

			err = Danale_StartAudio(hsess);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_REQ_IFRAME:
		{
			#if 0
			/************************************************************************/
			/* 
				第一期不必实现
				you can Reference the start live video*/
			/************************************************************************/
			//RequestIFrame(hsess->chn, hsess->vid);
			//DAP2pSessSendSimpleResponse(hsess, CMD_REQ_IFRAME, trans_id);
			#endif
		}
		break;
	case CMD_STOP_VIDEO:
		{
			err = Danale_StopVideo(hsess);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_STOP_AUDIO:
		{
			err = Danale_StopAudio(hsess);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_PTZ_CTRL:
		{
			err = Danale_PTZControl((char *)pData);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_TALKBACK:
		{
			//第一期必须实现
			//Once Recive this command ,revice the audio data packet,so device should
			//	decode and play immediately,only fill the PlayAudio function;
			MEDIATYPE format = pData[0];
			PlayAudio(format, pData + 1, len - 1);
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, NULL, 0, trans_id);
		}
		break;
	case CMD_STOP_TALKBACK:
		{
			//第一期必须实现功能
			//This command is tell device the status,that talk back is stop at this session
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, NULL, 0, trans_id);
		}	
		break;
	case CMD_GET_VIDEO_PARAMETER_NEW:
		{
			union { struct CmdListVideoPara resp; char buff[1300]; } u;
			struct dcs_video_param_new data[3];

			//目前只获取副码流参数
			u.resp.n_num = 3;
			
			Danale_GetEncodeParam(&data);
			#if 0
			td_printf(0, " fps[%d]", data.fps);
			td_printf(0, " gop[%d]", data.gop);
			td_printf(0, " kbps[%d]", data.kbps);
			td_printf(0, " max_fps[%d]", data.max_fps);
			td_printf(0, " save[%d]", data.save);
			td_printf(0, " quality[%d]", data.quality);
			#endif
			memcpy(u.resp.dcs, &data, sizeof(data) * u.resp.n_num);

			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, u.buff,sizeof(struct CmdListVideoPara) + sizeof(struct dcs_video_param_new)*u.resp.n_num, trans_id);
		}
		break;
	case CMD_SET_VIDEO_PARAMETER_NEW:
		{
		 	err = Danale_SetEncodeParam(pData);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_GET_COLOR:
		{
			struct dcs_video_color dvc;
			Danale_GetColor(&dvc);
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, &dvc, sizeof(dvc), trans_id);
		}
		break;
	case CMD_SET_COLOR:
		{
			err = Danale_SetColor((char *)pData);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;

	case CMD_GET_POWER_FREQ:
		{
			/************************************************************************/
			/*
			第一期必须实现
			you can get the power freq from ipc,and send to client
			0: 50HZ
			1: 60HZ
			*/
			/************************************************************************/
			//int *vchn = *((int*)pData);
			//int freq = 0;
			//DAP2pSessSendCmdResponseLast(hsess, 0, CMD_GET_POWER_FREQ, &freq, sizeof(freq), trans_id);
			err = DCSS_UNRECOGNIZED_CMD;
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_SET_POWER_FREQ:
		{
			/************************************************************************/
			/*
			第一期必须实现
			you need set the vaule to ipc
			0:50hz
			1:60hz
			*/
			/************************************************************************/
			//struct CmdSetPowerFreqReq *req = (struct CmdSetPowerFreqReq*)pData;
			err = DCSS_UNRECOGNIZED_CMD;
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_SNAPSHOT:
		{
			td_printf(0xff0000, "CMD_SNAPSHOT");
			//第一期必须实现
			//Snap a picture from channel req->channel ，then send to client
			BYTE *pBuff;
			UINT size = 0;
			int rlt = 0;
			struct CmdSnapshotReq *req = (struct CmdSnapshotReq*)pData;
			//printf("...CMD_SNAPSHOT...\n");
			if(((rlt = Snapshot(req->channel, req->quality, &pBuff, &size)) == 0) )
			{
				BYTE *ptr = pBuff;
				while(size)
				{
					int n = size > 1300 ? 1300 : size;
					size -= n;
					if(size)
						DAP2pSessSendCmdResponse0(hsess, 0, cmd, ptr, n, trans_id); 
					else
						DAP2pSessSendCmdResponseLast(hsess, 0, cmd, ptr, n, trans_id);
					ptr += n;
				}
				free(pBuff);
			}
			else
			{
				DAP2pSessSendResponseError(hsess, 0, cmd, DCSS_LACK_OF_RESOURCE, trans_id);
			}

		}
		break;
	case CMD_GET_WIFI:
		{
			
			/************************************************************************/
			/* 
				第一期必须实现
				you should get the ipc already set the wifi
			   amd call DAP2pSessSendCmdResponseLast send to client
			*/
			/************************************************************************/
			#if 0
			struct CmdGetWifiResp resp;
			//for example
			memset(&resp, 0, sizeof(resp));
			strcpy(resp.essid, "Wifi AP 0");
			strcpy(resp.key, "12345678");
			resp.enctype = WIFI_ENCTYPE_WEP;
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, &resp, sizeof(resp), trans_id);
			#else
			err = DCSS_UNRECOGNIZED_CMD;
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
			#endif
		}
		break;
	case CMD_SET_WIFI:
		{
			
			/************************************************************************/
			/*
			第一期必须实现
			the client will send the new wifi information
			you need set it to ipc
			and return the result
			*/
			/************************************************************************/
			//struct CmdSetWifiReq *req = (struct CmdSetWifiReq*)pData;
			err = DCSS_UNRECOGNIZED_CMD;
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;

	case CMD_LIST_WIFI_AP:
		{
			
			/************************************************************************/
			/* 
			第一期必须实现
			you need get the wifis information that the ipc can search
			and put in CmdListWifiApResp
			then send to client*/
			/************************************************************************/
			#if 0
			union {	struct CmdListWifiApResp resp; char buff[1024]; } u;
			const struct WifiAP aps[] = {
				{ "WifiAp 1", WIFI_ENCTYPE_WEP, 90 },
				{ "WifiAp 2", WIFI_ENCTYPE_WPA2_AES, 80 }
			};
			memcpy(u.resp.aps, aps, sizeof(aps));
			u.resp.nAP = 2;
			//resp.enctype = WIFI_ENCTYPE_WEP;			
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, u.buff,sizeof(struct CmdListWifiApResp) + sizeof(struct WifiAP)*u.resp.nAP, trans_id);
			#else
			err = DCSS_UNRECOGNIZED_CMD;
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
			#endif
		}
		break;
	case CMD_GET_FLIP:
		{
			
			/************************************************************************/
			/*
			第一期必须实现
			get the ipc's state
			0:Upright
			1:Flip Horizontal
			2:Flip Vertical 
			3:turn 180
			*/
			/************************************************************************/
			#if 0
			int rot = 0;
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, &rot, sizeof(int), trans_id);
			#else
			err = DCSS_UNRECOGNIZED_CMD;
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
			#endif
		}
		break;
	case CMD_SET_FLIP:
		{
			err = Danale_SetFlip((char *)pData);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_CHANGE_PSWD:
		{
			/************************************************************************/
			/*
			第一期必须实现
			Modify the access danale's password
			*/
			/************************************************************************/
			td_printf(0xff0000, "CMD_CHANGE_PSWD");
			err = Danale_SetPsw(pData);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_Get_BASEINFO:
		{
			struct CmdGetBaseInfo pDev;
			DABaseInfoCalleeget(&pDev);
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, &pDev, sizeof(pDev), trans_id);
		}
		break;
	case CMD_GET_OSDINFO:
		{
			#if 0
			/***********************************************************************/
			/*  
			第一期必须实现
			 This command  should respons those OSD setting information,then respons 
			 to client

			*/
			/************************************************************************/
			printf("Get OSD information******\n");
			struct  DAOSDInfo resp={0,0,leftup,rightup,NULL,0};
			
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(struct DAOSDInfo), trans_id);
			#else
			err = DCSS_UNRECOGNIZED_CMD;
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
			#endif	
		}
		break;
	case CMD_SET_OSDINFO:
		{
			
			/***********************************************************************/
			/* 
			第一期必须实现
			 Get the command should set those OSD setting information to device

			*/
			/************************************************************************/
			//struct  DAOSDInfo *req = (struct  DAOSDInfo *)pData;
			err = DCSS_UNRECOGNIZED_CMD;
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
		}
		break;
	case CMD_GET_NETINFO:
		{
			struct DANetInfo resp;
			Danale_GetNetworkInfo(&resp);
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(struct DANetInfo), trans_id);
		}
		break;
	case CMD_SET_NETINFO:
		{
			err = Danale_SetNetworkInfo(pData, 1);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
			if(0 == err)
			{
				DAP2pCalleeTerminate();
				DAP2pCalleeCleanup();
				OnvifCmdRebootReq();	
			}
		}
		break;
	case CMD_SET_INIT:
		{
			err = Danale_RebootOrRestore(pData);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);
			if(0 == err)
			{
				DAP2pCalleeTerminate();
				DAP2pCalleeCleanup();
				exit(0);	
			}
		}
		break;
	case CMD_GET_TIME:
		{
			/***********************************************************************/
			/*    
			第一期必须实现
			 This command  should respons those TIME setting information,then respons 
			 to client

			*/
			/************************************************************************/
			struct DATimeInfo resp;
			Danale_GetTime(&resp);
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(struct DATimeInfo), trans_id);
				
		}
		break;
	case CMD_SET_TIME:
		{
			/***********************************************************************/
			/*  
			第一期必须实现
			 Get the command should set those TIME setting information to device

			*/
			/************************************************************************/
			err = Danale_SetTime(pData);
			DAP2pSessSendResponseError(hsess, 0, cmd, err, trans_id);	
		}
		break;
	case CMD_GET_CLOUdALARM:
		{
			/***********************************************************************/
			/*    
			第一期必须实现
	    		This command  should respons the device current cloud alarm function if is
	    		open or close information to the client
	     		respons to client
 
     		 	*/
			/************************************************************************/			
			struct CloudAlarm resp ={0,1,1,1,1,1,1,1};
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(resp), trans_id);
		}
		break;
	case CMD_SET_CLOUDALARM:
		{
			
			/***********************************************************************/
			/*    
			第一期必须实现
	    		This command  should respons the device current cloud alarm function if is
	    		open or close information to the client
	     		respons to client
 
     		 	*/
			/************************************************************************/			
			//struct CloudAlarm *req =( struct CloudAlarm *)pData;
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
		}
		break;
	/*--------------------以下信令demo都是一期不需要实现的--------------------------*/
	case CMD_SET_DAINFO:
		{
			#if 0	
				/***********************************************************************/
				/*	  
				第一期不必实现
				Set Danale's information into ipc
				*/
				/************************************************************************/									
				printf("Set device base info:\n");
				struct CmdDAInfo *req = (struct CmdDAInfo*)pData;
				DAP2pSessSendCmdResponseLast(hsess, 0, cmd, NULL, 0, trans_id);
			#endif
		}
		break;
	case CMD_GET_DEFAULT:
		{
			#if 0		
				/***********************************************************************/
				/*	  
				第一期不必实现
				Transparent channel
		
				*/
				/************************************************************************/
				printf("get owner info:\n");
				//eg:
		
				printf("%s\n",pData);
				char *buf = pData;
				//eg:send back pData to client
				int max_len = 1300;//must be 1300
				int size=strlen(pData)+1;
				int offset=0;
				if(strcmp(pData,"1001"))
				{
					  while(size > max_len)
					   {
						   DAP2pSessSendCmdResponse0(hsess, 0, cmd, buf+offset, max_len, trans_id);
						   offset +=1300;
							size -=1300;
		
					   }
					   DAP2pSessSendCmdResponseLast(hsess, 0, cmd, buf+offset, size, trans_id);
				}
				else
				{
						printf("Send Network Stata:\n");
						char state='0';
						DAP2pSessSendCmdResponseLast(hsess, 0, cmd, &state, 1, trans_id);
				}
					
				DAP2pSessSendCmdResponseLast(hsess, 0, cmd, buf+offset, size, trans_id);
				#endif
			}
			break;
		case CMD_LIST_EVENT:
			{
				#if 0
				/***********************************************************************/
				/*	  
				
				第一期不必实现
				Get the corresponding date 's record events
		
				*/
				/************************************************************************/
				union { struct CmdListEventResp resp; char buff[1400]; } u;
				struct dcs_datetime date={2013,5,21,10,40,10};
				struct dcs_event items[]={0x01,date};
				u.resp.n_item=1;
				memcpy(u.resp.items, items, sizeof(items));
				
				DAP2pSessSendCmdResponseLast(hsess, 0, cmd, u.buff,sizeof(struct CmdListEventResp) + sizeof(struct dcs_event)*u.resp.n_item, trans_id);
				#endif
			}
			break;
		case CMD_SEARCH_RECORD:
			{
				#if 0
				/***********************************************************************/
				/*	  
				
				第一期不必实现
				should respons those record file that meet condition ,then send to client
		
				*/
				/************************************************************************/
				printf("Search record :\n");
				union { struct CmdListSearchRecord resp; char buff[1400]; } u;
				struct dcs_datetime date[]={{2013,5,21,10,40,10},{2013,6,21,10,40,10},{2013,7,21,10,40,10}};
				u.resp.n_item=3;
				memcpy(u.resp.items, date, sizeof(date));
				printf("sizeof :%d\n",sizeof(struct CmdListSearchRecord));	
				DAP2pSessSendCmdResponseLast(hsess, 0, cmd, u.buff,sizeof(struct CmdListSearchRecord) + sizeof(struct dcs_datetime)*u.resp.n_item, trans_id);
				#endif
			}
			break;

	case CMD_GET_MAILINFO:
			{
				
				/***********************************************************************/
				/*	 
				第一期不必实现
				 This command  should respons those mail setting information,then response
				 to client
		
				*/
				/************************************************************************/
				struct	DAMailInfo resp={"test@danale.com","thingsNIC.com","admin","123456","my@danale.com",8080,0,_SSL};
				DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(struct DAMailInfo), trans_id);
			}
			break;
	case CMD_SET_MAILINFO:
			{
				
				/***********************************************************************/
				/* 
				第一期不必实现
				 Get the command should set those mail setting information to device
		
				*/
				/************************************************************************/
				//struct	DAMailInfo *req = (struct  DAMailInfo *)pData;				
				DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
			}
			break;

	case CMD_GET_DEVLOGINF:
		{
			/***********************************************************************/
			/*    
			第一期不必实现
			 This command  should get and respons meet request condition Log information to 
			 client

			*/
			/************************************************************************/
			//struct  DASearchLOGInf *req = (struct  DASearchLOGInf *)pData;
			union {	struct DAGetLogInfList resp; char buff[1400]; } u;
			struct dcs_datetime date[]={{2013,5,21,10,40,10,0},{2013,6,21,10,40,10,0},{2013,7,21,10,40,10,0}};
			u.resp.num = 3;
			struct LogInf log[]={{date[0],logall,0,"login"},{date[1],logAlarm,1,"login2"},{date[2],logOperating,2,"login3"}};
			memcpy(u.resp.loginf,log,sizeof(struct LogInf)*u.resp.num);
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, u.buff,sizeof(struct DAGetLogInfList) + sizeof(struct LogInf)*u.resp.num, trans_id);
		}
		break;
	case CMD_GET_MotionDetection:
		{
			#if 0
			printf("Get MotionDetection information******\n");
			/***********************************************************************/
			/*    
			 第一期不必实现
			 This command  should  respons meet request condition MotionDetection 
			 information to client

			*/
			/************************************************************************/
			int i,j;
			union {	struct DAMotionDetection resp; char buff[1400]; } u;
			struct  DAMotionDetection temp={1,0,1,1,2,3};
			struct DAAlarmArea area[3] = {{1,10,20,200,200},{2,3,4,5,10},{3,5,6,5,10}};
			memcpy(&temp.alarmArea,area,sizeof(area));
			temp.timenum = 3;
			struct DADetectionTime  decTime_1[] ={{1,30,2,30},{2,30,3,30}};
			struct DADetectionTime  decTime_2[] ={{4,30,5,30},{6,30,7,30}};
			struct DADetectionTime  decTime_3[] ={{8,30,9,30},{10,30,11,30}};
			
			struct DATimePlan plan[3]={{1,2},{2,2},{3,2}};
			
				
			memcpy(plan[0].decTime,decTime_1,sizeof(decTime_1));
			memcpy(plan[1].decTime,decTime_2,sizeof(decTime_2));
			memcpy(plan[2].decTime,decTime_3,sizeof(decTime_3));
				
			memcpy(&u.resp,&temp,sizeof(struct DAMotionDetection));
			memcpy(u.resp.plan,plan,sizeof(plan));
		
			
			printf("resp.timenum: %d\n",u.resp.timenum);	
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd, u.buff,sizeof(struct DAMotionDetection)+temp.timenum*sizeof(struct DATimePlan), trans_id);
			#endif
		}
		break;
	case CMD_SET_MotionDetection:
		{
			#if 0
			printf("Set MotionDetection information******\n");
			/***********************************************************************/
			/*    
			第一期不必实现
			 This command  should set those MotionDetection setting information
			 to ipcam

			*/
			/************************************************************************/
			int i,j;
			struct  DAMotionDetection *req = (struct  DAMotionDetection *)pData;
			printf("req.OpenIt: %d, req.chn:%d, req.email:%d,req.sen: %d, req.Triggeroutput: %d\n",req->OpenIt,req->chn,req->emailNotification,req->triggerOutput);
			printf("req->.ResolutionType: d%, req->areanum: %d,req->timenum: %d\n",req->ResolutionType,req->areanum,req->timenum);
			if(req->areanum <=5)
			{
			for(i=0;i<req->areanum;i++)
			{
					printf("x: %d,y: %d, h: %d,w: %d\n",req->alarmArea[i].sen,req->alarmArea[i].startx,req->alarmArea[i].starty,req->alarmArea[i].Height,req->alarmArea[i].Width);
			}
			}
			if(req->timenum <= 50)
			{
			for(i=0;i<req->timenum;i++)
		   	{
				printf("req.plan[%d].num: %d,weekInf: %d\n",i,req->plan[i].num,req->plan[i].weekInf);
				if(req->plan[i].num <= 5)
				{
				for(j=0;j<req->plan[i].num;j++)
				{
					printf("req->timeplan.plan[%d].decTime[%d]: from %d-%d to %d-%d\n",i,j,req->plan[i].decTime[j].fromhour,req->plan[i].decTime[j].frommin,req->plan[i].decTime[j].tohour,req->plan[i].decTime[j].tomin);
				}
				}		
			}
			}	
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
			#endif
		}
		break;
	case CMD_GET_I2ODetection:
		{
			#if 0
			int i;
			printf("Get I2ODetection information******\n");
			/***********************************************************************/
			/*    
			第一期不必实现
			 This command  should respons those I2ODetection setting information,then
			 respons to client

			*/
			/************************************************************************/
			int n_item = 3;
			union {	struct DAI2ODetection resp; char buff[1400]; } u;
			struct  DAI2ODetection temp={1,0,1,1,1,10,0,1,0,0,1,5,0x3};
			
			memcpy(&u.resp,&temp,sizeof(struct DAI2ODetection));
			struct DADetectionTime  decTime_1[] ={{1,30,2,30},{2,30,3,30}};
			struct DADetectionTime  decTime_2[] ={{4,30,5,30},{6,30,7,30}};
			struct DADetectionTime  decTime_3[] ={{8,30,9,30},{10,30,11,30}};
			struct DATimePlan plan[3]={{1,2},{2,2},{3,2}};
			memcpy(plan[0].decTime,decTime_1,sizeof(decTime_1));
			memcpy(plan[1].decTime,decTime_2,sizeof(decTime_2));
			memcpy(plan[2].decTime,decTime_3,sizeof(decTime_3));
			memcpy(u.resp.plan,plan,sizeof(plan));
			
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,u.buff,sizeof(struct DAI2ODetection)+n_item*sizeof(struct DATimePlan), trans_id);
			#endif
		}
		break;
	case CMD_SET_I2ODetection:
		{
			int i,j;
			/***********************************************************************/
			/*    
			第一期不必实现
			 Get the command should set those I2ODetection setting information to device

			*/
			/************************************************************************/
			struct  DAI2ODetection *req = (struct  DAI2ODetection *)pData;
			if(req->num<=5)
			{
				for(i=0;i<req->num;i++)
			   	{
					if(req->plan[i].num <= 5)
					{
						for(j=0;j<req->plan[i].num;j++)
						{
						}
					}
				}	
			}
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
		}
		break;
	case CMD_GET_CoverDetection:
		{
			/***********************************************************************/
			/*  
			第一期不必实现
			 This command  should respons those CoverDetection setting information,then
			 respons to client

			*/
			/************************************************************************/
			struct  CoverDetection resp={1,0,3};
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(struct CoverDetection), trans_id);
		}
		break;
	case CMD_SET_CoverDetection:
		{
			/***********************************************************************/
			/*    
			第一期不必实现
			 Get the command should set those CoverDetection setting information to device

			*/
			/************************************************************************/
			//struct  CoverDetection *req = (struct  CoverDetection *)pData;
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
				
		}
		break;
	case CMD_GET_TimingCapture:
		{
			/***********************************************************************/
			/*	  
			第一期不必实现
			 This command  should respons those TimingCapture setting information,then
			 respons to client
		
			*/
			/************************************************************************/
			struct	TimingCaptureDetection resp={1,0,5,1};
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(struct TimingCaptureDetection), trans_id);
		}
		break;
	case CMD_SET_TimingCapture:
		{
			/***********************************************************************/
			/*	  
			第一期不必实现
			 Get the command should set those TimingCapture setting information to device
		
			*/
			/************************************************************************/
			//struct	TimingCaptureDetection *req = (struct  TimingCaptureDetection *)pData;
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
		}
		break;
	case CMD_GET_TimingRecord:
		{
			/***********************************************************************/
			/*    
			第一期不必实现
			 This command  should respons those TimingRecord setting information,then
			 respons to client

			*/
			/************************************************************************/
			struct  TimingRecordDetection resp={1,0,10};
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(struct TimingRecordDetection), trans_id);
		}
		break;
	case CMD_SET_TimingRecord:
		{
			/***********************************************************************/
			/*    
			第一期不必实现
			 Get the command should set those TimingRecord setting information to device

			*/
			/************************************************************************/
			//struct  TimingRecordDetection *req = (struct  TimingRecordDetection *)pData;			
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
		}
		break;
	case CMD_GET_SoundDetection:
		{
			#if 0
			printf("Get SoundDetection information******\n");
			/***********************************************************************/
			/*    
			第一期不必实现
			 This command  should respons those SoundDetection setting information,then
			 respons to client

			*/
			/************************************************************************/
			
			union {	struct DASoundDetection resp; char buff[1300]; } u;
			//memset(u.buff,0,1300);
			struct  DASoundDetection temp={1,0,1,1,2};
			temp.num = 3;
			memcpy(&u.resp,&temp,sizeof(struct DASoundDetection));
			int i,j;
			struct DADetectionTime  decTime_1[5] ={{1,30,2,30},{2,30,3,30}};
			struct DADetectionTime  decTime_2[5] ={{4,30,5,30},{6,30,7,30}};
			struct DADetectionTime  decTime_3[5] ={{8,30,9,30},{10,30,11,30}};
			struct DATimePlan plan[3]={{1,2},{2,2},{3,2}};
			
			memcpy(plan[0].decTime,decTime_1,sizeof(decTime_1));
			memcpy(plan[1].decTime,decTime_2,sizeof(decTime_2));
			memcpy(plan[2].decTime,decTime_3,sizeof(decTime_3));
		
			memcpy(&u.resp.plan,plan,sizeof(plan));
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,u.buff,sizeof(struct DASoundDetection)+u.resp.num*sizeof(struct DATimePlan), trans_id);	
			#endif
		}
		break;
	case CMD_SET_SoundDetection:
		{
			int i,j;
			/***********************************************************************/
			/*    
			第一期不必实现
			 Get the command should set those SoundDetection setting information to device

			*/
			/************************************************************************/
			struct  DASoundDetection *req = (struct  DASoundDetection *)pData;
			if(req->num <= 50)
			{
				for(i=0;i<req->num;i++)
			   	{
					if(req->plan[i].num<=5)
					{
						for(j=0;j<req->plan[i].num;j++)
						{
						}
					}		
				}
			}		
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
		}
		break;
	case CMD_GET_DEVICESTATE:
		{
			/***********************************************************************/
			/*    
			第一期不必实现
    			 This command  should respons those DEVICESTATE  information,then
     			  respons to client
 
     		 */
			/************************************************************************/
			struct DADeviceState resp = {1,2};
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,&resp,sizeof(resp), trans_id);
		}
		break;
	case CMD_WM_AP_WIFI:
		{
			/***********************************************************************/
			/*    
			第一期不必实现
    			This command  should respons those DEVICESTATE  information,then
     			respons to client
 
     		 	*/
			/************************************************************************/
			//struct AP_Wifi *req = (struct AP_Wifi *)pData;
			DAP2pSessSendCmdResponseLast(hsess, 0, cmd,NULL,0, trans_id);
		}
		break;
	default :
		DAP2pSessSendResponseError(hsess, 0, cmd, DCSS_UNRECOGNIZED_CMD, trans_id);
	}
	
}


//--------------------------------------------------------------------------
//the function will create a session for the client
//you will do nothing but for call it ,on the main function starting
//-------------------------------------------------------------------------
int session_created(HTPCSESS hsess)
{
	td_printf(0x12345678, "session_created:%p", hsess);
	#if 0
	MYDATA *p = (MYDATA*)calloc(sizeof(MYDATA), 1);
	p->hSess = hsess;
	DAP2pSessSetUserData(hsess, (void*)p);
	td_printf(0xff0000, "session_created:%p", hsess);
	#endif
	return 0;
}

//--------------------------------------------------------------------------
//the function will aborted session
//you will do nothing
//-------------------------------------------------------------------------
void session_aborted(HTPCSESS hsess)
{
	td_printf(0x12345678, "session_aborted:%p", hsess);
	Danale_StopVideo(hsess);
	#if 0
	MYDATA *pMyData = NULL;
	td_printf(0xff0000, "session_aborted:%p", hsess);
    if(DAP2pSessGetUserData(hsess, (void **)&pMyData) == 0)
	{
		Dana_Session * p_session = (Dana_Session*)Danale_FindSession(&hsess);
		if(p_session)
		{
			p_session->hsess = NULL;
		}
		
		stopMediaThread(pMyData, VIDEO);
		stopMediaThread(pMyData, AUDIO);
		free(pMyData);
	}
	#endif
}

/* get cloud storage information configue file */
void get_storage_info(struct  storage_info *info)
{
	td_printf(0x12345678, "get_storage_info");
	info->type = 1;
	strcpy(info->svr, "https://api.vmall.com/rest.php"); 
	strcpy(info->etoken, "BFKIjx%2FIFoPawaLyDrWt7xt0p8n6KrLVYHvMrUtDHV4E8Co%2FL8oMe%2Fb8LOM%3D");
	strcpy(info->user, "danale");
	strcpy(info->passwd, "123456");
}

/*save cloud storage  information to configue file */
void set_storage_info(struct storage_info *info)
{
	td_printf(0x12345678, "set_storage_info");
	//printf("CallBack set_storage_info:\n");
	/* Save cloud storage infomation */
	//printf("Save cloud storage infomation:\n");
	//printf("type: %d, svr: %s, etoken: %s\n,user: %s, passwd: %s\n",info->type,info->svr,info->etoken,info->user,info->passwd);
	
}
/* Get alarm push service information */
void get_push_info(struct push_info *info)
{
	td_printf(0x12345678, "get_push_info");
	//printf("CallBack get_push_info:\n");
	info->type = 100;
	strcpy(info->svr,"pushsvr.danale.com");

}

/* Save alarm push service information */
void set_push_info(struct push_info *info)
{
	td_printf(0x12345678, "set_push_info");
	//printf("CallBack set_push_info:\n");
	/* save push service infomation*/
	//printf("set push service infomation:\n");
	//printf("type: %d, svr: %s\n",info->type,info->svr);

}

/* set the network param information to device ,and let those information active*/
BOOL set_network_info(struct NetworkInfo *info)
{
	td_printf(0x12345678, "set_network_info");
	struct DAInitSet stReq;
	if(0 == Danale_SetNetworkInfo(info, 0))
	{
		stReq.Type = 0;
		Danale_RebootOrRestore(&stReq);
		return TRUE;
	}
	
	return FALSE;
}
//----------------------------------------------------------------------------------
const char *kernel_servers[] = { "sp1.danale.com","pushsvr.danale.com" };

DAP2PCALLEECBFUNCS CallbackFuncs = {
	.GetPassword = get_password,
	.SessionCreated = session_created,
	.SessionAborted = session_aborted,
	.CommandHandler = handle_command,
	.GetStorageInfo = get_storage_info,
	.SetStorageInfo = set_storage_info,
	.GetPushInfo = get_push_info,
	.SetPushInfo = set_push_info,
	.SetNetworkInfo = set_network_info,
};

void *test_storage(void *p)
{
	int i;
		for(i=0;i<2;i++)
		{
			DAP2pSendpushreq(i, NULL);
			printf("send msgtype: %d\n",i);
			sleep(30);
		}
	return NULL;
}

void *ThreadDanaleInit(void *_pArg)
{
	OnvifCtrl *ptOnvifCtrl = _pArg;
	int iRet = 0;

	//配置环回接口lo
	system("ifconfig lo 127.0.0.1");

	//config_init
	Danale_GetPuId();

	iRet = Danale_BaseInfoInit(ptOnvifCtrl);
	if(TD_OK != iRet)
	{
		goto EXIT;	
	}
	iRet = DAP2pCalleeInitialize(kernel_servers, 2, ptOnvifCtrl->m_stDanaleInfo.m_cPuId, &CallbackFuncs);
	if(TD_OK != iRet)
	{
		goto EXIT;
	}

	return NULL;
EXIT:
	DAP2pCalleeTerminate();
	DAP2pCalleeCleanup();
	return NULL;
}

void *ThreadDanaleHeart(void *_pArg)
{
	OnvifCtrl *ptOnvifCtrl = _pArg;
	char cTmp[128];
	
	func_info(-1);
	while(1)
	{
		if(g_iConNum > 0)
		{
			if(ptOnvifCtrl->m_stDanaleInfo.m_iHeart >= 5)
			{
				DAP2pCalleeTerminate();
				DAP2pCalleeCleanup();
				OnvifCmdRebootPlatform();
				sprintf(cTmp, "echo danale heart lost!!! g_iConNum[%d]> heart.txt", g_iConNum);
				system(cTmp);
				exit(1);
			}
			ptOnvifCtrl->m_stDanaleInfo.m_iHeart++;
		}
		sleep(1);
	}
}

int dana_init(void *_pThis)
{
	pthread_t pId;
	//pthread_t pHeartId;
	int iRet = 0;
	
	iRet = pthread_create(&pId, NULL, (void *)ThreadDanaleInit, _pThis);
	if(TD_OK != iRet)
	{
		td_printf(0, "%s %d %s soap_tcp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);	
		return TD_ERROR;
	}
	pthread_detach(pId);
#if 0
	iRet = pthread_create(&pHeartId, NULL, (void *)ThreadDanaleHeart, _pThis);
	if(TD_OK != iRet)
	{
		td_printf(0, "%s %d %s soap_tcp_server thread created error!\n", __FILE__, __LINE__, __FUNCTION__);	
		return TD_ERROR;
	}
	pthread_detach(pHeartId);
#endif
	return TD_OK;
}


