/* ------------------------------------------------------------------------ *
 *  main.c	TrailBlazer					                                	*
 *  编写时间：	2014.05.20.                                                 *
 *  更新时间：	2015.05.28.													*
 *	编写人：	chepengyu                                  					*
 *  更新说明：                                                              *
 *  	2015.05.28.
 *		1.更新为标准化模板                                                  *
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
// PC通讯相关
//------------------------------------------------------
unsigned char buff_cmd[SIZE_C_BUFF] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int flag_commu = 1;
//------------------------------------------------------
// 时间参数(对应1KHz定时中断)
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
// 系统信息(请勿修改)
//------------------------------------------------------
unsigned short info_fpga[4] = {0,0,0,0};
unsigned char info_Version[16] = {0,0,0,0,0x4,0x11,0x21,0xC4,0,0,0,0,0,0,0,0};

unsigned short frame_cnt = 0;	// FPGA硬件定时中断计数
unsigned char sframe_cnt = 0;	// 软件定时中断计数

int flag_format = 0;			// 输出数据格式标志
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
	OMAPL137_PLL_init(1,17); //主频为18.432MHz*（17+1）=331 MHz
	OMAPL137_PSC_init();
	OMAPL137_EMIFA_init();
	OMAPL137_GPIO_init();	
	OMAPL137_DSPINT_init();	
	FLASHSpiInit(); 
	_wait(1000); 
    
    SpiInit();    //SPI1初始化
    _wait(10);   
	TIMER_EN(INTR6_Base,TIMER_DIS_CMD);				// 关闭FPGA定时中断输出 
	
	UART_init(UART0_Base, 10, 3,1,1);				// 波特率115200 主串口
	UART_init(UART1_Base, 10, 3,1,1);				// 波特率115200	UBLOX-NEO-M8T
	UART_init(UART2_Base, 20, 3,1,1);				// 波特率57600	原子钟串口
	
	TIMER_CLKSet(360,TIMER0_Base);

  //	bint_enable(5);
	IER = 0xFFFF;									// 开中断	
	
	TIMER_WriteL(INTR6_Base,0x4800);				// 写定时中断分频寄存器低16位
	TIMER_WriteH(INTR6_Base,0x0);						// 写定时中断分频寄存器高16位
	TIMER_EN(INTR6_Base,TIMER_EN_CMD);				// 使能FPGA定时中断输出
	UART_FIFOCLR(UART0_Base);
	UART_FIFOCLR(UART1_Base);
	UART_FIFOCLR(UART2_Base);

   	while(1)
   	{   		
   		if(flag_commu == 0)	// 如果握手成功
   		{
   			IER = 0x0;					// 关中断	
//   			IRQ_disable(IRQ_EVT_EXTINT4);
   			UART_FIFOCLR(UART0_Base);	// 读空串口接收FIFO的数据
   			
   			// 进入与PC通讯模式   			
   			parse_command(buff_cmd,UART0_Base);
   		    
   			switch(buff_cmd[3])			// 判断指令代码	                   
			{                                                              
				case CODE_CMD_UP:		// 如果是上传指令   
					                                                                           
					Execute_UpLoad(buff_cmd[4],UART0_Base);		
						               
					break;                                                 
					                                                       
				case CODE_CMD_DRQ:		// 如果是数据请求指令    
					
					switch(buff_cmd[4])	// 判断数据类型
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
					
				case CODE_CMD_SET:							// 如果是设置指令
					
					switch(buff_cmd[4])						// 设置项目代码
					{
						case CODE_SET_FORMAT:				// 输出数据格式						
							
							flag_format = buff_cmd[5];		// 输出标定数据格式
							flag_commu = 1;					// 退出与PC通讯模式
							command_status(S_FOMATOK, UART0_Base);	// 返回设置成功状态指令
					//		IRQ_enable(IRQ_EVT_EXTINT4);	// 开中断
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
 		
 		// 以下添加用户代码
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
	static unsigned int	uiTimer_1msINT=0;           //== 1ms中断计时	
	int i =0;
	// IRP CSR 均在c6x.h文件中预定义
	old_irp = IRP ;			// Save IRP 								
	old_csr = CSR ;			// Save CSR (and thus PGIE)
	CSR = old_csr | 1 ;		// Enable interrupts			

	cnt_5ms++;
	
	// 使用中断方式握手请打开以下代码	
 	 	
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

/*	uiTimer_1msINT++;         //== 1ms 中断计时
    if(uiTimer_1msINT>=2)     //== 2ms 到 
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
	// IRP CSR 均在c6x.h文件中预定义
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
	// IRP CSR 均在c6x.h文件中预定义
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
	// IRP CSR 均在c6x.h文件中预定义
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
	unsigned short * GPS_FIFO_REG = 0;		//GPS串口FIFO剩余字节寄存器地址
	unsigned short * MAIN_FIFO_REG = 0;		//MAIN串口FIFO剩余字节寄存器地址
	GPS_FIFO_REG = ((unsigned short *)(UART1_Base + 0xA));
	MAIN_FIFO_REG = ((unsigned short *)(UART0_Base + 0xA));

	while((*GPS_FIFO_REG & 0x01) == 0x01)		// 如果FIFO不空,读空FIFO
	{
		temp_char1  = *((unsigned short *)UART1_Base);	
		UART_Txmit(temp_char1,UART0_Base);
			
	}
	while((*MAIN_FIFO_REG & 0x01) == 0x01)		// 如果FIFO不空,读空FIFO
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
	unsigned short * CSAC_FIFO_REG = 0;		//GPS串口FIFO剩余字节寄存器地址
	unsigned short * MAIN_FIFO_REG = 0;		//MAIN串口FIFO剩余字节寄存器地址
	CSAC_FIFO_REG = ((unsigned short *)(UART2_Base + 0xA));
	MAIN_FIFO_REG = ((unsigned short *)(UART0_Base + 0xA));

	while((*CSAC_FIFO_REG & 0x01) == 0x01)		// 如果FIFO不空,读空FIFO
	{
		temp_char1  = *((unsigned short *)UART2_Base);	
		UART_Txmit(temp_char1,UART0_Base);
			
	}
	while((*MAIN_FIFO_REG & 0x01) == 0x01)		// 如果FIFO不空,读空FIFO
	{
		temp_char2  = *((unsigned short *)UART0_Base);	
		UART_Txmit(temp_char2,UART2_Base);
			
	}

}
