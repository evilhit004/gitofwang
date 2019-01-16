/* ------------------------------------------------------------------------ *
 *  main.c	TrailBlazer					                                	*
 *  ��дʱ�䣺	2014.05.20.                                                 *
 *  ����ʱ�䣺	2015.05.28.													*
 *	��д�ˣ�	chepengyu                                  					*
 *  ����˵����                                                              *
 *  	2015.05.28.
 *		1.����Ϊ��׼��ģ��                                                  *
 * ------------------------------------------------------------------------ */

#include "stdio.h"
#include "fpga_init.h"
#include "pinmux.h"
#include "omapl137_gpio.h"
#include "omapl137_psc.h"
//#include "omapl137_uart.h"
#include "omapl137_i2c.h"
#include "Atmel_SPI_Flash.h"
//#include "flash.h"
#include "data_type.h"
#include "omapl137.h"

void interrupt ext_int04(void);
void interrupt ext_int05(void);
void interrupt ext_int06(void);
void interrupt ext_int07(void);
void GPS_Data_SetUp(void);
void CSAC_Data_SetUp(void);

unsigned int a,b,c;


//------------------------------------------------------
// PCͨѶ���
//------------------------------------------------------
unsigned char buff_cmd[SIZE_C_BUFF] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int flag_commu = 1;
//------------------------------------------------------
// ʱ�����(��Ӧ1KHz��ʱ�ж�)
//------------------------------------------------------
#define	NUM_5MS	5
#define	NUM_1S	1000
#define	NUM_5S	5000

int cnt_5ms = 0;
int cnt_5s  = 0;
int cnt_1s  = 0;
int cnt_1ms = 0;

int flag_5ms = 0;
//------------------------------------------------------
// ϵͳ��Ϣ(�����޸�)
//------------------------------------------------------
unsigned short info_fpga[4] = {0,0,0,0};
unsigned char info_Version[16] = {0,0,0,0,0x4,0x11,0x21,0xC4,0,0,0,0,0,0,0,0};

unsigned short frame_cnt = 0;	// FPGAӲ����ʱ�жϼ���
unsigned char sframe_cnt = 0;	// �����ʱ�жϼ���

int flag_format = 0;			// ������ݸ�ʽ��־
//------------------------------------------------------
// ADS1278
//------------------------------------------------------
unsigned char	cnt_cur = 0;
unsigned char 	cnt_pre = 0;
unsigned char 	adfifo_cnt = 0;

unsigned char	adbufL[80];
unsigned char	adbufM[80];
unsigned char	adbufH[80];

