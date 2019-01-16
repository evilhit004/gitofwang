/********************************************************************************/
// fpga_ad7894.c
// �汾��		2.1.0.
// �ļ�˵����	FPGA AD7894ģ��C�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.09.20.
// ����ʱ�䣺	
// ����˵����
/********************************************************************************/
#include "fpga_ad7894.h"
#include "fpga_spi.h"

//-----------------------------------------------------------------
// AD7894��ʼ������
//-----------------------------------------------------------------
void AD7894_init(unsigned int addrbase, unsigned char Con_Width, unsigned char sck_div, unsigned char spi_mode)				
{
	AD7894_WRITE_SET(Con_Width,addrbase);	//����ADת�������źŸߵ�ƽ��Ч��� CONVST = ConWidth*Tclk
	SPI_SCK_Set(sck_div,addrbase);			//����AD7894 SPI�ӿڵĴ�����λʱ��Ƶ�� fSCK = Tclk/sck_div
	SPI_Mode_Set(spi_mode,addrbase);		//SCK�����ز�����MSB��ǰ��SCK����Ϊ�͵�ƽ(mode0)
}
//-----------------------------------------------------------------
// ��AD7894����
//-----------------------------------------------------------------
void AD7894_Read(float * ptrAD_result, unsigned int addrbase)
{
	unsigned short AD_Data = 0; 
	unsigned char status = 0;

	status = AD7894_READ_STATUS(addrbase);			//��ѯADת����ϱ�־
	while((status | 0xfe) != 0xfe)				
	{
		status = AD7894_READ_STATUS(addrbase);
	}	
	
	SPI_Trans(addrbase);

	status = SPI_Status_Read(addrbase);
	
	while((status & 0x40) != 0x40)					
	{
		status = SPI_Status_Read(addrbase);
	}
	
 	AD_Data = SPI_RBR_Read(addrbase) & 0x3fff;		//��ȡAD����
	*(ptrAD_result) = AD_Data * AD7894_LSB;
}


