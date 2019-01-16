/********************************************************************************/
// fpga_ad976.c
// �汾��		2.1.0.
// �ļ�˵����	FPGA AD976ģ��c�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.12.25.
// ����ʱ�䣺	
// ����˵����
/********************************************************************************/
#include "fpga_ad976.h"

/*-------------------------------------------------------------------
-- AD976���ݶ�ȡ����
-- AddrBase:		���ڻ���ַ
-- channel:			ADͨ����
-- AD_result��		AD�����ͽ��
-- ����ֵ��			��
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
	if((AD_Data & 0x8000) == 0x8000)	// ����ѹ
	{
		*AD_result = -1 * AD976_LSB * ((~AD_Data + 1) & 0xFFFF);
	}
	else	// ����ѹ
	{
		*AD_result = AD976_LSB * AD_Data;
	}
	
}