float fgyrotemp[3];
float faccytemp[3];
float fgadata[10];


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void main(void)
{
    int i = 0;
    
    /* Initialize BSL */
    
	OMAPL137_SYSCFG_init();
	OMAPL137_PLL_init(1,17); //��ƵΪ18.432MHz*��17+1��=331 MHz
	OMAPL137_PSC_init();
	OMAPL137_EMIFA_init();
	OMAPL137_GPIO_init();	
	OMAPL137_DSPINT_init();	
	FLASHSpiInit(); 
	_wait(1000); 
    
    SpiInit();    //SPI1��ʼ��
    _wait(10);   
	TIMER_EN(INTR6_Base,TIMER_DIS_CMD);				// �ر�FPGA��ʱ�ж���� 
	
	UART_init(UART0_Base, 10, 3,1,1);				// ������115200 ������
	UART_init(UART1_Base, 10, 3,1,1);				// ������115200	UBLOX-NEO-M8T
	UART_init(UART2_Base, 20, 3,1,1);				// ������57600	ԭ���Ӵ���
	
	TIMER_CLKSet(360,TIMER0_Base);

  //	bint_enable(5);
	IER = 0xFFFF;									// ���ж�	
	
	TIMER_WriteL(INTR6_Base,0x4800);				// д��ʱ�жϷ�Ƶ�Ĵ�����16λ
	TIMER_WriteH(INTR6_Base,0x0);						// д��ʱ�жϷ�Ƶ�Ĵ�����16λ
	TIMER_EN(INTR6_Base,TIMER_EN_CMD);				// ʹ��FPGA��ʱ�ж����
	UART_FIFOCLR(UART0_Base);
	UART_FIFOCLR(UART1_Base);
	UART_FIFOCLR(UART2_Base);

   	while(1)
   	{   		
   		if(flag_commu == 0)	// ������ֳɹ�
   		{
   			IER = 0x0;					// ���ж�	
//   			IRQ_disable(IRQ_EVT_EXTINT4);
   			UART_FIFOCLR(UART0_Base);	// ���մ��ڽ���FIFO������
   			
   			// ������PCͨѶģʽ   			
   			parse_command(buff_cmd,UART0_Base);
   		    
   			switch(buff_cmd[3])			// �ж�ָ�����	                   
			{                                                              
				case CODE_CMD_UP:		// ������ϴ�ָ��   
					                                                                           
					Execute_UpLoad(buff_cmd[4],UART0_Base);		
						               
					break;                                                 
					                                                       
				case CODE_CMD_DRQ:		// �������������ָ��    
					
					switch(buff_cmd[4])	// �ж���������
					{
						case SOFTV:
							
							for (i = 0; i < 2; i++)
							{
								info_fpga[i] = __ReadFPGA(SYSREG_Base + (i * 0x2));	
								info_Version[i*2] = (unsigned char)(info_fpga[i]);	
								info_Version[i*2+1] = (unsigned char)(info_fpga[i] >> 8);																	
							}
															
							break;
							
						case ADDRCHK:
							
							break;													
							
						default:                                                   					             
				                                                           
							break;
					}
					
					Execute_DownLoad(buff_cmd[4],buff_cmd[6],UART0_Base); 
					
					break;
					
				case CODE_CMD_SET:							// ���������ָ��
					
					switch(buff_cmd[4])						// ������Ŀ����
					{
						case CODE_SET_FORMAT:				// ������ݸ�ʽ						
							
							flag_format = buff_cmd[5];		// ����궨���ݸ�ʽ
							flag_commu = 1;					// �˳���PCͨѶģʽ
							command_status(S_FOMATOK, UART0_Base);	// �������óɹ�״ָ̬��
					//		IRQ_enable(IRQ_EVT_EXTINT4);	// ���ж�
							IER = 0xFFFF;

							break;
							
						default:                                                   
				                                                           
							break;  
					}
					
					break;
				                                                           
				default:                                                   
				                                                           
					break;                                                 
			}  			  
   		} 		
 		
 		// ��������û�����
 		if(flag_5ms == 1)
 		{
 			flag_5ms = 0;	
 			
 		}
			
	}
}

void interrupt ext_int04()
{
	unsigned int old_csr;
	unsigned int old_irp;	
	static unsigned int	uiTimer_1msINT=0;           //== 1ms�жϼ�ʱ	
	int i =0;
	// IRP CSR ����c6x.h�ļ���Ԥ����
	old_irp = IRP ;			// Save IRP 								
	old_csr = CSR ;			// Save CSR (and thus PGIE)
	CSR = old_csr | 1 ;		// Enable interrupts			

	cnt_5ms++;
	
	// ʹ���жϷ�ʽ����������´���	
 	 	
/*	if(cnt_5s < NUM_5S)
	{
		flag_commu = commu_link(UART0_Base);
		cnt_5s++;
	}
*/	
	cnt_1s++;	
	if(cnt_1s >= NUM_1S)
	{
		cnt_1s = 0;
	//	UART_Txmit(0x55,UART0_Base);
	//	UART_Txmit(0xAA,UART0_Base);
		
	}	

/*	uiTimer_1msINT++;         //== 1ms �жϼ�ʱ
    if(uiTimer_1msINT>=2)     //== 2ms �� 
    {
        uiTimer_1msINT=0;	
       
		read_MGZ310_Gyro(GYROX, & fgadata[1], & fgadata[0]);
		read_MGZ310_Gyro(GYROY, & fgadata[2], & fgyrotemp[0]);
		read_MGZ310_Gyro(GYROZ, & fgadata[3], & fgyrotemp[1]);
		read_MSA8000D_Acc(ACCX, & fgadata[4], & fgadata[7]);
		read_MSA8000D_Acc(ACCY, & fgadata[5], & faccytemp[0]);
	 	read_MSA8000D_Acc(ACCZ, & fgadata[6], & faccytemp[1]);
	}
*/
	GPS_Data_SetUp();
//	CSAC_Data_SetUp();

	CSR=CSR&0xfffffffe;
	CSR = old_csr ;			// Restore CSR (and thus PGIE) 
	IRP = old_irp ;			// Restore IRP
}

