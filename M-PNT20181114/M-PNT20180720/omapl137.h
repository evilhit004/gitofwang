/* ------------------------------------------------------------------------ *
 *  OMAPL137.h	TrailBlazer					                                *
 *  编写时间：	2014.05.20.                                                 *
 *  更新时间：																*
 *	编写人：	chepengyu                                  					*
 *  更新说明：                                                              *
 *                                                                          *
 * ------------------------------------------------------------------------ */

#ifndef OMAPL137_
#define OMAPL137_

#include <c6x.h>
#include	"general.h"
//#include	"flash.h"
#include	"DSPexPC.h"
#include	"data_type.h"


#include 	"omapl137_psc.h"


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Variable types                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */

#define Uint32  unsigned int
#define Uint16  unsigned short
#define Uint8   unsigned char
#define Int32   int
#define Int16   short
#define Int8    char


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Software Breakpoint code                                                *
 *      Uses inline assembly command                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */

#define SW_BREAKPOINT       asm( " SWBP 0" );

/* ------------------------------------------------------------------------ *
 *  系统用宏函数定义                                                        *
 *		使用以下函数会降低程序移植性和可读性，但可以提高运行效率
 *		建议使用在对时间速度有要求的地方
 * ------------------------------------------------------------------------ */
#define __WriteAddr(data, AddrBase)			*((Uint32*)(AddrBase)) = (data)		// 向绝对地址写入数据
#define __ReadAddr(AddrBase)				(*((Uint32*)(AddrBase)))			// 从绝对地址读出数据

/* ------------------------------------------------------------------------ *
 *                                                                          *
 * External Memory Interface (EMIFA) Registers                                                        *
 * 
 * 更新时间：	2014.05.20.
 * 更新者：		chepengyu
 * 更新说明：
 *	1.寄存器名称修改为与datasheet一致
 *	2.TrailBlazer使用的是NOR FLASH，所以只定义了相关寄存器                                                                         *
 * ------------------------------------------------------------------------ */
#define EMIFA_BASE              0x68000000 
#define EMIFA_AWCC             	(( volatile Uint32* )( EMIFA_BASE + 0x04 ))	// Asynchronous Wait Cycle Configuration Register
#define EMIFA_CE2CFG            (( volatile Uint32* )( EMIFA_BASE + 0x10 ))	// Asynchronous 1 Configuration Register
#define EMIFA_CE3CFG            (( volatile Uint32* )( EMIFA_BASE + 0x14 ))
#define EMIFA_CE4CFG            (( volatile Uint32* )( EMIFA_BASE + 0x18 ))
#define EMIFA_CE5CFG            (( volatile Uint32* )( EMIFA_BASE + 0x1C ))
                                
#define EMIFA_INTRAW            (( volatile Uint32* )( EMIFA_BASE + 0x40 )) // EMIFA Interrupt Raw Register
#define EMIFA_INTMSK            (( volatile Uint32* )( EMIFA_BASE + 0x44 )) // EMIFA Interrupt Mask Register
#define EMIFA_INTMSKSET         (( volatile Uint32* )( EMIFA_BASE + 0x48 )) // EMIFA Interrupt Mask Set Register
#define EMIFA_INTMSKCLR         (( volatile Uint32* )( EMIFA_BASE + 0x4C ))	// EMIFA Interrupt Mask Clear Register	 

#define EMIFA_CS0_BASE         	0x40000000			// EMIFA SDRAM data (CS0)
#define EMIFA_CS2_BASE          0x60000000			// EMIFA async data (CS2)
#define EMIFA_CS3_BASE          0x62000000			// EMIFA async data (CS3)
#define EMIFA_CS4_BASE          0x64000000			// EMIFA async data (CS4)
#define EMIFA_CS5_BASE          0x66000000			// EMIFA async data (CS5)

/* ------------------------------------------------------------------------ *
 *                                                                          *
 * System Configuration(SYSCFG) Module Register Access                                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */
