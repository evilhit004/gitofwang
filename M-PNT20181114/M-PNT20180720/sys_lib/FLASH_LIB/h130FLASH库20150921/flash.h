/************************************************************	
	FLASH�ͺţ�	AM29LV400	
	��д�ˣ�	������
	��дʱ�䣺	2014.07.31.
	����ʱ�䣺	2015.09.18.
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
		2015.09.18
			1.OMAPƽ̨��FLASH�������ַӦ������1λ���Ƕ�Ӧ��DSP�߼���ַ��
		  	  FLASH_ADDRSHIFT�޸�Ϊ1
************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

extern unsigned int sector_addr[11];	// FLASH��sector�׵�ַ
extern unsigned char gp_num[5];	

/* AM29LV400BB FLASH��ص�ַ��16bit�˿�ģʽ	*/

#define C6747_FLASH									// ����ǵ���C6747����OMAP�뿪���˶��壬C6713�����δ˶���
#define GPIOADDR				0x01E2603C
#define FLASH_HIGADDR_SHIFT		14
#define FLASH_ADDRSHIFT			1					/* FLASH�����ַת��ΪDSP�߼���ַ��Ҫ����λ��	*/

#define FLASH_BaseAddr			0x60000000			/* FLASH����ַ	*/

// FLASH�������ַ
#define PSY_flashSA0		0x00000
#define PSY_flashSA1		0x02000
#define PSY_flashSA2		0x03000
#define PSY_flashSA3		0x04000
#define PSY_flashSA4		0x08000
#define PSY_flashSA5		0x10000
#define PSY_flashSA6		0x18000
#define PSY_flashSA7		0x20000
#define PSY_flashSA8		0x28000
#define PSY_flashSA9		0x30000
#define PSY_flashSA10		0x38000

// WORD MODE FLASH�ζ�Ӧ��DSP�߼���ַ
#define flashSA0		(FLASH_BaseAddr + (0x00000 << FLASH_ADDRSHIFT))
#define flashSA1		(FLASH_BaseAddr + (0x02000 << FLASH_ADDRSHIFT))
#define flashSA2		(FLASH_BaseAddr + (0x03000 << FLASH_ADDRSHIFT))
#define flashSA3		(FLASH_BaseAddr + (0x04000 << FLASH_ADDRSHIFT))
#define flashSA4		(FLASH_BaseAddr + (0x08000 << FLASH_ADDRSHIFT))
#define flashSA5		(FLASH_BaseAddr + (0x10000 << FLASH_ADDRSHIFT))
#define flashSA6		(FLASH_BaseAddr + (0x18000 << FLASH_ADDRSHIFT))
#define flashSA7		(FLASH_BaseAddr + (0x20000 << FLASH_ADDRSHIFT))
#define flashSA8		(FLASH_BaseAddr + (0x28000 << FLASH_ADDRSHIFT))
#define flashSA9		(FLASH_BaseAddr + (0x30000 << FLASH_ADDRSHIFT))
#define flashSA10		(FLASH_BaseAddr + (0x38000 << FLASH_ADDRSHIFT))

// BYTE MODE FLASH�ζ�Ӧ��DSP�߼���ַ
/*  
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
*/

// FLASH�����ֵ�ַ	
/*
#define FLASH_OP1		0xAAA				// FLASH byte mode contrl address			
#define FLASH_OP2		0x555
*/
#define FLASH_OP1		0x555				// FLASH word mode contrl address			
#define FLASH_OP2		0xAAA

#define ctrladdr1		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP1 << FLASH_ADDRSHIFT)))
#define ctrladdr2		((unsigned short *)(FLASH_BaseAddr + (FLASH_OP2 << FLASH_ADDRSHIFT)))



//-------------------------------------------------
// ����ԭ��
//-------------------------------------------------
/*-------------------------------------------------------------------
-- FLASH��д����
-- addr��		FLASH��ַ
-- data��		16λ����
-- ����ֵ��		0-�������		1-����ʧ��
-- ע�⣺		���#define C6747_FLASH��߱���GPIOģ���λ��ַ�Ŀ���
-------------------------------------------------------------------*/ 
int flash_program(unsigned int, unsigned short);
/*-------------------------------------------------------------------
-- FLASH�β�������
-- sector��		FLASH�ε�ַ
-- ����ֵ��		0-�������		1-����ʧ��
-------------------------------------------------------------------*/ 
int sector_erase(unsigned int);
int chip_erase(void);
/*-------------------------------------------------------------------
-- ʹ��Data# Polling�ж�FLASH����״̬����
-- addr��		FLASH��ַ
-- ����ֵ��		0-�������		1-����ʧ��
-------------------------------------------------------------------*/ 
int data_polling(unsigned int);
/*-------------------------------------------------------------------
-- ʹ��toggle bit�ж�FLASH����״̬����
-- addr��		FLASH��ַ
-- ����ֵ��		0-�������		1-����ʧ��
-------------------------------------------------------------------*/ 
int toggle_bit(unsigned int );
unsigned int highEA_conv(unsigned int, unsigned char []);
/*-------------------------------------------------------------------
-- FLASH���ݶ�ȡ����
-- addr��		FLASH��ַ
-- ����ֵ:		��
-------------------------------------------------------------------*/ 
void flash_read(unsigned int addr, unsigned short *);		

#endif	/*	__FLASH_H	*/
