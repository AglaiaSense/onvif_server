#ifndef __tasp2pcmd_h__
#define __tasp2pcmd_h__

#include "platform_adpt.h"
/************************************************************************/
/* 内部接口 多了设置设备信息

*/
/************************************************************************/

//==================================================================================
//==================================================================================
//==================================================================================

/*
#define CMD_OPEN_CHANNEL	0x100
//Req: struct CmdOpenChannelReq
//resp:

#define CMD_CLOSE_CHANNEL	0x101
//req: struct CmdCloseChannelReq
//resp:
*/
#define CMD_REBOOT        0x102
//req:
//resp:

#define CMD_UPDATE_FW     0x103
//req:
//resp:

#define CMD_START_VIDEO		0x110	/// Start live video
//req: struct CmdStartVideoReq
//resp:

#define CMD_STOP_VIDEO		0x111	/// Stop live video
//req:
//resp:

#define CMD_START_AUDIO		0x112	/// Start live audio
//req:
//resp:

#define CMD_STOP_AUDIO		0x113	/// Stop live audio
//req:
//resp:

#define CMD_REQ_IFRAME		0x114	/// Request I-frame, called after START_VIDEO
//req: 
//resp: 

#define CMD_PTZ_CTRL		0x115
//req: struct CmdPtzCtrlReq
//resp: none

#define CMD_TALKBACK		0x116	//Start Talkback
//req
//resp

#define CMD_STOP_TALKBACK		0x117 //stop Talkback
//req
//resp

#define CMD_GET_POWER_FREQ	0x11E
//req: int vchn;
//resp: int freq;	//POWERFREQ_xxx

#define CMD_SET_POWER_FREQ	0x11F
//req: struct CmdSetPowerFreqReq
//resp:

#define CMD_GET_VIDEO_PARAMETER		0x120	/// Get
//req: struct dcs_chnvideo
//resp: struct dcs_video_param
#define CMD_SET_VIDEO_PARAMETER		0x121	/// Set brmode/bps/fps/quality...
//req: 	struct CmdSetVideoParameterReq
//resp:

#define CMD_GET_COLOR		0x122	/// Get
//req:
//resp: struct dcs_video_color
#define CMD_SET_COLOR		0x123	/// Set brightness/contrast/hue/saturatur ...
//req: struct CmdSetVideoColorReq
//resp:

#define CMD_LIST_DATE		0x030	/// Get the dates when there's record in that day
//req: 
//resp: struct CmdListDateResp

#define CMD_GET_TIMEMAP		0x131	/// Get time map of a date
//req: struct dcs_date
//resp: struct dcs_videotimemap

#define CMD_LIST_EVENT		0x132
//req:  struct CmdListEventReq
//resp: struct CmdListEventResp

#define CMD_LOCATE_AND_PLAY	0x133	/// Locate and playback
//req:	char s_datetime[20] --> yyyy-MM-dd hh:mm:ss
//

#define CMD_SET_XRATE		0x134	/// Set XRate
//req: int xrate;
//

#define CMD_PAUSE_PLAYBACK	0x135
//req: int pause; //1: pause, 0: continue
//resp:

#define CMD_TERMINATE_PLAYBACK	0x136
//req:
//resp:

#define CMD_SNAPSHOT		0x137
//req: struct CmdSnapshotReq
//resp: ushort end_flag;
//      ushort reserved;
//      char data[0];

#define CMD_DOWNLOAD_FILE	0x138
//req: int filepath_len; //length of file path
//     char filepath[0];
//resp: ushort end_flag;
//      ushort reserved;
//      char data[0];

#define CMD_ECHO		0x140
//req: anything
//resp: anything

#define CMD_LIST_WIFI_AP	0x141
//req:
//resp: struct CmdListWifiApResp

#define CMD_GET_WIFI	0x142
//req:
//resp: struct WifiSettng

#define CMD_SET_WIFI	0x143
//req: struct WifiSetting
//resp:

#define CMD_GET_FLIP	0x144//翻动
//req: int vchn;
//resp: int flips;	//combine of VIDEOFLIP_HORZ and VIDEOFLIP_VERT

#define CMD_SET_FLIP	0x145
//req: CmdSetFlipReq
//resp:

#define CMD_CHANGE_PSWD	0x146
//req: char *new_pswd;
//resp:
#define CMD_Get_BASEINFO 0x147
//req: 
//resp: struct CmdGetBaseInfo

