/********************************************************************************/
// fpga_motor.c
// �汾��		2.1.0.
// �ļ�˵����	FPGA�������ģ��c�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.12.25.
// ����ʱ�䣺	
// ����˵����
/********************************************************************************/
#include "fpga_motor.h"

/*-------------------------------------------------------------------
-- �����ƺ���
-- ����FIFO����
-- AddrBase:		����ַ
-- Pulse_Freq:		����Ƶ�ʷ�Ƶϵ��
-- Pulse_Num:		�������
-- Pulse_Dir:		����
-- ����ֵ��			��
-------------------------------------------------------------------*/
void Motor_Ctrl(unsigned int AddrBase, unsigned int Pulse_Freq, unsigned short Pulse_Num, unsigned char Pulse_Dir)
{
	unsigned short Pulse_FreqL = 0;
	unsigned short Pulse_FreqH = 0;
	
	Pulse_FreqL = Pulse_Freq & 0xFFFF;
	Pulse_FreqH = (Pulse_Freq >> 16) & 0xFFFF;
	
	MOTOR_WRITE_FDIVL(AddrBase,Pulse_FreqL);		// д��������Ƶϵ����16λ 
	MOTOR_WRITE_FDIVH(AddrBase,Pulse_FreqH);		// д��������Ƶϵ����16λ 
	MOTOR_WRITE_PNUM(AddrBase,Pulse_Num);			// д����������               
	MOTOR_WRITE_DIR(AddrBase,Pulse_Dir);			// д������巽��               								
}



