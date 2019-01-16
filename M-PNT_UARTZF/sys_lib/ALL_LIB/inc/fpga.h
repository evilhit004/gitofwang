/********************************************************************************/
// fpga.h
// �汾��		3.0.0.
// �ļ�˵����	FPGAϵͳͷ�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.09.11.
// ����ʱ�䣺	2014.12.24.
// ����˵����
//	2014.12.24.
//		1.������OW0_Base��OW1_Base��AD976_Base��AD7734_Base2��6·�������ַ
/********************************************************************************/
#ifndef __FPGA_PERIPH_H
#define __FPGA_PERIPH_H

/* FPGA ��ַ	*/
#define FPGA_Base   	0xA0000000 	//0x62000000	//
#define FPGA_SHIFT   	 1 						 

/* FPGA �����ַ	*/

#define UART0_Base  	(FPGA_Base + (0x40 << FPGA_SHIFT))
#define UART1_Base   	(FPGA_Base + (0x48 << FPGA_SHIFT))
#define UART2_Base   	(FPGA_Base + (0x50 << FPGA_SHIFT))
#define UART3_Base   	(FPGA_Base + (0x58 << FPGA_SHIFT))
#define UART4_Base   	(FPGA_Base + (0x60 << FPGA_SHIFT))
#define UART5_Base   	(FPGA_Base + (0x68 << FPGA_SHIFT))
#define UART6_Base   	(FPGA_Base + (0x70 << FPGA_SHIFT))
#define GPS_Base   	 	(FPGA_Base + (0x78 << FPGA_SHIFT))		//GPS����(ר��)

#define SPI0_Base   	(FPGA_Base + (0x80 << FPGA_SHIFT))
#define SPI1_Base   	(FPGA_Base + (0x88 << FPGA_SHIFT))
#define SPI2_Base   	(FPGA_Base + (0x90 << FPGA_SHIFT))
#define SPI3_Base   	(FPGA_Base + (0x98 << FPGA_SHIFT))

#define OW0_Base   		(FPGA_Base + (0xE8 << FPGA_SHIFT))		//OneWire����1
#define OW1_Base   		(FPGA_Base + (0xF0 << FPGA_SHIFT))

#define CNT_S0_Base		(FPGA_Base + (0xF8 << FPGA_SHIFT))		//��ʱ�жϼ�����(ר��)
#define CNT_S1_Base		(FPGA_Base + (0xF9 << FPGA_SHIFT))		

#define ADS1278_Base	(FPGA_Base + (0x100 << FPGA_SHIFT))
                        
#define TIMER0_Base		(FPGA_Base + (0x160 << FPGA_SHIFT))		//PCͨѶ���ڶ�ʱ��(ר��)
#define TIMER1_Base		(FPGA_Base + (0x164 << FPGA_SHIFT))
#define TIMER2_Base		(FPGA_Base + (0x168 << FPGA_SHIFT))

#define AD7734_Base		(FPGA_Base + (0x180 << FPGA_SHIFT))

#define GPIO0_Base		(FPGA_Base + (0x190 << FPGA_SHIFT))
#define GPIO1_Base		(FPGA_Base + (0x192 << FPGA_SHIFT))
#define GPIO2_Base		(FPGA_Base + (0x194 << FPGA_SHIFT))
#define GPIO3_Base		(FPGA_Base + (0x196 << FPGA_SHIFT))

#define PECULIAR_Base	(FPGA_Base + (0x198 << FPGA_SHIFT))   	//24��16λ���ԼĴ���

#define SYSREG_Base		(FPGA_Base + (0x1C0 << FPGA_SHIFT))		//32��FPGAϵͳ�Ĵ���

#define OSP0_Base		(FPGA_Base + (0x1E0 << FPGA_SHIFT))		//�����巢����(������)

#define MOTOR0_Base		(FPGA_Base + (0x1F0 << FPGA_SHIFT))		//�������0
#define MOTOR1_Base		(FPGA_Base + (0x1F8 << FPGA_SHIFT))		//�������1
#define MOTOR2_Base		(FPGA_Base + (0x200 << FPGA_SHIFT))		//�������2
#define MOTOR3_Base		(FPGA_Base + (0x208 << FPGA_SHIFT))		//�������3
#define MOTOR4_Base		(FPGA_Base + (0x210 << FPGA_SHIFT))		//�������4
#define MOTOR5_Base		(FPGA_Base + (0x218 << FPGA_SHIFT))		//�������5

#define AD976_Base		(FPGA_Base + (0x220 << FPGA_SHIFT))		//AD976ģ��

#define AD7734_Base2	(FPGA_Base + (0x230 << FPGA_SHIFT))		//��2·AD7734

#define INTR0_Base		(FPGA_Base + (0x240 << FPGA_SHIFT))		//intr0


/* ��������	*/	

/* �û�����ԭ�Ͷ���	*/


#endif	/*	__FPGA_PERIPH_H	*/



