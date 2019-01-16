/********************************************************************************/
// fpga_ad7608.c
// 版本：		1.0.0
// 文件说明：	FPGA AD760x模块C文件
// 编写者：		jesse yang
// 编写时间：	2016.02.28.

/********************************************************************************/
#include "fpga_ad7608.h"
#include "fpga_spi.h"

void AD7606_Init(unsigned int addrbase, unsigned char ad7606_mode, unsigned char sck_div)
{
	AD760x_WRITE_SET(addrbase, ad7606_mode);	// 设置AD7606工作模式	
	SPI_SCK_Set(sck_div, addrbase);			  //设置SPI时钟
//	SPI_Mode_Set(spi_mode, addrbase); 		// MODE0 = 0,SCK空闲为低电平,上升沿采样数据,MSB在前
//	SPI_SS_Set(spi_ss, addrbase);
}

void AD7608_Init(unsigned int addrbase, unsigned char ad7608_mode, unsigned char sck_div)
{
	AD760x_WRITE_SET(addrbase, ad7608_mode);	// 设置AD7608工作模式	
	SPI_SCK_Set(sck_div, addrbase);			  //设置SPI时钟
//	SPI_Mode_Set(spi_mode, addrbase); 		// MODE0 = 0,SCK空闲为低电平,上升沿采样数据,MSB在前
//	SPI_SS_Set(spi_ss, addrbase);
}

void AD7606_Read(unsigned int addrbase, float * ptr_AD_result, const int size)
{
	int i = 0;	
	unsigned short AD_data = 0;	
	
	for(i=0;i<size;i++)
	{
		AD_data = AD760x_READ_DATA(addrbase,i);
				
		if((AD_data & 0x8000) != 0x8000)		// 正电压
		{
			*ptr_AD_result = AD_data * AD7606_LSB;
		}
		else
		{
			*ptr_AD_result = -1 * AD7606_LSB * ((~AD_data + 1) & 0xFFFF);
		}
		
		ptr_AD_result++;
	}	
} 
void AD7608_Read(unsigned int addrbase, float * ptr_AD_result, const int size)
{
	int i = 0;	
	unsigned short tempH = 0; 
	unsigned short tempEX = 0; 
	unsigned int AD_data = 0;	
	
	tempEX = AD760x_READ_DATA(addrbase,8);
	
	for(i=0;i<size;i++)
	{
		tempH = AD760x_READ_DATA(addrbase,i);
		
		AD_data = ((tempH << 2) + tempEX&0x0003) & 0x3FFFF;		
		
		if((AD_data & 0x8000) != 0x8000)		// 正电压
		{
			*ptr_AD_result = AD_data * AD7608_LSB;
		}
		else
		{
			*ptr_AD_result = -1 * AD7608_LSB * ((~AD_data + 1) & 0x3FFFF);
		}
		
		ptr_AD_result++;
		tempEX = tempEX >>2;
	}	
}

void AD7606_Read1chn(unsigned int addrbase, float * ptr_AD_result, const int chn)
{
	unsigned short AD_data = 0;	
	
	AD_data = AD760x_READ_DATA(addrbase,chn);
			
	if((AD_data & 0x8000) != 0x8000)		// 正电压
	{
		*ptr_AD_result = AD_data * AD7606_LSB;
	}
	else
	{
		*ptr_AD_result = -1 * AD7606_LSB * ((~AD_data + 1) & 0xFFFF);
	}
} 

void AD7608_Read1chn(unsigned int addrbase, float * ptr_AD_result, const int chn)
{
	unsigned short tempH = 0; 
	unsigned short tempEX = 0; 
	unsigned int AD_data = 0;	
	
	tempEX = (AD760x_READ_DATA(addrbase,8))>>(2*chn);
	tempH = AD760x_READ_DATA(addrbase,chn);
	AD_data = ((tempH << 2) + tempEX&0x0003) & 0x3FFFF;		
	
	if((AD_data & 0x8000) != 0x8000)		// 正电压
	{
		*ptr_AD_result = AD_data * AD7608_LSB;
	}
	else
	{
		*ptr_AD_result = -1 * AD7608_LSB * ((~AD_data + 1) & 0x3FFFF);
	}
}