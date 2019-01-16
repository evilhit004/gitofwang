/* ------------------------------------------------------------------------ *
 *  OMAPL137.C	TrailBlazer					                                *
 *  ��дʱ�䣺	2014.05.20.                                                 *
 *  ����ʱ�䣺																*
 *	��д�ˣ�	chepengyu                                  					*
 *  ����˵����                                                              *
 *                                                                          *
 * ------------------------------------------------------------------------ */
#include "omapl137.h"
#include "pinmux.h"
#include "omapl137_gpio.h"
#include "omapl137_psc.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _wait( delay )                                               			*
 *                                                                          *
 *      Wait in a software loop for 'x' delay                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void _wait( Uint32 delay )
{
    volatile Uint32 i = delay;

    while(i--);
}
///* ------------------------------------------------------------------------ *
// *                                                                          *
// *  EVMOMAPL137_waitusec( usec )                                            *
// *                                                                          *
// *      Wait in a software loop for 'x' microseconds                        *
// *                                                                          *
// * ------------------------------------------------------------------------ */
//void EVMOMAPL137_waitusec( Uint32 usec )
//{
//    _wait( usec * 3 );
//}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  OMAPL137_GPIO_init( )                                                   *
 *                                                                          *
 *      GPIO��ʼ��                            								*
 *      *����ʹ��__omapl137_gpio.h�ṩ�ĺ���                                *
 * ------------------------------------------------------------------------ */
void OMAPL137_GPIO_init()
{
/*	  
	__WriteAddr(0xFFFFFCEA,0x01E26038);				
	__WriteAddr(0x0,0x01E2603C);					
	__WriteAddr(0xFFFFFFFF,0x01E26060);	
	__WriteAddr(0x1,0x62000240);			
	__WriteAddr(0xFFFFFD1D,0x01E26088);	
	__WriteAddr(0x2C0,0x01E2608C);//RESET AD AND SET OVERSAMPLE	
*/	
	EVMOMAPL137_GPIO_setDirection(0x5F,0); 	//���������Ӧ��GPIOΪ���ģʽ
    EVMOMAPL137_GPIO_setDirection(0x71,1); 	//����INT0��Ӧ��GPIOΪ����ģʽ
    EVMOMAPL137_GPIO_setDirection(0x41,1); 	//����INT1��Ӧ��GPIOΪ����ģʽ
    EVMOMAPL137_GPIO_setDirection(0x2A,1); 	//����INT2��Ӧ��GPIOΪ����ģʽ
    EVMOMAPL137_GPIO_setDirection(0x3F,1); 	//����INT3��Ӧ��GPIOΪ����ģʽ
      
    EVMOMAPL137_GPIO_setDirection(0x75,0); 	    //����GYROZ��Ӧ��GPIOΪ���ģʽ
	EVMOMAPL137_GPIO_setDirection(0x74,0); 	    //����GYROY���Ӧ��GPIOΪ���ģʽ
	EVMOMAPL137_GPIO_setDirection(0x73,0); 	    //����GYROX��Ӧ��GPIOΪ���ģʽ
	EVMOMAPL137_GPIO_setDirection(0x78,0); 	    //����ACCZ��Ӧ��GPIOΪ���ģʽ
	EVMOMAPL137_GPIO_setDirection(0x77,0); 	    //����ACCY���Ӧ��GPIOΪ���ģʽ
	EVMOMAPL137_GPIO_setDirection(0x76,0); 	    //����ACCX��Ӧ��GPIOΪ���ģʽ

	EVMOMAPL137_GPIO_setOutput(0x75,1);         //����GPIOΪ1 ��SPICS��Ч
	EVMOMAPL137_GPIO_setOutput(0x74,1);         //����GPIOΪ1 
	EVMOMAPL137_GPIO_setOutput(0x73,1);         //����GPIOΪ1 
	EVMOMAPL137_GPIO_setOutput(0x78,1);         //����GPIOΪ1 
	EVMOMAPL137_GPIO_setOutput(0x77,1);         //����GPIOΪ1 
	EVMOMAPL137_GPIO_setOutput(0x76,1);         //����GPIOΪ1 
			
}	
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  OMAPL137_SYSCFG_init( )                                                 *
 *                                                                          *
 *  system Configures                              							*
 *                                                                          *
 * ------------------------------------------------------------------------ */ 
