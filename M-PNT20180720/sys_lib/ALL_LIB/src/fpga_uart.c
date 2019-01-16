/********************************************************************************/
// FPGA_UART.c
// 版本：		2.1.0.
// 文件说明：	FPGA串口模块c文件
// 编写者：		车鹏宇
// 编写时间：	2014.08.25.
// 更新时间：	2014.09.11.
// 更新说明：
//	2014.09.10.
//		1.将pUART_Txmit函数改为查询方式串口单字节发送函数
//		2.查询方式串口单字节发送函数(FIFO模式)改名为pfUART_Txmit
//		3.增加了查询方式串口发送函数(FIFO模式)pfUART_Trans，可以发送数据流
//	2014.09.11.
//		1.增加了串口初始化函数UART_init
//	2014.12.26.
//		1.修改了部分注释
/********************************************************************************/
#include "fpga_uart.h"

/*-------------------------------------------------------------------
-- 串口初始化函数
-- com_addrbase：	串口基地址
-- v_baud:			16x波特率时钟分频系数(f16xclk = fmclk/v_baud)
-- v_LCR：			设置数据位、停止位、校验位
-- v_IER：			设置串口中断使能
-- v_fifo：			接收FIFO触发中断水平(0-关闭FIFO，其他-触发中断字节数)
-- 返回值：			无
-------------------------------------------------------------------*/
void UART_init(unsigned int com_addrbase, unsigned char v_baud, unsigned char v_LCR, unsigned char v_IER, unsigned char v_fifo)
{
	UART_Baud_Set(v_baud, com_addrbase);			// 设置波特率为 18.432MHz/10 = 1.8432MHz = 115.2Kbps x 16
	UART_LCR_Set(v_LCR, com_addrbase);				// 8位数据位，1位停止位，无校验位
	UART_IER_Set(v_IER, com_addrbase);				// 开线路中断和接收中断
	
	if(v_fifo == 0)
	{
		UART_FIFO_Disable(com_addrbase);			// 关闭FIFO
	}
	else
	{
		UART_FIFO_Enable(com_addrbase);				// 打开FIFO
		UART_FIFO_Level_Set(v_fifo,com_addrbase);	// 设置接收FIFO为1字节触发中断
	}
}
/*-------------------------------------------------------------------
-- 串口接收FIFO读空函数
-- 串口FIFO清零
-- com_addrbase：	串口基地址
-- 返回值：			FIFO数据
-------------------------------------------------------------------*/
unsigned char UART_FIFOCLR(unsigned int com_addrbase)
{
	unsigned char status = 0;
	unsigned char data = 0;
	
	status = UART_LSR_Read(com_addrbase);
	while((status &	0x1) ==	0x1)		// FIFO内有数据
	{
		data = UART_Recv(com_addrbase);
		status = UART_LSR_Read(com_addrbase);
	}
	
	return(data);
}
/*-------------------------------------------------------------------
-- 查询方式串口单字节发送函数
-- 查询串口THR寄存器状态，为空就写入数据，不空则继续查询
-- TxData:			发送数据
-- com_addrbase：	串口基地址
-- 返回值：			0表示正常
-------------------------------------------------------------------*/
int pUART_Txmit(unsigned char TxData, unsigned int com_addrbase)
{
	unsigned char status = 0;
	
	status = UART_LSR_Read(com_addrbase);
	
	while((status & 0x40) != 0x40)	// THR满
	{
		status = UART_LSR_Read(com_addrbase);
	}
	
	UART_Txmit(TxData, com_addrbase);
	
	return(0);
}

/*-------------------------------------------------------------------
-- 查询方式串口发送函数(FIFO模式)
-- 查询串口发送FIFO的满标志，不满就写入数据，满则继续查询
-- ptrTx_Buff:		传输数组的地址指针
-- num：			传输数据的个数
-- com_addrbase：	串口基地址
-- 返回值：			0表示正常
-------------------------------------------------------------------*/
int pfUART_Trans(const unsigned char *ptrTx_Buff, int num, unsigned int com_addrbase)
{
	unsigned char status = 0;
	int i = 0;
	
	for(i=0;i<num;i++)
	{
	
		status = UART_LSR_Read(com_addrbase);
	
		while((status & 0x80) == 0x80)	// FIFO满
		{
			status = UART_LSR_Read(com_addrbase);
		}
	
		UART_Txmit(*(ptrTx_Buff + i), com_addrbase);
	}
	
	return(0);
}

/*-------------------------------------------------------------------
-- 查询方式串口单字节发送函数(FIFO模式)
-- 查询串口发送FIFO的满标志，不满就写入数据，满则继续查询
-- TxData:			发送数据
-- com_addrbase：	串口基地址
-- 返回值：			0表示正常
-------------------------------------------------------------------*/
int pfUART_Txmit(unsigned char TxData, unsigned int com_addrbase)
{
	unsigned char status = 0;
	
	status = UART_LSR_Read(com_addrbase);
	
	while((status & 0x80) == 0x80)	// FIFO满
	{
		status = UART_LSR_Read(com_addrbase);
	}
	
	UART_Txmit(TxData, com_addrbase);
	
	return(0);
}

/*-------------------------------------------------------------------
-- 串口查询接收函数
-- 查询串口是否有未读的数据，如果有则读出并退出函数；否则继续查询标志位
-- RxDataPtr:		串口收到的数据地址指针
-- com_addrbase：	串口基地址
-- 返回值：			无
-------------------------------------------------------------------*/
void pUART_Recv(unsigned char *RxDataPtr, unsigned int com_addrbase)
{
	unsigned char status = 0;

	status = UART_LSR_Read(com_addrbase);

	while((status &	0x1) !=	0x1)			// 如果FIFO空或者RBR内无新数据
	{				
		status = UART_LSR_Read(com_addrbase);
	}
	
	*RxDataPtr = UART_Recv(com_addrbase);		
}







