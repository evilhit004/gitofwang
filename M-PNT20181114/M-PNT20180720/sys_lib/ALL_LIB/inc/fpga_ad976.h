/********************************************************************************/
// fpga_AD976.h
// 版本：		2.1.0.
// 文件说明：	FPGA AD976模块头文件
// 编写者：		ChePengyu
// 编写时间：	2014.12.25.	
// 更新时间：	
// 更新说明：
/********************************************************************************/

#ifndef __AD976_H__
#define __AD976_H__

#define AD976_SHIFT			1

/*	相关设置参考	*/
#define AD976_LSB			0.000305175
#define AD976_CONV_CMD		0x1

/* 寄存器偏移地址	*/
#define AD976_CTR			0						// 状态寄存器(R)/控制寄存器(W)			             
#define AD976_Data_CH0	    (0x1 << AD976_SHIFT)	// AD数据寄存器
#define AD976_Data_CH1	    (0x2 << AD976_SHIFT)	
#define AD976_Data_CH2	    (0x3 << AD976_SHIFT)	
	
/* 宏函数		*/
#define AD976_READ_DATA(base,offset)		(*((unsigned short *)(base + AD976_Data_CH0 + (offset << AD976_SHIFT))))	
#define AD976_READ_STATUS(base)				(*((unsigned short *)(base + AD976_CTR)))	
#define AD976_WRITE_CTR(base,data)			*((unsigned short *)(base + AD976_CTR)) = (data)	

/* 函数原型	*/
void AD976_Read(unsigned int AddrBase, unsigned char channel, float *AD_result);

#endif 		/*	__AD976_H__		*/
