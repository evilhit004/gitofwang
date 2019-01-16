/********************************************************************************/
// fpga_OneWire_DMA.h
// �汾��		2.1.0.
// �ļ�˵����	1-Wire���� DMAģ��ͷ�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.03.25.
// ����ʱ�䣺	2014.12.24.
// ����˵����
// 	2014.12.24.
//		1.�����˷�ֹ�ظ����������
//		2.������״̬�ֵĺ�
//		3.����DS18B20��صĺ���ԭ��
/********************************************************************************/
/********************************************************************************/
// status�Ĵ���
// ISR(7):	0				����
// ISR(6):	0				����
// ISR(5):	0				����
// ISR(4):	OverRun			'1'��ʾ������overrun����
// ISR(3):	DMA_Done		'1'��ʾDMA���У�'0'��ʾDMA������
// ISR(2):	reset_PDR		'1'��ʾ��⵽���豸��'0'��ʾδ��⵽���豸���ñ�־��ÿ�ν��и�λ��������ˢ��
// ISR(1):	0				����
// ISR(0):	Data_updata		'1'��ʾ�������ݼĴ����������ݣ�'0'��ʾ�������ݡ��ñ�־�ڶ�ȡ�������ݼĴ�������0�� 
/********************************************************************************/
#ifndef __FPGA_OW_H
#define __FPGA_OW_H

#define	ONEWIRE_DMA_SHIFT			1 

/* �����Ϣ�����òο�	*/
#define MASK_OW_DMA_DONE			0x8
#define MASK_OW_DATA_UPD			0x1

/* �Ĵ���ƫ�Ƶ�ַ	*/ 
#define ONEWIRE_CMD					0								// ����Ĵ���				W/R	
#define ONEWIRE_TRB					(0x1 << ONEWIRE_DMA_SHIFT)      // ����/����(��8λ)�Ĵ��� 	W/R	
#define ONEWIRE_ISR					(0x2 << ONEWIRE_DMA_SHIFT)      // �ж�״̬�Ĵ���     		R   
//#define ONEWIRE_IER				(0x3 << ONEWIRE_DMA_SHIFT)      // �ж�ʹ�ܼĴ��� 			W/R	(δʹ��)	
//#define ONEWIRE_DIV				(0x4 << ONEWIRE_DMA_SHIFT)      // ʱ�ӷ�Ƶϵ���Ĵ��� 		W/R (δʹ��)
//#define ONEWIRE_CTR				(0x5 << ONEWIRE_DMA_SHIFT)      // ���ƼĴ���         		W/R (δʹ��)
#define ONEWIRE_RBH					(0x6 << ONEWIRE_DMA_SHIFT)      // ����(��8λ)�Ĵ���        R   

/* �꺯��		*/
#define OW_Status_Read(AddrBase)			(*((unsigned short *)((AddrBase) + ONEWIRE_ISR)))
#define OW_DataL_Read(AddrBase)				(*((unsigned short *)((AddrBase) + ONEWIRE_TRB)))
#define OW_DataH_Read(AddrBase)				(*((unsigned short *)((AddrBase) + ONEWIRE_RBH)))
#define OW_DMA_Start(AddrBase)				*((unsigned short *)(AddrBase)) = 0x1

/* ����ԭ��		*/
int DS18B20_Conv(unsigned int AddrBase);
int DS18B20_Read(unsigned int AddrBase, float *temp_result);

#endif	/*	__FPGA_OW_H	*/