#define CMD_SET_DAINFO   0x148
//req: 
//resp: 

#define CMD_GET_DEFAULT 0x149
//req:	void *
//resp: void *

#define CMD_SEARCH_RECORD 0x150
//req:	struct GetSearchRecordReq
//resp: struct CmdListSearchReocrd


#define CMD_SET_VIDEO_PARAMETER_NEW		0x153	/// Set brmode/bps/fps/quality...
//req:	struct dcs_video_param_new
//resp: 

#define CMD_GET_VIDEO_PARAMETER_NEW 0x154
//req:	
//resp: struct dcs_video_param_new

#define CMD_LOCATE_AND_PLAY_NEW	0x155	/// Locate and playback
//req:	CmdListDateTime_NEW
//

#define CMD_PLAYBACK_SELECT 0x156
//req:	LONG        choose playback
//


/************************************************************************/
/* 20130725增加相关菜单设置
*/
/************************************************************************/
#define CMD_GET_MAILINFO  0x200
//req:	null
//resp: struct DAMailInfo

#define CMD_SET_MAILINFO 0x201
//req:	struct DAMailInfo
//resp: 

#define CMD_GET_OSDINFO 0x202
//req:	
//resp: struct DAOSDInfo

#define CMD_SET_OSDINFO 0x203
//req:	struct DAOSDInfo
//resp: 

#define CMD_GET_NETINFO 0x204
//req:	
//resp: struct DANetInfo

#define CMD_SET_NETINFO 0x205
//req:	struct DANetInfo
//resp: 

#define CMD_SET_INIT 0x206
//req:	struct DAInitSet
//resp: 

#define CMD_GET_TIME 0x207
//req:	
//resp: struct DATimeInfo

#define CMD_SET_TIME 0x208
//req:	struct DATimeInfo
//resp: 

#define CMD_GET_DEVLOGINF 0x209
//req:	struct DASearchLOGInf
//resp: struct DAGetLogInfList

#define CMD_GET_MotionDetection  0x20B
//req:	
//resp: struct DAMotionDetection

#define CMD_SET_MotionDetection  0x20C
//req:	struct DAMotionDetection
//resp: 

#define CMD_GET_I2ODetection  0x210 //input and output
//req:	struct DAI2ODetection
//resp: 

#define CMD_SET_I2ODetection  0x211
//req:	struct DAI2ODetection
//resp: 

#define CMD_GET_CoverDetection  0x212 //Cover
//req:	struct CoverDetection
//resp: 

#define CMD_SET_CoverDetection  0x213
//req:	
//resp: struct CoverDetection


#define  CMD_GET_TimingCapture 0x214    //Timing to capture
//req:	struct TimingCaptureDetection
//resp: 

#define  CMD_SET_TimingCapture 0x215
//req:	
//resp: struct TimingCaptureDetection

#define  CMD_GET_TimingRecord 0x216    //Timing to Record
//req:struct TimingRecordDetection
//resp:

#define  CMD_SET_TimingRecord 0x217
//req:
//resp:struct TimingRecordDetection


#define CMD_GET_SoundDetection  0x218 //Cover
//req:	struct DASoundDetection
////resp: 
//
#define CMD_SET_SoundDetection   0x219
////req:	
////resp: struct DASoundDetection
//
//
#define CMD_GET_DEVICESTATE 0x220
//req:
//resp: struct DADeviceState

#define CMD_WM_AP_WIFI	0x221
//req:	struct AP_Wifi
//resp: struct AP_Wifi

//20130930 Application of sence

#define CMD_GET_Scenes 0x230
//req:
//resp:int  0:auto  1:indoor  2:outdoor


#define CMD_SET_Scenes 0x231
//req:int  0:auto  1:indoor  2:outdoor
//resp:

#define CMD_GET_CLOUdALARM 0x232
//req:
//resp:struct CloudAlarm

#define CMD_SET_CLOUDALARM 0x233
//req:struct CloudAlarm
//resp:


//2013-4-12 新增关于获取设备基本信息部分
typedef enum
{
	TYPE_IPCAM,
	TYPE_DVR,
}DevKind;//device type

typedef enum
{
	chn_one=1,
	chn_4num=4,
	chn_8num=8,
	chn_16num=16,
	chn_32num=32,
}ChnNum;

