/********************************************************************************/
// fpga_oneshot.h
// 版本：		1.0.0.
// 文件说明：	FPGA单脉冲发生器模块头文件
// 编写者：		车鹏宇
// 编写时间：	2014.11.14.
// 更新时间：	
// 更新说明：

/********************************************************************************/
#ifndef __FPGA_OSP_H
#define __FPGA_OSP_H

#define OSP_SHIFT					1

/* 相关信息和设置参考	*/

/* 寄存器偏移地址	*/                                                        	
#define OSP_WIDTH					0  						// 单脉冲宽度
#define	OSP_CTR						(0x1 << OSP_SHIFT)  	// 控制寄存器

/* 宏函数		*/
#define OSP_Trigger(AddrBase)					*((unsigned short *)((AddrBase) + OSP_CTR)) = (0x1)
#define OSP_Write_Width(AddrBase,data)			*((unsigned short *)(AddrBase)) = (data)

#endif	/*	__FPGA_OSP_H	*/
