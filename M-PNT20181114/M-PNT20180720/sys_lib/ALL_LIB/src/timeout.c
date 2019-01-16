/********************************************************************************/
// timeout.c
// 版本：		2.1.0.
// 文件说明：	超时判断机制文件
// 编写者：		车鹏宇
// 编写时间：	2014.09.11.
// 更新时间：	
// 更新说明：
/********************************************************************************/
#include "timeout.h"
#include "fpga_timer.h"
#include "fpga.h"
#include "fpga_uart.h"

/*-------------------------------------------------------------------
-- 带timeout的串口查询接收函数
-- RxDataPtr:		串口收到的数据地址指针
-- timer_addrbase:	定时器基地址
-- com_addrbase：	串口基地址
-- 返回值：			timeout标志，0表示正常；1表示超时
-------------------------------------------------------------------*/
int	tUART_Recv(unsigned char *RxDataPtr, unsigned int com_addrbase)
{
	unsigned short time	= 0xFFFF;
	unsigned char status = 0;
	
	TIMER_Write(TIMEOUT_UART,TIMER0_Base);

	status = UART_LSR_Read(com_addrbase);

	while((status &	0x1) !=	0x1)
	{				
		time = TIMER_Read(TIMER0_Base);
		
		if(time	== 0)
		{
			return(1);
		}

		status = UART_LSR_Read(com_addrbase);
	}
	
	*RxDataPtr = UART_Recv(com_addrbase);
	return(0);		
}