#define SYS_BASE            0x01C14000
#define REVID               ((unsigned int*)(SYS_BASE + 0x000))  
#define DIEIDR0             ((unsigned int*)(SYS_BASE + 0x008))
#define DIEIDR1             ((unsigned int*)(SYS_BASE + 0x00C))
#define DIEIDR2             ((unsigned int*)(SYS_BASE + 0x010))
#define DIEIDR3             ((unsigned int*)(SYS_BASE + 0x014))
#define DEVIDR0             ((unsigned int*)(SYS_BASE + 0x018))
#define DEVIDR1             ((unsigned int*)(SYS_BASE + 0x01C))
#define BOOTCFG             ((unsigned int*)(SYS_BASE + 0x020))		// Boot Configuration Register
#define CHIPREVIDR          ((unsigned int*)(SYS_BASE + 0x024))
#define KICK0R              ((unsigned int*)(SYS_BASE + 0x038))  
#define KICK1R              ((unsigned int*)(SYS_BASE + 0x03c))  
#define HOST0CFG            ((unsigned int*)(SYS_BASE + 0x040))
#define HOST1CFG            ((unsigned int*)(SYS_BASE + 0x044))
#define IRAWSTAT            ((unsigned int*)(SYS_BASE + 0x0E0))  	// Interrupt Raw Status/Set Register
#define IENSTAT             ((unsigned int*)(SYS_BASE + 0x0E4))		// Interrupt Enable Status/Clear Register  
#define IENSET              ((unsigned int*)(SYS_BASE + 0x0E8))  	// Interrupt Enable Register	
#define IENCLR              ((unsigned int*)(SYS_BASE + 0x0EC)) 	// Interrupt Enable Clear Register 	
#define EOI                 ((unsigned int*)(SYS_BASE + 0x0F0))  	// End of Interrupt Register
#define FLTADDRR            ((unsigned int*)(SYS_BASE + 0x0F4))  
#define FLTSTAT             ((unsigned int*)(SYS_BASE + 0x0F8))  
#define MSTPRI0             ((unsigned int*)(SYS_BASE + 0x110))  
#define MSTPRI1             ((unsigned int*)(SYS_BASE + 0x114))  
#define MSTPRI2             ((unsigned int*)(SYS_BASE + 0x118)) 
#define PINMUX0				((unsigned int*)(SYS_BASE + 0x120))		// Pin Multiplexing Control 0 Register
#define PINMUX1				((unsigned int*)(SYS_BASE + 0x124))
#define PINMUX2				((unsigned int*)(SYS_BASE + 0x128))
#define PINMUX3				((unsigned int*)(SYS_BASE + 0x12C))
#define PINMUX4				((unsigned int*)(SYS_BASE + 0x130))
#define PINMUX5				((unsigned int*)(SYS_BASE + 0x134))
#define PINMUX6				((unsigned int*)(SYS_BASE + 0x138))
#define PINMUX7				((unsigned int*)(SYS_BASE + 0x13C))
#define PINMUX8				((unsigned int*)(SYS_BASE + 0x140))
#define PINMUX9				((unsigned int*)(SYS_BASE + 0x144))
#define PINMUX10			((unsigned int*)(SYS_BASE + 0x148))
#define PINMUX11			((unsigned int*)(SYS_BASE + 0x14C))
#define PINMUX12			((unsigned int*)(SYS_BASE + 0x150))
#define PINMUX13			((unsigned int*)(SYS_BASE + 0x154))
#define PINMUX14			((unsigned int*)(SYS_BASE + 0x158))
#define PINMUX15			((unsigned int*)(SYS_BASE + 0x15C))
#define PINMUX16			((unsigned int*)(SYS_BASE + 0x160))
#define PINMUX17			((unsigned int*)(SYS_BASE + 0x164))
#define PINMUX18			((unsigned int*)(SYS_BASE + 0x168))
#define PINMUX19			((unsigned int*)(SYS_BASE + 0x16C))
#define SUSPSRC		 	    ((unsigned int*)(SYS_BASE + 0x170)) 	// Suspend Source Register
#define CHIPSIG	            ((unsigned int*)(SYS_BASE + 0x174)) 
#define CHIPSIG_CLR         ((unsigned int*)(SYS_BASE + 0x178)) 
#define CFGCHIP0            ((unsigned int*)(SYS_BASE + 0x17C)) 	// Chip Configuration 0 Register
#define CFGCHIP1            ((unsigned int*)(SYS_BASE + 0x180)) 
#define CFGCHIP2            ((unsigned int*)(SYS_BASE + 0x184)) 
#define CFGCHIP3           	((unsigned int*)(SYS_BASE + 0x188)) 
#define CFGCHIP4		 	((unsigned int*)(SYS_BASE + 0x18C)) 

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  DSP Interrupts controller                                               *
 *		DSP 中断控制寄存器     	                                            *
 * ------------------------------------------------------------------------ */
