/********************************************************************************/
// DSPexPC.c
// °æ±¾£º		2.1.0.
// ÎÄ¼şËµÃ÷£º	DSPÓëPCÍ¨Ñ¶cÎÄ¼ş
// ±àĞ´Õß£º		³µÅôÓî
// ±àĞ´Ê±¼ä£º	2014.08.01.
// ¸üĞÂÊ±¼ä£º	2014.09.10.
// ¸üĞÂËµÃ÷£º
//	2014.08.24.
//		1.½«UART_Txmit¸Ä³ÉUART_Sendº¯Êı
//		2.assemble_dataº¯ÊıÖĞ·¢ËÍÊı¾İÁ÷Ê±²»µ÷ÓÃUART_Sendº¯Êı¶øÊ¹ÓÃºê
//		  º¯ÊıÊµÏÖ£¬ÒÔ½ÚÊ¡Ê±¼ä¿ªÏú	
//	2014.09.10.
//		1.ÒòÎªĞŞ¸ÄÁËfpga_uart.c£¬¶ÔÓ¦µØ½«º¯ÊıpUART_Txmit¸ÄÎªpfUART_Txmit£¬
//		  º¯Êı¹¦ÄÜ²»±ä	
/********************************************************************************/

#include <math.h>
#include "DSPexPC.h"
#include "flash.h"
#include "timeout.h"
#include "fpga_uart.h"
							
unsigned char queue_FirmV[3] = {0x0, 0x1, 0x10};	// ¹Ì¼ş°æ±¾ĞÅÏ¢
unsigned int sector_addr[11] = {flashSA0, flashSA1,	flashSA2, flashSA3,	flashSA4, flashSA5,
								flashSA6, flashSA7,	flashSA8, flashSA9,	flashSA10};	

