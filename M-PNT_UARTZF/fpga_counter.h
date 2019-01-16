/********************************************************************************/
// FPGA_Counter.h
// 版本：		2.1.0.
// 文件说明：	FPGA计数器模块头文件
// 编写者：		车鹏宇
// 编写时间：	2014.03.25.
// 更新时间：	2014.10.14.
// 更新说明：
// 2014.10.14:
//	1.Counter_Read函数适用于所有计数器模块；
//	  Counter_Read_Chn函数适用于多路计数器模块
/********************************************************************************/
#ifndef __FPGA_COUNT_H
#define __FPGA_COUNT_H

#define COUNT_SHIFT					1

/* 相关信息和设置参考	*/
#define	COUNT_CHNNUM				14						// 计数器通道数
#define	COUNT_TEST_ENABLE			0x55AA					// 使能测试模式KEY值

/* 寄存器偏移地址	*/
#define A_CNT_TEST					(0xC << COUNT_SHIFT)	// 测试脉冲计数器寄存器
#define A_CNT_FRAME					(0xD << COUNT_SHIFT)	// 帧计数寄存器
#define A_CNT_KEY1					(0xE << COUNT_SHIFT)	// KEY1寄存器(保留)
#define A_CNT_KEY2					(0xF << COUNT_SHIFT)	// KEY2寄存器

/* 宏函数		*/
#define Counter_Read(AddrBase)					(*((unsigned short *)(AddrBase)))
#define Counter_Read_Chn(AddrBase,chn)			*((unsigned short *)((AddrBase) + ((chn) << COUNT_SHIFT)))
#define Counter_Write_Key2(AddrBase,data)		*((unsigned short *)((AddrBase) + A_CNT_KEY2)) = (data)
#define Counter_Read_Key2(AddrBase)				(*((unsigned short *)((AddrBase) + A_CNT_KEY2)))

#endif	/*	__FPGA_COUNT_H	*/