struct CmdGetBaseInfo {
	char daID[20];//DANA ID
	char versionApi[20];//API vertion
	char mainuser[20];//main user name
	char sSn[20];//Device seriaze Number
	char mac[32];//device mac address
	char name[20];//device's name
	char version[20];//Device model
	DevKind type;// DevKind
	int chnum;//ChnNum
	int AllSize;//All Storage size
	int FreeSize;//Free Storage size
};

struct CmdChnInf
{
	int chn;
	char chnname[20];//chn name
	int chnType;//0：analog  1:digital
};


//新增关于设置大拿相关信息
struct CmdDAInfo //该接口只适合内部使用,刷号，记录信息时
{
	char mainServer[32];
	char backupServer[32];
	char daID[20];
	char pwd[20];//default :admin
	char version[20];//当前数据的版本
	char mainuser[20];//激活标志
};


typedef enum {
	BITRATE_LOWEST,
	BITRATE_LOW,
	BITRATE_NORMAL,
	BITRATE_HIGH,
	BITRATE_HIGHEST,
	QUALITY_AUTO = 100,
	QUALITY_MANUALLY = 101	//
} VIDEOQUALITY;

typedef enum {
	DEVTYPE_PHONE,
	DEVTYPE_PAD,
	DEVTYPE_PC
} DEVTYPE;//client device type
/*
   struct CmdOpenChannelReq {
   int	chnType;	//0: command; 1 - media
   int chno;
   int maxFrameSize;
   };

   struct CmdCloseChannelReq {
   int chnType;
   int chno;
   };
   */
struct CmdStartVideoReq {
	int channel;
	DEVTYPE devType;
	unsigned short quality;	//0~4: lowest~highest. 100: auto
	unsigned short vstrm;	//0:main; 1:sub1; 2:sub2...
	int mediaChn;
};

struct CmdStartAudioReq {
	int mediaChn;
};

enum { 
	DAP2P_PTZ_STOP = 100, 
	DAP2P_PTZ_MOVE_UP, 
	DAP2P_PTZ_MOVE_UP_STOP,
	DAP2P_PTZ_MOVE_DOWN,
	DAP2P_PTZ_MOVE_DOWN_STOP,
	DAP2P_PTZ_MOVE_LEFT,
	DAP2P_PTZ_MOVE_LEFT_STOP,
	DAP2P_PTZ_MOVE_RIGHT, 
	DAP2P_PTZ_MOVE_RIGHT_STOP,
	DAP2P_PTZ_MOVE_UPLEFT, 
	DAP2P_PTZ_MOVE_UPLEFT_STOP,
	DAP2P_PTZ_MOVE_DOWNLEFT, 
	DAP2P_PTZ_MOVE_DOWNLEFT_STOP,
	DAP2P_PTZ_MOVE_UPRIGHT, 
	DAP2P_PTZ_MOVE_UPRIGHT_STOP,
	DAP2P_PTZ_MOVE_DOWNRIGHT, 
	DAP2P_PTZ_MOVE_DOWNRIGHT_STOP,
	
	DAP2P_PTZ_IRIS_IN = 117, 
	DAP2P_PTZ_IRIS_IN_STOP, 
	DAP2P_PTZ_IRIS_OUT, 
	DAP2P_PTZ_IRIS_OUT_STOP, 

	DAP2P_PTZ_ZOOM_IN = 121, 
	DAP2P_PTZ_ZOOM_IN_STOP, 
	DAP2P_PTZ_ZOOM_OUT, 
	DAP2P_PTZ_ZOOM_OUT_STOP, 

	DAP2P_PTZ_FOCUS_OUT = 125, 
	DAP2P_PTZ_FOCUS_ON_STOP, 
	DAP2P_PTZ_FOCUS_ON,
	DAP2P_PTZ_FOCUS_OUT_STOP, 

	DAP2P_PTZ_SET_PSP, 
	DAP2P_PTZ_CALL_PSP, 
	DAP2P_PTZ_DELETE_PSP, 

	DAP2P_PTZ_BEGIN_CRUISE_SET, 
	DAP2P_PTZ_SET_CRUISE, 
	DAP2P_PTZ_END_CRUISE_SET, 
	DAP2P_PTZ_CALL_CRUISE, 
	DAP2P_PTZ_DELETE_CRUISE, 
	DAP2P_PTZ_STOP_CRUISE, 

	DAP2P_PTZ_AUTO_SCAN = 138, 
	DAP2P_PTZ_AUTO_SCAN_STOP,