/*-------------------------------------------------------------------
 	Ö¸Áî½âÎöº¯Êı
	queue_cmd:		Ö¸ÁîĞòÁĞ
	com_addrbase£º	´®¿Ú»ùµØÖ·
	·µ»ØÖµ£º		0-½âÎöÍê±Ï
	Á÷³Ì:
		1.²éÑ¯´®¿Ú½ÓÊÕ
		2.Èç¹û½ÓÊÕµ½2¸öÖ¡Í·£¬Ôò¿ªÆô´®¿Ú½ÓÊÕtimeout»úÖÆ
		3.Èç¹û½ÓÊÕtimeout»òÕßĞ£ÑéºÍ´íÎó£¬ÔòÖØĞÂ¿ªÊ¼½âÎöÁ÷³Ì
		4.Èç¹û½âÎöÕıÈ·£¬ÔòÍË³ö½âÎöÁ÷³Ì£¬Ö¸ÁîÖ¡±£´æÔÚqueue_cmdÖĞ			
-------------------------------------------------------------------*/
int	parse_command(unsigned char	queue_cmd[], unsigned int com_addrbase)
{
	int	flag_timeout = 0;			// ´®¿Ú½ÓÊÕ³¬Ê±±êÖ¾
	int flag_gethead = 0;			// ½ÓÊÕµ½Ö¸ÁîÖ¡Í·±êÖ¾
	unsigned short check_sum = 0;	
	int	i =	0; 
	int step = 0;
	unsigned char RxData = 0;
	
	while(1)
	{
		flag_timeout = tUART_Recv(&RxData,com_addrbase);	// ¶Á´®¿ÚÊı¾İ
		
		if(flag_gethead == 1)
		{
			if(flag_timeout == 1)
			{
				command_status(S_TIMEOUT,com_addrbase);		// ·¢ËÍÖ¸ÁîÖ¡²»ÍêÕû(Í¨Ñ¶³¬Ê±)×´Ì¬Ö¸Áî
				flag_gethead = 0;
				step = 0;				
				continue;
			}
		}
		
		switch(step)
		{
			case 0:		// ÅĞÖ¡Í·1							
				
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
				
			case 1:		// ÅĞÖ¡Í·2
				if(RxData == CMD_HEADER2)
				{
					queue_cmd[1] = RxData;
					flag_gethead  = 1;			// ÖÃ½ÓÊÕµ½Ö¡Í·±êÖ¾
					step = 2;				
				}
				else
				{
					step = 0;
				}
				
				break;
			
			case 2:		// ½âÎö³¤¶È
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// ½âÎöĞÅÏ¢ 
				
				queue_cmd[i + LEN_C_HEADER + LEN_L] = RxData;
				i++;
				
				if(i >= queue_cmd[2])
				{					
					flag_gethead = 0;	// Çå½ÓÊÕµ½Ö¡Í·±êÖ¾
					step = 0;
										
					for(i=0;i<(LEN_C_HEADER + LEN_L + queue_cmd[2] - LEN_C_CHECK);i++)
					{
						check_sum =	check_sum +	queue_cmd[i];
					}
					
					if(check_sum ==	((queue_cmd[i+1]<<8) + queue_cmd[i]))
					{							
						check_sum = 0;
						return(0);		// ÍË³öÖ¸Áî½âÎöÁ÷³Ì							
					}
					else
					{
						check_sum = 0;
						command_status(S_CHKERR,com_addrbase);		// ·¢ËÍĞ£Ñé´íÎó×´Ì¬Ö¸Áî	
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
	Êı¾İ½âÎöº¯Êı
		½âÎö1Ö¡Êı¾İ£¬Èç¹û·¢Éú³¬Ê±»òÕßĞ£Ñé´íÎó½«ÍË³öµ±Ç°µÄ½âÎöÁ÷³Ì£¬
	²¢·µ»Ø½âÎöµÄ×´Ì¬				
	data_queue£º	½âÎöµÃµ½µÄÊı¾İÖ¡
	com_addrbase£º	´®¿Ú»ùµØÖ·
	·µ»ØÖµ£º		0-½âÎöÍê±Ï
					1-S_TIMEOUT ³¬Ê±
					2-S_CHKERR	Ğ£ÑéºÍ´íÎó
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
		if(flag_timeout == 1)		// ·¢Éú´®¿Ú½ÓÊÕ³¬Ê±
		{
			return(S_TIMEOUT);		// ÍË³ö£¬·µ»Ø³¬Ê±
		}
		
		switch(step)
		{
			case 0:		// ÅĞÖ¡Í·1
				
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
				
			case 1:		// ÅĞÖ¡Í·2
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
			
			case 2:		// ½âÎö³¤¶È
				queue_data[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// ½âÎöĞÅÏ¢ 
				
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
						return(0);			// ÍË³ö£¬·µ»ØÊı¾İ½âÎöÍê±Ï						
					}
					else
					{
						check_sum = 0;
						return(S_CHKERR);	// ÍË³ö£¬·µ»ØĞ£ÑéºÍ´íÎó
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
-- Êı¾İÇëÇóÖ¸Áî
-- data_type£º	Êı¾İÀàĞÍ
-- rq_reason£º	ÇëÇóÔ­Òò
-- data_frame£º	Êı¾İÖ¡ºÅ
-- com_addrbase£º´®¿Ú»ùµØÖ·
-- ·µ»ØÖµ£º		ÎŞ
-------------------------------------------------------------------*/
void command_DRq(unsigned char data_type, unsigned char	rq_reason, unsigned	char data_frame, unsigned int com_addrbase)
{
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_DRQ + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// Ö¡Í·
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// µ¥Ö¡³¤¶È
	pfUART_Txmit(CODE_CMD_DRQ,com_addrbase);		// Ö¸Áî´úÂë
	
	pfUART_Txmit(data_type,com_addrbase);			// Êı¾İÀàĞÍ
	pfUART_Txmit(rq_reason,com_addrbase);			// ÇëÇóÔ­Òò
	pfUART_Txmit(data_frame,com_addrbase);		// ÇëÇóÊı¾İÖ¡ºÅ
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_DRQ + data_type + rq_reason + data_frame;
	pfUART_Txmit(check_sum,com_addrbase);			// Ğ£ÑéºÍ
	pfUART_Txmit((check_sum>>8),com_addrbase);		
}
/*-------------------------------------------------------------------
-- ×´Ì¬Ö¸Áî
-- status_type£º	×´Ì¬ÀàĞÍ
-- com_addrbase£º	´®¿Ú»ùµØÖ·
-- ·µ»ØÖµ£º			ÎŞ
-------------------------------------------------------------------*/
void command_status(unsigned char status_type, unsigned	int	com_addrbase)
{
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_STA + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// Ö¡Í·
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// µ¥Ö¡³¤¶È
	pfUART_Txmit(CODE_CMD_STA,com_addrbase);		// Ö¸Áî´úÂë
	pfUART_Txmit(status_type,com_addrbase);		// ×´Ì¬ÀàĞÍ
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_STA + status_type;
	
	pfUART_Txmit(check_sum,com_addrbase);			// Ğ£ÑéºÍ
	pfUART_Txmit((check_sum>>8),com_addrbase);
}
/*-------------------------------------------------------------------
-- ÉÏ´«º¯Êı
-- data_type:		ÉÏ´«µÄÊı¾İÀàĞÍ
-- com_addrbase£º	´®¿Ú»ùµØÖ·
-- ·µ»ØÖµ£º			4 - S_ERAERR	FLASH²Á³ı´íÎó
					5 - S_PRGERR	FLASHÉÕĞ´´íÎó	
					8 - S_UPDONE	ÉÏ´«Íê±Ï
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
	
	switch(data_type)		// ÅĞ¶ÏÉÏ´«Êı¾İÀàĞÍ
	{
		case PROGRAM:		// ³ÌĞò
			
			for(i=FSTART_PROG;i<FSTART_COEFF;i++)			// ²Á³ıFLASH
			{
				status = sector_erase(sector_addr[i]);
				
				if(status == 1)								// Èç¹ûFLASH²Á³ıÊ§°Ü¾ÍÖÕÖ¹ÉÏ´«Á÷³Ì
				{
					command_status(S_ERAERR,com_addrbase);	// ·¢ËÍ²Á³ıÊ§°Ü×´Ì¬Ö¸Áî
					return(S_ERAERR);							
				}
			}
			
			flash_addr = sector_addr[FSTART_PROG];			// ³ÌĞò¶ÎµÄFLASHÆğÊ¼µØÖ·								
			
			break;
			
		case COEFF:			// ÏµÊı
			
			status = sector_erase(sector_addr[FSTART_COEFF]);		
			flash_addr = sector_addr[FSTART_COEFF];			// ÏµÊı¶ÎµÄFLASHÆğÊ¼µØÖ·		
			
			break;
		
		default:
	
			break;
	}	// ·ÖÖ§ÅĞ¶Ï½áÊø		
				
				
	// Èç¹ûFLASH²Á³ı³É¹¦ÔòÌĞøÉÏ´«Á÷³Ì
				
	command_status(S_ERAOK,com_addrbase);		// ·¢ËÍ²Á³ı³É¹¦×´Ì¬Ö¸Áî
										
	while(1)		
	{
		command_DRq(data_type,rq_reason,frame_num,com_addrbase);		// ·¢ËÍÊı¾İÇëÇóÖ¸Áî
		rq_reason = parse_data(queue_data,com_addrbase);		// ½âÎö³ÌĞòÊı¾İ
		
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
						
			if(status == 1)								// Èç¹ûÉÕĞ´Ê§°Ü¾ÍÍ£Ö¹ÉÏ´«Á÷³Ì
			{		
				command_status(S_PRGERR,com_addrbase);	// ·¢ËÍÉÕĞ´Ê§°Ü×´Ì¬Ö¸Áî	
				return(S_PRGERR);
			}
		}
		
		//Èç¹ûÉÕĞ´Õı³£Ôò¼ÌĞøÉÏ´«Á÷³Ì
		
		flash_addr = flash_addr	+ len_program;
		
		frame_num++;
		
		if( (queue_data[4] == 0) && (queue_data[5] == 0) )	// Èç¹ûÊ£ÓàÖ¡ÊıÄ¿Îª0
		{
			command_status(S_UPDONE,com_addrbase);	// ·¢ËÍÉÏ´«½áÊø×´Ì¬Ö¸Áî£¬²¢½áÊøÉÏ´«Á÷³Ì
			return(S_UPDONE);
		}
	}		
}
/*-------------------------------------------------------------------
-- ÏÂÔØº¯Êı
-- data_type:		ÏÂÔØµÄÊı¾İÀàĞÍ
-- data_size:		ÏÂÔØµÄÊı¾İ´óĞ¡(KByte)
-- com_addrbase£º	´®¿Ú»ùµØÖ·
-------------------------------------------------------------------*/						
int Execute_DownLoad(unsigned char	data_type, unsigned char data_size, unsigned int com_addrbase)
{
	int	i =	0;
	unsigned char 	data_queue[SIZE_D_BUFF];	
	unsigned int  	flash_addr;
	unsigned char 	lenth_remain = 0;		// Ê£ÓàÁãÉ¢×Ö½Ú³¤¶È
	unsigned short 	frame_num = 0;			// Ê£ÓàÖ¡ÊıÄ¿
	unsigned char 	len = 0;				// µ±Ç°Ö¡×Ö½Ú³¤¶È
	unsigned int  	flash_data = 0;
	long			size = 0;				// ÏÂÔØÊı¾İ´óĞ¡
	
	switch(data_type)	
	{
		case PROGRAM:

			size = data_size * 1024;		
			flash_addr = sector_addr[FSTART_PROG];		// ³ÌĞò¶ÎµÄFLASHÆğÊ¼µØÖ·								
			
			break;
			
		case COEFF:
			
			size = data_size * 1024;			
			flash_addr = sector_addr[FSTART_COEFF];		// ÏµÊı¶ÎµÄFLASHÆğÊ¼µØÖ·		
			
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
	}	// ·ÖÖ§ÅĞ¶Ï½áÊø
		
	frame_num = ceil( ((float)size) / LEN_UPDN_ASM );		// ³ÌĞò¶Î×ÜÖ¡Êı
	lenth_remain = size % LEN_UPDN_ASM;			// ³ÌĞò¶Î×îºóÒ»Ö¡ĞÅÏ¢³¤¶È
	
	while(frame_num)
	{
		frame_num--;
		
		if(frame_num == 0)				// ×îºóÊÇ×îºóÒ»Ö¡£¬ÔòÅĞÊÇ·ñÊÇ²ĞÓàµÄ
		{
			if(lenth_remain == 0)		// Èç¹ûÊÇÍêÕûµÄ100×Ö½Ú
			{
				len = LEN_UPDN_ASM;
			}
			else						// Èç¹ûÊÇ²ĞÓàµÄ
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
			case FIRMV:			// ÇëÇó¹Ì¼ş°æ±¾ºÅ
				
				while(i < len)
				{
					data_queue[i] = queue_FirmV[i];
					i++;	
				}
				
				break;
				
			case ADDRCHK:		// ÇëÇó¼ì²âµØÖ·Êı¾İ	
			
				break; 	
				
			default:			// ¹Ì¼ş/³ÌĞò/ÏµÊıÏÂÔØ
				
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
-- ÏÂÔØÊı¾İµÄ´ò°üºÍ·¢ËÍ
-- data_type£º		Êı¾İÀàĞÍ
-- len_info£º		ĞÅÏ¢¶Î³¤¶È
-- frame_num£º		Ê£ÓàÖ¡ÊıÄ¿
-- queue_data£º 	ĞÅÏ¢Êı×é
-- com_addrbase£º	´®¿Ú»ùµØÖ·
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
		// ²éÑ¯FIFOÂú±êÖ¾
		status = UART_LSR_Read(com_addrbase);		// *1	´Ë´¦´®¿ÚµÄ·¢ËÍ²Ù×÷ÓëUART_Sendº¯ÊıÏàÍ¬
		while((status & 0x80) == 0x80)				//		²»Ê¹ÓÃUART_Sendº¯ÊıµÄÔ­ÒòÊÇ·¢ËÍÊı¾İÁ÷Ê±Æµ·±µ÷ÓÃº¯Êı»áÔö¼ÓÊ±¼ä¿ªÏú
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
-- ·¢ËÍÎÕÊÖÖ¸Áîº¯Êı
-------------------------------------------------------------------*/
void command_link(unsigned int com_addrbase)
{	
	unsigned char len = 0;
	unsigned short check_sum = 0;
	
	len = LEN_C_INDICATE + LEN_C_INFO_LINK + LEN_C_CHECK;
	
	pfUART_Txmit(CMD_HEADER1,com_addrbase);		// Ö¡Í·
	pfUART_Txmit(CMD_HEADER2,com_addrbase);
	pfUART_Txmit(len,com_addrbase);				// µ¥Ö¡³¤¶È
	pfUART_Txmit(CODE_CMD_LINK,com_addrbase);		// Ö¸Áî´úÂë
	
	pfUART_Txmit(0x7E,com_addrbase);				
	pfUART_Txmit(0x7E,com_addrbase);				
	
	check_sum =	CMD_HEADER1	+ CMD_HEADER2 +	len + CODE_CMD_LINK + 0x7E + 0x7E;
	pfUART_Txmit(check_sum,com_addrbase);			// Ğ£ÑéºÍ
	pfUART_Txmit((check_sum>>8),com_addrbase);		
}
/*-------------------------------------------------------------------
 	ÎÕÊÖº¯Êı(µÈ´ı²éÑ¯°æ)
	com_addrbase£º	´®¿Ú»ùµØÖ·
	·µ»ØÖµ£º		0	ÎÕÊÖ³É¹¦	
					-1	ÎÕÊÖ³¬Ê±
	Á÷³Ì:
		1.²éÑ¯´®¿Ú½ÓÊÕ£¬Èç¹û1sÄÚÎ´ÊÕµ½´®¿ÚÊı¾İÔòtime_cnt¼Ó1£¬²¢ÖØĞÂ¿ª
		  Ê¼Á÷³Ì
		2.Èç¹ûtime_cnt³¬¹ı5s£¬ÍË³öÁ÷³Ì²¢·µ»Ø-1
		3.Èç¹û½ÓÊÕµ½ÎÕÊÖÖ¸ÁîÖ¡£¬ÍË³öÁ÷³Ì²¢·µ»Ø0£»·ñÔòÖØĞÂ¿ªÊ¼Á÷³Ì		
-------------------------------------------------------------------*/
int	commu_link1(unsigned int com_addrbase)
{
	int	flag_timeout = 0;			// ´®¿Ú½ÓÊÕ³¬Ê±±êÖ¾
	unsigned short check_sum = 0;	
	int	i =	0; 
	int step = 0;
	unsigned char RxData = 0;
	int time_cnt = 0;
	unsigned char queue_cmd[SIZE_C_BUFF];
	
	while(1)
	{
		flag_timeout = tUART_Recv(&RxData,com_addrbase);	// ¶Á´®¿ÚÊı¾İ
		
		if(flag_timeout == 1)
		{	
			if(time_cnt >= TIME_WTCOMMU)	// ³¬Ê±5s
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
			case 0:		// ÅĞÖ¡Í·1							
				
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
				
			case 1:		// ÅĞÖ¡Í·2
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
			
			case 2:		// ½âÎö³¤¶È
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// ½âÎöĞÅÏ¢ 
				
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
							command_status(S_LINK,com_addrbase);	// ·¢ËÍÎÕÊÖ³É¹¦×´Ì¬Ö¸Áî							
							return(0);								// ÍË³öÖ¸Áî½âÎöÁ÷³Ì	
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
 	ÎÕÊÖº¯Êı(ÖĞ¶Ï²éÑ¯°æ)
	com_addrbase£º	´®¿Ú»ùµØÖ·
	·µ»ØÖµ£º		0	ÎÕÊÖ³É¹¦	
					-1	Î´ÎÕÊÖ
	Á÷³Ì:
		1.²éÑ¯´®¿Ú½ÓÊÕ£¬Èç¹û´®¿ÚÖĞÓĞÊı¾İÔò¶Á³ö²¢½øĞĞÖ¸Áî½âÎö£»Èç¹ûÃ»Êı
		  ¾İ¾ÍÍË³öÁ÷³Ì²¢·µ»Ø-1
		2.Èç¹û½ÓÊÕµ½ÎÕÊÖÃüÁîÖ¡ÍË³öÁ÷³Ì²¢·µ»Ø0£»·ñÔòÍË³ö²¢·µ»Ø-1	
-------------------------------------------------------------------*/
int	commu_link(unsigned int com_addrbase)
{
	unsigned char status = 0;
//	int	flag_timeout = 0;			// ´®¿Ú½ÓÊÕ³¬Ê±±êÖ¾
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
			case 0:		// ÅĞÖ¡Í·1							
				
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
				
			case 1:		// ÅĞÖ¡Í·2
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
			
			case 2:		// ½âÎö³¤¶È
				queue_cmd[2] = RxData;
				i = 0;
				step = 3;
				
				break;
				
			case 3:		// ½âÎöĞÅÏ¢ 
				
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
							command_status(S_LINK,com_addrbase);	// ·¢ËÍÎÕÊÖ³É¹¦×´Ì¬Ö¸Áî							
							return(0);								// ÍË³öÖ¸Áî½âÎöÁ÷³Ì	
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
