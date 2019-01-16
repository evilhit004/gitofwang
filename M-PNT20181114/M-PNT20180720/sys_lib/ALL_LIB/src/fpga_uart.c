/********************************************************************************/
// FPGA_UART.c
// �汾��		2.1.0.
// �ļ�˵����	FPGA����ģ��c�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.08.25.
// ����ʱ�䣺	2014.09.11.
// ����˵����
//	2014.09.10.
//		1.��pUART_Txmit������Ϊ��ѯ��ʽ���ڵ��ֽڷ��ͺ���
//		2.��ѯ��ʽ���ڵ��ֽڷ��ͺ���(FIFOģʽ)����ΪpfUART_Txmit
//		3.�����˲�ѯ��ʽ���ڷ��ͺ���(FIFOģʽ)pfUART_Trans�����Է���������
//	2014.09.11.
//		1.�����˴��ڳ�ʼ������UART_init
//	2014.12.26.
//		1.�޸��˲���ע��
/********************************************************************************/
#include "fpga_uart.h"

/*-------------------------------------------------------------------
-- ���ڳ�ʼ������
-- com_addrbase��	���ڻ���ַ
-- v_baud:			16x������ʱ�ӷ�Ƶϵ��(f16xclk = fmclk/v_baud)
-- v_LCR��			��������λ��ֹͣλ��У��λ
-- v_IER��			���ô����ж�ʹ��
-- v_fifo��			����FIFO�����ж�ˮƽ(0-�ر�FIFO������-�����ж��ֽ���)
-- ����ֵ��			��
-------------------------------------------------------------------*/
void UART_init(unsigned int com_addrbase, unsigned char v_baud, unsigned char v_LCR, unsigned char v_IER, unsigned char v_fifo)
{
	UART_Baud_Set(v_baud, com_addrbase);			// ���ò�����Ϊ 18.432MHz/10 = 1.8432MHz = 115.2Kbps x 16
	UART_LCR_Set(v_LCR, com_addrbase);				// 8λ����λ��1λֹͣλ����У��λ
	UART_IER_Set(v_IER, com_addrbase);				// ����·�жϺͽ����ж�
	
	if(v_fifo == 0)
	{
		UART_FIFO_Disable(com_addrbase);			// �ر�FIFO
	}
	else
	{
		UART_FIFO_Enable(com_addrbase);				// ��FIFO
		UART_FIFO_Level_Set(v_fifo,com_addrbase);	// ���ý���FIFOΪ1�ֽڴ����ж�
	}
}
/*-------------------------------------------------------------------
-- ���ڽ���FIFO���պ���
-- ����FIFO����
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			FIFO����
-------------------------------------------------------------------*/
unsigned char UART_FIFOCLR(unsigned int com_addrbase)
{
	unsigned char status = 0;
	unsigned char data = 0;
	
	status = UART_LSR_Read(com_addrbase);
	while((status &	0x1) ==	0x1)		// FIFO��������
	{
		data = UART_Recv(com_addrbase);
		status = UART_LSR_Read(com_addrbase);
	}
	
	return(data);
}
/*-------------------------------------------------------------------
-- ��ѯ��ʽ���ڵ��ֽڷ��ͺ���
-- ��ѯ����THR�Ĵ���״̬��Ϊ�վ�д�����ݣ������������ѯ
-- TxData:			��������
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			0��ʾ����
-------------------------------------------------------------------*/
int pUART_Txmit(unsigned char TxData, unsigned int com_addrbase)
{
	unsigned char status = 0;
	
	status = UART_LSR_Read(com_addrbase);
	
	while((status & 0x40) != 0x40)	// THR��
	{
		status = UART_LSR_Read(com_addrbase);
	}
	
	UART_Txmit(TxData, com_addrbase);
	
	return(0);
}

/*-------------------------------------------------------------------
-- ��ѯ��ʽ���ڷ��ͺ���(FIFOģʽ)
-- ��ѯ���ڷ���FIFO������־��������д�����ݣ����������ѯ
-- ptrTx_Buff:		��������ĵ�ַָ��
-- num��			�������ݵĸ���
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			0��ʾ����
-------------------------------------------------------------------*/
int pfUART_Trans(const unsigned char *ptrTx_Buff, int num, unsigned int com_addrbase)
{
	unsigned char status = 0;
	int i = 0;
	
	for(i=0;i<num;i++)
	{
	
		status = UART_LSR_Read(com_addrbase);
	
		while((status & 0x80) == 0x80)	// FIFO��
		{
			status = UART_LSR_Read(com_addrbase);
		}
	
		UART_Txmit(*(ptrTx_Buff + i), com_addrbase);
	}
	
	return(0);
}

/*-------------------------------------------------------------------
-- ��ѯ��ʽ���ڵ��ֽڷ��ͺ���(FIFOģʽ)
-- ��ѯ���ڷ���FIFO������־��������д�����ݣ����������ѯ
-- TxData:			��������
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			0��ʾ����
-------------------------------------------------------------------*/
int pfUART_Txmit(unsigned char TxData, unsigned int com_addrbase)
{
	unsigned char status = 0;
	
	status = UART_LSR_Read(com_addrbase);
	
	while((status & 0x80) == 0x80)	// FIFO��
	{
		status = UART_LSR_Read(com_addrbase);
	}
	
	UART_Txmit(TxData, com_addrbase);
	
	return(0);
}

/*-------------------------------------------------------------------
-- ���ڲ�ѯ���պ���
-- ��ѯ�����Ƿ���δ�������ݣ��������������˳����������������ѯ��־λ
-- RxDataPtr:		�����յ������ݵ�ַָ��
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			��
-------------------------------------------------------------------*/
void pUART_Recv(unsigned char *RxDataPtr, unsigned int com_addrbase)
{
	unsigned char status = 0;

	status = UART_LSR_Read(com_addrbase);

	while((status &	0x1) !=	0x1)			// ���FIFO�ջ���RBR����������
	{				
		status = UART_LSR_Read(com_addrbase);
	}
	
	*RxDataPtr = UART_Recv(com_addrbase);		
}