	DAP2P_PTZ_RAINBRUSH_START, 
	DAP2P_PTZ_RAINBRUSH_STOP,
	DAP2P_PTZ_LIGHT_ON, 
	DAP2P_PTZ_LIGHT_OFF,

	DAP2P_PTZ_MAX 
};
struct CmdPtzCtrlReq {
	int channel;
	int code;	//DAP2P_PTZ_xxx
	int para1, para2;
};

struct CmdSnapshotReq {
	int channel;
	int quality;
};

struct dcs_chnvideo {
	int channel;
	int video;
};

struct dcs_date {
	uint16 year;
	uint8 mon;
	uint8 mday;
} __PACKED__;

struct dcs_datetime {
	uint16 year;
	uint8 mon;
	uint8 mday;
	uint8 hour;
	uint8 min;
	uint8 sec;
	uint8 dummy;
} __PACKED__;

struct CmdListDateResp {
	int n_date;
	struct dcs_date dates[0];
};

struct CmdListEventReq {
	struct dcs_date from, to;
	uint32	events;
};

struct GetSearchRecordReq { //搜索数据
	int chn;//chn
	struct dcs_datetime from,to;		//查询的开始时间和结束时间
};

typedef struct dcs_videotimemap1 {
	int chn;//chn
	struct dcs_datetime from;//播放的开始时间
	struct dcs_datetime  to;//播放的结束时间
} VIDEOTIMEMAP_NEW;

struct CmdListSearchRecord
{
	uint32 n_item;//uint16 xxxx n_item;sizijiexg
	//struct dcs_datetime items[0];//20130530modify
	VIDEOTIMEMAP_NEW items[0];
};

struct CmdListDateTime_NEW
{
	//回放视频
	int chn;//设备的通道 ,ipc default 0
	struct dcs_datetime datetimes;//具体时间
};

#define DAP2P_ALERT_MOTION	1
#define DAP2P_ALERT_IO		2
#define DAP2P_ALERT_SOUND		3
struct dcs_event {
	int event;
	struct dcs_datetime dtt;
};
struct CmdListEventResp {
	uint32 n_item;//uint16 xxxx  n_item;  sizijiexg
	struct dcs_event items[0];
};

struct dcs_video_color {
	uint8 brightness, contrast, saturation, hue;
} __PACKED__;

struct CmdSetVideoColorReq {
	struct dcs_chnvideo chnvid;
	struct dcs_video_color color;
};

struct dcs_video_param {
	uint8	brmode:1;
	uint8	norm:1;	//0-PAL; 1-NTSC. unmodifiable, used by client to show fps range
	uint8	quality:3;
	uint8	res:3;
	uint8	fps;
	uint16	kbps;
	uint16	gop;
	uint16	save:1;
	uint16	max_fps:5;
	uint16	reserved:10;
	uint16	supported_res;//xxxx sizijiexiug
} __PACKED__;


struct dcs_video_param_new {
	unsigned char res;  ///< 图像大小。 
	unsigned char quality; ///< 图像质量，取值1-6，值越大，图像质量越好,只有在可变码流下才起作用。
	unsigned char fps; ///< 每秒钟的帧数。一般PAL制1-25帧/秒，NTSC制1-30帧/秒。 
	unsigned char brmode; //0 主码流  1辅码流  2第三码流  20130709
	uint16 save;//是否为固定码流 0 否 1 yes         20130709
	uint16 kbps;////output kbps	value:16~4000     20130709
	uint16	gop;//Gops of I-Frame	value:Recommend 50~300
	uint16	max_fps;//max_fps:5;//the max Frames per second
	char reserved[8];  ///< 保留字节
};

struct CmdListVideoPara {
	int n_num;
	struct dcs_video_param_new dcs[0];
};

struct CmdSetVideoParameterReq {
	struct dcs_chnvideo  chnvid;
	struct dcs_video_param video_param;
};

/* Power freq & Video Norm */
#define POWERFREQ_50HZ  0
#define POWERFREQ_60HZ  1
#define VIDEONORM_PAL  0
#define VIDEONORM_NTSC 1

struct CmdSetPowerFreqReq {
	int vchn;
	int freq;	//POWERREQ_xxx
};

typedef struct dcs_videotimemap {
	struct dcs_date date;
	uint32 h_mask;			//mask for hours. If bit N is set, hour N has record
	uint32 m_mask[24][2];	//mask for minutes of each hour.
} VIDEOTIMEMAP;

