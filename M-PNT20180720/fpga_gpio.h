/********************************************************************************/
// fpga_gpio.h
// �汾��		2.1.0.
// �ļ�˵����	FPGA GPIOģ��ͷ�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.10.14.
// ����ʱ�䣺	
// ����˵����
// 
/********************************************************************************/
#ifndef __FPGA_GPIO_H
#define __FPGA_GPIO_H

#define GPIO_SHIFT					1

/* ���������Ϣ	*/
#define GPIO_DIR_IN					0
#define GPIO_DIR_OUT				1


/* �Ĵ���ƫ�Ƶ�ַ	*/
#define A_GPIO_DATA					0						// ����������ݼĴ���		��/д
#define A_GPIO_DIR					(0x1 << GPIO_SHIFT)  	// ����Ĵ���		    	��/д

/* �꺯��		*/
#define GPIO_Write(AddrBase,data)				*((unsigned short *)(AddrBase)) = (data)
#define GPIO_Read(AddrBase)						(*((unsigned short *)(AddrBase)))
#define GPIO_Dir(AddrBase,data)					*((unsigned short *)((AddrBase) + A_GPIO_DIR)) = (data)


#endif	/*	__FPGA_GPIO_H	*/
