/********************************************************************************/
// timeout.c
// �汾��		2.1.0.
// �ļ�˵����	��ʱ�жϻ����ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.09.11.
// ����ʱ�䣺	
// ����˵����
/********************************************************************************/
#include "timeout.h"
#include "fpga_timer.h"
#include "fpga.h"
#include "fpga_uart.h"

/*-------------------------------------------------------------------
-- ��timeout�Ĵ��ڲ�ѯ���պ���
-- RxDataPtr:		�����յ������ݵ�ַָ��
-- timer_addrbase:	��ʱ������ַ
-- com_addrbase��	���ڻ���ַ
-- ����ֵ��			timeout��־��0��ʾ������1��ʾ��ʱ
-------------------------------------------------------------------*/
int	tUART_Recv(unsigned char *RxDataPtr, unsigned int com_addrbase)
{
	unsigned short time	= 0xFFFF;
	unsigned char status = 0;
	
	TIMER_Write(TIMEOUT_UART,TIMER0_Base);

	status = UART_LSR_Read(com_addrbase);

	while((status &	0x1) !=	0x1)
	{				
		time = TIMER_Read(TIMER0_Base);
		
		if(time	== 0)
		{
			return(1);
		}

		status = UART_LSR_Read(com_addrbase);
	}
	
	*RxDataPtr = UART_Recv(com_addrbase);
	return(0);		
}






