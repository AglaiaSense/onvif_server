/*
 *	file: proparser.h
 *	description: profile parse
 *	history: dongliqiang created at 20120224
 */

#ifndef __PROPARSER_H__
#define __PROPARSER_H__

#include "dictionary.h"

#define MAX_POINT_LIST (32)		//最大节点数




//配置文件操作ID
typedef enum{

	CFG_ADD		= 0,		//添加

	CFG_RM,					//删除
	
	CFG_REPLACE,			//替换
	
}ProOption;


//链表,用来记录配置文件修改内容
typedef struct{

	int m_iCount;		//记录节点数目
	
	char *m_pcSec;		//section

	int m_iSecNum;		//section number

	char *m_pcKey[MAX_POINT_LIST];	//keys

	int m_iKeyNum[MAX_POINT_LIST];	//keys number

	char *m_pcVal[MAX_POINT_LIST];	//values

	int m_iValNum[MAX_POINT_LIST];	//values number
	
	//struct PointList *m_ptNext;		//next point list

}PointList;

/**************************************************
*功能: 	 	设备第一次启动时,根据输入路数和模板生成相应的配置文件config_onvif.ini
*参数: 	 	_pThis:
*返回值:	0 - 成功, !0 - 失败
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
int  BuildProfile(void *_pThis);

/**************************************************
*功能: 	 	加载配置文件
*参数: 	 	_cProfile:		配置文件
*返回值:	dictionary
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
dictionary *LoadProfile(char *_cProfile);

/**************************************************
*功能: 	 	存储配置文件
*参数: 	 	_cProfile:		配置文件
*返回值:	无
*修改历史:
	20120226 v1.0 creat by pangtb
**************************************************/
int SaveProfile(dictionary *_ptDict, char *_pcFilePath);

/**************************************************
*功能: 	 	释放配置文件
*参数: 	 	_cProfile:		配置文件
*返回值:	无
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
void ReleaseProfile(dictionary *_ptDict);

/**************************************************
*功能: 	 	获取配置文件相应信息
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 追加通道号到section, _iNum <= 0 不追加
		 	_cKey:			key
		 	_iNum: 			_iNum >0 追加通道号到key, _iNum <= 0 不追加
		 	_Default:		默认返回值
*返回值:	0: 成功, !0: 失败
*修改历史:
 20120224 v1.0 creat by pangtb
**************************************************/
char *GetProStr(dictionary *_ptDict, char *_cSec, int _iSecNum, char *_cKey, int _iKeyNum, char *_Default);

/**************************************************
*功能: 	 	获取配置文件相应int信息
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 追加通道号到section, _iNum <= 0 不追加
		 	_cKey:			key
		 	_iNum: 			_iNum >0 追加通道号到key, _iNum <= 0 不追加
		 	_iDefault:		默认返回值
*返回值:	要获取的整形数
*修改历史:
 20120224 v1.0 creat by pangtb
**************************************************/
int GetProInt(dictionary *_ptDict, char *_cSec, int _iSecNum, char *_cKey, int _iKeyNum, int _iDefault);

/**************************************************
*功能: 	 	获取配置文件相应double信息
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 追加通道号到section, _iNum <= 0 不追加
		 	_cKey:			key
		 	_iNum: 			_iNum >0 追加通道号到key, _iNum <= 0 不追加
		 	_dDefault:		默认返回值
*返回值:	要获取的整形数
*修改历史:
 20120224 v1.0 creat by pangtb
**************************************************/
double GetProDouble(dictionary *_ptDict, char *_cSec, int _iSecNum, char *_cKey, int _iKeyNum, double _dDefault);

/**************************************************
*功能: 	 	获取配置文件相应bool信息
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 追加通道号到section, _iNum <= 0 不追加
		 	_cKey:			key
		 	_iNum: 			_iNum >0 追加通道号到key, _iNum <= 0 不追加
		 	_iDefault:		默认返回值
*返回值:	要获取的整形数
*修改历史:
 20120224 v1.0 creat by pangtb
**************************************************/
int GetProBool(dictionary *_ptDict, char *_cSec, int _iSecNum, char *_cKey, int _iKeyNum, int _iDefault);

/**************************************************
*功能: 	 	检查section是否存在
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
*返回值:	存在(1), 不存在(0)
*修改历史:
	20120224 v1.0 creat by sy
**************************************************/

int FindProEntry(dictionary *_ptDict, char *_pcSec, int _iSecNum);

