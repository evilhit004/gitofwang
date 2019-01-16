/********************************************************************************/
// FPGA_UART.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA����ģ��ͷ�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.03.21.
// ����ʱ�䣺	2014.12.26.
// ����˵����
//	2014.08.26.
//		1.�����˷�ֹ�ظ����������
//		2.������UART_FIFOCLR��pUART_Txmit��pUART_Recv��������������
//	2014.09.10.                                                           
//		1.��pUART_Txmit������Ϊ��ѯ��ʽ���ڵ��ֽڷ��ͺ���                 
//		2.��ѯ��ʽ���ڵ��ֽڷ��ͺ���(FIFOģʽ)����ΪpfUART_Txmit          
//		3.�����˲�ѯ��ʽ���ڷ��ͺ���(FIFOģʽ)pfUART_Trans�����Է���������
//	2014.09.11.
//		1.�����˴��ڳ�ʼ������UART_init
//	2014.12.26.
//		1.�����˺���ԭ�͵�ע��
/********************************************************************************/
#ifndef __FPGA_UART_H
#define __FPGA_UART_H

#include "data_type.h"

#define UART_SHIFT					1

/* �����Ϣ�����òο�	*/
#define UART_FIFO_SIZE				256						// ����FIFO��С                                                       	
#define MASK_UART_TXDONE			0x40					// ���ͼĴ�����
#define MASK_UART_RXRDY				0x1						// ���ռĴ���������
#define	UART_LCR_CMD1				0x3						// 8λ����λ��1λֹͣλ����У��λ
#define	UART_IER_CMD1				0x1						// �������ж�

/* �Ĵ���ƫ�Ƶ�ַ	*/                                                        	
#define	UART_RBR					0x0  					// ���ջ���Ĵ���
#define	UART_THR					0x0  					// ���ͻ���Ĵ���
#define	UART_IER					(0x1 << UART_SHIFT) 	// �ж�ʹ�ܼĴ���
#define	UART_IIR					(0x2 << UART_SHIFT) 	// �ж�״̬�Ĵ���
#define	UART_FCR					(0x2 << UART_SHIFT) 	// FIFO���ƼĴ���
#define	UART_LCR					(0x3 << UART_SHIFT) 	// ��·���ƼĴ���
#define	UART_FRBNR					(0x4 << UART_SHIFT) 	// ����FIFOʣ���ֽ����Ĵ���
#define	UART_MCR					(0x4 << UART_SHIFT) 	// Modem���ƼĴ���
#define	UART_LSR					(0x5 << UART_SHIFT) 	// ��·״̬�Ĵ���
#define	UART_BDR					(0x5 << UART_SHIFT) 	// ������16��ʱ�ӷ�Ƶϵ���Ĵ���
#define	UART_MSR					(0x6 << UART_SHIFT) 	// Modem״̬�Ĵ���
#define	UART_FTR					(0x6 << UART_SHIFT) 	// ����FIFO�жϴ����ֽ����Ĵ���

/* �꺯��		*/
#define UART_Txmit(data,AddrBase)				*((unsigned short *)(AddrBase)) = (data)
#define UART_Recv(AddrBase)						(*((unsigned short *)(AddrBase)))
#define UART_IER_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + UART_IER)) = (data)	
#define UART_IIR_Read(AddrBase)					(*((unsigned short *)((AddrBase) + UART_IIR)))
#define UART_FIFO_Enable(AddrBase)				*((unsigned short *)((AddrBase) + UART_FCR)) = 0x1
#define UART_FIFO_Disable(AddrBase)				*((unsigned short *)((AddrBase) + UART_FCR)) = 0x0
#define UART_FIFO_Level_Set(data,AddrBase)		*((unsigned short *)((AddrBase) + UART_FTR)) = (data)
#define UART_FIFO_Remain_Read(AddrBase)			(*((unsigned short *)((AddrBase) + UART_FRBNR)))
#define UART_LCR_Set(data,AddrBase)				*((unsigned short *)((AddrBase) + UART_LCR)) = (data)
#define UART_LSR_Read(AddrBase)					(*((unsigned short *)((AddrBase) + UART_LSR)))
#define UART_Baud_Set(data,AddrBase)			*((unsigned short *)((AddrBase) + UART_BDR)) = (data)

/*	����ԭ��	*/

/*-------------------------------------------------------------------
-- ���ڳ�ʼ������
-- com_addrbase��	���ڻ���ַ
-- v_baud:			16x������ʱ�ӷ�Ƶϵ��(f16xclk = fmclk/v_baud)
-- v_LCR��			��������λ��ֹͣλ��У��λ
-- v_IER��			���ô����ж�ʹ��
-- v_fifo��			����FIFO�����ж�ˮƽ(0-�ر�FIFO������-�����ж��ֽ���)
-- ����ֵ��			��
-------------------------------------------------------------------*/
void UART_init(uint32_t com_addrbase, uint8_t v_baud, uint8_t v_LCR, uint8_t v_IER, uint8_t v_fifo);

/*-------------------------------------------------------------------
-- ���ڽ���FIFO���պ���
-- ����FIFO����
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			FIFO����
-------------------------------------------------------------------*/
uint8_t UART_FIFOCLR(uint32_t com_addrbase);

/*-------------------------------------------------------------------
-- ��ѯ��ʽ���ڵ��ֽڷ��ͺ���
-- ��ѯ����THR�Ĵ���״̬��Ϊ�վ�д�����ݣ������������ѯ
-- TxData:			��������
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			0��ʾ����
-------------------------------------------------------------------*/
int pUART_Txmit(uint8_t TxData, uint32_t com_addrbase);

/*-------------------------------------------------------------------
-- ��ѯ��ʽ���ڷ��ͺ���(FIFOģʽ)
-- ��ѯ���ڷ���FIFO������־��������д�����ݣ����������ѯ
-- ptrTx_Buff:		��������ĵ�ַָ��
-- num��			�������ݵĸ���
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			0��ʾ����
-------------------------------------------------------------------*/
int pfUART_Trans(const uint8_t *ptrTx_Buff, int16_t num, uint32_t com_addrbase);

/*-------------------------------------------------------------------
-- ��ѯ��ʽ���ڵ��ֽڷ��ͺ���(FIFOģʽ)
-- ��ѯ���ڷ���FIFO������־��������д�����ݣ����������ѯ
-- TxData:			��������
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			0��ʾ����
-------------------------------------------------------------------*/
int pfUART_Txmit(uint8_t TxData, uint32_t com_addrbase);

/*-------------------------------------------------------------------
-- ���ڲ�ѯ���պ���
-- ��ѯ�����Ƿ���δ�������ݣ��������������˳����������������ѯ��־λ
-- RxDataPtr:		�����յ������ݵ�ַָ��
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			��
-------------------------------------------------------------------*/
void pUART_Recv(uint8_t *RxDataPtr, uint32_t com_addrbase);


#endif	/*	__FPGA_UART_H	*/
