/*
 *	file: debug.c
 *	description:debug for test
 *	history: ptb created at 20130419
 */
 
#include "debug.h"
#include "common.h"
#include "rtsp_data.h"

 /**************************************************
 *����           :      ��ӡ֡��Ϣ
 *����           : 	_ptFrameInfo      :       ֡��Ϣ�ṹ��
 *����ֵ      :	0: �ɹ�, !0: ʧ��
 *�޸���ʷ :	201304228 v2.3.0 modify by ptb
 **************************************************/
 void print_frameinfo(void *_ptFrameInfo)
 {
	 stFrameInfoPlate *ptFrameInfo = (stFrameInfoPlate *)_ptFrameInfo;
	 
	 if(pointer_valid_check(ptFrameInfo))
	 {
		 return;
	 }
	 
	 td_printf(0, "*****************");
	 td_printf(0, "frame type = %d", ptFrameInfo->cFrameType);
	 td_printf(0, "frame rate = %d", ptFrameInfo->cFrameRate);
	 td_printf(0, "reserve1 = %d", ptFrameInfo->cReserve1);
	 td_printf(0, "reserve2 = %d", ptFrameInfo->cReserve2);
	 td_printf(0, "DataLen = %d", ptFrameInfo->iDataLength);
	 td_printf(0, "*****************");
 }

