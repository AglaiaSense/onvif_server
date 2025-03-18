#ifndef _CHARSET_H
#define _CHARSET_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UNI_INDEX	6860//6837
#define MAX_OSD_X	22.0
#define MAX_OSD_Y	18.0

void UTF8ToGB2312(const unsigned char *_Utf8, unsigned char *_Gb2312, int len);

void GB2312ToUTF8(const unsigned char *_Gb2312, unsigned char *_Utf8, int len);

#endif

