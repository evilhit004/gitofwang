/********************************************************************************/
// fpga_PECULIAR_MPNT.h
// 版本：		1.0.0.
// 文件说明：	MPNT FPGA 专用寄存器头文件
// 编写者：		
// 编写时间：	2018.06.11.
// 更新时间：	
// 更新说明：

/********************************************************************************/

#ifndef __PECULIAR_MPNT_H__
#define __PECULIAR_MPNT_H__

#define PECULIAR_MPNT_SHIFT		1
/* 相关信息和设置参考	*/


/* 寄存器偏移地址	*/
#define A_REG0					(0x0 << PECULIAR_MPNT_SHIFT)		//CSAC_PPS_IN输入选择寄存器
#define A_REG1  				(0x1 << PECULIAR_MPNT_SHIFT)		//原子钟状态寄存器
#define A_REG2					(0x2 << PECULIAR_MPNT_SHIFT)		//授时误差寄存器低16位
#define A_REG3          		(0x3 << PECULIAR_MPNT_SHIFT)     	//授时误差寄存器高16位
#define A_REG4	        		(0x4 << PECULIAR_MPNT_SHIFT)     	//GPS授时输出寄存器低16位
#define A_REG5	        		(0x5 << PECULIAR_MPNT_SHIFT)	    //GPS授时输出寄存器高16位
#define A_REG6	        		(0x6 << PECULIAR_MPNT_SHIFT)		//原子钟授时输出寄存器低16位
#define A_REG7	    			(0x7 << PECULIAR_MPNT_SHIFT)		//原子钟授时输出寄存器高16位
//#define A_REG6	        		(0x8 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG7	    			(0x9 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8	        		(0xa << PECULIAR_MPNT_SHIFT)		//

//#define A_REG8	    	    (0xb << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8	    	    (0xc << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8    	      	(0xc << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8	    	    (0xd << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8	    	    (0xe << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8	    	    (0xf << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8	    	    (0x10 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8	    	    (0x11 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8	    	    (0x12 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8				(0x12 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8				(0x13 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8				(0x14 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8				(0x15 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8				(0x16 << PECULIAR_MPNT_SHIFT)		//
//#define A_REG8				(0x17 << PECULIAR_MPNT_SHIFT)		//



/* 宏函数		*/

	
#define PECULIAR_WRITE(base,reg,data)        			*((unsigned short *)(base + reg)) = (data)	
#define PECULIAR_READ(base,reg)	      	      			(*((unsigned short *)(base + reg)))	


/* 函数原型	*/
/********************************************

********************************************/


#endif 
