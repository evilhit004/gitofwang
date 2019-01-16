/********************************************************************************/
// fpga_OneWire_DMA.h
// 版本：		2.1.0.
// 文件说明：	1-Wire总线 DMA模块头文件
// 编写者：		ChePengyu
// 编写时间：	2014.03.25.
// 更新时间：	2014.12.24.
// 更新说明：
// 	2014.12.24.
//		1.增加了防止重复包含的语句
//		2.增加了状态字的宏
//		3.增加DS18B20相关的函数原型
/********************************************************************************/
/********************************************************************************/
// status寄存器
// ISR(7):	0				保留
// ISR(6):	0				保留
// ISR(5):	0				保留
// ISR(4):	OverRun			'1'表示发生了overrun错误
// ISR(3):	DMA_Done		'1'表示DMA空闲，'0'表示DMA运行中
// ISR(2):	reset_PDR		'1'表示检测到从设备，'0'表示未检测到从设备；该标志在每次进行复位检测操作后刷新
// ISR(1):	0				保留
// ISR(0):	Data_updata		'1'表示接收数据寄存器有新数据，'0'表示无新数据。该标志在读取接收数据寄存器后清0。 
/********************************************************************************/
#ifndef __FPGA_OW_H
#define __FPGA_OW_H

#define	ONEWIRE_DMA_SHIFT			1 

/* 相关信息和设置参考	*/
#define MASK_OW_DMA_DONE			0x8
#define MASK_OW_DATA_UPD			0x1

/* 寄存器偏移地址	*/ 
#define ONEWIRE_CMD					0								// 命令寄存器				W/R	
#define ONEWIRE_TRB					(0x1 << ONEWIRE_DMA_SHIFT)      // 发送/接收(低8位)寄存器 	W/R	
#define ONEWIRE_ISR					(0x2 << ONEWIRE_DMA_SHIFT)      // 中断状态寄存器     		R   
//#define ONEWIRE_IER				(0x3 << ONEWIRE_DMA_SHIFT)      // 中断使能寄存器 			W/R	(未使用)	
//#define ONEWIRE_DIV				(0x4 << ONEWIRE_DMA_SHIFT)      // 时钟分频系数寄存器 		W/R (未使用)
//#define ONEWIRE_CTR				(0x5 << ONEWIRE_DMA_SHIFT)      // 控制寄存器         		W/R (未使用)
#define ONEWIRE_RBH					(0x6 << ONEWIRE_DMA_SHIFT)      // 接收(高8位)寄存器        R   

/* 宏函数		*/
#define OW_Status_Read(AddrBase)			(*((unsigned short *)((AddrBase) + ONEWIRE_ISR)))
#define OW_DataL_Read(AddrBase)				(*((unsigned short *)((AddrBase) + ONEWIRE_TRB)))
#define OW_DataH_Read(AddrBase)				(*((unsigned short *)((AddrBase) + ONEWIRE_RBH)))
#define OW_DMA_Start(AddrBase)				*((unsigned short *)(AddrBase)) = 0x1

/* 函数原型		*/
int DS18B20_Conv(unsigned int AddrBase);
int DS18B20_Read(unsigned int AddrBase, float *temp_result);

#endif	/*	__FPGA_OW_H	*/
