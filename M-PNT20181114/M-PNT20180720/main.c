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
#include "MatrixLib.h"
#include "GPS_DVL.h"

void interrupt ext_int04(void);
void interrupt ext_int05(void);
void interrupt ext_int06(void);
void interrupt ext_int07(void);

unsigned int a,b,c;

//��������
void save_send_f32TD();
unsigned int c4touint(unsigned char cData_in[4]);
void pntowa(float fgadata[]);   

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
unsigned char  gpstemp = 0;


unsigned char g_ucflag_1ms=0;
unsigned char g_ucflag_5ms=0;
unsigned char g_ucCnt_1ms=0;
unsigned int  g_uiTimer1ms_IMU=0;   // ����1ms����
unsigned int  g_uiSumimuCnt=0;
unsigned char g_ucUart1[256];

float   g_dSumPn[9]  ={0,0,0,0,0,0};        //== �ɼ�ԭʼ�������ԭʼ��ѹ
float   g_dNavPn[9]  ={0,0,0,0,0,0};        //== 5ms����ֵ�����ڵ���  
float   g_fpnImu[12] ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; 

//IMUXiShuStruct        ptr_pnxscompen;
IMUDataStruct 	      IMUData;
//NavResultStruct       NavResult;
GPS_STR			      GpsDat;
//KF_Struct		      KF_Result;
NavToFControlStruct   DRamFConData;

//IMUXiShuStruct       *P0 = &ptr_pnxscompen;
IMUDataStruct 		 *P1 = &IMUData;		 // IMUԭʼ������
//NavResultStruct 	 *P2 = &NavResult;       // ��������
GPS_STR 			 *P3 = &GpsDat;          // GPS����
//KF_Struct		     *P4 = &KF_Result;	     // KF�˲�����
NavToFControlStruct  *P5 = &DRamFConData;    // ������������ɿ�DSP

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
	PECULIAR_WRITE(PECULIAR_Base,A_REG0,0x01) ;     //CSAC_PPS_IN����ѡ��,GPS����������
	TIMER_EN(INTR6_Base,TIMER_DIS_CMD);				// �ر�FPGA��ʱ�ж���� 
	
	UART_init(UART0_Base, 10, 3,1,1);				// ������115200 ������
	UART_init(UART1_Base, 10, 3,1,1);				// ������115200	UBLOX-NEO-M8T
	UART_init(UART2_Base, 20, 3,1,1);				// ������57600	ԭ���Ӵ���
	
	TIMER_CLKSet(360,TIMER0_Base);

  //	bint_enable(5);
	IER = 0xFFFF;									// ���ж�	
	
	TIMER_WriteL(INTR6_Base,0x4800);				// д��ʱ�жϷ�Ƶ�Ĵ�����16λ     0x4800= 18432  /1ms��ʱ�ж�
	TIMER_WriteH(INTR6_Base,0);						// д��ʱ�жϷ�Ƶ�Ĵ�����16λ
	TIMER_EN(INTR6_Base,TIMER_EN_CMD);				// ʹ��FPGA��ʱ�ж����

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
 	 if(200==cnt_1s)   // 20ms ��������
		{
		    cnt_1s=0;
		//	pntowa(fgadata);
		  //  save_send_f32TD();  
		  Read_Gps_WRJ(UART1_Base,P3);		    
		}	          
			
	}
}