#define INTC_EVTFLAG0    	((unsigned int *)0x01800000) 	// Event flag register 0
#define INTC_EVTFLAG1    	((unsigned int *)0x01800004) 	// Event flag register 1
#define INTC_EVTFLAG2    	((unsigned int *)0x01800008) 	// Event flag register 2
#define INTC_EVTFLAG3    	((unsigned int *)0x0180000C) 	// Event flag register 3
#define INTC_EVTSET0 		((unsigned int *)0x01800020) 	// Event set register 0
#define INTC_EVTSET1 		((unsigned int *)0x01800024) 	// Event set register 1
#define INTC_EVTSET2 		((unsigned int *)0x01800028) 	// Event set register 2
#define INTC_EVTSET3 		((unsigned int *)0x0180002C) 	// Event set register 3
#define INTC_EVTCLR0 		((unsigned int *)0x01800040) 	// Event clear register 0
#define INTC_EVTCLR1 		((unsigned int *)0x01800044) 	// Event clear register 1
#define INTC_EVTCLR2 		((unsigned int *)0x01800048) 	// Event clear register 2
#define INTC_EVTCLR3 		((unsigned int *)0x0180004C) 	// Event clear register 3
#define INTC_EVTMASK0    	((unsigned int *)0x01800080) 	// Event mask register 0
#define INTC_EVTMASK1    	((unsigned int *)0x01800084) 	// Event mask register 1
#define INTC_EVTMASK2    	((unsigned int *)0x01800088) 	// Event mask register 2
#define INTC_EVTMASK3    	((unsigned int *)0x0180008C) 	// Event mask register 3
#define INTC_MEVTFLAG0   	((unsigned int *)0x018000A0)  	// Masked event flag register 0
#define INTC_MEVTFLAG1   	((unsigned int *)0x018000A4)  	// Masked event flag register 1
#define INTC_MEVTFLAG2   	((unsigned int *)0x018000A8)  	// Masked event flag register 2
#define INTC_MEVTFLAG3   	((unsigned int *)0x018000AC)  	// Masked event flag register 3
#define INTC_EXPMASK0    	((unsigned int *)0x018000C0) 	// Exception mask register 0
#define INTC_EXPMASK1    	((unsigned int *)0x018000C4) 	// Exception mask register 1
#define INTC_EXPMASK2    	((unsigned int *)0x018000C8) 	// Exception mask register 2
#define INTC_EXPMASK3    	((unsigned int *)0x018000CC) 	// Exception mask register 3
#define INTC_MEXPFLAG0   	((unsigned int *)0x018000E0)  	// Masked exception flag register 0
#define INTC_MEXPFLAG1   	((unsigned int *)0x018000E4)  	// Masked exception flag register 1
#define INTC_MEXPFLAG2   	((unsigned int *)0x018000E8)  	// Masked exception flag register 2
#define INTC_MEXPFLAG3   	((unsigned int *)0x018000EC)  	// Masked exception flag register 3
#define INTC_INTMUX1 		((unsigned int *)0x01800104) 	// Interrupt mux register 1
#define INTC_INTMUX2 		((unsigned int *)0x01800108) 	// Interrupt mux register 2
#define INTC_INTMUX3 		((unsigned int *)0x0180010C) 	// Interrupt mux register 3
#define INTC_INTXSTAT		((unsigned int *)0x01800180)  	// Interrupt exception status
#define INTC_INTXCLR 		((unsigned int *)0x01800184) 	// Interrupt exception clear
#define INTC_INTDMASK		((unsigned int *)0x01800188)  	// Dropped interrupt mask register
#define INTC_EVTASRT 		((unsigned int *)0x018001C0) 	// Event assert register
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  I2C Controller                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
#define I2C_BASE                0x01E28000
#define I2C_OAR                 (( volatile Uint32* )( I2C_BASE + 0x00 ))
#define I2C_ICIMR               (( volatile Uint32* )( I2C_BASE + 0x04 ))
#define I2C_ICSTR               (( volatile Uint32* )( I2C_BASE + 0x08 ))
#define I2C_ICCLKL              (( volatile Uint32* )( I2C_BASE + 0x0C ))
#define I2C_ICCLKH              (( volatile Uint32* )( I2C_BASE + 0x10 ))
#define I2C_ICCNT               (( volatile Uint32* )( I2C_BASE + 0x14 ))
#define I2C_ICDRR               (( volatile Uint32* )( I2C_BASE + 0x18 ))
#define I2C_ICSAR               (( volatile Uint32* )( I2C_BASE + 0x1C ))
#define I2C_ICDXR               (( volatile Uint32* )( I2C_BASE + 0x20 ))
#define I2C_ICMDR               (( volatile Uint32* )( I2C_BASE + 0x24 ))
#define I2C_ICIVR               (( volatile Uint32* )( I2C_BASE + 0x28 ))
#define I2C_ICEMDR              (( volatile Uint32* )( I2C_BASE + 0x2C ))
#define I2C_ICPSC               (( volatile Uint32* )( I2C_BASE + 0x30 ))
#define I2C_ICPID1              (( volatile Uint32* )( I2C_BASE + 0x34 ))
#define I2C_ICPID2              (( volatile Uint32* )( I2C_BASE + 0x38 ))

