/********************************************************************************/
// fpga_spi.c
// �汾��		2.1.0.
// �ļ�˵����	FPGA SPIģ��c�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.09.29.
// ����ʱ�䣺	
// ����˵����
/********************************************************************************/
#include "fpga_spi.h"

//-----------------------------------------------------------------
// SPI���ݷ��ͺ���
//-----------------------------------------------------------------
void SPI_Write(unsigned int addrbase, unsigned short data)
{
	unsigned char	status = 0;
	
	status = SPI_Status_Read(addrbase);		
	while((status & MASK_SPI_IDLE) != MASK_SPI_IDLE)// SPI���������
	{
		status = SPI_Status_Read(addrbase);
	}
	
	SPI_THR_Write(data,addrbase);					// дSPI���ͼĴ���
	SPI_Trans(addrbase);							// ����1��SPI����			
}	
