/********************************************************************************/
// fpga_AD7894.h
// 版本：		2.1.0.
// 文件说明：	FPGA AD7894模块头文件
// 编写者：		ChePengyu
// 编写时间：	2014.03.25.
// 更新时间：	2014.09.28.	
// 更新说明：
// 2014.03.25:
//	1.采用宏代替函数调用，提高运行速度
//	2.修改了部分宏函数名称，与之前版本不兼容
// 2014.09.28:
//	1.去掉了多路开关控制的宏函数与地址
//	2.去掉了AD7894_Conv函数，增加了AD7894_CTR函数，以增加控制的灵活性
//	3.增加了AD7894_LSB的定义，比以前(0.00015)更精确了
//	4.重新写了宏函数，与之前的版本不兼容
/********************************************************************************/
/*	SETR(7:3):	AD启动信号有效电平宽度，宽度 = ADSetConv·Ticlk 					*/			
/*	SETR(2)：	RSV                                                          	*/		
/*	SETR(1)：	RSV                                                          	*/			
/********************************************************************************/
/* 	CTR(0)：	启动转换命令，写'1'启动转换										*/
/********************************************************************************/
#ifndef __AD7894_REGS_H__
#define __AD7894_REGS_H__

#define	AD7894_SHIFT				1 

#define AD7894_LSB					0.0001525879

/* 寄存器偏移地址	*/
#define	AD7894_CTR					(0x8 << AD7894_SHIFT)	// 控制寄存器
#define	AD7894_STA					(0x9 << AD7894_SHIFT)	// 状态寄存器
#define	AD7894_SETR					(0xA << AD7894_SHIFT)	// 设置寄存器

/* 宏函数		*/
#define AD7894_WRITE_SET(base,data)		*((unsigned short *)((base) + AD7894_SETR)) = (data)
#define AD7894_READ_SET(base)			(*((unsigned short *)((base) + AD7894_SETR)))
#define AD7894_WRITE_CTR(base,data)		*((unsigned short *)((base) + AD7894_CTR)) = (data)
#define AD7894_READ_STATUS(base)		(*((unsigned short *)((base) + AD7894_STA)))

void AD7894_Read(float * , unsigned int);
void AD7894_init(unsigned int addrbase, unsigned char Con_Width, unsigned char sck_div, unsigned char spi_mode);


#endif 
