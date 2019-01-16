/********************************************************************************
fpga_ads1278.h
�汾��		2.1.0.
�ļ�˵����	FPGA ADS1278ģ��ͷ�ļ�
��д�ߣ�	ChePengyu
��дʱ�䣺	2014.06.10.
����ʱ�䣺	2014.09.30.
����˵����
	2014.09.30.
		1.���¶����˼Ĵ�����ַ
		2.�޸��˺���
	2015.05.29.
		1.�޸�ΪFIFOģʽ�������˶�FIFO�����ĺ꺯��
	2015.06.11.
		1.ȥ����FIFOģʽ�������������������
	2015.06.18.
		��ӦFPGA���޸ģ�
		1.ȥ����FIFO��صĺ궨��
		2.ȥ�������湦����صĺ궨��
		3.�����˺���ԭ��
********************************************************************************/
#ifndef __ADS1278_H
#define __ADS1278_H

#define ADS1278_SHIFT		1

/* �����Ϣ�����òο�	*/
#define ADS1278_LSB			0.00000029802//0.000002384184//

/* �Ĵ���ƫ�Ƶ�ַ	*/

#define	A_ADS1278_CTR			(0x8 << ADS1278_SHIFT) 	// ADS1278 ���ƼĴ���(W)
#define	A_ADS1278_STATUS		(0x9 << ADS1278_SHIFT) 	// ADS1278 ״̬�Ĵ���(R)
#define	A_ADS1278_SETR			(0xA << ADS1278_SHIFT) 	// ADS1278 ���üĴ���(R/W)
#define	A_ADS1278_CLKDIV		(0xB << ADS1278_SHIFT) 	// ADS1278 ��ʱ�ӷ�Ƶ�Ĵ���(W)


#define A_Data_BASE	    		(0x10 << ADS1278_SHIFT)	// ���ݻ���ַ


/* �꺯��		*/								
#define ADS1278_READ_STATUS(base)				(*((unsigned short *)(base + A_ADS1278_STATUS)))
#define ADS1278_READ_DATA(base,offset)			(*((unsigned short *)(base + A_Data_BASE + (offset << ADS1278_SHIFT))))	
#define ADS1278_READ_SET(base)					(*((unsigned short *)(base + A_ADS1278_SETR)))

#define ADS1278_SYNC(base)						*((unsigned short *)(base + A_ADS1278_CTR)) = (0x1)									  
#define ADS1278_WRITE_SET(base,data)			*((unsigned short *)(base + A_ADS1278_SETR)) = (data)
#define ADS1278_WRITE_CLK(base,data)			*((unsigned short *)(base + A_ADS1278_CLKDIV)) = (data)

	
/* ����ԭ��	*/
/********************************************
addrbase	:ADS1278����ַ
ad_mode		:0x1B
sck_div		:0x4
spi_mode	:SPI_MODE0
spi_ss		:0x2
********************************************/
void ADS1278_Init(unsigned int addrbase, unsigned char ad_mode, unsigned char sck_div, unsigned char spi_mode, unsigned char spi_ss);
/********************************************
addrbase		:ADS1278����ַ
ptr_AD_result	:AD���������׵�ַ
size			:AD���������С
********************************************/
void ADS1278_Read(unsigned int addrbase, float * ptr_AD_result, const int size);
/********************************************
addrbase		:ADS1278����ַ
ptr_AD_result	:AD���ݵ�ַ
chn				:ADͨ���ţ�0~7��
********************************************/
void ADS1278_Read1chn(unsigned int addrbase, float * ptr_AD_result, const int chn);

#endif	/*	__ADS1278_H	*/