/* I2C Field Definitions */
#define ICOAR_MASK_7                    0x007F
#define ICOAR_MASK_10                   0x03FF
#define ICSAR_MASK_7                    0x007F
#define ICSAR_MASK_10                   0x03FF
#define ICOAR_OADDR                     0x007f
#define ICSAR_SADDR                     0x0050

#define ICSTR_SDIR                      0x4000
#define ICSTR_NACKINT                   0x2000
#define ICSTR_BB                        0x1000
#define ICSTR_RSFULL                    0x0800
#define ICSTR_XSMT                      0x0400
#define ICSTR_AAS                       0x0200
#define ICSTR_AD0                       0x0100
#define ICSTR_SCD                       0x0020
#define ICSTR_ICXRDY                    0x0010
#define ICSTR_ICRRDY                    0x0008
#define ICSTR_ARDY                      0x0004
#define ICSTR_NACK                      0x0002
#define ICSTR_AL                        0x0001

#define ICMDR_NACKMOD                   0x8000
#define ICMDR_FREE                      0x4000
#define ICMDR_STT                       0x2000
#define ICMDR_IDLEEN                    0x1000
#define ICMDR_STP                       0x0800
#define ICMDR_MST                       0x0400
#define ICMDR_TRX                       0x0200
#define ICMDR_XA                        0x0100
#define ICMDR_RM                        0x0080
#define ICMDR_DLB                       0x0040
#define ICMDR_IRS                       0x0020
#define ICMDR_STB                       0x0010
#define ICMDR_FDF                       0x0008
#define ICMDR_BC_MASK                   0x0007

///* ------------------------------------------------------------------------ *
// *                                                                          *
// *  INTC controller                                                         *
// *      Controls the Interrupts                                             *
// *                                                                          *
// * ------------------------------------------------------------------------ */
//#define INTC_BASE               0xFFFE0000
//#define INTC_FIQ0               (( volatile Uint32* )( INTC_BASE + 0x00 ))
//#define INTC_FIQ1               (( volatile Uint32* )( INTC_BASE + 0x04 ))
//#define INTC_IRQ0               (( volatile Uint32* )( INTC_BASE + 0x08 ))
//#define INTC_IRQ1               (( volatile Uint32* )( INTC_BASE + 0x0C ))
//#define INTC_FIQENTRY           (( volatile Uint32* )( INTC_BASE + 0x10 ))
//#define INTC_IRQENTRY           (( volatile Uint32* )( INTC_BASE + 0x14 ))
//#define INTC_EINT0              (( volatile Uint32* )( INTC_BASE + 0x18 ))
//#define INTC_EINT1              (( volatile Uint32* )( INTC_BASE + 0x1C ))
//#define INTC_INTCTL             (( volatile Uint32* )( INTC_BASE + 0x20 ))
//#define INTC_EABASE             (( volatile Uint32* )( INTC_BASE + 0x24 ))
//#define INTC_INTPRI0            (( volatile Uint32* )( INTC_BASE + 0x30 ))
//#define INTC_INTPRI1            (( volatile Uint32* )( INTC_BASE + 0x34 ))
//#define INTC_INTPRI2            (( volatile Uint32* )( INTC_BASE + 0x38 ))
//#define INTC_INTPRI3            (( volatile Uint32* )( INTC_BASE + 0x3C ))
//#define INTC_INTPRI4            (( volatile Uint32* )( INTC_BASE + 0x40 ))
//#define INTC_INTPRI5            (( volatile Uint32* )( INTC_BASE + 0x44 ))
//#define INTC_INTPRI6            (( volatile Uint32* )( INTC_BASE + 0x48 ))
//#define INTC_INTPRI7            (( volatile Uint32* )( INTC_BASE + 0x4C ))

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  PLL0 Controller                                                         *
 *      Generates DSP, ARM clocks                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
