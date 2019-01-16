/********************************************************************************/
// omapl137_psc.h
// 版本：		2.1.0.
// 文件说明：	PSC header file	
// 编写者：		车鹏宇
// 编写时间：	2014.05.20. 
// 更新时间：	
// 更新说明：
/********************************************************************************/
 
#ifndef PSC_H_
#define PSC_H_


/* ------------------------------------------------------------------------ *
 *                                                                          *
 * PSC Module Related Registers                                             *
 *                                                                          *
 * ------------------------------------------------------------------------ */           
#define PSC0_BASE  	    0x01C10000
#define PSC1_BASE  	    0x01E27000

#define PSC0_MDCTL 		(PSC0_BASE+0xA00)
#define PSC0_MDSTAT 	(PSC0_BASE+0x800)

#define PSC1_MDCTL 		(PSC1_BASE+0xA00)
#define PSC1_MDSTAT 	(PSC1_BASE+0x800)
       
#define PSC0_REVID		0x01C10000 	//  Revision Identification Register 
#define PSC0_INTEVAL	0x01C10018 	//  Interrupt Evaluation register
#define PSC0_MERRPR0	0x01C10040 	//  Module Error Pending Register 0 (module 0-15)
#define PSC0_MERRCR0	0x01C10050 	//  Module Error Clear Register 0 (module 0-15) 
#define PSC0_PERRPR		0x01C10060 	//  Power Error Pending Register 
#define PSC0_PERRCR		0x01C10068 	//  Power Error Clear Register 
#define PSC0_PTCMD		0x01C10120 	//  Power Domain Transition Command Register 
#define PSC0_PTSTAT		0x01C10128 	//  Power Domain Transition Status Register 
#define PSC0_PDSTAT0	0x01C10200 	//  Power Domain 0 Status Register 
#define PSC0_PDSTAT1	0x01C10204 	//  Power Domain 1 Status Register 
#define PSC0_PDCTL0		0x01C10300 	//  Power Domain 0 Control Register 
#define PSC0_PDCTL1		0x01C10304 	//  Power Domain 1 Control Register 
#define PSC0_PDCFG0		0x01C10400 	//  Power Domain 0 Configuration Register 
#define PSC0_PDCFG1		0x01C10404 	//  Power Domain 1 Configuration Register 

#define PSC1_REVID		0x01E27000 	// Revision Identification Register Section 9.6.1
#define PSC1_INTEVAL	0x01E27018 	// Interrupt Evaluation Register Section 9.6.2
#define PSC1_MERRPR0	0x01E27040 	// Module Error Pending Register 0 (module 0-31) Section 9.6.4
#define PSC1_MERRCR0	0x01E27050 	// Module Error Clear Register 0 (module 0-31) Section 9.6.6
#define PSC1_PERRPR		0x01E27060 	// Power Error Pending Register Section 9.6.7
#define PSC1_PERRCR		0x01E27068 	// Power Error Clear Register Section 9.6.8
#define PSC1_PTCMD		0x01E27120 	// Power Domain Transition Command Register Section 9.6.9
#define PSC1_PTSTAT		0x01E27128 	// Power Domain Transition Status Register Section 9.6.10
#define PSC1_PDSTAT0	0x01E27200 	// Power Domain 0 Status Register Section 9.6.11
#define PSC1_PDSTAT1	0x01E27204 	// Power Domain 1 Status Register Section 9.6.12
#define PSC1_PDCTL0		0x01E27300 	// Power Domain 0 Control Register Section 9.6.13
#define PSC1_PDCTL1		0x01E27304 	// Power Domain 1 Control Register Section 9.6.14
#define PSC1_PDCFG0		0x01E27400 	// Power Domain 0 Configuration Register Section 9.6.15
#define PSC1_PDCFG1		0x01E27404 	// Power Domain 1 Configuration Register Section 9.6.16

/* ------------------------------------------------------------------------*
 *  PSC相关函数原型                                                        *
 * ------------------------------------------------------------------------*/
void PSC0_lPSC_enable(unsigned int PD, unsigned int LPSC_num);
void PSC0_lPSC_disable(unsigned int PD, unsigned int LPSC_num);
void PSC1_lPSC_enable(unsigned int PD, unsigned int LPSC_num);
void PSC1_lPSC_disable(unsigned int PD, unsigned int LPSC_num); 


#endif
