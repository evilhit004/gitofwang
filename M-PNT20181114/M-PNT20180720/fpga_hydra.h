/********************************************************************************/
// fpga_tp.h
// 版本：		1.0.0.
// 文件说明：	FPGA Hydra模块头文件
// 编写者：		jesse yang
// 编写时间：	2016.03.03
// 更新时间：	
// 更新说明：
// 
/********************************************************************************/
#ifndef __FPGA_HYDRA_H
#define __FPGA_HYDRA_H



/* 相关设置信息	*/
/*      
            Timer_int_en         0  
		    wr_en                1 
		    AD7608_DRDY          2 
		    UART0_int            3 
		    UART1_int            4 
		    UART2_int            5 
		    UART3_int            6 
		    UART4_int            7 
		    UART5_int            8 
		    UART6_int            9 
		    UART7_int            10
		    AddrScan_id(0)       11
		    AddrScan_id(1)       12
		    AddrScan_id(2)       13
		    AddrScan_id(3)       14
		    AddrScan_id(4)       15                           
*/
/* 寄存器偏移地址	*/


/* 宏函数		*/
#define TP_Write(AddrBase,data)				*((unsigned short *)(AddrBase)) = (data)
#define TP_Read(AddrBase)					(*((unsigned short *)(AddrBase)))

#endif	/*	__FPGA_TP_H	*/
