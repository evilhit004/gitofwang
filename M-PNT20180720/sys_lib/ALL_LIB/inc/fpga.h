/********************************************************************************/
// fpga.h
// 版本：		3.0.0.
// 文件说明：	FPGA系统头文件
// 编写者：		车鹏宇
// 编写时间：	2014.09.11.
// 更新时间：	2014.12.24.
// 更新说明：
//	2014.12.24.
//		1.增加了OW0_Base、OW1_Base、AD976_Base、AD7734_Base2和6路电机基地址
/********************************************************************************/
#ifndef __FPGA_PERIPH_H
#define __FPGA_PERIPH_H

/* FPGA 基址	*/
#define FPGA_Base   	0xA0000000 	//0x62000000	//
#define FPGA_SHIFT   	 1 						 

/* FPGA 外设基址	*/

#define UART0_Base  	(FPGA_Base + (0x40 << FPGA_SHIFT))
#define UART1_Base   	(FPGA_Base + (0x48 << FPGA_SHIFT))
#define UART2_Base   	(FPGA_Base + (0x50 << FPGA_SHIFT))
#define UART3_Base   	(FPGA_Base + (0x58 << FPGA_SHIFT))
#define UART4_Base   	(FPGA_Base + (0x60 << FPGA_SHIFT))
#define UART5_Base   	(FPGA_Base + (0x68 << FPGA_SHIFT))
#define UART6_Base   	(FPGA_Base + (0x70 << FPGA_SHIFT))
#define GPS_Base   	 	(FPGA_Base + (0x78 << FPGA_SHIFT))		//GPS串口(专用)

#define SPI0_Base   	(FPGA_Base + (0x80 << FPGA_SHIFT))
#define SPI1_Base   	(FPGA_Base + (0x88 << FPGA_SHIFT))
#define SPI2_Base   	(FPGA_Base + (0x90 << FPGA_SHIFT))
#define SPI3_Base   	(FPGA_Base + (0x98 << FPGA_SHIFT))

#define OW0_Base   		(FPGA_Base + (0xE8 << FPGA_SHIFT))		//OneWire总线1
#define OW1_Base   		(FPGA_Base + (0xF0 << FPGA_SHIFT))

#define CNT_S0_Base		(FPGA_Base + (0xF8 << FPGA_SHIFT))		//定时中断计数器(专用)
#define CNT_S1_Base		(FPGA_Base + (0xF9 << FPGA_SHIFT))		

#define ADS1278_Base	(FPGA_Base + (0x100 << FPGA_SHIFT))
                        
#define TIMER0_Base		(FPGA_Base + (0x160 << FPGA_SHIFT))		//PC通讯串口定时器(专用)
#define TIMER1_Base		(FPGA_Base + (0x164 << FPGA_SHIFT))
#define TIMER2_Base		(FPGA_Base + (0x168 << FPGA_SHIFT))

#define AD7734_Base		(FPGA_Base + (0x180 << FPGA_SHIFT))

#define GPIO0_Base		(FPGA_Base + (0x190 << FPGA_SHIFT))
#define GPIO1_Base		(FPGA_Base + (0x192 << FPGA_SHIFT))
#define GPIO2_Base		(FPGA_Base + (0x194 << FPGA_SHIFT))
#define GPIO3_Base		(FPGA_Base + (0x196 << FPGA_SHIFT))

#define PECULIAR_Base	(FPGA_Base + (0x198 << FPGA_SHIFT))   	//24组16位个性寄存器

#define SYSREG_Base		(FPGA_Base + (0x1C0 << FPGA_SHIFT))		//32组FPGA系统寄存器

#define OSP0_Base		(FPGA_Base + (0x1E0 << FPGA_SHIFT))		//单脉冲发生器(负脉冲)

#define MOTOR0_Base		(FPGA_Base + (0x1F0 << FPGA_SHIFT))		//步进电机0
#define MOTOR1_Base		(FPGA_Base + (0x1F8 << FPGA_SHIFT))		//步进电机1
#define MOTOR2_Base		(FPGA_Base + (0x200 << FPGA_SHIFT))		//步进电机2
#define MOTOR3_Base		(FPGA_Base + (0x208 << FPGA_SHIFT))		//步进电机3
#define MOTOR4_Base		(FPGA_Base + (0x210 << FPGA_SHIFT))		//步进电机4
#define MOTOR5_Base		(FPGA_Base + (0x218 << FPGA_SHIFT))		//步进电机5

#define AD976_Base		(FPGA_Base + (0x220 << FPGA_SHIFT))		//AD976模块

#define AD7734_Base2	(FPGA_Base + (0x230 << FPGA_SHIFT))		//第2路AD7734

#define INTR0_Base		(FPGA_Base + (0x240 << FPGA_SHIFT))		//intr0


/* 常量定义	*/	

/* 用户函数原型定义	*/


#endif	/*	__FPGA_PERIPH_H	*/