#define TEST_MIN(v,h,m) (v.m_mask[h][m/32] & (1<<(m%32)))	//Test to see if hour of h, minute of m has record

typedef enum {
	WIFI_ENCTYPE_INVALID,
	WIFI_ENCTYPE_NONE,
	WIFI_ENCTYPE_WEP,
	WIFI_ENCTYPE_WPA_TKIP,
	WIFI_ENCTYPE_WPA_AES,
	WIFI_ENCTYPE_WPA2_TKIP,
	WIFI_ENCTYPE_WPA2_AES
} WIFI_ENCTYPE;

struct _tagWifiSetting {
	char essid[32];
	char key[32];
	WIFI_ENCTYPE enctype;
};
#define CmdGetWifiResp _tagWifiSetting
#define CmdSetWifiReq _tagWifiSetting

struct WifiAP {
	char essid[32];
	WIFI_ENCTYPE enctype;
	uint32 quality;//unsigned short quality;//xxxx sizijie
};
//CMD_LIST_WIFI_AP
struct CmdListWifiApResp {
	int nAP;	// < 0: No wifi nic
	struct WifiAP aps[0];
};

//--------------------------------------
#define VIDEOFLIP_HORZ	0x01
#define VIDEOFLIP_VERT	0x02

struct CmdSetFlipReq {
	int vchn;	//0
	int flips;	//combination of VIDEOFLIP_xxx
};
//----------------------------------------------
//Events:

#define	DAP2PET_PB_CLOCK		0x10000
//data: struct dcs_datetime dt;

/************************************************************************/
/* 20130725
*/
/************************************************************************/
//Set Mail Information
typedef enum
{
	NONE,
	_SSL,
	_TLS,
}MailEncryptionTypeKind;//encrypt class

struct  DAMailInfo
{
	char RvrAdr[48];//receiver address
	char SvrAdr[48];//server address
	char Uname[48];//user name
	char Upwd[48];//user pwd
	char SendAdr[48];//send address
	int port;//server port
	int Authenticate;//0:no  1:need
	MailEncryptionTypeKind EncryptionType;
	char reserved[48];
};

typedef enum
{
	leftup,
	rightup,
	leftdown,
	rightdown
}Pos;//position

struct  DAOSDInfo
{
	uint16 NeedDate;//0:need show date   1:no
	uint16 NeedDevName;//0:need show device's name  1:no
	Pos DataPos;//the position of date
	Pos DevNamePos;//the position of device's name
	char OtherInfo[48];//other information
	Pos OtherPos;//the position of other information
};

struct DANetInfo
{
	uint16 IpType;//0:Fixed IP  1:dynamic ip
	uint16 DnsType;//0:no  1:yes
	char cIp[16];//local ip
	char cNetMask[16];//net mask
	char cGateway[16];
	char cMDns[16];//main dns
	char cSDns[16];//second dns
	uint16 chttpPort;//http port
};

struct DAInitSet
{
	int Type;//0:reboot 1:Restore factory settings
};

typedef enum
{
	NONETIME,
	SaveCurrent,
	ManuallySet,
	SynchronizationComputer,//Computer time synchronization
	SynchronizationNTP,//Computer time NTP
}TimeMode;

