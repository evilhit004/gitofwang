/********************************************************************************/
// FPGA_Counter.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA������ģ��ͷ�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.03.25.
// ����ʱ�䣺	2014.10.14.
// ����˵����
// 2014.10.14:
//	1.Counter_Read�������������м�����ģ�飻
//	  Counter_Read_Chn���������ڶ�·������ģ��
/********************************************************************************/
#ifndef __FPGA_COUNT_H
#define __FPGA_COUNT_H

#define COUNT_SHIFT					1

/* �����Ϣ�����òο�	*/
#define	COUNT_CHNNUM				14						// ������ͨ����
#define	COUNT_TEST_ENABLE			0x55AA					// ʹ�ܲ���ģʽKEYֵ

/* �Ĵ���ƫ�Ƶ�ַ	*/
#define A_CNT_TEST					(0xC << COUNT_SHIFT)	// ��������������Ĵ���
#define A_CNT_FRAME					(0xD << COUNT_SHIFT)	// ֡�����Ĵ���
#define A_CNT_KEY1					(0xE << COUNT_SHIFT)	// KEY1�Ĵ���(����)
#define A_CNT_KEY2					(0xF << COUNT_SHIFT)	// KEY2�Ĵ���

/* �꺯��		*/
#define Counter_Read(AddrBase)					(*((unsigned short *)(AddrBase)))
#define Counter_Read_Chn(AddrBase,chn)			*((unsigned short *)((AddrBase) + ((chn) << COUNT_SHIFT)))
#define Counter_Write_Key2(AddrBase,data)		*((unsigned short *)((AddrBase) + A_CNT_KEY2)) = (data)
#define Counter_Read_Key2(AddrBase)				(*((unsigned short *)((AddrBase) + A_CNT_KEY2)))

#endif	/*	__FPGA_COUNT_H	*/
