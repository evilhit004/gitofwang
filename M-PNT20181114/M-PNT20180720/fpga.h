/*******************************************************************************
// fpga.h
// 版本：		2017.05.03
// 文件说明：	C4 DCU FPGA系统头文件
// 编写者：		车鹏宇
// 编写时间：	2014.09.11.
// 更新时间：	2017.09.23.
// 更新说明：

*******************************************************************************/
#ifndef __FPGA_PERIPH_H
#define __FPGA_PERIPH_H


/* FPGA 基址	*/
#define FPGA_Base   		0x62000000	// C6747 CE3	
#define FPGA_SHIFT   	 	1			 						 

/* FPGA 外设基址	*/

#define UART0_Base   		(FPGA_Base + (0x40 << FPGA_SHIFT))      //
#define UART1_Base   		(FPGA_Base + (0x48 << FPGA_SHIFT))      //
#define UART2_Base   		(FPGA_Base + (0x50 << FPGA_SHIFT))      //
#define UART3_Base   		(FPGA_Base + (0x58 << FPGA_SHIFT))      //
//#define UART4_Base   		(FPGA_Base + (0x60 << FPGA_SHIFT))      //
//#define UART5_Base   		(FPGA_Base + (0x68 << FPGA_SHIFT))      //
//#define UART6_Base   		(FPGA_Base + (0x70 << FPGA_SHIFT))      //
//#define UART7_Base   	 	(FPGA_Base + (0x78 << FPGA_SHIFT))		//

//#define SPI0_Base   		(FPGA_Base + (0x80 << FPGA_SHIFT))
//#define SPI1_Base   		(FPGA_Base + (0x88 << FPGA_SHIFT))
//#define SPI2_Base   		(FPGA_Base + (0x90 << FPGA_SHIFT))
//#define SPI3_Base   		(FPGA_Base + (0x98 << FPGA_SHIFT))

//#define Cymometer0_Base   	(FPGA_Base + (0xB0 << FPGA_SHIFT))      //主门电机U相电流
//#define Cymometer1_Base   	(FPGA_Base + (0xB4 << FPGA_SHIFT))		//主门电机V相电流
//#define Cymometer2_Base   	(FPGA_Base + (0xB8 << FPGA_SHIFT))		//从门电机U相电流
//#define Cymometer3_Base   	(FPGA_Base + (0xBC << FPGA_SHIFT))		//从门电机V相电流
//#define Cymometer4_Base   	(FPGA_Base + (0xC0 << FPGA_SHIFT))
//#define Cymometer5_Base   	(FPGA_Base + (0xC4 << FPGA_SHIFT))
//#define Cymometer6_Base   	(FPGA_Base + (0xC8 << FPGA_SHIFT))
//#define Cymometer7_Base   	(FPGA_Base + (0xCC << FPGA_SHIFT))

//#define OW0_Base   			(FPGA_Base + (0xE8 << FPGA_SHIFT))		
//#define OW1_Base   			(FPGA_Base + (0xF0 << FPGA_SHIFT))      

#define CNT_S0_Base			(FPGA_Base + (0xF8 << FPGA_SHIFT))		//定时中断计数器(专用)
//#define CNT_S1_Base			(FPGA_Base + (0xF9 << FPGA_SHIFT))		
//
//#define ADS1278_Base		(FPGA_Base + (0x100 << FPGA_SHIFT))
                        
#define TIMER0_Base			(FPGA_Base + (0x160 << FPGA_SHIFT))		//PC通讯串口定时器(专用)
//#define TIMER1_Base			(FPGA_Base + (0x164 << FPGA_SHIFT))		//看门狗 
//#define TIMER2_Base			(FPGA_Base + (0x168 << FPGA_SHIFT))
//
//#define AD7734_Base			(FPGA_Base + (0x180 << FPGA_SHIFT))
//
//#define GPIO0_Base			(FPGA_Base + (0x190 << FPGA_SHIFT))		
//#define GPIO1_Base			(FPGA_Base + (0x192 << FPGA_SHIFT))		
//#define GPIO2_Base			(FPGA_Base + (0x194 << FPGA_SHIFT))
//#define GPIO3_Base			(FPGA_Base + (0x196 << FPGA_SHIFT))

