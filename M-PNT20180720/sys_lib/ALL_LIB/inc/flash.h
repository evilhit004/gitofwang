/************************************************************	
	FLASH型号：	AM29LV400	
	编写人：	车鹏宇
	编写时间：	2014.07.31.
	更新时间：	2015.05.29.
	更新说明：	
		2014.08.26.
			1.增加了防止重复包含的语句
			2.去掉了#include <stdio.h>语句
			3.增加了变量sector_addr和gp_num的声明
		2014.10.28.
			1.将地址修改为8bit模式，否则会出现在C6713下无法擦除
			  FLASH部分sector的问题
			2.增加了data_polling函数原型
		2015.05.29.
			1.修改为由预编译方式选择是OMAPL13x还是C6713
				使能宏定义C6747_FLASH：	C6747/OMAP
				屏蔽宏定义C6747_FLASH：	C6713
************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

extern unsigned int sector_addr[11];	// FLASH各sector首地址
extern unsigned char gp_num[5];	

/* AM29LV400BB，16bit端口模式	*/
#define FLASH_SIZE		0x40000							/* FLASH容量	256K x 16bit	*/

//#define C6747_FLASH		// 如果是调试C6747或者OMAP请开启此定义，C6713请屏蔽此定义


#ifdef C6747_FLASH
	//-------------------------------------------------
	// OMAPL137请打开以下代码
	//-------------------------------------------------
	#define GPIOADDR		0x01E2603C
	#define FLASH_BaseAddr	0x60000000						// omapl137 FLASH基地址
	#define flashSA0		(FLASH_BaseAddr + 0x00000)
	#define flashSA1		(FLASH_BaseAddr + 0x02000)
	#define flashSA2		(FLASH_BaseAddr + 0x03000)
	#define flashSA3		(FLASH_BaseAddr + 0x04000)
	#define flashSA4		(FLASH_BaseAddr + 0x08000)
	#define flashSA5		(FLASH_BaseAddr + 0x10000)
	#define flashSA6		(FLASH_BaseAddr + 0x18000)
	#define flashSA7		(FLASH_BaseAddr + 0x20000)
	#define flashSA8		(FLASH_BaseAddr + 0x28000)
	#define flashSA9		(FLASH_BaseAddr + 0x30000)
	#define flashSA10		(FLASH_BaseAddr + 0x38000)
	
	// word 模式对应的命令字地址	
	
	#define FLASH_OP1		0xAAA				// FLASH word mode contrl address			
	#define FLASH_OP2		0x555
	
	#define AddrShift		0								
	
	#define ctrladdr1		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP1<<AddrShift)))
	#define ctrladdr2		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP2<<AddrShift)))
#else
//-------------------------------------------------
// C6713请打开以下代码
//-------------------------------------------------
	#define FLASH_BaseAddr	0x90000000					// C6713 FLASH基地址
	#define flashSA0		(FLASH_BaseAddr + 0x00000)
	#define flashSA1		(FLASH_BaseAddr + 0x04000)
	#define flashSA2		(FLASH_BaseAddr + 0x06000)
	#define flashSA3		(FLASH_BaseAddr + 0x08000)
	#define flashSA4		(FLASH_BaseAddr + 0x10000)
	#define flashSA5		(FLASH_BaseAddr + 0x20000)
	#define flashSA6		(FLASH_BaseAddr + 0x30000)
	#define flashSA7		(FLASH_BaseAddr + 0x40000)
	#define flashSA8		(FLASH_BaseAddr + 0x50000)
	#define flashSA9		(FLASH_BaseAddr + 0x60000)
	#define flashSA10		(FLASH_BaseAddr + 0x70000)
	
	#define FLASH_OP1		0x555				// FLASH word mode contrl address			
	#define FLASH_OP2		0x2AA
	
	#define AddrShift		0	
	
	#define ctrladdr1		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP1<<1)))
	#define ctrladdr2		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP2<<1)))
#endif	
//-------------------------------------------------
// 函数原型
//-------------------------------------------------
int flash_program(unsigned int, unsigned short);
int sector_erase(unsigned int);
int chip_erase(void);
int data_polling(unsigned int);
int toggle_bit(unsigned int );
unsigned int highEA_conv(unsigned int, unsigned char []);
void flash_read(unsigned int addr, unsigned short *);		

#endif	/*	__FLASH_H	*/
