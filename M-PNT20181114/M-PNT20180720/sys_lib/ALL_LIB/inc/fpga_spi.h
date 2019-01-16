/********************************************************************************/
// fpga_spi.h
// 版本：		2.1.0.
// 文件说明：	FPGA SPI模块头文件
// 编写者：		ChePengyu
// 编写时间：	2014.03.21.
// 更新时间：	2014.04.08.
// 更新说明：
// 2014.03.25:
//	1.采用宏代替函数调用，提高运行速度
//	2.修改了部分宏函数名称，与之前版本不兼容
//	3.RBR的地址必须分配为SPI模块的基地址,这与之前版本不兼容
// 2014.08.26.
//	1.增加了防止重复包含的语句
/********************************************************************************/
#ifndef __FPGA_SPI_H
#define __FPGA_SPI_H

#define SPI_SHIFT					1

/* 相关信息和设置参考	*/
//*********************************************
// SPI模式(MSB在前)
//*********************************************/
#define SPI_MODE0					0
#define SPI_MODE1					0x4
#define SPI_MODE2					0x2
#define SPI_MODE3					0x6
#define MASK_SPI_DONE				0x40		// SPI传输完成
#define MASK_SPI_IDLE				0x20		// SPI传输空闲

/* 寄存器偏移地址	*/
#define SPI_GLBSET		(0x1 << SPI_SHIFT)		// 写SPI全局设置寄存器		读/写
#define SPI_STATUS		(0x2 << SPI_SHIFT)  	// SPI状态寄存器	    	读
#define SPI_CTR			(0x3 << SPI_SHIFT)  	// SPI控制寄存器        	写
#define SPI_SCKDIV		(0x4 << SPI_SHIFT)  	// SPI串行时钟分频寄存器	写
#define SPI_SCKSETUP	(0x5 << SPI_SHIFT)  	// SPI建立时间寄存器    	写
#define	SPI_RBR			0 						// SPI接收/发送数据寄存器	读/写

/* 宏函数		*/
#define SPI_Trans(AddrBase)						*((unsigned short *)((AddrBase) + SPI_CTR)) = 0x1			// 启动SPI传输
#define SPI_SCK_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + SPI_SCKDIV)) = (data)		// 设置SCK分频系数	
#define SPI_Mode_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + SPI_GLBSET)) = (data)		// 设置SPI工作模式
#define SPI_SS_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + SPI_SCKSETUP)) = (data)	// 设置SS到SCK建立时间(周期数)
#define SPI_THR_Write(data,AddrBase)			*((unsigned short *)((AddrBase) + SPI_RBR)) = (data)
#define SPI_RBR_Read(AddrBase)					(*((unsigned short *)((AddrBase) + SPI_RBR)))
#define SPI_Status_Read(AddrBase)				(*((unsigned short *)((AddrBase) + SPI_STATUS)))

void SPI_Write(unsigned int addrbase, unsigned short data);

#endif	/*	__FPGA_SPI_H	*/
