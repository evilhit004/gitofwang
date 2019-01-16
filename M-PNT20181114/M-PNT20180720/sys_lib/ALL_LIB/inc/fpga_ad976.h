/********************************************************************************/
// fpga_AD976.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA AD976ģ��ͷ�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.12.25.	
// ����ʱ�䣺	
// ����˵����
/********************************************************************************/

#ifndef __AD976_H__
#define __AD976_H__

#define AD976_SHIFT			1

/*	������òο�	*/
#define AD976_LSB			0.000305175
#define AD976_CONV_CMD		0x1

/* �Ĵ���ƫ�Ƶ�ַ	*/
#define AD976_CTR			0						// ״̬�Ĵ���(R)/���ƼĴ���(W)			             
#define AD976_Data_CH0	    (0x1 << AD976_SHIFT)	// AD���ݼĴ���
#define AD976_Data_CH1	    (0x2 << AD976_SHIFT)	
#define AD976_Data_CH2	    (0x3 << AD976_SHIFT)	
	
/* �꺯��		*/
#define AD976_READ_DATA(base,offset)		(*((unsigned short *)(base + AD976_Data_CH0 + (offset << AD976_SHIFT))))	
#define AD976_READ_STATUS(base)				(*((unsigned short *)(base + AD976_CTR)))	
#define AD976_WRITE_CTR(base,data)			*((unsigned short *)(base + AD976_CTR)) = (data)	

/* ����ԭ��	*/
void AD976_Read(unsigned int AddrBase, unsigned char channel, float *AD_result);

#endif 		/*	__AD976_H__		*/
