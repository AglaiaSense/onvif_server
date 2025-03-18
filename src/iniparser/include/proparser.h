/*
 *	file: proparser.h
 *	description: profile parse
 *	history: dongliqiang created at 20120224
 */

#ifndef __PROPARSER_H__
#define __PROPARSER_H__

#include "dictionary.h"

#define MAX_POINT_LIST (32)		//���ڵ���




//�����ļ�����ID
typedef enum{

	CFG_ADD		= 0,		//���

	CFG_RM,					//ɾ��
	
	CFG_REPLACE,			//�滻
	
}ProOption;


//����,������¼�����ļ��޸�����
typedef struct{

	int m_iCount;		//��¼�ڵ���Ŀ
	
	char *m_pcSec;		//section

	int m_iSecNum;		//section number

	char *m_pcKey[MAX_POINT_LIST];	//keys

	int m_iKeyNum[MAX_POINT_LIST];	//keys number

	char *m_pcVal[MAX_POINT_LIST];	//values

	int m_iValNum[MAX_POINT_LIST];	//values number
	
	//struct PointList *m_ptNext;		//next point list

}PointList;

/**************************************************
*����: 	 	�豸��һ������ʱ,��������·����ģ��������Ӧ�������ļ�config_onvif.ini
*����: 	 	_pThis:
*����ֵ:	0 - �ɹ�, !0 - ʧ��
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
int  BuildProfile(void *_pThis);

/**************************************************
*����: 	 	���������ļ�
*����: 	 	_cProfile:		�����ļ�
*����ֵ:	dictionary
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
dictionary *LoadProfile(char *_cProfile);

/**************************************************
*����: 	 	�洢�����ļ�
*����: 	 	_cProfile:		�����ļ�
*����ֵ:	��
*�޸���ʷ:
	20120226 v1.0 creat by pangtb
**************************************************/
int SaveProfile(dictionary *_ptDict, char *_pcFilePath);

/**************************************************
*����: 	 	�ͷ������ļ�
*����: 	 	_cProfile:		�����ļ�
*����ֵ:	��
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
void ReleaseProfile(dictionary *_ptDict);

/**************************************************
*����: 	 	��ȡ�����ļ���Ӧ��Ϣ
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_Default:		Ĭ�Ϸ���ֵ
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
 20120224 v1.0 creat by pangtb
**************************************************/
char *GetProStr(dictionary *_ptDict, char *_cSec, int _iSecNum, char *_cKey, int _iKeyNum, char *_Default);

/**************************************************
*����: 	 	��ȡ�����ļ���Ӧint��Ϣ
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_iDefault:		Ĭ�Ϸ���ֵ
*����ֵ:	Ҫ��ȡ��������
*�޸���ʷ:
 20120224 v1.0 creat by pangtb
**************************************************/
int GetProInt(dictionary *_ptDict, char *_cSec, int _iSecNum, char *_cKey, int _iKeyNum, int _iDefault);

/**************************************************
*����: 	 	��ȡ�����ļ���Ӧdouble��Ϣ
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_dDefault:		Ĭ�Ϸ���ֵ
*����ֵ:	Ҫ��ȡ��������
*�޸���ʷ:
 20120224 v1.0 creat by pangtb
**************************************************/
double GetProDouble(dictionary *_ptDict, char *_cSec, int _iSecNum, char *_cKey, int _iKeyNum, double _dDefault);

/**************************************************
*����: 	 	��ȡ�����ļ���Ӧbool��Ϣ
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_iDefault:		Ĭ�Ϸ���ֵ
*����ֵ:	Ҫ��ȡ��������
*�޸���ʷ:
 20120224 v1.0 creat by pangtb
**************************************************/
int GetProBool(dictionary *_ptDict, char *_cSec, int _iSecNum, char *_cKey, int _iKeyNum, int _iDefault);

/**************************************************
*����: 	 	���section�Ƿ����
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
*����ֵ:	����(1), ������(0)
*�޸���ʷ:
	20120224 v1.0 creat by sy
**************************************************/

int FindProEntry(dictionary *_ptDict, char *_pcSec, int _iSecNum);

