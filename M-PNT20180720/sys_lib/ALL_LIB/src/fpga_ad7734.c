/********************************************************************************/
// fpga_ad7734.c
// 版本：		2.1.0.
// 文件说明：	FPGA AD7734模块C文件
// 编写者：		ChePengyu
// 编写时间：	2014.09.20.
// 更新时间：	2015.08.03.
// 更新说明：
//	2014.09.29.
//		1.对应vhd和头文件的修改进行修改
//		2.去掉了AD7734_write2reg函数，在fpga_spi.c中实现SPI_Write
//	2014.10.29.
//		1.删除了AD7734_Conv函数
//		2.AD7734_ReadData中增加了AD7734_Latch(AddrBase)语句来锁定缓冲区，防止读取
//		  数据时意外更新数据
//	2015.08.03.
//		1.增加了读取单通道数据的函数AD7734_Read1chn	
/********************************************************************************/
#include "fpga_ad7734.h"
	
//-----------------------------------------------------------------
// 读AD7734 4通道数据
//-----------------------------------------------------------------
void AD7734_ReadData(unsigned int AddrBase, float AD_result[4])
{
	int	i = 0;
	unsigned char	status = 0;
	unsigned char 	DL = 0;
	unsigned char	DM = 0;
	unsigned char 	DH = 0;
	unsigned int	AD_data[4] = {0,0,0,0};
	
	status = AD7734_READ_STATUS(AddrBase);
	while((status & 0x1) != 0x1)					// 查询是否有新的转换数据
	{
		status = AD7734_READ_STATUS(AddrBase);
	}
	
	AD7734_Latch(AddrBase);	
	
	for(i=0;i<4;i++)
	{
		DL = AD7734_READ_DATA(AddrBase,i);
		DM = AD7734_READ_DATA(AddrBase,i);
		DH = AD7734_READ_DATA(AddrBase,i);
		AD_data[i] = (((DH&0xFF) << 16) + ((DM&0xFF) << 8) + (DL&0xFF)) & 0xFFFFFF;
		
		if((AD_data[i] & 0x800000) == 0x800000)		// 0 & 正电压
		{
			AD_result[i] = (AD_data[i] - 0x800000) * AD7734_LSB;
		}
		else
		{
			AD_result[i] = -1 * AD7734_LSB * (0x800000 - AD_data[i]);
		}
	}	
}

void AD7734_Read1chn(unsigned int addrbase, float * ptr_AD_result, const int chn)
{	
	unsigned char 	DL = 0;
	unsigned char	DM = 0;
	unsigned char 	DH = 0;
	unsigned int	AD_data = 0;
	
	DL = AD7734_READ_DATA(AddrBase,chn);
	DM = AD7734_READ_DATA(AddrBase,chn);
	DH = AD7734_READ_DATA(AddrBase,chn);
	
	AD_data = (((DH&0xFF) << 16) + ((DM&0xFF) << 8) + (DL&0xFF)) & 0xFFFFFF;
		
	if((AD_data & 0x800000) == 0x800000)		// 0 & 正电压
	{
		*result = (AD_data - 0x800000) * AD7734_LSB;
	}
	else
	{
		*result = -1 * AD7734_LSB * (0x800000 - AD_data);
	}	
}
 