/**************************************************
*功能: 	 	设置配置文件信息.
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 追加通道号到section, _iNum <= 0 不追加
		 	_cKey:			key
		 	_iNum: 			_iNum >0 追加通道号到key, _iNum <= 0 不追加
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 追加通道号到val, _iValNum <= 0 不追加
			_iID:			配置文件操作ID: CFG_ADD - 添加, CFG_RM - 删除, CFG_REPLACE - 替换
*返回值:	0 - 成功, -1 - 失败, -2 - 存在, -3 - 不存在
*修改历史:
	20120227 v1.0 creat by pangtb
**************************************************/
int SetProStr(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, char *_pcVal, int _iValNum, int _iID);

/**************************************************
*功能: 	 	设置配置文件信息.
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 追加通道号到section, _iNum <= 0 不追加
		 	_cKey:			key
		 	_iNum: 			_iNum >0 追加通道号到key, _iNum <= 0 不追加
		 	_fVal:			val
			_iID:			配置文件操作ID: CFG_ADD - 添加, CFG_RM - 删除, CFG_REPLACE - 替换
*返回值:	0 - 成功, -1 - 失败, -2 - 存在, -3 - 不存在
*修改历史:
	20120227 v1.0 creat by pangtb
**************************************************/
int SetProFloat(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, float _fVal, int _iID);

/**************************************************
*功能: 	 	通过Point List设置配置文件信息.
			注意: 当删除section时,section对应的内容也会被完全删除
*参数: 	 	_ptDict:		配置文件dictionary
		 	_ptList:		信息链表
		 	_iID:			配置文件操作ID: CFG_ADD - 添加, CFG_RM - 删除, CFG_REPLACE - 替换
*返回值:	0 - 成功, -1 - 失败, -2 - 存在, -3 - 不存在
*修改历史:
	20120224 v1.0 creat by pangtb
**************************************************/
int SetProStrList(dictionary *_ptDict, PointList *_ptList, int _iID);

/**************************************************
*功能: 	 	查找对应section空闲号码
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 追加通道号到section, _iNum <= 0 不追加
		 	_cKey:			key
		 	_Default:		默认返回值
*返回值:	iRet: <=0 - 失败, >0 - 空闲号码,从1开始
*修改历史:
	20120308 v1.0 creat by pangtb
**************************************************/

int GetIdleNum(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iDefault);

/**************************************************
*功能: 	 	获取val对应key号码
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 追加通道号到section, _iNum <= 0 不追加
		 	_cKey:			key
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 追加通道号到val, _iValNum <= 0 不追加
*返回值:	iRet: <=0 - 失败, >0 - 空闲号码,从1开始
*修改历史:
	20120308 v1.0 creat by pangtb
**************************************************/
int GetKeyNum(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, char *_pcVal, int _iValNum, int _iDefault);

/**************************************************
*功能: 	 	获取val对应section号码
*参数: 	 	_ptDict:		配置文件dictionary
		 	_cSec: 			section
		 	_cKey:			key
		 	_iKeyNum:		key number
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 追加通道号到val, _iValNum <= 0 不追加
*返回值:	iRet: <=0 - 失败, >0 - 空闲号码,从1开始
*修改历史:
	20120308 v1.0 creat by pangtb
**************************************************/
int GetSecNum(dictionary *_ptDict, char *_pcSec, char *_pcKey, int _iKeyNum, char *_pcVal, int _iValNum, int _iDefault);
/****************************************************
 *功能		:	找出字符串(如:PT10S)中的时间并以返回时间(单位:S)
 *参数		:	_pcstrPT	:	要转换的字符串

 *返回值		: 	正常:(时间秒为单位),	错误(-1)
 *修改历史	:	20120314 v1.0 created by ptb
****************************************************/
int PTStrToTime(const char *_pcStrPT);
/****************************************************
 *功能		:	根据Topic信息填充需要发送的消息内容
 *参数		:	_pcTopicName:	Topic名称
 				_pcTopicVal	:	Topic值
 				_pcTopicType:	Topic类型
 				_iTopicNum	:	当前支持的Topic数

 *返回值		: 	正常:返回消息内容字符串,	错误:NULL
 *修改历史	:	20120312 v1.0 created by ptb
****************************************************/
char* SendMsg(const char *_pcTopicName, const char *_pcTopicVal, const char *_pcTopicType, int _iTopicNum );

#endif

