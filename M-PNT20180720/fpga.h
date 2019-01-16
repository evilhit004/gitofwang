/*******************************************************************************
// fpga.h
// �汾��		2017.05.03
// �ļ�˵����	C4 DCU FPGAϵͳͷ�ļ�
// ��д�ߣ�		������
// ��дʱ�䣺	2014.09.11.
// ����ʱ�䣺	2017.09.23.
// ����˵����

*******************************************************************************/
#ifndef __FPGA_PERIPH_H
#define __FPGA_PERIPH_H


/* FPGA ��ַ	*/
#define FPGA_Base   		0x62000000	// C6747 CE3	
#define FPGA_SHIFT   	 	1			 						 

/* FPGA �����ַ	*/

#define UART0_Base   		(FPGA_Base + (0x40 << FPGA_SHIFT))      //
#define UART1_Base   		(FPGA_Base + (0x48 << FPGA_SHIFT))      //
#define UART2_Base   		(FPGA_Base + (0x50 << FPGA_SHIFT))      //
#define UART3_Base   		(FPGA_Base + (0x58 << FPGA_SHIFT))      //
//#define UART4_Base   		(FPGA_Base + (0x60 << FPGA_SHIFT))      //
//#define UART5_Base   		(FPGA_Base + (0x68 << FPGA_SHIFT))      //
//#define UART6_Base   		(FPGA_Base + (0x70 << FPGA_SHIFT))      //
//#define UART7_Base   	 	(FPGA_Base + (0x78 << FPGA_SHIFT))		//

//#define SPI0_Base   		(FPGA_Base + (0x80 << FPGA_SHIFT))
//#define SPI1_Base   		(FPGA_Base + (0x88 << FPGA_SHIFT))
//#define SPI2_Base   		(FPGA_Base + (0x90 << FPGA_SHIFT))
//#define SPI3_Base   		(FPGA_Base + (0x98 << FPGA_SHIFT))

//#define Cymometer0_Base   	(FPGA_Base + (0xB0 << FPGA_SHIFT))      //���ŵ��U�����
//#define Cymometer1_Base   	(FPGA_Base + (0xB4 << FPGA_SHIFT))		//���ŵ��V�����
//#define Cymometer2_Base   	(FPGA_Base + (0xB8 << FPGA_SHIFT))		//���ŵ��U�����
//#define Cymometer3_Base   	(FPGA_Base + (0xBC << FPGA_SHIFT))		//���ŵ��V�����
//#define Cymometer4_Base   	(FPGA_Base + (0xC0 << FPGA_SHIFT))
//#define Cymometer5_Base   	(FPGA_Base + (0xC4 << FPGA_SHIFT))
//#define Cymometer6_Base   	(FPGA_Base + (0xC8 << FPGA_SHIFT))
//#define Cymometer7_Base   	(FPGA_Base + (0xCC << FPGA_SHIFT))

//#define OW0_Base   			(FPGA_Base + (0xE8 << FPGA_SHIFT))		
//#define OW1_Base   			(FPGA_Base + (0xF0 << FPGA_SHIFT))      

#define CNT_S0_Base			(FPGA_Base + (0xF8 << FPGA_SHIFT))		//��ʱ�жϼ�����(ר��)
//#define CNT_S1_Base			(FPGA_Base + (0xF9 << FPGA_SHIFT))		
//
//#define ADS1278_Base		(FPGA_Base + (0x100 << FPGA_SHIFT))
                        
#define TIMER0_Base			(FPGA_Base + (0x160 << FPGA_SHIFT))		//PCͨѶ���ڶ�ʱ��(ר��)
//#define TIMER1_Base			(FPGA_Base + (0x164 << FPGA_SHIFT))		//���Ź� 
//#define TIMER2_Base			(FPGA_Base + (0x168 << FPGA_SHIFT))
//
//#define AD7734_Base			(FPGA_Base + (0x180 << FPGA_SHIFT))
//
//#define GPIO0_Base			(FPGA_Base + (0x190 << FPGA_SHIFT))		
//#define GPIO1_Base			(FPGA_Base + (0x192 << FPGA_SHIFT))		
//#define GPIO2_Base			(FPGA_Base + (0x194 << FPGA_SHIFT))
//#define GPIO3_Base			(FPGA_Base + (0x196 << FPGA_SHIFT))

