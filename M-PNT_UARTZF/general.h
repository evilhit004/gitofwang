/********************************************************************************/
// general.h
// 版本：		2.1.0.
// 文件说明：	硬件系统通用头文件
// 编写者：		车鹏宇
// 编写时间：	2014.09.11.
// 更新时间：	2014.09.24.
// 更新说明：
// 	2014.09.24.
//		1.增加了读写DSP、FPGA地址的宏函数，方便不同数据接口的程序移植
/********************************************************************************/
#ifndef _GENERAL_H_
#define _GENERAL_H_

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Variable types                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */

/*

#define Uint32  unsigned int
#define Uint16  unsigned short
#define Uint8   unsigned char
#define Int32   int
#define Int16   short
#define Int8    char

*/

/* ------------------------------------------------------------------------ *
 *  系统用宏函数定义                                                        *
 *		使用以下函数会降低程序移植性和可读性，但可以提高运行效率
 *		建议使用在对时间速度有要求的地方
 * ------------------------------------------------------------------------ */
#define __WriteAddr32(data, AddrBase)		*((unsigned int *)(AddrBase)) = (data)		// 向32位绝对地址写入数据
#define __ReadAddr32(AddrBase)				(*((unsigned int *)(AddrBase)))				// 从32位绝对地址读出数据
#define __WriteAddr16(data, AddrBase)		*((unsigned short *)(AddrBase)) = (data)	// 向16位绝对地址写入数据
#define __ReadAddr16(AddrBase)				(*((unsigned short *)(AddrBase)))			// 从16位绝对地址读出数据

#define __ReadFPGA(AddrBase)				(*((unsigned short *)(AddrBase)))			// 从FPGA地址读出数据
#define __WriteFPGA(data, AddrBase)			*((unsigned short *)(AddrBase)) = (data)	// 向FPGA地址写入数据
#define __ReadDSP(AddrBase)					(*((unsigned int *)(AddrBase)))			   // 从DSP地址读出数据
#define __WriteDSP(data, AddrBase)			*((unsigned int *)(AddrBase)) = (data)	   // 向DSP地址写入数据
/* ------------------------------------------------------------------------ *
 *  系统软件延时函数                                                        *
 * ------------------------------------------------------------------------ */
void _wait(unsigned int );

#endif