/**************************************************
*����: 	 	���������ļ���Ϣ.
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 ׷��ͨ���ŵ�val, _iValNum <= 0 ��׷��
			_iID:			�����ļ�����ID: CFG_ADD - ���, CFG_RM - ɾ��, CFG_REPLACE - �滻
*����ֵ:	0 - �ɹ�, -1 - ʧ��, -2 - ����, -3 - ������
*�޸���ʷ:
	20120227 v1.0 creat by pangtb
**************************************************/
int SetProStr(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, char *_pcVal, int _iValNum, int _iID);

/**************************************************
*����: 	 	���������ļ���Ϣ.
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_iNum: 			_iNum >0 ׷��ͨ���ŵ�key, _iNum <= 0 ��׷��
		 	_fVal:			val
			_iID:			�����ļ�����ID: CFG_ADD - ���, CFG_RM - ɾ��, CFG_REPLACE - �滻
*����ֵ:	0 - �ɹ�, -1 - ʧ��, -2 - ����, -3 - ������
*�޸���ʷ:
	20120227 v1.0 creat by pangtb
**************************************************/
int SetProFloat(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iKeyNum, float _fVal, int _iID);

/**************************************************
*����: 	 	ͨ��Point List���������ļ���Ϣ.
			ע��: ��ɾ��sectionʱ,section��Ӧ������Ҳ�ᱻ��ȫɾ��
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_ptList:		��Ϣ����
		 	_iID:			�����ļ�����ID: CFG_ADD - ���, CFG_RM - ɾ��, CFG_REPLACE - �滻
*����ֵ:	0 - �ɹ�, -1 - ʧ��, -2 - ����, -3 - ������
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
int SetProStrList(dictionary *_ptDict, PointList *_ptList, int _iID);

/**************************************************
*����: 	 	���Ҷ�Ӧsection���к���
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_Default:		Ĭ�Ϸ���ֵ
*����ֵ:	iRet: <=0 - ʧ��, >0 - ���к���,��1��ʼ
*�޸���ʷ:
	20120308 v1.0 creat by pangtb
**************************************************/

int GetIdleNum(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, int _iDefault);

/**************************************************
*����: 	 	��ȡval��Ӧkey����
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_iSecNum: 		_iNum >0 ׷��ͨ���ŵ�section, _iNum <= 0 ��׷��
		 	_cKey:			key
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 ׷��ͨ���ŵ�val, _iValNum <= 0 ��׷��
*����ֵ:	iRet: <=0 - ʧ��, >0 - ���к���,��1��ʼ
*�޸���ʷ:
	20120308 v1.0 creat by pangtb
**************************************************/
int GetKeyNum(dictionary *_ptDict, char *_pcSec, int _iSecNum, char *_pcKey, char *_pcVal, int _iValNum, int _iDefault);

/**************************************************
*����: 	 	��ȡval��Ӧsection����
*����: 	 	_ptDict:		�����ļ�dictionary
		 	_cSec: 			section
		 	_cKey:			key
		 	_iKeyNum:		key number
		 	_cVal:			val
		 	_iValNum: 		_iValNum >0 ׷��ͨ���ŵ�val, _iValNum <= 0 ��׷��
*����ֵ:	iRet: <=0 - ʧ��, >0 - ���к���,��1��ʼ
*�޸���ʷ:
	20120308 v1.0 creat by pangtb
**************************************************/
int GetSecNum(dictionary *_ptDict, char *_pcSec, char *_pcKey, int _iKeyNum, char *_pcVal, int _iValNum, int _iDefault);
/****************************************************
 *����		:	�ҳ��ַ���(��:PT10S)�е�ʱ�䲢�Է���ʱ��(��λ:S)
 *����		:	_pcstrPT	:	Ҫת�����ַ���

 *����ֵ		: 	����:(ʱ����Ϊ��λ),	����(-1)
 *�޸���ʷ	:	20120314 v1.0 created by ptb
****************************************************/
int PTStrToTime(const char *_pcStrPT);
/****************************************************
 *����		:	����Topic��Ϣ�����Ҫ���͵���Ϣ����
 *����		:	_pcTopicName:	Topic����
 				_pcTopicVal	:	Topicֵ
 				_pcTopicType:	Topic����
 				_iTopicNum	:	��ǰ֧�ֵ�Topic��

 *����ֵ		: 	����:������Ϣ�����ַ���,	����:NULL
 *�޸���ʷ	:	20120312 v1.0 created by ptb
****************************************************/
char* SendMsg(const char *_pcTopicName, const char *_pcTopicVal, const char *_pcTopicType, int _iTopicNum );

#endif

