//*****************************************
//   文件名     : common.h
//   文件描述: 公用函数及宏定义
//	修改记录: dongliqiang created at 20120207
//                           modify at 20130527 by ptb
//*****************************************

#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef TD_TRUE
#define TD_TRUE  1
#endif

#ifndef TD_FALSE
#define TD_FALSE 0
#endif

#define TD_DEBUG

/*************基本数据类型宏定义**********/
#ifndef bool
//#define bool int
#endif



/*************返回值类型宏定义****************/
#ifndef TD_OK
#define TD_OK    0
#endif

#ifndef TD_ERROR
#define TD_ERROR -1
#endif

#ifndef TD_AGAIN
#define TD_AGAIN -2
#endif

#ifdef TD_DEBUG
#define SYS_INFO_PRINTF 0x12345678
#endif

/*************函数宏定义**************************/
#define Max(x, y)	((x) >= (y) ? (x) : (y))
#define Min(x, y)	((x) >= (y) ? (y) : (x))
#define pointer_valid_check(n) ((n) == NULL)

/*************release与debug下宏定义**************************/	
#ifndef TD_DEBUG
#define td_printf(a, b, ...)
#define DBG_HERE
#define func_info(fmt) \
		if (fpProc != NULL){\
			prc_WriteFuncInfo(__func__, fmt);\
		}

#else
#define func_info(fmt) prc_WriteFuncInfo(__func__, fmt)

int td_printf(unsigned int handle, char* pszfmt, ...);
#define DBG_HERE td_printf(0, "%s %s %d Run here!\n", __FILE__, __FUNCTION__, __LINE__)
#endif

/*************设备属性宏定义**************************/	
#define QCIF		0
#define HCIF		1   	//352*144	
#define FCIF		2		//352*288
#define HD1         3
#define FD1			4		//704*576
#define MD1			5
#define QVGA		6
#define FVGA		7		//640*480
#define HVGA		8
#define F720P		9		//1280*720
#define F960P		10		//1280*960
#define F1200       11      //1600*1200
#define F1080P		12		//1920*1080
#define QXGA		13      //2048*1536
#define H960        0x0200  //960*576
#define MAXVIDEONO 	QXGA	//如果添加更大的值，需要修改这里

#define MAX_CHANNEL_NUM		32	//最大通道数
#define MAX_STREAM			2	//码流个数
#define MAX_VIDEOSIZE_LIST	32  //最多支持的分辨率列表

#define STRLEN_128	128
#define STRLEN_256	256
#define STRLEN_512	512

#define NAME_LEN	32
#define TOKEN_LEN	32
#define IPADDR_LEN	32

#define PROT_ILLEGAL	-2
#define PROT_USED 		-3

#define CRLF "\r\n" //changguoxing

//ptz by qiaohaijun 
//水平坐标，范围: 0~36000
//垂直坐标，范围: -1500~9000
//变倍大小，范围: 10-500
#define PTZ_ABS_X_MIN 0
#define PTZ_ABS_X_MAX 36000

#define PTZ_ABS_Y_MIN -1500
#define PTZ_ABS_Y_MAX 9000

#define PTZ_ABS_Z_MIN 10
#define PTZ_ABS_Z_MAX 500
//end

/*************结构体定义**************************/	
typedef struct
{
	unsigned short m_u16VideoWidth[2];  //0->PAL 1->NTSC
	unsigned short m_u16VideoHeight[2];
}VIDEOSIZE_INFO;

typedef struct
{
	int  m_iProductModel;
	char m_chProductName[128];
}PRODUCTINFO;

typedef enum enAudioEncodeType
{
	G711A 			= 0x01,
	G711U 			= 0x02,
	ADPCM_DIV4 		= 0x03,
	G726_24 		= 0x05,
	G726_32 		= 0x06,
	G726_40 		= 0x07,
	ADPCM_IMA 		= 0x23,
	MEDIA_G726_16 	= 0x24,
	MEDIA_G726_24 	= 0x25,
	MEDIA_G726_32 	= 0x26,
	MEDIA_G726_40 	= 0x27,
	AMR 			= 0x15,
	AAC4 			= 0x16
}ENAudioEncType;

typedef struct
{
	float Min;
	float Max;
}TFloatRange;

typedef enum enDeviceType
{
	DEVICE_IPC = 0,
	DEVICE_DVR
}DeviceType;

/*****************函数声明**************************/	

char *FindNumPos(char *_cSource);


int SplitStringToArray(char *_cSource, char *_cSplitChar, char**_cDest);

char *base64_encode(unsigned char *_pSrc, int _iLen, char *_pBuf, int _iBufLen);

float ExchangeValue(float _pfSrc, TFloatRange _tSrcRange, TFloatRange _tDstRange);

int GetVideoWidthHeight(int _iPicSize, int _iNorm, unsigned short* _u16Width, unsigned short *_u16Height);

int get_uptime(int* _msec);

int RemoveFile(char *_pcFilePath);

int Strcpy(char *_pcDest, const char *_pcSrc, int _iMaxLen);

int CheckPort(int _iPort);

int CheckTimeZoneFile();

#endif

