/********************************************************************************/
// fpga_ads1278.c
// �汾��		2.1.0.
// �ļ�˵����	FPGA ADS1278ģ��C�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.06.10.
// ����ʱ�䣺	2015.06.18.
// ����˵����
//	2015.06.18.
//		1.�޸���ADS1278_Read�������ɴ�ֵ���ø�Ϊ�����õ���
//		2.�����˶�ȡ��ͨ�����ݵĺ���ADS1278_Read1chn
/********************************************************************************/
#include "fpga_ads1278.h"
#include "fpga_spi.h"

void ADS1278_Init(unsigned int addrbase, unsigned char ad_mode, unsigned char sck_div, unsigned char spi_mode, unsigned char spi_ss)
{
	ADS1278_WRITE_SET(addrbase, ad_mode);	// ����ADS1278����ģʽ	
	SPI_SCK_Set(sck_div, addrbase);			
	SPI_Mode_Set(spi_mode, addrbase); 		// MODE0 = 0,SCK����Ϊ�͵�ƽ,�����ز�������,MSB��ǰ
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
		
		if((AD_data & 0x800000) != 0x800000)		// ����ѹ
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
	
	if((AD_data & 0x800000) != 0x800000)		// ����ѹ
	{
		*ptr_AD_result = AD_data * ADS1278_LSB;
	}
	else
	{
		*ptr_AD_result = -1 * ADS1278_LSB * ((~AD_data + 1) & 0xFFFFFF);
	}
}

