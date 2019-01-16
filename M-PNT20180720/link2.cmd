/* ------------------------------------------------------------------------ *
 *  link2.cmd	TrailBlazer				                                	*
 *  ��дʱ�䣺	2014.06.13.	                                                *
 *  ����ʱ�䣺																*
 *	��д�ˣ�	chepengyu                                  					*
 *  ����˵����                                                              *
 *  	2014.06.13.                                                         *
 *			1.����C674x��NOR Legacy Boot��ʽ��cmd�����ļ�					*
 * ------------------------------------------------------------------------ */

-l rts64plus.lib

-stack           0x00000800      /* Stack Size */
-heap            0x00000800      /* Heap Size */

MEMORY
{
  /*  CFG_L2RAM:	 o = 0x11800000	 l = 0x00000020	*/	/* NOR Configuration Word		*/
 /*   PROG_L2RAM:	 o = 0x11800020  l = 0x00003BE0*/		/* 16K bootloader RAM			*/
    VECS:        o = 0x11803C00  l = 0x00000400		/* Interrupts Table	Relocate at DSP L2 RAM			*/
    DSPRAM:      o = 0x11804000  l = 0x0003BC00		/* 256K DSP L2 RAM				*/
    ARMRAM:      o = 0xFFFF0000  l = 0x00002000		/* 8K ARM Local RAM				*/	    
    SHAREDRAM:   o = 0x80000000  l = 0x00020000		/* 128K Shared RAM				*/
    SDRAM:       o = 0xC0000000  l = 0x20000000		/* 256M EMIFB SDRAM Data		*/
}

SECTIONS
{
/*	.nor_config_word:>	CFG_L2RAM */	
/*	.bootload   	:>	PROG_L2RAM */
	.vecs   		:> 	VECS	
    .bss        	:>	DSPRAM
    .cinit      	:>	DSPRAM
    .cio        	:>	DSPRAM
    .const      	:>	DSPRAM
    .stack      	:>	DSPRAM
    .sysmem     	:>	SHAREDRAM
    .text       	:>	DSPRAM
    .switch     	:>	DSPRAM
    .far        	:>	DSPRAM
}
