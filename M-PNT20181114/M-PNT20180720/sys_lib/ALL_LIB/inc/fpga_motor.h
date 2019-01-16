/********************************************************************************/
// fpga_motor.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA motorģ��ͷ�ļ�
// ��д�ߣ�		ChePengyu
// ��дʱ�䣺	2014.12.25.
// ����ʱ�䣺	
// ����˵����
/********************************************************************************/
#ifndef __FPGA_MOTOR_H
#define __FPGA_MOTOR_H

#define MOTOR_SHIFT			1

/* ��زο�����	*/
#define MOTOR_DIR_POS		1
#define MOTOR_DIR_NAG		0

/* �Ĵ���ƫ�Ƶ�ַ	*/
#define MOTOR_DIR			(0x1 << MOTOR_SHIFT)		// ��﷽�����üĴ���			д
#define MOTOR_FDIVL			(0x2 << MOTOR_SHIFT)  		// �����Ƶϵ����16λ�Ĵ���    	д
#define MOTOR_FDIVH			(0x3 << MOTOR_SHIFT)  		// �����Ƶϵ����16λ�Ĵ���     д
#define MOTOR_PNUM			(0x4 << MOTOR_SHIFT)  		// ��������Ĵ���				д

/* �꺯��		*/
#define MOTOR_WRITE_FDIVL(AddrBase,data)			*((unsigned short *)((AddrBase) + MOTOR_FDIVL)) = (data)	
#define MOTOR_WRITE_FDIVH(AddrBase,data)			*((unsigned short *)((AddrBase) + MOTOR_FDIVH)) = (data)		
#define MOTOR_WRITE_PNUM(AddrBase,data)				*((unsigned short *)((AddrBase) + MOTOR_PNUM)) = (data)			
#define MOTOR_WRITE_DIR(AddrBase,data)				*((unsigned short *)((AddrBase) + MOTOR_DIR)) = (data)		

/* ����ԭ������		*/
void Motor_Ctrl(unsigned int AddrBase, unsigned int Pulse_Freq, unsigned short Pulse_Num, unsigned char Pulse_Dir);


#endif	/*	__FPGA_MOTOR_H	*/
