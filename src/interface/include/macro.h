/*
 *	file: onvif_extern.c
 *	description: onvif macro
 *	history: dongliqiang created at 20120129
 */

#ifndef _MACRO__H_
#define _MACRO__H_
 
//注:所有范围值将不存入配置文件中,直接增加至此处

//added by qiaohaijun @2013.5.29
#define 	AUDIO_SAMPLE_NUM 			3			//采样率列表的数量
#define		AUDIO_SAMPLE_8K				8			//8K采样率
#define		AUDIO_SAMPLE_32K			32			//32K采样率
#define		AUDIO_SAMPLE_48K			48			//48K采样率
#define		AUDIO_SAMPLE_QUOTIETY		1000		//例:AUDIO_SAMPLE_8K * AUDIO_SAMPLE_QUOTIETY == 8000,采样率为8000
//end

//added by ptb at 20130128
#define		DEFAULT_AUDIOBITRATE		64000       //默认音频码率
#define		DEFAULT_AUDIOSAMPLERATE		8000        //默认音频采样率
#define		PAL_MAXFRAMERATE            25          //最大帧率PAL
#define		NTSC_MAXFRAMERATE           30          //最大帧率NTSC

//added by qiaohaijun at 2013.1.6
#define		MULT_ADDR					("239.255.255.250")
#define		MULT_PORT					3702

//device info
#define		DEVICE_MANUFACTURER				("AglaiaSense")
#define		DEVICE_MODEL					("GS500")

//VideoSourceConfigurationsResponse
#define		VSCBOUNDSX					0;			//vscboundsx;
#define		VSCBOUNDSY					0;			//vscboundsy;
#define		VSCBOUNDSW					512;		//vscboundsw;
#define		VSCBOUNDSH					240;		//vscboundsh;

//VideoSourceConfigurationOptions
#define		VSCOBRXMIN					0			//vscobrxmin
#define		VSCOBRXMAX					100			//vscobrxmax
#define		VSCOBRYMIN					0			//vscobrymin
#define		VSCOBRYMAX					100			//vscobrymax
#define		VSCOBRWMIN					0			//vscobrwmin
#define		VSCOBRWMAX					512		//vscobrwmax
#define		VSCOBRHMIN					0			//vscobrhmin
#define		VSCOBRHMAX					240		//vscobrhmax

//VideoEncoderConfigurationOptionsResponse
#define		VECOQRMIN					6			//vecoqrmin	
#define		VECOQRMAX					14			//vecoqrmax
#define		VECOJPEGFRRMIN				1			//vecojpegfrrmin - FrameRateRange
#define		VECOJPEGFRRMAX				30			//vecojpegfrrmax
#define		VECOJPEGEIRMIN				1			//vecojpegeirmin - EncodingIntervalRange
#define		VECOJPEGEIRMAX				30			//vecojpegeirmax

#define		VECOH264GLRMIN				10			//I帧帧率范围
#define		VECOH264GLRMAX				100			
#define		VECOH264GLRMIN_DVR			10			
#define		VECOH264GLRMAX_DVR			255			

#define		VECOH264FRRMIN				1			//vecoh264frrmin - FrameRateRange
#define		VECOH264FRRMAX				30			//vecoh264frrmax
#define		VECOH264EIRMIN				1			//vecoh264eirmin - EncodingIntervalRange
#define		VECOH264EIRMAX				30			//vecoh264eirmax
#define		VECOH264PS_SIZE				4			//vecoh264ps_size
#define		VECOH264PSMODE_01			0			//vecoh264psmode_01 - Baseline
#define		VECOH264PSMODE_02			1			//vecoh264psmode_02 - Main
#define		VECOH264PSMODE_03			2			//vecoh264psmode_03 - Extended
#define		VECOH264PSMODE_04			3			//vecoh264psmode_04 - High
#define		VECOBITRATEMIN				0			//vecobitratemin
#define		VECOBITRATEMAX				8192		//vecobitratemax