void interrupt ext_int04()
{
	unsigned int old_csr;
	unsigned int old_irp;	
	static unsigned int	uiTimer_1msINT=0;           //== 1ms�жϼ�ʱ	
//	int i = 0;

	// IRP CSR ����c6x.h�ļ���Ԥ����
	old_irp = IRP ;			// Save IRP 								
	old_csr = CSR ;			// Save CSR (and thus PGIE)
	CSR = old_csr | 1 ;		// Enable interrupts			

	cnt_5ms++;
	
	// ʹ���жϷ�ʽ����������´���	
 	 	
	if(cnt_5s < NUM_5S)
	{
		flag_commu = commu_link(UART0_Base);
		cnt_5s++;
	}
	
	cnt_1s++;	
/*	if(cnt_1s >= NUM_1S)
	{
		cnt_1s = 0;
		UART_Txmit(0x55,UART0_Base);
		UART_Txmit(0xAA,UART0_Base);
		
	}	*/

	uiTimer_1msINT++;         //== 1ms �жϼ�ʱ
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
	


/***************************************************************/
/* �������ƣ� save_send_32TD() 
    �������ܣ�save nav data  33*4���ֽ�
    ��������?
    ���������*/ 
/***************************************************************/	    
void save_send_f32TD() 
{
	unsigned char i=0,j=0; 
	unsigned int Dcheck=0;
	unsigned char  head[4]={0x7e,0x7e,0x20,0x00}; 
	static unsigned int Framecount=0;
	unsigned short test[4];

	P5->head=c4touint(head);
	P5->T_SYS=Framecount++;   // ֡����
	P5->SC_STA=1;           // �Լ��̬
//	P5->NAV_STA=1;          // ����״̬
//	P5->GNSS_STA=1;         // ����״̬   
//	P5->GNSS_Week=P3->Tweek;
//	P5->GNSS_mm=  P3->Tweek_second;
//	P5->NAV_Long=( int )(P2->dPos[0]*dtor/180*0x7fffffff);
//	P5->NAV_Latt=( int )(P2->dPos[1]*dtor/180*0x7fffffff);
//	P5->NAV_High=(float)(P2->dPos[2]);
//	P5->NAV_VE  =(float)(P2->dVi[0]);
//	P5->NAV_VN  =(float)(P2->dVi[1]);
//	P5->NAV_VU  =(float)(P2->dVi[2]);
//	P5->NAV_Pitch=(float)(P2->dAtti[0])*dtor;
//	P5->NAV_Yaw = (float)(P2->dAtti[2])*dtor;
//	P5->NAV_Roll= (float)(P2->dAtti[1])*dtor;
	P5->AccX=(float)(P1->Acc[0]);
	P5->AccY=(float)(P1->Acc[1]);
	P5->AccZ=(float)(P1->Acc[2]);
	P5->Wx=(float)(P1->Gyro[0]);
	P5->Wy=(float)(P1->Gyro[1]);
	P5->Wz=(float)(P1->Gyro[2]);
//	P5->GNSS_Long=(int )(P3->Pos[1]*0x7fffffff/180);
//  P5->GNSS_Latt=(int )(P3->Pos[0]*0x7fffffff/180);
//	P5->GNSS_High=(float)(P3->Pos[2]);
//	P5->GNSS_VE=  (float)(P3->V[0]);
//	P5->GNSS_VN=  (float)(P3->V[1]);
//	P5->GNSS_VU=  (float)(P3->V[2]);
//	P5->GNSS_PDOP=(float)(P3->pdop);
//	P5->static_press=KF_Result.wKal_Num;   // ˫����GPS����
//	P5->static_temp=P3->DGPS_Course;
//	P5->dynam_press=P1->Gyro_temp[0];
//	P5->dynam_temp=P1->Gyro_temp[1];
//	P5->groy_temp=P1->Gyro_temp[2];
//	P5->imu_temp= P1->Gyro_temp[2];


    for(i=0;i<140;i++)
	{
	   Dcheck+=*((unsigned char *)P5+i);
	}

	P5->checksum=Dcheck & 0xffff;
    
	memset(g_ucUart1,0,256);

	for(i=0;i<144;i++)
	{
	   g_ucUart1[i]=*((unsigned char *)P5+i);
	}
	//
	// for(i = 0;i<144;i++)
   //	UART_Txmit(g_ucUart1[i],UART0_Base);
   pfUART_Trans(&g_ucUart1[0], 144, UART0_Base);
}



unsigned int c4touint(unsigned char cData_in[4])      //���ֽ�ת��Ϊ����
{
	unsigned int iData_out;
	memcpy(&iData_out,cData_in,4); 
	return iData_out;
}


/***************************************************************/
/* �������ƣ�
    �������ܣ� IMU copmsate  
    ���������	����ѹֵת��Ϊ���ٶȺͽ��ٶ�ֵ�� rad/s    m/S^2
    ���ֵ��  */
/***************************************************************/	     
void pntowa(float fgadata[])   		//���˻��ߵ���,
{

  	unsigned int 	 i=0;
//	FLOAT64  Aibbx=0, Aibby=0, Aibbz=0;
//	FLOAT64	 Wibbx=0, Wibby=0, Wibbz=0;
//	static FLOAT64  	bb[6]={0.0,0.0,0.0,0.0,0.0,0.0};

    IMUData.Acc[0]=fgadata[1] ;		    // m/s^2			 
	IMUData.Acc[1]=fgadata[2] ;         // m/s^2
	IMUData.Acc[2]=fgadata[3] ;          // m/s^2 
	IMUData.Gyro[0]=fgadata[4]  ;  // rad/s
	IMUData.Gyro[1]=fgadata[5]  ;  // rad/s
	IMUData.Gyro[2]=fgadata[6] ;  // rad/s

	//IMUData.Gyro_temp[0]=g_dNavPn[6];
//	IMUData.Gyro_temp[1]=g_dNavPn[7];
//	IMUData.Gyro_temp[2]=g_dNavPn[8];
	//fTemperature_acc=g_dNavPn[9];


}	