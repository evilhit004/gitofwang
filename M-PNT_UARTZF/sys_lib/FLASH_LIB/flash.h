/************************************************************	
	FLASH型号：	AM29LV400	
	编写人：	车鹏宇
	编写时间：	2014.07.31.
	更新时间：	2015.09.18.
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
		2015.09.18
			1.OMAP平台下FLASH的物理地址应向左移1位才是对应的DSP逻辑地址，
		  	  FLASH_ADDRSHIFT修改为1
************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

extern unsigned int sector_addr[11];	// FLASH各sector首地址
extern unsigned char gp_num[5];	

/* AM29LV400BB FLASH相关地址，16bit端口模式	*/

#define C6747_FLASH									// 如果是调试C6747或者OMAP请开启此定义，C6713请屏蔽此定义
#define GPIOADDR				0x01E2603C
#define FLASH_HIGADDR_SHIFT		14
#define FLASH_ADDRSHIFT			1					/* FLASH物理地址转换为DSP逻辑地址需要左移位数	*/

#define FLASH_BaseAddr			0x60000000			/* FLASH基地址	*/

// FLASH段物理地址
#define PSY_flashSA0		0x00000
#define PSY_flashSA1		0x02000
#define PSY_flashSA2		0x03000
#define PSY_flashSA3		0x04000
#define PSY_flashSA4		0x08000
#define PSY_flashSA5		0x10000
#define PSY_flashSA6		0x18000
#define PSY_flashSA7		0x20000
#define PSY_flashSA8		0x28000
#define PSY_flashSA9		0x30000
#define PSY_flashSA10		0x38000

// WORD MODE FLASH段对应的DSP逻辑地址
#define flashSA0		(FLASH_BaseAddr + (0x00000 << FLASH_ADDRSHIFT))
#define flashSA1		(FLASH_BaseAddr + (0x02000 << FLASH_ADDRSHIFT))
#define flashSA2		(FLASH_BaseAddr + (0x03000 << FLASH_ADDRSHIFT))
#define flashSA3		(FLASH_BaseAddr + (0x04000 << FLASH_ADDRSHIFT))
#define flashSA4		(FLASH_BaseAddr + (0x08000 << FLASH_ADDRSHIFT))
#define flashSA5		(FLASH_BaseAddr + (0x10000 << FLASH_ADDRSHIFT))
#define flashSA6		(FLASH_BaseAddr + (0x18000 << FLASH_ADDRSHIFT))
#define flashSA7		(FLASH_BaseAddr + (0x20000 << FLASH_ADDRSHIFT))
#define flashSA8		(FLASH_BaseAddr + (0x28000 << FLASH_ADDRSHIFT))
#define flashSA9		(FLASH_BaseAddr + (0x30000 << FLASH_ADDRSHIFT))
#define flashSA10		(FLASH_BaseAddr + (0x38000 << FLASH_ADDRSHIFT))

// BYTE MODE FLASH段对应的DSP逻辑地址
/*  
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
*/

// FLASH命令字地址	
/*
#define FLASH_OP1		0xAAA				// FLASH byte mode contrl address			
#define FLASH_OP2		0x555
*/
#define FLASH_OP1		0x555				// FLASH word mode contrl address			
#define FLASH_OP2		0xAAA

#define ctrladdr1		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP1 << FLASH_ADDRSHIFT)))
#define ctrladdr2		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP2 << FLASH_ADDRSHIFT)))



//-------------------------------------------------
// 函数原型
//-------------------------------------------------
/*-------------------------------------------------------------------
-- FLASH烧写函数
-- addr：		FLASH地址
-- data：		16位数据
-- 返回值：		0-操作完成		1-操作失败
-- 注意：		如果#define C6747_FLASH则具备了GPIO模拟高位地址的控制
-------------------------------------------------------------------*/ 
int flash_program(unsigned int, unsigned short);
/*-------------------------------------------------------------------
-- FLASH段擦除函数
-- sector：		FLASH段地址
-- 返回值：		0-操作完成		1-操作失败
-------------------------------------------------------------------*/ 
int sector_erase(unsigned int);
int chip_erase(void);
/*-------------------------------------------------------------------
-- 使用Data# Polling判断FLASH操作状态函数
-- addr：		FLASH地址
-- 返回值：		0-操作完成		1-操作失败
-------------------------------------------------------------------*/ 
int data_polling(unsigned int);
/*-------------------------------------------------------------------
-- 使用toggle bit判断FLASH操作状态函数
-- addr：		FLASH地址
-- 返回值：		0-操作完成		1-操作失败
-------------------------------------------------------------------*/ 
int toggle_bit(unsigned int );
unsigned int highEA_conv(unsigned int, unsigned char []);
/*-------------------------------------------------------------------
-- FLASH数据读取函数
-- addr：		FLASH地址
-- 返回值:		无
-------------------------------------------------------------------*/ 
void flash_read(unsigned int addr, unsigned short *);		

#endif	/*	__FLASH_H	*/
