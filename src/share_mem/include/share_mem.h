/****************************************************************************
[File information]
FileName = hw_app.h
Description = common funtion headfile
Date = 2010-11	
****************************************************************************/

#ifndef _SHARE_MEM_H_
#define _SHARE_MEM_H_

#include "guiparameter.h"

#define alignedchar __attribute__((aligned (4)))char 

int snd_rcv(int _id, char *_rspbuf, char *_reqbuf, int _rspsize, int _reqsize, int _cmd);

int snd_rcv_variable(int _id, char *_rspbuf, char *_reqbuf, int _rspmaxsize, int _reqsize, int _cmd);

int gui_transtream_init(int _iChn, void *_ptRsp);

int gui_transtream_uninit(int _iChn);

int gui_trans_init();

int gui_datatrans_init();

int snd_rcv_dvr(int _id, int *_pVideoList, char *_reqbuf, int _rspsize, int _reqsize, int _cmd);

char *get_chn_shmaddr(int _iChno);

#endif
