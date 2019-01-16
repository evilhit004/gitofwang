/********************************************************************************/
// FPGA_UART.h
// 版本：		2.1.0.
// 文件说明：	FPGA串口模块头文件
// 编写者：		车鹏宇
// 编写时间：	2014.03.21.
// 更新时间：	2014.12.26.
// 更新说明：
//	2014.08.26.
//		1.增加了防止重复包含的语句
//		2.增加了UART_FIFOCLR，pUART_Txmit和pUART_Recv三个函数的声明
//	2014.09.10.                                                           
//		1.将pUART_Txmit函数改为查询方式串口单字节发送函数                 
//		2.查询方式串口单字节发送函数(FIFO模式)改名为pfUART_Txmit          
//		3.增加了查询方式串口发送函数(FIFO模式)pfUART_Trans，可以发送数据流
//	2014.09.11.
//		1.增加了串口初始化函数UART_init
//	2014.12.26.
//		1.增加了函数原型的注释
/********************************************************************************/
#ifndef __FPGA_UART_H
#define __FPGA_UART_H

#include "data_type.h"

#define UART_SHIFT					1

/* 相关信息和设置参考	*/
#define UART_FIFO_SIZE				256						// 串口FIFO大小                                                       	
#define MASK_UART_TXDONE			0x40					// 发送寄存器空
#define MASK_UART_RXRDY				0x1						// 接收寄存器有数据
#define	UART_LCR_CMD1				0x3						// 8位数据位，1位停止位，无校验位
#define	UART_IER_CMD1				0x1						// 开接收中断

/* 寄存器偏移地址	*/                                                        	
#define	UART_RBR					0x0  					// 接收缓冲寄存器
#define	UART_THR					0x0  					// 发送缓冲寄存器
#define	UART_IER					(0x1 << UART_SHIFT) 	// 中断使能寄存器
#define	UART_IIR					(0x2 << UART_SHIFT) 	// 中断状态寄存器
#define	UART_FCR					(0x2 << UART_SHIFT) 	// FIFO控制寄存器
#define	UART_LCR					(0x3 << UART_SHIFT) 	// 线路控制寄存器
#define	UART_FRBNR					(0x4 << UART_SHIFT) 	// 接收FIFO剩余字节数寄存器
#define	UART_MCR					(0x4 << UART_SHIFT) 	// Modem控制寄存器
#define	UART_LSR					(0x5 << UART_SHIFT) 	// 线路状态寄存器
#define	UART_BDR					(0x5 << UART_SHIFT) 	// 波特率16倍时钟分频系数寄存器
#define	UART_MSR					(0x6 << UART_SHIFT) 	// Modem状态寄存器
#define	UART_FTR					(0x6 << UART_SHIFT) 	// 接收FIFO中断触发字节数寄存器

/* 宏函数		*/
#define UART_Txmit(data,AddrBase)				*((unsigned short *)(AddrBase)) = (data)
#define UART_Recv(AddrBase)						(*((unsigned short *)(AddrBase)))
#define UART_IER_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + UART_IER)) = (data)	
#define UART_IIR_Read(AddrBase)					(*((unsigned short *)((AddrBase) + UART_IIR)))
#define UART_FIFO_Enable(AddrBase)				*((unsigned short *)((AddrBase) + UART_FCR)) = 0x1
#define UART_FIFO_Disable(AddrBase)				*((unsigned short *)((AddrBase) + UART_FCR)) = 0x0
#define UART_FIFO_Level_Set(data,AddrBase)		*((unsigned short *)((AddrBase) + UART_FTR)) = (data)
#define UART_FIFO_Remain_Read(AddrBase)			(*((unsigned short *)((AddrBase) + UART_FRBNR)))
#define UART_LCR_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + UART_LCR)) = (data)
#define UART_LSR_Read(AddrBase)					(*((unsigned short *)((AddrBase) + UART_LSR)))
#define UART_Baud_Set(data,AddrBase)			*((unsigned short *)((AddrBase) + UART_BDR)) = (data)

/*	函数原型	*/

/*-------------------------------------------------------------------
-- 串口初始化函数
-- com_addrbase：	串口基地址
-- v_baud:			16x波特率时钟分频系数(f16xclk = fmclk/v_baud)
-- v_LCR：			设置数据位、停止位、校验位
-- v_IER：			设置串口中断使能
-- v_fifo：			接收FIFO触发中断水平(0-关闭FIFO，其他-触发中断字节数)
-- 返回值：			无
-------------------------------------------------------------------*/
void UART_init(uint32_t com_addrbase, uint8_t v_baud, uint8_t v_LCR, uint8_t v_IER, uint8_t v_fifo);

/*-------------------------------------------------------------------
-- 串口接收FIFO读空函数
-- 串口FIFO清零
-- com_addrbase：	串口基地址
-- 返回值：			FIFO数据
-------------------------------------------------------------------*/
uint8_t UART_FIFOCLR(uint32_t com_addrbase);

/*-------------------------------------------------------------------
-- 查询方式串口单字节发送函数
-- 查询串口THR寄存器状态，为空就写入数据，不空则继续查询
-- TxData:			发送数据
-- com_addrbase：	串口基地址
-- 返回值：			0表示正常
-------------------------------------------------------------------*/
int pUART_Txmit(uint8_t TxData, uint32_t com_addrbase);

/*-------------------------------------------------------------------
-- 查询方式串口发送函数(FIFO模式)
-- 查询串口发送FIFO的满标志，不满就写入数据，满则继续查询
-- ptrTx_Buff:		传输数组的地址指针
-- num：			传输数据的个数
-- com_addrbase：	串口基地址
-- 返回值：			0表示正常
-------------------------------------------------------------------*/
int pfUART_Trans(const uint8_t *ptrTx_Buff, int16_t num, uint32_t com_addrbase);

/*-------------------------------------------------------------------
-- 查询方式串口单字节发送函数(FIFO模式)
-- 查询串口发送FIFO的满标志，不满就写入数据，满则继续查询
-- TxData:			发送数据
-- com_addrbase：	串口基地址
-- 返回值：			0表示正常
-------------------------------------------------------------------*/
int pfUART_Txmit(uint8_t TxData, uint32_t com_addrbase);

/*-------------------------------------------------------------------
-- 串口查询接收函数
-- 查询串口是否有未读的数据，如果有则读出并退出函数；否则继续查询标志位
-- RxDataPtr:		串口收到的数据地址指针
-- com_addrbase：	串口基地址
-- 返回值：			无
-------------------------------------------------------------------*/
void pUART_Recv(uint8_t *RxDataPtr, uint32_t com_addrbase);


#endif	/*	__FPGA_UART_H	*/
