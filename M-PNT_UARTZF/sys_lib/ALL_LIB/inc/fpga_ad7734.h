/********************************************************************************/
// fpga_AD7734.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA AD7734ģ��ͷ�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.09.20.	
// ����ʱ�䣺	2014.10.29.
// ����˵����
//	2014.09.29.
//		1.ɾ����ַ0~5����ΪSPI_Core�ĵ�ַ
//		2.ɾ��AD7734ר�е�SPI������������Ϊ����fpga_spi.hʵ��
//		3.AD7734_Conv��AD7734_ReadData������������unsigned int addrbase�β�
//	2014.10.29.
//		1.ɾ����AD7734_Conv����ԭ��
//		2.������AD7734_Latch�꺯����A_AD_LATR��AD7734_RST_CMD�ĺ궨��
//	2015.08.03.
//		1.�����˶�ȡ��ͨ�����ݵĺ���AD7734_Read1chn�ĺ�������
/********************************************************************************/

#ifndef __AD7734_H__
#define __AD7734_H__

#define AD7734_RST_CMD		0x4

#define AD7734_SHIFT		1

#define AD7734_LSB			0.0000011921

#define A_AD_CTR			(8 << AD7734_SHIFT)	
#define A_AD_Status			(9 << AD7734_SHIFT)	
#define A_AD_SETR			(0xa << AD7734_SHIFT)
#define A_AD_LATR			(0xb << AD7734_SHIFT)
			             
#define A_Data_CH0	    	(0xc << AD7734_SHIFT)	
#define A_Data_CH1	    	(0xd << AD7734_SHIFT)	
#define A_Data_CH2	    	(0xe << AD7734_SHIFT)	
#define A_Data_CH3	    	(0xf << AD7734_SHIFT)	

#define AD7734_READ_DATA(base,offset)		(*((unsigned short *)(base + A_Data_CH0 + (offset << AD7734_SHIFT))))	
#define AD7734_READ_STATUS(base)			(*((unsigned short *)(base + A_AD_Status)))

#define AD7734_WRITE_SET(base,data)			*((unsigned short *)(base + A_AD_SETR)) = (data)	
#define AD7734_WRITE_CTR(base,data)			*((unsigned short *)(base + A_AD_CTR)) = (data)	
#define AD7734_Latch(base)					*((unsigned short *)(base + A_AD_LATR)) = (0x55)

/* ����ԭ��	*/

/********************************************
addrbase		:AD7734����ַ
float			:4·AD����
********************************************/
void AD7734_ReadData(unsigned int AddrBase, float AD_result[4]);
/********************************************
addrbase		:AD7734����ַ
ptr_AD_result	:AD���ݵ�ַ
chn				:ADͨ���ţ�0~3��
********************************************/
void AD7734_Read1chn(unsigned int addrbase, float * ptr_AD_result, const int chn);

#endif 
