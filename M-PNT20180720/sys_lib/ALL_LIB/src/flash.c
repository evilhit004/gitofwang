/************************************************************	
	FLASH��������
	FLASH�ͺţ�	AM29LV400	
	��д�ˣ�	������
	��дʱ�䣺	2014.07.31.
	����ʱ�䣺	2014.10.28.
	����˵����
		2014.09.09.
			1.��û���βεĺ������void	
			2.��unsigned short flash_read(unsigned int addr)
			  �޸�Ϊvoid flash_read(unsigned int addr, unsigned short *data)
			  ȥ���˷���ֵ������������������
		2014.09.11.
			1.�����˱���gpio_v�����������ж�
		2014.10.24.
			1.�޸�����дC6713ʱ������д��2�εĴ���
		2014.10.28.
			1.������data_polling����������������״̬
************************************************************/
#include "flash.h"

unsigned char gp_num[5] = {0,2,4,8,9};

/*-------------------------------------------------------------------
-- FLASH�β�������
-- sector��		FLASH�ε�ַ
-- ����ֵ��		0-�������		1-����ʧ��
-------------------------------------------------------------------*/ 
int sector_erase(unsigned int sector)
{ 
	unsigned int status = 0;
	
	#ifdef C6747_FLASH
		unsigned int gpio_v = 0;
	#endif
 
    *ctrladdr1 = 0xaa; 
	*ctrladdr2 = 0x55;
	*ctrladdr1 = 0x80;  
	*ctrladdr1 = 0xaa; 
	*ctrladdr2 = 0x55; 
	
	#ifdef C6747_FLASH
		gpio_v = highEA_conv(sector,gp_num);
		*((unsigned int *)GPIOADDR) = gpio_v;	// ����GPIO���Կ���FLASH��4λ��ַ	
		*((unsigned short *)sector) = 0x30;		/* ���������	*/
		status = toggle_bit(0xa);				// #1 ������#2ǰ	������ɺ�����ͷŵ�ַ��/��λ��ַ�ߣ����򽫴���
		*((unsigned int *)GPIOADDR) = 0;		// #2
	#else					
		*((unsigned short *)sector) = 0x30;
		status = toggle_bit(0xa);
	#endif	
			
	return(status);					
}
/*-------------------------------------------------------------------
-- FLASH��������
-- ����FLASHȫ������
-- ����ֵ��		0-�������		1-����ʧ��
-------------------------------------------------------------------*/ 
int chip_erase(void)
{
	int status = 0;
	
	*ctrladdr1 = 0xaa; 
	*ctrladdr2 = 0x55;
	*ctrladdr1 = 0x80; 
	*ctrladdr1 = 0xaa;
	*ctrladdr2 = 0x55;
	*ctrladdr1 = 0x10;

	status = toggle_bit(0xa);
	
	return(status);			
}

