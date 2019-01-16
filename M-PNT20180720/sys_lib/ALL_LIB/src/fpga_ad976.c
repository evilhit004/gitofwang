/********************************************************************************/
// fpga_ad976.c
// 版本：		2.1.0.
// 文件说明：	FPGA AD976模块c文件
// 编写者：		车鹏宇
// 编写时间：	2014.12.25.
// 更新时间：	
// 更新说明：
/********************************************************************************/
#include "fpga_ad976.h"

/*-------------------------------------------------------------------
-- AD976数据读取函数
-- AddrBase:		串口基地址
-- channel:			AD通道号
-- AD_result：		AD浮点型结果
-- 返回值：			无
-------------------------------------------------------------------*/
void AD976_Read(unsigned int AddrBase, unsigned char channel, float *AD_result)
{
	unsigned char status = 0;			
	unsigned int AD_Data = 0;
	
	status = AD976_READ_STATUS(AddrBase);
	
	while((status & (0x1 << channel)) != (0x1 << channel))
	{
		status = AD976_READ_STATUS(AddrBase);
	}
	
	AD_Data = AD976_READ_DATA(AddrBase,channel) & 0xFFFF;   
	if((AD_Data & 0x8000) == 0x8000)	// 负电压
	{
		*AD_result = -1 * AD976_LSB * ((~AD_Data + 1) & 0xFFFF);
	}
	else	// 正电压
	{
		*AD_result = AD976_LSB * AD_Data;
	}
	
}