void OMAPL137_SYSCFG_init()   
{
	*KICK0R = 0x83e70b13;  // Kick0 register + keydata (unlock SYSCFG module)
    *KICK1R = 0x95a4f1e0;  // Kick1 register + keydata (unlock SYSCFG module)
	
	*PINMUX0 = PINMUX0_VALUE;
	*PINMUX1 = PINMUX1_VALUE;
	*PINMUX2 = PINMUX2_VALUE;
	*PINMUX3 = PINMUX3_VALUE;
	*PINMUX4 = PINMUX4_VALUE;
	*PINMUX5 = PINMUX5_VALUE;
	*PINMUX6 = PINMUX6_VALUE;
	*PINMUX7 = PINMUX7_VALUE;    // SPI0
	*PINMUX8 = PINMUX8_VALUE;    // UART0
	*PINMUX9 = PINMUX9_VALUE;    //  UART2  TX
	*PINMUX10 = PINMUX10_VALUE;
	*PINMUX11 = PINMUX11_VALUE;  //   UART1
	*PINMUX12 = PINMUX12_VALUE;
	*PINMUX13 = PINMUX13_VALUE;
	*PINMUX14 = PINMUX14_VALUE;
	*PINMUX15 = PINMUX15_VALUE;
	*PINMUX16 = PINMUX16_VALUE;
	*PINMUX17 = PINMUX17_VALUE;
	*PINMUX18 = PINMUX18_VALUE;
	*PINMUX19 = PINMUX19_VALUE;
	
	*CFGCHIP3 &= ~0x2;			// EMIFAʱ��ԴΪPLLC SYSCLK3(Ĭ��)
//	*HOST1CFG |= 0x1;			// allowing DSP to boot(Ĭ��)
} 
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  OMAPL137_EMIFA_init( )                                                  *
 *                                                                          *
 *      EMIFA��ʼ��                            								*
 * ------------------------------------------------------------------------ */