#define PLL0_BASE   	0x01C11000   							/*SYSTEM PLL BASE ADDRESS*/

#define PLL0_PID      	((unsigned int*) (PLL0_BASE + 0x00))	/*PID*/
#define PLL0_FUSERR   	((unsigned int*) (PLL0_BASE + 0xE0))	/*x*FuseFarm Error Reg*/
#define PLL0_RSTYPE	    ((unsigned int*) (PLL0_BASE + 0xE4))	/*Reset Type status Reg*/
#define PLL0_PLLCTL     ((unsigned int*) (PLL0_BASE + 0x100)) 	/*PLL Control Register*/
#define PLL0_OCSEL      ((unsigned int*) (PLL0_BASE + 0x104))	/*OBSCLK Select Register*/
#define PLL0_SECCTL     ((unsigned int*) (PLL0_BASE + 0x108))	/*PLL Secondary Control Register*/
#define PLL0_PLLM       ((unsigned int*) (PLL0_BASE + 0x110))  	/*PLL Multiplier*/	
#define PLL0_PREDIV     ((unsigned int*) (PLL0_BASE + 0x114))  	/*Pre divider*/	
#define PLL0_PLLDIV1    ((unsigned int*) (PLL0_BASE + 0x118))  	/*Diveder-1*/	
#define PLL0_PLLDIV2    ((unsigned int*) (PLL0_BASE + 0x11C))  	/*Diveder-2*/	
#define PLL0_PLLDIV3    ((unsigned int*) (PLL0_BASE + 0x120))  	/*Diveder-3*/	
#define PLL0_OSCDIV1    ((unsigned int*) (PLL0_BASE + 0x124))  	/*Oscilator Divider*/	
#define PLL0_POSTDIV    ((unsigned int*) (PLL0_BASE + 0x128))  	/*Post Divider*/	
#define PLL0_BPDIV      ((unsigned int*) (PLL0_BASE + 0x12C))	/*Bypass Divider*/
#define PLL0_WAKEUP     ((unsigned int*) (PLL0_BASE + 0x130))	/*Wakeup Reg*/
#define PLL0_PLLCMD     ((unsigned int*) (PLL0_BASE + 0x138))	/*Command Reg*/
#define PLL0_PLLSTAT    ((unsigned int*) (PLL0_BASE + 0x13C))	/*Status Reg*/
#define PLL0_ALNCTL     ((unsigned int*) (PLL0_BASE + 0x140))	/*Clock Align Control Reg*/
#define PLL0_DCHANGE    ((unsigned int*) (PLL0_BASE + 0x144))	/*PLLDIV Ratio Chnage status*/
#define PLL0_CKEN       ((unsigned int*) (PLL0_BASE + 0x148))	/*Clock Enable Reg*/
#define PLL0_CKSTAT     ((unsigned int*) (PLL0_BASE + 0x14C))	/*Clock Status Reg*/
#define PLL0_SYSTAT     ((unsigned int*) (PLL0_BASE + 0x150))	/*Sysclk status reg*/
#define PLL0_PLLDIV4    ((unsigned int*) (PLL0_BASE + 0x160))	/*Divider 4*/
#define PLL0_PLLDIV5    ((unsigned int*) (PLL0_BASE + 0x164))	/*Divider 5*/
#define PLL0_PLLDIV6    ((unsigned int*) (PLL0_BASE + 0x168))	/*Divider 6*/
#define PLL0_PLLDIV7    ((unsigned int*) (PLL0_BASE + 0x16C))	/*Divider 7*/
#define PLL0_PLLDIV8    ((unsigned int*) (PLL0_BASE + 0x170))	/*Divider 8*/
#define PLL0_PLLDIV9    ((unsigned int*) (PLL0_BASE + 0x174))	/*Divider 9*/
#define PLL0_PLLDIV10   ((unsigned int*) (PLL0_BASE + 0x178))	/*Divider 10*/
#define PLL0_PLLDIV11   ((unsigned int*) (PLL0_BASE + 0x17C))	/*Divider 11*/
#define PLL0_PLLDIV12   ((unsigned int*) (PLL0_BASE + 0x180))	/*Divider 12*/
#define PLL0_PLLDIV13   ((unsigned int*) (PLL0_BASE + 0x184))	/*Divider 13*/
#define PLL0_PLLDIV14   ((unsigned int*) (PLL0_BASE + 0x188))	/*Divider 14*/
#define PLL0_PLLDIV15   ((unsigned int*) (PLL0_BASE + 0x18C))	/*Divider 15*/
#define PLL0_PLLDIV16   ((unsigned int*) (PLL0_BASE + 0x190))	/*Divider 16*/
#define PLLEN_MUX_SWITCH  4 
#define PLL_LOCK_TIME_CNT 2400
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Timer Controller                                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
#define TIMER0_BASE         0x01C20000
#define TIMER0_EMUMGT       (( volatile Uint32* )( TIMER0_BASE + 0x04 ))
#define TIMER0_TIM12        (( volatile Uint32* )( TIMER0_BASE + 0x10 ))
#define TIMER0_TIM34        (( volatile Uint32* )( TIMER0_BASE + 0x14 ))
#define TIMER0_PRD12        (( volatile Uint32* )( TIMER0_BASE + 0x18 ))
#define TIMER0_PRD34        (( volatile Uint32* )( TIMER0_BASE + 0x1C ))
#define TIMER0_TRC          (( volatile Uint32* )( TIMER0_BASE + 0x20 ))
#define TIMER0_TGCR         (( volatile Uint32* )( TIMER0_BASE + 0x24 ))
#define TIMER0_INTCTLSTAT   (( volatile Uint32* )( TIMER0_BASE + 0x44 ))

