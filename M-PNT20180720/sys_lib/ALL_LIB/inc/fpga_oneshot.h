/********************************************************************************/
// fpga_oneshot.h
// �汾��		1.0.0.
// �ļ�˵����	FPGA�����巢����ģ��ͷ�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.11.14.
// ����ʱ�䣺	
// ����˵����

/********************************************************************************/
#ifndef __FPGA_OSP_H
#define __FPGA_OSP_H

#define OSP_SHIFT					1

/* �����Ϣ�����òο�	*/

/* �Ĵ���ƫ�Ƶ�ַ	*/                                                        	
#define OSP_WIDTH					0  						// ��������
#define	OSP_CTR						(0x1 << OSP_SHIFT)  	// ���ƼĴ���

/* �꺯��		*/
#define OSP_Trigger(AddrBase)					*((unsigned short *)((AddrBase) + OSP_CTR)) = (0x1)
#define OSP_Write_Width(AddrBase,data)			*((unsigned short *)(AddrBase)) = (data)

#endif	/*	__FPGA_OSP_H	*/
