/********************************************************************************/
// fpga_ad7734.c
// �汾��		2.1.0.
// �ļ�˵����	FPGA AD7734ģ��C�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.09.20.
// ����ʱ�䣺	2015.08.03.
// ����˵����
//	2014.09.29.
//		1.��Ӧvhd��ͷ�ļ����޸Ľ����޸�
//		2.ȥ����AD7734_write2reg��������fpga_spi.c��ʵ��SPI_Write
//	2014.10.29.
//		1.ɾ����AD7734_Conv����
//		2.AD7734_ReadData��������AD7734_Latch(AddrBase)�������������������ֹ��ȡ
//		  ����ʱ�����������
//	2015.08.03.
//		1.�����˶�ȡ��ͨ�����ݵĺ���AD7734_Read1chn	
/********************************************************************************/
#include "fpga_ad7734.h"
	
//-----------------------------------------------------------------
// ��AD7734 4ͨ������
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
	while((status & 0x1) != 0x1)					// ��ѯ�Ƿ����µ�ת������
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
		
		if((AD_data[i] & 0x800000) == 0x800000)		// 0 & ����ѹ
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
		
	if((AD_data & 0x800000) == 0x800000)		// 0 & ����ѹ
	{
		*result = (AD_data - 0x800000) * AD7734_LSB;
	}
	else
	{
		*result = -1 * AD7734_LSB * (0x800000 - AD_data);
	}	
}
 



