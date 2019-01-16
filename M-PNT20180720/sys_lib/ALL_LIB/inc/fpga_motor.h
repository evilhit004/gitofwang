/********************************************************************************/
// fpga_motor.h
// 版本：		2.1.0.
// 文件说明：	FPGA motor模块头文件
// 编写者：		ChePengyu
// 编写时间：	2014.12.25.
// 更新时间：	
// 更新说明：
/********************************************************************************/
#ifndef __FPGA_MOTOR_H
#define __FPGA_MOTOR_H

#define MOTOR_SHIFT			1

/* 相关参考设置	*/
#define MOTOR_DIR_POS		1
#define MOTOR_DIR_NAG		0

/* 寄存器偏移地址	*/
#define MOTOR_DIR			(0x1 << MOTOR_SHIFT)		// 马达方向设置寄存器			写
#define MOTOR_FDIVL			(0x2 << MOTOR_SHIFT)  		// 脉冲分频系数低16位寄存器    	写
#define MOTOR_FDIVH			(0x3 << MOTOR_SHIFT)  		// 脉冲分频系数高16位寄存器     写
#define MOTOR_PNUM			(0x4 << MOTOR_SHIFT)  		// 脉冲个数寄存器				写

/* 宏函数		*/
#define MOTOR_WRITE_FDIVL(AddrBase,data)			*((unsigned short *)((AddrBase) + MOTOR_FDIVL)) = (data)	
#define MOTOR_WRITE_FDIVH(AddrBase,data)			*((unsigned short *)((AddrBase) + MOTOR_FDIVH)) = (data)		
#define MOTOR_WRITE_PNUM(AddrBase,data)				*((unsigned short *)((AddrBase) + MOTOR_PNUM)) = (data)			
#define MOTOR_WRITE_DIR(AddrBase,data)				*((unsigned short *)((AddrBase) + MOTOR_DIR)) = (data)		

/* 函数原型声明		*/
void Motor_Ctrl(unsigned int AddrBase, unsigned int Pulse_Freq, unsigned short Pulse_Num, unsigned char Pulse_Dir);


#endif	/*	__FPGA_MOTOR_H	*/
