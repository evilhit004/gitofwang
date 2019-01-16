/********************************************************************************/
// general.h
// �汾��		2.1.0.
// �ļ�˵����	Ӳ��ϵͳͨ��ͷ�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.09.11.
// ����ʱ�䣺	2014.09.24.
// ����˵����
// 	2014.09.24.
//		1.�����˶�дDSP��FPGA��ַ�ĺ꺯�������㲻ͬ���ݽӿڵĳ�����ֲ
/********************************************************************************/
#ifndef _GENERAL_H_
#define _GENERAL_H_

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Variable types                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */

/*

#define Uint32  unsigned int
#define Uint16  unsigned short
#define Uint8   unsigned char
#define Int32   int
#define Int16   short
#define Int8    char

*/

/* ------------------------------------------------------------------------ *
 *  ϵͳ�ú꺯������                                                        *
 *		ʹ�����º����ή�ͳ�����ֲ�ԺͿɶ��ԣ��������������Ч��
 *		����ʹ���ڶ�ʱ���ٶ���Ҫ��ĵط�
 * ------------------------------------------------------------------------ */
#define __WriteAddr32(data, AddrBase)		*((unsigned int *)(AddrBase)) = (data)		// ��32λ���Ե�ַд������
#define __ReadAddr32(AddrBase)				(*((unsigned int *)(AddrBase)))				// ��32λ���Ե�ַ��������
#define __WriteAddr16(data, AddrBase)		*((unsigned short *)(AddrBase)) = (data)	// ��16λ���Ե�ַд������
#define __ReadAddr16(AddrBase)				(*((unsigned short *)(AddrBase)))			// ��16λ���Ե�ַ��������

#define __ReadFPGA(AddrBase)				(*((unsigned short *)(AddrBase)))			// ��FPGA��ַ��������
#define __WriteFPGA(data, AddrBase)			*((unsigned short *)(AddrBase)) = (data)	// ��FPGA��ַд������
#define __ReadDSP(AddrBase)					(*((unsigned int *)(AddrBase)))			   // ��DSP��ַ��������
#define __WriteDSP(data, AddrBase)			*((unsigned int *)(AddrBase)) = (data)	   // ��DSP��ַд������
/* ------------------------------------------------------------------------ *
 *  ϵͳ�����ʱ����                                                        *
 * ------------------------------------------------------------------------ */
void _wait(unsigned int );

#endif
