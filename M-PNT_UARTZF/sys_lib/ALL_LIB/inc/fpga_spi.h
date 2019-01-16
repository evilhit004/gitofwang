/********************************************************************************/
// fpga_spi.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA SPIģ��ͷ�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.03.21.
// ����ʱ�䣺	2014.04.08.
// ����˵����
// 2014.03.25:
//	1.���ú���溯�����ã���������ٶ�
//	2.�޸��˲��ֺ꺯�����ƣ���֮ǰ�汾������
//	3.RBR�ĵ�ַ�������ΪSPIģ��Ļ���ַ,����֮ǰ�汾������
// 2014.08.26.
//	1.�����˷�ֹ�ظ����������
/********************************************************************************/
#ifndef __FPGA_SPI_H
#define __FPGA_SPI_H

#define SPI_SHIFT					1

/* �����Ϣ�����òο�	*/
//*********************************************
// SPIģʽ(MSB��ǰ)
//*********************************************/
#define SPI_MODE0					0
#define SPI_MODE1					0x4
#define SPI_MODE2					0x2
#define SPI_MODE3					0x6
#define MASK_SPI_DONE				0x40		// SPI�������
#define MASK_SPI_IDLE				0x20		// SPI�������

/* �Ĵ���ƫ�Ƶ�ַ	*/
#define SPI_GLBSET		(0x1 << SPI_SHIFT)		// дSPIȫ�����üĴ���		��/д
#define SPI_STATUS		(0x2 << SPI_SHIFT)  	// SPI״̬�Ĵ���	    	��
#define SPI_CTR			(0x3 << SPI_SHIFT)  	// SPI���ƼĴ���        	д
#define SPI_SCKDIV		(0x4 << SPI_SHIFT)  	// SPI����ʱ�ӷ�Ƶ�Ĵ���	д
#define SPI_SCKSETUP	(0x5 << SPI_SHIFT)  	// SPI����ʱ��Ĵ���    	д
#define	SPI_RBR			0 						// SPI����/�������ݼĴ���	��/д

/* �꺯��		*/
#define SPI_Trans(AddrBase)						*((unsigned short *)((AddrBase) + SPI_CTR)) = 0x1			// ����SPI����
#define SPI_SCK_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + SPI_SCKDIV)) = (data)		// ����SCK��Ƶϵ��	
#define SPI_Mode_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + SPI_GLBSET)) = (data)		// ����SPI����ģʽ
#define SPI_SS_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + SPI_SCKSETUP)) = (data)	// ����SS��SCK����ʱ��(������)
#define SPI_THR_Write(data,AddrBase)			*((unsigned short *)((AddrBase) + SPI_RBR)) = (data)
#define SPI_RBR_Read(AddrBase)					(*((unsigned short *)((AddrBase) + SPI_RBR)))
#define SPI_Status_Read(AddrBase)				(*((unsigned short *)((AddrBase) + SPI_STATUS)))

void SPI_Write(unsigned int addrbase, unsigned short data);

#endif	/*	__FPGA_SPI_H	*/
