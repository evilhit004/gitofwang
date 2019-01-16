/********************************************************************************/
// fpga_ads1278.c
// 版本：		2.1.0.
// 文件说明：	FPGA ADS1278模块C文件
// 编写者：		ChePengyu
// 编写时间：	2014.06.10.
// 更新时间：	2015.06.18.
// 更新说明：
//	2015.06.18.
//		1.修改了ADS1278_Read函数，由传值调用改为传引用调用
//		2.增加了读取单通道数据的函数ADS1278_Read1chn
/********************************************************************************/
#include "fpga_ads1278.h"
#include "fpga_spi.h"

void ADS1278_Init(unsigned int addrbase, unsigned char ad_mode, unsigned char sck_div, unsigned char spi_mode, unsigned char spi_ss)
{
	ADS1278_WRITE_SET(addrbase, ad_mode);	// 设置ADS1278工作模式	
	SPI_SCK_Set(sck_div, addrbase);			
	SPI_Mode_Set(spi_mode, addrbase); 		// MODE0 = 0,SCK空闲为低电平,上升沿采样数据,MSB在前
	SPI_SS_Set(spi_ss, addrbase);
}
 
void ADS1278_Read(unsigned int addrbase, float * ptr_AD_result, const int size)
{
	int i = 0;	
	unsigned short tempL = 0; 
	unsigned short tempH = 0; 
	unsigned int AD_data = 0;	
	
	for(i=0;i<size;i++)
	{
		tempL = ADS1278_READ_DATA(addrbase,i);
		tempH = ADS1278_READ_DATA(addrbase,i);

		AD_data = ((tempH << 16) + tempL) & 0xFFFFFF;		
		
		if((AD_data & 0x800000) != 0x800000)		// 正电压
		{
			*ptr_AD_result = AD_data * ADS1278_LSB;
		}
		else
		{
			*ptr_AD_result = -1 * ADS1278_LSB * ((~AD_data + 1) & 0xFFFFFF);
		}
		
		ptr_AD_result++;
	}	
}

void ADS1278_Read1chn(unsigned int addrbase, float * ptr_AD_result, const int chn)
{
	unsigned short tempL = 0; 
	unsigned short tempH = 0; 
	unsigned int AD_data = 0;		

	tempL = ADS1278_READ_DATA(addrbase,chn);
	tempH = ADS1278_READ_DATA(addrbase,chn);

	AD_data = ((tempH << 16) + tempL) & 0xFFFFFF;		
	
	if((AD_data & 0x800000) != 0x800000)		// 正电压
	{
		*ptr_AD_result = AD_data * ADS1278_LSB;
	}
	else
	{
		*ptr_AD_result = -1 * ADS1278_LSB * ((~AD_data + 1) & 0xFFFFFF);
	}
}