/*-------------------------------------------------------------------
-- FLASH��д����
-- addr��		FLASH��ַ
-- data��		16λ����
-- ����ֵ��		0-�������		1-����ʧ��
-- ע�⣺		���#define GPIOADDR��߱���GPIOģ���λ��ַ�Ŀ���
-------------------------------------------------------------------*/ 
int flash_program(unsigned int addr, unsigned short data)	
{	
	int status = 0;
	
	#ifdef C6747_FLASH
		unsigned int gpio_v = 0;
	#endif

	*ctrladdr1 = 0xaa; 
	*ctrladdr2 = 0x55;
	*ctrladdr1 = 0xa0; 
		
	#ifdef C6747_FLASH
		gpio_v = highEA_conv(addr,gp_num);

		*((unsigned int *)GPIOADDR) = gpio_v;		// ��λ��ַ���
		
		*((unsigned short *)(addr))	= data;	
		status = toggle_bit(0xa);					// #1 ������#2ǰ	��д��ɺ�����ͷŵ�ַ��/��λ��ַ�ߣ����򽫴���
		
		*((unsigned int *)GPIOADDR) = 0;			// #2 ��λ��ַ�ر�
	#else
		*((unsigned short *)(addr))	= data;	
		status = toggle_bit(0xa);					
	#endif	
		
	return(status);
}
/*-------------------------------------------------------------------
-- ʹ��Data# Polling�ж�FLASH����״̬����
-- addr��		FLASH��ַ
-- ����ֵ��		0-�������		1-����ʧ��
-------------------------------------------------------------------*/ 
int data_polling(unsigned int addr)
{
	unsigned short temp = 0;
	
	while(1)
	{
		temp = *((unsigned short *)(addr));
		if((temp & 0x80) != 0x80)	// ����δ���
		{
			if((temp & 0x20) != 0x20)	// δ��ʱ
			{
				continue;	
			}
			else
			{
				temp = *((unsigned short *)(addr));
				if((temp & 0x80) != 0x80)
				{
					return(1);
				}
				else
				{
					return(0);
				}
			}
		}
		else
		{
			return(0);
		}	
	}		
}
/*-------------------------------------------------------------------
-- ʹ��toggle bit�ж�FLASH����״̬����
-- addr��		FLASH��ַ
-- ����ֵ��		0-�������		1-����ʧ��
-------------------------------------------------------------------*/ 
int toggle_bit(unsigned int addr)
{
	unsigned short temp1 = 0;
	unsigned short temp2 = 0;			
		
	
	while(1)
	{		
		temp1 = *((unsigned short *)(FLASH_BaseAddr + (addr << AddrShift))) & 0x40;	
		temp2 = *((unsigned short *)(FLASH_BaseAddr + (addr << AddrShift)));
		
		if((temp2 & 0x40) != temp1)				/* D6 = ~D6	����δ���*/
		{
			if((temp2 & 0x20) != 0x20)			/* D5 = 0	δ��ʱ������ѯ	*/
			{
				continue;
			}
			else							/* D5 = 1	*/
			{
				temp1 = *((unsigned short *)(FLASH_BaseAddr + (addr << AddrShift))) & 0x40;	
				temp2 = *((unsigned short *)(FLASH_BaseAddr + (addr << AddrShift)));
				
				if((temp2 & 0x40) != temp1)			/* D6 = ~D6	*/
				{					
					return(1);					
				}
				else
				{
					return(0);
				}
			}
		}
		else				/* D6 == D6	*/
		{
			return(0);
		}
	}		/* end while(1)	*/
		
}
/*-------------------------------------------------------------------
-- FLASH��λ��ַתDSP GPIOֵ
-- addr��		FLASH��ַ
-- pin_num��	���飬���������λ��ַ�߶�Ӧ��GPIO���ź�
--				����pin_num = {0,2,4,8}��ʾ:
--					GPIO[0]��ӦA14
--					GPIO[2]��ӦA15
--					GPIO[4]��ӦA16
--					GPIO[8]��ӦA17
-- ע�⣺		ʹ��GPIOģ���ַʱӦѡ��ͬһBANK��GPIO����
-- ����ֵ:		GPIO�Ĵ�������ֵ
-------------------------------------------------------------------*/ 
unsigned int highEA_conv(unsigned int addr, unsigned char pin_num[])
{
	int i = 0;
	int n = 0;
	unsigned int highEA = 0;
	unsigned int gpio_pin[5] = {0,0,0,0,0};
	unsigned int gpio_value = 0;
	
	highEA = 0xFF & (addr >> 14);		// ��ȡ��λ��ַ
	
	n = sizeof(pin_num);
	
	for(i=0;i<n;i++)
	{
		gpio_pin[i] = ((highEA >> i) & 0x1) << pin_num[i];
		gpio_value = gpio_value + gpio_pin[i];
	}

	return(gpio_value);		 
}
/*-------------------------------------------------------------------
-- FLASH���ݶ�ȡ����
-- addr��		FLASH��ַ
-- ����ֵ:		��
-------------------------------------------------------------------*/ 
void flash_read(unsigned int addr, unsigned short *data)
{		
	#ifdef C6747_FLASH	
		unsigned int gpio_v = 0;
		
		gpio_v = highEA_conv(addr,gp_num);
		
		*((unsigned int *)GPIOADDR) = gpio_v;		// ��λ��ַ���		
		*data = *((unsigned short *)(addr));			
		*((unsigned int *)GPIOADDR) = 0;			// ��λ��ַ�ر�
	#else
		*data = *((unsigned short *)(addr));	
	#endif		
}

