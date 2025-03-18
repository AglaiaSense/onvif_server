
/*
 *	file: array.c
 *	descripetion: array option
 *	history: dongliqiang created at 20120302
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "common.h"

/**************************************************
*����:		����ԭ����,��ȡ������
*����:		_pSrcAddr:			ԭ�����ַ
*				_iSrcSize:			ԭ�����С			
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
void *ArrayAdd(void *_pSrcAddr, int _iSrcSize, void *_pDestAddr, int _idDestSize)
{
	void *pReturn = NULL;
	int iSize = 0;

	if(NULL == _pDestAddr || _idDestSize < 0)
	{
		return _pSrcAddr;
	}

	if(NULL == _pSrcAddr)
	{
		td_printf(0, "warnning:SrcAddr is NULL\n");
		iSize = _idDestSize;
	}
	else
	{
		iSize = _iSrcSize + _idDestSize;
	}
	
	pReturn = realloc(_pSrcAddr, iSize);
	if(NULL == pReturn)
	{
		td_printf(0, "warnning:realloc ERROR!\n");
		return NULL;
	}

	if(NULL == _pSrcAddr)
	{
		memcpy(pReturn, _pDestAddr, _idDestSize);
	}
	else
	{
		memcpy(pReturn + _iSrcSize, _pDestAddr, _idDestSize);
	}

	return pReturn;
}

/**************************************************
*����:		��ʼ��ȫ�ֱ�����Ӧ�Ľṹ��
*����:		_pThis:		ȫ�ֱ��� g_stOnvifCtrl
*����ֵ:	0: �ɹ�, !0: ʧ��
*�޸���ʷ:
	20120224 v1.0 creat by pangtb
**************************************************/
void *ArrayDel(void *_pSrcAddr, int _iSrcSize, void *_pDestAddr, int _idDestSize)
{
	void *pReturn = NULL;
	int iPos = 0;
	int iSize = 0;

	if(NULL == _pSrcAddr || NULL == _pDestAddr || _iSrcSize < 0 || _idDestSize < 0)
	{
		return _pSrcAddr;
	}

	iPos = (int)((char*)_pDestAddr - (char*)_pSrcAddr);
	if(iPos < 0)
	{
		return _pSrcAddr;
	}

	iSize = _iSrcSize - iPos - _idDestSize;
	if(iSize < 0)
	{
		return _pSrcAddr;
	}

	memmove(_pDestAddr, _pDestAddr + _idDestSize, iSize);
	pReturn = realloc(_pSrcAddr, _iSrcSize - _idDestSize);

	return pReturn;
	
}