#define PECULIAR_Base		(FPGA_Base + (0x198 << FPGA_SHIFT))   	//24组16位个性寄存器

//#define OW2_Base   			(FPGA_Base + (0x1B0 << FPGA_SHIFT))     
//#define OW3_Base   			(FPGA_Base + (0x1B8 << FPGA_SHIFT))     

#define SYSREG_Base			(FPGA_Base + (0x1C0 << FPGA_SHIFT))		//32组FPGA系统寄存器

//#define OSP0_Base			(FPGA_Base + (0x1E0 << FPGA_SHIFT))		//单脉冲发生器(负脉冲)

//#define MOTOR0_Base			(FPGA_Base + (0x1F0 << FPGA_SHIFT))		//步进电机0
//#define MOTOR1_Base			(FPGA_Base + (0x1F8 << FPGA_SHIFT))		//步进电机1
//#define MOTOR2_Base			(FPGA_Base + (0x200 << FPGA_SHIFT))		//步进电机2
//#define MOTOR3_Base			(FPGA_Base + (0x208 << FPGA_SHIFT))		//步进电机3
//#define MOTOR4_Base			(FPGA_Base + (0x210 << FPGA_SHIFT))		//步进电机4
//#define MOTOR5_Base			(FPGA_Base + (0x218 << FPGA_SHIFT))		//步进电机5

//#define AD976_Base			(FPGA_Base + (0x220 << FPGA_SHIFT))		//AD976模块
//
//#define AD7734_Base2		(FPGA_Base + (0x230 << FPGA_SHIFT))		//第2路AD7734

//#define INTR0_Base			(FPGA_Base + (0x240 << FPGA_SHIFT))		//intr0     DOI0
//#define INTR1_Base			(FPGA_Base + (0x248 << FPGA_SHIFT))		//intr1     DOI1
//#define INTR2_Base			(FPGA_Base + (0x250 << FPGA_SHIFT))		//intr2     DOI2
//#define INTR3_Base			(FPGA_Base + (0x258 << FPGA_SHIFT))		//intr3     DOI3
//#define INTR4_Base			(FPGA_Base + (0x260 << FPGA_SHIFT))		//intr4     
//#define INTR5_Base			(FPGA_Base + (0x268 << FPGA_SHIFT))		//intr5    	LCB滤波定时器 
#define INTR6_Base			(FPGA_Base + (0x270 << FPGA_SHIFT))		//intr6     1ms定时中断定时器
//#define INTR7_Base			(FPGA_Base + (0x278 << FPGA_SHIFT))		//intr7     锁信号滤波时钟定时器

//#define PWM0_Base   		(FPGA_Base + (0x280 << FPGA_SHIFT))      //PWM0		蜂鸣器
//#define PWM1_Base   		(FPGA_Base + (0x288 << FPGA_SHIFT))      //PWM1		备用蜂鸣器
//#define PWM2_Base   		(FPGA_Base + (0x290 << FPGA_SHIFT))      //PWM2		主/从门电磁锁控制
//#define PWM3_Base   		(FPGA_Base + (0x298 << FPGA_SHIFT))      //PWM3
//#define PWM4_Base   		(FPGA_Base + (0x2A0 << FPGA_SHIFT))      //PWM4
//#define PWM5_Base   		(FPGA_Base + (0x2A8 << FPGA_SHIFT))      //PWM5

//#define AD7608_Base   		(FPGA_Base + (0x2E0 << FPGA_SHIFT))      //AD7608 底板
//#define AD7606_Base   		(FPGA_Base + (0x2F0 << FPGA_SHIFT))      //AD7606 本板


//#define ARAM_Base   	    (FPGA_Base + (0x380 << FPGA_SHIFT))      //RAM
//#define DRAM_Base   	    (FPGA_Base + (0x380 << FPGA_SHIFT))      //RAM





/* 常量定义	*/	

/* 用户函数原型定义	*/


#endif	/*	__FPGA_PERIPH_H	*/