#define PECULIAR_Base		(FPGA_Base + (0x198 << FPGA_SHIFT))   	//24��16λ���ԼĴ���

//#define OW2_Base   			(FPGA_Base + (0x1B0 << FPGA_SHIFT))     
//#define OW3_Base   			(FPGA_Base + (0x1B8 << FPGA_SHIFT))     

#define SYSREG_Base			(FPGA_Base + (0x1C0 << FPGA_SHIFT))		//32��FPGAϵͳ�Ĵ���

//#define OSP0_Base			(FPGA_Base + (0x1E0 << FPGA_SHIFT))		//�����巢����(������)

//#define MOTOR0_Base			(FPGA_Base + (0x1F0 << FPGA_SHIFT))		//�������0
//#define MOTOR1_Base			(FPGA_Base + (0x1F8 << FPGA_SHIFT))		//�������1
//#define MOTOR2_Base			(FPGA_Base + (0x200 << FPGA_SHIFT))		//�������2
//#define MOTOR3_Base			(FPGA_Base + (0x208 << FPGA_SHIFT))		//�������3
//#define MOTOR4_Base			(FPGA_Base + (0x210 << FPGA_SHIFT))		//�������4
//#define MOTOR5_Base			(FPGA_Base + (0x218 << FPGA_SHIFT))		//�������5

//#define AD976_Base			(FPGA_Base + (0x220 << FPGA_SHIFT))		//AD976ģ��
//
//#define AD7734_Base2		(FPGA_Base + (0x230 << FPGA_SHIFT))		//��2·AD7734

//#define INTR0_Base			(FPGA_Base + (0x240 << FPGA_SHIFT))		//intr0     DOI0
//#define INTR1_Base			(FPGA_Base + (0x248 << FPGA_SHIFT))		//intr1     DOI1
//#define INTR2_Base			(FPGA_Base + (0x250 << FPGA_SHIFT))		//intr2     DOI2
//#define INTR3_Base			(FPGA_Base + (0x258 << FPGA_SHIFT))		//intr3     DOI3
//#define INTR4_Base			(FPGA_Base + (0x260 << FPGA_SHIFT))		//intr4     
//#define INTR5_Base			(FPGA_Base + (0x268 << FPGA_SHIFT))		//intr5    	LCB�˲���ʱ�� 
#define INTR6_Base			(FPGA_Base + (0x270 << FPGA_SHIFT))		//intr6     1ms��ʱ�ж϶�ʱ��
//#define INTR7_Base			(FPGA_Base + (0x278 << FPGA_SHIFT))		//intr7     ���ź��˲�ʱ�Ӷ�ʱ��

//#define PWM0_Base   		(FPGA_Base + (0x280 << FPGA_SHIFT))      //PWM0		������
//#define PWM1_Base   		(FPGA_Base + (0x288 << FPGA_SHIFT))      //PWM1		���÷�����
//#define PWM2_Base   		(FPGA_Base + (0x290 << FPGA_SHIFT))      //PWM2		��/���ŵ��������
//#define PWM3_Base   		(FPGA_Base + (0x298 << FPGA_SHIFT))      //PWM3
//#define PWM4_Base   		(FPGA_Base + (0x2A0 << FPGA_SHIFT))      //PWM4
//#define PWM5_Base   		(FPGA_Base + (0x2A8 << FPGA_SHIFT))      //PWM5

//#define AD7608_Base   		(FPGA_Base + (0x2E0 << FPGA_SHIFT))      //AD7608 �װ�
//#define AD7606_Base   		(FPGA_Base + (0x2F0 << FPGA_SHIFT))      //AD7606 ����


//#define ARAM_Base   	    (FPGA_Base + (0x380 << FPGA_SHIFT))      //RAM
//#define DRAM_Base   	    (FPGA_Base + (0x380 << FPGA_SHIFT))      //RAM





/* ��������	*/	

/* �û�����ԭ�Ͷ���	*/


#endif	/*	__FPGA_PERIPH_H	*/



