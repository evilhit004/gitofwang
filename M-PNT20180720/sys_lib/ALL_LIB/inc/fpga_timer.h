/********************************************************************************/
// fpga_timer.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA��ʱ��ģ��ͷ�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.08.08.
// ����ʱ�䣺	2015.01.29.	
// ����˵����
// 	2015.01.29.
//		1.������intrģ����ش���
/********************************************************************************/
#ifndef __FPGA_TIMER_H
#define __FPGA_TIMER_H

#define TIMER_SHIFT					1

/* �����Ϣ�����òο�	*/

/* �Ĵ���ƫ�Ƶ�ַ	*/                                                        	
#define	TIMER_CNT					0x0  					// ��ʱֵ�Ĵ���
#define	TIMER_RDR					(0x2 << TIMER_SHIFT)  	// ��ʱʱ�ӷ�Ƶ�Ĵ���

/* �꺯��		*/
#define TIMER_Read(AddrBase)				(*((unsigned short *)(AddrBase)))
#define TIMER_Write(data,AddrBase)			*((unsigned short *)((AddrBase) + TIMER_CNT)) = (data)
#define TIMER_CLKSet(data,AddrBase)			*((unsigned short *)((AddrBase) + TIMER_RDR)) = (data)

// intrģ�����

#define TIMER_EN_CMD				0x55
#define TIMER_DIS_CMD				0x0

/* �Ĵ���ƫ�Ƶ�ַ	*/                                                        	
#define	TIMER_FDVL					0x0  					// ��Ƶϵ����16λ�Ĵ���
#define	TIMER_FDVH					(0x1 << TIMER_SHIFT)  	// ��Ƶϵ����16λ�Ĵ���
#define	TIMER_CTR					(0x2 << TIMER_SHIFT)  	// ��ʱ��ʹ�ܼĴ���
#define	TIMER_WDR					(0x3 << TIMER_SHIFT)  	// ���Ź�ʹ�ܼĴ���

/* �꺯��		*/
#define TIMER_WriteL(AddrBase,data)			*((unsigned short *)((AddrBase) + TIMER_FDVL)) = (data)
#define TIMER_WriteH(AddrBase,data)			*((unsigned short *)((AddrBase) + TIMER_FDVH)) = (data)
#define TIMER_EN(AddrBase,data)				*((unsigned short *)((AddrBase) + TIMER_CTR)) = (data)
#define TIMER_DOG(AddrBase,data)			*((unsigned short *)((AddrBase) + TIMER_WDR)) = (data)

#endif	/*	__FPGA_TIMER_H	*/
