/********************************************************************************/
// fpga_gpio.h
// 版本：		2.1.0.
// 文件说明：	FPGA GPIO模块头文件
// 编写者：		车鹏宇
// 编写时间：	2014.10.14.
// 更新时间：	
// 更新说明：
// 
/********************************************************************************/
#ifndef __FPGA_GPIO_H
#define __FPGA_GPIO_H

#define GPIO_SHIFT					1

/* 相关设置信息	*/
#define GPIO_DIR_IN					0
#define GPIO_DIR_OUT				1


/* 寄存器偏移地址	*/
#define A_GPIO_DATA					0						// 输入输出数据寄存器		读/写
#define A_GPIO_DIR					(0x1 << GPIO_SHIFT)  	// 方向寄存器		    	读/写

/* 宏函数		*/
#define GPIO_Write(AddrBase,data)				*((unsigned short *)(AddrBase)) = (data)
#define GPIO_Read(AddrBase)						(*((unsigned short *)(AddrBase)))
#define GPIO_Dir(AddrBase,data)					*((unsigned short *)((AddrBase) + A_GPIO_DIR)) = (data)


#endif	/*	__FPGA_GPIO_H	*/