#define TIMER1_BASE         0x01C21000
#define TIMER1_EMUMGT       (( volatile Uint32* )( TIMER1_BASE + 0x04 ))
#define TIMER1_TIM12        (( volatile Uint32* )( TIMER1_BASE + 0x10 ))
#define TIMER1_TIM34        (( volatile Uint32* )( TIMER1_BASE + 0x14 ))
#define TIMER1_PRD12        (( volatile Uint32* )( TIMER1_BASE + 0x18 ))
#define TIMER1_PRD34        (( volatile Uint32* )( TIMER1_BASE + 0x1C ))
#define TIMER1_TRC          (( volatile Uint32* )( TIMER1_BASE + 0x20 ))
#define TIMER1_TGCR         (( volatile Uint32* )( TIMER1_BASE + 0x24 ))

#define TIMER_EMUMGT            ( 0x04 )
#define TIMER_TIM12             ( 0x10 )
#define TIMER_TIM34             ( 0x14 )
#define TIMER_PRD12             ( 0x18 )
#define TIMER_PRD34             ( 0x1C )
#define TIMER_TRC               ( 0x20 )
#define TIMER_TGCR              ( 0x24 )


#define GPIO_BASE            0x01E26000

#define DIR67            (( volatile Uint32* ) ( GPIO_BASE + 0x88 ))
#define OUT_DATA67       (( volatile Uint32* ) ( GPIO_BASE + 0x8C ))


