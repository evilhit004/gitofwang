/********************************************************************************/
// fpga_tp.h
// �汾��		1.0.0.
// �ļ�˵����	FPGA Hydraģ��ͷ�ļ�
// ��д�ߣ�		jesse yang
// ��дʱ�䣺	2016.03.03
// ����ʱ�䣺	
// ����˵����
// 
/********************************************************************************/
#ifndef __FPGA_HYDRA_H
#define __FPGA_HYDRA_H



/* ���������Ϣ	*/
/*      
            Timer_int_en         0  
		    wr_en                1 
		    AD7608_DRDY          2 
		    UART0_int            3 
		    UART1_int            4 
		    UART2_int            5 
		    UART3_int            6 
		    UART4_int            7 
		    UART5_int            8 
		    UART6_int            9 
		    UART7_int            10
		    AddrScan_id(0)       11
		    AddrScan_id(1)       12
		    AddrScan_id(2)       13
		    AddrScan_id(3)       14
		    AddrScan_id(4)       15                           
*/
/* �Ĵ���ƫ�Ƶ�ַ	*/


/* �꺯��		*/
#define TP_Write(AddrBase,data)				*((unsigned short *)(AddrBase)) = (data)
#define TP_Read(AddrBase)					(*((unsigned short *)(AddrBase)))

#endif	/*	__FPGA_TP_H	*/
