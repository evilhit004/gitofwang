/********************************************************************************/
// fpga_AD7734.h
// 版本：		2.1.0.
// 文件说明：	FPGA AD7734模块头文件
// 编写者：		ChePengyu
// 编写时间：	2014.09.20.	
// 更新时间：	2014.10.29.
// 更新说明：
//	2014.09.29.
//		1.删除地址0~5，此为SPI_Core的地址
//		2.删除AD7734专有的SPI操作函数，改为调用fpga_spi.h实现
//		3.AD7734_Conv和AD7734_ReadData函数都增加了unsigned int addrbase形参
//	2014.10.29.
//		1.删除了AD7734_Conv函数原型
//		2.增加了AD7734_Latch宏函数和A_AD_LATR、AD7734_RST_CMD的宏定义
//	2015.08.03.
//		1.增加了读取单通道数据的函数AD7734_Read1chn的函数声明
/********************************************************************************/

#ifndef __AD7734_H__
#define __AD7734_H__

#define AD7734_RST_CMD		0x4

#define AD7734_SHIFT		1

#define AD7734_LSB			0.0000011921

#define A_AD_CTR			(8 << AD7734_SHIFT)	
#define A_AD_Status			(9 << AD7734_SHIFT)	
#define A_AD_SETR			(0xa << AD7734_SHIFT)
#define A_AD_LATR			(0xb << AD7734_SHIFT)
			             
#define A_Data_CH0	    	(0xc << AD7734_SHIFT)	
#define A_Data_CH1	    	(0xd << AD7734_SHIFT)	
#define A_Data_CH2	    	(0xe << AD7734_SHIFT)	
#define A_Data_CH3	    	(0xf << AD7734_SHIFT)	

#define AD7734_READ_DATA(base,offset)		(*((unsigned short *)(base + A_Data_CH0 + (offset << AD7734_SHIFT))))	
#define AD7734_READ_STATUS(base)			(*((unsigned short *)(base + A_AD_Status)))

#define AD7734_WRITE_SET(base,data)			*((unsigned short *)(base + A_AD_SETR)) = (data)	
#define AD7734_WRITE_CTR(base,data)			*((unsigned short *)(base + A_AD_CTR)) = (data)	
#define AD7734_Latch(base)					*((unsigned short *)(base + A_AD_LATR)) = (0x55)

/* 函数原型	*/

/********************************************
addrbase		:AD7734基地址
float			:4路AD数据
********************************************/
void AD7734_ReadData(unsigned int AddrBase, float AD_result[4]);
/********************************************
addrbase		:AD7734基地址
ptr_AD_result	:AD数据地址
chn				:AD通道号（0~3）
********************************************/
void AD7734_Read1chn(unsigned int addrbase, float * ptr_AD_result, const int chn);

#endif 
