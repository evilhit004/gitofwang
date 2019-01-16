/************************************************************	
	DS18B20��������	
	��д�ˣ�	������
	��дʱ�䣺	2014.12.24.
	ģ��˵����	����δʹ�õȴ���ѯ��ʽ���Ƿ�ֹDS18B20��OW����
				���ֹ��Ϻ��³��������ѭ�����޷���������ִ��
	����ʱ�䣺	
	����˵����
************************************************************/
#include "fpga_OneWire_DMA.h"

/*-------------------------------------------------------------------
-- DS18B20��������
-- ��ѯOW����DMA�Ƿ���У��������������OW����DMA����������0�����򷵻�1
-- AddrBase��		DS18B20����ַ
-- ����ֵ��			0-ִ������	1-δִ������
-------------------------------------------------------------------*/
int DS18B20_Conv(unsigned int AddrBase)
{
	unsigned char status = 0;
	
	status = OW_Status_Read(AddrBase);
	
	if((status & MASK_OW_DMA_DONE) == MASK_OW_DMA_DONE)
	{
		OW_DMA_Start(AddrBase);		// ����OW
		return(0);
	}
	else
	{
		return(1);
	}		
}
/*-------------------------------------------------------------------
-- DS18B20���ݶ�ȡ����
-- ��ѯ�Ƿ��������ݣ���������ȡ����Ⲣ����0�����򷵻�1
-- temp_result:		�¶Ƚ��
-- AddrBase��		DS18B20����ַ
-- ����ֵ��			0-������ݶ�ȡ	1-�����޸��£�δִ�ж�ȡ
-------------------------------------------------------------------*/
int DS18B20_Read(unsigned int AddrBase, float *temp_result)
{
	unsigned char temp_L = 0;
	unsigned char temp_H = 0;
	unsigned short temp = 0;	
	unsigned char status = 0;
	
	status = OW_Status_Read(AddrBase);
	
	if((status & MASK_OW_DATA_UPD) == MASK_OW_DATA_UPD)	// ��������ݸ���
	{	
		temp_L = OW_DataL_Read(AddrBase);
		temp_H = OW_DataH_Read(AddrBase);
		
		temp = (temp_H << 8) + temp_L;
		
		if((temp_H & 0x80) == 0x80)		// ����λΪ1
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

