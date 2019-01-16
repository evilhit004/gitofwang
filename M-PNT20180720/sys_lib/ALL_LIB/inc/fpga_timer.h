/********************************************************************************/
// fpga_timer.h
// 版本：		2.1.0.
// 文件说明：	FPGA定时器模块头文件
// 编写者：		ChePengyu
// 编写时间：	2014.08.08.
// 更新时间：	2015.01.29.	
// 更新说明：
// 	2015.01.29.
//		1.增加了intr模块相关代码
/********************************************************************************/
#ifndef __FPGA_TIMER_H
#define __FPGA_TIMER_H

#define TIMER_SHIFT					1

/* 相关信息和设置参考	*/

/* 寄存器偏移地址	*/                                                        	
#define	TIMER_CNT					0x0  					// 定时值寄存器
#define	TIMER_RDR					(0x2 << TIMER_SHIFT)  	// 计时时钟分频寄存器

/* 宏函数		*/
#define TIMER_Read(AddrBase)				(*((unsigned short *)(AddrBase)))
#define TIMER_Write(data,AddrBase)			*((unsigned short *)((AddrBase) + TIMER_CNT)) = (data)
#define TIMER_CLKSet(data,AddrBase)			*((unsigned short *)((AddrBase) + TIMER_RDR)) = (data)

// intr模块相关

#define TIMER_EN_CMD				0x55
#define TIMER_DIS_CMD				0x0

/* 寄存器偏移地址	*/                                                        	
#define	TIMER_FDVL					0x0  					// 分频系数低16位寄存器
#define	TIMER_FDVH					(0x1 << TIMER_SHIFT)  	// 分频系数高16位寄存器
#define	TIMER_CTR					(0x2 << TIMER_SHIFT)  	// 定时器使能寄存器
#define	TIMER_WDR					(0x3 << TIMER_SHIFT)  	// 看门狗使能寄存器

/* 宏函数		*/
#define TIMER_WriteL(AddrBase,data)			*((unsigned short *)((AddrBase) + TIMER_FDVL)) = (data)
#define TIMER_WriteH(AddrBase,data)			*((unsigned short *)((AddrBase) + TIMER_FDVH)) = (data)
#define TIMER_EN(AddrBase,data)				*((unsigned short *)((AddrBase) + TIMER_CTR)) = (data)
#define TIMER_DOG(AddrBase,data)			*((unsigned short *)((AddrBase) + TIMER_WDR)) = (data)

#endif	/*	__FPGA_TIMER_H	*/
