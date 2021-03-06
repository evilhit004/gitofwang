/********************************************************************************/
// DSPexPC.c
// 版本：		1.04
// 文件说明：	DSP与PC通讯c文件
// 编写者：		车鹏宇
// 编写时间：	2014.08.01.
// 更新时间：	2015.02.04.
// 更新说明：
//	2014.08.24.
//		1.将UART_Txmit改成UART_Send函数
//		2.assemble_data函数中发送数据流时不调用UART_Send函数而使用宏
//		  函数实现，以节省时间开销	
//	2014.09.10.
//		1.因为修改了fpga_uart.c，对应地将函数pUART_Txmit改为pfUART_Txmit，
//		  函数功能不变	
//	2015.02.04.
//		1.更新了Execute_DownLoad函数，增加了以下功能：
//			a)下载软件版本信息
//		2.去掉了queue_FirmV数组变量，固件版本号应保存在ROM中，并由固件程序从ROM
//		  中读取后发送(未实现)
/********************************************************************************/

#include <math.h>
#include "DSPexPC.h"
#include "flash.h"
#include "timeout.h"
#include "fpga_uart.h"
							
unsigned int sector_addr[11] = {flashSA0, flashSA1,	flashSA2, flashSA3,	flashSA4, flashSA5,
								flashSA6, flashSA7,	flashSA8, flashSA9,	flashSA10};	