void OMAPL137_EMIFA_init()
{
	// ����EMIFA CS2�ռ�:	16bit ASRAM	
	// 		16bit���ݿ��
	//		���ⲿ�ȴ�
	// 		Normalģʽ
	//		��ʱ��setup-15	strobe-20	hold-6		(1 clycle = 10ns)
	//		дʱ��setup-15	strobe-20	hold-6		(1 clycle = 10ns)		
	//		TA��3
	*EMIFA_CE2CFG = 0x3D4DEA6D;		
	*EMIFA_CE3CFG = 0x3D4DEA6D;			
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  OMAPL137_PLL_init( )                                                    *
 *                                                                          *
 *      clock_source    <- 0: Onchip Oscillator                             *
 *                         1: External Clock                                *
 *                                                                          *
 *      pll_mult        <- 15: 16x Multiplier * 18.432MHz Clk = 295 MHz     *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void OMAPL137_PLL_init(int clock_source, int pll_mult)
{
	// Initializing PLL Mode from PLL Power Down
		
    // Step 1
    *PLL0_PLLCTL &= 0xFFFFFFDF;			

	*PLL0_PLLCTL &= 0xFFFFFFFE;			
	*PLL0_PLLCTL &= 0xFFFFFFF7;			
	_wait(150);							
	
	// step2
	*PLL0_PLLCTL &= 0xFFFFFFDF;			
	*PLL0_PLLCTL &= 0xFFFFFEFF;
		
    // Set clock mode	
    if ( clock_source == 0 )
        *PLL0_PLLCTL &= ~0x0100;        
    else
        *PLL0_PLLCTL |= 0x0100;         
 
    
//    *PLL0_PLLCTL &= 0xFFFFFDFF;		
        
    // step3
    *PLL0_PLLCTL &= 0xFFFFFFF7;			
    
//    *PLL0_PLLCTL |= 0x10;
    
    // step4
    *PLL0_PLLCTL &= 0xFFFFFFFD;			
    
//    *PLL0_PLLCTL &= 0xFFFFFFEF;	

	// step5
	*PLL0_PLLM = pll_mult;				
	*PLL0_POSTDIV = 0x8000;				
	

	// step6	set PLLDIVn by default value
	// SYSCLK1 = 1						
	// SYSCLK2 = 2						
	// SYSCLK3 = 3						
	
	// step7
	*PLL0_PLLCTL |= 0x8;				
	
	*PLL0_OCSEL |= 0x1A;     //OSCIN
	*PLL0_CKEN |= 0x02;      //2
	*PLL0_OSCDIV1 |= 0x8000; //8000
	
	// step8
	_wait(2500);						
	
	// step9
	*PLL0_PLLCTL |= 0x1;				
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  OMAPL137_PSC_init( )              	                                    *
 *                                                                          *
 *      Power & Sleep Contoller ��ʼ��         								*
 * ------------------------------------------------------------------------ */
void OMAPL137_PSC_init()
{
	PSC0_lPSC_disable(0,0);		// EDMA3 Channel Controller
	PSC0_lPSC_disable(0,1);		// EDMA3 Transfer Controller 0
	PSC0_lPSC_disable(0,2);		// EDMA3 Transfer Controller 1
	PSC0_lPSC_enable(0,3);		// enable EMIFA	
	PSC0_lPSC_enable(0,4);		// SPI0
	PSC0_lPSC_disable(0,5);		// MMC/SD0
	PSC0_lPSC_disable(0,6);		// ARM Interrupt Controller
//	PSC0_lPSC_disable(0,7);		// ARM RAM/ROM
	PSC0_lPSC_enable(0,9);		// UART0
	PSC0_lPSC_disable(0,13);	// PRU
	PSC0_lPSC_disable(0,14);	// disable ARM
	
	PSC1_lPSC_disable(0,1);		// USB0 (USB2.0)
	PSC1_lPSC_disable(0,2);		// USB1 (USB1.1)
	PSC1_lPSC_enable(0,3);		// enable GPIO
	PSC1_lPSC_disable(0,4);		// HPI
	PSC1_lPSC_disable(0,5);		// EMAC
	PSC1_lPSC_disable(0,6);		// EMIFB (BR20)
	PSC1_lPSC_disable(0,7);		// McASP0 (+ McASP0 FIFO)
	PSC1_lPSC_disable(0,8);		// McASP1 (+ McASP1 FIFO)
	PSC1_lPSC_disable(0,9);		// McASP2 (+ McASP2 FIFO)
	PSC1_lPSC_enable(0,10);	    // SPI1
	PSC1_lPSC_disable(0,11);	// I2C1
	PSC1_lPSC_disable(0,12);	// UART1
	PSC1_lPSC_disable(0,13);	// UART2
	PSC1_lPSC_disable(0,16);	// LCDC
	PSC1_lPSC_disable(0,17);	// eHRPWM0/1/2
	PSC1_lPSC_disable(0,20);	// eCAP0/1/2
	PSC1_lPSC_disable(0,21);	// eQEP0/1
//	PSC1_lPSC_enable(1,31);		// enable shared RAM(Ĭ��)
}

//void OMAPL137_init()
//{
//	OMAPL137_SYSCFG_init();
//	OMAPL137_PLL_init(1,15);
//	OMAPL137_PSC_init();
//	OMAPL137_EMIFA_init();
//	OMAPL137_GPIO_init();	
//	OMAPL137_DSPINT_init();	
//}

void OMAPL137_DSPINT_init()
{
//	CSR &= 0xFFFE;							// �ر�GIEλ���ر�ȫ���ж�ʹ��
	asm("DINT");							// �ر�GIEλ���ر�ȫ���ж�ʹ��
//	asm("NOP");
	//ʹ�õ����ж�������INT0:0X71;INT1:0X41;INT2:0X2A;INT3:0X3F����
	__WriteAddr(0xFFFFFFFF,0x01E2605C);		// ���GPIO2��GPIO3���жϱ�־λ
	__WriteAddr(0xFFFFFFFF,0x01E26084);		// ���GPIO4��GPIO5���жϱ�־λ
	__WriteAddr(0xFFFFFFFF,0x01E260AC);		// ���GPIO6��GPIO7���жϱ�־λ

	__WriteAddr(0x80000400,0x01E2604C);		// ����GPIO2[10]��GPIO3[15]Ϊ�����ش����ж�
	__WriteAddr(0x00000002,0x01E26074);		// ����GPIO4[1]Ϊ�����ش����ж�
	__WriteAddr(0x00020000,0x01E2609C);		// ����GPIO7[1]Ϊ�����ش����ж�

	__WriteAddr(0x80000400,0x01E26058);		// �ر�GPIO2[10]��GPIO3[15]�½��ش����ж�	
	__WriteAddr(0x00000002,0x01E26080);		// �ر�GPIO4[1]�½��ش����ж�
	__WriteAddr(0x00020000,0x01E260A8);		// �ر�GPIO7[1]�½��ش����ж�

	__WriteAddr(0x9C,0x01E26008);			// ʹ��GPIO bank 2/3/4/7���ж�
//   clear event flag
     *((unsigned int *)0x01800040)=0xFFFFFFFF; 
     *((unsigned int *)0x01800044)=0xFFFFFFFF;
     *((unsigned int *)0x01800048)=0xFFFFFFFF;
     *((unsigned int *)0x0180004C)=0xFFFFFFFF; 
// set event 
     *((unsigned int *)0x01800020)=0x0; 
     *((unsigned int *)0x01800024)=0x0;
     *((unsigned int *)0x01800028)=0x0;
     *((unsigned int *)0x0180002C)=0x0;
// Mask event 
     *((unsigned int *)0x01800080)=0x0000000F;
	 *((unsigned int *)0x01800084)=0xFFFFFFFF;
//	 *((unsigned int *)0x01800080)=0x0000000F;
// intmux
	*((unsigned int *)0x01800104) = 0x34313648;	// �ж�selcet  intmux1
	*((unsigned int *)0x01800108) = 0x00000000;	// �ж�selcet  intmux2
	*((unsigned int *)0x0180010C) = 0x00000040;	// �ж�selcet  intmux3
	
	ISTP = 0x11803C00;						// ���ж�������ַ�׵�ַ�͸�ISTP�Ĵ���
//	asm("NOP");
	
	ICR = 0xFFFF;
//	asm("NOP");
	
	ISR = 0x0;
//	asm("NOP");		
//	asm("RINT");							// ��GIE��ʹ��ȫ���ж�	

	CSR |= 0x1;

	IER = 0xFFFF;							// ʹ���ж�
}

void OMAPL137_TIMER_init()
{
//Timer0 mode select
//step1  
//Timer0 mode select
//step1  

/*	 *TIMER0_TRC&=0x00000000;
	 *TIMER0_TGCR&=0x00000000;
	 *TIMER0_TIM34&=0x00000000;
     *TIMER0_TGCR&=0xfffffff7;    // 0111
     *TIMER0_TGCR|=0x00000002;    // 0010	
     *TIMER0_TGCR|=0x00000004;    // 0100

//     *TIMER0_PRD34=0x47ff;			// 1k  18432
	 *TIMER0_PRD34=0x2400;			// 2k  9216 
     // 0100
//	  *TIMER0_PRD34=0x11FF;			// 4k  4607
	//  *TIMER0_TGCR|=0x00000016;
	 //set 18432
	 //prediv 10
	 *TIMER0_TGCR&=0xfffff0ff;
	 *TIMER0_TRC&=0xffbffeff;
	 *TIMER0_TRC|=0x00800000;
	 //Enable TINT
	 *TIMER0_INTCTLSTAT|=0x00010000;*/
     
     //////////////
//	 *TIMER0_TGCR |=0x00000004;      //  32 bit unchained mode 
//	 *TIMER0_TGCR |=0x00000002;      //   
//	 *TIMER0_PRD34=0x47ff;           //  ���� �Ĵ���
	 //
//     *TIMER0_TGCR |= 0x0000ff00;    // DDR34  PSD43
//	 *TIMER0_TRC  |= 0x00300000;
	 //
//	 *TIMER0_INTCTLSTAT|=0x00010000; 

     // ���Դ�������ʱ����20171023
	 *TIMER0_TRC&=0x00000000;    // ����ļ��������
	 *TIMER0_TGCR&=0x00000000;   //ȫ�ֿ��ƼĴ�������
	 *TIMER0_TIM34&=0x00000000;  //TIM34�����Ĵ�����
	 *TIMER0_TIM12&=0x00000000;  //TIM12�����Ĵ�����
     *TIMER0_TGCR&=0xfffffff7;    // 0111
	 //
   //*TIMER0_TGCR|=0x00000002;    // 0011
     *TIMER0_TGCR|=0x00000003;    // 0011	
     *TIMER0_TGCR|=0x00000004;    // 0100

//     *TIMER0_PRD34=0x47ff;		// 1k  18432
	 *TIMER0_PRD34=0x2400;			// 2k  9216 
	 *TIMER0_PRD12=0xFFFF;			// 2k  9216 
     // 0100
//	  *TIMER0_PRD34=0x11FF;			// 4k  4607
	//  *TIMER0_TGCR|=0x00000016;
	 //set 18432
	 //prediv 10
	 *TIMER0_TGCR&=0xfffff0ff;
	 *TIMER0_TRC&=0xffbffeff;
	 *TIMER0_TRC|=0x00800000;
	 //Enable TINT
	 *TIMER0_INTCTLSTAT|=0x00010000;

}


void OMAPL137_DSPINT_OPENALL()
{
//   *((unsigned int *)0x01800104) = 0x34400000;	    //== ��PPS�ʹ��ж�  
//   *((unsigned int *)0x01800108) = 0x00000031;    	//== ����ʱ�ж�  
//   *((unsigned int *)0x01800104) = 0x34310000;	    //== ��PPS�ʹ��ж�  
//   *((unsigned int *)0x01800108) = 0x00000040;    	//== ����ʱ�ж�    
//   *((unsigned int *)0x01800104) = 0x00340000;	    //== ��PPS�ʹ��ж�  
   *((unsigned int *)0x01800104) = 0x34400000;	    //== ��PPS�ʹ��ж�  
   *((unsigned int *)0x01800108) = 0x00000000;    	//== ����ʱ�ж� 
//   *((unsigned int *)0x01800108) = 0x00000000;    	//== ����ʱ�ж�   

}

void OMAPL137_DSPINT_CLOSEALL()
{
	*((unsigned int *)0x01800104)= 0x0; 		//�ر��ж� 201510153 
	*((unsigned int *)0x01800108) =0x0;    //�ر��ж� 20151015 			
}


void OMAPL137_PPSINT_OPEN()
{
	*((unsigned int *)0x01E26008)= 0x08; 		  	
}

void OMAPL137_PPSINT_CLOSE()
{
	*((unsigned int *)0x01E26008)= 0x00; 			
}




/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  GPIO BANK �ж�ʹ��/�رպ���        	                                    *
 *                                                                          *
 *      							         								*
 * ------------------------------------------------------------------------ */
/*void bint_enable(int bank)
{
	*((unsigned int *)0x01E26008) |= (1<<bank);		
}*/

/*void bint_disable(int bank)
{
	*((unsigned int *)0x01E26008) &= ~(1<<bank);		
}*/

