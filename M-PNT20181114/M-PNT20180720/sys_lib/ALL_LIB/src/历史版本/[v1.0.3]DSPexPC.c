/********************************************************************************/
// DSPexPC.c
// �汾��		2.1.0.
// �ļ�˵����	DSP��PCͨѶc�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.08.01.
// ����ʱ�䣺	2014.09.10.
// ����˵����
//	2014.08.24.
//		1.��UART_Txmit�ĳ�UART_Send����
//		2.assemble_data�����з���������ʱ������UART_Send������ʹ�ú�
//		  ����ʵ�֣��Խ�ʡʱ�俪��	
//	2014.09.10.
//		1.��Ϊ�޸���fpga_uart.c����Ӧ�ؽ�����pUART_Txmit��ΪpfUART_Txmit��
//		  �������ܲ���	
/********************************************************************************/

#include <math.h>
#include "DSPexPC.h"
#include "flash.h"
#include "timeout.h"
#include "fpga_uart.h"
							
unsigned char queue_FirmV[3] = {0x0, 0x1, 0x10};	// �̼��汾��Ϣ
unsigned int sector_addr[11] = {flashSA0, flashSA1,	flashSA2, flashSA3,	flashSA4, flashSA5,
								flashSA6, flashSA7,	flashSA8, flashSA9,	flashSA10};	

