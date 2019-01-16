/************************************************************	
	DS18B20驱动程序	
	编写人：	车鹏宇
	编写时间：	2014.12.24.
	模块说明：	函数未使用等待查询方式，是防止DS18B20的OW总线
				出现故障后导致程序进入死循环而无法继续向下执行
	更新时间：	
	更新说明：
************************************************************/
#include "fpga_OneWire_DMA.h"

/*-------------------------------------------------------------------
-- DS18B20启动函数
-- 查询OW总线DMA是否空闲，如果空闲则启动OW总线DMA操作并返回0，否则返回1
-- AddrBase：		DS18B20基地址
-- 返回值：			0-执行启动	1-未执行启动
-------------------------------------------------------------------*/
int DS18B20_Conv(unsigned int AddrBase)
{
	unsigned char status = 0;
	
	status = OW_Status_Read(AddrBase);
	
	if((status & MASK_OW_DMA_DONE) == MASK_OW_DMA_DONE)
	{
		OW_DMA_Start(AddrBase);		// 启动OW
		return(0);
	}
	else
	{
		return(1);
	}		
}
/*-------------------------------------------------------------------
-- DS18B20数据读取函数
-- 查询是否有新数据，如果有则读取并求解并返回0；否则返回1
-- temp_result:		温度结果
-- AddrBase：		DS18B20基地址
-- 返回值：			0-完成数据读取	1-数据无更新，未执行读取
-------------------------------------------------------------------*/
int DS18B20_Read(unsigned int AddrBase, float *temp_result)
{
	unsigned char temp_L = 0;
	unsigned char temp_H = 0;
	unsigned short temp = 0;	
	unsigned char status = 0;
	
	status = OW_Status_Read(AddrBase);
	
	if((status & MASK_OW_DATA_UPD) == MASK_OW_DATA_UPD)	// 如果有数据更新
	{	
		temp_L = OW_DataL_Read(AddrBase);
		temp_H = OW_DataH_Read(AddrBase);
		
		temp = (temp_H << 8) + temp_L;
		
		if((temp_H & 0x80) == 0x80)		// 符号位为1
		{	
			temp = ~temp + 1;
			*temp_result = -0.5*temp/8;
		}
		else
		{
			*temp_result = 0.5*temp/8;
		}
	
		return(0);
	}
	else
	{
		return(1);
	}
}

