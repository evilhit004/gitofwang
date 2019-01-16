

#include "omapl137_psc.h"

/*Enable Function for PSC0*/
void PSC0_lPSC_enable(unsigned int PD, unsigned int LPSC_num)
{
	unsigned int *addr1 = 0;
	
	addr1 = (unsigned int *)(PSC0_MDCTL + 4*LPSC_num);
	*addr1 = (*addr1 & 0xFFFFFFE0) | 0x0003;	// MDCTLn�Ĵ�������NEXT̬ΪEnable state
	
	addr1 = (unsigned int *)(PSC0_PTCMD);
	*addr1 = 0x1<<PD;							// д1��PTCMD�Ĵ�����PD��CURRENT̬ת��ΪNEXT̬         
	
	addr1 = (unsigned int *)(PSC0_PTSTAT);
	while( (*addr1 & (0x1 << PD)) != 0 );		// ���PDת��״̬	/*Wait for power state transition to finish*/
	
	addr1 = (unsigned int *)(PSC0_MDSTAT + 4*LPSC_num);
	while( (*addr1 & 0x3F) != 0x3 );			// ���ģ�鵱ǰ״̬�Ƿ�Ϊenable(03h��ʾenable)
}

/*Disable Function for PSC0*/
void PSC0_lPSC_disable(unsigned int PD, unsigned int LPSC_num)
{
	unsigned int *addr1 = 0;
	
	addr1 = (unsigned int *)(PSC0_MDCTL + 4*LPSC_num);
	*addr1 = (*addr1 & 0xFFFFFFE0) | 0x0002;	// MDCTLn�Ĵ�������NEXT̬ΪDisable state
	
	addr1 = (unsigned int *)(PSC0_PTCMD);
	*addr1 = 0x1<<PD;							// д1��PTCMD�Ĵ�����PD��CURRENT̬ת��ΪNEXT̬ 
	
	addr1 = (unsigned int *)(PSC0_PTSTAT);
	while( (*addr1 & (0x1 << PD)) != 0 );		// ���PDת��״̬	/*Wait for power state transition to finish*/
	
	addr1 = (unsigned int *)(PSC0_MDSTAT + 4*LPSC_num);
	while( (*addr1 & 0x1F) != 0x2 );			// ���ģ�鵱ǰ״̬�Ƿ�ΪDisable(02h��ʾDisable)
}

/*Enable Function for PSC1*/
void PSC1_lPSC_enable(unsigned int PD, unsigned int LPSC_num)
{
	unsigned int *addr1 = 0;
	
	addr1 = (unsigned int *)(PSC1_MDCTL + 4*LPSC_num);
	*addr1 = (*addr1 & 0xFFFFFFE0) | 0x0003;	// MDCTLn�Ĵ�������NEXT̬ΪEnable state
	
	addr1 = (unsigned int *)(PSC1_PTCMD);
	*addr1 = 0x1<<PD;							// д1��PTCMD�Ĵ�����PD��CURRENT̬ת��ΪNEXT̬ 
	
	addr1 = (unsigned int *)(PSC1_PTSTAT);
	while( (*addr1 & (0x1 << PD)) != 0 );		// ���PDת��״̬	/*Wait for power state transition to finish*/
	
	addr1 = (unsigned int *)(PSC1_MDSTAT + 4*LPSC_num);
	while( (*addr1 & 0x1F) != 0x3 );			// ���ģ�鵱ǰ״̬�Ƿ�Ϊenable(03h��ʾenable)
}

/*Disable Function for PSC1*/
void PSC1_lPSC_disable(unsigned int PD, unsigned int LPSC_num)
{
	unsigned int *addr1 = 0;
	
	addr1 = (unsigned int *)(PSC1_MDCTL + 4*LPSC_num);
	*addr1 = (*addr1 & 0xFFFFFFE0) | 0x0002;	// MDCTLn�Ĵ�������NEXT̬ΪDisable state
	
	addr1 = (unsigned int *)(PSC1_PTCMD);
	*addr1 = 0x1<<PD;							// д1��PTCMD�Ĵ�����PD��CURRENT̬ת��ΪNEXT̬ 
	
	addr1 = (unsigned int *)(PSC1_PTSTAT);
	while( (*addr1 & (0x1 << PD)) != 0 );		// ���PDת��״̬	/*Wait for power state transition to finish*/
	
	addr1 = (unsigned int *)(PSC1_MDSTAT + 4*LPSC_num);
	while( (*addr1 & 0x1F) != 0x2 );			// ���ģ�鵱ǰ״̬�Ƿ�ΪDisable(02h��ʾDisable)
}