typedef enum 
{
	TimezoneKeepcurrentsettingSpan,
	TimezoneInternationalDateLineWestSpan,
	TimezoneMidwayIslandSamoaSpan,
	TimezoneHawaiiSpan,
	TimezoneAlaskaSpan,
	TimezonePacificTimeUSCanadaTijuanaSpan,
	TimezoneMountainTimeUSCanadaSpan,
	TimezoneChihuahuaLaPazMazatlanSpan,
	TimezoneArizonaSpan,
	TimezoneSaskatchewanSpan,
	TimezoneGuaddlajaraMexicoCityMonterreySpan,
	TimezoneCentralTimeUSCanadaSpan,
	TimezoneCentralAmericaSpan,
	TimezoneIndianaEastSpan,
	TimezoneEasternTimeUSCanadaSpan,
	TimezoneBogotaLimaQuitoSpan,
	TimezoneSantiagoSpan,
	TimezoneCaracasLaPazSpan,
	TimezoneAtlanticTimeCanadaSpan,
	TimezoneNenfoundLandSpan,
	TimezoneGreenlandSpan,
	TimezoneBuenosAiresGeorgentownSpan,
	TimezoneBrasiliaSpan,
	TimezoneMidAtlanticSpan,
	TimezoneCapeVerdeIsSpan,
	TimezoneAzoresSpan,
	TimezoneGreenwichMeanTimeDublinEdinburghLisbonLondonSpan,
	TimezoneCasablancamonroviaSpan,
	TimezoneAmsterdamBerlinBernRomeStockholmViennaSpan,
	TimezoneBelgradeBratislavaBudapestLjubljanaPragueSpan,
	TimezoneBrusselsCopenhagenMadridParisSpan,
	TimezoneSarajevoSkopjeWarsawZagrebSpan,
	TimezoneWestCentralAfricaSpan,
	TimezonehensIstanbulMinskSpan,
	TimezoneBucharestSpan,
	TimezoneCairoSpan,
	TimezonerarePretoriaSpan,
	TimezoneHelsinkiKyivRigaSofiaTallinnVilniusSpan,
	TimezoneJerusalemSpan,
	TimezoneBaghdadSpan,
	TimezoneKuwaitRiyadhSpan,
	TimezoneMoscowStPetersburgVolgogradSpan,
	TimezoneNairobiSpan,
	TimezoneTehranSpan,
	TimezoneAbuDhabiMuscatSpan,
	TimezoneBakuTbilisiYerevanSpan,
	TimezoneKabulSpan,
	TimezoneEkaterinburgSpan,
	TimezoneIslamabadKarachiTashkentSpan,
	TimezoneChennaiKolkataMumbaiNewDelhiSpan,
	TimezoneKathmanduSpan,
	TimezoneAlmatyNovosibirskSpan,
	TimezoneAstanaDhakaSpan,
	TimezoneSriJayawardenepuraSpan,
	TimezoneRangoonSpan,
	TimezoneBangkokHanoiJakartaSpan,
	TimezoneKrasnoyarskSpan,
	TimezoneBeijingChongqingHongKongUrumqiSpan,
	TimezoneIrkutskUlaanBataarSpan,
	TimezoneKualaLumpurSingaporeSpan,
	TimezonePerthSpan,
	TimezoneTaipeiSpan,
	TimezoneOsakaSapporoTokyoSpan,
	TimezoneSeoulSpan,
	TimezoneYakutskSpan,
	TimezoneAdelaideSpan,
	TimezoneBrisbaneSpan,
	TimezoneCanberraMelbourneSydneySpan,
	TimezoneGuamPortMoresbySpan,
	TimezoneHobartSpan,
	TimezoneVladivostokSpan,
	TimezoneMagadanSolomonIsNewSpan,
	TimezoneAucklandWellingtonSpan,
	TimezoneFijiKamchatkaMarshallIsSpan,
	TimezoneNukualofaSpan,
}TimeArea;

struct DATimeInfo
{
	struct dcs_datetime nowTime;
	TimeMode timeMode;
	TimeArea timeArea;//Time zone
	struct dcs_datetime SetTime;//when timemode is SaveCurrent or ManuallySet or SynchronizationComputer
	char server[48];//ntp's server,when timemode is SynchronizationNTP
	int Interval;//ntp's Interval   :hour
	char gmtInfo[48];//gmt信息，例如:GMT-09:00
};

typedef enum
{
	logall,
	logAlarm,
	logOperating,
}LogType;

struct  DASearchLOGInf
{	
	struct dcs_datetime from;
	struct dcs_datetime to;
	int chn;
	LogType type;
};

struct LogInf
{
	struct dcs_datetime  times;//Time of occurrence
	LogType type;
	int chn;//ipc:0
	char description[120];//A brief description of
};

struct DAGetLogInfList
{
	int num;
	struct LogInf loginf[0];
};

typedef enum
{
	NONESensitivity,
	lowerest,
	lower,
	General,
	High,
	Highest,
	Auto,
}Sensitivity;

typedef enum
{
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday,
}WeekInf;

struct DADetectionTime
{
	//24-hour mode
	uint8 fromhour;
	uint8 frommin;
	uint8 tohour;
	uint8 tomin;
};
struct DATimePlan//Time to plan
{
	WeekInf weekInf;//the day
	int num;
	struct DADetectionTime  decTime[5];//the max of time area is 5
};
struct DADayTimePlay
{
	int num;
	struct DATimePlan plan[];
};
struct DAAlarmArea
{
	Sensitivity sen;//0~6
	uint8 startx;
	uint8 starty;
	uint8 Height;
	uint8 Width;
};
struct DAAlarmListArea
{
	uint16 ResolutionType;//0:720p  1:vga  2:qvga
	uint16 num;//the max is 5
	struct DAAlarmArea alarmArea[5];//the max of alarm area is 5
};

