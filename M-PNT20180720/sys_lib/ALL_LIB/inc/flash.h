/************************************************************	
	FLASH�ͺţ�	AM29LV400	
	��д�ˣ�	������
	��дʱ�䣺	2014.07.31.
	����ʱ�䣺	2015.05.29.
	����˵����	
		2014.08.26.
			1.�����˷�ֹ�ظ����������
			2.ȥ����#include <stdio.h>���
			3.�����˱���sector_addr��gp_num������
		2014.10.28.
			1.����ַ�޸�Ϊ8bitģʽ������������C6713���޷�����
			  FLASH����sector������
			2.������data_polling����ԭ��
		2015.05.29.
			1.�޸�Ϊ��Ԥ���뷽ʽѡ����OMAPL13x����C6713
				ʹ�ܺ궨��C6747_FLASH��	C6747/OMAP
				���κ궨��C6747_FLASH��	C6713
************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

extern unsigned int sector_addr[11];	// FLASH��sector�׵�ַ
extern unsigned char gp_num[5];	

/* AM29LV400BB��16bit�˿�ģʽ	*/
#define FLASH_SIZE		0x40000							/* FLASH����	256K x 16bit	*/

//#define C6747_FLASH		// ����ǵ���C6747����OMAP�뿪���˶��壬C6713�����δ˶���


#ifdef C6747_FLASH
	//-------------------------------------------------
	// OMAPL137������´���
	//-------------------------------------------------
	#define GPIOADDR		0x01E2603C
	#define FLASH_BaseAddr	0x60000000						// omapl137 FLASH����ַ
	#define flashSA0		(FLASH_BaseAddr + 0x00000)
	#define flashSA1		(FLASH_BaseAddr + 0x02000)
	#define flashSA2		(FLASH_BaseAddr + 0x03000)
	#define flashSA3		(FLASH_BaseAddr + 0x04000)
	#define flashSA4		(FLASH_BaseAddr + 0x08000)
	#define flashSA5		(FLASH_BaseAddr + 0x10000)
	#define flashSA6		(FLASH_BaseAddr + 0x18000)
	#define flashSA7		(FLASH_BaseAddr + 0x20000)
	#define flashSA8		(FLASH_BaseAddr + 0x28000)
	#define flashSA9		(FLASH_BaseAddr + 0x30000)
	#define flashSA10		(FLASH_BaseAddr + 0x38000)
	
	// word ģʽ��Ӧ�������ֵ�ַ	
	
	#define FLASH_OP1		0xAAA				// FLASH word mode contrl address			
	#define FLASH_OP2		0x555
	
	#define AddrShift		0								
	
	#define ctrladdr1		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP1<<AddrShift)))
	#define ctrladdr2		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP2<<AddrShift)))
#else
//-------------------------------------------------
// C6713������´���
//-------------------------------------------------
	#define FLASH_BaseAddr	0x90000000					// C6713 FLASH����ַ
	#define flashSA0		(FLASH_BaseAddr + 0x00000)
	#define flashSA1		(FLASH_BaseAddr + 0x04000)
	#define flashSA2		(FLASH_BaseAddr + 0x06000)
	#define flashSA3		(FLASH_BaseAddr + 0x08000)
	#define flashSA4		(FLASH_BaseAddr + 0x10000)
	#define flashSA5		(FLASH_BaseAddr + 0x20000)
	#define flashSA6		(FLASH_BaseAddr + 0x30000)
	#define flashSA7		(FLASH_BaseAddr + 0x40000)
	#define flashSA8		(FLASH_BaseAddr + 0x50000)
	#define flashSA9		(FLASH_BaseAddr + 0x60000)
	#define flashSA10		(FLASH_BaseAddr + 0x70000)
	
	#define FLASH_OP1		0x555				// FLASH word mode contrl address			
	#define FLASH_OP2		0x2AA
	
	#define AddrShift		0	
	
	#define ctrladdr1		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP1<<1)))
	#define ctrladdr2		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP2<<1)))
#endif	
//-------------------------------------------------
// ����ԭ��
//-------------------------------------------------
int flash_program(unsigned int, unsigned short);
int sector_erase(unsigned int);
int chip_erase(void);
int data_polling(unsigned int);
int toggle_bit(unsigned int );
unsigned int highEA_conv(unsigned int, unsigned char []);
void flash_read(unsigned int addr, unsigned short *);		

#endif	/*	__FLASH_H	*/
