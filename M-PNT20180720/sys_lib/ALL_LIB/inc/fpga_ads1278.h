/********************************************************************************
fpga_ads1278.h
版本：		2.1.0.
文件说明：	FPGA ADS1278模块头文件
编写者：	ChePengyu
编写时间：	2014.06.10.
更新时间：	2014.09.30.
更新说明：
	2014.09.30.
		1.重新定义了寄存器地址
		2.修改了函数
	2015.05.29.
		1.修改为FIFO模式，增加了对FIFO操作的宏函数
	2015.06.11.
		1.去掉了FIFO模式，增加了数据锁存控制
	2015.06.18.
		对应FPGA的修改：
		1.去掉了FIFO相关的宏定义
		2.去掉了锁存功能相关的宏定义
		3.更新了函数原型
********************************************************************************/
#ifndef __ADS1278_H
#define __ADS1278_H

#define ADS1278_SHIFT		1

/* 相关信息和设置参考	*/
#define ADS1278_LSB			0.00000029802//0.000002384184//

/* 寄存器偏移地址	*/

#define	A_ADS1278_CTR			(0x8 << ADS1278_SHIFT) 	// ADS1278 控制寄存器(W)
#define	A_ADS1278_STATUS		(0x9 << ADS1278_SHIFT) 	// ADS1278 状态寄存器(R)
#define	A_ADS1278_SETR			(0xA << ADS1278_SHIFT) 	// ADS1278 设置寄存器(R/W)
#define	A_ADS1278_CLKDIV		(0xB << ADS1278_SHIFT) 	// ADS1278 主时钟分频寄存器(W)


#define A_Data_BASE	    		(0x10 << ADS1278_SHIFT)	// 数据基地址


/* 宏函数		*/								
#define ADS1278_READ_STATUS(base)				(*((unsigned short *)(base + A_ADS1278_STATUS)))
#define ADS1278_READ_DATA(base,offset)			(*((unsigned short *)(base + A_Data_BASE + (offset << ADS1278_SHIFT))))	
#define ADS1278_READ_SET(base)					(*((unsigned short *)(base + A_ADS1278_SETR)))

#define ADS1278_SYNC(base)						*((unsigned short *)(base + A_ADS1278_CTR)) = (0x1)									  
#define ADS1278_WRITE_SET(base,data)			*((unsigned short *)(base + A_ADS1278_SETR)) = (data)
#define ADS1278_WRITE_CLK(base,data)			*((unsigned short *)(base + A_ADS1278_CLKDIV)) = (data)

	
/* 函数原型	*/
/********************************************
addrbase	:ADS1278基地址
ad_mode		:0x1B
sck_div		:0x4
spi_mode	:SPI_MODE0
spi_ss		:0x2
********************************************/
void ADS1278_Init(unsigned int addrbase, unsigned char ad_mode, unsigned char sck_div, unsigned char spi_mode, unsigned char spi_ss);
/********************************************
addrbase		:ADS1278基地址
ptr_AD_result	:AD数据数组首地址
size			:AD数据数组大小
********************************************/
void ADS1278_Read(unsigned int addrbase, float * ptr_AD_result, const int size);
/********************************************
addrbase		:ADS1278基地址
ptr_AD_result	:AD数据地址
chn				:AD通道号（0~7）
********************************************/
void ADS1278_Read1chn(unsigned int addrbase, float * ptr_AD_result, const int chn);

#endif	/*	__ADS1278_H	*/