#define AD7608_Address   (( volatile Uint32* )( 0x62222222))


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  UART Controller                                                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 /*
#define UART0_BASE          0x01C42000
#define UART0_RBR           *( volatile Uint32* )( UART0_BASE + 0x00 )
#define UART0_THR           *( volatile Uint32* )( UART0_BASE + 0x00 )
#define UART0_IER           *( volatile Uint32* )( UART0_BASE + 0x04 )
#define UART0_IIR           *( volatile Uint32* )( UART0_BASE + 0x08 )
#define UART0_FCR           *( volatile Uint32* )( UART0_BASE + 0x08 )
#define UART0_LCR           *( volatile Uint32* )( UART0_BASE + 0x0C )
#define UART0_MCR           *( volatile Uint32* )( UART0_BASE + 0x10 )
#define UART0_LSR           *( volatile Uint32* )( UART0_BASE + 0x14 )
#define UART0_DLL           *( volatile Uint32* )( UART0_BASE + 0x20 )
#define UART0_DLH           *( volatile Uint32* )( UART0_BASE + 0x24 )
#define UART0_PID1          *( volatile Uint32* )( UART0_BASE + 0x28 )
#define UART0_PID2          *( volatile Uint32* )( UART0_BASE + 0x2C )
#define UART0_PWREMU_MGMT   *( volatile Uint32* )( UART0_BASE + 0x30 )

#define UART1_BASE          0x01D0C000
#define UART1_RBR           *( volatile Uint32* )( UART1_BASE + 0x00 )
#define UART1_THR           *( volatile Uint32* )( UART1_BASE + 0x00 )
#define UART1_IER           *( volatile Uint32* )( UART1_BASE + 0x04 )
#define UART1_IIR           *( volatile Uint32* )( UART1_BASE + 0x08 )
#define UART1_FCR           *( volatile Uint32* )( UART1_BASE + 0x08 )
#define UART1_LCR           *( volatile Uint32* )( UART1_BASE + 0x0C )
#define UART1_MCR           *( volatile Uint32* )( UART1_BASE + 0x10 )
#define UART1_LSR           *( volatile Uint32* )( UART1_BASE + 0x14 )
#define UART1_DLL           *( volatile Uint32* )( UART1_BASE + 0x20 )
#define UART1_DLH           *( volatile Uint32* )( UART1_BASE + 0x24 )
#define UART1_PID1          *( volatile Uint32* )( UART1_BASE + 0x28 )
#define UART1_PID2          *( volatile Uint32* )( UART1_BASE + 0x2C )
#define UART1_PWREMU_MGMT   *( volatile Uint32* )( UART1_BASE + 0x30 )

#define UART2_BASE          0x01D0D000
#define UART2_RBR           *( volatile Uint32* )( UART2_BASE + 0x00 )
#define UART2_THR           *( volatile Uint32* )( UART2_BASE + 0x00 )
#define UART2_IER           *( volatile Uint32* )( UART2_BASE + 0x04 )
#define UART2_IIR           *( volatile Uint32* )( UART2_BASE + 0x08 )
#define UART2_FCR           *( volatile Uint32* )( UART2_BASE + 0x08 )
#define UART2_LCR           *( volatile Uint32* )( UART2_BASE + 0x0C )
#define UART2_MCR           *( volatile Uint32* )( UART2_BASE + 0x10 )
#define UART2_LSR           *( volatile Uint32* )( UART2_BASE + 0x14 )
#define UART2_DLL           *( volatile Uint32* )( UART2_BASE + 0x20 )
#define UART2_DLH           *( volatile Uint32* )( UART2_BASE + 0x24 )
#define UART2_PID1          *( volatile Uint32* )( UART2_BASE + 0x28 )
#define UART2_PID2          *( volatile Uint32* )( UART2_BASE + 0x2C )
#define UART2_PWREMU_MGMT   *( volatile Uint32* )( UART2_BASE + 0x30 )

#define UART_RBR                ( 0x00 )
#define UART_THR                ( 0x00 )
#define UART_IER                ( 0x04 )
#define UART_IIR                ( 0x08 )
#define UART_FCR                ( 0x08 )
#define UART_LCR                ( 0x0C )
#define UART_MCR                ( 0x10 )
#define UART_LSR                ( 0x14 )
#define UART_DLL                ( 0x20 )
#define UART_DLH                ( 0x24 )
#define UART_PID1               ( 0x28 )
#define UART_PID2               ( 0x2C )
#define UART_PWREMU_MGMT        ( 0x30 )
*/
/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */

void OMAPL137_SYSCFG_init(void);
void OMAPL137_PLL_init(int clock_source, int pll_mult);
void OMAPL137_PSC_init(void);
void OMAPL137_EMIFA_init(void);
void OMAPL137_GPIO_init(void);
//void DSP_INTSel(unsigned char , unsigned char );
void OMAPL137_DSPINT_init(void);
void OMAPL137_TIMER_init(void);
//void bint_enable(int );
//void bint_disable(int );
void _wait(Uint32 delay );


void OMAPL137_DSPINT_OPENALL(void);
void OMAPL137_DSPINT_CLOSEALL(void);

void OMAPL137_PPSINT_OPEN(void);
void OMAPL137_PPSINT_CLOSE(void);

#endif
