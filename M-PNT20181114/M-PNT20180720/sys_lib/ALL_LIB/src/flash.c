/************************************************************	
	FLASH驱动程序
	FLASH型号：	AM29LV400	
	编写人：	车鹏宇
	编写时间：	2014.07.31.
	更新时间：	2014.10.28.
	更新说明：
		2014.09.09.
			1.对没有形参的函数添加void	
			2.将unsigned short flash_read(unsigned int addr)
			  修改为void flash_read(unsigned int addr, unsigned short *data)
			  去掉了返回值，数据以输出参数获得
		2014.09.11.
			1.增加了变量gpio_v声明的条件判断
		2014.10.24.
			1.修改了烧写C6713时命令字写了2次的错误
		2014.10.28.
			1.增加了data_polling函数，用来检测擦除状态
************************************************************/
#include "flash.h"

unsigned char gp_num[5] = {0,2,4,8,9};

/*-------------------------------------------------------------------
-- FLASH段擦除函数
-- sector：		FLASH段地址
-- 返回值：		0-操作完成		1-操作失败
-------------------------------------------------------------------*/ 
int sector_erase(unsigned int sector)
{ 
	unsigned int status = 0;
	
	#ifdef C6747_FLASH
		unsigned int gpio_v = 0;
	#endif
 
    *ctrladdr1 = 0xaa; 
	*ctrladdr2 = 0x55;
	*ctrladdr1 = 0x80;  
	*ctrladdr1 = 0xaa; 
	*ctrladdr2 = 0x55; 
	
	#ifdef C6747_FLASH
		gpio_v = highEA_conv(sector,gp_num);
		*((unsigned int *)GPIOADDR) = gpio_v;	// 控制GPIO口以控制FLASH高4位地址	
		*((unsigned short *)sector) = 0x30;		/* 块擦除命令	*/
		status = toggle_bit(0xa);				// #1 必须在#2前	擦除完成后才能释放地址线/高位地址线，否则将错误
		*((unsigned int *)GPIOADDR) = 0;		// #2
	#else					
		*((unsigned short *)sector) = 0x30;
		status = toggle_bit(0xa);
	#endif	
			
	return(status);					
}
/*-------------------------------------------------------------------
-- FLASH擦除函数
-- 擦除FLASH全部内容
-- 返回值：		0-操作完成		1-操作失败
-------------------------------------------------------------------*/ 
int chip_erase(void)
{
	int status = 0;
	
	*ctrladdr1 = 0xaa; 
	*ctrladdr2 = 0x55;
	*ctrladdr1 = 0x80; 
	*ctrladdr1 = 0xaa;
	*ctrladdr2 = 0x55;
	*ctrladdr1 = 0x10;

	status = toggle_bit(0xa);
	
	return(status);			
}

