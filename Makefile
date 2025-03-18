#file: Makefile
#project: onvif_v2.3
#history: pangtb created at 20120206

#define cross compiler

#CROSS:=
#CC:=$(CROSS)gcc
CC:=g++
AR:=$(CROSS)ar
STRIP:=$(CROSS)strip

#modules enable
#DBG = 1
WSSE_EN = 1
#MD5 = 1


#compiler flags
CFLAGS = -O3 -Wall
CFLAGS += -Wno-unused-value
CFLAGS += -ffunction-sections -fdata-sections

ifeq ($(DBG),1)
CFLAGS += -g3 -DDEBUG_LV1 -DDEBUG_LV0 -DTD_DEBUG
OUTPUTFILE = onvif_debug.bin
else
OUTPUTFILE = onvif.bin
endif

#ld flags
LDFLAGS = -Wl,-gc-sections

#chose onvif unnecessary module
CFLAGS 	+= -DWITH_NOIDREF
CFLAGS 	+= -DWITH_ANALYTICS
CFLAGS 	+= -DWITH_EVENT
CFLAGS	+= -DWITH_PTZ
CFLAGS	+= -DWITH_IMAGE
CFLAGS	+= -DWITH_TELECOM
#CFLAGS	+= -DWITH_RECORDING
#CFLAGS	+= -DWITH_DISPLAY
#CFLAGS	+= -DWITH_TESTTOOL
#CFLAGS += -DWITH_FASTCGI
#CFLAGS += -DWITH_FUNC_BAK

ifeq ($(MD5),1)
CFLAGS += -DWITH_MD5
endif

ifeq ($(WSSE_EN),1)
#CFLAGS	+= -DWITH_OPENSSL
#CFLAGS	+= -DWITH_DOM
CFLAGS	+= -DWITH_WSSE
endif

ifeq ($(DANALE),1)
CFLAGS	+= -DWITH_DANALE -D__LINUX__
ifeq ($(DBG),1)
CFLAGS	+= -D_DEBUG -g
endif
endif



#dir path
ONVIF_ROOT			= .
BIN_PATH        = $(ONVIF_ROOT)/bin
ONVIF_PATH			= $(ONVIF_ROOT)/src/onvif
MAIN_PATH       = $(ONVIF_ROOT)/src/main
COMMON_PATH			= $(ONVIF_ROOT)/src/common
INTERFACE_PATH  = $(ONVIF_ROOT)/src/interface
GSOAP_PATH			= $(ONVIF_ROOT)/src/gsoap
TRANSPORT_PATH  = $(ONVIF_ROOT)/src/transport
RTSP_PATH       = $(ONVIF_ROOT)/src/rtsp
INIPARSER_PATH  = $(ONVIF_ROOT)/src/iniparser
SHARE_MEM_PATH  = $(ONVIF_ROOT)/src/share_mem
ARRAY_PATH			= $(ONVIF_ROOT)/src/array
LOCK_PATH       = $(ONVIF_ROOT)/src/lock
DATACENTER_PATH = $(ONVIF_ROOT)/src/datacenter
RINGBUF_PATH		= $(ONVIF_ROOT)/src/ringbuf
THREADS_PATH		= $(ONVIF_ROOT)/src/threads
DLIST_PATH			= $(ONVIF_ROOT)/src/dlist
MEDIA_PATH			= $(ONVIF_ROOT)/src/media
ifeq ($(MD5),1)
MD5_PATH			= $(ONVIF_ROOT)/src/md5
endif
OBJS_PATH			  = $(ONVIF_ROOT)/obj
LIB_PATH			  = $(ONVIF_ROOT)/lib

ifeq ($(WSSE_EN),1)
WSSE_PATH       = $(ONVIF_ROOT)/src/wsse
endif

ifeq ($(DANALE),1)
DANALE_PATH     = $(ONVIF_ROOT)/src/danale
endif