void interrupt ext_int05()
{
	unsigned int old_csr;
	unsigned int old_irp;
	// IRP CSR ����c6x.h�ļ���Ԥ����
	old_irp = IRP ;			// Save IRP 								
	old_csr = CSR ;			// Save CSR (and thus PGIE)
	CSR = old_csr | 1 ;		// Enable interrupts 
	
	CSR=CSR&0xfffffffe;
	CSR = old_csr ;			// Restore CSR (and thus PGIE) 
	IRP = old_irp ;			// Restore IRP
}

void interrupt ext_int06()
{
	unsigned int old_csr;
	unsigned int old_irp;
	// IRP CSR ����c6x.h�ļ���Ԥ����
	old_irp = IRP ;			// Save IRP 								
	old_csr = CSR ;			// Save CSR (and thus PGIE)
	CSR = old_csr | 1 ;		// Enable interrupts 
	
	CSR=CSR&0xfffffffe;
	CSR = old_csr ;			// Restore CSR (and thus PGIE) 
	IRP = old_irp ;			// Restore IRP
}

void interrupt ext_int07()
{
	unsigned int old_csr;
	unsigned int old_irp;
	// IRP CSR ����c6x.h�ļ���Ԥ����
	old_irp = IRP ;			// Save IRP 								
	old_csr = CSR ;			// Save CSR (and thus PGIE)
	CSR = old_csr | 1 ;		// Enable interrupts 
	
	CSR=CSR&0xfffffffe;
	CSR = old_csr ;			// Restore CSR (and thus PGIE) 
	IRP = old_irp ;			// Restore IRP
}
	


void GPS_Data_SetUp()
{
	int i = 0;
	unsigned char temp_char1 = 0x00;
	unsigned char temp_char2 = 0x00;
	unsigned short * GPS_FIFO_REG = 0;		//GPS����FIFOʣ���ֽڼĴ�����ַ
	unsigned short * MAIN_FIFO_REG = 0;		//MAIN����FIFOʣ���ֽڼĴ�����ַ
	GPS_FIFO_REG = ((unsigned short *)(UART1_Base + 0xA));
	MAIN_FIFO_REG = ((unsigned short *)(UART0_Base + 0xA));

	while((*GPS_FIFO_REG & 0x01) == 0x01)		// ���FIFO����,����FIFO
	{
		temp_char1  = *((unsigned short *)UART1_Base);	
		UART_Txmit(temp_char1,UART0_Base);
			
	}
	while((*MAIN_FIFO_REG & 0x01) == 0x01)		// ���FIFO����,����FIFO
	{
		temp_char2  = *((unsigned short *)UART0_Base);	
		UART_Txmit(temp_char2,UART1_Base);
			
	}
		
}

void CSAC_Data_SetUp()
{
	int i = 0;
	unsigned char temp_char1 = 0x00;
	unsigned char temp_char2 = 0x00;
	unsigned short * CSAC_FIFO_REG = 0;		//GPS����FIFOʣ���ֽڼĴ�����ַ
	unsigned short * MAIN_FIFO_REG = 0;		//MAIN����FIFOʣ���ֽڼĴ�����ַ
	CSAC_FIFO_REG = ((unsigned short *)(UART2_Base + 0xA));
	MAIN_FIFO_REG = ((unsigned short *)(UART0_Base + 0xA));

	while((*CSAC_FIFO_REG & 0x01) == 0x01)		// ���FIFO����,����FIFO
	{
		temp_char1  = *((unsigned short *)UART2_Base);	
		UART_Txmit(temp_char1,UART0_Base);
			
	}
	while((*MAIN_FIFO_REG & 0x01) == 0x01)		// ���FIFO����,����FIFO
	{
		temp_char2  = *((unsigned short *)UART0_Base);	
		UART_Txmit(temp_char2,UART2_Base);
			
	}

}
