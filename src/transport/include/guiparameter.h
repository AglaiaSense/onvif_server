/********************************************************
// gui/dvr主程序间通信协议定义
Create by duxiulong(duxlong@live.cn), 2009
*********************************************************/
#ifndef _GUI_PARAMETER_H_
#define _GUI_PARAMETER_H_

//公共设置成功失败返回码
#define GUI_SET_OK  0  //设置成功
#define GUI_SET_ERR 1  //设置失败

#define GUI_SET_DISENABLE   0 //设置不使能
#define GUI_SET_ENABLE 1      //设置使能

//公共数组的长度 (使用时需要 + 1)
#define GUI_PUBLIC_LEN_31     31
#define GUI_PUBLIC_LEN_63     63
#define GUI_PUBLIC_LEN_127    127
#define GUI_PUBLIC_LEN_255    255


#define GUI_DVR_SCR_COLS			22			//屏幕小格列数
#define GUI_DVR_SCR_ROWS			18			//屏幕小格行数

#define GUI_DVR_CHN_CNT			    32			//通道个数

#define GUI_SERIAL_PORT_CNT			16			//串口总个数

#define GUI_PAL_MODE				1			//PAL制
#define GUI_NTSC_MODE				2			//NTSC制
#define GUI_AUTO_MODE               3           //自适应制式

// 显示设备编号
#define GUI_VGA_DEV					0			//VGA输出
#define GUI_BNC_DEV					1			//BNC输出
#define GUI_BNCA_DEV				2			//BNC输出(辅口)
#define GUI_VC_DEV 					16          //虚拟设备
#define GUI_HDMI0_DEV				0         //HDMI0设备
#define GUI_HDMI1_DEV				1		  //HDMI1设备

//磁盘编号
#define GUI_NFS_DISK_ID				12			//nfs磁盘编号
#define GUI_DISK_CD					13			//光驱磁盘编号 
#define GUI_ESATA_DISK_ID			14          //ESATA磁盘编号
#define GUI_DISK_SEC_CD_ID			15          //第二个光驱编号
#define GUI_SD_DISK_ID				50			//SD卡编号


// VGA分辨率
#define VGA_800x600_60				0
#define VGA_1024x768_60				10
#define VGA_1280x1024_60			20
#define VGA_1440x900_60				30
#define VGA_1366x768_60				40
#define VGA_DEF_SIZE				VGA_1024x768_60
#define VGA_640x480					90
//HDMI
#define HDMI_1280x720_50            100
#define HDMI_1280x720_60            110
#define HDMI_1920x1080_25			120
#define HDMI_1920x1080_30			130
#define HDMI_1920x1080_50           140
#define HDMI_1920x1080_60           150
#define HDMI_1280x800_60            160

// 视频分辨率
#define GUI_QCIF 				0		  //P:176×144 N:176×120
#define GUI_HCIF 			    1         //P:352×144 N:352×120
#define GUI_CIF					2		  //P:352×288 N:352×240
#define GUI_2CIF				3		  //P:720×288 N:720×240
#define GUI_D1					4         //P:720×576 N:720×480
#define GUI_MD1					5         //P:720×288 N:720×240
#define GUI_QVGA            	6		  //320×240
#define GUI_FVGA            	7         //640×480
#define GUI_HVGA            	8         //640×240
#define GUI_720P           		9		  //1280×720
#define GUI_960P           		10		  //1280×960
#define GUI_1200           		11		  //1600×1200
#define GUI_1080P          		12        //1920×1080
#define GUI_QXGA				13        //2048×1536
#define GUI_QHD					0x0100    //960×540
#define GUI_960H				0x0200    //P:960×576 N:960×480
#define GUI_5MA					0x0300    //2448×2048
#define GUI_5M 					0x0400    //2560×1920
#define GUI_QSXGA				0x0500    //2560×2048
//走廊模式分辨率格式:100000 + N(N为以上分辨率代码),以此类推
#define GUI_COORIDOR_BASE		0x100000     //走廊模式起始    
#define GUI_720P_9_16			0x100009    //720x 1280
#define GUI_1080P_9_16		  	0x10000C	//1080x 1920

// 视频编码方式
#define GUI_H264			1
#define GUI_MPG4			2
#define GUI_MJPG			3

//音频编码方式
#define GUI_AUDIO_TYPE_G711A	1
#define GUI_AUDIO_TYPE_G711U	2
#define GUI_AUDIO_TYPE_ADPCM	3
#define GUI_AUDIO_TYPE_AAC		22

// 全帧效(PAL:25 NTSC:30)
#define GUI_FULL_FRAME_RATE		2009
//视频参数配置帧率
#define GUI_0_FRAME_RATE     0
#define GUI_1_FRAME_RATE     1
#define GUI_5_FRAME_RATE     5
#define GUI_10_FRAME_RATE    10
#define GUI_15_FRAME_RATE    15
#define GUI_20_FRAME_RATE    20
#define GUI_25_FRAME_RATE    25
#define GUI_30_FRAME_RATE    30
#define GUI_MAX_FRAME_RATE   480

#define GUI_NUM_FRAME_RATE   	7  //0, 1, 5, 10, 15, 20, full

#define GUI_CMD_NOP				0			//空操作

//网络重启关机命令参数 赋值给g_ptGuiSysStatus->m_iParams[0]变量
#define GUI_GET_NET_REBOOT_CMD  20120224 

// 用户管理
#define GUI_USER_LOGIN_REQ		1				//用户登录请求
#define GUI_USER_LOGIN_RSP		-1				//用户登录回应
#define GUI_USER_LOGOUT_REQ		2			//用户注销请求
#define GUI_USER_LOGOUT_RSP		-2			//用户注销回应
#define GUI_ADD_USER_REQ		3			//添加用户请求
#define GUI_ADD_USER_RSP		-3			//添加用户回应
#define GUI_RM_USER_REQ			4			//删除用户请求
#define GUI_RM_USER_RSP			-4				//删除用户回应
#define GUI_EDIT_USER_REQ		5			//修改用户资料请求
#define GUI_EDIT_USER_RSP		-5			//修改用户资料回应
#define GUI_GET_USER_MSG_REQ	6			//获取用户资料请求
#define GUI_GET_USER_MSG_RSP	-6			//获取用户资料回应
#define GUI_GET_USER_GROUP_REQ	7			//获取用户组列表请求
#define GUI_GET_USER_GROUP_RSP	-7				//获取用户组列表回应
#define GUI_GET_USER_LIST_REQ	8			//获取用户列表请求
#define GUI_GET_USER_LIST_RSP	-8			//获取用户列表回应

//UI启动信息
#define GUI_START_UP_MSG		10			//UI启动信息
#define GUI_START_UP_RSP		-10			//UI启动信息回应

//设置VGA格式
#define GUI_SET_VGA_MOD_REQ		11			//设置VGA格式请求
#define GUI_SET_VGA_MOD_RSP		-11			//设置VGA格式回应

//设置BNC格式
#define GUI_SET_BNC_MOD_REQ		12
#define GUI_SET_BNC_MOD_RSP		-12


//------------------------------系统设置---------------------------------
// 1.磁盘管理
#define GUI_GET_DISK_LIST_REQ	50			//获取磁盘列表请求
#define GUI_GET_DISK_LIST_RSP	-50			//获取硬盘列表回应
#define GUI_FORMAT_DISK_REQ		51			//格式化磁盘请求
#define GUI_FORMAT_DISK_RSP		-51			//格式化磁盘回应
#define GUI_GET_FORMAT_STAT_REQ	52			//获取磁盘格式化状态请求
#define GUI_GET_FORMAT_STAT_RSP	-52			//获取磁盘格式化状态回应
#define GUI_DISK_PART_REQ		53			//硬盘分区请求
#define GUI_DISK_PART_RSP		-53			//磁盘分区回应
#define GUI_GET_DISK_MSG_REQ	54			//获取磁盘信息请求
#define GUI_GET_DISK_MSG_RSP	-54			//获取磁盘信息回应
#define GUI_SET_DISK_MSG_REQ	55			//设置磁盘信息请求
#define GUI_SET_DISK_MSG_RSP	-55			//徽墨磁盘信息回应
// 2.串口设置
#define GUI_GET_SPORT_MSG_REQ		60			//获取串口配置请求
#define GUI_GET_SPORT_MSG_RSP		-60			//获取串口配置回应
#define GUI_SET_SPORT_MSG_REQ		61			//设置串口配置请求
#define GUI_SET_SPORT_MSG_RSP		-61			//设置串口配置回应

// 3.系统信息
#define GUI_GET_SYS_MSG_REQ		70			//获取系统信息请求
#define GUI_GET_SYS_MSG_RSP		-70			//获取系统信息回应
#define GUI_SET_SYS_MSG_REQ		71			//设置系统信息请求
#define GUI_SET_SYS_MSG_RSP		-71			//设置系统信息回应
#define GUI_CHG_SYS_TIME_MSG		72			//系统时间更新通知


// 5.系统升级
#define GUI_SYS_UPDATE_STA_REQ		91			//系统升级状态查询请求
#define GUI_SYS_UPDATE_STA_RSP		-91			//系统升级状态查询回应
#define GUI_SYS_FILE_UPDATE_REQ	92			//系统文件升级请求
#define GUI_SYS_FILE_UPDATE_RSP	-92			//系统文件升级回应
#define GUI_SYS_FTP_UPDATE_REQ		93			//系统FTP升级请求
#define GUI_SYS_FTP_UPDATE_RSP		-93			//系统FTP升级回应
#define GUI_SYS_FILE_UPDATE_NUM_REQ	94		//系统文件请求
#define GUI_SYS_FILE_UPDATE_NUM_RSP	-94		//系统文件回应
#define GUI_SYS_UPDATE_SUCCESS 	0
#define GUI_SYS_UPDATE_FAIL    		1


// 6.恢复出厂设置
#define GUI_SET_RENEW_REQ			100		//系统参数重设请求
#define GUI_SET_RENEW_RSP			-100		//系统参数重设回应

// 7.网络设置
#define GUI_GET_IP_REQ				110		//获取IP参数请求
#define GUI_GET_IP_RSP				-110		//获取IP参数回应
#define GUI_SET_IP_REQ				111		//设置IP参数请求
#define GUI_SET_IP_RSP				-111		//设置IP参数回应
#define GUI_GET_NET_CNF_REQ		112		//获取网络服务配置请求
#define GUI_GET_NET_CNF_RSP		-112		//获取网络服务配置回应
#define GUI_SET_NET_CNF_REQ		113		//设置网络服务配置请求
#define GUI_SET_NET_CNF_RSP			-113		//设置网络服务配置回应
#define GUI_GET_REG_CNF_REQ		114		//获取注册中心信息请求
#define GUI_GET_REG_CNF_RSP		-114		//获取注册中心信息回应
#define GUI_SET_REG_CNF_REQ		115		//设置注册中心信息请求
#define GUI_SET_REG_CNF_RSP		-115		//设置注册中心信息回应
#define GUI_GET_DDNS_REQ			130     	 //DDNS请求
#define GUI_GET_DDNS_RSP 			-130      	//DDNS请求回应
#define GUI_SET_DDNS_REQ			131  		//给dvr发送有关DDNS的设置参数
#define GUI_SET_DDNS_RSP			-131   	//给dvr发送有关DDNS的设置参数回应
#define GUI_GET_NTP_REQ				132      	//NTP请求
#define GUI_GET_NTP_RSP				-132      //NTP请求回应
#define GUI_SET_NTP_REQ				133   	//给dvr发送有关NPT的设置参数
#define GUI_SET_NTP_RSP				-133 	//dvr回应
#define GUI_CONNECT_OVER_PPPOE_STAT_REQ 	134        //获取PPPoE状态请求
#define GUI_CONNECT_OVER_PPPOE_STAT_RSP	-134		//回应
#define GUI_CONNECT_OVER_PPPOE_REQ     	135       // PPPoE连接请求
#define GUI_CONNECT_OVER_PPPOE_RSP 	-135       //PPPoE连接回应
#define GUI_CUT_CONNECT_OVER_PPPOE_REQ 136     // 断开PPPoE连接
#define GUI_CUT_CONNECT_OVER_PPPOE_RSP -136   // 断开PPPoE连接回应
#define GUI_SET_PPPOE_REQ 		137                      //设置PPPOE拨号请求
#define GUI_SET_PPPOE_RSP 		-137                     //回应

// 8.异常处理
#define GUI_GET_EXCEP_CNF_REQ		120		//获取异常处理参数请求
#define GUI_GET_EXCEP_CNF_RSP		-120		//获取异常处理参数回应
#define GUI_SET_EXCEP_CNF_REQ		121		//设置异常处理参数请求
#define GUI_SET_EXCEP_CNF_RSP		-121		//设置异常处理参数回应

//9.别名管理
#define GUI_GET_CHN_ALIAS_REQ		140      	//获取通道别名请求
#define GUI_GET_CHN_ALIAS_RSP		-140
#define GUI_SET_CHN_ALIAS_REQ 		141     	 //通道设置
#define GUI_SET_CHN_ALIAS_RSP		-141  	//通道设置回应
#define GUI_GET_CHN_ALIAS_VER_REQ	142		//获取通道别名版本请求
#define GUI_GET_CHN_ALIAS_VER_RSP	-142		//获取通道别名版本回应


//------------------------------图像设置---------------------------------
// 1.预览设置
#define GUI_GET_PREVIEW_REQ		200			//获取预览参数请求
#define GUI_GET_PREVIEW_RSP		-200		//获取预览参数回应
#define GUI_SET_PREVIEW_REQ		201			//设置预览参数请求
#define GUI_SET_PREVIEW_RSP		-201		//设置预览参数回应
#define GUI_PREVIEW_SW_START_REQ	204			//预览切换开始请求
#define GUI_PREVIEW_SW_START_RSP	-204		//预览切换开始回应
#define GUI_PREVIEW_SW_STOP_REQ	205			//预览切换暂停请求
#define GUI_PREVIEW_SW_STOP_RSP	-205		//预览切换暂停回应
#define GUI_PREVIEW_SINGLE_PIC	206			//单画面(与GUI_PREVIEW_SPIC_REQ同)
#define GUI_PREVIEW_SPIC_REQ		206			//单画面
#define GUI_PREVIEW_SPIC_RSP		-206		//单画面回应
#define GUI_PREVIEW_SPIC_EXIT_MSG	251		//单画面退出消息

// 1.1 声音控制
#define GUI_SND_DEC_MSG			253			//音量减消息
#define GUI_SND_ASC_MSG			254			//音量增消息
#define GUI_VOL_CTR_REQ			255			//音量设置请求(音频输入)
#define GUI_VOL_CTR_RSP				-255	

// 2.本地显示
#define GUI_GET_DISPLAY_CNF_REQ	210			//获取本地显示属性请求
#define GUI_GET_DISPLAY_CNF_RSP	-210		//获取本地显示属性回应
#define GUI_SET_DISPLAY_CNF_REQ	211			//设置本地显示属性请求
#define GUI_SET_DISPLAY_CNF_RSP	-211		//设置本地显示属性回应


// 3.视频输入参数
#define GUI_GET_VIDEOIN_CNF_REQ	220			//获取视频输入参数请求
#define GUI_GET_VIDEOIN_CNF_RSP	-220		//获取视频输入参数回应
#define GUI_SET_VIDEOIN_CNF_REQ	221			//设置视频输入参数请求
#define GUI_SET_VIDEOIN_CNF_RSP	-221		//设置视频输入参数回应
#define GUI_CHG_VIDEOIN_CNF_REQ	222			//(临时)更改视频输入参数请求
#define GUI_CHG_VIDEOIN_CNF_RSP	-222		//(临时)更改视频输入参数回应
#define GUI_GET_VIDEOIN_NOR_REQ	223			//获取通道视频输入制式请求	//2009/12/22
#define GUI_GET_VIDEOIN_NOR_RSP	-223		//获取通道视频输入制式回应
#define GUI_SET_VIDEOIN_NOR_REQ	224
#define GUI_SET_VIDEOIN_NOR_RSP	-224

#define GUI_GET_VIDEOIN_MOD_AUTO_CHK_REQ 225	//获取视频制式自动检测请求
#define GUI_GET_VIDEOIN_MOD_AUTO_CHK_RSP -225	//获取视频制式自动检测回应
#define GUI_SET_VIDEOIN_MOD_AUTO_CHK_REQ 226	//设置视频制式自动检测请求
#define GUI_SET_VIDEOIN_MOD_AUTO_CHK_RSP -226	//设置视频制式自动检测回应


// 5.视频前端叠加
#define GUI_GET_VOVERLAY_CNF_REQ	240			//获取视频前端叠加参数请求
#define GUI_GET_VOVERLAY_CNF_RSP	-240		//获取视频前端叠加参数回应
#define GUI_SET_VOVERLAY_CNF_REQ	241			//设置视频前端叠加参数请求
#define GUI_SET_VOVERLAY_CNF_RSP	-241		//设置视频前端叠加参数回应
#define GUI_CHG_LOGO_BMP_REQ		242			//更换logo图标请求
#define GUI_CHG_LOGO_BMP_RSP		-242		//更换logo图标回应
//----------------------视频编码能力配置----------------------------
//视频参数配置
#define GUI_GET_VIDEOPARA_CNF_REQ 232       //获取视频参数配置请求
#define GUI_GET_VIDEOPARA_CNF_RSP -232     //获取视频参数配置回应
#define GUI_SET_VIDEOPARA_CNF_REQ 233	   //设置视频参数配置请求
#define GUI_SET_VIDEOPARA_CNF_RSP -233     //设置视频参数配置回应
//视频总编码能力配置
#define GUI_GET_TOTALCODE_CNF_REQ 234      //获取视频参数配置中总的编码能力请求
#define GUI_GET_TOTALCODE_CNF_RSP -234     //获取视频参数配置中总的编码能力回应


//------------------------------录像设置---------------------------------
// 1.录像模板
#define GUI_GET_VR_TEMPLATE_REQ	300			//获取录像模板请求
#define GUI_GET_VR_TEMPLATE_RSP	-300		//获取录像模板回应
#define GUI_SET_VR_TEMPLATE_REQ	301			//设置录像模板请求
#define GUI_SET_VR_TEMPLATE_RSP	-301		//设置录像模板回应

// 2.录像策略
#define GUI_GET_VR_STRATEGY_REQ	310			//获取录像策略请求
#define GUI_GET_VR_STRATEGY_RSP	-310		//获取录像策略回应
#define GUI_SET_VR_STRATEGY_REQ	311			//设置录像策略请求
#define GUI_SET_VR_STRATEGY_RSP	-311		//设置录像策略回应

// 3.录像文件
#define GUI_COUNT_VIDEO_FILE_REQ	320			//统计录像文件个数请求
#define GUI_COUNT_VIDEO_FILE_RSP	-320		//统计录像文件个数回应
#define GUI_BACKUP_VIDEO_FILE_REQ	322		//备份录像文件请求
#define GUI_BACKUP_VIDEO_FILE_RSP	-322	//备份录像文件回应
#define GUI_DEL_VIDEO_FILE_REQ	323			//删除录像文件请求
#define GUI_DEL_VIDEO_FILE_RSP	-323		//删除录像文件回应
#define GUI_BACKUP_TODAY_VIDEO_REQ	324		//备份当天录像文件请求
#define GUI_BACKUP_TODAY_VIDEO_RSP	-324	//备份当天录像文件回应	
#define GUI_QUERY_VIDEO_END_MSG		325		//录像文件查询结束
#define GUI_GET_VIDEO_FILE_MAP_REQ	326		//获取录像文件快照请求(一个月哪些天有录像)
#define GUI_GET_VIDEO_FILE_MAP_RSP	-326	//获取录像文件快照回应


// 抓拍图片
#define GUI_COUNT_SNATCH_PIC_REQ	327			//统计抓拍图片个数请求
#define GUI_COUNT_SNATCH_PIC_RSP	-327		//统计抓拍图片个数回应
#define GUI_QUERY_SNATCH_PIC_REQ	328			//查询抓拍图片请求
#define GUI_QUERY_SNATCH_PIC_RSP	-328		//查询抓拍图片回应
#define GUI_DEL_SNATCH_PIC_REQ	329				//删除抓拍图片请求
#define GUI_DEL_SNATCH_PIC_RSP	-329			//删除抓拍图片回应


// 4.录像回放
#define GUI_PLAY_VIDEO_START_REQ	330			//播放录像文件请求
#define GUI_PLAY_VIDEO_START_RSP	-330		//播放录像文件回应
#define GUI_PLAY_VIDEO_PAUSE_REQ	331			//播放暂停请求
#define GUI_PLAY_VIDEO_PAUSE_RSP	-331		//播放暂停回应
#define GUI_PLAY_VIDEO_CONT_REQ		332			//播放继续请求
#define GUI_PLAY_VIDEO_CONT_RSP		-332		//播放继续回应
#define GUI_PLAY_VIDEO_STOP_REQ		333			//播放停止请求
#define GUI_PLAY_VIDEO_STOP_RSP		-333		//播放停止回应
#define GUI_PLAY_VIDEO_SPEEDX_REQ	334		//N倍速度播放请求
#define GUI_PLAY_VIDEO_SPEEDX_RSP	-334	//N倍速度播放回应
#define GUI_PLAY_VIDEO_SPEED_CMD	336		//设置快放/慢放倍数请求
#define GUI_PLAY_VIDEO_BY_STEP_REQ	335		//单步播放请求
//#define GUI_PLAY_VIDEO_BY_STEP_RSP	-335	//单步播放回应
//#define GUI_PLAY_VIDEO_NEXT_FRM_REQ	336		//前进一帧请求
//#define GUI_PLAY_VIDEO_PRE_FRM_REQ	337		//后进一帧请求
#define GUI_PLAY_VIDEO_LOC_REQ	338			//播放定位请求
//#define GUI_PLAY_VIDEO_LOC_RSP	-338		//播放定位回应
#define GUI_GET_VPLAY_STA_REQ		339			//请求录像文件播放状态
#define GUI_GET_VPLAT_STA_RSP		-339		//回应录像文件播放状态
#define GUI_GET_VPLAY_STA_RSP		GUI_GET_VPLAT_STA_RSP
//#define GUI_PLAY_VIDEO_RESIZE_REQ	340		//播放窗口重设大小请求
//#define GUI_PLAY_VIDEO_RESIZE_RSP	-340	//播放窗口重设大小回应
#define GUI_VIDEO_PLAYER_ACTIVE_MSG		341		//播放器激活通知(初始化VO，并保存当前预览状态)
#define GUI_VIDEO_PLAYER_ACTIVE_REQ		GUI_VIDEO_PLAYER_ACTIVE_MSG
#define GUI_VIDEO_PLAYER_ACTIVE_RSP		-341	//播放器激活回应
#define GUI_VIDEO_PLAYER_EXIT_MSG		342		//播放器退出通知(应当停止当前回放，前恢复此前保存的预览状态)
#define GUI_VPLAY_FULL_SCR_REQ			343		//指定画面全屏播放请求
#define GUI_VPLAY_FULL_SCR_RSP			-343	//指定画面全屏播放回应
#define GUI_VPLAY_FULL_SCR_EXIT_REQ		344		//全屏播放退出请求
#define GUI_VPLAY_FULL_SCR_EXIT_RSP		-344	//全屏播放退出回应
#define GUI_VPLAY_SET_SND_REQ			355		//回放声音设置请求
#define GUI_VPLAY_SET_SND_RSP			-355	//回放声音设置回应

#define GUI_GET_VPLAY_MARK_MSG_REQ      358    //获取录像文件的打点信息请求
#define GUI_GET_VPLAY_MARK_MSG_RSP      -358   //获取录像文件的打点信息回应

#define GUI_SYNC_PLAY_VIDEO_FILE_REQ 		370		//同步请求播放文件
#define GUI_SYNC_PLAY_VIDEO_FILE_RSP  		-370		//同步播放文件请求回应
#define GUI_SYNC_PLAY_VIDEO_NOFILE 			371		//同步回放继续
#define GUI_SYNC_PLAY_VIDEO_PAUSE_REQ 		372 		//同步回放暂停
#define GUI_SYNC_PLAY_VIDEO_FILE_STOP_REQ 373 	//同步停止请求
#define GUI_SYNC_PLAY_VIDEO_FILE_STEP_REQ 374 	//同步步进请求
#define GUI_SYNC_VPLAY_SET_SND_REQ 375       		//同步声音设置请求
#define GUI_SYNC_VPLAY_SET_SND_RSP -375        	//同步声音设置回应
#define GUI_VIDEO_PLAYER_COORDIRATE_REQ 376	//发送坐标,播放器初始化请求
#define GUI_VIDEO_PLAYER_COORDIRATE_RSP -376	//播放器初始化请求回应
#define GUI_SYNC_PLAY_VIDEO_LOC_REQ		377	//同步播放定位请求
#define GUI_SYNC_GET_VPLAY_STA_REQ		378	//请求录像文件播放状态
#define GUI_SYNC_GET_VPLAY_STA_RSP		-378	//回应录像文件播放状态
#define GUI_SYNC_VIDEO_PLAYER_EXIT_MSG	379		//播放器退出通知(应当停止当前回放，前恢复此前保存的预览状态)
#define GUI_SYNC_VPLAY_FULL_SCR_REQ		380		//指定画面全屏播放请求
#define GUI_SYNC_VPLAY_FULL_SCR_RSP		-380		//指定画面全屏播放回应
#define GUI_SYNC_PLAY_VIDEO_SPEEDX_REQ	381		//N倍速度播放请求
#define GUI_SYNC_VPLAY_FULL_SCR_EXIT_REQ	382		//全屏播放退出请求
#define GUI_SYNC_VPLAY_FULL_SCR_EXIT_RSP    -382    //全屏播放退出请求回应
#define GUI_SYNC_SND_DEC_MSG              		383       //同步播放音量减
#define GUI_SYNC_SND_ASC_MSG              		384        //同步播放音量增
#define GUI_SYNC_PLAY_VIDEO_SPEED_CMD		385		//同步设置快放/慢放倍数请求

// 视频解密
#define GUI_VDECODE_PWD_SET_REQ    386    //视频播放输入密码请求
#define GUI_VDECODE_PWD_SET_RSP    -386    //视频播放输入密码回应

// 视频加密
#define GUI_VENCODE_PWD_SET_REQ	387 //设置视频加密请求
#define GUI_VENCODE_PWD_SET_RSP	-387//设置视频加密回应	
#define GUI_VENCODE_PWD_GET_REQ	388 //获取视频加密请求
#define GUI_VENCODE_PWD_GET_RSP	-388//获取视频加密回应

// 5.手动录像
#define GUI_HUM_REC_VIDEO_CMD			350		//手动录像命令
#define GUI_REC_VIDEO_REQ				351		//通用录像命令
#define GUI_REC_VIDEO_RSP				-351	//通用录像回应

// 6.重建索引
#define GUI_REBUILD_INDEX_REQ			356		//重建索引请求
#define GUI_REBUILD_INDEX_RSP			-356	//重建索引回应
#define GUI_REBUILD_INDEX_STA_REQ	357		//获取重建索引状态请求
#define GUI_REBUILD_INDEX_STA_RSP	-357	//获取重建索引状态回应

// 抓拍浏览
#define GUI_JPEG_VIEWER_INIT_REQ		360		//JPEG显示初始化请求
#define GUI_JPEG_VIEWER_INIT_RSP		-360		//JPEG显示初始化回应
#define GUI_JPEG_VIEWER_EXIT_MSG		361		//JPEG显示退出消息
#define GUI_JPEG_DSP_REQ				362		//JPEG显示请求
#define GUI_JPEG_DSP_RSP				-362		//JPEG显示回应



//------------------------------云台控制---------------------------------
// 解码器设置
#define GUI_GET_PTZ_DECODE_CNF_REQ	400		//获取解码器参数请求
#define GUI_GET_PTZ_DECODE_CNF_RSP	-400	//获取解码器参数回应
#define GUI_SET_PTZ_DECODE_CNF_REQ	401		//设置解码器参数请求
#define GUI_SET_PTZ_DECODE_CNF_RSP	-401	//设置解码器参数回应

// 云台控制
#define GUI_GET_PTZ_PRESETPOINT_REQ	405		//获取预置点列表请求
#define GUI_GET_PTZ_PRESETPOINT_RSP	-405	//获取预置点列表回应
#define GUI_ADD_PTZ_PRESETPOINT_REQ	406		//添加新预置点请求
#define GUI_ADD_PTZ_PRESETPOINT_RSP	-406	//添加新预置点回应
#define GUI_DEL_PTZ_PRESETPOINT_REQ	407		//删除预置点请求
#define GUI_DEL_PTZ_PRESETPOINT_RSP	-407	//删除预置点回应
#define GUI_TRN_PTZ_PRESETPOINT_REQ	408		//调用预置点请求
#define GUI_TRN_PTZ_PRESETPOINT_RSP	-408	//调用预置点回应
#define GUI_GET_PTZ_CRUISROUTE_REQ	409		//获取巡航路径请求
#define GUI_GET_PTZ_CRUISROUTE_RSP	-409	//获取巡航路径回应
#define GUI_SET_PTZ_CRUISROUTE_REQ	410		//设置巡航路径请求
#define GUI_SET_PTZ_CRUISROUTE_RSP	-410	//设置巡航路径回应

#define GUI_PTZ_LOCAL_CODE			411		//本地(UI)生成的云台控制码
#define GUI_PTZ_REMOTE_CODE			412		//远程(网络键盘)生成的云台控码	

#define GUI_PTZ_TRACK_START_REQ		415		//开始记录轨迹请求
#define GUI_PTZ_TRACK_START_RSP		-415	//开始记录轨迹回应
#define GUI_PTZ_TRACK_END_REQ			416		//结束记录轨迹请求
#define GUI_PTZ_TRACK_END_RSP			-416	//结束记录轨迹回应
#define GUI_PTZ_TRACK_PLAY_START_REQ	417		//开始播放轨迹请求
#define GUI_PTZ_TRACK_PLAY_START_RSP	-417	//开始播放轨迹回应
#define GUI_PTZ_TRACK_PLAY_END_REQ	418		//结束播放轨迹请求
#define GUI_PTZ_TRACK_PLAY_END_RSP	-418	//结束播放轨迹回应

#define GUI_PTZ_PATH_PLAY_REQ			419		//演示路径请求
#define GUI_PTZ_PATH_PLAY_RSP			-419	//演示路径回应

#define GUI_PTZ_3D_MSG					420		// 3d定位

#define GUI_GET_PROTOCOL_DECODE_CNT_REQ	421		//获取协议总数
#define GUI_GET_PROTOCOL_DECODE_CNT_RSP	-421		//获取协议总数
#define GUI_GET_PROTOCOL_DEL_REQ			422		//删除协议请求
#define GUI_GET_PROTOCOL_DEL_RSP			-422		//删除协议回应

//PTZ set and get caizhaoxu 20131216
//------------------------------PTZ坐标--------------------------------
#define GUI_GET_PTZ_COMPASS_CNF_REQ    423    //获取PTZ坐标请求
#define GUI_GET_PTZ_COMPASS_CNF_RSP 	 -423    //获取PTZ坐标回应

typedef struct			//获取PTZ坐标请求
{
	int m_iChn;			//通道号
} TGuiGetPTZCompassReq;

typedef struct			//获取PTZ坐标回应
{
	int m_iChn;
	int m_iXp;	//水平坐标，范围: 0~36000
	int m_iYp;	//垂直坐标，范围: -1500~9000
	int m_iZoom;	//变倍大小，范围: 10-500
} TGuiGetPTZCompassRsp;

#define GUI_SET_PTZ_COMPASS_CNF_REQ   424     //设置PTZ坐标请求
#define GUI_SET_PTZ_COMPASS_CNF_RSP  -424     //设置PTZ坐标回应


#define TGuiSetPTZCompassReq TGuiGetPTZCompassRsp

typedef struct			//设置PTZ坐标回应
{
	int m_iRetCode;		//返回码，0表示成功 1表示失败
} TGuiSetPTZCompassRsp;
////////////



//------------------------------报警设置---------------------------------
// 报警输入
#define GUI_GET_ALARM_IN_CNF_REQ		500		//获取报警输入参数请求
#define GUI_GET_ALARM_IN_CNF_RSP		-500	//获取报警输入参数回应
#define GUI_SET_ALARM_IN_CNF_REQ		501		//设置报警输入参数请求
#define GUI_SET_ALARM_IN_CNF_RSP		-501	//设置报警输入参数回应

#define GUI_GET_ALARM_OUT_CNF_REQ	510		//获取报警输出参数请求
#define GUI_GET_ALARM_OUT_CNF_RSP	-510	//获取报警输出参数回应
#define GUI_SET_ALARM_OUT_CNF_REQ	511		//设置报警输出参数请求
#define GUI_SET_ALARM_OUT_CNF_RSP	-511	//设置报警输出参数回应

#define GUI_GET_MD_ALARM_CNF_REQ		520		//获取移动侦测报警参数请求
#define GUI_GET_MD_ALARM_CNF_RSP		-520	//获取移动侦测报警参数回应
#define GUI_SET_MD_ALARM_CNF_REQ		521		//设置移动侦测报警参数请求
#define GUI_SET_MD_ALARM_CNF_RSP		-521	//设置移动侦测报警参数回应

#define GUI_GET_VL_ALARM_CNF_REQ		530		//获取视频丢失报警参数请求
#define GUI_GET_VL_ALARM_CNF_RSP		-530	//获取视频丢失报警参数回应
#define GUI_SET_VL_ALARM_CNF_REQ		531		//设置视频丢失报警参数请求
#define GUI_SET_VL_ALARM_CNF_RSP		-531	//设置视频丢失报警参数回应

#define GUI_CLEAR_ALARM_CMD			540		//消除报警设置


// 注销
#define GUI_SYS_RESET_REQ				600		//系统重启请求
#define GUI_SYS_RESET_RSP				-600	//系统重启回应
#define GUI_SYS_POWEROFF_REQ			601		//关机请求
#define GUI_SYS_POWEROFF_RSP			-601	//关机回应
#define GUI_EXIT_MSG					602		//UI(外挂)退出消息

// 电子放大
#define GUI_EZOOM_ENTER_REQ			700		//进入电子放大功能请求
#define GUI_EZOOM_ENTER_RSP			-700	//进入电子放大功能回应
#define GUI_EZOOM_EXIT_REQ			701		//退出电子放大功能请求
#define GUI_EZOOM_EXIT_RSP			-701	//退出电子放大功能请求
#define GUI_EZOOM_SET_ATTR_REQ		702		//设置电子放大参数请求(包括通道与放大区域)
#define GUI_EZOOM_SET_ATTR_RSP		-702	//设置电子放大参数回应


// 光盘刻录
#define GUI_CD_BACKUP_START_REQ		720	//刻录开始通知
#define GUI_CD_BACKUP_START_RSP		-720
#define GUI_CD_BACKUP_FILE_REQ			721 	//传送刻录文件列表
#define GUI_CD_BACKUP_FILE_RSP			-721
#define GUI_CD_BACKUP_EXEC_REQ		722	//刻录执行请求
#define GUI_CD_BACKUP_EXEC_RSP		-722
#define GUI_CD_BACKUP_PRG_REQ			723	//刻录进度请求
#define GUI_CD_BACKUP_PRG_RSP			-723

// 实时刻录		20100913 dxl
#define GUI_CBL_MODE_GET_REQ			724	//获取实时刻录模式请求
#define GUI_CBL_MODE_GET_RSP			-724 //获取实时刻录模式回应
#define GUI_CBL_START_REQ				726	//启动光盘实时刻录请求
#define GUI_CBL_START_RSP				-726 //启动光盘实时刻录回应
#define GUI_CBL_END_REQ					727	//终止实时刻录打包命令
#define GUI_CBL_END_RSP					-727	//终止实时刻录打包命令
#define GUI_CBL_TAG_REQ					728	//刻录打点命令
#define GUI_CBL_TAG_RSP					-728	//刻录打点命令
#define GUI_CBL_FORCE_END_REQ            729     //强制终止刻录命令
#define GUI_CBL_FORCE_END_RSP            -729    //强制终止刻录命令回应
// 光盘回放相关协议(只添加光盘文件查询及打点信息获取协议)
// 光盘回放协议走原有的标准回放协议
#define GUI_CBL_FILE_QUERY_REQ			730	//搜索光盘文件请求
#define GUI_CBL_FILE_QUERY_RSP			-730
// 数字通道
#define GUI_SET_DIG_CHN_REQ			740	//设置数字通道参数请求
#define GUI_SET_DIG_CHN_RSP			-740 //设置数字通道参数回应
#define GUI_GET_DIG_CHN_REQ			741	//设置数字通道参数请求
#define GUI_GET_DIG_CHN_RSP			-741 //设置数字通道参数回应
#define GUI_SEARCH_DIG_CHN_REQ		742	//搜索数字通道请求
#define GUI_SEARCH_DIG_CHN_RSP			-742	//搜索数字通道回应
#define GUI_GET_SDIG_CHN_REQ			743	//获取数字通道结果请求
#define GUI_GET_SDIG_CHN_RSP			-743	//获取数字通道结果回应
#define GUI_GET_DIG_CHN_PROTOCOL_CNT_REQ  744  //获取数字通道协议总数
#define GUI_GET_DIG_CHN_PROTOCOL_CNT_RSP  -744 //获取数字通道协议总数回应

//通道管理
#define GUI_GET_CHN_TYPE_REQ         		  800   //获取通道类型请求
#define GUI_GET_CHN_TYPE_RSP           		 -800  //获取通道类型回应
#define GUI_GET_LOCAL_CHN_ATTR_REQ 	 		  801   //获取本地模拟通道属性请求
#define GUI_GET_LOCAL_CHN_ATTR_RSP	 		-801  //获取本地模拟通道属性回应
#define GUI_GET_VC_CHN_ATTR_REQ				  802  //获取合成通道属性请求
#define GUI_GET_VC_CHN_ATTR_RSP 				-802  //获取合成通道属性回应
#define GUI_SET_CHN_TYPE_REQ                   803   //设置通道类型请求
#define GUI_SET_CHN_TYPE_RSP                   -803  //设置通道类型回应
#define GUI_GET_CHN_TYPE_EDIT_ABLE_FLG_REQ     804   //获取通道可编辑类型请求
#define GUI_GET_CHN_TYPE_EDIT_ABLE_FLG_RSP     -804  //获取通道可编辑类型回应

#define GUI_SYSTEM_EXIT			1000		//系统退出通知

// 消息
#define GUI_KEY_TX					5000		//遥控器按键
#define GUI_KEY_NET_TX				5001		//网络键盘

// 码流传输
#define GUI_STREAM_CONNECT_REQ		2001
#define GUI_STREAM_CONNECT_RSP		-2001
#define GUI_STREAM_DISCONNECT_REQ		2002
#define GUI_STREAM_DISCONNECT_RSP		-2002
#define GUI_FORCE_IFRAME_REQ			2003
#define GUI_FORCE_IFRAME_RSP			-2003

/********************************************************************************/
//***********************************报警主机******************************//
#define GUI_GET_NET_ALARMER_CNF_REQ     	100010	//获取网络报警主机配置请求
#define GUI_GET_NET_ALARMER_CNF_RSP    	   -100010	//获取网络报警主机配置回应
#define GUI_SET_NET_ALARMER_CNF_REQ			100011	//设置网络报警主机配置请求
#define GUI_SET_NET_ALARMER_CNF_RSP		   -100011	//设置网络报警主机配置回应

#define GUI_GET_SERIAL_ALARMER_CNF_REQ		100012	//获取串口报警主机配置请求
#define GUI_GET_SERIAL_ALARMER_CNF_RSP		-100012	//获取串口报警主机配置回应
#define GUI_SET_SERIAL_ALARMER_CNF_REQ      100013	//设置串口报警主机配置请求
#define GUI_SET_SERIAL_ALARMER_CNF_RSP      -100013	//设置串口报警主机配置回应


//多国语言语言切换通知
#define GUI_SET_CHANGE_LANG_REQ                100100  //发送语言切换请求
#define GUI_SET_CHANGE_LANG_RSP                -100100  //发送语言切换回应



// 标准协议包
typedef struct
{
	int m_iCmd;									//命令码
	int m_iLength;								//pdu负载长度
	char m_cPayload[0];						//实际数据
} TGuiParamPdu;								//数据传输单元结构

// -----------------------用户管理-----------------------------

#define GUI_USER_NAME_LEN			15
#define GUI_USER_PSW_LEN			15
typedef struct									//用户登录请求结构体
{
	char m_strName[GUI_USER_NAME_LEN + 1];		//用户名
	char m_strPsw[GUI_USER_PSW_LEN + 1];		//密码
} TGuiUserLoginReq;

#define GUI_USER_LOGIN_OK			0			//登录成功
#define GUI_USER_LOGIN_PSW_ERR	1			//密码错误
#define GUI_USER_LOGIN_NAME_ERR	2			//用户名不存在
#define GUI_USER_LOGIN_ERR		3			//登录失败

typedef struct 								//用户登录回应
{
	int m_iLoginRs;							//返回码
} TGuiUserLoginRsp;

typedef struct 								//用户注销请求
{
	char m_strName[GUI_USER_NAME_LEN + 1];		//用户名
} TGuiUserLogoutReq;

#define GUI_USER_LOGOUT_OK		0			//注销成功
#define GUI_USER_LOGOUT_ERR		1			//注销失败
typedef struct 								//用户注销回应
{
	int m_iLogoutRs;							//返回码
} TGuiUserLogoutRsp;

typedef struct 								//添加用户请求
{
	int m_iGroupId;							//组编号
	char m_strName[GUI_USER_NAME_LEN + 1]; 	//用户名
	char m_strPsw[GUI_USER_PSW_LEN + 1];		//密码
} TGuiAddUserReq;

#define GUI_ADD_USER_OK			0			//添加用户成功
#define GUI_ADD_USER_GID_ERR		1			//用户组不存在
#define GUI_ADD_USER_EXIST		2			//同名用户已存在
#define GUI_ADD_USER_OTHER_ERR	3			//添加用户其它失败码
typedef struct 								//添加用户回应
{
	int m_iAddUserRs;							//回应码
} TGuiAddUserRsp;

typedef struct 								//删除用户请求
{
	char m_strName[GUI_USER_NAME_LEN + 1]; 	//用户名
} TGuiRmUserReq;

#define GUI_RM_USER_OK				0			//删除用户成功
#define GUI_RM_USER_NOT_EXIST		1			//删除的用户不存在
#define GUI_RM_USER_CAN_NO_RM		2			//指定用户无法被删除
#define GUI_RM_USER_OTHER_ERR		3			//删除用户失败其它错误码

typedef struct									//删除用户回应
{
	int m_iRmUserRes;
} TGuiRmUserRsp;

typedef struct 								//修改用户资料请求
{
	char m_strName[GUI_USER_NAME_LEN + 1];		//用户名
	int m_iNewGroupId;							//新组号
	char m_strNewPsw[GUI_USER_PSW_LEN + 1];	//新密码，为空则不修改
} TGuiEditUserReq;

#define GUI_EDIT_USER_OK			0			//修改用户成功
#define GUI_EDIT_USER_ERR			1			//修改用户失败
typedef struct 								//修改用户资料回应
{
	int m_iEditUserRes;
} TGuiEditUserRsp;


typedef struct 								//获取用户信息请求
{
	char m_strName[GUI_USER_NAME_LEN + 1];
} TGuiGetUserMsgReq;


//在线用户
#define GUI_GET_USERS_ONLINE_REQ		23
#define GUI_GET_USERS_ONLINE_RSP	   -23
typedef struct
{
	int m_iGid;  //用户组,目前可以不用
	char m_strUser[GUI_USER_NAME_LEN + 1];
	char m_strIp[GUI_PUBLIC_LEN_31 + 1];
}TGuiOnlineUserMsg;

typedef struct
{
	int m_iUsersCnt;
}TGuiGetUsersMsgRsp; //先返回在线用户个数，然后逐条返回用户信息

//断开在线用户协议
#define GUI_DISCONNECT_ONLINE_USER_CMD_REQ		24
#define GUI_DISCONNECT_ONLINE_USER_CMD_RSP		-24
typedef struct
{
	int m_iDisconnectTime; //断开时间
	TGuiOnlineUserMsg m_tOnlineUserMsg;
}TGuiSetDisOnlineUserCmdReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetDisOnlineUserCmdRsp;

#define GUI_GET_USER_MSG_OK				0	//获取用户信息成功
#define GUI_GET_USER_MSG_ERR				1	//获取用户信息失败
#define GUI_USER_FLG_CNT					12	//用户权限标识数
#define GUI_USER_FLG_ADD_AND_RM_USER		0	//添加用户权限
#define GUI_USER_FLG_SYS_SETTING			1	//系统参数设置权限
#define GUI_USER_FLG_VIDEO_VIEW			2	//视频预览权限
#define GUI_USER_FLG_VIDEO_PLAY			3	//视频回放权限
#define GUI_USER_FLG_VIDEO_RM				4	//视频删除权限
#define GUI_USER_FLG_CAN_BE_DELETED		5	//该帐号是否能被删除标识索引
#define GUI_USER_FLG_USER_MNT				6	//用户管理权限
#define GUI_USER_FLG_SYS_SET_HI			7	//系统设置高级权限
#define GUI_USER_FLG_PTZ					8	//云台控制权限
#define GUI_USER_FLG_REC					9	//手动录像权限

#define GUI_COMMON_USER					1	//普通用户(只允许浏览视频)
#define GUI_SPECIAL_USER					2	//特权用户(可进行云台控制)
#define GUI_SUPPER_USER					3	//超级用户(可进行系统设置)
#define GUI_ADIMN_USER						4	//管理员(可进行用户管理)

typedef struct 								//获取用户信息回应
{
	int m_iGetUserMsgRs;						//返回码
	int m_iGroupId;							//组编号
	// 权限标识暂时保留，现以组id判断
	char m_cFlgs[GUI_USER_FLG_CNT];				//用户权限标识，每种权限对应一个字节
												// 1表示用户拥有该权限，0表示无
} TGuiGetUserMsgRsp;


typedef struct 								//获取组列表回应
{
	int m_iGrpCnt;	 							//组总个数
} TGuiGetUserGroupListRsp;

#define GUI_USER_GROUP_NAME_LEN		23
typedef struct 								//用户组数据
{
	int m_iGrpId; 								//组编号
	char m_strName[GUI_USER_GROUP_NAME_LEN + 1]; //组名称
} TGuiUserGroupMsg;

typedef struct 								//获取用户列表回应
{
	int m_iUserCnt;							//用户总数
} TGuiGetUserListRsp;

typedef struct 								//用户列表项
{
	int m_iGrpId; 								//用户所属组编号
	char m_strName[GUI_USER_NAME_LEN + 1]; 	//用户名
} TGuiUserMsg;

//===================用户权限细化====================
#define	GUI_GET_USER_RIGHT_REQ		20			//获取用户权限请求
#define	GUI_GET_USER_RIGHT_RSP		-20			//获取用户权限回应

#define GUI_USER_RIGHT_MAX_NUM    16         //用户权限最大条数

#define GUI_USER_RIGHT_PTZ			0		//通道PTZ控制权限
#define GUI_USER_RIGHT_VIDEO_PLAY		1		//回放控制权限

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
}TGuiGetUserRightReq;

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iUserRight[GUI_USER_RIGHT_MAX_NUM];   //用户权限，每个元素代表一个权限，通道权限int每一位代表一个通道
}TGuiGetUserRightRsp;


#define GUI_SET_USER_RIGHT_REQ		21			//设置用户权限请求
#define GUI_SET_USER_RIGHT_RSP		-21			//设置用户权限回应

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iUserRight[GUI_USER_RIGHT_MAX_NUM];  //用户权限，每个元素代表一个权限，通道权限int每一位代表一个通道
}TGuiSetUserRightReq;

typedef struct
{
	int m_iRetCode;									//返回码
}TGuiSetUserRightRsp;						//设置用户权限回应

//===================设置获取/设置用户权限范围======================
typedef enum 
{
	GUI_RIGHT_REMOTE_PTZ = 2,       //远程云镜控制   2
	GUI_RIGHT_REMOTE_VPLAY_PSCAN,	//远程回放浏览3
	GUI_RIGHT_REMOTE_PREV,			//远程预览 4
	GUI_RIGHT_LOCAL_HAND_RECSNAP,	//本地手动录像抓拍5
	GUI_RIGHT_REMOTE_HAND_RECSNAP,  //远程手动录像抓拍6
	GUI_RIGHT_LOCAL_ALARMCLEAR,   	//本地消警7
	GUI_RIGHT_LOCAL_DVD,			//本地实时刻录和光盘回放8
	GUI_RIGHT_LOCAL_LOG,			//本地日志搜索9
	GUI_RIGHT_LOCAL_ALARMSET,		//本地报警设置10
	GUI_RIGHT_LOCAL_CHNMANAGE,		//本地通道管理11
	GUI_RIGHT_LOCAL_PARASET,		//本地参数设置(编码，图像显示，录像模板，录像策略等) 12
	GUI_RIGHT_LOCAL_SYSSET,			//本地系统设置13
	GUI_RIGHT_LOCAL_USERMANAGE,		//本地用户管理14
	GUI_RIGHT_REMOTE_ALARMCLEAR,	//远程消警15
	GUI_RIGHT_REMOTE_REBOOT,		//远程重启/关机16
	GUI_RIGHT_REMOTE_DVD,			//远程实时刻录17
	GUI_RIGHT_REMOTE_LOG,			//远程日志搜索18
	GUI_RIGHT_REMOTE_ALARMSET,		//远程报警设置19
	GUI_RIGHT_REMOTE_CHNMANAGE,		//远程通道管理20
	GUI_RIGHT_REMOTE_PARASET,		//远程参数设置21
	GUI_RIGHT_REMOTE_SYSSET,		//远程系统设置22
	GUI_RIGHT_REMOTE_USERMANAGE,	//远程用户管理23
	GUI_RIGHT_REMOTE_TALK,			//语音对讲24
	GUI_RIGHT_LOCAL_REBOOT,			//本地关机、重启 25
	GUI_RIGHT_MAX_NUM,				
}GUI_USER_RIGHT_ID;

#define GUI_SET_USER_RIGHT_ID_REQ		22		//设置用户权限id范围请求(获取/设置前先发此命令标识获取/设置的id范围)
#define GUI_SET_USER_RIGHT_ID_RSP		-22		//设置用户权限id范围回应

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iStartId;
	int m_iEndId;
}TGuiSetRightIdReq; //设置用户权限id范围请求(获取/设置前先发此命令标识获取/设置的id范围)

#define GUI_SET_RIGHT_ID_OK   0	//设置成功
#define GUI_SET_RIGHT_ID_ERR  1	//设置失败
typedef struct
{
	int m_iRetCode;		//返回码
}TGuiSetRightIdRsp;	//设置用户权限id范围回应



// ------------------------系统设置----------------------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~1.磁盘管理~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef struct 								//获取磁盘列表回应
{
	int m_iDiskCnt;							//磁盘总数
} TGuiGetDiskListRsp;

#define GUI_DISK_NAME_LEN		31

typedef struct 								//磁盘列表项
{
	int m_iDiskId; 							//磁盘唯一编号
	char m_strDiskName[GUI_DISK_NAME_LEN + 1]; //磁盘名
	unsigned long long m_ullDiskSize; 			//磁盘总大小
	unsigned long long m_ullDiskUsed;			//磁盘已用空间
	unsigned long long m_ullDiskFree;			//空闲空间
	int m_iFormatStat;							//是否格式化(0:no, 1:yes)
	int m_iSleepStat;							//休眠状态(0:no, 1:yes)
	int m_iSubAreaCnt;							//分区数
	int m_iUseForBackup;						//是否为备份盘(0:no, 1:yes)08/12/22添加
} TGuiDiskListItem;

typedef struct									//格式化磁盘请求
{
	int m_iDiskId;								//磁盘编号
} TGuiFormatDiskReq;

typedef struct									//格式化磁盘回应
{
	int m_iDiskId; 							//磁盘编号，
	int m_iFormatId;							//格式化编号，每次格式化操作值唯一, 返回-1表示拒绝格式化操作
} TGuiFormatDiskRsp;

typedef struct 								//获取格式化磁盘状态请求
{
	int m_iDiskId;
	int m_iFormateId;
} TGuiGetDiskFormatStatReq;

#define GUI_DISK_FORMAT_ERROR		0			//格式化错误状态
#define GUI_DISK_FORMAT_DOING		1			//格式化进行中状态
#define GUI_DISK_FORMAT_DONE		2			//格式化完成状态

typedef struct 								//获取格式化磁盘状态回应
{
	int m_iDiskId;
	int m_iFormatId;
	int m_iStat;								//当前格式化状态
	int m_iFormatRate;							//格式化进度百分比,-1表式进度不可知
} TGuiGetDiskFormatStatRsp;

typedef struct									//磁盘分区请求
{
	int m_iDiskId;								//磁盘编号
	int m_iSubAreaCnt;							//分区数目
} TGuiDiskPartReq;

#define GUI_DISK_PART_OK			0			//磁盘分区成功
#define GUI_DISK_PART_ERR			1			//磁盘分区失败
typedef struct									//磁盘分区回应
{
	int m_iDiskPartRetCode;					//分区返回码
} TGuiDiskPartRsp;

typedef struct									//获取磁盘信息请求
{
	int m_iDiskId;								//磁盘编号
} TGuiGetDiskMsgReq;

#define GUI_DISK_USE_FOR_REC			1			//磁盘用于录像
#define GUI_DISK_USE_FOR_BACK			2			//磁盘用于备份
#define GUI_DISK_USE_FOR_REDUNDANCE	    3      		//磁盘冗余
#define GUI_DISK_USE_FOR_READONLY		4        	//磁盘用于只读

typedef struct									//磁盘信息
{
	int m_iDiskUsage;							//磁盘用途
	int m_iAlarmWhenErr;						//磁盘错误时报警标识
	int m_iAlarmWhenOutOfSpace;				//磁盘空间不足时报警标识
	unsigned int m_uiDiskReservedSize;		//磁盘空间下限(M)
	int m_iAlartMethod[2];					//磁盘报警开关量, 0:屏幕显示,1声音提示
} TGuiDiskMsg;

typedef struct									//获取磁盘信息回应
{
	int m_iDiskId;								//磁盘编号，负值表示错误
	TGuiDiskMsg m_tMsg;							//磁盘信息
} TGuiGetDiskMsgRsp;

typedef struct									//设置磁盘信息请求
{
	int m_iDiskId;								//磁盘编号
	TGuiDiskMsg m_tMsg;							//磁盘信息
} TGuiSetDiskMsgReq;

#define GUI_SET_DISK_MSG_OK		0			//设置磁盘信息成功
#define GUI_SET_DISK_MSG_ERR		1			//设置磁盘信息失败
typedef struct									//设置磁盘信息回应
{
	int m_iSetDiskMsgRetCode;					//设置磁盘信息返回码
} TGuiSetDiskMsgRsp;


#define GUI_SET_DISK_SMART_ENABLE_REQ  56   //设置磁盘启用smart信息请求
#define GUI_SET_DISK_SMART_ENABLE_RSP -56   //设置磁盘启用smart信息回应

typedef struct
{
	int m_iDiskNo;  //盘号
	int m_iEnable;   //启用读取smart信息，1-启用，0-不启用
}TGuiSetDiskSmartEnableReq;

typedef struct
{
	int m_iRetCode; //0 - 启用成功，1-失败
}TGuiSetDiskSmartEnableRsp;

#define GUI_GET_DISK_SMART_ENABLE_START_REQ  57  //获取磁盘smart 是否启用状态请求
#define GUI_GET_DISK_SMART_ENABLE_START_RSP  -57 //获取磁盘smart 是否启用状态回应
#define GUI_SMART_STR_NAME_LEN 31
typedef struct
{
	int m_iDiskNo;
}TGuiGetDiskSmartEnableStartReq;
typedef struct
{
	int m_iDiskNo;
	int m_iEnable; //0 - 启用成功，1-失败
}TGuiGetDiskSmartEnableStartRsp; 

#define GUI_GET_DISK_SMART_MSG_REQ   58 //获取磁盘smart信息请求
#define GUI_GET_DISK_SMART_MSG_RSP   -58 //获取磁盘smart 信息回应
typedef struct
{
	long long m_llCapacity;  //硬盘容量
	char m_cDevModel[GUI_SMART_STR_NAME_LEN + 1];
	char m_cSerialNum[GUI_SMART_STR_NAME_LEN + 1];
	int m_iTemp; //温度
	int m_iUsedTime;// 按天计算
	int m_iHealth; // 1-通过 0-不通过
	int m_iAllHealth; // 1-健康状况良好
}TGuiDiskSmartBasicMsg;

typedef struct
{
	int m_iDiskNo;
}TGuiGetDiskSmartMsgReq;

typedef struct
{
	int m_iDiskNo;
	int m_iSmartMsgCnt;
	TGuiDiskSmartBasicMsg m_tSmartBasicMsg;
}TGuiGetDiskSmartMsgRsp;

typedef struct
{
	int m_iSmartID;  //HEX 值
	char m_cSmartName[GUI_SMART_STR_NAME_LEN + 1];    //smart 属性名称
	int m_iSmartFlag;   //标识
	int m_iSmartValue;
	int m_iSmartWorst;
	int m_iSmartThresh;
	char m_cSmartStatus[GUI_SMART_STR_NAME_LEN + 1]; //硬盘状态
	long long int m_llSmartRawValue;
}TGuiDiskSmartMsg;


#define GUI_GET_DISK_PART_MSG_REQ    59  //获取磁盘分区信息请求
#define GUI_GET_DISK_PART_MSG_RSP    -59  //获取磁盘分区信息回应

typedef struct  
{
	int m_iDiskId;   //磁盘id号
	int m_iDiskPartId; //磁盘分区id 号从0开始，-1-表示所有分区
}TGuiGetDiskPartMsgReq;

typedef struct
{
	int m_iDiskId;   //磁盘id号
	int m_iDiskPartCnt;  //磁盘分区数
}TGuiGetDiskPartMsgRsp;

#define GUI_DISK_WRITE_PROTECT_YES   1
#define GUI_DISK_WRITE_PROTECT_NO    0
typedef struct
{
	int m_iDiskId; //磁盘id号
	int m_iDiskPartId; //磁盘分区id 号从0开始，-1-表示所有分区
	int m_iDiskPartWriteProtect;					//磁盘分区写保护
	char m_strDiskName[GUI_DISK_NAME_LEN + 1]; //磁盘名
	unsigned long long m_ullDiskPartSize; 			//磁盘分区大小
	unsigned long long m_ullDiskPartUsed;			//磁盘分区已用空间
	unsigned long long m_ullDiskPartFree;			//磁盘分区空闲空间
}TGuiDiskPartListItem;




// ---------------------2.串口设置------------------------------------

#define GUI_SPORT_DATA_NONE_CHECK	0 		//无校验
#define GUI_SPORT_DATA_ODD_CHECK		1 		//奇校验
#define GUI_SPORT_DATA_EVEN_CHECK	2 		//偶校验

typedef struct									//串口配置信息结构体
{
	int m_iPortId;								//串口编号
	unsigned int m_uiBitRate;					//波特率
	unsigned char m_ucDataBits;				//数据位
	unsigned char m_ucStopBits;				//停止位
	unsigned char m_ucCheckType;				//校验位
} TGuiSerialPortMsg;


typedef struct									//获取串口配置信息请求
{
	int m_iPortId;								//串口编号
} TGuiGetSerialPortMsgReq;

#define GUI_GET_SPORT_MSG_OK		0
#define GUI_GET_SPORT_MSG_ERR		-1

typedef struct									//获取串口配置信息回应
{
	int m_iRetCode;							//返回码
	TGuiSerialPortMsg m_tPortMsg;				//串口配置信息
} TGuiGetSerialPortMsgRsp;

typedef struct									//串口设置请求
{
	int m_iPortId;								//串口编号
	TGuiSerialPortMsg m_tSetMsg;				//欲设置的配置信息
} TGuiSetSerialPortMsgReq;


#define GUI_SET_SPORT_MSG_OK		0
#define GUI_SET_SPORT_MSG_ERR		-1

typedef struct									//设置串口回应
{
	int m_iRetCode;							//返回码
} TGuiSetSerialPortMsgRsp;


enum
{
	GUI_SERIAL_WORKMODE_PROCO = 1, //协议模式
	GUI_SERIAL_WORKMODE_CLARITY, //透明通道
	GUI_SERIAL_WORKMODE_APP,       //外挂模式
	GUI_SERIAL_WORKMODE_ALARMHOST,  //报警主机
	GUI_SERIAL_WORKMODE_485COM,		//485串口
	GUI_SERIAL_WORKMODE_GPS,		//车载GPS
	GUI_SERIAL_WORKMODE_ATM,		//ATM 
};

#define GUI_GET_SPORT_WORKMODE_REQ  62  //获取串口的工作模式请求
#define GUI_GET_SPORT_WORKMODE_RSP  -62  //获取串口的工作模式回应
typedef struct
{
	int m_iPortId;	//串口编号
}TGuiGetSerialWorkModeReq;

typedef struct
{
	int m_iPortId;
	int m_iWorkMode;
}TGuiGetSerialWorkModeRsp;

#define GUI_SET_SPORT_WORKMODE_REQ  63 //设置串口的工作模式请求
#define GUI_SET_SPORT_WORKMODE_RSP -63 //设置串口的工作模式回应
typedef struct
{
	int m_iPortId;
	int m_iWorkMode;
}TGuiSetSerialWorkModeReq;
typedef struct
{
	int m_iRetCode;
}TGuiSetSerialWorkModeRsp;
	



// ---------------------3.系统信息------------------------------------

#define GUI_DVE_NAME_LEN			31			//设备名长度
#define GUI_SYS_MSG_STR_LEN			31			//系统信息各字符串长度

typedef struct									//获取系统信息回应
{
	char m_strDveName[GUI_DVE_NAME_LEN + 1];	//设备名
	int m_iDevNo;								//设备编号
	char m_strManufacturer[GUI_SYS_MSG_STR_LEN + 1];	//制造商
	char m_strSN[GUI_SYS_MSG_STR_LEN + 1];		//序列号
	char m_strKernelVer[GUI_SYS_MSG_STR_LEN + 1];		//内核版本
	char m_strKernelVerSlaver[GUI_SYS_MSG_STR_LEN + 1];	 //从片内核版本号
	char m_strWebVer[GUI_SYS_MSG_STR_LEN + 1];	//网页版本
	char m_strSDKVer[GUI_SYS_MSG_STR_LEN + 1];	//sdk版本
} TGuiGetSysMsgRsp;

typedef struct									//设置系统信息请求
{
	char m_strDveName[GUI_DVE_NAME_LEN + 1];	//设备名
	int m_iDevNo;								//设备编号	
} TGuiSetSysMsgReq;

#define GUI_SET_SYS_MSG_OK		0			//设置系统信息成功
#define GUI_SET_SYS_MSG_ERR		1			//设置系统信息失败

typedef struct									//设置系统信息回应
{
	int m_iSetSysMsgRetCode;					//返回码
} TGuiSetSysMsgRsp;


//===================================
//获取单片机版本号(车载)
#define GUI_SET_SCM_VER_REQ	 79	//告诉内核单片机版本请求
#define GUI_SET_SCM_VER_RSP	-79	//告诉内核单片机版本回应

#define GUI_SCM_VER_LEN   31
//单片机版本
typedef struct
{
	char m_strScmVer[GUI_SCM_VER_LEN + 1];//单片机版本号
}TGuiSetScmVerReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetScmVerRsp;
//===================================


// ---------------------4.日志管理------------------------------------
// ===================统计日志总数=====================
#define GUI_LOG_COUNT_REQ			80			//统计日志总数请求
#define GUI_LOG_COUNT_RSP			-80			//统计日志总数回应

#define GUI_LOG_TYPE_ALL		0				//全部日志
#define GUI_LOG_LANG_CHI		1				//中文日志
#define GUI_LOG_LANG_ENG		2				//英文日志
#define GUI_LOG_LANG_TCHI		3				//繁体中文日志
#define GUI_LOG_LANG_KOR		4				//韩文日志
#define GUI_LOG_LANG_SPAN		5				//西班牙文日志
#define GUI_LOG_LANG_ITA		6				//意文日志
#define GUI_LOG_LANG_THAI		7				//泰文日志


typedef struct									//统计日志请求
{
	int m_iBeginTm;							//开始时间
	int m_iEndTm;								//结束时间
	int m_iType;								//日志类型
	int m_iChn;								//通道，0表全部 dxl 2009/12/01
} TGuiLogCountReq;

typedef struct									//统计日志回应
{
	int m_iLogCnt;								//日志总条数
} TGuiLogCountRsp;

//======================查询日志=======================
#define GUI_LOG_QUERY_REQ			81			//查询日志请求
#define GUI_LOG_QUERY_RSP			-81			//查询日志回应

typedef struct									//日志查询请求
{
	int m_iBeginTm;							//日志开始时间
	int m_iEndTm;								//日志结束时间
	int m_iType;								//日志类型
	int m_iChn;								//通道dxl 2009/12/01
	int m_iLang;								//语言,
	int m_iStartId;							//日志起始编号
	int m_iCnt;									//请求日志条数
} TGuiLogQueryReq;

typedef struct									//日志查询回应
{
	int m_iRcdCnt;								//查获的日志总条数
} TGuiLogQueryRsp;

#define GUI_LOG_CONTENT_LEN	127				//日志内容最大长度
typedef struct									//日志结构体
{
	int m_iRecTm;								//记录时间
	int m_iType;								//日志类型码
	int m_iChn;									//通道,0表示与通道无关dxl 2009/12/01
	char m_strContent[GUI_LOG_CONTENT_LEN + 1];	//日志内容
} TGuiLog;
//======================清除日志=======================

#define GUI_LOG_CLEAR_REQ			82			//清空日志请求
#define GUI_LOG_CLEAR_RSP			-82			//清空日志回应

#define GUI_LOG_CLEAR_OK		0				//清空日志成功
#define GUI_LOG_CLEAR_ERR		1				//清空日志失败
typedef struct									//清空日志回应
{
	int m_iRetCode;							//日志清空返回码
} TGuiLogClearRsp;

//======================记录日志=======================

#define GUI_LOG_NOTE_REQ		83     //记录日志请求
#define GUI_LOG_NOTE_RSP        -83    //记录日志回应

typedef struct
{
	int m_iChn;			//通道，0-表示与通道无关
	int m_iLogLevel;       //日志类型
	char strContent[GUI_LOG_CONTENT_LEN + 1]; //日志内容
}TGuiLogNoteReq;              

typedef struct
{
	int m_iRetCode;
}TGuiLogNoteRsp;   //记录日志回应

//================扩展日志查询========================
#define GUI_LOG_QUERY_EX_REQ		84			//(扩展)日志查询请求
#define GUI_LOG_QUERY_EX_RSP		-84			//(扩展)日志查询回应
typedef TGuiLogQueryReq TGuiLogQueryExReq;

typedef TGuiLogQueryRsp TGuiLogQueryExRsp;

//新加日志用户
typedef struct									//日志结构体
{
	int m_iRecTm;								//记录时间
	int m_iType;								//日志类型码
	int m_iChn;								//通道,0表示与通道无关dxl 2009/12/01
	char m_strContent[GUI_LOG_CONTENT_LEN + 1];	//日志内容
	char m_strUserName[GUI_USER_NAME_LEN + 1];   //日志用户
} TGuiLogEx;



// ---------------------5.系统升级------------------------------------
#define GUI_SYS_UPDATE_USE_USB	0			//USB方式升级
#define GUI_SYS_UPDATE_USE_FTP	1			//FTP方式升级
#define GUI_SYS_UPDATE_FTP_LEN	64			//FTP地址长度
#define GUI_SYS_UPDATE_ACCEPT		0			//系统升级请求被接受
#define GUI_SYS_UPDATE_USB_ERR	1			//系统升级找不到USB设备
#define GUI_SYS_UPDATE_FTP_ERR	2			//系统升级无法连接FTP
#define GUI_SYS_UPDATE_FILE_ERR	3			//在指定设备上找不到升级文件
#define GUI_SYS_UPDATE_OTHER_ERR	4			//其它错误

typedef struct									//系统升级请求
{
	int m_iMethod;								//升级方式
	char m_strFtpAddr[GUI_SYS_UPDATE_FTP_LEN + 1]; //FTP地址
} TGuiSysUpdateReq;

typedef struct								//系统升级请求
{
	char m_strFtpAddr[GUI_SYS_UPDATE_FTP_LEN + 1];	//FTP地址
	char m_strFtpUser[GUI_SYS_UPDATE_FTP_LEN + 1]; 	//FTP用户名
	char m_strFtpPasswd[GUI_SYS_UPDATE_FTP_LEN + 1]; 	//FTP密码
} TGuiSysFtpUpdateReq;

typedef struct										//升级请求
{
	char m_strFileName[GUI_SYS_UPDATE_FTP_LEN + 1]; 	 //升级文件名
} TGuiSysUpdateFileReq;

typedef struct										//升级文件请求
{
	char m_strFileName[GUI_SYS_UPDATE_FTP_LEN + 1];	 //升级文件名
} TGuiSysUpdateFileRsp;

typedef struct 								//获取升级文件列表回应
{
	int m_iUpdateId;							//升级序号
	int m_iCfgCnt;							//文件总数
} TGuiGetSysUpdateFileListRsp;

typedef struct									//系统升级回应
{
	int m_iUpdateId;							//升级序号
	int m_iRetCode;							//返回码
} TGuiSysUpdateRsp;

typedef struct									//系统升级状态查询
{
	int m_iUpdateId;							//升级序号
} TGuiSysUpdateStaReq;

#define GUI_SYS_UPDATE_DOING		0			//系统升级进行中
#define GUI_SYS_UPDATE_DONE		1			//系统升级完成
#define GUI_SYS_UPDATE_ERR		2			//系统升级错误
typedef struct
{
	int m_iUpdateId;							//升级序号
	int m_iUpdateSta;							//升级状态
	int m_iUpdateRate;							//升级进度百分比(0-100,其它值表示不可预测)
} TGuiSysUpdateStaRsp;

// ---------------------6.参数重设------------------------------------
#define GUI_SET_RENEW_ACCEPT		0			//系统接受参数重设
#define GUI_SET_RENEW_REFUSE		1			//系统拒绝参数重设
typedef struct
{
	int m_iRetCode;							//返回码
} TGuiSettingRenewRsp;

#define GUI_SET_EXPORT_CNF_REQ       866             //导出获取磁盘列表请求
#define GUI_SET_EXPORT_CNF_RSP		  867			 //导出获取磁盘列表回应
#define GUI_CNF_EXPORT_REQ   868             //导出请求
#define GUI_CNF_EXPORT_RSP   869			//导出回应
#define GUI_SET_IMPORT_CNF_REQ 874			//导入获取磁盘列表请求
#define GUI_SET_IMPORT_CNF_RSP 875			//导入获取磁盘列表回应
#define GUI_CNF_IMPORT_REQ   870			//导入请求
#define GUI_CNF_IMPORT_RSP	  871			//导入回应
#define GUI_CNF_IMPORT_FILE_REFRESH_REQ   872   //导入文件刷新请求
#define GUI_CNF_IMPROT_FILE_REFRESH_RSP   873   //导入文件刷新回应
#define GUI_CNF_SUCESS 0
#define GUI_CNF_FAIL   1
#define GUI_CNF_NAME_LEN 32
#define GUI_IMPORT_FILE_STAT_YES	1				//导入文件选中
#define GUI_IMPORT_FILE_STAT_NO		2				//导入文件未选中

typedef struct							
{
	char m_iDstDevName[GUI_CNF_NAME_LEN + 1];	//导出配置文件磁盘名称
} TGuiCfgPositionExportReq;
typedef struct							
{
	char m_strFileName[GUI_CNF_NAME_LEN + 1];  //导出配置文件名
} TGuiCfgFileExportReq;

typedef struct							
{
	char m_strFileName[GUI_CNF_NAME_LEN + 1];  //导入配置文件名
} TGuiCfgFileImportReq;

typedef struct 								
{
	int m_iCfgCnt;							//总数
} TGuiGetCfgListRsp;

typedef struct								//回应
{
	int m_iRetCode;
} TGuiCfgRsp;




// ---------------------7.网络管理------------------------------------
//====================SMTP设置============================
#define GUI_GET_SMTP_CNF_REQ			108    	//获取SMTP配置信息请求
#define GUI_GET_SMTP_CNF_RSP			-108	//获取SMTP配置信息回应
#define GUI_SET_SMTP_CNF_REQ			109		//设置SMTP配置信息请求
#define GUI_SET_SMTP_CNF_RSP			-109	//设置SMTP配置信息回应

#define GUI_RECEVIE_COUNT				4   //支持4个收件人地址
#define GUI_STR_SMTP_ADDRESS_LEN		63
typedef struct
{
	char	m_cServerName[GUI_STR_SMTP_ADDRESS_LEN + 1];    //smtp服务器名称
	char	m_cUserName[GUI_STR_SMTP_ADDRESS_LEN + 1];      //用户名
	char	m_cPassword[GUI_STR_SMTP_ADDRESS_LEN + 1];		//密码
	char	m_cReciveAddress[GUI_RECEVIE_COUNT][GUI_STR_SMTP_ADDRESS_LEN + 1];	//收件人地址
	char	m_cEmailTitle[GUI_STR_SMTP_ADDRESS_LEN + 1];		//邮件主题
	int		m_iPort;											//端口号
	int		m_iType;											//登录模式
}TGuiSMTPCnfMsg;		//SMTP配置信息

typedef struct
{
	TGuiSMTPCnfMsg m_tSMTPCnf;
}TGuiGetSMTPCnfRsp;		//获取SMTP配置信息回应

typedef struct
{
	TGuiSMTPCnfMsg m_tSMTPCnf;
}TGuiSetSMTPCnfReq;		//设置SMTP配置信息请求

#define GUI_SET_SMTP_CNF_OK		0 //设置成功
#define GUI_SET_SMTP_CNF_ERR	1  //设置失败
typedef struct
{
	int m_iRetCode;
}TGuiSetSMTPCnfRsp;		//设置SMTP配置信息回应

//===================网络服务========================

#define GUI_GET_NET_SERVICE_ENABLE_REQ		106			//获取网络服务启用状态请求
#define GUI_GET_NET_SERVICE_ENABLE_RSP	    -106		//获取网络服务启用状态回应
#define GUI_SET_NET_SERVICE_ENABLE_REQ		107			//设置网络服务启用状态请求
#define GUI_SET_NET_SERVICE_ENABLE_RSP		-107		//设置网络服务启用状态回应

enum
{
	SNMP_SERVICE = 1, //SNMP网络服务
	HTTPS_SERVICE,    //HTTPS服务
	FTP_SERVICE,	  //ftp服务
	HTTP_SERVICE,	  //http服务
};//网络服务id

typedef struct
{
	int m_iId;
}TGuiGetNetServiceReq; //获取网络服务启用状态请求

typedef struct
{
	int m_iId;   //网络服务id号
	int m_iEnable;  //是否使能
}TGuiGetNetServiceRsp;    //获取网络服务启用状态回应

typedef TGuiGetNetServiceRsp TGuiSetNetServiceReq;

#define GUI_SET_NET_SERVICE_OK    	0   //设置成功
#define GUI_SET_NET_SERVICE_ERR  	1	//设置失败
typedef struct
{
	int m_iRetCode; 
} TGuiSetNetServiceRsp;  //设置网络服务启用状态回应

#define GUI_GET_NET_SERVICE_PORT_REQ	104	 //获取网络服务端口请求
#define GUI_GET_NET_SERVICE_PORT_RSP	-104 //获取网络服务端口回应
#define GUI_SET_NET_SERVICE_PORT_REQ	105	//设置网络服务端口请求
#define GUI_SET_NET_SERVICE_PORT_RSP	   -105	//设置网络服务端口回应

typedef struct
{
	int m_iId; 
}TGuiGetNetServicePortReq;   //获取网络服务端口号

typedef struct
{
	int m_iId;
	int m_iPort;
} TGuiGetNetServicePortRsp;

typedef TGuiGetNetServicePortRsp TGuiSetNetServicePortReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetNetServicePortRsp;


//===================流量监控===========================
#define GUI_GET_NET_FLUX_STATE_REQ		119   //获取网络流量状态请求
#define GUI_GET_NET_FLUX_STATE_RSP		-119  //获取网络流量状态回应

#define GUI_NET_FLUX_STATE_CNT    31
typedef struct
{
	char m_cUploadRate[GUI_NET_FLUX_STATE_CNT + 1];   //上传速率
	char m_cDownloadRate[GUI_NET_FLUX_STATE_CNT + 1]; //下载速率
	char m_cUploadTotal[GUI_NET_FLUX_STATE_CNT + 1];     //上传总流量 
	char m_cDownloadTotal[GUI_NET_FLUX_STATE_CNT + 1]; //下载总流量  
}TGuiNetFluxState;    //网络信息，包括速率和总流量

typedef struct
{
	int m_iCardNo;  //网卡号  0-预留 1-网卡1，  2-网卡2
}TGuiGetNetFluxStateReq; //获取网络流量状态请求

typedef struct
{
	int m_iCardNo;  //网卡号0-预留 1-网卡1，  2-网卡2
	TGuiNetFluxState m_iNetFluxState; 
}TGuiGetNetFluxStateRsp;  //获取网络流量状态回应

//=================网络丢包、延时测试===================
#define GUI_NET_DELAY_TEST_CMD_REQ	124  //网络延时测试命令请求
#define GUI_NET_DELAY_TEST_CMD_RSP	-124  //网络延时测试命令

typedef struct
{
	int m_iCardNo;         //网卡编号0保留，1，2
	char m_strDstIp[GUI_PUBLIC_LEN_31 + 1];			//目的ip地址
}TGuiNetDelayTestCmdReq;

typedef struct
{
	int m_iCardNo;
	int m_iReachable; 	//网络地址是否可达,0:可达；其它:不可达
	int m_iDelayTm;		//网络延时,单位(ms)
	int m_iLostrate;		//丢包率,取值范围:0-100,值为50表示:丢包率:50%
}TGuiNetDelayTestCmdRsp;

//====================抓包备份====================
#define GUI_NETPACK_CAPTURE_CMD_REQ	125  //网络抓包命令请求
#define GUI_NETPACK_CAPTURE_CMD_RSP	-125  //网络抓包命令请求回应

typedef struct
{
	int m_iCardNo;	     //网卡号
	int m_iDiskNo;       //磁盘号
	int m_iPackSize;      //网络包大小，最大1M
	int m_iTmDelay;      //超时时间，默认1分钟，先到达为准 
}TGuiNetPackCaptureReq;

typedef struct
{
	int m_iRetCode;
} TGuiNetPackCaptureRsp;

//=================状态检测=======================
#define GUI_NET_STATUS_TEST_CMD_REQ  126   //网络状态检测
#define GUI_NET_STATUS_TEST_CMD_RSP	  -126  //回应

typedef struct
{
	int m_iCardNo;
}TGuiGetNetStatusTestReq;  //获取网络检测请求

//0表示所有网络都可达，1表示网关不可达，2表示DNS不可达，3表示外网不可达
typedef struct
{
	int m_iCardNo;   //网卡号
	int m_iRetCode;	//按位表示，从最低位开始
}TGuiGetNetStatusTestRsp;

//==============网络资源检测=========================
#define GUI_NET_RESOURCE_CHECK_REQ   127  //网络资源检测请求
#define GUI_NET_RESOURCE_CHECK_RSP	   -127  //网络资源检测回应

typedef struct
{
	int m_iIpChnBandWidth;	   //ip通道带宽（单位为M，以下类似）
	int m_iRePreBandWidth;    //远程预览
	int m_iRePlayBandWidth;   //远程回放
	int m_iNetReciveBandWidth;   //网络接收带宽
	int m_iNetSendBandWidth;    //网络发送带宽
}TGuiNetResourceMsg;

typedef struct
{
	TGuiNetResourceMsg m_tNetResMsg;
}TGuiGetNetResourceMsgRsp;  //回复各种带宽


//============网络设置管理====================

#define GUI_GET_IP_VIA_DHCP			0			//dhcp方式获取IP
#define GUI_GET_IP_VIA_HUM			1			//手动设置IP
#define GUI_GET_IP_VIA_PPPOE		2			//PPPoE方式
#define GUI_IP_STR_LEN				31			//IP地址字符串长度
#define GUI_GET_DNS_VIA_DHCP    	0
#define GUI_GET_DNS_VIA_HUM    		1

#define GUI_GET_IP_VIA_PPPOE_ENABLE		true			//PPPoE方式能够设置
#define GUI_GET_IP_VIA_PPPOE_UNENABLE		false			//PPPoE方式不能够设置

typedef struct									//网络参数
{
	int m_iGetIpMethod;						//获取ip地址的方式
	int m_blIsHandSetDns;						//手动设置DNS
	char m_strMac[GUI_IP_STR_LEN + 1];			//mac地址
	char m_strIp[GUI_IP_STR_LEN + 1];			//ip地址
	char m_strSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strGetWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];			//首选域名解析
	char m_strSecDNS[GUI_IP_STR_LEN + 1];			//备选域名解析
} TGuiIpMsg;

typedef struct									//获取IP参数回应
{
	TGuiIpMsg m_tIpMsg;						//ip信息
} TGuiGetIpRsp;

typedef struct									//设置IP参数请求
{
	TGuiIpMsg m_tIpMsg;
} TGuiSetIpReq;

#define GUI_SET_IP_OK			0				//设置IP地址成功
#define GUI_SET_IP_ERR			1				//设置IP地址失败
typedef struct									//设置IP地址回应
{
	int m_iRetCode;							//返回码
	TGuiIpMsg m_tIpMsg;						//重设后的IP信息
} TGuiSetIpRsp;

#define GUI_GET_SECOND_NETWORK_IP_REQ  138  //获取第二个网口ip地址信息请求
#define GUI_GET_SECOND_NETWORK_IP_RSP  -138 //获取第二个网口ip地址信息回应

#define GUI_SECOND_NETWORK_CARD_NO 2  //第二个网卡号
enum
{
	GUI_NETWORK_CARD_MODE_DIFFERENT = 0, // 第二个网卡的模式和网卡1不同
	GUI_NETWORK_CARD_MODE_SAME,   //第二个网卡的模式和网卡1相同
};

enum
{
	GUI_NET_WORK_Redundancy = 1, //冗余模式
	GUI_NET_WORK_Balance,        //负载均衡模式
};

typedef struct
{
	int m_iNetWorkCardNo; //网口号
}TGuiGetSecondNetWorkIpReq;
typedef struct
{
	int m_iNetWorkCardNo;
	int m_iNetWorkCardMode;//是否和网卡1相同的配置
	int m_iNetWorkMode; //工作模式，1-冗余模式，2-负载均衡模式
	TGuiIpMsg m_tIpMsg;		
}TGuiGetSecondNetWorkIpRsp;

#define GUI_SET_SECOND_NETWORK_IP_OK			0				//设置IP地址成功
#define GUI_SET_SECOND_NETWORK_IP_ERR			1				//设置IP地址失败
#define GUI_SET_SECOND_NETWORK_IP_REQ   139  //设置第二个网口ip地址信息请求
#define GUI_SET_SECOND_NETWORK_IP_RSP   -139 //设置第二个网口ip地址信息回应

typedef TGuiGetSecondNetWorkIpRsp TGuiSetSecondNetWorkIpReq;

typedef struct
{
	int m_iRetCode;
	int m_iNetWorkCardNo;
	int m_iNetWorkCardMode;//是否和网卡1相同的配置
	int m_iNetWorkMode; //工作模式，1-冗余模式，2-负载均衡模式
	TGuiIpMsg m_tIpMsg;		
}TGuiSetSecondNetWorkIpRsp;
	
	

#define GUI_NET_SV_STR_LEN		64
#define GUI_NET_SV_IP_CNT			16
#define GUI_NET_IP_DISENABLE		0			//禁用ip权限设定
#define GUI_NET_IP_FORBID			1			//禁止IP访问
#define GUI_NET_IP_ACCESS			2			//允许IP访问
typedef struct
{
	char m_strDnsServer[GUI_NET_SV_STR_LEN + 1];	//DNS服务器
	int m_iFtpPort;		//ftp端口号
	int m_iHttpPort;		//http端口号
	int m_iNfsEnable;		//网络存储服务使能
	int m_bupnpenable;                //UPNP使能
	char m_strNfsServer[GUI_NET_SV_STR_LEN + 1];	//网络存储服务器
	int m_iNfsPort;								//网络存储端口号
	char m_strNfsPath[GUI_NET_SV_STR_LEN + 1];		//网络存储路径
	char m_strNfsUser[GUI_NET_SV_STR_LEN + 1];		//网络存储帐号
	char m_strNfsPsw[GUI_NET_SV_STR_LEN + 1];		//网络存储密码
	int m_iIpPopedomType;							//IP权限类型 1:禁止,2:允许，其它保留 
	int m_iIpListSize;								//IP列表长度,0-16(GUI_NET_SV_IP_CNT)
	char m_strIpList[GUI_NET_SV_IP_CNT][GUI_NET_SV_STR_LEN + 1]; //IP列表
} TGuiNetServerCnf;

typedef struct										//获取网络配置回应
{
	TGuiNetServerCnf m_tCnf;
} TGuiGetNetCnfRsp;

typedef struct										//设置网络配置请求
{
	TGuiNetServerCnf m_tCnf;
} TGuiSetNetCnfReq;

#define GUI_SET_NET_CNF_OK		0				//设置网络配置成功
#define GUI_SET_NET_CNF_ERR		1				//设置网络配置失败
typedef struct										//设置网络配置回应
{
	int m_iRetCode;
} TGuiSetNetCnfRsp;

#define GUI_REG_CENTER_SVR_LEN		31
#define GUI_REG_CENTER_USER_LEN		15
#define GUI_REG_CENTER_PSW_LEN		15
#define GUI_DEV_NAME_LEN				31
typedef struct										//注册中心信息
{
	int m_iPort1;									//端口
	char m_strSvr1[GUI_REG_CENTER_SVR_LEN + 1];	//地址
	int m_iPort2;									//端口
	char m_strSvr2[GUI_REG_CENTER_SVR_LEN + 1];	//地址
	char m_strDevName[GUI_DEV_NAME_LEN + 1];		//设备名称
	char m_strName[GUI_REG_CENTER_USER_LEN + 1];	//用户名
	char m_strPsw[GUI_REG_CENTER_PSW_LEN + 1];		//密码
} TGuiRegCenterCnf;

#define GUI_GET_REG_CNF_OK			0			//获取成功
#define GUI_GET_REG_CNF_ERR			1			//获取失败
typedef struct										//获取注册中心信息回应
{
	int m_iRetCode;								//返回码
	TGuiRegCenterCnf m_tCnf;						//注册中心配置信息
} TGuiGetRegCenterCnfRsp;

typedef struct										//设置注册中心信息请求	
{
	TGuiRegCenterCnf m_tCnf;						//注册中心配置信息
} TGuiSetRegCenterCnfReq;

#define GUI_SET_REG_CNF_OK			0
#define GUI_SET_REG_CNF_ERR			1
typedef struct
{
	int m_iRetCode;								//返回码
} TGuiSetRegCenterCnfRsp;

#define GUI_PPPOE_DISCONNECT	0				//PPPoE拨号未连接
#define GUI_PPPOE_CONNECTING 	1				//PPPoE正在拨号
#define GUI_PPPOE_CONNECTED    	2				//PPPoE拨号成功(已连接)

typedef struct									//网络参数
{
	int m_iRetCode;                           				 //返回码 0:成功；1:失败
	int m_iConnectStat;							//PPPoE当前连接状态
	int m_bIsPppoE;                            				//PPPoE自动拨号使能开关
	char m_strPPPoeUsr[GUI_IP_STR_LEN + 1]; 		//PPPoE帐号
	char m_strPPPoePsw[GUI_IP_STR_LEN + 1];		//PPPoE密码
	char m_strPPPoEIp[GUI_IP_STR_LEN + 1];			//PPPoE地址
	char m_strPPPoESubMsk[GUI_IP_STR_LEN + 1];		//PPPoE子网掩码
	char m_strPPPoEGetWay[GUI_IP_STR_LEN + 1];		//PPPoE网关
} TGuiPppoeMsg;

typedef struct
{
	char	m_cNvsName[GUI_IP_STR_LEN + 1];          //设备域名
	char	m_cDomainName[GUI_IP_STR_LEN + 1];       //服务器地址
	char 	m_cUserName[GUI_IP_STR_LEN + 1];         //用户名
	char	m_cPassword[GUI_IP_STR_LEN + 1];			//密码
	int		m_iPort;					//端口号
	int 	m_iDdnsEnable;				//DDNS使能
}TGuiStrDdnsPara;

typedef struct
{
	int m_iRetCode;
}TGuiSetDdnsRsp;

typedef struct
{
	char m_strNtpIp[GUI_IP_STR_LEN + 1];			//网络地址
	int m_iport;                           //端口号
	int m_itime;                           //时间间隔                   
}TGuiSetNtp;

typedef struct
{
	int m_iRetCode;
}TGuiSetNtpRsp;

//---------------------------DHCP服务端-------------------------------
#define GUI_GET_DHCP_SERVER_MSG_REQ		116		//获取DHCP服务信息请求
#define GUI_GET_DHCP_SERVER_MSG_RSP		-116		//获取DHCP服务信息回应

typedef struct						//DHCP网络参数
{
	int m_blEnable;				//是否启用DHCP服务
	int m_iRentTime;				//地址租期（分钟）
	char m_strStartIp[GUI_IP_STR_LEN + 1];		//地址池开始IP地址
	char m_strEndIp[GUI_IP_STR_LEN + 1];		//地址池结束IP地址
	char m_strGetWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];		//首选域名解析
	char m_strSecDNS[GUI_IP_STR_LEN + 1];		//备选域名解析
} TGuiDhcpServerMsg;

typedef struct						//获取DHCP服务参数回应
{
	TGuiDhcpServerMsg m_tDhcpServerMsg;
}TGuiGetDhcpServerMsgRsp;

#define GUI_SET_DHCP_SERVER_MSG_REQ		117		//设置DHCP服务信息请求
#define GUI_SET_DHCP_SERVER_MSG_RSP		-117	//设置DHCP服务信息回应

#define GUI_SET_DHCP_SERVER_MSG_OK			0
#define GUI_SET_DHCP_SERVER_MSG_ERR			1

typedef struct						//设置DHCP服务参数请求
{
	TGuiDhcpServerMsg m_tDhcpServerMsg;
}TGuiSetDhcpServerMsgReq;

typedef struct					//设置DHCP服务参数回应
{
	int m_iRetCode;				//设置DHCP服务参数回应，0表示成功
}TGuiSetDhcpServerMsgRsp;

#define GUI_GET_DHCP_CLIENT_MSG_REQ 		118		//获取DHCP客户端连接列表请求
#define GUI_GET_DHCP_CLIENT_MSG_RSP 		-118	//获取DHCP客户端连接列表回应

#define GUI_CLIENT_NAME_LEN		31

typedef struct						//客户端信息
{
	int m_iClientID;				//客户端ID
	int m_iRentTime;				//有效时间
	char m_strClientName[GUI_CLIENT_NAME_LEN + 1];	//客户端名
	char m_strMac[GUI_IP_STR_LEN + 1];		//mac地址
	char m_strIp[GUI_IP_STR_LEN + 1];		//IP地址
} TGuiDhcpClientMsg;

typedef struct						//客户端连接列表请求回应
{
	int m_iDhcpClientCnt;			//客户端连接总条数
}TGuiGetDhcpClientMsgRsp;


// ---------------------8.异常处理------------------------------------
#define GUI_MAX_EXCEPTION_CNT		12
#define GUI_SUGGEST_CNT       3
#define GUI_SCREEN_DSP        0
#define GUI_SOUND_ENABEL      1
#define GUI_TRAN_TO_SERVER    2

typedef struct										//异常处理配置信息
{
	int m_iExHanldFlg[GUI_MAX_EXCEPTION_CNT]; 		// 1: 处理, 0 : 不处理
	int m_iSuggestFlg[GUI_MAX_EXCEPTION_CNT][GUI_SUGGEST_CNT];   //1:处理 0:不处理
} TGuiExceptionCnf;

#define GUI_GET_EXCEP_CNF_OK		1
#define GUI_GET_EXCEP_CNF_ERR		0

typedef struct										//获取配置信息回应
{
	int m_iRetCode;								//回应码
	TGuiExceptionCnf m_tCnf;						//配置信息
} TGuiGetExceptionCnfRsp;

typedef struct										//设置配置信息请求	
{
	TGuiExceptionCnf m_tCnf;
} TGuiSetExceptionCnfReq;

#define GUI_SET_EXCEP_CNF_OK		1
#define GUI_SET_EXCEP_CNF_ERR		0

typedef struct										//设置配置信息回应
{
	int m_iRetCode;								//回应码
} TGuiSetExceptionCnfRsp;
//-------------------------------9.别名管理-------------------------------
#define GUI_ALIAS_NAME_STR_LEN 	16
#define GUI_VIDEO_FLAG 1
#define GUI_ALARM_IN_FLAG 2
#define GUI_ALARM_OUT_FLAG 3

typedef struct
{
	int m_iNameFlag;//别名标志，1为视频、2为报警输入、3为报警输出
	int m_iChnNum;//通道号
	char m_cChnName[GUI_ALIAS_NAME_STR_LEN + 1];//通道名	
} TGuiChnAlais;	//通道别名

typedef struct
{
	int m_iNameFlag;
	int m_iChnNum;
} TGuiGetChnAliasReq;

typedef struct
{
	int m_iRetCode;		// 0表示成功
	TGuiChnAlais m_tAlias;
} TGuiGetChnAliasRsp;

typedef struct
{
	TGuiChnAlais m_tAlias;
}TGuiSetChnAliasReq;

typedef struct
{
	int m_iRetCode;	// 0表示成功
}TGuiSetChnAliasRsp;

typedef struct		//获取通道别名版本请求
{
	int m_iType;		//类型号
} TGuiGetChnAliasVerReq;

typedef struct
{
	int m_iType;		//类型号
	int m_iCurVer;	//版本号
} TGuiGetChnAliasVerRsp;

//扩展的通道别名(支持64个字节)
#define GUI_GET_CHN_ALIAS_REQ_EX		143
#define GUI_GET_CHN_ALIAS_RSP_EX		-143
#define GUI_SET_CHN_ALIAS_REQ_EX		144
#define GUI_SET_CHN_ALIAS_RSP_EX		-144

typedef struct
{
	int m_iNameFlag;//别名标志，1为视频、2为报警输入、3为报警输出
	int m_iChnNum;//通道号
	char m_cChnName[GUI_PUBLIC_LEN_63 + 2];//通道名	
} TGuiChnAlaisEx;	//通道别名

typedef struct
{
	int m_iNameFlag;
	int m_iChnNum;
} TGuiGetChnAliasReqEx;

typedef struct
{
	int m_iRetCode;		// 0表示成功
	TGuiChnAlaisEx m_tAliasEx;
} TGuiGetChnAliasRspEx;

typedef struct
{
	TGuiChnAlaisEx m_tAliasEx;
}TGuiSetChnAliasReqEx;

typedef struct
{
	int	m_iRetCode;
}TGuiSetChnAliasRspEx;
//==============end=========


//自动维护
#define GUI_GET_AUTOMAINTAIN_REQ	880
#define GUI_GET_AUTOMAINTAIN_RSP	-880
#define GUI_SET_AUTOMAINTAIN_REQ	881
#define GUI_SET_AUTOMAINTAIN_RSP	-881

//星期日到星期六为0～6，7-每天，8-从不
typedef struct
{
	int m_iType;      //操作类型，按位表示，最低一位为1，表示定时重启
	int m_iWeekDay;   //星期几
	int m_iTm;        //每天从0点开始流逝的秒数
}TGuiClockMsg;

typedef struct
{
	TGuiClockMsg m_tClockMsg;
}TGuiGetAutoMaintainRsp;

typedef struct
{
	TGuiClockMsg m_tClockMsg;
}TGuiSetAutoMaintainReq;

//使用公用的返回码，GUI_SET_OK表成功，GUI_SET_ERR表失败
typedef struct
{
	int m_iRetCode;
} TGuiSetAutoMaintainRsp;


//==============================图像设置 ===============================
//============================预览效果===========================
#define GUI_GET_PREVIEW_PIC_QUALITY_REQ		198			//获取预览效果参数请求
#define GUI_GET_PREVIEW_PIC_QUALITY_RSP		-198		//获取预览效果参数回应
#define GUI_SET_PREVIEW_PIC_QUALITY_REQ		199			//设置预览效果参数请求
#define GUI_SET_PREVIEW_PIC_QUALITY_RSP		-199		//设置预览效果参数回应

enum
{
	PIC_HIGH_QUALITY = 0,			//最佳图像质量
	PIC_HIGH_PERFORMANCE = 1,		//最大预览性能
}; //预览图像效果参数

typedef struct
{
	int m_iPicQP;		//预览图像效果参数
}TGuiPreviewPicQualityCnf;			

typedef struct									//获取预览效果参数回应
{
	TGuiPreviewPicQualityCnf m_tCnf;			//预览效果
} TGuiGetPreviewPicQualityRsp;					//获取预览效果参数回应			

typedef struct											
{
	TGuiPreviewPicQualityCnf m_tCnf;			//预览效果
} TGuiSetPreviewPicQualityReq;				//设置预览效果参数请求

#define GUI_SET_PREVIEW_PIC_QUALITY_OK		0	//设置成功
#define GUI_SET_PREVIEW_PIC_QUALITY_ERR		1	//设置失败
typedef struct											
{
	int m_iRetCode;			//返回码
} TGuiSetPreviewPicQualityRsp;				//设置预览效果参数回应





//------------------------------ 1.预览设置 ----------------------------




typedef struct									//预览切换序列
{
	int m_iCnt;									//序列长度
	int m_iChnList[GUI_DVR_CHN_CNT];			//存放通道号
} TGuiPreviewSeq;

// 在获取预览参数回应与设置预览参数请求时，除发送相应的rsp与req外，随即发
// 送相应个数的TGuiPreviewSeq

typedef struct
{
	int m_iDevId;								//设备编号
} TGuiGetPreviewReq;						//获取预览参数请求 dxl 2009/11/30

typedef struct									//获取预览参数回应
{
	int m_iPicCnt;								//画面数，0表示获取失败
	int m_iInterval;							//切换时间间隔,0表不切抱换
} TGuiGetPreviewRsp;

typedef struct									//设置预览参数请求
{
	int m_iDevId;								//设备编号 2009/11/30
	int m_iPicCnt;
	int m_iInterval;
} TGuiSetPreviewReq;

#define GUI_SET_PREVIEW_OK		0			//设置成功
#define GUI_SET_PREVIEW_ERR		1			//设置失败
typedef struct									//设置预览参数回应
{
	int m_iRetCode;							//返回码
} TGuiSetPreviewRsp;

typedef struct
{
	int m_iDevId;									//设备编号
} TGuiPreviewSwStartReq;					//开始视频预览切换请求dxl 2009/11/30

#define GUI_PREVIEW_SW_START_DONE	0		//成功开启视频切换
#define GUI_PREVIEW_SW_START_ERR		1		//开启视频切换失败
typedef struct									//开启视频切换回应
{
	int m_iRetCode;
} TGuiPreviewSwStartRsp;

typedef struct
{
	int m_iDevId;									//设备编号
} TGuiPreviewSwStopReq;						//停止预览切换请求 2009/11/30

#define GUI_PREVIEW_SW_STOP_DONE		0		//成功停止视频切换
#define GUI_PREVIEW_SW_STOP_ERR		1		//停止视频切换失败
typedef struct									//停止视频切换回应
{
	int m_iRetCode;
} TGuiPreviewSwStopRsp;

typedef struct									//单画面播放命令
{
	int m_iDevId;									//设备编号 dxl 2009/11/30
	int m_iChn;									//单画面通道号
} TGuiPreviewSingPicReq;

#define GUI_PREVIEW_SPIC_OK		0			//设置单画面成功
#define GUI_PREVIEW_SPIC_ERR		1			//设置单画面失败
typedef struct									//单画面播放命令回应
{
	int m_iDevId;
	int m_iChn;									//单画面通道号
	int m_iRetCode;							//返回码
} TGuiPreviewSingPicRsp;

typedef struct
{
	int m_iDevId;									//设备编号
} TGuiPreviewSingPicExitMsg;					//单画面退出通知dxl 2009/11/30

#define GUI_PREVIEW_MAP_REQ		207			//视频映射请求
#define GUI_PREVIEW_MAP_RSP		-207		//视频映射回应
typedef struct									//视频映射请求
{
	int m_iDevId;									//设备号
	int m_iChn;									//通道号
	int m_iLeft;									//左上角x坐标
	int m_iTop;									//左上角y坐标
	unsigned int m_uiWidth;						//宽
	unsigned int m_uiHeight;						//高
} TGuiPreviewMapReq;

#define GUI_PREVIEW_MAP_OK			0		//视频映射成功
#define GUI_PREVIEW_MAP_ERROR		1		//视频映射失败
typedef struct								//视频映射回应
{
	int m_iRetCode;
} TGuiPreviewMapRsp;

#define GUI_PREVIEW_MAP_EXIT_MSG	260		//结束视频映射(无回应)
typedef struct								//结束视频映射
{
	int m_iDevId;	 //设备号
} TGuiPreviewMapExitMsg;

#define GUI_PREVIEW_SW_STA_REQ	208			//获取视频切换状态请求
#define GUI_PREVIEW_SW_STA_RSP	-208		//获取视频切换状态回应
typedef struct
{
	int m_iDevId;								//设备号
} TGuiPreviewSwStaReq;					//获取预览状态请求 2009/11/30

#define GUI_PREVIEW_SW_STA_STARTED	0		//视频预览切换已开启
#define GUI_PREVIEW_SW_STA_STOPED	1		//视频预览切换已停止	
typedef struct									//获取视频预览切换状态回应
{
	int m_iRetStat;							//返回状态
} TGuiPreviewSwStaRsp;

#define GUI_PREVIEW_MULTI_PIC_REQ	209		//设置预览画面数请求
#define GUI_PREVIEW_MULTI_PIC_RSP	-209	//设置预览画面数回应
typedef struct									//更改当前预览画面数请求
{
	int m_iDevId;								//设备号	2009/11/30 dxl
	int m_iPicCnt;								//欲设置的预览画面数(0表示查询)
} TGuiPreviewMultiPicReq;

typedef struct
{
	int m_iPicCnt;								//当前的预览画面数
} TGuiPreviewMultiPicRsp;

#define GUI_PREVIEW_PAGE_UP		202			//预览上一页
typedef struct
{
	int m_iDevId;
} TGuiPreviewPageUpReq;					//预览上翻页

#define GUI_PREVIEW_PAGE_DOWN	203			//预览下一页
typedef struct
{
	int m_iDevId;
} TGuiPreviewPageDownReq;					//预览下翻页

#define GUI_PREVIEW_CHG_CHN_REQ	250			//临时更改画面显示的视频输入通道请求
#define GUI_PREVIEW_CHG_CHN_RSP	-250		//临时更改画面显示的视频输入通道回应
typedef struct									//临时更改预览画面的视频输入通道请求
{
	int m_iDevId;								//设备编号dxl 2009/11/30
	int m_iVoChn;								//vo画面编号(1-16)
	int m_iViChn;								//vi编号(0-16, 0表示查询当前画面的当前视频输入通道)
} TGuiPreviewChgChnReq;

typedef struct									//临时更改预览画面的视频输入通道回应
{
	int m_iDevId;									//设备编号
	int m_iVoChn;								//vo画面编号(1-16)
	int m_iViChn;								//当前vo对应的vi编号
} TGuiPreviewChgChnRsp;

#define GUI_PREVIEW_SND_BIND_MSG	252			//预览声音绑定到VO通知
typedef struct									//预览声音绑定
{
	int m_iDevId;									//显示设备编号
	int m_iVoChn;								//vo通道号(1-16, 0表全部???)
} TGuiPreviewSoundBindMsg;

#define GUI_SET_PREVIEW_EXCHANGE_REQ  249     //设置交换两视频画面请求
#define GUI_SET_PREVIEW_EXCHANGE_RSP  -249     //设置交换两视频画面回应

typedef struct
{
	int m_iDevId;
	int m_iSrcVoChn;       //vo交换源通道号 从1开始
	int m_iDstVoChn;       //vo交换目的通道号 从1开始
}TGuiSetPreviewExchangeReq;   //设置交换两视频画面请求

#define GUI_SET_PREVIEW_EXCHANGE_OK  0
#define GUI_SET_PREVIEW_EXCHANGE_ERR 1
typedef struct
{
	int m_iRetCode;
}TGuiSetPreviewExchangeRsp;  //设置交换两视频画面回应

//------------------------------ 2.本地显示 ----------------------------

#define GUI_LAN_DEFAULT		0				//默认语言
#define GUI_LAN_CHI			1				//简体中文
#define GUI_LAN_ENG			2				//英文
#define GUI_LAN_TCHI		3				//繁体中文
#define GUI_LAN_KOR			4				//韩文
#define GUI_LAN_SPAN		5				//西班牙文
#define GUI_LAN_ITA			6				//意大利文
typedef struct
{
	int m_iDspMode;							//制式,PAL/NTSC
	int m_iTimeDspMode;						//时间显示格式，0表示不显示
	int m_iGuiTrnsRate;						//菜单透明度0-100，值大越透明
	int m_iVgaMode;							//低16位vga显示模式  高16位 HDMI模式
	int m_iSoundFlg;							//声音使能开关
	int m_iAlarmStaDspFlg;					//报警状态显示开关
	int m_iVedioRcdStaDspFlg;					//录像状态显示开关
	int m_iGuiTimeOut;							//图形界面超时设置,0表不超时
	int m_iGuiLang;							//语言
	int m_iStyle;								//样式
	int m_iDeflicker;							//抗闪砾
	int m_iMainportDetect;					//主口检测
	int m_iLinkSPicInterval;          		//报警联动单画面切换时间间隔
	int m_iAutoHidenRecLed;						//是否隐藏录像状态灯,1隐藏,0不隐藏
	int m_iHdmiDspMode;                     //HDMI 显示模式0-Auto , 1-YCBCR, 2-sRGB
	int m_iLcdScreenTmOut;					//ATM液晶关屏时间
	int m_iBrightness;						//液晶亮度调节
	int m_iAudioWaveState;					//是否显示音频波形,1显示,0不显示
	int m_iShowCblMsg;					//是否显示刻录信息，1显示，0不显示
	int m_iSizeAuto;						//分辨率自适应，1选中，0不选
	int m_iMouseSpeed;					//鼠标移动速度0-100
} TGuiDisplayCnf;

typedef struct									//获取本地显示参数回应
{
	int m_iVideoMode;							//视频输出制式, PAL/NTSC
} TGuiGetDisplayCnfRsp;

typedef struct									//设置本地显示参数请求
{
	int m_iVideoMode;							//视频输出制式, PAL/NTSC
} TGuiSetDisplayCnfReq;

#define GUI_SET_DSP_CNF_OK	0				//设置本地显示参数成功
#define GUI_SET_DSP_CNF_ERR	1				//失败
typedef struct									//设置本地显示参数回应
{
	int m_iRetCode;
} TGuiSetDisplayCnfRsp;

#define GUI_GET_MASTER_DEV_REQ  212         //获取主设备号请求
#define GUI_GET_MASTER_DEV_RSP  -212        //获取主设备号回应
typedef struct
{
	int m_iMasterDev;
}TGuiGetMasterDevRsp; 

//=========TV调节======
#define GUI_GET_DEV_TV_MSG_REQ   256      	//获取设备TV调节参数信息请求
#define GUI_GET_DEV_TV_MSG_RSP   -256		//获取设备TV调节参数信息回应

typedef struct
{
	int m_iVoDevNo;		//输出设备号
	int m_iTop;			//上边距(范围:0~100)
	int m_iBottom;		//下边距(范围:0~100)
	int m_iLeft;		//左边距(范围:0~100)
	int m_iRight;		//右边距(范围:0~100)
}TGuiDevTVMsg;

typedef struct
{
	int m_iVoDevNo;		//输出设备号 默认是BNC设备 GUI_BNC_DEV
}TGuiGetDevTvMsgReq;						//获取设备TV调节参数信息请求

typedef struct
{
	TGuiDevTVMsg m_tTvMsg;
}TGuiGetDevTvMsgRsp;						//获取设备TV调节参数信息回应


#define GUI_SET_DEV_TV_MSG_REQ   257      	//设置设备TV调节参数信息请求
#define GUI_SET_DEV_TV_MSG_RSP   -257		//设置设备TV调节参数信息回应
typedef struct
{
	TGuiDevTVMsg m_tTvMsg;
}TGuiSetDevTvMsgReq;         		//设置设备TV调节参数信息请求

typedef struct
{
	int m_iVoDevNo;
	int m_iRetCode;
}TGuiSetDevTvMsgRsp;   			//设置设备TV调节参数信息回应


//-------------------------预览裁边-----------------------------------------------
#define GUI_GET_PREVIEW_CUT_MSG_REQ		213		//获取视频预览裁边信息请求
#define GUI_GET_PREVIEW_CUT_MSG_RSP		-213		//获取视频预览裁边信息回应

typedef struct					//视频裁边
{
	int m_iEnable;					//是否启用预览裁边
	int m_iTop;					//千分比上部
	int m_iBottom;				//千分比底部
	int m_iLeft;					//千分比左边
	int m_iRight;					//千分比右边
} TGuiPreviewCutMsg;

typedef struct						//获取视频预览裁边信息请求
{
	int m_iChn;					//通道号
}TGuiGetPreviewCutMsgReq;

typedef struct						//获取视频预览裁边信息回应
{
	int m_iChn;
	TGuiPreviewCutMsg m_tPreviewCutMsg;	//裁剪千分比信息	
}TGuiGetPreviewCutMsgRsp;

#define GUI_SET_PREVIEW_CUT_MSG_REQ		214		//设置视频预览裁边信息请求
#define GUI_SET_PREVIEW_CUT_MSG_RSP		-214		//设置视频预览裁边信息回应

typedef struct						//设置视频预览裁边请求
{
	int m_iChn;							//通道号	
	TGuiPreviewCutMsg m_tPreviewCutMsg;	//裁剪千分比信息		
}TGuiSetPreviewCutMsgReq;

#define IP_CHN_NOT_ENABLE  			2				//数字通道未配置或禁用
#define IP_CHN_NOT_VIDEO_CONTECT  	3				//数字通道没有视频连接
#define IP_CHN_NOT_INIT    			4				//数字通道初始化错误

typedef struct						//设置视频预览裁边回应
{
	int m_iRetCode;					//返回码,0:成功2/3/4失败，失败说明如上
}TGuiSetPreviewCutMsgRsp;

//------------------------------ 3.视频输入参数 --------------------------

typedef struct			//获取视频输入通道制式请求
{
	int m_iChn;			//通道号,1 - 16
} TGuiGetVideoInNorReq;

typedef struct			//获取视频输入通道制式回应
{
	int m_iNor;			//制式, GUI_PAL_MODE / GUI_NTSC_MODE
 } TGuiGetVideoInNorRsp;

typedef struct
{
	int m_iChn;
	int m_iNor;
} TGuiSetVideoInNorReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetVideoInNorRsp;

#define GUI_VIVEOIN_TM_CNF_CNT		5		//同一通道支持五个时段	

enum //彩转黑类型
{
	VIDEOIN_COLOR = 0,   		//彩色
	VIDEOIN_GRAY,				//黑白	
	VIDEOIN_INNER,      		//内同步
	VIDEOIN_OUTSIDE,	 		//外同步
	VIDEOIN_ALARM,       		//报警同步
};								

typedef struct									//视频输入参数时段信息
{
	int m_iBenginTm;							//开始时间，从当天00:00流过的秒
	int m_iEndTm;								//结束时间
	int m_iHue;									//色　调 0-100
	int m_iLum;									//亮　度 0-100
	int m_iContrast;							//对比度 0-100
	int m_iSaturation;							//饱和度 0-100
	// 2008-12-26 添加彩转黑
	int m_iGrayFlg;								//彩转黑类型(0:彩色, 1:灰色)
} TGuiVideoInCnf;

typedef struct									//获取视频输入参数请求
{
	int m_iChn;									//通道号
} TGuiGetVideoInCnfReq;

typedef struct									//获取视频输入参数回应
{
	int m_iChn;									//通道号, 1-16，0表示错误
	int m_iVideMode;							//视频输入制式,PAL/NTSC/自动检测
	TGuiVideoInCnf m_tCnf[GUI_VIVEOIN_TM_CNF_CNT]; //配置参数
} TGuiGetVideoInCnfRsp;

typedef struct									//设置视频输入参数请求
{
	int m_iChn;									//通道号
	int m_iVideMode;							//视频输入制式,PAL/NTSC
	TGuiVideoInCnf m_tCnf[GUI_VIVEOIN_TM_CNF_CNT]; //配置参数
} TGuiSetVideoInCnfReq;

//设置自动检测视频输入制式
typedef struct
{
	int m_iChn;						//0表示所有通道,其余通道号对应
} TGuiGetVideoInModeAutoCheckReq;

typedef struct
{
	int m_iChn;				
	int m_iViModeAutoCheck;
} TGuiGetVideoInModeAutoCheckRsp;	//获取视频输入自动检测回应


typedef struct
{
	int m_iChn;	
	int m_iViModeAutoCheck;			//视频输入制式自动检测,1表示开启,0表示关闭
} TGuiSetVideoInModeAutoCheckReq;

typedef struct
{
	int m_iRetCode;					//回应码,0:成功，1:失败
} TGuiSetVideoInModeAutoCheckRsp;


#define GUI_SET_VIDEOIN_CNF_OK	0			//设置视频输入参数成功
#define GUI_SET_VIDEOIN_CNF_ERR	1			//设置视频输入参数失败
typedef struct									//设置视频输入参数回应
{
	int m_iRetCode;
} TGuiSetVideoInCnfRsp;

typedef struct									//修改视频输入参数请求
{
	int m_iChn;									//通道编号
	int m_iVideoMod;							//视频输入制式
	int m_iHue;									//色　调 0-100
	int m_iLum;									//亮　度 0-100
	int m_iContrast;							//对比度 0-100
	int m_iSaturation;							//饱和度 0-100
	// 2008-12-26 添加彩转黑
	int m_iGrayFlg;
} TGuiChgVideoInCnfReq;

#define GUI_CHG_VIDEOIN_CNF_OK	0			//修改视频输入参数成功
#define GUI_CHG_VIDEOIN_CNF_ERR	1			//修改视频输入参数失败
typedef struct
{
	int m_iRetCode;
} TGuiChgVideoInCnfRsp;


//=============================编码参数=========================
// 4.视频编码参数
#define GUI_GET_VENCODE_CNF_REQ	230			//获取视频编码参数请求
#define GUI_GET_VENCODE_CNF_RSP	-230		//获取视频编码参数回应
#define GUI_SET_VENCODE_CNF_REQ	231			//设置视频编码参数请求
#define GUI_SET_VENCODE_CNF_RSP	-231		//设置视频编码参数回应


#define GUI_VIDEO_FULL_FRAME_RATE			2009	//视频全帧率

// 因GUI_VIDEO_BIT_STREAM_STATIC定义有差别，
// 判断定码率以!= GUI_VIDEO_BIT_STREAM_DYNAMIC为准
#define GUI_VIDEO_BIT_STREAM_DYNAMIC		1	//变码率
#define GUI_VIDEO_BIT_STREAM_STATIC			2	//定码率老定义,注意兼容
#define GUI_VIDEO_BIT_STREAM_STATIC_NEW		0	//定码率 dxl 2011/02/23

// 因GUI_VIDEO_AUDIO定义有差别，判断复合码流以
// != GUI_VIDEO_ONLY为准
#define GUI_VIDEO_ONLY						1	//纯视频
#define GUI_VIDEO_AUDIO						2	//复合音频	老定义,注意兼容			
#define GUI_VIDEO_AUDIO_NEW					3	//复合音频	
#define GUI_AUDIO_ONLY						20

//编码模式
enum
{
	GUI_ENCODE_MODE_BASELINE_PROFILE = 0,
	GUI_ENCODE_MODE_MAIN_PROFILE,
	GUI_ENCODE_MODE_HIGH_PROFILE,
};

typedef enum//参数类型//不加typedef hismall编不过，by qiaohaijun @2013.12.04
{
	GUI_MAIN_CHANNEL_INSTANT = 0, //即时值
	GUI_MAIN_CHANNEL_TIME,    	//主码流（定时）
	GUI_MAIN_CHANNEL_INCIDENT,      //主码流（事件）	
	GUI_SUB_CHANNEL,          		//副码流
	GUI_MAIN_CHANNEL_TIME_TEMP1,    //主码流（定时模板1）
	GUI_MAIN_CHANNEL_TIME_TEMP2,     //主码流（定时模板2）
}TGuiVTempType; 

typedef struct									//视频编码参数
{
	int m_iEncodeType;							//低16位视频编码方式H264, 高16位为编码模式 
	int m_iPicSize;							//分辨率编号
	int m_iFrameRate;							//视频帧率
	int m_iBitStreamType;						//码流类型
	int m_iBitRate;							//码流大小(上限):kbps
	int m_iPicQualityLv;						//画质级别
	int m_iIFrameRate;							//I帧帧率
	int m_iComplexType;						//音视频复合类型
	int m_iAudioEncodeType;					//音频编码方式
} TGuiVideoEncodeCnf;

typedef struct					//获取视频编码参数请求
{
	int m_iChn;					//通道号(扩展参数类型，高16位说明见参数类型TGuiVTempType)
} TGuiGetVideoEncodeCnfReq;

typedef struct					//获取视频编码参数回应
{
	int m_iChn;					//通道号(扩展参数类型，高16位说明见参数类型TGuiVTempType)
	TGuiVideoEncodeCnf m_tCnf;	//配置信息
} TGuiGetVideoEncodeCnfRsp;

// 设置视频编码参数请求
typedef TGuiGetVideoEncodeCnfRsp TGuiSetVideoEncodeCnfReq;

#define GUI_SET_VENCODE_CNF_OK		0		//设置成功
#define GUI_SET_VENCODE_CNF_ERR		1		//设置失败
typedef struct									//设置视频编码参数回应
{
	int m_iRetCode;
} TGuiSetVideoEncodeCnfRsp;

//------------------------------音频采样率--------------------------------
#define GUI_GET_AUDIO_RATE_CNF_REQ    236    //获取音频采样率请求
#define GUI_GET_AUDIO_RATE_CNF_RSP 	 -236    //获取音频采样率回应

typedef struct			//获取音频采样率参数请求
{
	int m_iChn;			//通道号
} TGuiGetAudioRateCnfReq;

typedef struct			//获取音频采样率参数回应
{
	int m_iAudioRate;	//音频采样率8000:8K;32000:32K;48000:48K
} TGuiGetAudioRateCnfRsp;

#define GUI_SET_AUDIO_RATE_CNF_REQ   237     //设置音频采样率请求
#define GUI_SET_AUDIO_RATE_CNF_RSP  -237     //设置音频采样率回应

#define GUI_SET_AUDIO_RATE_CNF_OK   0		 //设置成功
#define GUI_SET_AUDIO_RATE_CNF_ERR  1		 //设置失败

typedef struct			//设置音频采样率参数请求
{
	int m_iChn;	//通道号
	int m_iAudioRate;	//音频采样率8:8K;32:32K;48:48K
} TGuiSetAudioRateCnfReq;

typedef struct			//设置音频采样率参数回应
{
	int m_iRetCode;		//返回码，0表示成功
} TGuiSetAudioRateCnfRsp;

#define GUI_GET_CHN_AUDIO_RATE_LST_REQ    238  //获取每个通道的音频采样率列表请求
#define GUI_GET_CHN_AUDIO_RATE_LST_RSP    -238  //获取每个通道的音频采样率列表回应

typedef struct
{
	int m_iChn;   //请求通道
}TGuiGetChnAudioRateLstReq;

typedef struct
{
	int m_iChn;
	int m_iCnt;   //返回请求通道支持几种音频采样率
}TGuiGetChnAudioRateLstRsp;

typedef struct
{
	int m_iAudioRate;  //音频采样率
}TGuiAudioRateItem;

//------------------------音频编码方式-------------------------------
#define GUI_GET_CHN_AUDIO_CODETYPE_LST_REQ  239   //获取通道音频编码方式列表请求
#define GUI_GET_CHN_AUDIO_CODETYPE_LST_RSP  -239  //获取通道音频编码方式列表回应

typedef struct
{
	int m_iChn;
}TGuiGetChnAudioCodeTypeLstReq;   //获取通道音频编码方式列表请求

typedef struct
{
	int m_iChn;
	int m_iCnt;
}TGuiGetChnAudioCodeTypeLstRsp;   //获取通道音频编码方式列表回应

typedef struct
{
	int m_iAudioCodeType;
}TGuiAudioCodeTypeItem;  //音频编码方式

//------------------------------ 4.视频前端叠加 --------------------------
typedef struct
{
	int m_iXp;
	int m_iYp;
} TGuiLoc;

typedef struct
{
	int m_iTop;
	int m_iLeft;
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
} TGuiArea;

#define GUI_CHN_NAME_LEN		32				//通道名最大长度
#define GUI_CHN_TEXT_LEN		255				//自定义文本最大长度
#define GUI_MAX_COVER_AREA		4

typedef struct									//视频叠加参数
{
	int m_iNameOverlayEnable;					//通道名叠加使能标识
	char m_strName[GUI_CHN_NAME_LEN + 1];		//通道名
	int m_iChnColor;                            //通道颜色                          
	TGuiLoc m_tNameLoc;						//通道名叠加位置
	
	int m_iTimeOverlayEnable;					//时间叠加使能标识
	int m_iTimeFormat;							//时间格式,低八位代表format，第九位代表是否叠加星期，第十位为0代表24时制、为1代表12时制
	int m_iTimeColor;                           //叠加时间颜色
	TGuiLoc m_tTimeLoc;						//时间叠加位置
	
	int m_iTextEnable;							//自定义文本叠加使能
	char m_strText[GUI_CHN_TEXT_LEN + 1];		//自定义文本
	TGuiLoc m_tTextLoc;						//文本叠加位置

	int m_iLogoEnable;							//Logo叠加使能标识
	TGuiLoc m_tLogoLoc;						//Logo叠加位置

	int m_iVideoCoverEnable;					//视频遮档使能标识
	int m_iCoverAreaCnt;						//视频遮档区域已启用的个数
	TGuiArea m_tCoverAreas[GUI_MAX_COVER_AREA]; //视频遮档区域
} TGuiVideoOverlayCnf;

typedef struct									//获取视频叠加参数请求
{
	int m_iChn;									//通道号
} TGuiGetVideoOverlayCnfReq;

typedef struct									//获取视频叠加参数回应
{
	int m_iChn;									//通道号
	TGuiVideoOverlayCnf m_tCnf;					//配置参数
} TGuiGetVideoOverlayCnfRsp;

typedef struct									//设置视频叠加参数请求
{
	int m_iChn;									//通道号
	TGuiVideoOverlayCnf m_tCnf;					//配置参数							
} TGuiSetVideoOverlayCnfReq;

#define GUI_SET_VOVERLAY_CNF_OK	0			//设置成功
#define GUI_SET_VOVERLAY_CNF_ERR	1			//设置失败
typedef struct									//设置视频叠加参数回应
{
	int m_iRetCode;							//返回码
} TGuiSetVideoOverlayCnfRsp;

typedef struct									//更换logo图标请求
{
	int m_iChn;									//通道号
} TGuiChgLogoBmpReq;

#define GUI_CHG_LOGO_BMP_OK		0			//更换图标成功
#define GUI_CHG_LOGO_BMP_ERR		1			//更换图标失败
typedef struct									//更换logo图标回应
{
	int m_iRetCode;
} TGuiChgLogoBmpRsp;

//==============================录像设置 ===============================
//=======================假日计划========================
#define GUI_GET_HOLIDAY_MSG_REQ		296	 	//获取假日计划信息请求
#define GUI_GET_HOLIDAY_MSG_RSP		-296	//获取假日计划信息回应

#define GUI_SET_HOLIDAY_MSG_REQ		297		//设置假日计划信息请求
#define GUI_SET_HOLIDAY_MSG_RSP		-297	//设置假日计划信息回应

#define GUI_HOLIDAY_NAME_LEN	32
#define GUI_MAX_HOLIDAY_CNT		10

enum
{
	HOLIDAY_TYPE_WEEK = 1, //星期
	HOLIDAY_TYPE_MONTH,		//月份
	HOLIDAY_TYPE_DATE,		//日期
}; //假日类型

typedef struct
{
	int m_iId;   //假日编号
	char m_strHolidayName[GUI_HOLIDAY_NAME_LEN + 1];  //假日名称
	int m_iStatus;  //0表不开启，1表示开启
	int m_iType;   //假日类型，按照哪种类型，分为星期、月份、日期
	int m_iStartFirst;    //
	int m_iStartSecond;  //
	int m_iStartLast;   //
	int m_iEndFirst;
	int m_iEndSecond;
	int m_iEndLast;
}TGuiHolidayMsg;
/*如果类型是日期，则m_iStartFirst，m_iStartSecond，m_iStartLast表示开始时间的年月日
类型是星期，则m_iStartFirst，m_iStartSecond，m_iStartLast表示开始时间的第几个月、第几个星期，星期几；如果类型是月份，则m_iStartFirst，m_iStartSecond表示第几个月，几号，m_iStartLast，m_iEndLast不使用 */

typedef struct
{
	TGuiHolidayMsg m_tHolidayMsg[GUI_MAX_HOLIDAY_CNT];
}TGuiGetHolidayMsgRsp; //获取假日计划信息回应

typedef struct
{
	TGuiHolidayMsg m_tHolidayMsg[GUI_MAX_HOLIDAY_CNT];
}TGuiSetHolidayMsgReq;       //设置假日计划信息请求

#define GUI_SET_HOLIDAY_MSG_OK   0 //设置成功
#define GUI_SET_HOLIDAY_MSG_ERR	1	//设置失败
typedef struct
{
	int m_iRetCode;         //0表示成功，1表示失败
} TGuiSetHolidayMsgRsp; 	//设置假日计划信息回应

//=======================录像保留时间====================
#define GUI_GET_REC_REMAIN_TIME_REQ   298 	//获取录像文件保留时间请求
#define GUI_GET_REC_REMAIN_TIME_RSP   -298	//获取录像文件保留时间回应
#define GUI_SET_REC_REMAIN_TIME_REQ   299	//设置录像文件保留时间请求
#define GUI_SET_REC_REMAIN_TIME_RSP   -299	//设置录像文件保留时间回应

typedef struct
{
	int m_iChn;					//高16位 0-主码流1-副码流
}TGuiGetRecRemainTmReq;		//获取录像文件保留时间请求

typedef struct
{
	int m_iChn;				//高16位 0-主码流1-副码流
	int m_iTm;  			//单位：天数
}TGuiGetRecRemainTmRsp;	//获取录像文件保留时间回应

typedef struct
{
	int m_iChn;				//高16位 0-主码流1-副码流
	int m_iTm;	
}TGuiSetRecRemainTmReq;	//设置录像文件保留时间请求

#define GUI_SET_REC_REMAIN_TM_OK  0 //设置成功
#define GUI_SET_REC_REMAIN_TM_ERR 1	//设置失败
typedef struct
{
	int m_iRetCode;
}TGuiSetRecRemainTmRsp;	//设置录像文件保留时间回应




//=========================录像类型名==============================
//录像类型名
#define GUI_GET_VIDEO_TYPE_REQ 			390      	//录像类型请求
#define GUI_GET_VIDEO_TYPE_RSP			-390
#define GUI_SET_VIDEO_TYPE_REQ 			391      	//录像类型设置
#define GUI_SET_VIDEO_TYPE_RSP 			-391      //录像类型设置回应

#define GUI_VIDEO_TYPE_NAME_STR_LEN	16

typedef struct
{
	int m_iChnNum;       //类型号
	char m_strVideoType[GUI_VIDEO_TYPE_NAME_STR_LEN + 1];           //录像类型名
}TGuiVideoTypeName;

typedef struct
{
	int m_iRetCode;
}TGuiSetTypeRsp;

typedef struct
{
	int m_iTypeId;			//自定义录像类型号，32-63
	char m_strTypeName[GUI_VIDEO_TYPE_NAME_STR_LEN + 1];    //录像类型名
} TGuiGetVideoTypeReq;

typedef struct
{
	int m_iTypeId;			//自定义录像类型号，32-63，-1表获取失败
	char m_strTypeName[GUI_VIDEO_TYPE_NAME_STR_LEN + 1];    //录像类型名
} TGuiGetVideoTypeRsp;

typedef struct
{
	int m_iTypeId;			//自定义录像类型号，32-63
	char m_strTypeName[GUI_VIDEO_TYPE_NAME_STR_LEN + 1];    //录像类型名
} TGuiSetVideoTypeReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetVideoTypeRsp;

//=======================录像文件加锁=====================
#define GUI_VF_NAME_LEN		67				//录像文件名最大长度


#define GUI_GET_RECFILE_LOCK_STATUS_REQ		394   //获取录像文件锁状态请求
#define GUI_GET_RECFILE_LOCK_STATUS_RSP		-394  //获取录像文件锁状态回应

enum//锁状态
{
	RECFILE_UNLOCK = 0,  //0表示解锁
	RECFILE_LOCK,        //1表加锁
};

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
}TGuiGetRecFileLockStatusReq;	

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	int m_iAction;  //锁状态
} TGuiGetRecFileLockStatusRsp;

#define GUI_SET_RECFILE_LOCK_STATUS_REQ		395   //设置录像文件锁状态请求
#define GUI_SET_RECFILE_LOCK_STATUS_RSP		-395 //设置录像文件锁状态回应

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	int m_iAction;  //锁状态
} TGuiSetRecFileLockStatusReq;

#define GUI_SET_RECFILE_LOCK_STATUS_OK	 0  //设置成功
#define GUI_SET_RECFILE_LOCK_STATUS_ERR 1	//设置失败
typedef struct
{
	int m_iRetCode;   //0表成功，1表示失败
} TGuiSetRecFileLockStatusRsp;



//=========================录像模板=================================

// 录像类型码
#define GUI_VR_TYPE_ALL					0		
#define GUI_VR_TYPE_HUM					1		//手动录像
#define GUI_VR_TYPE_ALARM				2		//报警录像
#define GUI_VR_TYPE_TIME				3		//定时录像
#define GUI_VR_TYPE_MD					4		//移动侦测录像
#define GUI_VR_TYPE_LOST				5		//视频丢失录像
#define GUI_VR_TYPE_MD_EX				6		//移动侦测|报警录像
#define GUI_VR_TYPE_ALOST				7		//音频丢失报警
#define GUI_VR_TYPE_OD					8	    //视频遮挡报警

#define GUI_VR_TYPE_VCA                 32       //智能分析| 报警录像  
#define GUI_VR_TYPE_TEMP				33		//温湿度报警录像
#define GUI_VR_TYPE_ATM					34		//ATM 卡号查询


typedef struct									//录像时段模板
{
	int m_iEnable;								//该录像时段是否启用
	int m_iStartTm;							//录像开始时间,以当天00:00起流过的秒数
	int m_iEndTm;								//录像结束时间
	int m_iType;								//录像类型
} TGuiVideoRecTmTmp;

#define GUI_VR_TM_CNT_PER_DAY		4			//每天最大可设时段
typedef struct									//录像日期模板
{
	TGuiVideoRecTmTmp m_tRecTm[GUI_VR_TM_CNT_PER_DAY];
} TGuiVideoRecDayTmp;

typedef struct									//录像模板
{
	int m_iPreRcdTm;							//预录时长，以秒计
	int m_iDelayTm;							//延时时长，以秒计
	TGuiVideoRecDayTmp m_tRecDay[7];			//一周七天录像参数
} TGuiVideoRecTemplate;

typedef struct									//获取录像模板请求
{
	int m_iChn;									//录像通道
} TGuiGetVideoTemplateReq;

typedef struct									//获取录像模板回应
{
	int m_iChn;									//通道号
	TGuiVideoRecTemplate m_tTmp;				//通道对应的录像模板
} TGuiGetVideoTemplateRsp;

typedef struct									//设置录像模板请求
{
	int m_iChn;
	TGuiVideoRecTemplate m_tTmp;				//通道对应的录像模板
} TGuiSetVideoTemplateReq;

#define GUI_SET_VR_TEMPLATE_OK		0		//设置录像模板成功
#define GUI_SET_VR_TEMPLATE_ERR		1		//设置录像模板失败
typedef struct									//设置录像模板回应
{
	int m_iRetCode;
} TGuiSetVideoTemplateRsp;

#define GUI_SET_AUDIOREC_TEMPLATE_REQ   302  //录像模板中设置记录音频请求
#define GUI_SET_AUDIOREC_TEMPLATE_RSP   -302  //录像模板中设置记录音频回应
typedef struct
{
	int m_iChn;
	int m_iAudioRec; // 1-记录 0 -不记录
}TGuiSetAudioRecTemplateReq;
typedef struct
{
	int m_iRetCode;
}TGuiSetAudioRecTemplateRsp;

#define GUI_GET_AUDIOREC_TEMPLATE_REQ   303  //录像模板中获取记录音频请求
#define GUI_GET_AUDIOREC_TEMPLATE_RSP   -303  //录像模板中获取记录音频回应
typedef struct
{
	int m_iChn;
}TGuiGetAudioRecTemplateReq;
typedef struct
{
	int m_iChn;
	int m_iAudioRec; // 1-记录 0 -不记录
}TGuiGetAudioRecTemplateRsp;


#define GUI_GET_BACKUPREC_TEMPLATE_REQ   304	//录像模板中获取冗余录像请求
#define GUI_GET_BACKUPREC_TEMPLATE_RSP   -304	//录像模板中获取冗余录像回应

typedef struct
{
	int m_iChn;
}TGuiGetBackUpRecTemplateReq;

typedef struct
{
	int m_iChn;
	int m_iBackUpRec; // 1-冗余录像0 -不冗余录像
}TGuiGetBackUpRecTemplateRsp;

#define GUI_SET_BACKUPREC_TEMPLATE_REQ   305  //录像模板中设置冗余录像请求
#define GUI_SET_BACKUPREC_TEMPLATE_RSP   -305  //录像模板中设置冗余录像回应

#define GUI_SET_BR_TEMPLATE_OK		0		//设置录像模板成功
#define GUI_SET_BR_TEMPLATE_ERR		1		//设置录像模板失败
typedef struct
{
	int m_iChn;
	int m_iBackUpRec; // 1-冗余录像0 -不冗余录像
}TGuiSetBackUpRecTemplateReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetBackUpRecTemplateRsp;

//===================假日计划录像模板=========================
#define GUI_GET_HOLIDAY_VR_TEMPLATE_REQ	  	306		//获取假日计划录像模板请求
#define GUI_GET_HOLIDAY_VR_TEMPLATE_RSP		-306	//获取假日计划录像模板回应

typedef struct
{
	int m_iPreRcdTm;							//预录时长，以秒计
	int m_iDelayTm;							//延时时长，以秒计
	TGuiVideoRecDayTmp m_tRecDay;			//一周七天录像参数	
}TGuiHolidayRecTemplate;

typedef struct
{
	int m_iChn;
}TGuiGetHolidayTempReq;		//获取假日计划录像模板请求

typedef struct
{
	int m_iChn;
	TGuiHolidayRecTemplate m_tHolidayTemp;
}TGuiGetHolidayTempRsp;	//获取假日计划录像模板回应

#define GUI_SET_HOLIDAY_VR_TEMPLATE_REQ		307		//设置假日计划录像模板请求
#define GUI_SET_HOLIDAY_VR_TEMPLATE_RSP		-307	//设置假日计划录像模板回应

typedef struct
{
	int m_iChn;
	TGuiHolidayRecTemplate m_tHolidayTemp;
}TGuiSetHolidayTempReq;		//设置假日计划录像模板请求

#define GUI_SET_HOLIDAY_TEMP_OK   0  //设置成功
#define GUI_SET_HOLIDAY_TEMP_ERR  1	 //设置失败
typedef struct
{
	int m_iRetCode;
}TGuiSetHolidayTempRsp;		//设置假日计划录像模板回应

//=======================抽帧录像===============================
#define GUI_GET_TAKEIFRAME_TEMPLATE_REQ		308		//获取是否抽帧录像请求
#define GUI_GET_TAKEIFRAME_TEMPLATE_RSP		-308	//获取是否抽帧录像回应

typedef struct
{
	int m_iChn;
}TGuiGetTakeIFrameTemplateReq;		//获取是否抽帧录像请求

typedef struct
{
	int m_iChn;
	int m_iTakeIFrame;	 // 0 -不抽帧录像 1-抽帧录像
}TGuiGetTakeIFrameTemplateRsp;		//获取是否抽帧录像回应

#define GUI_SET_TAKEIFRAME_TEMPLATE_REQ		309		//设置是否抽帧录像请求
#define GUI_SET_TAKEIFRAME_TEMPLATE_RSP		-309	//设置是否抽帧录像回应

#define GUI_SET_TAKEIFRAME_TEMPLATE_OK		0		//设置录像模板成功
#define GUI_SET_TAKEIFRAME_TEMPLATE_ERR		1		//设置录像模板失败
typedef struct
{
	int m_iChn;
	int m_iTakeIFrame; //0 -不抽帧录像  1-抽帧录像
} TGuiSetTakeIFrameTemplateReq;		//设置是否抽帧录像请求

typedef struct
{
	int m_iRetCode;
} TGuiSetTakeIFrameTemplateRsp;		//设置是否抽帧录像回应

//=================================
#define GUI_GET_SUBSTREAMREC_TEMPLATE_REQ		312		//获取是否副码流录像请求
#define GUI_GET_SUBSTREAMREC_TEMPLATE_RSP		-312	//获取是否副码流录像回应

typedef struct
{
	int m_iChn;
}TGuiGetSubStreamRecTemplateReq;		//获取是否开启副码流录像请求

typedef struct
{
	int m_iChn;
	int m_iSubStreamRec;	 			// 0 -不启用副码流录像 1-启用(见公共宏定义)
}TGuiGetSubStreamRecTemplateRsp;		//获取是否副码流录像回应

//=================================
#define GUI_SET_SUBSTREAMREC_TEMPLATE_REQ		313		//设置是否副码流录像请求
#define GUI_SET_SUBSTREAMREC_TEMPLATE_RSP		-313	//设置是否副码流录像回应

typedef struct
{
	int m_iChn;
	int m_iSubStreamRec; 		// 0 -不启用副码流录像 1-启用(见公共宏定义)
} TGuiSetSubStreamRecTemplateReq;		//设置是否副码流录像请求

typedef struct
{
	int m_iRetCode; 
} TGuiSetSubStreamRecTemplateRsp;		//设置是否副码流录像回应

//=================================

#define GUI_GET_VR_SMART_TEMPLATE_REQ	314			//获取智能录像模板请求
#define GUI_GET_VR_SMART_TEMPLATE_RSP	-314		//获取智能录像模板回应
#define GUI_SET_VR_SMART_TEMPLATE_REQ	315			//设置智能录像模板请求
#define GUI_SET_VR_SMART_TEMPLATE_RSP	-315		//设置智能录像模板回应

#define GUI_VR_TM_DAY_CNT  8

enum
{
	TEMPLATE_DISABLE,  //模板不生效
	TIME_TEMPLATE_1,   //定时模板1
	TIME_TEMPLATE_2,   //定时模板2
};

typedef  struct
{
	int m_iChn;
}TGuiGetSmartTemplateReq;

//iTemplateType[x][x] 的值枚举参考编码类型，默认为-1 
typedef struct
{
	int iTemplateType[GUI_VR_TM_DAY_CNT] [GUI_VR_TM_CNT_PER_DAY];   //每个通道对应每天对应4个时间段，每个时间段一个模板，按下表索引
}TGuiVideoRecSmartTemplate;


typedef struct
{
	int m_iChn;
	TGuiVideoRecSmartTemplate m_tSmartTempCnf;
} TGuiGetRecSmartTemplateRsp;  //获取智能录像模板回应

typedef struct
{
	int m_iChn;
	TGuiVideoRecSmartTemplate m_tSmartTempCnf;
} TGuiSetRecSmartTemplateReq; //设置智能录像模板请求

//0表成功 1表失败
typedef struct
{
	int m_iRetCode;
} TGuiSetRecSmartTemplateRsp; //设置智能录像模板回应




//===================录像策略===================================


#define GUI_VRF_ENAME_LEN				5		//录像文件扩展名长度
#define GUI_VRF_PACK_BY_TIME			2		//按时间长度打包
#define GUI_VRF_PACK_BY_SIZE			1		//按文件大小打包

// 磁盘空间不足时动作
#define GUI_DEL_VRF_WHEN_DOS			1		//删除文件
#define GUI_DEL_NA_VRF_WHEN_DOS			2		//删除非报警录像文件
#define GUI_STOP_VR_WHEN_DOS			3		//停止录像

typedef struct									//录像策略参数
{
	char m_strRxtName[GUI_VRF_ENAME_LEN + 1];	//录像文件扩展名
	int m_iFilePackType;						//录像文件打包方式
	int m_iFilePackTime;						//打包时间(单位秒)
	int m_iFilePackSize;						//者打包大小(单位K)
	int m_iActionWhenDiskOutOfSpace;			//磁盘空间不足时的动作(编号1到3)
} TGuiVideoStrategy;

typedef struct									//获取录像策略回应
{
	TGuiVideoStrategy m_tStrategy;
} TGuiGetVideoStrategyRsp;

typedef struct									//设置录像策略请求
{
	TGuiVideoStrategy m_tStrategy;
} TGuiSetVideoStrategyReq;

#define GUI_SET_VR_STRATEGY_OK		0			//设置录像策略成功
#define GUI_SET_VR_STRATEGY_ERR		1			//设置录像策略失败
typedef struct									//设置录像策略回应
{
	int m_iRetCode;
} TGuiSetVideoStrategyRsp;


// 手动录像
#define GUI_START_HUM_VIDEO		1			//启动手动录像
#define GUI_STOP_HUM_VIDEO		2			//停止手动录像
typedef struct									//手动录像命令
{
	int m_iChn;									//通道号(0-16, 0表示针对全部通道)
	int m_iCmd;									//手动录像命令:启动/停止
} TGuiHumRecVideoCmd;

//定义的新的有回应的手动录像命令协议
#define GUI_HUM_REC_VIDEO_CMD_REQ  352            //手动录像命令
#define GUI_HUM_REC_VIDEO_CMD_RSP  -352            //手动录像命令
typedef struct
{
	int m_iChn;              //通道号(0-16, 0表示针对全部通道)
	int m_iCmd;              //手动录像命令:启动/停止 GUI_START_HUM_VIDEO /GUI_STOP_HUM_VIDEO
}TGuiHumRecVideoCmdReq;    //手动录像命令请求

#define GUI_HUM_REC_VIDEO_CMD_OK   0 //命令执行成功
#define GUI_HUM_REC_VIDEO_CMD_ERR  1 //命令执行失败
typedef struct
{
	int m_iRetCode;
}TGuiHumRecVideoCmdRsp;   //手动录像命令回应

// 通用录像
#define GUI_START_REC_CMD		1
#define GUI_STOP_REC_CMD		2
typedef struct
{
	int m_iChn;					//通道号(0表全部,1-16表通道)
	int m_iCmd;					//开始/停止录像
	int m_iType;				//录像类型
} TGuiRecReq;

typedef struct
{
	int m_iRetCode;				//返回码, 0成功,其他失败
} TGuiRecRsp;

// 录像文件
//月照
typedef struct									//获取录像文件快照请求
{
	int m_iChn;     //查询的通道 
	int m_iYear;	//年 >= 2008
	int m_iMon;		//月: 1-12
} TGuiGetVideoFileMapReq;

typedef struct
{
	int m_iChn;
	int m_iYear;
	int m_iMon;
	int m_iFileMap[32];	//0号位置保留，需填充0, 1-31号位置存放对应日期是否有录像标志:0无，1有
} TGuiGetVideoFileMapRsp;

//日照
#define GUI_GET_VIDEO_FILE_DAY_MAP_REQ  319     //获取录像文件按天快照请求(1天)
#define GUI_GET_VIDEO_FILE_DAY_MAP_RSP  -319    //获取录像文件按天快照回应(1天)

#define GUI_MAX_VIDEO_SECT_PER_DAY 300  //每个通道最大300个时间段
typedef struct
{
	int m_iChn;
	int m_iYear;
	int m_iMon;
	int m_iDay;
}TGuiGetVideoFileDayMapReq;

typedef struct
{
	int m_iChn;
	int m_iYear;
	int m_iMon;
	int m_iDay;
	int m_iCnt;  //录像个数
}TGuiGetVideoFileDayMapRsp;

typedef struct
{
	int m_iType; //录像类型
	int m_iStartTime;
	int m_iEndTime;
}TGuiGetVideoFileDayMapMsg;  //录像文件信息

//====================================
//录像查询
#define GUI_QUERY_VIDEO_FILE_REQ	321			//查询录像文件请求
#define GUI_QUERY_VIDEO_FILE_RSP	-321		//查询录像文件回应

typedef struct									//录像文件查询条件
{
	int m_iChn;									//通道号 高16位0-主码流1-副码流
	int m_iDev;									//0~7位表ATM操作类型，8~15ATM异常，16~23ATM端口号34~31预留
	int m_iType;								//录像文件类型
	int m_iTypeRange;                        //录像范围
	int m_iBeginTm;							//开始时间
	int m_iEndTm;								//结束时间
} TGuiVideoFileQueryCondition;

typedef struct									//统计录像文件请求
{
	TGuiVideoFileQueryCondition m_tCondition;	//查询条件
} TGuiCountVideoFileReq;

typedef struct									//统计录像文件回应
{
	int m_iVideoFileCnt;						//录像文件总个数
} TGuiCountVideoFileRsp;

typedef struct									//查询录像文件请求
{
	TGuiVideoFileQueryCondition m_tCondition;	//查询条件
	int m_iPageId;								//页码(第一页为0)
	int m_iRcntPerPage;						//每页记录条数
} TGuiQueryVideoFileReq;

typedef struct									//查询录像文件回应
{
	int m_iCnt;									//次此查得录像文件个数
} TGuiQueryVideoFileRsp;						//随后发送相应个数的录像文件源料
//====================================


typedef long long TGuiVideoFileId;			//录像文件编号类型
typedef struct									//录像文件资料
{
	#if 0 //文件编号未用
	TGuiVideoFileId m_tFileId;					//录像文件编号
	#endif
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	unsigned int m_uiFileSize;					//录像文件大小
	int m_iBeginTm;							//录像文件开始时间
	int m_iEndTm;								//录像文件结束时间
	int m_iChn;									//录像文件对应的通道号
	int m_iDev;									//录像文件对应的设备号
	int m_iType;								//录像文件类型
} TGuiVideoFileMsg;

typedef struct									//备份录像文件请求
{
	#if 0
	TGuiVideoFileId m_tFileId;					//录像文件编号
	#endif
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	int m_iDstDevId;							//目录设备号
} TGuiVideoFileBackupReq;

#define GUI_BACKUP_VIDEO_FILE_OK			0	//备份录像文件成功
#define GUI_BACKUP_VIDEO_FILE_ERR			1	//备份录像文件失败
#define GUI_BACKUP_VIDEO_FILE_NO_DEV		2	//找不到设备错误
#define GUI_BACKUP_VIDEO_FILE_NO_SPACE		3	//磁盘空间不足错误
typedef struct									//备份录像文件回应
{
	int m_iRetCode;
} TGuiVideoFileBackupRsp;

typedef struct									//删除录像文件请求
{
	#if 0 //文件编号未用
	TGuiVideoFileId m_tFileId;					//录像文件编号
	#endif
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名	
} TGuiVideoFileDeleteReq;

#define GUI_DEL_VIDEO_FILE_OK			0		//删除文件成功
#define GUI_DEL_VIDEO_FILE_ERR			1		//删除文件失败
typedef struct									//删除录像文件回应
{
	int m_iRetCode;							//返回码
} TGuiVideoFileDeleteRsp;

//重建索引
typedef struct									//重建索引回应
{
	int m_iRebuildId;							//重建索引标识号(小于0表拒绝请求，其他值唯一标识此次重建操作)
} TGuiRebuildIndexRsp;

typedef struct									//获取索引状态请求
{
	int m_iRebuildId;							//重建索引标识号
} TGuiGetRebuildIndexStaReq;

#define GUI_REBUILD_INDEX_ERR		0			//重建索引发生错误
#define GUI_REBUILD_INDEX_DOING		1			//重建索引进行中
#define GUI_REBUILD_INDEX_DONE		2			//重建索引完成
typedef struct									//获取索引状态回应
{
	int m_iState;								//状态码
	int m_iProgress;							//当前进度0-100(保留，如果不支持填充0)
} TGuiGetRebuildIndexStaRsp;

//录像回放
typedef struct									//录像回放请求
{
	int m_iVoId;								//VO通道号(暂时保留)
	char m_strFile[GUI_VF_NAME_LEN + 1];		//欲播放的录像文件
	int m_iSoundEnable;						//声音使能(0/1)
} TGuiVideoPlayReq;

#define GUI_VIDEO_PLAY_OK				0		//录像回放请求成功
#define GUI_VIDEO_PLAY_FILE_LOST		1		//录像文件不存在
#define GUI_VIDEO_PLAY_FILE_FERR		2		//无法打开或者播放指定的文件
typedef struct									//录像回放回应
{
	int m_iRetCode;							//返回码
	int m_iPlayId;								//录像回放编号，其后对该次播放的相关操作均
												//以此号标识
	long long m_llFrameCnt;					//总帧数
	int m_iFrameRate;							//帧率，每秒回放的帧数
} TGuiVideoPlayRsp;

typedef struct									//录像回放暂停请求
{
	int m_iPlayId;								//回放编号，由GuiVideoPlayRsp指定
} TGuiVideoPlayPauseReq;

typedef struct									//录像文件继播放请求
{
	int m_iPlayId;								//回放编号
} TGuiVideoPlayCntnReq;

typedef struct									//录像文件播放停止请求
{
	int m_iPlayId;
} TGuiVideoPlayStopReq;

typedef struct									//单帧播放(无回应)
{
	int m_iPlayId;								//播放编号
	int m_iType;								//步进类型(1:步进, -1步退)
} TGuiVideoPlayStepCmd;

typedef struct									//播放定位请求(无回应)
{
	int m_iPlayId;								//播放编号
	int m_iLoc;									//定位百分比(0-100)
} TGuiVideoPlayLocCmd;

//========================录像倒放====================
#define GUI_VIDEO_PLAY_REVERSE_REQ	            348    //倒放
typedef struct									//录像回放倒放请求
{
	int m_iPlayId;								//回放编号，由GuiVideoPlayRsp指定
} TGuiVideoPlayReverseReq;

//========================录像回放中抓拍=============
#define GUI_VIDEO_PLAY_SNAP_REQ					349    //回放抓拍
typedef struct									//录像回放抓拍请求
{
	int m_iPlayId;								//回放编号，由GuiVideoPlayRsp指定
} TGuiVideoPlaySnapReq;

//-----------------------ATM录像查询定位-----------------------------------------------
//ATM 根据操作时间定位录像
#define GUI_PLAY_VIDEO_LOCEX_REQ		345		//按绝对时间定位
typedef struct
{
	int m_iPlayId;				//播放编号
	int m_iAbsLocTm;			//具体时间
}TGuiVideoPlayLocCmdEx; //按绝对时间定位

//卡号录像查询
#define GUI_QUERY_ATM_VIDEO_FILE_REQ		346
#define GUI_QUERY_ATM_VIDEO_FILE_RSP		-346

#define MAX_CRADNO_LEN		64
#define MAX_FILED_NUM       5   //5个字段

typedef enum
{
	QUERY_BY_CARDNO = 0,    //按卡号查询
	QUERY_BY_FIELD,			//按域查询
}TAtmQueryType;      //卡号录像查询方式，分为按卡号和按域查询

typedef enum
{
	ATM_OPERATE_ALL = 0,	//全部
	ATM_QUERY,				//查询
	ATM_TAKE,				//取款
	ATM_CHANGEPASSWD,		//改密
	ATM_TRANSFER,			//转账
	ATM_DEPOSIT,			//存款
	ATM_QUERY_NOCARD,		//无卡查询
	ATM_DEPOSIT_NOCARD,		//无卡存款
	ATM_OTHER,				//其他
	ATM_INSERT_CARD,    	//插卡
	ATM_REMOVE_CARD,		//拔卡
	ATM_NULL = 0xff,		//无操作
}TAtmOperateType;

typedef enum
{
	EXCEPTION_ALL = 0,			//全部	
	EXCEPTION_EATMONEY,		//吞钞
	EXCEPTION_EATCARD,			//吞卡
	EXCEPTION_NULL = 0xff,		//无异常
}TAtmExpType;

typedef struct
{
	int  iType;                     //0为按卡号查询，1为按域查询
	char m_cCardNum[MAX_FILED_NUM][MAX_CRADNO_LEN + 1];	  //卡号 和域复用，若是卡号，则字符串数组分别为卡号、操作类型、金额、异常，若是域，则分别为域1-域4
}TAtmFileInfo;

typedef struct									//统计录像文件请求
{
	TGuiQueryVideoFileReq m_tBaseCondition;	//查询条件
	TAtmFileInfo	m_tCardCondition;	//卡号查询条件
} TGuiQueryVideoFileExReq;        //扩展的卡号查询录像

#define GUI_QUERY_VIDEO_FILE_BEGIN_END_TM_REQ		347   //查询录像文件的开始时间结束时间请求
#define GUI_QUERY_VIDEO_FILE_BEGIN_END_TM_RSP		-347  //查询录像文件的开始时间结束时间回应
typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名	
}TGuiQueryVideoFileBeginEndTmReq;

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名	
	int m_iFileBeginTm;                        //文件开始时间
	int m_iFileEndTm;                          //文件结束时间
}TGuiQueryVideoFileBeginEndTmRsp;

//====================即时回放=======================
#define GUI_VIDEO_INSTANT_PLAY_REQ     1100   //即时回放请求
#define GUI_VIDEO_INSTANT_PLAY_RSP	   -1100  //即时回放回应

typedef struct
{
	int	m_iChn;            //通道号
	int	m_iVoDevId; 
	int	m_iVoChn;
	int	m_iTime;
}TGuiVideoInstantPlayReq;

typedef struct
{
	int m_iPlayId;  //-1表示失败，其余表示id
} TGuiVideoInstantPlayRsp;    //0表成功，其余表示失败

//============标签回放============
//标签的增删改查
typedef struct
{
	int m_iTagTm;              //标签时间（与打点信息一致）
	int m_iTagChn;              //标签所在录像文件的通道号
	char m_strFileName[GUI_VF_NAME_LEN + 1];//录像文件名
	char m_strTagName[GUI_PUBLIC_LEN_63 + 1];   //标签名称   
}TGuiVpTagMsg;     //标签结构体

#define GUI_ADD_VP_TAG_REQ	 1110    //增加标签
#define GUI_ADD_VP_TAG_RSP    -1110
typedef struct
{
	TGuiVpTagMsg m_tVpTagMsg;   
}TGuiAddVpTagReq;                  //增加标签请求

typedef struct
{
	int m_iRetCode; //0表示成功，1表示标签已存在，2表示其余所有的失败情况
	int m_iTagTm;   //返回标签的实际时间
} TGuiAddVpTagRsp;     
      
#define GUI_DELETE_VP_TAG_REQ 	1111	    //删除标签
#define GUI_DELETE_VP_TAG_RSP	-1111
typedef struct
{
	TGuiVpTagMsg m_tVpTagMsg;   
}TGuiDelVpTagReq;                  //删除标签请求

typedef struct
{
	int m_iRetCode;
} TGuiDelVpTagRsp;

#define GUI_MODIFY_VP_TAG_REQ 	1112	    //编辑标签
#define GUI_MODIFY_VP_TAG_RSP	-1112

typedef struct
{
	TGuiVpTagMsg m_tVpTagMsg;   
}TGuiEditVpTagReq;                  //标签请求

typedef struct
{
	int m_iRetCode;
} TGuiEditVpTagRsp;

#define GUI_SEARCH_VP_TAG_REQ	1113    //搜索标签
#define GUI_SEARCH_VP_TAG_RSP	-1113

typedef struct
{
	TGuiQueryVideoFileReq m_tBaseCondition;
	char m_strTagName[GUI_PUBLIC_LEN_63 + 1];//名字为空表示全部获取，否则按关键字获取
}TGuiSearchVpTagReq;

#define GUI_MAX_VPTAG_CNT_PER_FILE	64   //每个文件最多支持64个标签
//查询时，先返回文件总数，再逐条返回TGuiSearchVpTagRetRsp
typedef TGuiQueryVideoFileRsp TGuiQueryVideoTagRsp;

typedef struct
{
	int m_iFB;   //文件开始时间
	int m_iFE;	 //标签所在文件的结束时间
	TGuiVpTagMsg m_tVpTagMsg;
}TGuiSearchVpTagRetRsp;


//----------------------------------------------------------------------
typedef struct									//获取当前回放状态请求
{
	int m_iPlayId;
} TGuiGetVideoPlayStaReq;

#define GUI_VIDEO_PLAY_DOING		1			//正在播放状态
#define GUI_VIDEO_PLAY_PUASED		2			//暂停状态
#define GUI_VIDEO_PLAY_STOPED		3			//播放已停止
#define GUI_VIDEO_PLAY_STEP			4			//步进状态
#define GUI_VIDEO_PLAY_REVERSE		5			//倒放状态
#define GUI_VIDEO_PLAY_STEP_REVERSE	6			//步退状态

#define GUI_VIDEO_PLAY_ERR			100			//播放发生错误
#define GUI_VIDEO_PLAY_NEED_PWD		200			//等待密码
//以下四个对应同步回放 按位来
#define CHN_PLAY_PWD(n) 			(200 + (1<<(n - 1))) // n 画面号从1开始
#define GUI_VIDEO_PLAY_NEED_PWD_1  201      //第一画面需要密码
#define GUI_VIDEO_PLAY_NEED_PWD_2  202      //第二画面需要密码
#define GUI_VIDEO_PLAY_NEED_PWD_3  204      //第三画面需要密码
#define GUI_VIDEO_PLAY_NEED_PWD_4  208      //第四画面需要密码

#define GUI_VIDEO_PLAY_DECODE_CAP_LIMIT	300         //回放解码性能不足
#define GUI_VIDEO_PLAY_SPEED_CAP_LIMIT  301			//回放速度性能受限，只能越到最大

typedef struct									//获取当前回放状态回应
{
	int m_iPlayId;								//播放序列号
	int m_iCurPlayStat;						//当前播放状态
	int m_iPlayRate;							//当前播放的进度(0-100)
	int m_iSpeed;								//当前播放倍数(正表加速，负表减速, 0表单步)
} TGuiGetVideoPlayStatRsp;

#define GUI_PLAYER_MAX_VO_CNT		4			//播放器最大支持画面数

typedef struct									//播放器VO属性
{
	int m_iLeft;								//左上角x坐标
	int m_iTop;									//左上角y坐标	
	int m_iWidth;
	int m_iHeight;
} TGuiVoAttr;

typedef struct									//播放器激活初始化消息
{
	int m_iDevId;								//输出设备编号,dxl 2009/12/02
	int m_iVoCnt;								//播放器VO画面数
	// 2008-12-17 dxl 添加Vo属性
	TGuiVoAttr m_tVoAttrs[GUI_PLAYER_MAX_VO_CNT];
} TGuiVideoPlayerActiveMsg;

typedef TGuiVideoPlayerActiveMsg TGuiVideoPlayerActiveReq;

#define GUI_VIDEO_PLAYER_ACTIVE_OK	0		//初始化成功
#define GUI_VIDEO_PLAYER_ACTIVE_ERR	1		//初始化失败
typedef struct									//播放器激活初始化结果回应
{
	int m_iRetCode;
} TGuiVideoPlayerActiveRsp;

typedef struct									//播放倍速命令
{
	int m_iPlayId;								//播放编号
	int m_iSpeed;								//正n表示当前倍速xn倍速, -n表示/n倍速
} TGuiVideoPlayChgSpeedCmd;

typedef struct									//快放/慢放命令
{
	int m_iPlayId;								//播放编号
	int m_iSpeed;								//正n表示n倍速播放,-n表示1/n倍速播放(1, 0, -1均表示正常速播放)
} TGuiVideoPlaySpeedCmd;

typedef struct									//全屏播放请求
{
	int m_iVoId;								//画面号(1-4)
} TGuiVideoPlayFullScrReq;

#define GUI_VIDEO_PLAY_FULL_SCR_OK		0		//全屏播放成功
#define GUI_VIDEO_PLAY_FULL_SCR_ERR		1		//全屏播放失败
typedef struct									//全屏播放回应
{
	int m_iRetCode;
} TGuiVideoPlayFullScrRsp;

typedef struct									//退出全屏播放请求
{
	int m_iVoId;								//画面号(1-4)
} TGuiVideoPlayFullScrExitReq;

#define GUI_VIDEO_PLAY_FULL_SCR_EXIT_OK		0		//退出全屏播放成功
#define GUI_VIDEO_PLAY_FULL_SCR_EXIT_ERR	1		//退出全屏播放失败
typedef struct									//退出全屏播放回应
{
	int m_iRetCode;
} TGuiVideoPlayFullScrExitRsp;

typedef struct									//设置回放声音请求
{
	int m_iActiveVoId;		//当前激活的声音输出通道，1-4表示切换，其它值无操作
	int m_iSoundEnFlg;		// 1 - 4    :　使能指定通道的声音输出
							// - 1 - -4 :  关闭指定通道的声音输出(vo通道号取绝对值)
} TGuiVideoPlaySetSoundReq;

#define	GUI_VPLAY_SET_SOUND_OK		0		//设置回放声音成功
#define GUI_VPLAY_SET_SOUND_ERR		1		//设置回放声音失败
typedef struct									//设置回放声音回应
{
	int m_iRetCode;		//返回码，GUI_VPLAY_SET_SOUND_OK/ERR
} TGuiVideoPlaySetSoundRsp;

//同步回放
#define GUI_VIDEO_SYNC_PLAYER_ACTIVE_OK	0		//初始化成功
#define GUI_VIDEO_SYNC_PLAYER_ACTIVE_ERR	1		//初始化失败

#define GUI_SYNC_PLAY_SOUND_ENABLE 1
#define GUI_SYNC_PLAY_SOUND_DISABLE 0

typedef struct									//录像文件查询条件
{
	int m_iChn[4];									//通道号
	int m_iDev;									//设备号
	int m_iBeginTm;							//开始时间
	int m_iEndTm;								//结束时间
	int m_iSoundEnable[4];
} TGuiVideoFileQuerySyncCondition;


typedef struct
{
	int m_iCmd;									//命令码
	int m_iLength;								//pdu负载长度
	char m_cPayload[0];						//实际数据
} TGuiSyncParamPdu;								//数据传输单元结构

typedef struct									//播放器激活初始化消息
{
	int m_iDevId;								//设备号
	int m_iVoCnt;								//播放器VO画面数
	// 2008-12-17 dxl 添加Vo属性
	TGuiVoAttr m_tVoAttrs[GUI_PLAYER_MAX_VO_CNT];
} TGuiVideoSyncPlayerActiveMsg;
/********************************************/
//新同步回放
#define GUI_SYNC_PLAYER_VIDEO_FILE_REQ 	368			//同步请求播放文件
#define GUI_SYNC_PLAYER_VIDEO_FILE_RSP  -368		//同步播放文件请求回应

#define GUI_SYNC_PLAYER_MAX_CNT   16
typedef struct									//录像文件查询条件
{
	int m_iChn[GUI_SYNC_PLAYER_MAX_CNT];	//通道号高16位0-主码流1-副码流
	int m_iDev;								//设备号
	int m_iBeginTm;							//开始时间
	int m_iEndTm;							//结束时间
	int m_iSoundEnable[GUI_SYNC_PLAYER_MAX_CNT];
} TGuiSyncPlayerVideoQueryConditionReq;

typedef struct
{
	int m_iRetCode;
}TGuiSyncPlayerVideoQueryConditionRsp;

#define GUI_VIDEO_SYNC_PLAYER_COORDIRATE_REQ 369	//发送坐标,播放器初始化请求
#define GUI_VIDEO_SYNC_PLAYER_COORDIRATE_RSP -369	//播放器初始化请求回应

typedef struct									//播放器激活初始化消息
{
	int m_iDevId;								//设备号
	int m_iVoCnt;								//播放器VO画面数
	// 2008-12-17 dxl 添加Vo属性
	TGuiVoAttr m_tVoAttrs[GUI_SYNC_PLAYER_MAX_CNT];
} TGuiSyncPlayerActiveMsgReq;
typedef struct
{
	int m_iRetCode;
}TGuiSyncPlayerActiveMsgRsp;



typedef struct									//播放器激活初始化结果回应
{
	int m_iRetCode;
} TGuiVideoSyncPlayerActiveRsp;

typedef struct									//播放定位请求(无回应)
{
	int m_iLoc;									//定位百分比(0-100)
} TGuiVideoSyncPlayLocCmd;
typedef struct									//播放倍速命令
{
	int m_iSpeed;								//正n表示当前倍速xn倍速, -n表示/n倍速
} TGuiVideoSyncPlayChgSpeedCmd;

typedef struct									//播放倍速命令
{
	int m_iSpeed;								//正n表示当前倍速xn倍速, -n表示/n倍速
} TGuiVideoSyncPlaySpeedCmd;


typedef struct									//获取当前回放状态回应
{
	int m_iCurPlayStat;						//当前播放状态
	int m_iPlayRate;							//当前播放的进度(0-100)
	int m_iSpeed;								//当前播放倍数(正表加速，负表减速, 0表单步)
} TGuiGetVideoSyncPlayStatRsp;

typedef struct									//全屏播放请求
{
	int m_iVoId;								//画面号(1-4)
} TGuiVideoSyncPlayFullScrReq;

#define GUI_VIDEO_SYNC_PLAY_FULL_SCR_OK	0		//全屏播放成功
#define GUI_VIDEO_SYNC_PLAY_FULL_SCR_ERR	1		//全屏播放失败
typedef struct									//全屏播放回应
{
	int m_iRetCode;
} TGuiVideoSyncPlayFullScrRsp;

#define GUI_VIDEO_SYNC_PLAY_FULL_SCR_EXIT_OK		0		//退出全屏播放成功
#define GUI_VIDEO_SYNC_PLAY_FULL_SCR_EXIT_ERR	1		//退出全屏播放失败
typedef struct									//退出全屏播放回应
{
	int m_iRetCode;
} TGuiVideoSyncPlayFullScrExitRsp;

typedef struct									//退出全屏播放请求
{
	int m_iVoId;								//画面号(1-4)
} TGuiVideoSyncPlayFullScrExitReq;

typedef struct									//播放回应
{
	int m_iRetCode;
} TGuiVideoSyncPlayVideoRsp;

typedef struct									//设置回放声音请求
{
	int m_iActiveVoId;		//当前激活的声音输出通道，1-4表示切换，其它值无操作
	int m_iSoundEnFlg;		// 1 - 4    :　使能指定通道的声音输出
							// - 1 - -4 :  关闭指定通道的声音输出(vo通道号取绝对值)
} TGuiVideoSyncPlaySetSoundReq;

#define	GUI_SYNC_VPLAY_SET_SOUND_OK		0		//设置回放声音成功
#define GUI_SYNC_VPLAY_SET_SOUND_ERR		1		//设置回放声音失败

typedef struct									//设置回放声音回应
{
	int m_iRetCode;		//返回码，GUI_VPLAY_SET_SOUND_OK/ERR
} TGuiVideoSyncPlaySetSoundRsp;



// 抓拍浏览

// 首先，UI向主程序发送jpeg浏览初始化请求(GUI_JPEG_VIEWER_INIT_REQ)
// 主程序接到请求初始化资源并回应GUI_JPEG_VIEWER_INIT_ESP
typedef struct
{
	int m_iDevId;		//设备号
} TGuiJpegViewerInitReq;

#define GUI_JPEG_VIEWER_INIT_OK		0		//jpeg浏览初始化成功
#define GUI_JPEG_VIEWER_INIT_ERR		1		//jpeg浏览初始化失败
typedef struct									//jpeg浏览初始化回应
{
	int m_iRetCode;
} TGuiJpegViewerInitRsp;

// 初始化成功后，UI随时可向主程序发送若干jpeg显示请求
typedef struct
{
	char m_strJpegFile[GUI_VF_NAME_LEN + 1];
} TGuiJpegDspReq;

#define GUI_JPEG_DSP_OK				0		//jpeg显示成功
#define GUI_JPEG_DSP_ERR				1		//jpeg显示失败
typedef struct
{
	int m_iRetCode;
} TGuiJpegDspRsp;

// 最后，UI通知主程序结束jpeg浏览(GUI_JPEG_VIEWER_EXIT_MSG)，
// 主程序应当释放jpeg显示相关的资源，并
// 将dvr恢复到初始化jpeg浏览前的状态。


//==============================云台控制=================================
//-----------------------------解码器设置--------------------------------


typedef struct									//获取解码器参数请求
{
	int m_iChn;									//通道号
} TGuiGetPtzDecodeCnfReq;

#define GUI_PTZ_DEC_PRO_NAME_LEN		31		//解码器协议名长度

typedef struct
{
	int m_iAddr;								//地址
	#if 1
	char m_strProtocol[GUI_PTZ_DEC_PRO_NAME_LEN + 1]; //协议名
	#else
	int m_iProtocol;							//协议编号
	#endif
	int m_iSportId;							//串口编号
	int m_iComAttrEn;							//自定义串口属性使能标识
	int m_iBitRate;							//波特率
	int m_iDataBits;							//数据位
	int m_iStopBits;							//停止位
	int m_iCheckType;							//校验位	
} TGuiPtzDecodeCnf;

typedef struct									//获取解码器参数回应
{
	int m_iChn;									//通道号
	TGuiPtzDecodeCnf m_tCnf;					//配置信息
} TGuiGetPtzDecodeCnfRsp;

typedef struct									//设置解码器参数请求
{
	int m_iChn;									//通道号
	TGuiPtzDecodeCnf m_tCnf;					//配置信息
} TGuiSetPtzDecodeCnfReq;

#define GUI_SET_PTZ_DECODE_CNF_OK	0		//设置参数成功
#define GUI_SET_PTZ_DECODE_CNF_ERR	1		//设置参数失败

typedef struct									//设置解码器参数回应
{
	int m_iRetCode;							//返回码
} TGuiSetPtzDecodeCnfRsp;

typedef struct 								//获取协议列表回应
{
	int m_iProtocolCnt;							//协议总数
} TGuiGetProtocolListRsp;

typedef struct 								//协议列表项
{
	#if 0
	int m_iProtocolId; 							//协议唯一编号
	#endif
	int m_iExistProtocol;      //1为已有协议，0为待添加的协议
	char m_strProtocolName[GUI_PTZ_DEC_PRO_NAME_LEN + 1]; //协议名
} TGuiProtocolListItem;

typedef struct 								//获取协议列表回应
{
	int m_iProtocolCnt;							//协议总数
} TGuiSetProtocolListReq;

//------------------------------云台控制---------------------------------

typedef struct									//获取预置位列表请求
{
	int m_iChn;									//通道编号
} TGuiGetPtzPresetPointListReq;

typedef struct									//获取预置位列表回应
{
	int m_iChn;									//通道编号
	int m_iCnt;									//预置位个数
} TGuiGetPtzPresetPointListRsp;
//随后连续发送各个预置点的编号,用整形表式


#define GUI_PTZ_MAX_POINT_PER_ROUTE		32	//每条巡航路径中最大包含的巡航点数
#define GUI_PTZ_MAX_ROUTE_PER_CHN		8	//每个通道最多可设的巡航路径数

typedef struct									//巡航点
{
	int m_iPointId;							//预置位编号
	int m_iSpeed;								//巡航速度
	int m_iDelay;								//停留时间(单位:s)
} TGuiPtzCruiseRoutePoint;

typedef struct									//巡航路径
{
	int m_iEnable;								//使能标识(1使能/0未使能)
	int m_iPointCnt;							//巡航点数
	TGuiPtzCruiseRoutePoint m_tPoints[GUI_PTZ_MAX_POINT_PER_ROUTE]; //巡航点
} TGuiPtzCruiseRoute;

typedef struct									//获取巡航路径请求
{
	int m_iChn;									//通道号
} TGuiGetPtzCruiseRouteReq;

typedef struct									//获取巡航路径回应
{
	int m_iChn;									//通道号(0表示获取失败)
	TGuiPtzCruiseRoute m_tRoutes[GUI_PTZ_MAX_ROUTE_PER_CHN]; //通道对应的巡航路径
} TGuiGetPtzCruiseRouteRsp;

typedef struct									//设置巡航路径请求
{
	int m_iChn;									//通道号
	TGuiPtzCruiseRoute m_tRoutes[GUI_PTZ_MAX_ROUTE_PER_CHN]; //通道对应的巡航路径
} TGuiSetPtzCruiseRouteReq;

#define GUI_SET_PTZ_CRUISROUTE_OK	0		//设置成功
#define GUI_SET_PTZ_CRUISROUTE_ERR	1		//设置失败
typedef struct									//设置巡航路径回应
{
	int m_iRetCode;							//返回码
} TGuiSetPtzCruiseRouteRsp;

//注意，UI的预置位编号从1开始，0表示特殊值
typedef struct									//添加预置位请求
{
	int m_iChn;									//通道编号
	int m_iPointId;							//预置点编号(0表式添加新预置位,
												//其它值表式在指定编号处更新预置位信息)
} TGuiAddPtzPresetPointReq;

typedef struct									//添加预置位回应
{
	int m_iChn;									//0表示添加预置位失败
	int m_iPointId;							//新加的预置位编号
} TGuiAddPtzPresetPointRsp;

typedef struct									//删除预置位请求
{
	int m_iChn;
	int m_iPointId;
} TGuiDelPtzPresetPointReq;

#define GUI_DEL_PTZ_PRESETPOINT_OK		0	//删除预置位成功
#define GUI_DEL_PTZ_PRESETPOINT_ERR		1	//删除预置位失败
typedef struct									//删除预置位回应
{
	int m_iRetCode;							//返回码
} TGuiDelPtzPresetPointRsp;

typedef struct									//调用预置位请求
{
	int m_iChn;									//通道编号
	int m_iPointId;							//预置位编号
} TGuiTurnToPtzPresetPointReq;

#define GUI_TRN_PTZ_PRESETPOINT_OK		0	//调用预置位成功
#define GUI_TRN_PTZ_PRESETPOINT_ERR		1	//调用预置位失败
typedef struct									//调用预置位回应
{
	int m_iRetCode;							//返回码
} TGuiTurnToPtzPresetPointRsp;

// 云台控制本地控制码:
// 1. 方向控制, m_iParam[0]存放速度(双方向先水平后竖直速度)
#define GUI_PTZ_LOCAL_CODE_UP				1
#define GUI_PTZ_LOCAL_CODE_DOWN			2
#define GUI_PTZ_LOCAL_CODE_LEFT			3
#define GUI_PTZ_LOCAL_CODE_RIGHT			4
#define GUI_PTZ_LOCAL_CODE_RIGHTUP		5
#define GUI_PTZ_LOCAL_CODE_LEFTUP		6
#define GUI_PTZ_LOCAL_CODE_RIGHTDOWN		7
#define GUI_PTZ_LOCAL_CODE_LEFTDOWN		8
#define GUI_PTZ_LOCAL_CODE_STOP			9

// 2. 变焦、变倍、光圈调节
#define GUI_PTZ_LOCAL_CODE_ZOOM_ASC		10	//变倍+
#define GUI_PTZ_LOCAL_CODE_ZOOM_DEC		11	//变倍-
#define GUI_PTZ_LOCAL_CODE_ZOOM_STOP		12	//变倍stop
#define GUI_PTZ_LOCAL_CODE_FOCUS_ASC		13	//变焦+
#define GUI_PTZ_LOCAL_CODE_FOCUS_DEC		14	//变焦-
#define GUI_PTZ_LOCAL_CODE_FOCUS_STOP	15	//变焦stop
#define GUI_PTZ_LOCAL_CODE_IRIS_ASC		17	//光圈+
#define GUI_PTZ_LOCAL_CODE_IRIS_DEC		18	//光圈-

// 3. 自动扫描
#define GUI_PTZ_LOCAL_CODE_AUTO_START	23
#define GUI_PTZ_LOCAL_CODE_AUTO_STOP		24

// 灯光、雨刷、辅助
#define GUI_PTZ_LOCAL_CODE_LIGHT_ON		21
#define GUI_PTZ_LOCAL_CODE_LIGHT_OFF		22
#define GUI_PTZ_LOCAL_CODE_WIPE_START	19
#define GUI_PTZ_LOCAL_CODE_WIPE_STOP		20
#define GUI_PTZ_LOCAL_CODE_ASS_ON		29
#define GUI_PTZ_LOCAL_CODE_ASS_OFF		30

// 4. 预置位
#define GUI_PTZ_LOCAL_CODE_GO_PRESET		25	//转到预置位
#define GUI_PTZ_LOCAL_CODE_SET_PRESET	28	//设置预置位

// 5. 轨迹

typedef struct									//云台控制本地控制码
{
	int m_iChn;									//通道号
	int m_iCode;								//控制码值
	int m_iParam[4];							//相关参数
} TGuiPtzLocalCode;

typedef struct									//开始记录轨迹请求
{
	int m_iChn;									//通道号
} TGuiStartPtzTrackReq;

#define GUI_PTZ_TRACK_START_OK			0	//开始记录轨迹成功
#define GUI_PTZ_TRACK_START_ERR			1	//开始记录轨迹失败
typedef struct									//开始记录轨迹回应
{
	int m_iRetCode;
} TGuiStartPtzTrackRsp;

typedef struct									//结束记录轨迹请求
{
	int m_iChn;
} TGuiEndPtzTrackReq;

#define GUI_PTZ_TRACK_END_OK				0	//结束记录轨迹成功
#define GUI_PTZ_TRACK_END_ERR				1	//结束记录轨迹失败
typedef struct									//结束记录轨迹回应
{
	int m_iRetCode;
} TGuiEndPtzTrackRsp;

typedef struct									//开始演示轨迹请求
{
	int m_iChn;									//通道号
} TGuiStartPtzTrackPlayReq;

#define GUI_PTZ_TRACK_PLAY_START_OK		0	//开始演示轨迹成功
#define GUI_PTZ_TRACK_PLAY_START_ERR		1	//开始演示轨迹失败
typedef struct									//开始演示轨迹回应
{
	int m_iRetCode;
} TGuiStartPtzTrackPlayRsp;

typedef struct									//结束演示轨迹请求
{
	int m_iChn;
} TGuiEndPtzTrackPlayReq;

#define GUI_PTZ_TRACK_PLAY_END_OK		0	//结束演示轨迹成功
#define GUI_PTZ_TRACK_PLAY_END_ERR		1	//结束演示轨迹失败
typedef struct									//结束演示轨迹回应
{
	int m_iRetCode;
} TGuiEndPtzTrackPlayRsp;

typedef struct									// 3d定位
{
	int m_iChn;				//通道号
	int m_iMxp;				//鼠标位置xp
	int m_iMyp;				//鼠标位置yp
	int m_iScrWidth;		//屏幕宽
	int m_iScrHeight;		//屏幕高
	int m_iWidth;			//区域宽(可正负，负表反方向, 0表点)
	int m_iHeight;			//区域高(可正负，负表反方向, 0表点)
} TGuiPtz3DLocMsg;

//==============================报警设置=================================
//------------------------------报警输入---------------------------------
typedef struct									//时间段
{
	int m_iEnable;								//时段使能标识
	int m_iBeginTm;							//开始时间，以当天流过的秒计
	int m_iEndTm;								//结束时间
} TGuiTimePair;

#define GUI_ALARM_TIME_PER_DAY	4			//每天可设时间段个数
#define GUI_ALARM_OPEN_TYPE		1			//常开报警
#define GUI_ALARM_CLOSE_TYPE		2			//常闭报警
#define GUI_DVR_ALARM_OUT_CNT		4			//报警输出通道个数

typedef struct									//ptz联动
{
	int m_iPreSetPointEnable;					//预置点使能
	int m_iPreSetPointId;						//预置点编号
	int m_iCruisePathEnable;					//巡航路径使能
	int m_iCruisePathId;						//巡航路径编号
	int m_iTrackEnable;						//轨迹使能
} TGuiPtzLinkage;

typedef struct									//报警输入配置
{
	int m_iAlarmType;							//报警器类型，常开/常闭
	int m_iEnable;								//报警处理使能 0/1
	int m_iSoundEnable;						//声音使能 0/1
	int m_iDisplayEnable;						//屏幕显示使能低字节 0/1 次第字节表示使能联动那个画面
	int m_iTranToServerEnable;                  //上传中心使能 0/1
	int m_iVideoEnable[GUI_DVR_CHN_CNT];		//联动各通道录像使能 0/1
	int m_iPicEnable[GUI_DVR_CHN_CNT];			//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];			//联动报警输出使能 0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4通道，依次循环
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//ptz联动
} TGuiAlarmInCnf;

typedef struct									//获取报警输入参数请求
{
	int m_iPortId;								//报警输入端口编号1-16
} TGuiGetAlarmInCnfReq;

typedef struct									//获取报警输入参数回应
{
	int m_iPortId;								//端口号
	TGuiAlarmInCnf m_tCnf;						//参数配置
} TGuiGetAlarmInCnfRsp;

typedef struct									//设置报警输入参数请求
{
	int m_iPortId;								//端口号
	TGuiAlarmInCnf m_tCnf;						//参数配置
} TGuiSetAlarmInCnfReq;

#define GUI_SET_ALARM_IN_CNF_OK		0		//设置参数成功
#define GUI_SET_ALARM_IN_CNF_ERR		1		//设置参数失败
typedef struct									//设置报警输入参数回应
{
	int m_iRetCode;
} TGuiSetAlarmInCnfRsp;

//------------------------------报警输出---------------------------------
typedef struct									//报警输出参数
{
	int m_iMsgDelay;							//信号延时
	int m_iType;								//报警器类型:常开/常闭
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY]; //布防时间,7天，每天四个时段
} TGuiAlarmOutCnf;

typedef struct									//获取报警输出参数请求
{
	int m_iPortId;								//报警输出通道
} TGuiGetAlarmOutCnfReq;

typedef struct									//获取报警输出参数请求
{
	int m_iPortId;								//报警输出通道号
	TGuiAlarmOutCnf m_tCnf;
} TGuiGetAlarmOutCnfRsp;

typedef struct									//设置报警输出参数请求
{
	int m_iPortId;								//报警输出通道号
	TGuiAlarmOutCnf m_tCnf;
} TGuiSetAlarmOutCnfReq;

#define GUI_SET_ALARM_OUT_CNF_OK		0		//设置报警输入参数成功
#define GUI_SET_ALARM_OUT_CNF_ERR	1		//设置报警输入参数失败

typedef struct									//设置报警输出参数回应
{
	int m_iRetCode;							//返回码
} TGuiSetAlarmOutCnfRsp;

// 手动消警

#define GUI_ALARM_ALL					0
// 其它类型见GuiTransport

typedef struct									//消除报警命令
{
	int m_iType;
} TGuiClearAlarmCmd;
//----------------------------报警联动单画面-------------------------
#define GUI_GET_ALARM_LINK_SPIC_INTERVAL_REQ   541 //获取报警联动单画面巡航间隔时间请求
#define GUI_GET_ALARM_LINK_SPIC_INTERVAL_RSP   -541   //获取报警联动单画面巡航间隔时间回应
typedef struct
{
	int m_iTm;
}TGuiGetAlarmLinkSpicIntervalRsp;


#define GUI_SET_ALARM_LINK_SPIC_INTERVAL_REQ  542  //设置报警联动单画面巡航间隔请求
#define GUI_SET_ALRRM_LINK_SPIC_INTERVAL_RSP  -542 //设置报警联动单画面巡航间隔回应
typedef struct
{
	int m_iTm; // 0- 不巡航，其他时间均巡航
}TGuiSetAlarmLinkSpicIntervalReq;
#define GUI_SET_ALARM_LINK_OK    0 //设置成功
#define GUI_SET_ALARM_LINK_ERR   1 //设置失败
typedef struct
{
	int m_iRetCode;   
}TGuiSetAlarmLinkSpicIntervalRsp;


//------------------------------移动侦测---------------------------------
typedef struct									//获取移动侦测报警参数请求
{
	int m_iChn;									//通道号
} TGuiGetMdAlarmCnfReq;

typedef struct									//移动侦测报警配置
{
	int m_iSensitive;							//灵敏度
	int m_iEnable;								//移动侦测使能 0/1
	int m_iSoundEnable;						//声音使能 0/1
	int m_iDisplayEnable;						//屏幕显示使能 0/1
	int m_iToServerEnable;
	int m_iVideoEnable[GUI_DVR_CHN_CNT];		//联动各通道录像使能 0/1
	int m_iPicEnable[GUI_DVR_CHN_CNT];			//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];			//联动报警输出使能 0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	char m_cAreas[GUI_DVR_SCR_ROWS][GUI_DVR_SCR_COLS];		//移动侦测区域，22x18小格，0/1
} TGuiMdAlarmCnf;

typedef struct									//获取移动侦测报警参数回应
{
	int m_iChn;									//通道号
	TGuiMdAlarmCnf m_tCnf;						//参数配置表
} TGuiGetMdAlarmCnfRsp;						//获取移动侦测报警参数

typedef struct									//设置移动侦测报警参数请求
{
	int m_iChn;									//通道号
	TGuiMdAlarmCnf m_tCnf;						//参数配置表
} TGuiSetMdAlarmCnfReq;						//获取移动侦测报警参数

#define GUI_SET_MD_ALARM_CNF_OK		0		//设置参数成功
#define GUI_SET_MD_ALARM_CNF_ERR		1		//设置参数失败
typedef struct									//设置移动侦测报警参数回应
{
	int m_iRetCode;
} TGuiSetMdAlarmCnfRsp;


//------------------------------视频丢失---------------------------------
typedef struct									//获取视频丢失报警参数请求
{
	int m_iChn;									//通道号
} TGuiGetVideoLostAlarmCnfReq;

typedef struct									//移动侦测报警配置
{
	int m_iEnable;								//移动侦测使能 0/1
	int m_iSoundEnable;							//声音使能 0/1
	int m_iDisplayEnable;						//屏幕显示使能 0/1
	int m_iToServerEnalbe;
	int m_iVideoEnable[GUI_DVR_CHN_CNT];		//联动各通道录像使能 0/1
	int m_iPicEnable[GUI_DVR_CHN_CNT];			//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];			//联动报警输出使能 0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//ptz联动
} TGuiVideoLostAlarmCnf;

typedef struct									//获取视频丢失报警参数回应
{
	int m_iChn;									//通道号
	TGuiVideoLostAlarmCnf m_tCnf;				//参数配置表
} TGuiGetVideoLostAlarmCnfRsp;

typedef struct									//设置视频丢失报警参数请求
{
	int m_iChn;									//通道号
	TGuiVideoLostAlarmCnf m_tCnf;				//参数配置表
} TGuiSetVideoLostAlarmCnfReq;

#define GUI_SET_VL_ALARM_CNF_OK		0		//设置参数成功
#define GUI_SET_VL_ALARM_CNF_ERR		1		//设置参数失败
typedef struct									//设置移动侦测报警参数回应
{
	int m_iRetCode;
} TGuiSetVideoLostAlarmCnfRsp;
//===========================音频丢失报警==================================
#define GUI_GET_AL_ALARM_CNF_REQ		532		//设置音频丢失报警参数请求
#define GUI_GET_AL_ALARM_CNF_RSP		-532	//设置音频丢失报警参数回应

typedef struct									//移动侦测报警配置
{
	int m_iEnable;								//移动侦测使能 0/1
	int m_iSoundEnable;						//声音使能 0/1
	int m_iDisplayEnable;						//屏幕显示使能 0/1
	int m_iToServerEnalbe;
	int m_iVideoEnable[GUI_DVR_CHN_CNT];		//联动各通道录像使能 0/1
	int m_iPicEnable[GUI_DVR_CHN_CNT];			//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];			//联动报警输出使能 0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//ptz联动
} TGuiAudioLostAlarmCnf;

typedef struct									//获取音频丢失报警参数请求
{
	int m_iChn;									//通道号
} TGuiGetAudioLostAlarmCnfReq;

typedef struct									//获取视频丢失报警参数回应
{
	int m_iChn;									//通道号
	TGuiAudioLostAlarmCnf m_tCnf;				//参数配置表
} TGuiGetAudioLostAlarmCnfRsp;


#define GUI_SET_AL_ALARM_CNF_REQ		533		//设置音频丢失报警参数请求
#define GUI_SET_AL_ALARM_CNF_RSP		-533	//设置音频丢失报警参数回应

typedef struct									//设置视频丢失报警参数请求
{
	int m_iChn;									//通道号
	TGuiAudioLostAlarmCnf m_tCnf;				//参数配置表
} TGuiSetAudioLostAlarmCnfReq;

#define GUI_SET_AL_ALARM_CNF_OK			0		//设置参数成功
#define GUI_SET_AL_ALARM_CNF_ERR		1		//设置参数失败

typedef struct									//设置移动侦测报警参数回应
{
	int m_iRetCode;
} TGuiSetAudioLostAlarmCnfRsp;

//=========================温湿度报警==========================
//行嵌无人值守主机
#define GUI_GET_TEMPHUM_ALARM_CNF_REQ		534		//获取温湿度报警参数请求
#define GUI_GET_TEMPHUM_ALARM_CNF_RSP		-534	//获取温湿度报警参数回应

typedef struct	//报警输入配置
{
	int m_iEnable;									//报警处理使能 0/1
	int m_iSoundEnable;								//声音使能 0/1
	int m_iDisplayEnable;							//屏幕显示使能低字节 0/1 次第字节表示使能联动哪个画面
	int m_iToServerEnalbe;                  		//上传中心使能 0/1
	int m_iVideoEnable[GUI_DVR_CHN_CNT];			//联动各通道录像使能 0/1
	int m_iPicEnable[GUI_DVR_CHN_CNT];				//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];		//联动报警输出使能 0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4通道，依次循环
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];	//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];		//ptz联动
} TGuiTempHumAlarmCnf;

typedef struct											//获取温湿度报警参数请求
{
	int m_iDevId;										//温湿度模块编号
} TGuiGetTempHumAlarmCnfReq;

typedef struct											//温湿度模块编号
{
	int m_iDevId;										//温湿度模块编号
	TGuiTempHumAlarmCnf m_tCnf;							//参数配置
} TGuiGetTempHumAlarmCnfRsp;

#define GUI_SET_TEMPHUM_ALARM_CNF_REQ		535			//设置温湿度报警参数请求
#define GUI_SET_TEMPHUM_ALARM_CNF_RSP		-535		//设置温湿度报警参数回应

#define GUI_SET_ALARM_TEMPHUM_CNF_OK		0			//设置参数成功
#define GUI_SET_ALARM_TEMPHUM_CNF_ERR		1			//设置参数失败

typedef struct											//设置温湿度报警参数请求
{
	int m_iDevId;										//温湿度模块地址
	TGuiTempHumAlarmCnf m_tCnf;							//参数配置
} TGuiSetTempHumAlarmCnfReq;

typedef struct											//设置温湿度报警参数回应
{
	int m_iRetCode;
} TGuiSetTempHumAlarmCnfRsp;

//==========================视频遮挡报警=============================
#define GUI_GET_OD_ALARM_CNF_REQ	  536  		//获取视频遮挡报警参数请求
#define GUI_GET_OD_ALARM_CNF_RSP	  -536		//获取视频遮挡报警参数回应
#define	GUI_SET_OD_ALARM_CNF_REQ      537	    //设置视频遮挡报警参数请求
#define	GUI_SET_OD_ALARM_CNF_RSP   	  -537		//设置视频遮挡报警参数回应

typedef struct							//获取视频遮挡报警参数请求
{
	int m_iChn;							//通道号
} TGuiGetODAlarmCnfReq;

typedef struct										//视频遮挡报警配置
{
	int m_iSensitive;								//灵敏度
	int m_iEnable;									//移动侦测使能 0/1
	int m_iSoundEnable;								//声音使能 0/1
	int m_iDisplayEnable;							//屏幕显示使能 0/1
	int m_iToServerEnable;
	int m_iVideoEnable[GUI_DVR_CHN_CNT];			//联动各通道录像使能 0/1
	int m_iPicEnable[GUI_DVR_CHN_CNT];				//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];	//联动报警输出使能 0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//联动ptz
	char m_cAreas[GUI_DVR_SCR_ROWS][GUI_DVR_SCR_COLS];		//视频遮挡区域，22x18小格，0/1
} TGuiODAlarmCnf;

typedef struct							//获取视频遮挡报警参数回应
{
	int m_iChn;							//通道号
	TGuiODAlarmCnf m_tCnf;				//参数配置表
} TGuiGetODAlarmCnfRsp;				//获取视频遮挡报警参数

typedef struct							//设置视频遮挡报警参数请求
{
	int m_iChn;							//通道号
	TGuiODAlarmCnf m_tCnf;				//参数配置表
} TGuiSetODAlarmCnfReq;				//获取视频遮挡报警参数

#define GUI_SET_OD_ALARM_CNF_OK			0			//设置参数成功
#define GUI_SET_OD_ALARM_CNF_ERR		1			//设置参数失败
typedef struct								
{
	int m_iRetCode;
} TGuiSetODAlarmCnfRsp;			//设置视频遮挡报警参数回应


// ===========================系统注销==================================
#define GUI_SYS_SPEC_CODE1			0x20090624
#define GUI_SYS_SPEC_CODE2			0x06242009   //本地
#define GUI_SYS_SPEC_CODE3          0x20130425   //远程

typedef struct
{
	int m_iCode1;								//固定值 GUI_SYS_SPEC_CODE1
	int m_iCode2;								//固定值 GUI_SYS_SPEC_CODE2	
} TGuiSystemReSetReq;

#define GUI_SYS_RESET_ACCEPT			0		//允许重启
#define GUI_SYS_RESET_REFUSE			1		//拒绝重启
typedef struct									//系统重启回应
{
	int m_iRetCode;
} TGuiSystemReSetRsp;


typedef struct
{
	int m_iCode1;								//固定值 GUI_SYS_SPEC_CODE1
	int m_iCode2;								//固定值 GUI_SYS_SPEC_CODE2
} TGuiSystemPowerOffReq;
	
#define GUI_SYS_POWEROFF_ACCEPT		0		//允许关机
#define GUI_SYS_POWEROFF_REFUSE		1		//拒绝关机
typedef struct									//系统关机回应
{
	int m_iRetCode;
} TGuiSystemPowerOffRsp;

// 消息
typedef int TGuiKey;

// JPGE抓拍
#define GUI_SNAPSHOT_REQ				363		//JPEG抓拍请求
#define GUI_SNAPSHOT_RSP				364		//JPEG抓拍回应
typedef struct									//抓拍请求
{
	int iChn;	 								// 1-16对应通道，0:全部通道
} TGuiSnapshotReq;

#define GUI_SNAPSHOT_OK			0
#define GUI_SNAPSHOT_ERR		1
typedef struct									//抓拍回应
{
	int m_iRetCode;							//返回码
} TGuiSnapshotRsp;

// 加强的抓拍功能，返回抓拍文件名 (IPC使用)
#define GUI_SNAPSHOT_REQ_EX			365			//JPED抓拍，同时返回抓拍图片名称
#define GUI_SNAPSHOT_RSP_EX			-365
typedef struct									//抓拍请求
{
	int iChn;	 								// 1-16对应通道，0:全部通道
} TGuiSnapshotReqEx;

typedef struct									//抓拍回应
{
	int m_iRetCode;							//返回码
	char m_cPicName[64];						//抓拍图片名称	
} TGuiSnapshotRspEx;

//多通道抓拍 (上海地标，外挂使用)
#define GUI_MULTI_CHN_SNAPSHORT_REQ			366  //多通道抓拍请求 （上海地标，外挂使用）
#define GUI_MULTI_CHN_SNAPSHORT_RSP	    	-366 //多通道抓拍回应  (上海地标，外挂使用)

typedef struct
{
	int m_iChn;  //需要抓拍的通道列表, 通道按位表示 0-不需要抓，1-需要抓拍。
	int m_iChnEx; //预留扩展用。
}TGuiSnapshotMultiChnReq;

typedef struct
{
	int m_iChnCnt;     //返回抓拍成功通道个数
}TGuiSnapshotMultiChnRsp;

typedef struct
{
	char m_iChn;   //通道号
	char m_cPicName[GUI_PUBLIC_LEN_63 + 1];  //对应通道号抓拍图片路径及名称
}TGuiSnapshotMultiChnMsg;  //返回抓拍图片路径及名称


// 路径演示
#define GUI_PTZ_PATH_PLAY_START	1			//开始演示路径
#define GUI_PTZ_PATH_PLAY_STOP	0			//结束演示路径
typedef struct
{
	int m_iCmd;									//命令码:开始/结束
	int m_iChn;									//通道号, 1-16
	int m_iPath;								//路径号, 1-8
} TGuiPtzPathPlayReq;

#define GUI_PTZ_PATH_PLAY_OK		0			//演示/结束路径成功
#define GUI_PTZ_PATH_PLAY_ERR		1			//演示/结束路径失败
typedef struct
{
	int m_iRetCode;							//返回码
} TGuiPtzPathPlayRsp;

// 电子放大

#define EZOOM_VO_START				1000

typedef struct
{
	int m_iChn;			//通道号: 1-16
	int m_iLeft;		//放大区域XP坐标
	int m_iTop;			//放大区域YP坐标
	int m_iWidth;		//放大区域宽度
	int m_iHeight;		//放大区域高度
} TGuiEzoomAttr;

typedef struct
{
	int m_iDevId;								//设备号dxl 2009/12/02
	TGuiEzoomAttr m_tAttr;						//初始化电子放大参数
} TGuiEzoomEnterReq;							//进入电子放大请求

#define GUI_EZOOM_ENTER_OK		0
#define GUI_EZOOM_ENTER_ERR		1
#define GUI_EZOOM_CHN_BIGGER_14	20131105  //当回放路数超过14路时，不支持电子放大
typedef struct									//进入电子放大回应
{
	int m_iRetCode;
} TGuiEzoomEnterRsp;    

// 退出电子放

typedef struct
{
	int m_iDevId;	
} TGuiEzoomExitReq; // 退出电子放大请求dxl 2009/12/02

#define GUI_EZOOM_EXIT_OK			0
#define GUI_EZOOM_EXIT_ERR		1
typedef struct
{
	int m_iRetCode;
} TGuiEzoomExitRsp;							//退出电子放大回应

typedef struct
{
	TGuiEzoomAttr m_tAttr;						//放大参数
} TGuiEzoomSetAttrReq;						//设置电子放大参数请求

#define GUI_EZOOM_SET_ATTR_OK		0
#define GUI_EZOOM_SET_ATTR_ERR	1
typedef struct
{
	int m_iRetCode;
} TGuiEzoomSetAttrRsp;						//设置电子放大参数请求

// UI启动信息
#define GUI_VER_LEN		31
typedef struct
{
	int m_iMasterDspDev;			//主显示设备
	int m_iVgaMode;				//VGA显示模式
	int m_iBncMode;				//BNC显示模式
	char m_strGuiVer[GUI_VER_LEN + 1];	//UI版本号
} TGuiStartUpMsg;				//通知消息

//add by wanzy 2011/10/11
typedef struct
{
	int m_iMasterDspDev;				//主显示设备
	int m_iVgaMode;				//VGA显示模式
	int m_iBncMode;				//BNC显示模式
	char m_strGuiVer[GUI_VER_LEN + 1 - 4];	//UI版本号
	char m_strCharSet[4];			//字符集版本 "GB"/"UTF"
} TGuiStartUpMsgEx;					//通知消息

typedef struct
{
	int m_iRetCode;				//返回值,0表示成功，其余值表失败
} TGuiStartUpRsp;				//消息回应

//设置VGA分辨率(和HDMI 设备1同源)
//设置HD0\HD1分辨率(高16位：HD0 低16位：HD1)
typedef struct
{
	int m_iVgaMode;				//VGA显示模式
} TGuiSetVgaModeReq;		//设置VGA格式

#define GUI_SET_VGA_OK		0
#define GUI_SET_VGA_ERR		1
typedef struct
{
	int m_iRetCode;				//返回码
} TGuiSetVgaModeRsp;

//设置BNC制式
typedef struct
{
	int m_iBncId;					//BNC编号，暂时使用0，表示全部BNC
	int m_iBncMode;				//模式
} TGuiSetBncModeReq;

#define GUI_SET_BNC_OK		0
#define GUI_SET_BNC_ERR		1
typedef struct
{
	int m_iRetCode;
} TGuiSetBncModeRsp;

//设置HDMI分辨率(设备0)
#define GUI_SET_HDMI_MOD_REQ 13
#define GUI_SET_HDMI_MOD_RSP -13

typedef struct
{
	int m_iHDMIMode;				//HDMI显示模式
} TGuiSetHDMIModeReq;		//设置HDMI格式

#define GUI_SET_HDMI_OK			0
#define GUI_SET_HDMI_ERR		1
typedef struct
{
	int m_iRetCode;				//返回码
} TGuiSetHDMIModeRsp;

//设置HDMI的显示器显示模式

#define GUI_SET_HDMI_DSP_MODE_REQ  14
#define GUI_SET_HDMI_DSP_MODE_RSP  -14
typedef struct
{
	int m_iHDMIDspMode;   //0-Auto, 1-YCBCR, 2-RGB
}TGuiSetHDMIDspModReq;

#define GUI_SET_HDMI_DSP_MODE_OK   0
#define GUI_SET_HDMI_DSP_MODE_ERR  1
typedef struct
{
	int m_iRetCode;
}TGuiSetHDMIDspModeRsp;

// 光盘刻录
typedef struct				// 刻录开始回应
{
	int m_iRetCode;			// 0成功, 1失败
} TGuiCdBackupStartRsp;

typedef struct				// 刻录文件列表
{
	int m_iSeq;							// 序列号，保留
	char m_strFile[GUI_VF_NAME_LEN + 1];	// 文件名
} TGuiCdBackupFileReq;

typedef struct
{
	int m_iRetCode;			// 0成功，1失败
} TGuiCdBackupFileRsp;

typedef struct
{
	int m_iDid;				// 光盘编号
} TGuiCdBackupExecReq;

typedef struct
{
	int m_iRetCode;			// 0成功，1失败
	int m_iBackupId;			//刻录ID，获取进度时使用
} TGuiCdBackupExecRsp;

typedef struct				// 进度请求
{
	int m_iBackupId;			//刻录ID
} TGuiCdBackupPrgReq;

enum	// 刻录状态
{
	GUI_CD_BACKUP_FREE	= 0,   //空闲
	GUI_CD_BACKUP_DOING = 1,	// 正在刻录
	GUI_CD_BACKUP_DONE,		// 刻录完成
	GUI_CD_BACKUP_ERROR,	// 错误	
	GUI_CD_BACKUP_FULL, 	//备份满一张盘
};

typedef struct
{
	int m_iStat;				//当前状态
	int m_iPrg;				//当前进度
} TGuiCdBackupPrgRsp;


/***************************************************/
//视频参数配置(编码能力配置)
typedef struct
{
	int m_iMaxPicSize; //最大分辨率
	int m_iMaxFrameRate; //最大帧率
}TGuiVideoParaConf;
typedef struct
{
	int m_iValue;
}TGuiVideoParaUsedFpsConf;
//获取视频配置参数请求
typedef struct									
{
	int m_iChn;									//通道号
} TGuiGetVideoParaCnfReq;

//获取视频参数配置请求回应
typedef struct									//获取视频编码参数回应
{
	int m_iChn;									//通道号
	TGuiVideoParaConf m_tCnf;					//配置信息
} TGuiGetVideoParaCnfRsp;
// 设置视频编码参数请求
typedef TGuiGetVideoParaCnfRsp TGuiSetVideoParaCnfReq;

#define GUI_SET_VIDEOPARA_CNF_OK		0		//设置成功
#define GUI_SET_VIDEOPARA_CNF_ERR		1		//设置失败

//设置视频参数配置回应
typedef struct									
{
	int m_iRetCode;
} TGuiSetVideoParaCnfRsp;
//************************************




//************************************

//获取总的编码能力请求
typedef struct
{
	int m_iTotalCode;
}TGuiGetTotalFrameRateRsp;
//************************************
//获取某通道支持的视频分辨率大小
#define GUI_GET_CHN_VIDEO_SIZE_REQ     235   //获取单个通道支持的视频分辨率大小请求
#define GUI_GET_CHN_VIDEO_SIZE_RSP     -235   //获取单个通道支持的视频分辨率大小回应
typedef struct
{
	int m_iChn;   //请求通道
}TGuiGetChnVideoSizeReq;

typedef struct
{
	int m_iChn;
	int m_iCnt;   //返回请求通道支持几种视频分辨率
}TGuiGetChnVideoSizeRsp;

typedef struct
{
	int m_iVideoSize;  //分辨率大小(GUI_1080P 为12)
}TGuiVideoSizeItem;

//*****************************WIFI相关**************************************
//无线设置

//Wifi在线状态
#define NET_WIFI_UNONLINE    0
#define NET_WIFI_ONLINE       1
//wifi安全方式
#define NET_WIFI_WEP      1
#define NET_WIFI_WPA_PSK      2
#define NET_WIFI_WPA2_PSK     3
//wifi 密钥格式
#define NET_WIFI_KEYTYPE_HEX    1
#define NET_WIFI_KEYTYPE_ASCII  2
//wifi加密方法
#define NET_WIFI_ENCRYPT_TKIP  1
#define NET_WIFI_ENCRYPT_AES   2
//wifi 密码类型
#define NET_WIFI_PWDTYPE_DISEABLE   0
#define NET_WIFI_PWDTYPE_64BITS  1
#define NET_WIFI_PWDTYPE_128BITS 2
//wifi 密钥选择
#define NET_WIFI_KEYNUM_1     1
#define NET_WIFI_KEYNUM_2     2
#define NET_WIFI_KEYNUM_3     3
#define NET_WIFI_KEYNUM_4     4

//wifi 网络设置
#define GUI_GET_WIFI_NET_CNF_REQ    100000   //获取wifi网络设置信息请求
#define GUI_GET_WIFI_NET_CNF_RSP    -100000//获取wifi网络设置信息回应

//wifi 网络设置设置界面
#define GUI_GET_WIFI_IP_VIA_DHCP		0			//dhcp方式获取IP
#define GUI_GET_WIFI_IP_VIA_HUM		1			//手动设置IP
#define GUI_GET_WIFI_DNS_VIA_DHCP    0      //dhcp方式获取dns
#define GUI_GET_WIFI_DNS_VIA_HUM    1       //手动方式设置dns
#define GUI_WIFIESSID_STR_LEN  31

typedef struct 
{
	//int m_iHaveWifiCard;
	int m_iOnLineState;
	int m_iGetWifiIpMethod; 					//获取ip地址的方式
	int m_blWifiIsHandSetDns;						//手动设置DNS
	char m_strWifiMac[GUI_IP_STR_LEN + 1]; //mac地址
	char m_strWifiEssid[GUI_WIFIESSID_STR_LEN + 1];  //essid
	char m_strWifiIp[GUI_IP_STR_LEN + 1];			//ip地址
	char m_strWifiSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strWifiGateWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strWifiDNS[GUI_IP_STR_LEN + 1];			//首选域名解析
	//char m_strWifiSecDNS[GUI_IP_STR_LEN + 1];			//备选域名解析
}TGuiWifiIpMsg;

//获取wifi网络设置ip参数回应
typedef struct
{
	TGuiWifiIpMsg m_tWifiIpMsg;
}TGuiGetWifiIpRsp;    

#define GUI_SET_WIFI_NET_CNF_REQ    100001 //设置wifi网络设置信息请求
#define GUI_SET_WIFI_NET_CNF_RSP    -100001//设置wifi网络设置信息回应
//设置wifi网络设置ip参数请求
typedef struct
{
	TGuiWifiIpMsg m_tWifiIpMsg;
}TGuiSetWifiIpReq;   


//设置wifi网络设置ip参数回应
#define GUI_SET_WIFI_NET_IP_OK			0				//设置WIFI IP地址成功
#define GUI_SET_WIFI_NET_IP_ERR			1				//设置WIFI IP地址失败
typedef struct
{
	int m_iRetCode; 						//返回码
	TGuiWifiIpMsg m_tWifiIpMsg;             //重设后的IP信息
}TGuiSetWifiIpRsp;

//wifi设置界面
#define GUI_GET_WIFI_PARA_CNF_REQ      100002   //获取wifi设置配置请求
#define GUI_GET_WIFI_PARA_CNF_RSP      -100002 //获取wifi设置配置回应

#define GUI_WIFI_PWD_STR_LEN           64
#define GUI_SAFE_WIFI_UNSET           0
#define GUI_SAFE_WIFI_SET             1

typedef struct
{
	//int m_iOnLineState;
	char m_strEssid[GUI_WIFIESSID_STR_LEN + 1];
	int m_iSetSafeEnable;
	int m_iSecurityType;
	int m_iKeyFormat;
	int m_iKeyType;
	int m_iKeySelect;
	char m_cPassword[GUI_WIFI_PWD_STR_LEN + 1];
}TGuiWifiParaMsg;

typedef struct								//获取wifi参数回应	
{
	TGuiWifiParaMsg m_tWifiParaMsg;						
} TGuiGetWifiParaCnfRsp;

#define GUI_SET_WIFI_PARA_CNF_REQ      100003   //设置wifi设置配置请求
#define GUI_SET_WIFI_PARA_CNF_RSP      -100003   //设置wifi设置配置回应
typedef struct								//设置wifi参数请求
{
	TGuiWifiParaMsg m_tWifiParaMsg;
} TGuiSetWifiParaCnfReq;

#define GUI_SET_WIFI_PARA_CNF_OK			0
#define GUI_SET_WIFI_PARA_CNF_ERR			1
typedef struct								//设置wifi参数回应
{
	int m_iRetCode;
}TGuiSetWifiParaCnfRsp;


//wifi设置界面-搜索命令
//wifi设置-搜索事件
#define GUI_GET_SEARCH_WIFI_NET_REQ    100004 //获取wifi可用网络参数请求
#define GUI_GET_SEARCH_WIFI_NET_RSP   -100004 //获取wifi 可用网络参数回应
typedef struct
{
	char m_cWifiName[GUI_WIFIESSID_STR_LEN + 1];
	char m_cWifiEssid[GUI_WIFIESSID_STR_LEN + 1];
	int m_iSafeEnable;
}TGuiWifiNetListItem;

//获取wifi网络参数回应
#define GUI_WIFI_SEARCH_OK    0   //wifi  执行搜索成功 
#define GUI_WIFI_SEARCH_ERR   1   //wifi 执行搜索 失败
typedef struct								
{
	int m_iWifiNetCnt;
	int m_iRetCode;
}TGuiGetWifiNetParaListRsp;

//=================WIFI=AP===================================
#define GUI_GET_WIFI_AP_CNF_REQ      100005		//获取WIFI AP 参数请求
#define GUI_GET_WIFI_AP_CNF_RSP      -100005	//获取WIFI AP 参数回应

typedef struct
{
	int m_iOnLineState;							//无线状态
	char m_strWifiIp[GUI_IP_STR_LEN + 1];		//ip地址
	char m_strWifiSubMsk[GUI_IP_STR_LEN + 1];	//子网掩码
	TGuiDhcpServerMsg m_tDhcpServerMsg;
	TGuiWifiParaMsg	m_tWifiParaMsg;
}TGuiWifiApMsg;

typedef struct
{
	TGuiWifiApMsg m_tWifiApMsg;
}TGuiGetWifiApRsp;								//获取WIFI AP 参数回应

#define GUI_SET_WIFI_AP_CNF_REQ			100006		//设置WIFI AP参数请求
#define GUI_SET_WIFI_AP_CNF_RSP			-100006		//设置WIFI AP参数回应

typedef struct
{
	TGuiWifiApMsg m_tWifiApMsg;
}TGuiSetWifiApReq;				//设置WIFIAP请求

#define GUI_SET_WIFI_AP_OK    0
#define GUI_SET_WIFI_AP_ERR	  1
typedef struct
{
	int m_iRetCode; 
}TGuiSetWifiApRsp;				//设置WIFIAP回应 


//=====================================================================
//WIFI新增协议

#define GUI_GET_WIFI_ENABLE_STAT_REQ	100021 //获取wifi使能状态请求
#define GUI_GET_WIFI_ENABLE_STAT_RSP	-100021 //获取wifi使能状态回应

#define GUI_WIFI_ENABLE_STAT    1  //使能
#define GUI_WIFI_DISENABLE_STAT 0  //不使能

typedef struct								
{	
	int m_iEnable;  
}TGuiGetWifiEnableStatRsp;

#define GUI_SET_WIFI_ENABLE_STAT_REQ	100022 //设置wifi使能状态请求
#define GUI_SET_WIFI_ENABLE_STAT_RSP	-100022 //设置wifi使能状态回应

typedef struct								
{	
	int m_iEnable;
}TGuiSetWifiEnableStatReq;

typedef struct
{
	int m_iRetCode; // GUI_SET_OK .GUI_SET_ERR
}TGuiSetWifiEnableStatRsp;


//============================
//设置wifi断开和连接

#define GUI_WIFI_DISCONNECT		0  //未连接
#define GUI_WIFI_CONNECT		1  //连接

#define GUI_GET_WIFI_CONNECT_STAT_REQ	100023 //获取wifi连接状态请求
#define GUI_GET_WIFI_CONNECT_STAT_RSP	-100023 //获取wifi连接状态回应

typedef struct								
{
	int m_iConnectCmd;	//0:断开，1：连接
}TGuiGetWifiConnectStaRsp;

#define GUI_SET_WIFI_CONNECT_STAT_REQ	100024 //设置连接或断开wifi请求
#define GUI_SET_WIFI_CONNECT_STAT_RSP	-100024 //设置连接或断开wifi回应

typedef struct								
{
	int m_iConnectStat;	//0:断开，1：连接
}TGuiSetWifiConnectCmdReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetWifiConnectCmdRsp;
//============================











//-----------------------------------------------------------------------------------------
#define GUI_CD_BACKUP_STOP_REQ			718	//强制停止光盘备份刻录请求
#define GUI_CD_BACKUP_STOP_RSP			-718 //强制停止光盘备份刻录请求回应

#define GUI_CD_BACKUP_STOP_OK      		0    //强制取消成功
#define GUI_CD_BACKUP_STOP_ERR     		1    //强制取消失败

typedef struct
{
	int m_iCdNo;
}TGuiCdBackupStopReq;     //强制停止光盘备份刻录请求

typedef struct				// 强制停止备份回应
{
	int m_iCdNo;
	int m_iRetCode;			
} TGuiCdBackupStopRsp;  	//强制停止光盘备份刻录请求回应


#define GUI_CD_BACKUP_START_REQ_EX		719	//刻录开始通知
#define GUI_CD_BACKUP_START_RSP_EX		-719 

typedef struct
{
	int m_iCdNo;              //光驱号 GUI_DISK_CD(原来逻辑) GUI_DISK_SEC_CD_ID (现在逻辑)
	int m_iCdBackUpMode;      //预留备份模式
}TGuiCdBackUpStartReqEx;    //扩展刻录备份开始协议请求

typedef struct
{
	int m_iCdNo;            //光驱号
	int m_iRetCode;
}TGuiCdBackUpStartRspEx; //扩展刻录备份开始协议回应


// 光盘实时刻录
#define GUI_CBL_MAX_CD		2

enum	//刻录模式
{
	GUI_CBL_MODE_SCD = 0,	//单盘刻录
	GUI_CBL_MODE_DCD = 1,	//双盘刻录
	GUI_CBL_MODE_CYC = 2,	//轮刻
	GUI_CBL_MODE_NSP = -1, //不支持实时刻录功能
};

typedef struct
{
	int m_iMode;				// 刻录模式
	int m_iCd;				// 光驱号,单盘刻录时使用(1, 2)
	int m_iChnEnBits; 			// 刻录通道使能位,bit:0-31，1-32通道
	int m_iReserved;			//通道数目大于32时使用
} TGuiCblModeMsg;

typedef struct				// 获取刻录模式回应
{
	TGuiCblModeMsg m_tMode;
} TGuiCblModeGetRsp;

typedef struct				// 启动实时刻录请求
{
	TGuiCblModeMsg m_tMode;
} TGuiCblStartReq;

typedef struct				// 启动实时刻录回应
{
	int m_iPid;				// 实时刻录操作号，-1表式启动失败
} TGuiCblStartRsp;



//=======================行业嵌入式==================
#define GUI_CBL_GET_MODEEX_REQ			733	//获取实时刻录扩展模式请求
#define GUI_CBL_GET_MODEEX_RSP			-733 //获取实时刻录扩展模式回应

#define GUI_CBL_DISC_LABEL_LEN       	63
typedef struct
{
	int m_iSealDisc;									//刻录完成是否封盘
	int m_iSpeed;										//刻录倍速,按倍速值为实际值，“最大”为16倍速
	int m_PopDisc;										//刻录完成是否弹盘
	int m_iDiscType;   									//光盘类型
	int m_iBurnTimes;  									//刻录时长
	char m_cDiscLabel[GUI_CBL_DISC_LABEL_LEN + 1];		//光盘标签
}TGuiCblModeMsgEx;

typedef struct
{
	TGuiCblModeMsgEx m_tModeEx;
}TGuiCblGetModeExRsp;

#define GUI_CBL_SET_MODEEX_REQ			734	//设置实时刻录扩展模式请求
#define GUI_CBL_SET_MODEEX_RSP			-734 //设置实时刻录扩展模式回应

typedef struct
{
	TGuiCblModeMsgEx m_tModeEx;
}TGuiCblSetModeExReq;

#define GUI_CBL_SET_MODEEX_OK  0   //设置成功
#define GUI_CBL_SET_MODEEX_ERR 1	//设置失败
typedef struct
{
	int m_iRetCode;
}TGuiCblSetModeExRsp;

#define GUI_CBL_SET_POP_DISC_REQ      735 //光盘出仓请求
#define GUI_CBL_SET_POP_DISC_RSP      -735 //光盘出仓回应

typedef struct
{
	int m_iMode;   //刻录模式
	int m_iCdNo;   // 0 --所有盘 (双盘和循环刻录所有盘出仓)，单盘时为获取的光驱号
}TGuiCblSetPopDiscReq;

typedef struct
{
	int m_iRetCode;
}TGuiCblSetPopDiscRsp;

#define GUI_ONEKEY_CBL_EX_REQ			736 //新增一键刻录请求
#define GUI_ONEKEY_CBL_EX_RSP			-736

typedef struct
{
	int m_iAct; //0表示开始一键刻录，1表示结束
}TGuiOneKeyCblExReq;

typedef struct
{
	int m_iRetCode;    //返回值含义同TGuiOneKeyCblRsp的m_iRetCode;
}TGuiOneKeyCblExRsp;


//关闭刻录窗体保存刻录模式
#define GUI_CBL_MODE_SET_REQ			725	//设置实时刻录模式请求(预留)
#define GUI_CBL_MODE_SET_RSP			-725	//设置实时刻录模式回应(预留)

typedef struct
{
	TGuiCblModeMsg m_tMode;
}TGuiSetCblModeReq;                   //设置刻录模式请求

typedef struct
{
	int m_iRetCode;
}TGuiSetCblModeRsp;                  //设置刻录模式回应

//======================================================




typedef struct				// 终止光盘实时刻录并打包命令
{
	int m_iPid;				// 操作号，由TGuiCblStartRsp给定
} TGuiCblEndReq;

#define GUI_CBL_END_OK		0
#define GUI_CBL_END_ERR		1
typedef struct				// 终止光盘实时刻录并打包命令
{
	int m_iRetCode;				// 操作号，由TGuiCblStartRsp给定
} TGuiCblEndRsp;


#define GUI_CBL_TAG_ALL_PID	-1
typedef struct				// 打点命令
{
	int m_iPid;				// 操作号，-1表式全部操作均打点
	int m_iChn;				// 通道号，默认0表所有通道
} TGuiCblTagReq;

#define GUI_CBL_TAG_OK 	0
#define GUI_CBL_TAG_ERR	1
typedef struct
{
	int m_iRetCode;
} TGuiCblTagRsp;

//==================一键刻录=====================
#define GUI_ONEKEY_CBL_REQ              732  //一键刻录请求
#define GUI_ONEKEY_CBL_RSP              -732 //一键刻录回应

typedef struct
{
	int m_iRetCode;
}TGuiOneKeyCblRsp;

//=================光盘文件查询==================
enum // 刻录状态
{
	GUI_CBL_STA_FREE	= 0, //空闲中，
	GUI_CBL_STA_DOING	= 1, //正在刻录，
	GUI_CBL_STA_PACK	= 2, //正在打包
};

enum // 光盘状态
{
	GUI_CBL_CD_STA_FREE	= 0, //光盘未使用
	GUI_CBL_CD_STA_BUSY	= 1, //光盘正在刻录
	GUI_CBL_CD_STA_BAD	= 2, //光盘已损坏
	GUI_CBL_CD_STA_FULL	= 3, //光盘已经满
	GUI_CBL_CD_STA_PACK	= 4, //光盘正在打包
	GUI_CBL_CD_STA_NODISK = 5, //没放入光盘
	GUI_CBL_CD_STA_CHECKDISK = 6,//光盘检测中
};

typedef struct		// 光盘录像文件查询请求
{
	int m_iCd;		// cd编号，默认0表式全部cd
	int m_iChn;		// 通道号, 0表示所有
	int m_iStartId;	// 按时间排序后记录的开始条数
	int m_iMaxCnt;	// 想要获取的记录条数
	int m_iStartTm;	// 开始时间(预留，默认为0)
	int m_iEndTm;	// 结束时间(预留，默认为0)
} TGuiCblFileQueryReq;

typedef struct
{
	int m_iCnt;		//查获取的数据条数(-1表失败)
} TGuiCblFileQueryRsp; //后紧跟m_iCnt条TGuiVideoFileMsg记录

//*******获取光盘录像文件打点信息***********//
#define GUI_CBL_TAG_QUERY_REQ			731	//获取打点信息请求
#define GUI_CBL_TAG_QUERY_RSP			-731

typedef struct		// 光盘录像打点信息查询请求
{
	int m_iCd;		// cd编号, 0表所有cd
	int m_iChn;		// 通道号, 0表所有通道
	int m_iStartId;	// 记录开始编号
	int m_iMaxCnt;	// 打点信息最大条数
	int m_iStartTm;	// 开始时间
	int m_iEndTm;	// 结束时间
} TGuiCblTagQueryReq;    //全部获取打点信息

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];
}TGuiCblTagQueryReqEx; //按文件获取打点信息

typedef struct
{
	int m_iCnt;		//查获的条数
} TGuiCblTagQueryRsp; //后紧跟m_iCnt条TGuiCblTagMsg记录;

typedef struct
{
	int m_iChn;
	int m_iStartTm;
	int m_iEndTm;
} TGuiCblTagMsg;

//*****获取硬盘录像文件的打点信息*********//
typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];
}TGuiGetMarkMsgReq;   //按文件获取打点信息

typedef struct
{
	int m_iCnt;		//查获的条数
} TGuiGetMarkMsgRsp; //后紧跟m_iCnt条TGuiCblTagMsg记录;

typedef struct
{
	int m_iChn;
	int m_iStartTm;
	int m_iEndTm;
} TGuiMarkMsg;


//================获取硬盘录像文件一段时间的打点信息============
#define GUI_GET_PERIODOFTIME_MARK_MSG_REQ   739  //获取一段时间内的打点信息请求
#define GUI_GET_PERIODOFTIME_MARK_MSG_RSP   -739 //获取一段时间内的打点信息回应
typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];  //录像文件名(可按照文件名查询)
	int m_iChn;								  //录像通道
	int m_iStartTm;							  //查询打点信息的开始时间
	int m_iEndTm;							  //查询打点信息的结束时间
}TGuiGetPeriodOfTimeMarkMsgReq;			  //获取一段时间内的打点信息请求

#define TGuiGetPeriodOfTimeMarkMsgRsp TGuiGetMarkMsgRsp  //获取一段时间内的打点信息回应
#define TGuiPeriodOfTimeMarkMsg TGuiMarkMsg				//打点信息结构体
	

//================通道管理相关协议=========================
typedef enum
{
	GUI_DIGITAL_CHN  = 0,//数字通道
	GUI_LOCAL_CHN,  //本地模拟通道
	GUI_VC_CHN,	     //合成通道
}TGuiChannelType; //通道类型

typedef struct
{
	int m_iChn; //通道号
}TGuiGetChnTypeReq;

typedef struct
{
	int m_iChn;
	int m_iType;  //通道类型
}TGuiGetChnTypeRsp;


#define GUI_STREAM_ALL_TYPE   0 //全部
#define GUI_STREAM_MAIN_TYPE  1 //主码流 
#define GUI_STREAM_SUB_TYPE   2 //副码流

#define GUI_GET_CHN_ENABLE_STAT_REQ   805  //获取通道连接状态请求
#define GUI_GET_CHN_ENABLE_STAT_RSP   -805  //获取通道连接状态回应
typedef struct
{
	int m_iChn;
	int m_iStreamType; //0-全部，1-主码流,2-副码流 ( 默认为1)
}TGuiGetChnEnableStatReq;
typedef struct
{
	int m_iChn;
	int m_iStat;//通道启用状态1-启用, 2-不启用，(扩展16位,0-通道状态，与原来兼容，1-已连接，2-未连接)
}TGuiGetChnEnableStatRsp;

#define GUI_SET_CHN_ENABLE_STAT_REQ   806  //设置通道连接状态请求
#define GUI_SET_CHN_ENABLE_STAT_RSP   -806  //设置通道连接状态回应


typedef struct
{
	int m_iChn;
	int m_iStat;//通道启用状态1-启用 ,2-不启用(扩展16位,0-通道状态，与原来兼容，1-已连接，2-未连接)
	int m_iStreamType; //0-全部，1-主码流,2-副码流(默认为0)
}TGuiSetChnEnableStatReq;  //设置通道连接状态请求
typedef struct
{
	int m_iRetCode;
}TGuiSetChnEnableStatRsp; //设置通道连接状态回应

//本地模拟通道
typedef struct
{
	int m_iChn;
}TGuiGetLocalChnAttrReq; //获取本地模拟通道请求

typedef struct
{
	int m_iChn;
	char m_cLocalChnAttr[32];
}TGuiGetLocalChnAttrRsp; //获取本地模拟通道回应

//合成通道
typedef struct
{
	int m_iChn;
}TGuiGetVcChnAttrReq; //获取合成通道请求

typedef struct
{
	int m_iChn;
	char m_cVcChnAttr[32];
}TGuiGetVcChnAttrRsp; //合成通道属性回应


// 数字通道相关协议

enum
{
	GUI_DIG_CHN_IP = 0,			//IP地址
	GUI_DIG_CHN_HOSTNAME,	 //域名
	GUI_DIG_CHN_IMD,			//主动模式
};

enum
{
	GUI_DIG_CHN_TCP = 0,	//tcp
	GUI_DIG_CHN_UDP,		//udp
	GUI_DIG_CHN_BC,		//多播
};

#define GUI_DIG_CHN_URL_LEN		39
#define GUI_DIG_CHN_PTZ_NAME		31
#define GUI_DIG_CHN_NAME_LEN		31

#define GUI_DIG_CHN_STAT_ENABLE    1
#define GUI_DIG_CHN_STAT_DISENABLE 2


//设备类型
enum
{
	GUI_DEV_TYPE_PRIVATE = 0, //设备类型为私有类型
	GUI_DEV_TYPE_ONVIF,      //设备类型为onvif
	GUI_DEV_TYPE_PUSH,    //设备类型为push流
	GUI_DEV_TYPE_RTSP,   //设备类型为RTSP
};

//连接模式
enum
{
	GUI_DIG_CHN_S_LNET = 0, //局域网
	GUI_DIG_CHN_S_HNAME, 	//域名
	GUI_DIG_CHN_S_AMOD,	//主动模式
};

#define GUI_DIG_AGLAIASENSE_DEV   GUI_DEV_TYPE_PRIVATE
#define GUI_DIG_ONVIF_DEV    GUI_DEV_TYPE_ONVIF
#define GUI_DIG_RTSP_DEV     GUI_DEV_TYPE_RTSP

/*
m_iConnectMod 表示如下:

0x00000000  - 私有 ip 
0x00000001 - 私有 域名
0x00000002 - 私有 主动
0x00010000 - onvif TCP
0x00010001 - onvif UDP
0x00020000 - PUSH
0x00030000 - RTSP
*/

typedef struct
{
	int m_iConnectMod;		//高16位0-私有，1-onvif，解析如上 
	char m_strUrl[GUI_DIG_CHN_URL_LEN  + 1]; //服务器地址
	char m_strProxy[GUI_DIG_CHN_URL_LEN  + 1]; //代理IP
	int m_iPort;	//端口号
	int m_iChn;	//对方通道号
	int m_iStreamType;	//码流类型,0:主码流, 1副码流
	int m_iConntectType;  //tcp/udp/bc
	int m_iPtzAddr;	//ptz地址
	char m_strPtzProtocol[GUI_DIG_CHN_PTZ_NAME + 1]; //ptz协议
	char m_strUserName[GUI_DIG_CHN_NAME_LEN + 1]; //用户名
	char m_strUserPsw[GUI_DIG_CHN_NAME_LEN + 1];	//密码
	int m_iEnable;    //状态
	char m_strVdecPsw[GUI_DIG_CHN_NAME_LEN + 1];	//视频解密密码
} TGuiDigChnMsg;


#define GUI_SET_DIG_CHN_OK 0
#define GUI_SET_DIG_CHN_ERR 1

#define GUI_DIG_CHN    0  //数字通道
#define GUI_SIM_CHN    1  //模拟通道
typedef struct		//设置数字通道请求
{
	int m_iType;		//类型:0设置数字通道, 1设置为模式通道(预留)
	int m_iChn;		//通道号
	TGuiDigChnMsg m_tChnMsg; //通道信息
} TGuiSetDigChnReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetDigChnRsp;

typedef struct		//获取数字通道信息请求
{
	int m_iChn;
} TGuiGetDigChnReq;

typedef struct
{
	int m_iChn;		// -1表失败
	TGuiDigChnMsg m_tChnMsg;
} TGuiGetDigChnRsp;

#define GUI_DIG_CHN_ID_LEN  39
typedef struct
{
	char m_strUrl[GUI_DIG_CHN_URL_LEN  + 1]; //服务器地址
	int m_iPort;	//端口号
	int m_iChn;	//对方通道号
	char m_strServerType[GUI_DIG_CHN_NAME_LEN + 1];//服务器类型
	char m_strDeviceID[GUI_DIG_CHN_ID_LEN + 1];//出厂设备ID
	char m_strServerName[GUI_DIG_CHN_NAME_LEN + 1];//服务器名称
	char m_strProxy[GUI_DIG_CHN_URL_LEN  + 1]; //代理IP
}TGuiSearchChnMsg;

//新加数字通道获取命令码
#define GUI_GET_SDIG_CHN_REQ_EX			745	//获取数字通道结果请求
#define GUI_GET_SDIG_CHN_RSP_EX			-745	//获取数字通道结果回应

typedef struct
{
	int m_iPort;	//端口号
	int m_iChn;	//对方通道号
	char m_strUrl[GUI_DIG_CHN_URL_LEN  + 1]; //服务器地址,IP地址
	char m_strServerType[GUI_DIG_CHN_NAME_LEN + 1];//服务器类型
	char m_strDeviceID[GUI_DIG_CHN_ID_LEN + 1];//出厂设备ID
	char m_strServerName[GUI_DIG_CHN_NAME_LEN + 1];//服务器名称
	char m_strProxy[GUI_DIG_CHN_URL_LEN  + 1]; //代理IP
	char m_strMac[GUI_IP_STR_LEN + 1];			//mac地址
	char m_strSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strGateWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];			//域名解析
}TGuiSearchChnMsgEx;


/*
m_iSearchMod 表示如下:

0x00000000  - 私有 ip 
0x00000001 - 私有 域名
0x00000002 - 私有 主动
0x00010000 - onvif TCP
0x00010001 - onvif UDP
0x00020000 - PUSH
0x00030000 - RTSP

*/
//m_iSearchMod 和m_iConnectMod是一样的
typedef struct		//搜索数字通道请求
{
	int m_iSearchMod;	//连接模式 高16 表示设备类型，0-私有，1-onvif, 
						//低16位表连接模式，私有协议下0表示
} TGuiSearchDigChnReq;

typedef struct
{
	int m_iSid;	//搜索查询号，以后按此号获取搜索结果，-1表失败
} TGuiSearchDigChnRsp;

typedef struct	//获取通道搜索结果请求
{
	int m_iSid;
} TGuiGetSearchedDigChnReq;

typedef struct	//回应，后跟m_iCnt个TGuiSearchDigChnMsg结构
{
	int m_iCnt;	//此次返回的通道条数，可以是0，-1表式搜索结束
} TGuiGetSearchedDigChnRsp;

typedef struct 								//获取数字通道协议列表请求
{
	int m_iChn; //数字通道号默认为0
} TGuiGetDigChnProtocolListReq;

typedef struct 								//获取数字通道协议列表回应
{
	int m_iChn;                             //数字通道号
	int m_iProtocolCnt;						//协议总数
} TGuiGetDigChnProtocolListRsp;

typedef struct 								//协议列表项
{
	char m_strProtocolName[GUI_DIG_CHN_PTZ_NAME + 1]; //协议名
} TGuiDigChnProtocolListItem;

//数字通道编辑类型
#define GUI_SET_CHN_TYPE_PARAM  -1
typedef struct 
{
	int m_iChn;
	int m_iType;
	int m_iParam; //预留 默认设为-1
}TGuiChnMagSetChnTypeReq; //设置通道类型请求

typedef struct
{
	int m_iRetCode;
}TGuiChnMagSetChnTypeRsp; //设置通道类型回应

typedef struct
{
	int m_iChn;
}TGuiChnGetChnEditAbleTypeReq; //获取可编辑通道类型请求

typedef struct
{
	int m_iChn;
	int m_iEditTypeFlg;
}TGuiChnGetChnEditAbleTypeRsp;  //获取可编辑通道类型回应


//---------------------网络前端即插即用-------------------------------
typedef enum                  
{
	GUI_MANUALADD = 0,						//手动添加即不启用即插即用
	GUI_AUTOADD,							//自动添加
	GUI_PROMPTADD,							//提示添加
} EGuiChnAddType;                         //添加数字通道的方式

#define GUI_GET_DIG_CHN_ADD_TYPE_REQ   807  //获取数字通道添加方式请求
#define GUI_GET_DIG_CHN_ADD_TYPE_RSP   -807 //获取数字通道添加方式回应

typedef struct
{
	int m_iAddType;	//数字通道添加方式，0表示不启用，1表示自动添加，2表示提示添加
}TGuiGetDigChnAddTypeRsp;				//获取添加方式回应

#define GUI_SET_DIG_CHN_ADD_TYPE_REQ   808  //设置数字通道添加方式请求
#define GUI_SET_DIG_CHN_ADD_TYPE_RSP   -808  //设置数字通道添加方式回应

#define GUI_SET_DIG_ADD_TYPE_OK   0       //设置成功
#define GUI_SET_DIG_ADD_TYPE_ERR  1       //设置失败

typedef struct
{
	int m_iAddType;	//数字通道添加方式，1表示手动添加，2表示提示添加，3表示自动添加
}TGuiSetDigChnAddtypeReq;	//设置数字通道添加方式请求

typedef struct
{
	int m_iRetCode;
}TGuiSetDigChnAddTypeRsp;	//设置数字通道添加方式回应

#define GUI_GET_DIG_CHN_UPNP_NEW_DEVICE_REQ  809     //即插即用获取新设备请求
#define GUI_GET_DIG_CHN_UPNP_NEW_DEVICE_RSP  -809    //即插即用获取新设备回应
typedef struct
{
	int m_iCnt;   //默认为1
}TGuiGetDigChnUpnpReq;

typedef struct
{
	int m_iCnt;//返回发现的设备个数
}TGuiGetDigChnUpnpDevCntRsp;

typedef struct
{
	int m_iConStat;		//0 - 设备丢失连接，1-发现新设备
	int m_iChn;				//欲添加设备到的通道号
	int m_iConId;        //欲添加设备的连接ID
	char m_strMac[GUI_IP_STR_LEN + 1]; //欲添加设备的mac地址
	char m_strIp[GUI_IP_STR_LEN + 1]; //欲添加设备的IP地址
}TGuiGetDigChnUpnpDevMsgItem;

#define GUI_SET_ADD_DIG_CHN_UPNP_REQ          810    //设置添加数字通道请求
#define GUI_SET_ADD_DIG_CHN_UPNP_RSP          -810    //设置添加数字通道回应

typedef struct		//设置添加数字通道请求
{
	int m_iConCmd;      // 1-连接，0-不连接
	int m_iChn;		//欲添加设备到通道
	int m_iConId;        //欲添加设备的连接ID
	char m_strMac[GUI_IP_STR_LEN + 1]; //欲添加设备的mac地址
	char m_strIp[GUI_IP_STR_LEN + 1]; //欲添加设备的IP地址
} TGuiSetAddDigChnUpnpReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetAddDigChnUpnpRsp;

//------------------------------------------------------------------------------
// 网络连接数字通道剩余带宽

#define GUI_GET_NET_SPARE_BANDWIDTH_REQ  811  //获取网络连接剩余带宽请求
#define GUI_GET_NET_SPARE_BANDWIDTH_RSP  -811 //获取网络连接剩余带宽回应

typedef struct
{
	char m_cBandWidth[GUI_PUBLIC_LEN_31 + 1]; //传字符串
}TGuiGetNetSpareBandwidthRsp;

//==============IPC集中管理=====================
typedef struct
{
	int m_iChn;
	char m_strIpcVersion[GUI_PUBLIC_LEN_31 + 1];  	//版本号
}TGuiIPCMsg;

//获取IPC通道的设备号及版本号
#define GUI_GET_IPC_VERSION_REQ	812   
#define GUI_GET_IPC_VERSION_RSP	-812
typedef struct
{
	int m_iChn;
}TGuiGetIpcVerReq;

typedef struct
{
	TGuiIPCMsg m_tIpcMsg;
} TGuiGetIpcVerRsp;

//修改IP
#define GUI_IPC_CHANGE_IP_CMD_REQ	813  //修改前端IPC的IP请求
#define GUI_IPC_CHANGE_IP_CMD_RSP	-813  //回应

typedef struct
{
	int m_iChn;
	char m_strMac[GUI_IP_STR_LEN + 1];			//mac地址
	char m_strIp[GUI_IP_STR_LEN + 1];			//IP地址
	char m_strSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strGateWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];			//域名解析
}TGuiChangeIpcIpReq;

typedef struct
{
	int m_iRetCode;
} TGuiChangeIpcIpRsp;

//前端IPC重启
#define GUI_IPC_REBOOT_CMD_REQ		814  //IPC重启
#define GUI_IPC_REBOOT_CMD_RSP		-814 //回应
typedef struct
{
	int m_iChn;
}TGuiIpcRebootReq;           //重启请求

typedef struct
{
	int m_iRetCode;
} TGuiIpcRebootRsp;

#define GUI_IPC_CNF_EXPORT_REQ		815  //IPC配置导出请求
#define GUI_IPC_CNF_EXPORT_RSP		-815  //IPC配置导出回应
typedef struct
{
	int m_iChn;
 	char m_iDstDevName[GUI_CNF_NAME_LEN + 1];	//导出配置文件磁盘名称
	char m_strFileName[GUI_CNF_NAME_LEN + 1];  //导出配置文件名
}TGuiIpcExportReq;

typedef struct
{
	int m_iRetCode;
}TGuiIpcExportRsp;

#define GUI_IPC_CNF_IMPORT_REQ		816  //IPC导入请求
#define GUI_IPC_CNF_IMPORT_RSP		-816
//仿照以前的导入逻辑，只是在发送时，增加通道号
typedef struct
{
	int m_iChn;
char m_strFileName[GUI_CNF_NAME_LEN + 1]; //导入配置文件名
}TGuiIpcImportReq;

typedef struct
{
	int m_iRetCode;
}TGuiIpcImportRsp;

//IPC升级
#define GUI_IPC_UPDATE_CMD_REQ		817   //IPC升级
#define GUI_IPC_UPDATE_CMD_RSP		-817

typedef struct
{
	int m_iChn;
	char m_strFileName[GUI_SYS_UPDATE_FTP_LEN + 1];  //升级文件名
}TGuiIpcUpdateReq;

typedef struct
{
	int m_iRetCode;
}TGuiIpcUpdateRsp;

#define GUI_IPC_UPDATE_STA_REQ		818
#define GUI_IPC_UPDATE_STA_RSP		-818
//使用旧的结构体TGuiSysUpdateStaReq和TGuiSysUpdateStaRsp

#define GUI_IPC_FILE_UPDATE_NUM_REQ		819	//获取IPC升级文件个数请求
#define GUI_IPC_FILE_UPDATE_NUM_RSP		-819  //回应
//使用旧的结构体TGuiGetSysUpdateFileListRsp和TGuiSysUpdateFileRsp


//------------------------------------------------------------------------------
// 视频加密解密
#define GUI_VIDEO_PWD_LEN  31    //密码长度

#define GUI_VDECODE_PWD_OK 		0   //验证成功
#define GUI_VDECODE_PWD_ERR 	1  //验证失败
#define GUI_VDECODE_PWD_ERR1 	2 //刻录过程中加密失败


typedef struct	//设置视频解密密码请求
{
	int m_iPlayId;
	char m_strPwd[GUI_VIDEO_PWD_LEN + 1];
}TGuiVdecodePwdSetReq;

typedef struct
{
	int m_iRetCode;
}TGuiVdecodePwdSetRsp;

typedef struct	//设置视频编码加密密码请求
{
	int m_iChn;	//通道号，预留，默认为0
	char m_strPwd[GUI_VIDEO_PWD_LEN + 1]; //密码
} TGuiVEncodePwdSetReq;

typedef struct
{
	int m_iRetCode;	// 0成功，1失败
} TGuiVEncodePwdSetRsp;

typedef struct	//获取视频加密请求
{
	int m_iChn;	//通道号，预留，默认0
} TGuiVencodePwdGetReq;

typedef struct
{
	int m_iChn;	//通道号，与req中的通道号相等，-1表式失败
	char m_strPwd[GUI_VIDEO_PWD_LEN + 1]; //密码
} TGuiVencodePwdGetRsp;

enum
{
	GUI_AI_DEV_CHN_ALL_ID = 0,   //输入设备通道号，0表示全部通道
	GUI_AO_DEV_ID = 1001,
};

typedef struct		//音量控制请求
{
	int m_iDevId;	//声音输入设通道号(保留字，默认为0表全部通道), GUI_AO_MIC_DEV_ID表ao输出
	int m_iVolume;	//音量大小 0-255(0:静音,255:最大音量, -1表式查询当前音量)
} TGuiVolumeCtrReq;

typedef struct
{
	int m_iDevId;
	int m_iVolume;	//当前的音量值
} TGuiVolumeCtrRsp;

// 码流传输
typedef struct		//视频请求连接
{
	int m_iPid;	//程序标识
	int m_iChn;	//通道号
} TGuiStreamConnectReq;

typedef struct		//视频请求回应
{
	int m_iKey;	//共享内存key值(负值表式失败)
	int m_iSize;	//共享内存大小
} TGuiStreamConnectRsp;

typedef struct		//视频断开请求
{
	int m_iPid;	//程序标识
	int m_iChn;	//通道号
} TGuiStreamDisConnectReq;

typedef struct		//视频断开回应
{
	int m_iRetCode;	//返回码，0表式成功
} TGuiStreamDisConnectRsp;

typedef struct		//码流传输通道
{
	int m_iBase;
	int m_iTop;
	int m_iSize;
	char m_cBuf[0];
} TGuiStreamChannel;

typedef struct						//码流数据信息
{
	unsigned char m_ucFrameType;	//帧数据类型, 0x00:I帧,0x01:P帧,0xFF:填充数据(直接丢弃)
	unsigned char m_ucFrameRate;		//帧率
	unsigned char m_ucR1;			//保留字
	unsigned char m_ucR2;			//保留字
	unsigned int m_iFrameLenght;		//帧长度
	unsigned char m_ucStream[0];		//帧数据
} TGuiStreamFrame;


typedef struct		//强制I帧请求
{
	int m_iChn;		//通道号,0表示全部
} TGuiForceIframeReq;

typedef struct
{
	int m_iRetCode;
} TGuiForceIframeRsp;

//==============智能分析=================================
//录像定义

#define MAX_GUI_RULE_NAME_LEN		16
#define MAX_GUI_POLYGON_POINT_NUM	8
#define MAX_GUI_FDEPTH_LINE_NUM 	3

//智能分析通用类型
typedef struct
{
	int m_iX;
	int	m_iY;
} TGuiVcaPoint;

typedef struct
{
	TGuiVcaPoint 	m_stStart;	
	TGuiVcaPoint 	m_stEnd;	
} TGuiVcaLine;

typedef struct
{
	int 			m_iPointNum;
	TGuiVcaPoint 	m_stPoint[MAX_GUI_POLYGON_POINT_NUM];   	
} TGuiVcaPolygon;


//智能分析通道使能设置/获取
#define GUI_GET_VCACHN_CNF_REQ				 900		//获取智能分析通道使能请求
#define GUI_GET_VCACHN_CNF_RSP				-900		//获取智能分析通道使能回应

typedef struct
{
	int m_iChannelNo;
} TGuiGetVcaChnEnableReq;  //获取智能分析通道使能请求

enum
{
	GUI_VCA_DISABLE = 0,    //不使能智能分析
	GUI_VCA_USE_LOCAL,      //使能本地通道智能分析
	GUI_VCA_USE_REMOTE,		//使能前端通道智能分析
}; //智能分析使能标识

typedef struct
{
	int m_iChannelNo;
	int m_iVcaEnable;		//智能分析使能标识
} TGuiGetVcaChnEnableRsp;

#define GUI_SET_VCACHN_CNF_REQ				 901		//设置智能分析通道使能请求
#define GUI_SET_VCACHN_CNF_RSP				-901		//设置智能分析通道使能回应

typedef struct
{
	int m_iChannelNo;
	int m_iVcaEnable;		//智能分析使能位		0-不使能此通道智能分析	1-使能此通道智能分析
} TGuiSetVcaChnEnableReq;

#define GUI_SET_VCACHN_OK		0
#define GUI_SET_VCACHN_ERR		1
typedef struct
{
	int m_iRetCode;			//返回值,0表示成功，其余值表失败
} TGuiSetVcaChnEnableRsp;

//智能分析参数设置/获取
#define GUI_GET_VCAPARA_CNF_REQ		 		 902		//获取智能分析基本参数请求
#define GUI_GET_VCAPARA_CNF_RSP				-902		//获取智能分析基本参数回应

typedef struct
{
	//int m_iVcaEnable;				//智能分析使能位		0-不使能此通道智能分析	1-使能此通道智能分析
	int m_iVcaVideoSize;			//智能分析库视频大小	0-QCIF 1-HCIF 2-FCIF 3-HD1 4-FD1 目前只支持QCIF/FCIF
	int m_iDisplayTarget;			//视频叠加目标			0-不叠加	  1-叠加		
	int m_iDisplayTrace;			//视频叠加轨迹			0-不叠加	  1-叠加
	int m_iDisplayAllTargetTrace;	//是否叠加所有目标和轨迹0-只叠加触发警情的目标、轨迹	1-叠加所有目标、轨迹
	int m_iTargetColor;				//目标轨迹颜色			0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int m_iTargetAlarmColor;		//目标轨迹报警颜色		0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int m_iTraceLength;				//轨迹长度				0~40
} TGuiVcaParaCnf;

typedef struct
{
	int m_iEnableAdvanced;	//智能分析高级参数使能	0-不使能	1-使能					
	int m_iTargetMinSize;	//目标最小像素数
	int m_iTargetMaxSize;	//目标最大像素数
	int m_iTargetMinWidth;	//最小宽度
	int m_iTargetMaxWidth;	//最大宽度
	int m_iTargetMinHeight;	//最小高度
	int m_iTargetMaxHeight;	//最大高度
	int m_iTargetMinWHRatio;//目标的最小长宽比*100
	int m_iTargetMaxWHRatio;//目标的最大长宽比*100
	int m_iTargetMinSpeed;	//目标最小像素速度
	int m_iTargetMaxSpeed;	//目标最大像素速度
	int m_iSensitivity;		//目标输出灵敏度级别	0:低灵敏度 1:中灵敏度 2:高灵敏度
} TGuiVcaAdvParaCnf;

typedef struct 
{
	TGuiVcaLine m_stRefLine;		// 垂直线段坐标(unit: pixel)
	int 		m_s32RefLen;		// 线段长度(unit: cm)
} TGuiVcaFdeLine;	// 景深线结构

typedef struct
{
	unsigned int	m_i32NumUsed; 							// 样本数目 大于等于2
	TGuiVcaFdeLine 	m_stLines[MAX_GUI_FDEPTH_LINE_NUM];   	// 样本数目
} TGuiVcaFdepthMeasure;  

typedef struct
{
	unsigned int 			m_iEnableFdepth;	//启用标识
	TGuiVcaFdepthMeasure 	m_stMeasure;		//标定数据
} TGuiVcaFdeParaCnf;

typedef struct	//获取智能分析参数配置(通用/高级)请求
{
	int m_iChannelNo;						//智能分析通道号
} TGuiGetVcaParaReq;

typedef struct	//获取智能分析参数配置(通用/高级)回应	
{
	int m_iChannelNo;		
	TGuiVcaParaCnf		m_tVcaParaCnf;		//智能分析通用配置参数
	TGuiVcaAdvParaCnf	m_tVcaAdvParaCnf;	//智能分析高级配置参数
	TGuiVcaFdeParaCnf	m_tVcaFdeParaCnf;	//智能分析标定配置参数
} TGuiGetVcaParaRsp;

#define GUI_SET_VCAPARA_CNF_REQ		 		 903		//设置智能分析基本参数请求	
#define GUI_SET_VCAPARA_CNF_RSP				-903		//设置智能分析基本参数回应

typedef struct	//设置智能分析参数澹(通用/高级)请求
{
	int m_iChannelNo;		
	TGuiVcaParaCnf		m_tVcaParaCnf;
	TGuiVcaAdvParaCnf	m_tVcaAdvParaCnf;
	TGuiVcaFdeParaCnf	m_tVcaFdeParaCnf;
} TGuiSetVcaParaReq; 

#define GUI_SET_VCAPARA_OK		0
#define GUI_SET_VCAPARA_ERR		1
typedef struct	//设置智能分析参数澹(通用/高级)回应
{
	int m_iRetCode;							//返回值,0表示成功，其余值表失败
} TGuiSetVcaParaRsp;

//智能分析规则事件参数设置/获取
#define GUI_GET_VCAEVENT_CNF_REQ			 904		//获取智能分析事件参数请求
#define GUI_GET_VCAEVENT_CNF_RSP			-904		//获取智能分析事件参数回应

typedef enum  
{
	GUI_TRIPWIRE = 0,						//绊线事件
	GUI_DBTRIPWIRE,							//双绊线事件
	GUI_PERIMETER,							//周界事件
	GUI_LOITER,								//徘徊事件
	GUI_PARKING,							//停车事件
	GUI_RUN,								//奔跑事件
	GUI_HIGH_DENSITY,						//人员密度事件
	GUI_ABANDUM,							//遗弃物事件
	GUI_OBJSTOLEN,							//被盗物事件
	GUI_FACESPOT,           				// 人脸识别
	GUI_AVD,								//视频诊断
	GUI_CAP,								//目标跟踪
	GUI_MAX
} EGuiVcaEventType;

typedef struct
{
	int				m_iEventValid;		//是否使能单绊线事件
	int				m_iTargetTypeCheck;	//目标检测类型
	int				m_iMinDistance;		//最小距离
	int				m_iMinTime;			//最短时间
	int				m_iDisplayRule;		//是否显示规则
	int				m_iDisplayStat;		//是否显示报警计数统计
	int				m_iColor;			//单绊线颜色	0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int				m_iAlarmColor;		//报警绊线颜色	0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int				m_iType;			//穿越类型		0:单向 1:双向
	int				m_iTDirection;		//禁止穿越方向	0~359度
	TGuiVcaLine		m_stLine;			//单绊线坐标
} TGuiVcaTripwire;

typedef struct
{
	int				m_iEventValid;		//是否使能周界事件
	int				m_iDisplayRule;		//是否显示规则
	int				m_iDisplayStat;		//是否显示报警计数统计
	int				m_iColor;			//周界颜色		0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int				m_iAlarmColor;		//报警周界颜色	0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int				m_iTargetTypeCheck;	//目标检测类型
	int				m_iMode;			//周界检测模式	0:入侵 1:进入 2:离开
	int 			m_iMinDistance;		//最小距离					
	int 			m_iMinTime;			//最短时间				
	int				m_iType;			//是否做方向限制
	int 			m_iDirection;		//禁止方向角度	0~359度
	TGuiVcaPolygon	m_stRegion;			//周界坐标
} TGuiVcaPermeter;


typedef struct
{	
	int 			m_iEventValid;		//是否使能人脸侦测事件
	int 			m_iDisplayRule;		//是否显示规则
	int 			m_iDisplayStat;		//是否显示报警计数统计
	int				m_iColor;			//侦测轮廓颜色		0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int 			m_iAlarmColor;		//报警侦测轮廓颜色	0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	TGuiVcaPolygon	m_stRegion;			//人脸侦测区域坐标
} TGuiVcaFace;

typedef struct
{
	int				m_iEventValid;		//是否使能视频诊断事件
	int				m_iDisplayStat;		//是否显示报警计数统计
	int				m_iCheckTime;		//事件有效事件阀值，当事件发生超过此阀值时，认为警情发生
} TGuiVcaDiagnose;


typedef struct
{
    int				m_iEventValid;
	int				m_iDisplayRule;
	int				m_iDisplayStat;
	int				m_iColor;
	int				m_iAlarmColor;	
	int				m_iMinTime;
	int				m_iMinSize;
	int 			m_iMaxSize;	
	int				m_iChildAreaNum;    //无效子区域个数
	TGuiVcaPolygon	m_stMainRegion;	    //最外的多边形区域	
    TGuiVcaPolygon	m_stRegionA;	    //子区域A
	TGuiVcaPolygon	m_stRegionB;        //子区域B
	TGuiVcaPolygon	m_stRegionC;        //子区域C
}TGuiVcaLeave;
#define TGuiVcaLost TGuiVcaLeave


typedef struct
{
	TGuiVcaTripwire	m_tVcaTripwireCnf;		//单绊线事件具体配置参数
	TGuiVcaPermeter m_tVcaPermeterCnf;		//周界事件具体配置参数
	TGuiVcaFace		m_tVcaFaceCnf;			//人脸侦测事件具体配置参数
	TGuiVcaDiagnose m_tVcaDiagnoseCnf;		//视频诊断事件具体配置参数
	TGuiVcaLeave    m_tVcaLeaveCnf;         //物品遗留事件具体配置参数
	TGuiVcaLost     m_tVcaLostCnf;          //物品丢失事件具体配置参数
} TGuiVcaEventCnf;


typedef struct	//获取智能分析具体规则、事件参数请求
{
	int 				m_iChannelNo;		//通道号		
	int 				m_iRulesNo;			//事件号
	EGuiVcaEventType 	m_eEventType;		//事件类型			
} TGuiGetVcaEventReq;

typedef struct	//获取智能分析具体规则、事件参数回应
{
	int 				m_iChannelNo;		//通道号
	int 				m_iRulesNo;			//事件号
	unsigned char		m_cRuleName[MAX_GUI_RULE_NAME_LEN + 1];	//事件名称
	int 				m_iRuleEnable;		//规则是否使能
	EGuiVcaEventType 	m_eEventType;		//事件类型			
	TGuiVcaEventCnf 	m_tVcaEventCnf;		//事件具体配置参数
} TGuiGetVcaEventRsp;

#define	GUI_SET_VCAEVENT_CNF_REQ	 		 905		//设置智能分析事件参数请求
#define GUI_SET_VCAEVENT_CNF_RSP			-905		//设置智能分析事件参数回应

typedef struct	//设置智能分析具体规则、事件参数请求
{
	int 				m_iChannelNo;
	int 				m_iRulesNo;
	unsigned char		m_cRuleName[MAX_GUI_RULE_NAME_LEN + 1];
	int 				m_iRuleEnable;
	EGuiVcaEventType 	m_eEventType;					
	TGuiVcaEventCnf 	m_tVcaEventCnf;
} TGuiSetVcaEventReq;

#define GUI_SET_VCAEVENT_OK		0
#define GUI_SET_VCAEVENT_ERR	1
typedef struct	//设置智能分析具体规则、事件参数回应
{
	int m_iRetCode;			//返回值,0表示成功，其余值表失败
} TGuiSetVcaEventRsp;

//智能分析报警计数统计获取
#define GUI_GET_VCA_ALARM_COUNT_TOTAL_REQ	 	906		//获取智能分析报警计数请求
#define GUI_GET_VCA_ALARM_COUNT_TOTAL_RSP		-906		//获取智能分析报警计数回应

typedef struct
{
	int 				m_iChannelNo;
	int 				m_iRulesNo;
	EGuiVcaEventType 	m_eEventType;
} TGuiVcaAlarmCountCnf;

typedef struct	
{
	TGuiVcaAlarmCountCnf m_VcaAlarmCountCnf;
} TGuiGetVcaAlarmCountTotalReq;

#define GUI_GET_ALARM_COUNT_TOTAL_OK	0
#define GUI_GET_ALARM_COUNT_TOTAL_ERR	1
typedef struct
{
	TGuiVcaAlarmCountCnf 	m_VcaAlarmCountCnf;
	int 					m_iVcaAlarmCount;
} TGuiGetVcaAlarmCountTotalRsp;

//智能分析报警计数清零设置
#define GUI_SET_VCA_ALARM_COUNT_CLEAN_REQ		 907		//设置智能分析报警清零请求
#define GUI_SET_VCA_ALARM_COUNT_CLEAN_RSP		-907		//设置智能分析报警清零回应

typedef struct
{
	TGuiVcaAlarmCountCnf m_VcaAlarmCountCnf;
} TGuiSetVcaCleanAlarmCountReq;

#define GUI_SET_ALARM_COUNT_CLEAN_OK	0
#define GUI_SET_ALARM_COUNT_CLEAN_ERR	1
typedef struct
{
	int m_iRetCode;			//返回值,0表示成功，其余值表失败
} TGuiSetVcaCleanAlarmCountRsp;

//智能分析报警状态获取
#define GUI_GET_VCA_ALARM_STATE_MSG_REQ		 	908		//获取智能分析报警消息请求
#define GUI_GET_VCA_ALARM_STATE_MSG_RSP			-908		//获取智能分析报警消息回应

typedef	struct	
{
	TGuiVcaPoint 		m_stLeftTopPoint;	
	TGuiVcaPoint 		m_stRightBottomPoint;
} TGuiVcaTargetCoordinate;

typedef struct
{
	int 					m_tTime;			//警情发生时间
	int 					m_iState;			//报警状态
	unsigned int 			m_uiTargetId;		//目标ID
	int						m_iTargetType;		//目标类型
	TGuiVcaTargetCoordinate	m_stTargetRegion;	//目标区域坐标
	int						m_iTargetSpeed;		//目标速度
	int						m_iTargetDirection;	//目标运动方向	0~359
} TGuiVcaAlarmStateMsg;

typedef struct	//获取智能分析时时报警个数回应
{
	int 	m_iTol;
} TGuiGetVcaAlarmStateMsgRsp;

typedef struct	
{		
	int 					m_iChannelNo;		//通道号
	int 					m_iRulesNo;			//规则号
	EGuiVcaEventType		m_eEventType;		//事件类型
	TGuiVcaAlarmStateMsg	m_VcaAlarmStateMsg;	//报警状态消息
} TGuiGetVcaAlarmStateMsg;


#define	GUI_GET_VCA_ALARM_REQ				 909		//获取智能分析报警配置信息请求
#define GUI_GET_VCA_ALARM_RSP				-909		//获取智能分析报警配置信息回应
typedef	struct											//报警配置
{
	int m_iEnable;										//报警使能 				0/1
	int m_iSoundEnable;									//声音使能 				0/1
	int m_iDisplayEnable;								//屏幕显示使能 			0/1
	int m_iToServerEnalbe;								
	int m_iVideoEnable[GUI_DVR_CHN_CNT];				//联动各通道录像使能 	0/1
	int m_iPicEnable[GUI_DVR_CHN_CNT];					//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];		//联动报警输出使能 		0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];	//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];		//ptz联动
} TGuiVcaAlarmCnf;

typedef struct	//获取智能分析报警信息参数请求
{
	int 						m_iChannelNo;			//通道号
	int 						m_iRulesNo;				//规则号
	EGuiVcaEventType			m_eEventType;			//事件类型
} TGuiGetVcaAlarmReq;

typedef struct	//获取智能分析报警信息参数回应
{
	int 						m_iChannelNo;			//通道号
	int 						m_iRulesNo;				//规则号
	EGuiVcaEventType			m_eEventType;			//事件类型
	TGuiVcaAlarmCnf 			m_tAlarmCnf;			//智能分析报警配置	
} TGuiGetVcaAlarmRsp;

#define GUI_SET_VCA_ALARM_REQ				 910		//设置智能分析报警配置信息请求
#define GUI_SET_VCA_ALARM_RSP				-910		//设置智能分析报警配置信息回应

typedef struct	//设置智能分析报警信息参数请求
{
	int 						m_iChannelNo;			//通道号
	int 						m_iRulesNo;				//规则号
	EGuiVcaEventType			m_eEventType;			//事件类型
	TGuiVcaAlarmCnf 			m_tAlarmCnf;			//智能分析报警配置
} TGuiSetVcaAlarmReq;

#define GUI_SET_VCA_ALARM_OK		0
#define GUI_SET_VCA_ALARM_ERR		1
typedef struct	//设置智能分析报警信息参数回应
{
	int m_iRetCode;
} TGuiSetVcaAlarmRsp;


#define GUI_SET_VCA_REBOOT_REQ				 911		//重启智能库请求
#define GUI_SET_VCA_REBOOT_RSP				-911		//重启智能库回应

typedef struct
{
	int	m_iChannel;			//因为改变了某个通道参数而重启的通道号
} TGuiSetVcaRebootReq;

#define GUI_SET_VCA_REBOOT_OK	0
#define GUI_SET_VCA_REBOOT_ERR	1

typedef struct
{
	int m_iRetCode;			//返回值,0表示成功，其余值表失败
} TGuiSetVcaRebootRsp;

//获取智能分析规则事件使能状态
#define GUI_GET_VCA_RULE_EVENT_ENABLE_REQ	 	912		//获取智能分析规则事件使能状态请求
#define GUI_GET_VCA_RULE_EVENT_ENABLE_RSP		-912		//获取智能分析规则事件使能状态回应
typedef struct
{
	int m_iChannel;
	int m_iRulesNo;
} TGuiGetVcaRuleEventEnableReq;

typedef struct
{
	int m_iRuleEnable;			//规则是否使能
	int m_iTripwireEnable;		//单绊线事件是否使能
	int m_iPerimeterEnable;		//周界事件是否使能
	int m_iFaceEnable;			//人脸侦测事件是否使能
	int m_iDiagnoseEnable;		//视频诊断事件是否使能
	int m_iLeaveEnable;         //物品遗留事件是否使能
	int m_iLostEnabe;           //物品丢失事件是否使能
} TGuiVcaRuleEnableMsg;

typedef struct
{
	int 					m_iChannel;
	int 					m_iRulesNo;
	TGuiVcaRuleEnableMsg	m_iEnableMsg;
} TGuiGetVcaRuleEventEnableRsp;




// ================================================================
// 2010-11-9 协议扩展

// ========用户管理=======================================
// 获取用户信息协议(扩展)
#define GUI_GET_USER_MSG_EX_REQ	10000
#define GUI_GET_USER_MSG_EX_RSP	-10000

typedef struct
{
	char m_strUser[GUI_USER_NAME_LEN + 1];
} TGuiGetUserMsgExReq;

typedef struct
{
	int m_iGid;	//用户组
	char m_strUser[GUI_USER_NAME_LEN + 1];
	char m_strPsw[GUI_USER_PSW_LEN + 1];
} TGuiGetUserMsgExRsp;

// =====================字符叠加相关设置=============

#define GUI_SET_OSD_OK   0
#define GUI_SET_OSD_ERR  1
//===================扩展 通道名称叠加====================
#define GUI_GET_OSD_CHN_NAME_EX_REQ		1008		//扩展的获取通道名称叠加请求
#define GUI_GET_OSD_CHN_NAME_EX_RSP		-1008		//扩展的获取通道名称叠加回应
#define GUI_SET_OSD_CHN_NAME_EX_REQ		1009		//扩展的设置通道名称叠加请求
#define GUI_SET_OSD_CHN_NAME_EX_RSP		-1009		//扩展的设置通道名称叠加回应

typedef struct
{
	int m_iDsp;		//是否显示
	int m_iColor;		//叠加颜色
	int m_iLocType;	//坐标类型:0 绝对坐标, 1 : 24x16小方格
	int m_iXp;
	int m_iYp;
	char m_strName[0];  //实际的通道名称数据
} TGuiOsdChnNameCnfEx;  //扩展的设置通道名称的结构体

typedef struct
{
	int m_iChn;
} TGuiGetOsdChnNameExReq;		//扩展的获取通道名称叠加请求

typedef struct
{
	int m_iChn;
	TGuiOsdChnNameCnfEx m_tCnf;
} TGuiGetOsdChnNameExRsp; //扩展的获取通道名称叠加回应

typedef struct
{
	int m_iChn;		//通道号
	TGuiOsdChnNameCnfEx m_tCnf;
} TGuiSetOsdChnNameExReq;		//扩展的设置通道名称叠加请求

typedef struct
{
	int m_iRetCode;    //设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetOsdChnNameExRsp;	   //扩展的设置通道名称叠加回应


//=================== 通道名称叠加====================
#define GUI_SET_OSD_CHN_NAME_REQ		1010
#define GUI_SET_OSD_CHN_NAME_RSP		-1010
#define GUI_GET_OSD_CHN_NAME_REQ		1011
#define GUI_GET_OSD_CHN_NAME_RSP		-1011

typedef struct
{
	int m_iDsp;		//是否显示
	int m_iColor;		//叠加颜色
	int m_iLocType;	//坐标类型:0 绝对坐标, 1 : 24x16小方格
	int m_iXp;
	int m_iYp;
	char m_strName[GUI_CHN_NAME_LEN + 1];
} TGuiOsdChnNameCnf;

typedef struct
{
	int m_iChn;		//通道号
	TGuiOsdChnNameCnf m_tCnf;
} TGuiSetOsdChnNameReq;

typedef struct
{
	int m_iRetCode; //设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetOsdChnNameRsp;

typedef struct
{
	int m_iChn;
} TGuiGetOsdChnNameReq;

typedef struct
{
	int m_iChn;
	TGuiOsdChnNameCnf m_tCnf;
} TGuiGetOsdChnNameRsp;


//==================== 时间叠加=====================
#define GUI_SET_OSD_TM_REQ		1012		
#define GUI_SET_OSD_TM_RSP		-1012
#define GUI_GET_OSD_TM_REQ		1013
#define GUI_GET_OSD_TM_RSP		-1013

typedef struct
{
	int m_iDsp;		//叠加全能
	int m_iColor;	
	int m_iLocType;
	int m_iXp;
	int m_iYp;
	int m_iType;		//叠加格式
} TGuiOsdTmCnf;

typedef struct
{
	int m_iChn;
	TGuiOsdTmCnf m_tCnf;
} TGuiSetOsdTmReq;

typedef struct
{
	int m_iRetCode; //设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetOsdTmRsp;

typedef struct
{
	int m_iChn;
} TGuiGetOsdTmReq;

typedef struct
{
	int m_iChn;
	TGuiOsdTmCnf m_tCnf;
} TGuiGetOsdTmRsp;

// =================自定义文本===============
#define GUI_SET_OSD_TEXT_REQ	1014
#define GUI_SET_OSD_TEXT_RSP	-1014
#define GUI_GET_OSD_TEXT_REQ	1015
#define GUI_GET_OSD_TEXT_RSP	-1015

#define GUI_CHG_OSD_TEXT_REQ     1018
#define GUI_CHG_OSD_TEXT_RSP     -1018

typedef struct
{
	int m_iDsp;
	int m_iColor;
	int m_iLocType;
	int m_iXp;
	int m_iYp;
	char m_strText[0];
} TGuiOsdTextCnf;

typedef struct
{
	int m_iChn;
	TGuiOsdTextCnf m_tCnf;
} TGuiSetOsdTextReq;

typedef struct
{
	int m_iRetCode;//设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetOsdTextRsp;

typedef struct
{
	int m_iChn;
} TGuiGetOsdTextReq;

typedef struct
{
	int m_iChn;
	TGuiOsdTextCnf m_tCnf;
} TGuiGetOsdTextRsp;

// 视频遮挡区域设置
#define GUI_SET_OSD_PAREA_REQ		1016
#define GUI_SET_OSD_PAREA_RSP		-1016
#define GUI_GET_OSD_PAREA_REQ		1017
#define GUI_GET_OSD_PAREA_RSP		-1017

typedef struct
{
	int m_iEnable;	//视频遮挡区域全能标识
	int m_iLocType;	//坐标类似
	int m_iAreaCnt;
	TGuiArea m_tPrvAreas[GUI_MAX_COVER_AREA];
} TGuiOsdPrvAreaCnf;

typedef struct
{
	int m_iChn;
	TGuiOsdPrvAreaCnf m_tCnf;
} TGuiSetOsdPrvAreaReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetOsdPrvAreaRsp;

typedef struct
{
	int m_iChn;
} TGuiGetOsdPrvAreaReq;

typedef struct
{
	int m_iChn;
	TGuiOsdPrvAreaCnf m_tCnf;
} TGuiGetOsdPrvAreaRsp;

// ===============系统配置相关=================
#define GUI_SET_SYS_TIME_OK        0
#define GUI_SET_SYS_TIME_ERR       1
// 设置系统时间
#define GUI_SET_SYS_TIME_REQ			1020
#define GUI_SET_SYS_TIME_RSP			-1020
#define GUI_GET_SYS_TIME_ZONE_REQ		1023
#define GUI_GET_SYS_TIME_ZONE_RSP		-1023
#define GUI_SET_SYS_TIME_ZONE_REQ		1024
#define GUI_SET_SYS_TIME_ZONE_RSP		-1024		

#define GUI_TIME_ZONE_DEFAULT			100

typedef struct
{
	int m_iTimeZone;					//时区，默认GUI_TIME_ZONE_DEFAULT，表示不改变当前时区
	int m_iYear;
	int m_iMon;
	int m_iDay;
	int m_iHour;
	int m_iMin;
	int m_iSec;
} TGuiSetSysTimeReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetSysTimeRsp;

typedef struct
{
	int m_iTimeZone;
} TGuiGetSysTimeZoneRsp;

typedef struct
{
	int m_iTimeZone;
} TGuiSetSysTimeZoneReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetSysTimeZoneRsp;

// 获取通道数目配置信息
#define GUI_GET_DEV_NUM_REQ	1021
#define GUI_GET_DEV_NUM_RSP	-1021

enum
{
	VI_NO = 1,	//视频输入路数
	VO_NO,		//视频输出设备数
	VL_NO,		//视频环出路数
	AI_NO,		//音频输入数
	AO_NO,		//音频输出数
	AL_NO,		//音频环出路数
	AIP_NO,		//报警输入端口数
	AOP_NO,		//报警输出端口数
	COM_NO,		//串口数目
	VC_NO,		//合成编码通道数
	VC_VO_NO,	//合成编码虚拟VO数(0/1) 1-有0-无
	NETWORK_CARD_NO,  //网卡数量
	SYNCPLAY_NO,    //同步回放窗体数
};

typedef struct
{
	int m_iDevId;	//设备类型
} TGuiGetDevNumReq;

typedef struct
{
	int m_iDevId;	//设备类型
	int m_iNum;		//数目
} TGuiGetDevNumRsp;

// 获取参数修改版本号
#define GUI_GET_CNF_VER_REQ	1022
#define GUI_GET_CNF_VER_RSP	-1022

// 主程序对每种匹配作修改后，对其对应的版本号+1
// 外挂在必要的时候通过此协议询问配置的版本号，
// 在版本号发生变化的情况下才通过相应的协议重获取相关参数
typedef enum
{
	GUI_VI_CNF_ID = 0,	//VI配置
	GUI_VENC_CNF_ID,	//编码配置
	GUI_VREC_CNF_ID,	//录像配置
	GUI_SYS_CNF_ID,	//系统参数配置
	GUI_USER_CNF_ID,		//用户配置
	GUI_PTZ_CNF_ID,		//云镜配置
} TGuiCnfMajorId;

typedef struct
{
	int m_iCnfId;		//配置主类型
	int m_iCnfSubId;	//配置子类型，用于细化
	int m_iChn;		//参数对应的通道号,0表示所有通道
} TGuiGetCnfVerReq;

typedef struct
{
	int m_iVer;		//版本号
} TGuiGetCnfVerRsp;

// PUD参数获取
#define GUI_GET_PU_CNF_REQ		1025
#define GUI_GET_PU_CNF_RSP		-1025

typedef struct
{
	char    m_cManagerServerIp[16];    //服务器IP
	long    m_lManagerServerPort;      //服务器端口
	char    m_cHeartBeatServerIp[16];  //心跳服务器IP
	long    m_lHeartBeatPort;  			//心跳服务器端口
	char    m_cRegisterServerIP[16];	//注册服务器IP
	long    m_lRegisterPort;			//注册服务器端口
	char    m_cEnableHeartBeat[16];		//使能心跳
	int     m_iHeartBeatPeriod;			//心跳周期
	int     m_iRegisterTime;			//注册时间
	char    m_cVapPath[256];			//
	char    m_authServerIp[16];			//
	long    m_lauthServerPort;
	char    m_authUserName[16];
	char    m_authUserPassword[16];
	char    m_cputhreshold;
	char	m_memthreshold;
} TGuiPuCnf;

typedef struct
{
	int m_iRetCode;	//返回码
	TGuiPuCnf m_tCnf; //配置信息
} TGuiGetPuCnfRsp;

// PU信息获取
#define GUI_SET_PU_CNF_REQ		1026
#define GUI_SET_PU_CNF_RSP		-1026

typedef struct
{
	TGuiPuCnf m_tCnf;
} TGuiSetPuCnfReq;

typedef struct
{
	int m_iRet;
} TGuiSetPuCnfRsp;

// 获取硬件特征码
#define GUI_GET_HPM_CODE_REQ	1027
#define GUI_GET_HPM_CODE_RSP	-1027

typedef struct
{
	unsigned int m_uiCode[8];
} TGuiGetHpmCodeRsp;

//设置获取设备公共信息
#define GUI_DEV_COMMON_INFO_LEN  63
#define GUI_DEV_NFS_PATH_LEN  255 
#define GUI_DEV_IP_LEN 15
typedef struct
{
    char    m_cDeviceId[GUI_DEV_COMMON_INFO_LEN + 1];   //设备id
    char    m_cDeviceName[GUI_DEV_COMMON_INFO_LEN + 1]; //设备名称
    char    m_cNFSIp[GUI_DEV_IP_LEN + 1];      			//设备nfs挂载ip
    char  	m_cNFSPath[GUI_DEV_NFS_PATH_LEN + 1];  		//设备nfs挂载路径
	int		m_iDevNo;			//设备编号，陶永亮，2008-12-03，DVR新增字段
	int 	m_blNfsEnable;		//NFS使能
	int		m_iVspport;         //贝尔平台使用
	int		m_iVapport;			//贝尔平台使用
	char	m_cAccessnum[GUI_DEV_COMMON_INFO_LEN + 1];   //贝尔平台使用
}TGuiDevCommonInfoCnf;

#define GUI_GET_DEV_COMMON_INFO_CNF_REQ   1028  //获取设备commoninfo信息请求
#define GUI_GET_DEV_COMMON_INFO_CNF_RSP	 -1028	//获取设备commoninfo信息回应

typedef struct
{
	TGuiDevCommonInfoCnf m_tCommonInfo;
}TGuiGetDevCommonInfoCnfRsp;


#define GUI_SET_DEV_COMMON_INFO_CNF_REQ   1029 //设置设备commoninfo信息请求
#define GUI_SET_DEV_COMMON_INFO_CNF_RSP	  -1029 //设置设备commoninfo信息回应
typedef struct
{
	TGuiDevCommonInfoCnf m_tCommonInfo;
}TGuiSetDevCommonInfoCnfReq;  //设置设备commoninfo信息请求

typedef struct
{
	int m_iRetCode;  //GUI_SET_OK -成功，GUI_SET_ERR-失败
}TGuiSetDevCommonInfoCnfRsp;  //设置设备commoninfo信息回应


// 对讲控制
#define GUI_TALK_CTRL_REQ		1030
#define GUI_TALK_CTRL_RSP		-1030

enum //对讲音频数据格式
{
	GUI_AS_NORMAL = 0,
	GUI_AS_PCM,
};

typedef struct		//对讲控制
{
	int m_iAct;		//命令码:1 start, 0: stop
	int m_iDevId;	//对讲设备编号, 预留，默认值1
	int m_iType;		//码流类型
	int m_iReserve;	//保留字	, 默认0	
} TGuiTalkCtrlReq;

typedef struct
{
	int m_iRetCode;
} TGuiTalkCtrlRsp;

//IPC对讲
#define GUI_IPC_TALK_CTRL_REQ	1031   //对讲控制
#define GUI_IPC_TALK_CTRL_RSP	-1031 
typedef struct
{
	int m_iChn;
	int m_iActId;	   //命令码:1 start, 0: stop
	int m_iType;		//码流类型
}TGuiIpcTalkCtrlReq;

typedef struct
{
	int m_iRetCode; //-1表示失败
} TGuiIpcTalkCtrlRsp;

#define GUI_GET_IPC_WHO_TALK_REQ	1032 //获取跟哪个数字通道对讲
#define GUI_GET_IPC_WHO_TALK_RSP	-1032 //获取跟哪个通道对讲回应

typedef struct
{
	int m_iChn;  //当前没有通道进行对讲，则返回-1
}TGuiGetIpcWhoTalkRsp;

// 编码参数设置原子协议 2011/01/10 dxl
#define GUI_SET_ENCODE_PARAM_REQ	1040
#define GUI_SET_ENCODE_PARAM_RSP	-1040
#define GUI_GET_ENCODE_PARAM_REQ	1041
#define GUI_GET_ENCODE_PARAM_RSP	-1041

#define GUI_SET_ENCODE_PARAM_OK    0
#define GUI_SET_ENCODE_PARAM_ERR   1

enum
{
	GUI_ENCODE_VSIZE = 1,			//视频大小
	GUI_ENCODE_FRAME_RATE,		//帧率
	GUI_ENCODE_BITS_RATE_TYPE,	//位率类型
	GUI_ENCODE_BITS_RATE,			//码率
	GUI_ENCODE_QUALITY,			//图像质量
	GUI_ENCODE_TYPE,				//码流类型(复合码流/视频/音频)
	GUI_ENCODE_IFAME_RATE,		//I帧间隔
	GUI_ENCODE_AUDIO_TYPE,		//音频编码方式
};

typedef struct
{
	int m_iChn;				//通道号
	int m_iParamId;			//参数类别(由上述枚举指定)
	int m_iParamValue;		//参数值
} TGuiSetEncodeParamReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetEncodeParamRsp;

typedef struct
{
	int m_iChn;
	int m_iParamId;
} TGuiGetEncodeParamReq;

typedef struct
{
	int m_iParamId;		//与请求值对应，-1表式失败
	int m_iParamValue;	//参数值
} TGuiGetEncodeParamRsp;


// 透明通道/串口数据
#define GUI_TX_DATA_SEND_CMD			2000

enum	//数据类型
{
	GUI_TX_DATA_COM 		= 1, 	//串口数据
	GUI_TX_DATA_LOCAL,			//主程序与外挂的透明通道(不建议使用，除非非常特别的功能)
	GUI_TX_DATA_OTHER, 			//网络透明通道数据(客户端通过网络发给外挂的协议数据)
	GUI_TX_DATA_TALK,				//对讲数据
};

typedef struct
{
	int m_iType;	//数据类型
	int m_iSeq;	//序列号(可以递增或一直填充0)
	int m_iSrc;	//源编号(如来自己哪个串口)(默认值为0)
	int m_iDst;	//目标号(如要发往哪个串口)(默认值为0)
	int m_iR1;	//保留(默认值0)
	char m_cData[0];	 //数据
} TGuiTxDataSendCmd;

//=========================行业嵌入式部=========================
/*行业化时UI和内核通信的协议与结构*/
#define GUI_TX_APP_DATA_SEND_REQ		2100             //UI 发给内核再由内核转发给外挂请求
#define GUI_TX_APP_DATA_SEND_RSP		-2100			//UI 发给内核再由内核转发给外挂回应

typedef struct
{
	int m_iSendSize;		//成功发送的字节数
} TGuiAppDataSendRsp;

#define GUI_TX_APP_DATA_RCV_REQ			2101			//UI向内核发送请求字节数
#define GUI_TX_APP_DATA_RCV_RSP			-2101			//UI向内核发送数据的大小回应

typedef struct
{
	int m_iRcvBufSize;
} TGuiAppDataRecvReq;

//片头叠加
#define GUI_GET_VIDEO_HEADER_OSD_REQ	2180         //UI向内核获取片头叠加信息请求
#define GUI_GET_VIDEO_HEADER_OSD_RSP    -2180        //UI向内核获取片头叠加信息回应

#define MAX_INQUEST_VIDEO_HEADER_LEN	384

typedef struct 
{
	int 		m_iOverlayTime;			//叠加时间
	int			m_iX;					//片头叠加横坐标
	int			m_iY;					//片头叠加纵坐标
	int 		m_isDispaly;			//是否显示	//add by zxs 2012.9.5
	int 		m_iIedemo;				//是否是通过网络下传的片头
	char 		m_cHeaderMsg[MAX_INQUEST_VIDEO_HEADER_LEN + 1];	//片头叠加的内容，最多384字节
}TGuiGetVideoHeaderOsdMsgRsp;

#define GUI_SET_VIDEO_HEADER_OSD_REQ	2181		//外挂向内核设置片头叠加信息请求
#define GUI_SET_VIDEO_HEADER_OSD_RSP	-2181		//外挂向内核设置片头叠加信息回应

#define TGuiSetVideoHeaderOsdMsgReq TGuiGetVideoHeaderOsdMsgRsp

#define GUI_SET_VIDEO_HEADER_OSD_MSG_OK  	0   //设置成功
#define GUI_SET_VIDOE_HEADER_OSD_MSG_ERR	1	//设置失败
typedef struct
{
	int m_iRetCode;
}TGuiSetVideoHeaderOsdMsgRsp;

//-----------------------ATM------卡号叠加----------------------------------
#define GUI_GET_ATM_CARD_OSD_REQ	2010  //获取卡号叠加参数请求
#define GUI_GET_ATM_CARD_OSD_RSP	-2010 //回应

#define GUI_CARD_PROTOCOL_LEN			63
#define	GUI_CARD_PROTOCOL_VIA_COM		0
#define GUI_CARD_PROTOCOL_VIA_NET 		1

typedef struct
{
	int	m_iType;			//通信方式0:串口 1:UDP
	int m_iPortId; 				//串口ID
	char m_strProtocolName[GUI_CARD_PROTOCOL_LEN + 1];	//协议厂家名称	
	char m_strSrcIp[GUI_IP_STR_LEN + 1];	//ATM  IP地址
	char m_strDstIp[GUI_IP_STR_LEN + 1];			//设备IP地址
	int	m_iSrcPort;				//ATM端口号
	int	m_iDstPort;			//设备端口号
}TGuiAtmMsg;    //ATM参数信息

typedef struct
{
	int m_iChn;             //叠加的通道号按位表示 0x1表示第1通道,0x2表示第二通道，依次类推,0表示所有
	int m_iColor;			//叠加颜色
	int	m_iXp;
	int	m_iYp;				//叠加位置   m_iYp = -1按位置叠加，其余传坐标
	TGuiAtmMsg m_tAtmMsg;         //ATM 参数
}TGuiGetAtmCardNoOsdRsp;

#define	GUI_SET_ATM_CARD_OSD_REQ	2011	//设置卡号叠加参数请求
#define GUI_SET_ATM_CARD_OSD_RSP 	-2011    //回应

#define TGuiSetAtmCardNoOsdReq	TGuiGetAtmCardNoOsdRsp  //设置卡号叠加参数请求

#define GUI_SET_ATM_CardNoOsd_MSG_OK		0		//设置参数成功
#define GUI_SET_ATM_CardNoOsd_MSG_ERR		1		//设置参数失败
typedef struct							//设置卡号叠加参数回应
{
	int m_iRetCode;
} TGuiSetAtmCardNoOsdRsp;

#define GUI_GET_ATM_PROTOCOL_LIST_REQ   2012  	//获取ATM支持的协议列表请求
#define GUI_GET_ATM_PROTOCOL_LIST_RSP   -2012   //获取ATM支持的协议列表回应

#define GUI_ATM_PROTOCOL_NAME_LEN 		31
typedef struct 								//获取协议列表回应
{
	int m_iProtocolCnt;							//协议总数
} TGuiGetAtmProtocolListRsp;

typedef struct 								//协议列表项
{
	char m_strAtmProtocolName[GUI_ATM_PROTOCOL_NAME_LEN + 1]; //协议名
} TGuiAtmProtocolListItem;

//----------------------ATM----抓拍设置----------------------
#define GUI_GET_SNAPSHOT_CNF_REQ	2013
#define GUI_GET_SNAPSHOT_CNF_RSP	-2013

typedef struct
{
	int m_iSnaptype;	//抓拍模式，0单张抓拍 1定时抓拍
	int m_iQvalue;	//图片质量
	int m_iTmval;	//抓拍时间间隔 
	int m_iPicSize;   //图片大小 -1表示自动 其他值对应视频大小
	int m_iSnapLink;	//抓拍联动ftp 0/1 按位设置 0x1是ftp 0x2是email，其余扩展
}TGuiSnapShotCnf;

typedef struct                     //获取抓拍设置参数 按通道号查询
{
	int m_iChn;       //通道号	
}TGuiGetSnapshotCnfReq;

typedef struct					 // 获取抓拍设置参数回应
{
	int m_iChn;
	TGuiSnapShotCnf m_tSnapCnf;
}TGuiGetSnapshotCnfRsp;

#define GUI_SET_SNAPSHOT_CNF_REQ	2014
#define GUI_SET_SNAPSHOT_CNF_RSP	-2014

typedef struct
{
	int m_iChn;			//通道号
	TGuiSnapShotCnf m_tSnapCnf;
}TGuiSetSnapshotCnfReq;			//设置抓拍参数请求

#define GUI_SET_SNAPSHOT_CNF_OK		0		//设置参数成功
#define GUI_SET_SNAPSHOT_CNF_ERR	1		//设置参数失败

typedef struct							
{
	int m_iRetCode;
} TGuiSetSnapshotCnfRsp;			//设置抓拍参数回应



/*******************报警主机控制**************************/
#define GUI_SET_ALARMER_CNF_OK   0		//报警主机设置成功
#define GUI_SET_ALARMER_CNF_ERR  1		//报警主机设置失败

#define GUI_ALARM_IP_ADDRESS_LEN	31

typedef struct						//7601B报警主机
{
	int m_iIsUsed;		   	 		//是否启用
	int m_iSerialNo;	     		//串口号
	int m_iProtocoType;	     		//协议类型
	int m_iAddress;					//地址(0~255)
	int m_iAlarmInNum;	   	 		//报警输入数
	int m_iAlarmOutNum;   	 		//报警输出数
}TGuiSerialAlarmerMsg;


typedef struct						//网络报警主机
{
	int m_iIsUsed;					//启用
	char m_strAddress[GUI_ALARM_IP_ADDRESS_LEN + 1];	//ip地址
	int m_iPort;		//端口号
	int m_iAlarmInNum;	//报警输入数
	int m_iAlarmOutNum;	//报警输出数
}TGuiNetAlarmerMsg;

typedef struct									//获取网络报警主机参数请求
{
	int m_iDevNo;									//设备号
} TGuiGetNetAlarmerMsgReq;

typedef struct				
{
	int m_iDevNo;							
	TGuiNetAlarmerMsg	m_tNetAlarmMsg;			//主机信息
} TGuiGetNetAlarmerMsgRsp;						//获取网络报警主机信息

typedef struct
{
	int m_iDevNo;
	TGuiNetAlarmerMsg m_tNetAlarmerMsg;			//设置信息
} TGuiSetNetAlarmerMsgReq;						//设置网络报警主机信息请求

typedef struct
{
	int m_iRetCode;    							 //设置回应 0表示 成功
}TGuiSetNetAlarmerMsgRsp;					     //设置网络报警主机回应

typedef struct									//获取串口报警主机参数请求
{
	int m_iDevNo;									//设备号
} TGuiGetSerialAlarmerMsgReq;

typedef struct
{
	int m_iDevNo;								//返回码，0表示成功
	TGuiSerialAlarmerMsg	m_tSeialAlarmerMsg;	//主机信息
}TGuiGetSerialAlarmerMsgRsp;					//获取串口报警主机信息

typedef struct
{
	int m_iDevNo;
	TGuiSerialAlarmerMsg m_tSeialAlarmerMsg;
}TGuiSetSerialAlarmerMsgReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetSerialAlarmerMsgRsp;

//切换语言重启
typedef struct
{
	int m_iRetCode;
}TGuiChangeLangRebootRsp;

//============================解码器协议部分start==============================
//解码器
#define GUI_DEC_CLARITY_CMD_REQ			3000  //解码器透传命令请求
#define GUI_DEC_CLARITY_CMD_RSP 		-3000 //解码器透传命令回应
#define GUI_DEC_CLARITY_DATA_LEN  		63
typedef struct
{
	int m_iDevId;								//设备编号dxl 2009/11/30
	int m_iVoChn;								//vo画面编号(1-16)
	int m_iDataSize;							//数据长度
	char m_cData[0];							//数据信息
}TGuiDecClarityReq; //解码器透传命令请求

#define GUI_DEC_CLARITY_OK   0 //透传成功
#define GUI_DEC_CLARITY_ERR  1 //透传失败
typedef struct
{
	int m_iRetCode;
}TGuiDecClarityRsp; //解码器透传命令回应


//解码器叠加logo
#define GUI_GET_DEC_LOGO_STAT_REQ     3001  //获取解码器叠加logo信息请求
#define GUI_GET_DEC_LOGO_STAT_RSP     -3001 //获取解码器叠加logo信息回应

#define GUI_BMP_FILE_NAME_LEN 63
typedef struct
{
	int m_iDspEnable;		//是否叠加
	int m_iXp;			//横坐标
	int m_iYp;			//纵坐标
	int m_iAlpha;			//透明度	
	int m_iTransparentColor;	//透明色
	char m_cBmpFileName[GUI_BMP_FILE_NAME_LEN + 1];	//BMP图片文件名
}TGuiDecDspLogoMsgRsp;

#define GUI_GET_DEC_COM_ADDR_REQ    3002  //获取解码器串口地址请求
#define GUI_GET_DEC_COM_ADDR_RSP    -3002 //获取解码器串口地址回应

typedef struct
{
	int m_iPortId;       //串口uart号
}TGuiDecComAddrReq; //获取解码器地址属性号

typedef struct
{
	int m_iAddr;    	//设备地址
}TGuiDecComAddrRsp; 	//获取解码器地址回应

#define GUI_DEC_PREVIEW_PAGE_UP		3003			//预览上一页
#define GUI_DEC_PREVIEW_PAGE_DOWN	3004			//预览下一页
//解码器上一页，下一页
typedef struct
{
	int m_iDevId;
	int m_iChn;
} TGuiDecoderPreviewPageUpReq;					//预览上翻页

typedef struct
{
	int m_iDevId;
	int m_iChn;
} TGuiDecoderPreviewPageDownReq;					//预览下翻页

#define GUI_DEC_PTZ_REMOTE_CODE			3005		//解码器生成的云台控码	

typedef struct									//云台控制本地控制码
{
	int m_iDevId;	//设备号							
	int m_iChn;	//通道号							
	int m_iCode;	//控制码值						
	int m_iParam[4];	//相关参数
} TGuiDecPtzLocalCode;

#define GUI_DEC_SET_NVS_PARA_REQ    3006   //UI设置NVS参数请求
#define GUI_DEC_SET_NVS_PARA_RSP    -3006  //UI设置NVS参数回应

typedef struct
{
    char            	m_serName[36];		//服务器名称(暂时保留)
    char            	m_serIP[36];		//服务器地址，可以是IP地址，也可以是域名；
    char            	m_serProxy[16];		//服务器上级代理
    unsigned char  	 	m_serChan;		//服务器通道
    unsigned char		m_serStream;		//服务器码流类型,主副码流
    unsigned char		m_serNetmode;		//传输协议1：TCP，2：UDP，3：多播；
    unsigned char		m_bUseddns;		//通过解析服务器连接
    unsigned short		m_serPort;		//服务器端口
    char            	m_username[20];		//用户名
    char            	m_password[20];		//密码
    int         		m_holdtime;		//保持时间，取值范围：5――60秒
    int         		m_bisView;
    int         		m_iAudioOut;		//音频解码是否输出
    char            	m_deviceType[16];   	//设备控制协议（需要翻译到的协议）
    unsigned char   	m_iAddress;         	//NVS设备地址（取值范围：1-255）
    unsigned char   	m_iAudioEType;         	//音频编码类型
    unsigned char   	m_cSubStreamFlag;       //设备是否有副码流0----无   1-----有   默认是0
    char            	m_cEncrypt[20];		//密码
    unsigned char		m_u8SerType;		//设备类型0为TD，1为onvif  ， 2为push流
} TDECNVSITEM; 

typedef struct
{
	int  m_iChan;   		//  通道(即输出设备号m_iDevId)
	int  m_iPic;      		// 画面(即通道号 m_iChn)
	TDECNVSITEM m_nvsItem;  //前端设备的连接信息
}TGuiDecSetNvsParaReq;

typedef struct
{
	int  m_iRetCode;   //设置返回, 0-表示成功，1-表示失败
}TGuiDecSetNvsParaRsp;

#define GUI_SET_DEC_AUDIO_CTRL_REQ  3007    //设置解码器音频控制请求
#define GUI_SET_DEC_AUDIO_CTRL_RSP  -3007   //设置解码器音频控制回应

#define GUI_DEC_AUDIO_ON   1
#define GUI_DEC_AUDIO_OFF  0

typedef struct
{
	int m_iChn;		//  通道(即输出设备号m_iDevId)
	int m_iPic;		// 画面(即通道号 m_iChn)
	int m_iCtrl;    // 1: GUI_DEC_AUDIO_ON 开  0:GUI_DEC_AUDIO_OFF 关
}TGuiSetDecAuidoCtrlReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetDecAuidoCtrlRsp;


#define GUI_VIRTUAL_MATRIX_DATA_SEND_REQ  3010   //虚拟矩阵接收数据发送给内核请求
#define GUI_VIRTUAL_MATRIX_DATA_SEND_RSP  -3010 //虚拟矩阵接收数据发送给内核回应

#define GUI_VIRTUAL_MATRIX_DATA_LEN  255
typedef struct
{
	int m_iDataSize;   							   //数据长度
	char m_cData[GUI_VIRTUAL_MATRIX_DATA_LEN + 1]; //数据信息
}TGuiVirtualMatrixDataSendReq;

#define GUI_VIRTUAL_MATRIX_DATA_SEND_OK  0 //发送成功
#define GUI_VIRTUAL_MATRIX_DATA_SEND_ERR  1 //发送失败

typedef struct
{
	int m_iRetCode;
}TGuiVirtualMatrixDataSendRsp;


//解码器协议部分end

//*********************Miracle项目新增协议*********************//
//手动触发报警
#define GUI_GET_MANUAL_ALARM_LST_REQ  550    //获取手动报警列表请求
#define GUI_GET_MANUAL_ALARM_LST_RSP  -550   //获取手动报警列表回应

#define GUI_MANUAL_ALARM_NAME    31
typedef struct
{
	int m_iAlarmOutChn; //手动触发报警端口号
}TGuiManualGetAlarmOutReq;

typedef struct
{
	int m_iStat; //手动触发报警状态 0 
	char m_cAlarmOutName[GUI_MANUAL_ALARM_NAME + 1];//报警别名
	char m_cAlarmAddr[GUI_MANUAL_ALARM_NAME + 1];   //模拟通道为"本地",数字通道为"IP地址"
}TGuiManualAlarmOutMsg;

typedef struct
{
	int m_iAlarmOutChn; //手动触发报警端口号
	TGuiManualAlarmOutMsg m_tManAlarmOut;
}TGuiManualGetAlarmOutRsp;   //手动触发报警回应

#define GUI_SET_MANUAL_ALARM_REQ           551     //手动设置报警请求
#define GUI_SET_MANUAL_ALARM_RSP           -551    //设置手动报警回应

typedef struct
{
	int m_iAlarmOutChn;
	int m_iStat; // 1-报警，0 - 不报警
}TGuiSetManualAlarmOutReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetManulAlarmOutRsp;

/********************************************************************/
//视频剪辑功能

#define GUI_VIDEO_CLIP_DISK_NOSAPCE  -3  //保存视频剪辑片断磁盘空间不足
#define GUI_VIDEO_CLIP_GENERIC_ERR    -1 //其他一般性错误

#define GUI_DO_VIDEO_CLIP_REQ    560  //视频剪辑开始请求
#define GUI_DO_VIDEO_CLIP_RSP    -560  //视频剪辑开始回应

#define GUI_VIDEO_CLIP_SAVE_PATH_LEN   127
typedef struct
{
	int m_iChn; //需要剪辑的通道
	int m_iStartTime; //开始时间
	int m_iEndTime;   //结束时间
	char m_cPath[GUI_VIDEO_CLIP_SAVE_PATH_LEN + 1];  //保存路径
}TGuiDoVideoClipReq;
typedef struct
{
	int m_iClipID;  //剪辑ID  据此ID 查询保存进度 
}TGuiDoVideoClipRsp;

#define GUI_VIDEO_CLIP_QUERY_PRO_REQ   561 //视频剪辑保存进度请求
#define GUI_VIDEO_CLIP_QUERY_PRO_RSP   -561  //视频 剪辑保存进度回应
typedef struct
{
	int m_iClipID;
}TGuiVideoClipQueryProReq;

typedef struct
{
	int m_iPro; //百分比
}TGuiVideoClipQueryProRsp;

//录像文件备份、剪辑备份一个文件的过程中中止命令
#define GUI_VIDEO_BACKUP_PRO_STOP_REQ   562 //视频剪辑一个文件的过程中中止请求
#define GUI_VIDEO_BACKUP_PRO_STOP_RSP   -562 //视频剪辑一个文件的过程中中止回应
typedef struct
{
	int m_iRetCode; //0 成功，1失败
}TGuiVideoBackupProStopRsp;

#define GUI_GET_CLIPFILE_MSG_REQ 		563	//获取录像剪辑大小请求
#define GUI_GET_CLIPFILE_MSG_RSP 		-563	//获取录像剪辑大小回应

typedef struct						//获取录像剪辑大小请求
{
	int m_iChn;					//通道
	int m_iStartTime;				//开始剪辑时间
	int m_iEndTime;				//结束剪辑时间
} TGuiGetClipFileMsgReq;

typedef struct						//获取录像剪辑大小回应
{
	TGuiGetClipFileMsgReq m_tGuiClipFileMsg;
	unsigned int m_uiFileSize;				//文件大小
}TGuiGetClipFileMsgRsp;


//磁盘检测与修复功能
#define GUI_GET_DISK_CHECK_REQ      566         //获取磁盘断电检测请求
#define GUI_GET_DISK_CHECK_RSP      -566        //获取磁盘断电检测请求回应
#define GUI_CHECK_ALL_DISK   1    //检测
#define GUI_UNCHECK_ALL_DISK  0   //不检测
#define GUI_REPAIR_DISK   1       //修复坏盘
#define GUI_UNREPAIR_DISK 0       //不修复坏盘
typedef struct
{
	int m_iCheckAllDisk;
	int m_iRepairDisk;   
}GuiGetDiskCheckRsp;

#define GUI_SET_DISK_CHECK_REQ       567        //设置磁盘断电检测请求
#define GUI_SET_DISK_CHECK_RSP       -567        //设置磁盘断电检测请求回应
typedef struct
{
	int m_iCheckAllDisk;
	int m_iRepairDisk;
}TGuiSetDiskCheckReq; //设置磁盘断电检测请求

typedef struct
{
	int m_iRetCode;
}TGuiSetDiskCheckRsp;  //设置磁盘断电检测请求回应

//====================盘组模式==============================
#define GUI_GET_STORAGE_MODE_REQ   	 	568  //获取存储模式请求(盘组或配额   )
#define GUI_GET_STORAGE_MODE_RSP 		-568 //获取存储模式回应
#define GUI_SET_STORAGE_MODE_REQ		569  //设置存储模式请求
#define GUI_SET_STORAGE_MODE_RSP		-569 //设置存储模式回应

enum
{
	DISK_GROUP = 1,   //盘组
	DISK_QUOTA,      //配额
}; //存储模式

typedef struct
{
	int m_iType;  //存储模式
}TGuiGetStorageModeRsp;   //获取存储模式回应(盘组或配额   )

typedef struct
{
	int m_iType;
} TGuiSetStorageModeReq; //设置存储模式请求

typedef struct
{
	int m_iRetCode;
} TGuiSetStorageModeRsp;//设置存储模式回应

//==================盘组设置===============================
#define GUI_GET_DISK_GROUP_REQ      570 	//获取盘组信息请求
#define GUI_GET_DISK_GROUP_RSP		-570	//获取盘组信息回应
#define GUI_SET_DISK_GROUP_REQ		571		//设置盘组信息请求
#define GUI_SET_DISK_GROUP_RSP		-571	//设置盘组信息回应

#define GUI_MAX_DISK_GROUP_CNT		8		//最大的盘组个数

typedef struct
{
	unsigned int m_iDiskLst[GUI_MAX_DISK_GROUP_CNT][8];     //硬盘列表
	unsigned int m_iChannelLst[GUI_MAX_DISK_GROUP_CNT][8];   //通道列表
} TGuiGetDiskGroupRsp; //获取盘组信息回应

typedef TGuiGetDiskGroupRsp TGuiSetDiskGroupReq;  //设置盘组信息请求

#define GUI_SET_DISK_GROUP_OK   0  //设置成功
#define GUI_SET_DISK_GROUP_ERR  1  //设置失败
typedef struct
{
	int m_iRetCode;
} TGuiSetDiskGroupRsp;  //设置盘组信息回应

//==================配额设置==============================
#define GUI_GET_DISK_QUOTA_REQ    572	//获取配额信息请求
#define GUI_GET_DISK_QUOTA_RSP    -572	//获取配额信息回应
#define GUI_SET_DISK_QUOTA_REQ	  573	//设置配额信息请求
#define GUI_SET_DISK_QUOTA_RSP    -573  //设置配额信息回应

typedef struct
{
	int m_iChn;  //通道号
}TGuiGetDiskQuotaReq;      //获取配额信息请求

typedef struct
{
	int m_iChn;
	unsigned int m_iTotalCap;  //所有磁盘的总容量(单位:G)
	unsigned int m_iRecUsed;   //已用录像配额(单位: M)
	unsigned int m_iPicUsed;    //已用图片配额(单位: M)
	unsigned int m_iRecQuota;  //录像配额(单位: G)
	unsigned int m_iPicQuota;    //图片配额(单位: G)
} TGuiGetDiskQuotaRsp;		//获取配额信息回应

typedef TGuiGetDiskQuotaRsp TGuiSetDiskQuotaReq;

typedef struct
{
	int  m_iRetCode;   //0表成功，1表失败
}TGuiSetDiskQuotaRsp;		//设置配额信息回应





//==================/*混音相关add  by quxinfeng 2012.2.7*/============
#define GUI_SET_MIXAUDIO_REQ       20000
#define GUI_SET_MIXAUDIO_RSP 		-20000 	
#define GUI_GET_MIXAUDIO_REQ       20001
#define GUI_GET_MIXAUDIO_RSP 		-20001
#define MIXAUDIO_OPERATER_OK     	0
#define GUI_MAX_AUDIO_SOURCE  		4
typedef struct
{
	int m_iChn;		
}TGuiGetMixAudioReq;

typedef struct
{	 int m_iChn;
	 int m_iMixAudioEnable;//是否混音
	 int m_iMixAudioSourceChnNo[GUI_MAX_AUDIO_SOURCE];//声源
}TGuiGetMixAudioRsp;

#define TGuiSetMixAudioReq   TGuiGetMixAudioRsp

typedef struct
{
	int m_iRetCode;		// 0表示成功
}TGuiSetMixAudioRsp;

#define GUI_SET_AUDIO_THRESHOLD_REQ   20002  //设置音频阈值请求
#define GUI_SET_AUDIO_THRESHOLD_RSP   -20002  //设置音频阈值回应
typedef struct
{
	int m_iChn;
	int m_iThreshold;	 //阈值
}TGuiSetAudioThresholdReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetAudioThresholdRsp; 

#define GUI_GET_AUDIO_THRESHOLD_REQ   20003  //获取音频阈值请求
#define GUI_GET_AUDIO_THRESHOLD_RSP   -20003  //获取音频阈值回应
typedef struct
{
	int m_iChn;
}TGuiGetAudioThresholdReq;

typedef struct
{
	int m_iChn;
	int m_iThreshold;  //阈值
}TGuiGetAudioThresholdRsp;
//****************************************//
//设置摄像机高清参数

typedef struct
{
  int m_iStructLenth;//结构体长度
  int m_iExposureMode;//曝光模式:0,手动;1,自动
  int m_iExpPriority;//曝光策略:0，噪声优先;1，帧率优先
  int m_iExpCompensiton;//目标亮度0-255
  int m_iExpMaxTime;//最大曝光时间，单位us
  int m_iExpMinTime;//最小曝光时间，单位us
  int m_iExpMaxGain;//最大增益，单位 0.01倍
  int m_iExpMinGain;//最小增益，单位 0.01倍
  TGuiArea m_tExpArray;//曝光区域
  int m_iIrisMode;//光圈模式:0，手动；1，自动
  int m_iWBMode;//白平衡模式:0，手动;1,自动
  int m_iWBRGain;//手动模式下的红增益:0-255
  int m_iWBBGain;//手动模式下的蓝增益:0-255
  int m_iWDRMode;//宽动态模式:1，打开；2关闭
  int m_iWDRLevel;//宽动态级别（保留）
  int m_iBLCMode;//背光补偿模式:1，打开；2关闭
  int m_iBLCLevel; //背光补偿级别
  int m_iIRMode;//红外滤光片模式:0，自动;1,打开;2，关闭
  int m_iSharpness;//锐度0-255
  int m_iNoise;//噪声0-255
  int m_iGamma;//gamma值*100
}TGuiVideoISPInfo;

#define GUI_GET_VIDEO_ISP_INFO_REQ  4000 //获取摄像机高清参数请求
#define GUI_GET_VIDEO_ISP_INFO_RSP	-4000 //获取摄像机高清参数请求回应
typedef struct
{
	TGuiVideoISPInfo m_tVideoISPInfo;
}TGuiGetVideoISPInfoRsp; //获取摄像机高清参数请求回应

#define GUI_SET_VIDEO_ISP_INFO_REQ   4001 //设置摄像机高清参数请求
#define GUI_SET_VIDEO_ISP_INFO_RSP   -4001 //设置摄像机高清参数回应

typedef struct
{
	TGuiVideoISPInfo m_tVideoISPInfo;
}TGuiSetVideoISPInfoReq; //设置摄像机高清参数请求

#define GUI_SET_VIDEO_ISP_INFO_OK  0 //设置摄像机高清参数成功
#define GUI_SET_VIDEO_ISP_INFO_ERR	1 //设置摄像机高清参数失败

typedef struct
{
	int m_iRetCode;
}TGuiSetVideoISPInfoRsp; //设置摄像机高清参数回应

//=====================================================
//设置S3E 视频翻转参数
//翻转类型  UI上的视频翻转给内核发枚举类型3即可
typedef enum
{
	GUI_VIDEO_TURN_H = 1,		//水平翻转
	GUI_VIDEO_TURN_V,			//垂直翻转
	GUI_VIDEO_TURN_R,			//倒置翻转
}TGuiVideoTurnType;

#define GUI_SET_VIDEO_TURN_TYPE_REQ  4101 //设置视频翻转类型请求
#define GUI_SET_VIDEO_TURN_TYPE_RSP  -4101 //设置视频翻转类型请求回应

typedef struct
{
	int m_iChn;
	int m_iType; 		//翻转类型
}TSetVideoTurnTypeReq; //设置视频翻转类型请求

typedef struct
{
	int m_iRetCode;     //使用公共设置成功失败返回码
}TSetVideoTurnTypeRsp; //设置视频翻转类型请求回应


//------------------------------------------------------------------
//车载DVR电源管理
#define GUI_GET_POWER_CNF_REQ   200000     //获取电源管理配置请求
#define GUI_GET_POWER_CNF_RSP    -200000    //获取电源管理配置回应
#define GUI_SET_POWER_CNF_REQ    200001    //设置电源管理配置请求
#define GUI_SET_POWER_CNF_RSP    -200001   //设置电源管理配置回应
typedef struct 
{
	int m_iOpenEnable;
	int m_iOpenDevDelayTime;
	int m_iCloseEnable;
	int m_iCloseDevDelayTime;
}TGuiPowerManageMsg;

typedef struct								//获取电源管理回应	
{
	TGuiPowerManageMsg m_tPowerManageMsg;						
} TGuiGetPowerManageCnfRsp;

typedef struct                          //设置电源管理请求
{
	TGuiPowerManageMsg m_tPowerManageMsg;
}TGuiSetPowerManageCnfReq;

#define GUI_SET_POWER_MANAGE_CNF_OK			0    //设置成功
#define GUI_SET_POWER_MANAGE_CNF_ERR		1    //设置失败
typedef struct                           //设置电源管理回应
{
	int m_iRetCode;
}TGuiSetPowerManageCnfRsp;

//============================ 

typedef struct 
{
	int m_iPoweroffVoltage;  	//关机低压值，需乘以10
	int m_iAlarmVoltage;		//低压报警值，需乘以10
}TGuiPowerAlarmMsg;

#define GUI_GET_POWER_ALARM_MSG_REQ	200002	//获取关机电压和报警电压请求
#define GUI_GET_POWER_ALARM_MSG_RSP	-200002	//获取关机电压和报警电压回应

typedef struct 
{
	TGuiPowerAlarmMsg m_tPowerAlarmMsg;
}TGuiGetPowerAlarmMsgRsp;

#define GUI_SET_POWER_ALARM_MSG_REQ	200003	//设置关机电压和报警电压请求
#define GUI_SET_POWER_ALARM_MSG_RSP	-200003	//设置关机电压和报警电压回应

typedef struct 
{
	TGuiPowerAlarmMsg m_tPowerAlarmMsg;
} TGuiSetPowerAlarmMsgReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetPowerAlarmMsgRsp;

//============================ 
//FTP上传配置(该部分协议废止)
#define GUI_STR_SERVER_NAME_LEN    63
typedef struct
{
	char	m_cServerName[GUI_STR_SERVER_NAME_LEN + 1];       //服务器地址
	char    m_cServerPath[GUI_STR_SERVER_NAME_LEN + 1];       //在服务器上得路径
	char 	m_cUserName[GUI_IP_STR_LEN + 1];         //用户名
	char	m_cPassword[GUI_IP_STR_LEN + 1];			//密码
	int		m_iPort;					//端口号
	int     m_iAutoFtpEnable;                 //自动上传是能
}TGuiFTPServerParaMsg;

#define GUI_GET_FTP_CNF_REQ       200004   //获取FTP上传配置请求
#define GUI_GET_FTP_CNF_RSP       -200004  //获取FTP上传配置回应
typedef struct									//获取FTP上传服务参数回应
{
	TGuiFTPServerParaMsg m_tFTPParaMsg;
}TGuiGetFTPServerParaCnfRsp;

#define GUI_SET_FTP_CNF_REQ       200005   //设置FTP 上传配置请求
#define GUI_SET_FTP_CNF_RSP       -200005  //设置FTP上传配置回应
#define GUI_UP_FTP_CNF_REQ        200006   //上传命令请求
#define GUI_UP_FTP_CNF_RSP        -200006  //上传命令回应
typedef struct                               //设置FTP上传服务参数请求
{
	TGuiFTPServerParaMsg m_tFTPParaMsg;
}TGuiSetFTPServerParaCnfReq;

#define GUI_SET_FTP_PARA_CNF_OK			0   //设置成功
#define GUI_SET_FTP_PARA_CNF_ERR			1   //设置失败
typedef struct								//设置FTP上传服务参数回应
{
	int m_iRetCode;
}TGuiSetFTPServerParaCnfRsp;

//============================ 
#define GUI_GET_WIFI_AUTO_FTP_STAT_REQ	200101	//获取wifi自动ftp上传使能状态请求
#define GUI_GET_WIFI_AUTO_FTP_STAT_RSP	-200101	//获取wifi自动ftp上传使能状态回应
typedef struct								
{	
	int m_iEnable; //见公共宏定义GUI_SET_ENABLE
}TGuiGetAutoFtpStatRsp;

#define GUI_SET_WIFI_AUTO_FTP_STAT_REQ	200102	//设置wifi自动ftp上传使能状态请求
#define GUI_SET_WIFI_AUTO_FTP_STAT_RSP	-200102	//设置wifi自动ftp上传使能状态回应

typedef struct								
{	
	int m_iEnable; //见公共宏定义GUI_SET_ENABLE
}TGuiSetAutoFtpStatReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetAutoFtpStatRsp;

//============================ 
#define GUI_SEND_WIFI_FTP_UP_CMD_REQ        200103   //发送手动上传命令请求
#define GUI_SEND_WIFI_FTP_UP_CMD_RSP        -200103  //发送手动上传命令回应

typedef struct								
{	
	int m_iRetCode;
}TGuiSendWifiFtpUpCmdRsp;
//============================ 




//------------------------------------------------------------------
//短信管理
#define GUI_SMS_COUNT_REQ        200009   //统计短信总数请求
#define GUI_SMS_COUNT_RSP        -200009  //统计短信总数回应
#define GUI_SMS_QUERY_REQ         200010  //查询短信请求
#define GUI_SMS_QUERY_RSP         -200010  //查询短信回应
#define GUI_SMS_CLEAR_REQ        200011    //清空短信请求
#define GUI_SMS_CLEAR_RSP         -200011   //清空短信回应
#define GUI_SMS_DEL_REQ           200012   //删除短信请求
#define GUI_SMS_DEL_RSP           -200012   //删除短信回应
#define GUI_SMS_SIGN_READ_REQ    200013   //标记已读信息请求
#define GUI_SMS_SIGN_READ_RSP    -200013  //标记已读信息回应

//查询事件
#define GUI_SMS_ALL     1
#define GUI_SMS_READ    2
#define GUI_SMS_UNREAD  3
typedef struct
{
	int m_iCondition;         //查询编号
}TGuiSmsQueryCondition;

typedef struct        //统计短信个数请求
{
	TGuiSmsQueryCondition m_tCondition;
}TGuiCountSmsReq;
typedef struct       //统计短信个数回应
{
   int m_iSmsCnt;   //统计的短信个数
}TGuiCountSmsRsp;

#define GUI_DIAL_NUMBER   31                  //发信人号码最大长度
#define GUI_SMS_CONTENT_LEN  159               //短信内容最大长度(以字符计算)
#define GUI_SMS_NAME   63
typedef struct
{
	int m_iRecTm;                                     //短信接收时间
	char m_strFrom[GUI_DIAL_NUMBER + 1];            //发信人
	char m_strContent[GUI_SMS_CONTENT_LEN + 1];     // 短信内容	
	char m_strName[GUI_SMS_NAME + 1];	//短信名
}TGuiSmsMsg;


typedef struct					//短信查询请求
{
	TGuiSmsQueryCondition m_tCondition; //查询条间
	int m_iPageId;          //起始短信页(第一页为0)
 	int m_iSmsCnt;               //每页短信条数
}TGuiSmsQueryReq;

typedef struct                 //查询短信回应
{
	int m_iSmsCnt;			//返回的条数
}TGuiSmsQueryRsp;

//短信删除事件
typedef struct									//删除短信请求
{
	char m_strName[GUI_SMS_NAME + 1];	//短信文件名	
} TGuiSmsDeleteReq;

#define GUI_SMS_DEL_OK			0		//删除短信成功
#define GUI_SMS_DEL_ERR			1		//删除短信失败
typedef struct									//删除短信回应
{
	int m_iRetCode;							//返回码
} TGuiSmsDeleteRsp;

//短信清空事件
#define GUI_SMS_CLEAR_OK		0				//清空短信成功
#define GUI_SMS_CLEAR_ERR		1				//清空短信失败
typedef struct									//清空短信回应
{
	int m_iRetCode;							//短信清空返回码
} TGuiSmsClearRsp;

//短信标记已读事件
typedef struct									//短信标记请求
{
	char m_strName[GUI_SMS_NAME + 1];	//短信文件名	
} TGuiSmsSignReadReq;

#define GUI_SMS_SIGN_READ_OK			0		//短信标记已读成功
#define GUI_SMS_SIGN_READ_ERR			1		//短信标记已读失败
typedef struct									//短信标记回应
{
	int m_iRetCode;							//返回码
} TGuiSmsSignReadRsp;

//-----------------------------------------------------------------------------------------
//3G管理
#define GUI_GET_3G_STATUS_CNF_REQ       200018 //获取3G管理配置请求
#define GUI_GET_3G_STATUS_CNF_RSP        -200018 //获取3G管理配置回应
//3G拨号方式
#define NET_3G_AUTO_DIAL  0
#define NET_3G_SMS_DIAL   1
#define NET_3G_PHONE_DIAL 2
#define NET_3G_ALARM_DIAL 3

//3G设备类型
#define NET_3G_NONE_DEVICE   3
#define NET_3G_WCDMA_DEVICE  2
#define NET_3G_EVDO_DEVICE   1
#define NET_3G_TD_DEVICE     0
//3G在线状态
#define NET_3G_NOTONLINE    0  //不在线
#define NET_3G_ONLINE       1  // 在线 
#define NET_3G_LINING       2   //连接中...
//3G断线方式 
#define NET_3G_INITIATIVE   0
#define NET_3G_PASSIVE       1

//3G信号强度
#define NET_3G_SIGNAL_WEAKER   0
#define NET_3G_SIGNAL_WEAK     1
#define NET_3G_SIGNAL_GENERAL  2
#define NET_3G_SIGNAL_STRONG   3


#define NET_3G_DISCONNECT	    0				//3G未连接
#define NET_3G_CONNECTED    	1				//3G连接成功(已连接)
#define NET_3G_CONNECTING       2               //3G连接中。。。

typedef struct
{
	int m_iConnectState; 
	int m_iDeviceType;//设备类型 
	int m_iOnLineState;//在线状态
	int m_iIntensity;  //信号强度
	char m_strIP[GUI_IP_STR_LEN + 1]; //IP地址
	int m_iDialBeginTm;  //初始拨号时间
}TGui3GStatusMsg;

typedef struct          //获取3G设置信息回应
{
	TGui3GStatusMsg m_t3GStatusMsg;
}TGuiGet3GStatusCnfRsp;

//3G拨号
#define GUI_GET_3G_DIAL_STATUS_CNF_REQ   200019 //获取3G拨号状态请求
#define GUI_GET_3G_DIAL_STATUS_CNF_RSP   -200019 //获取3G拨号状态回应
#define GUI_SET_3G_DIAL_STATUS_CNF_REQ   200020  //  设置3G拨号状态请求
#define GUI_SET_3G_DIAL_STATUS_CNF_RSP   -200020  //  设置3G拨号状态回应

typedef struct
{
	int m_iDialType;   //拨号方式
	int m_iStopType;   //断线方式
	int m_iDurationTm; //持续时间
}TGui3GDialStatusMsg;

typedef struct                            //获取3G拨号状态参数回应
{
	TGui3GDialStatusMsg m_t3GDialStatusMsg;
}TGuiGet3GDialStatusMsgRsp;

typedef struct                           //设置3g拨号状态参数请求
{
	TGui3GDialStatusMsg m_t3GDialStatusMsg;
}TGuiSet3GDialStatusMsgReq;

#define GUI_SET_3G_DIAL_STATUS_OK    0    //设置成功
#define GUI_SET_3G_DIAL_STATUS_ERR   1    //设置失败

typedef struct                          //设置3g拨号状态参数回应
{
	int m_iRetCode;
}TGuiSet3GDialStatusMsgRsp;

//3G管理-断开事件
#define GUI_SET_3G_DISCONN_CNF_REQ   200023 //设置3G网落断开请求
#define GUI_SET_3G_DISCONN_CNF_RSP   -200023 //设置3G网络断开回应

//设置3G断开网络的回应
typedef struct
{
	int m_iRetCode;
}TGuiSet3GDisconnectRsp;

//3G管理-连接事件
#define GUI_SET_3G_CONNECT_CNF_REQ   200024 //设置3G网落连接请求
#define GUI_SET_3G_CONNECT_CNF_RSP   -200024 //设置3G网络连接回应
//设置3G连接网络的回应
typedef struct
{
	int m_iRetCode;
}TGuiSet3GConnectRsp;

//3G管理-更多设置
#define GUI_GET_3G_DIAL_CNF_REQ      200025   //获取3G管理拨号配置请求
#define GUI_GET_3G_DIAL_CNF_RSP      -200025  //获取3G 管理拨号配置回应
#define GUI_SET_3G_DIAL_CNF_REQ      200026   //设置3G管理拨号配置请求
#define GUI_SET_3G_DIAL_CNF_RSP      -200026   //设置3G 管理拨号配置回应

#define GUI_DIAL_NOTIFY_LEN 19
typedef struct
{
	char m_strNotify[GUI_DIAL_NOTIFY_LEN + 1];
	char m_strPhone1[GUI_DIAL_NUMBER + 1];
	char m_strPhone2[GUI_DIAL_NUMBER + 1];
	char m_strPhone3[GUI_DIAL_NUMBER + 1];
	char m_strPhone4[GUI_DIAL_NUMBER + 1];
	char m_strPhone5[GUI_DIAL_NUMBER + 1];
}TGui3GDialSetParaMsg;

typedef struct                                //获取3G管理-更多设置界面参数回应
{
	TGui3GDialSetParaMsg m_t3GDialSetParaMsg;
}TGuiGet3GDialSetParaCnfRsp;

typedef struct                               //设置3G 管理-更多设置界面参数请求
{
	TGui3GDialSetParaMsg m_t3GDialSetParaMsg;
}TGuiSet3GDialSetParaCnfReq;

#define GUI_SET_3G_DIAL_SET_PARA_OK    0    //设置成功
#define GUI_SET_3G_DIAL_SET_PARA_ERR   1    //设置失败
typedef struct                              //设置3G管理-更多设置界面参数回应
{
	int m_iRetCode;
}TGuiSet3GDialSetParaCnfRsp;

//3G管理-高级设置
//3G管理-高级设置-3G任务时间表
#define GUI_GET_3G_ADV_SCH_CNF_REQ     200029   //获取3G高级管理中3G任务时间表请求
#define GUI_GET_3G_ADV_SCH_CNF_RSP    -200029 //获取3G高级管理中3G任务时间表回应
#define GUI_SET_3G_ADV_SCH_CNF_REQ     200030  //设置3G高级管理中3G任务时间表请求
#define GUI_SET_3G_ADV_SCH_CNF_RSP    -200030 //设置3G高级管理中3G 任务时间表回应

#define NET_3G_ENABLE_TASK_SCH      1          //启用任务时间表
#define NET_3G_UNENABLE_TASK_SCH    0          //禁用任务时间表
typedef struct
{
	int m_iTmSchEnable;
	int m_iStartTm;
	int m_iEndTm;
}TGui3GTmSchMsg;

typedef struct							//获取3G任务时间表回应
{
	TGui3GTmSchMsg m_t3GTmSchMsg;
}TGuiGet3GTmSchCnfRsp;

typedef struct                       //设置3G任务时间表请求
{
	TGui3GTmSchMsg m_t3GTmSchMsg;
}TGuiSet3GTmSchCnfReq;

#define GUI_SET_3G_TMSCH_OK    0  //设置成功
#define GUI_SET_3G_TMSCH_ERR   1  //设置失败
typedef struct                      //设置3G任务时间表回应
{
	int m_iRetCode;
}TGuiSet3GTmSchCnfRsp;

//3G管理-高级设置-通知
#define GUI_GET_3G_ADV_NOTIFY_CNF_REQ   200031 //获取3G高级管理中短信通知设置请求
#define GUI_GET_3G_ADV_NOTIFY_CNF_RSP    -200031 //获取3G高级管理中短信通知设置回应
#define GUI_SET_3G_ADV_NOTIFY_CNF_REQ   200032 //设置3G高级管理中短信通知设置请求
#define GUI_SET_3G_ADV_NOTIFY_CNF_RSP    -200032 //设置3G高级管理中短信通知设置回应

typedef struct
{
	int m_iNotifyType;
	char m_strNotifyMsg[GUI_SMS_CONTENT_LEN + 1];
}TGui3GNotifyMsg;

typedef struct                        //获取3G通知的参数信息回应
{
	TGui3GNotifyMsg m_t3GNotifyMsg;
}TGuiGet3GNotifyCnfRsp;


typedef struct                        //设置3G通知的参数信息请求
{
	TGui3GNotifyMsg m_t3GNotifyMsg;
}TGuiSet3GNotifyCnfReq;

#define GUI_SET_3G_NOTIFY_MSG_OK    0  //设置成功
#define GUI_SET_3G_NOTIFY_MSG_ERR   1  //设置失败
typedef struct							//设置3G通知的参数信息回应
{
	int m_iRetCode;
}TGuiSet3GNotifyCnfRsp;

//3G管理-高级设置-VPN
#define GUI_GET_3G_ADV_VPN_CNF_REQ  200035 //获取3G高级管理中VPN配置请求
#define GUI_GET_3G_ADV_VPN_CNF_RSP   -200035 //获取3G高级管理中VPN配置回应
#define GUI_SET_3G_ADV_VPN_CNF_REQ  200036 //设置3G高级管理中VPN配置请求
#define GUI_SET_3G_ADV_VPN_CNF_RSP   -200036 //设置3G高级管理中VPN配置回应

typedef struct
{
	char m_strDial[GUI_DIAL_NUMBER + 1];
	char m_strAccount[GUI_DIAL_NUMBER + 1];
	char m_strPassword[GUI_DIAL_NUMBER + 1];
}TGui3GVPNMsg;
typedef struct                     //获取3G VPN 参数信息配置回应
{
	TGui3GVPNMsg m_t3GVPNMsg;
}TGuiGet3GVPNCnfRsp;


typedef struct                     //设置3G VPN 参数信息配置请求
{
	TGui3GVPNMsg m_t3GVPNMsg;
}TGuiSet3GVPNCnfReq;

#define GUI_SET_3G_VPN_CNF_OK    0  //设置成功
#define GUI_SET_3G_VPN_CNF_ERR   1  //设置失败

typedef struct                    //设置3GVPN 参数信息配置回应
{
    int m_iRetCode;
}TGuiSet3GVPNCnfRsp;

//3G 管理-高级设置 - SIM 卡
#define GUI_GET_3G_ADV_SIM_CNF_REQ    200037 //获取3G高级管理中SIM卡号配置请求
#define GUI_GET_3G_ADV_SIM_CNF_RSP    -200037//获取3G高级管理中SIM卡号配置回应
#define GUI_SET_3G_ADV_SIM_CNF_REQ    200038 //设置3G高级管理中SIM卡号请求
#define GUI_SET_3G_ADV_SIM_CNF_RSP    -200038// 设置3G高级管理中SIM卡号回应

#define GUI_3G_ADV_SIM_LEN     31
typedef struct
{
	char m_strSim[GUI_3G_ADV_SIM_LEN + 1];
}TGui3GSimMsg;
typedef struct							//获取SIM请求回应
{
	TGui3GSimMsg m_t3GSimMsg;
}TGuiGet3GSimCnfRsp;

typedef struct              //设置SIM请求
{
	TGui3GSimMsg m_t3GSimMsg;
}TGuiSet3GSimCnfReq;  

typedef struct                      //设置SIM回应
{
	int m_iRetCode;
}TGuiSet3GSimCnfRsp;

//--------------------------------------------------------------
//GPS信息叠加预览
#define GUI_GET_GPS_MSG_REQ          200041//获取GPS地址信息请求
#define GUI_GET_GPS_MSG_RSP          -200041//获取GPS地址信息回应

#define GUI_GPS_MSG_LEN  31
#define GUI_GPS_DSP_OFF  0   //不显示
#define GUI_GPS_DSP_ON   1   //显示

typedef struct   //获取GPS地址信息回应
{
	int m_iDspEnable;
	char m_cGPSMsg[GUI_GPS_MSG_LEN + 1];
}TGuiGPSMsg;

typedef struct
{
	TGuiGPSMsg m_tGpsMsg;
}TGuiGetGPSMsgRsp;

#define GUI_SET_GPS_MSG_REQ          200042//设置GPS地址信息请求
#define GUI_SET_GPS_MSG_RSP          -200042//设置GPS地址信息回应
#define GUI_SET_GPS_CNF_OK			0
#define GUI_SET_GPS_CNF_ERR			1

typedef struct
{
	TGuiGPSMsg m_tGpsMsg;
}TGuiSetGPSMsgReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetGPSMsgRsp;

//=============================
#define GUI_GET_GPS_MODULE_TYPE_REQ          200043//获取GPS模块类型请求
#define GUI_GET_GPS_MODULE_TYPE_RSP          -200043//获取GPS模块类型回应

#define MODULE_TYPE_GPS		0
#define MODULE_TYPE_BEIDOU	1

typedef struct
{
	int m_iGPSModuleType;		//GPS模块类型，0：GPS，1：北斗
}TGuiGetGPSModuleTypeRsp;

//=============================
#define GUI_GPS_TIMING_DISENABLE  0 //未使能
#define GUI_GPS_TIMING_ENABLE  	  1 //使能 

#define GUI_GET_GPS_TIMING_STA_REQ	200044 //获取GPS校时使能状态请求
#define GUI_GET_GPS_TIMING_STA_RSP	-200044 //获取GPS校时使能状态回应

typedef struct								
{	
	int m_iEnable; //见宏定义
}TGuiGetGPStimingStatRsp;

#define GUI_SET_GPS_TIMING_STA_REQ          200045//设置GPS校时使能状态请求
#define GUI_SET_GPS_TIMING_STA_RSP          -200045//设置GPS校时使能状态回应

typedef struct								
{	
	int m_iEnable; //见宏定义
}TGuiSetGPStimingStatReq;

typedef struct								
{	
	int m_iRetCode; //公共宏定义返回值
}TGuiSetGPStimingStatRsp;
//=============================








//#define GUI_TEST   //某些功能的重启使能，release版本应该注释掉
#endif