#src files
SRCS				= $(wildcard $(MAIN_PATH)/*.c)
SRCS				+= $(wildcard $(ONVIF_PATH)/*.c)
SRCS				+= $(wildcard $(COMMON_PATH)/*.c)
SRCS				+= $(wildcard $(INTERFACE_PATH)/*.c)
SRCS				+= $(wildcard $(GSOAP_PATH)/*.c)
SRCS				+= $(wildcard $(TRANSPORT_PATH)/*.c)
SRCS				+= $(wildcard $(RTSP_PATH)/*.c)
SRCS				+= $(wildcard $(INIPARSER_PATH)/*.c)
SRCS				+= $(wildcard $(SHARE_MEM_PATH)/*.c)
SRCS				+= $(wildcard $(ARRAY_PATH)/*.c)
SRCS				+= $(wildcard $(LOCK_PATH)/*.c)
SRCS				+= $(wildcard $(DATACENTER_PATH)/*.c)
SRCS				+= $(wildcard $(RINGBUF_PATH)/*.c)
SRCS				+= $(wildcard $(THREADS_PATH)/*.c)
SRCS				+= $(wildcard $(DLIST_PATH)/*.c)
SRCS				+= $(wildcard $(MEDIA_PATH)/*.c)
ifeq ($(MD5),1)
SRCS				+= $(wildcard $(MD5_PATH)/*.c)
endif
ifeq ($(WSSE_EN),1)
SRCS				+= $(wildcard $(WSSE_PATH)/*.c)
endif

ifeq ($(DANALE),1)
SRCS				+= $(wildcard $(DANALE_PATH)/*.c)
endif

OBJS				:=  $(SRCS:%.c=%.o)
OBJS				:=	$(OBJS:$(GSOAP_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(MAIN_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(ONVIF_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=	$(OBJS:$(COMMON_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=	$(OBJS:$(INTERFACE_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(TRANSPORT_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(RTSP_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(INIPARSER_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(SHARE_MEM_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(ARRAY_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(LOCK_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(DATACENTER_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(RINGBUF_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(THREADS_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(DLIST_PATH)/%=$(OBJS_PATH)/%)
OBJS				:=  $(OBJS:$(MEDIA_PATH)/%=$(OBJS_PATH)/%)
ifeq ($(MD5),1)
OBJS				:=  $(OBJS:$(MD5_PATH)/%=$(OBJS_PATH)/%)
endif
ifeq ($(WSSE_EN),1)
OBJS				:=  $(OBJS:$(WSSE_PATH)/%=$(OBJS_PATH)/%)
endif

ifeq ($(DANALE),1)
OBJS				:=  $(OBJS:$(DANALE_PATH)/%=$(OBJS_PATH)/%)
endif

#include path
LIB_INC				:= -I$(ONVIF_PATH)/include
LIB_INC				+= -I$(MAIN_PATH)/include
LIB_INC				+= -I$(COMMON_PATH)/include
LIB_INC				+= -I$(INTERFACE_PATH)/include
LIB_INC				+= -I$(GSOAP_PATH)/include
LIB_INC				+= -I$(TRANSPORT_PATH)/include
LIB_INC				+= -I$(RTSP_PATH)/include
LIB_INC				+= -I$(INIPARSER_PATH)/include
LIB_INC				+= -I$(SHARE_MEM_PATH)/include
LIB_INC				+= -I$(ARRAY_PATH)/include
LIB_INC				+= -I$(LOCK_PATH)/include
LIB_INC				+= -I$(DATACENTER_PATH)/include
LIB_INC				+= -I$(RINGBUF_PATH)/include
LIB_INC				+= -I$(THREADS_PATH)/include
LIB_INC				+= -I$(DLIST_PATH)/include
LIB_INC				+= -I$(MEDIA_PATH)/include
ifeq ($(MD5),1)
LIB_INC				+= -I$(MD5_PATH)/include
endif
ifeq ($(WSSE_EN),1)
LIB_INC				+= -I$(WSSE_PATH)/include
endif

ifeq ($(DANALE),1)
LIB_INC				+= -I$(DANALE_PATH)/include
LIB_INC				+= -I$(DANALE_PATH)/include/miniupnpc
endif

D_LIBS				:= -L $(LIB_PATH)	\
					-lpthread -ldl -lm -lrt

ifeq ($(DANALE),1)
D_LIBS				+= -L $(LIB_PATH) -lp2pcallee -lminiupnpc

S_LIBS	:=
ifeq ($(DBG),1)
S_LIBS	+= $(LIB_PATH)/libp2pcalleed.a
else
S_LIBS	+= $(LIB_PATH)/libp2pcallee.a
S_LIBS	+= $(LIB_PATH)/libminiupnpc.a
endif
endif

BIN					= 	$(BIN_PATH)/$(OUTPUTFILE)

COMPILE   	=   $(CC) $(CFLAGS) -c "$<" -o "$@" $(LIB_INC)
LINK				=   $(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN) $(OBJS) $(S_LIBS) $(D_LIBS)

#.PHONY:all
all:$(BIN)

$(BIN): $(OBJS)
	@echo "~~~~~~~~~~~~~~~~~~~~"
	@echo "| LINK ALL MODULES |"
	@echo "~~~~~~~~~~~~~~~~~~~~"
	$(LINK)
	@echo "******************************************************"
ifeq ($(DBG),1)
else
	$(STRIP) $(BIN)
	@echo "******************************************************"
endif

$(OBJS_PATH)/%.o : $(MAIN_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE MAIN MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"

$(OBJS_PATH)/%.o : $(DANALE_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE DANALE MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"

$(OBJS_PATH)/%.o : $(ONVIF_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE ONVIF MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"
	
$(OBJS_PATH)/%.o : $(COMMON_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE COMMON MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"
		
$(OBJS_PATH)/%.o : $(INTERFACE_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE INTERFACE MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"

$(OBJS_PATH)/%.o : $(GSOAP_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE GSOAP MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"
	
$(OBJS_PATH)/%.o : $(ARRAY_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE ARRAY MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"	
	
$(OBJS_PATH)/%.o : $(LOCK_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE LOCK MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"	
	
$(OBJS_PATH)/%.o : $(DATACENTER_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE DATACENTER MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"		
	
$(OBJS_PATH)/%.o : $(RINGBUF_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE RINGBUF MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"		
	
$(OBJS_PATH)/%.o : $(THREADS_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE THREADS MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"	
	
$(OBJS_PATH)/%.o : $(DLIST_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE DLIST MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"	
	
$(OBJS_PATH)/%.o : $(MEDIA_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE MEDIA MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"	
ifeq ($(MD5),1)
$(OBJS_PATH)/%.o : $(MD5_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE MEDIA MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"	
endif
ifeq ($(WSSE_EN),1)
$(OBJS_PATH)/%.o : $(WSSE_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE WSSE MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE) $(EXTERN_LIB_INC)
	@echo "******************************************************"
endif
	
$(OBJS_PATH)/%.o : $(TRANSPORT_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE TRANSPORT MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE)  $(EXTERN_LIB_INC)
	@echo "******************************************************"
	
$(OBJS_PATH)/%.o : $(RTSP_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE RTSP MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE)  $(EXTERN_LIB_INC)
	@echo "******************************************************"

$(OBJS_PATH)/%.o : $(INIPARSER_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE INIPARSER MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE)  $(EXTERN_LIB_INC)
	@echo "******************************************************"

$(OBJS_PATH)/%.o : $(SHARE_MEM_PATH)/%.c
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE SHM MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~"
	$(COMPILE)  $(EXTERN_LIB_INC)
	@echo "******************************************************"

#.PHONY:clean
clean:
	@echo "******************************************************"
	@echo "~~~~~~~~~~~~~~~~~~~"
	@echo "| CLEAN ALL MODULE |"
	@echo "~~~~~~~~~~~~~~~~~~~"
	rm -f $(BIN)
	rm -f obj/*[!C].o
#	rm -f obj/*.o
	@echo "******************************************************"

#.PHONY:success
success:
	@echo "******************************************************"
	@echo "~~~~~~~~~~~~~~~~~~~~~~"
	@echo "| SUCCESSFUL COMPILE |"
	@echo "~~~~~~~~~~~~~~~~~~~~~~"
	@echo "******************************************************"