/*-------------------------------------------------------------------
-- FLASH烧写函数
-- addr：		FLASH地址
-- data：		16位数据
-- 返回值：		0-操作完成		1-操作失败
-- 注意：		如果#define GPIOADDR则具备了GPIO模拟高位地址的控制
-------------------------------------------------------------------*/ 
int flash_program(unsigned int addr, unsigned short data)	
{	
	int status = 0;
	
	#ifdef C6747_FLASH
		unsigned int gpio_v = 0;
	#endif

	*ctrladdr1 = 0xaa; 
	*ctrladdr2 = 0x55;
	*ctrladdr1 = 0xa0; 
		
	#ifdef C6747_FLASH
		gpio_v = highEA_conv(addr,gp_num);

		*((unsigned int *)GPIOADDR) = gpio_v;		// 高位地址输出
		
		*((unsigned short *)(addr))	= data;	
		status = toggle_bit(0xa);					// #1 必须在#2前	烧写完成后才能释放地址线/高位地址线，否则将错误
		
		*((unsigned int *)GPIOADDR) = 0;			// #2 高位地址关闭
	#else
		*((unsigned short *)(addr))	= data;	
		status = toggle_bit(0xa);					
	#endif	
		
	return(status);
}
/*-------------------------------------------------------------------
-- 使用Data# Polling判断FLASH操作状态函数
-- addr：		FLASH地址
-- 返回值：		0-操作完成		1-操作失败
-------------------------------------------------------------------*/ 
int data_polling(unsigned int addr)
{
	unsigned short temp = 0;
	
	while(1)
	{
		temp = *((unsigned short *)(addr));
		if((temp & 0x80) != 0x80)	// 擦除未完成
		{
			if((temp & 0x20) != 0x20)	// 未超时
			{
				continue;	
			}
			else
			{
				temp = *((unsigned short *)(addr));
				if((temp & 0x80) != 0x80)
				{
					return(1);
				}
				else
				{
					return(0);
				}
			}
		}
		else
		{
			return(0);
		}	
	}		
}
/*-------------------------------------------------------------------
-- 使用toggle bit判断FLASH操作状态函数
-- addr：		FLASH地址
-- 返回值：		0-操作完成		1-操作失败
-------------------------------------------------------------------*/ 
int toggle_bit(unsigned int addr)
{
	unsigned short temp1 = 0;
	unsigned short temp2 = 0;			
		
	
	while(1)
	{		
		temp1 = *((unsigned short *)(FLASH_BaseAddr + (addr << AddrShift))) & 0x40;	
		temp2 = *((unsigned short *)(FLASH_BaseAddr + (addr << AddrShift)));
		
		if((temp2 & 0x40) != temp1)				/* D6 = ~D6	操作未完成*/
		{
			if((temp2 & 0x20) != 0x20)			/* D5 = 0	未超时继续查询	*/
			{
				continue;
			}
			else							/* D5 = 1	*/
			{
				temp1 = *((unsigned short *)(FLASH_BaseAddr + (addr << AddrShift))) & 0x40;	
				temp2 = *((unsigned short *)(FLASH_BaseAddr + (addr << AddrShift)));
				
				if((temp2 & 0x40) != temp1)			/* D6 = ~D6	*/
				{					
					return(1);					
				}
				else
				{
					return(0);
				}
			}
		}
		else				/* D6 == D6	*/
		{
			return(0);
		}
	}		/* end while(1)	*/
		
}
/*-------------------------------------------------------------------
-- FLASH高位地址转DSP GPIO值
-- addr：		FLASH地址
-- pin_num：	数组，内容是与高位地址线对应的GPIO引脚号
--				例如pin_num = {0,2,4,8}表示:
--					GPIO[0]对应A14
--					GPIO[2]对应A15
--					GPIO[4]对应A16
--					GPIO[8]对应A17
-- 注意：		使用GPIO模拟地址时应选择同一BANK的GPIO引脚
-- 返回值:		GPIO寄存器配置值
-------------------------------------------------------------------*/ 
unsigned int highEA_conv(unsigned int addr, unsigned char pin_num[])
{
	int i = 0;
	int n = 0;
	unsigned int highEA = 0;
	unsigned int gpio_pin[5] = {0,0,0,0,0};
	unsigned int gpio_value = 0;
	
	highEA = 0xFF & (addr >> 14);		// 提取高位地址
	
	n = sizeof(pin_num);
	
	for(i=0;i<n;i++)
	{
		gpio_pin[i] = ((highEA >> i) & 0x1) << pin_num[i];
		gpio_value = gpio_value + gpio_pin[i];
	}

	return(gpio_value);		 
}
/*-------------------------------------------------------------------
-- FLASH数据读取函数
-- addr：		FLASH地址
-- 返回值:		无
-------------------------------------------------------------------*/ 
void flash_read(unsigned int addr, unsigned short *data)
{		
	#ifdef C6747_FLASH	
		unsigned int gpio_v = 0;
		
		gpio_v = highEA_conv(addr,gp_num);
		
		*((unsigned int *)GPIOADDR) = gpio_v;		// 高位地址输出		
		*data = *((unsigned short *)(addr));			
		*((unsigned int *)GPIOADDR) = 0;			// 高位地址关闭
	#else
		*data = *((unsigned short *)(addr));	
	#endif		
}

