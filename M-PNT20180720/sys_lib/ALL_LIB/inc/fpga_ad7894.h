/********************************************************************************/
// fpga_AD7894.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA AD7894ģ��ͷ�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.03.25.
// ����ʱ�䣺	2014.09.28.	
// ����˵����
// 2014.03.25:
//	1.���ú���溯�����ã���������ٶ�
//	2.�޸��˲��ֺ꺯�����ƣ���֮ǰ�汾������
// 2014.09.28:
//	1.ȥ���˶�·���ؿ��Ƶĺ꺯�����ַ
//	2.ȥ����AD7894_Conv������������AD7894_CTR�����������ӿ��Ƶ������
//	3.������AD7894_LSB�Ķ��壬����ǰ(0.00015)����ȷ��
//	4.����д�˺꺯������֮ǰ�İ汾������
/********************************************************************************/
/*	SETR(7:3):	AD�����ź���Ч��ƽ��ȣ���� = ADSetConv��Ticlk 					*/			
/*	SETR(2)��	RSV                                                          	*/		
/*	SETR(1)��	RSV                                                          	*/			
/********************************************************************************/
/* 	CTR(0)��	����ת�����д'1'����ת��										*/
/********************************************************************************/
#ifndef __AD7894_REGS_H__
#define __AD7894_REGS_H__

#define	AD7894_SHIFT				1 

#define AD7894_LSB					0.0001525879

/* �Ĵ���ƫ�Ƶ�ַ	*/
#define	AD7894_CTR					(0x8 << AD7894_SHIFT)	// ���ƼĴ���
#define	AD7894_STA					(0x9 << AD7894_SHIFT)	// ״̬�Ĵ���
#define	AD7894_SETR					(0xA << AD7894_SHIFT)	// ���üĴ���

/* �꺯��		*/
#define AD7894_WRITE_SET(base,data)		*((unsigned short *)((base) + AD7894_SETR)) = (data)
#define AD7894_READ_SET(base)			(*((unsigned short *)((base) + AD7894_SETR)))
#define AD7894_WRITE_CTR(base,data)		*((unsigned short *)((base) + AD7894_CTR)) = (data)
#define AD7894_READ_STATUS(base)		(*((unsigned short *)((base) + AD7894_STA)))

void AD7894_Read(float * , unsigned int);
void AD7894_init(unsigned int addrbase, unsigned char Con_Width, unsigned char sck_div, unsigned char spi_mode);


#endif 
