/********************************************************************************/
// fpga_init.c
// �汾��		2.1.0.
// �ļ�˵����	FPGA��ʼ���ļ������ļ�����Ӳ������ʦ����Ӳ��ϵͳ�����޸�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.09.09.
// ����ʱ�䣺	2014.09.29.
// ����˵����
//	2014.09.29.
//		1.������AD7734_init������SPI����
/********************************************************************************/
#include "fpga_init.h"

//-----------------------------------------------------------------
// AD7734��ʼ������
//-----------------------------------------------------------------
//void AD7734_init(void)
//{
//	// ��ʼ��AD7734 SPI�ӿ�
//	SPI_Mode_Set(0x6, AD7734_Base);			// SPI����Ϊ��1�������ط������ݣ���2�������زɼ����ݣ�����״̬Ϊ��1����MSB��ǰ��LSB�ں�
//	SPI_SCK_Set(0xc, AD7734_Base);			// SCK����18.432MHz/3 = 6.114MHz
//	SPI_SS_Set(0x2, AD7734_Base);			// SCK��1�������ص�CSn�Ľ���ʱ�� 20ns x 2 = 40ns	
//	
//	// ��ʼ��AD7734 SPI�ӿ�
//	SPI_Mode_Set(0x6, AD7734_Base2);		// SPI����Ϊ��1�������ط������ݣ���2�������زɼ����ݣ�����״̬Ϊ��1����MSB��ǰ��LSB�ں�
//	SPI_SCK_Set(0xc, AD7734_Base2);			// SCK����18.432MHz/3 = 6.114MHz
//	SPI_SS_Set(0x2, AD7734_Base2);			// SCK��1�������ص�CSn�Ľ���ʱ�� 20ns x 2 = 40ns		
//	
//	// ��λAD7734	
////	AD7734_WRITE_CTR(AD7734_Base,AD7734_RST_CMD);	// ��λ����Ҫ�ӳٵȴ�2us ��λ�͵�ƽ����ʱ������Ϊ1us
////
//
//// 1			
//	// ����AD7734 ����ģʽ
//	AD7734_WRITE_SET(AD7734_Base2,0x0);		// ����SPI����Ϊ��DMA��ʽ
//	
//	SPI_Write(AD7734_Base2,0x00);			// �����λ����
//	SPI_Write(AD7734_Base2,0xFF);
//	SPI_Write(AD7734_Base2,0xFF);
//	SPI_Write(AD7734_Base2,0xFF);
//	SPI_Write(AD7734_Base2,0xFF);
//
//	_wait(185180);		// 1�� = 0.054us
//
//	// CH0
//	SPI_Write(AD7734_Base2,0x28);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base2,0x08);		//enable the channel AIN0 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);	
//
//	SPI_Write(AD7734_Base2,0x30);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base2,0x86);		//chop enabled, and the update rate is 2534Hz(395us@6.144MHz)
//	_wait(250);
//	                 
//	// CH1
//	SPI_Write(AD7734_Base2,0x29);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base2,0x08);		//enable the channel AIN1 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base2,0x31);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base2,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                
//	// CH2
//	SPI_Write(AD7734_Base2,0x2A);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base2,0x08);		//enable the channel AIN2 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base2,0x32);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base2,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                 
//	// CH3
//	SPI_Write(AD7734_Base2,0x2B);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base2,0x08);		//enable the channel AIN3 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base2,0x33);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base2,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                 
//	//I/O Port 
//	SPI_Write(AD7734_Base2,0x01);		//Write I/O Port Reg
//	SPI_Write(AD7734_Base2,0x19);		//P0 as an output,P1/SYNCn configured as SYNCn,RDYn pin go low all enabled channels have unread data 
//	_wait(250);
//
///*		
//	// ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base2,0x38);		//Write CH0 Mode Reg
//	SPI_Write(AD7734_Base2,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base2,0x39);		//Write CH1 Mode Reg
//	SPI_Write(AD7734_Base2,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base2,0x3A);		//Write CH2 Mode Reg
//	SPI_Write(AD7734_Base2,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base2,0x3B);		//Write CH3 Mode Reg
//	SPI_Write(AD7734_Base2,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//*/		
//	SPI_Write(AD7734_Base2,0x38);
//	SPI_Write(AD7734_Base2,0x32);
//	_wait(250);
//	
//// 0				
//	
//	// ����AD7734 ����ģʽ
//	AD7734_WRITE_SET(AD7734_Base,0x0);		// ����SPI����Ϊ��DMA��ʽ
//	
//	SPI_Write(AD7734_Base,0x00);			// �����λ����
//	SPI_Write(AD7734_Base,0xFF);
//	SPI_Write(AD7734_Base,0xFF);
//	SPI_Write(AD7734_Base,0xFF);
//	SPI_Write(AD7734_Base,0xFF);
//	
//	_wait(185180);		
//	
//	// CH0
//	SPI_Write(AD7734_Base,0x28);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base,0x08);		//enable the channel AIN0 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base,0x30);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base,0x86);		//chop enabled, and the update rate is 2534Hz(395us@6.144MHz)
//	_wait(250);
//	                 
//	// CH1
//	SPI_Write(AD7734_Base,0x29);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base,0x08);		//enable the channel AIN1 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//	
//	SPI_Write(AD7734_Base,0x31);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                
//	// CH2
//	SPI_Write(AD7734_Base,0x2A);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base,0x08);		//enable the channel AIN2 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base,0x32);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                 
//	// CH3
//	SPI_Write(AD7734_Base,0x2B);		//write to communication register. The next step is writing to channel setup register
//	SPI_Write(AD7734_Base,0x08);		//enable the channel AIN3 in the continuous conversion mode, the input voltage range is +/-10V
//	_wait(250);
//
//	SPI_Write(AD7734_Base,0x33);		//write to communication register. The next step is writing to channel conversion time register
//	SPI_Write(AD7734_Base,0x86);		//chop enabled, and the update rate is 2534Hz
//	_wait(250);
//	                 
//	//I/O Port 
//	SPI_Write(AD7734_Base,0x01);		//Write I/O Port Reg
//	SPI_Write(AD7734_Base,0x19);		//P0 as an output,P1/SYNCn configured as SYNCn,RDYn pin go low all enabled channels have unread data 
//	_wait(250);
///*		
//	// ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base,0x38);		//Write CH0 Mode Reg
//	SPI_Write(AD7734_Base,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base,0x39);		//Write CH1 Mode Reg
//	SPI_Write(AD7734_Base,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base,0x3A);		//Write CH2 Mode Reg
//	SPI_Write(AD7734_Base,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//	SPI_Write(AD7734_Base,0x3B);		//Write CH3 Mode Reg
//	SPI_Write(AD7734_Base,0x9B);		//24bit; ADC Zero-Scale Self-Calibration
//
//	_wait(250);
//*/
//	
//	SPI_Write(AD7734_Base,0x38);
//	SPI_Write(AD7734_Base,0x32);	
//	_wait(2500);
//	
//		
//
//	// ����SYNCn�ź�
//	OSP_Write_Width(OSP0_Base,37);		// SYNCn�źſ��2us
//	OSP_Trigger(OSP0_Base);	
//	
//	_wait(250);					
//	
//	
//	// ����ת��
//	AD7734_WRITE_SET(AD7734_Base,0x1);		// ����SPI����ΪDMA��ʽ	
//	AD7734_WRITE_SET(AD7734_Base2,0x1);		// ����SPI����ΪDMA��ʽ		
//	
//}