/*-------------------------------------------------------------------
 	ָ���������
	queue_cmd:		ָ������
	com_addrbase��	���ڻ���ַ
	����ֵ��		0-�������
	����:
		1.��ѯ���ڽ���
		2.������յ�2��֡ͷ���������ڽ���timeout����
		3.�������timeout����У��ʹ��������¿�ʼ��������
		4.���������ȷ�����˳��������̣�ָ��֡������queue_cmd��			
-------------------------------------------------------------------*/
int	parse_command(unsigned char	queue_cmd[], unsigned int com_addrbase)
{
	int	flag_timeout = 0;			// ���ڽ��ճ�ʱ��־
	int flag_gethead = 0;			// ���յ�ָ��֡ͷ��־
	unsigned short check_sum = 0;	
	int	i =	0; 
	int step = 0;
	unsigned char RxData = 0;
	
	while(1)
	{
		flag_timeout = tUART_Recv(&RxData,com_addrbase);	// ����������
		
		if(flag_gethead == 1)
		{
			if(flag_timeout == 1)
			{
				command_status(S_TIMEOUT,com_addrbase);		// ����ָ��֡������(ͨѶ��ʱ)״ָ̬��
				flag_gethead = 0;
				step = 0;				
				continue;
			}
		}
		
		switch(step)
		{
			case 0:		// ��֡ͷ1							
				
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
				
			case 1:		// ��֡ͷ2
				if(RxData == CMD_HEADER2)
				{
					queue_cmd[1] = RxData;
					flag_gethead  = 1;			// �ý��յ�֡ͷ��־
					step = 2;				
				}
				else
				{
					step = 0;
				}
				
				break;
			
			case 2:		// ��������
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// ������Ϣ 
				
				queue_cmd[i + LEN_C_HEADER + LEN_L] = RxData;
				i++;
				
				if(i >= queue_cmd[2])
				{					
					flag_gethead = 0;	// ����յ�֡ͷ��־
					step = 0;
										
					for(i=0;i<(LEN_C_HEADER + LEN_L + queue_cmd[2] - LEN_C_CHECK);i++)
					{
						check_sum =	check_sum +	queue_cmd[i];
					}
					
					if(check_sum ==	((queue_cmd[i+1]<<8) + queue_cmd[i]))
					{							
						check_sum = 0;
						return(0);		// �˳�ָ���������							
					}
					else
					{
						check_sum = 0;
						command_status(S_CHKERR,com_addrbase);		// ����У�����״ָ̬��	
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
	���ݽ�������
		����1֡���ݣ����������ʱ����У������˳���ǰ�Ľ������̣�
	�����ؽ�����״̬				
	data_queue��	�����õ�������֡
	com_addrbase��	���ڻ���ַ
	����ֵ��		0-�������
					1-S_TIMEOUT ��ʱ
					2-S_CHKERR	У��ʹ���
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
		if(flag_timeout == 1)		// �������ڽ��ճ�ʱ
		{
			return(S_TIMEOUT);		// �˳������س�ʱ
		}
		
		switch(step)
		{
			case 0:		// ��֡ͷ1
				
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
				
			case 1:		// ��֡ͷ2
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
			
			case 2:		// ��������
				queue_data[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// ������Ϣ 
				
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
						return(0);			// �˳����������ݽ������						
					}
					else
					{
						check_sum = 0;
						return(S_CHKERR);	// �˳�������У��ʹ���
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
-- ��������ָ��
-- data_type��	��������
-- rq_reason��	����ԭ��
-- data_frame��	����֡��
-- com_addrbase�����ڻ���ַ
-- ����ֵ��		��
-------------------------------------------------------------------*/
void command_DRq(unsigned char data_type, unsigned char	rq_reason, unsigned	char data_frame, unsigned int com_addrbase)
{
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_DRQ + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// ֡ͷ
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// ��֡����
	pfUART_Txmit(CODE_CMD_DRQ,com_addrbase);		// ָ�����
	
	pfUART_Txmit(data_type,com_addrbase);			// ��������
	pfUART_Txmit(rq_reason,com_addrbase);			// ����ԭ��
	pfUART_Txmit(data_frame,com_addrbase);		// ��������֡��
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_DRQ + data_type + rq_reason + data_frame;
	pfUART_Txmit(check_sum,com_addrbase);			// У���
	pfUART_Txmit((check_sum>>8),com_addrbase);		
}
/*-------------------------------------------------------------------
-- ״ָ̬��
-- status_type��	״̬����
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			��
-------------------------------------------------------------------*/
void command_status(unsigned char status_type, unsigned	int	com_addrbase)
{
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_STA + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// ֡ͷ
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// ��֡����
	pfUART_Txmit(CODE_CMD_STA,com_addrbase);		// ָ�����
	pfUART_Txmit(status_type,com_addrbase);		// ״̬����
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_STA + status_type;
	
	pfUART_Txmit(check_sum,com_addrbase);			// У���
	pfUART_Txmit((check_sum>>8),com_addrbase);
}
/*-------------------------------------------------------------------
-- �ϴ�����
-- data_type:		�ϴ�����������
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			4 - S_ERAERR	FLASH��������
					5 - S_PRGERR	FLASH��д����	
					8 - S_UPDONE	�ϴ����
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
	
	switch(data_type)		// �ж��ϴ���������
	{
		case PROGRAM:		// ����
			
			for(i=FSTART_PROG;i<FSTART_COEFF;i++)			// ����FLASH
			{
				status = sector_erase(sector_addr[i]);
				
				if(status == 1)								// ���FLASH����ʧ�ܾ���ֹ�ϴ�����
				{
					command_status(S_ERAERR,com_addrbase);	// ���Ͳ���ʧ��״ָ̬��
					return(S_ERAERR);							
				}
			}
			
			flash_addr = sector_addr[FSTART_PROG];			// ����ε�FLASH��ʼ��ַ								
			
			break;
			
		case COEFF:			// ϵ��
			
			status = sector_erase(sector_addr[FSTART_COEFF]);		
			flash_addr = sector_addr[FSTART_COEFF];			// ϵ���ε�FLASH��ʼ��ַ		
			
			break;
		
		default:
	
			break;
	}	// ��֧�жϽ���		
				
				
	// ���FLASH�����ɹ�������ϴ�����
				
	command_status(S_ERAOK,com_addrbase);		// ���Ͳ����ɹ�״ָ̬��
										
	while(1)		
	{
		command_DRq(data_type,rq_reason,frame_num,com_addrbase);		// ������������ָ��
		rq_reason = parse_data(queue_data,com_addrbase);		// ������������
		
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
						
			if(status == 1)								// �����дʧ�ܾ�ֹͣ�ϴ�����
			{		
				command_status(S_PRGERR,com_addrbase);	// ������дʧ��״ָ̬��	
				return(S_PRGERR);
			}
		}
		
		//�����д����������ϴ�����
		
		flash_addr = flash_addr	+ len_program;
		
		frame_num++;
		
		if( (queue_data[4] == 0) && (queue_data[5] == 0) )	// ���ʣ��֡��ĿΪ0
		{
			command_status(S_UPDONE,com_addrbase);	// �����ϴ�����״ָ̬��������ϴ�����
			return(S_UPDONE);
		}
	}		
}
/*-------------------------------------------------------------------
-- ���غ���
-- data_type:		���ص���������
-- data_size:		���ص����ݴ�С(KByte)
-- com_addrbase��	���ڻ���ַ
-------------------------------------------------------------------*/						
int Execute_DownLoad(unsigned char	data_type, unsigned char data_size, unsigned int com_addrbase)
{
	int	i =	0;
	unsigned char 	data_queue[SIZE_D_BUFF];	
	unsigned int  	flash_addr;
	unsigned char 	lenth_remain = 0;		// ʣ����ɢ�ֽڳ���
	unsigned short 	frame_num = 0;			// ʣ��֡��Ŀ
	unsigned char 	len = 0;				// ��ǰ֡�ֽڳ���
	unsigned int  	flash_data = 0;
	long			size = 0;				// �������ݴ�С
	
	switch(data_type)	
	{
		case PROGRAM:

			size = data_size * 1024;		
			flash_addr = sector_addr[FSTART_PROG];		// ����ε�FLASH��ʼ��ַ								
			
			break;
			
		case COEFF:
			
			size = data_size * 1024;			
			flash_addr = sector_addr[FSTART_COEFF];		// ϵ���ε�FLASH��ʼ��ַ		
			
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
	}	// ��֧�жϽ���
		
	frame_num = ceil( ((float)size) / LEN_UPDN_ASM );		// �������֡��
	lenth_remain = size % LEN_UPDN_ASM;			// ��������һ֡��Ϣ����
	
	while(frame_num)
	{
		frame_num--;
		
		if(frame_num == 0)				// ��������һ֡�������Ƿ��ǲ����
		{
			if(lenth_remain == 0)		// �����������100�ֽ�
			{
				len = LEN_UPDN_ASM;
			}
			else						// ����ǲ����
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
			case FIRMV:			// ����̼��汾��
				
				while(i < len)
				{
					data_queue[i] = queue_FirmV[i];
					i++;	
				}
				
				break;
				
			case ADDRCHK:		// �������ַ����	
			
				break; 	
				
			default:			// �̼�/����/ϵ������
				
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
-- �������ݵĴ���ͷ���
-- data_type��		��������
-- len_info��		��Ϣ�γ���
-- frame_num��		ʣ��֡��Ŀ
-- queue_data�� 	��Ϣ����
-- com_addrbase��	���ڻ���ַ
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
		// ��ѯFIFO����־
		status = UART_LSR_Read(com_addrbase);		// *1	�˴����ڵķ��Ͳ�����UART_Send������ͬ
		while((status & 0x80) == 0x80)				//		��ʹ��UART_Send������ԭ���Ƿ���������ʱƵ�����ú���������ʱ�俪��
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
-- ��������ָ���
-------------------------------------------------------------------*/
void command_link(unsigned int com_addrbase)
{	
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_LINK + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// ֡ͷ
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// ��֡����
	pfUART_Txmit(CODE_CMD_LINK,com_addrbase);		// ָ�����
	
	pfUART_Txmit(0x7E,com_addrbase);				
	pfUART_Txmit(0x7E,com_addrbase);				
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_LINK + 0x7E + 0x7E;
	pfUART_Txmit(check_sum,com_addrbase);			// У���
	pfUART_Txmit((check_sum>>8),com_addrbase);		
}
/*-------------------------------------------------------------------
 	���ֺ���(�ȴ���ѯ��)
	com_addrbase��	���ڻ���ַ
	����ֵ��		0	���ֳɹ�	
					-1	���ֳ�ʱ
	����:
		1.��ѯ���ڽ��գ����1s��δ�յ�����������time_cnt��1�������¿�
		  ʼ����
		2.���time_cnt����5s���˳����̲�����-1
		3.������յ�����ָ��֡���˳����̲�����0���������¿�ʼ����		
-------------------------------------------------------------------*/
int	commu_link1(unsigned int com_addrbase)
{
	int	flag_timeout = 0;			// ���ڽ��ճ�ʱ��־
	unsigned short check_sum = 0;	
	int	i =	0; 
	int step = 0;
	unsigned char RxData = 0;
	int time_cnt = 0;
	unsigned char queue_cmd[SIZE_C_BUFF];
	
	while(1)
	{
		flag_timeout = tUART_Recv(&RxData,com_addrbase);	// ����������
		
		if(flag_timeout == 1)
		{	
			if(time_cnt >= TIME_WTCOMMU)	// ��ʱ5s
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
			case 0:		// ��֡ͷ1							
				
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
				
			case 1:		// ��֡ͷ2
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
			
			case 2:		// ��������
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// ������Ϣ 
				
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
							command_status(S_LINK,com_addrbase);	// �������ֳɹ�״ָ̬��							
							return(0);								// �˳�ָ���������	
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
 	���ֺ���(�жϲ�ѯ��)
	com_addrbase��	���ڻ���ַ
	����ֵ��		0	���ֳɹ�	
					-1	δ����
	����:
		1.��ѯ���ڽ��գ���������������������������ָ����������û��
		  �ݾ��˳����̲�����-1
		2.������յ���������֡�˳����̲�����0�������˳�������-1	
-------------------------------------------------------------------*/
int	commu_link(unsigned int com_addrbase)
{
	unsigned char status = 0;
//	int	flag_timeout = 0;			// ���ڽ��ճ�ʱ��־
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
			case 0:		// ��֡ͷ1							
				
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
				
			case 1:		// ��֡ͷ2
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
			
			case 2:		// ��������
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// ������Ϣ 
				
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
							command_status(S_LINK,com_addrbase);	// �������ֳɹ�״ָ̬��							
							return(0);								// �˳�ָ���������	
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
