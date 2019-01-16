

#include "omapl137_psc.h"

/*Enable Function for PSC0*/
void PSC0_lPSC_enable(unsigned int PD, unsigned int LPSC_num)
{
	unsigned int *addr1 = 0;
	
	addr1 = (unsigned int *)(PSC0_MDCTL + 4*LPSC_num);
	*addr1 = (*addr1 & 0xFFFFFFE0) | 0x0003;	// MDCTLn寄存器设置NEXT态为Enable state
	
	addr1 = (unsigned int *)(PSC0_PTCMD);
	*addr1 = 0x1<<PD;							// 写1到PTCMD寄存器将PD的CURRENT态转换为NEXT态         
	
	addr1 = (unsigned int *)(PSC0_PTSTAT);
	while( (*addr1 & (0x1 << PD)) != 0 );		// 检测PD转换状态	/*Wait for power state transition to finish*/
	
	addr1 = (unsigned int *)(PSC0_MDSTAT + 4*LPSC_num);
	while( (*addr1 & 0x3F) != 0x3 );			// 检测模块当前状态是否为enable(03h表示enable)
}

/*Disable Function for PSC0*/
void PSC0_lPSC_disable(unsigned int PD, unsigned int LPSC_num)
{
	unsigned int *addr1 = 0;
	
	addr1 = (unsigned int *)(PSC0_MDCTL + 4*LPSC_num);
	*addr1 = (*addr1 & 0xFFFFFFE0) | 0x0002;	// MDCTLn寄存器设置NEXT态为Disable state
	
	addr1 = (unsigned int *)(PSC0_PTCMD);
	*addr1 = 0x1<<PD;							// 写1到PTCMD寄存器将PD的CURRENT态转换为NEXT态 
	
	addr1 = (unsigned int *)(PSC0_PTSTAT);
	while( (*addr1 & (0x1 << PD)) != 0 );		// 检测PD转换状态	/*Wait for power state transition to finish*/
	
	addr1 = (unsigned int *)(PSC0_MDSTAT + 4*LPSC_num);
	while( (*addr1 & 0x1F) != 0x2 );			// 检测模块当前状态是否为Disable(02h表示Disable)
}

/*Enable Function for PSC1*/
void PSC1_lPSC_enable(unsigned int PD, unsigned int LPSC_num)
{
	unsigned int *addr1 = 0;
	
	addr1 = (unsigned int *)(PSC1_MDCTL + 4*LPSC_num);
	*addr1 = (*addr1 & 0xFFFFFFE0) | 0x0003;	// MDCTLn寄存器设置NEXT态为Enable state
	
	addr1 = (unsigned int *)(PSC1_PTCMD);
	*addr1 = 0x1<<PD;							// 写1到PTCMD寄存器将PD的CURRENT态转换为NEXT态 
	
	addr1 = (unsigned int *)(PSC1_PTSTAT);
	while( (*addr1 & (0x1 << PD)) != 0 );		// 检测PD转换状态	/*Wait for power state transition to finish*/
	
	addr1 = (unsigned int *)(PSC1_MDSTAT + 4*LPSC_num);
	while( (*addr1 & 0x1F) != 0x3 );			// 检测模块当前状态是否为enable(03h表示enable)
}

/*Disable Function for PSC1*/
void PSC1_lPSC_disable(unsigned int PD, unsigned int LPSC_num)
{
	unsigned int *addr1 = 0;
	
	addr1 = (unsigned int *)(PSC1_MDCTL + 4*LPSC_num);
	*addr1 = (*addr1 & 0xFFFFFFE0) | 0x0002;	// MDCTLn寄存器设置NEXT态为Disable state
	
	addr1 = (unsigned int *)(PSC1_PTCMD);
	*addr1 = 0x1<<PD;							// 写1到PTCMD寄存器将PD的CURRENT态转换为NEXT态 
	
	addr1 = (unsigned int *)(PSC1_PTSTAT);
	while( (*addr1 & (0x1 << PD)) != 0 );		// 检测PD转换状态	/*Wait for power state transition to finish*/
	
	addr1 = (unsigned int *)(PSC1_MDSTAT + 4*LPSC_num);
	while( (*addr1 & 0x1F) != 0x2 );			// 检测模块当前状态是否为Disable(02h表示Disable)
}