struct DAMotionDetection
{
	uint8 OpenIt;//0:close  1:open
	uint8 chn;//ipc=0
	uint8 emailNotification;//0:no  1:yes   Mail notification
	uint8 triggerOutput;//0:no  1:yes   Trigger alarm output
	uint16 ResolutionType;//0:720p  1:vga  2:qvga
	uint16 areanum;//the max is 5
	struct DAAlarmArea alarmArea[5];//the max of alarm area is 5
	int timenum;
	struct DATimePlan plan[];
};
struct DAI2ODetection
{
	uint8 OpenInputDetection;//0:close  1:open
	uint8 Inputchn;//ipc=0
	uint8 InputElectricityLevel;//0:lower  1:high

	uint8 OpenOutputDetection;//0:no  1:yes   Trigger alarm output
	uint8 OutPutType;//0:always close   1:always open
	uint8 OutPutTime;//s


	uint8 LinkageEmailNotification;//0:no  1:yes   Mail notification
	uint8 LinkageNeedPic;//0：no  1:yes   Whether with pictures
	uint8 LinkagetriggerOutput;//0:no  1:yes   Trigger alarm output
	uint8 LinkagePTZchn;
	uint8 LinkagePTZPreset;//0:no 1:yes
	uint8 LinkagePTZPos;//0~15	
	int num;
	struct DATimePlan plan[];
};

struct CoverDetection
{
	uint16 OpenCoverDetection;//0:close  1:open
	uint16 chn;//ipc=0
	Sensitivity sen;//0~6
};

struct TimingCaptureDetection
{
	uint8 OpenTimingCapture;//0:close  1:open
	uint8 chn;//ipc=0
	uint8 Interval;//min
	uint8 emailNotification;//0:no  1:yes   Mail notification
	char  reserved[48];
};

struct TimingRecordDetection
{
	uint16 OpenTimingRecord;//0:close  1:open
	uint16 chn;//ipc=0
	int PackingTime;//:min   Packing time
	char  reserved[48];//
};

struct DASoundDetection
{
	uint8 OpenIt;//0:close  1:open
	uint8 chn;//ipc=0
	uint8 emailNotification;//0:no  1:yes   Mail notification
	uint8 triggerOutput;//0:no  1:yes   Trigger alarm output
	Sensitivity sen;//0~6
	char  reserved[48];
	int num;
	struct DATimePlan plan[];
};

struct DADeviceState
{
	uint8 onLineNum;//当前在线人数
	uint8 IsPtzContol;//是否包含云台控制
	uint16 reserved; //保留字段
};
struct   AP_Wifi
{
	char essid[32];
	char key[32];
	WIFI_ENCTYPE enctype;//wifi 类型
	uint8  keyindex;//默认0 用户匹配老的路由器加密类型  wep
     	uint8  isDHCP;//0:no 1:yes
	char cIp[16];//如果是静态IP,即非DHCP，需要输入网络的相关信息
	char cNetMask[16];
	char cGateway[16];
	char cMDns[16];
	char cSDns[16];
};

struct CloudAlarm
{
	uint8 IsOpenMotionDetection;//移动侦测 0：关闭  1：开启
	uint8 MontionDetectionLevel;//只有开启移动侦测时生效 0：低   1：一般  2：灵敏

	uint8 IsOpenSoundDetection;//声音侦测 0：关闭  1：开启
	uint8 SoundDetectionLevel;//只有开启声音侦测时生效 0：低   1：一般  2：灵敏

	uint8 IsOpenI2ODetection;//I2O告警 0：关闭  1：开启
	uint8 I2ODetectionLevel;//只有开启I2O侦测时生效 0：低   1：一般  2：灵敏

	uint8 IsOpenOther;//其他类型的告警  0：关闭  1：开启
	uint8 OtherDetectionLevel;//只有开启其他侦测时生效 0：低   1：一般  2：灵敏
};

struct NetworkInfo
{
		char cIp[16];
		char cNetMask[16];
		char cMDns[16];
		char cSDns[16];
		char cGateway[16];
		BOOL bDhcp;
};

typedef enum
{
	Default_AudioType=100,
	Default_bOpenTalk
}DefaultType;
#endif