//AudioEncoderConfigurationsResponse
#define		AECMULADDRTP				0			//aecmuladdrtp			= 0					;0 -- ipv4, 1 -- ipv6
#define		AECMULADDRIPV4				("239.255.255.250")//aecmuladdripv4		= 239.255.255.250
#define		AECMULPORT					8001		//aecmulport				= 8001
#define		AECMULTTL					64			//aecmulttl				= 64
#define		AECMULAS					0			//aecmulas				= 0					;0 -- faulse, 1 -- true
#define		AECST						60000		//aecst					= 60000				;SessionTimeout
#define		AECOBL_SIZE					1			//aecobl_size				= 1
#define		AECOBL_01					4096		//aecobl_01				= 4096
#define		AECOSL_SIZE					1			//aecosl_size
#define		AECOSL_01					8000		//aecosl_01				= 8000

//VideoSourcesResponse
#define		VSFREAMRATE					25			//video freamrate
#define		VSVRWIDTH					512		//vsvrwidth
#define		VSVRHEIGHT					240		//vsvrheight
#define		VSIOBCOLEVMIN				0			//vsiobcolevmin
#define		VSIOBCOLEVMAX				1			//vsiobcolevmax
#define		VSIOBCOBNMIN				0			//vsiobcobnmin
#define		VSIOBCOBNMAX				255.0		//vsiobcobnmax
#define		VSIOBCOCSMIN				0			//vsiobcocsmin
#define		VSIOBCOCSMAX				255.0		//vsiobcocsmax
#define		VSIOBCOCTMIN				0			//vsiobcoctmin
#define		VSIOBCOCTMAX				255.0		//vsiobcoctmax
#define		VSIOEOMINETMIN				0			//MinExposureTime, min
#define		VSIOEOMINETMAX				40000		//MinExposureTime, max
#define		VSIOEOMAXETMIN				0			//MaxExposureTime, min
#define		VSIOEOMAXETMAX				40000		//MaxExposureTime, max
#define		VSIOEOMINGMIN				0			//MinGain, min
#define		VSIOEOMINGMAX				128			//MinGain, max
#define		VSIOEOMAXGMIN				0			//MaxGain, min
#define		VSIOEOMAXGMAX				128			//MaxGain, max
#define		VSIOEOMINIMIN				0			//MinIris, min
#define		VSIOEOMINIMAX				1			//MinIris, max
#define		VSIOEOMAXIMIN				0			//MaxIris, min
#define		VSIOEOMAXIMAX				1			//MaxIris, max
#define		VSIOEOETMIN					0			//ExposureTime, min
#define		VSIOEOETMAX					40000		//ExposureTime, max
#define		VSIOEOGMIN					0			//Gain, min
#define		VSIOEOGMAX					128			//Gain, max
#define		VSIOEOIMIN					-1			//Iris, min
#define		VSIOEOIMAX					1			//Iris, max
#define		VSIOFODSMIN					0			//DefaultSpeed, min
#define		VSIOFODSMAX					1			//DefaultSpeed, max
#define		VSIOFONLMIN					0			//NearLimit, min
#define		VSIOFONLMAX					1			//NearLimit, max
#define		VSIOFOFLMIN					0			//FarLimit, min
#define		VSIOFOFLMAX					1			//FarLimit, max
#define		VSIOSNMIN					0			//Sharpness, min
#define		VSIOSNMAX					255			//Sharpness, max
#define		VSIOWDRLEVMIN				0			//Sharpness, min
#define		VSIOWDRLEVMAX				1			//Sharpness, max
#define		VSIOWBYRGMIN				0			//YrGain, min
#define		VSIOWBYRGMAX				255			//YrGain, max
#define		VSIOWBYBGMIN				0			//YbGain, min
#define		VSIOWBYBGMAX				255			//YbGain, max
#define		ASCHN						1			//aschn
#define		AOCOSPO_SIZE				2			//aocospo_size	
#define		AOCOOLR_MAX					100			//aocoolr_max
#define		AOCOOLR_MIN					1			//aocoolr_min

#define		TIMEINTERVALNUM				5			//DVR支持的时间段个数
#define		BRIGHTNESS_MAX				100			//Brightness
#define		BRIGHTNESS_MIN				1
#define		COLOR_SATURATION_MAX		100			//ColorSaturation
#define		COLOR_SATURATION_MIN		1
#define		CONTRAST_MAX				100			//Contrast
#define		CONTRAST_MIN				1
#define		SHARPNESS_MAX				100			//Sharpness
#define		SHARPNESS_MIN				1

#define     PRESET_HOME                 2           //0和1号位置不能设置预置点
#endif