/*-------------------------------------------------------------------
 	指令解析函数
	queue_cmd:		指令序列
	com_addrbase：	串口基地址
	返回值：		0-解析完毕
	流程:
		1.查询串口接收
		2.如果接收到2个帧头，则开启串口接收timeout机制
		3.如果接收timeout或者校验和错误，则重新开始解析流程
		4.如果解析正确，则退出解析流程，指令帧保存在queue_cmd中			
-------------------------------------------------------------------*/
int	parse_command(unsigned char	queue_cmd[], unsigned int com_addrbase)
{
	int	flag_timeout = 0;			// 串口接收超时标志
	int flag_gethead = 0;			// 接收到指令帧头标志
	unsigned short check_sum = 0;	
	int	i =	0; 
	int step = 0;
	unsigned char RxData = 0;
	
	while(1)
	{
		flag_timeout = tUART_Recv(&RxData,com_addrbase);	// 读串口数据
		
		if(flag_gethead == 1)
		{
			if(flag_timeout == 1)
			{
				command_status(S_TIMEOUT,com_addrbase);		// 发送指令帧不完整(通讯超时)状态指令
				flag_gethead = 0;
				step = 0;				
				continue;
			}
		}
		
		switch(step)
		{
			case 0:		// 判帧头1							
				
				if(RxData == CMD_HEADER1)
				{
					queue_cmd[0] = RxData;
					step = 1;
				}
				else
				{
					step = 0;
				}
				
				break;
				
			case 1:		// 判帧头2
				if(RxData == CMD_HEADER2)
				{
					queue_cmd[1] = RxData;
					flag_gethead  = 1;			// 置接收到帧头标志
					step = 2;				
				}
				else
				{
					step = 0;
				}
				
				break;
			
			case 2:		// 解析长度
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// 解析信息 
				
				queue_cmd[i + LEN_C_HEADER + LEN_L] = RxData;
				i++;
				
				if(i >= queue_cmd[2])
				{					
					flag_gethead = 0;	// 清接收到帧头标志
					step = 0;
										
					for(i=0;i<(LEN_C_HEADER + LEN_L + queue_cmd[2] - LEN_C_CHECK);i++)
					{
						check_sum =	check_sum +	queue_cmd[i];
					}
					
					if(check_sum ==	((queue_cmd[i+1]<<8) + queue_cmd[i]))
					{							
						check_sum = 0;
						return(0);		// 退出指令解析流程							
					}
					else
					{
						check_sum = 0;
						command_status(S_CHKERR,com_addrbase);		// 发送校验错误状态指令	
					}
				}
				else
				{					
					step = 3;	
				}
					
				break;
				
			default:
				break;
		}
	}
}
/*-------------------------------------------------------------------
	数据解析函数
		解析1帧数据，如果发生超时或者校验错误将退出当前的解析流程，
	并返回解析的状态				
	data_queue：	解析得到的数据帧
	com_addrbase：	串口基地址
	返回值：		0-解析完毕
					1-S_TIMEOUT 超时
					2-S_CHKERR	校验和错误
-------------------------------------------------------------------*/
int parse_data(unsigned char queue_data[], unsigned int com_addrbase)
{
	int	flag_timeout = 0;
	unsigned short check_sum = 0;	
	int	i =	0;
	int step = 0; 
	unsigned char RxData = 0;
		
	while(1)
	{
		flag_timeout = tUART_Recv(&RxData, com_addrbase);
		if(flag_timeout == 1)		// 发生串口接收超时
		{
			return(S_TIMEOUT);		// 退出，返回超时
		}
		
		switch(step)
		{
			case 0:		// 判帧头1
				
				if(RxData == DATA_HEADER1)
				{
					queue_data[0] = RxData;
					step = 1;
				}
				else
				{
					step = 0;
				}
				
				break;
				
			case 1:		// 判帧头2
				if(RxData == DATA_HEADER2)
				{
					queue_data[1] = RxData;					
					step = 2;				
				}
				else
				{
					step = 0;
				}
				
				break;
			
			case 2:		// 解析长度
				queue_data[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// 解析信息 
				
				queue_data[i + LEN_D_HEADER + LEN_L] = RxData;
				i++;
				
				if(i >= queue_data[2])
				{			
					step = 0;
					
					for(i=0;i<(LEN_D_HEADER + LEN_L + queue_data[2] - LEN_D_CHECK);i++)
					{
						check_sum =	check_sum +	queue_data[i];
					}
					
					if(check_sum ==	((queue_data[i+1]<<8) + queue_data[i]))
					{							
						check_sum = 0;
						return(0);			// 退出，返回数据解析完毕						
					}
					else
					{
						check_sum = 0;
						return(S_CHKERR);	// 退出，返回校验和错误
					}
				}
				else
				{					
					step = 3;	
				}
					
				break;
				
			default:
				break;
		}
			
	}

}
/*-------------------------------------------------------------------
-- 数据请求指令
-- data_type：	数据类型
-- rq_reason：	请求原因
-- data_frame：	数据帧号
-- com_addrbase：串口基地址
-- 返回值：		无
-------------------------------------------------------------------*/
void command_DRq(unsigned char data_type, unsigned char	rq_reason, unsigned	char data_frame, unsigned int com_addrbase)
{
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_DRQ + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// 帧头
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// 单帧长度
	pfUART_Txmit(CODE_CMD_DRQ,com_addrbase);		// 指令代码
	
	pfUART_Txmit(data_type,com_addrbase);			// 数据类型
	pfUART_Txmit(rq_reason,com_addrbase);			// 请求原因
	pfUART_Txmit(data_frame,com_addrbase);		// 请求数据帧号
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_DRQ + data_type + rq_reason + data_frame;
	pfUART_Txmit(check_sum,com_addrbase);			// 校验和
	pfUART_Txmit((check_sum>>8),com_addrbase);		
}
/*-------------------------------------------------------------------
-- 状态指令
-- status_type：	状态类型
-- com_addrbase：	串口基地址
-- 返回值：			无
-------------------------------------------------------------------*/
void command_status(unsigned char status_type, unsigned	int	com_addrbase)
{
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_STA + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// 帧头
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// 单帧长度
	pfUART_Txmit(CODE_CMD_STA,com_addrbase);		// 指令代码
	pfUART_Txmit(status_type,com_addrbase);		// 状态类型
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_STA + status_type;
	
	pfUART_Txmit(check_sum,com_addrbase);			// 校验和
	pfUART_Txmit((check_sum>>8),com_addrbase);
}
/*-------------------------------------------------------------------
-- 上传函数
-- data_type:		上传的数据类型
-- com_addrbase：	串口基地址
-- 返回值：			4 - S_ERAERR	FLASH擦除错误
					5 - S_PRGERR	FLASH烧写错误	
					8 - S_UPDONE	上传完毕
-------------------------------------------------------------------*/						
int Execute_UpLoad(unsigned char data_type, unsigned int com_addrbase)
{
	int	i =	0;
	unsigned char 	status = 0;
	unsigned char 	queue_data[SIZE_D_BUFF];
	unsigned int  	flash_addr;
	unsigned short 	flash_data = 0;
	unsigned char 	len_program = 0;
	unsigned char 	frame_num = 0;
	unsigned char 	rq_reason = 0;
	
	switch(data_type)		// 判断上传数据类型
	{
		case PROGRAM:		// 程序
			
			for(i=FSTART_PROG;i<FSTART_COEFF;i++)			// 擦除FLASH
			{
				status = sector_erase(sector_addr[i]);
				
				if(status == 1)								// 如果FLASH擦除失败就终止上传流程
				{
					command_status(S_ERAERR,com_addrbase);	// 发送擦除失败状态指令
					return(S_ERAERR);							
				}
			}
			
			flash_addr = sector_addr[FSTART_PROG];			// 程序段的FLASH起始地址								
			
			break;
			
		case COEFF:			// 系数
			
			status = sector_erase(sector_addr[FSTART_COEFF]);		
			flash_addr = sector_addr[FSTART_COEFF];			// 系数段的FLASH起始地址		
			
			break;
		
		default:
	
			break;
	}	// 分支判断结束		
				
				
	// 如果FLASH擦除成功则绦洗鞒�
				
	command_status(S_ERAOK,com_addrbase);		// 发送擦除成功状态指令
										
	while(1)		
	{
		command_DRq(data_type,rq_reason,frame_num,com_addrbase);		// 发送数据请求指令
		rq_reason = parse_data(queue_data,com_addrbase);		// 解析程序数据
		
		if(rq_reason != 0)
		{
			continue;	
		}
		
		len_program = queue_data[2] - LEN_D_INDICATE - LEN_D_CHECK;
		
		i = 0;
		
		while(i < len_program)
		{
			// 
			flash_data = (queue_data[i + LEN_D_HEADER + LEN_L + LEN_D_INDICATE + 1]<<8) + queue_data[i + LEN_D_HEADER + LEN_L + LEN_D_INDICATE];	
			status = flash_program((flash_addr+i),flash_data);
			i = i + 2;	
						
			if(status == 1)								// 如果烧写失败就停止上传流程
			{		
				command_status(S_PRGERR,com_addrbase);	// 发送烧写失败状态指令	
				return(S_PRGERR);
			}
		}
		
		//如果烧写正常则继续上传流程
		
		flash_addr = flash_addr	+ len_program;
		
		frame_num++;
		
		if( (queue_data[4] == 0) && (queue_data[5] == 0) )	// 如果剩余帧数目为0
		{
			command_status(S_UPDONE,com_addrbase);	// 发送上传结束状态指令，并结束上传流程
			return(S_UPDONE);
		}
	}		
}
/*-------------------------------------------------------------------
-- 下载函数
-- data_type:		下载的数据类型
-- data_size:		下载的数据大小(KByte)
-- com_addrbase：	串口基地址
-------------------------------------------------------------------*/						
int Execute_DownLoad(unsigned char	data_type, unsigned char data_size, unsigned int com_addrbase)
{
	int	i =	0;
	unsigned char 	data_queue[SIZE_D_BUFF];	
	unsigned int  	flash_addr;
	unsigned char 	lenth_remain = 0;		// 剩余零散字节长度
	unsigned short 	frame_num = 0;			// 剩余帧数目
	unsigned char 	len = 0;				// 当前帧字节长度
	unsigned int  	flash_data = 0;
	long			size = 0;				// 下载数据大小
	
	switch(data_type)	
	{
		case PROGRAM:

			size = data_size * 1024;		
			flash_addr = sector_addr[FSTART_PROG];		// 程序段的FLASH起始地址								
			
			break;
			
		case COEFF:
			
			size = data_size * 1024;			
			flash_addr = sector_addr[FSTART_COEFF];		// 系数段的FLASH起始地址		
			
			break;
			
		case FIRMV:
			
			size = LEN_D_INFO_FIRMV;
			
			break;
			
		case ADDRCHK:
						
			break;			
			
		case SOFTV:
			
			size = LEN_D_INFO_SOFTV;	
			
			break;					
		
		default:
	
			break;
	}	// 分支判断结束
		
	frame_num = ceil( ((float)size) / LEN_UPDN_ASM );		// 程序段总帧数
	lenth_remain = size % LEN_UPDN_ASM;			// 程序段最后一帧信息长度
	
	while(frame_num)
	{
		frame_num--;
		
		if(frame_num == 0)				// 最后是最后一帧，则判是否是残余的
		{
			if(lenth_remain == 0)		// 如果是完整的100字节
			{
				len = LEN_UPDN_ASM;
			}
			else						// 如果是残余的
			{
				len = lenth_remain;
			}
		}
		else
		{
			len = LEN_UPDN_ASM;
		}
		
		i = 0;
		
		switch(data_type)
		{
			case FIRMV:			// 请求固件版本号
												
				break;	
				
			case ADDRCHK:		// 请求地址扫描数据
												
				break;				
				
			case SOFTV:			// 请求软件版本号
				
				for(i=0;i<16;i++)
				{
					data_queue[i] = info_Version[i];	
				}
				
				break;			
				
			default:			// 固件/程序/系数下载
				
				while(i < len)
				{
					flash_data = *((unsigned short *)(flash_addr + i));
					data_queue[i] = (unsigned char)(flash_data);
					data_queue[i+1] = (unsigned char)(flash_data>>8);
					i = i + 2;	
				}
		
				flash_addr = flash_addr + len;
				
				break;
		}
		
		assemble_data(data_type, len, frame_num, data_queue, com_addrbase);	
	}

	return(0);	
}
/*-------------------------------------------------------------------
-- 下载数据的打包和发送
-- data_type：		数据类型
-- len_info：		信息段长度
-- frame_num：		剩余帧数目
-- queue_data： 	信息数组
-- com_addrbase：	串口基地址
-------------------------------------------------------------------*/
void assemble_data(unsigned	char data_type, unsigned char len_info, unsigned short frame_num, unsigned char queue_data[], unsigned int com_addrbase)
{
	unsigned short check_sum = 0;
	int	i =	0;
	unsigned char len = 0;
	unsigned char status = 0;
	
	len = len_info + LEN_D_INDICATE + LEN_D_CHECK;
	
	pfUART_Txmit(DATA_HEADER1,com_addrbase);	
	pfUART_Txmit(DATA_HEADER2,com_addrbase);	
	pfUART_Txmit(len,com_addrbase);
	pfUART_Txmit(data_type,com_addrbase);
	pfUART_Txmit(frame_num,com_addrbase);
	pfUART_Txmit((frame_num>>8),com_addrbase);
	
	check_sum =	DATA_HEADER1 + DATA_HEADER2	+ len + data_type + (0xff & frame_num) + (0xff & (frame_num >> 8));
	
	for(i=0;i<len_info;i++)
	{
		// 查询FIFO满标志
		status = UART_LSR_Read(com_addrbase);		// *1	此处串口的发送操作与UART_Send函数相同
		while((status & 0x80) == 0x80)				//		不使用UART_Send函数的原因是发送数据流时频繁调用函数会增加时间开销
		{
			status = UART_LSR_Read(com_addrbase);
		}		
		UART_Txmit(queue_data[i], com_addrbase);
		
		check_sum =	check_sum +	queue_data[i];
	}	
	
	pfUART_Txmit(check_sum,com_addrbase);
	pfUART_Txmit((check_sum>>8),com_addrbase);		
}
/*-------------------------------------------------------------------
-- 发送握手指令函数
-------------------------------------------------------------------*/
void command_link(unsigned int com_addrbase)
{	
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_LINK + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// 帧头
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// 单帧长度
	pfUART_Txmit(CODE_CMD_LINK,com_addrbase);		// 指令代码
	
	pfUART_Txmit(0x7E,com_addrbase);				
	pfUART_Txmit(0x7E,com_addrbase);				
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_LINK + 0x7E + 0x7E;
	pfUART_Txmit(check_sum,com_addrbase);			// 校验和
	pfUART_Txmit((check_sum>>8),com_addrbase);		
}
/*-------------------------------------------------------------------
 	握手函数(等待查询版)
	com_addrbase：	串口基地址
	返回值：		0	握手成功	
					-1	握手超时
	流程:
		1.查询串口接收，如果1s内未收到串口数据则time_cnt加1，并重新开
		  始流程
		2.如果time_cnt超过5s，退出流程并返回-1
		3.如果接收到握手指令帧，退出流程并返回0；否则重新开始流程		
-------------------------------------------------------------------*/
int	commu_link1(unsigned int com_addrbase)
{
	int	flag_timeout = 0;			// 串口接收超时标志
	unsigned short check_sum = 0;	
	int	i =	0; 
	int step = 0;
	unsigned char RxData = 0;
	int time_cnt = 0;
	unsigned char queue_cmd[SIZE_C_BUFF];
	
	while(1)
	{
		flag_timeout = tUART_Recv(&RxData,com_addrbase);	// 读串口数据
		
		if(flag_timeout == 1)
		{	
			if(time_cnt >= TIME_WTCOMMU)	// 超时5s
			{
				return(-1);
			}
			else
			{
				time_cnt++;
				continue;
			}			
		}
		
		switch(step)
		{
			case 0:		// 判帧头1							
				
				if(RxData == CMD_HEADER1)
				{
					queue_cmd[0] = RxData;
					step = 1;
				}
				else
				{
					step = 0;
				}
				
				break;
				
			case 1:		// 判帧头2
				if(RxData == CMD_HEADER2)
				{
					queue_cmd[1] = RxData;
					step = 2;				
				}
				else
				{
					step = 0;
				}
				
				break;
			
			case 2:		// 解析长度
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// 解析信息 
				
				queue_cmd[i + LEN_C_HEADER + LEN_L] = RxData;
				i++;
				
				if(i >= queue_cmd[2])
				{
					for(i=0;i<(LEN_C_HEADER + LEN_L + queue_cmd[2] - LEN_C_CHECK);i++)
					{
						check_sum =	check_sum +	queue_cmd[i];
					}
					
					if(check_sum ==	((queue_cmd[i+1]<<8) + queue_cmd[i]))
					{							
						check_sum = 0;
						
						if(queue_cmd[3] == CODE_CMD_LINK)
						{
							command_status(S_LINK,com_addrbase);	// 发送握手成功状态指令							
							return(0);								// 退出指令解析流程	
						}						
					}
					else
					{
						check_sum = 0;
					}
	
					step = 0;
				}
				else
				{					
					step = 3;	
				}
					
				break;
				
			default:
				break;
		}
	}
}
/*-------------------------------------------------------------------
 	握手函数(中断查询版)
	com_addrbase：	串口基地址
	返回值：		0	握手成功	
					-1	未握手
	流程:
		1.查询串口接收，如果串口中有数据则读出并进行指令解析；如果没数
		  据就退出流程并返回-1
		2.如果接收到握手命令帧退出流程并返回0；否则退出并返回-1	
-------------------------------------------------------------------*/
int	commu_link(unsigned int com_addrbase)
{
	unsigned char status = 0;
//	int	flag_timeout = 0;			// 串口接收超时标志
	unsigned short check_sum = 0;	
	static int	i =	0; 
	static int step = 0;
	unsigned char RxData = 0;
//	int time_cnt = 0;
	static unsigned char queue_cmd[SIZE_C_BUFF];
	
	status = UART_LSR_Read(com_addrbase);
	
	while( (status & 0x1) == 0x1)
	{
		RxData = UART_Recv(com_addrbase);
		status = UART_LSR_Read(com_addrbase);
		
		switch(step)
		{
			case 0:		// 判帧头1							
				
				if(RxData == CMD_HEADER1)
				{
					queue_cmd[0] = RxData;
					step = 1;
				}
				else
				{
					step = 0;
				}
				
				break;
				
			case 1:		// 判帧头2
				if(RxData == CMD_HEADER2)
				{
					queue_cmd[1] = RxData;
					step = 2;				
				}
				else
				{
					step = 0;
				}
				
				break;
			
			case 2:		// 解析长度
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// 解析信息 
				
				queue_cmd[i + LEN_C_HEADER + LEN_L] = RxData;
				i++;
				
				if(i >= queue_cmd[2])
				{
					for(i=0;i<(LEN_C_HEADER + LEN_L + queue_cmd[2] - LEN_C_CHECK);i++)
					{
						check_sum =	check_sum +	queue_cmd[i];
					}
					
					if(check_sum ==	((queue_cmd[i+1]<<8) + queue_cmd[i]))
					{							
						check_sum = 0;
						
						if(queue_cmd[3] == CODE_CMD_LINK)
						{
							command_status(S_LINK,com_addrbase);	// 发送握手成功状态指令							
							return(0);								// 退出指令解析流程	
						}						
					}
					else
					{
						check_sum = 0;
					}
	
					step = 0;
				}
				else
				{					
					step = 3;	
				}
					
				break;
				
			default:
				break;
		}		
	}
	
	return(-1);
}
