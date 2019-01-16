/*-------------------------------------------------------------------
文件名称：	DSPexPC.h(DSP端)
版本：		1.04
编写时间：	2014.08.01.
更新时间：	2015.02.04.
编写人：	车鹏宇
更新说明：
	2014.09.12.		
		1.去掉TIME_WTCOMMU的宏定义，统一放在timeout.h文件中	
	2015.01.29.		
		1.增加了通信握手成功与失败的标志定义COMMU_SUCC和COMMU_FAULT	
	2015.02.04.		
		1.增加了软件版本号的数据类型及其相关的宏定义SOFTV和LEN_D_INFO_SOFTV	
		2.增加了全局变量info_Version[16]的声明，用于传递软件版本信息
		  该全局变量在main.c中被定义和赋值，在DSPexPC.c中被引用		
		3.增加了设置指令代码及其相关宏定义
		4.增加了输出数据格式的设置代码的宏定义，实现设置DSP输出数据
		  格式的功能	
-------------------------------------------------------------------*/
#ifndef __DSPEXPC_H
#define __DSPEXPC_H

#define	CMD_HEADER1		0xEB	// 命令帧头1
#define	CMD_HEADER2		0x90	// 命令帧头2

#define	CODE_CMD_UP		0x1		// 上传指令代码
#define	CODE_CMD_STA	0x2		// 状态指令代码
#define	CODE_CMD_DRQ	0x3		// 数据请求指令代码
#define	CODE_CMD_LINK	0x4		// 握手指令代码
#define	CODE_CMD_SET	0x5		// 设置指令代码

#define	CODE_SET_FORMAT	0x1		// 设置项-数据输出格式 代码

#define	V_FORMAT_IMU	0x0		// IMU数据格式
#define	V_FORMAT_CALIB	0x1		// 标定数据格式
#define	V_FORMAT_NAV	0x2		// 导航数据格式

#define	S_TIMEOUT		0x1		// 超时代码
#define	S_CHKERR		0x2		// 校验和错误代码
#define	S_ERAERR		0x4		// 擦除失败代码
#define	S_PRGERR		0x5		// 烧写失败代码
#define	S_ERAOK			0x6		// 擦除成功代码
#define	S_PRGEOK		0x7		// 烧写成功代码
#define	S_UPDONE		0x8		// 上传结束代码
#define	S_LINK			0x9		// 握手成功代码
#define	S_FOMATOK		0xA		// 数据输出格式设置成功代码

#define	FIRMWARE		0x1		// 固件类型代码
#define	PROGRAM			0x2		// 程序类型代码
#define	COEFF			0x3		// 系数类型代码
#define	FIRMV			0x5		// 固件版本类型代码
#define	ADDRCHK			0x6		// 地址检测类型代码
#define	SOFTV			0x8		// 软件版本类型代码

#define	DATA_HEADER1	0x55	// 数据帧头1
#define	DATA_HEADER2	0xAA	// 数据帧头2

#define	FSTART_FIRM		0		// 固件在FLASH中的起始sector号	
#define	FSTART_PROG		0		// 程序在FLASH中的起始sector号
#define	FSTART_COEFF	10		// 系数在FLASH中的起始sector号

#define	LEN_COEFF		200		// 系数数据长度
#define LEN_UPDN_ASM	100		// 上传/下载数据帧打包的信息段长度

#define LEN_D_HEADER	2		// 数据帧帧头端长度
#define LEN_D_INDICATE	3		// 数据帧指示段长度
#define LEN_D_CHECK		2		// 数据帧校验段长度
// 信息段长度 = 帧长度 - LEN_D_INDICATE - LEN_D_CHECK

#define LEN_C_HEADER	2		// 指令帧帧头端长度
#define LEN_C_INDICATE	1		// 指令帧指示段长度
#define LEN_C_CHECK		2		// 指令帧校验段长度

#define LEN_C_INFO_STA	1		// 状态指令的信息段长度		
#define LEN_C_INFO_DRQ	3		// 数据请求指令的信息段长度	
#define LEN_C_INFO_UPLOAD	1	// 上传指令的信息段长度	
#define	LEN_C_INFO_LINK	2		// 握手指令的信息段长度
#define LEN_C_INFO_SET	2		// 设置指令的信息段长度

#define LEN_D_INFO_FIRMV	3	// 固件版本数据帧的信息段长度
#define LEN_D_INFO_SOFTV	16	// 软件版本数据帧的信息段长度	

#define LEN_L			1		// 长度段长度

#define SIZE_D_BUFF		120		// 数据帧数组大小
#define SIZE_C_BUFF		20		// 指令帧数组大小	

#define COMMU_SUCC		0	
#define COMMU_FAULT		-1		

int	parse_command(unsigned char	[], unsigned int );
int parse_data(unsigned char [], unsigned int );
void command_DRq(unsigned char , unsigned char	, unsigned	char , unsigned int );
void command_status(unsigned char , unsigned int );					
int Execute_UpLoad(unsigned char , unsigned int );					
int Execute_DownLoad(unsigned char	, unsigned char , unsigned int );
void assemble_data(unsigned	char , unsigned	char , unsigned short , unsigned char [], unsigned int );
void command_link(unsigned int );
int	commu_link(unsigned int );
int	commu_link1(unsigned int );

extern unsigned char info_Version[16];

#endif	/*	__DSPEXPC_H	*/
